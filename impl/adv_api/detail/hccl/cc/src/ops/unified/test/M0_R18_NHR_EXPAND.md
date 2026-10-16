# M0_R18 · NHR 动词对称化（#11——语义面展开 + 编码面折叠分离）

> **轮次任务书**：R18_TASK.md。本文件 = 六必答定稿，**复核通过才动代码**。
> **基线**：断言 1920（run.sh 533 / st_checker 977 / exec 410），只增不减；旧 NHR_RUN 路径一行不动（灰度共存）。
> **积木行号已复核**（2026-10-14 实测）：DoNhrRun 逐 slice 几何 hcomm_verb_sink.h:502-538 / ACK 半边 :487-496 / DATA 半边 :537-543 / isDmaRead+skipStep0Tx :446-452 / DomainThreadTotalNeed action_spec.h:291 / v5 槽区 verb_serializer.h:70-110 / KFC_UNIFIED_VERB_* 真源 common/kfc_server_message.h:285-294 / Validate dataFrames==channelCount verb_serializer.h:402-415。

---

## 一、必答 1：几何公共件签名与落点

**落点：`inc/nhr_slice_geometry.h`（与 nhr_steps.h 同级同理由——纯值函数零引擎依赖）。**

从 DoNhrRun :502-538 提取四函数（DoNhrRun 改为消费公共件——**改造而非并存**，理由：几何公式单源纪律（REVIEW #16）要求"展开 Emit 与存量 sink 同一实现"，并存两份几何必然漂移；改造的风险由既有 exec T4/T5 NHR 断言锁护航——原语序 golden 不变即证零漂移）：

```cpp
// 步/slice 的方向无关参数组（peer + 本地侧偏移 + 对端侧偏移 + size）
// ★方向语义：peer = WR 时 sendTo / RD 时 recvFrom（两侧 peer 不同——步表对称性）
struct NhrSliceGeom {
    u32 peerAlg;        // WR: st.sendTo / RD: st.recvFrom（经 domain.ranks 换算 userRank 由调用方）
    u64 localOff;       // WR: 本地 tx 源偏移 / RD: 本地 scratch rx 槽偏移
    u64 remoteOff;      // WR: 对端 buff tx 槽偏移 / RD: 对端 buff rx 槽偏移
    u64 size;           // SliceSizeOf(rpt 内恒定——slice 维不乘 rpt)
};

// 逐 slice 几何（一函数出全部——WR/RD 由 isDmaRead 入参选侧，与 DoNhrRun :509-538 同式）
inline NhrSliceGeom NhrSliceOf(const VerbCall::NhrRunScalars& sc, u32 step, u32 sliceIdx,
                               bool isDmaRead, u64 rpt);

// skipStep0Tx 判定（:452 同式——纯函数化：isDmaRead 与 n 由入参）
inline bool NhrSkipStep0Tx(bool isDmaRead, u32 n) { return (!isDmaRead) && (n > 1U); }

// repeat 轮偏移（:505 同式）
inline u64 NhrRptInOff(const VerbCall::NhrRunScalars& sc, u64 rpt);
```

**双消费方**：①新 Emit（展开序产 SLICE_REDUCE——但 **isDmaRead Emit 期不可知**，见下）；②旧 DoNhrRun（改造为调 NhrSliceOf，逐 slice 几何一行不差）。

**关键裁定——Emit 展开的"方向无关"实现**：Emit 产 SLICE_REDUCE 时 **isDmaRead 传"两义哨兵"**——动词帧只带 **`(peerWR, localOffWR, remoteOffWR, peerRD, localOffRD, remoteOffRD, size)` 不行（槽不够）**。改为：**动词带两侧 peer + 两侧偏移的并集参数（ARG0..3 + 槽 4 peerWR + 槽 5 peerRD）**？过度。**正解：动词只带 RD 侧参数（peer/本地/对端偏移），WR 侧由"步表对称性"执行端推导**——WR 与 RD 是同一 slice 移动的两个视角：RD 的 `(recvFrom, 本地rx, 对端rx)` 与 WR 的 `(sendTo, 本地tx, 对端tx)` 满足 **tx[i] 与 rx[i] 是同一步表的两个数组**（CalcNhrSteps 已给出两者），Emit 发动词时带 `step` 与 `sliceIdx` 两个**索引**而非偏移，执行端按 NhrSliceOf(载荷种子, step, sliceIdx, 自判 isDmaRead) 推全部几何——**帧槽只花 2 个索引槽，几何唯一实现点仍在公共件**。最终签名修正：

```cpp
// 动词形态（方向无关）：载荷 = NhrRunScalars 种子（复用现有 VerbCall.nhr 11 标量）
// + step/sliceIdx 索引。sink/server 执行时：NhrSliceOf(call.nhr, step, sliceIdx,
// isDmaRead 自判, rpt 循环) → 偏移；方向由通道协议定（卡点④解法）
```

即 **SLICE_REDUCE 的帧/Emit 参数 = {nhr 种子载荷, step, sliceIdx}**——与 NHR_RUN 帧的差别仅是"步/slice 循环外置到动词序"，种子仍随动词走（AICPU 值内嵌 / v5 帧槽段级补写——与现状 NHR_RUN 载荷两引擎落点差异同型）。

## 二、必答 2：SLICE_REDUCE/CH_SYNC 的 ThreadPolicy 与编排推演

**裁定：两动词均 BY_CHANNEL（与 NHR_RUN 同列）**，理由：执行线程 = `threads[channelIdx]`（通道序自持）——与 DoNhrRun :456-459 完全一致，不进 READ/WRITE 计数器（混排段隔离，#24 纪律延续）。

**DomainThreadTotalNeed 不变**：NHR 段 need 公式是**段级**的（`1×channelsPerRank`，action_spec.h:297），与段内动词形态无关——展开后动词多了但都走 BY_CHANNEL（threads[0..ch-1]），need 仍 1×ch。**这是重要不变量**：R15-③ Σ 申报断言、R16-#7 表声明断言全部零修改通过。

**DryRun 推演**（pipeline_model.h 现有 BY_CHANNEL 分支直接复用）：SLICE_REDUCE/CH_SYNC 逐动词 1 行（devkit ch=1 → 全 t_{base}）；16P tier A 形态 nhr 段展开后 16 动词全 t4——**DryRun 步数锚：8/16/16/9 = 49 步**（对照宏形态 8/6/6/9=29——实施勘误：CH_SYNC 成对展开后每步 4 条握手，N=4 段 12→16）。

## 三、必答 3：三组手算（python 复算与公式独立推导一致）

### 1. NHR 8P me=3 展开动词总数与逐条序

步表（CalcNhrSteps(3,8)）：step0{sendTo=2, recvFrom=4, nSlices=4} / step1{sendTo=1, recvFrom=5, nSlices=2} / step2{sendTo=7, recvFrom=7, nSlices=1}。

**动词序（28 动词——实施勘误：CH_SYNC 半边成对展开为 Record+Wait 两动词，与 DoNhrRun :487-496 "先 Record 后 Wait" 两段序逐条对应；原手算 22 漏了 Wait 侧）**：

```
COPY×8（上料 r=0..7）
CH_SYNC(步0, ACK-Record)  CH_SYNC(步0, ACK-Wait)     ← 步0 半边①（两动词）
SLICE_REDUCE(step0, i=0..3)×4   ← tx=[2,0,6,4] rx=[3,1,7,5]
CH_SYNC(步0, DATA-Record) CH_SYNC(步0, DATA-Wait)    ← 步0 半边②
CH_SYNC(步1, ACK-Record)  CH_SYNC(步1, ACK-Wait)
SLICE_REDUCE(step1, i=0..1)×2   ← tx=[1,5] rx=[3,7]
CH_SYNC(步1, DATA-Record) CH_SYNC(步1, DATA-Wait)
CH_SYNC(步2, ACK-Record)  CH_SYNC(步2, ACK-Wait)     ← 步2（sendTo==recvFrom=7 同 peer）
SLICE_REDUCE(step2, i=0)×1      ← tx=[7] rx=[3]
CH_SYNC(步2, DATA-Record) CH_SYNC(步2, DATA-Wait)
COPY×1（归位 SB+S·3 → OutOffset(3)）
```

计数：8 + (4+4) + (4+2) + (4+1) + 1 = **28**。CH_SYNC 四态编码（size 槽）：{0=ACK-Record, 1=ACK-Wait, 2=DATA-Record, 3=DATA-Wait}——WR/RD 方向由执行端查通道协议自判取 peer（src=WR 侧 sendTo / dst=RD 侧 recvFrom 双 peer 槽）。

### 2. 16P tier A 形态展开后 perSeg 与 DryRun 步数

nhr 段（域 1 n=4，me=0）：步表 2 步（step0{sendTo=3, recvFrom=1, nSlices=2, tx=[3,1], rx=[0,2]} / step1{sendTo=2, recvFrom=2, nSlices=1, tx=[2], rx=[0]}——python 复算与公式独立推导一致）。

动词数 = 上料 COPY×4 + (4+2) + (4+1) + 归位 COPY×1 = **16**（实施勘误：CH_SYNC 成对后每步 4 条握手）。全段 perSeg = **8/16/16/9**，DryRun steps = **49**（threadId：nhr 段 16 动词全 t4——BY_CHANNEL ch=1）。

### 3. 贡献集等式（等价锁的判据定义）

**定义**：展开形态的 SLICE_REDUCE 贡献集 = {(peer, slotIdx) 动词对全集}；宏形态贡献集 = CalcNhrSteps 全步 tx/rx 展开同型对。**两侧集合相等 = 等价**（checker 的镜像贡献核对在两侧应产出相同 violation 集——即双 PASS）。

n=8 me=3 python 复算：
- **WR 侧** {(peer,tx)}：{(2,0),(2,2),(2,4),(2,6),(1,1),(1,5),(7,7)} —— 7 对无重复
- **RD 侧** {(peer,rx)}：{(4,3),(4,1),(4,7),(4,5),(5,3),(5,7),(7,3)} —— 7 对无重复
- 全步 tx slot 并集 {0,1,2,4,5,6,7} = 除 me=3 外全集 ✓（步表闭包既有断言）

**注意 step2 的特殊性**：sendTo==recvFrom==7——该步 WR/RD 同 peer（对端 7 既是发送目标又是接收源），CH_SYNC 与 SLICE_REDUCE 的方向自判在该步退化为"同通道双向"——原语序与 DoNhrRun :487-496 四分支逐条对应，无新增形态。

## 四、必答 4：v5 帧槽布局表（两新动词）+ ID_MAX 升位互锁

**SLICE_REDUCE 帧（verb ID=9）**：

| 槽 | 语义 | 备注 |
|---|---|---|
| 0 | verb ID = 9 | KFC_UNIFIED_VERB_SLICE_REDUCE == VerbCall::SLICE_REDUCE |
| 1 | step 索引 | 执行端 NhrSliceOf 入参 |
| 2 | sliceIdx 索引 | 同上 |
| 3 | size | SliceSizeOf（校验用——与推导值比对，不等即拒收） |
| 4 | peerWR（algRank） | WR 半边 peer（sendTo） |
| 5 | peerRD（algRank） | RD 半边 peer（recvFrom） |
| 7-10 | repeat/jetty 种子 | 照 NHR_RUN 帧槽 7-10 同型（REPEAT_NUM_INV/IN_REP_STRIDE/OUT_REP_STRIDE/JETTY_NUM） |
| 11..30 | 0 | 预留 |
| 31 | 0 | pattern 载体槽（SLICE_REDUCE 帧不携带——仅 SYNC_PRE 帧有效约定不变） |

种子九标量（input/stride/scratchBase 等）**段级补写**（序列化器从 ActionSpec layout 直取——与 NHR_RUN 帧同机制，M0_R13 §八模式）；peer 用 **algRank**（执行端经域成员表换 userRank 查通道——server 侧 `ctx.arg` 有域成员、AICPU 侧载荷 domainRanks，两侧同式）。

**CH_SYNC 帧（verb ID=10）**：

| 槽 | 语义 |
|---|---|
| 0 | verb ID = 10 |
| 1 | peer（algRank） |
| 2 | dir ∈ {0=WR_ACK, 1=WR_DATA, 2=RD_ACK, 3=RD_DATA} |
| 3 | idx（NOTIFY_IDX_ACK=0 / NOTIFY_IDX_DATA_SIGNAL=1——与 dir 冗余校验） |

**互锁同步点**（跨线打卡 3 处）：
1. `common/kfc_server_message.h:285-294`——加 `KFC_UNIFIED_VERB_SLICE_REDUCE=9U`、`KFC_UNIFIED_VERB_CH_SYNC=10U`，`KFC_UNIFIED_VERB_ID_MAX` 8→10（static_assert 同步）；
2. `unified/inc/verb_sink.h`——VerbCall::Verb 枚举尾追加 SLICE_REDUCE=9/CH_SYNC=10（值序镜像注释同步）；kAicpuVerbMap/kCcuVerbMap 各加两行（target/note/thread=BY_CHANNEL）；
3. `unified/inc/verb_serializer.h`——enum UnifiedV5Slot 加 `U_V5_SR_STEP/U_V5_SR_SLICE_IDX/U_V5_SR_SIZE/U_V5_SR_PEER_WR/U_V5_SR_PEER_RD/U_V5_CS_PEER/U_V5_CS_DIR/U_V5_CS_IDX`；**Validate**：两动词**不计 dataFrames**（:402-415 的 READ/WRITE 计数循环不加——隐式映射只索引 READ/WRITE 不变）；新帧序规则：SLICE_REDUCE 必须夹在同 dir 的 ACK/DATA CH_SYNC 对之间（步闭包形态校验）。

**server 新分支**（ccu_kernel_kfc_server.cc，跨线打卡）：VerbSliceReduce（peer 槽显式查通道表**不动 ve.dataCursor**——查表逻辑从 VerbNhrRun 的 connectRanks 序镜像；`ccu::WriteReduce`/方向自判按通道属性；repeat 帧内循环照 :502-505 同式）+ VerbChSync（`CcuNotifyRecord/Wait` 半边——按 VerbSyncPre/Post :291/:395 既有形态）。

## 五、必答 5：server 侧编译验证策略

本机不可编译 CANN 环境（R16 #6 已证）。**纯函数抽取面**：
- **VerbSliceReduce 的参数映射**：`(帧槽值, 通道属性, 域成员表) → (原语参数四元组)`——抽 `NhrSliceReduceMapping()` 纯函数进独立编译单元，单测喂帧槽组合断输出（WR/RD 两方向 × step0/非 step0 × repeat 首末轮 8 用例，期望值手算）；
- **VerbChSync 的 dir 分派**：四态 → (Record/Wait, 通道, idx) 三元组——同法 4 用例；
- **通道查表**：connectRanks 序重建逻辑已在 VerbNhrRun（host 期常量段）——新分支复用同表，抽取 `NhrChannelIndexOf(peerAlg)` 单测对照 AICPU 侧 ChannelOfIdx 语义（同一建链集）。
真机复核 P2（与 #6 注册对拍同批）。

## 六、必答 6：灰度开关矩阵

**三层同式**（unified_route.h NHR_AICPU 族同款）：
- ① 编译期宏：`MC2_UNIFIED_NHR_EXPAND_RESOURCE`（默认**关**）
- ② 运行期 env：`MC2_USE_UNIFIED_NHR_EXPAND`（默认关）
- ③ 范围：NHR 段 Emit 形态选择（**仅 AICPU 半边第一步**；ungated 照旧发 NHR_RUN 宏动词 = 现状零变化）

**与 UseUnifiedNhrAicpuPath 的正交关系（裁定：蕴含式，非独立）**：

```
UseUnifiedNhrExpandPath() := 宏开 && env 开 && UseUnifiedNhrAicpuPath()
```

理由：expand 的 sink 执行面（SLICE_REDUCE 映射 WR/RD 原语）与 DoNhrRun 同属 NHR AICPU 执行面——aicpu gate 关时 sink 无 NHR 语义（DoNhrRun 分支不可达，:190-193 现状），expand 单开无消费方。**不设"expand 开 + aicpu 关"的拒绝逻辑**——蕴含式天然不存在该状态（布尔与）。CCU 半边（第二步 v5/server）**独立灰度**：`MC2_UNIFIED_NHR_EXPAND_V5`（序列化分叉——verbPath 既有三合取之外第四合取项；宏关时 CCU 恒发 NHR_RUN 紧凑帧 = R13 现状）。

**测试落位**（#29 纪律）：
- st_checker（宏关）：仅"expand gate 恒 false"断言 + 公共件 NhrSliceOf 纯函数直测（几何手算锚——NHR 8P me=3 逐 slice 三函数值）；
- exec tier_payload（宏开 aicpu 开 env 开）：展开动词序 22/12 锚 + checker 双 PASS + 贡献集等式断言 + DryRun 41 步锚 + **ungated 对拍**（同段两 Emit 路径 verbs 数 == 22 vs 10 宏形态）；
- verb_exec（v5 宏开）：两新帧 golden-bytes + Validate 帧序规则正负例 + dataFrames 不计数断言。

## 七、改动面清单与验收

**新增**：`inc/nhr_slice_geometry.h`（几何公共件三函数）；R18 三入口测试段。
**改动**：verb_sink.h（枚举+两表行）/ verb_serializer.h（槽 enum+两帧序列化+Validate）/ backend_aicpu_reduce_slice.h（gated 展开分支）/ hcomm_verb_sink.h（Dispatch 两 case + DoNhrRun 改造消费公共件）/ unified_route.h（expand 开关）/ pipeline_model.h（零改动——BY_CHANNEL 既有分支复用）/ semantic_checker.h（SLICE_REDUCE/CH_SYNC 动词级判定分支——**pattern 级 NHR special-case 保留至旧动词退役**，双形态都判）。
**跨线打卡**：common/kfc_server_message.h（verb ID 常量+ID_MAX）+ kfc_server/template/ccu/kernel/ccu_kernel_kfc_server.cc（两分支）——CCU 侧纯函数抽取单测覆盖（§五）。
**不动**：DoNhrRun 几何逻辑（改委托公共件，原语序 golden 锁）、mesh 全部、NHR_RUN 帧格式、R13/R15 golden。

**验收（2026-10-14 实测收尾）**：三入口 0 failed，断言基线 1920 → **2017**（run.sh 533→**542**：verb_exec R18 帧 golden +9 / st_checker 977→**987**：R18 段 +10 / exec 410→**488**：三遍跑法 +78——宏形态遍 94、展开遍 76（R18 段 9 + 门卫 skip PASS）、回归遍 69）。**对称性验收锚达成**：NHR 段展开形态逐动词 Emit（28 动词/8P、16 动词/4P nhr 段），checker 判据落在动词级（SLICE_REDUCE 贡献集等式——非 pattern 分叉）；等价锁双跑常跑（checker 双 PASS + 贡献集 WR/RD 各 7 对 == 步表展开手算清单 + 28 vs 10 双形态动词数对拍）；v5 帧 golden（16 帧逐槽 spot check + 种子段级补写 + Validate 两负例）；跨线打卡 kfc_server_message.h（verb ID 9/10 + ID_MAX 10 + 帧槽镜像两 enum）+ ccu_kernel_kfc_server.cc（VerbSliceReduce/VerbChSync 两分支 + 分发 + 兜底排除）——server 侧纯函数抽取单测 12 断言独立验证（R16 #6 同法）。

**实施期登记**（4 项）：①CH_SYNC 半边成对展开（每半边 Record+Wait 两动词，四态编码 size 槽 {0,1,2,3}）——M0 手算 22→28、16P nhr 段 12→16、DryRun 41→49（勘误已回写 §二/§三）；②既有断言合法更新一处：run.sh R12 探针 verb ids {0..8}→{0..10}（ID_MAX 升位是 M0 §四裁决的协议演进，非行为回归）；③exec run.sh 改**三遍跑法**（宏形态遍/展开遍/回归遍——两形态断言同进程互斥（env 定格），#29 灰度矩阵纪律的标准场景），T4/T5/R17 段加 expand 门卫（expand 开 skip）；④DoNhrRun 改造为消费 NhrSliceOf 公共件（几何单源——exec T4/T5 既有原语序 golden 全绿即证零漂移）。


---

## 八、全删轮验收记录（2026-10-14 终局——用户裁定"现在就全删，段切分兜底留到 B 形态启用时再验证"）

### 删除面（生产码）

| 位置 | 删除内容 | 保留面 |
|---|---|---|
| l5/hcomm_verb_sink.h | Dispatch 的 NHR_RUN case + DoNhrRun 本体（原 ：533-678 步循环内核） | SLICE_REDUCE/CH_SYNC 两 case gate 改 UseUnifiedNhrAicpuPath（转正）；ChannelOfIdx 查表 |
| l4/backend_aicpu_reduce_slice.h | NHR_RUN 发射块（段前屏障+宏动词+归位） | 展开分支转正唯一 Emit 形态；MakeSliceReduce/MakeChSync 便捷件 |
| l4/backend_ccu_reduce_slice.h | NHR_RUN 单帧发射 | 改产展开动词序（SYNC_PRE + 逐步[CH_SYNC×4 + SLICE_REDUCE×nSlices] + SYNC_POST——**CCU 无上料/归位 COPY 口径**，步进流本体在 server 帧外承载） |
| kfc_server/.../ccu_kernel_kfc_server.cc | VerbNhrRun（原 ：588-785 host 期重建步表 + gen 期四层循环）+ 分发 case + 兜底排除 | VerbSliceReduce（:789 起）+ VerbChSync（四态 dir 分派） |
| inc/verb_serializer.h | Serialize 紧凑帧分支（三帧布局）+ Validate 宏形态分支 | 展开分支转正；无 SLICE_REDUCE 的 NHR 段诚实报错；jettyNum∈[1,4] 校验迁入展开分支 |
| inc/unified_route.h | UseUnifiedNhrExpandPath（三层灰度蕴含式） | UseUnifiedNhrAicpuPath 保留（NHR AICPU 执行面灰度） |
| inc/verb_sink.h | — | **VerbCall::NHR_RUN 枚举值 8 保留**（ABI 值序空洞：SLICE_REDUCE=9/CH_SYNC=10 的 ID_MAX=10 互锁依赖；两映射表行改"已删形态·历史锚"注） |
| common/kfc_server_message.h | — | KFC_UNIFIED_VERB_NHR_RUN=8 常量 + KFC_RS_SOLE_NHR 五槽 static_assert 互锁保留（KFC_VERB_NHR_* 常量族） |

### 全删轮实施期修复（4 项——等价改写暴露的真缺漏）

1. **DoChSync 四态解码错位**（l5/hcomm_verb_sink.h :512-519）：旧解码 `idx=(size==0?ACK:DATA)` + `isWait=(size>=2)` 在 ACK-Wait(1)/DATA-Rec(2) 两态错位（ACK-Wait 误用 DATA idx 且误判 Record）。修复为位解码：半边=bit1、Record/Wait=bit0。旧断言只锁首条 Record 未及 Wait 侧——T5 展开序位锚暴露。
2. **SLICE_REDUCE 帧 peer 槽不落帧**（两 Emit MakeSliceReduce）：server VerbSliceReduce 的通道查表消费帧槽 4/5（peerWR/peerRD），便捷件未填 src/dst 致查表恒 0。修复：便捷件按步表填 st.sendTo/recvFrom。
3. **展开分支 SYNC_PRE/POST 段级补写缺失**（inc/verb_serializer.h）：紧凑帧分支删除后 NHR 口径掩码 0x5/pattern 槽 31/POST 掩码 0x10 无承载——server VerbSyncPre 掩码分派入口（0x5）依赖此补写。首版补写用 break 跳出整循环致尾部 SYNC_POST 漏补，改 seedWritten 标志。
4. **Validate/checker 上料归位校验分口径**（verb_serializer.h + semantic_checker.h）：CCU 展开无 COPY 口径下 upCount==n 必误拒——判据改动词级（序内有无 COPY，#28 纪律，两处同式）。Validate 的 jettyNum 校验从宏形态分支尾部迁入展开分支内部（宏序段形态优先报错）。

### 等价改写（宏形态断言退役——用户裁定，断言只增不减的显式例外）

| 测试段 | 旧断言（宏形态） | 新断言（展开形态） |
|---|---|---|
| st R13-① G1/G2/G3 | 3 帧 103 槽、帧1 verb id=8、槽5/6 jetty 公式 | 21 帧 679 槽、逐帧 verb id 21 锚、步0/1/2 SR peer 对 {2,4}/{1,5}/{7,7}、首 SR 帧种子槽 6..10、CH_SYNC dir/idx 冗余 |
| st R13-② jettyNum 负例 | 双拒（Validate+序列化器同文案） | 宏序段报"must be expanded form"（形态优先）+ 展开+jettyNum=0 独立拒（越界校验保留面） |
| st R13-④' A | seg1 Emit 3 动词、帧预算 16 | seg1 Emit 13 动词、帧预算 36（nhr 13/12 逐动词一帧） |
| st R13-④' B | 帧预算 54 ≤ 63 | **114 = 6×(6+13) > 63 拒**（B 形态 12 段全展开超限——段切分兜底触发点，M0 §五登记留 B 形态启用时验证） |
| st R17-② CCU | steps=16（5+3+2+6） | steps=36（5+13+12+6——与 R13-④' A 帧预算 36 两锚互证） |
| st R17-④ 锚2/3 | NHR CCU 3 动词帧 | 21 动词（1+8+6+5+1）+ checker 分口径 PASS |
| verb_exec V8 | NHR_RUN 帧展开 61 原语镜像 | 21 帧 dispatch：WVN10+NW16+NR11+WR7+EW7+LC0=51 条（模拟器加 SLICE_REDUCE/CH_SYNC 两镜像分支，NHR_RUN 镜像分支删）；7 WR 几何锚保持（tx 序同——等价锁传递） |
| verb_exec 未知 verb 反例 | id 9 点名拒绝 | **id 11**（合法域 0..10——探针二次迁移） |
| exec T4 | 6 动词 [COPY×4, NHR_RUN, COPY] | 16 动词 [COPY×4, (CH×2+SR×n+CH×2)×2, COPY]（AICPU 含 COPY 口径） |
| exec T5 | 序位 Rec(ch1,ACK)→Wait(ch3) | Rec(ch3,ACK)→Wait(ch3)（CH_SYNC 单动词单侧半边——peer 由方向选定；读侧对称翻转 ch1） |
| exec R17 | steps=29（8/6/6/9）、NHR 10 动词 | steps=49（8/16/16/9）、NHR 28 动词、SLICE_REDUCE 在表 |
| exec R18 段 | expand gate skip 门（三处） | gate 改 UseUnifiedNhrAicpuPath；④ 双形态对拍退役为三源一致锚（贡献集+checker+ST 基线） |
| exec run.sh | 三遍跑法（宏形态遍/展开遍/回归遍） | **两遍**（展开遍合一——expand 宏/env 全退役） |

### 新基线（2026-10-14 实测）

**1958 = run.sh 543 / st_checker 995 / exec 420，三入口 0 failed。**

较 R18 交付轮 2017 净 -59：宏形态断言等价改写退役（用户显式裁定例外）+ 三遍退两遍（宏形态遍整遍的 skip PASS 计数随遍数消失）。#31 三遍跑法条目随全删退化两遍（checklist 待同步）。

### 已知边界（登记）

- **B 形态帧预算**：12 段全展开 114 帧 > 63 上限（宏形态 54 可过）——段切分兜底（chunk 限定词）留 B 形态启用时验证（用户裁定 2026-10-14）。A 形态 36 帧正常。
- **CCU RD（DMA 拉）侧 server 分支**：VerbSliceReduce 首版 WR 单侧（peerRd 槽已进帧，RD 形态 P2 登记——AICPU 侧 DoSliceReduce 双侧已实现，等价由 checker 判）。
- **真机复核**：宏/展开两形态均未经真机验证（全删依据：无真机数据下双路径无保留价值）。真机 server 复核 P2 与既有登记同批。
