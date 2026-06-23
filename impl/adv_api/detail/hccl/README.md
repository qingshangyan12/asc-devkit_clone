# HCCL detail 实现说明

## 目录定位

`impl/adv_api/detail/hccl` 是 Ascend C 高阶 HCCL/MC2 能力在本仓内的实现目录，覆盖两类代码：

- Kernel 侧内联封装：面向 Ascend C kernel 调用，提供 `AscendC::Hccl<serverType, config>` 的 `Init`、集合通信 prepare、`Commit`、`Wait`、`Finalize` 等接口。
- Host/AICPU/CCU 侧实现：面向 HCCL host API、AICPU server、CCU 算法和资源申请，提供 AllGather、ReduceScatter 当前新路径的参数校验、拓扑识别、算法选择、executor 编排、template 执行和构建打包逻辑。

整体上，Kernel 侧不直接执行集合通信算法，而是把通信任务描述写入 GM 中的 HCCL 消息区；Host/AICPU/CCU 侧负责解析消息、申请通信资源、选择算法并下发执行。

## 顶层结构

```text
hccl
├── common/                 # Kernel 侧公共定义、消息写入、cache flush、DFX 和控制消息
├── impl/                   # Kernel 侧 HcclImpl 平台分发与 v220/v310 平台实现
├── ccu/                    # Kernel 侧 CCU v0 准备和 XN/CKE 相关工具
└── cc/                     # Host/AICPU/CCU C++ 实现、CMake 构建与打包
```

关键入口文件：

| 路径 | 作用 |
|------|------|
| `impl/hccl_impl.h` | `AscendC::Hccl` 对外接口的内联实现，统一转发到 `impl_`。 |
| `impl/hccl_impl_def.h` | 按 `__NPU_ARCH__` 包含 v220 或 v310 平台实现定义。 |
| `common/hccl_base.h` | `HcclImpl` 泛型默认实现，未特化时接口返回失败或空值。 |
| `common/hccl_aicpu_def.h` | `HcclServerType::HCCL_SERVER_TYPE_AICPU` 的类特化声明。 |
| `common/hccl_aicpu_impl.h` | AICPU server 类型的通用 prepare/commit/wait/finalize 实现。 |
| `common/hccl_inner_def.h` | Kernel 侧上下文、通信参数、CCU 消息和常量定义。 |
| `cc/src/ops/op_common/op_common.h` | Host 侧执行公共入口和资源申请接口声明。 |
| `cc/src/ops/all_gather/all_gather_op.cc` | AllGather 新路径入口、校验、`OpParam` 组装与执行。 |
| `cc/src/ops/reduce_scatter/reduce_scatter_op.cc` | ReduceScatter 新路径入口、校验、`OpParam` 组装与执行。 |
| `cc/src/ops/op_common/selector/execute_selector.cc` | 根据 opType 和 selector 优先级选择算法名。 |
| `cc/src/ops/op_common/executor/registry/` | executor 注册表，通过算法名创建 executor。 |

## Kernel 侧接口层

`AscendC::Hccl<serverType, config>` 是 kernel 代码看到的统一接口。`impl/hccl_impl.h` 中每个接口先做 DFX guard，再转发到 `impl_`：

```cpp
auto handle = hccl.AllGather(sendBuf, recvBuf, sendCount, dataType, strideCount, repeat);
hccl.Commit(handle);
hccl.Wait(handle);
hccl.Finalize();
```

接口可以分为四类：

| 类型 | 接口 | 说明 |
|------|------|------|
| 初始化 | `Init`、`InitV2`、`SetCcTiling`、`SetCcTilingV2` | 建立 HCCL context、消息区、tiling 表和平台上下文。 |
| 集合通信 prepare | `AllReduce`、`AllGather`、`ReduceScatter`、`AlltoAll`、`AlltoAllV` | 不直接执行通信，只生成任务描述并返回 `HcclHandle`。 |
| 执行控制 | `Commit`、`Wait`、`Query`、`Iterate`、`Finalize` | 通过 turn count 和 finished count 驱动 AICPU server 执行并等待完成。 |
| 辅助能力 | `BatchWrite`、`AlltoAllvWrite`、`QueueBarrier`、`InterHcclGroupSync`、`GetWindowsInAddr`、`GetWindowsOutAddr`、`GetRankId`、`GetRankDim`、`GetQueueNum` | 提供 batch write、窗口地址、rank 信息、队列同步等功能。 |

`common/hccl_base.h` 提供的是泛型兜底实现，大多数接口返回 `-1`、`nullptr` 或 `0`。实际可用实现依赖 `HcclServerType` 和平台特化，目前主要是 `HCCL_SERVER_TYPE_AICPU`。

## 平台分发

`impl/hccl_impl_def.h` 按编译期 `__NPU_ARCH__` 选择平台头文件：

```cpp
#if defined(__NPU_ARCH__) && __NPU_ARCH__ == 2201
#include "hccl_v220_impl.h"
#endif

#if defined(__NPU_ARCH__) && __NPU_ARCH__ == 3510
#include "hccl_v310_impl.h"
#endif
```

差异点：

| 平台 | 包含内容 | 主要能力 |
|------|----------|----------|
| v220 / `__NPU_ARCH__ == 2201` | `common/hccl_aicpu_def.h` 和 `platform_v220/hccl_aicpu.h` | AICPU 路径功能较完整，支持 `Init`/`InitV2`、窗口地址查询、`BatchWrite`、`AlltoAllvWrite`、`InterHcclGroupSync`、`QueueBarrier`、`Iterate` 等。 |
| v310 / `__NPU_ARCH__ == 3510` | `common/hccl_aicpu_def.h`、`platform_v310/hccl_aicpu.h`、`platform_v310/hccl_ccu_v0_def.h`、`platform_v310/hccl_ccu_v0.h` | AICPU 路径走 context decouple 版本，同时引入 CCU v0；部分 v220 接口在 v310 AICPU 实现中直接返回失败或空实现。 |

v310 的 AICPU `InitV2` 使用 `OpResCtx`，从 `Mc2InitTilingInner` 读取 `debugMode`、`devType`、`tilingBaseAddr_`，再以 `HcclTilingVersion::CONTEXT_DECOUPLE_VERSION` 初始化消息区。`SetCcTilingV2` 除记录 tiling offset 外，还会从 `hcclContext_->algInfo` 中匹配算法参数并写入 `ccOpParamTable_`。

## Kernel 侧上下文和消息区

### 主要数据结构

`common/hccl_inner_def.h` 集中定义 kernel 侧和 AICPU/CCU 交互的关键结构：

| 结构 | 用途 |
|------|------|
| `CommonPrepareParam` | AICPU prepare 的统一参数，包含命令类型、send/recv buffer、count、数据类型、reduce op、stride、repeat，以及 AlltoAllV/AlltoAllvWrite 扩展参数。 |
| `CommonPrepareParamCcu` | CCU prepare 参数，布局上带 reserved 区，适配 CCU 消息格式。 |
| `HcclCombineOpParam` | v220 风格 context，包含 workspace、rankId、rankNum、winSize、windowsIn/out、multiFlag、IB verbs 数据等。 |
| `HcclContextDef::HcclOpResParam` | 新版 context 参数，记录 workspace、rank 信息、本地 window、hcomId、扩展 window 等。 |
| `CCUMsg`、`CCUMsgExt`、`CcuPrepareParam` | CCU 消息、AlltoAllV 扩展信息和 CCU prepare 运行参数。 |
| `AlltoAllVParamCcu` | CCU AlltoAllV 的 send/recv counts 和 displacement 数组。 |

`DATA_TYPE_MAP` 用于把 HCCL 数据类型映射为元素字节数，`HCCL_MSG_CNT`、`HCCL_MAX_HANDLE_ID` 等上限来自 internal HCCL 消息头文件。

### 消息区初始化

`HcclImpl<..., AICPU>::InitInner` 做三件关键事情：

1. 检查不能重复初始化。
2. 将 context 中的 workspace 地址按 512B 对齐后解释为 `HcclMsgArea`。
3. 初始化 handle 到消息位置的映射，并根据 `config` 决定当前 block 是否为写消息的 working block。

`config` 类型必须是 `const HcclServerConfig&`。`InitWorkingFlag` 根据 `config.type` 和 `config.blockId` 判断当前 block：

- `CoreType::ON_AIV`：要求当前核类型为 AIV，且 `GetBlockIdx() == blockId`。
- `CoreType::ON_AIC`：要求当前核类型为 AIC，且 `GetBlockIdx() == blockId`。
- 其他：只检查 block id。

这个设计避免所有核同时写 HCCL 消息区。

## AICPU prepare/commit/wait 生命周期

### Prepare

所有集合通信 prepare 最终都会走 `CommonPrepareImpl`：

```text
AllGather/ReduceScatter/AllReduce/AlltoAll/AlltoAllV
  -> CommonPrepareImpl
     -> CheckCommonPrepareParamValid
     -> SendMsgToServer
     -> 记录 handleId、msgPosition、repeat、cmdType
     -> 如果模板参数 commit=true，则立即写 commit turn count
```

校验规则包括：

- `Init` 或 `InitV2` 必须已成功调用。
- 新 tiling 版本必须提前调用 `SetCcTiling` 或 `SetCcTilingV2`，旧版本不允许带 tiling。
- `sendBuf` 和 `recvBuf` 不能为空。
- `dataType` 必须在 HCCL 支持范围内，`BatchWrite` 除外。
- 普通算子 `count` 不能为 0；AlltoAllV 则要求 `sendCounts`、`sdispls`、`recvCounts`、`rdispls` 均不为空。
- Reduce 类算子的 `HcclReduceOp` 必须在合法范围内。
- `repeat` 为 0 时直接返回无效 handle。

`CommonPrepareImpl` 返回 `HcclHandle`。后续 `Commit`、`Wait`、`Query` 都通过该 handle 找回消息位置和命令类型。

### 写消息

`SendMsgToServer` 会根据 `queueNum_` 选择 singleMsg 或 multiMsg：

- `queueNum_ == 0`：写 `hcclMsgArea_->commMsg.singleMsg.sendMsgs[curMsgPosition_[0]]`。
- `queueNum_ != 0`：写 `multiMsg.sendMsgs[queId + GetBlockIdx() * queueNum_][curMsgPosition_[queId]]`。

写入前会通过 `FlushDataCache` 轮询消息槽，确保上一次消息已经被 server 读取；如果异常重启标志已置位，`HCCL_CHECK_RESTART` 会提前退出。消息组装分两类：

- 普通 prepare：`AssembleHcclMsg` 或 v310 下的 `AssembleHcclMsgV2`。
- AlltoAllV 扩展参数：额外写 `HcclMsgExt`，并刷新 send/recv counts 与 offset 数组。

### Commit

`Commit(handleId)` 不再重写通信参数，只更新 GM 中对应消息槽的 commit turn count：

- 普通任务每次 commit 增加 1。
- 细粒度 AlltoAllV 根据 `stepSize` 增加多个 step。
- commit 次数不能超过 `repeat * stepsPerRepeat`。

`commit=true` 模板参数可以让 prepare 时直接 commit，适合“一次 prepare 后立即提交”的场景。

### Wait 和 Query

`Wait(handleId)` 校验 handle 后读取 `finishedTurnCnt`，直到完成计数达到预期：

- singleMsg 模式下执行真实等待。
- multiMsg 模式当前直接返回成功。
- `Wait` 调用次数不能超过已 commit 次数。
- 对细粒度 AlltoAllV，wait 计数按 `stepSize` 增加。

`Query(handleId)` 返回当前完成计数，不主动增加等待次数，适合查询式轮询。

### Iterate

`Iterate<sync>(handleId, seqSlices, seqSliceLen)` 仅面向带 step 的细粒度任务，当前注释指向 AlltoAllV + pairwise 场景。它根据当前 slice、rankId 和 rankDim 生成本轮访问序列：

- `sync=true` 时在 step 边界调用 `Wait(handleId)`，并按逆向关系生成 rank。
- `sync=false` 时只推进 slice，不等待。

### Finalize

`Finalize<sync>()` 完成一次 kernel 调用尾部的收尾：

1. `sync=true` 时等待最后一个 HCCL handle 完成。
2. 写 `HCCL_CMD_FINALIZE` 控制消息。
3. `sync=true` 时等待 server 读完 finalize 消息。
4. 重置 `finishedTurnCnt`，保证下一次 launch 不复用旧完成计数。
5. working block 记录控制消息计数和 API 快照。

如果当前 block 不是 working block 且 `queueNum_ == 0`，只推进本地消息位置，不写 finalize 消息。

## 异常重启和 cache 一致性

`common/hccl_control.h` 提供异常重启控制：

- `GetRestart(ctxNum)`：从最多两个 HCCL context 中读取 restart 标志。
- `SetRestart(ctxNum)`：在 block 0 上重置 restart 标志，并设置 restarting/resetSeq。
- `CheckIfRestart(msgArea)`：在 `AICORE_EXCEPTION_RESTART == 1` 时检查 `controlMsg.restart`。

消息区读写依赖 `common/hccl_utils.h` 中的 cache flush：

- `FlushDataCache`：普通 GM 地址刷新，内部使用 `DataCacheCleanAndInvalid` 和 `DataSyncBarrier<MemDsbT::ALL>`。
- `FlushDataCacheAicpu`：AICPU 轮询读取版本；v310 下会插入固定 NOP 周期，避免 A5 AICPU 高频 polling 读导致总线挂起。
- v310 还提供 `ReadHBMData` 和 `WriteHBMData`，通过 `ReadGmByPassDCache`/`WriteGmByPassDCache` 绕过 DCache 读写 HBM。

## CCU v0 路径

v310 平台额外引入 CCU v0：

```text
impl/platform_v310/hccl_ccu_v0_def.h
impl/platform_v310/hccl_ccu_v0.h
ccu/hccl_ccu_v0_prepare.h
ccu/hccl_ccu_xn_utils.h
```

CCU 相关结构主要围绕 XN 数据、CKE 地址和消息扩展区：

- `CCUMsg` 保存 XN 数据地址、XN 写入地址、commit CKE 地址和 wait CKE 地址。
- `CCUMsgExt` 用于 AlltoAllV/AlltoAllvWrite 的 send size、loop num、send offset、recv offset。
- `CcuPrepareParam` 保存 rank 信息、repeat index、AlltoAllV 计数、scratch 地址等。
- 常量 `CCU_LOOP_COUNT_M2M_RE`、`CCU_LOOP_COUNT_M2M_AG`、`CCU_LOOP_COUNT_ATAVW` 分别约束 ReduceScatter/AllGather/AlltoAllvWrite 场景的 CCU 循环数。

CCU 路径和 AICPU 路径共用部分 tiling/context 信息，但执行侧更偏向 CCU kernel 与 XN/CKE 消息同步。

## Host 侧执行框架

`cc/src` 是可编译的 C++ 实现，分为 common、ops 和 aicpu_kfc 三块。

```text
cc/src
├── common/        # 日志、环境变量、ACL/RT 适配、动态符号加载、参数校验、设备兼容
├── ops/           # Host API 新路径、算法选择、executor、template、topo 和 graph mode
└── aicpu_kfc/     # AICPU/KFC server、通信资源、线程、通道、调度和设备侧 framework
```

### Host API 入口

当前 `ops` 下实现了 AllGather 和 ReduceScatter 新路径：

- `HcclAllGather`
- `HcclAllGatherGraphMode`
- `HcclReduceScatter`
- `HcclReduceScatterGraphMode`

入口判断逻辑不是无条件走新路径：

1. 如果 AICPU/CCU 新路径开关未打开，回退到 `HcclAllGatherInner` 或 `HcclReduceScatterInner`。
2. 如果 `GetHcommVersion() < 90000000`，回退到老流程。
3. 如果设备类型不是 950/910_95 族，回退到老流程。
4. count 为 0 时直接返回成功。
5. 参数校验通过后组装 `OpParam`，再进入 selector 和 executor。

AllGather 的 `OpParam` 关键字段：

- `inputSize = sendCount * datatypeSize`
- `outputSize = inputSize * rankSize`
- `DataDes.count = sendCount`
- `opType = HCCL_CMD_ALLGATHER`
- `opMode = OPBASE` 或 graph mode 下的 `OFFLOAD`

ReduceScatter 的 `OpParam` 关键字段：

- `outputSize = recvCount * datatypeSize`
- `inputSize = outputSize * rankSize`
- `DataDes.count = recvCount`
- `reduceType = op`
- `opType = HCCL_CMD_REDUCE_SCATTER`
- `opMode = OPBASE` 或 `OFFLOAD`

graph mode 会额外组装 `ResPackGraphMode`，记录 tag、额外 stream 列表、scratch memory 地址和大小。

### 通用执行链

Host 侧执行链可以概括为：

```text
HcclAllGather / HcclReduceScatter
  -> InitEnvConfig
  -> 参数校验、rank 校验、tag 校验
  -> 组装 OpParam
  -> Selector(comm, param, topoInfo, algName)
  -> ShouldUseInnerOp 判断是否回退老流程
  -> rankSize == 1 时 SingleRankProc
  -> HcclExecOp
       -> 获取/申请算法资源
       -> 通过 algName 创建 executor
       -> executor.CalcResRequest
       -> executor.Orchestrate
       -> AICPU/CCU kernel launch 或 graph mode 资源复用
```

`op_common/op_common.h` 声明了这条链上的公共能力：

- 拓扑与算法选择：`HcclCalcTopoInfo`、`Selector`、`CheckAsymmetricTopoSupport`。
- 资源申请：`HcclGetAlgRes`、`GetAlgResAICPU`、`GetAlgResCcu`、`GetAlgResDPU`。
- 线程与通道：`HcclGetThread`、`HcclGetChannel`、`HcclGetChannelForCcu`。
- graph mode buffer：`RegGraphModeBuffers`、`GetGraphModeBuffers`。
- kernel launch：`HcclAicpuKernelEntranceLaunch`、`AicpuKernelLaunch`、`HcclExecOpCcuFastLaunch`。
- fallback 和策略：`ShouldUseInnerOp`、`ShouldGoCcuFastLaunch`、`SetCommEngine`。

## 算法选择

选择器基类和注册逻辑位于 `ops/op_common/selector`。`ExecuteSelector::Run` 的行为：

- 如果 `UseCannBridge(opParam)` 为真，转到 CANN bridge 选择。
- 如果 `opParam.isMc2` 为真，固定查找优先级 18 的 selector。
- 否则按 opType 找到所有 selector，并按优先级尝试，返回第一个 `MATCH` 的算法名。

AllGather 和 ReduceScatter selector 都通过 `REGISTER_SELECTOR_BY_OPTYPE(..., 18, ...)` 注册。

### AllGather 选择规则

`all_gather/selector/all_gather_auto_selector.cc` 按执行引擎分为：

| 引擎 | 代表算法名 | 选择依据 |
|------|------------|----------|
| CCU MS | `CcuAllGatherMesh1D`、`CcuAllGatherParallelMeshNHR`、`CcuAllGatherConcurrentMesh1DNHR` | topo 层数、`MESH_1D`/`MESH_1D_CLOS`、数据量是否大于 512KB、rank 数是否不超过 4。 |
| CCU schedule | `CcuAllGatherMesh1DMem2Mem`、`CcuAllGatherNHR1DMem2Mem`、`CcuAllGatherParallelMesh1DNHR`、UBX 变体 | topo 层数、`Level1Nhr`、2-die full mesh、UBX 的 mesh/clos 数量关系。 |
| AICPU | `InsAllGatherMesh1D`、`InsAllGatherNHR`、`InsAllGatherParallelMesh1DNHR`、UBX 变体 | topo 层数、`Level0Nhr`/`Level1Nhr`、level0 topo、local net instance 数和数据量。 |
| AIV | `AivAllGatherMesh1D` | 当前固定返回 Mesh1D。 |
| DPU | `InsAllGatherMeshNhrDPU` | 多层 topo 且满足 local net 或 Mesh1D 条件。 |

### ReduceScatter 选择规则

`reduce_scatter/selector/reduce_scatter_auto_selector.cc` 同样按引擎区分，规则更依赖数据类型和 reduce op：

| 引擎 | 代表算法名 | 关键限制和选择依据 |
|------|------------|--------------------|
| CCU MS | `CcuReduceScatterMesh1D`、`CcuReduceScatterMesh2Die`、`CcuReduceScatterConcurrentMeshNHRMs` | 不支持 `INT8`、`PROD`、64 bit 数据类型；只支持单层 topo；MESH_1D_CLOS 下按 mesh/clos 关系和数据量选择。 |
| CCU schedule | `CcuReduceScatterMesh1DMem2Mem`、`CcuReduceScatterNHR1DMem2Mem`、`CcuReduceScatterParallelMesh1DNHR`、MultiJetty 变体 | 不支持 `PROD` 和部分 64 bit 类型；按 topo 层数、`Level1Nhr`、level0 topo、数据量阈值和 UBX 关系选择。 |
| AICPU | `InsReduceScatterMesh1D`、`InsReduceScatterNHR`、`InsReduceScatterParallelMesh1DNHR`、`InsReduceScatterMesh1DMeshChunk`、`InsReduceScatterAicpuReduceNHR` | 64 bit 类型或 `PROD` 走 AICPU reduce NHR；大数据按阈值选择 mesh chunk 或 Z-axis detour。 |
| AIV | `AivReduceScatterMesh1D` | 不支持 `PROD` 和 64 bit 类型。 |
| DPU | `InsReduceScatterSequenceMeshMeshDPU` | 当前只在多层 topo 下匹配。 |

ReduceScatter 中常见阈值：

- `RS_AICPU_1D_MIN_DATA_SIZE = 4MB`
- `RS_AICPU_1D_MAX_DATA_SIZE = 16MB`
- `RS_AICPU_1D_TWO_LEVER_DATA_SIZE_THRESHOLD = 1536MB`
- `RS_AICPU_SEQUENCE_SIZE_THRESHOLD = 1GB`

这些阈值用于在 Mesh1D、NHR、parallel、sequence、meshchunk 和 detour 算法之间切换。

## Executor 和 template 分层

Host 侧算法不是直接由 selector 执行。selector 只产出算法名，实际执行由 executor 和 template 两层完成。

### Executor

`ExecutorBase` 定义两个核心虚接口：

- `CalcResRequest`：根据 `OpParam`、拓扑和算法层级信息计算线程、通道、notify、scratch 等资源需求。
- `Orchestrate`：拿到资源后编排具体算法流程。

AllGather executor：

- `ins_v2_all_gather_sole_executor`
- `ins_v2_all_gather_parallel_executor`
- `ins_v2_all_gather_sequence_executor`
- `ins_v2_all_gather_concurrent_executor`

ReduceScatter executor：

- `ins_v2_reduce_scatter_sole_executor`
- `ins_reduce_scatter_parallel_executor`
- `ins_reduce_scatter_concurrent_executor`
- `ins_v2_reduce_scatter_sequence_executor`
- `ins_v2_reduce_scatter_sequence_executor_aicpu`

executor 通过 `REGISTER_EXEC` 注册到 `CollAlgExecRegistry`。运行时 `CollAlgExecRegistry::GetAlgExec(algName)` 按算法名创建 executor 实例。

### Template

`AlgTemplateBase` 是更底层的数据搬运/同步模板基类，负责：

- 保存输入、输出、scratch、count、dataType、reduceOp、thread、slice 等执行上下文。
- 提供 `Prepare` 重载，用于不同算法准备输入参数。
- 提供 `RunAsync` 虚接口，由具体模板实现异步通信步骤。
- 提供 slice 计算、barrier、数据类型字节数、链路关系计算等工具。

AllGather template：

- AICPU：`ins_temp_all_gather_mesh_1D`、`ins_temp_all_gather_nhr`、`ins_temp_all_gather_mesh_1D_Z_axis_detour`、`ins_temp_all_gather_nhr_dpu`
- CCU：`ccu_temp_all_gather_mesh_1D`、`ccu_temp_all_gather_mesh_1D_mem2mem`、`ccu_temp_all_gather_nhr_1D_mem2mem`、2-die 和 multi-jetty 变体

ReduceScatter template：

- AICPU：`ins_temp_reduce_scatter_mesh_1D`、`ins_temp_reduce_scatter_nhr`、`ins_temp_reduce_scatter_mesh_1D_meshchunk`、`ins_temp_reduce_scatter_mesh_1D_Z_axis_detour`、`ins_temp_reduce_scatter_aicpu_reduce_nhr`、DPU 变体
- CCU：`ccu_temp_reduce_scatter_mesh_1D`、`ccu_temp_reduce_scatter_mesh_1D_mem2mem`、`ccu_temp_reduce_scatter_mesh2die`、`ccu_temp_reduce_scatter_nhr_1D_mem2mem`、multi-jetty 变体

## Topo 和资源

`ops/op_common/topo` 根据通信域和网络层级生成 `TopoInfoWithNetLayerDetails`。selector 主要依赖这些字段：

- `topoLevelNums`：拓扑层数。
- `level0Topo`：如 `MESH_1D`、`MESH_1D_CLOS`、`CLOS`。
- `level0MeshType`：如 two-die regular / not regular。
- `Level0Nhr`、`Level1Nhr`：是否走 NHR。
- `is2DieFullMesh`、`level0PcieMix`：特殊拓扑判断。
- `netLayerDetails.localNetInsSizeOfLayer`：每层 local net 实例数量。
- `userRankSize`：通信域 rank 数。

资源申请阶段会根据 executor 的 `AlgResourceRequest` 分配：

- 通信线程或 AICPU/CPU TS thread。
- 通道 `ChannelInfo`，包括本地/远端 rank、notify、transport 等。
- graph mode 下的用户 buffer 和 scratch buffer。
- CCU fast launch 所需 kernel 和 channel。

## AICPU/KFC 框架

`cc/src/aicpu_kfc` 是 device/AICPU 侧支撑框架，目录较深，按职责可以理解为：

| 子目录 | 职责 |
|--------|------|
| `pub_inc/` | AICPU/KFC 对外或跨模块公共头，包括 transport、notify、stream、memory、adapter、network、primitive 等。 |
| `platform/` | 任务调度、通信原语、HCCP、remote access、resource、debug 等平台层实现。 |
| `framework/` | communicator、group、op_base、device framework、next coll/comms、namespace recovery、DFX 等高层框架。 |
| `algorithm/` | AICPU 侧 allreduce、allgather、reduce_scatter、alltoall、dispatcher、task orchestrator 等算法框架。 |
| `legacy/` | 旧版 framework/service/interface/unified platform 兼容实现。 |

Host 侧 `mc2_client` 和 device 侧 `mc2_server` 通过这些模块完成资源构建、任务下发、AICPU server 执行、异常和 profiling 支撑。

## 构建和产物

`cc/CMakeLists.txt` 是 HCCL detail C++ 部分的构建入口。

### Device 侧

当 `PRODUCT_SIDE == device` 时，`cc/src/CMakeLists.txt` 构建 `mc2_server`：

- 编译 common、op_common、topo、executor、template、AllGather/ReduceScatter AICPU template 等。
- 定义 `AICPU_COMPILE` 和 `MC2_SERVER_ONLY`。
- 链接 `mc2_kernel_compat`、`mmpa`、ACL RT 头和 error manager 头。

当同时满足 `DEVICE_MODE && KERNEL_MODE` 时，顶层 CMake 会：

1. 打包 `mc2_server` 和 `mc2_kernel_compat` 为 `mc2_server.tar.gz`。
2. 使用 `scripts/sign/hccl_check_cfg.xml` 签名。
3. 安装到 `opp/built-in/op_impl/aicpu/kernel`。

### Host 侧

当 `PRODUCT_SIDE == host` 且不是 `KERNEL_MODE` 时，构建 `mc2_client`：

- 生成 `libmc2_server.json`，来源是 `ops/op_common/mc2_server.ini`。
- 安装 JSON 到 `opp/built-in/op_impl/aicpu/config`。
- 定义 `HOST_COMPILE` 和 `MC2_CLIENT_ENABLE_CCU`。
- 链接 `acl_rt`、runtime headers 和 error manager headers。

host 侧还会安装 HCCL public header：

- `hccl.h`
- `hccl_mc2.h`

安装目标为 `${CMAKE_SYSTEM_PROCESSOR}-linux/include/hccl`。

## 扩展一个新集合通信算子的典型位置

如果要在这套框架中新增一个集合通信算子，通常需要同时处理 kernel prepare 接口和 host 执行链：

1. 在 internal HCCL 消息定义中增加命令类型和消息字段。
2. 在 `common/hccl_inner_def.h` 或相关公共头中补充 prepare 参数结构。
3. 在 `common/hccl_aicpu_def.h` 声明 AICPU 特化接口。
4. 在 `common/hccl_aicpu_impl.h` 增加 prepare 参数校验和 `CommonPrepareImpl` 调用。
5. 在 `impl/hccl_impl.h` 给 `AscendC::Hccl` 增加对外薄封装。
6. 如果平台差异明显，在 `impl/platform_v220` 或 `impl/platform_v310` 中补平台特化。
7. 在 `cc/src/ops/<op>` 下增加 host API 入口、参数校验和 `OpParam` 组装。
8. 在 `selector` 中新增 auto selector，并通过 `REGISTER_SELECTOR_BY_OPTYPE` 注册。
9. 在 `executor` 中新增 executor，并通过 `REGISTER_EXEC` 注册算法名。
10. 在 `template` 中实现具体 AICPU/CCU/AIV/DPU 通信步骤。
11. 更新 `cc/src/CMakeLists.txt`，把新源文件加入 `mc2_server` 或 `mc2_client`。
12. 如果需要 AICPU config，更新 `ops/op_common/mc2_server.ini`。

## 阅读建议

理解该目录时建议按以下顺序阅读：

1. `impl/hccl_impl.h`：先看对外接口有哪些。
2. `common/hccl_aicpu_def.h` 和 `common/hccl_aicpu_impl.h`：理解 handle、消息区、commit/wait/finalize 生命周期。
3. `impl/platform_v220/hccl_aicpu.h` 和 `impl/platform_v310/hccl_aicpu.h`：比较平台差异。
4. `cc/src/ops/all_gather/all_gather_op.cc` 和 `cc/src/ops/reduce_scatter/reduce_scatter_op.cc`：看 host API 如何进入新路径。
5. `cc/src/ops/*/selector/*_auto_selector.cc`：看算法名如何由 topo、数据量和 engine 决定。
6. `cc/src/ops/*/executor`：看算法名如何转为资源请求和流程编排。
7. `cc/src/ops/*/template`：看底层通信步骤如何被 executor 调用。
8. `cc/src/aicpu_kfc`：最后再看 AICPU server、通信资源和 framework 细节。

