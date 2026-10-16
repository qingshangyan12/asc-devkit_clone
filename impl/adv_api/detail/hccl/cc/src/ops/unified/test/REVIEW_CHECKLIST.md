# unified 第一轮增量 · 审查用断言基线（生成 agent 不得修改本文件）

生成 agent 已领走 5 个工作包：WP1 切轮（A2）、WP2 LoopSplitter 完整版（B12）、WP3 kernel 注册链（C9）、WP4 HcclResult（C7）、WP5 C++14 合规。审查时逐项核对本清单。

## 数学断言（必须逐值手算核对，不许只看测试通过）
1. 切轮：maxPerRound = min(256MB, cclBuffSize/scratchMultiple/512*512)；loopTimes = ceil(sliceSize/maxPerRound)；逐轮 baseOff = i*maxPerRound（512 对齐后）；尾轮 sliceSize = 余量。构造 ≥3 组参数（整除/有余量/单轮）核对。PoC 对齐粒度 512 vs 真实 HCCL_MIN_SLICE_ALIGN=128（alg_template_base.h:34），P2 对齐时统一。
2. symmem 单轮：transport==SYMMEM 时即使数据 > cclBuff 约束也不切。
3. LoopSplitter 边界（对齐方案 §6.5 规格而非 CalGoSize 现状——两者差异要显式标注；**取满特例的 GetParallelParam 填 n-1（真实 ccu_kernel_alg_base.cc:57），不是 n**）：
   - 整除：n==0 && p==0 → 只走 LoopGroup0，addrOffset = memSlice*loopCount*m
   - 有余量：m/n/p 三段值手算
   - 小数据（m==0）：只走零头/尾块
   - size==maxSize：取满特例（GetMaxLoopIterNum/loopCount−1/memSlice）
   - parallelParam：GetParallelParam(n−1) 打包语义
4. AR AUTO 档阈值（8MiB）行为不变：小于阈值 one_shot（CollectAll+LocalReduce 两段）、大于 two_shot（RS→AG）——现有断言语义不得漂移。

## 结构断言
5. Plan 切轮后 segments 数 = 原 segments × loopTimes（或等价结构），chunk 与切轮叠加时段的排列语义明确（chunk 是域内行为、切轮是容量行为，两层嵌套的顺序要在注释/代码里讲清）。
6. CcuKernelPlan：kernelFuncName="CcuActionCollectAll"；channels 条数 = (domain.Size()−1)×channelsPerRank；operator+= 逐条追加（两个段求和后条数翻倍）。
7. HcclResult 映射：语法错→HCCL_E_PARA；**拓扑不适用→HCCL_E_NOT_SUPPORT（§6.9 错误分类：能力类）**；无后端/能力不支持/裁剪→HCCL_E_NOT_SUPPORT；成功路径 HCCL_SUCCESS。错误码不吞（透传），err string 保留。断言应同时校验 ret 与 err 关键词——仅校验 ret 的用例在错误来源与预期不同时会误 PASS（第一轮 st_verification 的 'P2 gap' 用例即此隐患：topoClos 下 mesh 先被拓扑校验拦截，返回值巧合同为 E_NOT_SUPPORT）。
8. 原 34 项断言语义全部保留（允许改写为新签名，不允许删减覆盖面）。

## 合规断言
9. C++14：无结构化绑定、无 if-init、无 CTAD 依赖；test/run.sh 用 -std=c++14 -Werror -Wall 且全绿。
10. 只动 unified/ 目录；未 include 引擎头（l3/l2 不出现 ccu/aicpu 头）；无设备调用。

## 设计对齐断言（对照方案文档）
11. 切轮位置：按 §6.2"逐轮推进 baseOff 的循环在 L2"——切轮应发生在 Expand 后的段上（不是 recipe 内部 hard-code），且 symmem 例外语义与 §6.2 一致。
12. LoopSplitter 保持"唯一实现点"性质：host tiling 预编排 / BuildArgs / ST 三处共用同一个 Calc（BuildArgs 的 goSize 改由新版产出）。
13. ResourceClaim 求和语义不回归：线程 max、CCU 计数 sum、channels/kernels 追加（§6.5）。

## 流程护栏增补（2026-09-21 第四轮教训，用户催办触发收作业裁决）

1. **契约分歧上报裁决**：多 agent 对同一协议（槽位/布局/枚举值）出现口径分歧时，任何一方不得与对方直接协商等待收敛——立即上报主会话裁决。第四轮 ST 与生成方就 slot1/2 等价口径（直比 vs base+offset 合成）互等 40 分钟无拍板，属无主状态协商僵局。
2. **范围变更报备**：生成方超出任务书的工作（如第四轮设备侧编译验证、include 环治理）须先报备主会话再投入——有价值但会拖垮时间估计。
3. **里程碑制替代时间估计**：交付承诺改为报里程碑（"golden-bytes 测试写完"/"回归跑完"）而非时间（"几分钟内"）。第四轮"几分钟收尾"实际拖 40 分钟。

4. **判定入口必须直接测试（第四轮 BLOCKER-1 教训）**：`compare(0,10,"CcuAction")` 前缀判定恒假——compare 第二参是子串长度非字面量长度。464 断言全绿不漏的原因：测试只测链路两端（序列化器直测+模拟器喂帧），从未以真实 kernelFuncName 走 host 判定入口。规则：凡"字符串/枚举/常量判定"类入口（前缀比较、白名单、dispatch 键），必须有以真实注册值直测该判定式的断言，且病灶反例（错误形态）保持可判防"改回也过"。
5. **字面量长度与用法脱钩用 static_assert 钉死**：`compare(0, N, "LIT")` 类代码必须配 `static_assert(sizeof("LIT")-1 == N)`（第四轮修复模板）。

## 发布流程断言（第五轮新增，2026-09-21）
13. 发版前跑三处同名登记断言：v3 协议三处（unified ActionKind 枚举 ↔ kfc_server_message.h 槽 0 常量 ↔ server DispatchUnifiedPlanV3 分支 ID）+ 158 三处（host AlgorithmType 枚举 ↔ device CCU_ALGORITHM_TYPE_UNIFIED_PLAN constexpr ↔ GetAlgorithmType 比较）+ res[1] 写读两端（组装段写入 ↔ CcuPrepareForUnifiedPlan 读取）。空 Plan 三道闸口（序列化器/组装段/写入方）跨树版本同步同此——第四轮 BLOCKER-1（三处登记断一中段断裂、两端测试各自全绿）的发布级防线，跨树改动后必跑（八套测试中 poc_unified_kfc_gate + st_verification ㉙ 覆盖）。

## 跨线改动纪律（R6 新增，2026-09-22，B0 修订事件——结案版）
14. 跨线改动必须打卡显式声明：任何 agent 修改非本线任务书范围的文件（他线交付物地盘/共享文件的非追加改动），必须在里程碑打卡中显式声明"动了哪个文件哪个区域+为什么"，或先报备主会话再动。多线并行轮次（R6 起动作×引擎双线）每线打卡必含"改动文件清单+是否含他线地盘"。
15. 改动归属排查必须用 transcript 取证，不做 mtime 关联推断；**回执时序错位是归属误报的高发源**：agent 收到新指令并执行后，旧指令的回执可能晚于新动作落盘——此时"文件里有我回执没报告的改动"不等于"他人改动"。R6 B0 案最终结论：公共线按主会话 16:25 UTC 指令完成双引擎拒收改写（transcript Edit 记录 16:26:11 与 mtime 吻合），但第一版修复的回执晚发+随后自我改写被误报为"他人后续修订"，引发对 AICPU 线的错误指认（其全程 0 写该文件）——教训：自查归属时先核对自己是否收到过更新的指令（含主会话裁决），再下"非我所为"结论。

## 通道序与等价判据纪律（R12 新增，2026-10-08，⑥⑧ 键拆分裁定存档）

16. **单源派生纪律（通道序错位的构造性防线）**：v5 动词帧的段内帧序与 server 侧通道迭代序必须从**同一 channels 数组**派生（unified plan.channels → 注册链 HcclChannelAcquire → InheritKfcServerKernelArg → ctx.arg->channels，一杆到底），禁止两侧各自独立排序（升序 vs 探测序两次推导 = 错位温床——帧序几何完全相同时数据取错 peer，任何帧内容判据抓不住）。落点三件：①序列化器（生成方 M0）帧序迭代与 server verb 分发循环的通道迭代同数组；②host Validate 在"帧数 == channelCount"之外追加**同输入双推导断言**（同一 plan.channels 输入，序列化器与模拟器各自推导帧序后逐帧对照）；③HcclChannelAcquire 的通道序保持是 host 测试不可观测的一环，登记 P2 设备侧复核（真机打印通道表 vs blob 帧序）。
17. **等价判据配对敏感性（合并键必选）**：动词序等价断言的 multiset 键必须用**合并键**（producer, consumer, srcOff, dstOff, size）——几何键（srcOff/dstOff/size）与端点键（producer/consumer/size）各看一半，配对错误（帧带 A 的几何打到 B 的通道）两键都盲；合并键把 owner 与几何绑进同一条边，批次乱序容差天然保持（multiset 语义不变）、配对敏感恢复。**CCU 侧的通道置换免疫是有前提的**：①帧不携带对端裸地址（远端基址按通道从 XN 表取、帧内 offset 恒 peer 无关——R12 裁决④机制）；②规约算子交换律（RS scratch 槽序无关，GroupLocalReduce 全槽求和）。前提任一失效（未来动词携带 peer 相关远端 offset、引入非交换规约）即免疫静默失效，合并键从"必选"升"硬性"。**host checker 能力极限**：只能证明"帧内容 ↔ checker 的 server 序模型（升序）"一致性，"server 真实通道表恰为升序"靠 16 条的单源构造保证，不靠测试——两纪律互为充要，删一即洞。反例锚：R12 ST 的 J6 READ 逆序/J7 WRITE 逆序用例在几何键下正向通过（错位形态被容差固化），是本条的直接病案。

## NHR 段协议纪律（R13 新增，2026-10-09，M0 复核修正 A/B 存档）

18. **NHR 通道表必须按建链集注册（不得按 mesh 全连接假设推导 chIdx）**：NHR 段的 chIdx(rank) 推导输入必须是 CalcNHRChannelConnect 的 connectRanks{rank±2^s} 建链集（升序去重），不是"升序跳 me"的 mesh 规则——NHR 是 log2 条链的稀疏拓扑（8 卡 me=3 建链集 {1,2,4,5,7} 五条 ≠ 全连接 7 条，±4 同 rank 去重是偶数 N 固有形态），mesh 规则直接错位（M0_R13 复核修正 A：初版按 mesh 假设写规格被主会话证伪）。落地状态（R13 已收尾）：L3 ReduceSlice::CalcRes + L4 ClaimEngineRes NHR 分支按建链集申报（st_verification R13-⑥ 断言守门）；注册链 BuildUnifiedChannelDescs 按注册名前缀分叉（"CcuVerb"→CalcChannelRequestNhrMultiJetty / "CcuAction"→CalcChannelRequestMesh1D，op_common.cc R13 注）；server VerbNhrRun 的 connectRanks 重建同公式——四端单源。混排断言的 channelCount 按段各自口径（NHR 段建链集大小 ≠ mesh 段 N−1）。
19. **jetty 公式对齐序以 v2 写入方 :507-510 为唯一基准**：sliceOneJetty = sliceSize/nhrJettyNum/sliceAlignCount*sliceAlignCount——被除数是**字节量**、先除 jettyNum 再对齐下取整；不得先转元素再转回（双截断，G1 场景 1000B 两条路差 960 vs 896，M0 复核修正 B 的实值核对项）。v5 序列化器（host Compile 期）与 CcuPrepareForUnifiedVerbPlan（设备 exec 期）两道按同式计算，槽 5 值在 jettyNum=1 时仍按公式写入（≠0）——kernel :114 循环零次不消费，但帧值是公式真值。

## A 形态并发编排纪律（R14 新增，2026-10-10，M0 复核 A/B/C 存档）

20. **合取屏障下发者模型必须显式声明（host 单线程顺序下发）**：批尾跨域合取交叉对（域 mains 互 notify）的全部指令由 host 顺序下发——"OnThread" 形参指明指令落在哪个线程队列，非"由该线程发起调用"（hcomm_primitives.h:181/:191 签名 + wrapper.cc:1033-1074 四循环均 host for 直调为证；host 阻塞等待另有 HcommThreadJoin）。若未来屏障改由线程自驱（指令队列模型），record 先 wait 后的死锁免疫即失效——届时交叉对须重论证。锁点：exec orch 测试 T4 "record×2 先于 wait" 断言。
21. **线程不足降级是预检不是硬跑**：单域 need[d] > 池时**预检降级**（整序列串行）——sink 的 "exceeds thread count" Fail 在动词越界条目才触发，半途 Fail 留悬空动词；预检降级的 Fail 语义/时机与既有 for 循环完全一致（既有行为零变化）。双域 Σ 超池同样降级（ResourceClaim 求和取 max 是串行复用假设——action_spec.h:213，双域并发需 Σ 而实给 max，真机收益需资源申请翻倍，P2）。锁点：orch 测试 T3 两形态断言。
22. **分叉分支与既有循环体的单源关系靠常跑对拍锁**：aicpu_consumer.h 的 R14 分叉（RebuildActionSpecFromFrame/DriveSegmentViaSink）是循环体的逐行提抽（Instantiate*/BuildArgs/Emit 同一函数调用，非第二实现）——两路输出逐条相等是**常跑断言**（exec orch 测试 T5，每次测试跑，非一次性验证）。改循环体重建逻辑必须同步改分叉件，否则 T5 FAIL（漂移即抓）。

## R15 三件同轮纪律（2026-10-11 新增，M0_R15_TRIPLE.md 存档）

23. **槽位协议判别式纪律（①TierParams 槽 16..24 承载）**：新征用协议槽区必须三证齐——①缺省全 0（非承载 Plan 逐槽 0 是构造性事实：序列化器清零后无人写，M0_R15 §三穷举写入路径）；②判别式唯一（槽 16==1 单判，布尔语义非计数——多档位扩槽不破坏）；③消费方零影响（CCU 写入方透传/server 只读 0..15 的双证，新槽落 HBM 物理面已预留）。三证缺一即回归"槽位碰撞静默错读"病案面。锁点：st_verification 59/59'''' + exec T1 槽 golden。Tier 级数据只写段 0 帧（Plan 级挂首段，判别式只查帧 0）——多段承载须先升判别式再扩槽。
24. **NHR 动词与七动词合取隔离纪律（②NHR AICPU 执行面）**：NHR_RUN 是自持执行面——①不动 READ/WRITE 计数器（sink ThreadForRead/Write 只数各自动词，NHR 段零 READ/WRITE，混排段计数不受扰）；②通道查表走 algRank→userRank 映射（载荷 domainRanks——步表值是 algRank、通道表键是 userRank，直查必 Fail）；③线程/通道 notify 是两个原语族（HcommThreadNotify* vs HcommChannelNotify*，槽空间不相交——R14 批尾交叉对与 NHR 步间握手无冲突）；④单线程域 no-op 门槛（threads<2 域内补 Post 直接返回——对齐模板 threadNum>1，单通道 NHR 域 need=1）；⑤既有 mesh 写侧分支一行不动（gated NHR 分支是新增 case，ungated 照旧全连接迭代=现状行为，exec T4 env 关断言锁）。锁点：exec T4/T5 + st 59'。

## R16 设计债收敛纪律（2026-10-13 新增，M0_R16_CONVERGENCE.md 存档）

25. **线程策略表实一致是常跑锁不是一次性验证**：kAicpuVerbMap.thread 列（verb_sink.h）与 hcomm_verb_sink.h 的 ThreadFor 查表行为互为镜像——改表列不改 sink（或反之）由 poc_unified_verb_sink_test R16 段（表列声明 + 混发动词的 thread 字面量对拍）FAIL 抓。策略边界：表只管"段内动词→池内哪条"，池切分（编排器域切池）与 notify 槽位策略（DoSyncPre idx 全 0 / DoSyncPost idx=i-1）不归表管——扩策略先扩边界声明。
26. **claim.ccu 公式与对拍的顺序纪律**：先补全量公式（R16 已补 event=UNROLL×ceil(N/16)，RS=16/AG=1 实证 kernel resize）再上注册对拍——反序会让对拍在"已知不完整公式"上狼来了。对拍漂移常规 WARN、env MC2_UNIFIED_RES_DEBUG 非零 FAIL；对拍落点 HcclCcuKernelRegisterUnified 注册成功后（两套数同作用域）；FAIL 不回滚注册（申报零消费方已考证）。真机复核 P2：WARN 首现时的逐 resType 核对（instruction 是已知占位项——R12-1 裁决⑤登记）。

## R17 流水模型纪律（2026-10-13 新增，M0_R17_PIPELINE.md 存档）

27. **DryRun 单源三件套**：①Emit 步 = CaptureVerbCalls（verb_capture.h 提取件——v5 生产链/DryRun/checker 三消费方同一实现，不自建第二份 Emit）；②编排步 = OrchestrateSegments 单源（批划分/域切池/barriers 全由编排器产出，DryRun 合成足额池零侵入）；③threadId = 查 ThreadPolicy 表纯推演（域 base + 段内独立计数器，与 sink ThreadByCounter 同构——不真驱动）。主锁 = DryRun 逐段动词序 == CaptureVerbCalls 直连（st 61-b/exec R17 段常跑）。改任一积木必须想 DryRun 是否跟着变（编排器/后端 Emit/线程表）——主锁+步数锚 FAIL 即抓。
28. **语义 checker 的判定式必须落在"稳定判据"上**：动词语义判定用**序位/形态**而非裸值域——①NHR 上料/归位 COPY 靠 NHR_RUN 前后序位判（tier 半片形态 ForChunk 后 inBase/outBase/SB 可三基址重合，上料 r=0 与归位逐字段相等——值域判据必歧义）；②CCU 全槽折叠 REDUCE 靠前置形态扫描（REDUCE 恰 1 且 src=scratchBaseOff 起点）而非循环序推导；③READ src 几何独立核对（符号从 dst 反查推导会吞掉 src 错位——反例②防线）。病灶反例三件（删 READ/几何错位/少 REDUCE）是"改回也过"的存在证明，改动判定式必须三反例重跑。
29. **灰度矩阵断言的入口可达性**：宏关编译的 TU 里 env 开断言不可达（编译期闸门 static bool 恒 false，setenv 晚了也不生效——R17 st 段 8 FAIL 教训）；同一二进制 env 开/关两遍跑时，分支依赖断言必须带 env 门卫（skip + 口径说明 PASS，不虚报 FAIL——env 关形态走旧分支是既有断言锁定的预期行为）。断言落位先查 run.sh 的宏/env 矩阵结构。

## R18 NHR 动词对称化纪律（2026-10-14 新增，M0_R18_NHR_EXPAND.md 存档）

30. **宏动词对称化纪律（#11）**：pattern 引入新流形态时，L4 Emit 的产物形态必须与既有 pattern 对等（逐动词展开），**禁止宏动词打包**——步/片循环外置到动词序，循环维（repeat/jetty）留动词参数。几何唯一实现点在公共件（NhrSliceOf——Emit 发索引不自算偏移，sink/server 执行端推导，双消费方单源）；方向无关动词（SLICE_REDUCE）的 WR/RD 由执行端查通道协议自判（知识下沉格局不变）。等价锁：新形态贡献集 == 步表展开（多重集相等，checker PASS）——R18 全删轮后宏形态已删，双形态对拍退役为三源一致锚（贡献集等式 + checker + ST 独立基线）。消费方特例信号：checker/DryRun/ST 对某 pattern 出现 special-case 分支 = 词汇表完备性偏离的报警（R18 的立项起因）。
31. **灰度矩阵拆遍跑法（★R18 全删轮后退化两遍）**：两形态断言（宏动词 vs 展开动词）依赖不同 env 且同进程 env 定格（static bool 首调定格）——run.sh 必须拆遍跑，断言段带形态门卫（skip + 口径 PASS）。新增灰度开关时先查 run.sh 跑法结构是否需要拆遍（#29 的执行面细化）。**R18 全删轮（2026-10-14）后**：NHR_RUN 宏形态删除、expand 开关退役——exec 入口从三遍（宏形态遍/展开遍/回归遍）退化为两遍（展开遍 + 回归遍）；本条纪律保留（UseUnifiedNhrAicpuPath 等其余灰度开关仍适用）。

## R19 线程申报查表纪律（2026-10-15 新增，M0_R19_THREAD_CLAIM.md 存档）

32. **申报公式与编排方式绑定纪律（#12）**：slaveThreadNum 公式的选择键 = **Recipe**（ClaimFormulaOf 查表，recipe_sole.h）而非 env/全局开关——新增编排方式 = 查表加一行 case（switch 全枚举覆盖，-Werror 漏 case 即编译错 = 新枚举值的强制显式决策；REJECTED 是显式决策非缺省）；claim 是 host 侧 Plan 字段，不序列化进 blob（v5/server/kfc 零触碰）。执行面灰度（UseUnifiedConcurrencyPath 编排器分叉）与申报面解耦——跨进程失配从"公式错配"收窄为"执行面机器新旧"。

## R20 L3 引擎知识出清纪律（2026-10-16 新增，M0_R20_L3_ENGINE_EXIT.md 存档）

33. **L3 动作头零 Engine 知识（#13）**：L3 动作头不出现 Engine 枚举字面量与 BackendRegistry 反查——引擎半边收集唯一落点 L2 `SegmentBuilder::CalcResOf` 的 kAllEngines 循环（直列数组 {CCU, AICPU}，**序=行为契约**：ClaimEngineRes 对 slave/notify 直赋非累加，段本地终值取末次写入，倒序即 slave 恒 0）；收集形态为段本地 segClaim 聚齐后 `+=`（直写累积器会破坏 operator+= 的 max/追加合并语义——chunk2/two_shot 锚必断）。新引擎 = 注册表加后端 + kAllEngines 加一项（L3/L2 case 均零改）。等价锁：CalcResOf 产物 == 旧 CalcRes 全量（st 64-② 常跑锚）；L3 出清锁（kernels 恒空 + 通道裸值）st 64-①。
