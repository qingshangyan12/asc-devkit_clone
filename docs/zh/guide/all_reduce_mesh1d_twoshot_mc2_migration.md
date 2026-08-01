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
uint32_t opType = HCCL_CMD_ALLGATHER; // 设置通信任务类型
std::string algConfig = "AllGather=level0:doublering"; // 设置通信算法，该参数为预留字段，配置后不生效
uint32_t reduceType = HCCL_REDUCE_SUM; // 设置Reduce操作类型，仅对有归约操作的通信任务有效，作为AllGather通信，可以直接配置默认值HCCL_REDUCE_SUM
AscendC::Mc2CcTilingConfig mc2CcTilingConfig(group, opType, algConfig, reduceType);
mc2CcTilingConfig.GetTiling(tiling->mc2InitTiling);
mc2CcTilingConfig.SetSkipLocalRankCopy(0); // 输出gatherOut需带有本卡的A矩阵，因此设置为0
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
        -> SelectAlgAndPrepareEngine：计算通信拓扑，选择算法并准备 AICPU 通信引擎。
          -> ExecuteSelector::Run：从 AllReduce selector 注册表中查找能够匹配当前拓扑的算法选择器。
            -> AllReduceAutoSelector::SelectAicpuAlgo：检查单层 Mesh1D 拓扑并选择标准 TwoShot 算法。
              -> algName = InsAllReduceMesh1DTwoShot：记录本次资源和运行阶段使用的算法名称。
        -> GetOpParamResCtx：根据算法名称获取 executor，并创建对应的算法资源上下文。
          -> CollAlgExecRegistryV2::GetAlgExec：从本地注册表中获取 `InsAllReduceMesh1DTwoShot` executor。
          -> HcclGetAlgRes：使用获取到的 executor 计算并申请算法资源。
            -> InsV2AllReduceSoleExecutor::CalcAlgHierarchyInfo：通过 `TopoMatch1D` 计算 0 级通信域和 rank 信息。
            -> InsV2AllReduceSoleExecutor::CalcRes：创建标准 TwoShot template 并发起资源计算。
              -> InsTempAllReduceMesh1DTwoShot::CalcRes：计算线程、notify 和 Mesh1D channel 资源需求。
            -> GetAlgResAICPU：根据 `AlgResourceRequest` 申请 AICPU 算法资源，并将资源上下文保存到 device。。
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
            -> LaunchOpenOpParamDataImpl：初始化 batch mode，并按算子类型分发到本地算法注册表。
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

## 4. mc2 分发适配

迁移前，AICPU AllReduce 被强制分发到外部 CANN bridge：

- Host 侧 `UseCannBridge` 对 `HCCL_CMD_ALLREDUCE` 返回 `true`，selector 和资源计算使用外部 `ops_hccl` 注册表。
- AICPU open-kernel 侧也将 `HCCL_CMD_ALLREDUCE` 交给 `LaunchViaCann`。

这会绕过本次迁移到 `mc2_ops_hccl` 的 selector、executor 和 template。适配后：

- `UseCannBridge` 仅保留 AllToAll 和 AllToAllV，AllReduce 使用本地 selector 与本地资源注册表。
- AICPU open-kernel 侧 AllReduce 使用本地 `CollAlgExecRegistryV2` 和 `Orchestrate`。
- Host 目标 `mc2_client` 编译 selector、executor 和 template。
- Device 目标 `mc2_server` 编译 executor 和 template，保证设备运行阶段存在相同算法注册。

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

## 7. 日志判定方法

建议先按以下关键字过滤一次运行日志：

```text
MC2_AR_TRACE
MC2_ALG_INFO
InsAllReduceMesh1DTwoShot
InsV2AllReduceSoleExecutor
```

一次成功的本地 AICPU TwoShot 运行，关键日志应按以下阶段出现：

```text
[MC2_ALG_INFO] ... opType[ALLREDUCE], algName[InsAllReduceMesh1DTwoShot]
[MC2_AR_TRACE][AICPU_API] AllReduce enter ...
[MC2_AR_TRACE][AICPU_PREPARE] type[ALLREDUCE] ... commit[1]
[MC2_AR_TRACE][DISPATCH] local V2 executor hit ... algName[InsAllReduceMesh1DTwoShot]
[MC2_AR_TRACE][EXECUTOR] Orchestrate start ...
[MC2_AR_TRACE][EXECUTOR] OrchestrateLoop start ...
[MC2_AR_TRACE][TWOSHOT] KernelRun start ...
[MC2_AR_TRACE][TWOSHOT] ReduceScatter start ...
[MC2_AR_TRACE][TWOSHOT] ReduceScatter end.
[MC2_AR_TRACE][TWOSHOT] AllGather start.
[MC2_AR_TRACE][TWOSHOT] AllGather end.
[MC2_AR_TRACE][TWOSHOT] KernelRun end.
[MC2_AR_TRACE][EXECUTOR] OrchestrateLoop end ...
[MC2_AR_TRACE][DISPATCH] local V2 executor end ...
```

各节点含义如下：

- `[MC2_ALG_INFO]`：Host 资源创建阶段最终保存的算法名；必须是 `InsAllReduceMesh1DTwoShot`。
- `[AICPU_API]`：融合算子的 AICore Kernel 确实调用了 `Hccl::AllReduce`。
- `[AICPU_PREPARE]`：AllReduce 消息已写入共享消息区；`commit[1]` 表示该调用直接提交，`commit[0]` 需要后续 `Commit`。
- `[DISPATCH] local V2 executor hit`：Device 侧命中本地 `CollAlgExecRegistryV2`，不是外部 CANN Bridge。
- `[EXECUTOR]`：TwoShot executor 已恢复资源并开始编排；可同时检查 `count`、`rankSize` 和 `loopTimes`。
- `[TWOSHOT]`：标准模板已实际执行，且 ReduceScatter、AllGather 两阶段都完成。

异常定位建议：

1. 只有 `[MC2_ALG_INFO]`，没有 `[AICPU_API]`：资源创建成功，但融合 Kernel 没有调用 AllReduce，或运行日志不包含 AICore/KERNEL_LOG。
2. 有 `[AICPU_API]`，没有 `[AICPU_PREPARE]`：Prepare 参数校验失败、消息区不可用或日志级别不足。
3. 有 `[AICPU_PREPARE]`，没有 `[DISPATCH]`：消息未被 KFC/AICPU server 消费，重点检查 commit、消息序号和 server 轮询日志。
4. `[MC2_ALG_INFO]` 的算法名不是 `InsAllReduceMesh1DTwoShot`：资源阶段 selector 或强制算法配置未命中标准 TwoShot。
5. 有 `[DISPATCH]`，没有 `[TWOSHOT]`：executor 命中但 template 未进入，重点检查 executor 编排、线程资源和 device 构建产物。
6. 只有 `[TWOSHOT] ReduceScatter start`，没有对应 end：重点检查 channel、线程同步或 Send/Recv 操作失败。
7. 有完整 `[TWOSHOT]` 链路但结果错误：重点检查输入输出地址、运行时 `count`、rank 列表、切片信息和 Reduce 操作。

当前新增的 `[MC2_AR_TRACE]` 日志位置包括：

- `common/hccl_aicpu_impl.h`：AICPU API 和 Prepare。
- `impl/platform_v310/hccl_ccu_v0.h`：CCU API。
- `cc/src/ops/all_reduce/selector/all_reduce_auto_selector.cc`：selector 输入和匹配结果。
- `cc/src/ops/all_reduce/executor/ins_v2_all_reduce_sole_executor.cc`：executor 编排和 loop。
- `cc/src/ops/all_reduce/template/aicpu/ins_temp_all_reduce_mesh_1D_two_shot.cc`：TwoShot 两阶段。
- `cc/src/aicpu_kfc/framework/device/aicpu_kfc/decoupler/comm_kfc_open_kernel_executor.cc`：本地 V2 executor 分发。

## 8. 验证建议

1. Host 和 device 两侧分别构建，确认 `mc2_client` 与 `mc2_server` 均包含 `InsAllReduceMesh1DTwoShot` 注册。
2. 在单层 Mesh1D 环境执行 mc2 AllReduce，按本节顺序检查 `[MC2_AR_TRACE]` 日志。
3. 检查日志中的 `[MC2_ALG_INFO]` 算法名为 `InsAllReduceMesh1DTwoShot`。
4. 覆盖 count 小于 rank 数、不能被 rank 数整除、超过单轮 HCCL buffer 容量三类数据量。
5. 覆盖 FP16/FP32 SUM，以及 INT64 或 PROD 的 AICPU reduce 同步分支。
6. 校验 in-place 和 out-of-place 场景的结果与基准 AllReduce 一致。
