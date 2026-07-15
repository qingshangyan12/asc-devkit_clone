# ReduceScatter 代码流程与算法总结

本文基于 `src/ops/reduce_scatter` 目录及其依赖的 `src/ops/op_common` 公共执行框架，梳理 ReduceScatter 的主调用链、算法选择逻辑、executor 分层和核心模板算法。

ReduceScatter 的输入布局为：

```text
sendBuf = rank0_slice | rank1_slice | ... | rankN_slice
recvBuf = 本 rank 对应 slice 的规约结果
```

也就是说，每个 rank 的输入都包含 `rankSize * recvCount` 个元素，最终每个 rank 只得到属于自己的 `recvCount` 个元素，且该 slice 是所有 rank 对应片段按 `op` 规约后的结果。

## 目录分层

```text
src/ops/reduce_scatter
├── reduce_scatter_op.cc/.h            # C API 入口、参数检查、OpParam 构造
├── selector/                          # ReduceScatterAutoSelector，按拓扑/引擎/数据量选择算法名
├── executor/                          # 算法 executor，负责资源申请和组合 template
├── template/aicpu/                    # AICPU/CPU 展开模板，Mesh/NHR/OmniPipe/保序等
├── template/aiv/                      # AIV kernel 下发模板
├── template/ccu/                      # CCU kernel 下发模板
└── op_graph/                          # 图模式算子原型
```

整体上，`reduce_scatter_op.cc` 只负责把用户 API 调用转成统一的 `OpParam`，真正的算法选择和执行由 `op_common` 的 selector/executor 框架完成。

## 主调用链

### OPBASE 模式

```text
HcclReduceScatter
 -> CANN 版本/芯片判断，必要时回退 HcclReduceScatterInner
 -> InitEnvConfig
 -> CheckReduceScatterInputPara
 -> recvCount == 0 时直接返回成功
 -> HcclGetRankSize / HcclGetRankId / HcomCheckUserRank
 -> CheckCount / CheckDataType(dataType, true) / CheckReduceOp
 -> HcclGetCommName
 -> 生成 tag: ReduceScatter_<commName>
 -> HcclCheckTag
 -> ReduceScatterEntryLog
 -> ReduceScatterOutPlace
    -> PrepareReduceScatterParam
       -> outputSize = recvCount * typeSize
       -> inputSize = outputSize * rankSize
       -> DataDes.count = recvCount
       -> reduceType = op
       -> opType = HCCL_CMD_REDUCE_SCATTER
    -> HcclGetOpExpansionMode
    -> CANN 9.0 CCU 回退 HcclReduceScatterInner
    -> ShouldGoCcuFastLaunch
    -> AIV: HcclAivCacheCheckAndReplay
    -> Selector
    -> ShouldUseInnerOp 时回退 HcclReduceScatterInner
    -> rankSize == 1 时 SingleRankProc
    -> HcclExecOp
```

`PrepareReduceScatterParam` 的关键点是 `DataDes.count` 只保存单 rank 输出元素数 `recvCount`。executor/template 会用 `rankSize` 和 stride 推导输入中各 rank slice 的位置。

### GraphMode / OFFLOAD 模式

```text
HcclReduceScatterGraphMode
 -> HcomGetCommHandleByGroup
 -> InitEnvConfig
 -> CheckReduceScatterInputPara
 -> HcclGetCommName / HcclCheckTag
 -> CheckCount / CheckDataType / CheckReduceOp
 -> HcclGetRankSize / HcclGetRankId / HcomCheckUserRank
 -> 构造 ResPackGraphMode(tag, streams, scratchMem)
 -> ReduceScatterEntryLog
 -> ReduceScatterOutPlaceGraphMode
    -> PrepareReduceScatterParam(..., OpMode::OFFLOAD)
    -> Selector
    -> HcclExecOp(..., resPack)
```

GraphMode 与 OPBASE 共用 selector/executor 体系，主要差异是 `opMode = OFFLOAD`，并通过 `ResPackGraphMode` 传入外部 stream 和 scratch memory。

## Selector 流程

公共入口在 `op_common.cc`：

```text
Selector
 -> HcclCommGetStatus
 -> HcclCalcTopoInfo
 -> CheckAsymmetricTopoSupport
 -> ExecuteSelector::Run
 -> ReduceScatterAutoSelector::Select...
 -> SetCommEngine
 -> LoadAICPUKernel / RegisterKernel
 -> SetOpParamAlgTag
 -> SetExecTimeout
 -> SetMultipleDimensionSplitRatio
```

`ReduceScatterAutoSelector` 注册在 `HCCL_CMD_REDUCE_SCATTER` 上，优先级为 18。它根据 `OpExecuteConfig` 走不同选择函数：

- `SelectAicpuAlgo`：AICPU/CPU 展开路径。
- `SelectAivAlgo`：AIV 路径。
- `SelectCcuMsAlgo`：CCU MS 路径。
- `SelectCcuScheduleAlgo`：CCU SCHED 路径。
- `SelectDPUAlgo`：DPU 路径。

### AICPU 主要选择

常见选择结果：

```text
严格确定性/保序:
  rankSize 较大: ReduceScatterOrderPreservedGroup
  其他: ReduceScatterOrderPreserved

单层 MESH_1D:
  64-bit 数据或 PROD: InsReduceScatterMesh1D
  大数据: InsReduceScatterMesh1DMeshChunk
  其他: InsReduceScatterMesh1D

单层 CLOS:
  64-bit 数据或 PROD: InsReduceScatterAicpuReduceNHR
  其他: InsReduceScatterNHR

单层 MESH_1D_CLOS:
  pcie-mix 且全连通: 退化为 Mesh1D/MeshChunk
  pcie-mix 小数据: InsReduceScatterParallelMesh1DNHRPcie
  pcie-mix 大数据: InsV2ReduceScatterOmniPipePcie
  64-bit 数据或 PROD: InsReduceScatterAicpuReduceNHR
  UBX mesh/clos 成倍且大数据: InsReduceScatterParallelMesh1DNHRUBX
  其他: InsReduceScatterNHR

多层拓扑:
  64-bit 数据或 PROD: InsReduceScatterAicpuReduceNHR
  3 层且 UBOE: InsV2ReduceScatterOmniPipeUboe / InsReduceScatterParallelNHRNHRUboe
  Level1Nhr / Level0Nhr / 单 net instance: InsReduceScatterNHR
  level0 MESH_1D 大数据: InsReduceScatterParallelMesh1DNHR 或 InsReduceScatterSequenceMesh1DNHRNHR
```

### CCU 主要选择

CCU 会主动排除严格确定性保序、`PROD`、部分 64-bit 数据类型；SCHED 路径还排除 inplace。

```text
CCU MS:
  MESH_1D: CcuReduceScatterMesh1D / CcuReduceScatterMesh2Die
  MESH_1D_CLOS 小数据: CcuReduceScatterMesh1D
  MESH_1D_CLOS 大数据且 4P 内: CcuReduceScatterConcurrentMeshNHRMs
  UBX clos 是 mesh 倍数且大数据: CcuV2ReduceScatterOmniPipeMs

CCU SCHED:
  单层 MESH_1D: CcuReduceScatterMesh1DMem2Mem / CcuReduceScatterMeshMem2Mem1D2Die
  单层 CLOS: CcuReduceScatterNHR1DMem2Mem
  MESH_1D_CLOS UBX:
    小数据: CcuReduceScatterMesh1DMem2Mem
    4P 内大数据: CcuReduceScatterConcurrentMeshNHRSche
    clos 是 mesh 倍数且大数据: CcuV2ReduceScatterOmniPipe
    其他: CcuReduceScatterNhr1DMem2MemMultiJetty
  多层 MESH_1D:
    Level1Nhr 或单 net instance: CcuReduceScatterNHR1DMem2Mem
    小数据 flatten: CcuReduceScatterMesh1DMem2Mem
    64 ranks 中小数据: CcuReduceScatterSequenceMeshMesh / CcuReduceScatterParallelMesh1DNHR
```

### AIV 选择

`SelectAivAlgo` 会排除严格确定性、`PROD`、`UINT64/FP64`、rank 数过大和超过 AIV/CCL buffer 限制的数据量。满足条件后统一选择：

```text
AivReduceScatterMesh1D
```

### DPU 选择

多层 `MESH_1D_CLOS` 选择：

```text
InsV2ReduceScatterOmniPipe
```

其他多层场景选择：

```text
InsReduceScatterSequenceMeshMeshDPU
```

## HcclExecOp 执行框架

`HcclExecOp` 是新 ReduceScatter 的统一执行入口：

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

CCU 还有 fast launch 逻辑：当 `ShouldGoCcuFastLaunch` 在通信域 engine context 中找到 `CcuFastLaunchCtx` 时，直接通过 `HcclExecOpCcuFastLaunch` 取 executor 并调用 `executor->FastLaunch`，跳过重新计算资源和重新构造完整 kernel 参数。ReduceScatter 的 OPBASE 路径还在 AIV 下先做 `HcclAivCacheCheckAndReplay`，命中缓存时直接重放返回。

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
InsReduceScatterMesh1D
InsReduceScatterMesh1DMeshChunk
InsReduceScatterNHR
InsReduceScatterAicpuReduceNHR
InsReduceScatterMesh1DZAxisDetour
AivReduceScatterMesh1D
CcuReduceScatterMesh1DMem2Mem
CcuReduceScatterMesh1D
CcuReduceScatterNHR1DMem2Mem
CcuReduceScatterMeshMem2Mem1D2Die
CcuReduceScatterMesh2Die
CcuReduceScatterNhr1DMem2MemMultiJetty
```

执行逻辑：

```text
InsV2ReduceScatterSoleExecutor::Orchestrate
 -> RestoreChannelMap，AIV/CCU 跳过普通 channel map
 -> 构造单个 template
 -> 计算 maxCountPerLoop
 -> for each loop:
      inBuffBaseOff = processedCount * typeSize
      outBuffBaseOff = processedCount * typeSize
      inputSliceStride = recvCount * typeSize
      outputSliceStride = 0
      sliceSize = currCount * typeSize
      template.KernelRun
 -> CCU 单 loop 时保存 fast launch ctx
```

Sole 适合单层拓扑或一个 template 就能完成的算法，例如单层 Mesh、单层 NHR、AIV Mesh、CCU Mesh/NHR。这里的 `inputSliceStride` 等于单 rank 输出大小，用来定位 `sendBuf` 中 rank0/rank1/... 的连续 slice。

### Sequence Executor

Sequence 用两个或三个 template 串行做多级 ReduceScatter。AICPU 版本典型流程是：

```text
InsV2ReduceScatterSequenceExecutorAicpu
 -> level0/intra template:
      input -> CCL buffer
      在框内/机内先规约各自 slice
 -> level1/inter template:
      CCL buffer -> output
      在框间继续规约并把本 rank 结果落到 recvBuf
```

三层版本 `InsV2ReduceScatterSequenceExecutor3Level` 是：

```text
level0: INPUT -> HCCL_BUFFER
level1: HCCL_BUFFER -> HCCL_BUFFER，可在 rankSizeLevel1 == 1 时跳过
level2: HCCL_BUFFER -> OUTPUT
```

普通 DPU sequence `InsV2ReduceScatterSequenceExecutor` 也按两级串行展开，用 CCL buffer 分成 in/out 两段作为中转。

### Parallel Executor

Parallel 用两个 template 并行处理两块数据，典型组合是 Mesh + NHR：

```text
InsReduceScatterParallelExecutor
 -> 数据按 multipleDimensionSplitRatio 切成 part0/part1
 -> 第一步:
      part0 跑 intra Mesh，input -> scratch
      part1 跑 inter NHR，input -> scratch/output
 -> 同步
 -> 第二步:
      part0 跑 inter NHR，scratch -> output
      part1 跑 intra Mesh，scratch -> output
 -> 同步
```

这种方式让层内和层间链路同时工作，提高多维拓扑大数据吞吐。注册算法包括：

```text
InsReduceScatterParallelMesh1DNHR
InsReduceScatterParallelMesh1DNHRUBX
InsReduceScatterParallelMesh1DNHRPcie
InsReduceScatterParallelNHRNHRUboe
CcuReduceScatterParallelMesh1DNHR
CcuReduceScatterParallelMesh1DNHRMultiJetty
```

### Concurrent Executor

Concurrent 也是两个 template，但更偏向 UBX/4P 内并发场景。它会：

- 把 `algHierarchyInfo` 拆成 mesh 子通信域和 clos/NHR 子通信域。
- 分别计算两个 template 的资源。
- 按端口数或链路能力把 `recvCount` 切给两个 template。
- 给两个 template 分配不同 thread/channel。
- 在同一轮内并发运行 Mesh 和 NHR，前后用 notify 同步。

注册算法包括：

```text
InsReduceScatterConcurrentMeshNHR
CcuReduceScatterConcurrentMeshNHRSche
CcuReduceScatterConcurrentMeshNHRMs
```

### OmniPipe Executor

OmniPipe 面向多层、PCIe mixed、UBX/UBOE 场景，把数据切成多级 pipeline slice，并按 level 调度 scratch-to-scratch template：

```text
InsV2ReduceScatterOmniPipeExecutor
 -> BuildSubCommAndTempMap
 -> 根据 level 带宽和 scratch 计算 maxCountPerLoop / loopTimes
 -> CalcRSOmniPipeSliceInfo
 -> for each loop:
      localcopy: user input -> CCL buffer
      for each level2 step:
          level2 NHR/Mesh template
          for each level0/level1 step:
              level0 Mesh template
              level1 NHR template
      localcopy: CCL buffer[myRank] -> user output
```

ReduceScatter 的 OmniPipe 中间过程主要在 CCL scratch 中完成，开始时把用户输入拷进 scratch，结束时只把 `myRank` 对应规约结果拷回 `recvBuf`。

注册算法包括：

```text
InsV2ReduceScatterOmniPipeMultilevel
InsV2ReduceScatterOmniPipePcie
InsV2ReduceScatterOmniPipe
InsV2ReduceScatterOmniPipeUboe
CcuV2ReduceScatterOmniPipe
CcuV2ReduceScatterOmniPipeMs
```

### OrderPreserved Executor

保序 executor 用于严格确定性规约：

```text
InsV2ReduceScatterOrderPreservedExecutor
 -> InitOrderPreservedBaseParams
 -> 构造 MemBlockInfo:
      每个 rank 的用户输入偏移
      scratch 中每个 rank block 的偏移
      输出 block 偏移
 -> template.SetMemBlockInfo
 -> template.KernelRun
```

注册算法包括：

```text
ReduceScatterOrderPreserved
ReduceScatterOrderPreservedGroup
```

`ReduceScatterOrderPreserved` 用于较小 rank 数，`ReduceScatterOrderPreservedGroup` 用于更大 rank 数。两者的目标都是固定数据块交换和本地树形 reduce 的顺序，降低不同执行顺序造成的浮点规约差异。

## 核心模板算法

### Mesh 1D

实现文件：

```text
template/aicpu/ins_temp_reduce_scatter_mesh_1D.cc
template/aicpu/ins_temp_reduce_scatter_mesh_1D_meshchunk.cc
template/ccu/ccu_temp_reduce_scatter_mesh_1D*.cc
template/aiv/aiv_temp_reduce_scatter_mesh_1D.cc
```

AICPU Mesh 的核心流程：

```text
KernelRun
 -> RunReduceScatter
    -> 遍历 connectedRank
    -> 每个 peer 只交换/规约目标 slice
    -> SendRecvBatchWriteReduce 或相关 send/recv-reduce
 -> PostCopy
    -> 如果规约结果在 scratch，把 myRank slice 拷到 output
    -> FP64/PROD 等场景可能用 LocalReduce 补充规约
```

Mesh 适合全连接或 rank 数较小的 1D 拓扑。它的通信语义是每个 rank 只关心自己的输出 slice，但会和其他 rank 交换对应 slice 并做 reduce。

`InsTempReduceScatterMesh1DMeshChunk` 是大数据分块版本：

```text
PreCopy: user input -> scratch
RunReduceScatter:
  -> CalcSliceInfoVec
  -> DoMeshChunk
  -> SendRecvBatchWriteReduce
PostCopy: scratch[myRank] -> output
```

它通过 chunk/slice 降低单次 UB/CCL buffer 压力，适合 selector 中的大数据 Mesh 场景。

CCU Mesh 与 AICPU Mesh 的算法语义一致，但 host 侧只准备 `CcuKernelArgReduceScatterMesh1D`、channel 和 task args，真正搬运由 `CcuReduceScatterMesh1DKernel` 或 `*Mem2Mem*` kernel 完成。AIV Mesh 则把输入输出地址、rank 拓扑、`count`、`reduceType`、stride/repeat 写入 `AivOpArgs` 后调用 `ExecuteKernelLaunch`。

### NHR

实现文件：

```text
template/aicpu/ins_temp_reduce_scatter_nhr.cc
template/ccu/ccu_temp_reduce_scatter_nhr_1D_mem2mem.cc
template/ccu/ccu_temp_reduce_scatter_nhr_1D_multi_jetty_mem2mem.cc
```

NHR 按递归减半/超立方思想展开 ReduceScatter。步数为：

```text
nSteps = ceil(log2(templateRankSize))
```

每一步由 `GetStepInfoList` 计算：

```text
deltaRank = 1 << step 或对应反向步长
recvFrom / sendTo = 按 myAlgRank 和 deltaRank 环形计算
txSliceIdx / rxSliceIdx = 本轮发送和接收规约的 slice 集合
```

AICPU NHR 主流程：

```text
KernelRun
 -> 可选 LocalDataCopy: input -> scratch
 -> for channelIdx:
      for step in nSteps:
          GetStepInfoList
          构造 tx/rx DataSlice
          SendRecvReadReduce 或 SendRecvBatchWriteReduce
      PostLocalCopy: scratch -> output
```

NHR 的特点：

- 通信步数约为 `log2(rankSize)`，比 Mesh 全连接遍历更适合大 rank 数或非全连接拓扑。
- 每一步发送自己当前负责的一部分 slice，接收对端 slice 后立即规约，逐步缩小到本 rank 结果。
- 支持多 channel 数据切分，`channelsPerRank` 可让同一个 peer 使用多条链路。
- CCU NHR 会预先计算 step info 并填入 `CcuKernelArgReduceScatterNHR1D`，由 `CcuReduceScatterNHR1DMem2MemKernel` 执行。

### AicpuReduceNHR

实现文件：

```text
template/aicpu/ins_temp_reduce_scatter_aicpu_reduce_nhr.cc
```

该路径主要服务 64-bit 数据类型或 `PROD` 等普通 engine reduce 支持受限的场景。核心思路不是在通信过程中做 reduce，而是：

```text
KernelRun
 -> LocalDataCopy: 把本 rank 目标 slice 放入 scratch
 -> RunAllGather: 用 NHR 样式把所有 rank 的目标 slice 收集到本地 scratch
 -> 同步通信任务完成
 -> PostLocalReduce: 在本地按 rank 顺序执行 LocalReduce
 -> LocalCopyToOutput: 输出 myRank 结果
```

这种路径牺牲部分通信融合能力，换取对特殊数据类型和 reduce op 的兼容性。

### OrderPreserved

实现文件：

```text
template/aicpu/ins_temp_reduce_scatter_order_preserved_level1.cc
template/aicpu/ins_temp_reduce_scatter_order_preserved_group.cc
```

保序模板分三步：

```text
PreLocalCopy:
  -> 按 MemBlockInfo 把用户输入各 rank block 拷到 scratch
RunAllToAll:
  -> 以固定 round/localRank 顺序交换 block
RunLocalReduce:
  -> 用树形 reduce 固定规约顺序
PostCopy:
  -> 把最终 block 拷到 recvBuf
```

`RunLocalReduce` 使用 `GetLargestPowerOfTwoLessThan` 计算每轮规约跨度，按虚拟 rank 顺序做 `LocalReduce`。这条路径的重点是确定性，不是极致吞吐。

### OmniPipe Template

实现文件：

```text
template/aicpu/ins_temp_reduce_scatter_omnipipe_mesh_1D.cc
template/aicpu/ins_temp_reduce_scatter_omnipipe_nhr.cc
template/ccu/ccu_temp_reduce_scatter_omnipipe_*.cc
```

OmniPipe template 的输入不是完整用户 buffer 语义，而是 executor 算出的 `StepSliceInfo`。每个 step 只处理当前 pipeline slice，通常在 CCL scratch 内读写。这样可以把 level0/level1/level2 的通信重叠成流水，降低大数据多级拓扑的尾延迟。

### AIV / CCU Kernel 路径

AIV template：

```text
AivTempReduceScatterMesh1D::KernelRun
 -> 填充 AivOpArgs:
      cmdType = HCCL_CMD_REDUCE_SCATTER
      input/output
      rank/rankSize/topo
      count/dataType/op
      inputSliceStride/outputSliceStride/repeat
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

CCU 路径对数据类型、inplace 和保序模式限制更多，但在支持范围内可以减少 host 编排开销，并通过 fast launch 复用已构造的 kernel 参数。

## 关键理解点

- `recvCount` 是每个 rank 最终输出的元素数，`sendBuf` 总元素数是 `recvCount * rankSize`。
- `OpParam.inputSize = outputSize * rankSize`，`OpParam.outputSize = outputSize`。
- `inputSliceStride` 通常等于单 rank 输出大小，用来在 `sendBuf` 中跨 rank slice 定位。
- 单 rank 场景不进复杂算法，直接走 `SingleRankProc`。
- 普通高性能路径尽量在通信中做 reduce；`AicpuReduceNHR` 和保序路径会显式拆成收集/交换加本地 reduce。
- Selector 的核心分流依据是 engine、拓扑层级、`MESH_1D/CLOS/MESH_1D_CLOS` 形态、数据量、数据类型、reduce op、inplace 和严格确定性要求。
