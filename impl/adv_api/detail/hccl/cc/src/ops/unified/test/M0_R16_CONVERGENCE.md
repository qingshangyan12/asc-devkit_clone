# M0_R16 · 设计债收敛轮（单 agent 全包）

> **轮次定位**：落地用户 2026-10-12/13 提出的代码设计意见闭环（四类设计债中的生产码收敛件）。
> **交付六件**：#7 线程策略表驱动（含 8g）、8a 建链集单源、8b stride 访问器、8c GroupDomains、8f 骨架 helper、#6 claim.ccu 公式补全+注册对拍。
> **统一模式**：单点声明 + 派生实现 + 断言锁。零新功能；除 #6 申报值与新增 WARN 面外零行为变化。
> **基线**：断言只增不减 1843 → 1873（run.sh 519→533 / st_checker 927→943 / exec 397→397），三入口 0 failed。

---

## 一、六件交付四栏表（现状 → 单点声明 → 调用点 → 断言锁）

### ① #7+8g 动词线程策略表驱动

| 栏 | 内容 |
|---|---|
| 现状 | 策略散在 hcomm_verb_sink.h 七个取线程点：DoRead:197（计数器 off0）/DoCopy:254（main）/DoWrite:282（计数器 off1）/DoReduce:348（main）/DoNhrRun:426（threads[channelIdx]）/DoSyncPre:540+DoSyncPost:571（main+全 sub）。Emit 调用点不可见——用户点名"两个 COPY 哪个并行哪个串行看不出来" |
| 单点声明 | verb_sink.h `ThreadPolicy` 枚举（MAIN/COUNTER_OFF_0/COUNTER_OFF_1/BY_CHANNEL/MAIN_PLUS_SLAVES/NOT_WIRED/NA）+ `VerbMapEntry.thread` 列：kAicpuVerbMap 九行各填（COPY/POST_COPY/REDUCE=MAIN、READ=COUNTER_OFF_0、WRITE=COUNTER_OFF_1、NHR_RUN=BY_CHANNEL、SYNC×2=MAIN_PLUS_SLAVES、BROADCAST=NOT_WIRED）；kCcuVerbMap 全 9 行=NA（CCU 微码自驱动无线程）。便捷件 `ThreadPolicyOf(verb, isCcu)` |
| 派生实现 | hcomm_verb_sink.h 新增 `ThreadFor(verb)` 统一查表入口 + `ThreadByCounter(countVerb, offset, name)` 计数器策略唯一实现点；六个 Do* 取线程行改走 ThreadFor；MainThread 保留（MAIN/MAIN_PLUS_SLAVES 的底层）。**边界写进表头注释**：表不管池切分（编排器）、不管 notify 槽位策略（sink 实现层，登记 R17 复核） |
| 断言锁 | poc_unified_verb_sink_test R16 段 14 断言：表列声明逐动词核对（9+1 条）+ 同一 sink 混发动词的 thread 字面量对拍（COPY/REDUCE→threads[0]、READ 首条 threads[0] 次条 threads[1]、WRITE 首条 threads[1] 且与 READ 计数独立）；st_verification R16-#7 三条交叉 |

### ② 8a NHR 建链集单源回收（最讽刺项）

| 栏 | 内容 |
|---|---|
| 现状 | 单源函数 CalcNhrConnectRanks（nhr_steps.h:91）生产零调用；两处内联逐字镜像各 22 行：action_reduce_slice.h CalcRes（L3）与 backend_ccu_reduce_slice.h ClaimEngineRes（L4）。注：原台账"三处"记载有误——backend_aicpu_reduce_slice.h 无建链代码（探查更正；server 侧一处跨树不可收编，见下） |
| 单点声明 | CalcNhrConnectRanks 本体不动（已存在），注释升格："五端单源"改为"两端单源调用 + 注册链/VerbNhrRun 跨树独立同源（ST 传递等价锁，REVIEW_CHECKLIST #18 原文不动）" |
| 调用点 | action_reduce_slice.h:117-138 内联块 → `CalcNhrConnectRanks(me, n)` 一行 + include nhr_steps.h；backend_ccu_reduce_slice.h:64-85 同（include 经 action_reduce_slice.h 传递） |
| 断言锁 | st_verification R16-8a 两条：单源函数直测（8 卡 me=3 → {1,2,4,5,7} 手算锚）+ CalcRes(NHR) channels 产物经单源路径产出同值；R13-⑥ 既有断言（值锁）自动成为路径锁 |

### ③ 8b stride 三分支升访问器

| 栏 | 内容 |
|---|---|
| 现状 | U/ 树七处三元式：A 族 6 处（layout 形态——action_reduce_slice.h:181/:215、backend_aicpu_reduce_slice.h:89/:155/:179、verb_serializer.h:210）+ B 族 1 处（l1_adapter.h:117-118 生产者侧，输入 OpParam.strideCount 不同域） |
| 单点声明 | DataLayout::RsInStride()（action_spec.h，访问器区）——inputSliceStride 非零直用，为零退 outputSliceStride |
| 调用点 | A 族 6 处替换；B 族**不动**（注记：输入域不同，强行统一需第二签名收益低）；**plan_serializer.h:274 维持裸值只改注释**（裁决：改访问器会让 Compile 期 blob 字节从 0 变 outputSliceStride*me——协议字节零变化优先；语义由消费方弃读+设备覆写双端中性承载）；consumer 侧 3 处 msg 形态登记 P2 |
| 断言锁 | st_verification R16-8b 两条（input=0 退 output 2048 / input=4096 直用）；既有全链等价断言（Emit/序列化 golden bytes）自动锁替换零漂移——三入口全绿即证 |

### ④ 8c 域分组去重提 GroupDomains

| 栏 | 内容 |
|---|---|
| 现状 | 两处镜像：segment_orchestrator.h:170-186（R14）与 action_spec.h CalcConcurrencySlaveClaim 内（R15 复制——探查更正：第二处不在 aicpu_consumer.h）。循环体逐字同构，仅编排器多产 domainOfSeg |
| 单点声明 | action_spec.h `GroupDomains(segs, domainOfSegOut=nullptr)`——域分组唯一实现点（编排器出参形态覆盖 domainOfSeg 需求） |
| 调用点 | CalcConcurrencySlaveClaim 内联块 → 一行；OrchestrateSegments 域分组块 → 一行（`GroupDomains(segs, &out.domainOfSeg)`）；action_spec.h 过期交叉引用注释同步 |
| 断言锁 | st_verification R16-8c 两条：GroupDomains 直测（3 段 2 域 domainOfSeg={0,1,0}）+ Σ 经 GroupDomains（手算 (7-1)+(7-1)+2=14）；既有 R15-③ Σ 断言 + R14 orch 测试 T2 域映射断言自动成为路径锁 |
| 顺带登记（不实施） | aicpu_consumer.h:500-513/:581-595 myAlgRank 查找镜像（需 found 拒错变体，挂 P2） |

### ⑤ 8f tier 骨架构造提 BuildTierSkeletonSeg

| 栏 | 内容 |
|---|---|
| 现状 | aicpu_consumer.h skelFast（15 字段）与 skelSlow（16 字段）手写镜像——14 字段同源，真差异仅帧序（0/1）/域表/pattern 三自由度 |
| 单点声明 | aicpu_consumer.h 本文件内 `BuildTierSkeletonSeg(frame, domRanks, pattern, myRank, rsSliceSize, rsStride, dts, repeatNum, scratchBase)` 九参（BlobFrameView 是局部类型不下沉 inc/；NHR pattern 条件置 nhrJettyNum=1） |
| 调用点 | 两个手写块整体替换为两行调用；单源锚注释（#22 对拍）保留 |
| 断言锁 | exec 入口既有 tier payload 测试 T1-T4（A 展开 4 段/B 步进/槽 golden/#22 对拍）全部常跑——替换零漂移由它们自动锁（397 全绿即证） |

### ⑥ #6 claim.ccu 公式补全 + 注册时刻双源对拍

| 栏 | 内容 |
|---|---|
| 现状 | backend_ccu_{reduce_slice,collect_all}.h `req.event=2`（"Pre/PostSync 各一"）是误注——Pre/PostSync 走通道级 Notify（CKE_IDX_0）不占 events；kernel 真值 events.resize(UNROLL×ceil(N/16))（RS_UNROLL_NUM=16、AG_M2M_UNROLL_NUM=1、BIT_NUM_PER_CKE=16，两 kernel InitResource 实证）。dieId 恒 0。真值链 AccumulateKernelRes 只在 v2 路径，unified 注册链无反查 |
| 公式补全 | 两 CCU backend `req.event = UNROLL×ceil(N/16)`（RS=16×ceil(N/16)、AG=1×ceil(N/16)）；variable/loop/ccuBuf/instruction 不变（instruction 仍是 R12-1 裁决⑤登记的占位项）。dieId 仍恒 0 + 注记"真值注册链反查，双域分 die 回填挂 P2" |
| 对拍实现 | ① ResourceBroker::AcquireCcuKernel 加可选参 `claimCcu=nullptr`（NoopBroker/既有面零影响）；② Materialize CCU 半边透传 `&claim.ccu`；③ HcclCcuKernelRegisterUnified 尾部（注册成功后、kernelHandle 回传前——两套数同作用域）：HcommCcuKernelQueryResReq 反查（AccumulateKernelRes 同链）→ CompareClaimVsQueriedRes 逐 resType 比对（纯函数，位掩码返回）→ ReportClaimResDrift（常规 WARN+diff / env MC2_UNIFIED_RES_DEBUG 非零 HCCL_ERROR+FAIL E_INTERNAL，static bool 首调定格与 unified_route.h:118 同式）。对拍失败不回滚注册（申报零消费方已考证）。查询链不可用时 INFO 跳过（防御，不阻塞注册） |
| 断言锁 | st_verification R16-#6 四条（RS 8P event=16/variable=26、AG 8P event=1/variable=18 手算锚）；对拍逻辑抽出独立编译单测（/tmp 探针模式）：all-match drift=0、event 漂移 bit4、WARN/FAIL 双态 env 切换验证。真机复核 P2 登记（instruction 占位项已知会 WARN——首现时核对，防狼来了由"公式先行补全"纪律缓解） |

## 二、手算锚三组（M0 必答）

1. **线程表**：池 7 条（0x1000 主 + 0x1001..0x1006）。COPY/REDUCE→0x1000；READ 第 i 条→threads[i]（首条 0x1000 主线程承载）；WRITE 第 i 条→threads[i+1]（首条 0x1001，与 READ 计数独立）。
2. **建链集**：8 卡 me=3：delta=1→{4,2}、delta=2→{5,1}、delta=4→{7,7 去重} → 升序 {1,2,4,5,7} 五条 ≠ mesh 全连接 7 条。
3. **event 公式**：RS 8P = 16×ceil(8/16)=16×1=16；AG 8P = 1×1=1。Σ 申报例（三段 AG、两域各 8 成员）：(7-1)+(7-1)+2=14。

## 三、行为变化面显式声明（唯一两处）

1. **申报值变化**：claim.ccu 的 event 字段 2→16（RS 8P 例）。消费方=零（host 单测/ST/dry-run/编译期探测四消费方只读线程族/通道族/kernels——已考证），影响面=维测 dump 数值更准。
2. **新增 WARN 面**：注册链对拍漂移时 HCCL_WARNING（常规）/HCCL_ERROR+FAIL（debug env）。真机上 instruction 项**预期会 WARN**（占位 vs 真值）——已知项非回归，R12-1 裁决⑤原登记。

## 四、跨线打卡

- **op_common.cc**（unified 树外）：新增 CompareClaimVsQueriedRes/ReportClaimResDrift 两静态函数（:3049-3116 区域）+ HcclCcuKernelRegisterUnified 尾部对拍块（:3323-3350 区域）+ 签名加可选参 claimCcu。理由：对拍落点必须在"注册真值可反查"的作用域（GetUnifiedOpParamResCtxImpl 经 broker 透传到此）。
- **hccl_alloc_ctx_res.cc**（unified 树外）：UnifiedResourceBroker::AcquireCcuKernel 签名加可选参 + 转发 + 前向声明同步。理由：claim.ccu 从 Materialize 到注册链的透传通道。

## 五、验收记录

- 三入口实测：run.sh **533**（519+14）/ st_checker **943**（927+16）/ exec **397**（+0，既有 tier/orch 测试自动锁 8f/8c 替换）——合计 **1873 ≥ 1843**，0 failed。
- 既有断言零修改验证：R13-⑥（建链集值）、R15-③（Σ）、R14 T2（域映射）、exec T1-T4（tier）、verb_sink ①-⑧（线程字面量 0x2006/0x1001 锚）全部原样通过。
- 中途三次手算修正留痕：Σ 期望 7→22→14（第一次漏算 kind 缺省、第二次把 8 成员域当 16 卡）、CalcRes 签名（返回值非出参）——期望值独立手算纪律的自证。

## 六、遗留（登记不实施）

- consumer 侧 3 处 msg 形态 stride（8b B 族同族，P2）；myAlgRank 查找镜像（8c 顺带，P2）；dieId 分 die申报（P2，随真机双域）；对拍真机复核（P2）；notify 槽位策略收编评估（R17）；10b 无死锁论证落文档（R17）；R17 任务书=#9 PipelineModel/DryRun + 10a 语义后置条件 checker（积木签名已考证，见 R16 探查）。
