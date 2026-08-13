# InsAllReduceMesh1DTwoShot mc2 迁移方案

## 1. 迁移目标

将 `D:/code/comm/hccl` 中标准版 `InsAllReduceMesh1DTwoShot` 迁移到 `asc-devkit` 的 mc2 HCCL 实现，使 mc2 AllReduce 在支持的 AICPU Mesh1D 场景使用该算法。

本次范围严格限定如下：

- 迁移 `InsTempAllReduceMesh1DTwoShot`、`InsV2AllReduceSoleExecutor` 中标准 TwoShot 所需逻辑及算法注册。
- 不迁移 `InsAllReduceMesh1DTwoShotMeshChunk`。
- 不迁移 `InsAllReduceMesh1DTwoShotZAxisDetour`。
- 不迁移 OneShot、NHR、并行或多层 AllReduce 算法。
- 不新增独立 opbase AllReduce 接口，复用 mc2 已有的 tiling、资源创建和 AICPU KFC 执行入口。
- 算法实现保持源仓库的切片、通信、归约和线程同步结构，不在迁移过程中进行算法优化或重构。

## 2. 迁移内容

### 2.1 Template

新增：

- `impl/adv_api/detail/hccl/cc/src/ops/all_reduce/template/aicpu/ins_temp_all_reduce_mesh_1D_two_shot.h`
- `impl/adv_api/detail/hccl/cc/src/ops/all_reduce/template/aicpu/ins_temp_all_reduce_mesh_1D_two_shot.cc`

保留的原始实现逻辑：

1. `CalcScratchMultiple` 申请 2 倍 scratch 空间，覆盖非均匀切片场景。
2. `CalcRes` 通过 `CalcChannelRequestMesh1D` 申请 Mesh1D channel。
3. 每个 rank 使用一个线程，主从线程通过 notify 同步。
4. `SplitData` 按 rank 数对输入 count 向上取整切片，并保留尾部零长度切片处理。
5. ReduceScatter 阶段通过 `SendRecvBatchWrite` 将目标切片汇聚到本地 HCCL buffer，再按固定顺序执行 `LocalReduce`。
6. INT64、UINT64、FP64 或 PROD 场景在本地归约前执行 batch mode 切换和线程 join。
7. AllGather 阶段通过 `SendRecvBatchRead` 将各 rank 已归约切片写回完整输出。

仅进行以下适配：

- 命名空间由 `ops_hccl` 改为 `mc2_ops_hccl`。
- 使用 `asc-devkit` mc2 HCCL 已有公共头文件和构建目标。
- `KernelRun` 的资源参数适配为 `const TemplateResource&`，与 `asc-devkit` 的 `InsAlgTemplateBase` 接口一致；算法未修改资源对象。

### 2.2 Executor

新增：

- `impl/adv_api/detail/hccl/cc/src/ops/all_reduce/executor/ins_v2_all_reduce_sole_executor.h`
- `impl/adv_api/detail/hccl/cc/src/ops/all_reduce/executor/ins_v2_all_reduce_sole_executor.cc`

Executor 保留源实现的编排方式：

1. `TopoMatch1D` 计算 0 级通信域。
2. `CalcRes` 实例化标准 TwoShot template 并计算线程、notify、channel 资源。
3. `Orchestrate` 从序列化资源中恢复 channel 和 thread。
4. `OrchestrateLoop` 根据 HCCL buffer 容量及 2 倍 scratch 要求计算单轮 count。
5. 大数据按轮次执行同一个标准 TwoShot template，不切换到 MeshChunk。
6. 仅注册以下映射：

```text
(HCCL_CMD_ALLREDUCE, InsAllReduceMesh1DTwoShot)
    -> InsV2AllReduceSoleExecutor<TopoMatch1D, InsTempAllReduceMesh1DTwoShot>
```

没有注册 MeshChunk 和 ZAxisDetour。

### 2.3 Selector

新增：

- `impl/adv_api/detail/hccl/cc/src/ops/all_reduce/selector/all_reduce_auto_selector.h`
- `impl/adv_api/detail/hccl/cc/src/ops/all_reduce/selector/all_reduce_auto_selector.cc`

mc2 创建通信资源时，`PrepareParamForAllReduce` 将 `DataDes.count` 初始化为 0，真实 count 在设备运行时由通信消息写回。因此不能直接沿用源仓库按照数据量选择 OneShot、标准 TwoShot、MeshChunk 或 ZAxisDetour 的逻辑，否则资源创建阶段会因 `count=0` 选择 OneShot。

本次适配在受支持的 mc2 AICPU 拓扑中固定返回 `InsAllReduceMesh1DTwoShot`：

- 支持单网络层 `MESH_1D`。
- 支持单网络层、Mesh 链路可连接全部 rank 的 PCIE mixed `MESH_1D_CLOS`。
- 其他拓扑返回 `NOT_MATCH`，不以标准 TwoShot 错误覆盖多层或非 Mesh 通信语义。
- CCU、AIV 和 DPU 路径未新增 AllReduce 算法，最终仅 AICPU 路径可匹配。

## 3. mc2 调用流程

### 3.1 Host 侧资源创建

设置Hccl高阶API Tiling结构体。

根据通信任务类型、算法配置等，创建一个Mc2CcTilingConfig类对象，通过向[GetTiling](../../../../../api/SIMD-API/高阶API/HCCL通信类/HCCL-Tiling侧接口/GetTiling-100.md)方法传入算子Tiling结构体中mc2InitTiling和mc2CcTiling成员的引用，完成需要传递给Kernel侧的Mc2InitTiling参数和Mc2CcTiling参数的获取。Hccl高阶API Tiling结构体的具体使用方法详见[Hccl Tiling使用说明](../../../../../api/SIMD-API/高阶API/HCCL通信类/HCCL-Tiling侧接口/HCCL-Tiling使用说明.md)。

```text
uint32_t opType = HCCL_CMD_ALLREDUCE;
std::string algConfig = ""; // 空字符串表示使用默认 selector
uint32_t reduceType = HCCL_REDUCE_SUM;
AscendC::Mc2CcTilingConfig mc2CcTilingConfig(group, opType, algConfig, reduceType);
mc2CcTilingConfig.GetTiling(tiling->mc2InitTiling);
mc2CcTilingConfig.GetTiling(tiling->mc2CcTiling);
```

mc2 算子初始化通信资源时的调用链如下：
参考：docs\zh\api\SIMD-API\高阶API\HCCL通信类\HCCL-Tiling侧接口\HCCL-Tiling使用说明.md

```text
Mc2CcTilingConfig / Mc2CcTiling：生成并保存 mc2 初始化参数和 AllReduce 通信参数。
  -> HcclAllocComResourceByTiling：解析 tiling，并根据通信引擎进入对应的资源分配流程。
    -> AllocComResourceByTilingAicpu：为 AICPU 执行流程构造算子参数并申请通信资源。
      -> GetOpParam：根据 tiling 初始化 `OpParam`，完成算法选择和资源上下文创建。
        -> InitOpParamByTiling：将 AllReduce 类型、数据类型、归约类型和通信配置写入 `OpParam`。
          -> PrepareParamForAllReduce：补充 AllReduce 专用参数，并将资源阶段的 `DataDes.count` 初始化为 0。
        -> TryForcedAlgAndPrepareEngine  指定算法名称，走此路径
              -> GetForcedAlgName
                  -> forcedAlgName = InsAllReduceMesh1DTwoShot
              -> 备份 OpParam
              -> PrepareTopoInfoForOp
              -> PrepareEngineForAlg
              -> CheckForcedAlgResource
                  -> GetAlgExecutorForOp
                    -> UseCannBridge：判断 Host 资源计算是否交给外部 CANN registry。
                      -> true：GetAlgExecViaCann，从外部 CANN registry 获取 executor。
                      -> false：CollAlgExecRegistryV2::GetAlgExec，从本地 registry 获取 executor；当前 AllReduce 走此分支。
                    -> executor->CalcAlgHierarchyInfo
                    -> executor->CalcRes
              -> algName = InsAllReduceMesh1DTwoShot
              -> forcedAlgAccepted = true
              -> 跳过 SelectAlgAndPrepareEngine
        -> SelectAlgAndPrepareEngine：计算通信拓扑，选择算法并准备 AICPU 通信引擎。
          -> ExecuteSelector::Run
            -> UseCannBridge：判断算法选择是否交给外部 CANN selector。
              -> true：SelectViaCann，调用外部 CANN ExecuteSelector::Run 返回算法名。
              -> false：从本地 AllReduce selector 注册表查找匹配项；当前 AllReduce 走此分支。
                -> AllReduceAutoSelector::SelectAicpuAlgo：检查单层 Mesh1D 拓扑并选择标准 TwoShot 算法。
                  -> algName = InsAllReduceMesh1DTwoShot：记录本次资源和运行阶段使用的算法名称。
        -> GetOpParamResCtx：根据算法名称获取 executor，并创建对应的算法资源上下文。
          -> UseCannBridge：判断资源上下文使用外部 CANN executor 还是本地 executor。
            -> true：GetAlgExecViaCann，从外部 CANN registry 获取 executor。
            -> false：CollAlgExecRegistryV2::GetAlgExec，从本地注册表获取 `InsAllReduceMesh1DTwoShot` executor；当前 AllReduce 走此分支。
          -> HcclGetAlgRes：使用获取到的 executor 计算并申请算法资源。
            -> InsV2AllReduceSoleExecutor::CalcAlgHierarchyInfo：通过 `TopoMatch1D` 计算 0 级通信域和 rank 信息。
            -> InsV2AllReduceSoleExecutor::CalcRes：创建标准 TwoShot template 并发起资源计算。
              -> InsTempAllReduceMesh1DTwoShot::CalcRes：计算线程、notify 和 Mesh1D channel 资源需求。
            -> GetAlgResAICPU：根据 `AlgResourceRequest` 申请 AICPU 算法资源，并将资源上下文保存到 device。
                -> HcclAllocAlgResourceAICPU
                    -> HcclGetHcclBuffer
                    -> HcclGetThread
                    -> HcclGetChannel
                -> HcclMemcpyCtxHostToDevice
        -> 如果GetOpParamResCtx失败 and forcedAlgAccepted
          -> 回退到select
      -> HcclAllocOpResCtx：保存基础 `OpParam` 和序列化资源上下文，生成 mc2 `OpResCtx`。
```

资源阶段主要完成以下工作：

- 根据 `TopoMatch1D` 得到 rank 列表和本地 rank 信息。
- 为每个远端 rank 申请 Mesh1D channel。
- 申请 `rankSize` 个执行线程及对应 notify。
- 分配 HCCL buffer，并保存算法名 `InsAllReduceMesh1DTwoShot`。
- 将基础 `OpParam` 和序列化资源上下文写入 mc2 `OpResCtx`。

### 3.2 AICPU 侧运行

Host 侧通过 `HcclAllocComResourceByTiling` 完成资源初始化后，`OpResCtx` 已包含 AllReduce 的基础 `OpParam` 和 TwoShot 资源上下文。随后框架启动融合算子的 AICore Kernel；当 Kernel 执行到 `AscendC::Hccl::AllReduce` 高阶 API 时，AICore 将输入输出地址、运行时 count、数据类型和归约类型组装成 `HcclMsg`，写入 AICore 与 KFC/AICPU server 共享的消息区。

`AllReduce<true>` 会在写入消息后直接提交通信任务；使用 `AllReduce<false>` 时，需要后续调用 `Commit` 提交通信任务。KFC/AICPU server 轮询到有效消息后，根据提交计数确认任务已经提交，随后恢复资源初始化阶段保存的 `OpParam` 和 `AlgResourceCtxSerializable`，最终调用本地注册的 `InsAllReduceMesh1DTwoShot` executor。`Wait` 或后续依赖该通信结果的计算用于等待 server 更新完成计数，保证通信完成后再继续使用输出数据。

设备侧调用链如下：

```text
AscendC::Hccl::AllReduce：融合算子 Kernel 调用 AllReduce 高阶 API，传入运行时输入输出地址、count、数据类型和归约类型。
  -> HcclImpl::AllReduce：校验归约类型，并构造 HCCL_CMD_ALLREDUCE 通信参数。
    -> CommonPrepareImpl：分配通信 handle，将通信参数组装成消息，并记录 repeat、消息位置和通信类型。
      -> SendMsgToServer：将 HcclMsg 写入 AICore 与 KFC/AICPU server 的共享消息区。
      -> SetCommitTurnCntToGm：当调用 AllReduce<true> 时直接更新提交计数，触发 server 执行该通信任务。
AscendC::Hccl::Commit：当调用 AllReduce<false> 时，由 Kernel 显式提交指定 handle 对应的通信任务。
  -> HcclImpl::Commit：校验 handle，并通过 SetCommitTurnCntToGm 更新共享消息区中的提交计数。
KFC/AICPU server 消息轮询：持续检查共享消息区中是否出现 AICore 写入的有效 HcclMsg。
  -> GetCurrentMsg：从共享消息区读取当前待处理的 HcclMsg。
    -> PrepareProcess：校验消息序号，并将有效通信消息交给 server 编排。
      -> CommKfcAicpuServer::Orchestrate：根据消息中的 opParamKey 匹配资源上下文，并按 repeat 次数执行通信任务。
        -> FormatOpParamFromMsg：使用 Host 侧保存的基础 OpParam 和当前 HcclMsg 构造本次运行参数。
          -> FormatOpenOpParamDataFromMsg：处理 repeat 偏移，并将消息字段更新到运行参数。
            -> CreateOpParamByBaseOpParam：写入 sendBuffer、recvBuffer、dataCnt、dataType、reduceType 和 stream。
        -> LaunchOpenCcoreWait：等待 AICore 通过 AllReduce<true> 或 Commit 更新提交计数。
        -> LaunchOpenAicpuKernelServer：使用运行参数和已恢复的资源上下文启动本地 AICPU 算法。
          -> LaunchOpenOpParamData：检查运行参数是否有效，并进入 open-kernel 执行入口。
            -> LaunchOpenOpParamDataImpl：恢复变长数据并初始化 batch mode。
              -> useCannBridge：按算子类型判断 Device 执行是否交给外部 CANN。
                -> true：RunWithMc2BatchMode
                  -> LaunchViaCann
                    -> 外部 CANN CollAlgExecRegistryV2::GetAlgExec
                    -> 外部 executor->Orchestrate
                -> false：使用本地 mc2_ops_hccl registry；当前 AllReduce 走此分支。
              -> CollAlgExecRegistryV2::GetAlgExec：根据 HCCL_CMD_ALLREDUCE 和 InsAllReduceMesh1DTwoShot 获取 executor。
              -> InsV2AllReduceSoleExecutor::Orchestrate：恢复 channel 和 thread，并读取运行时 count 与数据类型。
                -> InsV2AllReduceSoleExecutor::OrchestrateLoop：根据 HCCL buffer 容量计算单轮 count，循环处理全部数据。
                  -> InsTempAllReduceMesh1DTwoShot::KernelRun：切分当前轮数据，依次执行 ReduceScatter 和 AllGather。
                    -> InsTempAllReduceMesh1DTwoShot::RunReduceScatter：交换各 rank 的目标切片，并在本地完成归约。
                      -> InsTempAllReduceMesh1DTwoShot::ScatterData：通过 Mesh1D channel 收发各 rank 的目标数据片。
                      -> InsTempAllReduceMesh1DTwoShot::ReduceData：将收到的数据片按固定顺序归约到本 rank 数据片。
                    -> InsTempAllReduceMesh1DTwoShot::RunAllGather：交换已归约的数据片，生成每个 rank 的完整输出。
                      -> InsTempAllReduceMesh1DTwoShot::GatherData：通过 Mesh1D channel 收发归约结果并写入输出 buffer。
        -> LaunchOpenCcorePost：更新完成计数，通知 AICore 当前 repeat 的通信任务已经完成。
AscendC::Hccl::Wait：等待完成计数达到预期值，确保 AllReduce 输出可以被后续计算安全使用。
```

运行阶段的关键点：

- 资源创建时的 `count=0` 只用于提前准备与数据量无关的拓扑、channel、thread 和 notify 资源。
- `CreateOpParamByBaseOpParam` 在每次实际通信时用 `msg.dataCnt` 更新 `DataDes.count`，executor 据此计算循环次数和每轮数据量。
- 所有数据量均执行标准 TwoShot；超过单轮 scratch 容量时由 executor 循环分块，不调用 TwoShotMeshChunk。
- 每轮先完成 ReduceScatter，再完成 AllGather，输出地址按 `processedDataCount` 递增。



## 5. 构建接入

新增 `ops/all_reduce` 下的分层 `CMakeLists.txt`，并完成以下接入：

- `src/ops/CMakeLists.txt` 增加 `add_subdirectory(all_reduce)`。
- `src/CMakeLists.txt` 增加 AllReduce 各目录到 `INCLUDE_LIST`。
- `mc2_client` 编译 AllReduce selector、executor 和 AICPU template。
- `mc2_server` 编译 AllReduce executor 和 AICPU template。

构建源列表中没有 TwoShotMeshChunk 或 TwoShotZAxisDetour 文件。

## 6. 支持边界

- 通信引擎：AICPU TS。
- 算子：AllReduce。
- 拓扑：单层 Mesh1D，或 Mesh 链路可全连接的单层 PCIE mixed Mesh1DClos。
- 算法：仅 `InsAllReduceMesh1DTwoShot`。
- 数据类型和 reduce op：沿用源 template 支持范围，64 位类型及 PROD 使用 AICPU reduce 同步分支。
- 多层网络、纯 CLOS、非全连接 Mesh1DClos、CCU、AIV 和 DPU 不在本次迁移范围内。

## 7. 基于现有日志验证调用链

本节只使用当前代码已有日志，不要求新增专用日志。建议同时收集 Host 和 AICPU device 日志，并至少开启 INFO 级别；部分 selector、batch mode 和切片日志使用 DEBUG 级别，需要进一步定位时再开启 DEBUG。

建议先使用以下关键字过滤日志：

```text
MC2_ALG_INFO
MC2_FORCE_ALG
AllReduceAutoSelector
InsAllReduceMesh1DTwoShot
InsV2AllReduceSoleExecutor
CollAlgExecRegistryV2::GetAlgExec
MC2_OPEN_DIAG
MC2_BATCH
HostCannBridge
CannBridge
```

### 7.1 强制算法与 CANNBridge 路由判断

本迁移通过 `Mc2CcTilingInner::algConfig` 强制指定 `InsAllReduceMesh1DTwoShot`，Host 侧首先进入 `TryForcedAlgAndPrepareEngine`，不经过默认 selector。强制算法生效时应出现：

```text
[MC2_FORCE_ALG] accepted, opType[...], algName[InsAllReduceMesh1DTwoShot].
```

该日志只说明强制算法通过资源能力检查，不能单独证明是否走 CANNBridge。需要结合 `CheckForcedAlgResource -> GetAlgExecutorForOp` 的 executor 查询日志判断。

Host 侧走 CANNBridge 时会出现：

```text
[HostCannBridge][GetAlgExec] opType[...] algName[InsAllReduceMesh1DTwoShot] executor[...]
```

Host 侧走本地 non-CANNBridge registry 时会出现：

```text
[asc][AlgoExecute][CollAlgExecRegistryV2::GetAlgExec] hit, opType[...], algName[InsAllReduceMesh1DTwoShot], executor[...]
```

一次强制算法资源初始化中可能出现两次相同的 `GetAlgExec` 日志：第一次来自 `CheckForcedAlgResource` 的资源能力检查，第二次来自 `GetOpParamResCtx` 的正式资源申请。两次都必须命中本地 `mc2_ops_hccl::CollAlgExecRegistryV2`，不能出现对应算法的 `[HostCannBridge][GetAlgExec]`。

如果出现以下日志，说明强制算法未最终生效，后续会恢复默认 selector：

```text
[MC2_FORCE_ALG] fallback, ...
[MC2_FORCE_ALG] fallback to default selector, ...
[MC2_FORCE_ALG] fallback to default selector after resource failure, ...
```

回退后若再出现以下日志，表示默认 selector 走了 CANNBridge；这不能作为强制 TwoShot 路径成功的证据：

```text
[asc][AlgoSelect][ExecuteSelector::Run] before SelectViaCann, ...
[HostCannBridge][Select] before CANN ExecuteSelector::Run, ...
[HostCannBridge][Select] CANN ExecuteSelector::Run success, ...
```

### 7.2 Host 侧资源计算

检查标准 TwoShot template 的资源计算日志：

```text
[InsTempAllReduceMesh1DTwoShot] Ccl Buffer multiple is [2].
[InsTempAllReduceMesh1DTwoShot] Calculate resource finished.resource request: threadNum[...], main thread notifyNum[...], channelNum[...]
```

应满足：

- `threadNum` 等于通信域 rank 数。
- `main thread notifyNum` 等于 `rankSize - 1`。
- `channelNum` 能覆盖所有远端 rank，不能为 0。
- scratch multiple 必须为 2。

资源创建完成后检查最终保存的算法摘要：

```text
[MC2_ALG_INFO] rank[...], group[...], opType[ALLREDUCE](...), algName[InsAllReduceMesh1DTwoShot], srcDataType[...], dstDataType[...], engine[...]
```

该日志是 Host 资源阶段最重要的确认点。需要同时确认：

- `opType` 是 `ALLREDUCE`。
- `algName` 是 `InsAllReduceMesh1DTwoShot`。
- `engine` 是预期的 AICPU TS 引擎。
- 数据类型与测试输入一致。

### 7.3 Device 侧消息和运行参数恢复

Host 资源阶段的 `DataDes.count` 为 0，真实 count 来自 AICore 写入的通信消息。检查以下日志：

```text
[MC2_OPEN_DIAG][FormatMsg] ... msgOpType ..., msgReduceType ..., msgDataCnt ..., msgDataType ..., msgRepeatCnt ...
[MC2_OPEN_DIAG][FormatBase] baseOpType ..., baseAlgName[InsAllReduceMesh1DTwoShot], ... baseCount 0 ...
[MC2_OPEN_DIAG][FormatRun] runOpType ..., runAlgName[InsAllReduceMesh1DTwoShot], ... runCount ..., runDataType ..., ...
```

重点确认：

- `msgOpType`/`runOpType` 对应 `HCCL_CMD_ALLREDUCE`。
- `msgReduceType` 与 SUM、PROD 等测试配置一致。
- `msgDataCnt` 和 `runCount` 等于 Kernel 实际传入的 count，且运行阶段不能为 0。
- `msgDataType` 和 `runDataType` 与测试数据类型一致。
- `baseAlgName` 和 `runAlgName` 均为 `InsAllReduceMesh1DTwoShot`。
- `runInputPtr`、`runOutputPtr` 非空，且与预期 in-place/out-of-place 场景一致。

随后检查 open-kernel 的加载和启动日志：

```text
[MC2_OPEN_DIAG][Load] ... opType ..., algName[InsAllReduceMesh1DTwoShot], ...
[MC2_OPEN_DIAG][LaunchOpenOpParamDataImpl] ... opType ..., algName[InsAllReduceMesh1DTwoShot]
[MC2_OPEN_DIAG][Launch] opType ..., algName[InsAllReduceMesh1DTwoShot], inputPtr ..., outputPtr ..., ... userRank ..., userRankSize ..., threadNum ..., cclMemAddr ..., cclMemSize ...
[MC2_OPEN_DIAG][Launch][Else] count ..., dataType ..., outputType ..., strideCount ...
```

其中 `threadNum` 应等于 rank 数，`cclMemAddr` 应非空，`cclMemSize` 应足以满足 2 倍 scratch 要求。

### 7.4 Device 侧本地 executor 分发

当前 AllReduce 已从 CANN Bridge 分发中移除，应由本地 `CollAlgExecRegistryV2` 获取 executor。检查 device 日志中再次出现：

```text
[asc][AlgoExecute][CollAlgExecRegistryV2::GetAlgExec] hit, opType[...], algName[InsAllReduceMesh1DTwoShot], executor[...]
```

同时不应出现 AllReduce 的以下日志：

```text
[CannBridge] dispatch failed ... opType=ALLREDUCE ...
[CannBridge] GetAlgExec returned null ... algName[InsAllReduceMesh1DTwoShot]
```

若开启 DEBUG，可通过以下日志确认 batch mode 包裹了本地 executor：

```text
[MC2_BATCH][Start] opType[...], algName[InsAllReduceMesh1DTwoShot], algTag[...]
[MC2_BATCH][End] opType[...], algName[InsAllReduceMesh1DTwoShot], algTag[...], execRet[0], endRet[0]
```

### 7.5 Executor 编排和大数据循环

检查 executor 已进入编排：

```text
[InsV2AllReduceSoleExecutor][Orchestrate] Orchestrate Start.
[InsV2AllReduceSoleExecutor][OrchestrateLoop] Start
```

随后检查单轮容量计算：

```text
[InsV2AllReduceSoleExecutor]maxTmpMemSize_ [...]
[InsV2AllReduceSoleExecutor][OrchestrateOpbase] maxDataCountPerLoop[...], maxDataSizePerLoop[...], transportBoundDataSize[...], templateScratchMultiplier[2]
```

重点确认：

- `templateScratchMultiplier` 为 2。
- `maxDataCountPerLoop` 和 `maxDataSizePerLoop` 大于 0。
- 只有运行时 `dataCount > maxDataCountPerLoop` 时，单次 `Orchestrate` 内部才会出现 `loop [0]`、`loop [1]` 等多组分块日志；“大数据”应以该条件判断，而不是固定字节阈值。

每轮已有以下日志：

```text
[InsV2AllReduceSoleExecutor] loop [...] ... sliceSize [...]
[InsV2AllReduceSoleExecutor] loop [...] ... inBuffBaseOff [...], ... outBuffBaseOff [...]
```

大数据循环场景中，`inBuffBaseOff` 和 `outBuffBaseOff` 应逐轮递增，最后必须出现：

```text
[InsV2AllReduceSoleExecutor][OrchestrateLoop] End.
```

多条 `[InsV2AllReduceSoleExecutor][Orchestrate] Orchestrate Start.` 不表示发生了 executor 内部分块。每次 `Orchestrate` 调用只进入一次 `OrchestrateLoop`，内部再通过 `for (loop = 0; loop < loopTimes; loop++)` 多次调用 `KernelRun`。聚合日志中出现多条 `Orchestrate Start`，通常来自不同 rank、消息中的多个 repeat、融合 Kernel 多次调用 AllReduce，或同一测试执行了多轮。应结合相邻的 `userRank`、`msgPos`、`repeatIdx`、`turnIdx` 和 loop 编号区分。

### 7.6 标准 TwoShot template 执行

每个 executor loop 都应出现一组：

```text
[InsTempAllReduceMesh1DTwoShot] KernelRun Start.
[InsTempAllReduceMesh1DTwoShot] KernelRun finished.
```

这两个日志同时出现，说明该轮标准 TwoShot 已依次完成 `RunReduceScatter` 和 `RunAllGather`。当前代码没有分别打印 ReduceScatter/AllGather 成功日志，因此不能仅凭现有日志区分两个阶段；若中途失败，应结合以下错误日志定位：

```text
[InsTempAllReduceMesh1DTwoShot][ScatterData] LocalCopy failed.
[InsTempAllReduceMesh1DTwoShot][ScatterData] Recv failed.
[InsTempAllReduceMesh1DTwoShot][ScatterData] Send failed.
[InsTempAllReduceMesh1DTwoShot][ScatterData] SendRecv failed.
[InsTempAllReduceMesh1DTwoShot][ReduceData] LocalReduce failed
```

开启 DEBUG 后还可检查切片结果：

```text
[InsTempAllReduceMesh1DTwoShot] SliceInfo: offset[...] size[...] count[...]
```

切片验证重点：

- slice 数量等于 rank 数。
- 所有 slice 的 count 总和等于本轮 count。
- count 小于 rank 数时允许尾部 slice 的 count/size 为 0。
- count 不能被 rank 数整除时，最后一个有效 slice 可以小于前面的 slice。

### 7.7 Server 完成和结果可用

检查 server 在 executor 返回后记录成功，并完成回写：

```text
[MC2_OPEN_DIAG][LoopAfterKernel] ... ret 0
[MC2_OPEN_DIAG][LoopAfterPost] ... ret 0
[MC2_OPEN_DIAG][LoopEnd] ...
```

`LoopAfterKernel ret 0` 表示算法执行成功，`LoopAfterPost ret 0` 表示完成计数已回写，AICore 侧 `Wait` 才能正常结束并安全使用输出。

### 7.8 日志断点定位

1. 没有 `[MC2_ALG_INFO]`：Host 资源创建未完成，先检查 tiling、通信域初始化和 selector。
2. selector 匹配但 registry `miss`：`mc2_client` 未编入或未注册 AllReduce executor。
3. Host 完整但没有 `FormatMsg`：AICore 未发送 AllReduce 消息，或消息未被 KFC/AICPU server 消费。
4. `FormatBase` 正确但 `FormatRun` 的 count/type 错误：检查 AICore `Hccl::AllReduce` 入参和消息格式化。
5. device registry `miss`：`mc2_server` 未编入标准 TwoShot executor/template，或 device 算法名不一致。
6. 有 `Orchestrate Start` 但没有 `KernelRun Start`：检查 channel/thread 资源恢复和单轮容量计算。
7. 有 `KernelRun Start` 但没有 `KernelRun finished`：根据 ScatterData/ReduceData 错误定位通信、同步或归约失败。
8. 有 `KernelRun finished` 但 `LoopAfterKernel ret` 非 0：检查 executor 收尾或 batch mode 结束失败。
9. `LoopAfterKernel ret 0` 但没有 `LoopAfterPost ret 0`：检查完成计数回写和 AICore/server 同步。
10. 全部日志完整但结果错误：检查输入输出地址、count、data type、reduce op、切片结果，并用基准 AllReduce 比对每个 rank 输出。

## 8. 功能验证建议

1. Host 和 device 两侧分别构建，确认 `mc2_client` 与 `mc2_server` 均包含 `InsAllReduceMesh1DTwoShot` 注册。
2. 在单层 Mesh1D 环境执行 mc2 AllReduce，按第 7 节顺序核对现有日志。
3. 覆盖 count 小于 rank 数、不能被 rank 数整除、超过单轮 HCCL buffer 容量三类数据量。
4. 覆盖 FP16/FP32 SUM，以及 INT64 或 PROD 的 AICPU reduce 同步分支。
5. 校验 in-place 和 out-of-place 场景的结果与基准 AllReduce 一致。
