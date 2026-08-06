# InsAllGatherConcurrentMesh1DNHR迁移分析

## 需求

基于最新HCCL源码，对比`InsAllGatherConcurrentMesh1DNHR`与`InsAllGatherMesh1D`的算法语义、选择条件、调用链、任务并发和资源模型，并分析前者迁移到asc-devkit及适配AICPU KFC Server所需的框架修改。

本阶段只做分析，不修改算法实现。分析范围限定为AllGather、单die和AICPU执行路径，不讨论双die算法，也不把AICPU算法接入CCU KFC Server。

## 分析基线

| 项目 | 路径 | 分支 | 提交 |
| --- | --- | --- | --- |
| HCCL源仓 | `../hccl` | `master` | `406b6502dc3eaaeb36f835237738269119aba450` |
| asc-devkit目标仓 | 当前仓 | `cc0806_2` | `b337078caf2f49e94c7053dee30fbe39109070f0` |
| HCCL更新前提交 | `../hccl` | `master` | `530d77c111ede79f390e1e14cc27a50c93ae2e01` |

HCCL已执行`git pull --ff-only`，当前`master`与`origin/master`一致，工作区干净。HCCL从更新前提交到当前提交的差异包含一次较大范围的格式整理，因此不能依据全文件diff直接迁移，必须按调用语义和资源不变量识别有效修改。

本文使用以下状态标记：

- **源码事实**：可以由当前两个仓的源码直接证明。
- **迁移建议**：根据源码事实提出，实施时仍需编译和ST验证。
- **上板确认**：依赖真实UBX拓扑、AICPU TS资源或链路性能，host ST不能代替。

## 核心结论

**源码事实：**用户所说的“多个AICPU kernel”更准确地应描述为“一次AICPU入口kernel launch中的多模板、多AICPU TS thread任务图”。`InsAllGatherMesh1D`和`InsAllGatherConcurrentMesh1DNHR`在host侧都只启动一次通用入口`HcclLaunchAicpuKernel`。区别不在入口kernel数量，而在入口内由executor展开的任务图：

- `InsAllGatherMesh1D`使用一个Mesh1D模板。
- `InsAllGatherConcurrentMesh1DNHR`使用Mesh1D和NHR两个模板，将数据、thread、notify、channel和scratch分区后并发执行。
- Mesh1D本身也不等于单TS thread。其线程数为`rankSize - 1`，只有2 rank场景才是1个TS thread。
- Concurrent算法的两个`KernelRun`在C++编排代码中交替调用，但各自生成的任务被下发到不同TS thread，因此设备任务可以重叠执行。

**源码事实：**asc-devkit当前已经包含Concurrent executor、`TopoMatchUBX`、双模板注册、selector分支和旧版NHR模板。后续工作不是从零迁移，而是将其同步到最新HCCL语义，并完成KFC多thread资源校验和ST闭环。

**源码事实：**最新HCCL通用NHR模板已经支持多Jetty，但Concurrent executor尚未形成完整的多Jetty闭环。它一方面调用`CalcChannelRequestNhrMultiJetty`，另一方面又要求Mesh和NHR的channel描述总数相等。Mesh侧每个对端只取1条channel，因此该校验使当前Concurrent有效路径实际上只能使用NHR单Jetty。若真实拓扑返回多条NHR channel，`CalcRes`会直接失败。

**迁移建议：**不得把Concurrent算法改成host侧连续启动两个`HcclLaunchAicpuKernel`。正确方案是保留一次入口launch，扩展并验证同一个`AlgResourceCtxSerializable`内的多thread、notify、channel和scratch资源。

## HCCL调用链

### 公共入口

两个算法共用以下调用链：

```text
HcclAllGather
  -> AllGatherOutPlace
  -> AllGatherOutPlaceCommon
  -> Selector
  -> HcclExecOp
  -> HcclAicpuKernelEntranceLaunch
  -> AicpuKernelLaunch("HcclLaunchAicpuKernel")
  -> HcclLaunchAicpuKernel
  -> CollAlgExecRegistryV2::GetAlgExec
  -> executor->Orchestrate
```

`AllGatherOutPlaceCommon`构造`OpParam`，调用selector获得`algName`，再调用`HcclExecOp`。AICPU路径通过`HcclAicpuKernelEntranceLaunch`启动固定入口`HcclLaunchAicpuKernel`。入口根据`param->algName`从注册表取得executor，并调用一次`Orchestrate`。

关键源码位置如下：

- HCCL `src/ops/all_gather/all_gather_op.cc:22`：AllGather公共API入口。
- HCCL `src/ops/all_gather/all_gather_op.cc:179`：`AllGatherOutPlaceCommon`。
- HCCL `src/ops/all_gather/all_gather_op.cc:245`：selector和`HcclExecOp`调用。
- HCCL `src/ops/op_common/template/aicpu/kernel_launch.cc:275`：`OpOrchestrate`。
- HCCL `src/ops/op_common/template/aicpu/kernel_launch.cc:308`：按算法名获取executor。
- HCCL `src/ops/op_common/template/aicpu/kernel_launch.cc:315`：调用`executor->Orchestrate`。
- HCCL `src/ops/op_common/template/aicpu/kernel_launch.cc:342`：通用AICPU入口。

### 算法注册

`InsAllGatherMesh1D`注册为单模板executor：

```cpp
REGISTER_EXEC_V2(
    HcclCMDType::HCCL_CMD_ALLGATHER,
    InsAllGatherMesh1D,
    InsV2AllGatherSoleExecutor,
    TopoMatch1D,
    InsTempAllGatherMesh1D);
```

源码位置为HCCL `src/ops/all_gather/executor/ins_v2_all_gather_sole_executor.cc:273`。

`InsAllGatherConcurrentMesh1DNHR`注册为双模板executor：

```cpp
REGISTER_EXECUTOR_BY_TWO_TEMPS(
    HcclCMDType::HCCL_CMD_ALLGATHER,
    InsAllGatherConcurrentMesh1DNHR,
    InsV2AllGatherConcurrentExecutor,
    TopoMatchUBX,
    InsTempAllGatherMesh1D,
    InsTempAllGatherNHR);
```

源码位置为HCCL `src/ops/all_gather/executor/ins_v2_all_gather_concurrent_executor.cc:493`。

注册差异决定了后续资源计算和编排方式：Sole executor只构造一个template，Concurrent executor必须分别计算两个template的资源，再组合成一个可序列化资源上下文。

## Selector选择条件

### InsAllGatherMesh1D

在AICPU选择路径中，`InsAllGatherMesh1D`主要覆盖以下情况：

- `topoLevelNums == 1`且`level0Topo == MESH_1D`时，默认选择Mesh1D。
- 上述Mesh1D场景若被判断为两层网络的大数据路径，可能改选`InsAllGatherMesh1D1DZAxisDetour`。
- `level0Topo == MESH_1D_CLOS`且为PCIe mix时，如果Mesh通路已经连接全部rank，也选择Mesh1D。
- `level0Topo == MESH_1D_CLOS`、非PCIe mix、Mesh rank数等于CLOS rank数、`userRankSize <= 4`时，单rank数据量不大于512 KiB则选择Mesh1D。

### InsAllGatherConcurrentMesh1DNHR

Concurrent算法只在以下条件同时满足时选择：

- AICPU路径。
- `topoLevelNums == 1`。
- `level0Topo == MESH_1D_CLOS`。
- `level0PcieMix == false`，即UBX路径。
- Mesh通信域rank数与CLOS通信域rank数相等。
- `userRankSize <= MAX_RANK_NUM_FOR_CONCURRENT_ALGO`，当前常量语义为最多4 rank。
- 单rank数据量`dataSize > 512 KiB`。

当数据量恰好为512 KiB时不会进入Concurrent算法，而是回退Mesh1D。条件位于HCCL `src/ops/all_gather/selector/all_gather_auto_selector.cc:358`至`src/ops/all_gather/selector/all_gather_auto_selector.cc:390`。

这说明Concurrent算法不是Mesh1D的通用替代，而是UBX小rank大数据场景中同时利用Mesh和CLOS两类通路的性能算法。

## 拓扑匹配差异

`InsAllGatherMesh1D`使用`TopoMatch1D`，为模板提供一个通信rank集合。`InsAllGatherConcurrentMesh1DNHR`使用`TopoMatchUBX`。在`MESH_1D_CLOS`场景中，`TopoMatchUBX`遍历第0层网络实例并按拓扑类型拆分：

```text
algHierarchyInfo.infos[0][0] = Mesh1D ranks
algHierarchyInfo.infos[0][1] = CLOS ranks
```

Concurrent executor固定把`infos[0][0]`交给Mesh1D模板，把`infos[0][1]`交给NHR模板。最新HCCL已在`CalcRes`和`Orchestrate`入口检查`infos[0]`至少包含两个子通信域，避免旧代码直接索引造成越界。证据位于HCCL `src/ops/op_common/topo/topo_match_ubx.cc:19`至`src/ops/op_common/topo/topo_match_ubx.cc:69`以及Concurrent executor的`CalcRes`、`Orchestrate`入口。

## 算法语义对比

| 对比项 | InsAllGatherMesh1D | InsAllGatherConcurrentMesh1DNHR |
| --- | --- | --- |
| executor | `InsV2AllGatherSoleExecutor` | `InsV2AllGatherConcurrentExecutor` |
| template数量 | 1个Mesh1D模板 | Mesh1D和NHR两个模板 |
| 拓扑输入 | 1个全rank通信域 | 1个Mesh通信域和1个CLOS通信域 |
| 数据范围 | 单模板处理全部输入 | 前段交给Mesh，后段交给NHR |
| 网络利用 | 主要使用Mesh直连 | Mesh和CLOS通路并发利用 |
| 通信方式 | 每个对端直接读取对应slice | Mesh直接读取，NHR按多步交换传播slice |
| TS thread | `P - 1` | 当前有效路径为`P + 1`；完整多Jetty目标为`(P - 1) + 2C` |
| channel | Mesh对端channel | Mesh channel集合和NHR多Jetty channel集合 |
| scratch | 单模板使用 | 按模板比例切成不重叠子区 |
| 模板间同步 | 无 | 前同步启动，尾同步汇合 |
| host入口launch | 1次 | 1次 |

表中`P`为模板rank数，`C`为最新NHR中每个remote rank的channel数。由于当前Concurrent executor保留channel总数等量校验，其可达成功路径中`C = 1`。

### Mesh1D执行过程

Mesh1D的任务过程如下：

```text
本rank输入
  -> LocalDataCopy到本rank输出位置
  -> 必要时同时复制到scratch
  -> 主从thread前同步
  -> 每个对端rank使用独立thread直接读取其slice
  -> 主从thread尾同步
```

`GetThreadNum()`返回`templateRankSize - 1`，最小为1。对于每个非本rank，模板在对应Mesh channel上构造直接读取任务。它不需要像NHR那样经过多步传播来收集全部数据。

相关证据位于HCCL `src/ops/all_gather/template/aicpu/ins_temp_all_gather_mesh_1D.cc:25`至`src/ops/all_gather/template/aicpu/ins_temp_all_gather_mesh_1D.cc:99`、`src/ops/all_gather/template/aicpu/ins_temp_all_gather_mesh_1D.cc:156`至`src/ops/all_gather/template/aicpu/ins_temp_all_gather_mesh_1D.cc:278`。

### Concurrent执行过程

Concurrent executor先独立调用两个模板的`CalcRes`，再把资源合并：

```text
总slaveThreadNum
  = Mesh slaveThreadNum
  + NHR slaveThreadNum
  + 1个NHR模板主thread
```

所以理论总thread数为两个模板thread数之和：

```text
Mesh threads = P - 1
NHR threads = 2C
Concurrent total threads = (P - 1) + 2C
```

两个模板还分别取得独立channel子集。最新HCCL对Mesh使用`CalcChannelRequestMesh1DWithPriorityTopo`，对NHR使用`CalcChannelRequestNhrMultiJetty`。

但当前源码随后检查`temp0Channels.size() == temp1Channels.size()`。对于最多4 rank的Concurrent选择范围，NHR连接rank集合覆盖全部其他rank；Mesh侧每个remote rank只取1条channel，所以只有NHR也为每个remote rank取1条channel时才能通过。当前成功路径实际为：

```text
C = 1
Mesh threads = P - 1
NHR threads = 2
Concurrent total threads = P + 1
```

这说明`CalcChannelRequestNhrMultiJetty`在Concurrent中的接入尚未完成。如果目标是严格复现当前HCCL行为，应保留单Jetty可达范围和失败条件。如果目标是让Concurrent真正利用多个NHR Jetty，则必须同时修改channel分区、`channelsPerRank_`恢复、thread分区和ST，不能只删除等量校验。

在AICPU TS模式下，数据按固定带宽权重切分：

```text
Mesh权重 = 37
CLOS权重 = 25
Mesh数据比例 = 37 / 62
NHR数据比例 = 25 / 62
```

Mesh处理从偏移0开始的前段，NHR处理紧随其后的后段。切分点向下按`HCCL_MIN_SLICE_ALIGN`对齐，二者数据区间不重叠且覆盖完整输入。scratch也按两个模板的`CalcScratchMultiple`和数据比例分为不重叠子区。

executor先调用`PreSyncInterThreads`使第二个模板主thread与第一个模板主thread建立启动顺序，再按loop索引交替调用两个模板的`KernelRun`，最后调用`PostSyncInterThreads`汇合。这里的“交替调用”只表示任务生成顺序；任务目标thread不同，因此TS可以并行调度。

相关证据位于HCCL `src/ops/all_gather/executor/ins_v2_all_gather_concurrent_executor.cc:73`至`src/ops/all_gather/executor/ins_v2_all_gather_concurrent_executor.cc:134`、`src/ops/all_gather/executor/ins_v2_all_gather_concurrent_executor.cc:171`至`src/ops/all_gather/executor/ins_v2_all_gather_concurrent_executor.cc:221`以及`src/ops/all_gather/executor/ins_v2_all_gather_concurrent_executor.cc:289`至`src/ops/all_gather/executor/ins_v2_all_gather_concurrent_executor.cc:409`。

## 最新NHR性能实现

asc-devkit当前NHR仍是旧实现：

- 使用`CalcChannelRequestNhr`，每个目标只取单channel。
- `GetThreadNum()`固定返回1。
- 所有NHR step都写入scratch。
- 全部通信结束后才统一执行`PostLocalCopy`。

最新HCCL NHR已经改为多Jetty、多thread和通信复制重叠：

- UBX路径使用`CalcChannelRequestNhrMultiJetty`并过滤`COMM_PROTOCOL_UBC_CTP` channel。
- 用`CalcChannelsPerRank`计算`channelsPerRank_`。
- `GetThreadNum()`返回`channelsPerRank_ * 2`。
- 每个从thread申请2个notify，一个用于模板主从同步，另一个用于末步通信与`PostLocalCopy`并行。
- 依据端口组将每个slice进一步切到多个channel，每个channel使用独立通信thread。
- 满足条件时，NHR最后一步直接从远端读到output，避免先落scratch再复制。
- 在最后一步通信thread读取剩余slice的同时，第二组thread把已经位于scratch的数据复制到output。
- 输入输出布局满足严格条件时，可以跳过本rank slice的重复复制。
- 对thread和channel下标增加边界检查。

源码证据位于HCCL `src/ops/all_gather/template/aicpu/ins_temp_all_gather_nhr.cc:23`至`src/ops/all_gather/template/aicpu/ins_temp_all_gather_nhr.cc:68`、`src/ops/all_gather/template/aicpu/ins_temp_all_gather_nhr.cc:79`至`src/ops/all_gather/template/aicpu/ins_temp_all_gather_nhr.cc:159`、`src/ops/all_gather/template/aicpu/ins_temp_all_gather_nhr.cc:257`至`src/ops/all_gather/template/aicpu/ins_temp_all_gather_nhr.cc:361`以及`src/ops/all_gather/template/aicpu/ins_temp_all_gather_nhr.cc:400`之后。

这部分既改变性能，也改变NHR的资源规模和notify布局，是后续同步的核心，不能只复制Concurrent executor而保留旧NHR。但对于Concurrent算法，最新HCCL还存在两个需要在迁移中显式处理的状态问题：

- `CalcRes`阶段的NHR对象根据真实channel计算`channelsPerRank_`并申请`2C`个thread。
- AICPU入口执行`Orchestrate`时会重新构造NHR对象，其`channelsPerRank_`恢复为基类默认值1。Concurrent executor没有像AllGather Sole、AllReduce Concurrent和ReduceScatter Concurrent executor那样调用`SetchannelsPerRank`。

当前channel等量校验使成功路径的`C`只能为1，暂时掩盖了第二个问题。若后续解除等量限制以支持真实多Jetty，必须先按实际NHR channel map调用`SetchannelsPerRank`，再执行`GetRes`和thread分区，否则host申请`2C`个thread，AICPU展开却只使用2个thread和第1条Jetty。

## asc-devkit现状与缺口

### 已具备能力

asc-devkit当前已经具备以下骨架：

- selector已经在单层`MESH_1D_CLOS`、Mesh和CLOS rank数相等、最多4 rank、数据量大于512 KiB时选择`InsAllGatherConcurrentMesh1DNHR`。
- 已有`InsV2AllGatherConcurrentExecutor`及双模板注册。
- 已有`TopoMatchUBX`。
- 已有Mesh1D和NHR模板，相关文件已进入AllGather的CMake组织。
- `HcclGetThread`按照`slaveThreadNum + 1`动态申请AICPU TS thread，不限制为单thread。
- `AlgResourceCtxSerializable`能够携带完整thread、notify、channel、拓扑和scratch信息。
- AICPU KFC open server缓存完整的`AlgResourceCtxSerializable`，再在server侧按`algName`取得本地executor并调用`Orchestrate`。

因此，现有公共框架在结构上能够承载Concurrent算法，无需新建“多AICPU kernel数组”或新增第二个入口kernel。

### 必须同步的缺口

**源码事实：**asc-devkit与最新HCCL相比存在以下实际缺口：

- `channel.h`和`channel.cc`缺少`CalcChannelRequestNhrMultiJetty`。
- asc-devkit的NHR仍为固定1 thread、单channel旧版，缺少最新多Jetty拆分和末步复制重叠。
- Concurrent executor仍使用旧的`CalcChannelRequestNhrWithPriorityTopo`，没有同步最新多Jetty资源计算。
- Concurrent executor缺少最新HCCL中的拓扑数组、thread、CCU kernel和channel访问边界检查。
- 当前Concurrent executor的`scratchSizeforTemp1`仍为`scratchMemBlockSize - scratchSizeforTemp0`，而最新HCCL已改为`maxTmpMemSize_ - scratchSizeforTemp0`。旧表达式在`totalScratchMultiple > 1`时存在无符号下溢或错误缩小第二模板scratch的风险。
- 最新Concurrent executor显式设置`tailSize`，并按执行引擎选择数据切分权重；asc-devkit需要逐项对齐。
- 最新HCCL Concurrent自身仍保留Mesh与NHR channel总数相等的限制，尚不支持NHR真实多Jetty。
- 最新HCCL Concurrent在AICPU展开阶段未恢复NHR的`channelsPerRank_`。该问题在当前`C = 1`限制下被掩盖，但解除限制后会造成申请资源与实际任务图不一致。

**迁移建议：**同步顺序必须是channel helper、NHR template、Concurrent executor，不能只替换最后一个文件。否则executor计算的NHR thread和channel资源与实际模板需求不一致。实施前还必须在以下两个目标中做出明确选择：

- **同源等价阶段**：保持`C = 1`的有效范围，与当前HCCL行为一致，优先完成正确性和KFC闭环。
- **完整多Jetty阶段**：解除channel总数等量限制，并按实际channel map恢复NHR状态。这是对当前HCCL Concurrent路径的补全，需要独立性能和稳定性验证。

### 暂不合入的公共变化

HCCL最新代码新增AICPU task cache，并将编排过程抽出为`OpOrchestrate`以支持cache lookup、start、end和execute。这是公共性能优化，不是Concurrent算法正确运行的前置条件。

**迁移建议：**第一阶段保持asc-devkit现有一次入口和直接`Orchestrate`路径，先证明Concurrent任务图正确。task cache应作为独立公共框架需求评估，不能与本算法同步混在同一个最小变更中。

## AICPU KFC Server适配方案

### 适配边界

AICPU KFC open server与本算法直接相关。server侧`OpenResCtxHolder`是完整`AlgResourceCtxSerializable`的只读共享指针，`LaunchOpenOpParamDataImpl`根据`algName`取得executor后直接调用：

```cpp
executor->Orchestrate(*param, resCtx);
```

因此正确的server执行单元是“整个Concurrent executor”，不是两个独立子kernel。

CCU KFC Server及`DispatchKfcSubKernel`不在本算法适配范围内。它们负责CCU IR中的子kernel分发，不应为AICPU版`InsAllGatherConcurrentMesh1DNHR`新增dispatch分支。未来若迁移CCU同类Concurrent算法，需要另行分析多个`ccuKernelInfos`的替换和server IR资源继承。

### 框架修改建议

**迁移建议：**不改AICPU入口launch数量，完成以下增强：

- 在open resource构建阶段校验`threads.size() == slaveThreadNum + 1`，而不是只检查`threads[0]`有效。
- 校验`notifyNumPerThread.size() == slaveThreadNum`，并检查每个thread的notify需求可由已申请资源满足。
- 校验`algHierarchyInfo.infos[0].size() >= 2`，且两个子通信域均包含本rank。
- 校验序列化和反序列化后的channel总数、remote rank分组及每rank channel数与资源计算一致。
- 若启用完整多Jetty，在划分thread前先从NHR channel map调用`SetchannelsPerRank`，再计算NHR的`GetThreadNum`和notify布局。
- 在调用Concurrent executor前记录thread数、两模板thread切分点、Mesh和NHR channel数、scratch分界，便于e2e故障定位。
- 保持同一个`OpenResCtxHolder`覆盖整个算法生命周期，禁止为两个模板分别建立独立cache entry。
- KFC消息只更新本轮动态输入、输出、count和stride等参数，不覆盖静态拓扑、thread、notify和channel资源。

这些修改是资源完整性保护，不改变KFC Server的调度协议。

### 资源不变量

实现和ST应共同检查以下不变量：

```text
threads.size() == slaveThreadNum + 1
MeshThreadRange和NHRThreadRange不重叠
notifyNumPerThread.size() == slaveThreadNum
MeshChannelRange和NHRChannelRange不重叠
MeshDataRange并集NHRDataRange == 完整输入范围
MeshDataRange交集NHRDataRange == 空集
MeshScratchRange交集NHRScratchRange == 空集
MeshScratchSize + NHRScratchSize <= cclMem.size
```

若任一不变量不满足，应在展开任务前返回参数或内部错误，不能依赖运行时访问失败暴露问题。

## 建议实施阶段

### 同步HCCL算法依赖

- 在asc-devkit的channel模块适配`CalcChannelRequestNhrMultiJetty`及其直接依赖，复用本仓已有Hcomm抽象。
- 同步最新`InsTempAllGatherNHR`的多Jetty、双倍thread、数据拆分、末步直写output和复制重叠逻辑。
- 对新增helper逐个核对asc-devkit已有声明、命名空间、CANN版本条件和C++标准，不做目录整体覆盖。

### 同步Concurrent executor

- 同步拓扑、thread、channel和kernel资源边界检查。
- 同步AICPU TS固定带宽权重`37:25`和引擎参数传递。
- 修正scratch第二分区大小计算。
- 同步`tailSize`和loop尾块处理。
- 同源等价阶段保留channel等量校验，并明确断言NHR每个remote rank只有1条channel。
- 完整多Jetty阶段改为按已知Mesh channel数量或显式资源元数据分区channel，不能继续依赖总数对半或等量假设。
- 完整多Jetty阶段必须在`PrepareResForTemplate`前恢复两个模板的`channelsPerRank_`，再依据恢复后的`GetThreadNum`切分thread。
- 保持双模板注册名、selector算法名和`AlgorithmType`映射完全一致。

### 增强KFC资源检查

- 在`open_res_ctx_cache`或等价资源入口增加多thread资源一致性检查。
- 在`comm_kfc_open_kernel_executor`增加Concurrent资源摘要日志和前置校验。
- 保持一次server调度调用整个executor。

### 补充测试

- 先完成host级selector、资源计算、任务图和KFC资源序列化测试。
- 再执行AICPU KFC e2e，证明调用未转发到安装版HCCL，并验证结果正确。
- 最后上真实UBX设备验证并发重叠和性能收益。

## ST方案

### Selector与注册ST

至少覆盖以下选择边界：

| 场景 | 期望算法 |
| --- | --- |
| 单层纯`MESH_1D` | `InsAllGatherMesh1D` |
| 单层`MESH_1D_CLOS`，Mesh和CLOS rank数相等，4 rank，512 KiB | `InsAllGatherMesh1D` |
| 同上，512 KiB加一个数据元素 | `InsAllGatherConcurrentMesh1DNHR` |
| 同上，大于4 rank | 不选择Concurrent |
| `level0PcieMix == true` | 不选择Concurrent |
| Mesh和CLOS rank数不相等 | 不选择Concurrent |

同时断言`CollAlgExecRegistryV2`能以`HCCL_CMD_ALLGATHER`和准确算法名取得Concurrent executor。

### 资源与任务图ST

覆盖2、3、4 rank。单Jetty是同源等价阶段的必测项，多Jetty是完整多Jetty阶段的必测项。检查：

- 单Jetty时总thread数为`P + 1`；完整多Jetty时满足`(P - 1) + 2C`。
- 两模板thread切片的起止位置正确。
- notify索引不越界，模板间前同步和尾同步成对。
- channel按Mesh和NHR正确分组，相同remote rank的多Jetty没有丢失。
- 37:25数据切分在对齐后无重叠、无空洞。
- scratch切分无下溢、无越界。
- 单loop、多loop、非整除尾块和最小对齐边界均能生成合法任务。
- NHR末步直写output路径和保守scratch路径都被覆盖。

### AICPU KFC e2e

e2e必须走真实路径：

```text
selector
  -> CalcRes
  -> AlgResourceCtxSerializable序列化
  -> KFC open server缓存资源
  -> server更新动态OpParam
  -> 本地Concurrent executor->Orchestrate
  -> 多TS thread任务执行
  -> AllGather结果校验
```

测试需要以日志或stub断言证明没有通过`cann_host_bridge`转发到安装版HCCL。覆盖2、3、4 rank、阈值以上数据、多loop和连续多轮调用，验证resource cache复用后thread、notify和channel仍保持一致。

### 上板性能验证

以下项目只能在真实UBX硬件确认：

- Mesh和CLOS任务是否真实时间重叠。
- `channelsPerRank_`与实际Jetty资源数量是否一致。
- 总TS thread和notify数量是否处于平台资源上限内。
- 37:25切分在目标机型上的收益是否仍优于纯Mesh1D。
- NHR末步直写和`PostLocalCopy`重叠是否产生预期收益。

性能验收不能只看总耗时，还应抓取任务时间线，确认不是串行执行后因其他波动获得表面收益。

## 风险与处理

| 风险 | 影响 | 处理建议 |
| --- | --- | --- |
| 将多thread误判为多入口kernel | 重复资源申请、错误同步或两次完成通知 | 保持一次`HcclLaunchAicpuKernel`和一次executor调度 |
| 只迁移Concurrent executor | NHR资源需求与模板实现不一致 | 按channel helper、NHR、executor顺序同步 |
| 认为上游Concurrent已支持真实多Jetty | 多channel场景在`CalcRes`失败，或解除校验后状态恢复错误 | 区分同源等价和完整多Jetty两个阶段 |
| 解除channel等量校验但未恢复`channelsPerRank_` | host申请`2C`个thread，展开只使用2个 | 先构造channel map并调用`SetchannelsPerRank`，再计算和划分thread |
| 旧scratch计算下溢 | NHR scratch越界或loop计算异常 | 使用总CCL Buffer余量计算第二分区并增加边界断言 |
| channel排列假设变化 | Mesh和NHR拿到错误channel | 在CalcRes保留稳定分组，并在Orchestrate校验数量和remote rank |
| KFC只校验主thread | 缺失从thread直到任务下发才失败 | 在资源缓存入口校验完整thread和notify向量 |
| 机械合入HCCL全仓diff | 引入格式变更、task cache和范围外算法 | 按语义提取最小patch，task cache独立评估 |
| host ST误走安装版HCCL | 假通过 | 增加本地selector、registry和executor命中断言 |
| host ST代替硬件并发验证 | 无法证明性能和资源上限 | 将正确性ST与UBX上板性能验收分开 |

## 开放问题

以下事项在实现前或上板前需要确认，但不阻塞当前架构结论：

- 目标UBX机型每个remote rank实际可提供的`COMM_PROTOCOL_UBC_CTP` channel数及最大Jetty数。
- 本次迁移验收目标是严格对齐HCCL当前单Jetty Concurrent有效路径，还是同时补全Concurrent真实多Jetty能力。
- 2、3、4 rank下AICPU TS thread和notify总量的硬件上限。
- asc-devkit KFC e2e环境能否构造同时包含Mesh和CLOS实例的真实rank graph；若不能，需要host mock和上板用例共同覆盖。
- 37:25权重是否直接继承HCCL当前值，还是需要针对目标版本重新性能标定。正确性实现应先保持与HCCL一致。

## 迁移结论

`InsAllGatherConcurrentMesh1DNHR`与`InsAllGatherMesh1D`共享同一个AllGather入口和一次AICPU通用kernel launch。前者的本质是双模板并发任务图，而不是多个独立AICPU入口kernel。其性能来自同时使用Mesh和CLOS通路、按37:25切分数据，并把两个模板的任务下发到独立TS thread和channel。

asc-devkit已经具备该算法的注册、selector、拓扑匹配、Concurrent executor和KFC完整资源上下文骨架，但算法内容落后于最新HCCL。后续应以“增量同步和框架加固”为目标：先补多Jetty channel helper和最新NHR，再同步Concurrent executor的资源与边界修复，最后增强AICPU KFC对多thread资源的完整校验和e2e ST。

最新HCCL通用NHR的多Jetty能力不能直接等同于Concurrent算法已经支持多Jetty。Concurrent当前的channel等量校验把成功路径限制为`C = 1`，并掩盖了AICPU展开阶段没有恢复`channelsPerRank_`的问题。建议先完成与上游当前可达行为一致的单Jetty迁移，再将完整多Jetty支持作为明确的第二阶段框架增强。

当前证据不支持增加第二个AICPU kernel launch，也不支持把该AICPU算法接入CCU的`DispatchKfcSubKernel`。这两种改法都会偏离HCCL现有执行模型。
