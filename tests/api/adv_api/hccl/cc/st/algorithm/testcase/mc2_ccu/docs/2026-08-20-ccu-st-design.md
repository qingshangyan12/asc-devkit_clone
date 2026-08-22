# CCU ST 框架设计与实现

> 本文档描述 asc-devkit 仓 CCU ST（System Test）框架的设计与实现，与代码保持同步。
>
> 代码路径：`tests/api/adv_api/hccl/cc/st/algorithm/`

## 目录

- [1. 概述](#1-概述)
- [2. MC2 端到端调用栈与交互边界](#2-mc2-端到端调用栈与交互边界)
  - [2.1 MC2 路径概述](#21-mc2-路径概述)
  - [2.2 功能代码真实调用链](#22-功能代码真实调用链以-alltoall-为例)
  - [2.3 ST 调用链](#23-st-调用链与功能代码的差异)
  - [2.4 op 交互边界分类](#24-op-交互边界分类)
  - [2.5 selector/executor/template/kernel 关联机制](#25-selectorexecutortemplatekernel-关联机制)
  - [2.6 参与的 ops 代码](#26-参与的-ops-代码)
- [3. 目录结构](#3-目录结构)
- [4. 框架分层](#4-框架分层)
- [5. AllToAll 用例](#5-alltoall-用例)
- [6. CcuStScenario — 用例配置](#6-ccustscenario--用例配置)
- [7. VerifyScenario — 通用流程](#7-verifyscenario--通用流程)
- [8. 检查机制](#8-检查机制)
  - [8.1 保留的检查](#81-保留的检查)
  - [8.2 去掉的检查](#82-去掉的检查)
  - [8.3 死锁精确定位](#83-死锁精确定位)
- [9. Program IR 与原语桩](#9-program-ir-与原语桩)
- [10. Channel 映射机制](#10-channel-映射机制)
- [11. 多线程执行模型](#11-多线程执行模型)
- [12. 注册机制](#12-注册机制)
- [13. 新增算法变体接入指南](#13-新增算法变体接入指南)
- [14. 新增 CCU 硬件原语接入指南](#14-新增-ccu-硬件原语接入指南)
- [15. 新增通信原语接入指南](#15-新增通信原语接入指南)
- [16. Loop Engine 仿真](#16-loop-engine-仿真)
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
HcclAllocComResourceByTiling [hccl_mc2.cc:324]         ← tiling 入口
│
├─ Phase 1: Host 侧准备
│   ├─ AllocCcuOpResCtx         (device 内存分配)
│   ├─ CcuSelectAlg             (算法选择 + 资源准备 + kernel 注册)
│   │   ├─ SelectCcuAlgorithm   (selector 选算法)
│   │   ├─ FillCcuAlgorithmInfo (设 isKfc=true)
│   │   └─ PrepareCcuAlgorithmResource → GetCcuOpParamResCtx
│   │       ├─ executor->CalcRes        (collective kernel CalcRes)
│   │       ├─ ccuKfcExecutor->CalcRes  (KFC server CalcRes，替换 collective → KFC server)
│   │       └─ GetAlgResCcu             (channel 分配 + kernel 注册)
│   └─ CopyCcuOpParamToDevice   (opParam H2D)
│
├─ Phase 2: CcuKernelLaunch    (启动 KFC server)
│
└─ Phase 3: Device 侧          (KFC server 微码 → DispatchKfcSubKernel → collective kernel)
```

**ST 验证范围**：ST 直接调 `SelectAlgAndPrepareEngine` + `GetCcuOpParamResCtx`，跳过 `CcuSelectAlg` → `FillCcuAlgorithmInfo`，因此 `isKfc=false`，不走 `ccuKfcExecutor->CalcRes`。注册的是原始 collective kernel（如 `CcuAlltoAllMesh1DKernel`），不是 KFC server kernel。

**ST 不验证**：KFC server 的 `InheritKfcServerKernelArg` 替换逻辑、`DispatchKfcSubKernel` 分发逻辑、Phase 2/3（KFC server / 微码 / 硬件）。

### 2.2 功能代码真实调用链（以 AllToAll 为例）

★ = AllToAll 算法交互边界，🔶 = KFC server 交互边界，🔹 = ST 使用的产品函数

```
HcclAllocComResourceByTiling [hccl_mc2.cc:324]
│
├─ AllocComResourceByTilingCcu [hccl_mc2.cc:289]
│   │
│   ├─ 🔹 AllocCcuOpResCtx [hccl_alloc_ctx_res.cc:161]
│   │   分配 workspace/XN/CKE device 内存
│   │
│   ├─ CcuSelectAlg [hccl_alloc_ctx_res.cc:987]
│   │   │  遍历 tilingList，逐算子处理：
│   │   │
│   │   └─ ProcessCcuTiling [hccl_alloc_ctx_res.cc:963]
│   │       │
│   │       ├─ RunCcuSelectAlgCheck (参数校验)
│   │       │
│   │       ├─ SelectCcuAlgorithm [hccl_alloc_ctx_res.cc:880]
│   │       │   ├─ 🔹 InitOpParamByTiling (从 tiling 填 opParam: opType/engine/dataType)
│   │       │   ├─ 🔹 TryForcedAlgAndPrepareEngine / 🔹 SelectAlgAndPrepareEngine
│   │       │   │   → AlltoAllAutoSelector::SelectCcuScheduleAlgo
│   │       │   │     → algName = "CcuAlltoAllMesh1D"
│   │       │   └─ algorithmMap 查表 → algorithmType
│   │       │
│   │       ├─ FillCcuAlgorithmInfo [hccl_alloc_ctx_res.cc:915]
│   │       │   → opParam.isKfc = true           ← ST 跳过此步（isKfc 保持 false）
│   │       │   → opResCtx.isKfc[tilingIndex] = true
│   │       │
│   │       ├─ PrepareCcuAlgorithmResource [hccl_alloc_ctx_res.cc:929]
│   │       │   ├─ 🔹 HandleSingleRankAndCommMode
│   │       │   └─ 🔹 GetCcuOpParamResCtx
│   │       │       └─ HcclGetAlgRes [op_common.cc:777]
│   │       │           │
│   │       │           ├─ ★ 🔹 executor->CalcRes
│   │       │           │   → CcuTempAlltoAllMesh1D::CalcRes
│   │       │           │     ★ kernelFunc = CcuAlltoAllMesh1DKernel
│   │       │           │     ★ kernelArg = CcuKernelArgAlltoAllMesh1D{channels, rankId, rankSize, ...}
│   │       │           │     ★ channelDescs = CalcChannelRequestMesh1D 产出
│   │       │           │
│   │       │           ├─ 🔶 ccuKfcExecutor->CalcRes (isKfc=true 时执行)
│   │       │           │   → CcuTempKfcServer::CalcRes            ← ST 不走（isKfc=false）
│   │       │           │     🔶 InheritKfcServerKernelArg:
│   │       │           │       - 新建 CcuKernelArgKfcServer（继承 rankId/rankSize/loadFromMem/opParam/subCommRanks）
│   │       │           │       - 继承源 kernelInfo 的 channels
│   │       │           │       - 替换 kernelFuncName = "CcuKernelKfcServer"
│   │       │           │       - 替换 kernelFunc = CcuKfcServerKernel
│   │       │           │       - 覆盖 ccuKernelInfos.back()（替换 collective kernel 为 KFC server）
│   │       │           │
│   │       │           └─ GetAlgResCcu [op_common.cc:1305]
│   │       │               ├─ HcclAllocAlgResourceCcu (CCU 资源分配)
│   │       │               ├─ 🔹 HcclGetThread (线程/notify 分配)
│   │       │               ├─ 🔹 HcclGetChannelForCcu [op_common.cc:1394]
│   │       │               │   → HcclChannelAcquire → channel handle 写入 kernelArg
│   │       │               └─ 🔹 HcclGetCcuKernel [op_common.cc:2095]
│   │       │                   → RegisterCcuKernels [op_common.cc:1698]
│   │       │                     ├─ 🔹 HcclCommQueryCcuIns
│   │       │                     ├─ 🔹 HcommCcuKernelRegisterStart
│   │       │                     ├─ 🔹 HcommCcuKernelRegister(kernelFuncName="CcuKernelAlltoAllMesh1D")
│   │       │                     │   → 真实：录制 Rep → CcuInstr 微码 → LoadInstruction
│   │       │                     │   → ST：查 capture 注册表 → adapter → RegisterManager → Program IR
│   │       │                     └─ 🔹 HcommCcuKernelRegisterEnd
│   │       │                         → ST：Finalize（IF/ELSE/WHILE 匹配 + LoadArg 排序）
│   │       │
│   │       └─ CopyCcuOpParamToDevice (opParam H2D 拷贝)    ← ST 不走
│   │
│   └─ GetOrCreateCcuCtx + aclrtMemcpy (opResCtx H2D 拷贝)  ← ST 不走
│
├─ Phase 2: CcuKernelLaunch [hccl_mc2.cc]
│   → HcommCcuKernelLaunch(KFC server kernel, startup args)
│
└─ Phase 3: Device 侧 (CCU 硬件执行微码)
    → KFC server 微码循环
      → CCU_IF(param[0] == opId)
        → DispatchKfcSubKernel
          → ★ CcuAllToAllMesh1DKernel(...)
            → CCU API 执行（Write/Notify/LocalCopy...）
```

### 2.3 ST 调用链（与功能代码的差异）

ST 的 `RegisterKernelForRank` 从 `AllocCcuOpResCtx` 之后直接调 `SelectAlgAndPrepareEngine`，跳过 `CcuSelectAlg` → `FillCcuAlgorithmInfo`：

```
ST RegisterKernelForRank:
│
├─ AllocCcuOpResCtx [hccl_alloc_ctx_res.cc:161]     ← 同功能代码
│
├─ InitOpParamByTiling [hccl_alloc_ctx_res.cc:518]   ← 同功能代码
│
├─ PrepareOpParam（虚函数）                            ← ST 专属：填 union 字段
│
├─ TryForcedAlgAndPrepareEngine / SelectAlgAndPrepareEngine  ← 同功能代码
│   → algName = "CcuAlltoAllMesh1D"
│
├─ HandleSingleRankAndCommMode                         ← 同功能代码
│
├─ GetCcuOpParamResCtx                                 ← 同功能代码
│   └─ HcclGetAlgRes [op_common.cc:777]
│       ├─ ★ executor->CalcRes                         ← 同功能代码
│       ├─ (isKfc=false → 跳过 ccuKfcExecutor->CalcRes) ← 差异：不走 KFC server
│       └─ GetAlgResCcu [op_common.cc:1305]
│           ├─ HcclGetChannelForCcu
│           │   → ChannelCommCreate → RegisterChannelMap ← ST 专属：channel 映射
│           └─ HcclGetCcuKernel → RegisterCcuKernels
│               └─ HcommCcuKernelRegister(kernelFuncName="CcuKernelAlltoAllMesh1D")
│                   → 查 capture 注册表 → adapter        ← ST 专属：桥接到 RegisterManager
│                     → ConvertAllToAllArg → RegisterManager::Register
│                       → capture function → CcuAlltoAllMesh1DKernel
│                         → CCU API 拦截 → Program IR
│
└─ 返回 kernelHandle
```

**关键差异**：

| 维度 | 功能代码 | ST |
|------|---------|-----|
| 入口 | `CcuSelectAlg` | 直接调 `SelectAlgAndPrepareEngine` |
| `isKfc` | `true`（`FillCcuAlgorithmInfo` 设置） | `false`（跳过 `FillCcuAlgorithmInfo`） |
| KFC server CalcRes | ✅ 执行（替换 collective → KFC server） | ❌ 跳过 |
| 注册的 kernel | `CcuKernelKfcServer` | `CcuAlltoAllMesh1DKernel` |
| `InheritKfcServerKernelArg` | ✅ 验证 | ❌ 不验证 |
| `DispatchKfcSubKernel` | ✅ 验证（Phase 3） | ❌ 不验证 |

### 2.4 op 交互边界分类

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

### 2.5 selector/executor/template/kernel 关联机制

| 关联 | 机制 | 时机 |
|------|------|------|
| opType → selector | `SelectorRegistry` 查表 | 运行时 |
| 引擎 → SelectXxxAlgo | `AutoSelectorBase::Select` if 分支 | 运行时 |
| topo+opType → algName | `SelectCcuScheduleAlgo` 条件逻辑 | 运行时 |
| opType+algName → executor | `CollAlgExecRegistryV2` 查表 | 运行时 |
| executor → template | `REGISTER_EXEC_V2` 模板参数 | 编译期 |
| template → kernel | `#include` + 取函数指针 | 编译期 |

selector 和 executor 之间没有直接调用，只通过 algName 字符串间接耦合。

### 2.6 参与的 ops 代码

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

### 3.1 构建目标

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

## 5. AllToAll 用例

### 5.1 用例清单

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

### 5.2 用例写法

```cpp
TEST_F(CcuStAllToAll, Mesh1D_2Rank_SingleDie_Fp16_100B)
{
    TopoMeta topo{{{{0, 1}}}};
    VerifyScenario(MakeScenario(topo, HCCL_DATA_TYPE_FP16, 100));
}
```

用例只传 `TopoMeta` + `dataType` + `sliceSize`，框架自动完成全链路。

---

## 6. CcuStScenario — 用例配置

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

## 7. VerifyScenario — 通用流程

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

## 8. 检查机制

### 8.1 保留的检查

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

### 8.2 去掉的检查

以下检查已去掉，由 R2（运行时执行）和 D2（逐字节比对）间接覆盖：

- 算法专属静态校验（channel 数量、full-mesh、mask 值、notify index、操作计数）
- taskArgs 值比对（自证无意义）
- taskArgs 数量/mission 绑定（R2 越界覆盖）
- CheckCompleted（和 R2 重复）

### 8.3 死锁精确定位

R2 死锁时输出每个 rank 卡在哪个 operation：

```
deadlock detected:
  rank 0 blocked at operation[26]: code=10(NOTIFY_WAIT) channel=0x7f.. index=0 mask=0x6 current=0x4
  rank 1 blocked at operation[26]: code=10(NOTIFY_WAIT) channel=0x7f.. index=0 mask=0x6 current=0x4
```

`mask=0x6`（期望）vs `current=0x4`（实际）→ 精确定位到缺少哪个 notify bit。

---

## 9. Program IR 与原语桩

### 9.1 OpCode

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

### 9.2 录制

产品 kernel 代码的 CCU API 调用被 `ccu_primitives_stub` 拦截，转成 `Operation` 追加到 `Program::operations[]`：

```
ccu::LoadArg(var, argId)           → Operation{LOAD_ARG}
ccu::Write(ch, dst, src, len)      → Operation{WRITE}
ccu::NotifyWait(ch, idx, mask)     → Operation{NOTIFY_WAIT}
CCU_IF(cond)                       → Operation{IF_BEGIN} + body + Operation{IF_END}
CCU_ELSE                           → Operation{ELSE_BEGIN} + body + Operation{IF_END}
CCU_WHILE(cond)                    → Operation{WHILE_BEGIN} + body + Operation{WHILE_END}
```

### 9.3 RuntimeWorld 执行

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

## 10. Channel 映射机制

### 10.1 问题

ST 的 channel handle 是 `SimChannel*` 裸指针。RuntimeWorld/checker 需要从 handle 反查 localRank/remoteRank。

### 10.2 方案

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

## 11. 多线程执行模型

### 11.1 设计

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

### 11.2 与非 CCU 路径统一

CCU 和非 CCU 都用 `ChannelCommCreate`（轮询配对方式），不用 `ChannelCommCreateForCcu`（已删除）。

### 11.3 线程安全

- `RegisterManager`：`mutex_` 保护 `kernels_`/`instances_`，`channelMapMutex_` 保护 `channelMap_`
- `GetLastCompiledGroup`：用 `thread_local` 避免多 rank 覆盖
- `HcclCommQueryCcuIns`：返回 per-rank instance handle，避免 instance 状态冲突
- `SimChannelExchangeHandler`：已有 `mutex_`
- `SimWorld`：Init 一次后不修改，并发读安全

### 11.4 生命周期

- `SimWorld::Init` 在 `VerifyScenario` 开头调一次
- `CleanupHostEnv` 只删 `comm`（不 Deinit SimWorld）
- `SimWorld::Deinit` 在 `VerifyScenario` 结尾调一次

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

## 14. 新增 CCU 硬件原语接入指南

CCU 硬件原语是 `ccu::Write`、`ccu::Notify`、`ccu::LocalCopy` 等底层操作。如果产品代码新增了 CCU 硬件原语（如 `ccu::Scatter`），需要扩展框架（utils）：

### 14.1 框架层改动

| 文件 | 改动 |
|------|------|
| `ccu_st_types.h` | `OpCode` 加新值（如 `SCATTER`） |
| `ccu_primitives_stub.h` | 加 `Result Scatter(...)` 声明 |
| `ccu_primitives_stub.cc` | 实现 stub：拦截调用 → 追加 `Operation{SCATTER}` 到 Program IR |
| `ccu_sim_world.cc` | `ExecuteOne` 加 `case OpCode::SCATTER` 执行逻辑 |
| `ccu_checker.cc` | `CcuLogicalResourceChecker` 加 `case OpCode::SCATTER` 资源校验 |

所有通信原语和算法变体自动获得新 CCU 硬件原语支持。

---

## 15. 新增通信原语接入指南

通信原语是 AllToAll、AllGather、AllReduce、ReduceScatter、Broadcast 等。新增通信原语需要改 testcase（原语层 + 算法层），不改 utils 框架。

以 AllGather 为例：

### 15.1 原语层（testcase）

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

### 15.2 算法层（testcase）

算法级实现：
- `BuildMesh1DScenario` — 填 `ScenarioData` 的 `srcOffsets`/`dstOffsets` 矩阵
- `CaptureAllGatherMesh1DKernel` — 录制 IR
- `ConvertAllGatherArg` — 产品 arg → ST config
- 静态注册 `RegisterCaptureFunction` + `RegisterVariant`

### 15.3 框架层（utils）

新增通信原语时 utils 需要改动：

| 文件 | 改动 |
|------|------|
| `testcase/mc2_ccu/CMakeLists.txt` | 加 AllGather product sources + include 路径 + testcase 源文件 |
| `utils/src/ccu_scenario/ccu_scenario.h` | `CcuStScenario` 的 `opType` 支持新原语枚举值（如 `HCCL_OP_ALLGATHER`） |
| `utils/src/hccl_proxy/stubs.cc` | 如新原语有额外的产品函数需要桩，补充 stub |

utils 的框架逻辑（`VerifyScenario`、`CcuProgramChecker`、`RuntimeWorld`、`CaptureRuntimeExpectation`、`CheckRuntimeOutput`）不需要改——它们是原语无关的通用流程。

### 15.4 期望输出计算

`CaptureRuntimeExpectation`（utils 通用）按 `ScenarioData` 的矩阵搬运数据，不需要各通信原语实现。如果通信原语有 reduce 语义（如 ReduceScatter），设置 `ScenarioData::reduceOp`（当前预留，未实现 reduce 逻辑）。

---

## 16. Loop Engine 仿真

### 16.1 背景

CCU 硬件的 `GroupCopy` 不是简单的单次 `memmove`，而是通过 **Loop Engine** 两层嵌套循环分块搬运数据。ST 框架的 `RuntimeWorld` 在遇到 `GROUP_COPY` IR 时，需要按硬件 Loop Engine 的语义展开执行，否则无法正确验证 `goSize` 分块参数和地址偏移。

### 16.2 硬件 Loop Engine 模型

参考 hcomm 仓 `CcuSimulator`（`test/hccl_vm/src/plugin/runner/ccu_executor/ccu_simulator.cc`），Loop Engine 由两层嵌套组成：

```
LoopGroup（外层展开）
 ├─ parallelParam（Xn 寄存器）: repeatNum, repeatLoopIndex, totalLoopNum
 ├─ offsetParam（Xm 寄存器）: gsaOffset, msOffset, ckeOffset
 └─ Loop[]（内层迭代）
     └─ loopParam（Xn 寄存器）: iterNum, gsaOffsetLoop
         └─ LoopBody（IR 序列）: LocalCopy(src→buf) + EventWait + LocalCopy(buf→dst) + EventWait
```

#### 参数解析

**parallelParam**（来自 `GetParallelParam`）：

| 字段 | 位段 | 含义 |
|------|------|------|
| repeatNum | [61:55] | 外层展开次数（实际执行 repeatNum+1 次） |
| repeatLoopIndex | [54:48] | 展开时从哪个 Loop 开始 |
| totalLoopNum | [47:41] | Loop 总数 |

**offsetParam**（来自 `GetOffsetParam`）：

| 字段 | 位段 | 含义 |
|------|------|------|
| gsaOffset | [52:21] | 外层每次展开的 GSA 地址步进 |
| msOffset | [20:10] | 外层每次展开的 MS（buffer）索引步进 |
| ckeOffset | [9:0] | 外层每次展开的 CKE 索引步进 |

**loopParam**（来自 `GetLoopParam`）：

| 字段 | 位段 | 含义 |
|------|------|------|
| loopCtxId | [52:45] | LoopEngine ID |
| gsaOffsetLoop | [44:13] | 内层每次迭代的 GSA 地址步进 |
| iterNum | [12:0] | 内层迭代次数 |

#### 地址偏移公式

每次 LoopBody 执行时，源/目标地址的偏移量（参考 `CcuSimulator::GetLoopGsaAddrOffset`）：

```
addrOffset = gsaOffset × loopExtendIndex + gsaOffsetLoop × curLoopRound
```

- `loopExtendIndex`：外层展开序号（0 ~ repeatNum）
- `curLoopRound`：内层迭代序号（0 ~ iterNum-1）

#### 执行流程

参考 `CcuSimulator::ExecuteLoopGroup`（ccu_simulator.cc:235）：

```
for (idx = 0; idx <= repeatNum; idx++) {
    loopStartIdx = (idx == 0) ? 0 : repeatLoopIndex
    for (li = loopStartIdx; li < totalLoopNum; li++) {
        loopExtendIndex = idx
        // 执行 Loop 指令 → LoopExecutor::RunV1()
        //   解析 loopParam: iterNum, gsaOffsetLoop
        //   for (round = 0; round < iterNum; round++) {
        //       curLoopRound = round
        //       执行 LoopBody IR:
        //         LocalCopy(buf, src, len): src += addrOffset, msId += msOffset × idx
        //         EventWait: 阻塞等待 event mask
        //         LocalCopy(dst, buf, len): dst += addrOffset, msId += msOffset × idx
        //         EventWait: 阻塞等待 event mask
        //   }
    }
}
```

关键规则：
- `idx=0` 时从 Loop 0 开始执行所有 `totalLoopNum` 个 Loop
- `idx>0` 时从 `repeatLoopIndex` 开始执行
- 每个 Loop 内层迭代 `iterNum` 次
- 每次 LoopBody 中 `src→buf→dst` 两次搬运（buffer 中转）
- `EventWait` 是阻塞等待，不跳过

### 16.3 ST RuntimeWorld 实现

`ccu_sim_world.cc` 中 `GROUP_COPY` case 按上述硬件模型实现：

```cpp
for (idx = 0; idx <= repeatNum; idx++) {
    loopStart = (idx == 0) ? 0 : repeatLoopIndex;
    for (li = loopStart; li < totalLoopNum; li++) {
        // 解析 loopParam → iterNum, gsaOffsetLoop
        for (round = 0; round < iterNum; round++) {
            addrOffset = gsaOffsetGroup * idx + gsaOffsetLoop * round;
            msIdx = msOffset * idx;
            // 遍历 LoopBody IR:
            //   LOCAL_COPY: src→buffer 或 buffer→dst（按 handle 区分）
            //     src/dst 地址 += addrOffset
            //     buffer 用 handle + msIdx 索引（模拟 MS 偏移）
            //   EVENT_WAIT: 阻塞等待（if ((value & mask) != mask) return false）
        }
    }
}
```

与硬件的对应关系：

| 硬件 | ST 实现 |
|------|---------|
| `ExecuteLoopGroup` 外层展开 | `for (idx = 0; idx <= repeatNum; idx++)` |
| `loopStartIdx = (idx == 0) ? 0 : repeatLoopIndex` | `loopStart = (idx == 0) ? 0 : repeatLoopIndex` |
| `ExecuteLoop` 内层迭代 | `for (round = 0; round < iterNum; round++)` |
| `GetLoopGsaAddrOffset()` | `addrOffset = gsaOffsetGroup * idx + gsaOffsetLoop * round` |
| `GetLoopMsOffset()` | `msIdx = msOffset * idx` |
| `TransMemToMS` / `TransMSToMem` | `state.buffers[handle + msIdx]` + `memmove` |
| `WaitCkeProcess` 阻塞等待 | `if ((value & mask) != mask) return false` |

### 16.4 GroupCopy 编译阶段

产品代码 `GroupCopy`（`ccu_kernel_alg_base.cc:716`）在 kernel 注册时被调用，通过 CCU 原语桩生成 IR：

1. `CreateMultiOpCopy` → `AllocGoResource(loopCount=8, msPerLoop=8)` → `memSlice=32768`
   - 分配 `loopCount` 个 Event + `loopCount * msInterleave` 个 CcuBuffer
2. `CreateMultiOpCopy` → 创建 2 个 `Func`（lambda）+ 2 个 `Loop`
   - 每个 `Loop` 构造时调 `func.RunBody()`，lambda 中的 `LocalCopy/EventWait` 生成 IR
   - IR 存入 `program.loopBodies[loopHandle]`（通过 `CompilerContext::LoopBodyEnter/Exit`）
3. `GroupCopy` 中 `CCU_IF(goSize.addrOffset != 0)` → `LoopGroup` 构造
   - `LoopGroup` 调 `CreateLoopGroup`，存入 `GroupCopyDescriptor`
   - 主操作流追加一条 `GROUP_COPY` op（`op.index` 指向 descriptor）

### 16.5 goSize 计算

`CalGoSize(size, config)` 根据 `size` 和 `LoopGroupConfig` 计算分块参数：

```
loopSize = config.loopCount × config.memSlice
m = size / loopSize       // 完整外层迭代次数
n = (size % loopSize) / config.memSlice  // 剩余的 memSlice 数
p = size % config.memSlice  // 尾块
```

返回 `{offset, loopIterNum, loopExtendNum, tailSize}`：

| 字段 | 含义 | GroupCopy 用途 |
|------|------|---------------|
| offset (=addrOffset) | m × loopSize | 第一个 LoopGroup 总搬运量 |
| loopIterNum (=loopParam) | m | 第一个 LoopGroup 的内层迭代数 |
| loopExtendNum (=parallelParam) | GetParallelParam(n-1, ...) | 第二个 LoopGroup 的外层展开参数 |
| tailSize (=residual) | p 或 memSlice | 第二个 LoopGroup 的尾块大小 |

**重要**：`CalGoSize` 的 `config.memSlice` 必须与 `AllocGoResource` 的 `memSlice` 一致（均为 `LOCAL_COPY_MS_PER_LOOP × CCU_MS_SIZE = 32768`），否则 goSize 分块参数与 loop 参数不匹配。

参考 hccl 仓 AllToAll template（`ccu_temp_all_to_all_v_mesh_1D.cc:193`）：
```cpp
config.loopCount = CCU_MS_INTERLEAVE;              // 8
config.memSlice = CCU_MS_INTERLEAVE * CCU_MS_SIZE; // 32768
auto goSize = CalGoSize(sliceSize, config, GetCcuVersion());
```

ST 用例构造时必须使用相同的 config：
```cpp
LoopGroupConfig goConfig{};
goConfig.msInterleave = CCU_MS_INTERLEAVE;
goConfig.loopCount = CCU_MS_LOCAL_COPY_LOOP_COUNT;
goConfig.memSlice = LOCAL_COPY_MS_PER_LOOP * CCU_MS_SIZE;
auto goSize = CalGoSize(sliceSize, goConfig);
```

### 16.6 1MB 数据搬运示例

以 `sliceSize=1MB=1048576` 为例：

**CalGoSize 计算**（`memSlice=32768, loopCount=8`）：
```
loopSize = 8 × 32768 = 262144
m = 1048576 / 262144 = 4
n = 0, p = 0
goSize = {addrOffset=1048576, loopParam=4, parallelParam=0, residual=0}
```

**第一个 LoopGroup 执行**（`goSize.addrOffset != 0`）：
- `loopParam = GetLoopParam(0, 32768×8, 0) + 4` → `gsaOffsetLoop=262144, iterNum=4`
- `paraCfg = GetParallelParam(7, 0, 1)` → `repeatNum=7, repeatLoopIndex=0, totalLoopNum=1`
- `offsetCfg = GetOffsetParam(32768, 8, 1)` → `gsaOffsetGroup=32768, msOffset=8`

**执行展开**：
```
idx=0: loop 0, round 0~3 → addrOffset = 0, 262144, 524288, 786432
idx=1: loop 0, round 0~3 → addrOffset = 32768, 294912, 557056, 819200
idx=2: loop 0, round 0~3 → addrOffset = 65536, 327680, 589824, 851456
idx=3: loop 0, round 0~3 → addrOffset = 98304, 360448, 622592, 883712
idx=4: loop 0, round 0~3 → addrOffset = 131072, 393216, 655360, 915968
idx=5: loop 0, round 0~3 → addrOffset = 163840, 425984, 688128, 948224
idx=6: loop 0, round 0~3 → addrOffset = 196608, 458752, 720896, 980480
idx=7: loop 0, round 0~3 → addrOffset = 229376, 491520, 753664, 1012736
```

总搬运量 = 8 × 4 × 32768 = 1048576 = 1MB ✓

每次搬运：`src → buffer[msId] → dst`，buffer 索引 = `msOffset × idx = 8 × idx`。

### 16.7 100B 数据搬运示例

以 `sliceSize=100` 为例：

**CalGoSize 计算**：
```
loopSize = 8 × 32768 = 262144
m = 100 / 262144 = 0
n = 100 / 32768 = 0
p = 100
goSize = {addrOffset=0, loopParam=0, parallelParam=GetParallelParam(0,0,1), residual=100}
```

**第二个 LoopGroup 执行**（`goSize.parallelParam != 0`）：
- `paraCfg = GetParallelParam(0, 0, 1)` → `repeatNum=0, repeatLoopIndex=0, totalLoopNum=1`
- `loopParam = GetLoopParam(0, 0, 1)` → `gsaOffsetLoop=0, iterNum=1`
- `offsetCfg = GetOffsetParam(32768, 8, 1)` → `gsaOffsetGroup=32768, msOffset=8`
- `loopLen[0] = goSize.residual = 100`

**执行展开**：
```
idx=0: loop 0, round 0 → addrOffset=0, 搬运 100 字节
```

总搬运量 = 1 × 1 × 100 = 100B ✓

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
- GroupCopy Loop Engine 两层嵌套展开（参见 [§16](#16-loop-engine-仿真)）
- GroupCopy loop body buffer 中转（src→MS→dst）
- 驱动、硬件时序、性能
- 浮点逐元素数值精度
- reduce 语义（reduceOp 字段预留，ApplyReduce 未实现）
