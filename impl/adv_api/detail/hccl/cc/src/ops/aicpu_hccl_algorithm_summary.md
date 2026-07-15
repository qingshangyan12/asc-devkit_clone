# AICPU 场景 HCCL 算法逻辑与调用链总结

本文基于 `impl/adv_api/detail/hccl/cc/src/ops` 下的 AllGather、ReduceScatter 和公共执行框架代码整理，重点说明 AICPU 场景下的算法选择、资源创建、AICPU kernel 下发以及模板执行逻辑。

## 1. 目录职责

- `all_gather/`：AllGather 对外入口、算法选择器、executor 与 AICPU/CCU 模板。
- `reduce_scatter/`：ReduceScatter 对外入口、算法选择器、executor 与 AICPU/CCU 模板。
- `op_common/`：公共入口编排，包含 selector、executor registry、资源上下文、线程/通道申请、AICPU kernel 加载与 launch。
- `op_common/template/aicpu/`：AICPU kernel 入口、二进制加载、device 侧反序列化和执行。
- `op_common/topo/`：拓扑提取和匹配，输出 selector/executor 使用的 `TopoInfoWithNetLayerDetails` 与分层通信域信息。
- `interface_graph_mode/`：图模式资源计算接口，目前 AICPU 离线资源固定申明 `taskNum = 3`。

## 2. AICPU 路径启用条件

AllGather 和 ReduceScatter 的公开入口默认先判断开关、版本和设备型号：

1. `HCCL_ENABLE_OPEN_AICPU=1` 或 `HCCL_ENABLE_OPEN_CCU=1` 未打开时，直接回退 `HcclAllGatherInner` / `HcclReduceScatterInner`。
2. `GetHcommVersion() < 90000000` 时回退 inner。
3. 设备类型不是 950/910_95 时回退 inner。
4. 入参、count、dtype、rank 合法性校验通过后进入 `Selector -> HcclExecOp` 新框架。

入口位置：

- `all_gather/all_gather_op.cc`
- `reduce_scatter/reduce_scatter_op.cc`

## 3. Host 侧主调用链

### 3.1 OPBASE 调用链

AllGather：

```text
HcclAllGather
  -> AllGatherInitAndCheck
  -> AllGatherOutPlace
  -> AllGatherOutPlaceCommon
  -> Selector
  -> HcclExecOp
```

ReduceScatter：

```text
HcclReduceScatter
  -> InitEnvConfig / CheckReduceScatterInputPara / CheckDataType
  -> ReduceScatterOutPlace
  -> PrepareReduceScatterParam
  -> Selector
  -> HcclExecOp
```

### 3.2 Graph Mode 调用链

AllGather：

```text
HcclAllGatherGraphMode
  -> HcomGetCommHandleByGroup
  -> AllGatherOutPlaceGraphMode
  -> AllGatherOutPlaceCommon(..., OpMode::OFFLOAD, resPack)
  -> Selector
  -> HcclExecOp
```

ReduceScatter：

```text
HcclReduceScatterGraphMode
  -> HcomGetCommHandleByGroup
  -> ReduceScatterOutPlaceGraphMode
  -> PrepareReduceScatterParam(..., OpMode::OFFLOAD)
  -> Selector
  -> HcclExecOp(..., resPack)
```

Graph Mode 通过 `ResPackGraphMode` 传入 tag、外部 streams、scratchMem。建链时 `HcclGetChannelImpl` 会调用 `RegGraphModeBuffers` 注册 input/output buffer，再通过 `GetGraphModeBuffers` 获取远端图模式 buffer 地址。

## 4. Selector 逻辑

公共入口在 `op_common/op_common.cc::Selector`：

1. 设置 `param.hcclComm`。
2. 调用 `HcclGetOpExpansionMode`，当前默认应用 `HCCL_OP_EXPANSION_MODE_AI_CPU`，把 `param.opExecuteConfig` 设为 `AICPU_TS`，`param.engine` 设为 `COMM_ENGINE_AICPU_TS`，并加载 AICPU kernel。
3. 调用 `HcclCalcTopoInfo` 获取拓扑。
4. `CheckAsymmetricTopoSupport` 限制跨框非对称拓扑仅 AllGather/AllReduce/ReduceScatter 支持。
5. `ExecuteSelector::Run` 执行算法选择。
6. `SetCommEngine` 根据最终 `opExecuteConfig` 写回 `param.engine`。
7. AICPU/CPU engine 下再次 `LoadAICPUKernel`，该函数内部防重复加载。
8. `SetOpParamAlgTag` 生成资源复用 tag：`param.tag + algName + device/host`。

`ExecuteSelector::Run` 先判断是否走 CANN bridge；否则按 `opType` 从 `SelectorRegistry` 找 selector。AllGather 和 ReduceScatter 都以优先级 18 注册：

```text
REGISTER_SELECTOR_BY_OPTYPE(HCCL_CMD_ALLGATHER, 18, AllGatherAutoSelector)
REGISTER_SELECTOR_BY_OPTYPE(HCCL_CMD_REDUCE_SCATTER, 18, ReduceScatterAutoSelector)
```

`AutoSelectorBase::Select` 的顺序：

1. 检查 Host DPU only 场景，满足时切到 `HOSTCPU`。
2. 尝试 CCU_MS，不匹配则 fallback 到 CCU_SCHED。
3. 尝试 CCU_SCHED，不匹配则进入 `CCU_FAIL`。
4. 处理 AIV 配置。
5. 当状态是 `AICPU_TS`、`HOSTCPU_TS` 或 `CCU_FAIL` 时，调用算子自己的 `SelectAicpuAlgo`。

## 5. AICPU 资源创建链路

`HcclExecOp` 先按 `algName` 从 `CollAlgExecRegistryV2` 取 executor，然后走资源创建：

```text
HcclExecOp
  -> HcclThreadAcquireWithStream(COMM_ENGINE_CPU_TS, param.stream)
  -> HcclThreadExportToCommEngine(..., COMM_ENGINE_AICPU_TS)
  -> HcclGetAlgRes
       -> executor->CalcAlgHierarchyInfo
       -> executor->CalcRes
       -> GetAlgResAICPU
            -> HcclAllocAlgResourceAICPU
                 -> HcclGetHcclBuffer
                 -> HcclGetThread
                 -> HcclGetChannel
            -> HcclMemcpyCtxHostToDevice
  -> HcclDfxRegOpInfo
  -> GetMainThreadInfo / GetUnfoldThreadInfo
  -> CaptureSlaveStreams
  -> HcclAicpuKernelEntranceLaunch
```

### 5.1 资源请求

executor 的 `CalcRes` 产出 `AlgResourceRequest`：

- `slaveThreadNum`：除主 thread 外需要的从 thread 数。
- `notifyNumOnMainThread`：主 thread 上的 notify 数。
- `notifyNumPerThread`：每个从 thread 上的 notify 数。
- `channels`：按逻辑层级组织的 `HcclChannelDesc`。

### 5.2 资源上下文

AICPU 使用 `AlgResourceCtxSerializable` 作为可序列化上下文，核心字段包括：

- `algHierarchyInfo`：各层通信域 rank 分组。
- `cclMem`：通信域 CCL buffer，作为 scratch buffer。
- `threads`：AICPU_TS 主 thread 和从 thread。
- `unfoldThread`：提前展开使用的 CPU thread。
- `channels`：每层建链后的 `ChannelInfo`。
- `topoInfo`：拓扑详情。

`HcclMemcpyCtxHostToDevice` 将该结构序列化后，通过 `HcclEngineCtxCreate(..., COMM_ENGINE_AICPU_TS, ...)` 创建 device engine ctx，再用 `HcclEngineCtxCopy` 拷贝到 device。

### 5.3 Thread 和同步

AICPU/CPU engine 下：

- `HcclGetThread` 申请 `slaveThreadNum + 1` 个 `COMM_ENGINE_AICPU_TS` thread，`threads[0]` 是 device 主 thread。
- 主 thread 的 notify 数取 `notifyNumOnMainThread` 和所有 `notifyNumPerThread` 的最大值，再额外加 1 个 host-device 同步 notify。
- `SaveMainThreadInfo` 用 `COMM_ENGINE_CPU_TS` ctx 保存 device 主 thread 和 notify 数。
- 另申请 `COMM_ENGINE_CPU` unfold thread，并用 `SaveUnfoldThreadInfo` 保存。
- launch 前 host 侧通过 `HcommThreadNotifyRecordOnThread(cpuTsThread, exportedCpuTsThread, notifyNumOnMainThread - 1)` 通知 device 主 thread。
- AICPU kernel 执行完成后，device 主 thread 在 notify 0 上通知 host，host 侧 `HcommThreadNotifyWaitOnThread(cpuTsThread, 0, timeout)` 等待。

### 5.4 Channel

`HcclGetChannel` 按层遍历 `resRequest.channels`，将请求拆为 device endpoint 和 host endpoint：

- device endpoint 使用 `COMM_ENGINE_AICPU_TS` 建链。
- host endpoint 使用 `COMM_ENGINE_CPU` 建链。

建链后填充 `ChannelInfo`：

- `remoteRank`
- `protocol`
- `locationType`
- `notifyNum`
- `handle`
- `remoteCclMem`
- Graph Mode 下的 `remoteInputGraphMode` / `remoteOutputGraphMode`
- 非 AICPU 编译侧还会从 rank graph 查询 `portGroupSize`，用于按端口组切分数据。

## 6. AICPU kernel 侧执行链

Host 侧 `AicpuKernelLaunch` 固定下发 kernel 名 `HcclLaunchAicpuKernel`：

```text
AicpuKernelLaunch
  -> aclrtBinaryGetFunction(g_binKernelHandle, "HcclLaunchAicpuKernel")
  -> aclrtKernelArgsInit
  -> aclrtKernelArgsAppend(&param, sizeof(OpParam) + param.varMemSize)
  -> aclrtKernelArgsFinalize
  -> aclrtLaunchKernelWithConfig
```

kernel 文件通过 `LoadAICPUKernel` 从 `${ASCEND_HOME_PATH}/opp/built-in/op_impl/aicpu/config/libmc2_server.json` 加载；未设置 `ASCEND_HOME_PATH` 时默认 `/usr/local/Ascend/cann/`。

Device 侧入口在 `op_common/template/aicpu/kernel_launch.cc::HcclLaunchAicpuKernel`：

1. `HcommAcquireComm(param->commName)` 获取通信域。
2. 950/910_95 路径下：
   - 从 `param->resCtx` 反序列化 `AlgResourceCtxSerializable`，并用 thread-local `CommDomainCacheManager` 按 `algTag` 缓存。
   - 恢复 AllToAllV、ReduceScatterV、AllGatherV 等变长参数。
   - 取 `resCtx.threads[0]` 作为 device 主 thread。
   - `HcommBatchModeStart(param->algTag)`。
   - device 主 thread 等待 host 通知。
   - `CollAlgExecRegistryV2::Instance().GetAlgExec(param->opType, algName)` 找 executor。
   - `executor->Orchestrate(*param, resCtx)` 执行算法编排。
   - profiling 上报。
   - device 主 thread notify host。
   - `HcommBatchModeEnd(param->algTag)`。

也就是说，host 侧只负责选择算法、准备资源并 launch 一个 AICPU kernel；真实的 HCCL 指令编排在 AICPU kernel 内通过同一套 V2 executor 完成。

## 7. AllGather AICPU 算法选择

选择器：`all_gather/selector/all_gather_auto_selector.cc::SelectAicpuAlgo`。

| 拓扑/条件 | 选择算法 |
| --- | --- |
| 多层拓扑且 `Level1Nhr` | `InsAllGatherNHR` |
| 多层拓扑且 `Level0Nhr` | `InsAllGatherNHR` |
| 多层拓扑且 level0 本地实例大小为 1 | `InsAllGatherNHR` |
| 多层拓扑且 level0 是 `MESH_1D` | `InsAllGatherParallelMesh1DNHR` |
| 单层 `MESH_1D` | `InsAllGatherMesh1D` |
| 单层 `MESH_1D_CLOS`，mesh 数等于 clos 数且 rank 数不超过 4，小数据 | `InsAllGatherMesh1D` |
| 单层 `MESH_1D_CLOS`，mesh 数等于 clos 数且 rank 数不超过 4，大数据 | `InsAllGatherConcurrentMesh1DNHR` |
| 单层 `MESH_1D_CLOS`，clos 数是 mesh 数倍数且大数据 | `InsAllGatherParallelMesh1DNHRUBX` |
| 单层 `MESH_1D_CLOS` 其他场景 | `InsAllGatherNHR` |
| 单层 `CLOS` | `InsAllGatherNHR` |

AllGather 的主要模板：

### 7.1 `InsTempAllGatherMesh1D`

文件：`all_gather/template/aicpu/ins_temp_all_gather_mesh_1D.cc`

资源：

- 线程数为 `rankSize - 1`，每个对端一个 thread。
- 通过 `CalcChannelRequestMesh1D` 申请 level0 channel。
- OPBASE 下 scratch multiple 为 `rankSize`。

执行：

1. `LocalDataCopy` 把本 rank 输入 slice 放到输出对应位置；非 remote memory/symmetric memory 场景还会放入 scratch。
2. 多 thread 时主 thread 通过 notify 唤醒从 thread。
3. `RunAllGatherMesh` 遍历所有对端 rank，每个对端构造一组 tx/rx slices：
   - tx：本地 output slice 写到远端 scratch 或远端 output。
   - rx：从远端 scratch 或远端 output 读回本地 output 对端 slice。
   - 支持 symmetric memory 时使用 `HcclSymWinGetPeerPointer` 直接取得远端 input/output 指针。
4. 调用 `SendRecvRead` 完成收发。
5. 从 thread 完成后 notify 主 thread。

### 7.2 `InsTempAllGatherNHR`

文件：`all_gather/template/aicpu/ins_temp_all_gather_nhr.cc`

资源：

- 只需要 1 个主 thread。
- 通过 `CalcChannelRequestNhr` 申请 NHR channel。
- scratch multiple 为 `rankSize`。

执行：

1. `LocalDataCopy` 将本 rank 输入 slice 拷贝到 scratch 对应位置。
2. `RunAllGatherNHR` 执行 `ceil(log2(rankSize))` 轮 NHR 通信。
3. 每轮 `GetStepInfo` 计算：
   - `fromRank = myRank - deltaRank`
   - `toRank = myRank + deltaRank`
   - 本轮需要发送/接收的 slice 列表。
4. 使用 `SendRecvWrite` 在 scratch 与远端 scratch 间交换数据。
5. `PostLocalCopy` 将 scratch 中完整 AllGather 结果拷贝到 output。

## 8. ReduceScatter AICPU 算法选择

选择器：`reduce_scatter/selector/reduce_scatter_auto_selector.cc::SelectAicpuAlgo`。

关键阈值：

- `RS_AICPU_1D_MIN_DATA_SIZE = 4 MiB`
- `RS_AICPU_1D_MAX_DATA_SIZE = 16 MiB`
- `RS_AICPU_1D_TWO_LEVER_DATA_SIZE_THRESHOLD = 1536 MiB`
- `RS_AICPU_SEQUENCE_SIZE_THRESHOLD = 1024 MiB`

多层拓扑：

| 条件 | 选择算法 |
| --- | --- |
| 64-bit dtype 或 `HCCL_REDUCE_PROD` | `InsReduceScatterAicpuReduceNHR` |
| `Level1Nhr` | `InsReduceScatterNHR` |
| `Level0Nhr` | `InsReduceScatterNHR` |
| level0 本地实例大小 > 1 且 level0 为 `MESH_1D`，数据大于 4 MiB，且总数据大于 1024 MiB | `InsReduceScatterSequenceMesh1DNhr` |
| level0 本地实例大小 > 1 且 level0 为 `MESH_1D`，数据大于 4 MiB，且总数据不超过 1024 MiB | `InsReduceScatterParallelMesh1DNHR` |
| level0 本地实例大小 > 1 且数据不超过 4 MiB | `InsReduceScatterNHR` |
| level0 本地实例大小为 1 或 level0 为 `CLOS` | `InsReduceScatterNHR` |

单层拓扑：

| 拓扑/条件 | 选择算法 |
| --- | --- |
| `MESH_1D`，64-bit dtype 或 `PROD` | `InsReduceScatterMesh1D` |
| `MESH_1D`，两级 net layer 且总数据大于 1536 MiB | `InsReduceScatterMesh1DZAxisDetour` |
| `MESH_1D`，按 rank 数缩放后大于 16 MiB | `InsReduceScatterMesh1DMeshChunk` |
| `MESH_1D` 其他场景 | `InsReduceScatterMesh1D` |
| `CLOS`，64-bit dtype 或 `PROD` | `InsReduceScatterAicpuReduceNHR` |
| `CLOS` 其他场景 | `InsReduceScatterNHR` |
| `MESH_1D_CLOS` 且 PCIE mix，全连 1D mesh | `InsReduceScatterMesh1D` |
| `MESH_1D_CLOS` 且 PCIE mix，64-bit dtype 或 `PROD` | `InsReduceScatterAicpuReduceNHR` |
| `MESH_1D_CLOS` 且 PCIE mix 其他场景 | `InsReduceScatterParallelMesh1DNHRPcie` |
| `MESH_1D_CLOS` 且 1D mesh 可覆盖所有 rank | `InsReduceScatterMesh1D` |
| `MESH_1D_CLOS`，64-bit dtype 或 `PROD` | `InsReduceScatterAicpuReduceNHR` |
| `MESH_1D_CLOS`，clos 数是 mesh 数倍数且非小数据 | `InsReduceScatterParallelMesh1DNHRUBX` |
| `MESH_1D_CLOS` 其他场景 | `InsReduceScatterNHR` |

## 9. ReduceScatter 主要模板逻辑

### 9.1 `InsTempReduceScatterMesh1D`

文件：`reduce_scatter/template/aicpu/ins_temp_reduce_scatter_mesh_1D.cc`

资源：

- thread 数为 `rankSize`，其中主 thread + `rankSize - 1` 个从 thread。
- 普通 `MESH_1D` 通过 `CalcChannelRequestMesh1D` 建链。
- `MESH_1D_CLOS` 且非 PCIE mix 时优先 CLOS 拓扑，并过滤 `COMM_PROTOCOL_UBC_CTP` channel。
- scratch multiple 为 `rankSize`。

执行：

1. 多 thread 时主 thread 通过 notify 唤醒从 thread。
2. `RunReduceScatter` 对每个非本 rank slice：
   - 根据 port group 将数据切分到多个 channel。
   - 把本 rank input 中属于远端 rank 的 slice 写到远端 CCL buffer。
   - 从远端 CCL buffer 接收对方写入本 rank slice 的数据。
   - 使用 `SendRecvWrite` 下发通信。
3. 必要时对 64-bit dtype 或 `PROD` 做 batch mode end/start 和 thread join，确保通信完成后再规约。
4. `PostCopy` 先把本 rank 自己的 slice 从 input 拷到 output，再把 scratch 中其他 rank 对应 slice 用 `LocalReduce` 规约到 output。

### 9.2 `InsTempReduceScatterNHR`

文件：`reduce_scatter/template/aicpu/ins_temp_reduce_scatter_nhr.cc`

资源：

- 只需要 1 个主 thread。
- 通过 `CalcChannelRequestNhr` 建链。
- scratch multiple 为 `rankSize`。

执行：

1. `LocalDataCopy` 将所有本地输入 slice 拷贝到 scratch。
2. `RunNHR` 执行 `ceil(log2(rankSize))` 轮通信。
3. `GetStepInfoList` 对每轮计算发送 rank、接收 rank 和 slice 列表。
4. 每轮构造 `SendRecvReduceInfo`，调用 `SendRecvWriteReduce`：
   - tx 从本地 scratch 写到远端 scratch。
   - rx/reduce 在本地 scratch 上进行规约。
5. `PostLocalCopy` 将本 rank 最终 slice 从 scratch 拷贝到 output。

### 9.3 `InsTempReduceScatterAicpuReduceNHR`

文件：`reduce_scatter/template/aicpu/ins_temp_reduce_scatter_aicpu_reduce_nhr.cc`

适用：64-bit dtype 或 `HCCL_REDUCE_PROD` 等需要 AICPU 本地规约的场景。

执行：

1. `LocalDataCopy` 把 input 拷贝到 scratch。
2. `RunAllGather` 用 NHR 模式执行 scratch 到 scratch 的 AllGather，通信中不做 reduce。
3. 强制 batch mode end/start，并 `HcommThreadJoin`，确保所有通信任务完成。
4. `PostLocalReduce` 在 AICPU 上用 `LocalReduce` 对 scratch 中各 rank 的 slice 做本地规约。
5. 必要时 `LocalCopyToOutput` 将结果写到 output。

### 9.4 `InsV2ReduceScatterSequenceExecutorAicpu`

文件：`reduce_scatter/executor/ins_v2_reduce_scatter_sequence_executor_aicpu.cc`

适用：多层拓扑大数据 `InsReduceScatterSequenceMesh1DNhr`。

注册：

```text
InsReduceScatterSequenceMesh1DNhr
  -> InsV2ReduceScatterSequenceExecutorAicpu
  -> TopoMatchMultilevel
  -> InsTempReduceScatterMesh1DZAxisDetour
  -> InsTempReduceScatterNHR
```

逻辑：

1. `CalcAlgHierarchyInfo` 匹配两层拓扑。
2. `CalcRes` 分别计算 intra template 和 inter template 的资源，再取 thread/notify 最大值；channel level 0 使用 intra，level 1 使用 inter。
3. `OrchestrateLoop` 用 CCL buffer 承接两阶段数据：
   - intra：`INPUT -> HCCL_BUFFER`
   - inter：`HCCL_BUFFER -> OUTPUT`
4. 根据 CCL buffer 大小和两个 template 的 scratch multiple 计算 `maxCountPerLoop`，分 loop 处理大数据。
5. 每个 loop 先执行 intra，再执行 inter。

## 10. Executor 组合关系

V2 executor 通常由拓扑匹配器和一个或两个 template 组成。典型模式：

- sole executor：单 template 完成整个算法。
- sequence executor：先执行一个 template，再执行另一个 template，适合多层拓扑分阶段。
- parallel/concurrent executor：两个 template 按数据切分或拓扑层级并发/交错执行，并通过 notify 在控制 thread 与 template 主 thread 间同步。

已从源码确认的 AICPU 相关注册包括：

- AllGather:
  - `InsAllGatherMesh1D`
  - `InsAllGatherNHR`
  - `InsAllGatherParallelMesh1DNHR`
  - `InsAllGatherParallelMesh1DNHRUBX`
  - `InsAllGatherConcurrentMesh1DNHR`
  - `InsAllGatherMeshNhrDPU`
- ReduceScatter:
  - `InsReduceScatterMesh1D`
  - `InsReduceScatterMesh1DMeshChunk`
  - `InsReduceScatterMesh1DZAxisDetour`
  - `InsReduceScatterNHR`
  - `InsReduceScatterAicpuReduceNHR`
  - `InsReduceScatterParallelMesh1DNHR`
  - `InsReduceScatterParallelMesh1DNHRUBX`
  - `InsReduceScatterParallelMesh1DNHRPcie`
  - `InsReduceScatterSequenceMesh1DNhr`

## 11. 关键数据流

AllGather：

```text
input[myRank slice]
  -> local output 或 scratch
  -> Mesh1D: 直接和每个 peer 交换，结果落 output
  -> NHR: 多轮 scratch 交换，最终 scratch 拥有完整数据
  -> output
```

ReduceScatter Mesh1D：

```text
input[all rank slices]
  -> 每个 rank 将对端目标 slice 写到对端 CCL buffer
  -> 本 rank 从 CCL buffer 收到其他 rank 写来的本 rank slice
  -> output 先放本 rank 自身 slice
  -> scratch 中其他 rank slice LocalReduce 到 output
```

ReduceScatter NHR：

```text
input
  -> scratch
  -> NHR 多轮 SendRecvWriteReduce，在通信过程中规约
  -> output[myRank slice]
```

ReduceScatter AicpuReduceNHR：

```text
input
  -> scratch
  -> NHR AllGather 到 scratch，不在通信中规约
  -> 等待所有通信任务完成
  -> AICPU LocalReduce
  -> output
```

## 12. 源码索引

- 入口：
  - `all_gather/all_gather_op.cc`
  - `reduce_scatter/reduce_scatter_op.cc`
- 公共执行：
  - `op_common/op_common.cc`
  - `op_common/op_common.h`
  - `op_common/inc/alg_param.h`
- Selector：
  - `op_common/selector/execute_selector.cc`
  - `op_common/selector/auto_selector_base.cc`
  - `all_gather/selector/all_gather_auto_selector.cc`
  - `reduce_scatter/selector/reduce_scatter_auto_selector.cc`
- AICPU kernel：
  - `op_common/template/aicpu/load_kernel.cc`
  - `op_common/template/aicpu/kernel_launch.cc`
- AllGather AICPU template：
  - `all_gather/template/aicpu/ins_temp_all_gather_mesh_1D.cc`
  - `all_gather/template/aicpu/ins_temp_all_gather_nhr.cc`
  - `all_gather/template/aicpu/ins_temp_all_gather_nhr_dpu.cc`
- ReduceScatter AICPU template：
  - `reduce_scatter/template/aicpu/ins_temp_reduce_scatter_mesh_1D.cc`
  - `reduce_scatter/template/aicpu/ins_temp_reduce_scatter_mesh_1D_meshchunk.cc`
  - `reduce_scatter/template/aicpu/ins_temp_reduce_scatter_mesh_1D_Z_axis_detour.cc`
  - `reduce_scatter/template/aicpu/ins_temp_reduce_scatter_nhr.cc`
  - `reduce_scatter/template/aicpu/ins_temp_reduce_scatter_aicpu_reduce_nhr.cc`
  - `reduce_scatter/executor/ins_v2_reduce_scatter_sequence_executor_aicpu.cc`
