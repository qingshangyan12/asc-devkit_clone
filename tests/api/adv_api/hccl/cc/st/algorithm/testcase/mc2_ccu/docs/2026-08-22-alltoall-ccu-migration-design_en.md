# Introduction

## Purpose

This document describes the product code design of the AllToAll CCU algorithm migration to the asc-devkit repository. It is intended for asc-devkit developers and code reviewers, explaining the scope of changes, module structure, key flows, and interface design for porting the AllToAll Mesh1D CCU algorithm from the hccl repository to asc-devkit.

This document **does not include** ST test framework design (see `2026-08-20-ccu-st-design_en.md` for ST design).

## Scope

**Includes**:
- AllToAll CCU algorithm Selector implementation
- AllToAll CCU Template (CalcRes resource computation) implementation
- AllToAll CCU Kernel (`CcuAlltoAllMesh1DKernel`) implementation
- AICore v0 prepare parameter changes
- KFC Server support for AllToAll
- Tiling registration
- Build system changes

**Does not include**:
- ST test framework design
- hccl repository changes (hccl already has AllToAll CCU; asc-devkit does alignment porting)

# Overall Description

## Software Overview

### Project Introduction

The asc-devkit repository, as part of the CANN development toolkit, contains the host-side implementation of MC2 communication operators. Previously, asc-devkit supported AllGather and ReduceScatter CCU algorithms. This iteration ports the AllToAll CCU algorithm from the hccl repository to asc-devkit, providing full AllToAll CCU algorithm capability aligned with hccl.

### Product Environment

The CCU algorithm module in asc-devkit is located under `impl/adv_api/detail/hccl/cc/`, as a sub-module of the MC2 communication framework. CCU (Communication Compute Unit) is a dedicated on-chip communication hardware unit, implementing inter-rank communication through CCU primitives (Variable, Address, Write, LocalCopy, GroupCopy, Notify, Event, etc.).

CCU algorithms coexist with existing AICPU algorithms, with execution path selected by `opExecuteConfig`:
- `CCU_MS`: Tries CCU_MS mode first, falls back to CCU_SCHED if not matched
- `CCU_SCHED`: Uses CCU schedule mode, falls back to AICPU if not matched
- `AICPU_TS`: Uses AICPU algorithm

Overall call chain:

```
┌─────────────────────────────────────────────────────────┐
│          MC2 Operator Tiling (algConfig="CcuAlltoAllMesh1D") │
└──────────────────────┬──────────────────────────────────┘
                       ▼
┌──────────────────────────────────────────────────────────┐
│  HcclAllocComResourceByTiling [hccl_mc2.cc:324]          │
│                                                          │
│  Phase 1: Host-side preparation                          │
│  ┌────────────────────────────────────────────────────┐  │
│  │ AllocCcuOpResCtx (device memory allocation)         │  │
│  ├────────────────────────────────────────────────────┤  │
│  │ CcuSelectAlg (algo selection + resource prep + kernel recording)  │  │
│  │  ├─ SelectCcuAlgorithm (selector chooses algo)      │  │
│  │  ├─ FillCcuAlgorithmInfo (set isKfc=true)           │  │
│  │  └─ GetCcuOpParamResCtx                              │  │
│  │      ├─ executor->CalcRes (collective kernel)       │  │
│  │      ├─ ccuKfcExecutor->CalcRes (KFC server replace)│  │
│  │      └─ GetAlgResCcu (channel alloc + kernel reg)   │  │
│  ├────────────────────────────────────────────────────┤  │
│  │ CopyCcuOpParamToDevice (opParam H2D)                │  │
│  └────────────────────────────────────────────────────┘  │
│                                                          │
│  Phase 2: CcuKernelLaunch (launch KFC server)            │
│                                                          │
│  Phase 3: Device-side                                    │
│  KFC server microcode → DispatchKfcSubKernel → CcuAlltoAll│
│  ┌──────────┐  ┌──────────┐  ┌──────────┐               │
│  │ PreSync  │→ │ DoAlltoAll│→ │ PostSync │               │
│  └──────────┘  └──────────┘  └──────────┘               │
│                  ├── loadFromMem=true: LocalCopy+Write    │
│                  └── loadFromMem=false: GroupCopy+Write   │
└──────────────────────────────────────────────────────────┘
```

## Software Functions

1. **Algorithm Selection**: Automatically selects CCU AllToAll algorithm variant based on topology (Mesh1D / MESH_1D_CLOS), rank count, and data size
2. **Resource Computation**: Computes channel requests, kernel parameters, and kernel count
3. **Kernel Recording**: `CcuAlltoAllMesh1DKernel` records the AllToAll data transfer IR sequence via CCU primitives, supporting both `loadFromMem` and `!loadFromMem` (GroupCopy) paths
4. **Chunked Transfer**: Data larger than 256MB is automatically chunked (WriteChunked + CCU_WHILE loop)
5. **KFC Server Compatibility**: Supports AllToAll Kernel inheritance and dispatch in KFC Server mode

## Design Constraints

- **C++ Standard**: C++14, aligned with product code requirements
- **Compilation Macros**: `MC2_CLIENT_ENABLE_CCU` controls whether CCU code is compiled; `AICPU_COMPILE` distinguishes AICPU and CCU compilation paths
- **CCU Primitive Constraint**: `1 << rankSize` used for event mask, `allBit` is `uint16_t`, requires rankSize ≤ 16
- **Inplace Limitation**: CCU schedule mode does not support inplace alltoall (overlapping input/output addresses)
- **Topology Limitation**: Does not support `level2Ubg` and `topoLevelNums >= 3` multi-level topologies
- **Platform Limitation**: `IsDevType960()` currently returns false; 960 chip special path not enabled

## Assumptions and Dependencies

- Depends on `ccu_kernel_alg_base.cc` for `GroupCopy`, `CreateMultiOpCopy` and other base functions
- Depends on `ccu_kernel_utils.cc` for `CalGoSize`, `GetLoopParam`, `GetOffsetParam`, `DataTypeSizeGet` utility functions
- Depends on `ccu_alg_template_base.h` for `CcuAlgTemplateBase` base class
- Depends on `ccu_kernel_kfc_server.cc` for KFC Server Kernel dispatch framework
- Depends on `hccl_ccu_v0_prepare.h` for AICore-side parameter preparation (device code)
- Depends on `REGISTERED_CCU_ALGORITHMS` registry in `hccl_tiling.cpp`

# AllToAll CCU Algorithm Migration Requirements Analysis & Design

## Overview

This migration adds a complete AllToAll CCU algorithm implementation to asc-devkit, covering the following modules:

| Module | File | Description |
|--------|------|-------------|
| Selector | `alltoall_auto_selector.cc/.h` | New `SelectCcuScheduleAlgo` and sub-methods |
| Template | `ccu_temp_all_to_all_mesh_1D.cc/.h` | New `CcuTempAlltoAllMesh1D` class |
| Kernel | `ccu_kernel_all_to_all_mesh1d.cc/.h` | New `CcuAlltoAllMesh1DKernel` function |
| KFC Server | `ccu_temp_kfc_server.cc` / `ccu_kernel_kfc_server.cc` | New AllToAll support |
| AICore v0 | `hccl_ccu_v0_prepare.h` | Fix `writeLoopCount` computation |
| Tiling | `hccl_tiling.cpp` | Register `CcuAlltoAllMesh1D` |
| Resource | `hccl_alloc_ctx_res.cc/.h` | New AllToAll opType support |
| Executor | `ins_v2_all_to_all_v_sole_executor.cc` | Register CCU executor |
| CMake | Multiple CMakeLists.txt | New build targets |

## Functional Requirements

### Functional Requirement 1: CCU Algorithm Selection

1. Introduction

`AlltoAllAutoSelector` adds a `SelectCcuScheduleAlgo` method, invoked when `opExecuteConfig` is `CCU_SCHED` (under `CCU_MS` mode, AllToAll does not override `SelectCcuMsAlgo`, automatically falling back to `CCU_SCHED`), selecting the appropriate CCU AllToAll algorithm variant based on topology and data size.

2. Input

- `topoInfo`: Topology information (level0Topo, level0MeshType, userRankSize, topoLevelNums, level2Ubg, etc.)
- `opParam`: Operation parameters (all2AllVDataDes.sendCounts, all2AllDataDes.sendType, input/output addresses)
- `configAlgMap`: Algorithm configuration map (currently unused, reserved)

3. Processing

Selection flow:

```
SelectCcuScheduleAlgo
├── Check level2Ubg → not supported, return NOT_MATCH
├── Check topoLevelNums >= 3 → not supported, return NOT_MATCH
├── Check inplace (input/output overlap) → not supported, return NOT_MATCH
├── topoLevelNums > 1 → SelectMultiLevelAlgo
│   ├── MESH_1D + rankSize <= 64 → "CcuAllToAllMesh1D2Die"
│   └── Other → NOT_MATCH
└── topoLevelNums <= 1 → SelectSingleLevelAlgo
    ├── MESH_1D
    │   ├── TWO_DIE_REGULAR → "CcuAllToAllMesh2Die"
    │   ├── TWO_DIE_NOT_REGULAR → NOT_MATCH
    │   ├── 960 + dataSize > 16MB + two-level network → "CcuAllToAllSoleMeshScheConcur"
    │   └── Default → "CcuAlltoAllMesh1D"
    └── MESH_1D_CLOS → SelectMesh1DClosAlgo
        ├── pcieMix + fully-connected Mesh1D → "CcuAllToAllMesh1D"
        ├── meshNum == closNum + rankSize <= 4 + dataSize > 512B → "CcuAllToAllMesh1DConcurrent"
        └── Default → "CcuAlltoAllMesh1DMultiJetty"
```

**Data size calculation**: `CalcDataSize = sendCounts[0] * dataTypeSize * userRankSize`

4. Output

- `selectAlgName`: Selected algorithm name string
- Returns `SelectorStatus::MATCH` or `SelectorStatus::NOT_MATCH`

### Functional Requirement 2: Resource Computation (CalcRes)

1. Introduction

`CcuTempAlltoAllMesh1D::CalcRes` computes the resources needed by the AllToAll CCU algorithm, including channel requests, kernel info, and kernel arguments.

2. Input

- `comm`: Communication domain
- `param`: Operation parameters
- `topoInfo`: Topology information
- `subCommRanks`: Sub-communication domain rank lists

3. Processing

```
CalcRes
├── Set resource request: notifyNumOnMainThread=0, slaveThreadNum=0, ccuKernelNum=[1]
├── Compute typeSize_ = DataTypeSizeGet(sendType)
├── Construct CcuKernelInfo
│   ├── kernelFuncName = "CcuKernelAlltoAllMesh1D"
│   ├── kernelFunc = CcuAlltoAllMesh1DKernel
│   └── channels = CalcChannelRequestMesh1D(...)
├── Construct CcuKernelArgAlltoAllMesh1D
│   ├── rankSize = subCommRanks[0].size()
│   ├── rankId = mySubCommRank_
│   ├── loadFromMem = param.isMc2
│   ├── opParam = param
│   └── subCommRanks = subCommRanks_
└── Fill resourceRequest.ccuKernelInfos
```

**Key parameter `loadFromMem`** (`loadFromMem = param.isMc2`):
- `isMc2=false` (default): `loadFromMem=false`, local rank data via GroupCopy path
- `isMc2=true`: `loadFromMem=true`, local rank data via LocalCopy path
- KFC Server mode: `InheritKfcServerKernelArg` sets `loadFromMem=false`, uses GroupCopy path

4. Output

- `resourceRequest`: Resource request (ccuKernelNum, ccuKernelInfos)

### Functional Requirement 3: Kernel Recording

1. Introduction

`CcuAlltoAllMesh1DKernel` is the core kernel function of the AllToAll CCU algorithm, implementing inter-rank AllToAll data transfer via CCU primitives.

2. Input

11 `ccu::Variable` parameters + 1 `CcuKernelArg`:

| Index | Parameter | Description | Source |
|-------|-----------|-------------|--------|
| 0 | inputAddr | Input memory base address | AICore prepare xnData_[1] |
| 1 | outputAddr | Output memory base address | AICore prepare xnData_[2] |
| 2 | tokenInfo | Token information | AICore prepare xnData_[0] |
| 3 | sliceSize | Per-slice data size (bytes) | AICore prepare xnData_[3] |
| 4 | srcStride | Source address stride | AICore prepare xnData_[4] |
| 5 | writeLoopCount | Write loop count | AICore prepare xnData_[5] |
| 6 | dstOffset | Destination address offset | AICore prepare xnData_[6] |
| 7 | goSize1 | GroupCopy addrOffset | AICore prepare xnData_[7] |
| 8 | goSize2 | GroupCopy loopParam | AICore prepare xnData_[8] |
| 9 | goSize3 | GroupCopy parallelParam | AICore prepare xnData_[9] |
| 10 | goSize4 | GroupCopy residual | AICore prepare xnData_[10] |

3. Processing

Kernel recording has 5 stages:

```
CcuAlltoAllMesh1DKernel
├── 1. ParseKernelArg — Parse arguments (currently empty, reserved)
├── 2. InitResource — Initialize remote resources
│   ├── Iterate channels, get remote input/output/token variables by rank order
│   └── GetResByChannel<Variable>(channel, XN_ID)
├── 3. LoadArgs — Load local rank parameters
│   ├── Fill local rank input/output/token
│   ├── Set sliceSize/srcStride/writeLoopCount/dstOffset
│   ├── Set goSize (addrOffset/loopParam/parallelParam/residual)
│   └── Compute srcOffset = input[rankId]
├── 4. PreSync → DoAlltoAll → PostSync
│   ├── PreSync: WriteVariableWithNotify to exchange output/token addresses + NotifyWait
│   ├── DoAlltoAll: Data transfer (core)
│   └── PostSync: NotifyRecord + NotifyWait for completion sync
└── 5. Return CCU_SUCCESS
```

**DoAlltoAll core logic**:

```
DoAlltoAll
├── Compute src/dst addresses for each rank
│   ├── dst[peer].addr = output[peer] + dstOffset
│   └── src[rankIdx].addr = srcOffset + rankIdx * srcStride
├── loadFromMem=true path (isMc2=true)
│   ├── Iterate all ranks
│   │   ├── Local rank: LocalCopy(localDst, src[rankId], sliceSize)
│   │   └── Remote rank: WriteChunked(channel, dst[r], src[r])
│   └── EventWait(event, (1<<rankSize)-1) — wait for all ranks
└── loadFromMem=false path (MC2 mode / KFC Server mode, isMc2=false)
    ├── Iterate remote ranks: WriteChunked(channel, dst[r], src[r])
    ├── GroupCopy(ctx, localDst, src[rankId], goSize) — local rank data transfer
    └── EventWait(event, allBit) — wait for all remote ranks
```

**WriteChunked chunked transfer**:

```
WriteChunked
├── chunkSize = 256MB, neg256MB = -256MB
├── loopCount = writeLoopCount (= UINT64_MAX - fullChunkCount)
├── remaining = sliceSize
├── CCU_WHILE(loopCount != UINT64_MAX)
│   ├── Write(channel, dst, src, 256MB) — write one full chunk
│   ├── dst.addr += 256MB, src.addr += 256MB
│   ├── remaining += neg256MB — decrement remaining by 256MB
│   └── loopCount += 1 — increment counter (exits at UINT64_MAX)
├── CCU_IF(remaining != 0)
│   └── Write(channel, dst, src, remaining) — write remaining bytes
└── CCU_ELSE
    └── EventRecord(event, 1<<rankIdx) — no remaining, record event directly
```

4. Output

- Returns `CcuResult::CCU_SUCCESS` or error code
- Recording result: Generates CCU primitive IR sequence (PreSync + DoAlltoAll + PostSync), dispatched to hardware by `CcuKernelLaunch`

### Functional Requirement 4: AICore v0 Parameter Preparation

1. Introduction

`CcuPrepareForAllToAll` in `hccl_ccu_v0_prepare.h` prepares AICore-side parameters, filling the `xnData_` array for kernel use.

2. Input

- `commParam`: Common prepare parameters (count, dataType, sendBuf, recvBuf, strideCount, etc.)

3. Processing

This change fixes the computation of `writeLoopCount` (xnData_[5]):

```
// Before:
xnData_[5] = 0;

// After:
constexpr uint64_t MAX_DMA_TRANS_SIZE = 256ULL * 1024 * 1024;
uint64_t fullChunkCount = sliceSizeAlltoall / MAX_DMA_TRANS_SIZE;
xnData_[5] = UINT64_MAX - fullChunkCount;
```

`writeLoopCount` is set to `UINT64_MAX - fullChunkCount`. In the kernel, `CCU_WHILE(loopCount != UINT64_MAX)` loops `fullChunkCount` times before exiting. Each iteration transfers 256MB; remaining bytes are handled via the `remaining` computation.

4. Output

- `xnData_[0~10]`: 11 parameters for CCU Kernel use

### Functional Requirement 5: KFC Server Support

1. Introduction

In KFC Server mode, the AllToAll Kernel needs to be inherited and dispatched by the KFC Server Kernel.

2. Input

- KFC Server param array

3. Processing

**`ccu_temp_kfc_server.cc` changes**:

- `InheritKfcServerKernelArg`: New AllToAll recognition (`srcName == "CcuKernelAlltoAllMesh1D"`), sets `loadFromMem = false`
- `CcuTempKfcServer::CalcRes`: New AllToAll opType support
- Source kernel validation: New AllToAll recognition

**`ccu_kernel_kfc_server.cc` changes**:

- New `HBM_PARAM_IDX_9` and `HBM_PARAM_IDX_10` constants (AllToAll needs 11 params, more than AllGather/ReduceScatter)
- `DispatchKfcSubKernel` new AllToAll dispatch branch:
  ```
  CCU_IF(opType == HCCL_CMD_ALLTOALL)
  {
      // param[1]=inputAddr, param[2]=outputAddr, ctx.token=tokenInfo,
      // param[3]=sliceSize, param[4]=srcStride, param[5]=writeLoopCount,
      // param[6]=dstOffset, param[7~10]=goSize1~4
      CcuAlltoAllMesh1DKernel(param[1], param[2], ctx.token, param[3], ...,
                              param[10], alltoAllArg);
  }
  ```

4. Output

- AllToAll Kernel correctly dispatched in KFC Server mode

## Non-Functional Requirements

### Maintainability

- Algorithm selection logic centralized in one `SelectCcuScheduleAlgo` method, sub-methods split by topology type, easy to extend with new algorithm variants
- Kernel function split by stages (ParseKernelArg → InitResource → LoadArgs → PreSync → DoAlltoAll → PostSync), clear structure
- New files follow the same directory structure and naming conventions as existing AllGather/ReduceScatter CCU code

### Testability

- Kernel function written via CCU primitives, can be captured and simulated in ST framework via stubs
- `CcuKernelArgAlltoAllMesh1D` struct contains complete parameters, easy to construct for testing

### Portability

- `MC2_CLIENT_ENABLE_CCU` macro controls CCU code compilation, does not affect non-CCU builds
- `AICPU_COMPILE` macro isolates CCU executor registration, prevents AICPU compilation from introducing CCU dependencies

### Platform Requirements

`IsDevType960()` currently returns false; the `CcuAllToAllSoleMeshScheConcur` path for 960 chips is not enabled. Future 960 platform support requires modifying this function.

### Cross-Feature Analysis

AllToAll CCU coexists with AICPU AllToAll in the same Selector, switched by `opExecuteConfig`. They do not affect each other.

## Performance

### Model Compilation Time

This adds approximately 400 lines of product code (kernel + template + selector); compilation increment is negligible.

### OM Size and Load Memory

New `CcuAlltoAllMesh1DKernel` function and `CcuTempAlltoAllMesh1D` class add approximately 20KB to code segment; impact on OM size is negligible.

### Execution Performance

- `WriteChunked` uses 256MB chunks, reducing DMA count for large data
- `GroupCopy` path (loadFromMem=false) uses Loop Engine hardware acceleration for local rank data transfer
- `PreSync`/`PostSync` use CKE signal sync, fixed overhead (proportional to rank count, does not grow with data size)

## Interface Design

### New/Modified Interface Description

| Interface | Type | Description |
|-----------|------|-------------|
| `CcuAlltoAllMesh1DKernel` | New | AllToAll CCU Kernel entry function, 11 Variable params + CcuKernelArg |
| `CcuTempAlltoAllMesh1D` | New | AllToAll CCU Template class, inherits CcuAlgTemplateBase |
| `AlltoAllAutoSelector::SelectCcuScheduleAlgo` | New | CCU schedule algorithm selection method |
| `AlltoAllAutoSelector::SelectMultiLevelAlgo` | New | Multi-level topology algorithm selection |
| `AlltoAllAutoSelector::SelectSingleLevelAlgo` | New | Single-level topology algorithm selection |
| `AlltoAllAutoSelector::SelectMesh1DClosAlgo` | New | Mesh1D-CLOS topology algorithm selection |
| `AlltoAllAutoSelector::CalcDataSize` | New | Data size computation |
| `CcuPrepareForAllToAll` | Modified | AICore v0 parameter preparation, fix writeLoopCount computation |
| `InheritKfcServerKernelArg` | Modified | KFC Server new AllToAll recognition |
| `CcuTempKfcServer::CalcRes` | Modified | KFC Server new AllToAll opType support |
| `DispatchKfcSubKernel` | Modified | KFC Server new AllToAll dispatch |
| `IsCcuKfcSupportedOp` | Modified | New AllToAll opType |
| `PrepareParamForAlltoAll` | Modified | New reduceType setting |

### Interface Checklist

| **Check Item** | **Sub-check Item** | **Involved** |
| ------------ | ------------------------------------------------------------ | :----------- |
| **Interface Description** | Does interface need review | Yes, Kernel function signature needs review |
| **Interface Description** | Need supplementary documentation | Yes, CalGoSize parameter computation needs explanation |
| **Interface Description** | Clear interface prototype, function, return value | Yes |
| **Interface Compatibility** | Interface behavior compatibility | New interface, no compatibility issue |
| **Interface Compatibility** | New interface works on old version | N/A, depends on CCU hardware |
| **Interface Compatibility** | Usage scenario / call sequence constraints | Yes, loadFromMem path selection |
| **Interface Constraint** | Clear error reporting on constraint violation | Yes, inplace check + topo check |
| **Interface Constraint** | Need dedicated test cases | Yes, ST test coverage |

## Software Design

### Key Data Structures

#### CcuKernelArgAlltoAllMesh1D

```cpp
struct CcuKernelArgAlltoAllMesh1D : CcuKernelArgBase {
    // Inherited from CcuKernelArgBase:
    //   ChannelHandle channels[CCU_MAX_RANK_SIZE];  // channel handle array
    //   uint32_t channelCount;                      // channel count
    uint64_t rankSize;                      // Total rank count
    uint32_t rankId;                        // Local rank ID
    bool loadFromMem;                       // Data load path flag
    OpParam opParam;                        // Operation parameters
    std::vector<std::vector<uint32_t>> subCommRanks; // Sub-comm rank lists
};
```

#### AlltoAllMesh1DContext

```cpp
struct AlltoAllMesh1DContext : CcuKernelCtxBase {
    const CcuKernelArgAlltoAllMesh1D* arg;  // Kernel arguments
    std::vector<ccu::Variable> input;       // Per-rank input address variables
    std::vector<ccu::Variable> output;      // Per-rank output address variables
    std::vector<ccu::Variable> token;       // Per-rank token variables
    ccu::Variable sliceSize;                // Per-slice data size
    ccu::Variable srcStride;                // Source stride
    ccu::Variable srcOffset;                // Source offset
    ccu::Variable writeLoopCount;           // Write loop count
    ccu::Variable dstOffset;                // Destination offset
    GroupOpSizeVars goSize;                 // GroupCopy chunking parameters
    ccu::Event event;                       // Completion event
};
```

#### AlgorithmType Enum Extension

```cpp
enum AlgorithmType {
    // ... existing enums ...
    CcuAlltoAllMesh1D = 150,               // New
};
```

### Key Techniques/Algorithms

#### 1. AllToAll Data Transfer Algorithm

In Mesh1D topology, each rank sends data to all other ranks while receiving from all others:

- **Remote rank data**: Written to remote memory via `ccu::Write` across channels
- **Local rank data**:
  - `loadFromMem=true`: `ccu::LocalCopy` local copy
  - `loadFromMem=false`: `GroupCopy` via Loop Engine (hardware acceleration)

#### 2. Chunked Transfer (WriteChunked)

When `sliceSize > 256MB`, data is chunked via `CCU_WHILE` loop:

- `writeLoopCount = UINT64_MAX - (sliceSize / 256MB)`: Loop count encoding
- Each iteration transfers 256MB, `loopCount += 1`, exits at `UINT64_MAX`
- `remaining = sliceSize - fullChunkCount * 256MB`: Remaining bytes, computed via `neg256MB` decrement
- If remaining > 0, one extra `Write`; otherwise `EventRecord`

#### 3. Synchronization Mechanism

- **PreSync**: `WriteVariableWithNotify` exchanges output address and token, `NotifyWait` waits for all remote ranks ready
- **PostSync**: `NotifyRecord` + `NotifyWait` ensures all ranks complete data transfer
- **EventWait**: Waits for all Write/LocalCopy/GroupCopy completions

#### 4. Algorithm Selection Strategy

Hierarchical selection by topology type:
- Single-level Mesh1D → `CcuAlltoAllMesh1D` (default) / `CcuAllToAllMesh2Die` (dual Die)
- Single-level Mesh1D_CLOS → `CcuAllToAllMesh1DConcurrent` / `CcuAlltoAllMesh1DMultiJetty`
- Multi-level → `CcuAllToAllMesh1D2Die`

### Flow Design

#### AllToAll CCU Overall Call Flow

```
MC2 Operator Tiling (algConfig="CcuAlltoAllMesh1D")
    │
    ▼
HcclAllocComResourceByTiling [hccl_mc2.cc:324]
    │
    ├── Phase 1: Host-side preparation
    │   ├── AllocCcuOpResCtx (device memory allocation)
    │   ├── CcuSelectAlg
    │   │   ├── SelectCcuAlgorithm → SelectCcuScheduleAlgo
    │   │   │   ├── level2Ubg? → NOT_MATCH
    │   │   │   ├── topoLevelNums >= 3? → NOT_MATCH
    │   │   │   ├── inplace? → NOT_MATCH
    │   │   │   └── Topology match → "CcuAlltoAllMesh1D"
    │   │   ├── FillCcuAlgorithmInfo (set isKfc=true)
    │   │   └─ GetCcuOpParamResCtx
    │   │       ├── executor->CalcRes → CcuTempAlltoAllMesh1D::CalcRes
    │   │       │   ├── CalcChannelRequestMesh1D → channels
    │   │       │   ├── Construct CcuKernelArgAlltoAllMesh1D
    │   │       │   └── Fill resourceRequest.ccuKernelInfos
    │   │       ├── ccuKfcExecutor->CalcRes (KFC server replaces collective)
    │   │       └─ GetAlgResCcu (channel allocation + kernel recording)
    │   └── CopyCcuOpParamToDevice (opParam H2D)
    │
    ├── Phase 2: CcuKernelLaunch (launch KFC server)
    │
    └── Phase 3: Device-side
        AICore: CcuPrepareForAllToAll
        │ ├── xnData_[0] = token
        │ ├── xnData_[1] = sendBuf + offset
        │ ├── xnData_[2] = recvBuf + offset
        │ ├── xnData_[3] = sliceSize
        │ ├── xnData_[4] = strideSize
        │ ├── xnData_[5] = UINT64_MAX - (sliceSize / 256MB)  ← This fix
        │ ├── xnData_[6] = strideSize * rankId
        │ └── xnData_[7~10] = CalcGoSize(sliceSize, 8, CCU_MEMSLICE_SIZE*8)
        │
        ▼
        KFC server microcode → DispatchKfcSubKernel → CcuAlltoAllMesh1DKernel(param[0~10], arg)
        │
        ├── InitResource: GetResByChannel for remote variables
        ├── LoadArgs: Fill local rank parameters
        ├── PreSync: WriteVariableWithNotify + NotifyWait
        ├── DoAlltoAll:
        │   ├── loadFromMem=true: LocalCopy + WriteChunked + EventWait
        │   └── loadFromMem=false: WriteChunked + GroupCopy + EventWait
        └── PostSync: NotifyRecord + NotifyWait
```

#### loadFromMem Path Selection

```
param.isMc2?
├── true (MC2 mode)
│   └── loadFromMem = true
│       └── DoAlltoAll iterates all ranks:
│           ├── Local rank: LocalCopy(localDst, src, sliceSize)
│           └── Remote rank: WriteChunked(channel, dst, src)
│           Then EventWait for all ranks (including local)
│
└── false (MC2 mode / KFC Server mode)
    └── loadFromMem = false
        └── DoAlltoAll:
            ├── First iterate remote ranks: WriteChunked(channel, dst, src)
            ├── GroupCopy(ctx, localDst, src[rankId], goSize)
            └── EventWait for all remote ranks
```

### Sub-module Modifications

| Sub-module | Modification | Notify MDE |
|------------|-------------|------------|
| hccl/cc/src/ops/all_to_all_v | New CCU selector/template/kernel | Yes |
| hccl/cc/src/ops/kfc_server | New AllToAll KFC Server support | Yes |
| hccl/cc/src/common | New AllToAll opType in resource allocation | Yes |
| hccl/ccu | Fix v0 prepare writeLoopCount | Yes |
| tiling/hccl | Register CCU algorithm name | No |

### Error Handling

#### System Errors

- `InitResource`: Returns `CCU_E_INTERNAL` when channels are empty
- `LoadArgs`: Always returns `CCU_SUCCESS` (no parameter validation currently)
- Chunked transfer has no memory allocation failure path (CCU primitives do not allocate memory)

#### Interface Errors

- `SelectCcuScheduleAlgo`: Returns `NOT_MATCH` for unsupported scenarios (fallback to AICPU)
- `CalcRes`: Returns `HCCL_E_INTERNAL` on channel computation failure
- KFC Server: Returns `HCCL_E_NOT_SUPPORT` for unsupported opType

## Security Check

### Coding Guidelines

Refer to coding guidelines.

### Coding Checklist

| **Check Item** | **Description** | **Involved** |
| -------------------------------------------------- | ------------------------------------------------------------ | ------------ |
| **Resource lifecycle management** | Kernel recording has no persistent resources; CcuKernelArgAlltoAllMesh1D managed via shared_ptr | No |
| **New thread creation** | No new threads created | No |

## Compatibility Check

- **New algorithm**: AllToAll CCU is a new algorithm, does not affect existing AICPU AllToAll behavior
- **Build switch**: `MC2_CLIENT_ENABLE_CCU` controls compilation, does not affect non-CCU builds by default
- **KFC Server**: New AllToAll branch does not affect existing AllGather/ReduceScatter branches
- **AICore v0**: `writeLoopCount` changed from 0 to `UINT64_MAX - fullChunkCount`, fixing correctness of large data chunked transfer

## DT Design

### Test Boundary

- Test entry: Capture `CcuAlltoAllMesh1DKernel` calls via ST framework
- Test exit: Verify correctness of each rank's output memory data
- Stub modules: CCU primitives (Variable/Write/LocalCopy/GroupCopy/Notify/Event)

### Test Design

| **Test Category** | **Key Test Items** | **Test Method** | **Case Type** |
| ------------ | -------------- | ------------ | ------------ |
| **Functional** | loadFromMem=true path | Construct isMc2=true scenario, verify LocalCopy + Write | ST |
| **Functional** | loadFromMem=false path | Construct isMc2=false scenario, verify GroupCopy + Write | ST |
| **Functional** | Chunked transfer boundary | sliceSize = 1B / 256MB-1 / 256MB / 256MB+1 / 512MB | ST |
| **Functional** | GroupCopy chunking boundary | CalGoSize m/n/p boundary values | ST |
| **Functional** | Multi-rank data correctness | 2~8 rank AllToAll data verification | ST |
| **Functional** | Algorithm selection | Mesh1D / Mesh1D_CLOS / multi-level topology | ST |
| **Compatibility** | KFC Server mode | KFC Server inherits AllToAll Kernel | ST |

### Test Framework Design

The ST test framework already has CCU simulation capability, supporting AllToAll CCU algorithm testing. See `2026-08-20-ccu-st-design_en.md`.

## Acceptance Criteria

1. AllToAll CCU algorithm functions correctly under Mesh1D topology, 2~8 rank data transfer results are correct
2. Both `loadFromMem=true` and `loadFromMem=false` paths execute correctly
3. Chunked transfer correctly handles 256MB boundary
4. KFC Server mode correctly dispatches AllToAll execution
5. Algorithm selector correctly matches topology and algorithm variants
6. No compilation warnings, CI passes
