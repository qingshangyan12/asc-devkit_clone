# M0_R20 · L3 引擎知识出清（设计债 #13——CalcRes 双引擎反查上移 L2）

> 立项 2026-10-15（用户裁定"各层之间应该单方向调用，这个层级调用关系有点乱"）。
> 任务书 unified/test/R20_TASK.md。本 M0 先答后动：期望值全部独立手算，不转抄
> 生成方输出。行号以 2026-10-16 本轮 grep 为准（任务书行号逐一核对无漂移：
> action_collect_all.h:62-67 / action_reduce_slice.h:130-136 / recipe_sole.h:249 /
> st :3808/:3824/:4463/:4955 均与现状一致）。

---

## 一、必答 1：逐字段等价论证（行为零变化）

**搬迁形态（本轮定稿）**：不是"引擎半边直写累积器"，而是**段本地 segClaim 先
聚齐再 `+=` 进累积器**——原 L3 CalcRes 的局部 claim 构造语句（通道循环 → CCU
Peek+ClaimEngineRes → AICPU Peek+ClaimEngineRes）逐字上移到 L2 CalcResOf，作用
对象从"L3 局部变量"换成"L2 段局部 segClaim"，随后 `claim += segClaim` 与原
`claim += L` 同式。**段本地 claim 逐语句同序同值 ⇒ 累积器逐段同值 ⇒ 终值逐字段
相同**。逐字段：

| 字段 | 原产物 L（段本地） | 新 segClaim（段本地） | `+=` 合并语义 | 等价性 |
|---|---|---|---|---|
| channels | L3 通道循环（NHR 建链集 / mesh 全连接跳 me） | 同一循环（L3 头原样保留） | 追加（append） | 同序同值；CCU/AICPU 的 ClaimEngineRes **均不触 claim.channels**（CCU 只写 kernels[*].channels——kernel plan 内部清单），故段内追加序 = 通道循环序，两形态一致 |
| kernels | CCU 条目 push（"CcuActionReduceSlice/CollectAll" + 通道组） | 同一 push（backend 代码零改动） | 追加 | 同值；AICPU 后端不 push kernels，段内至多 1 条，段间按段序追加——两形态一致 |
| ccu[die][grp] | CCU 条目 `ccu[0][0] += req` | 同一语句 | map 逐键 `+=`（整数和，可结合） | 同值（RS mesh 8P：{loop=1,ccuBuf=1,variable=26,event=16,instruction=1}；AG 8P：variable=18,event=1） |
| slaveThreadNum | CCU 置 0 → AICPU 置 t−1（**后写覆盖**）⇒ L.slave = t−1 | 同两次直赋（kAllEngines 序保持 CCU→AICPU）⇒ segClaim.slave = t−1 | `max(claim, o)` | 同值：max(claim, t−1) 两形态同式 |
| notifyNumOnMainThread | CCU 置 0 → AICPU 置 t−1 ⇒ L = t−1 | 同 ⇒ t−1 | `+=`（和） | 同值 |
| notifyNumPerThread | AICPU `assign(t−1, 1)`（CCU 不触） | 同 ⇒ [1]×(t−1) | 追加 | 同值 |

其中 t = DomainThreadTotalNeed(seg)（AG→max(1,n−1)、RS·mesh→max(1,n)、
RS·NHR→max(1,chan)）。

**两引擎间 channels 追加次序的可观察性**：不可观察——两引擎 ClaimEngineRes 都
不写 claim.channels（通道半边唯一来源是 L3 循环），不存在"两引擎各追加一段
channels"的交错面。kernels 的段间序由段序决定（每段至多 CCU 一条），两形态
一致。

**边界 case（三处）**：

- **LocalReduce**：原 CalcResOf `case LocalReduce: return;`（不调 L3 CalcRes、
  不反查）。新形态 case 改 break、统一走引擎循环：Peek(LocalReduce, CCU/AICPU)
  现状均 nullptr（后端未注册，P2）→ 循环体 no-op；`claim += 空 segClaim` 逐字段
  无操作（max(x,0)=x、x+0=x、空追加）。**零差**。且未来 LocalReduce 后端注册后
  自动纳入申报（原 return 形态会静默漏报——"缺省即漏报"陷阱，本轮一并消除）。
- **ExchangeAll/LocalCopy（default）**：原内联通道循环、不反查。新同循环入
  segClaim + 引擎循环 no-op（未注册）。**零差**。
- **未注册 (kind, engine) 组合**：Peek nullptr 跳过——与原 L3 两段 if 的守卫
  语义逐字相同。

**结论**：claim 逐值不变；既有全部经 Compile 链/CalcResOf 的断言（:678-691
AG 6 锚、:704 双半边锚、:717-725 chunk2 14 通道/2 kernel/notify 13 锚、expand
test two_shot 2 kernel 锚、R19-② sole RS 7 锚）全部原值保持——这正是 ② 号
验收锚的对象。

---

## 二、必答 2：引擎循环形态（kAllEngines 数组 + switch 后统一）

**形态定稿**：`constexpr Engine kAllEngines[] = {Engine::CCU, Engine::AICPU};`
（recipe_sole.h 命名空间域直列数组），循环放 **switch 之后统一执行**（各 case
由 return 改 break，LocalReduce 注释保留）。

**数组直列 vs 枚举界循环（候选 B vs A）——B 不是风格偏好，是行为约束**：
Engine 声明序为 `enum class Engine { AICPU, CCU }`（backend_registry.h:22，
AICPU=0）——**枚举界循环（`e <= Engine::CCU`）会先 AICPU 后 CCU**，而原 L3
CalcRes 的 Peek 次序是 **CCU 先、AICPU 后**。线程字段是"后写覆盖前写"
（CCU 置 slave=0/notifyMain=0，AICPU 置公式值）：倒序后 CCU 的 0 成为段本地
终值 ⇒ claim.slaveThreadNum 恒 0 ⇒ :678 AG 6 锚、:704 双半边锚、R19-②
sole RS 7 锚全灭。**候选 A 即行为变化**，故取 B；数组序 {CCU, AICPU} 是行为
契约（与 R19 ClaimFormulaOf switch 同哲学：显式决策非缺省——新引擎 = 注册表
加后端 + 本数组加一项，L3/L2 case 均零改）。

**循环落位（switch 后统一 vs 各 case 内）——统一**：LocalReduce/ExchangeAll
现无注册后端，两落位现状零差；统一落位使"引擎半边收集"无条件执行，后端
注册即纳入（各 case 内落位则每个新 case 都要记得抄一遍循环——缺省漏报面）。
LocalReduce 通道半边为空 + 引擎循环 no-op，零差（必答 1 边界 case 已证）。

**任务书伪码勘误（登记，实施已修正）**：任务书 §一伪码 `be->ClaimEngineRes
(seg, claim)` **直写累积器**——ClaimEngineRes 对 slave/notify 是直赋非累加
（CCU 置 0 / AICPU 置公式值），多段场景（chunk2 / two_shot）直写会破坏
operator+= 的 max/追加合并语义（chunk2 notifyMain 12→6、two_shot
max(7,6)→6——两条既有锚必断）。实施改为**段本地 segClaim 聚齐后一次
`claim += segClaim`**（必答 1 形态），语义与原 `claim += L` 逐字同构。

---

## 三、必答 3：直调断言改写表（st_verification.cc，行号本轮核实）

| # | 位置 | 现状 | R20 后 | 改写式（新断言文本 + 手算期望） |
|---|---|---|---|---|
| 1 | :3808 `claimNhr = ReduceSlice::CalcRes(seg)` + :3809/:3813-3820 kernels 断言 | 断 kernels.size()==1 && kernels[0].channels.size()==5 + 序 [1,2,4,5,7] | 直调产物只剩通道半边，kernels 恒空 → **原断言必断** | 拆两半：① L3 裸半边——`claimNhr.channels.size()==5`（手算：NHR me=3 建链集 {1,2,4,5,7} 五条，remoteRank 序 [1,2,4,5,7]，count 均 1=channelsPerRank 缺省）+ 裸序断言同 [1,2,4,5,7]；② L2 汇合——`mergedNhr`（经 `SegmentBuilder::CalcResOf(seg, mergedNhr)`）断 `kernels.size()==1 && kernels[0].channels.size()==5` + kernels[0].channels 序 [1,2,4,5,7]（CCU ClaimEngineRes NHR 分支同建链集公式——原断言语义在 L2 入口原样保持） |
| 2 | :3824 `claimMesh = ReduceSlice::CalcRes(mesh)` + :3825 kernels 断言 | 断 kernels.size()==1 && kernels[0].channels.size()==7 | 同上 | 同拆：L3 裸半边 `claimMesh.channels.size()==7`（手算：mesh 全连接跳 me=3 → [0,1,2,4,5,6,7] 七条）；L2 汇合 `mergedMesh.kernels.size()==1 && mergedMesh.kernels[0].channels.size()==7`（原断言原值保持） |
| 3 | :4463 `claim = ReduceSlice::CalcRes(nhrSeg)` + :4467 | **只读 claim.channels**（gotRanks == {1,2,4,5,7}） | 通道半边仍在 L3 CalcRes 产物里 → **原断言原值成立** | **零改**（任务书预计"同上"过估——该断言不触引擎半边；:4464 注释"CalcRes NHR 分支经单源函数产出：channels=..."仍准确）。登记任务书勘误 |
| 4 | :4955 `SegmentBuilder::CalcResOf(ag4, mr)` + :4956 | CalcResOf 入口，断 cs==2 && mr.slaveThreadNum==2 | CalcResOf 内部加了引擎循环——AG 4P 手算：CCU 半边 kernels 1 条/ccu{variable=4*2+2=10,event=1}/slave 置 0，AICPU 半边 t=max(1,3)=3 → slave=2/notifyMain=2/perThread=[1,1]；`mr += segClaim` → mr.slave=max(0,2)=**2** | **零改**（任务书预计零改——手算复核通过，原值 2 保持） |

**净效果**：改写仅 #1/#2 两块（3 条 Check → 6 条，+3），#3/#4 零触碰；新增
R20 段 10 条（下节）。

---

## 四、新增 R20 段锚（段号 64，四组）

手算基线（全部独立推导，backend 公式源：backend_ccu_collect_all.h:37-71 /
backend_ccu_reduce_slice.h:52-96 / action_spec.h:301-329）：

- **① L3 出清锁（4 条）**：AG mesh 8P me=0 直调 `CollectAll::CalcRes`：
  kernels 空 + channels 7 [1..7]；slave=0/ccu 空/perThread 空（L3 不触引擎申报
  面全零）。RS mesh 8P me=0 直调：kernels 空 + channels 7。RS NHR 8P me=3
  直调：kernels 空 + channels 5 [1,2,4,5,7]。
- **② CalcResOf 汇合锁（4 条，行为零变化验收锚）**：RS mesh 8P me=0 经
  CalcResOf：channels 7 + kernels 1 条 "CcuActionReduceSlice" 7 通道（旧
  R13-⑥ mesh 锚原值）；slave=7/notifyMain=7/perThread 7（== R19-② sole RS
  8P 旧锚——t=max(1,8)=8）；ccu[0][0]={loop=1,ccuBuf=1,variable=8*3+2=26,
  event=16*ceil(8/16)=16,instruction=1}（R16-#6 公式锚原值）。AG mesh 8P 经
  CalcResOf：slave=6 + kernels 1 + channels 7（== :704/:678 旧锚——t=7）。
- **③ 引擎循环覆盖锁（2 条）**：RS mesh 8P CalcResOf 产物同时含 AICPU 半边
  （slave=7）与 CCU 半边（kernels=1）；RS NHR 8P 同查两引擎——kernels=1（CCU
  半边在场）+ slave=0 且 perThread 空（AICPU 公式值 t=max(1,1)=1 → slave=0，
  **申报 0 ≠ 缺半边**——NHR 单线程域无从线程是公式真值）。
- **④ include 出清自证**：编译通过即构造性证明（L3 两头删 `#include
  "backend_registry.h"` 后，若仍消费 Engine/BackendRegistry 则三入口不可编；
  recipe_sole.h 显式补 include——原经 action_collect_all.h 传递可见的隐式依赖
  显式化）。无运行期断言。

---

## 五、改动面清单（实施规格）

**生产码 3 文件**：

1. `l3/action_collect_all.h`：CalcRes 删两段 Peek（:61-67）→ 只剩通道循环；
   接口①注释改"引擎半边由 L2 CalcResOf 统一收集（R20 · #13）"；删 `#include
   "backend_registry.h"`（grep 确认本头唯一消费方即该两段 Peek）。
2. `l3/action_reduce_slice.h`：同型（:130-136）+ include 删除（本头其余符号
   不依赖注册表）。
3. `l2/recipe_sole.h`：命名空间域加 `kAllEngines` 直列数组（注释含"序=行为
   契约"论证）；CalcResOf 改 segClaim 形态 + switch 后统一引擎循环（case 改
   break）；显式 `#include "backend_registry.h"`（Validate :426 既有消费 +
   新循环——原经 L3 头传递可见，出清后显式化）；文件头注释补 R20 · #13 一句。

**测试**：st_verification.cc——R13-⑥ 块改写（3→6 条）+ 新增 R20 段 64（11
条）+ 前置声明/调用各一行；其余文件零触碰（grep 证：unified_exec/test/ 六个
.cc 无 CalcRes 直调；expand/serialize/verb/kfc_gate/aicpu_exec/verb_sink 各测
试的 claim 断言全经 Compile 链，值不变）。

**文档**：本 M0 + REVIEW_CHECKLIST #33 + 导读（§1.3 依赖规则澄清段 / §3.4
CalcResOf 口径 / §4.1/§4.2 CalcRes 口径 / §十一-14 交付记录）+ 方案文档
（§6.5 伪码同步注 / §10.1 表 [CollectAll::CalcRes] 行 / R20 执行记录）。

---

## 六、验收记录（2026-10-16 实测收尾）

**行为零变化锚**：②号断言（CalcResOf 产物 == 旧全量：RS mesh 8P kernels 1
条 7 通道 / slave=7 / ccu 计数组、AG 8P slave=6+kernels=1）全 PASS；既有经
Compile 链断言（:678/:704/:717-725 chunk2、expand two_shot 2 kernel、R19-②
sole RS 7）零修改原值通过。

**新基线**：**1987 = run.sh 543 / st_checker 1024 / exec 420，三入口 0 failed。**

较 1973 净 +14，分解逐入口实测对账：

- **st_checker 1010 → 1024（+14）**：R13-⑥ 块改写 3→6 条（+3：L3 裸半边
  channels NHR 数量+序、mesh 数量三条新增；kernels 断言移 L2 CalcResOf 入口
  原语义保持）+ 新增 R20 段 64 共 11 条（① L3 出清锁 4——AG 两态/RS mesh/RS
  NHR；② CalcResOf 汇合锁 5——RS 全量+slave+ccu 计数组、AG 全量+ccu；③ 引擎
  循环覆盖锁 2。④ include 出清自证为编译期证明无断言。立项时预估 10 条，
  实施将 ② 组 RS/AG 的 ccu 计数组拆独立断言成 11——净差 +1 在此）。
- **run.sh 543 → 543（±0）**：七个目标全绿零改动（claim 断言全经 Compile 链，
  逐值不变）。
- **exec 420 → 420（±0）**：grep 证 unified_exec/test/ 无 L3 CalcRes 直调，
  tier_payload 等零触碰（任务书预计兑现）。

**git 触碰面**：生产码 3（l3 两头 + recipe_sole.h）/ 测试 1（st_verification.cc）/
文档 5（本 M0、REVIEW_CHECKLIST、导读、方案文档、——见 §五清单）；跨线零触碰
（unified_route.h / aicpu_consumer.h / verb_sink.h / verb_serializer.h /
kfc_server_message.h / server / blob / op_common.cc 全不动——工作树另有 48
文件未提交改动系 R11-R19 历轮存量，本轮净增仅上列 4+5）。

**任务书偏差及处置**（4 项）：
1. 任务书 §一伪码引擎半边直写累积器——多段场景破坏 max/追加合并语义（§二勘
   误），实施改段本地 segClaim 聚齐后 `+=`（必答 1 形态）。
2. :4463（R16-60c）任务书列受影响面"同上"——实读仅通道半边，零改（必答 3 #3）。
3. :4955（R19-③）预计零改——手算复核 AG 4P slave=2 原值成立，零改兑现。
4. 任务书行号全部核实无漂移（action_collect_all.h:62-67 / action_reduce_slice.
   h:130-136 / recipe_sole.h:249 / st :3808/:3824/:4463/:4955）。

**已知注释漂移不入触碰面**：poc_unified_expand_test.cc :641 注释"CalcRes 汇总
两引擎申报"（改后汇总点为 CalcResOf）——断言零变化、该文件不在任务书 §二清
单，登记不改（下轮顺手修）。
