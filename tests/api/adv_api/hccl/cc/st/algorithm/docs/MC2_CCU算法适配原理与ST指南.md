# Devkit 新通信算法 ST 接入与修改指南

> 本文回答“新增或修改通信算法时，需要改哪些 ST 文件、补哪些 case、怎样覆盖功能边界”。HCCL 原仓 ST 机制见 [HCCL_ST整体原理与迁移适配指南.md](HCCL_ST整体原理与迁移适配指南.md)，Devkit ST 总体架构和演进方案见 [Devkit_HCCL_ST整体设计与演进方案.md](Devkit_HCCL_ST整体设计与演进方案.md)。

## 1. 先确定改动属于哪一类

在改 ST 前先回答：

1. 是 AICPU 还是 CCU 数据面？
2. 是普通 HCCL Host 启动还是 MC2/KFC 启动？
3. 改的是算法次序、原语、TaskArg/LoadArg、peer 表、KFC 协议，还是 Host 资源？
4. 是否引入新的并发实体：从流、Mission、LoopGroup、LoopEngine？

不同答案决定适配层和 Checker 所属目录。不要按“这个文件能链接”决定代码位置。

## 2. AICPU 算法接入

### 2.1 只新增已有算法场景

通常修改：

- `testcase/<collective>_testcase.cpp`：拓扑、Rank 数、count、in-place、异常参数；
- 必要时 `semantics_check/<collective>_semantics_checker.*`：新增结果布局规则；
- testcase CMake：确保用例源进入目标。

### 2.2 新增 collective，但只使用已有 HCOMM 原语

需要：

- 将真实 selector/executor/template 源加入相应静态库或测试目标；
- 在 selector/topology stub 中补最小且可观测的环境行为；
- 新增 collective semantics checker；
- 增加覆盖算法选择、Rank 数据流和边界参数的 testcase。

通常不需要 `utils/src/aicpu/<collective>_adapter`。通用 adapter 已在 `hccl_proxy/hccl_stub.cc` 的 HCOMM 原语边界。

### 2.3 新增 HCOMM 原语

需要同时修改：

- `hccl_depends_stub` 或原语声明：捕获调用；
- `hccl_proxy`/`task_def.h`：新增任务表示；
- `sim_world/sim_task*`：新增对应 `TaskStub` 数据结构；
- `task_graph_generator`：依赖边；
- `singletask_check`：单任务合法性；
- `mem_conflict_check`：读写集合；
- `TaskCheckOpSemantics`：符号执行和数据来源传播；
- collective semantics checker；
- 原语级正反例和算法级正例。

## 3. 新增 MC2 CCU collective：按函数落地

下面用 `Xxx` 代表要接入的新 collective，例如 ReduceScatter。函数名不是强制命名规范，但职责和数据流应保持一致。当前 AllToAllV 可以直接作为参考实现。

### 3.1 最终应新增哪些文件

如果新算法同时支持普通 CCU kernel 和 MC2/KFC，最小文件集合是：

```text
utils/src/ccu/
  ccu_xxx_adapter.h
  ccu_xxx_adapter.cc

utils/src/hccl_verifier/semantics_check/
  ccu_xxx_semantics_checker.h
  ccu_xxx_semantics_checker.cc
  ccu_xxx_mc2_checker.h
  ccu_xxx_mc2_checker.cc

testcase/mc2_ccu/
  host/xxx_mc2_host_testcase.cpp
  device/xxx_kernel_device_testcase.cpp
  device/xxx_kfc_device_testcase.cpp
```

还要在 `testcase/mc2_ccu/CMakeLists.txt` 和 `utils/src/hccl_verifier/semantics_check/CMakeLists.txt` 中加入这些源文件。只有公共模型缺能力时，才修改 `hccl_proxy`、`sim_world` 或公共 Checker。

### 3.2 写代码前先做一张 ABI 表

不要一边写 adapter 一边猜参数顺序。先从产品 kernel、Host tiling 和 KFC message 中整理一张表：

| argId | 产品 `LoadArg` 变量 | 普通 Launch 来源 | KFC message/startup 来源 | 含义 |
|---:|---|---|---|---|
| 0 | `input` | `config.inputAddress` | `message[1]` | 输入基址 |
| 1 | `output` | `config.outputAddress` | `message[2]` | 输出基址 |
| ... | ... | ... | ... | 新算法字段 |

如果算法还有 peer 参数表，再单独写一张表：

| byte offset | 字段 | Host 写入者 | kernel 读取方式 | Checker |
|---:|---|---|---|---|
| 0 | `count` | MC2/测试场景 builder | `ccu::Load(..., 0)` | `CheckPeerTable` |
| 8 | `sendOffset` | MC2/测试场景 builder | `ccu::Load(..., 8)` | `CheckPeerTable` |

这两张表就是 adapter 和 Checker 共同遵守的 ABI。以下任意一个 producer/consumer 顺序不同都会出错：

```text
普通 testcase PrepareXxxTaskArgs
KFC BuildXxxKfcTaskArgs
产品 kernel LoadArg
RuntimeWorld PackTaskArgs
collective CheckTaskArgs
```

当前 `TaskParam` 每组有 13 个槽。参数超过 13 个时 `PackTaskArgs` 会分组，但这只是 ST 的 ABI 分组，不代表已经支持真实多 Mission 并发。

### 3.3 adapter 头文件具体写什么

在 `utils/src/ccu/ccu_xxx_adapter.h` 中至少定义两类配置和三个函数：

```cpp
constexpr uint32_t XXX_TASK_ARG_COUNT = /* LoadArg 数量 */;

// 注册期输入：只放决定 Program 静态结构的参数。
struct XxxKernelConfig {
    uint32_t rankId{0};
    uint32_t rankSize{0};
    std::vector<ChannelHandle> channels;
    // 例如 reduceType、algorithm variant 等静态 kernelArg。
};

// 启动期输入：与 LoadArg/KFC message 对应，并保留生成期望结果所需信息。
struct XxxLaunchConfig {
    uint64_t inputAddress{0};
    uint64_t outputAddress{0};
    uint64_t token{0};
    uint64_t parameterAddress{0};
    uint32_t rankId{0};
    // 例如 count、offset、goSize、peer table 的逻辑描述。
};

Result CaptureXxxKernel(void *kernelArg);

std::vector<uint64_t> PrepareXxxTaskArgs(
    const XxxLaunchConfig &config);

Result BuildXxxKfcTaskArgs(
    const CcuKfcMessage &message,
    const CcuKfcStartupArgs &startup,
    std::vector<uint64_t> &taskArgs);
```

两个配置不要混为一个：`XxxKernelConfig` 在注册期决定 Program 的 Rank、Channel 和静态分支；`XxxLaunchConfig` 在运行期提供地址、长度、offset 和 token。这样 Checker 才能区分“Program 编错”还是“Launch 参数传错”。

如果存在变长 peer 表，再定义一个纯数据结构，例如：

```cpp
struct XxxPeerParam {
    uint64_t count{0};
    uint64_t sourceOffset{0};
    uint64_t destinationOffset{0};
};
```

它既供 testcase 写入 `RankMemory.parameter`，也供 MC2 Checker 逐字段核对。不要让 testcase 和 Checker 各自声明一套布局。

### 3.4 `CaptureXxxKernel`：调用真实产品 kernel

这个函数承担“把不同签名的产品 kernel 接到统一 `KernelFunction(void *)`”的职责。标准步骤是：

```cpp
Result CaptureXxxKernel(void *kernelArg)
{
    if (kernelArg == nullptr) {
        return Result::PARAM_ERROR;
    }
    const auto &config = *static_cast<XxxKernelConfig *>(kernelArg);
    if (config.rankSize < 2 || config.rankId >= config.rankSize ||
        config.channels.size() != config.rankSize - 1) {
        return Result::PARAM_ERROR;
    }

    std::array<AscendC::ccu::Variable, XXX_TASK_ARG_COUNT> args;
    for (uint32_t i = 0; i < args.size(); ++i) {
        if (AscendC::ccu::LoadArg(args[i], i) != Result::SUCCESS) {
            return Result::CONTRACT_ERROR;
        }
    }

    CompilerContext::Current().ConfigureProgram(
        config.rankId, config.rankSize, config.channels);

    const CcuResult result = product_namespace::CcuXxxKernel(
        args[0], args[1] /* ... */,
        config.channels.data(),
        static_cast<uint32_t>(config.channels.size()),
        config.rankSize, config.rankId);
    return result == CCU_SUCCESS ? Result::SUCCESS : Result::CONTRACT_ERROR;
}
```

这个函数应该做：

- 校验 `kernelArg` 和静态 Rank/Channel 参数；
- 为每个产品 `LoadArg` 创建同下标的 `Variable`；
- 调用 `ConfigureProgram` 写入 Rank 和 Channel 元数据；
- 调用真实产品 kernel，让 `ccu_primitives_stub` 捕获 Program；
- 把产品返回码转换为 ST `Result`。

这个函数不应该做：

- 自己循环 peer；
- 自己生成 `WRITE`、`LOCAL_COPY`、GroupCopy/Reduce 或 notify/event；
- 自己计算产品算法的分块、尾块和同步顺序；
- 为了让 Checker 通过而手工补 Program operation。

一旦出现这些逻辑，adapter 就复制了被测算法，产品 kernel 写错时 ST 反而可能通过。

### 3.5 `PrepareXxxTaskArgs`：普通启动的 ABI 唯一出口

这个函数只按产品 `LoadArg` 顺序把 `XxxLaunchConfig` 展开成 `vector<uint64_t>`：

```cpp
std::vector<uint64_t> PrepareXxxTaskArgs(
    const XxxLaunchConfig &config)
{
    return {
        config.inputAddress,       // arg 0
        config.outputAddress,      // arg 1
        config.token,              // arg 2
        config.parameterAddress    // arg 3
        // ...必须与产品 LoadArg 顺序逐项一致
    };
}
```

它的职责不是“算算法”，而是集中管理 ABI 顺序。Kernel testcase 构造 `RankLaunch` 时必须调用这个函数：

```cpp
launches[rank] = RankLaunch{
    handles[rank],
    PrepareXxxTaskArgs(configs[rank]),
    &memories[rank]
};
```

不要在多个 testcase 中手写 `{input, output, token, ...}`。否则 ABI 改动时容易只改一个入口，普通启动与 KFC 启动悄悄分叉。

### 3.6 `BuildXxxKfcTaskArgs`：把消息映射到同一 ABI

KFC builder 的目标是生成与 `PrepareXxxTaskArgs` 完全相同的参数序列：

```cpp
Result BuildXxxKfcTaskArgs(
    const CcuKfcMessage &message,
    const CcuKfcStartupArgs &startup,
    std::vector<uint64_t> &taskArgs)
{
    if (message[0] == CCU_KFC_FINALIZE ||
        DecodeKfcCollective(message[0]) != CCU_KFC_XXX ||
        startup.token == 0) {
        return Result::CONTRACT_ERROR;
    }

    taskArgs = {
        message[1],        // inputAddress
        message[2],        // outputAddress
        startup.token,     // token 不一定在 message 中
        message[3]         // parameterAddress 或算法字段
        // ...按同一 argId 顺序继续映射
    };
    return Result::SUCCESS;
}
```

它应该校验 collective identity、finalize 和关键 startup/message 字段，然后只做字段映射。它不应该调用产品数据面 kernel，也不应该在 KFC 路径重新计算一套 peer 传输算法。

必须增加一个等价性 case，证明同一配置满足：

```text
PrepareXxxTaskArgs(launchConfig)
    == BuildXxxKfcTaskArgs(message, startup)
```

其中 token 等由 startup 提供的字段，要先填入等价的 `launchConfig` 再比较。

### 3.7 接入第二种 collective 时怎样改 KFC server adapter

当前 `CaptureKfcServerKernel` 把 `OpParam.opType` 写死为 `HCCL_CMD_ALLTOALLV`。因此第一种以外的 collective 不能只新增 `BuildXxxKfcTaskArgs`，还必须让 server 注册参数携带真实 opType。

推荐把当前单 `opId` 配置扩展为明确的支持项，例如：

```cpp
struct KfcSupportedOpConfig {
    uint64_t opId{0};
    mc2_ops_hccl::HcclCMDType opType;
};

struct KfcServerKernelConfig {
    uint32_t rankId{0};
    uint32_t rankSize{0};
    std::vector<ChannelHandle> channels;
    std::vector<KfcSupportedOpConfig> supportedOps;
};
```

然后让 `CaptureKfcServerKernel` 循环填充产品参数：

```cpp
for (const auto &supported : config.supportedOps) {
    mc2_ops_hccl::OpParam opParam{};
    opParam.opType = supported.opType;
    opParam.isMc2 = true;
    argument.supportedOpIds.push_back(supported.opId);
    argument.supportedOpParams.push_back(opParam);
}
```

原则上不需要为每个 collective 再复制一份 KFC server adapter，因为产品 KFC server kernel 是公共常驻调度器。collective 差异应落在：

- `CCU_KFC_XXX` identity；
- `opId -> collective kernel handle` 注册；
- `BuildXxxKfcTaskArgs`；
- `ccu_xxx_mc2_checker`。

`MakeKfcRegistration` 已经接收 `CcuKfcTaskArgBuilder`，新算法应把自己的 `BuildXxxKfcTaskArgs` 作为函数对象传入，而不是在 `CcuKfcSimWorld::DispatchOne` 里增加 `if (allgather) ... else if (reduce_scatter) ...`。

### 3.8 collective Checker 要新增哪些函数

在 `ccu_xxx_semantics_checker.h` 中建议沿用当前四阶段接口：

```cpp
struct XxxRuntimeExpectation {
    std::vector<std::vector<uint8_t>> outputs;
};

class CcuXxxSemanticsChecker {
public:
    CheckResult CheckRegistration(
        const Program &program,
        const XxxKernelConfig &config) const;

    CheckResult CheckTaskArgs(
        const Program &program,
        const std::vector<uint64_t> &taskArgs,
        const XxxLaunchConfig &config) const;

    CheckResult CaptureRuntimeExpectation(
        const std::vector<RankMemory> &memories,
        const std::vector<XxxLaunchConfig> &configs,
        XxxRuntimeExpectation &expectation) const;

    CheckResult CheckRuntime(
        const XxxRuntimeExpectation &expectation,
        const std::vector<RankMemory> &memories,
        const RuntimeWorld &world) const;
};
```

四个函数各自只回答一个问题：

| 函数 | 应检查什么 | 不要混入什么 |
|---|---|---|
| `CheckRegistration` | 先调用 `CcuProgramChecker`，再检查 Rank、Channel、LoadArg、原语数量/顺序、同步和 Loop 结构 | 运行期地址值和输出数据 |
| `CheckTaskArgs` | 先调用 `CcuTaskArgChecker`，再比较 `PrepareXxxTaskArgs(config)` 和实际 vector | 重新解释 Program |
| `CaptureRuntimeExpectation` | 在 Launch 前从初始 input/config 独立计算每 Rank 期望 output | 调用 `RuntimeWorld` 或读取执行后 output |
| `CheckRuntime` | 先调用 `CcuRuntimeChecker::CheckCompleted`，再逐 Rank 比较实际 output 与 expectation | 用 simulator 输出生成期望值 |

如果算法是 ReduceScatter，`XxxRuntimeExpectation` 应实际计算每个元素的 reduce 结果；如果当前 RuntimeWorld 尚未支持 `REDUCE`，先扩展公共 Operation/Stub/Runtime，再写 checker，不能用“只检查 operation 数量”冒充结果验证。

### 3.9 MC2 专用 Checker 要新增哪些函数

`ccu_xxx_mc2_checker` 只检查 KFC 消息与 MC2 参数布局，不重复普通 Program Checker：

```cpp
class CcuXxxMc2Checker {
public:
    CheckResult CheckMessage(
        const CcuKfcMessage &message,
        const RankMemory &memory,
        uint32_t rankSize,
        uint64_t expectedOpId) const;

    // 只有存在参数表时才需要。
    CheckResult CheckPeerTable(
        const RankMemory &memory,
        uint64_t tableAddress,
        const std::vector<XxxPeerParam> &peers) const;
};
```

`CheckMessage` 至少核对 `message[0]`、collective identity、input/output 地址、参数表范围和算法专用字段。`CheckPeerTable` 要按实际 byte stride 逐字段比较，并验证 `tableAddress + tableBytes` 没有越过当前 Rank 的 `parameter` 内存。

公共 `CcuKfcChecker` 继续负责 registration、六项 startup args、queue/CKE 状态和 server Program。不要把某个算法的 `message[5]` 含义写进公共 KFC Checker。

### 3.10 Kernel Device testcase 应有哪些辅助函数

参考 `all_to_all_v_kernel_device_testcase.cpp`，新文件建议保持以下骨架：

```cpp
XxxKernelConfig BuildKernelConfig(uint32_t rankId, uint32_t rankSize);

std::vector<KernelHandle> RegisterAllRanks(uint32_t rankSize);

struct XxxScenario {
    std::vector<RankMemory> memories;
    std::vector<XxxLaunchConfig> configs;
    std::vector<RankLaunch> launches;
};

XxxScenario BuildScenario(/* count/peer matrix 等测试输入 */,
    const std::vector<KernelHandle> &handles);

void VerifyScenario(/* 测试输入 */);
```

每个函数承担的功能是：

- `BuildKernelConfig`：构造当前 Rank 的静态 full-mesh Channel 和 kernelArg；
- `RegisterAllRanks`：执行一次 `RegisterStart`，逐 Rank 用 `CaptureXxxKernel` 注册，最后 `RegisterEnd`；
- `BuildScenario`：分配并初始化每 Rank input/output/parameter，构造 launch config，再调用 `PrepareXxxTaskArgs`；
- `VerifyScenario`：依次执行 `CheckRegistration -> CheckTaskArgs -> CaptureRuntimeExpectation -> RuntimeWorld::Launch -> CheckRuntime`。

至少增加以下正反例：

```text
正例：2 Rank 非对称数据
正例：4 Rank，含零长度 peer 或尾块
正例：in-place/out-of-place（产品支持时）
反例：少一个 taskArg
反例：交换两个同类型但语义不同的 taskArg
反例：parameterAddress/offset 越界
反例：执行后篡改一个 output byte，CheckRuntime 必须失败
```

最后一个反例用于证明最终语义 Checker 不是只看 `RuntimeWorld` completed。

### 3.11 KFC Device testcase 应有哪些辅助函数

参考 `all_to_all_v_kfc_device_testcase.cpp`，建议定义：

```cpp
std::vector<ChannelHandle> BuildChannels(
    uint32_t rankId, uint32_t rankSize);

struct CapturedKernels {
    std::vector<KernelHandle> server;
    std::vector<KernelHandle> collective;
};

CapturedKernels CaptureKernels(uint32_t rankSize);

struct XxxKfcScenario {
    std::vector<RankMemory> memories;
    std::vector<XxxLaunchConfig> configs;
    std::vector<CcuKfcMemory> queueMemory;
    std::vector<std::unique_ptr<CcuKfcQueueProxy>> queueOwners;
    std::vector<CcuKfcQueueProxy *> queues;
    std::vector<CcuKfcRegistration> registrations;
    CapturedKernels kernels;
};

XxxKfcScenario BuildKfcScenario(/* 测试输入 */);
void DispatchAndVerify(XxxKfcScenario &scenario);
```

`CaptureKernels` 在同一个 register group 中为每 Rank 捕获 server Program 和 collective Program。`BuildKfcScenario` 创建 queue memory/startup args，调用：

```cpp
MakeKfcRegistration(
    instance,
    serverHandle,
    collectiveHandle,
    opId,
    startup,
    BuildXxxKfcTaskArgs,
    registration);
```

随后按协议填写 `CcuKfcMessage` 并 `queue.Commit(message)`。`DispatchAndVerify` 的固定顺序应是：

```text
CcuKfcChecker::CheckRegistration
-> CcuXxxMc2Checker::CheckMessage/CheckPeerTable
-> CcuKfcChecker::CheckContext
-> CcuXxxSemanticsChecker::CaptureRuntimeExpectation
-> CcuKfcSimWorld::DispatchOne
-> CcuXxxSemanticsChecker::CheckRuntime
-> CcuKfcChecker::CheckQueue
-> queue.Wait
```

KFC 专项反例至少覆盖：错误 opId、未注册 collective、READY/CKE 不一致、槽未完成就复用、64 槽 wrap、部分 Rank finalize。不要为尚未接入的 collective 增加拒绝反例。

### 3.12 Host testcase 具体写什么

Host testcase 不使用上面的 Device adapter。它直接编译并调用真实 `hccl_mc2.cc`、`hccl_alloc_ctx_res.cc`，通常需要三个本地 builder：

```cpp
TopoMeta BuildTopoMeta(uint32_t rankSize);
Mc2CcTilingInner BuildXxxTiling(/* algorithm/dataType/rank */);
Mc2TilingTestData BuildMc2Tiling(
    const std::vector<Mc2CcTilingInner> &tilings);
```

case 调用真实资源入口后，应直接检查产物，而不是只断言 `HCCL_SUCCESS`：

- `commEngine` 是否进入 CCU 分支；
- `algName` 是否选择 Xxx；
- `opResCtx` 中 Rank、workspace、XN、CKE、token 是否有效；
- server/collective handle 和 startup args 是否匹配；
- 不同算法、dataType、Rank 或 tiling 是否命中正确 cache key；
- 依赖失败后是否返回真实错误并保持 cache/资源状态一致。

只有新产品 Host 源引入了尚未提供的外部符号，才修改 `utils/src/mc2_ccu/hccl_op_common_stub.cc` 或 `cann_host_bridge_stub.cc`。Stub 只补链接依赖，不要在其中伪造 `BuildXxxTiling` 或资源分配成功结果。

### 3.13 CMake 到底加哪几行

在 `testcase/mc2_ccu/CMakeLists.txt` 现有两个 `add_executable(...)` 的源文件列表中追加，不要为同名 target 再写第二个 `add_executable`：

```cmake
add_executable(${HCCL_MC2_CCU_HOST_TARGET}
    # 原有源文件
    host/xxx_mc2_host_testcase.cpp
)

add_executable(${HCCL_MC2_CCU_DEVICE_TARGET}
    # 原有源文件
    device/xxx_kernel_device_testcase.cpp
    device/xxx_kfc_device_testcase.cpp
    ${UTILS_DIR}/ccu/ccu_xxx_adapter.cc
    ${HCCL_XXX_CCU_KERNEL_DIR}/ccu_kernel_xxx.cc
)
```

如果仍复用同一个产品 KFC server kernel，不要重复加入第二份 server 源。新增产品目录时，同步把 kernel include 路径加入 `target_include_directories`。

在 `utils/src/hccl_verifier/semantics_check/CMakeLists.txt` 中：

```cmake
target_sources(hccl-verifier PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/ccu_xxx_semantics_checker.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/ccu_xxx_mc2_checker.cc
)
```

adapter 当前直接编入 Device target；Checker 编入 `hccl-verifier`。不要把 Checker `.cc` 直接塞进 testcase target，否则其他 collective 无法复用公共 verifier，也容易产生重复符号。

### 3.14 推荐的实现顺序

按下面顺序实现，出错时最容易定位：

1. 固定 ABI 表和产品 kernel 签名；
2. 写 `XxxKernelConfig/XxxLaunchConfig`；
3. 写 `CaptureXxxKernel`，先让 `CheckRegistration` 正例通过；
4. 写 `PrepareXxxTaskArgs`，让普通 Kernel Device 正例跑到 `RuntimeWorld`；
5. 写独立 expectation 和 `CheckRuntime`，补输出篡改反例；
6. 写 `BuildXxxKfcTaskArgs` 和 `CcuXxxMc2Checker`；
7. 接 KFC registration/queue，复用同一数据面结果 Checker；
8. 补 Host 资源用例；
9. 接入 CMake，运行 Host/Device 两个目标全量。

先做普通 Kernel Device，再接 KFC，可以把“数据面 Program 有问题”和“消息/调度有问题”分开定位。

## 4. 修改已有算法：具体改哪些函数

| 产品改动 | 直接修改的 ST 函数/结构 | 通常不要改 | 必补验证 |
|---|---|---|---|
| 只调整 kernel 原语顺序或同步，LoadArg 不变 | `CcuXxxSemanticsChecker::CheckRegistration`；`VerifyScenario` 对应 case | `CaptureXxxKernel`、`PrepareXxxTaskArgs`、KFC builder | Program 原语/notify/event 新契约和最终结果 |
| 新增、删除或交换 LoadArg | `XXX_TASK_ARG_COUNT`、`XxxLaunchConfig`、`CaptureXxxKernel` 的 Variable/产品调用、`PrepareXxxTaskArgs`、`BuildXxxKfcTaskArgs`、`CheckTaskArgs` | 公共 `CcuTaskArgChecker`，除非 13 槽通用 ABI 也变了 | 普通与 KFC 参数相等、少参、交换参数反例 |
| peer 表新增字段或改变 stride | `XxxPeerParam`、`BuildScenario/BuildKfcScenario` 的 parameter 写入、`CheckPeerTable`、产品 `LOAD_MEMORY` 静态检查 | `RegisterManager` | 字段顺序、表长、最后一个 peer 边界、越界/错位反例 |
| 最终数据布局或 reduce 规则变化 | `CaptureRuntimeExpectation`、`CheckRuntime`、Kernel/KFC 正例 | 公共 Program Checker | 独立期望、零长度/尾块/in-place、输出篡改反例 |
| 新 CCU OpCode/原语 | `OpCode/Operation`；`ccu_primitives_stub` 捕获函数；`RuntimeWorld::ExecuteOne`；`CcuLogicalResourceChecker`；collective checker | KFC queue 模型，除非协议也变 | 原语捕获、资源类型、地址 owner、执行结果、非法 handle |
| 新 KFC collective identity/opType | `CCU_KFC_XXX`；`KfcServerKernelConfig`/`CaptureKfcServerKernel`；`BuildXxxKfcTaskArgs`；`CcuXxxMc2Checker::CheckMessage`；testcase `CaptureKernels` | `CcuKfcSimWorld::DispatchOne` 的算法分支，应该继续走 registration builder | server Program dispatch、错误 opId、builder 映射 |
| KFC queue 状态或 CKE 协议变化 | `CcuKfcQueueProxy::{Commit,MarkCompleted,MarkFinalized,Wait}`；`CcuKfcSimWorld::DispatchOne`；`CcuKfcChecker::CheckQueue` | collective Program Checker | 每个状态迁移、wrap、未完成复用、跨 Rank finalize |
| MC2 tiling/资源/cache 变化 | Host testcase 的 `BuildXxxTiling` 和资源断言；必要时产品新依赖 Stub | Device adapter | 资源类型/数量、cache key、依赖失败和回滚 |
| Mission/LoopGroup 成为公共一等模型 | `ccu_st_types.h`；primitive/proxy；`RuntimeWorld`；公共 Checker；并发 testcase | 单个 collective checker 中硬编码公共调度 | 所有权、绑定、clone offset、调度依赖和冲突 |

最常见的判断方法是：

```text
产品函数签名/LoadArg 没变
  -> adapter 通常不改，只改 Program Checker 和 testcase

LoadArg 或 KFC message 字段变了
  -> Capture/Prepare/Build 三个 adapter 函数一起检查

产品用了 RuntimeWorld 不认识的新 CCU 原语
  -> 先贯通公共 OpCode/Stub/Runtime/Checker，再改算法 Checker
```

## 5. 现有 AllToAllV 文件索引

```text
utils/src/ccu/
  ccu_all_to_all_v_mesh1d_adapter.{h,cc}
  ccu_kfc_server_adapter.{h,cc}

utils/src/hccl_depends_stub/
  ccu_primitives_stub.{h,cc}

utils/src/hccl_proxy/
  ccu_st_types.h
  ccu_kernel_proxy.{h,cc}
  ccu_kfc_proxy.{h,cc}
  hcomm_ccu_stub.cc

utils/src/sim_world/
  ccu_sim_world.{h,cc}
  ccu_kfc_sim_world.{h,cc}

utils/src/hccl_verifier/
  ccu_checker.{h,cc}
  ccu_kfc_checker.{h,cc}
  semantics_check/ccu_all_to_all_v_semantics_checker.{h,cc}
  semantics_check/ccu_all_to_all_v_mc2_checker.{h,cc}

utils/src/mc2_ccu/
  cann_host_bridge_stub.cc
  hccl_op_common_stub.cc
```

当前 adapter 中的实际函数与上一章模板对应如下：

| 当前函数 | 所在文件 | 具体职责 | 什么时候修改 |
|---|---|---|---|
| `CaptureAllToAllVMesh1DKernel` | `ccu_all_to_all_v_mesh1d_adapter.cc` | 创建 10 个 `Variable/LoadArg`，配置 Rank/Channel，调用真实 `CcuAlltoallVMesh1DKernel` | 产品 kernel 签名、LoadArg 或静态 kernelArg 变化 |
| `PrepareAllToAllVTaskArgs` | 同上 | 把普通 Launch config 展开成 10 项 taskArgs | taskArg 数量、顺序或来源变化 |
| `BuildAllToAllVKfcTaskArgs` | 同上 | 把 KFC message/startup token 映射成同一 10 项 taskArgs | message ABI、opId 或 taskArg 映射变化 |
| `CaptureKfcServerKernel` | `ccu_kfc_server_adapter.cc` | 构造产品 `CcuKernelArgKfcServer` 并捕获真实 server Program | 支持的 opId/opType、Rank/Channel 或 server kernelArg 变化 |

AllToAllV 当前 ABI 可以直接作为逐项核对示例：

| argId | 普通 `PrepareAllToAllVTaskArgs` | KFC `BuildAllToAllVKfcTaskArgs` |
|---:|---|---|
| 0 | `config.inputAddress` | `message[1]` |
| 1 | `config.outputAddress` | `message[2]` |
| 2 | `config.token` | `startup.token` |
| 3 | `config.sourceBaseOffset` | `message[3]` |
| 4 | `config.destinationBaseOffset` | `message[4]` |
| 5 | `config.parameterAddress` | `message[9]` |
| 6 | `config.goSize[0]` | `message[5]` |
| 7 | `config.goSize[1]` | `message[6]` |
| 8 | `config.goSize[2]` | `message[7]` |
| 9 | `config.goSize[3]` | `message[8]` |

`CcuAllToAllVSemanticsChecker` 的四个函数分别对应 Program、taskArgs、独立期望和执行后结果；`CcuAllToAllVMc2Checker::CheckMessage/CheckPeerTable` 对应消息和四字段 peer 表。新增算法时应保持这种职责拆分，而不是写一个同时修改 Program、执行 Runtime、再比较结果的“大 Checker”。

`cann_host_bridge_stub.cc` 强制 Host ST 使用本地可控路径。`hccl_op_common_stub.cc` 为真实 `hccl_mc2.cc` 和 `hccl_alloc_ctx_res.cc` 提供最小 selector/executor/资源上下文依赖；它不校验完整产品 selector/executor。

## 6. 修改算法时怎样选择回归范围

| 修改范围 | 最小回归 |
|---|---|
| Host tiling/资源 | Host 目标全部 case |
| 普通 CCU kernel | Kernel Device 全部 case，随后 KFC Device 结果 case |
| KFC server/消息 | KFC Device 全部 case |
| taskArg/peer 表 | Kernel Device + KFC Device 全部 case |
| 公共 CCU proxy/sim/checker | Device 目标全部 CCU case |
| AICPU 通用原语/DAG | 所有受影响 AICPU suites |

共享数据面发生变化时，只跑某一个入口不够。普通 kernel 与 KFC 路径必须证明使用同一 ABI 和同一结果语义。

## 7. 构建和运行

当前 MC2 CCU 目标：

```text
ascendc_ut_adv_api_hccl_ccu_mc2_host
ascendc_ut_adv_api_hccl_ccu_mc2_device
```

构建命令以仓库 README 和当前构建目录为准。验证时至少执行：

```bash
<host-binary> --gtest_list_tests
<host-binary>
<device-binary> --gtest_list_tests
<device-binary>
```

调试顺序：先单 case，再同一校验域，最后两个目标全量。汇报必须给出实际通过数；不要把文档中的 9/8/10 或 27/27 当成无需复测的固定事实。

## 8. 提交前检查表

- [ ] 真实产品源进入构建，没有在 Stub 中复制被测逻辑。
- [ ] adapter 的 `Capture/Prepare/BuildKfcTaskArgs` 职责分开，没有复制 peer 循环和数据面算法。
- [ ] 普通 `PrepareXxxTaskArgs` 与 KFC `BuildXxxKfcTaskArgs` 有等价性断言。
- [ ] 公共 Checker 不含 collective 专用常量。
- [ ] Host、Kernel Device、KFC Device 覆盖与改动范围匹配。
- [ ] taskArgs/LoadArg/peer 表逐项对齐。
- [ ] `CaptureRuntimeExpectation` 在 Launch 前独立生成期望，正例检查最终数据而不只检查返回成功。
- [ ] 关键错误有定向反例，且不会被 success-only Stub 吞掉。
- [ ] `--gtest_list_tests`、实际通过数、CMake 和文档一致。
- [ ] `git diff --check` 通过，未残留旧目录或旧 suite 名。
