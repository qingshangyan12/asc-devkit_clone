# CCU ST 框架设计与实现

> 本文档描述 asc-devkit 仓 CCU ST（System Test）框架的设计与实现，与代码保持同步。
>
> 代码路径：`tests/api/adv_api/hccl/cc/st/algorithm/`

## 目录

- [1. 概述](#1-概述)
- [2. MC2 端到端调用栈与交互边界](#2-mc2-端到端调用栈与交互边界)
- [3. 目录结构](#3-目录结构)
- [4. 框架分层](#4-框架分层)
- [5. CcuStScenario — 用例配置](#5-ccustscenario--用例配置)
- [6. VerifyScenario — 通用流程](#6-verifyscenario--通用流程)
- [7. 检查机制](#7-检查机制)
- [8. Program IR 与原语桩](#8-program-ir-与原语桩)
- [9. Channel 映射机制](#9-channel-映射机制)
- [10. 多线程执行模型](#10-多线程执行模型)
- [11. CCU_ELSE 双分支支持](#11-ccu_else-双分支支持)
- [12. 注册机制](#12-注册机制)
- [13. AllToAll 用例](#13-alltoall-用例)
- [14. 新增算法变体接入指南](#14-新增算法变体接入指南)
- [15. 新增 CCU 硬件原语接入指南](#15-新增-ccu-硬件原语接入指南)
- [16. 新增通信原语接入指南](#16-新增通信原语接入指南)
- [17. 边界声明](#17-边界声明)

---

## 1. 概述

CCU ST 是无硬件仿真测试框架，在 x86 上验证 CCU 算法的正确性。核心思路：

1. 调用**真实产品代码**（selector → CalcRes → channel → kernel 注册）
2. CCU API 调用被桩拦截，录制为 **Program IR**（操作序列）
3. **RuntimeWorld** 解释执行 Program IR，模拟多 rank 并发
4. 通用校验：静态结构检查 + 运行时死锁检测 + 数据逐字节比对

与 hccl 仓非 CCU（AICPU）ST 的对比：

| 维度 | AICPU ST | CCU ST |
|------|---------|--------|
| kernel 执行方式 | host 直接调用 aicpu 模板 | host 调用 kernel，桩录制 IR |
| 中间表示 | TaskStub | Program IR（OpCode 序列） |
| 校验方式 | 语义传播（BufferSemantic） | 运行时执行 + 逐字节比对 |
| 执行模型 | 多线程（每 rank 一个线程） | 多线程（同上） |

---

## 2. MC2 端到端调用栈与交互边界

### 2.1 MC2 路径概述

MC2 路径不走普通 Host API（`HcclAlltoAllV()`），有独立的 Host 阶段 + Device 阶段：

```
Phase 1: CcuSelectAlg (算法选择 + 资源准备 + kernel 注册)
Phase 2: CcuKernelLaunch (启动 KFC server)
Phase 3: Device 侧 KFC server 执行微码 → DispatchKfcSubKernel → collective kernel
```

ST 验证的是 Phase 1 的核心链路（selector → CalcRes → channel → kernel 注册），通过 `HcommCcuKernelRegister` 桥接到 `RegisterManager` 捕获 Program IR。

**注意**：ST 直接调 `SelectAlgAndPrepareEngine` + `GetCcuOpParamResCtx`，不经过 `CcuSelectAlg` → `FillCcuAlgorithmInfo`，因此 `isKfc=false`，不走 KFC server CalcRes（`InheritKfcServerKernelArg`）。注册的是原始 collective kernel（如 `CcuAlltoAllMesh1DKernel`），不是 KFC server kernel。不验证 Phase 2/3（KFC server / 微码 / 硬件）。

### 2.2 端到端调用链（以 AllToAll 为例）

★ = AllToAll 算法交互边界

```
ST RegisterKernelForRank:
│
├─ AllocCcuOpResCtx                          [hccl_alloc_ctx_res.cc]
│   分配 workspace/XN/CKE device 内存
│
├─ InitOpParamByTiling
│   → 从 tiling 填 opParam（opType/engine/dataType）
│
├─ PrepareOpParam（虚函数）
│   → 子类填 union 专属字段（如 AllToAll 的 sendCounts）
│
├─ SelectAlgAndPrepareEngine（真实 selector）
│   → AlltoAllAutoSelector::SelectCcuScheduleAlgo
│     → algName = "CcuAlltoAllMesh1D"
│
├─ HandleSingleRankAndCommMode
│
├─ GetCcuOpParamResCtx
│   └─ HcclGetAlgRes                         [op_common.cc]
│       │
│       ├─ ★ executor->CalcRes
│       │   → CcuTempAlltoAllMesh1D::CalcRes
│       │     ★ kernelFunc = CcuAlltoAllMesh1DKernel
│       │     ★ kernelArg = CcuKernelArgAlltoAllMesh1D{...}
│       │
│       ├─ (isKfc=false → 不走 ccuKfcExecutor->CalcRes)
│       │
│       └─ GetAlgResCcu                      [op_common.cc]
│           ├─ HcclGetChannelForCcu (通用)
│           │   → HcclChannelAcquire → ChannelCommCreate
│           │   → channel 写入 kernelArg
│           │   → RegisterChannelMap(handle, loc, rmt)
│           └─ HcclGetCcuKernel
│               ├─ HcclCommQueryCcuIns (per-rank instance handle)
│               ├─ HcommCcuKernelRegisterStart
│               ├─ HcommCcuKernelRegister(kernelFuncName="CcuKernelAlltoAllMesh1D")
│               │   → 查 capture 注册表 → adapter
│               │     → ConvertAllToAllArg: 产品 kernelArg → ST config
│               │     → RegisterManager::Register → capture function
│               │       → LoadArg + ConfigureProgram + 调真实 CcuAlltoAllMesh1DKernel
│               │         → CCU API 被 ccu_primitives_stub 拦截 → Program IR
│               └─ HcommCcuKernelRegisterEnd
│                   → Finalize（IF/ELSE/WHILE 匹配 + LoadArg 排序）
│
└─ 返回 kernelHandle
```

### 2.3 op 交互边界分类

**ops ↔ hcomm 库交界（通用）**：

| 位置 | 代码 | 交界内容 |
|------|------|---------|
| `op_common.cc` | `HcommCcuKernelRegister` | ops 把 kernelFunc + kernelArg 交给 hcomm 库 |
| `op_common.cc` | `HcommCcuKernelRegisterEnd` | hcomm 库完成注册 |
| `hccl_mc2.cc` | `HcommCcuKernelLaunch` | ops 把 kernelHandle + taskArgs 交给 hcomm 库启动 |

**AllToAll 算法专属交界**：

| 位置 | 交界内容 |
|------|---------|
| `ccu_temp_all_to_all_mesh_1D.cc` CalcRes | kernelFunc 函数指针 + kernelArg 构建 |
| `ccu_kernel_all_to_all_mesh1d.cc` | collective kernel 函数（CCU API 调用） |

### 2.4 selector/executor/template/kernel 关联机制

| 关联 | 机制 | 时机 |
|------|------|------|
| opType → selector | `SelectorRegistry` 查表 | 运行时 |
| 引擎 → SelectXxxAlgo | `AutoSelectorBase::Select` if 分支 | 运行时 |
| topo+opType → algName | `SelectCcuScheduleAlgo` 条件逻辑 | 运行时 |
| opType+algName → executor | `CollAlgExecRegistryV2` 查表 | 运行时 |
| executor → template | `REGISTER_EXEC_V2` 模板参数 | 编译期 |
| template → kernel | `#include` + 取函数指针 | 编译期 |

selector 和 executor 之间没有直接调用，只通过 algName 字符串间接耦合。

### 2.5 参与的 ops 代码

| 代码 | 作用 | MC2 调用位置 |
|------|------|-------------|
| `selector/alltoall_auto_selector.cc` | 选算法 | CcuSelectAlg → SelectAlgAndPrepareEngine |
| `executor/ins_v2_all_to_all_v_sole_executor.cc` | CalcRes 构建模板 | HcclGetAlgRes → executor->CalcRes |
| `template/ccu/ccu_temp_all_to_all_mesh_1D.cc` | CalcRes 设置 kernelFunc+kernelArg | executor->CalcRes → template->CalcRes |
| `template/ccu/kernel/ccu_kernel_all_to_all_mesh1d.cc` | collective kernel 函数 | Register 时执行 |

---

## 3. 目录结构

```
tests/api/adv_api/hccl/cc/st/algorithm/
│
├── CMakeLists.txt                          ← 全局变量 + include 路径
│
├── testcase/mc2_ccu/                       ← 算子自包含
│   ├── CMakeLists.txt                      ← 构建 target
│   └── all_to_all/                         ← AllToAll 专属
│       ├── all_to_all_testcase.cpp          ← 用例（14 个 TEST_F）
│       ├── ccu_all_to_all.h/.cc             ← 原语级：CcuStAllToAll + 注册表 + PrepareOpParam
│       ├── ccu_all_to_all_mesh1d_adapter.h/.cc  ← 算法级：config + capture + BuildMesh1DScenario
│       └── shim/                            ← 桩
│           ├── ccu_primitives_dl.hpp         ← 头文件重定向（产品 include → ST stub）
│           └── stubs.cc                      ← 产品函数桩
│
└── utils/src/                              ← 纯通用框架
    │
    ├── ccu_scenario/                       ← 场景编排
    │   ├── ccu_scenario.h                  ← CcuStScenario 结构体
    │   └── ccu_fixture.h/.cc               ← CcuStFixture 基类 + VerifyScenario
    │
    ├── hccl_proxy/                         ← 桩/代理层
    │   ├── ccu_st_types.h                  ← OpCode / Operation / Program
    │   ├── ccu_primitives_stub.h/.cc       ← CCU API 桩（录制 IR）
    │   ├── ccu_kernel_proxy.h/.cc          ← RegisterManager + channelMap
    │   ├── hcomm_ccu_stub.cc               ← HcommCcuKernelRegister 桥接
    │   ├── hccl_stub.cc                    ← HcclChannelAcquire 等 stub
    │   ├── aclrt_stub.cc                   ← ACL Runtime stub
    │   ├── communicator/                   ← SimCommunicator / SimChannel
    │   └── topo_model/                     ← TopoModel 拓扑模型
    │
    ├── hccl_verifier/                      ← 校验器
    │   ├── ccu_checker.h/.cc               ← 通用静态校验（S1-S4）
    │   ├── ccu_dag_checker.h/.cc           ← DAG 内存冲突检测
    │   ├── ccu_dag_types.h                 ← DAG 类型
    │   ├── ccu_runtime_checker.h/.cc       ← ScenarioData + 期望输出计算 + 逐字节比对
    │   └── semantics_check/                ← AICPU legacy checker（非 CCU 用）
    │
    └── sim_world/                          ← 仿真世界
        ├── ccu_sim_world.h/.cc             ← RuntimeWorld + RankMemory + RankLaunch
        ├── sim_world.h/.cc                 ← SimWorld（host 拓扑模拟，AICPU legacy）
        └── ...
```

### 构建目标

| Target | 可执行文件 | 说明 |
|--------|-----------|------|
| `ascendc_ut_adv_api_ccu_st` | `ascendc_ut_adv_api_ccu_st` | CCU ST 全链路测试 |

链接库：`hccl-model-proxy`（桩/执行引擎）+ `hccl-verifier`（校验器）

---

## 4. 框架分层

```
用例层（testcase）
  TEST_F → VerifyScenario(scenario)
    │
    ├─ 原语级（CcuStAllToAll）
    │   ├─ PrepareOpParam（填 OpParam union）
    │   └─ 注册表查 builder
    │
    ├─ 算法级（adapter）
    │   ├─ BuildScenarioData（填 ScenarioData 矩阵）
    │   ├─ CaptureKernel（录制 Program IR）
    │   └─ RegisterCaptureFunction + RegisterVariant
    │
    └─ 框架级（utils）
        ├─ CcuProgramChecker（静态校验）
        ├─ RuntimeWorld（执行 + 死锁精确定位）
        ├─ CcuDagChecker（内存冲突检测）
        └─ CaptureRuntimeExpectation + CheckRuntimeOutput（数据校验）
```

层次划分原则：
- **框架级（utils）**：所有原语/算法共用，加新原语时改这里
- **原语级（testcase/ccu_all_to_all）**：同一原语所有变体共用
- **算法级（testcase/adapter）**：各算法变体专属

---

## 5. CcuStScenario — 用例配置

```cpp
struct CcuStScenario {
    TopoMeta topoMeta;                    // 拓扑，rankSize 从此推导
    DevType devType = DEV_TYPE_950;       // 设备类型
    HcclDataType dataType = FP16;         // 数据类型
    HcclCMDType opType;                   // 算子类型

    std::string expectedAlgName;          // 期望 selector 选出的算法名
    std::string algConfig;                // 强制指定算法（空=自动选择）

    std::vector<std::vector<uint64_t>> sizes;  // sizes[src][dst] = 数据大小（bytes）
    uint64_t count = 0;                        // AllGather/ReduceScatter 用
    HcclReduceOp reduceType = HCCL_REDUCE_SUM;
    HcclDataType outputType = HCCL_DATA_TYPE_RESERVED;
    uint32_t root = 0;                    // Broadcast/Reduce/Scatter 用
    bool inPlace = false;                 // AllReduce/AllGather 用

    bool corruptOutput = false;           // 负向：篡改 output
    bool outOfRangeParam = false;         // 负向：参数越界
};
```

用例只需构造 `CcuStScenario` + 调 `VerifyScenario`。

---

## 6. VerifyScenario — 通用流程

```
VerifyScenario(scenario)
│
├── 1. 初始化
│   RegisterManager::Reset()
│   SimWorld::Init(topoMeta, devType)    ← 只 Init 一次
│
├── 2. 多线程注册 kernel
│   for (rank = 0..N-1):
│     std::thread → RegisterKernelForRank(rank)
│       → aclrtSetDevice → Sim_HcclCommInitClusterInfo
│       → AllocCcuOpResCtx → InitOpParamByTiling
│       → PrepareOpParam（虚函数，填 union）
│       → SelectAlgAndPrepareEngine（真实 selector）
│       → GetCcuOpParamResCtx（真实 CalcRes + channel + HcommCcuKernelRegister）
│         → 桥接到 RegisterManager → 捕获 Program IR
│       → CleanupHostEnv（只删 comm，不 Deinit SimWorld）
│   join all threads
│
├── 3. 构造测试数据
│   data = BuildScenarioData(scenario, handles)
│     → 查注册表，调算法级 builder
│     → 填 ScenarioData（memories + launches + srcOffsets/dstOffsets 矩阵）
│
├── 4. 通用静态校验
│   for (rank):
│     CcuProgramChecker().Check(*program)  ← S1-S4 通用
│
├── 5. 期望输出计算（通用）
│   CaptureRuntimeExpectation(data, scenario, expectation)
│     → 按 srcOffsets/dstOffsets/sizes 矩阵搬运数据
│
├── 6. 运行时执行 + 数据校验
│   RuntimeWorld::Launch(data.launches)   ← 执行 IR，死锁时精确定位
│   CcuDagChecker::Check(executionLog)    ← 内存冲突检测
│   CheckRuntimeOutput(memories, expectation, world)  ← 逐字节比对
│
│   if (corruptOutput):
│     篡改 output → EXPECT_FALSE(CheckRuntimeOutput)
│
└── 7. 清理
    SimWorld::Deinit()
```

---

## 7. 检查机制

### 7.1 保留的检查

| 编号 | 检查 | 层次 | 实现位置 | 说明 |
|------|------|------|---------|------|
| S1 | 资源 handle 全局唯一 | 通用 | `CcuLogicalResourceChecker`（utils） | Variable/Address/Event/Buffer 不冲突 |
| S2 | LoadArg id 连续 | 通用 | `CcuProgramChecker`（utils） | argId 0,1,2... 连续 |
| S3 | operation 引用合法 | 通用 | `CcuLogicalResourceChecker`（utils） | 引用的 resource 已注册 |
| S4 | IF/ELSE/WHILE 匹配 | 通用 | `CompilerContext::Finalize`（utils） | begin/end 配对 |
| D1 | 期望输出计算 | 通用 | `CaptureRuntimeExpectation`（utils） | 按 srcOffsets/dstOffsets 矩阵搬运 |
| R2 | 运行时执行 | 通用 | `RuntimeWorld::Launch`（utils） | 轮询执行 + 死锁精确定位 |
| D2 | 逐字节比对 | 通用 | `CheckRuntimeOutput`（utils） | 实际 output vs 期望 output |
| DAG | 内存冲突检测 | 通用 | `CcuDagChecker`（utils） | WAW/WAR/RAR 无同步冲突 |

### 7.2 去掉的检查

以下检查已去掉，由 R2（运行时执行）和 D2（逐字节比对）间接覆盖：

- 算法专属静态校验（channel 数量、full-mesh、mask 值、notify index、操作计数）
- taskArgs 值比对（自证无意义）
- taskArgs 数量/mission 绑定（R2 越界覆盖）
- CheckCompleted（和 R2 重复）

### 7.3 死锁精确定位

R2 死锁时输出每个 rank 卡在哪个 operation：

```
deadlock detected:
  rank 0 blocked at operation[26]: code=10(NOTIFY_WAIT) channel=0x7f.. index=0 mask=0x6 current=0x4
  rank 1 blocked at operation[26]: code=10(NOTIFY_WAIT) channel=0x7f.. index=0 mask=0x6 current=0x4
```

`mask=0x6`（期望）vs `current=0x4`（实际）→ 精确定位到缺少哪个 notify bit。

---

## 8. Program IR 与原语桩

### 8.1 OpCode

```cpp
enum class OpCode {
    LOAD_ARG, LOAD_MEMORY, STORE_MEMORY,
    ASSIGN_IMM, ASSIGN_VAR, ADD_VAR,
    ADDRESS_ASSIGN_VAR, ADDRESS_ASSIGN_ADDRESS, ADDRESS_ADD_VAR,
    WRITE_VARIABLE_NOTIFY, NOTIFY_WAIT, NOTIFY_RECORD,
    EVENT_RECORD, EVENT_WAIT,
    WRITE, LOCAL_COPY, GROUP_COPY,
    IF_BEGIN, ELSE_BEGIN, IF_END,
    WHILE_BEGIN, WHILE_END
};
```

### 8.2 录制

产品 kernel 代码的 CCU API 调用被 `ccu_primitives_stub` 拦截，转成 `Operation` 追加到 `Program::operations[]`：

```
ccu::LoadArg(var, argId)           → Operation{LOAD_ARG}
ccu::Write(ch, dst, src, len)      → Operation{WRITE}
ccu::NotifyWait(ch, idx, mask)     → Operation{NOTIFY_WAIT}
CCU_IF(cond)                       → Operation{IF_BEGIN} + body + Operation{IF_END}
CCU_ELSE                           → Operation{ELSE_BEGIN} + body + Operation{IF_END}
CCU_WHILE(cond)                    → Operation{WHILE_BEGIN} + body + Operation{WHILE_END}
```

### 8.3 RuntimeWorld 执行

轮询调度，每轮每个 rank 执行一条 operation：

- `LOAD_ARG` → 从 taskArgs 取值
- `WRITE` → 跨 rank memmove
- `NOTIFY_WAIT` → 检查 mask，不满足则阻塞
- `IF_BEGIN` 条件 false → 跳到 ELSE_BEGIN+1（有 else）或 IF_END+1（无 else）
- `ELSE_BEGIN` → 跳到 IF_END+1（跳过 else body）
- `WHILE_BEGIN` 条件 false → 跳到 WHILE_END+1
- 全部 completed → SUCCESS
- 全部阻塞 → DEADLOCK（精确定位）

---

## 9. Channel 映射机制

### 9.1 问题

ST 的 channel handle 是 `SimChannel*` 裸指针。RuntimeWorld/checker 需要从 handle 反查 localRank/remoteRank。

### 9.2 方案

框架提供统一的 `RegisterChannelMap`：

```cpp
// ccu_kernel_proxy.h
class RegisterManager {
    static void RegisterChannelMap(ChannelHandle handle, uint32_t localRank, uint32_t remoteRank);
    static bool GetChannelRanks(ChannelHandle handle, uint32_t &localRank, uint32_t &remoteRank);
};

// ChannelRemoteRank/ChannelLocalRank 优先查 channelMap，fallback 到位运算
```

`ChannelCommCreate` 里自动注册：

```cpp
channelList[i] = reinterpret_cast<ChannelHandle>(channel.get());
RegisterManager::RegisterChannelMap(channelList[i], locRank, rmtRank);
```

Adapter 直接用产品真实 handle：

```cpp
// ConvertAllToAllArg
for (uint32_t i = 0; i < src->channelCount; ++i) {
    config->channels.push_back(src->channels[i]);  // 真实 handle，不推导
}
```

---

## 10. 多线程执行模型

### 10.1 设计

`VerifyScenario` 里每个 rank 一个 `std::thread`，并发执行 `RegisterKernelForRank`：

```cpp
std::vector<std::thread> threads;
for (uint32_t rank = 0; rank < rankSize; ++rank) {
    threads.emplace_back([this, rank, ...]() {
        handles[rank] = RegisterKernelForRank(rank, ...);
    });
}
for (auto &thread : threads) { thread.join(); }
```

### 10.2 与非 CCU 路径统一

CCU 和非 CCU 都用 `ChannelCommCreate`（轮询配对方式），不用 `ChannelCommCreateForCcu`（已删除）。

### 10.3 线程安全

- `RegisterManager`：`mutex_` 保护 `kernels_`/`instances_`，`channelMapMutex_` 保护 `channelMap_`
- `GetLastCompiledGroup`：用 `thread_local` 避免多 rank 覆盖
- `HcclCommQueryCcuIns`：返回 per-rank instance handle，避免 instance 状态冲突
- `SimChannelExchangeHandler`：已有 `mutex_`
- `SimWorld`：Init 一次后不修改，并发读安全

### 10.4 生命周期

- `SimWorld::Init` 在 `VerifyScenario` 开头调一次
- `CleanupHostEnv` 只删 `comm`（不 Deinit SimWorld）
- `SimWorld::Deinit` 在 `VerifyScenario` 结尾调一次

---

## 11. CCU_ELSE 双分支支持

### 11.1 问题

原 `CCU_ELSE` 是 `if(false)`，ELSE body 不录制。当 `remaining==0`（如 sliceSize=0 或 256MB 整数倍）时，ELSE body 的 `EventRecord` 丢失，`EventWait` 死等。

### 11.2 修复

`CCU_ELSE` 改为用 `ControlScope(isElse=true)` 录制 `ELSE_BEGIN` + body + `IF_END`。

IR 结构：
```
无 ELSE:  IF_BEGIN(peerIndex=IF_END) ... IF_END(peerIndex=IF_BEGIN)
有 ELSE:  IF_BEGIN(peerIndex=ELSE_BEGIN) ... IF_END ELSE_BEGIN(peerIndex=IF_END2) ... IF_END2
```

RuntimeWorld 执行：
- 条件 true → 执行 IF body → IF_END → ELSE_BEGIN → 跳到 IF_END2 后
- 条件 false → 跳到 ELSE_BEGIN+1 → 执行 ELSE body → IF_END2 → 继续

---

## 12. 注册机制

### 12.1 Capture 注册

adapter 静态初始化时注册 kernel 名 → capture 函数 + arg 转换器：

```cpp
bool g_registered = []() {
    RegisterCaptureFunction("CcuKernelAlltoAllMesh1D",
        CaptureAllToAllMesh1DKernel,    // 录制 IR
        ConvertAllToAllArg,             // 产品 arg → ST config
        [](void *p) { delete ...; });   // 释放
    return true;
}();
```

`HcommCcuKernelRegister` 按 `kernelFuncName` 查注册表，自动调 capture。

### 12.2 Variant 注册

原语级类提供 `RegisterVariant`，算法级 builder 注册：

```cpp
CcuStAllToAll::RegisterVariant("CcuAlltoAllMesh1D",
    [](scenario, handles) -> ScenarioData {
        return BuildMesh1DScenario(scenario, handles);
    });
```

`BuildScenarioData` 按 `expectedAlgName` 查注册表。

---

## 13. AllToAll 用例

### 13.1 用例清单

| 分组 | 用例名 | Topo | DataType | DataSize |
|------|--------|------|----------|----------|
| 拓扑泛化 | Mesh1D_2Rank_SingleDie_Fp16_100B | {{{0,1}}} | FP16 | 100B |
| 拓扑泛化 | Mesh1D_4Rank_SingleDie_Fp16_64B | {{{0,1,2,3}}} | FP16 | 64B |
| 拓扑泛化 | Mesh1D_8Rank_SingleDie_Fp16_64B | {{{0..7}}} | FP16 | 64B |
| 数据类型 | Mesh1D_2Rank_SingleDie_Fp32_100B | {{{0,1}}} | FP32 | 100B |
| 数据类型 | Mesh1D_2Rank_SingleDie_Int8_100B | {{{0,1}}} | INT8 | 100B |
| 数据类型 | Mesh1D_2Rank_SingleDie_Int32_100B | {{{0,1}}} | INT32 | 100B |
| 数据类型 | Mesh1D_2Rank_SingleDie_Bfp16_100B | {{{0,1}}} | BFP16 | 100B |
| 数据量 | Mesh1D_2Rank_SingleDie_Fp16_1KB | {{{0,1}}} | FP16 | 1KB |
| 数据量 | Mesh1D_2Rank_SingleDie_Fp16_1MB | {{{0,1}}} | FP16 | 1MB |
| 数据量 | Mesh1D_2Rank_SingleDie_Fp16_10MB | {{{0,1}}} | FP16 | 10MB |
| 边界 | Mesh1D_2Rank_SingleDie_Fp16_0B | {{{0,1}}} | FP16 | 0B |
| 边界 | Mesh1D_2Rank_SingleDie_Fp16_256MB | {{{0,1}}} | FP16 | 256MB |
| 负向 | Negative_OutputCorruption_2Rank_SingleDie_Fp16_80B | {{{0,1}}} | FP16 | 80B |
| 强制算法 | ForcedAlg_2Rank_SingleDie_Fp16_100B | {{{0,1}}} | FP16 | 100B |

### 13.2 用例写法

```cpp
TEST_F(CcuStAllToAll, Mesh1D_2Rank_SingleDie_Fp16_100B)
{
    TopoMeta topo{{{{0, 1}}}};
    VerifyScenario(MakeScenario(topo, HCCL_DATA_TYPE_FP16, 100));
}
```

用例只传 `TopoMeta` + `dataType` + `sliceSize`，框架自动完成全链路。

---

## 13. 新增算法变体接入指南

以 `CcuAlltoAllMesh2Die` 为例，加新变体只改 testcase，不改 utils：

### 13.1 新建 adapter

```
testcase/mc2_ccu/all_to_all/
├── ccu_all_to_all_mesh2die_adapter.h/.cc   ← 新建
```

需要实现：
1. `AllToAllMesh2DieKernelConfig` / `AllToAllMesh2DieLaunchConfig` 结构体
2. `CaptureMesh2DieKernel` — 录制 IR
3. `ConvertMesh2DieArg` — 产品 arg → ST config
4. `BuildMesh2DieScenario` — 构造 ScenarioData（填 srcOffsets/dstOffsets 矩阵）
5. 静态注册：
   ```cpp
   RegisterCaptureFunction("CcuKernelAlltoAllMesh2Die", capture, convert, deleter);
   CcuStAllToAll::RegisterVariant("CcuAlltoAllMesh2Die", builder);
   ```

### 13.2 用例

```cpp
TEST_F(CcuStAllToAll, Mesh2Die_4Rank_Fp16_64B)
{
    TopoMeta topo{{{{0, 1, 8, 9}}}};
    CcuStScenario scenario = MakeScenario(topo, HCCL_DATA_TYPE_FP16, 64);
    scenario.expectedAlgName = "CcuAlltoAllMesh2Die";
    VerifyScenario(scenario);
}
```

### 13.3 不需要改的

- `ccu_all_to_all.h/.cc` — 原语级，不变
- `ccu_fixture.h/.cc` — 框架，不变
- utils 所有文件 — 不变

---

## 14. 新增算法变体接入指南

（同上，§13）

---

## 15. 新增 CCU 硬件原语接入指南

CCU 硬件原语是 `ccu::Write`、`ccu::Notify`、`ccu::LocalCopy` 等底层操作。如果产品代码新增了 CCU 硬件原语（如 `ccu::Scatter`），需要扩展框架（utils）：

### 15.1 框架层改动

| 文件 | 改动 |
|------|------|
| `ccu_st_types.h` | `OpCode` 加新值（如 `SCATTER`） |
| `ccu_primitives_stub.h` | 加 `Result Scatter(...)` 声明 |
| `ccu_primitives_stub.cc` | 实现 stub：拦截调用 → 追加 `Operation{SCATTER}` 到 Program IR |
| `ccu_sim_world.cc` | `ExecuteOne` 加 `case OpCode::SCATTER` 执行逻辑 |
| `ccu_checker.cc` | `CcuLogicalResourceChecker` 加 `case OpCode::SCATTER` 资源校验 |

所有通信原语和算法变体自动获得新 CCU 硬件原语支持。

---

## 16. 新增通信原语接入指南

通信原语是 AllToAll、AllGather、AllReduce、ReduceScatter、Broadcast 等。新增通信原语需要改 testcase（原语层 + 算法层），不改 utils 框架。

以 AllGather 为例：

### 16.1 原语层（testcase）

新建 `testcase/mc2_ccu/all_gather/`：

```
all_gather/
├── all_gather_testcase.cpp              ← 用例
├── ccu_all_gather.h/.cc                 ← 原语级：CcuStAllGather + PrepareOpParam + 注册表
├── ccu_all_gather_mesh1d_adapter.h/.cc  ← 算法级：config + capture + builder
└── shim/
    ├── ccu_primitives_dl.hpp
    └── stubs.cc
```

原语级实现：
- `PrepareOpParam` — 填 `opParam.DataDes.count` / `opParam.DataDes.dataType`
- `RegisterVariant` — 注册算法变体

### 16.2 算法层（testcase）

算法级实现：
- `BuildMesh1DScenario` — 填 `ScenarioData` 的 `srcOffsets`/`dstOffsets` 矩阵
- `CaptureAllGatherMesh1DKernel` — 录制 IR
- `ConvertAllGatherArg` — 产品 arg → ST config
- 静态注册 `RegisterCaptureFunction` + `RegisterVariant`

### 16.3 期望输出计算

`CaptureRuntimeExpectation`（utils 通用）按 `ScenarioData` 的矩阵搬运数据，不需要各通信原语实现。如果通信原语有 reduce 语义（如 ReduceScatter），设置 `ScenarioData::reduceOp`（当前预留，未实现 reduce 逻辑）。

---

## 17. 边界声明

### 16.1 覆盖

- 真实 selector 算法选择逻辑
- 真实 template.CalcRes 构建 CcuKernelInfo
- 真实 channel 分配（HcclChannelAcquire）
- 真实 collective kernel 的 CCU API 调用形态
- Program IR 结构校验（资源唯一性、LoadArg 连续性、控制流匹配）
- RuntimeWorld 多 rank 执行 + 死锁精确定位
- DAG 内存冲突检测
- 独立期望字节结果对比

### 16.2 不覆盖

- KFC server kernel（queue/CKE/wrap/finalize）
- InheritKfcServerKernelArg（KFC server 替换逻辑）
- HcommCcuKernelRegister 真实微码录制（桥接到 RegisterManager，不录制真实微码）
- 真实 Translator、微码、CcuInstr
- 真实 XN/GSA/CKE/MS 物理资源分配
- 多 Mission/多 LoopEngine 真实并发
- GroupCopy 内部 MS/LoopEngine pipeline 逐节点执行
- 驱动、硬件时序、性能
- 浮点逐元素数值精度
- reduce 语义（reduceOp 字段预留，ApplyReduce 未实现）
