# R14 · M0 考古——A 形态执行期并发解锁（AICPU 消费方编排器）

> 2026-10-10 落盘。挂账 #6：R9 "执行期并发解锁挂 P3" 兑现——AICPU 半边的编排器三件事
> （①派发异步化按域绑线程组 ②in-flight 集维护 ③合取屏障，方案文档 :1587）。
> 裁决来源：任务书预置五条 + R9/R12/R13 资产。行号 = 本仓实数（ziliao/hccl/hcomm
> 老仓引用单独标注）。必读：aicpu_consumer.h / hcomm_verb_sink.h / tier_expand.h。

## 一、必答① AICPU 消费方现状（串行 for 的结构考古）

| 项 | 现状（行号实数） | 结论 |
|----|------------------|------|
| 段循环 | aicpu_consumer.h:402 `for (segIdx < view.frames.size())`，循环体 = Instantiate(:419/:428) → ActionSpec 组装(:439-456) → BuildArgs(:467/:476) → backend Emit(:474/:477) → trace 重放(:487-493) | R9 裁决"消费方串行"实证：**段间无任何编排**——隐式全序（for 的程序序） |
| 每段资源 | :458 `HcommVerbSink segSink(res)` 逐段新建 sink；res=**全局束**（threads 全池+通道全表，循环外 :359 形参） | READ 计数按段清零（sink 新建）；线程=全池共享——串行下复用正确，**并发下两域段会抢同一线程**（ThreadForRead 从 threads[0] 起算） |
| sync 位 | :455-456 重建 ActionSpec 时**硬编码 preBarrier=true/postBarrier=true** | v4 blob 段帧不承载 sync 位——消费方无从知道段间屏障语义（见 §四） |
| 段间同步 | 无显式屏障；串行 for 的隐式次序 = 屏障语义（段 k 全部动词下发完才进段 k+1） | 编排器要把它升级为**显式批窗 + 合取屏障** |

## 二、必答② resCtx.threads 现状（并发线程预算的硬约束）

| 项 | 现状（行号实数） | 结论 |
|----|------------------|------|
| AG 计数 | backend_aicpu_collect_all.h:42 `threadNum = N-1`（main+从，main 承载首条 read——ThreadForRead :422-436 实证 threads[0] 起算） | 域 d 需 **N_d − 1** 条线程 |
| RS 计数 | backend_aicpu_reduce_slice.h:53 `threadNum = N`（main 不发写，WRITE 用 threads[1..N-1]——ThreadForWrite :453-467 偏移 1） | 域 d 需 **N_d** 条线程 |
| 双域申请 | action_spec.h:213 `slaveThreadNum = max(...)`（ResourceClaim 求和取峰值——**串行复用假设**）；shim 面 AlgResourceCtxSerializable.threads 单 vector（fake_alg_param.h:147-152） | **双域并发需 Σ need[d] 条，资源期只给 max(need[d])**——16P 4×4 双域需 4+4=8，实给 4。预置裁决⑤（用既有池/不够降级）是**必然主路径**而非兜底；资源申请翻倍（改 SumRes 语义）动既有行为，超本轮范围，登记 P2 |
| 通道表 | res.channels = map<peer, vector<ChannelInfo>>（MakeHcommVerbRes :74-84 单层形态） | 按 peer 查表——两域 peer 集不相交，**通道天然按域分**，子束共享全表即可，无需切分 |

## 三、必答③ hcomm 线程 notify 原语面（合取屏障的机制考证）

| 项 | 证据（行号） | 结论 |
|----|--------------|------|
| 签名 | hcomm_primitives.h:181 RecordOnThread(thread, **dstThread**, dstNotifyIdx)——record 到**目标线程**的槽；:191 WaitOnThread(thread, notifyIdx, timeout)——thread 上等**自己**的槽 | notifyIdx 是"目标线程的信号槽编号"，槽空间**按线程独立** |
| 两段序 | DoSyncPre（hcomm_verb_sink.h:354-381）：先 record 全下发（main→sub idx 全 0）再 wait 全下发（sub 各自）；DoSyncPost（:385-413）：先 wait 全部（main 等 idx 0..S-1）再 record 全下发（sub→main）。真源 alg_data_trans_wrapper.cc:1010-1078 同序 | **照抄两段序**（任务书预置裁决②）——不做逐对 record-wait 交错 |
| 语义考证·异步下发 | PostSyncInterThreads"先 wait 全部后 record 全部"若 host 阻塞，第一循环 wait(main,0) 等 sub0 的 record（在第二循环才下发）→ **死锁**；存量在产（RS mesh KernelRun 每轮 :124/:130） | 反证 hcomm 线程 notify = **异步下发**（指令进目标线程队列，host 调用即返回）；host 阻塞等待另有 HcommThreadJoin（RS 64bit 分支用） |
| 语义考证·计数信号量 | 存量 KernelRun 每轮循环复用同一 idx 对（main→sub 恒 0）；若信号 sticky（record 一次后续 wait 恒过），第二轮屏障失效即存量错 | 反证 notify = **V/P 配对的计数信号量**（record=V，wait=P）——槽在配对完整后可**无限复用** |
| **下发者模型（复核 A 项定稿）** | wrapper.cc:1033-1074 四循环均在 **host** 的 KernelRun 路径直调（for 循环 + CHK_RET 即 host 顺序）；"OnThread" 形参 = 指令落哪个线程**队列**，非"由该线程发起"；host 阻塞等待另有 HcommThreadJoin（hcomm_primitives_dl.h:88，RS 64bit 分支 ins_temp_reduce_scatter_mesh_1D.cc:137）| 全部屏障指令（含批尾交叉对 4 条）**host 单线程顺序下发**——record 先 wait 后无死锁面；无"t4 自发 record 时 t0 未等到"的时序障碍（WP-3 断言锁：交叉对 4 条在桩记录里连续出现且 record×2 前 wait×2 后） |
| idx 分配 | main→sub 全 0（ins_temp_scatter_mesh_1D.cc:78-86 GetNotifyIdxMainToSub）；sub→main 0..slaveNum-1（:88-96 GetNotifyIdxSubToMain） | 单 main 对全 sub 的既定分配 |
| 双域撞 idx？ | 域组**各自封闭**：域 0 组 {threads[0..a]}（main=threads[0]）、域 1 组 {threads[a+1..b]}（main=threads[a+1]≠全局 main）；两组屏障的目标线程不重叠 → **槽空间独立，不撞**。跨域合取对用**已回零槽**（域内 Post 的 V/P 配对完成后槽 0..S-1 计数回零）复用槽 0——计数信号量语义下安全（§三上条） | 双域并发**不需要**两个全局 main 也不会撞槽；跨域合取走"域 mains 互 notify"（§六组 1 手算） |

## 四、必答④ A/B 段序列 sync 位数据流（协议面缺口 + 方案）【范围澄清，随 M0 落盘消息上报】

**结论：数据流断裂——sync 位与 TierParams 都不在 AICPU 消费方可达面。**

| 载体 | sync 位 | TierParams | AICPU 可达？ |
|------|---------|-----------|--------------|
| v3/v4 段帧（32 槽） | **无槽**（U_V3_* 槽 1..15 布局无 sync；消费方重建硬编码 T/T :455-456） | 无（R9 裁决记录 action_spec.h:299-305"host 侧 Plan 对象携带"——AICPU TS 进程**无 Plan 对象**，blob 是唯一载体） | 是（D2H 直取） |
| v5 动词帧 | **隐式承载**（SYNC_POST 帧有无 ↔ postBarrier，verb_serializer.h:236-242 NHR 分支两/三帧） | 无 | **否**（AICPU 引擎不进 v5——hccl_alloc_ctx_res.cc 注） |
| 展开 | ExpandTieredSegments 产出位正确（A：pre 全 T、post 批内 F 批边界 T；tier_expand.h:330-334；ST 锁存 expand_test:821-827） | — | **无消费方调用**（R13 只做格式级验证；运行时激活未开） |

**方案（给方案，本轮不落地）**：方案甲 = **骨架段 0 帧未用槽 16..24 承载 TierParams**（槽 16=hasTier 标记 1，槽 17..24 = threshBytes/partRatioNum/partRatioDen/bwFastNum/bwFastDen/bwSlowNum/bwSlowDen/alignBytes 九值）。优于任务书两候选：v5 加槽（AICPU 不走 v5 ✗）、blob 头扩展（升版本动 CCU 三方互锁 ✗）；甲不升版本、CCU 写入方/server 只读槽 0..15（16+ 透传或忽略零影响）、AICPU 路径 D2H 直取 host 序列化产物不经写入方改写。非 tier Plan 槽 16..31 恒 0（现状）→ 槽 16==1 天然判别。
**不落地理由**：A/B 展开的 **nhr 域段 AICPU 执行面未交付**——AicpuReduceSlice::Emit 无 NHR 分支（backend_aicpu_reduce_slice.h:61-131 按 mesh 写侧序发 WRITE×(N-1)，NHR 段建链集通道 5 条对 7 peer 必 ChannelOf Fail）——TierParams 落地 = 数据可达而执行不可达，A 形态批内 nhr 段必失败。承载落地应与 NHR AICPU 执行面（P2）同轮交付才自洽。**本轮编排器以直接段序列输入验证并发语义**（测试直调 ExpandTieredSegments 产段喂编排器——任务书 WP-3 时序断言形态）；消费方 env 分叉面向"blob 可达段序列"（骨架重建 T/T → 编排器批批单段 = 串行等价安全灰度）。

## 五、必答⑤ 串行→并发等价判据设计

- **并发段独立性证明**（A 形态批内）：批内段分属不同域 → ①通道集不相交（域 peer 集不相交，ChannelOf 按 peer 查表）；②数据区间不相交（part0/part1 = ForChunk(0,2)/(1,2) 切半，inBaseOff/outBaseOff 推进不重叠）；③线程组不重叠（§二分配）。三不相交 ⇒ 批内任意交错序语义等价。有数据依赖处（批间，A 形态批 2 的 S(P0) 依赖批 1 的 F(P0)）由 sync 位强制串行（批尾合取）。
- **判据三段式**（R12/R13 合并键模式扩展到编排层）：
  ① 线程绑定断言：段 k 的每条数据动词的 thread 参数 ∈ 其域线程组（并行执行的地基）；
  ② 屏障位置断言：批尾合取指令（域 mains 互 notify 对）在批内全部数据动词之后、下批首动词之前（下发序可判）；
  ③ 搬运等价：剥去编排器插入的屏障指令后，**逐段动词子序与串行版该段动词序逐条相等**（段内保序）+ 全局数据动词 **multiset 相等**（合并键 = (verb, src, dst, size, peer)——R12-4 合并键的动词序形态，owner=peer 字段天然在 VerbCall 内）。

## 六、必答⑥ 三组手算（验收锚——原语派发序逐条）

**构造**：16P 双 mesh 域（域 0 ranks{0,1,2,3}、域 1 ranks{0,4,8,12}，均 RS mesh 写侧、me=0、FP16）；A 形态 4 段（ExpandTieredSegments A 分支产出：seg0=域0 P0 / seg1=域1 P1 / seg2=域1 P0 / seg3=域0 P1；sync：pre 全 T，post=[F,T,F,T]）；线程池 8 条 t0..t7（t0=全局 main）；域 0 组={t0,t1,t2,t3}（RS 需 N0=4）、域 1 组={t4,t5,t6,t7}（N1=4）。半片 = slice/2 记 S/2，全片 stride 记 S。

**组 1 · A 交错 4 段并发编排时序**（下发序，桩粒度=hcomm 原语调用；**四条交叉对全由 host 单线程顺序下发**——§三下发者模型，record 先 wait 后无死锁）：
```
批1:
  seg0(域0,P0,post=F): DoSyncPre域0[t0 rec t1,t2,t3 idx0 ×3; t1,t2,t3 wait idx0 ×3]
    WRITE×3(t1,t2,t3 通道ch(peer1,2,3), src=in+stride*peer, dst=对端cclBuff me槽, S/2)
    (post=F 无段内POST) → 编排器补域0收尾Post[t0 wait idx0,1,2; t1,t2,t3 rec t0 idx0,1,2]
  seg1(域1,P1,post=T): DoSyncPre域1[t4 rec t5,t6,t7 idx0 ×3; t5,t6,t7 wait ×3]
    WRITE×3(t5,t6,t7) + DoSyncPost域1[t4 wait idx0,1,2; t5,t6,t7 rec t4 ×3](段自带)
  批尾跨域合取(编排器,先record后wait防御序):
    t4 rec t0 idx0; t0 rec t4 idx0; t0 wait idx0; t4 wait idx0   ← 4条交叉对
批2:
  seg2(域1,P0,post=F): DoSyncPre域1[t4 rec t5,t6,t7 idx0 ×3; ...](槽已回零可复用,§三)
    WRITE×3(t5,t6,t7) + 编排器补域1收尾Post[t4 wait 0,1,2; t5,t6,t7 rec t4 ×3]
  seg3(域0,P1,post=T): DoSyncPre域0[...] WRITE×3(t1,t2,t3) + DoSyncPost域0[t0 wait 0,1,2; rec ×3]
  批尾合取: t4 rec t0 idx0; t0 rec t4 idx0; t0 wait idx0; t4 wait idx0
```
每段 WRITE 后的 COPY+REDUCE×(N-1)（backend_aicpu_reduce_slice.h:105-130 写侧序第四段）在 WRITE 与补 Post 之间下发（COPY t0、REDUCE×3 t0 主线程）。**关键断言点**：①seg0 数据动词 thread∈{t0..t3}、seg1∈{t4..t7}（组不重叠）；②交叉对夹在两批之间；③批1 域0/域1 的屏障指令互不涉及对方线程（域内封闭）。
**组 2 · 单域退化**（全段同域，如 blob 骨架 2 段同域重建 T/T）：D=1 无并发窗——每段独立批，批=段（post=T 自带 Post），无交叉对、无补 Post；输出 = 串行 for 逐条相等（含每段 sink 重建的计数清零语义）。
**组 3 · 线程不足降级**（两形态）：①双域 Σ 超池（threads=4 < 需 8）：degraded=true + WARNING（"threads 4 < required 8"）+ 逐段全局束串行——输出与串行 for 逐条相等；②**单域预检超池**（N=8 域 threads=4——复核 B 项改写）：need[d]=8 > 池 4 时**预检降级**（非"硬跑透传"——sink 的 "exceeds thread count" Fail 在动词越界条目才触发，半途 Fail 留悬空动词；预检降级走串行 = 既有 for 循环的 Fail 语义与时机完全一致，既有行为零变化原则）。预检在切池前对逐域 need[d] vs 池大小判定，命中即整序列串行。

## 七、实施规格汇总（WP-1..3 契约锚）

| 项 | 定稿 |
|----|------|
| WP-1 编排器 | 新文件 **unified/l5/segment_orchestrator.h**（l5=引擎头消费面：裸调 HcommThreadNotifyRecord/WaitOnThread 做屏障，与 hcomm_verb_sink.h 同级理由）。接口：`OrchestrateSegments(segments, globalRes, driver, out)`——segments=ActionSpec 序列（含 sync/domain）、driver=SegmentDriver 闭包（消费方注入：Instantiate+BuildArgs+Emit 进域子束 sink）、out=OrchestratorResult{ret/err/segmentsRun/batches/degraded/warnings/perSegTrace}。域分组=domain.ranks 序列相等去重（首现序）；线程预算=§二公式（AG N-1/RS N 取域内 max）；批划分=扫 postBarrier（post=T 段收批）；批内 post=F 段下发后**补域内 Post 两段序**（DoSyncPost 同构裸原语）；批尾 D≥2 发**域 mains 互 notify 交叉对**（record 先 wait 后——§三异步语义下任意配对序安全，取双语义防御序）；不足降级=逐段全局束串行+warnings（不用 HCCL_* 宏——shim 面无定义，warnings 向量承载，真链路 P2 由 exec 薄壳转日志） |
| WP-2 接线 | aicpu_consumer.h：ConsumeUnifiedPlanAicpu 在 backend peek 后加**单点分叉**——env 开（UseUnifiedConcurrencyPath）→ 重建段序列+driver lambda 调 OrchestrateSegments；关 → 既有 for 循环**一行不动**。重建复用 InstantiateSegment/InstantiateRsSegment（单源）；分叉分支的重建与循环体重建的漂移由 WP-3 T5 对拍断言锁——**常跑断言**（每次测试跑 env 开/关两路输出逐条相等，复核 C 项形态，非一次性验证） |
| 灰度 | unified_route.h 追加 UseUnifiedConcurrencyPath()：宏 MC2_UNIFIED_CONCURRENCY_RESOURCE（缺省关=编译期零变化）+ env MC2_USE_UNIFIED_CONCURRENCY（缺省关），与 VERB 家族同式三层 |
| WP-3 测试 | exec 新文件 poc_unified_orch_test.cc（桩机制同 exec test：fake driver + hcomm 桩记录）：T1 批划分/T2 域映射/T3 降级/T4 组1 时序 golden/T5 等价对拍/T6 负例（空序列/尾批无 post/T/单域超线程 err 透传）。st_verification.cc R14 节（节 58 顺延）：真 ExpandTieredSegments A 段喂编排器批结构+组 2/3 golden+合并键等价+env 三态探针+防退化（env 关串行零变化）。预期 +60~70 断言 |
| 基线 | 断言只增不减：1559（run.sh 519 / st_checker 855 / exec 185）→ 预期 ≥1619 |
| 上报项 | ①TierParams 承载方案甲给方案不落地（§四，随落盘消息上报）；②双域资源申请 max 语义使真机 threads 恒不足（§二）——并发真机收益需资源申请翻倍（P2 登记）；③hcomm notify 异步/计数信号量两语义系考古反证（§三）——真机复核挂 P2（打印线程队列时序验证） |
