# M0_R17 · 流水模型轮（PipelineModel/DryRun + 语义后置条件 checker）

> **轮次任务书**：R17_TASK.md（铁律/两件定义/结构定稿草案）。本文件 = 六必答定稿，**复核通过才动代码**。
> **前置**：R16 已交付 ThreadPolicy 表（verb_sink.h:147）+ ThreadFor（hcomm_verb_sink.h:131）——threadId 推演直接查表。
> **积木行号已复核**（2026-10-13 实测，与任务书一致）：Compile l2/l1_adapter.h:178 / ExpandTieredSegments inc/tier_expand.h:291 / OrchestrateSegments l5/segment_orchestrator.h:154 / CaptureVerbSegment l5/verb_capture.h:52 / DomainThreadTotalNeed inc/action_spec.h:291 / CalcNhrSteps inc/nhr_steps.h:57。

---

## 〇、两处任务书勘误（手算推翻，先行声明）

1. **16P tier 域切池实值**：任务书写"域0 base=0..2、域1 base=3..4"（合计 5）——按 DomainThreadTotalNeed 实算：域 0（mesh RS，n=4）need=**4**；域 1（nhr RS，channelsPerRank=1）need=**1**；Σ need = 5 total。切池：**域 0 = threads[0..3]、域 1 = threads[4]**（任务书的 0..2/3..4 是把 Σ 申报 slaves 口径 3+0+2 误代入编排器切池口径——两口径差 1，M0_R16 §二已登记"1 条冗余=老控制线程位"）。断言锚以本勘误为准。
2. **"AG 14 动词"与"VIA_CCUBUF=11"**：不存在 14 的形态；VIA_CCUBUF 中转也是 **10 而非 11**——Emit ① 两判独立（backend_aicpu_collect_all.h:51-58），首判 `inputAddrOff != outputAddrOff` 对 mesh AG 恒 false（InOffset(me)==OutOffset(me)：同基址、同 stride、同 algRank），中转形态只有上料 COPY 1 条。三形态规则表全列见 §五-1（SYMMEM 9 / MEM2MEM 10 / VIA_CCUBUF 10），既有断言 `copies >= 1 && reads == 7`（st_verification.cc:156）与此一致。

---

## 一、必答 1：DryRun 的 driver 单源方案

**裁定：DryRun 不复用 DriveSegmentViaSink（exec 期真驱动链），Emit 步复用/提取 verb_capture.h 的捕获链。**

理由（考古结论）：
- `DriveSegmentViaSink`（unified_exec/aicpu_consumer.h:461）是 exec 期真驱动件——内部构造 `HcommVerbSink(res)` 真下发原语，依赖 HcommVerbRes 真资源束（threads/channels 查表）与 msg 现值重建；空 res 直接 Fail（DoNhrRun "empty thread pool" / ChannelOf 查空）。DryRun 若走它 = 必须合成整套假资源 + 假通道协议，注入面比复用收益更重。
- `CaptureVerbSegment`（l5/verb_capture.h:52）就是"BuildArgs 分派 → 真后端 Emit → sink 捕获"的**生产链单源件**（hccl_alloc_ctx_res.cc v5 灰度同款），零资源依赖，输入恰是 DryRun 的口径（Compile 期 ActionSpec + engine）。

**提取点（唯一生产码改动，unified 树内非跨线）**：verb_capture.h 加段级函数

```cpp
inline std::vector<VerbCall> CaptureVerbCalls(const ActionSpec& seg, Engine engine);
```

CaptureVerbSegment 改为"组装 VerbSegment + 委托 CaptureVerbCalls"（行为零变化，verb_capture 既有测试自动锁）。DryRun 的 driver = `CaptureVerbCalls(seg, engine)` 包装。

**编排步**：DryRun 调 `OrchestrateSegments`（单源——批划分/域切池/barriers 全部由它产出，不自建第二份批划分，REVIEW_CHECKLIST #16）。输入合成**足额池**（threads 数 = Σ DomainThreadNeed → degraded 恒 false）；driver 闭包旁路收集逐段 calls（`OrchestrateSegResult.calls` 字段现成，:83）——**不动 OrchestratorResult 结构**（R14 交付件零侵入）。

**#22 对拍锁扩展点（第三路）**：
- 主锁：DryRun 逐段动词序 == `CaptureVerbCalls(segs[i], engine)` 直连（同源必然相等——锁"DryRun 未自建第二 Emit"），常跑断言。
- 辅锁：tier A 形态（16P）DryRun steps 动词总数 == 29（8+6+6+9，§五-2 探针勘误后手算；灰度关对照口径 8/9/8/9=34），与 exec 入口 tier_payload T3 的 segmentsRun==4 + verb 序交叉。
- 边界声明：与 ConsumeUnifiedPlanAicpu 的 perSegTrace 全串对拍**只在 msg.count == Compile count 用例成立**（exec 期现值重算 sliceSize，几何不同）——M0 声明此边界，全量 trace 对拍不做（DryRun 是编译期口径，exec 动态量不在模型内，见 §四输入面）。

## 二、必答 2：threadId 推演口径

**基准：编排器全局池内 index**（= 域 base_d + 域内槽位）。域切池后 sink 拿到的 `domainRes.threads[i]` 即全局池 `threads[base_d + i]`（segment_orchestrator.h:269-279 切池实证），全局 index 才是跨段可比口径。

逐策略推演（查 kAicpuVerbMap，**段内独立计数器**——与 sink 同构，DriveSegmentViaSink 逐段新建 segSink，"READ 线程序按段从 threads[0] 起算"）：

| 策略 | 推演 | 全局 index |
|---|---|---|
| MAIN | 恒域子束 [0] | base_d |
| COUNTER_OFF_0 | 段内第 i 条 READ → 域内 i（ThreadByCounter hcomm_verb_sink.h:671-686：**已完成同动词计数**即第 i 条（0 起），threads[i+0]） | base_d + i |
| COUNTER_OFF_1 | 段内第 i 条 WRITE → 域内 i+1 | base_d + i + 1 |
| BY_CHANNEL | 载荷 `nhr.channelsPerRank`（VerbCall.nhr 现成）为循环上界：**展开为 C 条 step**（threadId = base_d + channelIdx，几何同）——动词粒度保持 1 动词 ↔ 多 step 行，与 sink 下发序（channelIdx 外循环）一致；devkit 恒 1 → 1 行零负担 | base_d + channelIdx |
| MAIN_PLUS_SLAVES | 主下发 + 全从参与：PipelineStep **加字段 `bool mainPlusAll`**（SYNC 动词 true，threadId 记 base_d 主） | base_d |
| NA / NOT_WIRED | CCU 段（kCcuVerbMap 全 NA）：threadId 恒 0 + 模型级 `ccuEngine` 标注（CCU 无线程概念，DryRun 主场景 AICPU） | 0 |

**batch 字段**：step 所属批号 = segIdx ≤ batchTail[b] 的最小 b（编排器口径）。DryRun 恒走编排算法（足额池非 degraded）→ 批结构 = 并发语义视角；运行时池不足降级是运行时事实，DryRun 不预测（`model.degraded` 恒 false 转录编排器输出）。

## 三、必答 3：checker 挂钩形态

**裁定：独立诊断入口（semantic_checker.h 纯函数），不挂钩 Compile、零生产码改动面、零跨线。**

理由：
1. **Compile 期零几何陷阱**（Explore 盘点实证）：真链路 Compile 时 count=0（msg 现值 exec 期才注入，R10-① 的 blob 是零几何布局）——挂在 Compile 内对真链路恒平凡通过，价值为零；
2. 测试/golden 交叉锚直接喂（不需要宏+env 矩阵——三入口 run.sh 不改）；
3. 真链路自动查（Compile 后置）随"exec 期 Plan 现值单源重建件"后续轮评估——**登记 P2**。

**落点与输入形态**：`l5/semantic_checker.h`（与 verb_capture.h 同级同理由——便捷件需 include 后端注册面触发静态注册，不进 inc/ 值对象层）：

```cpp
// 纯函数核心：段序列 + 逐段动词序 → 后置条件判定（零 Emit 依赖，可单测直喂）
struct SemanticReport { bool pass; std::vector<std::string> violations; };
SemanticReport SemanticCheck(const std::vector<ActionSpec>& segs,
                             const std::vector<std::vector<VerbCall>>& callsPerSeg);
// 便捷件：Plan + engine → 逐段 CaptureVerbCalls 组装后调核心（与 DryRun 同一捕获单源）
SemanticReport SemanticCheckPlan(const Plan& plan, Engine engine);
```

①②同基建落点：DryRun 的 perSegCalls 正是核心函数的第二参（PipelineModel → checker 零转换直喂）。

## 四、必答 4：区域判定规则（符号执行模型）

**裁定：不做地址值域重叠仲裁——按"动词语义 + SPMD 镜像约定"直接符号化；region 仅用于参数合法性与期望域。**

- **符号值**：字节区间 → 来源表达式。本地域三类 `in[off..]` / `out[off..]` / `cclbuf[off..]`（scratch 别名），远端符号变量命名 `peer<r>.{in|out|cclbuf}[off..]`（r=userRank）。区间离散化：全部动词边界并集做断点，`map<区间, 符号>` 维护。
- **SPMD 镜像约定**（跨 rank 数据流的符号化，M0 核心裁定）：
  - `X.WRITE(peer=Y, src=in[区间A])` ⇒ 符号断言 `Y.cclbuf[X 槽区间] ≡ X.in[区间A]`；
  - 本地 READ/REDUCE 读 cclbuf 槽时查贡献表：槽区间有 WRITE 记录 → 符号 = 对应 IN；无记录 → "读未写区" violation（病灶反例的捕获机制）。
- **SYMME in==out**：**transport==SYMMEM 声明优先于地址推断**（ActionSpec 字段结构性判定）——in/out 视为同域别名，Emit ① 不发 COPY（同址零拷贝）、READ src=OutOffset(peer) 直读符号 = `peer.out[peer 片] ≡ peer.in[peer 片]`。
- **后置条件式（逐 kind，collective 语义闭包）**：
  - **CollectAll**：∀r∈域 `out[OutOffset(r) 区间] ≡ in[InOffset(r) 区间]`（本地 COPY 落位 + 逐 peer READ 覆盖，来源逐片核对）；
  - **ReduceSlice（mesh）**：`out[OutOffset(me)] ≡ ⊕_r in[r 视角的 me 片]`——初值 COPY(InOffset(me)→OutOffset(me)) + REDUCE×(n−1) 累积（src=cclbuf[S·r] 查镜像贡献 = 对端 WRITE 的 in[me 片]），**贡献数恰 n−1 且来源互异**；
  - **ReduceSlice（NHR）**：NHR_RUN 展开为**全 rank 联合符号执行**（n 视角逐 rank CalcNhrSteps 同步推步——步表是 me 的纯函数，host 代价 O(n²·logn·slice)，n≤16 可控；步间握手 = SPMD 步同步假设，NHR 协议自带）。逐 slice 几何按 DoNhrRun 实现平移（hcomm_verb_sink.h:509-538）：写侧 `WR(sendCh, dst=peer.cclbuf[S·tx+rptInOff], src=源[tx])`——源在 step0 且非 DMA 时 = `in[InBase+inStride·tx]`（skipStep0Tx 直取 input），否则 = `cclbuf[S·tx]`（上料 COPY 的产物）；读侧 `RD(recvCh, dst=cclbuf[S·rx], src=peer.cclbuf[S·rx])`——本地 cclbuf 槽累积 `⊕`（ReadReduce 语义）。**skipStep0Tx 按"非 DMA"建模**（isDmaRead 是通道协议知识，checker 无通道表；DMA 形态 step0 tx 源 input 直取 vs 上料后取，被跳槽位的上料拷贝不被读——语义等价，几何差登记 P2 边界）。
- **SYNC_***：数据流无影响（不产生 violation；"序不越屏障读未写区"可选增强不做，登记）。
- **已知边界（不做不吹，任务书 §三同款）**：WRITE 远端消费序不验证（镜像约定只保证"写了什么"，不保证对端何时读）；两引擎混排 Plan 不校验引擎间等价（ST checker 已有职责）；NHR DMA 读侧形态 P2。

## 五、必答 5：三组手算（断言锚，期望值独立于生成方）

### 1. AG 8P sole[mesh] AICPU 动词规则表（transport 三形态，me 任取）

| transport | 动词序 | 计数 |
|---|---|---|
| SYMMEM | SYNC_PRE, READ×7(src=OutOffset(peer) 直读), SYNC_POST | 9 |
| MEM2MEM | COPY(in→out 归位), SYNC_PRE, READ×7, SYNC_POST | 10 |
| VIA_CCUBUF | COPY(in→scratch[me] 上料 **1 条**——①首判 `inputAddrOff != outputAddrOff` 对 mesh AG 恒 false，见 §〇勘误 2), SYNC_PRE, READ×7(src=ScratchOffset(peer) 对端槽), SYNC_POST | **10** |

READ 几何：`src` 按 transport（上表）、`dst=OutOffset(peer)`、`size=SliceSizeOf(peer, 8)`。**测试取行**：MakeOp 缺省 supportSymmetricMemory=false（st_verification.cc:74-83 不设）→ ResolveTransport（recipe_sole.h:243-246）产 **VIA_CCUBUF → 10 动词行**（COPY 上料 1 条恒发）；若用例显式置 symmem 则走 9 动词行。threadId（池 7 条 threads[0..6]，域 0 base=0，AG need=n−1=7）：SYNC→0（mainPlusAll）；READ 第 i 条（升序跳 me）→ 域内 i → **全局 i**（READ 首条恰 threads[0] 主承载，COUNTER_OFF_0）；COPY→0。

**DryRun 前置条件（同口径）**：本用例 plan.segments.size()==1 单段 → 编排器单批单段（batchTail={0}，域内补 Post 不触发——批内 post=T；跨域合取不触发——D=1）。steps 全 10 行 batch=0。

### 2. 16P tier A 形态（parallel[mesh,nhr] RS，**AICPU**，双域 {0,1,2,3}/{0,4,8,12}，me=0，构型同 st :3586-3593）

- 段序 4 段：`[mesh P0(域0), nhr P1(域1), nhr P0(域1), mesh P1(域0)]`（骨架 segments[0]=域0 mesh / [1]=域1 nhr——RecipeParallel d 升序 BuildStage + ExpandTieredSegments :320-332 排列实证）；批 2 个：{seg0,seg1} / {seg2,seg3}（postBarrier=(i==1‖i==3)）。
- **引擎注意**：本手算按 **AICPU** Emit（DryRun 主场景 = threadId 列）。CCU 同构型动词数不同（mesh 段 = SYNC+READ×3+REDUCE+SYNC 6 动词、NHR 段 = SYNC+NHR_RUN+SYNC 3 动词——R13-④' 既有锚 `vss[1].calls.size()==3` 实证），checker 交叉锚 #3 用 CCU 口径喂核心函数，DryRun 步数锚用 AICPU 口径——两者分开断言，不混。
- 切池（§〇勘误口径）：域 0 need=4 → threads[0..3]；域 1 need=1 → threads[4]；Σ=5。
- 动词数（**实施探针勘误 3**：mesh 段动词数随 post 位两态——批内段 post=F 无 SYNC_POST）：seg0 mesh post=F = SYNC_PRE+WRITE×3+COPY+REDUCE×3 = **8**；seg1 nhr = COPY×4+NHR_RUN+COPY = **6**；seg2 nhr = **6**；seg3 mesh post=T = SYNC+WRITE×3+SYNC+COPY+REDUCE×3 = **9**；合计 **29**（非 30——原手算 mesh 段恒 9 误含批内段的 SYNC_POST）。**测试须 env `MC2_USE_UNIFIED_NHR_AICPU=1`**（setenv，r15_env_probe 同型）——env 关时 nhr 段 Emit 走 mesh 写侧分支（perSeg 8/9/8/9=34 的灰度关口径，可作第二组灰度对照锚）。
- threadId 分布（全局 index）：
  - mesh 段（base=0）：SYNC→0(all)；WRITE#0/#1/#2→**1/2/3**；SYNC→0；COPY→0；REDUCE×3→0
  - nhr 段（base=4）：COPY×4→4；NHR_RUN→4（BY_CHANNEL channelIdx=0 单行）；COPY→4
- barriers 共 **4 条**（segment_orchestrator.h:333-337 域内补 Post / :368-421 跨域合取实证——两者 `segIdxAfter` 都记**批尾**，非段号）：批 1（seg0 mesh post=F、seg1 nhr post=T）→ 域内补 Post{segIdxAfter=1, domainInternal=T, 域0} + 跨域合取{segIdxAfter=1}；批 2（seg2 nhr post=F、seg3 mesh post=T）→ 域内补 Post{segIdxAfter=3, domainInternal=T, 域1} + 跨域合取{segIdxAfter=3}。触发条件：批内 post=F 段所在域（seg0→域0、seg2→域1）——两批双域各触发一次，尾段 post=T 不重复触发。

### 3. RS NHR 8 卡 me=3（AICPU gated 开）

- 动词序恒 **10**：COPY×8（上料全量 r=0..7，src=inBase+RsInStride·r、dst=SB+S·r、size=SliceSizeOf(r)）+ NHR_RUN（11 标量载荷）+ COPY×1（归位 SB+S·3 → OutOffset(3)）。
- 步表首步（CalcNhrSteps(3,8) step0，python 复算 = 公式独立手算一致）：deltaRank=1、**sendTo=2、recvFrom=4**、nSlices=4、tx=[2,0,6,4]（首 sendTo=2 步进 −2 mod 8）、rx=[3,1,7,5]（首 me=3 同步进）。全步 tx 并集 [0,1,2,4,5,6,7]（R13-③ 41 原语基线的 WR7 序 [2,0,6,4,1,5,7] 同集——步序 vs 升序差异是序不是集，交叉验证 ✓）。建链集 {1,2,4,5,7}（=步表 peer 值域闭包）。
- threadId：全部 10 动词 → threads[0]（域 need=1，COPY=MAIN、NHR_RUN=BY_CHANNEL channelIdx=0）。

## 六、必答 6：golden 交叉锚清单（Explore 盘点定稿，五锚）

| # | 用例 | 位置 | 组合 | 喂入方式 |
|---|---|---|---|---|
| 1 | R12-⑦ 全链等价三例 | st_verification.cc:3177-3268 | AG mesh 8P me=3 300k（CCU）+ RS mesh 8P + RS 9P | CompileVerbBlob lambda（:2798）同构造，plan+engine 直传 SemanticCheckPlan |
| 2 | R13-① NHR 三组 golden | st :3285-3377（配 verb_exec V8 :1018） | RS NHR 8P me=3，jetty 1/2、slice 三档 | 手造 ActionSpec（:3290 EmitNhrSegment 同款）包 `Plan{segments={seg}}` + CCU Emit 捕获喂核心函数（兼验"纯 ActionSpec+动词序"入口形态） |
| 3 | R13-④'/R15-③ 16P tier | st :3580-3662 / :4231-4284 | tier 双域 mesh+NHR、A 4 段 + B 12 段、16 卡 | Compile→Expand→既有 `CaptureVerbSegment(planX, i, Engine::CCU)` 循环产物直喂 |
| 4 | V1/V2/V3 golden-bytes | verb_exec_test.cc:423/:476/:527 | AG 4P me=1（CCU）+ RS 8P me=3 + RS 9P me=0 | MakeAgOp/MakeRsOp+EmitSegment 的 plan 出参 + engine 常量 |
| 5 | tier_payload T3 全链 | tier_payload_test.cc:286-364（helper :120） | RS tier 16P **AICPU**、A 展开 4 段消费实驱 | MakeTierSkeletonPlan 的 plan + AICPU + ExpandTieredSegments 段序列（AICPU 引擎+tier 唯一全链锚，补 #1-#4 偏 CCU 盲区） |

**排除陷阱**（盘点实证）：count=0 用例（R10-①/orch T5）零几何平凡通过不作锚；R10-② 真值 count 在 exec 期注入 Compile 期不可见不作锚；R12-③ two_shot（ForSecondStageViaScratch 衔接）M0 声明**不覆盖**（scratch[0] 恒槽语义绕，登记 checker 边界，后续轮评估）。

**checker 在 #1-#5 必须全 PASS**（有旧实现基线背书——FAIL 即 checker 错）。

**病灶反例三件**（"改回也过"防线，构造性 FAIL 锚）：① 删一条 READ → AG out[peer 片] 未覆盖 → violation"未覆盖片"；② READ src 几何错位（取错片）→ 来源表达式不匹配 → violation"来源错"；③ REDUCE 少一次 → RS Σ 贡献数 = n−2 ≠ n−1 → violation"Σ 不闭"。

## 七、改动面清单与验收

**新增**：`l5/pipeline_model.h`（PipelineStep/PipelineModel/RenderPipelineModel + DryRun 主链——落 l5/ 非 inc/：需 include verb_capture（后端注册面触发静态注册）+ segment_orchestrator（alg_param.h 的 ThreadHandle 面），与 verb_capture.h 同级同理由）；`l5/semantic_checker.h`（核心纯函数+便捷件）。
**改动**：`l5/verb_capture.h` 提取 CaptureVerbCalls（CaptureVerbSegment 改委托，行为零变化）；测试三入口新增断言（DryRun 手算锚三组 + checker 五锚全 PASS + 反例三件 FAIL + #22 主锁）。**DryRun 测试落点 = st_verification.cc 新 R17 段**（AICPU 主场景 + CCU 锚同文件可得；编排器池合成复用 orch_test :154 MakeDualDomainRes 同型手法——threads[i]=0x1000+i）。
**不动**：编排器/后端/配方/l1_adapter 全零改动；**跨线打卡 = 无**（本轮全部 unified 树内）。

**验收（2026-10-13 实测收尾）**：三入口 0 failed；断言基线 1873 → **1920**（run.sh 533 不变 / st_checker 943→**977** / exec 397→**410**——st +34（宏关可达面：AG/RS mesh 锚+CCU 锚+反例①②③+DryRun ①②）/ exec +13（宏开 env 开 NHR 半边：16P 29 steps+锚5+主锁+NHR 10 动词+checker；env 关跑法 R17 段 skip 门卫）。既有断言零修改（R13-⑥/R15-③/R16 表/T1-T7 全原样）。**实施期新发现 4 项**（登记）：① st_checker 宏关编译 → AICPU NHR 断言不可达，拆 exec 入口（灰度矩阵既有结构，非新增编译目标）；② exec run.sh 同一二进制 env 开/关两遍跑 → R17 段 env 关遍加 skip 门卫（该形态 NHR 走 mesh 分支是 R15-④ 锁定的预期行为，不虚报 FAIL）；③ checker 四处实现期修正（READ src 几何核对补——符号 dst 反查会吞错位；CCU 全槽折叠前置形态扫描；NHR 上料/归位序位判定——tier 半片形态值域歧义（ForChunk 后 inBase/outBase/SB 三基址重合，上料 r=0 与归位逐字段相等）；NHR CCU 载荷不在 VerbCall（序列化器段级补写）存在性判据放宽）；④ AG AICPU 探针确认 VIA_CCUBUF 上料 COPY 的 dst=ScratchOffset(me)（M0 §五-1 表内符号笔误已对齐）。
