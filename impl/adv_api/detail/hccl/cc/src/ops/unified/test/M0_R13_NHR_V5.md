# R13 · M0 考古——NHR 段接入 v5 动词序执行器

> 2026-10-09 落盘。挂账 #3：NHR（ReduceScatter NHR1D MultiJetty Mem2Mem）接进 v5，A/B tiered 上设备的地基。
> 裁决来源：任务书预置五条 + R12 资产（M0_R12_VERB_EXECUTOR.md）。行号 = 本仓实数。
> 必读 kernel：ccu_kernel_kfc_reduce_scatter_nhr1d_multi_jetty_mem2mem.cc（306 行，下称 NHR.cc）。

## 一、必答① NHR kernel 三阶段 → 动词序投影

| 阶段 | 行号 | 原语序 | v5 动词投影 |
|------|------|--------|------------|
| PreSync | NHR.cc:80-94 | 每通道 WVN(input[XN0])+WVN(token[XN1]) → NotifyWait(0x3) | SYNC_PRE（NHR 掩码，§五） |
| RunStep×S | :148-220 | 步前 NR/NW(STEP_PRE :161-162, step≠0) → 逐 slice×jetty WriteReduce(:115-136) + EventWait → 步末 NR/NW(STEP_POST :217-218) | NHR_RUN 帧内展开（§三） |
| 收尾 | :237-257 | repeat 循环 LocalCopy(output, input+off[me]) + EventWait | NHR_RUN 帧内展开 |
| PostSync | :96-105 | 每通道 NotifyRecord(0x10) → NotifyWait(0x10) | SYNC_POST（掩码 0x10 同 mesh） |

关键形态（与 mesh RS 的本质差）：
- **NHR 是写侧**：WriteReduce(channel, remoteDst, localSrc, size)——本端 input 分片直写对端 input 同偏移处并远端规约（:111-136）；**无 scratch、无本地 REDUCE 折叠**（规约在逐条 WriteReduce 中在线完成）——mesh RS 的 READ→REDUCE 两段在 NHR 合一。
- **通道选择**：sendChannel = channels[rank2ChannelIdx[toRank]]（:150-153）——由 stepInfo.toRank 决定，**不是通道表推进**。
- **步间同步**：NR(recvCh,STEP_PRE)+NW(sendCh,STEP_PRE) / NR(sendCh,STEP_POST)+NW(recvCh,STEP_POST)（CKE 位 2/3）——方向随步翻转，与 v5 VerbSyncPost 的全通道 record+wait 不同构。
- **jetty 循环**：jettyId 0..jettyNum-2 走 sliceOneJettySize（=0 时 EventRecord 占位 :124-126），尾 jetty 走 sliceLastJettySize（=0 时占位 :130）。devkit RS NHR 链恒 portNum=1（ccu_temp_kfc_reduce_scatter_nhr_1D_multi_jetty_mem2mem.cc:18 KFC_RS_NHR_PORT_NUM=1），jettyNum>1 是 kernel 能力、协议面保留。
- **repeat 语义**：repeatNumInv 形参（UINT64_MAX-repeatNum 倒计数），CCU_WHILE(repeatNum!=UINT64_MAX) 设备期循环（:178/:204/:245），轮步进 inputRepeatStride/outputRepeatStride。sole 链恒 repeatNum=1、双 stride=0（写入方 :519-521）。

## 二、必答② 隐式映射 NHR 适用性判定【本案核心】

**结论：NHR 数据帧序 ≠ 通道序——R12 隐式映射（第 k 数据帧 ↔ channels[k]）对 NHR 不适用。**

证据（8 卡 me=3，GetStepInfo 同式 ccu_temp_kfc_...nhr_1D_multi_jetty_mem2mem.cc:91-109）：
- stepNum=3；toRank(step)=（me+N−2^step)%N → 序 [2,1,7]；通道消费序=chIdx([2,1,7])=[1,0,4]（建链集 {1,2,4,5,7}——±1→{4,2}、±2→{5,1}、±4→{7,7} 同 rank 去重，共 5 条）。
- 注册通道序（建链集升序）= [1,2,4,5,7]——toRank 序 [2,1,7] ≠ 升序序，且同一 step 的全部 slice 走**同一**通道（:150-153 sendChannel 循环外取定）。NHR 段 channelCount=5 ≠ mesh 段 N−1=7（混排断言按段各自口径——M0 复核修正 A）。
- 每步总 slice 数 nSlices=(N−1+2^step)/2^(step+1)，Σ=N−1（8 卡=4+2+1=7）。

处置：NHR 段不走隐式游标——通道由 **rank2ChannelIdx 显式推导**（§三）。计数契约表（kfc_server_message.h:342-364）追加一条：
```
| NHR_RUN(8) | 不计 | 整段自持通道表（WriteReduce 通道=chIdx(toRank)，帧序≠通道序，不动 dataCursor） |
```
写入方 dataFrameIdx 分支（NHR_RUN 不 ++）与 server VerbNhrRun（不动 ve.dataCursor）两处同步——演进契约三处登记齐。

## 三、必答③ 三件自持量取值面 → 承载方案【证伪预置裁决 2，简化；M0 复核修正 A】

| 自持量 | 取值面（生成源行号） | 结论 |
|--------|---------------------|------|
| stepInfoVector | KfcNhrStepInfo{step,myRank,nSlices,toRank,fromRank,txSliceIdxs[],rxSliceIdxs[]}，kfc_server_protocol.h:46-54；生成 = GetStepInfo **纯函数**（rankSize/myRank/step 全决定，ccu_temp_kfc_...nhr.cc:91-109 逐行可逆，无 topo 输入——M0 复核确认） | **不进 blob**——server 微码 host 期按公式重建（与 GetStepInfo 同式）。证伪预置"进 blob 变长区/定长截断"：容器是纯函数投影，序列化即冗余且引入截断拒收面 |
| rank2ChannelIdx | map<rank,chIdx>，v2 链 = CalcChannelRequestNhrMultiJetty（channel.cc:839-901）产出：connectRanks = {rank±2^s} 逐层并集（CalcNHRChannelConnect :56-73，set 升序去重）→ 逐 peer 首条链去重（ccu_temp_kfc_...nhr.cc:47-57）。unified 链原走 BuildUnifiedChannelDescs → CalcChannelRequestMesh1D（全连接 mesh 序）——两链通道表序与成员都不同（M0 复核修正 A：NHR 是 log2 条链的稀疏拓扑，mesh 全连接假设下 chIdx 升序规则 (rank<me)?rank:rank−1 直接错位） | **甲路线（复核裁定，R13 已落地）**：unified NHR 段通道申请改调 CalcChannelRequestNhrMultiJetty（与 v2 同源同序——BuildUnifiedChannelDescs 按注册名前缀分叉，op_common.cc R13 注）；chIdx(rank) = server host 期按 CalcNHRChannelConnect 同式重建 connectRanks 升序集再顺序编号——建链集输入，非 mesh 全连接假设。四端单源（L3 CalcRes / L4 ClaimEngineRes / 注册链 / server 重建，R13-⑥ 断言守门）；灰度注意：NHR 段 channelCount（建链集大小，8 卡 me=3 = 5——±4 同 rank 去重）≠ mesh 段 N−1，混排断言按段各自口径 |
| jettyNum | v2 继承 srcArg->portNum（恒 1）；unified 无继承源 | **进帧槽 10**（资源期拓扑量，ActionSpec 加 nhrJettyNum 字段缺省 1——加法不改法） |

stepNum 上界：floor(log2(rankSize))；rankSize≤256 → ≤8 步（server host 期循环上限无帧约束）。无 stepInfoVector 超限拒收面（负例矩阵改 jettyNum=0 拒）。

## 四、必答④ 帧预算重算（NHR 段恒 3 帧——SYNC_PRE+NHR_RUN+SYNC_POST）

| 形态 | 帧数 | 手算 |
|------|------|------|
| NHR sole 段（任意 N） | **3** | step/slice/jetty 循环在 server 帧内展开，不随 N 涨 |
| A 交错（16P 4×4，parallel[mesh,nhr]） | 18 | (4+2)+3+3+(4+2)=18 ≤ 63 ✓ |
| B 步进（16P 4×4，stepNum=6，tier_expand.h:162） | **54** | 6×(6+3)=54 ≤ 63 ✓ |
| B 步进超限边界 | — | S×(N1+5)≤63：N1=4→S≤7（16P 档 S=6 安全）；N1=16（16×16 域）S=4→84>63 拒（超限维持，判据=SerializeVerbPlan 63 上限既有防线） |

**R12 审查方"72 帧超限"结论修正**：其数字按 nhr 段=N+2=6 帧（mesh 化假设）算（12×6=72）；NHR 动词化后 nhr 段 3 帧 → 16P PCIeMix 主流档 **54 帧 ≤ 63，B 步进超限解除**（A/B 上设备的帧预算依据）。A/B 运行时激活仍不在 R13（格式级验证，WP-5）。

## 五、必答⑤ pattern 槽位定稿 + 缺省兼容

**定稿：SYNC_PRE 帧槽 31 = pattern（MESH=0 / NHR=1）。**

| 论点 | 内容 |
|------|------|
| 层级 | pattern 是**段级**语义——挂段首 SYNC_PRE 帧（每段必有，天然段界标）；blob 头部槽是 Plan 级，不适用 |
| 缺省零变化 | 既有 MESH 段槽 31 恒 0（序列化器 EncodeVerbFrame 清 0 语义）——MESH 帧逐字节不变 |
| server 分派 | **不读槽 31**——按 SYNC_PRE 掩码分派（0xC=AG / 0x7=mesh RS / **0x5=NHR** 三掩码互异）；槽 31 是声明性载体（写入方/Validate/ST 消费，掩码漂移的冗余防线） |
| NHR SYNC_PRE 掩码 | v5 XN 表编码 IN\|TOK = (1<<0)\|(1<<2) = **0x5**（老 NHR 表 INPUT=0/TOKEN=1=0x3 是旧编码，迁 v5 位；POST=4 不动） |
| 范围 | 本轮 pattern∈{MESH=0,NHR=1}；NHR 仅 ReduceSlice 段受理（AG NHR 挂 P2） |

## 六、必答⑥ 三组 golden 手算（8 卡 me=3 FP16，jetty 公式 = v2 写入方 :507-510 同式：alignCnt=128/dts=64，oneJetty=slice/jetty/alignCnt*alignCnt 下取整，lastJetty=slice−(jetty−1)×oneJetty）

帧序恒：帧0 SYNC_PRE{6, 0x5, inBaseOff=0, …, 槽31=1} / 帧1 NHR_RUN{8, 槽1..10} / 帧2 SYNC_POST{7, 0x10}；blob=7+96=103 槽。

| 组 | 参数 | 帧 1 槽 3..10 手算 |
|----|------|-------------------|
| G1 基准 | count=500→slice=1000, stride=1000, jetty=1 | 3:1000 4:1000 5:1000/1/64*64=**960**（v2 :507-510 逐字同式：sliceOneJetty = sliceSize/nhrJettyNum/sliceAlignCount*sliceAlignCount——**先除 jettyNum 再对齐**；sliceAlignCount=128/dts=64）6:1000−(1−1)×960=**1000**（jetty=1 时槽 5 不被消费——kernel :114 循环零次、尾 jetty=唯一 jetty 走槽 6 承载全量；但槽 5 值仍按公式写入=960，非 0）7:~0−1=0xFFFFFFFFFFFFFFFE 8:0 9:0 10:1 |
| G2 多 jetty | count=1000→slice=2000, stride=2000, jetty=2 | 3:2000 4:2000 5:2000/2/64*64=**960** 6:2000−(2−1)×960=**1040** 7:~0−1 8:0 9:0 10:2 |
| G3 尾 jetty 短片 | count=750→slice=1500, stride=1500, jetty=2 | 3:1500 4:1500 5:1500/2/64*64=**704** 6:1500−704=**796**（尾 jetty 短片 ✓）7:~0−1 8:0 9:0 10:2 |

槽 1/2 = inBaseOff/outBaseOff（Compile 期结构量，本构造 0）；槽 3..9 双道防线（Compile 期真值 + 写入方覆写，mesh 同构）。

**等价断言原语序**（G1 8 卡 me=3，串行路径，jetty=1，与 kernel 逐行；★建链集口径——NHR 段通道数 5 {1,2,4,5,7} 非 mesh 7，M0 复核修正 A）：
PreSync 10 WVN（2 XN × 5 通道）+5 NW(0x5)；step0（无步前）4×(WR+EW) + NR+NW 步末；step1 步前 NR/NW → 2×(WR+EW) → 步末 NR/NW；step2（toRank=fromRank=7，±4 同 rank——NR/NW 同通道对）步前 NR/NW → 1×(WR+EW) → 步末 NR/NW；收尾 LC(output,input+3000,1000)+EW；PostSync 5 NR(0x10)+5 NW(0x10)。chIdx：建链集升序 [1,2,4,5,7]，toRank 序 [2,1,7]→chIdx [1,0,4]、fromRank 序 [4,5,7]→chIdx [2,3,4]。合计 10+5+7WR+7EW+10NR步+10NW步+1LC+1EW+5+5=**61 条**（步间屏障 10：step1/2 前置 2×2 + 三步步末 3×2——step2 的 ±4 同 rank 使 sendChannel==recvChannel 但 Record/Wait 仍各一条原语）。ST 可归一化基线（不含步间屏障）= 41 条（st_verification R13-③）。
WriteReduce 几何：dst=对端 IN+off[tx]（tx=步内 txSliceIdxs 逐项：step0=[2,0,6,4]、step1=[1,5]、step2=[7]）、src=本端 IN+off[tx] 同偏移、size=1000——**对端同构布局同偏移**是 NHR 写侧的关键不变量。

## 七、必答⑦ ve 混排复位验证清单（[A9] NHR 版）

1. mesh RS 段(0x7)→NHR 段(0x5)→AG 段(0xC) 三掩码交替：VerbSyncPre 入口四向量 clear + dataCursor=0（R12-8 既有行对 NHR 段同样生效——掩码分支之前，零改动验证点）。
2. NHR 段 SYNC_PRE(0x5) 后：peerIn/peerTok==channelCount，peerScr/peerOut==0（掩码不含位不 push——last-segment-wins）。
3. NHR_RUN 不动 dataCursor（计数契约新条目）；后续 mesh 段 SYNC_PRE 复位后游标从 0 起算。
4. 段间交替无残留：NHR 段（无 SCR 广播）后接 mesh RS 段，peerScr 由 mesh 段重建非残留。
5. XN 掩码交替断言（A/B 序列 blob 逐段槽 1 序列）：A 交错=[0x7,0x5,0x5,0x7]；B 步进=[0x7,0x5]×S。
6. 混排超限负例：帧预算 63 上限既有防线复证（B 步进超限构造 84>63 拒）。

## 八、实施规格汇总（WP-1..6 的契约锚）

| 项 | 定稿 |
|----|------|
| NHR_RUN 帧（32 槽） | 槽0=8；槽1=INPUT off(段内偏移,server 加 ve.localIn/peerIn)；槽2=OUTPUT **绝对**基址(写入方 recvBuf 合成式——本地收尾 Copy 直用)；槽3=SLICE_SIZE；槽4=INPUT_SLIDE_STRIDE；槽5=ONE_JETTY；槽6=LAST_JETTY；槽7=REPEAT_NUM_INV；槽8=IN_REPEAT_STRIDE；槽9=OUT_REPEAT_STRIDE；槽10=JETTY_NUM；槽11..31=0。槽序镜像 KFC_RS_SOLE_NHR_*（kfc_server_message.h:140-152）+ 槽 10 扩展 |
| 新动词 ID | KFC_UNIFIED_VERB_NHR_RUN=8（VerbCall::NHR_RUN=8 枚举尾追加；ID_MAX 7→8——仅 static_assert 值域，无既有消费回归） |
| server 分支 | VerbSyncPre 加 0x5 分支（广播 IN+TOK→NW→缓存 peerIn/peerTok）；VerbNhrRun 新函数（host 期公式重建 stepInfo+chIdx 升序推导；gen 期串行路径逐行平移 kernel RunStep/收尾——流水路径登记 P2）；DispatchUnifiedVerbs 加 ID==8 分支 + 未知 ID 链尾加一层 !=8 |
| 写入方覆写（CcuPrepareForUnifiedVerbPlan NHR_RUN 分支） | 槽1 透传；槽2=recvBuf+repeatOffset+槽值；槽3=count*dts；槽4=stride 三分支；槽5/6=jetty 公式（槽10 读 jettyNum）；槽7=~0−1；槽8/9=0；槽10 透传；jettyNum==0 拒发。SYNC_PRE 三槽注入既有分支零改动（多填无害） |
| 序列化器 | SerializeVerbSegment 加 NHR 分支（跳过 mesh chunk 限界——NHR 无 chunk；NHR_RUN 槽从 seg.layout 写 Compile 期值）；ValidateVerbSegment 加 NHR 分支（断言动词序恰{SYNC_PRE,NHR_RUN,SYNC_POST}+jettyNum∈[1,4]，跳过 mesh 数据帧两断言） |
| L3/L4 | ActionSpec 加 nhrJettyNum{1}；BuildArgs 放开 NHR 受理（产九标量——mesh 拒收分支改 pattern 分叉）；CcuReduceSlice::Emit 加 NHR 分支（3 动词：SYNC_PRE/NHR_RUN/SYNC_POST——NHR_RUN 的 VerbCall 载荷占位，帧槽由序列化器段级补写） |
| 灰度/注册链 | UseUnifiedVerbPath/路由 RS 行既有（零改动）；InheritKfcServerKernelArg 零改动（"CcuVerb" 前缀已覆盖）；**NHR 段通道申请按甲路线落地（M0 复核修正 A）：L3 CalcRes/L4 ClaimEngineRes NHR 分支按 connectRanks 建链集申报，注册链 BuildUnifiedChannelDescs 按注册名前缀分叉选 CalcChannelRequestNhrMultiJetty（"CcuVerb" 族）/CalcChannelRequestMesh1D（"CcuAction" 族，零变化）——与 server VerbNhrRun chIdx 重建四端单源（R13-⑥ 断言守门）**；hccl_alloc_ctx_res.cc 序列化分叉零改动（CaptureVerbSegment 经 BuildArgs 放开自动产 calls） |
| ST | StBuildRsNhrPrimBaseline（kernel 逐行基线构造器）；StExpandVerbFrames 加 NHR_RUN 展开；CheckNhrVerbSeqEquivalent（按步窗口比较：步间屏障保序 + 窗口内搬运 multiset + 段级 sync 边界——步间 NR/NW 两侧同构归一化为 STEP_SYNC 事件）；SimulateDispatchVerbs（host 镜像）加 NHR_RUN 展开与混排复位断言 |
