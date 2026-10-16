# R17 任务书：PipelineModel/DryRun + 语义后置条件 checker（两件同轮）

> **运行模式**：单 agent 全包（考古+代码+测试）。质量机制 = M0 手算落盘 + 主会话源码级复核验收锚。
> **上下文纪律**：你被拉起就是为本轮两件全部完成。若接近上下文上限，**写满断点文件后停止**，主会话用 SendMessage 复活你续推。
> **前置依赖（R16 已交付）**：kAicpuVerbMap.thread 线程策略列 + ThreadFor 表驱动（verb_sink.h / hcomm_verb_sink.h）——DryRun 的 threadId 列直接查该表。

---

## 〇、铁律（先读，违者整轮打回）

1. **断言只增不减：基线 1873**（run.sh 533 / st_checker 943 / exec 397）。三入口 `0 failed`。
2. **REVIEW_CHECKLIST 26 条全部适用**，尤其 #22 分叉分支与循环体单源关系常跑对拍锁（DryRun 是"第三条路"——与串行 for/编排器两路输出的对拍锁同型扩展）、#25 线程策略表实一致常跑锁、#16 单源派生纪律（DryRun 的通道序推导不得自建第二份）。
3. **测试期望值独立手算**，不转抄生成方。M0 六必答（§四）答完落盘 `unified/test/M0_R17_PIPELINE.md` 后**发消息请求复核**，复核通过才动代码。
4. 源码考证给**文件+行号**证据；默认不 commit/stash/checkout；大输出先过滤。

---

## 一、两件及耦合关系

| # | 事项 | 一句话 | 落点 |
|---|------|--------|------|
| ① | #9 PipelineModel/DryRun | 纯函数链 Compile→(tier)Expand→Orchestrate→Emit→Capture 产一张**流水推导表**（steps[seq/batch/domainId/threadId/verb/几何]+barriers+claim）——各层可单独 dump、不漂移 | 新文件 `unified/inc/pipeline_model.h`（纯函数，零引擎依赖面内）+ 消费入口挂 L1（DryRun(plan) 便捷件）+ 测试 |
| ② | 10a 语义后置条件 checker | 动作符号执行：沿 VerbCall 序列+DataLayout 检查 output 每片是否被正确来源覆盖（AG: out[i]==in_peer[i]；RS mesh: out[me]==Σ in_peer[me 片]）——补"新组合无 golden 基线"验证缺口 | 新文件 `unified/inc/semantic_checker.h` + 挂钩 Compile 后（与 Validate :198 同点位）+ 测试 |

**同轮理由**：同基建（两者都消费"Plan 纯数据 + 动词序列"——①的 Emit 捕获产物正是②的输入）；①给"看得见流水"，②给"证明流水对"——用户四类设计债的第四类（流水推导模型）与 MSCCLang 对照 10a 一次性兑现。

**scope 死线**：10b 无死锁论证（零代码，M0 里写论证节即可）、10d 引用最新性断言（顺手评估不强制）、AG NHR AICPU（仍不管）。

---

## 二、① PipelineModel/DryRun（积木已考证，R16 探查留档）

**积木签名速查**（R16 探查实证，实施前复核行号漂移）：

| 积木 | 位置 | 签名要点 |
|---|---|---|
| Compile | l2/l1_adapter.h:172 | (OpParam, topo, hierarchyRanks, algConfig) → Plan+err |
| ExpandTieredSegments | inc/tier_expand.h:291 | (skeleton[2], TierParams, totalBytes) → out 段序列（A 4 段/B 2S 段，sync 齐全） |
| OrchestrateSegments | l5/segment_orchestrator.h:154 | (segs, OrchestratorRes, driver 闭包) → OrchestratorResult{batchTail/domainOfSeg/barriers/perSegTrace/degraded}——**切池在内部 :269-279，无显式段→线程子组映射输出（本轮新增投影）** |
| 四 backend Emit | l4/backend_*.h | Emit(ActionSpec, argsRaw, EmitCtx{sink}) |
| CaptureVerbSink | l5/verb_capture.h:27 | 记完整 VerbCall（含 nhr 11 标量载荷） |

**PipelineModel 结构（定稿形态）**：

```cpp
struct PipelineStep {
    u32 seq;          // 全序号
    u32 batch;        // 编排批号（串行路径恒 0）
    u32 domainId;     // 域号（GroupDomains 产物）
    u32 threadId;     // 池内槽位（ThreadPolicyOf 查表推演：MAIN→0/COUNTER_OFF_n→计数器+n/
                      //   BY_CHANNEL→channelIdx——DryRun 不真驱动，纯表推演）
    VerbCall::Verb verb;
    u64 src, dst, size;   // 几何（段内相对量，与 Emit 产物同口径）
    u32 segIdx;       // 源段号
};
struct PipelineModel {
    std::vector<PipelineStep> steps;
    std::vector<u32> batchTail;             // 各批尾段（OrchestratorResult 转录）
    std::vector<BarrierRec> barriers;       // 屏障记录（域内补 Post/跨域合取）
    ResourceClaim claim;                    // Plan.claim 转录（资源画像）
    bool degraded;                          // 线程不足降级标记
};
```

**DryRun 管线**：`DryRun(plan, engine, msgLike 现值注入面)` → ①段序列（tier 骨架先 Expand——tierExpanded 非 AICPU 不可达的形态按 Plan.tier.hasTier 分叉）→ ②OrchestrateSegments（driver 闭包 = Instantiate*/BuildArgs+Emit 进 CaptureVerbSink，**复用 aicpu_consumer.h 的 DriveSegmentViaSink 同型件或提取公共件**——注意 #22 单源纪律：若提取公共件，串行 for/编排器/DryRun 三路必须同源）→ ③逐 step 查 ThreadPolicyOf 推演 threadId → ④转录 barriers/batchTail/claim。

**dump 面**：`RenderPipelineModel(model)` 文本表（seq/batch/dom/thr/verb/几何 一行一步）——各层单独 dump = 各积木自带函数；DryRun 全链 dump = 本件。与 §2.7 人肉推导形成手工/机器两版对照。

**测试锚（手算）**：8 卡 AG sole[mesh]（14 动词：COPY×1(或2)+SYNC+READ×7+SYNC——以 Emit 实产物为准手数）；16P tier A 形态（4 段 2 批：批1=mesh P0∥nhr P1、批2=nhr P0∥mesh P1——threadId 列按域切池手算：域0 base=0..2、域1 base=3..4 的 16P 实值）；RS NHR（10 动词恒定）。

**等价锁（#22 同型扩展）**：DryRun 的 driver 路径与既有 ConsumeUnifiedPlanAicpu 串行/编排器两路的 perSegTrace/calls 逐条相等——常跑断言（exec 入口新测试）。

---

## 三、② 语义后置条件 checker（10a）

**挂钩点**：L1Adapter::Compile 内 Validate（:198）之后新增可选拓扑（`SemanticCheck(plan, engine, err)`——默认编译期宏+env 开关形态与 unified_route.h 灰度族同式，**默认关**=零行为变化；或独立入口由测试/诊断显式调——M0 六必答裁定形态）。

**语义模型（动作级符号执行）**：
- 沿段序逐动词维护"符号内存"（region 标签：input/output/scratch/remote——按 DataLayout 访问器值域判定：inBaseOff..+stride*n=input 域、outBaseOff..=output 域、scratchBaseOff..=scratch 域）。
- 动词语义表（复用 kAicpuVerbMap/kCcuVerbMap 的 target/note 注记作真值来源声明）：
  - COPY(src→dst,size)：符号拷贝；
  - READ(peer,src,dst,size)：out 域 [dst,dst+size) ← peer 的 input 域 [src,src+size)（AG 后置条件：out 片 == 对端 input 同位片）；
  - WRITE(peer,src,dst,size)：对端 cclBuff 槽 ← 我的 input 片（远端基址符号变量——region=remote）；
  - REDUCE(src→dst,size)：dst ← dst ⊕ src（RS 后置条件：out[me] == Σ_peer in_peer[me 片]——初始 COPY 落位 + 逐 REDUCE 累积后校验闭包）；
  - NHR_RUN：CalcNhrSteps 重算步几何展开成逐 slice READ/WRITE 序（纯函数已存在）；
  - SYNC_*：栅栏（对数据流无影响，校验序不越屏障读未写区——可选增强）。
- **后置条件判定**（collective 语义闭包，M0 写清每 kind 的判定式）：CollectAll：∀r∈域 out 域 r 片 == in 域 r 片（我的 COPY + 逐 peer READ 覆盖全域且来源正确）；ReduceSlice：out 我的片 == Σ 全部 input 我的片（COPY 落位 + REDUCE×(N-1) 累积）。
- **已知边界（M0 显式声明，不做不吹）**：MESH AG/RS 全覆盖；NHR 经步展开覆盖；WRITE 远端基址是符号变量（跨 rank 数据流靠"对端槽位约定"符号化——不验证对端消费序）；两引擎混排 Plan 不校验引擎间等价（那是 ST checker 已有职责，本件只校验"单 Plan 的后置条件"）。

**与既有件的分工**：Validate 查可支持性（拓扑/dtype/能力）→ 本件查语义正确性（数据流闭包）→ ST checker 查两引擎等价 → golden 查旧实现基线。四层各管一段，本件补的是"新组合无基线"的空档（MSCCLang B1 对照结论）。

**交叉锚**：既有 ST golden 用例（st_verification R13 NHR golden/R15 59 系）的 Plan 输入喂本 checker 必须全 PASS——checker 若在既有 golden 上 FAIL 即 checker 错（这些组合有旧实现基线背书）。

**测试锚（手算）**：构造性正例（AG/RS 各一全 PASS）+ 病灶反例（① 删一条 READ → AG 覆盖不闭 → FAIL；② READ src 几何错位 → 来源不正确 → FAIL；③ REDUCE 少一次 → Σ 不闭 → FAIL）——反例是本件的存在意义（"改回也过"防线，REVIEW_CHECKLIST 流程护栏 4 同型）。

---

## 四、M0 六必答（答完落盘 M0_R17_PIPELINE.md 请求复核）

1. **①的 driver 单源方案**：DryRun 与 aicpu_consumer 两路（串行 for/编排器）的 BuildArgs+Emit 关系——提取公共件还是闭包复用 DriveSegmentViaSink？给出改动面与 #22 对拍锁扩展点。
2. **①的 threadId 推演口径**：BY_CHANNEL 在 channelsPerRank>1 时的推演（不真驱动怎么知道 channelIdx 循环边界）；串行路径（无编排）batch/domainId 的取值约定（恒 0？）。
3. **②的挂钩形态裁定**：编译期宏+env 灰度（默认关）vs 独立诊断入口——给推荐与理由（考虑：真链路编译期跑的代价 vs 测试期跑的覆盖）。
4. **②的区域判定规则**：DataLayout 值域重叠时的优先序（input/output/scratch 基址可同址——SYMME 形态 in==out）；WRITE 的 remote 符号变量命名约定。
5. **三组手算**：AG 14 动词表（verb/geometry/threadId 逐条）；16P tier A 形态 4 段批表（域切池实值）；RS NHR 10 动词 + 步展开首步的 slice 序。
6. **②的既有 golden 交叉锚清单**：哪些既有用例的 Plan 可复用（R13 NHR 三组/R15 59 系/R10 ② AG）——列出喂入方式。

## 五、验收

- 三入口 0 failed，断言只增不减（新基线 = 1873 + R17 新增数，收尾实测记录）。
- DryRun 等价对拍（三路 perSegTrace/calls 逐条相等）常跑锁落地。
- checker 病灶反例三件全 FAIL（"改回也过"防线）。
- M0 文档落盘 + 跨线打卡（若挂钩 Compile 动 l1_adapter.h——打卡声明）。
