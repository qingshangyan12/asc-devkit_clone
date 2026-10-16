# M0_R19 · 线程申报策略查表绑定（设计债 #12——编排方式 → 申报公式映射表）

> 立项 2026-10-15（用户裁定：公式是编排方式的属性，不是全局开关；B 档——
> PARALLEL/SPLIT 走 CONCURRENT_SUM，slave = Σ_d(need_d−1)+(D−1)）。任务书
> unified/test/R19_TASK.md。本 M0 先答后动：期望值全部独立手算（python 独立
> 复算两处推导互证，脚本留 /tmp/r19_m0_verify.py 不入库），不转抄生成方输出。

---

## 一、必答 1：手算全表（四配方 × 三场景，旧公式 vs 新公式 vs 老树公式）

**公式集**（新公式以本轮交付形态为准）：

- need(seg)（DomainThreadTotalNeed，R15-③ 既有）：RS·NHR→max(1,chan)、
  RS·mesh→max(1,n)、AG→max(1,n−1)。
- **旧 gated Σ**（R15 形态，宏+env 开才覆写）：`Σ_d(need_d−1)+D`。
- **旧 max**（宏/env 关，现状缺省）：逐段 CalcResOf 的 operator+= max 累积
  （action_spec.h:224）。
- **新 B 档 CONCURRENT_SUM**（SPLIT/PARALLEL，查表绑定恒生效、与 env 无关）：
  `Σ_d(need_d−1)+(D−1)`（D=域数，GroupDomains 去重）。
- **新 MAX_REUSE**（SOLE/SEQUENCE，查表绑定恒生效）：max 累积零改动。
- 老树（executor 源码实证，行号本轮 grep 逐一核实）：
  parallel=`intra+inter+2`（RS ins_reduce_scatter_parallel_executor.cc:76-77 /
  AG ins_v2_all_gather_parallel_executor.cc:130-131 / AR ins_v2_all_reduce_
  parallel_executor.cc:120 两域各+2 共 +4）、split=`temp0+temp1+1`（RS
  ins_reduce_scatter_concurrent_executor.cc:68-69 / AG ins_v2_all_gather_
  concurrent_executor.cc:89）、sequence=`max(inter,intra)`（RS
  ins_v2_reduce_scatter_sequence_executor.cc:84 / AG ins_v2_all_gather_
  sequence_executor.cc:66）、sole=模板 CalcRes 直通（AG ins_v2_all_gather_
  sole_executor.cc:44；AR ins_v2_all_reduce_sole_executor.cc:64 本轮核实）。

### 场景 ①：16P 双 4P 域（RS op，hier {{0,1,2,3},{0,4,8,12}}，mesh 域 need=4、NHR 4P chan=1 域 need=1）

| 配方 | 旧 max（宏关） | 旧 gated Σ（宏开） | 新（恒） | 老树 | 差异标注 |
|---|---|---|---|---|---|
| parallel[mesh,nhr] | 3 | 5 | **4**（CONCURRENT_SUM：(4−1)+(1−1)+(2−1)=3+0+1） | 5（3+0+2，专职控制位） | **PARALLEL −1**：控制线程职能已被编排器域 main 交叉握手接管，恒 ≤ 老树 |
| split[mesh,nhr] | 3 | 5 | **4**（CONCURRENT_SUM 同式） | 4（temp0+temp1+1=3+0+1） | **SPLIT 零差**：老树 +1（域0 main 兼控制）恰同 Σ(need−1)+(D−1) |
| sequence[mesh,nhr] | 3 | 5 | **3**（MAX_REUSE：max(mesh 3, nhr 0)） | 3（max(inter,intra)） | **零差**：MAX_REUSE 累积即老树公式逐字 |

### 场景 ②：8P 单域 AG（sole[mesh]，need=7）

| 配方 | 旧 max | 旧 gated Σ（宏开） | 新（恒） | 老树 | 差异 |
|---|---|---|---|---|---|
| sole[mesh] | 6 | 7（多报 1） | **6**（MAX_REUSE=need−1） | 6（模板直通 N−2，ins_temp_all_gather_mesh_1D 主线程认领一 peer） | **零差**；且修正旧 gated Σ 的多报（旧行为 sole 在宏开时 7 > 老树 6——R19 消除） |

### 场景 ③：8P 单域 RS（sole[mesh]，need=8）

| 配方 | 旧 max | 旧 gated Σ（宏开） | 新（恒） | 老树 | 差异 |
|---|---|---|---|---|---|
| sole[mesh] | 7 | 8（多报 1） | **7**（MAX_REUSE=need−1） | 7（模板直通 N−1） | **零差**；同②消除旧 gated 多报 |

**单域退化恒等式**（一致性锚，断言锁）：D=1 时 CONCURRENT_SUM =
(need−1)+0 = need−1 = 单域 MAX_REUSE——两档在单域配置下恒等。场景②的
4P 版（AG 4P：CONCURRENT_SUM 2 == MAX_REUSE 2）作断言锚。

**python 复算互证**（/tmp/r19_m0_verify.py）：上表全部数值两处独立推导
（手算公式代入 vs python 按同式编程）逐值一致；§四改写清单 8 条期望值
（4/2/4/4/13/4/4/4）与 §五新增锚期望值（4/3/4/7/2/0）同脚本复核通过。

---

## 二、必答 2：two_shot / 多段 MAX_REUSE 等价论证

**AR sole[mesh.two_shot]**：BuildStage 展开为 RS(n) + AG(n) 两段同域。
MAX_REUSE 累积 = max(RS slaves n−1, AG slaves n−2) = **n−1** == 老树单模板
`templateRankSize_ − 1`（ins_temp_all_reduce_mesh_1D_two_shot.cc:51 GetRes——
本轮行号核实）。两段池复用（RS 完成后 AG 复用同一批线程）是老树 two_shot
模板内部的轮次复用语义——unified 段模型把它显式化成两段 max 合并，值同。

**AR sole[mesh.chunk]**：多段 max 池复用 == 老树 chunk 轮次串行复用
（st :721 既有锚佐证："chunk2: slaveThreadNum still 6 (max, not doubled)"——
两段 chunk 各 6 slaves、合并取 6，与老树 chunk 模板同池串行复用同值）。

**SEQUENCE 多域**：max(inter,intra) 即 operator+= max 累积的逐字实现
（老树 ins_v2_all_gather_sequence_executor.cc:66 "分级算法，slaveThread和对应
notify可以复用"注释与 max 公式同源）——MAX_REUSE 档零改动即老等价。

---

## 三、必答 3：AR parallel（split[mesh.two_shot,nhr]）的 +4 → +1 差异登记（不实施）

老树 AR parallel（ins_v2_all_reduce_parallel_executor.cc:120）：
`slaveThreadNumIntra + slaveThreadNumInter + 4`。差异根源（§三不做项的数学
展开）：老树 AR parallel 域内是 **two_shot 双模板**（每域 intra RS+AG 两个
template 各带一条 main），两域共 4 条模板 main 需专职控制位协调 → +4。
unified 段模型每域 1 条 main（域内段序由 sink DoSync 序承载）→ CONCURRENT_SUM
的 (D−1)=+1（D=2 时恰 +1）。**恒 ≤ 老树**（+1 ≤ +4）：B 档"编排器域 main
交叉握手接管控制职能"原则的自然结果。AR 路由未激活（unified_route.h:70
注释行），数值差异无行为面——只登记不实现（任务书 §三）。

---

## 四、必答 4：notify size vs slave count 失配现状确认

16P parallel 例（mesh 段 slaves 3 + NHR 段 slaves 0）：CONCURRENT_SUM 覆写后
`slaveThreadNum=4`，而 notifyNumPerThread 拼接 = mesh 段 [1]×3 + NHR 段 [] =
**3 条**、notifyNumOnMainThread 累加 = 3+0 = **3**。差值 = 1（第 4 条线程 =
域 1 的 main 位，无逐线程 notify 载荷）。**现状对照**：旧 gated 路径同型失配
已登记 M0_R15 §九"超配安全"（maxNotifyNum = max(main 3, per 1)+1 = 4 ≥ 任一
线程需求——全线程统一申请、非逐线程精确）——R19 后该安全性论证原样成立
（notify 申请仍走同一 max+1 公式，claim 覆写只改 slaveThreadNum 数值）。
**本轮维持不扩面**（任务书 §三：notify 族合并逻辑不动）。

---

## 五、必答 5：Materialize 线程量变化面

16P parallel（tier A 展开 4 段，域 need 4+1）：R19 后 Materialize 申请
`slaveThreadNum+1 = 5` 条线程（旧宏关 max=3 → 4 条；旧宏开 Σ+D=5 → 6 条）。
**B 档恒 = 编排器 Σneed = 5**（Σ_d need_d = 4+1）——claim 与编排器实需首次
精确对齐（R15 首版 1 条冗余=老控制线程位，M0_R15 §十-3 登记的"真机复核后
可一行改"即本轮兑现）。env 关（执行面编排器灰度关）跑串行 for 时多出的
1 条域 main 闲置无害（失配安全性 M0_R15 §七既有登记同向：申报恒按编排方式
报足）。旧 gated Σ=5 slaves/6 total 比新 B 档多 1 条（专职控制位）——新公式
对线程池的申请量**只减不增**。

---

## 六、任务书勘误（实施前核对——以代码现状为准）

1. **任务书 §二-4 warning 新文案**："pool smaller than sum of domain needs"
   与现文案 "(parallel domains need sum; claim grants max)" 的结构不同——
   取语义中性的合并形 "pool smaller than sum of domain needs"（任务书本意：
   去 "claim grants max" 断言性表述），实施按此。
2. **任务书 §一表 sole 行 AR 证据**：任务书引 ins_v2_all_reduce_sole_executor.cc:64
   （本轮核实该行是 `CHK_RET(algTemplate->CalcRes(...))`——模板直通调用点，
   行号成立）。RS 侧 sole executor（ins_reduce_scatter_sole_executor.cc）未
   在任务书引证，本轮补核实同构（CalcRes 直通）。
3. **任务书 §五-2 sole[mesh] RS 8P 直编可达性**：探针实测
   （/tmp/test_rs_aicpu_compile.cc）RS 8P sole[mesh] AICPU Compile 返回
   SUCCESS、slave=7——**主路径可用**，无需降级为 ClaimAicpuThreadRes 直调
   （任务书预留的降级分支不触发）。
4. **任务书 §五-2 split/sequence 直编可达性**：探针实测
   （/tmp/test_seq_split_compile.cc）sequence[mesh,nhr] / split[mesh,nhr]
   16P AICPU Compile 均 SUCCESS（Validate 不拦多域非 parallel——后端注册齐
   即过）——**主路径可用**，无需 RecipeRegistry::Get 直调降级分支。
5. **行号核对**：任务书全部行号（st :4324/:4330/:4346/:4357-4358/:4482、
   tier :682/:689/:695-703、recipe_sole.h :225/:234-237/:352/:23、
   recipe_registry.h :70/:151/:182、action_spec.h :321/:283-295/:224、
   registry :203、aicpu_consumer.h:667、backend_ccu_*.h:39/:54、老树七处）
   逐一经本轮 grep 核实与代码现状一致，无漂移。

---

## 七、改动面清单（实施规格）

**生产码 5 文件**（零协议改动——claim 是 host 侧 Plan 字段，不序列化进 blob，
plan_serializer 只写 segments+tier）：

1. `inc/action_spec.h`：CalcConcurrencySlaveClaim 尾部 `+D` → `+(D−1)`（空
   segs 守卫 return 0）；函数头注释块更新为 R19 · #12 B 档口径。
2. `l2/recipe_sole.h`：新增 `enum class ClaimFormula {MAX_REUSE, CONCURRENT_SUM,
   REJECTED}` + `ClaimFormulaOf(Recipe)`（switch 全枚举覆盖，-Werror 漏 case
   即编译错）；SumRes 改签名 `SumRes(Plan&, Recipe)`，gate 块（UseUnified
   ConcurrencyPath）删除、换查表分派；:23 `#include "unified_route.h"` 删除
   （grep 确认本文件唯一消费方即该 gate，删后编译过即自证）。
3. `l2/recipe_registry.h`：三处 SumRes 调用点传身份（:70 PARALLEL / :151
   SEQUENCE / :182 SPLIT）。
4. `l5/segment_orchestrator.h`：:43-44 头注过时口径（"ResourceClaim 求和取
   max …实给 max"）与 :231 warning 文案（"claim grants max" 断言性表述）
   同步修正——逻辑零改动（降级判据 Σneed vs 池不变）。
5. **跨线零触碰自证**：unified_route.h / aicpu_consumer.h / verb_sink.h /
   verb_serializer.h / kfc_server_message.h / server / blob 全不动（git diff
   触碰面验收）。

**测试**：st_verification.cc §四清单 5 处改写 + 新增 R19 段（§五锚，段号 63）；
tier_payload_test.cc T7 三处改写（门卫双分支删、单断言恒值）；三入口 run.sh
零改动（tier_payload 编译宏 TIER+NHR_AICPU 不变、CONCURRENCY 探针测执行面
gate——gate 保留故探针有效）。

**文档**：本 M0 + REVIEW_CHECKLIST #32 + 导读 5 处漂移（:36/:173/:317/:342/
:1857 + §十一-13 交付记录）+ 方案文档 R19 执行记录。

---

## 八、验收记录（2026-10-15 实测收尾）

**等价改写例外登记（任务书铁律 2）**：本轮是公式设计变更（B 档用户裁定），
§四清单 8 条既有断言按"旧值→新值+手算依据"改写——这是 R18 全删轮先例的
同型显式例外（非回归、是设计变更）；清单外断言零触碰（复核清单见任务书
§四注）。

**新基线**：**1973 = run.sh 543 / st_checker 1010 / exec 420，三入口 0 failed。**

较 1958 净 +15，分解逐入口实测对账：

- **st_checker 995 → 1010（+15）**：全部来自 st_verification 新增 R19 段
  （段号 63）15 条 Check——①查表行锁 5（SOLE/PARALLEL/SEQUENCE/SPLIT/
  PIPELINE）+ ②逐配方端到端 8（parallel/sequence/split/sole 四 compile 各 1
  + 各 claim 值 1）+ ③单域退化恒等式 1 + ④空 segs 守卫 1。§四 5 处改写
  全部原位（期望值 5→4/3→2/5→4/3→4/14→13，条数不变）。- **exec 420 → 420（±0）**：T7 三处改写——#6（5→4）、#7（5→4）原位；
  #8 门卫双分支删并单断言。**计数对账**：tier_payload 按 TIER+NHR_AICPU 宏
  编译（不含 CONCURRENCY 宏）→ UseUnifiedConcurrencyPath 编译期恒 false →
  旧门卫双分支两跑恒走 if 支（else 支不可达、从未计数）——旧双跑 99+3fail
  / 66+3fail，3 个 fail（两 Σ 断言 + 门卫 if 支 max==3）改写后全转 pass：
  102 / 69。改写 #8 删除的是不可达 else 支与 if 支的包裹（执行面断言数
  不变：1 条 fail → 1 条 pass）。前 7 目标零改动（63/65/57/46/6/6/6），
  合计 420 == 旧 420。
- **run.sh 543 → 543（±0）**：七个目标全绿零改动（expand/serialize/
  kfc_gate/aicpu_exec/verb_sink/exec/verb_exec 均无 claim 断言）。

**跨线零触碰自证**：git 触碰面（相对本轮开工时工作区）= 生产码 4 文件
（action_spec.h / recipe_sole.h / recipe_registry.h / segment_orchestrator.h）
+ 测试 2 文件（st_verification.cc / poc_unified_tier_payload_test.cc）+ 文档
4 件（本 M0 / REVIEW_CHECKLIST / 导读 / 方案文档）。unified_route.h /
aicpu_consumer.h / verb_sink.h / verb_serializer.h / kfc_server_message.h /
server / blob 序列化零触碰（recipe_sole.h 的 unified_route.h include 随
R15 gate 退役一并移除——该 include 本就是 R15 未提交工作区态，移除后与
HEAD 一致）。

---

## 九、遗留与登记

- **AR parallel +4→+1**：§三登记不实施（AR 路由未激活）。
- **notify 族失配**：§四登记维持不扩面（超配安全既有论证覆盖）。
- **执行面编排器灰度**：UseUnifiedConcurrencyPath 保留（aicpu_consumer.h:667
  一行不动）——真机验证后整体退役（R14 机构放量开关）。
- **CCU 侧 claim**：不动（slaveThreadNum 恒 0，backend_ccu_collect_all.h:39 /
  backend_ccu_reduce_slice.h:54——本轮行号核实）。
