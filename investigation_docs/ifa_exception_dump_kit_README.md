# IFA（incre_flash_attention + meta）Exception Dump Kit（CANN 公开接口版）

> **用途**：① **外部用户 Demo**——基于 CANN 公开接口为自有算子接入异常现场自动保存（Exception Dump）能力的参考样例；② **日常看护**——IFA 算子 Exception Dump 能力在新 CANN 版本上的例行回归用例。
> **实测基线**：CANN 9.1.T103（2026-09-07，Ascend 910C）
> **配套文档**：[test_plan.md](test_plan.md)（测试方案）｜[test_report.md](test_report.md)（测试结果）｜[incre_flash_attention_exception_dump_report.md](incre_flash_attention_exception_dump_report.md)（技术主报告）

---

## 1. 能力简介

算子发生 AICore error 时，Runtime 异常回调自动把该次执行的全部输入 tensor（含 meta 算子输出与内联 tiling）从设备内存保存为宿主机 bin 文件——"案发现场"完整留档，供离线复现与定位。

| 特性 | 说明 |
|---|---|
| 零侵入 | 引导链路不修改算子代码即可生效；原生三段式链路需算子侧加 stub 导出补丁（`patches/`） |
| 零开销 | 正常执行路径不经过任何 dump 逻辑，无注入基线实测零产物 |
| 可信 | dump 数据与原始输入 bit-exact 一致，注入的损坏值原样保留 |
| 公开接口 | 仅依赖 CANN 公开 API（`-lacl_rt -lascendcl` + 公开头文件），随版本升级稳定 |

两条注册链路（详见主报告 §3.1）：

| 链路 | 注册方式 | 适用场景 |
|---|---|---|
| 引导链路（默认） | `RegisterPublicExceptionDump(kernelName)`，全局回调首次异常时自动挂载 per-binary 回调 | 零算子改动，最简接入 |
| 原生三段式（推荐） | `RegisterBySymbol(stub)`，launch 前预挂载 | 需算子侧导出 kernel stub（模板 kernel 外部 dlsym 不到） |

**接入自己的算子**：`src/` 回调库与算子无关，可整体复用；接入步骤见主报告 §6（五步法），`demo/` 提供最小可运行样例。

---

## 2. 环境基线

| 项 | 基线（实测） |
|---|---|
| CANN | 9.1.T103 |
| 硬件 | Ascend 910C（910_9392，NPU 0 卡） |
| 容器/编译器 | docker `ops_dev_sw`，bisheng clang 15.0.5 + g++ |
| Python 栈 | Python 3.11.9 + torch 2.7.1 + torch_npu 2.7.1.post2 + ascend_ops 1.0.0 |
| 算子代码基线 | ops-transformer `ecfc11431` + 本包补丁（§4.2） |

新 CANN 版本看护时环境以新版本为准，本表用于结果比对与回退参照。

---

## 3. 目录结构

```
ifa_exception_dump_kit/
├── README.md                                        # 本手册（总入口）
├── test_plan.md                                     # 测试方案：用例设计、通过标准
├── test_report.md                                   # 测试结果：实测记录与证据
├── incre_flash_attention_exception_dump_report.md   # 技术主报告：机制原理、适配分析、证据链
├── src/                                             # 公开接口回调库（可复用核心）
│   ├── exception_dump_public.cpp                    #   源码（引导 + 原生三段式两链路）
│   └── exception_dump_public.so                     #   aarch64 + CANN 9.1.T103 编译产物（新环境需重编译）
├── test/                                            # IFA 实战调测用例（日常看护载体）
│   ├── test_public_dump.py                          #   引导链路用例（scene1/scene2/baseline/all）
│   ├── test_ifa_native3stage.py                     #   原生三段式用例（8 实例注册 + scene1）
│   ├── verify_public_dump.py                        #   dump 数据 bit-exact 验证
│   └── parse_ifa_tiling.py                          #   args 指针区 + tiling 离线解析
├── patches/                                         # IFA 算子侧补丁（重放到 ops-transformer ecfc11431 基线）
│   ├── ifa_kernel_symbol_export.patch               #   kernel stub 导出（原生三段式前置）
│   └── ops_transform_local.patch                    #   算子仓构建适配（一次性，环境未打过时使用）
└── demo/                                            # 外部用户最小 Demo（自包含，不依赖 IFA）
    ├── mini_kernel.cpp                              #   最小 AICore kernel + <<<>>> 直调 + stub 导出
    ├── libmini.so                                   #   dav-2201 编译产物
    └── demo_mini_symbol.py                          #   aclrtGetFuncBySymbol 支持性演示
```

---

## 4. 使用方法

以下命令均在容器内执行；`<本包>` 指本 kit 根目录，`<ops仓>` 指 ops-transformer 仓根。

### 4.1 环境准备（每次进入容器先执行）

```bash
source /usr/local/Ascend/ascend-toolkit/set_env.sh    # 必须：设置 ASCEND_HOME_PATH / 头文件 / 库路径
export NPU_SOC_VERSION=ascend910_93                   # 必须：构建体系按 <op>/<SOC>/ 匹配目录
# 核查：
cat $ASCEND_HOME_PATH/aarch64-linux/ascend_toolkit_install.info | grep version   # 基线 → 9.1.T103
npu-smi info | head -6                                                           # 设备在位
```

### 4.2 算子构建与安装（跑 IFA 用例前）

```bash
cd <ops仓>/examples/fast_kernel_launch_example
git apply <本包>/patches/ops_transform_local.patch        # 构建适配（未打过时）
git apply <本包>/patches/ifa_kernel_symbol_export.patch   # 可选：原生三段式用例（D8–D10）所需
export OPS_BUILD_FILTER=incre_flash_attention,incre_flash_attention_meta   # 白名单只编 IFA 两个算子
python3 setup.py clean
python3 -m build --wheel --no-isolation
pip install dist/ascend_ops-1.0.0-*.whl --force-reinstall --no-deps
# 导入核查：
python3 -c "import torch,ascend_ops; print(torch.ops.ascend_ops.npu_fused_infer_attention_score is not None)"
#   → True
```

说明：不打 symbol 导出补丁也能构建安装，仅原生三段式用例（D8–D10）无法运行，引导链路用例（D1–D7）不受影响。

### 4.3 Exception Dump 看护用例（D 系列）

**第 1 步：编译回调 so（新环境必须重做，不能用包内旧 so）**

```bash
cd <本包>/src
g++ -g -std=c++11 -fPIC -shared exception_dump_public.cpp -o exception_dump_public.so \
    -lacl_rt -lascendcl -L$ASCEND_HOME_PATH/lib64 -I$ASCEND_HOME_PATH/include
nm -D exception_dump_public.so | grep -E ' T '
#   预期导出：AddInputInfo / ClearInputInfo / RegisterPublicExceptionDump / RegisterBySymbol
```

**第 2 步：执行用例（建议在 `test/` 内执行；脚本会自动切换 CWD 至脚本目录，产物固定落盘 `test/task_fail_dump/`）**

```bash
cd <本包>/test
rm -rf task_fail_dump
python3 test_public_dump.py scene1     # D1/D2：块表损坏注入 → 全局回调 dump + 三段式挂载
python3 verify_public_dump.py          # D5：7/7 bit-exact
python3 parse_ifa_tiling.py            # D6：tiling 12/12 字段一致
python3 test_public_dump.py all        # D3：单进程 scene1 → scene2（第二次异常由 per-binary
                                        #     回调接管）；末段 baseline 报 507057 属预期
python3 test_public_dump.py baseline   # D4：无注入对照 → 零产物、零误触发（独立进程判定）
python3 test_ifa_native3stage.py       # D8/D9：原生三段式 8/8 注册 + scene1 dump（需 §4.2 symbol 补丁）
```

**第 3 步：交叉验证（D7、D10）**

```bash
md5sum task_fail_dump/dev*/query.bin
#   预期：各 dump 目录 query.bin md5 一致，基线值 6789400e0fadc7fa271b0b8ed8f29e0d
```

### 4.4 外部用户最小 Demo（不依赖 IFA）

`demo/` 演示"自建最小 AICore kernel + `<<<>>>` 直调 + `aclrtGetFuncBySymbol` 三段式注册"完整链路，可作为接入自己算子前的练手样例：

```bash
cd <本包>/demo
python3 demo_mini_symbol.py
#   预期：stub: mini_dump_kernel → [HIT] funcHandle（launch 前/后均命中）；
#         TU 内导出地址与 dlsym stub 一致；内部函数/名字字符串 → [MISS]（对照）；
#         mini_launch 输出 [41, 100]；funcHandle → binHandle 三段式全通
```

kernel 编译方法见 `mini_kernel.cpp` 头部注释（bisheng `-xasc --npu-arch=dav-2201`；包内 libmini.so 为实测环境编译产物，重编后 `demo_mini_symbol.py` 的 NM_OFF 偏移须按 nm 输出更新）。

---

## 5. 预期结果基线（实测，新版本对照用）

| 用例 | 预期 |
|---|---|
| D1 注册 | `RegisterPublicExceptionDump ret=0`；全局回调注册成功 |
| D2 scene1 | 回调触发：`[ExcDumpPub] (global-cb) Task fail detected: ... retcode=0x7bc87`；上报 kernel 为 mangled 名并匹配；三段式挂载成功（funcHandle → binHandle → BinarySetExceptionCallback OK）；`Saved 8/8 inputs` |
| D3 scene2 | `all` 模式下同进程第二次异常由 per-binary 回调（binary-cb）接管并完成 dump |
| D4 baseline | kernel 正常结束、零 dump 产物、回调零触发 |
| D5 数据验证 | `7/7 tensor bit-exact 一致（含注入的损坏值）`；metadata `usedCoreNum=18` |
| D6 tiling | args[0..31] 指针区非空槽与布局表一致；BaseParams `12/12 字段一致`（含 `seqSize=8320`、`scaleValue=1/√128`） |
| D7 md5 交叉 | `query.bin` md5 = `6789400e0fadc7fa271b0b8ed8f29e0d`（两链路产物一致） |
| D8 三段式注册 | 8 个模板实例 `ifa_get_kernel_symbol` → `RegisterBySymbol` 全部 HIT（launch 前挂载） |
| D9 三段式 dump | scene1 注入后 binary-cb dump 8/8，全程无全局回调参与 |
| D10 三段式验证 | verify_public_dump.py 7/7 bit-exact，md5 与 D7 一致 |

**异常码层次**：设备侧回调 `retcode=0x7bc87`（AICore 异常；公开访问器族 DeviceId/TaskId/StreamId/ErrorCode/Args/FuncHandle/FunctionName 不含异常类型字段，`expandType` 为旧内部接口字段）与 Python 侧 `error code 507057` 是不同层的错误表示，不做数值换算；异常码可能随 CANN 版本变化，**以回调触发（kernel 名匹配）与 dump 是否落盘为判定标准**。

**8/8 输入布局表**（ptrIndex 0-based，来自 `LAUNCH_INCRE_FA` 宏形参序）：

| 输入 | ptrIndex | 输入 | ptrIndex |
|---|---|---|---|
| query | 0 | block_table | 14 |
| key_cache | 1 | dequant_scale_key | 17 |
| value_cache | 2 | dequant_scale_value | 19 |
| actual_seq_kvlen | 6 | metadata（meta 算子输出） | 28 |

tiling 内联于 args 区 offset 256 起（`IncreFlashAttentionBaseParams`，每字段 4 B）。

---

## 6. 执行注意事项（自动化判定前必读）

1. **退出码语义**：注入场景中 kernel 异常被捕获后脚本以 `sys.exit(2)` 结束——这是**预期成功路径**；baseline 正常结束退出码 0。自动化看护以日志与 dump 产物判定，勿按 exit code 判失败。
2. **verify 的比对基准固化 scene1**：`verify_public_dump.py` 的注入补丁只含 `block_table[0,0]=0x7FFFFFFF`，且默认验证最新 `dev*` 目录。验证 scene2 产物需显式传目录参数并人工核对 `actual_seq_kvlen[0]=0x7FFFFFFF`。
3. **多场景分进程执行（D3 除外）**：同进程 AICore error 后设备流状态未恢复，紧接的后续 launch 会报 507057（`all` 模式下 baseline 段的异常属此现象，非回调误触发）。scene1/baseline 建议独立进程执行；D3（per-binary 接管验证）例外，必须用 `all` 模式同进程完成，其末段 baseline 的 507057 不作为 D4 判定依据。
4. **双回调各 dump 一次**：引导链路挂载完成后，同一次异常 global-cb 与 binary-cb 可能各 dump 一次（内容一致，以时间戳目录区分），属预期现象。
5. **产物目录以脚本目录为基准**：复跑前 `rm -rf task_fail_dump`；baseline 的"零产物"判定标准是**本轮无新增 `dev*_ts*` 目录**（对照时间戳），而非目录为空。
6. **`extra-info/` 目录（CANN 崩溃日志）**：AICore error 时 CANN 在 CWD 自动生成，属正常副产物而非 dump 交付物；docker 内 root 生成，宿主机删除可能报权限不够，需 `docker exec` 处理。
7. **so 与环境绑定**：包内 so 为 aarch64 + CANN 9.1.T103 编译产物，新环境必须按 §4.3 第 1 步重编译；源码注释/提示文案的后续修订未重编时，行为以源码为准。
8. **全局回调为进程级**：`aclrtSetExceptionInfoCallback` 若宿主框架（如 torch_npu）也注册会互相覆盖——集成到官方框架时优先原生三段式链路（per-binary）。

---

## 7. CANN 版本升级复查清单

| # | 复查项 | 原因 | 动作 |
|---|---|---|---|
| 1 | **回调 so 必须重编译** | so 链接旧版本 CANN 库，ABI 可能不兼容 | 按 §4.3 第 1 步重编译后再跑用例 |
| 2 | **args 布局表 / tiling 偏移** | launch 宏形参序或 `BaseParams` 字段序变化会导致 ptrIndex/tiling 解析整体错位 | 跑 scene1 后对照脚本打印的 `data_ptr()` 与 `parse_ifa_tiling.py` 的 args[0..31]；不一致则按主报告 §4.3 重新校准 |
| 3 | **kernel mangled 名匹配** | 算子改名/模板实参变化可能导致失配 | 日志 `Kernel name mismatch (got=..., expected=...)` 直接可判；失配时改脚本内 `KERNEL_NAME` |
| 4 | **公开接口可用性** | 三段式 API 仅较新 CANN 提供（实测 8.0.0 的头文件/库无 `aclrtGetFuncBySymbol`/`aclrtFunctionGetBinary`/`aclrtBinarySetExceptionCallback`/`aclrtGetArgsFromExceptionInfo`；`aclrtSetExceptionInfoCallback` 已存在） | 编译报未定义符号时确认 CANN 版本 ≥ 实测基线（9.1），或向 CANN 侧确认接口存在性 |
| 5 | **守卫宏可否移除** | 新 CANN 若已含 SK / aclprof 新 API，`ops_transform_local.patch` 的守卫宏成为冗余 | 尝试去掉 `IFA_ENABLE_SK`/`IFA_ENABLE_PROF` 宏做全量构建，补跑官方 ACLGraph 用例 |
| 6 | **异常触发行为本身** | 故障注入依赖 PA 越界被硬件捕获；新版本若校验前置化，scene1 可能不再触发 AICore error | 属预期行为变化，记录即可；能力有效性以"异常发生 → dump 完整"链路为准 |
| 7 | **异常码值** | 0x7bc87 / 507057 为实测值，版本间可能不同 | 以回调触发（kernel 名匹配）与 dump 落盘为判定标准，不比对异常码数值 |
