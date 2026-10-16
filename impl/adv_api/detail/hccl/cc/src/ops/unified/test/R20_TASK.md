# R20 任务书：L3 引擎知识出清（#13——CalcRes 双引擎反查上移 L2）+ 导读依赖规则澄清

> **立项**：2026-10-15 用户裁定（"各层之间应该单方向调用，这个层级调用关系有点乱"的检讨结论）——L3 动作头的 CalcRes 内嵌 `BackendRegistry::Peek(..., Engine::CCU/AICPU)` 双引擎反查，"有哪些引擎"的知识泄露到 L3。**设计债 #13 登记并本轮闭环**。
> **定位**：小轮——生产码一处搬迁 + 测试等价改写（少量）+ 导读两处文档（依赖规则澄清段 + 台账 #13）。
> **基线**：1973 = run.sh 543 / st_checker 1010 / exec 420，三入口 0 failed。

---

## 〇、铁律

1. 断言只增不减；**行为零变化**（claim 产物逐值不变——本轮是"谁去查表"的搬迁，不是申报语义变化，sum 断言全部原值保持）。
2. REVIEW_CHECKLIST 32 条适用，尤其 #16 单源（引擎枚举循环唯一落点）、#22 常跑对拍。
3. 期望值独立手算；源码考证给行号；默认不 commit。

---

## 一、问题与方案（已裁定）

**现状**（两处同型，本轮全部收编）：

- `l3/action_collect_all.h:62-67`：CalcRes 尾部两段 `Peek(CollectAll, CCU/AICPU)` → `ClaimEngineRes`
- `l3/action_reduce_slice.h:130-136`：同型（ReduceSlice）

L3 文件里出现 `Engine::CCU`/`Engine::AICPU` 字面量——"有哪些引擎"知识下漏。用户判定的分层规则：**L3 动作语义层不应知道引擎存在**（L1/L2 编排层、L4 引擎层、L5 执行层各管各的知识维度）。

**方案**：引擎反查循环上移到 **L2 `SegmentBuilder::CalcResOf`**（recipe_sole.h:249——它已是"逐段申报"的唯一汇合点，且 L2 本来就 include backend_registry.h、持有 Engine 概念（Validate 形参））：

```cpp
// l3 动作头 CalcRes：只剩引擎无关半边（通道），删两段 Peek
static ResourceClaim CalcRes(const ActionSpec& a) { ...通道循环... }

// l2 recipe_sole.h CalcResOf：
static void CalcResOf(const ActionSpec& seg, ResourceClaim& claim)
{
    switch (seg.kind) {
        case ActionKind::CollectAll:
            claim += CollectAll::CalcRes(seg);      // 引擎无关半边（通道）
            break;
        case ActionKind::ReduceSlice:
            claim += ReduceSlice::CalcRes(seg);
            break;
        ...
    }
    // R20 · #13：引擎半边收集上移——L3 不知道引擎存在（枚举循环唯一落点，
    // 新引擎 = 注册表加后端 + 本循环自动覆盖，L2 零改）
    for (u32 e = 0; e < kEngineCount; ++e) {   // 或枚举直列 {CCU, AICPU}——M0 定形态
        if (IActionBackend* be = BackendRegistry::Peek(seg.kind, static_cast<Engine>(e))) {
            be->ClaimEngineRes(seg, claim);
        }
    }
}
```

**为什么放 L2 CalcResOf 而不是别处**：① 它是全部 recipe（sole/parallel/sequence/split 四处 Expand）共用的唯一申报汇合点（recipe_registry.h:70/:151/:182 + recipe_sole.h:352 都调它）——搬这里四处自动生效；② L2 Compile 链已有 Engine 概念（Validate 形参、l1_adapter.h:22 include）；③ 声明面 `Engine` 枚举（backend_registry.h:22）是 L4 知识的**入口类型**，L2 引用入口类型查表与 Validate→Caps 同性质（知识归属规则：L2 编排决策需问 L4 能力/资源，L3 不需要）。

**等价性**：搬迁前后 claim 逐值不变——原 CalcRes 产物 = 通道半边 + 两引擎半边；新 = CalcRes 产通道半边、CalcResOf 补两引擎半边，**相加次序不变**（`+=` 对 ccu/kernels/channels 追加序、slaveThreadNum max 均与序无关的部分确认；M0 必答 1 给逐字段等价论证）。注意 LocalReduce/ExchangeAll 的 CalcResOf 分支（return 型不走 +=）——引擎循环对其行为：Peek(LocalReduce, e) 现状 nullptr（未注册）→ 循环体 no-op，行为不变；**循环须放在 switch 之后统一执行还是各 case 内执行**（LocalReduce case 是 return 型）——M0 必答 2 定形态（倾向 switch 后统一，LocalReduce 通道半边为空、引擎循环 no-op，零差）。

**Engine 枚举遍历形态**（M0 必答 2 一并定）：
- 候选 A：`for (u32 e = 0; e <= static_cast<u32>(Engine::CCU); ++e)`——依赖枚举连续性，加注释锁
- 候选 B：`constexpr Engine kAllEngines[] = {Engine::CCU, Engine::AICPU};` 数组直列——显式、加引擎=改一处
- 倾向 B（"显式决策非缺省"，与 ClaimFormulaOf switch 同哲学）。

## 二、实施清单

### 生产码（2 文件）

1. `l3/action_collect_all.h`：CalcRes 删两段 Peek（:62-67）→ 只剩通道循环；注释更新（"引擎半边由 L2 CalcResOf 统一收集（R20 · #13——L3 不知引擎存在）"）；`#include "backend_registry.h"` 检查是否还有其他消费（CollectAll 头若无其他引用则删 include——grep 确认）。
2. `l3/action_reduce_slice.h`：同型（:130-136）+ include 检查。
3. `l2/recipe_sole.h`：CalcResOf 尾部加引擎收集循环（形态 M0 定）；文件头注释补 R20 · #13 一句。

### 测试（等价改写 + 新增）

4. `test/st_verification.cc`：
   - **直调 CalcRes 的断言受影响面**（grep 核实共 3 处 + R19 段 1 处）：
     - :3808/:3824（R13-⑥ `ReduceSlice::CalcRes(seg)` 断 kernels 通道序）——**直调产物不再含引擎半边**！改写：断言移到经 `CalcResOf` 的产物上（kernels 断言语义保持，调 L2 入口），或 CalcRes 直调断言改"通道半边裸值" + 新增 CalcResOf 断言补引擎半边。M0 必答 3 逐条给改写式。
     - :4463（R16 段 `ReduceSlice::CalcRes(nhrSeg)`）同上。
     - :4955（R19-③ `SegmentBuilder::CalcResOf(ag4, mr)`）——已经是 CalcResOf 入口，值不变（AG 4P 两引擎半边slave=2 叠加不变），预计零改。
   - **新增 R20 段**（断言锚，期望值独立手算）：
     - ① L3 出清锁：`ReduceSlice::CalcRes(seg)` 产物 kernels 恒空（引擎半边不在 L3 产物里）+ channels = 建链集 5 条（引擎无关半边裸值）；`CollectAll::CalcRes` 同理（kernels 空、channels 7）。
     - ② CalcResOf 汇合锁：`CalcResOf(seg, claim)` 产物 == 旧 CalcRes 全量（kernels 1 条通道 5 / slave=7 等旧锚原值——**这正是行为零变化的验收锚**）。
     - ③ 引擎循环覆盖锁：CalcResOf 后 claim 同时含 AICPU 半边（slaveThreadNum==7）与 CCU 半边（kernels.size()==1）——两半边都在。
     - ④ include 出清自证：编译通过即证（L3 头不再消费 backend_registry——若 include 删除后编译过即为构造性证明）。
   - 受影响的"CCU plan claim carries BOTH engine halves"（:705）等**经 Compile 链的断言零触碰**（行为不变）。
5. `../unified_exec/test/poc_unified_tier_payload_test.cc`：grep CalcRes/CalcResOf——预计零触碰（tier 面不直调 L3 CalcRes）。
6. 三入口 run.sh 零改动预期。

### 文档

7. `test/M0_R20_L3_ENGINE_EXIT.md`（新）：必答三题 + 验收记录。
8. `test/REVIEW_CHECKLIST.md`：新增 #33（L3 引擎知识出清纪律——L3 动作头不出现 Engine 枚举字面量，引擎半边收集唯一落点 L2 CalcResOf；新引擎 = 注册表加后端 + kAllEngines 加一项）。
9. 导读两处：
   - **§1.3 五层表后加"依赖规则澄清"段**（本任务书附录 A 的内容为底稿，改写为导读体）：知识归属/编译依赖/执行调用三概念区分 + "查询调用 vs 执行调用"两性质 + L2/L3 查表定位（编排决策问 L4 能力是同性质）+ #13 出清后的新格局（L3 零引擎知识）。
   - **§十一 台账新增 #14 条目**（#13 出清记录，格式照 #12）。
   - §3.5/§5.1 若有 CalcRes 描述漂移顺手修（grep "CalcRes" 导读核对）。
10. 方案文档：§6.5 资源申报段若引 CalcRes 双引擎反查则同步（grep 核实）；R20 执行记录追加（验收数字）。

## 三、显式不做

- L2 Validate→Caps 查表**不动**（知识归属规则内——L2 编排决策问能力，与 #13 不同性质，已在澄清段定位）。
- L5 → Emit 不动（生产者-消费者，方向正）。
- ExchangeAll/LocalReduce 后端注册（P2 既有，不随本轮）。
- claim.ccu/notify 族任何语义调整（零行为变化红线）。

## 四、M0 必答（先答后动代码）

1. **逐字段等价论证**：搬迁前后 claim 的 ccu map/kernels/channels/slaveThreadNum/notify 三件套逐字段对比（含 `+=` 追加序在两引擎间的次序是否可观察——channels 追加序：原序 = L3 通道循环 → CCU kernels 通道 → AICPU（AICPU ClaimEngineRes 不动 channels）；新序相同即零差）。
2. **引擎循环形态**：kAllEngines 数组（倾向）vs 枚举界循环；LocalReduce/ExchangeAll case（return 型）下循环的落位（switch 后统一 vs 各 case）。
3. **直调断言改写表**：st :3808/:3824/:4463 三处逐条给改写式（新断言文本 + 期望值手算）。

## 五、验收

- 三入口 0 failed；新基线实测（预计 1973 + R20 新增段数）。
- 行为零变化锚：②号断言（CalcResOf 产物 == 旧全量）PASS 即验收。
- git 触碰面 = §二清单文件；跨线零触碰自证。

---

## 附录 A：导读 §1.3 依赖规则澄清段（底稿）

**五层的依赖规则——知识归属，不是封装栈**：经典分层（OSI 式）规则是"每层只见下一层"，因为每层是上层的实现细节。本架构的五层是**知识维度的切分**：编排知识（L1/L2）/动作语义知识（L3）/引擎知识（L4）/发射执行知识（L5），**每类知识只有一个居所，谁需要谁引用**。三条具体规则：

1. **编译依赖单向朝下/朝公共层**（grep 事实：l2/l3/l5 都 include backend_registry.h，无反向）——Engine 枚举与注册表是 L4 知识的**公共入口类型**，供上层引用。
2. **执行调用只有一条方向链**：L5 消费动词序 → L4 生产（Emit）→ 其余层不构成执行路径。
3. **查询调用是"决策问知识"**，允许跨层：L2 Validate 问 L4 能力（Caps——编排决策"这个引擎做不做得动"）；L2 CalcResOf 收集 L4 资源申报（ClaimEngineRes）。查询不产生执行依赖。**#13 出清后（R20）**：L3 动作头零 Engine 字面量——引擎半边收集唯一落点 L2 CalcResOf，L3 彻底不知道引擎存在。
