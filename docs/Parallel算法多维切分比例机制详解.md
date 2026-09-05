# Parallel 算法多维切分比例（A7）机制详解

> 对应 hccl 上游提交 `46dcb41e`（2026-07-15，MR !1302 "parallel算法切分方案：comm配置+环境变量+公式计算"，作者 yuantangzhi）
> 本文回答四个问题：**这个逻辑是什么、为什么要有、影响点在哪、什么场景会用到**。作为 `InsReduceScatterParallelMesh1DNHRUBX` 迁移中 A7 项的决策与实现依据。

---

## 1. 这是什么逻辑

### 1.1 一句话

**Parallel 系列算法要把一份数据劈成两半走两级网络（框内 mesh + 框间 CLOS），这个逻辑决定"劈多少"**——把原来写死的 0.5（对半劈），升级为"按两级网络的真实带宽算出来的最优比例"，并提供三级优先级的配置覆盖手段。

### 1.2 背景：Parallel 算法为什么需要"劈比例"

Parallel 类算法（RS/AG/AR/Broadcast/Reduce 都有）的两级流水结构（以 RS 为例）：

```text
输入数据按比例 r 劈两份：
┌────────────────────────────┬────────────────────────────┐
│  r 份额（先走 Mesh 后走 Clos）│  (1-r) 份额（先走 Clos 后走 Mesh）│
└────────────────────────────┴────────────────────────────┘
        │                              │
   框内 Mesh 环先归约              框间 Clos 环先归约
        └────────── 交叉完成第二步 ──────────┘
```

两份数据走**相反的先后顺序**（一份先框内后框间、另一份先框间后框内），两步都做完后各自的结果拼起来是完整的 ReduceScatter。这是"两级网络同时跑满"的手段（和 Concurrent 算法的动机同源，但结构不同：Concurrent 劈的是数据段各自独立完成语义，Parallel 劈的是"处理顺序"配对交叉）。

**劈分比例 r 的含义**：给"先 Mesh 后 Clos"路线的数据量份额。r 偏大 = 更依赖框内带宽，r 偏小 = 更依赖框间带宽。**r 的最优值取决于两级网络谁的带宽大**。

### 1.3 旧逻辑的问题

```cpp
// 迁移前的 devkit（以及 46dcb41e 之前的 hccl）
void GetParallelDataSplit(std::vector<float>& splitDataSize) const
{
    double splitData = 0.5;               // 永远对半劈
    splitDataSize.push_back(splitData);
    splitDataSize.push_back(splitData);
}
```

对半劈隐含一个假设：**两级网络带宽对称**。框内 mesh 4 卡直连、框间 CLOS 通道数恰好配平的对称组网下这个假设成立；但真实机组网经常不对称（见 §4），此时对半劈会让快的一级网络提前干完活、等慢的那级——带宽利用率白白损失。

### 1.4 新逻辑：三级优先级 + 内置公式

```cpp
// 46dcb41e 之后的 hccl
double ratio = multipleDimensionSplitRatio_;          // Host 传下来的值
if (multipleDimensionSplitRatioSource_ == MultipleDimensionSplitRatioSource::BUILTIN_FORMULA) {
    // 来源没被显式配置时，用内置公式现算
    ratio = CalcParallelDataSplitRatio(
        rankSizeLevel0_, rankSizeLevel1_,        // 两级的卡数
        intraChannelMap_, interChannelMap_,      // 两级的实际链路（含每条链路的端口规模）
        ParallelDataSplitType::REDUCE_SCATTER_WITH_LOCAL_REDUCE,  // 算子的通信量模型
        multipleDimensionSplitRatio_);           // 公式失败时的回退值
}
splitDataSize.push_back(ratio);
splitDataSize.push_back(1.0 - ratio);
```

**三级优先级**（Host 侧 op_common.cc 组装，写入 `param.opConfig`）：

```text
① 通信域配置（COMM_CONFIG）—— HcclCommSetConfigInfo 按 comm 粒度设置，最高优先
② 环境变量（ENV_CONFIG）—— HCCL_ALG_MULTIPLE_DIMENSION_SPLIT_RATIO，全局粒度
③ 内置公式（BUILTIN_FORMULA）—— 默认；按机组网现算
任何一级的值异常（NaN/Inf/越界）都回退到下一级；最终兜底 0.5（=旧行为）
```

**内置公式本体**（template_utils.cc，CalcParallelDataSplitRatio）分四步：

1. **提端口规模**：`GetPortGroupSize` 从两级 channel map 各取"首个非空组的 portGroupSize 之和"——每条链路建链时从拓扑图查的 `ENDPOINT_ATTR_BW_COEFF`（真实带宽系数）就存在这个字段里
2. **算时间系数**：按算子通信量模型，分别算"先 Mesh 后 Clos 的那份"和"先 Clos 后 Mesh 的那份"处理单位数据的时间系数。以 RS 模型为例：
   ```cpp
   mesh 系数 = 21 × (intraRank-1) / (20 × intraRank × intraPortGroupSize)
   clos 系数 = (interRank-1) / (interRank × effectiveInterPortGroupSize)
   // 21/20 是 AICPU 上 mesh/clos 的实测带宽权重（同 ConcurMeshNHR 算法里那对 21/20）
   ```
3. **配平求比例**：`r = clos系数 / (mesh系数 + clos系数)`——**谁的时间系数小（跑得快），就多分数据给它对面的路线**，让两路同时完工
4. **量化 + 防护**：就近量化到 [1/8, 7/8] 的八分位（避免碎比例），全链路 isfinite 检查、空 channel/零端口/零卡数逐一回退并打日志

**通信量模型**（ParallelDataSplitType 三枚举）——不同算子数据在两级网络里的放大/收缩规律不同，公式要区分：

| 枚举 | 算子 | 通信量规律 |
|---|---|---|
| `REDUCE_SCATTER_WITH_LOCAL_REDUCE` | ReduceScatter / AllReduce / Broadcast | 数据逐级**收缩** k(P)=(P-1)/P，第一阶段主导耗时 |
| `SCATTER` | Scatter | 同为收缩模型（后拆出的第三类） |
| `ALL_GATHER` | AllGather | 数据逐级**放大** k(P)=P-1，第二阶段主导耗时——系数公式不同，且两份数据的"先/后"顺序对应关系反转 |

---

## 2. 为什么要有这个逻辑（动机）

**根因：组网不对称时，对半劈浪费带宽。** 逐层拆：

1. **物理事实**：框内 mesh（每对卡直连、无交换）和框间 CLOS（过交换芯片、有通道聚合）的带宽天然不同，且**随组网形态变化**——框数、卡数、jetty 数、POD 双 die 收敛都会改变两级带宽比
2. **性能目标**：两路并行的总时间 = max(两路各自时间)。最优状态是**两路同时完工**，即按带宽反比分摊数据
3. **工程矛盾**：最优比例是组网的函数，**编译期不知道、写死必错**；组网种类又多到无法枚举预置
4. **解法**：运行时从已建链路读真实带宽系数（portGroupSize 建链时已查好，零额外查询成本），公式现算；再留三级配置给运维兜底/调优

上游 PR 描述里的另一个动机细节：**Device 侧要能区分"显式配置的比例"和"公式回退的比例"**——所以才有 Source 枚举（默认值从原来混乱的 0.8 统一成 0.5，显式配置和回退语义分开），避免把运维手设的值和公式算的值混为一谈。

---

## 3. 影响点（迁移时动哪些地方 + 波及面）

### 3.1 直接改动面（以 RS Parallel 为例）

| 位置 | 内容 | 规模 |
|---|---|---|
| `template_utils.h/.cc` | 公式全家桶：`CalcParallelDataSplitRatio` + 8 个 static 辅助（校验/端口提取/时间系数/配平/量化/日志）+ `ParallelDataSplitType` 枚举 + `ParallelPortInfo`/`ParallelTimeCoeff` 结构 + `GetPortGroupSize` | **~270 行纯新增** |
| `ins_reduce_scatter_parallel_executor.cc` | ① `GetParallelDataSplit` 改造（三档判断 + 调公式）；② OrchestrateLoop 读 `param.opConfig` 两个成员并缓存 | ~15 行 |
| `alg_param.h`（如走 full） | 无——devkit 已有 `multipleDimensionSplitRatio/Source` 字段和枚举 | 0 |
| `alg_env_config.cc` + `op_common.cc`（仅 A7-full） | 环境变量解析（:125-165）+ 三档来源组装（:3503-3517，含 SFINAE 检测 comm 配置能力） | ~60 行，**触碰共享文件既有区域** |

### 3.2 数据流（一图看清依赖从哪来）

```text
建链阶段（已有，零改动）：
  拓扑图 ──ENDPOINT_ATTR_BW_COEFF──> ChannelInfo.portGroupSize   ← 公式的带宽输入
         ──ENDPOINT_ATTR_DIE_ID────> ChannelInfo.dieId           ← POD 判定输入
                                                    │
选择阶段：Host 组装 opConfig.{ratio, source}（三档优先级）  ← 仅 full 档要补
                                                    │
执行阶段（本次改造）：
  OrchestrateLoop 读 opConfig → GetParallelDataSplit
      → source==BUILTIN_FORMULA ? CalcParallelDataSplitRatio(两级卡数, 两级channel map,
                                    算子模型, 回退值) : 直接用传入值
      → splitDataSize = {r, 1-r}
      → 三个消费者：
         ① scratch 倍率分配（:498-502，四份 scratch 前置空间按 r 加权）
         ② 每轮循环数据切分（:558，currCountPart0 = r × currCount）
         ③ 对齐修正（16K 对齐后总和守恒）
```

### 3.3 波及面（谁会被影响）

| 对象 | 影响 |
|---|---|
| **本次目标**：RS Parallel UBX（MESH_1D_CLOS，clos 为 mesh 倍数） | 劈分从恒 0.5 → 公式值。对称组网公式≈0.5（行为几乎不变）；不对称组网行为改变（这正是目的） |
| 共享 `template_utils.h/.cc` 的所有代码 | **纯新增不碰旧**，既有函数零影响 |
| 共用该 executor 的其他 RS 注册（Multilevel/Pcie 版） | 同 executor，公式同样生效（与 hccl 上游一致——上游是 5 个 Parallel executor 全接入） |
| devkit 未迁移的 AG/AR/Broadcast parallel | 未接入（它们的 executor 没改造），不影响 |
| forced-alg / ST | ST 的 channel stub 里 `portGroupSize` 默认 1 → 公式退化到对称情形，能验证公式链路和回退；要测非对称需构造非 1 的 portGroupSize |

### 3.4 风险与回退设计（为什么可以放心）

- **回退链完备**：空 channel / 零端口 / 零卡数 / 非有限值 / 未知模型 → 逐项 fallback 到传入回退值（默认 0.5）+ WARN 日志。**最坏情况 = 旧行为**
- **量化到八分位**：比例永远落在 [1/8, 7/8] 的粗档位，杜绝极端劈分（如 0.97:0.03）导致的 scratch 碎片
- **上游成熟度**：07-15 合入至今 1.5 个月，无 revert 无后继修复；仅 `SCATTER` 枚举是后续小增（不影响 RS）
- **POD 修正**：框间双 die 链路（dieId 不同的两条 channel）带宽按 2:1 收敛——这是 A5 机型的实际物理特性，公式里已带

---

## 4. 什么场景下会用到（触发条件）

**入口条件**（selector 选到 Parallel 算法 + 走到 executor 切分）——以 RS Parallel UBX 为例：

```text
① 拓扑 = MESH_1D_CLOS（UBX 机型：框内 mesh + 框间 CLOS）
② clos 组卡数是 mesh 组卡数的整数倍（isClosNumMultipleOfMeshNum）
③ 数据量 ≥ 512KB（非小数据）
④ 非 64bit 类型、非 PROD
→ InsReduceScatterParallelMesh1DNHRUBX → OrchestrateLoop → GetParallelDataSplit
```

**公式产生与 0.5 明显差异的典型场景**（即这个逻辑真正发挥价值的场景）：

| 场景 | 两级带宽对比 | 公式倾向 | 收益 |
|---|---|---|---|
| 2 框 × 8 卡（mesh 8 卡/clos 16 卡），框间 jetty 少 | CLOS 慢 | r 偏小（少给先-Mesh 路线）| 避免 mesh 路干完等 CLOS |
| POD 机型（框间双 die 收敛链路） | CLOS 有效带宽减半 | 明显偏 mesh | POD 修正自动生效 |
| 框间多 jetty 聚合（4 jetty） | CLOS 快 | r 偏大 | 吃满框间带宽 |
| 对称组网（mesh=clos 带宽配平） | 相当 | r ≈ 0.5 | 与旧行为一致，零回退 |

**不会触发的场景**：非 UBX 拓扑（纯 mesh/CLOS 走其他算法）、小数据（CCU/OneShot 类）、64bit/PROD（走 AicpuReduceNHR）——这些在 selector 就分流了，根本到不了这个逻辑。

**运维手动介入场景**（仅 A7-full 提供）：压测发现某机组网公式值不是最优 → `HcclCommSetConfigInfo` 按 comm 设比例（最高优先）或设环境变量全局调——不用改代码重启即生效。

---

## 5. 与本次迁移的决策关联（小结）

| 档位 | 内容 | 增量 | 风险 |
|---|---|---|---|
| 不同步 | 保持 0.5 | 0 | 对称组网无损失；不对称组网带宽浪费延续 |
| **A7-lite** | 公式 + executor 接线（source 恒为 BUILTIN_FORMULA） | ~285 行（template_utils 纯新增 + executor 15 行） | 小：回退链兜底=旧行为；底层字段 devkit 全现成 |
| A7-full | 再补环境变量 + comm 配置链 | +60 行（alg_env_config/op_common 共享文件） | 中：触碰共享文件既有区域，需单独评估 |

依据：本文 §3.4 的回退完备性 + §4 的场景收益（目标算法恰恰服务非对称 UBX 组网——`isClosNumMultipleOfMeshNum` 本身就排除了对称场景，**公式在这个算法上几乎必然产生 ≠0.5 的值**），A7-lite 与算法迁移同分支做的性价比高；A7-full 的运维调优能力可后补。
