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
- [17. CCU 原语模拟与 hcomm 对比](#17-ccu-原语模拟与-hcomm-对比)
- [18. 边界声明](#18-边界声明)

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

**ST 验证范围**：ST 跳过 `CcuSelectAlg`（含 `FillCcuAlgorithmInfo`），直接调用 `AllocCcuOpResCtx` → `InitOpParamByTiling` → `TryForcedAlgAndPrepareEngine`/`SelectAlgAndPrepareEngine` → `HandleSingleRankAndCommMode` → `GetCcuOpParamResCtx`。因为不调 `FillCcuAlgorithmInfo`，所以 `isKfc=false`，不走 `ccuKfcExecutor->CalcRes`。注册的是原始 collective kernel（如 `CcuAlltoAllMesh1DKernel`），不是 KFC server kernel。

**ST 不验证**：KFC server 的 `InheritKfcServerKernelArg` 替换逻辑、`DispatchKfcSubKernel` 分发逻辑、Phase 2/3（KFC server / 微码 / 硬件）。

### 2.2 功能代码真实调用链（以 AllToAll 为例）

★ = AllToAll 算法交互边界，🔶 = KFC server 交互边界，🔹 = ST 使用的产品函数

> **产品路径 vs ST 路径的关键差异**：产品路径中 `FillCcuAlgorithmInfo` 设置 `opParam.isKfc = true`，
> `HcclGetAlgRes` 会执行 KFC Server 的 `CalcRes`，将 `ccuKernelInfos.back()` 替换为 KFC Server kernel
> （`kernelFuncName` 从 `"CcuKernelAlltoAllMesh1D"` 变为 `"CcuKernelKfcServer"`）。
> ST 路径不设置 `isKfc`，跳过 KFC Server 替换，直接注册原始 AllToAll kernel。

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
│   │       │   → 产品：opParam.isKfc = true
│   │       │   → 产品：opResCtx.isKfc[tilingIndex] = true
│   │       │   → ST：跳过此步（isKfc 保持 false）
│   │       │
│   │       ├─ PrepareCcuAlgorithmResource [hccl_alloc_ctx_res.cc:929]
│   │       │   ├─ 🔹 HandleSingleRankAndCommMode
│   │       │   └─ 🔹 GetCcuOpParamResCtx
│   │       │       └─ HcclGetAlgRes [op_common.cc:777]
│   │       │           │
│   │       │           ├─ ★ 🔹 executor->CalcRes
│   │       │           │   → CcuTempAlltoAllMesh1D::CalcRes
│   │       │           │     ★ kernelFuncName = "CcuKernelAlltoAllMesh1D"
│   │       │           │     ★ kernelFunc = CcuAlltoAllMesh1DKernel
│   │       │           │     ★ kernelArg = CcuKernelArgAlltoAllMesh1D{channels, rankId, rankSize, ...}
│   │       │           │     ★ channelDescs = CalcChannelRequestMesh1D 产出
│   │       │           │     ★ push_back 到 ccuKernelInfos
│   │       │           │
│   │       │           ├─ 🔶 ccuKfcExecutor->CalcRes (仅产品路径: isKfc=true 时执行)
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
│   │       │               │   → HcclChannelAcquire → channel handle 写入 kernelArg.channels[i]
│   │       │               │     (CalcChannelRequestMesh1D 产出 HcclChannelDesc → HcclChannelAcquire 申请 ChannelHandle)
│   │       │               └─ 🔹 HcclGetCcuKernel [op_common.cc:2095]
│   │       │                   → RegisterCcuKernels [op_common.cc:1698]
│   │       │                     ├─ 🔹 HcclCommQueryCcuIns
│   │       │                     ├─ 🔹 HcommCcuKernelRegisterStart
│   │       │                     ├─ 🔹 HcommCcuKernelRegister
│   │       │                     │   产品路径：kernelFuncName="CcuKernelKfcServer"（已被 KFC Server 替换）
│   │       │                     │     → 录制 KFC Server 微码 → 运行时 dispatch 到 CcuAlltoAllMesh1DKernel
│   │       │                     │   ST 路径：kernelFuncName="CcuKernelAlltoAllMesh1D"（未被替换）
│   │       │                     │     → 查 capture 注册表 → adapter → RegisterManager → Program IR
│   │       │                     └─ 🔹 HcommCcuKernelRegisterEnd
│   │       │                         → ST：Finalize（IF/ELSE/WHILE 匹配 + LoadArg 排序）
│   │       │
│   │       └─ CopyCcuOpParamToDevice (opParam H2D 拷贝)    ← ST 不走
│   │
│   └─ GetOrCreateCcuCtx + aclrtMemcpy (opResCtx H2D 拷贝)  ← ST 不走
│
├─ Phase 2: CcuKernelLaunch [hccl_mc2.cc]                   ← ST 不走
│   → HcommCcuKernelLaunch(KFC server kernel, startup args)
│
└─ Phase 3: Device 侧 (CCU 硬件执行微码)                     ← ST 不走
    → KFC server 微码循环
      → CCU_IF(param[0] == opId)
        → DispatchKfcSubKernel
          → ★ CcuAlltoAllMesh1DKernel(11个Variable参数, channels, channelCount, rankSize, rankId, loadFromMem)
            → CCU API 执行（Write/Notify/LocalCopy/GroupCopy...）
```

**注册名称对比**：

| 路径 | isKfc | 注册的 kernelFuncName | 说明 |
|------|-------|----------------------|------|
| 产品 | true | "CcuKernelKfcServer" | KFC Server 替换了原始 kernel，运行时 dispatch 到 AllToAll kernel |
| ST | false | "CcuKernelAlltoAllMesh1D" | 保留原始 kernel，ST capture 注册表按此名称查找 |

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
│       ├── all_to_all_testcase.cpp          ← 用例（24 个 TEST_F）
│       ├── ccu_all_to_all.h/.cc             ← 原语级：CcuStAllToAll + 注册表 + PrepareOpParam
│       └── ccu_all_to_all_mesh1d_adapter.h/.cc  ← 算法级：config + capture + BuildMesh1DScenario
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
    │   ├── ccu_primitives_dl.hpp           ← 头文件重定向（产品 include → ST stub）
    │   ├── ccu_kernel_proxy.h/.cc          ← RegisterManager + channelMap
    │   ├── hcomm_ccu_stub.cc               ← HcommCcuKernelRegister 桥接
    │   ├── hccl_stub.cc                    ← HcclChannelAcquire 等 stub
    │   ├── aclrt_stub.cc                   ← ACL Runtime stub
    │   ├── log_stub.cc                     ← 日志桩（支持 HCCL_LOG_LEVEL 环境变量）
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
    TopoMeta topoMeta;                                    // rankSize 从 topoMeta 自动推导
    DevType devType = DevType::DEV_TYPE_950;
    HcclDataType dataType = HCCL_DATA_TYPE_FP16;
    HcclCMDType opType = HcclCMDType::HCCL_CMD_ALLTOALLV;

    std::string expectedAlgName;          // 期望 selector 选出的算法名（空=不校验）
    std::string algConfig;                // 强制指定算法（空=自动选择）

    std::vector<std::vector<uint64_t>> sizes;  // sizes[src][dst] = 数据大小（bytes）
    uint64_t count = 0;                        // AllGather/ReduceScatter 的 per-rank 元素数
    HcclReduceOp reduceType = HCCL_REDUCE_SUM; // 归约操作类型（ReduceScatter/AllReduce）
    HcclDataType outputType = HCCL_DATA_TYPE_RESERVED;  // 低精度归约输出类型（RESERVED=同 dataType）
    uint32_t root = 0;                    // Broadcast/Reduce/Scatter 的 root rank
    bool inPlace = false;                 // 原地操作（input==output，AllReduce 用）
};
```

用例只需构造 `CcuStScenario` + 调 `VerifyScenario`。

**用例命名规范**：`{算法名}_{维度}_{参数}`，如 `CcuAlltoAllMesh1D_2Rank_SingleDie_Fp16_100B`。每个用例显式设置 `expectedAlgName`，不隐藏在 `MakeScenario` 辅助函数中。

**`expectedAlgName` 检查机制**：fixture 中 rank 0 检查 `algName != scenario.expectedAlgName` 时不匹配则返回 0（无 kernel handle），后续 `ASSERT_NE(handles[rank], 0U)` 失败。`expectedAlgName` 为空时跳过检查。

---

## 7. VerifyScenario — 通用流程

```
VerifyScenario(scenario)
│
├── 1. 初始化
│   RegisterManager::Reset()
│   setenv("HCCL_OP_EXPANSION_MODE", "CCU_SCHED")
│   InitEnvConfig()                     ← 初始化 g_algEnvConfig（解析环境变量）
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

### 9.4 产品函数桩消除

早期设计中，`testcase/mc2_ccu/all_to_all/shim/stubs.cc` 包含约 159 行产品函数桩，shadow 了 proxy 库中的真实实现。现已全部消除，改用 `hccl-model-proxy` 真实实现：

**链接策略**：

```
target_link_libraries(${HCCL_CCU_ST_TARGET} PRIVATE
    -Wl,--whole-archive hccl-model-proxy -Wl,--no-whole-archive
    ${HCCL_CHECKER_KERNEL_TARGET} hccl-verifier ...
)
```

`--whole-archive` 强制拉入 proxy 的所有 object（包括 `mmpa_stub.cc`、`hccl_profiling_dfx_stub.cc` 等），解决弱符号链接问题。CANN 头文件中 `mmSysGetEnv`、`HcclDfxRegOpInfoByCommId` 等声明为 `__attribute__((weak))`，弱定义在静态库中不会被链接器自动拉入。

**直接编译的产品源文件**（替代桩）：

| 产品源文件 | 替代的桩 | 说明 |
|-----------|---------|------|
| `ccu_log.cc` | `ConvertCcuToHccl` 桩 | CCU 错误码映射 |
| `ccu_res_dl.cc` | 9 个 `HcommIsSupportHcommCcu*` 桩 | CCU 资源 API 能力查询 |
| `hccl_ccu_res_dl.cc` | `HcommIsSupportHcclCommAssignCcuIns` 桩 | CCU ins 能力查询 |

**`InitEnvConfig()` 调用**：

fixture 在 `setenv("HCCL_OP_EXPANSION_MODE", "CCU_SCHED")` 之后调用 `InitEnvConfig()`，初始化 `g_algEnvConfig` 全局变量。真实版 `GetExternalInputHcclCcuSchedMode()` 返回 `g_algEnvConfig.ccuSchedMode`，依赖初始化；旧桩版直接读环境变量，行为不一致。

**剩余的唯一 header shim**：

`utils/src/hccl_proxy/ccu_primitives_dl.hpp` — 拦截产品代码的 `#include "ccu_primitives_dl.hpp"`，redirect 到 `ccu_primitives_stub.h`，是 ST 框架录制 CCU 原语为 IR 的核心入口。所有算子共享，不需要每个算子单独维护。

### 9.5 日志动态控制

`log_stub.cc` 支持运行时控制日志级别：

```
默认级别：INFO（0x01）
环境变量：HCCL_LOG_LEVEL=0（DEBUG）~ HCCL_LOG_LEVEL=3（ERROR）
API：     EnableLog(0) / DisableLog()（fixture 内调用）
```

日志级别对照：

| 值 | 级别 | 说明 |
|----|------|------|
| 0 | DEBUG | 全开 |
| 1 | INFO | 默认 |
| 2 | WARNING | 警告+错误 |
| 3 | ERROR | 只看错误 |

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
```

**注意**：新增算法**不需要** shim 目录。CCU 原语拦截由 `utils/src/hccl_proxy/ccu_primitives_dl.hpp` 统一处理，所有算子共享。产品函数桩已全部消除，改用 `hccl-model-proxy` 真实实现（见第 9 节）。

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

utils 的框架逻辑（`VerifyScenario`、`CcuProgramChecker`、`RuntimeWorld`、`CaptureRuntimeExpectation`、`CheckRuntimeOutput`）不需要改——它们是原语无关的通用流程。产品函数桩已全部消除，新增算子不需要写任何桩。

### 15.4 期望输出计算

`CaptureRuntimeExpectation`（utils 通用）按 `ScenarioData` 的矩阵搬运数据，不需要各通信原语实现。如果通信原语有 reduce 语义（如 ReduceScatter），设置 `ScenarioData::reduceOp`，`ApplyReduce` 按 dataType 逐元素归约（支持 INT8/FP16/BF16/INT32/FP32 × SUM/PROD/MAX/MIN）。

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

## 17. CCU 原语模拟与 hcomm 对比

本章节逐个对比 ST 框架（asc-devkit）与 hcomm 仓 `CcuSimulator` 中每个 CCU 原语的模拟逻辑，确认语义一致性。

参考代码：
- asc-devkit: `ccu_primitives_stub.h/.cc`（IR 生成）、`ccu_sim_world.cc`（执行）
- hcomm: `ccu_primitives.hpp`（原语定义）、`ccu_kernel.cc`（Rep 生成）、`ccu_simulator.cc` + 各 `XxxExecutor`（执行）

### 17.1 对比总表

| # | 原语 | 一致性 | 验证用例 | 关键差异 |
|---|------|--------|---------|---------|
| 1 | Variable 运算 | ✅ 一致 | Variable_AssignImm, Variable_AssignVar_And_Add | asc-devkit 扁平 OpCode；hcomm 多态 Rep + 延迟对象 |
| 2 | Address 运算 | ✅ 一致 | Address_AssignAndAdd | hcomm 额外支持 addr=imm, addr+addr 等 4 种重载 |
| 3 | LoadArg | ✅ 一致 | LoadArg_MultipleArgs | argId 切分逻辑一致（13 slot/SQE_ARGS_LEN） |
| 4 | Load/Store | ✅ 一致 | LoadStore_RoundTrip | hcomm 支持立即数地址；asc-devkit 仅变量地址 |
| 5 | WriteVariableWithNotify | ✅ 一致 | WriteVariableWithNotify_2Rank | asc-devkit 直接写远端 map；hcomm 经 SyncXn 微码 |
| 6 | NotifyWait/Record | ✅ 一致 | NotifyRecordWait_2Rank | asc-devkit 用 notifies map；hcomm 用 CKE 信号 |
| 7 | EventRecord/Wait | ✅ 一致 | EventRecordWait_SingleRank | asc-devkit 用 events map；hcomm 用 CKE 信号 |
| 8 | Write（跨 rank） | ✅ 一致 | Write_CrossRank_256B | hcomm 支持 reduce；asc-devkit 纯拷贝 |
| 9 | LocalCopy（Mem→Mem） | ✅ 一致 | LocalCopy_MemToMem_256B | 直接 memmove + 设置完成事件 |
| 10 | LocalCopy（Buffer↔Mem） | ✅ 一致 | LocalCopy_BufferRelay_256B | asc-devkit 用 state.buffers；hcomm 用 MS 数组 |
| 11 | Write with CcuBuffer | ✅ 一致 | Write_FromBuffer_2Rank_256B | asc-devkit 通用路径支持 buffer 索引 |
| 12 | CCU_IF/ELSE | ✅ 一致 | IfElse_BranchSelect, IfElse_ElseBranch | asc-devkit 用 peerIndex 跳转；hcomm 用 Label+Jump 微码 |
| 13 | CCU_WHILE | ✅ 一致 | While_Loop_Sum | 同 IF，asc-devkit 用 peerIndex 回跳 |
| 14 | GetResByChannel | ✅ 一致 | GetResByChannel_2Rank | asc-devkit 用逻辑 map；hcomm 绑定到 Xn 寄存器 |
| 15 | GroupCopy + Loop Engine | ✅ 一致 | 24 个 AllToAll 用例 | 参见 [§16](#16-loop-engine-仿真) |

### 17.2 逐原语详细对比

#### 1. Variable 运算

| 维度 | asc-devkit | hcomm |
|------|-----------|-------|
| IR 生成 | `ASSIGN_IMM`/`ASSIGN_VAR`/`ADD_VAR` Operation | `CcuRepAssign`/`CcuRepAdd` Rep 节点 |
| 执行 | `state.variables[dst] = immediate/variables[src0]` | `LoadImdToXn`/`LoadXnXn` 微码 → Xn 寄存器 |
| `operator+` | 立即生成 `ADD_VAR` IR | 延迟对象，赋值时才生成 `CcuRepAdd` 三参数形式 |
| `operator==/!=` | 返回 `Condition` 结构体，延迟到 CCU_IF | 返回 `CcuRelationalOperator`，延迟到 IfBegin/WhileBegin |

**语义一致**：变量赋值、加法、比较的最终计算结果相同。

#### 2. Address 运算

| 维度 | asc-devkit | hcomm |
|------|-----------|-------|
| IR 生成 | `ADDRESS_ASSIGN_VAR`/`ADDRESS_ASSIGN_ADDRESS`/`ADDRESS_ADD_VAR` | `CcuRepAssign(Address,Variable)` → `LoadXnToGsa`/`LoadGsaToGsa`/`LoadGsaXn` 微码 |
| 执行 | `state.addresses[dst] = variables/src0` | GSA 寄存器操作 |
| 支持的重载 | 3 种（=Variable, =Address, +=Variable） | 7 种（额外支持 =imm, +Address, +Variable 等） |

**语义一致**：asc-devkit 支持的 3 种操作与 hcomm 对应操作语义相同。hcomm 更完整但不影响现有 kernel。

#### 3. LoadArg

| 维度 | asc-devkit | hcomm |
|------|-----------|-------|
| IR 生成 | `LOAD_ARG` Operation + `RegisterArgument` | `CcuRepLoadArg` Rep → `LoadSqeArgsToXn` 微码 |
| 执行 | `taskParams[mission].args[slot]` | `GetSqeArgValue(sqeArgId)` |
| 切分逻辑 | `argId / 13` = mission, `argId % 13` = slot | `argId % CCU_SQE_ARGS_LEN` = slot |
| 校验 | `Finalize` 检查 argId 连续性 | `ValidateTaskArgs` 检查 `loadArgUsedSet_.size() == argsNum` |

**语义一致**：两者都从任务参数加载到变量寄存器，切分和校验逻辑等价。

#### 4. Load/Store

| 维度 | asc-devkit | hcomm |
|------|-----------|-------|
| IR 生成 | `LOAD_MEMORY`/`STORE_MEMORY` Operation | `CcuRepLoad`/`CcuRepStore` Rep |
| 执行 | `ResolveMemory` + 8 字节 `memcpy` | GSA 地址空间直接读写 |
| 地址源 | 仅 `Variable address` | 支持 `uint64_t addr` 和 `Variable addrVar` |
| owner 校验 | 强制地址属于当前 rank | 无此校验 |

**语义一致**：核心语义相同（内存↔变量 8 字节搬运）。差异在地址源类型和 owner 校验。

#### 5. WriteVariableWithNotify

| 维度 | asc-devkit | hcomm |
|------|-----------|-------|
| IR 生成 | `WRITE_VARIABLE_NOTIFY` Operation | `CcuRepRemPostVar` Rep → `SyncXn` 微码 |
| 执行 | 直接写远端 `variables[handle]` + 设置远端 `notifies` | `SyncXnExecutor`：同步 Xn 寄存器值 + 设置远端 CKE |
| 同步方式 | 同步立即生效（共享 RankState） | 通过 channel 同步（更接近硬件） |

**语义一致**：都将本端变量值写到远端并触发远端 notify 信号。

#### 6. NotifyWait/NotifyRecord

| 维度 | asc-devkit | hcomm |
|------|-----------|-------|
| IR 生成 | `NOTIFY_WAIT`/`NOTIFY_RECORD` Operation | `CcuRepRemWaitSem`/`CcuRepRemPostSem` Rep |
| 执行 | `notifies[{peer,index}]` mask 检查/设置 | CKE 信号 `WaitCkeProcess`/`SetRmtCKE` |
| Wait 语义 | 阻塞 + 自动清除 mask | 阻塞 + `clearType` 控制清除 |
| Record 语义 | 设置远端 notify mask | 设置远端 CKE 信号 |

**语义一致**：Record 设置远端信号，Wait 等待并消费本端信号。`clearType=1`（产品代码中默认）时清除行为一致。

#### 7. EventRecord/EventWait

| 维度 | asc-devkit | hcomm |
|------|-----------|-------|
| IR 生成 | `EVENT_RECORD`/`EVENT_WAIT` Operation | `CcuRepLocRecordEvent`/`CcuRepLocWaitEvent` Rep |
| 执行 | `events[handle]` mask 设置/检查 | CKE 信号 `SetCKE`/`WaitCKE` |
| Wait 语义 | 阻塞 + 自动清除 mask | 阻塞 + `clearType=1` 清除 |

**语义一致**：Record 设置本地事件信号，Wait 等待并消费本地事件信号。

#### 8. Write（跨 rank 数据搬运）

| 维度 | asc-devkit | hcomm |
|------|-----------|-------|
| IR 生成 | `WRITE` Operation | `CcuRepBufWrite` Rep → `TransLocMemToRmtMem` 微码 |
| 执行 | `ResolveMemory` + `memmove` 跨 rank | `TransMemToMem(locAddr, rmtAddr, length)` |
| rank 校验 | 校验 src 属本端、dst 属远端 | 无此校验 |
| reduce | 不支持（纯拷贝） | 支持 `reduceEn/reduceOpCode/reduceDataType` |

**语义一致**：跨 rank 数据搬运 + 设置完成事件。hcomm 额外支持 reduce。

#### 9. LocalCopy（Mem→Mem）

| 维度 | asc-devkit | hcomm |
|------|-----------|-------|
| IR 生成 | `LOCAL_COPY` Operation | `CcuRepLocCpy` Rep → `TransLocMemToLocMem` 微码 |
| 执行 | `ResolveMemory` + `memmove` | `TransMemToMem(srcAddr, dstAddr, length)` |
| rank 校验 | 校验 src/dst 都属本端 | 无此校验 |
| Loop 偏移 | GROUP_COPY 路径中加 `addrOffset` | `GetLoopGsaAddrOffset()` 通用 |

**语义一致**：本地内存拷贝 + 设置完成事件。

#### 10. LocalCopy（Buffer↔Mem）

| 维度 | asc-devkit | hcomm |
|------|-----------|-------|
| IR 生成 | `LOCAL_COPY` Operation（统一 OpCode） | `CcuRepBufLocRead`/`CcuRepBufLocWrite` Rep → `TransLocMemToLocMS`/`TransLocMSToLocMem` 微码 |
| 执行 | `state.buffers[handle]` + `memmove` | `TransMemToMS(msId)`/`TransMSToMem(msId)` |
| buffer 索引 | `handle + msOffset * idx` | `msId + msOffset * idx` |
| MS 容量 | `state.buffers` 动态 resize | 每个 MS 固定 4096 字节 |

**语义一致**：src→buffer→dst 两次搬运，buffer 用 `msOffset * idx` 索引偏移。asc-devkit 的 buffer 动态 resize 不限制 4096 字节，但不影响数据正确性。

#### 11. CCU_IF/CCU_ELSE

| 维度 | asc-devkit | hcomm |
|------|-----------|-------|
| IR 生成 | `IF_BEGIN`/`ELSE_BEGIN`/`IF_END` Operation + `peerIndex` | `CcuRepJumpEQ`/`CcuRepJumpNE`/`CcuRepJump` + Label |
| 执行 | 条件 false → `pc = peerIndex + 1` 跳过 | `JumpExecutor`：条件不满足 → 跳转到目标指令 ID |
| ELSE 语义 | `ELSE_BEGIN` 跳到 `IF_END` 之后 | 无条件 Jump 跳到 endLabel |
| 额外特性 | 无 | DoWhile、Label 栈、FlushPendingIfs |

**语义一致**：条件跳转实现 if/else 控制流。hcomm 额外支持 DoWhile 等高级控制流。

#### 12. CCU_WHILE

| 维度 | asc-devkit | hcomm |
|------|-----------|-------|
| IR 生成 | `WHILE_BEGIN`/`WHILE_END` Operation + `peerIndex` | `CcuRepJumpEQ/NE`（beginLabel）+ `CcuRepJump`（回跳）+ Label |
| 执行 | 条件 false → `pc = peerIndex + 1` 跳出；`WHILE_END` → `pc = peerIndex` 回跳 | `JumpExecutor` 跳转 |
| 回跳 | `WHILE_END` 的 `peerIndex` 指向 `WHILE_BEGIN` | 无条件 Jump 跳回 beginLabel |

**语义一致**：条件循环控制流。

#### 13. GetResByChannel

| 维度 | asc-devkit | hcomm |
|------|-----------|-------|
| IR 生成 | `GetChannelVariable` → `channelVariables[{peer,index}]` map | `CcuVariableCreateByChannel` → channel 的 `GetLocXnByIndex` |
| 执行 | `WriteVariableWithNotify` 中查远端 `channelVariables` | `SyncXn` 微码通过 Xn ID 同步 |
| 映射 | `(peerRank, index) → handle` 逻辑 map | channel → Xn 寄存器 ID 直接绑定 |

**语义一致**：创建与 channel 关联的变量，用于跨 rank 变量同步。

#### 14. GroupCopy + Loop Engine

参见 [§16 Loop Engine 仿真](#16-loop-engine-仿真)。两层嵌套展开、地址偏移公式、buffer 中转、event 同步全部对齐 hcomm `CcuSimulator`。

### 17.3 工程实现差异总结

| 维度 | asc-devkit | hcomm |
|------|-----------|-------|
| IR 类型 | 扁平 `Operation` 数组 + `OpCode` 枚举 | 多态 `CcuRepBase` 树 → `CcuInstr` 微码 |
| 执行模型 | switch-case 解释器 + 轮转调度 | 工厂模式 Executor + 状态机 |
| 资源模型 | `RankState` map（variables/addresses/events/buffers） | `CcuResourceManager`（XN/GSA/CKE/MS 数组） |
| 多 rank | 内置轮转调度 + 死锁检测 | 单 rank 执行，多 rank 由上层管理 |
| Reduce | 简化为 Copy | 实际 FP16/FP32/BF16 计算 |
| 校验 | 四层（编译→资源→运行→DAG+语义） | 基础执行检查 + profiling |

**结论**：15 类 CCU 原语的模拟语义全部与 hcomm `CcuSimulator` 一致。差异仅在工程实现层面（扁平 IR vs 树形 Rep、switch-case vs 工厂分发、map vs 数组），不影响验证结果的正确性。

---

## 18. 边界声明

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
- 浮点逐元素数值精度（FP16/BF16 转换精度，非严格 IEEE）
