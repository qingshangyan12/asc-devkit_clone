# R15 · M0 考古——TierParams 方案甲 + NHR AICPU 执行面 + 资源申请翻倍（三件同轮）

> 2026-10-10 落盘。任务书：R15_TASK.md（本轮唯一权威规格）。行号 = 本仓实数
> （`impl/adv_api/detail/hccl/` 前缀省略）。基线 1659（run.sh 519 / st_checker 891 /
> exec 249），已实测复核。**§十 5 条裁决项已由主会话复核裁定（2026-10-11：裁决 1-4 全部采纳建议、修正 A 引擎维度 gate / 修正 B 手算二动词序口径——两处修正已并入正文），M0 终稿放行进实施。**

---

## 一、必答 A：①写入方落点考古（blob 序列化 + 段 0 帧槽布局 + 槽 16 写入零影响再论证）

| 项 | 证据（文件+行号） | 结论 |
|----|------------------|------|
| 序列化产 blob 的代码 | `cc/src/ops/unified/inc/plan_serializer.h` 的 `SerializePlanV3`（:275-318）逐段调 `SerializeSegmentV3`（:145-267）；**两引擎共用**——生产调用点唯一：`cc/src/common/hccl_alloc_ctx_res.cc:1164`（v5 分叉 false 半边），AICPU 引擎同走此点（:1146 `verbPath = engine==CCU && UseUnifiedVerbPath` → AICPU 恒 false → :1164） | 写入方 = **plan_serializer.h**（任务书"CCU 侧 blob 序列化"的实体；AICPU 路径 D2H 直取其产物，不经 CCU 写入方改写——`unified/exec/unified_aicpu_exec.cc:87-98` 从 resCtx.unifiedBlobAddr 直读） |
| 段 0 帧槽布局定义 | `plan_serializer.h:67-83`（`UnifiedV3Slot`：槽 0=动作 ID，槽 1..14 CollectAll）；`:88-104`（`UnifiedV3RsSlot`：槽 1..15 ReduceSlice）；镜像 = `common/kfc_server_message.h:490-543`（`KFC_UNIFIED_COLLECT_ALL_*` / `KFC_UNIFIED_REDUCE_SLICE_*`，static_assert 互锁 :554-600） | 段帧 32 槽，**协议面只登记槽 0..15**；槽 16..31 无任何现存写入者 |
| 写槽 0..15 的循环 | `SerializeSegmentV3:151-153` 先清零全 32 槽（`for (i < 32) slots[i]=0`），switch 按 kind 写槽 0（动作 ID，:154）+ 槽 1..14（AG，:165-197）或槽 1..15（RS，:231-253） | **非 tier Plan 槽 16..31 恒 0 是构造性事实**（清零后无人再写）——槽 16==1 判别式的地基 |
| CCU 写入方只读 0..15 | ① device 写入方 `ccu/hccl_ccu_v0_prepare.h:933+ CcuPrepareForUnifiedPlan`：:1031-1035 逐帧拷全 32 槽（透传），覆写名单只动槽 1..15（RS 分支 :1046-1092）或槽 1..14（AG 分支 :1100-1124，透传槽 0/9/10/11/12）；② CCU server `kfc_server/template/ccu/kernel/ccu_kernel_kfc_server.cc:205-268 DispatchUnifiedPlanV3` 只消费 `param[KFC_UNIFIED_*]`（槽 0..15）；③ LoadFuncParamFromMemory（:178-190）Load 宽 32 但分派只读 0..15 | **槽 16..24 写入零影响**：写入方透传落 HBM（flush 4 行已覆盖槽 16..31，:1142-1146 注释"槽 16..31（P2 的 RS NHR 24 槽/并发 RS 25 槽在此区间）不再漏落 HBM"——物理面已预留），server 不解读。✅ 设计依据成立 |
| ★新发现：NHR 骨架段序列化拒收（①的隐藏断点） | `plan_serializer.h:220-224`：`SerializeSegmentV3` 对 `seg.pattern == NHR` 的 ReduceSlice 段**显式拒收**（err 上抛 → SerializePlanV3 失败 → unified 回退老路径）。tier 骨架 = parallel[mesh,nhr] 的 seg1 是 NHR RS 段 → **现状下 tier Plan 根本产不出 v4 blob**——①的数据可达前提被此拒收切断 | ①必须附带放行：NHR 拒收改条件化（gate = UseUnifiedTierPath && 规范形态 && **engine==AICPU**，见 §十-5——主会话复核修正 A：SerializeSegmentV3 两引擎共用且无引擎参，gate 只判 UseUnifiedTierPath 会在 CCU 的 verbPath=false 降级路径（v5 失败/未开→v3）上放行 NHR 段 → DispatchUnifiedPlanV3 只读槽 0..15 会把 NHR 段当普通 RS 段跑=静默错跑面。**仅 AICPU 引擎消费的 blob 放行**：实现形态 = 序列化器加布尔形参 allowNhrSkeleton，唯一生产调用点 hccl_alloc_ctx_res.cc:1164（此处 verbPath 判定已有 engine 上下文）传 `engine==AICPU && UseUnifiedTierPath(...)`，既有测试调用点缺省 false=现状拒收零变化）。拒收的原始理由（v3 帧无 pattern 载体、CCU server 会错读）对 AICPU 消费路径不成立——AICPU 消费方按帧 kind 分读槽 1..4（值是 layout 驱动、pattern 无关），pattern 由 tier 位置语义重建（必答 B）；对 CCU 降级路径**成立**，维持拒收 |

---

## 二、必答 B：①读取方落点考古（RebuildActionSpecFromFrame 结构 + TierParams 数据流）

| 项 | 证据（行号） | 结论 |
|----|--------------|------|
| 读槽现有结构 | `cc/src/ops/unified_exec/aicpu_consumer.h:112-191 ParseUnifiedBlobV4`：三重校验（magic/版本/槽数）→ 逐帧 `BlobFrameView`（:81-97）按 actionId 分读——RS 帧 :149-157 读槽 0..4（in/out/scratch/rankSliceOffset），AG 帧 :158-167 读槽 0..14 子集 | 帧解读层是 tier 槽读取的自然挂点：`BlobFrameView` 追加 tier 字段（读帧 0 槽 16..24），`ParseUnifiedBlobV4` 帧循环内一行分读 |
| RebuildActionSpecFromFrame | `aicpu_consumer.h:351-388`：Instantiate（:359-369）→ ActionSpec 组装（layout/domain/sync T/T）。**勘误**：ActionSpec 无 tier 字段（tier 在 Plan 级，`action_spec.h:284-306`）——任务书"塞进重建的 ActionSpec"实义 = 消费方局部 TierParams 变量经 `ExpandTieredSegments` 形参（`tier_expand.h:291`）进入展开 | tier 数据流：`SerializePlanV3` 写段 0 槽 16..24 → blob（ctx 旁路 H2D）→ `unified_aicpu_exec.cc:87-98` D2H → `ParseUnifiedBlobV4` 读帧 0 槽 16..24 → TierParams 重建 → **tier 分叉**：重建骨架 2 段（位置语义：seg0=快轴 mesh、seg1=慢轴 nhr——`tier_expand.h:304-307` segFast/segSlow 同位序；pattern 按位置赋 MESH/NHR，规范形态 gate 保证成立）→ `ExpandTieredSegments(骨架, tier, totalBytes)` → 展开段序列（sync 位/pattern 齐全）→ R14 编排器（env 开）或串行逐段驱动 → `DriveSegmentViaSink`（:392-433，BuildArgs+Emit 单源件） |
| 展开点数据流 | `ReduceSlice::BuildArgs` NHR 分支（`l3/action_reduce_slice.h:164-186`）已受理 NHR（R13 交付：九标量从 layout 直取）；AicpuReduceSlice::Emit 的 NHR 分支是②的落点（现状缺失，`l4/backend_aicpu_reduce_slice.h:61-131` 仅 mesh 写侧） | ①②数据流闭环：tier 槽 → 展开 → NHR 段 → NHR Emit 分支 → sink 动词（必答 D） |
| 域 1 ranks 来源 | 消费方 domainRanks 现状单域（`unified_aicpu_exec.cc:117-125` 取 `resCtx.algHierarchyInfo.infos[0][0]`；组装段 `hccl_alloc_ctx_res.cc:1266-1280` 只填 {level0}） | tier 骨架 seg1 的域成员从 `infos[0][1]` 取（组装段按 topo physicalLevels 二层**追加**填充——加法不改法；host 测试手构 resCtx 直填两域）。真机 level1 拓扑提取考证挂 P2（§十-4） |

---

## 三、必答 C：槽 16 判别式完备性（非 tier Plan 槽 16 恒 0 反例考古）

**穷举 v4 blob 段帧槽 16..31 的全部现存写入路径：**

1. `SerializeSegmentV3`（plan_serializer.h:151-153）：清零 32 槽后只写 0..15——**唯一写入者**，非 tier 恒 0。
2. device 写入方 `CcuPrepareForUnifiedPlan`（hccl_ccu_v0_prepare.h:1031-1035）：从 blob **透传拷贝**（不改写槽 16+；覆写名单止于槽 15）；哨兵帧分支 :1148-1154 全清 0（槽 0=INVALID）。透传不改值。
3. v2 既有布局的"槽 16..24 NHR 区间"（kfc_server_message.h:173-181 `KFC_CONCURRENT_RS_NHR_*`，R9 M0-④ 勘误记录）属 **v2 blob**（CcuPrepareForConcurrentReduceScatterM2M 链），与 v4 unified blob 是两条格式——ParseUnifiedBlobV4 校验 magic=UPL3（:124-127），v2 产物进不来。
4. v5 动词 blob（SerializeVerbPlan）：magic=UPL5，ParseUnifiedBlobV4 版本闸（:128-131）拒收。
5. AG 帧 U_V3_INPUT_OUTPUT_EQUAL=槽 14、RS 帧尾槽 15——均不越 15。

**结论：v4 blob 段帧槽 16 的非零值只可能来自 R15 新增的 tier 写入——判别式 `槽16==1` 完备，无反例。** 防线：①tier 写入 gate 在规范形态（kind 全 RS + 2 段 + seg0 MESH + seg1 NHR，见 §十-5），非规范 tier 形态（如 parallel[nhr,mesh] 角色倒置）不写槽 → 消费方 slot16=0 → 不展开（骨架 NHR 段照旧被序列化拒收 → 回退老路径，无静默错跑面）；②槽 16 写 1（布尔语义，非计数值）——未来多档位扩槽不破坏判别。

---

## 四、必答 D：②sink 动词方案（本轮最重设计点）

**复刻对象形态再确认**（`reduce_scatter/template/aicpu/ins_temp_reduce_scatter_nhr.cc`，442 行）：
- GetThreadNum = 1×channelsPerRank（:72）→ **devkit 恒 1 → 无 Pre/PostSyncInterThreads**（KernelRun :121/:143/:157 的 `threadNum_>1` 门槛不过——单线程域无从线程可同步，sink DoSyncPre 对 threads<2 显式拒绝（hcomm_verb_sink.h:360-363）亦为证）；
- KernelRun :133-156：LocalDataCopy×chan（上料 input→scratch）→ RunNHR×chan（步进流）→ PostLocalCopy×chan（归位 scratch→output）；
- RunNHR :247-361：逐 step host 期批量攒 tx/rx slices → **一步一原语组** SendRecvBatchWriteReduce（写侧 :353-357）/ SendRecvReadReduce（读侧 :348-351，isDmaRead_=IsPcieProtocol :87-88）；通道查表 `channels_[recvFromRank].at(channelIdx)`（:271-279，建链集成员）；
- 原语组内序（wrapper `op_common/template/wrapper/alg_data_trans_wrapper.cc`）：写侧 SendRecvWriteReduce :406-435 = Record(**recvCh**,ACK) → Wait(**sendCh**,ACK) → 逐 slice HcommWriteReduceOnThread(sendCh) → Record(**sendCh**,DATA) → Wait(**recvCh**,DATA)；读侧 SendRecvReadReduce :642-687 = Record(**sendCh**,ACK) → Wait(**recvCh**,ACK) → 逐 slice HcommReadReduceOnThread(**recvCh**) → Record(**recvCh**,DATA) → Wait(**sendCh**,DATA)。批量本质 = **每步一次握手**（非逐 slice）——DoSendRecvBatchTx :116-140 的批量快道（HcommHcommBatchTransferOnThread）不可用时 fallback 逐 slice（语义同、握手仍每步一次）。

**定稿方案（方案甲'：复用 NHR_RUN（动词 ID 8）+ VerbCall 内嵌标量载荷 + sink 公式重算步几何）：**

| 设计点 | 定稿 | 论证 |
|--------|------|------|
| 动词选型 | **复用 `VerbCall::NHR_RUN`（=8，R13 已登记、CCU v5 同名同值——verb_serializer.h:122 static_assert 不动）**，AICPU 半边接线（sink Dispatch 现状 default 拒绝 :157-163 → 新增 case） | 语义同一（"NHR ReduceScatter 步进流整段一动词"）；ID 空间不扩（UNIFIED_V5_VERB_ID_MAX=8 不变）；AICPU 不进 v5 序列化（verbPath 恒 CCU）→ 两引擎载荷互不相见，无互锁面 |
| 批量 slice 列表如何过 verb | **单 verb 整段承载 + 标量载荷**：`VerbCall` 追加 `NhrRunScalars`（valid 标记 + 11 标量：me/n/sliceSize/inBaseOff(绝对)/inputSliceStride/scratchBaseOff(绝对)/repeatNum/inputRepeatStride/outputRepeatStride/channelsPerRank）——AicpuReduceSlice::Emit NHR 分支从 ActionSpec+args 填充；**slice 列表不打包**（sink 按公式重算） | ①CCU v5 NHR_RUN 帧同构（帧 10 槽 ↔ 载荷标量，两侧"整段一步进流"同一形态）；②步几何是纯函数（GetStepInfoList :372-420 全由 me/n 决定）——M0_R13 §三"容器是纯函数投影，序列化即冗余"同裁决；③VerbCall 是 Emit→sink 唯一桥梁，载荷随动词走 → trace/calls_/编排器/等价断言全可见（指针方案会悬垂——Emit 栈帧即亡；值内嵌是值对象纪律）。代价：VerbCall 增 ~72B（PoC 可受） |
| 步公式单源 | 新增 `unified/inc/nhr_steps.h`（纯函数，零引擎依赖）：`CalcNhrSteps(me, n)` → 逐 step {sendTo/recvFrom/nSlices/txIdx[]/rxIdx[]}（GetStepInfoList :386-417 逐式：sendTo=(me+n−2^s)%n、recvFrom=(me+2^s)%n、txIdx 起 sendTo 步进 −2^(s+1)、rxIdx 起 me 同步进、nSlices=(n−1+2^s)/2^(s+1)、nSteps=floor(log2 n)——template_utils.cc:26-32 同式） | 消费方三处（Emit 步0 跳过集 / sink 步循环 / ST golden）共用一份；CCU server VerbNhrRun 的既有实现不动（"既有分支一行不动"——漂移由 ST 等价断言传递锁：AICPU 动词展开 vs kernel 基线、CCU 帧展开 vs 同一基线（R13-③ 既有）⇒ 传递相等） |
| DMA 读侧区分 | **sink 自判**：扫描 `res_.channels` 任意 `[0].protocol == COMM_PROTOCOL_PCIE` → isDmaRead（IsPcieProtocol 同式，alg_v2_template_base.cc:89-99——逐 peer 首通道协议判定）；载荷不携（通道属性非段属性，编排器域子束天然给出正确域范围） | skipStep0TxPreCopy = !isDmaRead && n>1（:94-97 同式）——同样 sink 侧判（step0 tx 源从 input 直取 :322-325） |
| Emit 侧步 0 跳过集 | Emit 需在**发上料 COPY 前**知道跳过集（LocalDataCopy :174-177 跳 step0TxSliceIdxs_），但 isDmaRead 是 sink 侧知识 → ** Emit 恒发全量 n 条上料 COPY（语义等价：被跳槽位的拷贝只是写了不被读的 scratch 槽——Σ nSlices=N−1 保证每槽位恰好被 tx 一次、被跳槽位的数据经 step0 直取 input，拷贝无害）；sink 侧实现 step0-skip（step0 tx 源=input，template 逐字对齐）** | 拆解知识不对称：Emit 无通道协议、sink 无步 0 前几何决策点。★代价修正（主会话复核修正 B）：多余拷贝数 = **nSlices(step0) = (N−1+1)/2 = N/2**（N=8 → 4 条——即 step0Tx 槽位集 {2,0,6,4} 的拷贝），非 (N−1)/2；数据等价，原语序与老链差异由 ST 归一化断言承载（"copy 后未读"槽位的多余拷贝可判）。PCIe 读侧无 skip → 全量 n 条本就是老链形态 ✓ |
| 与既有七动词合取语义 | NHR 段动词序 = **{COPY×(n−\|skip\|), NHR_RUN×1, COPY×1}**（channelsPerRank=1）；**不发 SYNC_PEER_PRE/POST**（threadNum=1 门槛——若 channelsPerRank>1（协议面保留）则按 sync 位发射）；NHR_RUN 不动 READ/WRITE 计数器（sink ThreadForRead/ThreadForWrite 只数 READ/WRITE，hcomm_verb_sink.h:422-436/:453-467——NHR 段零 READ/WRITE 动词，混排段计数不受扰） | ①步间全序由段内单动词承载（sink 内步序即下发序），无既有动词的合取参与；②跨 rank 数据序由每步 ACK/DATA 五段握手承载（通道级 HcommChannelNotify*），与 R14 编排器的**线程级** HcommThreadNotify* 是两个原语族（hcomm_primitives.h:116/:126 vs :334/:354）——槽空间不相交，批尾交叉对无冲突；③NHR 域单线程时编排器域内补 Post 需免（见必答 F-3 修正） |
| 线程选择 | sink 内 `threads[channelIdx]`（channelIdx 循环 channelsPerRank——RunNHR :254 同式；devkit=1 → threads[0]=域 main） | 不经 ThreadForRead/Write 计数（NHR 自持通道序——M0_R13 §二"NHR 数据帧序≠通道序"同源结论） |
| 归位 COPY 的 doPreCopy 分支 | 统一路径 input（用户 buffer）≠scratch（cclBuff）→ doPreCopy 恒 true → scOff = scratchBase+sliceSize*me（PostLocalCopy :231-235 的 doPreCopy 分支）；SYMMEM+NHR 组合不存在于 devkit 链（transport=SYMMEM 的 NHR 段 Emit 显式拒收，登记负例） | 老 doPreCopy_=false 分支（input 即 scratch）仅 symmem 形态可达，裁 |

---

## 五、必答 E：②NHR 段判别与建链集（L4 建链需求现状 + mesh/NHR 建链集冲突分析）

| 项 | 证据（行号） | 结论 |
|----|--------------|------|
| L4/L3 建链需求收集现状 | 通道申请**不在 L4 ClaimEngineRes**（AICPU 后端 :50-57 只报线程/notify；CCU 后端 `l4/backend_ccu_reduce_slice.h:50-99` 的 NHR 分支**已按建链集**申报 channels——R13 交付）；通道清单的唯一生产点 = L3 `ReduceSlice::CalcRes`（`l3/action_reduce_slice.h:107-155`）：NHR 分支 :114-141 connectRanks{me±2^s} 升序去重（CalcNHRChannelConnect 同式，`op_common/executor/channel/channel_request.cc:56-73`），mesh 分支 :142-147 全连接跳 me | R13 已按建链集申报（四端单源：L3/L4-CCU/注册链 BuildUnifiedChannelDescs（op_common.cc:3017-3043 "CcuVerb" 前缀分叉）/server chIdx 重建）。**AICPU 半边复用同一 claim.channels**（消费方 res.channels ← Materialize→broker.AcquireChannels） |
| "5 通道对 7 peer ChannelOf Fail"的病灶定位 | 现 Emit 仅 mesh 写侧分支：`backend_aicpu_reduce_slice.h:79-98` 逐 peerAlgRank **全连接迭代**（0..n 跳 me → N−1 个 peer 全要通道）；NHR 段建链集 = {±2^s}（N=8 me=3 → {1,2,4,5,7} 5 条 ≠ 7）→ 全连接迭代在建链集外 2 peer 上 ChannelOf Fail（hcomm_verb_sink.h:168-176） | Fail 根因 = **NHR 段进了 mesh 分支的全连接迭代**，不是建链不足。**NHR 执行面真实需求 = 建链集**：步公式 sendTo/recvFrom 的值域恰为 {me±2^s}（N=8 me=3：sendTo∈{2,1,7}、recvFrom∈{4,5,7}，并集=建链集 5 条）——5 条全命中，ChannelOf 零 Fail。**勘误建议（§十-1）**：任务书"NHR 需要 7 peer（N−1）全量建链（不裁）"系 mesh 分支迭代口径的误植——照抄会改 R13 已交付的建链集语义（claim.channels 从 5 条变 7 条，回归风险），且 7 条中 2 条永不被步公式触达 |
| mesh 段与 NHR 段建链集冲突 | 同 Plan 混排（tier 骨架 seg0 mesh 域0 + seg1 NHR 域1）：两段域成员不相交（域0=level0 mesh ranks、域1=level1 Clos ranks——RecipeParallel::Expand 逐域 BuildStage，recipe_registry.h:57-70）→ peer 集不相交 → claim.channels 追加合并无冲突（operator+= append，action_spec.h:217）；sink ChannelOf 按 peer 查表天然分域 | 无冲突。混排断言按段各自口径（M0_R13 §二同款：NHR 段通道数=建链集大小 ≠ mesh 段 N−1） |
| ★生产链差距（AICPU 半边通道协议） | AICPU 路径通道申请 `HcclChannelAcquireUnified`（op_common.cc:2876-2933）**恒走 CalcChannelRequestMesh1D（HCCS 协议探测）**——域1 的跨 server peer（PCIe/RoCE 链）HCCS 探测必失败 → 生产链 tier Plan 在通道申请步回退老路径 | host 测试直构 HcommVerbRes（不走探测）不受影响；**生产 NHR 协议探测接入（AICPU 半边按段 pattern 分叉探测函数——对齐 R13 注册链 BuildUnifiedChannelDescs 的前缀分叉）登记 P2**（§十-4 一并列出真机接入三差距） |

---

## 六、必答 F：②执行面与编排器接口（NHR 段进编排器的资源语义）

| 项 | 现状（行号） | R15 修正定稿 |
|----|--------------|--------------|
| 段粒度 | `l5/segment_orchestrator.h:158-431 OrchestrateSegments`：域分组（ranks 相等去重首现序 :174-190）→ 线程预算（DomainThreadNeed :126-140）→ 批划分（post=T 收批 :308-319）→ 逐段 driver（:328-339）→ 域内补 Post（:343-370）→ 批尾交叉对（:372-427） | NHR 段进编排器 = **一个 ActionSpec 一个段**（不按通道/步拆段）——driver 逐段驱动，段内步进流由 NHR_RUN 单动词承载（sink 内 channelIdx×step 循环） |
| 每通道一 StepInfo 批、多通道分段？ | 模板形态：`for (channelIdx < channelsPerRank) RunNHR(threads, channelIdx)`（ins_temp_reduce_scatter_nhr.cc:151-153）——每通道独立步进流批 | **不拆段**：NhrRunScalars.channelsPerRank 携通道数，sink 内 channelIdx 循环（每通道一 StepInfo 批——CalcNhrSteps 每通道重放，通道间天然串行下发序）。devkit 恒 1 单批；>1 是协议面保留（port-group 切分 CalcDataSplitByPortGroup 裁——任务书 §二之二已裁，P2 复核） |
| 域线程预算 pattern 盲 | DomainThreadNeed :126-140 对 ReduceSlice **恒取 n**（mesh 公式）——NHR 段（threadNum=1×channelsPerRank=1）被错算成 n | **pattern-aware 化**：新纯函数 `DomainThreadTotalNeed(seg)`（inc/ 层，见必答 H）——RS·NHR → max(1, channelsPerRank)、RS·mesh → max(1, n)、AG → max(1, n−1)（**AG 不按 pattern 分叉**——AG NHR AICPU 执行面未交付，保持 ClaimEngineRes 既有口径防 sole[nhr] AG 申报漂移）；编排器 DomainThreadNeed 改为委托该函数（单源）。行为影响面：仅 NHR 段（R15 前不可达——序列化拒收）→ 既有可连性状零变化 ✓ |
| 域内补 Post 单线程失败 | `domainPostBarrier` lambda :287-306：`threads.size() < 2 → return -1` → 编排器 HCCL_E_INTERNAL——NHR 域（need=1）的批内 post=F 段（A 形态批 2 的 seg2）必触发 | **<2 线程改 no-op（return 0）**——对齐模板 threadNum>1 门槛（单线程域无域内合取对象；批窗关闭语义由批尾跨域交叉对承载：域 mains notify 槽按线程独立，1 线程 main 参与交叉对无障碍）。行为影响面：仅单线程域（R15 前不可达——mesh 域 n>1 恒 ≥2）✓ |
| NHR 域 mains 承载 | 域子束切池 :273-283（首现序 first-fit）；NHR 域 need=1 → 子束=[threads[base_1]]，域 main=该线程；段内 COPY/NHR_RUN 全落 threads[0]（域 main） | 与 R14 模型一致（域 mains 承载数据动词）；NHR 段零 READ/WRITE 动词 → sink 计数器不扰同批 mesh 段 |

---

## 七、必答 G：③Σ 申报落点考古（tier_expand 产出点 / :213 max 触发路径 / 门控判定点）

| 项 | 证据（行号） | 结论 |
|----|--------------|------|
| tier_expand 产出段列表的位置 | `unified/inc/tier_expand.h:291-374 ExpandTieredSegments`（A 分支 :309-334 / B 分支 :337-373）——**exec 期消费方展开，不产资源申报**（资源期 count=0 展不开，R6-③ 裁决；骨架才是资源申报输入） | Σ 申报落点**不在 tier_expand**——在骨架的 claim 合并处（下行） |
| :213 max 语义触发路径 | `action_spec.h:203-220 operator+=`（:213 `slaveThreadNum = max`）唯一触发链 = `recipe_sole.h:224-228 SumRes` → `:203-221 CalcResOf` 逐段 `claim += CollectAll/ReduceSlice::CalcRes(seg)`；SumRes 调用方 = 全部四配方（recipe_registry.h:70/:151/:182 + recipe_sole.h:341）——**每 Plan 内段间合并**（无跨 Plan 聚合调用点：grep 全仓 operator+= 仅此两行） | max 是"单 Plan 段间合并"语义（串行复用假设：段一个个跑取峰值）。ResourceClaim 无跨 Plan aggregate 场景（任务书"合并 vs 单 Plan"之问的答案：只有单 Plan 段间合并） |
| Σ 与 max 切换判定点 | 门控 = `UseUnifiedConcurrencyPath()`（unified_route.h:147-161，宏 MC2_UNIFIED_CONCURRENCY_RESOURCE + env MC2_USE_UNIFIED_CONCURRENCY 三层，R14 交付）——**资源期 host 进程可判**（GetUnifiedOpParamResCtxImpl 与 exec 期 AICPU TS 是两进程：资源期判 Σ 申请；exec 期同 env 判编排。失配安全：资源 Σ/exec 串行 = 多余线程闲置无害；资源 max/exec 编排 = 编排器预检降级串行（R14 既有路径）——两向失配均安全） | 切换点 = **SumRes 尾部**（gated 覆写 slaveThreadNum；ungated 一行不动）。Σ 公式（任务书口径，老 CalcRes :76-77 逐字推广）：**claim = Σ_d (DomainThreadNeed_d − 1) + D**（D=域数=TEMPLATE_MAIN_THREAD_NUM 的推广；16P tier：3+0+2=5 slaves/6 total，编排器实需 5 total，1 条冗余=老控制线程位——结构性差异对照点） |
| Materialize/HcclGetThread 消费链 | `l2/l1_adapter.h:215-235 Materialize`：AICPU → `AcquireThreads(claim.slaveThreadNum+1, …)`（:221-223）→ 真实现 `UnifiedResourceBroker::AcquireThreads`（hccl_alloc_ctx_res.cc:967-981）→ `HcclGetAlgResUnified`（op_common.cc:2846-2873）→ HcclThreadAcquire(threadNum…) | 全链参数化消费 claim 值——Σ 只改 SumRes 产出的数值，下游零改动 |

---

## 八、必答 H：③资源切池与 R14 编排器对齐（域 mains 线程序 + 翻倍后切池影响）

| 项 | 证据（行号） | 对齐说明 |
|----|--------------|---------|
| R14 域分组/切池 | segment_orchestrator.h:174-190（ranks 相等去重首现序）、:273-283（first-fit 切池：域 d 子组 = threads[base_d..base_d+need_d)，base_d=Σ need_{<d}；**域 mains=子组[0]，域 0 main=全局 threads[0]**） | 域 0 main = **threads[0]**（≠老算法 threads[1]）——老算法 threads[0]=专职控制线程（ins_reduce_scatter_parallel_executor.cc:255 `controlThread_ = threads_.at(0)`、:256-257 mains=threads[1]/threads[1+intraNum]）；R14 无控制线程（交叉对省一条），threads[0] 兼任域 0 main。**结构差异登记真机复核**（任务书 §二之三注意项） |
| 翻倍后 threads 数量对切池的影响 | 消费方 res.threads = resCtx.threads（资源期 Σ 申请原批回收，hccl_alloc_ctx_res.cc:1260）→ 编排器 totalNeed 判定（:243 `threads.size() < totalNeed` 降级） | 现状 max 申报下双域恒降级（16P：max=3 slaves/4 total < 需 5 → 降级串行，R14 M0 §二实证）；Σ 申报后 6 total ≥ 5 → 正常切池：域0=threads[0..3]（main=t0）、域1=threads[4]（main=t4）——切池逻辑零改动，只是池从不足变足 |
| 切池公式单源 | DomainThreadNeed（编排器 :126-140）与 Σ 申报的域预算必须同式 | 新 `inc/` 纯函数 `DomainThreadTotalNeed(seg)` + `CalcConcurrencySlaveClaim(segs)`（action_spec.h 值对象居所，action_spec.h 被 l2/l5 双侧 include 无环）；编排器 DomainThreadNeed 改委托（pattern-aware 见必答 F）；SumRes gated 分支调 CalcConcurrencySlaveClaim。**ClaimEngineRes（backend 层引擎申报）一行不动**——既有 max 路径与全部既有断言零扰动（pattern-aware 只活在 gated Σ 与编排器两处，两者 R15 前均不可达 NHR 段） |

---

## 九、必答 I：③翻倍后资源适配（HcclGetThread Σ 适配 + threads 上限考证）

| 项 | 证据（行号） | 结论 |
|----|--------------|------|
| HcclGetThread 适配 | 老路径 `op_common.cc:1097-1150 HcclGetThread`：AICPU_TS 分支 threadNum = resRequest.slaveThreadNum+1（:1105-1107 附近）——**参数化消费，零适配需要**；unified 路径**不经 HcclGetThread**（Materialize→broker→HcclGetAlgResUnified :2846-2873 直调 HcclThreadAcquire，同参数化） | Σ 只改 claim 数值，两路消费端零改动。resCtxHost->threads 回填（:1131-1134 老路径 / hccl_alloc_ctx_res.cc:1260 unified 路径）同为参数化 ✓ |
| notify 申报 | HcclGetThread AICPU_TS：maxNotifyNum = max(notifyNumOnMainThread, max(perThread)) + 1（:1099-1104 同式；UnifiedResourceBroker::AcquireThreads :970-976 同式）——**全线程统一 notifyNum**（非逐线程） | 现状 operator+= 的 notify 合并（main 求和 :214 / perThread 追加 :215-216）在 Σ 路径下**维持不动**：maxNotifyNum+1 ≥ 任一线程需求（域 mains 的域内 Post 需 ≤ 域从线程数、交叉对用槽 0）——超配安全，改动面最小 |
| Σ 值域/上限考证 | ①拓扑界：tier Plan 恒 D=2（parallel 配方 DomainsRequired=2，recipe_registry.h:59）；need_d ≤ max(N_d, …) ≤ N_d+1 → Σ total ≤ N_0+N_1+2 ≈ 全域规模；②**在产对照**：老算法同拓扑在产申请 = intra+inter+3 总线程（parallel executor :76-77 + 控制位 :255）≥ R14 模型 Σ 需求（N0+N1 或更小——NHR 域 need=1）——**Σ 申报不超老算法在产同拓扑已申请量**，无新资源上限风险；③HcclThreadAcquire 单次批量申请（hccl_res.h:136-142 签名无条数上限常量；上限在 comm 级线程池——真机池上限无源码常量，挂 P2 真机复核：16P PCIeMix 档 Σ=6 total 远低于老链 6 total 同值） | D>2（未来多域配方）时 Σ 上限复核挂 P2（DomainPlan §6.11 落地时）；R15 范围内（D≤2）值域安全 |

---

## 十、三组手算（验收锚——期望值独立手算，禁转抄生成方）

### 手算一（①槽位编码）

给定 TierParams{hasTier=true, threshBytes=8MiB, partRatioNum=3, partRatioDen=4, bwFastNum=2, bwFastDen=1, bwSlowNum=1, bwSlowDen=2, alignBytes=32}，写入门控命中（规范形态 2 段 RS [mesh,nhr]）→ 段 0 帧（骨架 seg0 = 域 0 mesh RS 段）槽 16..24：

| 槽 | 字段 | 十进制 | 十六进制 |
|----|------|--------|---------|
| 16 | hasTier | 1 | 0x1 |
| 17 | threshBytes | 8×1024×1024 = 8388608 | 0x800000 |
| 18 | partRatioNum | 3 | 0x3 |
| 19 | partRatioDen | 4 | 0x4 |
| 20 | bwFastNum | 2 | 0x2 |
| 21 | bwFastDen | 1 | 0x1 |
| 22 | bwSlowNum | 1 | 0x1 |
| 23 | bwSlowDen | 2 | 0x2 |
| 24 | alignBytes | 32 | 0x20 |

段 0 帧其余不变（槽 0..15 照 RS mesh 布局）；**段 1 帧（NHR 骨架段）槽 16..24 恒 0**（tier 只写段 0——Plan 级数据挂首段，判别式只查帧 0）。

非 tier 反例：`sole[mesh]` AG Plan（单段 CollectAll）→ 槽 16..24 全 0（SerializeSegmentV3 :151-153 清零后无人写）→ 消费方槽 16==0 → 不走 tier 分叉，行为与现状逐字节相同。第二反例：two_shot AR Plan（RS+AG 两段，非 parallel 配方 tier 不置位——recipe_registry.h:86 `kind==ReduceSlice && …` 且 tier 挂载只在 RecipeParallel）→ 槽 16..24 全 0。

### 手算二（②NHR 动词序，N=8、me=3、写侧非 PCIe、channelsPerRank=1、FP16/SUM）

**步几何**（GetStepInfoList :386-417 公式，nSteps=floor(log2 8)=3）：

| step | sendTo=(me+N−2^s)%N | recvFrom=(me+2^s)%N | nSlices=(N−1+2^s)/2^(s+1) | txIdx（起 sendTo 步进 −2^(s+1)） | rxIdx（起 me 同步进） |
|------|------|------|------|------|------|
| 0 | (3+8−1)%8=**2** | (3+1)%8=**4** | (7+1)/2=**4** | {2, 0, 6, 4} | {3, 1, 7, 5} |
| 1 | (3+8−2)%8=**1** | (3+2)%8=**5** | (7+2)/4=**2** | {1, 5} | {3, 7} |
| 2 | (3+8−4)%8=**7** | (3+4)%8=**7** | (7+4)/8=**1** | {7} | {3} |

校验：ΣnSlices=4+2+1=7=N−1 ✓；txIdx 并集={2,0,6,4,1,5,7}=全体非 me ✓；步 2 sendTo==recvFrom=7（±4 同 rank）✓。

**skipStep0TxPreCopy**：非 PCIe && N>1 → **生效**（:94-97）→ step0 tx 源=input 直取（:322-325）、上料跳过 step0TxSet{2,0,6,4}。

**动词序（Emit 产物）**：`COPY×8（上料，r∈0..7 全量）→ NHR_RUN×1 → COPY×1（归位）`——**恒 10 动词，与协议无关**（isDmaRead 是 sink 侧知识，Emit 层不知——知识不对称拆解的自洽证明：Emit 产物两侧同构，读/写差异全部下沉 sink）。无 SYNC_PEER_PRE/POST（threadNum=1×1=1，:121/:143 门槛不过——**复核已裁定（裁决 2）**：任务书"批前 PreSync/批后 PostSync 动词"仅 channelsPerRank>1 形态存在）。

**NHR_RUN 的 sink 原语展开（写侧，记 IB=input 基址、SB=scratch 基址、S=sliceSize、IST=inputSliceStride、rc(p)=p 的 remoteCclMem、ch(p)=p 的通道句柄，全部落 threads[0]）**：

```
step0: ChNotifyRecord(t0, ch(4), ACK=0)                    ← Record(recvCh, ACK)
       ChNotifyWait (t0, ch(2), ACK=0, 1836)               ← Wait(sendCh, ACK)
       WriteReduce ×4: txIdx∈{2,0,6,4}:
           src = IB + IST*txIdx        （step0-skip：input 直取）
           dst = rc(2) + S*txIdx       （对端 cclBuff 同构槽位）
           count = S/2, FP16, SUM
       ChNotifyRecord(t0, ch(2), DATA=1)
       ChNotifyWait (t0, ch(4), DATA=1, 1836)
step1: ChNotifyRecord(t0, ch(5), 0); ChNotifyWait(t0, ch(1), 0, 1836)
       WriteReduce ×2: txIdx∈{1,5}: src = SB + S*txIdx（scratch）; dst = rc(1) + S*txIdx
       ChNotifyRecord(t0, ch(1), 1); ChNotifyWait(t0, ch(5), 1, 1836)
step2: ChNotifyRecord(t0, ch(7), 0); ChNotifyWait(t0, ch(7), 0, 1836)   ← sendCh==recvCh
       WriteReduce ×1: txIdx=7: src = SB + S*7; dst = rc(7) + S*7
       ChNotifyRecord(t0, ch(7), 1); ChNotifyWait(t0, ch(7), 1, 1836)
```

上料 COPY 原语（sink 侧 LocalCopy 展开，Emit 的 8 条 COPY 逐条下发）：`LocalCopy(t0, dst=SB+S*r, src=IB+IST*r, len=S)` × r∈0..7（:188-203：src 偏移=IST*r、dst 偏移=S*r）；归位：`LocalCopy(t0, dst=OB+OST*3, src=SB+S*3, len=S)` ×1（:229-243 doPreCopy 分支）。**sink 原语展开写侧总数 = 8（上料）+8+6+5（步序）+1（归位）= 28**。

**模板等价序（老链语义，ST 归一化断言锚——与 sink 展开差 4 条）**：老链 LocalDataCopy 跳 step0TxSliceIdxs_{2,0,6,4}（:174-177）→ 上料只 4 条（r∈{1,3,5,7}）→ **模板原语序写侧总数 = 4+8+6+5+1 = 24**；差的 4 条 = step0Tx 槽位 {2,0,6,4} 的多余拷贝（写不被读的 scratch 槽——step0 tx 直取 input，该槽位数据不经 scratch）。ST 断言口径：剥去这 4 条 LocalCopy（(dst=SB+S*r, src=IB+IST*r) 且 r∈step0TxSet）后逐条相等。

**PCIe 读侧（isDmaRead）sink 原语差异**：skipStep0TxPreCopy **不生效**（isDmaRead→false）→ sink 上料 8 条全量（= Emit 产物逐条直发，无 skip——与写侧的"sink 剥 4 条"不同，读侧 Emit/sink/模板三者一致）；步原语换 SendRecvReadReduce 序（:642-687：Record(**sendCh**,ACK)→Wait(**recvCh**,ACK)→ReadReduce×nSlices(**recvCh**, src=rc(recvFrom)+S*rxIdx, dst=SB+S*rxIdx)→Record(**recvCh**,DATA)→Wait(**sendCh**,DATA)）；步 0 rx={3,1,7,5}、步 1 rx={3,7}、步 2 rx={3}。**读侧 sink 原语总数 = 8+8+6+5+1 = 28**（恰等于写侧 sink 展开值——步序握手结构对称：每步 2 Record+2 Wait+ΣnSlices 条数据原语，两侧同构；总差只在数据原语类型 WR vs RD）。

### 手算三（③Σ 申报，题面 premises：AG mesh 段 need=4、RS NHR 段 need=5，两域 [0..3]/[4..7]）

| 量 | 串行路径（现状/门控关） | 编排路径（门控开，任务书口径 Σ+域 mains） |
|----|------------------------|------------------------------------------|
| slaveThreadNum 申报 | max(4,5)=**5**（operator+= :213 现状一行不动） | 4+5+**2**=**11**（TEMPLATE_MAIN_THREAD_NUM=2，老 CalcRes :76-77 逐字——premises 为逐域 slave 需求） |
| HcclThreadAcquire threadNum | 5+1=**6** | 11+1=**12** |
| 域切池 | 无切池——全局束串行：threads[0]=全局 main（sink 计数全局起算），段序=帧序全序 | first-fit 首现序：域0 = threads[0..3]（need 4，**域 0 main=threads[0]**——全局 main 兼任，R14 :273-283）；域1 = threads[4..8]（need 5，域 1 main=threads[4]）；threads[9..11] 3 条冗余 |
| 对照 | max 语义=串行复用假设（段一个个跑取峰值） | 老算法域 0 main=threads[1]（threads[0]=控制，:255-257）vs R14 域 0 main=threads[0]（无控制线程）——**结构性差异**：老公式含控制位 1 条 + mains 槽位差；R14 精确需求 = Σ need_d = 9 total（省 3 条）。真机复核对照点（任务书 §二之三注意项原文收录） |

实现取**任务书口径**（Σ_d domainSlaveNeed + D，D 推广 TEMPLATE_MAIN_THREAD_NUM）：16P tier 实值 = 域0 mesh RS（N0=4，slaves 3）+ 域1 NHR（channelsPerRank=1，slaves 0）+ 2 = **5 slaves/6 total**（编排器实需 5 total，1 条冗余=控制位）。R14 精确口径（Σ need_d − 1 = 4 slaves/5 total）作为对照登记——两口径差恒 = 1（控制线程），若主会话裁定取精确口径改动一行（§十-3）。

---

## 十一、实施规格汇总（WP-1..WP-3 契约锚）+ 灰度三层

| WP | 改动文件 | 内容 |
|----|---------|------|
| WP-1 ① | `inc/plan_serializer.h` | 槽 16..24 常量（U_V3_TIER_* 镜像，consumer 同树 include 共享）；SerializePlanV3 gated tier 写（规范形态判别：hasTier && 2 段全 RS && seg0 MESH && seg1 NHR）；SerializeSegmentV3 NHR 拒收条件化（同 gate 放行骨架段——非 tier NHR 照旧拒） |
| | `inc/unified_route.h` | `UseUnifiedTierPath()`（宏 MC2_UNIFIED_TIER_RESOURCE 缺省关 + env MC2_USE_UNIFIED_TIER 缺省关，VERB 家族同式） |
| | `unified_exec/aicpu_consumer.h` | BlobFrameView tier 字段（ParseUnifiedBlobV4 读帧 0 槽 16..24）；ConsumeUnifiedPlanAicpu tier 分叉（骨架重建位置赋 pattern → ExpandTieredSegments → 展开段进 R14 编排器/串行驱动——串行驱动复用 DriveSegmentViaSink 单源件）；域 1 ranks 从 algHierarchyInfo.infos[0][1] |
| | `common/hccl_alloc_ctx_res.cc` | AssembleUnifiedAicpuResCtx 追加 level1 填充（physicalLevels[1] 存在时——加法不改法） |
| WP-2 ② | `inc/nhr_steps.h`（新） | CalcNhrSteps(me,n) 纯函数（步公式唯一实现点，Emit/sink/ST 三消费方） |
| | `inc/verb_sink.h` | VerbCall 内嵌 NhrRunScalars（valid 标记缺省 false——非 NHR 动词零语义变化）；kAicpuVerbMap NHR_RUN 行 |
| | `l4/backend_aicpu_reduce_slice.h` | Emit NHR 分支（gated UseUnifiedNhrAicpuPath：上料 COPY×n（全量，sink 侧 skip 见必答 D）→ NHR_RUN×1（载荷填充）→ 归位 COPY×1；channelsPerRank>1 时 SYNC_PRE/POST 按 sync 位；mesh 写侧分支一行不动） |
| | `l5/hcomm_verb_sink.h` | Dispatch case NHR_RUN → DoNhrRun（gated；ungated 恒 default 拒绝=现状）；DoNhrRun：isDmaRead 自判（IsPcieProtocol 同式）→ CalcNhrSteps 步循环 → 每步五段握手 + 逐 slice HcommWriteReduceOnThread/HcommReadReduceOnThread（wrapper :406-435/:642-687 逐行对照；批量快道 HcommHcommBatchTransferOnThread 真机优化挂 P2）；threads[channelIdx]；shim/测试桩补两原语 |
| | `inc/unified_route.h` | `UseUnifiedNhrAicpuPath()`（宏 MC2_UNIFIED_NHR_AICPU_RESOURCE + env MC2_USE_UNIFIED_NHR_AICPU） |
| WP-2b ③ | `inc/action_spec.h` | `DomainThreadTotalNeed(seg)`（pattern-aware RS·NHR / AG 不分叉）+ `CalcConcurrencySlaveClaim(segs)`（Σ_d (need−1) + D） |
| | `l2/recipe_sole.h` | SumRes 尾 gated 覆写（UseUnifiedConcurrencyPath——R14 既有门控复用，无新宏；ungated 一行不动） |
| | `l5/segment_orchestrator.h` | DomainThreadNeed 委托 DomainThreadTotalNeed（单源）；domainPostBarrier <2 线程 no-op（模板 threadNum>1 门槛对齐） |
| WT-1 | `unified_exec/test/poc_unified_tier_payload_test.cc`（新）+ `unified_exec/test/run.sh` / `unified/test/run_st_checker.sh` / `st_verification.cc` R15 节（== 59 顺延） | ①槽 golden（手算一）/tier 分叉 A/B 展开/非 tier 零变化/负例（slot16=0、非规范形态、ungated NHR 拒收维持）；②步 golden（手算二 24/28 原语序）/skip/读侧/gate 三态/与直接喂段对拍；③Σ 数学（手算三）/切池表/串行 max 回归（既有断言不破）；#22 对拍锁扩展（非 tier Plan env 开/关两路逐条相等 + tier 展开 vs 直喂等价） |
| WT-2 | `unified/test/REVIEW_CHECKLIST.md` | #23 槽位协议判别式纪律（新槽位全 0 缺省 + 判别式唯一 + 写入方零影响三证）；#24 NHR 动词与七动词合取隔离纪律（NHR_RUN 自持通道序不动计数器、线程/通道 notify 原语族隔离、单线程域 no-op 门槛） |
| WP-3 | 三入口全绿 + 方案文档 R15 节 + 缺口账更新 | 目标 ≥1659（+≥60：①≥15/②≥30/③≥15） |

**生产链真机接入三差距（P2 登记，host 全链机制 R15 兑现）**：①多域 topo 提取（GetUnifiedOpParamResCtxImpl 现状单域 level0——hierarchyRanks 二层提取）；②AICPU 半边通道协议探测（HcclChannelAcquireUnified 恒 Mesh1D/HCCS——按段 pattern 分叉对齐 R13 注册链前缀分叉）；③批量快道原语（HcommHcommBatchTransferOnThread）与真机池上限复核。

---

## 十二、主会话复核裁决项（5 条——2026-10-11 已全部裁定，裁定结果内嵌各条）

1. **NHR 建链集口径勘误**（§五）：任务书②改动面 2"NHR 需要 7 peer（N−1）全量建链（不裁）"与 R13 已交付的建链集语义（{±2^s}，N=8 me=3 → 5 条）矛盾——步公式 peer 值域恰为建链集，5 条全命中。**【裁定：采纳勘误，按建链集执行（R13 claim 语义不动）——任务书"7 peer"系误植，主会话已亲核 backend_ccu_reduce_slice.h:58-73】**
2. **NHR 段 SYNC 动词口径**（§四/手算二）：channelsPerRank=1（devkit 恒值）下模板 threadNum=1 → 无 Pre/PostSyncInterThreads——NHR 段动词序不含 SYNC_PEER_PRE/POST（任务书手算二"批前 PreSync/批后 PostSync 动词"仅多通道形态存在）。**【裁定：采纳——按模板门槛执行（=1 不发；>1 按 sync 位）；恒发形态单线程域 sink 必 Fail 已排除】**
3. **Σ 公式口径**（§七/手算三）：任务书口径 = Σ_d domainSlaveNeed + D（老 CalcRes 逐字推广，16P 实值 5 slaves/6 total，1 条冗余=控制位）；R14 精确口径 = Σ_d need_d − 1（4 slaves/5 total，零冗余）。**【裁定：采纳任务书口径（对齐老 CalcRes 逐字、冗余无害）；精确口径已登记对照，真机复核后可一行改；手算三题面 premises 行与 16P 实值行两行并存自洽】**
4. **生产链三差距的缺口账措辞**（§十一）："A/B 运行时激活与 NHR AICPU 执行面两缺口 → 兑现"的兑现范围 = **host 全链机制**（序列化承载/消费方展开/编排/NHR 执行面，测试全绿）；真机接入三差距（多域 topo/通道协议/批量快道）P2 登记。**【裁定：采纳 P2 登记措辞，不加急收编】**
5. **SerializeSegmentV3 NHR 拒收条件化放行**（§一★）：①的数据可达前提（tier 骨架 seg1 是 NHR 段，现状序列化必拒）。**主会话复核已裁定（2026-10-11 修正 A）**：放行 gate = UseUnifiedTierPath && 规范形态（2 段全 RS && seg0 MESH && seg1 NHR）**&& engine==AICPU（仅 AICPU 引擎消费的 blob 放行）**——实现形态 = 序列化器加布尔形参 allowNhrSkeleton（缺省 false），唯一生产调用点 hccl_alloc_ctx_res.cc:1164 传 `engine==AICPU && UseUnifiedTierPath(op)`（该处 verbPath 判定已有 engine 上下文）；CCU 降级路径（verbPath=false 走 v3）维持拒收——DispatchUnifiedPlanV3 只读槽 0..15，NHR 段按 mesh 布局错读是静默错跑面。既有测试调用点（直调 SerializePlanV3）缺省 false = 现状拒收零变化。
