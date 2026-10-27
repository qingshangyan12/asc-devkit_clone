# SIMD VF GM printf 验证样例

从 `gm_sample` 迁移，用于验证当前仓库的仅地址 GM 字符串打印接口。构建默认使用本样例所在仓库的头文件。

## 验证范围

本样例验证新接口 `__asc_simd_vf::printf_gm(__asc_simd_vf::GmCStringRef, ...)`：调用者只传 GM 地址，由 AICore 有界扫描到 NUL 后计长、组包。**编译器自动将 VF 字面量迁移到 GM 尚未实现**，因此由 AICore 手动准备引用，模拟编译器将来的输出。

- AICore 用 `__asc_simd_vf::make_gm_cstring_ref` 为 GM 字面量构造仅地址引用，不提供长度。
- 用户 UB 句柄表放七个 8B GM 引用，使用 64B 分配中的 56B；VF 通过 `__asc_simd_vf::load_gm_cstring_ref` 读取句柄。
- VF 通过 `__asc_simd_vf::printf_gm` 产生引用记录；AICore 在 FIFO 容量预算内扫描格式串和 GM `%s` 至 NUL，再构造标准 TLV。
- 长字符串由 Host 生成，再通过 `aclrtMalloc` 和 `aclrtMemcpy` 放进设备 GM，流同步完成后才释放。kernel 只接收 GM 指针，不再接收 `hostTextBytes`；Host 分配和上传所需的大小仍保留，不属于打印接口参数。

六项输出检查：

1. int / uint / float / GM `%s`（hello）。
2. 十六进制与指针。
3. 原例中的纯字符串。
4. UB tensor 加法结果：1.25 + 10 = 11.25。
5. GM 长字符串：`GM_BEGIN:` + 2300 个 `G` + `:GM_END`，自动逐字符校验。
6. VF 内直接调用 `printf("[simd_vf] string in gm!\n")`，不经过句柄包装；它在同一次 VF 调用中先执行，再执行上述五项 GM 输出，覆盖普通记录与 GM 引用记录共用发布逻辑。

虽然文本含 `in gm`，直接字面量当前走 `printf(__ubuf__ const char*, ...)` 兼容入口。这一项验证直接写法可用，不证明编译器已自动把它转换为 GM 引用。

第五项字符串正文超过 2024B 调测 UB 数据区，验证 GM 正文不需要先完整装入该 UB。GM FIFO 配置为每核 32768B；并不表示支持任意长度字符串。

## 编译运行

```bash
source /usr/local/Ascend/cann/set_env.sh
# 在仓库根目录执行
cd examples/01_simd_cpp_api/01_utilities/00_printf/simd_vf_printf_gm

# Ascend950PR_9599 模拟器
bash run.sh sim

# 配套 NPU 环境
bash run.sh npu
```

`run.sh` 编译、运行并检查输出，任一阶段失败均返回非零；运行日志为 `run.log`，默认超时 120 秒。

可覆盖：`ASC_DEVKIT_SOURCE`（待测仓库）、`SIMULATOR_LIB`（模拟器 lib 目录）、`RUN_TIMEOUT`（秒）、`BUILD_JOBS`。

CMake 使用 `-iquote` 优先选择待测仓库中的头文件。bisheng 会提前添加安装包的 `-I` 路径，仅 `target_include_directories(BEFORE)` 不足以保证选择工作区实现。不修改已安装的 CANN 文件。

## 本次验证

2026-09-30：CANN 9.3.0 / dav-3510 编译成功；按 `run.sh sim` 配置 Ascend950PR_9599 模拟器路径运行成功，自动校验输出：

```text
PASS: direct literal + 5 address-only GM printf cases, including all 2300 payload characters
```

沙箱内曾在 `aclInit` 返回 500000，沙箱外重试通过。本次不是物理 NPU 验证，也不是 CPU `std::printf` fallback 测试。

文件说明：`simd_vf_printf.asc` 为样例；`acl.json` 配置 FIFO；`check_output.py` 校验六项输出；`CMakeLists.txt` 和 `run.sh` 负责构建运行。

所有 GM 字符串引用类型、构造/加载函数及打印入口均使用 `__asc_simd_vf::` 命名空间。
`make_gm_cstring_ref` / `make_gm_debug_string` 仍标记为 `__aicore__`，需要在 AICore 侧调用；命名空间不会改变执行域。
