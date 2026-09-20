# HCCL集合通信外部算法名配置指南

除了自动选路与直接指定算法注册名，集合通信算法还支持通过**外部算法名**（如`parallel[mesh, nhr]`）描述算法的"图式意图"，由运行环境结合通信引擎与组网拓扑，确定性地解析为唯一的具体算法。本指南面向需要精确控制通信算法的调优与测试人员，介绍外部算法名的语法、配置方式、运行时消歧规则、失败报错行为与版本兼容性。

## 1. 语法

### 1.1 文法

外部算法名不含算子前缀（不写`AllGather=`），算子类型由tiling配置中的`opType`决定。文法如下：

```text
external  := executor '[' layer (',' layer)* ']' ('[order.strong]')?
executor  := sole | sequence | parallel | concur | pipeline
layer     := atom ('.' attr)*
atom      := mesh | nhr
attr      := chunk | one_shot | two_shot | multi_channel | single_channel
```

-   `executor`为编排方式，必填；
-   逗号列表中的原子按拓扑层从低到高排列，至少1层、最多3层；
-   `.<属性>`描述该层的落地方式，可选，可叠加；
-   `[order.strong]`为算子级保序后缀，可选。

### 1.2 词表

**表1**  编排（executor，5个）

| 取值 | 含义 | 典型写法 |
| --- | --- | --- |
| `sole` | 单层（或摊成一层） | `sole[mesh]` |
| `sequence` | 各层串行 | `sequence[mesh, nhr]` |
| `parallel` | 各层并行 | `parallel[mesh, nhr]` |
| `concur` | 两种原子并跑 | `concur[mesh, nhr]`、`concur[mesh, mesh]` |
| `pipeline` | 流水编排 | `pipeline[mesh, nhr]` |

**表2**  原子（atom，2个）

| 原子 | 含义 |
| --- | --- |
| `mesh` | 1D Fullmesh |
| `nhr` | 非均衡层次环 |

**表3**  属性（attr，5个）

| 属性 | 含义 | 可修饰原子 |
| --- | --- | --- |
| `chunk` | 分chunk传输，减小scratch占用 | `mesh` |
| `one_shot` | 一趟Mesh完成AllReduce | `mesh` |
| `two_shot` | 同层先ReduceScatter再AllGather | `mesh` |
| `multi_channel` | 多通道 | `mesh`、`nhr` |
| `single_channel` | 每对端仅1条通道 | `mesh` |

属性写在原子之后、以`.`分隔，可按书写顺序叠加，如`mesh.two_shot.chunk`、`nhr.multi_channel`。

### 1.3 保序后缀

```text
sole[mesh]                 # 非保序
sole[mesh] [order.strong]  # 保序
```

-   未写`[order.strong]`即默认非保序；
-   当前版本该标记仅被校验与记录，不影响算法选定结果。

### 1.4 书写约束

-   **层数限制**：1~3层。超过3层直接判为语法错误，不会进入候选查询。
-   **空格容忍**：名称中的空格与制表符会被全部忽略，`parallel[mesh, nhr]`与`parallel[mesh,nhr]`等价。
-   **拼写警示**：多通道属性的拼写为`multi_channel`。早期需求文档曾误写为`muti_channel`（少一个字母l），实现时已纠正；词表严格按拼写匹配，旧拼写`muti_channel`不被识别，将导致解析失败（AICPU链回退默认选路，CCU链预检拒绝）。
-   **以下内容不要写进外部名**：

**表4**  不要写进外部名的内容

| 不要写 | 原因 |
| --- | --- |
| `AllGather=`等算子前缀 | 算子类型由opType决定 |
| 引擎（如Aicpu、CcuSched） | 由通信引擎与消歧规则决定 |
| `level0:`等层号 | 逗号顺序即层次 |
| `mesh.2die`、`nhr.soft_reduce` | 运行时按拓扑与数据类型自动识别 |
| `mesh.detour`、`mesh.peer_only` | 运行时按组网与链路条件自动识别 |
| `pcie`、`ubx`等机型词 | 机型由组网自动识别 |
| `nhr.multilink`、`mesh.multilink` | 旧写法，对应属性为`multi_channel` |

### 1.5 配置示例

```text
sole[mesh]                  # 单层Mesh
sole[mesh.two_shot]         # 带属性
sole[mesh] [order.strong]   # 保序后缀
sole[mesh.single_channel]   # AllToAll单通道
parallel[mesh, nhr]         # 两层并行
parallel[mesh, nhr.multi_channel]
concur[mesh, mesh]          # 两路Mesh并跑
sequence[mesh, nhr]         # 两层串行
```

语法合法不等于存在对应算法：候选是否存在由第3章映射表决定，配置映射表之外的外部名（如`pipeline[mesh,nhr,nhr]`）会按第4章处理（AICPU链回退默认选路，CCU链预检拒绝）。

## 2. 配置方式

外部算法名通过HCCL Tiling配置类`Mc2CcTilingConfig`的`algConfig`参数传入，与裸名强制算法为同一入口。`algConfig`为字符串类型，支持的最大长度为128字节，可通过[构造函数](../../api/SIMD-API/adv_api/HCCL_communication/HCCL_Tiling/HCCL_Tiling_constructor.md)的第3个参数或[SetAlgConfig](../../api/SIMD-API/adv_api/HCCL_communication/HCCL_Tiling/SetAlgConfig.md)接口设置。

```cpp
const char* groupName = "testGroup";
uint32_t opType = HCCL_CMD_ALLGATHER;
std::string algConfig = "parallel[mesh, nhr]";
AscendC::Mc2CcTilingConfig mc2CcTilingConfig(groupName, opType, algConfig);
```

系统按以下规则识别`algConfig`的内容：

**表5**  algConfig四态识别

| algConfig内容 | 识别结果 | 行为 |
| --- | --- | --- |
| 空字符串 | 未配置 | 走自动选路 |
| 含`=` | 老语法（如`ReduceScatter=level0:fullmesh`） | AICPU链走默认选路，不强制具体算法；CCU链报算法不支持（历来如此） |
| 含`[` | 外部算法名 | 按第1章语法解析并消歧，失败按第4章处理 |
| 其余 | 裸注册名 | 直接强制指定该注册名（原有语义不变） |

即：只要配置中包含`[`且不含`=`，就会进入外部算法名通道。

## 3. 运行时消歧规则

### 3.1 三级漏斗

同一个外部名在同一个算子下可能对应多个注册算法（最多4个候选）。运行时按三级漏斗逐级过滤，第一个通过全部检查的候选即为最终算法：

1.  **①引擎匹配**：候选算法带有引擎属性，仅保留与当前通信引擎匹配的候选：
    -   AICPU候选：通信引擎为AICPU（含AICPU TS通道）时保留；
    -   CCU候选：通信引擎为CCU时保留；
    -   DPU候选：仅通信引擎为HOST CPU（算法卸载至DPU执行的场景）时保留。
2.  **②变体条件**：带变体条件的候选，须满足对应条件（见表6）才保留。
3.  **②′机型flag**：带机型flag的候选，须满足对应组网条件（见表7）才保留。

无变体条件、无机型flag的候选（裸名兜底候选）始终通过②②′，但排在全部特化候选之后，即"特化候选优先、裸名兜底"。

### 3.2 消歧规则

**表6**  变体条件

| 规则 | 判定条件 |
| --- | --- |
| `COND_Z_AXIS` | 两层组网，且（卡数为2且单卡数据量不小于4MB，或单卡数据量×卡数大于1536MB） |
| `COND_PEER_ONLY` | 通信域卡数为2，且到对端仅有1条链路 |
| `COND_SPECIAL_DT` | 数据类型为64bit，或归约类型为PROD |

**表7**  机型flag

| 规则 | 判定条件 |
| --- | --- |
| `FLAG_MULTI_LEVEL` | 组网层数大于1且第0层为1D Mesh |
| `FLAG_UBX` | 第0层为1D CLOS且非PCIE混联 |
| `FLAG_PCIE_MIX` | 第0层为1D CLOS且PCIE混联，且第0层无法由单一1D Mesh全覆盖 |
| `FLAG_FLAT_1D` | 第0层不是1D CLOS（仅AllToAll裸名使用） |

### 3.3 确定性原则

同样的输入（外部名+通信引擎+通信域环境）总是解析为同一个算法：

-   候选顺序由"特异性降序、注册名字典序"唯一确定，与算法注册顺序无关；
-   消歧过程为纯规则判断，不做运行时资源探测，不依赖资源状态。

### 3.4 36组合映射表

下表列出当前版本全部36个「算子×外部名×引擎」组合及解析结果（注册名）。查表方式：先按算子与外部名定位，再按当前通信引擎取对应行。带变体条件/机型flag的组合在"消歧说明"中注明命中规则，规则含义见3.2节。

**表8**  AllGather（10组合）

| 外部名 | 引擎 | 解析结果（注册名） | 消歧说明 |
| --- | --- | --- | --- |
| `concur[mesh,nhr]` | AICPU | `AicpuAllGatherConcurMeshNHR` | 唯一候选 |
| `concur[mesh,nhr.multi_channel]` | CCU | `CcuSchedAllGatherConcurMeshNHRMultiLink` | 唯一候选 |
| `pipeline[mesh,nhr]` | AICPU | `AicpuAllGatherPipeLinePcie` | 唯一候选 |
| `parallel[mesh,nhr]` | AICPU | `InsAllGatherParallelMesh1DNHR` / `InsAllGatherParallelMesh1DNHRUBX` / `InsAllGatherParallelMesh1DNHRPcie` | 机型三选一：`FLAG_MULTI_LEVEL` / `FLAG_UBX` / `FLAG_PCIE_MIX`，均不命中则回退默认选路 |
| `parallel[mesh,nhr.multi_channel]` | AICPU | `InsAllGatherParallelMesh1DNHRMultiJetty` | 唯一候选 |
| `parallel[mesh,nhr.multi_channel]` | CCU | `CcuSchedAllGatherParallelMeshNHRMultiLink` | 唯一候选 |
| `sequence[mesh,nhr]` | DPU | `InsAllGatherMeshNhrDPU` | 唯一候选（仅HOST CPU引擎生效） |
| `sole[mesh]` | AICPU | `InsAllGatherMesh1D` | 唯一候选 |
| `sole[mesh]` | CCU | `CcuSchedAllGatherSoleMesh` | 唯一候选 |
| `sole[nhr]` | AICPU | `AicpuAllGatherSoleNHR` | 唯一候选 |

**表9**  ReduceScatter（11组合）

| 外部名 | 引擎 | 解析结果（注册名） | 消歧说明 |
| --- | --- | --- | --- |
| `concur[mesh,nhr]` | AICPU | `AicpuReduceScatterConcurMeshNHR` | 唯一候选 |
| `concur[mesh,nhr.multi_channel]` | CCU | `CcuSchedReduceScatterConcurMeshNHRMultiLink` | 唯一候选 |
| `parallel[mesh,nhr]` | AICPU | `InsReduceScatterParallelMesh1DNHR` / `AicpuReduceScatterParallelMeshNHRUBX` / `InsReduceScatterParallelMesh1DNHRPcie` | 机型三选一：`FLAG_MULTI_LEVEL` / `FLAG_UBX` / `FLAG_PCIE_MIX`，均不命中则回退默认选路 |
| `pipeline[mesh,nhr]` | AICPU | `AicpuReduceScatterPipeLinePcie` | 唯一候选 |
| `sequence[mesh,nhr]` | AICPU | `InsReduceScatterSequenceMesh1DNhr` | 唯一候选 |
| `sequence[mesh,mesh]` | DPU | `InsReduceScatterSequenceMeshMeshDPU` | 唯一候选（仅HOST CPU引擎生效） |
| `sole[mesh]` | AICPU | `InsReduceScatterMesh1DZAxisDetour` / `InsReduceScatterMesh1D` | `COND_Z_AXIS`命中选ZAxisDetour变体，否则裸名兜底 |
| `sole[mesh]` | CCU | `CcuSchedReduceScatterSoleMeshPeerOnly` / `CcuSchedReduceScatterSoleMesh` | `COND_PEER_ONLY`命中选PeerOnly变体，否则裸名兜底 |
| `sole[mesh.chunk]` | AICPU | `InsReduceScatterMesh1DMeshChunk` | 唯一候选 |
| `sole[nhr]` | AICPU | `InsReduceScatterAicpuReduceNHR` / `InsReduceScatterNHR` | `COND_SPECIAL_DT`命中选AicpuReduce变体，否则裸名兜底 |
| `sole[nhr.multi_channel]` | CCU | `CcuSchedReduceScatterSoleNHRMultiLink` | 唯一候选 |

**表10**  AllReduce（6组合）

| 外部名 | 引擎 | 解析结果（注册名） | 消歧说明 |
| --- | --- | --- | --- |
| `parallel[mesh,nhr]` | AICPU | `AicpuAllReduceParallelMeshNHR` / `AicpuAllreduceParallelMeshNHRPcie` | 机型二选一：`FLAG_MULTI_LEVEL` / `FLAG_PCIE_MIX`（无UBX变体），均不命中则回退默认选路 |
| `sole[mesh.one_shot]` | AICPU | `AicpuAllReduceSoleMeshOneShot` | 唯一候选 |
| `sole[mesh.two_shot]` | AICPU | `AicpuAllReduceSoleMeshTwoShot` | 唯一候选 |
| `sole[mesh.two_shot.chunk]` | AICPU | `AicpuAllReduceSoleMeshChunkTwoShot` | 唯一候选 |
| `sole[mesh]` | CCU | `CcuSchedAllReduceSoleMesh` | 唯一候选 |
| `sole[nhr]` | AICPU | `AicpuAllReduceSoleNHRAicpuReduce` / `AicpuAllReduceSoleNHR` | `COND_SPECIAL_DT`命中选AicpuReduce变体，否则裸名兜底 |

**表11**  AllToAll（6组合）

| 外部名 | 引擎 | 解析结果（注册名） | 消歧说明 |
| --- | --- | --- | --- |
| `concur[mesh,mesh]` | AICPU | `AicpuAllToAllSoleMeshConcurrent` | 唯一候选 |
| `concur[mesh,mesh]` | CCU | `CcuSchedAllToAllSoleMeshConcurrent` | 唯一候选 |
| `sole[mesh.single_channel]` | AICPU | `AicpuAllToAllSoleMeshSingleChannel` | 唯一候选 |
| `sole[mesh]` | AICPU | `AicpuAllToAllSoleMesh` / `AicpuAllToAllSoleMeshUBX` | 机型二选一：`FLAG_FLAT_1D`（第0层非1D CLOS）/ `FLAG_UBX`，均不命中则回退默认选路 |
| `sole[mesh.multi_channel]` | CCU | `CcuSchedAllToAllMesh1DMultiJetty` | 唯一候选 |
| `sole[mesh]` | CCU | `CcuSchedAllToAllSoleMesh` | 唯一候选 |

**表12**  AllToAllV（3组合）

| 外部名 | 引擎 | 解析结果（注册名） | 消歧说明 |
| --- | --- | --- | --- |
| `concur[mesh,mesh]` | AICPU | `AicpuAllToAllVSoleMeshConcurrent` | 唯一候选 |
| `sole[mesh]` | AICPU | `AicpuAllToAllVSoleMesh` | 唯一候选 |
| `sole[mesh]` | CCU | `CcuSchedAllToAllVSoleMesh` | 唯一候选 |

以上五表合计36个组合（10+11+6+6+3），其中8个为多候选组合——4个变体对（`COND_Z_AXIS`、`COND_PEER_ONLY`、`COND_SPECIAL_DT`×2，均带裸名兜底）与4个机型组（`FLAG_*`互斥选择，均不命中则回退默认选路），其余28个组合为唯一候选、纯查表定名。

## 4. 失败与回退

### 4.1 回退可见语义（与裸名强制算法一致）

外部算法名通道的失败处理与裸名强制算法**同链同终态**：AICPU链路上任何失败（解析、无候选、规则全灭、资源不足）都会打WARNING日志（含外部名原文与原因）并**回退到自动选路**，作业继续；CCU链路的准入预检保持严格——无CCU候选、或候选未全部通过注册校验时直接报算法不支持。所有回退路径均有`[MC2_EXT_ALG]`前缀的日志，不存在静默降级：若配置`parallel[mesh, nhr]`后回退、换了算法，可在日志中定位到对应WARNING。

### 4.2 六类失败场景

**表13**  外部算法名失败场景

| 失败场景 | 触发条件 | AICPU链 | CCU链 |
| --- | --- | --- | --- |
| 语法解析失败 | 词表外单词、格式错误、层数超限 | WARNING并回退默认选路 | 预检拒绝，报算法不支持 |
| 候选为空 | 外部名在当前版本无对应注册 | WARNING并回退默认选路 | 预检拒绝，报算法不支持 |
| 规则全灭 | 候选存在，但②②′均不命中 | WARNING并回退默认选路 | 分配时WARNING并回退默认选路 |
| 资源预检失败 | 引擎准备后资源预算不通过 | WARNING并回退默认选路 | 探测链回退默认选路；分配链不做预检 |
| 资源申请失败 | 算法已定名，资源申请失败 | WARNING并回退默认选路 | 报错 |
| 引擎准备失败 | 定名后引擎准备失败 | 报错 | 报错 |

### 4.3 日志格式与示例

相关日志均以`[MC2_EXT_ALG]`为前缀（回退路径为WARNING级；消歧器内部的ERROR级明细日志仍会打印，用于定位）。失败日志包含**外部名原文**（algConfig）与**失败原因**（哪一级、什么约束）。

注意：日志中的op/opType为算子类型枚举（HcclCMDType）的数值，常用取值：2=ALLREDUCE、6=ALLGATHER、7=REDUCE_SCATTER、8=ALLTOALLV、10=ALLTOALL。

选路审计日志（INFO级）示例——AllGather配置`parallel[mesh, nhr]`、UBX机型：

```text
[MC2_EXT_ALG] op[6] ext[parallel[mesh,nhr]] engine[AICPU] candidates(3): [InsAllGatherParallelMesh1DNHR(FLAG_MULTI_LEVEL), InsAllGatherParallelMesh1DNHRPcie(FLAG_PCIE_MIX), InsAllGatherParallelMesh1DNHRUBX(FLAG_UBX)]
[MC2_EXT_ALG] resolved: InsAllGatherParallelMesh1DNHRUBX (engine=AICPU)
[MC2_EXT_ALG] accepted, opType[6], algConfig[parallel[mesh, nhr]] -> algName[InsAllGatherParallelMesh1DNHRUBX].
```

其中`ext`为规范化外部名（已去除空格），`algConfig`保留配置原文。

AICPU链回退（WARNING级，`%s`依次为外部名原文与原因）：

```text
[MC2_EXT_ALG] fallback to default selector, algConfig[%s], opType[%u], reason[%s].
```

解析失败时`reason`为语法错误明细；规则全灭时消歧器会先打一条含完整候选列表的ERROR级明细，随后打本条WARNING。

消歧器内部明细（ERROR级，先于回退WARNING打印）——候选为空，外部名无对应注册：

```text
[MC2_EXT_ALG] ERROR: resolve failed, algConfig[pipeline[mesh,nhr,nhr]], opType[6], reason[resolve failed, ext="pipeline[mesh,nhr,nhr]" op=6, candidates=[] (no candidate registered in this build for engine AICPU)].
```

规则全灭——AllToAll配置`sole[mesh]`，PCIE混联组网下两个机型flag均不命中：

```text
[MC2_EXT_ALG] ERROR: resolve failed, algConfig[sole[mesh]], opType[10], reason[resolve failed, ext="sole[mesh]" op=10, candidates=[AicpuAllToAllSoleMesh(FLAG_FLAT_1D), AicpuAllToAllSoleMeshUBX(FLAG_UBX)] (all candidates rejected by rules for engine AICPU)].
```

资源回退（AICPU链，WARNING级）——外部名的algConfig与定名对应关系见定名时的`accepted`日志：

```text
[MC2_FORCE_ALG] fallback to default selector after resource failure, algName[%s], ret[%d].
```

CCU链预检拒绝（WARNING级，随后整体报算法不支持）——三条格式串依次为：解析失败场景（`%s`为解析错误原因）、某个CCU候选未过注册名单场景（全量校验：任一候选未过即拒，`%s`为该候选注册名）、无CCU候选场景：

```text
[MC2_EXT_ALG] ccu gate rejected, algConfig[%s], opType[%u], reason[%s].
[MC2_EXT_ALG] ccu gate rejected, algConfig[%s], opType[%u], ccuCandidate[%s] not registered (all CCU candidates must pass algorithmMap/V2 registry, funnel may select any of them).
[MC2_EXT_ALG] ccu gate rejected, algConfig[%s], opType[%u], ccuCandidates[0] (external name has no CCU candidate).
```

## 5. 版本兼容矩阵

**表14**  algConfig版本兼容矩阵

| algConfig内容 | 当前版本行为 | 旧版本行为 |
| --- | --- | --- |
| 空字符串 | 自动选路 | 自动选路（一致） |
| 老语法（含`=`） | AICPU链走默认选路；CCU链报算法不支持（历来如此，与外部名行为相同） | 一致 |
| 裸注册名 | 裸名强制算法（保持原有资源预检与回退语义） | 一致 |
| 外部名（含`[`） | 解析并消歧为唯一注册名，失败按第4章处理（AICPU链回退默认选路，CCU链预检拒绝） | 整串按裸名处理：AICPU链因该名字未注册而回退自动选路；CCU链报算法不支持 |

说明：

-   外部名语法是纯增量通道：裸名、老语法、自动选路三条既有路径的行为与旧版本完全一致。
-   旧版本收到新语法时降级路径明确（按裸名处理），不会产生歧义行为；跨版本执行同一配置前，请先确认两侧版本对`algConfig`的支持范围。

## 6. 约束与限制

-   **覆盖范围**：当前版本收录46条集合通信算法注册，归并为30个「算子×外部名」组合、36个「算子×外部名×引擎」组合，完整清单见3.4节。配置映射表之外的外部名会因候选为空按第4章处理（AICPU链回退默认选路，CCU链预检拒绝）。
-   **CCU链前置条件**：外部名走CCU引擎时，要求通信引擎为CCU系取值（commEngine为5或6，对应CCU_MS/CCU_SCHED），tiling阶段即设置CCU新版本标记（version=101）；该外部名在CCU引擎下是否真有候选算法，由资源分配阶段的CCU链入口校验（全部候选须过注册名单，任一未过或无候选时明确报错并给出原因）。AICPU引擎的外部名不设置该标记（该标记仅在CCU链消费）。
-   **长度限制**：`algConfig`最大长度128字节，超长在tiling阶段报错。
-   **保序标记**：`[order.strong]`当前仅被校验与记录，不影响算法选定结果。
-   **DPU行生效条件**：映射表中引擎为DPU的行，仅在通信引擎为HOST CPU时生效。
