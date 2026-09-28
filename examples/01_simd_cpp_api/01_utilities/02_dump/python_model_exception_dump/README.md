# <<<>>>算子 Exception Dump 外挂 Python Model 样例

## 概述

算子在 NPU 上发生 AICore 任务失败（如访存越界触发硬件 trap）时，默认行为是任务失败上抛、进程退出，设备内存中"这次执行到底喂了什么数据"的现场随之丢失，难以离线复现与定位。本样例演示 **Runtime 层外挂式 Exception Dump 能力**：不修改算子代码，仅通过 Python API 使能异常回调；算子发生 AICore 异常后，回调自动把该次执行的 kernel args 区与全部登记输入从设备内存抢救到宿主机落盘，形成可离线分析的异常现场。

被测算子为 ops-transformer 开源仓中的 `incre_flash_attention`（IFA）及其 `incre_flash_attention_meta` 前置算子（`<<<>>>` 直调的真实复杂算子），**算子代码零改动**。

| 特性 | 说明 |
|------|------|
| 零侵入 | 不修改算子代码即可生效；异常回调经 `aclrtExceptionInfoCallbackRegister` 注册，支持多回调共存，与宿主框架互不覆盖 |
| 零开销 | 正常执行路径不经过任何 dump 逻辑，无注入基线实测零产物、回调零触发 |
| 可信 | dump 数据与构造侧输入 bit-exact 一致，注入的损坏值原样保留 |
| 公开接口 | 仅依赖 CANN 公开 API（`-lacl_rt -lascendcl` + 公开头文件），随版本升级稳定 |

注册链路（唯一链路）：Python 调用 `enable_exception_dump(kernel_name)`，回调库经 `aclrtExceptionInfoCallbackRegister` 注册异常回调（旧接口 `aclrtSetExceptionInfoCallback` 已废弃）；首次匹配到目标 kernel 异常时，回调内自动取得 function handle 与 binary handle 并挂载 per-binary 回调，当前异常与后续异常均可保存现场。

```
enable_exception_dump(kernel_name)
  -> aclrtExceptionInfoCallbackRegister
  -> 首次异常时取得 funcHandle 和 binHandle
  -> aclrtBinarySetExceptionCallback（自动挂载 per-binary 回调）
  -> dump
```

**与 kernel 内 `asc_dump_*` 的区别**：`asc_dump_*` 系列（见 simple_dump 等样例）是写在核函数里的数据打印接口，用于正常执行路径的可视化；本样例是 Runtime 层异常现场自动保存能力，kernel 内不增加任何插桩，两者定位与使用场景不同、互不替代。

## 本样例支持的产品及CANN软件版本

| 产品 | CANN软件版本 |
|------|-------------|
| Ascend 910B4 | >= CANN 9.2.0（实测基线） |

异常回调注册依赖公开接口 `aclrtExceptionInfoCallbackRegister` / `aclrtExceptionInfoCallbackUnregister`（旧 CANN 仅有已废弃的 `aclrtSetExceptionInfoCallback`，不支持本样例）；其他产品与 CANN 组合需先经 `scripts/setup.sh check` 确认接口存在性并重新回归。

端到端用例仅支持 **NPU 实机**运行（CPU / sim 仿真模式不支持）；异常范围覆盖 AICore 任务失败（链路不区分异常类型），AICPU 算子异常、CPU_DEBUG、图模式统一异常采集不在覆盖范围内。

## 目录结构介绍

```
├── python_model_exception_dump
│   ├── runtime                                 // 可复用的通用外挂核心（C ABI 回调库）
│   │   ├── exception_dump_public.cpp           //   回调库源码（与算子无关，可整体复用）
│   │   └── exception_dump_public.h             //   样例自有 C ABI 头文件（3 个接口声明）
│   ├── python                                  // Python 层
│   │   ├── op_exception_dump.py                //   Python API 封装（对用户的主接口）
│   │   ├── ifa_exception_dump.py               //   IFA model 本体（输入构造 + 场景执行）
│   │   ├── verify_dump.py                      //   dump 数据 bit-exact 离线验证
│   │   └── parse_tiling.py                     //   args 指针区 + tiling 离线解析
│   ├── test                                    // IFA 真实算子看护用例
│   │   ├── test_public_dump.py                 //   用例入口（scene1/scene2/baseline/all）
│   │   └── patches                             //   构建适配补丁（与 dump 能力无关）
│   ├── scripts
│   │   ├── setup.sh                            // 环境检查、回调库编译、外部 IFA wheel 构建
│   │   └── run.sh                              // 场景执行、完整回归、产物清理
│   ├── CMakeLists.txt                          // 编译工程文件（回调库）
│   ├── README.md                               // 样例说明文档
│   └── README_en.md                            // 样例说明文档（英文）
```

环境基线（实测）：Ascend 910B4 + CANN 9.2.0；Python 3.11 + torch + torch_npu + ascend_ops wheel（由 `scripts/setup.sh ifa` 从 ops-transformer 固定 commit 构建，基线 `ecfc11431`）。仅静态检查（语法 / 回调库编译）不依赖 NPU。

## 使用方法

```bash
# 1. 环境准备（每次进入环境先执行）
source /usr/local/Ascend/ascend-toolkit/set_env.sh       # 设置 ASCEND_HOME_PATH / 头文件 / 库路径
export NPU_SOC_VERSION=ascend910_93                      # 构建体系按 <op>/<SOC>/ 匹配目录
./scripts/setup.sh check                                 # 新回调接口、NPU、Python 栈

# 2. 编译回调库（新环境 / CANN 升级后必须重做；产物 .so 不入仓）
./scripts/setup.sh callback
#   预期：nm -D 恰好导出 AddInputInfo / ClearInputInfo / RegisterPublicExceptionDump

# 3. 构建安装外部 IFA wheel（跑端到端用例前；算子零改动）
export OPS_TRANSFORMER_ROOT=<ops-transformer 仓根目录>
./scripts/setup.sh ifa

# 4. 执行用例（在 test/ 产物目录落盘；run.sh 验证注入场景的退出码 2 后返回 0）
./scripts/run.sh scene1        # 块表损坏注入 -> 异常回调 dump
./scripts/run.sh full          # 完整回归：scene1 + verify + parse + all + baseline
./scripts/run.sh clean         # 清理 dump 和 CANN 崩溃日志
```

离线验证也可单独执行：`python3 python/verify_dump.py`（dump 数据 bit-exact 校验）、`python3 python/parse_tiling.py`（args 指针区与 tiling 解析）。IFA 用例的 8 输入 args 槽位表（ptrIndex）见 `python/ifa_exception_dump.py` 的 `INPUT_PTR_INDEX`，tiling 内联于 args 区 offset 256 起。

## 新算子接入指南

任意通过 `<<<>>>` 下发的 AICore kernel 算子均可零改动接入本能力。接入涉及三件事：算子侧（无需修改）、Python 侧调用、外挂库执行逻辑。`runtime/` 回调库与算子无关，可整体复用。

### 算子怎么写（无需修改）

算子侧**不需要任何代码改动**，也不需要重新编译算子。原因：异常回调注册、kernel 名过滤、args 区读取全部发生在 Runtime 层，与算子实现无关；`<<<>>>` 模板 kernel 上报的是 Itanium mangled 名（如 `_Z21incre_flash_attentionILh0ELh1ELh0EE...`），回调库内置子串匹配，注册时直接传函数名本体即可命中。

接入前需要确认两点前提：

- 被测目标是通过 `<<<>>>`（或等价 Runtime 下发路径）启动的 **AICore kernel**；AICPU kernel 不在异常回调覆盖范围内；
- kernel 执行失败后其 args 区与输入 tensor 的设备内存不会被立即回收——这是一般情况（异常使 kernel 内部计算出错，不回收已分配内存），回调因此能在进程退出前完成现场抢救。

接入的唯一功课是 **args 布局表校准**：回调按"args 区第 N 个 8 字节槽"取各输入的 device 地址，槽位序必须严格按目标 kernel launch 形参顺序确定（从算子 host 侧 launch 代码抄录形参序；可选输入为 null 时槽位占位但不跳过）。校准方法是实证而非推断：首次运行后对照回调打印的 `probe args[N] = 0x...` 探测日志与 Python 侧各 tensor 的 `data_ptr()`，逐槽吻合即布局表正确，bit-exact 验证构成第三重确认。

### Python 如何调用

Python 侧三步（`python/op_exception_dump.py` 提供封装，内部经 ctypes 加载回调库，无需头文件）：

```python
from op_exception_dump import enable_exception_dump, register_input, clear_inputs

# 1. 使能 + 注册算子名（传 kernel 函数名本体，mangled 名子串匹配自动兼容模板 kernel）
enable_exception_dump("your_kernel_name")

# 2. 登记输入 handle：name + tensor + ptrIndex（args 槽位，按 launch 形参序校准）
#    size/shape/dtype 从 tensor 自动读取；每轮调用前先清理防残留
clear_inputs()
register_input("input_a", tensor_a, ptr_index=0)
register_input("input_b", tensor_b, ptr_index=1)

# 3. meta 类运行时生成的输入（如前置算子输出），在其生成之后补登记
metadata = torch.ops.xxx.your_meta_op(...)
register_input("metadata", metadata, ptr_index=8)

# 之后正常调用算子；异常发生时回调自动落盘 task_fail_dump/dev*_ts*/
```

注意事项：kernel 名等参数按 bytes 传递的细节已由封装层处理；回调库路径默认解析为 `<样例>/runtime/exception_dump_public.so`，可用 `lib_path` 参数或 `EXC_DUMP_LIB` 环境变量覆盖；登记的 size/shape/dtype 必须来自本次调用，不能依赖固定设备地址。

### 外挂库执行逻辑（外挂库如何写）

回调库（`runtime/exception_dump_public.cpp`）是纯外挂实现，核心流程：

1. **注册**：`RegisterPublicExceptionDump(kernelName)` 记录过滤名，经 `aclrtExceptionInfoCallbackRegister` 注册全局异常回调（该接口支持按函数指针标识的多回调共存，与宿主框架注册的回调互不覆盖；配套 `aclrtExceptionInfoCallbackUnregister` 注销）。
2. **异常触发**：kernel 任务失败时 Runtime 调起回调。回调先用访问器族提取 `deviceId / taskId / streamId / errorCode`，经 `aclrtGetFuncHandleFromExceptionInfo` + `aclrtGetFunctionName` 取得上报 kernel 名并做子串匹配过滤，不匹配则跳过。
3. **自动挂载 per-binary**：首次匹配时经 `aclrtFunctionGetBinary` 取得 binary handle 并 `aclrtBinarySetExceptionCallback` 挂载 per-binary 回调（只挂一次）；后续同 kernel 异常直走 per-binary 链路。
4. **现场抢救**：经 `aclrtGetArgsFromExceptionInfo` 取得 args 区位置与长度，`aclrtSetDevice` 建立设备上下文后 `aclrtMemcpy`（D2H）复制到 host；按登记的 ptrIndex 从 args 区解析各输入的 device 指针，逐个 D2H 复制落盘到 `task_fail_dump/dev{d}_task{t}_ts{ms}/`（`<name>.bin` × N + `args_raw.bin` + `inputs_info.txt`），完成后释放缓冲并 `aclrtResetDevice`。
5. **并发与时序约束**：输入登记与 dump 状态由互斥锁串行保护；dump 必须在回调内**同步**完成（进程随异常退出，异步写盘会丢现场）；回调运行在 Runtime 线程，Python 层回调不可用（无 GIL），因此回调库必须以 C/C++ 实现、Python 仅负责注册与登记。

## 执行注意事项（自动化判定前必读）

1. **退出码语义**：注入场景中 kernel 异常被捕获后 `test_public_dump.py` 以退出码 2 结束；`run.sh` 仅将此码判为成功并自身返回 0，未触发异常的退出码 0 会判失败。baseline 正常结束退出码 0。端到端结果还需结合回调日志与 dump 产物判定。
2. **verify 的比对基准固化 scene1**：`verify_dump.py` 的注入回填只含 `block_table[0,0]=0x7FFFFFFF`，且默认验证最新 `dev*` 目录。验证 scene2 产物需显式传目录参数并人工核对 `actual_seq_kvlen[0]=0x7FFFFFFF`。
3. **多场景分进程执行**：同进程 AICore error 后设备流状态未恢复，紧接的后续 launch 会在主机侧失败（实测现象有 507057、ERR00100 等，随 CANN 版本而异；`all` 模式下后续场景段的异常属此现象，非回调误触发）。scene1/baseline 建议独立进程执行；验证 per-binary 接管时例外，用 `all` 模式同进程完成，其判定以"双回调双写 dump"为 per-binary 生效判据，末段 baseline 的主机侧错误不作为 baseline 场景的判定依据。
4. **双回调各 dump 一次**：per-binary 链路自动挂载完成后，同一次异常 global-cb 与 binary-cb 可能各 dump 一次（内容一致，以时间戳目录区分），属预期现象。
5. **产物目录以脚本目录为基准**：复跑前执行 `./scripts/run.sh clean`；baseline 的"零产物"判定标准是**本轮无新增 `dev*_ts*` 目录**（对照时间戳），而非目录为空。
6. **`extra-info/` 目录（CANN 崩溃日志）**：AICore error 时 CANN 在 CWD 自动生成，属正常副产物而非 dump 交付物；docker 内 root 生成时，宿主机删除可能报权限不够，需在容器内处理。
7. **so 与环境绑定**：`runtime/` 下的 `.so` 为编译产物不入仓，新环境或 CANN 升级后必须重新执行 `scripts/setup.sh callback`。
8. **异常码分层**：设备侧回调 `retcode=0x7bc87` 与 Python 侧 `error code 507057` 是不同层的错误表示，不做数值换算；异常码可能随 CANN 版本变化，**以回调触发（kernel 名匹配）与 dump 是否落盘为判定标准**。

## CANN 版本升级复查清单

| # | 复查项 | 原因 | 动作 |
|---|--------|------|------|
| 1 | **回调 so 必须重编译** | so 链接旧版本 CANN 库，ABI 可能不兼容 | 执行 `scripts/setup.sh callback` 后再跑用例 |
| 2 | **args 布局表 / tiling 偏移** | launch 形参序或 `BaseParams` 字段序变化会导致 ptrIndex/tiling 解析整体错位 | 跑 scene1 后对照回调探测日志 `probe args[N]` 与脚本打印的 `data_ptr()`；不一致则重新校准布局表 |
| 3 | **kernel mangled 名匹配** | 算子改名/模板实参变化可能导致失配 | 日志 `Kernel name mismatch (got=..., expected=...)` 直接可判；失配时改注册的 kernel 名 |
| 4 | **回调注册接口可用性** | 样例依赖 `aclrtExceptionInfoCallbackRegister`/`Unregister`；旧版本仅有已废弃的 `aclrtSetExceptionInfoCallback` | `scripts/setup.sh check` 会自动检查头文件；编译/注册报未定义符号时向 CANN 侧确认接口存在性 |
| 5 | **异常触发行为本身** | 故障注入依赖越界访问被硬件捕获；新版本若校验前置化，scene1 可能不再触发 AICore error | 属预期行为变化，记录即可；能力有效性以"异常发生 -> dump 完整"链路为准 |
| 6 | **异常码值** | 0x7bc87 / 507057 为实测值，版本间可能不同 | 以回调触发（kernel 名匹配）与 dump 落盘为判定标准，不比对异常码数值 |
