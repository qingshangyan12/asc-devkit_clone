# AllGather 代码流程与算法总结

本文基于最新 `src/ops/all_gather` 目录及其依赖的 `src/ops/op_common` 公共执行框架，梳理 AllGather 的主调用链、算法选择逻辑、executor 分层和核心模板算法。

AllGather 的语义是每个 rank 输入一段 `sendCount` 数据，所有 rank 的输出都按 rank 顺序拼成完整结果：

```text
sendBuf = local_rank_data
recvBuf = rank0_data | rank1_data | ... | rankN_data
```

## 目录分层

```text
src/ops/all_gather
├── all_gather_op.cc/.h                # C API 入口、参数检查、OpParam 构造
├── selector/                          # AllGatherAutoSelector，按拓扑/引擎/数据量选择算法名
├── executor/                          # 算法 executor，负责资源申请和组合 template
├── template/aicpu/                    # AICPU/CPU 展开模板，Mesh/NHR/OmniPipe/DPU 等
├── template/aiv/                      # AIV kernel 下发模板
├── template/ccu/                      # CCU kernel 下发模板
└── op_graph/                          # 图模式算子原型
```

整体上，`all_gather_op.cc` 只负责把用户 API 调用转成统一的 `OpParam`，真正的算法选择和执行由 `op_common` 的 selector/executor 框架完成。

## 主调用链

### OPBASE 模式

```text
HcclAllGather
 -> CANN 版本/芯片判断，必要时回退 HcclAllGatherInner
 -> sendCount == 0 时直接返回成功
 -> AllGatherInitAndCheck
    -> InitEnvConfig
    -> CheckAllGatherInputPara
    -> HcclGetCommName
    -> opTag = AllGather_<commName>
    -> HcclCheckTag
    -> CheckCount / CheckDataType(dataType, false)
    -> HcclGetRankSize / HcclGetRankId / HcomCheckUserRank
 -> AllGatherEntryLog
 -> AllGatherOutPlace
 -> AllGatherOutPlaceCommon(..., OpMode::OPBASE)
    -> HcclGetRankSize
    -> inputSize = sendCount * typeSize
    -> outputSize = inputSize * rankSize
    -> 填充 OpParam
       -> DataDes.count = sendCount
       -> DataDes.dataType = dataType
       -> opType = HCCL_CMD_ALLGATHER
    -> HcclGetOpExpansionMode
    -> CANN 9.0 CCU 回退 HcclAllGatherInner
    -> ShouldGoCcuFastLaunch
    -> AIV: HcclAivCacheCheckAndReplay
    -> Selector
    -> ShouldUseInnerOp 时回退 HcclAllGatherInner
    -> rankSize == 1 时 SingleRankProc
    -> CANN >= 9.1 OPBASE: AllGatherSupportSymmetricMemory
    -> HcclExecOp
```

`AllGatherOutPlaceCommon` 是新流程的核心入口。它构造 `OpParam` 后先判断 CCU fast launch 和 AIV cache replay，再走 selector 选择算法名。`AllGatherSupportSymmetricMemory` 只在 CANN 9.1 及以后 OPBASE 模式尝试启用，命中后 Mesh 模板可以直接访问远端对称内存窗口。

### GraphMode / OFFLOAD 模式

```text
HcclAllGatherGraphMode
 -> HcomGetCommHandleByGroup
 -> sendCount == 0 时直接返回成功
 -> AllGatherInitAndCheck
 -> HcclCheckTag(tag)
 -> 构造 ResPackGraphMode(tag, streams, scratchMem)
 -> AllGatherEntryLog
 -> AllGatherOutPlaceGraphMode
 -> AllGatherOutPlaceCommon(..., OpMode::OFFLOAD, resPack)
```

GraphMode 与 OPBASE 共用 `AllGatherOutPlaceCommon`，主要差异是 `opMode = OFFLOAD`，并通过 `ResPackGraphMode` 传入外部 streams 和 scratch memory。模板层通过 `enableRemoteMemAccess` 决定是否访问 graph mode remote output。

## Selector 流程

公共入口在 `op_common.cc`：

```text
Selector
 -> HcclCommGetStatus
 -> HcclCalcTopoInfo
 -> CheckAsymmetricTopoSupport
 -> ExecuteSelector::Run
 -> AllGatherAutoSelector::Select...
 -> SetCommEngine
 -> LoadAICPUKernel / RegisterKernel
 -> SetOpParamAlgTag
 -> SetExecTimeout
 -> SetMultipleDimensionSplitRatio
```

`AllGatherAutoSelector` 注册在 `HCCL_CMD_ALLGATHER` 上，优先级为 18。它根据 `OpExecuteConfig` 走不同选择函数：

- `SelectAicpuAlgo`：AICPU/CPU 展开路径。
- `SelectAivAlgo`：AIV 路径。
- `SelectCcuMsAlgo`：CCU MS 路径。
- `SelectCcuScheduleAlgo`：CCU SCHED 路径。
- `SelectDPUAlgo`：DPU 路径。

### AICPU 主要选择

常见选择结果：

```text
单层 MESH_1D:
  普通: InsAllGatherMesh1D
  two-level net layer 且总量 > 1GB: InsAllGatherMesh1D1DZAxisDetour

单层 CLOS:
  InsAllGatherNHR

单层 MESH_1D_CLOS:
  pcie-mix 且 1D mesh 全连通: InsAllGatherMesh1D
  pcie-mix 小于 4MB: InsAllGatherParallelMesh1DNHRPcie
  pcie-mix 大于等于 4MB: InsV2AllGatherOmniPipePcie
  UBX mesh==clos 且 rank<=4:
    小数据: InsAllGatherMesh1D
    大数据: InsAllGatherConcurrentMesh1DNHR
  UBX clos 是 mesh 倍数且大数据:
    小于 4MB: InsAllGatherParallelMesh1DNHRMultiJetty
    大于等于 4MB: InsV2AllGatherOmniPipe
  其他: InsAllGatherNHR

多层拓扑:
  3 层且 level2Uboe:
    8 卡/module: InsV2AllGatherOmniPipeUboe
    layer1 单 net instance: InsAllGatherNHR
    其他: InsAllGatherParallelMesh1DNHRUboe
  Level1Nhr / Level0Nhr / layer0 单 net instance: InsAllGatherNHR
  level0 MESH_1D:
    3 层及以上: InsAllGatherSequenceNHRNHRMesh1D
    大数据且总量 > 4GB: InsAllGatherSequenceNHRMesh1D
    大数据且总量 <= 4GB: InsAllGatherParallelMesh1DNHR
    小数据: InsAllGatherNHR
```

### CCU 主要选择

CCU MS 只支持单层拓扑，且不支持 inplace allgather：

```text
CCU MS:
  MESH_1D:
    two-die regular: CcuAllGatherMesh2Die
    普通: CcuAllGatherMesh1D
  MESH_1D_CLOS:
    pcie-mix 非全连通: 回退
    大数据: 回退 CCU SCHED/AICPU
    小数据: CcuAllGatherMesh1D
```

CCU SCHED 的主要选择：

```text
单层 MESH_1D:
  two-die regular: CcuAllGatherMesh2DieMem2Mem
  普通: CcuAllGatherMesh1DMem2Mem

单层 CLOS:
  pcie-mix: 回退
  大于 1MB: CcuAllGatherMesh1DMem2Mem
  小数据: CcuAllGatherNHR1DMem2Mem

单层 MESH_1D_CLOS:
  pcie-mix 且 mesh 全连通: 退化到 Mesh1D mem2mem
  pcie-mix 非全连通: 回退
  UBX 小数据: CcuAllGatherMesh1DMem2Mem
  UBX mesh==clos 且 rank<=4 大数据: CcuAllGatherConcurrentMesh1DNHRMem
  UBX clos 是 mesh 倍数:
    小于 16MB: CcuAllGatherParallelMesh1DNHRMemMultiJetty
    大于等于 16MB: CcuAllGatherOmniPipe2D
  其他: CcuAllGatherNHR1DMem2MemMultiJetty

多层 MESH_1D:
  per-rank 数据 >= 4MB 且 rank > 8: 回退 AICPU
  rank > 64: 回退 AICPU
  Level1Nhr: CcuAllGatherNHR1DMem2Mem
  is2DieFullMesh: 回退
  layer0 单 net instance: CcuAllGatherNHR1DMem2Mem
  rank <= 8:
    小数据: CcuAllGatherMesh1DMem2Mem
    大数据: CcuAllGatherParallelMesh1DNHR
  rank <= 32 且小于 64KB: CcuAllGatherMesh1DMem2Mem
  小于 8MB: CcuAllGatherSequenceMeshMesh
  其他: CcuAllGatherParallelMesh1DNHR
```

### AIV 选择

`SelectAivAlgo` 要求：

- `rankSize <= MAX_RANK_SIZE`。
- 非 `AIV_ONLY` 时，`totalSize < AIV_MAX_PER_RANK_DATA_SIZE * rankSize`。
- `totalSize <= cclBufferSize * AIV_MAX_CCL_LOOP_NUM`。

满足后统一选择：

```text
AivAllGatherMesh1D
```

### DPU 选择

```text
多层拓扑:
  layer0 单 net instance 或 level0 MESH_1D: InsAllGatherMeshNhrDPU
  level0 MESH_1D_CLOS: InsV2AllGatherOmniPipe
```

## HcclExecOp 执行框架

`HcclExecOp` 是新 AllGather 的统一执行入口：

```text
HcclExecOp
 -> 根据 algName 从 CollAlgExecRegistryV2 取 executor
 -> HcclGetAlgRes
    -> executor->CalcAlgHierarchyInfo
    -> executor->CalcRes
    -> 申请/复用 thread、channel、CCL buffer、engine context
 -> DFX/profiling 注册
 -> 根据 engine 分发
```

不同 engine 的执行方式：

```text
AICPU_TS / CPU:
  -> HcclThreadAcquireWithStream 获取 CPU TS thread
  -> HcclThreadExportToCommEngine
  -> HcclAicpuKernelEntranceLaunch
  -> AICPU kernel HcclLaunchAicpuKernel
  -> 反序列化 AlgResourceCtxSerializable
  -> 从 registry 取 executor
  -> executor->Orchestrate(param, resCtx)
  -> device 通知 host 完成

AIV:
  -> HcclAivKernelEntranceLaunch
  -> executor/template 组装 AivOpArgs
  -> ExecuteKernelLaunch
  -> AIV cache/profiling 处理

CCU:
  -> host 侧反序列化或复用资源
  -> 覆盖主 thread
  -> CaptureSlaveStreams
  -> executor->Orchestrate(param, resCtx)
  -> template 内 HcommCcuKernelLaunch
```

CCU fast launch：当 `ShouldGoCcuFastLaunch` 在通信域 engine context 中找到 `CcuFastLaunchCtx` 时，直接通过 `HcclExecOpCcuFastLaunch` 取 executor 并调用 `executor->FastLaunch`，跳过重新计算资源和重新构造完整 kernel 参数。AllGather 的 OPBASE AIV 路径还会先执行 `HcclAivCacheCheckAndReplay`，命中时直接返回成功。

## Executor 分层

executor 负责把 selector 选出的算法名映射到一个或多个 template，并负责：

- 拓扑匹配：`CalcAlgHierarchyInfo` 调用 `TopoMatch*::MatchTopo`。
- 资源计算：`CalcRes` 汇总 template 所需 thread、notify、channel 或 CCU kernel。
- 执行编排：`Orchestrate` 反序列化资源后调用 `OrchestrateLoop`。
- 大数据分片：根据 CCL buffer、scratch multiplier、UB 上限计算 `maxCountPerLoop`。
- 多 template 同步：通过 `PreSyncInterThreads` / `PostSyncInterThreads` 协调主从 thread。

### Sole Executor

注册算法包括：

```text
InsAllGatherMesh1D
InsAllGatherMesh1D1DZAxisDetour
InsAllGatherNHR
InsAllGatherNHRDPU
AivAllGatherMesh1D
CcuAllGatherMesh1DMem2Mem
CcuAllGatherMesh1D
CcuAllGatherNHR1DMem2Mem
CcuAllGatherMesh2Die
CcuAllGatherMesh2DieMem2Mem
CcuAllGatherNHR1DMem2MemMultiJetty
```

执行逻辑：

```text
InsV2AllGatherSoleExecutor::Orchestrate
 -> RestoreChannelMap，AIV/CCU 跳过普通 channel map
 -> 构造单个 template
 -> template.CalcScratchMultiple
 -> 计算 maxCountPerLoop
    -> 普通路径受 CCL buffer 和 UB_MAX_DATA_SIZE 约束
    -> symmetric memory + InsAllGatherMesh1D 时可直接按 dataSize 单 loop
 -> for each loop:
      inBuffBaseOff = processedCount * typeSize
      outBuffBaseOff = processedCount * typeSize
      inputSliceStride = 0
      outputSliceStride = sendCount * typeSize
      sliceSize = currCount * typeSize
      template.KernelRun
 -> CCU 单 loop 时保存 fast launch ctx
```

Sole 适合单层拓扑或一个 template 就能完成的算法，例如单层 Mesh、单层 NHR、AIV Mesh、CCU Mesh/NHR。

### Sequence Executor

Sequence 用两个 template 串行做两级 AllGather，典型是“先框间，再框内”：

```text
InsV2AllGatherSequenceExecutor / Aicpu
 -> inter template:
      input -> output 或 CCL buffer
      先跨 level1 收集，把本 rank 的数据放到对应大块
 -> intra template:
      output/CCL buffer -> output
      在 level0 内扩散 inter 阶段结果
```

关键点：

- `CalcRes` 同时计算 intra/inter template 资源，slave thread 和 notify 取最大值复用。
- AICPU 版本的 inter 阶段会调用 `SplitData` 填充每个 rank slice 的 `allRankDispls/allRankSliceSize`。
- CCU sequence 版本用 output 作为中转：inter 写 output，intra 以 output 为输入和输出，减少一次 CCL buffer 拷贝。
- 单 loop CCU 可保存两段 template 的 fast launch 上下文，重放顺序是 inter 后 intra。

三层版本 `InsV2AllGatherSequenceExecutor3Level` 注册算法：

```text
InsAllGatherSequenceNHRNHRMesh1D
```

其执行顺序是：

```text
level2: INPUT -> HCCL_BUFFER
level1: HCCL_BUFFER -> HCCL_BUFFER
level0: HCCL_BUFFER -> OUTPUT
```

DPU 版本注册：

```text
InsAllGatherMeshNhrDPU
```

它用 `InsTempAllGatherMesh1D` + `InsTempAllGatherNHRDPU` 串行展开，资源里会携带 `npu2DpuShmemPtr/dpu2NpuShmemPtr`。

### Parallel Executor

Parallel 用两个 template 并行处理两块数据，典型组合是 Mesh + NHR：

```text
InsV2AllGatherParallelExecutor
 -> 数据按 multipleDimensionSplitRatio 切成 part0/part1
 -> 第一步:
      part0 跑 intra Mesh: input -> output
      part1 跑 inter NHR: input -> output
 -> 同步
 -> 第二步:
      part0 跑 inter NHR: output -> output
      part1 跑 intra Mesh: output -> output
 -> 同步
```

这种方式让层内和层间链路同时工作，提高多维拓扑大数据吞吐。注册算法包括：

```text
InsAllGatherParallelMesh1DNHR
InsAllGatherParallelMesh1DNHRMultiJetty
InsAllGatherParallelMesh1DNHRPcie
InsAllGatherParallelMesh1DNHRUboe
CcuAllGatherParallelMesh1DNHR
CcuAllGatherParallelMesh1DNHRMemMultiJetty
```

CCU 单 loop 时会把四段 kernel 下发信息按 `intra0, inter1, inter0, intra1` 的顺序保存到 fast launch ctx。

### Concurrent Executor

Concurrent 也是两个 template，但更偏向 UBX/4P 内并发场景：

```text
InsV2AllGatherConcurrentExecutor
 -> temp0 = Mesh 子拓扑
 -> temp1 = NHR/clos 子拓扑
 -> 按链路能力计算数据切分比例
 -> CCL buffer 按 scratch multiplier 比例切给两个 template
 -> 前同步
 -> 两个 template 按各自 loopTimes 交替下发
 -> 后同步
```

注册算法包括：

```text
InsAllGatherConcurrentMesh1DNHR
CcuAllGatherConcurrentMesh1DNHRMem
CcuAllGatherConcurrentMesh1DNHR
```

与 Parallel 的区别是：Parallel 对同一轮数据分两阶段交叉跑两种维度；Concurrent 是把数据整体分给两个 template，各自独立完成所分配片段。

### OmniPipe Executor

OmniPipe 面向多层或 PCIe mixed/UBOE 场景，把数据切成多级 pipeline slice，并按 level 调度 scratch-to-scratch template：

```text
InsV2AllGatherOmniPipeExecutor
 -> BuildSubCommAndTempMap
 -> 根据 level 带宽和 scratch 计算 maxCountPerLoop / loopTimes
 -> CalcAGOmniPipeSliceInfo
 -> for each loop/slice:
      level2 template
      level0 template
      level1 template
```

注册算法包括：

```text
InsV2AllGatherOmniPipeMultilevel
InsV2AllGatherOmniPipePcie
InsV2AllGatherOmniPipe
InsV2AllGatherOmniPipeUboe
```

`CcuAllGatherOmniPipe2D` 是最新 CCU UBX 两维流水 executor：

```text
InsV2AllGatherOmniPipe2DExecutor
 -> level0 Mesh mem2mem + level1 NHR mem2mem
 -> 本地拷贝 input -> output 对应 rank 位置
 -> CalcAGOmniPipeSliceInfo
 -> 每个 step 同步后同时下发 level0/level1 CCU template
```

它不依赖 CCL buffer 容量计算 loop，而主要按 `UB_MAX_DATA_SIZE` 切分。

## 核心模板算法

### Mesh 1D

实现文件：

```text
template/aicpu/ins_temp_all_gather_mesh_1D.cc
template/aicpu/ins_temp_all_gather_mesh_1D_Z_axis_detour.cc
template/ccu/ccu_temp_all_gather_mesh_1D*.cc
template/aiv/aiv_temp_all_gather_mesh_1D.cc
```

AICPU Mesh 的核心流程：

```text
KernelRun
 -> LocalDataCopy
    -> input -> output[myRank]
    -> OPBASE 非 remote/symmetric 时 input -> local CCL buffer[myRank]
 -> 多 thread 前同步
 -> RunAllGatherMesh
    -> 遍历除自己外的 connectedRank
    -> 构造 tx/rx DataSlice
    -> SendRecvRead
 -> 多 thread 后同步
```

普通 OPBASE 路径下，跨 rank 传输以 CCL buffer 为远端可访问中转区：

```text
input -> local output[myRank]
input -> local CCL buffer[myRank]
local output slice -> remote CCL buffer[myRank]
remote CCL buffer[remoteRank] -> local output[remoteRank]
```

GraphMode/OFFLOAD 下，`enableRemoteMemAccess` 会让远端目标切换到 `remoteOutputGraphMode`。当启用 symmetric memory 时，模板通过 `HcclSymWinGetPeerPointer(outputSymWindow, outputOffset, connectedRank)` 获取远端 output 对称窗口指针，可直接写远端 output，Sole executor 也会放宽 CCL/UB loop 限制。

### NHR

实现文件：

```text
template/aicpu/ins_temp_all_gather_nhr.cc
template/aicpu/ins_temp_all_gather_nhr_dpu.cc
template/ccu/ccu_temp_all_gather_nhr_1D_mem2mem.cc
template/ccu/ccu_temp_all_gather_nhr_1D_multi_jetty_mem2mem.cc
```

NHR 是按超立方/递归倍增思想展开的 AllGather。步数为：

```text
nSteps = ceil(log2(templateRankSize))
```

每一步由 `GetStepInfo` 计算：

```text
deltaRank = 1 << (nSteps - 1 - step)
recvFrom  = (myAlgRank + rankSize - deltaRank) % rankSize
sendTo    = (myAlgRank + deltaRank) % rankSize
nSlices   = (rankSize - 1 + deltaRank) / (2 * deltaRank)
```

AICPU NHR 主流程：

```text
KernelRun
 -> PrepareDataSplitForMultiChannel
 -> 判断 PCIe 链路，PCIe 使用 DMA read
 -> 判断最后一步是否可直接 read 到 output
 -> for channelIdx:
      LocalDataCopy: input -> scratch[myRank]
      for step in nSteps:
          GetStepInfo
          BuildNormalStepSlices 或 BuildLastStepWriteThenReadSlices
          SendRecvBatchWrite 或 SendRecvRead
      如最后一步未直接落 output，则 PostLocalCopy: scratch -> output
```

NHR 的特点：

- 通信步数约为 `log2(rankSize)`，比 Mesh 的全连接遍历更适合大 rank 数或非全连接拓扑。
- 数据先进入 CCL scratch，再随 step 逐步扩散。
- 支持多 channel 数据切分，`CalcDataSplitByPortGroup` 根据端口组拆分每个 slice。
- PCIe 链路场景会选择 DMA read 模式。
- 在 input/output 都是 output buffer、stride 一致且非 remote access 时，最后一步可直接读到 output，减少一次 post copy。

CCU NHR 同样先在 host 侧计算所有 `NHRStepInfo`，再构造 `CcuAllGatherNHR1DMem2MemKernel` 参数。multi-jetty 版本用于 UBX/clos 多 jetty 场景。

### OmniPipe Mesh/NHR

OmniPipe 模板是 Mesh/NHR 的流水化变体：

```text
InsTempAllGatherOmniPipeMesh1D
InsTempAllGatherOmniPipeNHR
InsTempAllGatherOmniPipeNHRDPU
CcuTempAllGatherOmniPipeMesh1DMem2Mem
CcuTempAllGatherOmniPipeNHR1DMem2Mem
```

它们不做完整 `input -> output` 或 `scratch -> output` 的收尾，而是按 executor 算出的 `StepSliceInfo` 在 CCL/output 中传递当前 pipeline slice。executor 负责计算每级 slice 的 offset、count、stride，并按 level 调用对应 template。这样多级拓扑可以边产生、边跨级传输，降低单级完成后再进入下一级的等待。

### AIV / CCU Kernel 路径

AIV template：

```text
AivTempAllGatherMesh1D::KernelRun
 -> IncSliceId
 -> 填充 AivOpArgs:
      cmdType = HCCL_CMD_ALLGATHER
      input/output
      rank/rankSize/topo
      count/dataType
      inputSliceStride/outputSliceStride/repeat
      buffersIn = aivCommInfoPtr
 -> CalNumBlocks
 -> ExecuteKernelLaunch
```

CCU template：

```text
CalcRes
 -> 计算 channel 或 kernel 数
 -> 填充 CcuKernelInfo / CcuKernelArg*

KernelRun
 -> 根据 tempAlgParams 填 task args
 -> HcommCcuKernelLaunch

FastLaunch
 -> 复用缓存的 CcuKernelSubmitInfo
 -> 只更新本次 input/output/hcclBuff 地址
 -> HcommCcuKernelLaunch
```

CCU 路径对 inplace 和部分拓扑限制更多，但在支持范围内可以减少 host 编排开销，并通过 fast launch 复用已构造的 kernel 参数。

## 资源模型

AllGather 新流程围绕 `AlgResourceCtxSerializable` 传递资源：

- `cclMem`：通信域管理的 CCL scratch buffer。
- `threads`：主 thread、slave thread 或 CCU/AICPU/AIV 所需 thread。
- `channels`：按 level 保存的远端 rank channel。
- `ccuKernels` / `ccuKernelNum`：CCU engine 的 kernel handle。
- `aivCommInfoPtr`：AIV engine 使用的通信资源。
- `algHierarchyInfo`：由 topo match 生成的层级 rank 分组。
- `npu2DpuShmemPtr` / `dpu2NpuShmemPtr`：DPU 模板使用的共享内存指针。

资源申请由 executor 的 `CalcRes` 描述需求，再由 `HcclGetAlgRes` 统一申请或复用。AICPU 路径会把资源序列化到 engine context，AICPU kernel 中再反序列化并执行 `executor->Orchestrate`。

## 特殊路径

### rankSize == 1

`AllGatherOutPlaceCommon` 在 selector 后判断 `userRankSize == 1`，调用 `SingleRankProc`。如果输入输出不同，则直接通过 CPU TS thread 做本地拷贝：

```text
HcommLocalCopyOnThread(outputPtr, inputPtr, sendCount * typeSize)
```

### symmetric memory

OPBASE 模式下，如果 input/output 都能通过 `HcclCommSymWinGet` 找到对称内存窗口，`AllGatherSupportSymmetricMemory` 会设置：

```text
supportSymmetricMemory = true
inputSymWindow / outputSymWindow
inputOffset / outputOffset
```

Mesh 模板随后可通过 `HcclSymWinGetPeerPointer` 获得远端 output 指针，减少对 CCL buffer 的依赖。当前只在 `InsAllGatherMesh1D` 的 Sole executor 路径显式放宽 loop 切分。

### CCU fast launch

当 CCU 算法单 loop 执行完后，executor 会把 thread、kernel handle、cached args 保存到 `CcuFastLaunchCtx`。下一次同 tag、同参数形态命中时：

```text
ShouldGoCcuFastLaunch
 -> HcclExecOpCcuFastLaunch
 -> executor->FastLaunch
```

FastLaunch 只刷新 input/output 地址和 offset，然后直接重放 CCU kernel 下发信息，减少资源计算和参数构造开销。Sole、Sequence、Parallel、Concurrent 都实现了对应保存/重放逻辑。

## 关键理解点

- `sendCount` 是每个 rank 输入元素数，`recvBuf` 总元素数是 `sendCount * rankSize`。
- `OpParam.inputSize = sendCount * typeSize`，`OpParam.outputSize = inputSize * rankSize`。
- `outputSliceStride` 通常等于单 rank 输入大小，用来在 `recvBuf` 中跨 rank slice 定位。
- Selector 的核心分流依据是 engine、拓扑层级、`MESH_1D/CLOS/MESH_1D_CLOS` 形态、数据量、rank 数、inplace、PCIe mixed、UBX mesh/clos 关系和 DPU/UBOE 标记。
- Mesh 适合全连接或小规模拓扑，直接和所有远端 rank 交换；NHR 通过 `ceil(log2(rankSize))` 步递归扩散数据，更适合多 rank 或非全连接拓扑。
- executor 负责编排 template：Sole 单模板、Sequence 串行两/三级、Parallel 两阶段交叉并行、Concurrent 两模板分片并发、OmniPipe 多级流水。
