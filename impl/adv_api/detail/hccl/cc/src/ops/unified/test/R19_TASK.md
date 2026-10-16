# R19 任务书：线程申报策略查表绑定（#12——编排方式 → 申报公式映射表）

> **立项**：2026-10-15 用户裁定——"公式是编排方式的属性，不是全局开关；老的算法逻辑不变；指定某种编排方式时就绑定其对应的 SumRes 方式，做一个 map 根据 executor 类型映射查到不同计算方式"。**B 档裁定**：PARALLEL 报编排器实需（Σ(need−1)+(D−1)），比老树少 1 条专职控制位。
> **问题定性**：设计债 #12（导读 §十一-13 已落盘）——unified 的 slaveThreadNum 公式选择挂在全局 env（UseUnifiedConcurrencyPath）上，配方身份不参与；老树公式逐编排方式各异，任何一档 env 都不能让四配方同时老等价。
> **台账**：导读 §十一-13；方案文档 R19 立项记录（:1713 后追加段）。

---

## 〇、铁律（先读，违者整轮打回）

1. **断言基线 1958**（run.sh 543 / st_checker 995 / exec 420）。三入口 `0 failed`。
2. **等价改写例外（R18 全删轮先例）**：本轮是**公式设计变更（B 档用户裁定）**，既有 claim 值断言按 §四清单逐条改写（旧值→新值+手算依据），在 M0 登记"只增不减"显式例外——不是回归是设计变更。**改写仅限 §四清单内的断言**，其余断言零触碰。
3. REVIEW_CHECKLIST 31 条全部适用，尤其 #16 单源（公式实现唯一落点 action_spec.h）、#23 判别式（claim 不进 blob——v5/server/kfc 零触碰）、#26 公式先补再对拍。
4. 期望值独立手算（M0 必答，不转抄生成方）；源码考证给文件+行号；默认不 commit。
5. **零协议改动**：claim 是 host 侧 Plan 字段，不序列化进 blob（plan_serializer 只写 segments+tier）——本轮不碰 verb_serializer/kfc_server_message/server。

---

## 一、方案（已裁定，勿再讨论）

**老树公式全景（AG/RS 两树同族，已实证行号）**：

| 编排方式 | 老公式 | 老树证据 |
|---|---|---|
| sole | 模板 CalcRes 直通（无合并层） | ins_v2_all_gather_sole_executor.cc:44（AG）；ins_v2_all_reduce_sole_executor.cc:64（AR） |
| parallel | `intra + inter + 2`（专职控制位） | RS ins_reduce_scatter_parallel_executor.cc:76-77；AG ins_v2_all_gather_parallel_executor.cc:130-131；AR ins_v2_all_reduce_parallel_executor.cc:120（two_shot 两模板/域 → +4） |
| split | `temp0 + temp1 + 1`（域0 main 兼控制） | RS ins_reduce_scatter_concurrent_executor.cc:68-69；AG ins_v2_all_gather_concurrent_executor.cc:89 |
| sequence | `max(inter, intra)`（池复用） | RS ins_v2_reduce_scatter_sequence_executor.cc:84；AG ins_v2_all_gather_sequence_executor.cc:66 |

**目标形态**：

```
Recipe（编排方式，alg_config_parser.h:31 既有枚举——不发明新维度）
  → ClaimFormulaOf(Recipe) 查表（l2，switch 全枚举覆盖——-Werror 漏 case 即编译错，
     新枚举值强制显式决策）
  → { MAX_REUSE, CONCURRENT_SUM, REJECTED } 三档
```

- **MAX_REUSE**（SOLE / SEQUENCE）：现状 operator+= max 累积（action_spec.h:224）即老树公式，**零改动**——逐段 CalcResOf 累积结果就是最终 claim。
- **CONCURRENT_SUM**（SPLIT / PARALLEL）：`slave = Σ_d(need_d − 1) + (D − 1)`（D=域数；GroupDomains 去重；逐域 need 取域内段 max——CalcConcurrencySlaveClaim 既有口径）。SPLIT 与老树 `+1` **恰同式零差**；PARALLEL 比老树 `+2` **少 1**（B 档：专职控制线程职能已被编排器域 main 交叉握手接管，恒 ≤ 老树申报量）。
- **REJECTED**（PIPELINE）：registry Get 本就返回 nullptr（recipe_registry.h:203），Expand 不可达——查表行登记"裁剪不实现"，函数内 unreachable 注释。

**单域退化恒等式（一致性锚）**：D=1 时 CONCURRENT_SUM = (need−1)+0 = need−1 = 单域 MAX_REUSE——两档在单域配置下恒等（断言锁）。

**门控处置**：`UseUnifiedConcurrencyPath` **退役出申报面**（SumRes 不再读它）；**执行面编排器灰度保留**（aicpu_consumer.h:667 分叉一行不动——R14 新机构独立放量开关，真机验证后整体退役）。跨进程失配从"公式错配"收窄为"执行面机器新旧"，且申报恒按编排方式报足（编排器没开时跑串行 for，多报线程闲置无害——M0_R15 §九失配安全性同向）。

---

## 二、实施清单

### 生产码（5 文件）

1. **`inc/action_spec.h`**（公式唯一实现点）：
   - `CalcConcurrencySlaveClaim`（:321）公式改：尾部 `+ static_cast<u32>(domainRanks.size())` → `+ (domainRanks.size() > 0 ? domainRanks.size() - 1 : 0)`；**空 segs 守卫**（domainRanks.empty() → return 0）。
   - 函数头注释块（:283-295 区域）更新：R19 · #12 B 档口径（Σ(need−1)+(D−1)——控制位由编排器域 main 承接；SPLIT 与老树 +1 同式、PARALLEL 比老树 +2 少 1；D=1 退化 = MAX_REUSE 自然值；消费方 = SumRes 查表 CONCURRENT_SUM 档）。
2. **`l2/recipe_sole.h`**（查表 + 分派）：
   - SegmentBuilder 上方新增 `enum class ClaimFormula { MAX_REUSE, CONCURRENT_SUM, REJECTED }` + `inline ClaimFormula ClaimFormulaOf(Recipe r)`（switch 全枚举覆盖；每 case 注老树公式与行号——§一表的镜像）；文档块说明"新编排方式 = 加一行 case，REJECTED 是显式决策非缺省"。
   - `SumRes` 改签名 `static void SumRes(Plan& plan, Recipe r)`（:225）：逐段 CalcResOf 累积不动 → 尾部删 `if (UseUnifiedConcurrencyPath())` gate 块（:234-237），换成 `if (ClaimFormulaOf(r) == ClaimFormula::CONCURRENT_SUM) { plan.claim.slaveThreadNum = CalcConcurrencySlaveClaim(plan.segments); }`（REJECTED unreachable——registry 已拒；MAX_REUSE 累积即终值零动作）。
   - `RecipeSole::Expand` 的调用点（:352）→ `SumRes(plan, Recipe::SOLE)`。
   - :23 `#include "unified_route.h"` 若无其他使用则删（grep 确认）。
3. **`l2/recipe_registry.h`**：三处调用点传身份——:70（RecipeParallel::Expand）→ `Recipe::PARALLEL`；:151（RecipeSequence）→ `Recipe::SEQUENCE`；:182（RecipeSplit）→ `Recipe::SPLIT`。
4. **`l5/segment_orchestrator.h`**：:43-44 头注（"ResourceClaim 求和取 max 是串行复用假设，双域并发需 Σ 而实给 max"——已过时，R19 后 parallel/split 恒按 Σneed 申报）与 :231 降级 warning 文案（"(parallel domains need sum; claim grants max)" → 去 "claim grants max" 断言性表述，改中性 "pool smaller than sum of domain needs"）同步修正。**逻辑零改动**（降级判据 Σneed vs 池不变）。
5. **跨线零触碰声明**：unified_route.h / aicpu_consumer.h / verb_sink.h / 序列化器 / server 全不动。

### 测试（等价改写 + 新增）

6. **`test/st_verification.cc`**：§四清单改写 + 新增 R19 段（§五锚）。
7. **`../unified_exec/test/poc_unified_tier_payload_test.cc`**：T7 三处改写（§四）。
8. **`test/run.sh` / `test/run_st_checker.sh` / `../unified_exec/test/run.sh`**：**零改动预期**（tier_payload 编译宏 TIER+NHR_AICPU 不变、env 跑法不变——CONCURRENCY 探针测的是执行面 gate，gate 保留故探针有效）。

### 文档（收尾）

9. `test/M0_R19_THREAD_CLAIM.md`：必答 + 验收记录（删除面/改写清单/新基线）。
10. `test/REVIEW_CHECKLIST.md`：新增 #32（申报公式与编排方式绑定纪律）。
11. 导读漂移修正：:36（Σ 申报行）、:173（UseUnifiedConcurrencyPath 行——"Σ 申报"职责摘除，仅执行面）、:317（"线程取 max"注）、:342、:1857（"Σ 申报若 CONCURRENCY 开"）+ §十一-13 补交付记录。
12. 方案文档：R19 立项记录后补执行记录（验收数字）。

---

## 三、显式不做

- **执行面 gate 删除**：不做——编排器机器灰度独立保留（真机验证后退役）。
- **notify 族合并逻辑**：不动——维持 M0_R15 §九"超配安全"口径（notifyNumPerThread 拼接 size 可能 < slaveThreadNum，现状既有，本轮不扩面；M0 必答 4 确认登记）。
- **AR parallel（split[mesh.two_shot,nhr]）的 +4 vs +1 差异**：只登记 M0 不实现（AR 路由未激活；unified 段模型每域 1 main vs 老树每模板 1 main，two_shot 两模板/域 → 老树 +4、unified B 档 +1——恒 ≤ 老树，B 档原则自然结果）。
- **CCU 侧 claim**：不动（CCU 微码自驱动 slaveThreadNum 恒 0——backend_ccu_*.h:39/:54 既有）。
- **blob/协议/server**：零触碰（铁律 5）。

---

## 四、既有断言等价改写清单（精确——全部因公式变更，逐条手算）

| # | 文件:行 | 现断言 | 新断言 | 手算 |
|---|---|---|---|---|
| 1 | st_verification.cc:4324 | `CalcConcurrencySlaveClaim(segs) == 5`（AG 5P need4 + NHR 6P chan1 need1） | `== 4` | (4−1)+(1−1)+(2−1) = 3+0+1 = 4 |
| 2 | st_verification.cc:4330 | 单域 AG 4P `== 3` | `== 2` | (3−1)+(1−1) = 2（D=1 退化 = need−1 = MAX_REUSE 自然值——改写标签注明恒等锚） |
| 3 | st_verification.cc:4346 | 16P tier 展开 `== 5` | `== 4` | mesh need4 + nhr need1，D=2 → 3+0+1 = 4 |
| 4 | st_verification.cc:4357-4358 | `plan.claim.slaveThreadNum == 3`（"串行 claim = max(3,3)——现状回归"） | `== 4`（标签改"parallel 配方 CONCURRENT_SUM = 4——R19 查表，env/宏无关"） | 同 #3（本 TU 宏关——改后公式与宏无关，4 恒成立） |
| 5 | st_verification.cc:4482 | R16-8c 两 8P 域 `== 14` | `== 13` | (7−1)+(7−1)+(2−1) = 6+6+1 = 13 |
| 6 | tier_payload_test.cc:682 | T7 `CalcConcurrencySlaveClaim(segs) == 5` | `== 4` | 同 #1（同构造——两域 AG 5P need4 + NHR 6P chan1 need1） |
| 7 | tier_payload_test.cc:689 | T7 16P tier `== 5` | `== 4` | 同 #3（mesh need4 + nhr need1，D=2 → 3+0+1） |
| 8 | tier_payload_test.cc:695-703 | `if (!UseUnifiedConcurrencyPath()) {==3} else {==5}` 门卫双分支 | 删门卫，单断言 `== 4`（标签"T7 parallel 配方 CONCURRENT_SUM = 4（3+0+1——R19 查表，与 env 无关）"） | 同 #3；宏关 TU 两跑恒 4 |

**不改的相邻断言（复核后确认不受影响）**：st :676/:702/:721（sole 8P AG slave=6——MAX_REUSE 单域不变）、:687/:730（Materialize threads:7/notify——SOLE 不变）、:4419/:4428（R16-8g backend ClaimEngineRes——动作级单源不变）、:4175/:4356（gate 恒 false 断言——UseUnifiedConcurrencyPath 函数保留、执行面 gate 不删）；tier_payload T7 池 6/池 5 切池断言（编排器 Σneed=5 不变——DomainThreadTotalNeed 零改动；标签"Σ 5"指编排器 totalNeed，语义仍真，可顺手改"Σneed 5"消歧）；R13-④'/R14/R17/R18 各 parallel compile 断言（无 claim 面）。

---

## 五、新增断言锚（R19 段，期望值独立手算）

落 st_verification.cc 新段（`== 60 R19 · #12 线程申报查表 ==` 编号顺延实际段号）：

1. **查表行锁**（5 断言）：`ClaimFormulaOf(Recipe::SOLE)==MAX_REUSE` / `PARALLEL==CONCURRENT_SUM` / `SEQUENCE==MAX_REUSE` / `SPLIT==CONCURRENT_SUM` / `PIPELINE==REJECTED`。
2. **逐配方 compile 端到端**（16P 双域 hier {{0,1,2,3},{0,4,8,12}}，RS op 同 R15-③ 构造，本 TU 宏关）：
   - `parallel[mesh,nhr]` → slave == 4（CONCURRENT_SUM：3+0+1）
   - `sequence[mesh,nhr]` → slave == 3（MAX_REUSE：max(mesh 3, nhr 0)——老 sequence max(inter,intra) 同值）
   - `split[mesh,nhr]` → slave == 4（CONCURRENT_SUM 与老树 temp0+temp1+1 = 3+0+1 **同值零差锚**）
   - `sole[mesh]` → slave == 7（RS 8P 单段 need 8——既有 :676 AG 6 锚的 RS 侧补位；若 8P sole[mesh] RS 在本构造不可编（路由/Validate），降级为直调 `ClaimAicpuThreadRes` 断 need−1，注明）
   - split/sequence 若 L1 直编报 NOT_SUPPORT（Validate 未放行多域非 parallel），改用 `RecipeRegistry::Get(...)->Expand` 直调（expand_test.cc:141 范式）+ 手工 SumRes，断言同值。
3. **单域退化恒等式**（1 断言）：`CalcConcurrencySlaveClaim(one) == 单域 MAX_REUSE 累积值`（AG 4P：2 == 2）。
4. **空 segs 守卫**（1 断言）：`CalcConcurrencySlaveClaim({}) == 0`。
5. **老树对拍表注记**（M0 落盘，断言承载 2/3 的同值锚即可）：parallel B=4 vs 老树 5（8P RS mesh+NHR 例：老 7+0+2=9 slaves？**注意**——8P 例老树两域为 mesh 8P(n=8→slave 7)+NHR(chan 8? )，与 16P 双 4P 构造不同；M0 必答 1 给全表，断言用 16P 构造为准）。

---

## 六、M0 必答（落盘 M0_R19_THREAD_CLAIM.md，先答后动代码）

1. **手算全表**：四配方 × {16P 双 4P 域（mesh need4+nhr need1）、8P 单域 AG、8P 单域 RS} 的 slave 值（旧公式 vs 新公式 vs 老树公式三列对照，含 PARALLEL −1 与 SPLIT 零差标注）。
2. **two_shot/多段 MAX_REUSE 等价论证**：AR sole[mesh.two_shot] 展开 RS(n)+AG(n−1) 两段 max = n−1 == 老树单模板 templateRankSize−1（ins_temp_all_reduce_mesh_1D_two_shot.cc:51）；sole[mesh.chunk] 多段 max 池复用 == 老树 chunk 轮次串行复用（st :721 既有锚佐证）。
3. **AR parallel +4→+1 差异登记**（§三不做项的数学展开）。
4. **notify size vs slave count 失配现状确认**：CONCURRENT_SUM 覆写后 notifyNumPerThread（拼接 size）与 slaveThreadNum 的差值（16P parallel：notify 拼接 3+0=3 条 vs slave 4）——现状 gated 路径同型失配已登记 M0_R15 §九超配安全，本轮维持不扩面。
5. **Materialize 线程量变化面**：parallel 16P 申请 5 条（旧 max 4 条 / 旧 Σ 6 条）——B 档恒 = 编排器 Σneed；env 关跑串行时多 1 条闲置无害（失配安全既有登记）。

---

## 七、验收

- 三入口 0 failed；新基线实测记录（1958 + R19 新增 − 改写净变化）。
- 等价改写例外在 M0 §登记（§四清单 8 条）。
- checklist #32 落盘；导读 5 处漂移修正；方案执行记录补验收。
- git diff 触碰面 = §二清单文件 + 测试 + 文档（跨线零触碰自证）。
