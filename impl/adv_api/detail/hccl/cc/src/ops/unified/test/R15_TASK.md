# R15 任务书：TierParams 方案甲 + NHR AICPU 执行面 + 资源申请翻倍（三件同轮）

> **运行模式**：单 agent 全包（考古+代码+测试）。质量机制 = M0 手算落盘 + 主会话源码级复核验收锚。
> **上下文纪律**：你被拉起就是为本轮三件全部完成。若接近上下文上限，**写满断点文件后停止**，主会话用 SendMessage 复活你续推（不重拉新 agent、不丢失考古成果）——复活后先读断点文件再继续。

---

## 〇、铁律（先读，违者整轮打回）

1. **断言只增不减：基线 1659**（run.sh 519 / st_checker 891 / exec 249）。三入口 `0 failed`，新测试只加码。
2. **`REVIEW_CHECKLIST.md` 22 条全部适用**，尤其 #20 下发者模型（"OnThread"形参指指令落哪个线程队列，非由该线程发起；四循环均 host for 直调为证）、#21 预检降级非半途 Fail、#22 分叉分支与循环体单源关系常跑对拍锁（env 开/关两路 ConsumeUnifiedPlanAicpu 输出逐条相等，每次测试跑）。
3. **测试期望值独立手算，不转抄生成方**（M0 六必答之三组手算即验收锚）。
4. 源码考证给**文件+行号**证据，不望文生义。
5. **默认不 commit/stash/checkout**。测试副产品（M0 文档、/tmp 探针）留在原处不清理。
6. M0 六必答答完、三组手算落盘后，**发消息给主会话请求复核**，复核通过才动代码。M0 落盘文件：`unified/test/M0_R15_TRIPLE.md`（与本任务书同目录）。
7. 大输出先过滤（grep -n 精确行号/head），不整段回显；读大文件先定位行号范围。

---

## 一、三件及耦合关系（为什么同轮）

| # | 事项 | 一句话 | 落点 |
|---|------|--------|------|
| ① | TierParams 方案甲承载 | 把 Plan.tier 九值通过骨架段 0 帧未用槽 16..24 送到 AICPU 消费方 | blob 序列化方（CCU 侧写槽）+ 消费方读槽（aicpu_consumer） |
| ② | NHR AICPU 执行面 | backend_aicpu_reduce_slice.h 无 NHR 分支（建链集 5 通道对 7 peer ChannelOf Fail）——补 Emit NHR 分支 + sink 动词扩展 | backend_aicpu_reduce_slice.h + hcomm_verb_sink.h |
| ③ | 资源申请翻倍 | unified 现状 slaveThreadNum=max 串行复用假设 → 编排路径改 Σ+域 mains 模型 | tier_expand / action_spec.h 资源合并语义 |

**①②必须同轮**：数据可达（tier 参数到达消费方）+执行可达（NHR 可被 AICPU 执行）缺一不可——只做①则 A 形态批内 nhr 段没有执行面，只做②则 A 展开仍缺 tier 参数。**③独立但搭车**：Σ 与 max 之争不依赖①②，同轮省一次上下文加载。

**scope 死线**：AG NHR v5 接入等其他候选**先不管**（用户裁定）。

---

## 二、① TierParams 方案甲（槽位协议设计已定，本轮落地）

**已定设计（M0_R14 §四，不重开）**：骨架段 0 帧**未用槽 16..24**——槽 16=hasTier 标记（1=tier 有效）、槽 17..24 = 九值 threshBytes/partRatioNum/partRatioDen/bwFastNum/bwFastDen/bwSlowNum/bwSlowDen/alignBytes。

设计依据（照抄执行，不重新论证）：
- 不升 blob 版本。CCU 写入方/server 只读槽 0..15，16+ 透传或忽略零影响。
- AICPU 路径 D2H 直取 host 序列化产物，不经 CCU 写入方改写。
- 非 tier Plan 槽 16..31 恒 0（现状）→ 槽 16==1 天然判别，无需额外版本位。
- **blob 仍是骨架 2 殯，A/B 展开仍在消费方 exec 期**（R6-③ 裁决不预编）——方案甲只是把 TierParams 送到消费方，展开逻辑不动。

**改动面**：
1. **写入方**（CCU 侧 blob 序列化，即 unified 侧产 blob 的代码——考古确认确切落点）：Plan.tier 有效（hasTier==true）时把九值写入段 0 帧槽 16..24。非 tier Plan 保持恒 0（现状）。
2. **读取方**（aicpu_consumer RebuildActionSpecFromFrame 或等价读取点）：读槽 16==1 则解析槽 17..24 重建 TierParams，塞进重建的 ActionSpec；否则走现状（无 tier）。
3. **灰度三层**（与 VERB 家族同式）：编译期宏（缺省关）+ env（缺省关）+ UseXxxPath 前置判断。合入零行为变化。

**M0 需答**：
- 必答 A：写入方落点考古——序列化产 blob 的代码在哪、段 0 帧槽布局定义在哪、当前写槽 0..15 的循环长什么样（文件+行号）。CCU 写入方只读 0..15 的论证再核一遍（槽 16 写入是否真的零影响）。
- 必答 B：读取方落点考古——RebuildActionSpecFromFrame（aicpu_consumer.h :351 附近）读槽的现有结构、TierParams 重建后沿哪条数据流到达 A/B 展开点（ActionSpec.tier 字段链路）。
- 必答 C：槽 16 判别式的完备性——非 tier Plan 槽 16 恒 0 是否有反例（考古所有写槽 16 的现存代码路径）。

---

## 二之二、② NHR AICPU 执行面（核心考古：AICPU 形态是批量+单原语，非 CCU 逐 slice）

**现状缺口**：backend_aicpu_reduce_slice.h（144 行）Emit 仅 mesh 写侧分支（SYNC_PEER_PRE→WRITE×(N-1)→COPY→REDUCE×(N-1)→…）——无 NHR 分支。建链集 5 通道对 7 peer，NHR 需要 7 peer 全量，ChannelOf Fail。

**复刻对象（存量 AICPU NHR RS，ins_temp_reduce_scatter_nhr.cc 442 行）**：
- GetThreadNum = 1*channelsPerRank（:72）；KernelRun :74-165：PreSync（:125）→ LocalDataCopy×chan（:137）→ RunNHR×chan（:152）→ PostLocalCopy×chan（:155）→ PostSync（:161）。
- **RunNHR :247-360 核心形态**：逐 step **host 期批量构造 tx/rx slices**（step 内全部 slice 攒 vector）→ 一步 SendRecvBatchWriteReduce（写侧 :355）/ SendRecvReadReduce（PCIe DMA 读侧 :351；isDmaRead_=IsPcieProtocol :87-88）。**与 CCU kernel 逐 slice WriteReduce 是不同形态——AICPU 是批量+单原语**。
- GetStepInfoList 公式同 CCU kernel：sendTo=(x+N−2^s)%N、nSlices=(N−1+2^s)/2^(s+1)、txIdx 步进 −2^(s+1)——与 R13 server 端重建同源。
- 特有分支：skipStep0TxPreCopy（非 PCIe 且 N>1，step0 tx 直接从 input 取 :128-131/:318-321）、CalcDataSplitByPortGroup 多通道切分（devkit 链 channelsPerRank 恒 1，可裁）。
- 通道查表 channels_[recvFromRank].at(channelIdx)（:275-278）——按 peer 查表，建链集成员才能查到（与 unified HcommVerbSink.ChannelOf 同型）。

**改动面**：
1. **backend_aicpu_reduce_slice.h 加 NHR 分支**：Emit 识别 NHR 段（按 R13 已交付的段类型判别）→ 按上述形态产出动词序列。**sink 动词面现状**（hcomm_verb_sink.h :143-155）仅 READ/WRITE/REDUCE/COPY/POST_COPY/SYNC_PEER_PRE/SYNC_PEER_POST——NHR 的 SendRecv 型原语（收发同通道对）需**新动词或复用扩展**，这是本轮最重的设计点，M0 必须给出动词方案（见必答 D）。
2. **建链集**：NHR 需要 7 peer（N-1 个）全量建链（不裁），5 通道对（channelsPerRank）——与 R13 段判别对齐。
    - 注意：unified 建链是按 verbs 聚合（L4→L5 的 ChainBuild 需求收集），NHR 段的建链集是全 peer 全通道。M0 考古 L4 CollectChannelRequirements（或等价物）对 NHR 段的现有处理——为什么现在 5 通道对 7 peer 会被裁掉。
3. **灰度三层**：同式（宏+env+UseXxxPath）。
4. **分叉/不升版本**：既有 mesh 写侧分支一行不动，NHR 分支是新加 case/branch。

**M0 需答**：
- 必答 D：**sink 动词方案**——NHR SendRecv 型原语映射到 verb 的方案（新动词 VERB_NHR_* vs 复用扩展既有动词）。论证：批量 slice 列表如何过 verb（vector 打包 vs 单 verb 携带 slice 数组）、DMA 读侧（isDmaRead）如何区分、与既有七动词的合取语义不冲突（每步 SendRecv 都是收发同通道对、屏障语义如何与 R14 编排器批尾合取共存）。
- 必答 E：NHR 段判别与建链集——R13 段类型在 L4 收集建链需求时的现状行为（文件+行号）；NHR 段需要全 peer 建链，与 mesh 段共享链路集时是否冲突（mesh 只需部分 peer？考古各段类型的建链集需求差异）。
- 必答 F：执行面与编排器接口——R14 编排器（segment_orchestrator.h）DriveSegmentViaSink 逐段驱动，NHR 段进编排器时的资源语义（每通道一 StepInfo 批、多通道时是否分段）。

---

## 二之三、③ 资源申请翻倍（Σ+域 mains 模型，门控防串行回归）

**现状**：action_spec.h :213 `slaveThreadNum = std::max(...)` 串行复用假设——段一个个跑，线程可复用，取峰值。
**目标**：编排路径下改 **Σ+域 mains**，向老算法在产语义对齐。老算法原型（ins_reduce_scatter_parallel_executor.cc）：
- CalcRes :76-77：`slaveThreadNum = intra + inter + TEMPLATE_MAIN_THREAD_NUM(2)`（:21 =2）。
- :252-257 切池：threads[0]=**专职控制线程**（只做同步不承载数据动词）、intra=[1..1+n0)、inter=其余、头条是域 0 main。
- notifyIdx :263-266：控制→域main 用各域槽数、域main→控制 {0,1}。
- Orchestrate while :410-464：PreSync→两域 KernelRun host 顺序下发→PostSync→P0/P1 两域轮换——**A 交错原型**。

**门控死线（用户可见行为零变化）**：编排路径（UseUnifiedConcurrencyPath()==true）→ Σ 申报；串行路径（env 关/宏关）→ **维持 max 现状不动**。两条路径的申报语义差异要写进 M0（必答 G）。
**注意**：R14 编排器是**域 mains 互 notify 交叉对**（t0↔t4，省一条线程），老算法是独立控制线程星形合取——这是结构性差异，本轮**不引入独立控制线程**（R14 交叉对已交付），资源语义对齐的是"域 mains 承载数据动词+合取"模型，非拓扑照抄。差异点记入 M0 作真机复核对照点。
**合并语义再考证**：action_spec.h :213 是 ResourceClaim 合并（多个 Plan 合并成 aggregate 时 max），单个 Plan 内 segments 是数据（任意段序列），Plan.tier 的 tier_expand（A/B 选择器）产出段列表时直接 Σ 申报——落点在 tier_expand 或等价产物生成处，M0 考古确切落点（必答 G）。

**M0 需答**：
- 必答 G：Σ 申报落点考古——tier_expand（A/B 展开产物生成处）在哪个文件哪个函数产出段列表与资源申报；:213 max 语义在哪些调用路径被触发（合并 vs 单 Plan）；Σ 与 max 的切换在哪个点判定（UseUnifiedConcurrencyPath 门控判定点）。
- 必答 H：资源切池与 R14 编排器对齐——R14 域分组（ranks 相等去重首现序）/线程切池已在 segment_orchestrator.h 实现，资源翻倍后 resCtx.threads 数量变化对编排器切池的影响（域 0 main 是否仍是 threads[1]？老算法域 0 main=threads[1]（threads[0] 是控制），R14 是 ranks 首现序——对齐说明）。
- 必答 I：翻倍后的资源回收/校验——op_common.cc :1097-1150 HcclGetThread（AICPU_TS 分支 threadNum=slaveThreadNum+1）对 Σ 申报的适配；Σ 值域（一 Plan 多段全并发时 threads 数是否超真机上限，给出上限考证）。

**改动面**：
1. Σ 申报逻辑（门控在 UseUnifiedConcurrencyPath）。
2. HcclGetThread 适配（若必答 I 考古显示需要）。
3. 灰度三层同式。
4. 测试：串行路径 max 语义回归断言（既有测试不破）+ 编排路径 Σ 断言（新）。

---

## 三、M0 九题总览（A..I，落盘 M0_R15_TRIPLE.md）

| 题 | 一句话 |
|---|------|
| A | blob 写入方落点（段 0 帧槽布局、写槽 0..15 循环、槽 16 写入零影响再论证） |
| B | 读取方落点（RebuildActionSpecFromFrame 读槽结构、TierParams 数据流到展开点） |
| C | 槽 16 判别式完备性（非 tier Plan 槽 16 恒 0 反例考古） |
| D | sink 动词方案（NHR SendRecv 型原语映射，本轮最重设计点） |
| E | NHR 段建链集（L4 廏集需求现状、mesh 与 NHR 建链集冲突分析） |
| F | 编排器接口（NHR 段进编排器资源语义、多通道分段） |
| G | Σ 申报落点（tier_expand 产出点、:213 max 触发路径、门控判定点） |
| H | 资源切池与编排器对齐（域 mains 线程序、资源翻倍后切池影响） |
| I | 翻倍后资源适配（HcclGetThread Σ 适配、threads 上限考证） |

## 四、三组手算（验收锚，M0 落盘，期望值独立手算）

1. **手算一（①槽位编码）**：给定 TierParams{hasTier=true, threshBytes=8MiB, partRatioNum=3, partRatioDen=4, bwFastNum=2, bwFastDen=1, bwSlowNum=1, bwSlowDen=2, alignBytes=32}——逐槽手算槽 16..24 的值（十进制与十六进制）；非 tier Plan 槽 16..24 全 0 案例一例。
2. **手算二（②NHR 动词序）**：给定 N=8 rank，写侧（非 PCIe）：逐 step 手算 SendTo/recvFrom/nSlices/txIdx 序列（GetStepInfoList 公式同 CCU），以及 step0 的 skipStep0TxPreCopy 是否生效（非 PCIe 且 N>1 → 生效）；每个 step 的动词序列（SendRecvBatchWriteReduce 一原语/步）+ 批前 PreSync/批后 PostSync 动词；PCIe 读侧（isDmaRead）动词序差异。
3. **手算三（③Σ 申报）**：给定一 Plan 两段（AG mesh 段 need=4、RS NHR 段 need=5，两域各 [0..3] 与 [4..7]）——手算 max vs Σ 两种申报值（4+5+2=11 vs max(4,5)=5）；编排路径域切池（域 mains 线程序对照，说明域 0 main 线程下标）与串行路径（维持 max=5）对照表。

## 五、实施规格（复核通过后）

- **WP-1**：①写入方+读取方+灰度（M0 必答 A/B/C 锁定落点）。
- **WP-2**：②backend NHR 分支+sink 动词扩展+建链集（必答 D/E/F 锁定）。
- **WP-2b**：③Σ 申报+门控+适配（必答 G/H/I 锁定）。
- **WT-1**：st_verification 新节（R15 节）+ exec 新测试（poc_unified_tier_payload_test.cc 或等价）+ run.sh env 三态探针扩展。R14 的 #22 单源对拍锁对分叉分支同样适用（env 开/关两路输出逐条相等）。
- **WT-2**：REVIEW_CHECKLIST 增补新纪律（预期：#23 槽位协议判别式纪律、#24 NHR 动词与既有七动词合取语义隔离纪律）。
- **WP-3**：三入口全绿验证（目标 ≥1659 只增不减）+ M0 补充 + 执行记录落盘（方案文档 R15 节，插在 R14 记录上方）。

## 六、验收标准（主会话终审）

1. 三入口 0 failed，总断言 ≥1659（预期新增 ≥60：①≥15/②≥30/③≥15）。
2. M0 九题齐、三组手算齐、行号锚齐全（主会话逐条复核）。
3. 既有分支一行不动验证（git diff：mesh 写侧分支、串行路径 max 申报、既有 for 循环）。既有七动词合取语义零扰动验证。
4. 灰度三层全关时零行为变化（与 R14 同式验证）。
5. 缺口账更新：A/B 运行时激活与 NHR AICPU 执行面两缺口 → 兑现（记入执行记录）。
6. 执行记录落盘方案文档；REVIEW_CHECKLIST 22→24 条。
