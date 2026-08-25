/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

# CCU ST Framework Design and Implementation

> This document describes the design and implementation of the CCU ST (System Test) framework in the asc-devkit repository, kept in sync with the code.
>
> Code path: `tests/api/adv_api/hccl/cc/st/algorithm/`

## Table of Contents

- [1. Overview](#1-overview)
- [2. MC2 End-to-End Call Stack and Interaction Boundary](#2-mc2-end-to-end-call-stack-and-interaction-boundary)
  - [2.1 MC2 Path Overview](#21-mc2-path-overview)
  - [2.2 Functional Code Real Call Chain](#22-functional-code-real-call-chainusing-alltoall-as-example)
  - [2.3 ST Call Chain](#23-st-call-chain-differences-from-functional-code)
  - [2.4 op Interaction Boundary Classification](#24-op-interaction-boundary-classification)
  - [2.5 selector/executor/template/kernel Association Mechanism](#25-selectorexecutortemplatekernel-association-mechanism)
  - [2.6 Participating ops Code](#26-participating-ops-code)
- [3. Directory Structure](#3-directory-structure)
- [4. Framework Layering](#4-framework-layering)
- [5. AllToAll Test Cases](#5-alltoall-test-cases)
- [6. CcuStScenario — Test Case Configuration](#6-ccustscenario--test-case-configuration)
- [7. VerifyScenario — Generic Flow](#7-verifyscenario--generic-flow)
- [8. Check Mechanism](#8-check-mechanism)
  - [8.1 Retained Checks](#81-retained-checks)
  - [8.2 Removed Checks](#82-removed-checks)
  - [8.3 Deadlock Precise Localization](#83-deadlock-precise-localization)
- [9. Program IR and Primitive Stubs](#9-program-ir-and-primitive-stubs)
- [10. Channel Mapping Mechanism](#10-channel-mapping-mechanism)
- [11. Multi-threaded Execution Model](#11-multi-threaded-execution-model)
- [12. Registration Mechanism](#12-registration-mechanism)
- [13. Guide to Adding New Algorithm Variants](#13-guide-to-adding-new-algorithm-variants)
- [14. Guide to Adding New CCU Hardware Primitives](#14-guide-to-adding-new-ccu-hardware-primitives)
- [15. Guide to Adding New Communication Primitives](#15-guide-to-adding-new-communication-primitives)
- [16. Loop Engine Simulation](#16-loop-engine-simulation)
- [17. CCU Primitive Simulation vs hcomm](#17-ccu-primitive-simulation-vs-hcomm)
- [18. Boundary Declaration](#18-boundary-declaration)

---

## 1. Overview

CCU ST is a hardware-free simulation test framework that verifies the correctness of CCU algorithms on x86. Core approach:

1. Call **real product code** (selector → CalcRes → channel → kernel registration)
2. CCU API calls are intercepted by stubs and recorded as **Program IR** (operation sequence)
3. **RuntimeWorld** interprets and executes the Program IR, simulating multi-rank concurrency
4. Generic verification: static structure check + runtime deadlock detection + byte-by-byte data comparison

Comparison with hccl repository non-CCU (AICPU) ST:

| Dimension | AICPU ST | CCU ST |
|------|---------|--------|
| Kernel execution method | Host directly calls aicpu template | Host calls kernel, stub records IR |
| Intermediate representation | TaskStub | Program IR (OpCode sequence) |
| Verification method | Semantic propagation (BufferSemantic) | Runtime execution + byte-by-byte comparison |
| Execution model | Multi-threaded (one thread per rank) | Multi-threaded (same as above) |

---

## 2. MC2 End-to-End Call Stack and Interaction Boundary

### 2.1 MC2 Path Overview

The MC2 path does not go through the ordinary Host API (`HcclAlltoAllV()`); it has an independent Host phase + Device phase:

```
HcclAllocComResourceByTiling [hccl_mc2.cc:324]         ← tiling entry
│
├─ Phase 1: Host-side preparation
│   ├─ AllocCcuOpResCtx         (device memory allocation)
│   ├─ CcuSelectAlg             (algorithm selection + resource preparation + kernel registration)
│   │   ├─ SelectCcuAlgorithm   (selector chooses algorithm)
│   │   ├─ FillCcuAlgorithmInfo (set isKfc=true)
│   │   └─ PrepareCcuAlgorithmResource → GetCcuOpParamResCtx
│   │       ├─ executor->CalcRes        (collective kernel CalcRes)
│   │       ├─ ccuKfcExecutor->CalcRes  (KFC server CalcRes, replaces collective → KFC server)
│   │       └─ GetAlgResCcu             (channel allocation + kernel registration)
│   └─ CopyCcuOpParamToDevice   (opParam H2D)
│
├─ Phase 2: CcuKernelLaunch    (launch KFC server)
│
└─ Phase 3: Device side        (KFC server microcode → DispatchKfcSubKernel → collective kernel)
```

**ST verification scope**: ST skips `CcuSelectAlg` (including `FillCcuAlgorithmInfo`), and directly calls `AllocCcuOpResCtx` → `InitOpParamByTiling` → `TryForcedAlgAndPrepareEngine`/`SelectAlgAndPrepareEngine` → `HandleSingleRankAndCommMode` → `GetCcuOpParamResCtx`. Because `FillCcuAlgorithmInfo` is not called, `isKfc=false` and does not go through `ccuKfcExecutor->CalcRes`. The registered kernel is the original collective kernel (e.g., `CcuAlltoAllMesh1DKernel`), not the KFC server kernel.

**ST does not verify**: KFC server's `InheritKfcServerKernelArg` replacement logic, `DispatchKfcSubKernel` dispatch logic, Phase 2/3 (KFC server / microcode / hardware).

### 2.2 Functional Code Real Call Chain (using AllToAll as example)

★ = AllToAll algorithm interaction boundary, 🔶 = KFC server interaction boundary, 🔹 = product function used by ST

> **Product path vs ST path key difference**: In the product path, `FillCcuAlgorithmInfo` sets `opParam.isKfc = true`,
> and `HcclGetAlgRes` executes KFC Server's `CalcRes`, which replaces `ccuKernelInfos.back()` with the KFC Server kernel
> (`kernelFuncName` changes from `"CcuKernelAlltoAllMesh1D"` to `"CcuKernelKfcServer"`).
> The ST path does not set `isKfc`, skips the KFC Server replacement, and directly registers the original AllToAll kernel.

```
HcclAllocComResourceByTiling [hccl_mc2.cc:324]
│
├─ AllocComResourceByTilingCcu [hccl_mc2.cc:289]
│   │
│   ├─ 🔹 AllocCcuOpResCtx [hccl_alloc_ctx_res.cc:161]
│   │   allocate workspace/XN/CKE device memory
│   │
│   ├─ CcuSelectAlg [hccl_alloc_ctx_res.cc:987]
│   │   │  iterate tilingList, process per operator:
│   │   │
│   │   └─ ProcessCcuTiling [hccl_alloc_ctx_res.cc:963]
│   │       │
│   │       ├─ RunCcuSelectAlgCheck (parameter validation)
│   │       │
│   │       ├─ SelectCcuAlgorithm [hccl_alloc_ctx_res.cc:880]
│   │       │   ├─ 🔹 InitOpParamByTiling (fill opParam from tiling: opType/engine/dataType)
│   │       │   ├─ 🔹 TryForcedAlgAndPrepareEngine / 🔹 SelectAlgAndPrepareEngine
│   │       │   │   → AlltoAllAutoSelector::SelectCcuScheduleAlgo
│   │       │   │     → algName = "CcuAlltoAllMesh1D"
│   │       │   └─ algorithmMap lookup → algorithmType
│   │       │
│   │       ├─ FillCcuAlgorithmInfo [hccl_alloc_ctx_res.cc:915]
│   │       │   → Product: opParam.isKfc = true
│   │       │   → Product: opResCtx.isKfc[tilingIndex] = true
│   │       │   → ST: skips this step (isKfc remains false)
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
│   │       │           │     ★ channelDescs = produced by CalcChannelRequestMesh1D
│   │       │           │     ★ push_back to ccuKernelInfos
│   │       │           │
│   │       │           ├─ 🔶 ccuKfcExecutor->CalcRes (product path only: executed when isKfc=true)
│   │       │           │   → CcuTempKfcServer::CalcRes            ← ST does not go (isKfc=false)
│   │       │           │     🔶 InheritKfcServerKernelArg:
│   │       │           │       - create new CcuKernelArgKfcServer (inherits rankId/rankSize/loadFromMem/opParam/subCommRanks)
│   │       │           │       - inherit channels from source kernelInfo
│   │       │           │       - replace kernelFuncName = "CcuKernelKfcServer"
│   │       │           │       - replace kernelFunc = CcuKfcServerKernel
│   │       │           │       - overwrite ccuKernelInfos.back() (replace collective kernel with KFC server)
│   │       │           │
│   │       │           └─ GetAlgResCcu [op_common.cc:1305]
│   │       │               ├─ HcclAllocAlgResourceCcu (CCU resource allocation)
│   │       │               ├─ 🔹 HcclGetThread (thread/notify allocation)
│   │       │               ├─ 🔹 HcclGetChannelForCcu [op_common.cc:1394]
│   │       │               │   → HcclChannelAcquire → channel handle written to kernelArg.channels[i]
│   │       │               │     (CalcChannelRequestMesh1D produces HcclChannelDesc → HcclChannelAcquire allocates ChannelHandle)
│   │       │               └─ 🔹 HcclGetCcuKernel [op_common.cc:2095]
│   │       │                   → RegisterCcuKernels [op_common.cc:1698]
│   │       │                     ├─ 🔹 HcclCommQueryCcuIns
│   │       │                     ├─ 🔹 HcommCcuKernelRegisterStart
│   │       │                     ├─ 🔹 HcommCcuKernelRegister
│   │       │                     │   Product path: kernelFuncName="CcuKernelKfcServer" (replaced by KFC Server)
│   │       │                     │     → records KFC Server microcode → runtime dispatch to CcuAlltoAllMesh1DKernel
│   │       │                     │   ST path: kernelFuncName="CcuKernelAlltoAllMesh1D" (not replaced)
│   │       │                     │     → lookup capture registry → adapter → RegisterManager → Program IR
│   │       │                     └─ 🔹 HcommCcuKernelRegisterEnd
│   │       │                         → ST: Finalize (IF/ELSE/WHILE matching + LoadArg sorting)
│   │       │
│   │       └─ CopyCcuOpParamToDevice (opParam H2D copy)    ← ST does not go
│   │
│   └─ GetOrCreateCcuCtx + aclrtMemcpy (opResCtx H2D copy)  ← ST does not go
│
├─ Phase 2: CcuKernelLaunch [hccl_mc2.cc]                   ← ST does not go
│   → HcommCcuKernelLaunch(KFC server kernel, startup args)
│
└─ Phase 3: Device side (CCU hardware executes microcode)     ← ST does not go
    → KFC server microcode loop
      → CCU_IF(param[0] == opId)
        → DispatchKfcSubKernel
          → ★ CcuAlltoAllMesh1DKernel(11 Variable params, channels, channelCount, rankSize, rankId, loadFromMem)
            → CCU API execution (Write/Notify/LocalCopy/GroupCopy...)
```

**Registration name comparison**:

| Path | isKfc | Registered kernelFuncName | Description |
|------|-------|--------------------------|-------------|
| Product | true | "CcuKernelKfcServer" | KFC Server replaced original kernel, runtime dispatch to AllToAll kernel |
| ST | false | "CcuKernelAlltoAllMesh1D" | Original kernel preserved, ST capture registry looks up by this name |

### 2.3 ST Call Chain (differences from functional code)

ST's `RegisterKernelForRank` directly calls `SelectAlgAndPrepareEngine` after `AllocCcuOpResCtx`, skipping `CcuSelectAlg` → `FillCcuAlgorithmInfo`:

```
ST RegisterKernelForRank:
│
├─ AllocCcuOpResCtx [hccl_alloc_ctx_res.cc:161]     ← same as functional code
│
├─ InitOpParamByTiling [hccl_alloc_ctx_res.cc:518]   ← same as functional code
│
├─ PrepareOpParam (virtual function)                  ← ST-specific: fill union fields
│
├─ TryForcedAlgAndPrepareEngine / SelectAlgAndPrepareEngine  ← same as functional code
│   → algName = "CcuAlltoAllMesh1D"
│
├─ HandleSingleRankAndCommMode                         ← same as functional code
│
├─ GetCcuOpParamResCtx                                 ← same as functional code
│   └─ HcclGetAlgRes [op_common.cc:777]
│       ├─ ★ executor->CalcRes                         ← same as functional code
│       ├─ (isKfc=false → skip ccuKfcExecutor->CalcRes) ← difference: does not go through KFC server
│       └─ GetAlgResCcu [op_common.cc:1305]
│           ├─ HcclGetChannelForCcu
│           │   → ChannelCommCreate → RegisterChannelMap ← ST-specific: channel mapping
│           └─ HcclGetCcuKernel → RegisterCcuKernels
│               └─ HcommCcuKernelRegister(kernelFuncName="CcuKernelAlltoAllMesh1D")
│                   → lookup capture registry → adapter        ← ST-specific: bridge to RegisterManager
│                     → ConvertAllToAllArg → RegisterManager::Register
│                       → capture function → CcuAlltoAllMesh1DKernel
│                         → CCU API interception → Program IR
│
└─ return kernelHandle
```

**Key differences**:

| Dimension | Functional code | ST |
|------|---------|-----|
| Entry | `CcuSelectAlg` | Direct call to `SelectAlgAndPrepareEngine` |
| `isKfc` | `true` (set by `FillCcuAlgorithmInfo`) | `false` (skip `FillCcuAlgorithmInfo`) |
| KFC server CalcRes | ✅ executed (replaces collective → KFC server) | ❌ skipped |
| Registered kernel | `CcuKernelKfcServer` | `CcuAlltoAllMesh1DKernel` |
| `InheritKfcServerKernelArg` | ✅ verified | ❌ not verified |
| `DispatchKfcSubKernel` | ✅ verified (Phase 3) | ❌ not verified |

### 2.4 op Interaction Boundary Classification

**ops ↔ hcomm library boundary (generic)**:

| Location | Code | Boundary content |
|------|------|---------|
| `op_common.cc` | `HcommCcuKernelRegister` | ops hands over kernelFunc + kernelArg to hcomm library |
| `op_common.cc` | `HcommCcuKernelRegisterEnd` | hcomm library completes registration |
| `hccl_mc2.cc` | `HcommCcuKernelLaunch` | ops hand over kernelHandle + taskArgs to hcomm library for launch |

**AllToAll algorithm-specific boundary**:

| Location | Boundary content |
|------|---------|
| `ccu_temp_all_to_all_mesh_1D.cc` CalcRes | kernelFunc function pointer + kernelArg construction |
| `ccu_kernel_all_to_all_mesh1d.cc` | collective kernel function (CCU API calls) |

### 2.5 selector/executor/template/kernel Association Mechanism

| Association | Mechanism | Timing |
|------|------|------|
| opType → selector | `SelectorRegistry` lookup | Runtime |
| engine → SelectXxxAlgo | `AutoSelectorBase::Select` if branch | Runtime |
| topo+opType → algName | `SelectCcuScheduleAlgo` conditional logic | Runtime |
| opType+algName → executor | `CollAlgExecRegistryV2` lookup | Runtime |
| executor → template | `REGISTER_EXEC_V2` template parameter | Compile time |
| template → kernel | `#include` + take function pointer | Compile time |

There is no direct call between selector and executor; they are only indirectly coupled through the algName string.

### 2.6 Participating ops Code

| Code | Purpose | MC2 call location |
|------|------|-------------|
| `selector/alltoall_auto_selector.cc` | Algorithm selection | CcuSelectAlg → SelectAlgAndPrepareEngine |
| `executor/ins_v2_all_to_all_v_sole_executor.cc` | CalcRes template construction | HcclGetAlgRes → executor->CalcRes |
| `template/ccu/ccu_temp_all_to_all_mesh_1D.cc` | CalcRes sets kernelFunc+kernelArg | executor->CalcRes → template->CalcRes |
| `template/ccu/kernel/ccu_kernel_all_to_all_mesh1d.cc` | collective kernel function | Executed during Register |

---

## 3. Directory Structure

```
tests/api/adv_api/hccl/cc/st/algorithm/
│
├── CMakeLists.txt                          ← global variables + include paths
│
├── testcase/mc2_ccu/                       ← operator self-contained
│   ├── CMakeLists.txt                      ← build target
│   └── all_to_all/                         ← AllToAll-specific
│       ├── all_to_all_testcase.cpp          ← test cases (14 TEST_F)
│       ├── ccu_all_to_all.h/.cc             ← primitive-level: CcuStAllToAll + registry + PrepareOpParam
│       ├── ccu_all_to_all_mesh1d_adapter.h/.cc  ← algorithm-level: config + capture + BuildMesh1DScenario
│       └── shim/                            ← stubs
│           ├── ccu_primitives_dl.hpp         ← header redirection (product include → ST stub)
│           └── stubs.cc                      ← product function stubs
│
└── utils/src/                              ← pure generic framework
    │
    ├── ccu_scenario/                       ← scenario orchestration
    │   ├── ccu_scenario.h                  ← CcuStScenario struct
    │   └── ccu_fixture.h/.cc               ← CcuStFixture base class + VerifyScenario
    │
    ├── hccl_proxy/                         ← stub/proxy layer
    │   ├── ccu_st_types.h                  ← OpCode / Operation / Program
    │   ├── ccu_primitives_stub.h/.cc       ← CCU API stubs (record IR)
    │   ├── ccu_kernel_proxy.h/.cc          ← RegisterManager + channelMap
    │   ├── hcomm_ccu_stub.cc               ← HcommCcuKernelRegister bridge
    │   ├── hccl_stub.cc                    ← HcclChannelAcquire etc. stubs
    │   ├── aclrt_stub.cc                   ← ACL Runtime stub
    │   ├── communicator/                   ← SimCommunicator / SimChannel
    │   └── topo_model/                     ← TopoModel topology model
    │
    ├── hccl_verifier/                      ← verifiers
    │   ├── ccu_checker.h/.cc               ← generic static checks (S1-S4)
    │   ├── ccu_dag_checker.h/.cc           ← DAG memory conflict detection
    │   ├── ccu_dag_types.h                 ← DAG types
    │   ├── ccu_runtime_checker.h/.cc       ← ScenarioData + expected output computation + byte-by-byte comparison
    │   └── semantics_check/                ← AICPU legacy checker (for non-CCU use)
    │
    └── sim_world/                          ← simulation world
        ├── ccu_sim_world.h/.cc             ← RuntimeWorld + RankMemory + RankLaunch
        ├── sim_world.h/.cc                 ← SimWorld (host topology simulation, AICPU legacy)
        └── ...
```

### 3.1 Build Targets

| Target | Executable | Description |
|--------|-----------|------|
| `ascendc_ut_adv_api_ccu_st` | `ascendc_ut_adv_api_ccu_st` | CCU ST full-link test |

Linked libraries: `hccl-model-proxy` (stub/execution engine) + `hccl-verifier` (verifier)

---

## 4. Framework Layering

```
Test case layer (testcase)
  TEST_F → VerifyScenario(scenario)
    │
    ├─ Primitive level (CcuStAllToAll)
    │   ├─ PrepareOpParam (fill OpParam union)
    │   └─ registry lookup for builder
    │
    ├─ Algorithm level (adapter)
    │   ├─ BuildScenarioData (fill ScenarioData matrix)
    │   ├─ CaptureKernel (record Program IR)
    │   └─ RegisterCaptureFunction + RegisterVariant
    │
    └─ Framework level (utils)
        ├─ CcuProgramChecker (static checks)
        ├─ RuntimeWorld (execution + deadlock precise localization)
        ├─ CcuDagChecker (memory conflict detection)
        └─ CaptureRuntimeExpectation + CheckRuntimeOutput (data verification)
```

Layering principles:
- **Framework level (utils)**: shared by all primitives/algorithms; modified here when adding new primitives
- **Primitive level (testcase/ccu_all_to_all)**: shared by all variants of the same primitive
- **Algorithm level (testcase/adapter)**: specific to each algorithm variant

---

## 5. AllToAll Test Cases

### 5.1 Test Case List

| Group | Test case name | Topo | DataType | DataSize |
|------|--------|------|----------|----------|
| Topology generalization | Mesh1D_2Rank_SingleDie_Fp16_100B | {{{0,1}}} | FP16 | 100B |
| Topology generalization | Mesh1D_4Rank_SingleDie_Fp16_64B | {{{0,1,2,3}}} | FP16 | 64B |
| Topology generalization | Mesh1D_8Rank_SingleDie_Fp16_64B | {{{0..7}}} | FP16 | 64B |
| Data type | Mesh1D_2Rank_SingleDie_Fp32_100B | {{{0,1}}} | FP32 | 100B |
| Data type | Mesh1D_2Rank_SingleDie_Int8_100B | {{{0,1}}} | INT8 | 100B |
| Data type | Mesh1D_2Rank_SingleDie_Int32_100B | {{{0,1}}} | INT32 | 100B |
| Data type | Mesh1D_2Rank_SingleDie_Bfp16_100B | {{{0,1}}} | BFP16 | 100B |
| Data volume | Mesh1D_2Rank_SingleDie_Fp16_1KB | {{{0,1}}} | FP16 | 1KB |
| Data volume | Mesh1D_2Rank_SingleDie_Fp16_1MB | {{{0,1}}} | FP16 | 1MB |
| Data volume | Mesh1D_2Rank_SingleDie_Fp16_10MB | {{{0,1}}} | FP16 | 10MB |
| Boundary | Mesh1D_2Rank_SingleDie_Fp16_0B | {{{0,1}}} | FP16 | 0B |
| Boundary | Mesh1D_2Rank_SingleDie_Fp16_256MB | {{{0,1}}} | FP16 | 256MB |
| Negative | Negative_OutputCorruption_2Rank_SingleDie_Fp16_80B | {{{0,1}}} | FP16 | 80B |
| Forced algorithm | ForcedAlg_2Rank_SingleDie_Fp16_100B | {{{0,1}}} | FP16 | 100B |

### 5.2 Test Case Writing

```cpp
TEST_F(CcuStAllToAll, Mesh1D_2Rank_SingleDie_Fp16_100B)
{
    TopoMeta topo{{{{0, 1}}}};
    VerifyScenario(MakeScenario(topo, HCCL_DATA_TYPE_FP16, 100));
}
```

The test case only passes `TopoMeta` + `dataType` + `sliceSize`; the framework automatically completes the full link.

---

## 6. CcuStScenario — Test Case Configuration

```cpp
struct CcuStScenario {
    TopoMeta topoMeta;                    // topology, rankSize derived from this
    DevType devType = DEV_TYPE_950;       // device type
    HcclDataType dataType = FP16;         // data type
    HcclCMDType opType;                   // operator type

    std::string expectedAlgName;          // expected algorithm name (empty = skip assertion)
    std::string algConfig;                // forced algorithm (empty = auto-select)

    std::vector<std::vector<uint64_t>> sizes;  // sizes[src][dst] = data size (bytes)
    uint64_t count = 0;                        // per-rank element count for AllGather/ReduceScatter
    HcclReduceOp reduceType = HCCL_REDUCE_SUM; // reduce operation type (ReduceScatter/AllReduce)
    HcclDataType outputType = HCCL_DATA_TYPE_RESERVED;  // low-precision reduce output type (RESERVED = same as dataType)
    uint32_t root = 0;                    // root rank for Broadcast/Reduce/Scatter
    bool inPlace = false;                 // in-place operation (input==output, for AllReduce)

    bool corruptOutput = false;           // negative: corrupt output
    bool outOfRangeParam = false;         // negative: parameter out of range (reserved, currently unused)
};
```

The test case only needs to construct a `CcuStScenario` + call `VerifyScenario`.

---

## 7. VerifyScenario — Generic Flow

```
VerifyScenario(scenario)
│
├── 1. Initialization
│   RegisterManager::Reset()
│   SimWorld::Init(topoMeta, devType)    ← Init only once
│
├── 2. Multi-threaded kernel registration
│   for (rank = 0..N-1):
│     std::thread → RegisterKernelForRank(rank)
│       → aclrtSetDevice → Sim_HcclCommInitClusterInfo
│       → AllocCcuOpResCtx → InitOpParamByTiling
│       → PrepareOpParam (virtual function, fill union)
│       → SelectAlgAndPrepareEngine (real selector)
│       → GetCcuOpParamResCtx (real CalcRes + channel + HcommCcuKernelRegister)
│         → bridge to RegisterManager → capture Program IR
│       → CleanupHostEnv (only delete comm, does not Deinit SimWorld)
│   join all threads
│
├── 3. Construct test data
│   data = BuildScenarioData(scenario, handles)
│     → lookup registry, call algorithm-level builder
│     → fill ScenarioData (memories + launches + srcOffsets/dstOffsets matrix)
│
├── 4. Generic static checks
│   for (rank):
│     CcuProgramChecker().Check(*program)  ← S1-S4 generic
│
├── 5. Expected output computation (generic)
│   CaptureRuntimeExpectation(data, scenario, expectation)
│     → move data according to srcOffsets/dstOffsets/sizes matrix
│
├── 6. Runtime execution + data verification
│   RuntimeWorld::Launch(data.launches)   ← execute IR, precise localization on deadlock
│   CcuDagChecker::Check(executionLog)    ← memory conflict detection
│   CheckRuntimeOutput(memories, expectation, world)  ← byte-by-byte comparison
│
│   if (corruptOutput):
│     corrupt output → EXPECT_FALSE(CheckRuntimeOutput)
│
└── 7. Cleanup
    SimWorld::Deinit()
```

---

## 8. Check Mechanism

### 8.1 Retained Checks

| ID | Check | Layer | Implementation location | Description |
|------|------|------|---------|------|
| S1 | Resource handle globally unique | Generic | `CcuLogicalResourceChecker` (utils) | Variable/Address/Event/Buffer do not conflict |
| S2 | LoadArg id consecutive | Generic | `CcuProgramChecker` (utils) | argId 0,1,2... consecutive |
| S3 | operation reference valid | Generic | `CcuLogicalResourceChecker` (utils) | Referenced resource is registered |
| S4 | IF/ELSE/WHILE matching | Generic | `CompilerContext::Finalize` (utils) | begin/end pairing |
| D1 | Expected output computation | Generic | `CaptureRuntimeExpectation` (utils) | Move data according to srcOffsets/dstOffsets matrix |
| R2 | Runtime execution | Generic | `RuntimeWorld::Launch` (utils) | Polling execution + deadlock precise localization |
| D2 | Byte-by-byte comparison | Generic | `CheckRuntimeOutput` (utils) | Actual output vs expected output |
| DAG | Memory conflict detection | Generic | `CcuDagChecker` (utils) | WAW/WAR/RAR without synchronization conflict |

### 8.2 Removed Checks

The following checks have been removed, indirectly covered by R2 (runtime execution) and D2 (byte-by-byte comparison):

- Algorithm-specific static checks (channel count, full-mesh, mask value, notify index, operation count)
- taskArgs value comparison (self-evident, meaningless)
- taskArgs count/mission binding (covered by R2 out-of-bounds)
- CheckCompleted (duplicate with R2)

### 8.3 Deadlock Precise Localization

When R2 detects a deadlock, it outputs which operation each rank is blocked at:

```
deadlock detected:
  rank 0 blocked at operation[26]: code=10(NOTIFY_WAIT) channel=0x7f.. index=0 mask=0x6 current=0x4
  rank 1 blocked at operation[26]: code=10(NOTIFY_WAIT) channel=0x7f.. index=0 mask=0x6 current=0x4
```

`mask=0x6` (expected) vs `current=0x4` (actual) → precisely locates which notify bit is missing.

---

## 9. Program IR and Primitive Stubs

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

### 9.2 Recording

CCU API calls in the product kernel code are intercepted by `ccu_primitives_stub` and converted into `Operation` appended to `Program::operations[]`:

```
ccu::LoadArg(var, argId)           → Operation{LOAD_ARG}
ccu::Write(ch, dst, src, len)      → Operation{WRITE}
ccu::NotifyWait(ch, idx, mask)     → Operation{NOTIFY_WAIT}
CCU_IF(cond)                       → Operation{IF_BEGIN} + body + Operation{IF_END}
CCU_ELSE                           → Operation{ELSE_BEGIN} + body + Operation{IF_END}
CCU_WHILE(cond)                    → Operation{WHILE_BEGIN} + body + Operation{WHILE_END}
```

### 9.3 RuntimeWorld Execution

Polling scheduling, each round every rank executes one operation:

- `LOAD_ARG` → take value from taskArgs
- `WRITE` → cross-rank memmove
- `NOTIFY_WAIT` → check mask, block if not satisfied
- `IF_BEGIN` condition false → jump to ELSE_BEGIN+1 (with else) or IF_END+1 (without else)
- `ELSE_BEGIN` → jump to IF_END+1 (skip else body)
- `WHILE_BEGIN` condition false → jump to WHILE_END+1
- All completed → SUCCESS
- All blocked → DEADLOCK (precise localization)

---

## 10. Channel Mapping Mechanism

### 10.1 Problem

The ST channel handle is a `SimChannel*` raw pointer. RuntimeWorld/checker needs to look up localRank/remoteRank from the handle.

### 10.2 Solution

The framework provides a unified `RegisterChannelMap`:

```cpp
// ccu_kernel_proxy.h
class RegisterManager {
    static void RegisterChannelMap(ChannelHandle handle, uint32_t localRank, uint32_t remoteRank);
    static bool GetChannelRanks(ChannelHandle handle, uint32_t &localRank, uint32_t &remoteRank);
};

// ChannelRemoteRank/ChannelLocalRank first looks up channelMap, falls back to bit arithmetic
```

`ChannelCommCreate` registers automatically:

```cpp
channelList[i] = reinterpret_cast<ChannelHandle>(channel.get());
RegisterManager::RegisterChannelMap(channelList[i], locRank, rmtRank);
```

Adapter uses the real product handle directly:

```cpp
// ConvertAllToAllArg
for (uint32_t i = 0; i < src->channelCount; ++i) {
    config->channels.push_back(src->channels[i]);  // real handle, no derivation
}
```

---

## 11. Multi-threaded Execution Model

### 11.1 Design

In `VerifyScenario`, each rank runs a `std::thread` that concurrently executes `RegisterKernelForRank`:

```cpp
std::vector<std::thread> threads;
for (uint32_t rank = 0; rank < rankSize; ++rank) {
    threads.emplace_back([this, rank, ...]() {
        handles[rank] = RegisterKernelForRank(rank, ...);
    });
}
for (auto &thread : threads) { thread.join(); }
```

### 11.2 Unification with Non-CCU Path

Both CCU and non-CCU use `ChannelCommCreate` (polling pairing method), not `ChannelCommCreateForCcu` (already deleted).

### 11.3 Thread Safety

- `RegisterManager`: `mutex_` protects `kernels_`/`instances_`, `channelMapMutex_` protects `channelMap_`
- `GetLastCompiledGroup`: uses `thread_local` to avoid multi-rank overwrite
- `HcclCommQueryCcuIns`: returns per-rank instance handle to avoid instance state conflict
- `SimChannelExchangeHandler`: already has `mutex_`
- `SimWorld`: Init once, no modification after; concurrent read is safe

### 11.4 Lifecycle

- `SimWorld::Init` called once at the beginning of `VerifyScenario`
- `CleanupHostEnv` only deletes `comm` (does not Deinit SimWorld)
- `SimWorld::Deinit` called once at the end of `VerifyScenario`

---

## 12. Registration Mechanism

### 12.1 Capture Registration

During adapter static initialization, kernel name → capture function + arg converter is registered:

```cpp
bool g_registered = []() {
    RegisterCaptureFunction("CcuKernelAlltoAllMesh1D",
        CaptureAllToAllMesh1DKernel,    // record IR
        ConvertAllToAllArg,             // product arg → ST config
        [](void *p) { delete ...; });   // release
    return true;
}();
```

`HcommCcuKernelRegister` looks up the registry by `kernelFuncName` and automatically calls capture.

### 12.2 Variant Registration

The primitive-level class provides `RegisterVariant`, and the algorithm-level builder registers:

```cpp
CcuStAllToAll::RegisterVariant("CcuAlltoAllMesh1D",
    [](scenario, handles) -> ScenarioData {
        return BuildMesh1DScenario(scenario, handles);
    });
```

`BuildScenarioData` looks up the registry by `expectedAlgName`.

---

## 13. Guide to Adding New Algorithm Variants

Taking `CcuAlltoAllMesh2Die` as an example, adding a new variant only modifies the testcase, not utils:

### 13.1 Create New Adapter

```
testcase/mc2_ccu/all_to_all/
├── ccu_all_to_all_mesh2die_adapter.h/.cc   ← new
```

Need to implement:
1. `AllToAllMesh2DieKernelConfig` / `AllToAllMesh2DieLaunchConfig` structs
2. `CaptureMesh2DieKernel` — record IR
3. `ConvertMesh2DieArg` — product arg → ST config
4. `BuildMesh2DieScenario` — construct ScenarioData (fill srcOffsets/dstOffsets matrix)
5. Static registration:
   ```cpp
   RegisterCaptureFunction("CcuKernelAlltoAllMesh2Die", capture, convert, deleter);
   CcuStAllToAll::RegisterVariant("CcuAlltoAllMesh2Die", builder);
   ```

### 13.2 Test Case

```cpp
TEST_F(CcuStAllToAll, Mesh2Die_4Rank_Fp16_64B)
{
    TopoMeta topo{{{{0, 1, 8, 9}}}};
    CcuStScenario scenario = MakeScenario(topo, HCCL_DATA_TYPE_FP16, 64);
    scenario.expectedAlgName = "CcuAlltoAllMesh2Die";
    VerifyScenario(scenario);
}
```

### 13.3 What Does Not Need to Change

- `ccu_all_to_all.h/.cc` — primitive level, unchanged
- `ccu_fixture.h/.cc` — framework, unchanged
- All utils files — unchanged

---

## 14. Guide to Adding New CCU Hardware Primitives

CCU hardware primitives are low-level operations such as `ccu::Write`, `ccu::Notify`, `ccu::LocalCopy`. If the product code adds a new CCU hardware primitive (e.g., `ccu::Scatter`), the framework (utils) needs to be extended:

### 14.1 Framework Layer Changes

| File | Change |
|------|------|
| `ccu_st_types.h` | Add new value to `OpCode` (e.g., `SCATTER`) |
| `ccu_primitives_stub.h` | Add `Result Scatter(...)` declaration |
| `ccu_primitives_stub.cc` | Implement stub: intercept call → append `Operation{SCATTER}` to Program IR |
| `ccu_sim_world.cc` | Add `case OpCode::SCATTER` execution logic to `ExecuteOne` |
| `ccu_checker.cc` | Add `case OpCode::SCATTER` resource check to `CcuLogicalResourceChecker` |

All communication primitives and algorithm variants automatically gain support for the new CCU hardware primitive.

---

## 15. Guide to Adding New Communication Primitives

Communication primitives are AllToAll, AllGather, AllReduce, ReduceScatter, Broadcast, etc. Adding a new communication primitive requires modifying the testcase (primitive layer + algorithm layer), not the utils framework.

Taking AllGather as an example:

### 15.1 Primitive Layer (testcase)

Create new `testcase/mc2_ccu/all_gather/`:

```
all_gather/
├── all_gather_testcase.cpp              ← test cases
├── ccu_all_gather.h/.cc                 ← primitive-level: CcuStAllGather + PrepareOpParam + registry
├── ccu_all_gather_mesh1d_adapter.h/.cc  ← algorithm-level: config + capture + builder
└── shim/
    ├── ccu_primitives_dl.hpp
    └── stubs.cc
```

Primitive-level implementation:
- `PrepareOpParam` — fill `opParam.DataDes.count` / `opParam.DataDes.dataType`
- `RegisterVariant` — register algorithm variants

### 15.2 Algorithm Layer (testcase)

Algorithm-level implementation:
- `BuildMesh1DScenario` — fill the `srcOffsets`/`dstOffsets` matrix of `ScenarioData`
- `CaptureAllGatherMesh1DKernel` — record IR
- `ConvertAllGatherArg` — product arg → ST config
- Static registration `RegisterCaptureFunction` + `RegisterVariant`

### 15.3 Framework Layer (utils)

When adding a new communication primitive, utils needs changes:

| File | Change |
|------|------|
| `testcase/mc2_ccu/CMakeLists.txt` | Add AllGather product sources + include paths + testcase source files |
| `utils/src/ccu_scenario/ccu_scenario.h` | `CcuStScenario`'s `opType` supports new primitive enum value (e.g., `HCCL_OP_ALLGATHER`) |
| `utils/src/hccl_proxy/stubs.cc` | If the new primitive has additional product functions that need stubs, add stub |

The framework logic of utils (`VerifyScenario`, `CcuProgramChecker`, `RuntimeWorld`, `CaptureRuntimeExpectation`, `CheckRuntimeOutput`) does not need to change — they are primitive-agnostic generic flows.

### 15.4 Expected Output Computation

`CaptureRuntimeExpectation` (utils generic) moves data according to the matrix in `ScenarioData`, and does not need to be implemented by each communication primitive. If the communication primitive has reduce semantics (e.g., ReduceScatter), set `ScenarioData::reduceOp`, and `ApplyReduce` performs per-element reduction by dataType (supports INT8/FP16/BF16/INT32/FP32 × SUM/PROD/MAX/MIN).

---

## 16. Loop Engine Simulation

### 16.1 Background

CCU hardware's `GroupCopy` is not a simple single `memmove`, but a two-level nested loop engine that moves data in chunks. ST framework's `RuntimeWorld` must expand and execute according to the hardware Loop Engine semantics when encountering `GROUP_COPY` IR, otherwise it cannot correctly verify `goSize` chunking parameters and address offsets.

### 16.2 Hardware Loop Engine Model

Referenced from hcomm repo `CcuSimulator` (`test/hccl_vm/src/plugin/runner/ccu_executor/ccu_simulator.cc`). The Loop Engine consists of two nested levels:

```
LoopGroup (outer expansion)
 ├─ parallelParam (Xn register): repeatNum, repeatLoopIndex, totalLoopNum
 ├─ offsetParam (Xm register): gsaOffset, msOffset, ckeOffset
 └─ Loop[] (inner iteration)
     └─ loopParam (Xn register): iterNum, gsaOffsetLoop
         └─ LoopBody (IR sequence): LocalCopy(src→buf) + EventWait + LocalCopy(buf→dst) + EventWait
```

#### Parameter Parsing

**parallelParam** (from `GetParallelParam`):

| Field | Bits | Description |
|-------|------|-------------|
| repeatNum | [61:55] | Outer expansion count (executed repeatNum+1 times) |
| repeatLoopIndex | [54:48] | Which Loop to start from during expansion |
| totalLoopNum | [47:41] | Total number of Loops |

**offsetParam** (from `GetOffsetParam`):

| Field | Bits | Description |
|-------|------|-------------|
| gsaOffset | [52:21] | GSA address step per outer expansion |
| msOffset | [20:10] | MS (buffer) index step per outer expansion |
| ckeOffset | [9:0] | CKE index step per outer expansion |

**loopParam** (from `GetLoopParam`):

| Field | Bits | Description |
|-------|------|-------------|
| loopCtxId | [52:45] | LoopEngine ID |
| gsaOffsetLoop | [44:13] | GSA address step per inner iteration |
| iterNum | [12:0] | Inner iteration count |

#### Address Offset Formula

Address offset for each LoopBody execution (from `CcuSimulator::GetLoopGsaAddrOffset`):

```
addrOffset = gsaOffset × loopExtendIndex + gsaOffsetLoop × curLoopRound
```

- `loopExtendIndex`: outer expansion index (0 ~ repeatNum)
- `curLoopRound`: inner iteration index (0 ~ iterNum-1)

#### Execution Flow

Referenced from `CcuSimulator::ExecuteLoopGroup` (ccu_simulator.cc:235):

```
for (idx = 0; idx <= repeatNum; idx++) {
    loopStartIdx = (idx == 0) ? 0 : repeatLoopIndex
    for (li = loopStartIdx; li < totalLoopNum; li++) {
        loopExtendIndex = idx
        // Execute Loop instruction → LoopExecutor::RunV1()
        //   Parse loopParam: iterNum, gsaOffsetLoop
        //   for (round = 0; round < iterNum; round++) {
        //       curLoopRound = round
        //       Execute LoopBody IR:
        //         LocalCopy(buf, src, len): src += addrOffset, msId += msOffset × idx
        //         EventWait: blocking wait for event mask
        //         LocalCopy(dst, buf, len): dst += addrOffset, msId += msOffset × idx
        //         EventWait: blocking wait for event mask
        //   }
    }
}
```

Key rules:
- When `idx=0`, execute all `totalLoopNum` Loops starting from Loop 0
- When `idx>0`, start from `repeatLoopIndex`
- Each Loop iterates `iterNum` times internally
- Each LoopBody does `src→buf→dst` two transfers (buffer relay)
- `EventWait` is blocking, not skipped

### 16.3 ST RuntimeWorld Implementation

The `GROUP_COPY` case in `ccu_sim_world.cc` implements the above hardware model:

```cpp
for (idx = 0; idx <= repeatNum; idx++) {
    loopStart = (idx == 0) ? 0 : repeatLoopIndex;
    for (li = loopStart; li < totalLoopNum; li++) {
        // Parse loopParam → iterNum, gsaOffsetLoop
        for (round = 0; round < iterNum; round++) {
            addrOffset = gsaOffsetGroup * idx + gsaOffsetLoop * round;
            msIdx = msOffset * idx;
            // Iterate LoopBody IR:
            //   LOCAL_COPY: src→buffer or buffer→dst (distinguished by handle)
            //     src/dst address += addrOffset
            //     buffer indexed by handle + msIdx (simulating MS offset)
            //   EVENT_WAIT: blocking wait (if ((value & mask) != mask) return false)
        }
    }
}
```

Hardware-to-ST mapping:

| Hardware | ST Implementation |
|----------|-------------------|
| `ExecuteLoopGroup` outer expansion | `for (idx = 0; idx <= repeatNum; idx++)` |
| `loopStartIdx = (idx == 0) ? 0 : repeatLoopIndex` | `loopStart = (idx == 0) ? 0 : repeatLoopIndex` |
| `ExecuteLoop` inner iteration | `for (round = 0; round < iterNum; round++)` |
| `GetLoopGsaAddrOffset()` | `addrOffset = gsaOffsetGroup * idx + gsaOffsetLoop * round` |
| `GetLoopMsOffset()` | `msIdx = msOffset * idx` |
| `TransMemToMS` / `TransMSToMem` | `state.buffers[handle + msIdx]` + `memmove` |
| `WaitCkeProcess` blocking wait | `if ((value & mask) != mask) return false` |

### 16.4 GroupCopy Compilation Phase

Product code `GroupCopy` (`ccu_kernel_alg_base.cc:716`) is called during kernel registration, generating IR through CCU primitive stubs:

1. `CreateMultiOpCopy` → `AllocGoResource(loopCount=8, msPerLoop=8)` → `memSlice=32768`
   - Allocates `loopCount` Events + `loopCount * msInterleave` CcuBuffers
2. `CreateMultiOpCopy` → Creates 2 `Func` (lambda) + 2 `Loop`
   - Each `Loop` constructor calls `func.RunBody()`, lambda's `LocalCopy/EventWait` generates IR
   - IR stored in `program.loopBodies[loopHandle]` (via `CompilerContext::LoopBodyEnter/Exit`)
3. `GroupCopy` → `CCU_IF(goSize.addrOffset != 0)` → `LoopGroup` construction
   - `LoopGroup` calls `CreateLoopGroup`, stores in `GroupCopyDescriptor`
   - Appends one `GROUP_COPY` op to main operation stream (`op.index` points to descriptor)

### 16.5 goSize Computation

`CalGoSize(size, config)` computes chunking parameters based on `size` and `LoopGroupConfig`:

```
loopSize = config.loopCount × config.memSlice
m = size / loopSize       // full outer iterations
n = (size % loopSize) / config.memSlice  // remaining memSlice count
p = size % config.memSlice  // tail block
```

Returns `{offset, loopIterNum, loopExtendNum, tailSize}`:

| Field | Description | GroupCopy Usage |
|-------|-------------|-----------------|
| offset (=addrOffset) | m × loopSize | First LoopGroup total transfer size |
| loopIterNum (=loopParam) | m | First LoopGroup inner iteration count |
| loopExtendNum (=parallelParam) | GetParallelParam(n-1, ...) | Second LoopGroup outer expansion param |
| tailSize (=residual) | p or memSlice | Second LoopGroup tail block size |

**Important**: `CalGoSize`'s `config.memSlice` must match `AllocGoResource`'s `memSlice` (both `LOCAL_COPY_MS_PER_LOOP × CCU_MS_SIZE = 32768`), otherwise goSize parameters won't match loop parameters.

Referenced from hccl repo AllToAll template (`ccu_temp_all_to_all_v_mesh_1D.cc:193`):
```cpp
config.loopCount = CCU_MS_INTERLEAVE;              // 8
config.memSlice = CCU_MS_INTERLEAVE * CCU_MS_SIZE; // 32768
auto goSize = CalGoSize(sliceSize, config, GetCcuVersion());
```

ST test case must use the same config:
```cpp
LoopGroupConfig goConfig{};
goConfig.msInterleave = CCU_MS_INTERLEAVE;
goConfig.loopCount = CCU_MS_LOCAL_COPY_LOOP_COUNT;
goConfig.memSlice = LOCAL_COPY_MS_PER_LOOP * CCU_MS_SIZE;
auto goSize = CalGoSize(sliceSize, goConfig);
```

### 16.6 1MB Data Transfer Example

With `sliceSize=1MB=1048576`:

**CalGoSize computation** (`memSlice=32768, loopCount=8`):
```
loopSize = 8 × 32768 = 262144
m = 1048576 / 262144 = 4
n = 0, p = 0
goSize = {addrOffset=1048576, loopParam=4, parallelParam=0, residual=0}
```

**First LoopGroup execution** (`goSize.addrOffset != 0`):
- `loopParam = GetLoopParam(0, 32768×8, 0) + 4` → `gsaOffsetLoop=262144, iterNum=4`
- `paraCfg = GetParallelParam(7, 0, 1)` → `repeatNum=7, repeatLoopIndex=0, totalLoopNum=1`
- `offsetCfg = GetOffsetParam(32768, 8, 1)` → `gsaOffsetGroup=32768, msOffset=8`

**Expansion**:
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

Total transfer = 8 × 4 × 32768 = 1048576 = 1MB ✓

Each transfer: `src → buffer[msId] → dst`, buffer index = `msOffset × idx = 8 × idx`.

### 16.7 100B Data Transfer Example

With `sliceSize=100`:

**CalGoSize computation**:
```
loopSize = 8 × 32768 = 262144
m = 100 / 262144 = 0
n = 100 / 32768 = 0
p = 100
goSize = {addrOffset=0, loopParam=0, parallelParam=GetParallelParam(0,0,1), residual=100}
```

**Second LoopGroup execution** (`goSize.parallelParam != 0`):
- `paraCfg = GetParallelParam(0, 0, 1)` → `repeatNum=0, repeatLoopIndex=0, totalLoopNum=1`
- `loopParam = GetLoopParam(0, 0, 1)` → `gsaOffsetLoop=0, iterNum=1`
- `offsetCfg = GetOffsetParam(32768, 8, 1)` → `gsaOffsetGroup=32768, msOffset=8`
- `loopLen[0] = goSize.residual = 100`

**Expansion**:
```
idx=0: loop 0, round 0 → addrOffset=0, transfer 100 bytes
```

Total transfer = 1 × 1 × 100 = 100B ✓

---

## 17. CCU Primitive Simulation vs hcomm

This section compares each CCU primitive's simulation logic between the ST framework (asc-devkit) and hcomm's `CcuSimulator`, confirming semantic consistency.

Reference code:
- asc-devkit: `ccu_primitives_stub.h/.cc` (IR generation), `ccu_sim_world.cc` (execution)
- hcomm: `ccu_primitives.hpp` (primitives), `ccu_kernel.cc` (Rep generation), `ccu_simulator.cc` + `XxxExecutor` (execution)

### 17.1 Comparison Summary

| # | Primitive | Consistency | Test Case | Key Difference |
|---|-----------|-------------|-----------|----------------|
| 1 | Variable ops | ✅ Consistent | Variable_AssignImm, Variable_AssignVar_And_Add | Flat OpCode vs polymorphic Rep + deferred objects |
| 2 | Address ops | ✅ Consistent | Address_AssignAndAdd | hcomm has 4 extra overloads (addr=imm, addr+addr, etc.) |
| 3 | LoadArg | ✅ Consistent | LoadArg_MultipleArgs | Same argId split logic (13 slot/SQE_ARGS_LEN) |
| 4 | Load/Store | ✅ Consistent | LoadStore_RoundTrip | hcomm supports immediate address; asc-devkit only Variable |
| 5 | WriteVariableWithNotify | ✅ Consistent | WriteVariableWithNotify_2Rank | Direct map write vs SyncXn microcode |
| 6 | NotifyWait/Record | ✅ Consistent | NotifyRecordWait_2Rank | notifies map vs CKE signal |
| 7 | EventRecord/Wait | ✅ Consistent | EventRecordWait_SingleRank | events map vs CKE signal |
| 8 | Write (cross-rank) | ✅ Consistent | Write_CrossRank_256B | hcomm supports reduce; asc-devkit pure copy |
| 9 | LocalCopy (Mem→Mem) | ✅ Consistent | LocalCopy_MemToMem_256B | Direct memmove + set event |
| 10 | LocalCopy (Buffer↔Mem) | ✅ Consistent | LocalCopy_BufferRelay_256B | state.buffers vs MS array |
| 11 | Write with CcuBuffer | ✅ Consistent | Write_FromBuffer_2Rank_256B | Generic path supports buffer indexing |
| 12 | CCU_IF/ELSE | ✅ Consistent | IfElse_BranchSelect, IfElse_ElseBranch | peerIndex jump vs Label+Jump microcode |
| 13 | CCU_WHILE | ✅ Consistent | While_Loop_Sum | Same as IF, peerIndex back-jump |
| 14 | GetResByChannel | ✅ Consistent | GetResByChannel_2Rank | Logical map vs Xn register binding |
| 15 | GroupCopy + Loop Engine | ✅ Consistent | 24 AllToAll cases | See [§16](#16-loop-engine-simulation) |

### 17.2 Engineering Differences Summary

| Dimension | asc-devkit | hcomm |
|-----------|-----------|-------|
| IR type | Flat `Operation` array + `OpCode` enum | Polymorphic `CcuRepBase` tree → `CcuInstr` microcode |
| Execution | switch-case interpreter + round-robin | Factory pattern Executor + state machine |
| Resource model | `RankState` map (variables/addresses/events/buffers) | `CcuResourceManager` (XN/GSA/CKE/MS arrays) |
| Multi-rank | Built-in round-robin + deadlock detection | Single rank, multi-rank by upper layer |
| Reduce | Simplified to Copy | Actual FP16/FP32/BF16 computation |
| Validation | Four layers (compile→resource→runtime→DAG+semantics) | Basic execution check + profiling |

**Conclusion**: All 15 CCU primitive simulation semantics are consistent with hcomm `CcuSimulator`. Differences are only in engineering implementation (flat IR vs tree Rep, switch-case vs factory dispatch, map vs array), not affecting verification correctness.

---

## 18. Boundary Declaration

### 16.1 Coverage

- Real selector algorithm selection logic
- Real template.CalcRes constructing CcuKernelInfo
- Real channel allocation (HcclChannelAcquire)
- Real collective kernel CCU API call patterns
- Program IR structure checks (resource uniqueness, LoadArg consecutiveness, control flow matching)
- RuntimeWorld multi-rank execution + deadlock precise localization
- DAG memory conflict detection
- Independent expected byte result comparison

### 16.2 Non-Coverage

- KFC server kernel (queue/CKE/wrap/finalize)
- InheritKfcServerKernelArg (KFC server replacement logic)
- HcommCcuKernelRegister real microcode recording (bridged to RegisterManager, does not record real microcode)
- Real Translator, microcode, CcuInstr
- Real XN/GSA/CKE/MS physical resource allocation
- Real multi-Mission/multi-LoopEngine concurrency
- GroupCopy Loop Engine two-level nested expansion (see [§16](#16-loop-engine-simulation))
- GroupCopy loop body buffer relay (src→MS→dst)
- Driver, hardware timing, performance
- Floating-point per-element numerical precision (FP16/BF16 conversion precision, not strict IEEE)
