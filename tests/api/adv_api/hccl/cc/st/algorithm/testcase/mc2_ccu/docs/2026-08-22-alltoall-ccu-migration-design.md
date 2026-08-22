# 简介

## 目的

本文档描述 Ascend DevKit（asc-devkit）仓中 AllToAll CCU 算法移植方案的产品代码设计。面向 asc-devkit 仓的开发者和代码检视人员，说明 AllToAll Mesh1D CCU 算法从 hccl 仓移植到 asc-devkit 仓的产品代码改动范围、模块结构、关键流程和接口设计。

本文档**不包含** ST 测试框架的设计内容（ST 设计详见 `2026-08-20-ccu-st-design.md`）。

## 范围

**包含**：
- AllToAll CCU 算法选择器（Selector）实现
- AllToAll CCU Template（CalcRes 资源计算）实现
- AllToAll CCU Kernel（CcuAlltoAllMesh1DKernel）实现
- AICore v0 prepare 参数准备改动
- KFC Server 对 AllToAll 的支持
- Tiling 注册
- 编译构建改动

**不包含**：
- ST 测试框架设计
- hccl 仓侧改动（hccl 仓已有 AllToAll CCU 实现，asc-devkit 仓做对齐移植）

# 总体概述

## 软件概述

### 项目介绍

asc-devkit 仓作为 CANN 开发工具套件，包含 MC2 通信算子的 host 侧实现。此前 asc-devkit 仓已支持 AllGather 和 ReduceScatter 两个 CCU 算法。本次迭代将 AllToAll CCU 算法从 hccl 仓移植到 asc-devkit 仓，使 asc-devkit 仓具备完整的 AllToAll CCU 算法能力，与 hccl 仓保持对齐。

### 产品环境介绍

asc-devkit 仓的 CCU 算法模块位于 `impl/adv_api/detail/hccl/cc/` 目录，是 MC2 通信框架的子模块。CCU（Communication Compute Unit）是芯片内专用的通信硬件单元，通过 CCU 原语（Variable、Address、Write、LocalCopy、GroupCopy、Notify、Event 等）实现 rank 间通信。

整体调用链：

```
aclnn API → MC2 Executor → Selector（算法选择）→ Template（CalcRes 资源计算）→ Kernel 下发
```

CCU 算法与现有 AICPU 算法共存，通过 `opExecuteConfig` 配置选择执行路径：
- `CCU_MS`：优先走 CCU_MS 模式，不匹配则降级到 CCU_SCHED
- `CCU_SCHED`：走 CCU schedule 模式，不匹配则降级到 AICPU
- `AICPU_TS`：走 AICPU 算法

```
┌─────────────────────────────────────────────────────────┐
│                    aclnn AllToAll API                    │
└──────────────────────┬──────────────────────────────────┘
                       ▼
┌──────────────────────────────────────────────────────────┐
│              AlltoAllAutoSelector                         │
│  ┌─────────────┐  ┌──────────────────┐                  │
│  │ SelectAicpu │  │ SelectCcuSchedule │                  │
│  │   Algo      │  │     Algo          │                  │
│  └─────────────┘  └────────┬─────────┘                  │
│                            ▼                              │
│              ┌─────────────────────────┐                 │
│              │  CcuTempAlltoAllMesh1D  │                 │
│              │    (CalcRes)            │                 │
│              └────────────┬────────────┘                 │
└───────────────────────────┼──────────────────────────────┘
                            ▼
┌──────────────────────────────────────────────────────────┐
│           CcuAlltoAllMesh1DKernel                         │
│  ┌──────────┐  ┌──────────┐  ┌──────────┐  ┌──────────┐ │
│  │ PreSync  │→ │ DoAlltoAll│→ │ PostSync │  │ GroupCopy │ │
│  └──────────┘  └──────────┘  └──────────┘  └──────────┘ │
└──────────────────────────────────────────────────────────┘
```

## 软件功能

1. **算法选择**：根据拓扑类型（Mesh1D / MESH_1D_CLOS）、rank 数、数据量大小，自动选择 CCU AllToAll 算法变体
2. **资源计算**：计算 channel 请求、kernel 参数、kernel 数量
3. **Kernel 执行**：通过 CCU 原语实现 AllToAll 数据搬运，支持 `loadFromMem` 和 `!loadFromMem`（GroupCopy）两种路径
4. **分块传输**：大于 256MB 的数据自动分块传输（WriteChunked + CCU_WHILE 循环）
5. **KFC Server 兼容**：支持 KFC Server 模式下 AllToAll Kernel 的继承和分发

## 设计约束

- **C++ 标准**：C++14，对齐产品代码要求
- **编译宏**：`MC2_CLIENT_ENABLE_CCU` 控制是否编译 CCU 代码；`AICPU_COMPILE` 区分 AICPU 和 CCU 编译路径
- **CCU 原语约束**：`1 << rankSize` 用于 event mask，`allBit` 为 `uint16_t`，要求 rankSize ≤ 16
- **inplace 限制**：CCU schedule 模式不支持 inplace alltoall（输入输出地址重叠）
- **拓扑限制**：不支持 `level2Ubg` 和 `topoLevelNums >= 3` 的多级拓扑
- **平台限制**：`IsDevType960()` 当前固定返回 false，960 芯片特殊路径未启用

## 假设和依赖关系

- 依赖 `ccu_kernel_alg_base.cc` 提供 `GroupCopy`、`CreateMultiOpCopy` 等基础函数
- 依赖 `ccu_kernel_utils.cc` 提供 `CalGoSize`、`GetLoopParam`、`GetOffsetParam`、`DataTypeSizeGet` 等工具函数
- 依赖 `ccu_alg_template_base.h` 提供 `CcuAlgTemplateBase` 基类
- 依赖 `ccu_kernel_kfc_server.cc` 提供 KFC Server Kernel 分发框架
- 依赖 `hccl_ccu_v0_prepare.h` 提供 AICore 侧参数准备（device 代码）
- 依赖 `hccl_tiling.cpp` 中 `REGISTERED_CCU_ALGORITHMS` 注册表

# AllToAll CCU 算法移植需求分析&设计

## 整体介绍

本次移植在 asc-devkit 仓中新增 AllToAll CCU 算法的完整实现，覆盖以下模块：

| 模块 | 文件 | 说明 |
|------|------|------|
| Selector | `alltoall_auto_selector.cc/.h` | 新增 `SelectCcuScheduleAlgo` 及子方法 |
| Template | `ccu_temp_all_to_all_mesh_1D.cc/.h` | 新增 `CcuTempAlltoAllMesh1D` 类 |
| Kernel | `ccu_kernel_all_to_all_mesh1d.cc/.h` | 新增 `CcuAlltoAllMesh1DKernel` 函数 |
| KFC Server | `ccu_temp_kfc_server.cc` / `ccu_kernel_kfc_server.cc` | 新增 AllToAll 支持 |
| AICore v0 | `hccl_ccu_v0_prepare.h` | 修复 `writeLoopCount` 计算 |
| Tiling | `hccl_tiling.cpp` | 注册 `CcuAlltoAllMesh1D` |
| 资源分配 | `hccl_alloc_ctx_res.cc/.h` | 新增 AllToAll opType 支持 |
| Executor | `ins_v2_all_to_all_v_sole_executor.cc` | 注册 CCU executor |
| CMake | 多个 CMakeLists.txt | 新增编译目标 |

## 功能需求

### 功能需求1：CCU 算法选择

1. 介绍

`AlltoAllAutoSelector` 新增 `SelectCcuScheduleAlgo` 方法，当 `opExecuteConfig` 为 `CCU_SCHED` 时被调用（`CCU_MS` 模式下 AllToAll 未覆写 `SelectCcuMsAlgo`，自动降级到 `CCU_SCHED`），根据拓扑和数据量选择合适的 CCU AllToAll 算法变体。

2. 输入

- `topoInfo`：拓扑信息（level0Topo、level0MeshType、userRankSize、topoLevelNums、level2Ubg 等）
- `opParam`：操作参数（all2AllVDataDes.sendCounts、all2AllDataDes.sendType、输入输出地址）
- `configAlgMap`：算法配置映射（当前未使用，预留）

3. 处理

选择流程：

```
SelectCcuScheduleAlgo
├── 检查 level2Ubg → 不支持，返回 NOT_MATCH
├── 检查 topoLevelNums >= 3 → 不支持，返回 NOT_MATCH
├── 检查 inplace（输入输出地址重叠）→ 不支持，返回 NOT_MATCH
├── topoLevelNums > 1 → SelectMultiLevelAlgo
│   ├── MESH_1D + rankSize <= 64 → "CcuAllToAllMesh1D2Die"
│   └── 其他 → NOT_MATCH
└── topoLevelNums <= 1 → SelectSingleLevelAlgo
    ├── MESH_1D
    │   ├── TWO_DIE_REGULAR → "CcuAllToAllMesh2Die"
    │   ├── TWO_DIE_NOT_REGULAR → NOT_MATCH
    │   ├── 960 + dataSize > 16MB + 两层网络 → "CcuAllToAllSoleMeshScheConcur"
    │   └── 默认 → "CcuAlltoAllMesh1D"
    └── MESH_1D_CLOS → SelectMesh1DClosAlgo
        ├── pcieMix + 全连接 Mesh1D → "CcuAllToAllMesh1D"
        ├── meshNum == closNum + rankSize <= 4 + dataSize > 512B → "CcuAllToAllMesh1DConcurrent"
        └── 默认 → "CcuAlltoAllMesh1DMultiJetty"
```

**数据量计算**：`CalcDataSize = sendCounts[0] * dataTypeSize * userRankSize`

4. 输出

- `selectAlgName`：选中的算法名称字符串
- 返回 `SelectorStatus::MATCH` 或 `SelectorStatus::NOT_MATCH`

### 功能需求2：资源计算（CalcRes）

1. 介绍

`CcuTempAlltoAllMesh1D::CalcRes` 负责计算 AllToAll CCU 算法所需的资源，包括 channel 请求、kernel 信息和 kernel 参数。

2. 输入

- `comm`：通信域
- `param`：操作参数
- `topoInfo`：拓扑信息
- `subCommRanks`：子通信域 rank 列表

3. 处理

```
CalcRes
├── 设置资源请求：notifyNumOnMainThread=0, slaveThreadNum=0, ccuKernelNum=[1]
├── 计算 typeSize_ = DataTypeSizeGet(sendType)
├── 构造 CcuKernelInfo
│   ├── kernelFuncName = "CcuKernelAlltoAllMesh1D"
│   ├── kernelFunc = CcuAlltoAllMesh1DKernel
│   └── channels = CalcChannelRequestMesh1D(...)
├── 构造 CcuKernelArgAlltoAllMesh1D
│   ├── rankSize = subCommRanks[0].size()
│   ├── rankId = mySubCommRank_
│   ├── loadFromMem = param.isMc2
│   ├── opParam = param
│   └── subCommRanks = subCommRanks_
└── 填充 resourceRequest.ccuKernelInfos
```

**关键参数 `loadFromMem`**：
- `param.isMc2 = true`：MC2 模式，走 `loadFromMem=true` 路径（LocalCopy 本 rank 数据）
- `param.isMc2 = false`：非 MC2 模式，走 `loadFromMem=false` 路径（GroupCopy 本 rank 数据）

4. 输出

- `resourceRequest`：资源请求（ccuKernelNum、ccuKernelInfos）

### 功能需求3：Kernel 执行

1. 介绍

`CcuAlltoAllMesh1DKernel` 是 AllToAll CCU 算法的核心 kernel 函数，通过 CCU 原语实现 rank 间 AllToAll 数据搬运。

2. 输入

11 个 `ccu::Variable` 参数 + 1 个 `CcuKernelArg`：

| 序号 | 参数名 | 含义 | 来源 |
|------|--------|------|------|
| 0 | inputAddr | 输入内存基地址 | AICore prepare xnData_[1] |
| 1 | outputAddr | 输出内存基地址 | AICore prepare xnData_[2] |
| 2 | tokenInfo | token 信息 | AICore prepare xnData_[0] |
| 3 | sliceSize | 每片数据大小（字节） | AICore prepare xnData_[3] |
| 4 | srcStride | 源地址步长 | AICore prepare xnData_[4] |
| 5 | writeLoopCount | 写循环计数 | AICore prepare xnData_[5] |
| 6 | dstOffset | 目标地址偏移 | AICore prepare xnData_[6] |
| 7 | goSize1 | GroupCopy addrOffset | AICore prepare xnData_[7] |
| 8 | goSize2 | GroupCopy loopParam | AICore prepare xnData_[8] |
| 9 | goSize3 | GroupCopy parallelParam | AICore prepare xnData_[9] |
| 10 | goSize4 | GroupCopy residual | AICore prepare xnData_[10] |

3. 处理

Kernel 执行分 5 个阶段：

```
CcuAlltoAllMesh1DKernel
├── 1. ParseKernelArg — 参数解析（当前空实现，预留）
├── 2. InitResource — 初始化远端资源
│   ├── 遍历 channels，按 rank 顺序获取远端 input/output/token 变量
│   └── GetResByChannel<Variable>(channel, XN_ID)
├── 3. LoadArgs — 加载本 rank 参数
│   ├── 填充本 rank 的 input/output/token
│   ├── 设置 sliceSize/srcStride/writeLoopCount/dstOffset
│   ├── 设置 goSize (addrOffset/loopParam/parallelParam/residual)
│   └── 计算 srcOffset = input[rankId]
├── 4. PreSync → DoAlltoAll → PostSync
│   ├── PreSync: WriteVariableWithNotify 交换 output/token 地址 + NotifyWait
│   ├── DoAlltoAll: 数据搬运（核心）
│   └── PostSync: NotifyRecord + NotifyWait 完成同步
└── 5. 返回 CCU_SUCCESS
```

**DoAlltoAll 核心逻辑**：

```
DoAlltoAll
├── 计算每个 rank 的 src/dst 地址
│   ├── dst[peer].addr = output[peer] + dstOffset
│   └── src[rankIdx].addr = srcOffset + rankIdx * srcStride
├── loadFromMem=true 路径（MC2 模式）
│   ├── 遍历所有 rank
│   │   ├── 本 rank: LocalCopy(localDst, src[rankId], sliceSize)
│   │   └── 远端 rank: WriteChunked(channel, dst[r], src[r])
│   └── EventWait(event, (1<<rankSize)-1) — 等待所有 rank 完成
└── loadFromMem=false 路径（非 MC2 / KFC Server 模式）
    ├── 遍历远端 rank: WriteChunked(channel, dst[r], src[r])
    ├── GroupCopy(ctx, localDst, src[rankId], goSize) — 本 rank 数据搬运
    └── EventWait(event, allBit) — 等待所有远端 rank 完成
```

**WriteChunked 分块传输**：

```
WriteChunked
├── chunkSize = 256MB, neg256MB = -256MB
├── loopCount = writeLoopCount (= UINT64_MAX - fullChunkCount)
├── remaining = sliceSize
├── CCU_WHILE(loopCount != UINT64_MAX)
│   ├── Write(channel, dst, src, 256MB) — 写一个完整块
│   ├── dst.addr += 256MB, src.addr += 256MB
│   ├── remaining += neg256MB — 剩余量减 256MB
│   └── loopCount += 1 — 计数器+1（到 UINT64_MAX 时退出）
├── CCU_IF(remaining != 0)
│   └── Write(channel, dst, src, remaining) — 写剩余块
└── CCU_ELSE
    └── EventRecord(event, 1<<rankIdx) — 无剩余，直接记录事件
```

4. 输出

- 返回 `CcuResult::CCU_SUCCESS` 或错误码
- 副作用：远端 rank 内存被写入数据，本地 event 被设置

### 功能需求4：AICore v0 参数准备

1. 介绍

`hccl_ccu_v0_prepare.h` 中 `CcuPrepareForAllToAll` 负责 AICore 侧参数准备，填充 `xnData_` 数组供 kernel 使用。

2. 输入

- `commParam`：通用准备参数（count、dataType、sendBuf、recvBuf、strideCount 等）

3. 处理

本次修改修复了 `writeLoopCount`（xnData_[5]）的计算：

```
// 修改前：
xnData_[5] = 0;

// 修改后：
constexpr uint64_t MAX_DMA_TRANS_SIZE = 256ULL * 1024 * 1024;
uint64_t fullChunkCount = sliceSizeAlltoall / MAX_DMA_TRANS_SIZE;
xnData_[5] = UINT64_MAX - fullChunkCount;
```

`writeLoopCount` 的值为 `UINT64_MAX - fullChunkCount`，kernel 中 `CCU_WHILE(loopCount != UINT64_MAX)` 循环 `fullChunkCount` 次后退出。每轮搬运 256MB，剩余字节通过 `remaining` 计算处理。

4. 输出

- `xnData_[0~10]`：11 个参数供 CCU Kernel 使用

### 功能需求5：KFC Server 支持

1. 介绍

KFC Server 模式下，AllToAll Kernel 需要被 KFC Server Kernel 继承和分发。

2. 输入

- KFC Server 接收到的 param 数组

3. 处理

**`ccu_temp_kfc_server.cc` 改动**：

- `InheritKfcServerKernelArg`：新增 AllToAll 识别（`srcName == "CcuKernelAlltoAllMesh1D"`），设置 `loadFromMem = false`
- `CcuTempKfcServer::CalcRes`：新增 AllToAll opType 支持
- source kernel 校验：新增 AllToAll 识别

**`ccu_kernel_kfc_server.cc` 改动**：

- 新增 `HBM_PARAM_IDX_9` 和 `HBM_PARAM_IDX_10` 常量（AllToAll 需要 11 个参数，比 AllGather/ReduceScatter 多）
- `DispatchKfcSubKernel` 新增 AllToAll 分发分支：
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

4. 输出

- KFC Server 模式下 AllToAll Kernel 被正确分发执行

## 非功能需求

### 可维护性

- 算法选择逻辑集中在一个 `SelectCcuScheduleAlgo` 方法中，子方法按拓扑类型分拆，便于扩展新算法变体
- Kernel 函数按阶段拆分（ParseKernelArg → InitResource → LoadArgs → PreSync → DoAlltoAll → PostSync），结构清晰
- 新增文件与现有 AllGather/ReduceScatter CCU 代码保持相同的目录结构和命名规范

### 可测试性

- Kernel 函数通过 CCU 原语编写，可在 ST 框架中通过 stub 捕获和模拟执行
- `CcuKernelArgAlltoAllMesh1D` 结构体包含完整参数，便于测试构造

### 可移植性

- 编译宏 `MC2_CLIENT_ENABLE_CCU` 控制是否编译 CCU 代码，不影响非 CCU 编译
- `AICPU_COMPILE` 宏隔离 CCU executor 注册，避免 AICPU 编译时引入 CCU 依赖

### 平台化要求

`IsDevType960()` 当前固定返回 false，960 芯片的 `CcuAllToAllSoleMeshScheConcur` 路径未启用。后续 960 平台支持时需要修改此函数。

### 特性交叉分析

AllToAll CCU 与 AICPU AllToAll 共存于同一 Selector，通过 `opExecuteConfig` 切换。两者不互相影响。

## 性能

### 模型编译时长

本次新增约 400 行产品代码（kernel + template + selector），编译增量可忽略。

### OM大小和加载占用内存

新增 `CcuAlltoAllMesh1DKernel` 函数和 `CcuTempAlltoAllMesh1D` 类，代码段增量约 20KB，对 OM 大小影响可忽略。

### 执行性能

- `WriteChunked` 使用 256MB 分块，对大数据量减少 DMA 次数
- `GroupCopy` 路径（loadFromMem=false）使用 Loop Engine 硬件加速本 rank 数据搬运
- `PreSync`/`PostSync` 使用 CKE 信号同步，开销固定（与 rank 数成正比，不随数据量增长）

## 接口设计

### 新增/修改接口描述

| 接口 | 类型 | 说明 |
|------|------|------|
| `CcuAlltoAllMesh1DKernel` | 新增 | AllToAll CCU Kernel 入口函数，11 个 Variable 参数 + CcuKernelArg |
| `CcuTempAlltoAllMesh1D` | 新增 | AllToAll CCU Template 类，继承 CcuAlgTemplateBase |
| `AlltoAllAutoSelector::SelectCcuScheduleAlgo` | 新增 | CCU schedule 算法选择方法 |
| `AlltoAllAutoSelector::SelectMultiLevelAlgo` | 新增 | 多级拓扑算法选择 |
| `AlltoAllAutoSelector::SelectSingleLevelAlgo` | 新增 | 单级拓扑算法选择 |
| `AlltoAllAutoSelector::SelectMesh1DClosAlgo` | 新增 | Mesh1D-CLOS 拓扑算法选择 |
| `AlltoAllAutoSelector::CalcDataSize` | 新增 | 数据量计算 |
| `CcuPrepareForAllToAll` | 修改 | AICore v0 参数准备，修复 writeLoopCount 计算 |
| `InheritKfcServerKernelArg` | 修改 | KFC Server 新增 AllToAll 识别 |
| `CcuTempKfcServer::CalcRes` | 修改 | KFC Server 新增 AllToAll opType 支持 |
| `DispatchKfcSubKernel` | 修改 | KFC Server 新增 AllToAll 分发 |
| `IsCcuKfcSupportedOp` | 修改 | 新增 AllToAll opType |
| `PrepareParamForAlltoAll` | 修改 | 新增 reduceType 设置 |

### 接口检查项

| **检查项** | **子检查项** | **是否涉及** |
| ------------ | ------------------------------------------------------------ | :----------- |
| **接口说明** | 接口是否需要评审 | 是，Kernel 函数签名需评审 |
| **接口说明** | 是否需要补充资料说明 | 是，需补充 CalGoSize 参数计算说明 |
| **接口说明** | 接口文档中是否明确接口原型、功能、返回值 | 是 |
| **接口兼容** | 接口行为兼容性 | 新增接口，无兼容性问题 |
| **接口兼容** | 新接口在老版本上是否能正常工作 | 不适用，依赖 CCU 硬件 |
| **接口兼容** | 接口是否涉及使用场景、调用时序等约束 | 是，loadFromMem 路径选择 |
| **接口约束** | 接口调用不满足约束条件时是否能清晰报错 | 是，inplace 检查 + topo 检查 |
| **接口约束** | 是否需要设计单独的测试用例 | 是，ST 测试覆盖 |

## 软件设计

### 关键数据结构

#### CcuKernelArgAlltoAllMesh1D

```cpp
struct CcuKernelArgAlltoAllMesh1D : CcuKernelArgBase {
    uint64_t rankSize;                      // rank 总数
    uint32_t rankId;                        // 本 rank ID
    bool loadFromMem;                       // 数据加载路径标志
    OpParam opParam;                        // 操作参数
    std::vector<std::vector<uint32_t>> subCommRanks; // 子通信域 rank 列表
};
```

#### AlltoAllMesh1DContext

```cpp
struct AlltoAllMesh1DContext : CcuKernelCtxBase {
    const CcuKernelArgAlltoAllMesh1D* arg;  // kernel 参数
    std::vector<ccu::Variable> input;       // 各 rank 输入地址变量
    std::vector<ccu::Variable> output;      // 各 rank 输出地址变量
    std::vector<ccu::Variable> token;       // 各 rank token 变量
    ccu::Variable sliceSize;                // 每片数据大小
    ccu::Variable srcStride;                // 源步长
    ccu::Variable srcOffset;                // 源偏移
    ccu::Variable writeLoopCount;           // 写循环计数
    ccu::Variable dstOffset;                // 目标偏移
    GroupOpSizeVars goSize;                 // GroupCopy 分块参数
    ccu::Event event;                       // 完成事件
};
```

#### AlgorithmType 枚举扩展

```cpp
enum AlgorithmType {
    // ... 现有枚举 ...
    CcuAlltoAllMesh1D = 150,               // 新增
};
```

### 关键技术/算法

#### 1. AllToAll 数据搬运算法

Mesh1D 拓扑下，每个 rank 向其他所有 rank 发送数据，同时接收其他所有 rank 的数据：

- **远端 rank 数据**：通过 `ccu::Write` 跨 channel 写入远端内存
- **本 rank 数据**：
  - `loadFromMem=true`：`ccu::LocalCopy` 本地拷贝
  - `loadFromMem=false`：`GroupCopy` 通过 Loop Engine 搬运（支持硬件加速）

#### 2. 分块传输（WriteChunked）

当 `sliceSize > 256MB` 时，通过 `CCU_WHILE` 循环分块传输：

- `writeLoopCount = UINT64_MAX - (sliceSize / 256MB)`：循环次数编码
- 每轮搬运 256MB，`loopCount += 1`，到 `UINT64_MAX` 退出
- `remaining = sliceSize - fullChunkCount * 256MB`：剩余字节，通过 `neg256MB` 递减计算
- 剩余字节 > 0 时额外一次 `Write`，否则 `EventRecord`

#### 3. 同步机制

- **PreSync**：`WriteVariableWithNotify` 交换 output 地址和 token，`NotifyWait` 等待所有远端 rank 就绪
- **PostSync**：`NotifyRecord` + `NotifyWait` 确保所有 rank 完成数据搬运
- **EventWait**：等待所有 Write/LocalCopy/GroupCopy 完成

#### 4. 算法选择策略

根据拓扑类型分级选择：
- 单级 Mesh1D → `CcuAlltoAllMesh1D`（默认）/ `CcuAllToAllMesh2Die`（双 Die）
- 单级 Mesh1D_CLOS → `CcuAllToAllMesh1DConcurrent` / `CcuAlltoAllMesh1DMultiJetty`
- 多级 → `CcuAllToAllMesh1D2Die`

### 流程设计

#### AllToAll CCU 整体调用流程

```
aclnn AllToAll
    │
    ▼
Tiling: IsCcuAlgorithmRegistered("CcuAlltoAllMesh1D") ✓
    │
    ▼
Executor: SelectCcuScheduleAlgo
    │ ├── level2Ubg? → NOT_MATCH
    │ ├── topoLevelNums >= 3? → NOT_MATCH
    │ ├── inplace? → NOT_MATCH
    │ └── 拓扑匹配 → "CcuAlltoAllMesh1D"
    │
    ▼
Template: CcuTempAlltoAllMesh1D::CalcRes
    │ ├── CalcChannelRequestMesh1D → channels
    │ ├── 构造 CcuKernelArgAlltoAllMesh1D
    │ └── 填充 resourceRequest.ccuKernelInfos
    │
    ▼
AICore: CcuPrepareForAllToAll
    │ ├── xnData_[0] = token
    │ ├── xnData_[1] = sendBuf + offset
    │ ├── xnData_[2] = recvBuf + offset
    │ ├── xnData_[3] = sliceSize
    │ ├── xnData_[4] = strideSize
    │ ├── xnData_[5] = UINT64_MAX - (sliceSize / 256MB)  ← 本次修复
    │ ├── xnData_[6] = strideSize * rankId
    │ └── xnData_[7~10] = CalcGoSize(sliceSize, 8, CCU_MEMSLICE_SIZE*8)
    │
    ▼
Kernel: CcuAlltoAllMesh1DKernel(param[0~10], arg)
    │
    ├── InitResource: GetResByChannel 获取远端变量
    ├── LoadArgs: 填充本 rank 参数
    ├── PreSync: WriteVariableWithNotify + NotifyWait
    ├── DoAlltoAll:
    │   ├── loadFromMem=true: LocalCopy + WriteChunked + EventWait
    │   └── loadFromMem=false: WriteChunked + GroupCopy + EventWait
    └── PostSync: NotifyRecord + NotifyWait
```

#### loadFromMem 路径选择

```
param.isMc2?
├── true (MC2 模式)
│   └── loadFromMem = true
│       └── DoAlltoAll 遍历所有 rank:
│           ├── 本 rank: LocalCopy(localDst, src, sliceSize)
│           └── 远端 rank: WriteChunked(channel, dst, src)
│           然后 EventWait 等待所有 rank（含本 rank）
│
└── false (非 MC2 / KFC Server 模式)
    └── loadFromMem = false
        └── DoAlltoAll:
            ├── 先遍历远端 rank: WriteChunked(channel, dst, src)
            ├── GroupCopy(ctx, localDst, src[rankId], goSize)
            └── EventWait 等待所有远端 rank
```

### 对子模块的修改

| 子模块 | 修改内容 | 是否需通知 MDE |
|--------|---------|---------------|
| hccl/cc/src/ops/all_to_all_v | 新增 CCU selector/template/kernel | 是 |
| hccl/cc/src/ops/kfc_server | 新增 AllToAll KFC Server 支持 | 是 |
| hccl/cc/src/common | 新增 AllToAll opType 到资源分配 | 是 |
| hccl/ccu | 修复 v0 prepare writeLoopCount | 是 |
| tiling/hccl | 注册 CCU 算法名 | 否 |

### 错误处理

#### 系统错误

- `InitResource`：channel 为空时返回 `CCU_E_INTERNAL`
- `LoadArgs`：始终返回 `CCU_SUCCESS`（当前无参数校验）
- 分块传输无内存分配失败路径（CCU 原语不分配内存）

#### 接口错误

- `SelectCcuScheduleAlgo`：不支持的场景返回 `NOT_MATCH`（fallback 到 AICPU）
- `CalcRes`：channel 计算失败返回 `HCCL_E_INTERNAL`
- KFC Server：不支持 opType 返回 `HCCL_E_NOT_SUPPORT`

## 安全检查

### 编码军规

参考编码军规.md。

### 编码检查项

| **检查项** | **检查项说明** | **是否涉及** |
| -------------------------------------------------- | ------------------------------------------------------------ | ------------ |
| **是否涉及资源生命周期管理** | Kernel 执行无持久资源，CcuKernelArgAlltoAllMesh1D 通过 shared_ptr 管理 | 否 |
| **是否创建新线程** | 无新线程创建 | 否 |

## 兼容性检查

- **新增算法**：AllToAll CCU 为新增算法，不影响现有 AICPU AllToAll 行为
- **编译开关**：`MC2_CLIENT_ENABLE_CCU` 控制编译，默认不影响非 CCU 构建
- **KFC Server**：新增 AllToAll 分支不影响现有 AllGather/ReduceScatter 分支
- **AICore v0**：`writeLoopCount` 从 0 改为 `UINT64_MAX - fullChunkCount`，修复了大数据分块传输的正确性

## DT设计

### 测试边界

- 测试入口：通过 ST 框架捕获 `CcuAlltoAllMesh1DKernel` 调用
- 测试出口：校验各 rank output 内存数据正确性
- 打桩模块：CCU 原语（Variable/Write/LocalCopy/GroupCopy/Notify/Event）

### 测试设计

| **测试类别** | **关键测试项** | **测试方法** | **用例类型** |
| ------------ | -------------- | ------------ | ------------ |
| **功能** | loadFromMem=true 路径 | 构造 isMc2=true 场景，验证 LocalCopy + Write | ST |
| **功能** | loadFromMem=false 路径 | 构造 isMc2=false 场景，验证 GroupCopy + Write | ST |
| **功能** | 分块传输边界 | sliceSize = 1B / 256MB-1 / 256MB / 256MB+1 / 512MB | ST |
| **功能** | GroupCopy 分块边界 | CalGoSize m/n/p 边界值 | ST |
| **功能** | 多 rank 数据正确性 | 2~8 rank AllToAll 数据校验 | ST |
| **功能** | 算法选择 | Mesh1D / Mesh1D_CLOS / 多级拓扑 | ST |
| **兼容性** | KFC Server 模式 | KFC Server 继承 AllToAll Kernel | ST |

### 测试框架设计

ST 测试框架已具备 CCU 模拟能力，支持 AllToAll CCU 算法测试。详见 `2026-08-20-ccu-st-design.md`。

## 验收标准

1. AllToAll CCU 算法在 Mesh1D 拓扑下功能正确，2~8 rank 数据搬运结果正确
2. `loadFromMem=true` 和 `loadFromMem=false` 两种路径均可正确执行
3. 分块传输正确处理 256MB 边界
4. KFC Server 模式下 AllToAll 正确分发执行
5. 算法选择器正确匹配拓扑和算法变体
6. 编译无 warning，CI 通过
