Device_id	Task Duration(us)	Block Num	aiv_time(us)	aiv_vec_time(us)	aiv_vec_ratio	aiv_scalar_time(us)	aiv_scalar_ratio	aiv_mte2_time(us)	aiv_mte2_ratio	aiv_mte3_time(us)	aiv_mte3_ratio
静态Tensor+不打开double buffer											
case1	707.917	64	707.29	517.249	0.731	2.808	0.004	185.047	0.262	41.096	0.058
case2	698.192	64	697.53	506.753	0.726	5.032	0.007	186.598	0.268	40.819	0.059
case3	565.888	64	565.12	310.329	0.549	5.138	0.009	249.989	0.442	44.142	0.078
开启doublebuffer											
case1	551.996	64	551.2	516.444	0.937	3.554	0.006	440.587	0.799	211.968	0.385
case2	558.931	64	558.14	503.212	0.902	3.196	0.006	550.632	0.987	73.738	0.132
case3	525.704	64	524.93	306.582	0.584	3.282	0.006	504.872	0.962	338.252	0.644
doublebuffer+L2Bypass											
case1	527.278	64	526.53	514.955	0.978	3.617	0.007	249.731	0.474	67.581	0.128
case2	545.457	64	544.77	503.459	0.924	3.383	0.006	537.388	0.986	43.056	0.079
case3	458.838	64	458.07	305.655	0.667	3.267	0.007	444.485	0.97	108.877	0.238
case4	471.095	64	470.25	405.567	0.862	5.287	0.011	403.068	0.857	99.05	0.211
case5	459.97	64	459.17	310.356	0.676	18.683	0.041	446.217	0.972	107.044	0.233
case0：simt gm stride：全部使用 SIMT 直接访问 GM，不经过 GM->UB/UB->GM DataCopy（新增对照组，本文历史 msprof 数据暂未覆盖）											
case1：simd Regbase											
case2：simt：每个线程处理一段连续元素（非连续访问）											
case3：simt：每轮 warp 内相邻线程访问连续元素（连续访问）
case4：simt：分阶段 block-based，2 阶段每阶段各处理 4096 元素，减少单阶段 DCache 压力（对 case2 的优化）
case5：simt stride-based + TILE_LENGTH=9216，最大化 UB 利用率以提升 MTE2 单次搬移吞吐（对原 case4/当前 case3 的历史优化尝试，因无收益不在样例中展示）


---

# FloorMod 算子四个样例 Case 与历史 Case4/Case5 性能分析报告

> 当前样例的 `SCENARIO_NUM` 为 case0~case3。本文中的 case4/case5 仅保留历史 msprof 分析：case4 是对 case2 非连续访问的分阶段优化，case5 是对原 case4（当前 case3 stride 方案）的 MaxTile 尝试；两者均不在当前样例实现中展示。

## 一、测试场景说明

### 四种当前样例实现方式

| Case | 实现方式 | 核心函数 | 内存访问特征 |
|---|---|---|---|
| case0 | SIMT-GM-Stride | `floor_mod_simt_gm_stride` | 全部使用 SIMT 线程直接读写 GM，不经过 GM→UB `DataCopy`，作为 GM 直接访问对照组 |
| case1 | 纯 SIMD（RegTensor 向量指令） | `floor_mod_int_simd` | MTE2 DataCopy 整块搬移，Vector 引擎用 RegTensor 操作 UB→寄存器→计算→寄存器→UB |
| case2 | SIMT block-based | `floor_mod_simt_block` | 每线程处理 `tid*8` 起始的连续 8 元素，**warp 内相邻线程地址间隔 8 元素 → 非 coalesced** |
| case3 | SIMT stride-based | `floor_mod_simt_stride` | 每线程跨步 1024，**warp 内相邻线程地址间隔 1 元素 → coalesced** |

### 历史对照项

| Case | 实现方式 | 核心函数 | 结论 |
|---|---|---|---|
| case4 | SIMT staged block-based | `floor_mod_simt_block_range` | 对 case2 非连续访问的分阶段优化，实测有效但当前样例不展示 |
| case5 | SIMT stride-based MaxTile | `floor_mod_simt_stride` (同 case3) + TILE_LENGTH=9216 | 对原 case4（当前 case3 stride 方案）的 MaxTile 尝试。实测与 case3 基本持平，remainder tile 还引入额外 scalar 开销，因此不在当前样例中展示 |

### 三层编译/运行配置

| 配置名 | Double Buffer | L2 Bypass | 说明 |
|---|---|---|---|
| 静态Tensor+不打开double buffer | ❌ | ❌ | 基线：串行流水，MTE2→Compute→MTE3 无重叠 |
| 开启doublebuffer | ✅ | ❌ | MTE2/Compute/MTE3 可乒乓流水重叠 |
| doublebuffer+L2Bypass | ✅ | ✅ | 在上述基础上，GM→UB 搬移绕过 L2 Cache |

### 公共参数

- 数据类型：`int32_t`
- 总数据量：`TOTAL_LENGTH = 8192 × 8192 = 67,108,864` 元素 ≈ 256 MB
- Block 数：64 Core
- 每 Core 数据量：`1,048,576` 元素
- Tile 大小：`TILE_LENGTH = 8192` 元素
- 每 Core Tile 数：128
- SIMT 线程数：`THREAD_COUNT = 1024`
- case1~case3 双缓冲：Ping/Pong 各 3 个 Tensor（self、other、out），共占 UB `8192 × 4 × 6 = 192 KB`
- 历史 case4 沿用上述 UB 双缓冲布局；历史 case5 使用 `TILE_LENGTH = 9216`
- case0：纯 SIMT GM 访问，动态 UB 申请为 0

---

## 二、详细性能数据

> 以下数据来自原有 msprof 记录并已按当前 case 编号重排；新增 case0 暂无对应性能数据。case4/case5 为历史对照项，不属于当前样例。

### 配置 A：静态 Tensor + 不开启 Double Buffer（基线，串行流水）

| Case | Task Duration(us) | vec_time(us) | vec% | mte2_time(us) | mte2% | mte3_time(us) | mte3% |
|---|---|---|---|---|---|---|---|
| case1 | **707.9** | 517.2 | 73.1% | 185.0 | 26.2% | 41.1 | 5.8% |
| case2 | **698.2** | 506.8 | 72.6% | 186.6 | 26.8% | 40.8 | 5.9% |
| case3 | **565.9** | 310.3 | 54.9% | 250.0 | 44.2% | 44.1 | 7.8% |

> **流水特征**：vec% + mte2% + mte3% ≈ 100%，三个引擎严格串行，无重叠。
>
> **case3 最优（565.9us）**，比 case1 快 20.1%，比 case2 快 18.9%。

### 配置 B：开启 Double Buffer（乒乓流水）

| Case | Task Duration(us) | vec_time(us) | vec% | mte2_time(us) | mte2% | mte3_time(us) | mte3% | 相对 A 加速 |
|---|---|---|---|---|---|---|---|---|
| case1 | **552.0** | 516.4 | 93.7% | 440.6 | 79.9% | 212.0 | 38.5% | **1.28×** |
| case2 | **558.9** | 503.2 | 90.2% | 550.6 | 98.7% | 73.7 | 13.2% | **1.25×** |
| case3 | **525.7** | 306.6 | 58.4% | 504.9 | 96.2% | 338.3 | 64.4% | **1.08×** |

> **流水特征**：vec% + mte2% 远超 100%（case1: 173.6%, case2: 188.9%, case3: 154.6%），说明 MTE2 与 Vector 引擎实现了显著的并行重叠。
>
> **关键反转**：case1（552.0us）反超 case2（558.9us），详见第五节分析。

### 配置 C：Double Buffer + L2 Bypass

| Case | Task Duration(us) | vec_time(us) | vec% | mte2_time(us) | mte2% | mte3_time(us) | mte3% | 相对 B 加速 | 相对 A 总加速 |
|---|---|---|---|---|---|---|---|---|---|
| case1 | **527.3** | 515.0 | 97.8% | 249.7 | 47.4% | 67.6 | 12.8% | **1.05×** | **1.34×** |
| case2 | **545.5** | 503.5 | 92.4% | 537.4 | 98.6% | 43.1 | 7.9% | **1.02×** | **1.28×** |
| case3 | **458.8** | 305.7 | 66.7% | 444.5 | 97.0% | 108.9 | 23.8% | **1.13×** | **1.54×** |
| case4 | **471.1** | 405.6 | 86.2% | 403.1 | 85.7% | 99.1 | 21.1% | — | — |
| case5 | **460.0** | 310.4 | 67.6% | 446.2 | 97.2% | 107.0 | 23.3% | — | — |

> **case3 + DB + L2Bypass 以 458.8us 取得全局最优**，相比 case1 基线提速 **35.2%**。

---

## 三、各 Case 瓶颈根因分析

### Case0（SIMT-GM-Stride）—— 纯 SIMT GM 访问对照组

`floor_mod_simt_gm_stride` 不申请动态 UB，不经过 MTE2/MTE3 的 GM→UB→GM 搬移流水，而是由 SIMT 线程直接从 GM 读取 `self/other` 并直接写回 GM。它用于对照 case3 的“GM→UB DataCopy 后再由 SIMT 读写 UB”模式。

当前历史 msprof 数据暂未覆盖 case0，因此后续性能排名不包含该新增对照组。

### Case1（SIMD RegTensor）—— 向量指令依赖链长，计算密集型

`floor_mod_int_simd` 的核心计算是一条长依赖链：

```
DataCopy(UB→Reg) → Div → Mul → Sub → Compare(NE) → Select
                                         → Add
                                         → Compare(NE) → And → And → Compare(NE) → MaskAnd → Select
                   DataCopy(Reg→UB)
```

每轮循环包含 **10+ 条向量指令**，且大部分存在 RAW（Read-After-Write）依赖，无法被 Vector 引擎的指令级并行消化。`int32_t` 的 SIMD 整数除法（`Div`）本身延迟较高，后续 MaskReg 比较和 Select 进一步拉长了关键路径。

**vec_time 稳定在 ~515us，三层配置下几乎不变**，证明这是纯计算瓶颈，与访存优化无关。

**优势**：SIMD 使用 RegTensor，数据从 UB Load 到寄存器后计算完全在寄存器内完成，不占用 UB 带宽。这为 Double Buffer 场景下 MTE2 与 Vector 的并行留下了充足的 UB 带宽余量。

### Case2（SIMT block-based）—— 非 Coalesced 访存 + UB 带宽争抢

**访问模式**：
```cpp
index = tid * elems_per_thread + i;  // tid∈[0,1023], i∈[0,7]
```
相邻线程（tid=N 和 tid=N+1）访问的元素地址间隔为 8 个 int32（32 bytes）。一个 warp 内 32 个线程的 load 请求分散在 32 条不同的 cache line 上，每条 cache line 仅被 1 个线程使用，**warp 级访存合并率极低**。

**vec_time（~505us）**：介于 case1 和 case3 之间。SIMT 标量整数除法和分支比 SIMD 向量指令链略高效，但因非 coalesced 访存导致 SIMT DCache 命中率低，拖累了计算效率。

**关键问题——UB 带宽争抢**：SIMT 线程直接通过 `__ubuf__` 指针读写 UB。在 Double Buffer 场景下，即使 MTE2 和 SIMT Compute 访问不同的逻辑 buffer（ping vs pong），物理 UB 的 bank/port 资源是共享的。case2 的非 coalesced 访存模式产生大量离散的 UB 事务，与 MTE2 的连续写入形成带宽竞争，导致 MTE2 效率下降。

### Case3（SIMT stride-based）—— Coalesced 访存，计算效率最高

**访问模式**：
```cpp
index = threadIdx.x; index += blockDim.x;  // blockDim.x=1024
```
相邻线程访问相邻元素，warp 内 32 个线程的 load 可合并为 few cache-line-sized transactions。DCache 和 UB 带宽利用率最大化。

**vec_time 仅 ~306us**，比 case1 低 **40.8%**。原因：
1. SIMT 标量 `%`、比较、条件赋值被编译器映射为高效的标量整数指令
2. 1024 个线程提供足够并行度掩盖单条标量指令延迟
3. Coalesced 访存使 DCache 命中率最优，数据供给不成为瓶颈

**局限**：vec_time 太短（仅占 kernel 的 55~67%），compute 段提前结束，流水线等待 MTE2 完成搬移，**MTE2 成为新瓶颈**（mte2_ratio 高达 96~97%）。

---

## 四、Double Buffer 收益差异分析

### 为什么加速比不同？

| Case | 无 DB 时 vec% | DB 后 vec% | DB 后 mte2% | 加速比 | 瓶颈 |
|---|---|---|---|---|---|
| case1 | 73.1% | 93.7% | 79.9% | **1.28×** | Vec 主导，MTE2 有喘息空间 |
| case2 | 72.6% | 90.2% | 98.7% | **1.25×** | MTE2 接近饱和（98.7%） |
| case3 | 54.9% | 58.4% | 96.2% | **1.08×** | MTE2 几乎饱和，compute 段太短无法有效重叠 |

Double Buffer 的加速效果取决于 **compute 段长度能否覆盖 MTE2 搬移时间**。流水线理想情况下：

```
MTE2(Tile N+1) || Compute(Tile N) || MTE3(Tile N-1)
```

- **case1**：Compute 长（517us），足以掩盖 MTE2 的大部分时间，加速显著
- **case2**：Compute 中等（507us），但 MTE2 因 UB 争抢被拉长到 551us，MTE2 反成瓶颈
- **case3**：Compute 太短（307us），MTE2（505us）是绝对瓶颈，重叠窗口极小

---

## 五、核心问题：为什么 case2 在开启 Double Buffer 后被 case1 反超？

### 现象

| 配置 | case1 (us) | case2 (us) | 优胜 |
|---|---|---|---|
| 无 DB | 707.9 | **698.2** | case2 快 1.4% |
| 有 DB | **552.0** | 558.9 | case1 快 1.3% |
| DB+L2Bypass | **527.3** | 545.5 | case1 快 3.5% |

### 根因：关键路径的转移——从 Compute 瓶颈变为 MTE2 瓶颈

#### 无 Double Buffer（串行流水）

串行流水下，总时间 ≈ MTE2 + Vec + MTE3，MTE2 和 MTE3 对三个 case 几乎相同（都是同样的 `DataCopy` 操作）：

```
case1: 185.0 + 517.2 + 41.1 = 743.3 → 实测 707.3
case2: 186.6 + 506.8 + 40.8 = 734.2 → 实测 697.5
```

**case2 胜出**，因为它的 Vec 段比 case1 短约 10us（506.8 < 517.2），而 MTE2/MTE3 时间几乎相同。SIMT 标量除法的执行效率略优于 SIMD 向量除法指令链。

#### 有 Double Buffer（乒乓流水）

Double Buffer 使 MTE2、Compute、MTE3 可以并行。此时总时间 ≈ **max(MTE2通路, Vec通路, MTE3通路)**，不再是简单相加。

对比两个 case 在 DB 下的各引擎活跃时间：

| 引擎 | case1 活跃时间 | case2 活跃时间 | 差异 |
|---|---|---|---|
| Vec | **516.4us** ← 关键路径 | 503.2us | case2 略短 |
| MTE2 | 440.6us | **550.6us** ← 关键路径 | **case2 长 25%！** |
| MTE3 | 212.0us | 73.7us | case2 更短 |

**case1 的关键路径是 Vec（516us），case2 的关键路径变成了 MTE2（551us）。虽然 case2 的 Vec 仍然比 case1 快，但 MTE2 被严重拖慢，成为新的瓶颈，导致总时间反而更长。**

### 为什么 case2 的 MTE2 被拖慢？

三个 case 的 `CopyIn` 函数完全相同——都是 `AscendC::DataCopy(selfLocal, selfGm[tileOffset], count)` 做连续数据搬移。**MTE2 指令本身是等价的，差异来自 UB 带宽的竞争环境不同。**

#### SIMD vs SIMT 的 UB 访问模式差异

| 维度 | case1 (SIMD) | case2 (SIMT block) |
|---|---|---|
| Compute 期间 UB 访问 | RegTensor：DataCopy 一次性 Load 到寄存器，**计算全程不访问 UB** | SIMT 线程通过 `__ubuf__*` 指针**直接读写 UB**，每个线程每条指令都可能产生 UB 事务 |
| UB 事务特征 | 无（计算在寄存器内） | 1024 线程 × 非 coalesced 地址 → **大量离散 UB 事务** |
| 对 MTE2 的影响 | MTE2 独占 UB 写带宽 | SIMT 的离散 UB 读与 MTE2 的连续 UB 写**竞争同一物理 UB 的 bank/port** |

在 Double Buffer 的乒乓流水下：

```
                  时间轴 →
case1:  [MTE2写Pong████████████████░░░░]   ← UB带宽充裕
        [SIMD计算Ping    ████████████████████████]  ← 不访问UB
                             ↑ 互不干扰

case2:  [MTE2写Pong████████████████████████████]   ← UB带宽被争抢，拉长！
        [SIMT计算Ping ████████████████████]  ← 1024线程离散UB读写
                             ↑ 物理UB bank/port冲突
```

**case2 的 SIMT 非 coalesced 访存模式在 Compute 期间产生了大量离散 UB 事务，与 MTE2 的连续写入形成物理 UB 资源竞争，导致 MTE2 每 tile 的有效带宽下降、完成时间拉长。** 这就是 case2 的 MTE2 时间从 186.6us 飙升至 550.6us（增幅 195%）而 case1 仅从 185.0us 升至 440.6us（增幅 138%）的根本原因。

#### L2 Bypass 下的佐证

| 配置 | case1 MTE2 (us) | case2 MTE2 (us) | case2 MTE2 变化 |
|---|---|---|---|
| DB only | 440.6 | 550.6 | — |
| DB + L2Bypass | 249.7 (**↓43%**) | 537.4 (**↓2.4%**) | 几乎无改善 |

L2 Bypass 将 case1 的 MTE2 时间大幅削减 43%（L2 延迟被消除），但 case2 的 MTE2 仅下降 2.4%。这明确说明：**case2 的 MTE2 瓶颈不在 L2 Cache 延迟，而在 UB 端的带宽竞争**。即使 GM→UB 通路更快了，UB 写入仍然被 SIMT 的离散访问拖慢。

### 总结

```
无DB:  case2 > case1   因为 Vec(506.8) < Vec(517.2)，MTE2 都是 ~186us
                          ↓ 开启 Double Buffer
有DB:  case1 > case2   因为 case2 的 MTE2(550.6) >> case1 的 MTE2(440.6)
                        关键路径从 Vec 转移到 MTE2
                        case2 的 SIMT 非coalesced UB访问争抢了MTE2带宽
```

---

## 六、L2 Bypass 对各 Case 的差异化影响

| Case | MTE2(DB only) | MTE2(DB+L2Bypass) | 降幅 | 根因 |
|---|---|---|---|---|
| case1 | 440.6 | 249.7 | **↓43.3%** | SIMD 不访问 UB，MTE2 是纯 GM→UB 通路，受 L2 延迟直接影响 |
| case2 | 550.6 | 537.4 | **↓2.4%** | 瓶颈在 UB 带宽竞争，不在 L2 延迟 |
| case3 | 504.9 | 444.5 | **↓12.0%** | Coalesced 访存 UB 竞争较小，但仍有部分影响 |

**结论**：L2 Bypass 的效果取决于 MTE2 是否真的是瓶颈。如果瓶颈在 UB 带宽竞争（case2），绕过 L2 无济于事；如果瓶颈在 GM→L2→UB 的数据通路延迟（case1），效果立竿见影。

---

## 七、整体结论与优化建议

### 性能排名

#### BLOCK_DIM=64（每 core 128 tiles，原配置，含历史 case4，对应样例不含 case0）

| 排名 | Case | 最优 Task Duration | 相对基线提速 |
|---|---|---|---|
| 🥇 | case3 (SIMT stride) | **458.8 us** | **35.2%** |
| 🥈 | case4 (SIMT staged) | 471.1 us | 33.5% |
| 🥉 | case1 (SIMD) | 527.3 us | 25.6% |
| 4 | case2 (SIMT block) | 545.5 us | 21.9% |

> 表中 case4 为历史对照数据，不在当前样例中展示。历史 case5（SIMT stride MaxTile）实测为 460.0 us，与 case3 基本持平且没有收益，因此不纳入当前样例排名。

#### BLOCK_DIM=32（每 core 256 tiles，瓶颈反转为 Vec，含历史 case4）

| 排名 | Case | Task Duration | 相对 case2 提速 |
|---|---|---|---|
| 🥇 | **case3 (SIMT stride)** | **616.1 us** | **41.9%** |
| 🥈 | case4 (SIMT staged) | 819.8 us | 22.7% |
| 🥉 | case1 (SIMD) | 1021.4 us | 3.7% |
| 4 | case2 (SIMT block) | 1060.6 us | 基线 |

**排名顺序与 BLOCK_DIM=64 完全一致（case3 > case4 > case1 > case2），但 case3 对 case4 的领先从 2.7% 暴增至 33.1%。BLOCK_DIM=32 解除了 MTE2 瓶颈，case3 的真实计算优势完整暴露。**

### 关键结论

1. **case3（SIMT stride）在所有 BLOCK_DIM 配置下均为最优**。Coalesced 访存 + 单次 VF 调用 = 不可超越的基线。

2. **瓶颈决定优化效果的"可见度"**：BLOCK_DIM=64 时 MTE2 瓶颈遮蔽了 case3 的计算优势（端到端仅领先 2.7%），BLOCK_DIM=32 解除 MTE2 瓶颈后，case3 对 case4 的 33% vec 优势完整转化为端到端优势。

3. **case4 的 vec_time 始终比 case3 慢 ~33%**——来自 stride=4（非完美 coalesced）和双倍 VF 调用开销。这是结构性差距，与数据量无关。

4. **case5（增大 TILE_LENGTH）无效**：MTE2 在 32KB burst 已达峰值带宽，12.5% 增量不足以触发带宽台阶，remainder tile 的 scalar 开销反而侵蚀了任何微小的收益。

5. **SIMT block-based（case2）在所有配置下均最差**。非 coalesced 访存同时伤害 Vec 和 MTE2。

6. **调优策略**：在核数充足时 case3 一骑绝尘；若需较少核数（BLOCK_DIM≤32），case3 的优势反而更大（MTE2 瓶颈消除）。

### 进一步优化方向

- **case3 当前已是最优**。如仍有性能压力，唯一突破方向是改变 vec/mte2 比例——Vec 引擎协助 MTE2 或算子融合。
- **case4 可通过增加 STAGE_COUNT 逼近 case3**：STAGE_COUNT=8 时 stride=1，理论上等价于 case3 的 coalesced，但 8× VF 调用开销可能抵消收益。
- **case2 应优先改为 case3 的 stride 访问模式**，非 coalesced 访存的惩罚在单核负载增加时急剧放大。

---

## 八、Case4 优化原理深度分析

### 8.1 Case4 vs Case2 的性能变化

Case4 是 case2（SIMT block-based）的直接改进版，仅在 DB+L2Bypass 配置下有数据。对比如下：

| 指标 | case2 (DB+L2Bypass) | case4 (DB+L2Bypass) | 变化 |
|---|---|---|---|
| Task Duration | 545.5 us | **471.1 us** | **↓13.6%** |
| vec_time | 503.5 us | 405.6 us | **↓19.5%** |
| mte2_time | 537.4 us | 403.1 us | **↓25.0%** |
| mte3_time | 43.1 us | 99.1 us | ↑129.9% |
| vec_ratio | 92.4% | 86.2% | — |
| mte2_ratio | 98.6% | 85.7% | — |

### 8.2 核心优化：缩小单次 VF 调用的 DCache 工作集

#### 问题回顾：case2 的 DCache 困境

```
case2: 单次 asc_vf_call 处理整个 TILE (8192 元素)
┌─────────────────────────────────────────────────────┐
│ 每个线程处理 8 个连续元素 (elems_per_thread = 8)      │
│ Thread N: [N*8, N*8+7]                             │
│ 相邻线程 stride = 8                                  │
│                                                     │
│ DCache 工作集 = 3 数组 × 8192 × 4B = 96KB           │
│ DCache 容量    = MIN_SIMT_DCACHE_BYTES = 32KB       │
│ → 96KB >> 32KB，DCache 严重过载，频繁 evict/reload  │
└─────────────────────────────────────────────────────┘
```

SIMT 访问 UB 数据时，需经过 SIMT DCache。case2 的单次 VF 调用要求 1024 个线程同时处理 8192 个元素（共 3 个数组 = 96KB），远超 DCache 的 32KB 下限，导致：

- **DCache thrashing**：数据被反复逐出和重新加载，Vector 引擎大量时间浪费在等待 DCache 回填上
- **UB 带宽竞争**：DCache miss 触发大量 UB 读请求，与并发的 MTE2 写请求争抢 UB bank/port

#### Case4 的解决：分阶段执行

```cpp
// case4: 分 STAGE_COUNT=2 阶段
for (stage = 0; stage < 2; stage++) {
    // 每阶段只处理 4096 元素 (TILE_LENGTH/2)
    asc_vf_call<floor_mod_simt_block_range<T>>(
        dim3(THREAD_COUNT), input1Addr, input2Addr, dstAddr, offset, curLen);
    AscendC::DataSyncBarrier<AscendC::MemDsbT::UB>();  // 阶段间同步
    offset += curLen;
}
```

```
case4 Stage 0: 处理前半段 [0, 4096)
┌─────────────────────────────────────────────────────┐
│ 每个线程处理 4 个连续元素 (elems_per_thread = 4)      │
│ Thread N: [N*4, N*4+3]                             │
│ 相邻线程 stride = 4  (case2 的一半)                  │
│                                                     │
│ DCache 工作集 = 3 数组 × 4096 × 4B = 48KB           │
│ → 48KB vs 32KB，仍然过载但 miss 率显著下降            │
└─────────────────────────────────────────────────────┘

case4 Stage 1: 处理后半段 [4096, 8192)
│ DCache 被 Stage 1 的新数据替换，同样 ~48KB 工作集     │
│ 注意：Stage 0 和 Stage 1 访问的是同一 UB buffer 的    │
│       不同偏移，DCache 无法跨阶段复用                  │
└─────────────────────────────────────────────────────┘
```

### 8.3 三重收益链条

```
分阶段 (STAGE_COUNT=2)
  │
  ├─→ ① 单阶段 DCache 工作集减半 (96KB → 48KB)
  │      └─→ DCache miss 率下降
  │           └─→ vec_time ↓19.5% (503.5 → 405.6 us)
  │
  ├─→ ② stride 减半 (8 → 4)
  │      └─→ warp 内访存跨度减半 (256→128 元素)
  │           └─→ cache line 利用率提升，进一步减少 miss
  │
  └─→ ③ SIMT 对 UB 的读请求减少
         └─→ MTE2 可获取更多 UB 带宽
              └─→ mte2_time ↓25.0% (537.4 → 403.1 us)
```

### 8.4 代价：阶段间同步开销

case4 的 mte3_time 从 case2 的 43.1us 增加到 99.1us。原因：

- 每阶段后的 `DataSyncBarrier<UB>()` 确保 SIMT 写 UB 完成才进入下一阶段
- 两个阶段的输出写入 UB 后才能被 MTE3 搬出
- 阶段间 barrier 增加了 pipeline bubble，延迟了 MTE3 的启动时机

但这个代价（+56us）远小于 vec_time（-98us）和 mte2_time（-134us）的收益，综合净收益 = 545.5 - 471.1 = **74.4us 提升**。

### 8.5 Case4 与 Case3 的对比

| 维度 | case3 (stride-based) | case4 (staged block) |
|---|---|---|
| vec_time | **305.7 us** ✅ | 405.6 us |
| mte2_time | 444.5 us | **403.1 us** ✅ |
| mte3_time | 108.9 us | **99.1 us** ✅ |
| 访存模式 | stride=1, 完美 coalesced | stride=4, 部分 coalesced |
| DCache 效率 | 最优（连续访问，cache line 充分利用） | 中等（stride 4，仍有跨行浪费） |
| 额外开销 | 无 | 2 次 VF 调用 + 1 次 barrier |

case4 在 MTE2 和 MTE3 方面略优于 case3（403.1 vs 444.5, 99.1 vs 108.9），但 vec_time 差距更大（405.6 vs 305.7），因此总体仍落后 2.7%。case3 的 stride=1 完美 coalesced 访存仍是不可替代的优势。

### 8.5.1 本质：case4 与 case3 解决的是同一个问题

虽然 case3 和 case4 在代码层面改动方式截然不同，但两者的优化目标高度一致——**提升访存局部性、降低 DCache 压力**。case4 可以理解为从 case2 到 case3 的"中间态"：

```
优化路径：降低"等效 stride"以改善 DCache 效率

case2: stride=8, 单次 VF, DCache 工作集 96KB — 基线，最差
  │
  ├─ case4 的路线：保持 block-based 索引公式不变
  │    通过分阶段缩小每线程处理量，间接降低 stride
  │    ↓
  │  case4: stride=4, 2次 VF, DCache 工作集 48KB/阶段 — 中间态
  │    │
  │    └─ 继续增加 STAGE_COUNT...
  │       STAGE_COUNT=8 → stride=1, 8次 VF, 每线程 1 元素 — 到达 case3 的 stride
  │
  └─ case3 的路线：改变索引公式
       从 `tid*8+i` 换为 `tid + i*1024`，从根上消除非连续
       ↓
     case3: stride=1, 单次 VF, 完美 coalesced — 最优
```

| 对比维度 | case3 的策略 | case4 的策略 |
|---|---|---|
| **手段** | 换索引公式，让相邻线程访问相邻地址 | 保持 block-based 索引，分阶段让每线程处理更少元素 |
| **stride 变化** | 8 → 1（一步到位） | 8 → 4（走了一半） |
| **DCache 改善机制** | 同一 warp 32 线程访问连续 32 元素 → 最少 cache line | 每阶段工作集减半 → 减少 capacity miss |
| **额外代价** | 无 | 多次 VF 调用 + barrier |
| **效果** | 最优（458.8us） | 次优（471.1us，仅慢 2.7%） |

两条路线殊途同归，case3 是从算法层面根治，case4 是从工程层面缓解。如果沿 case4 的思路把 `STAGE_COUNT` 推到 8，理论上 stride 也能降到 1，等同于用 8 次 VF 调用的开销模拟 case3 的 coalesced 效果——那就是从 case2 坐"站站停"的慢车最终到达 case3 这个终点站。

### 8.6 参数调优启示

`STAGE_COUNT` 是一个可调参数，存在权衡：

| STAGE_COUNT | 单阶段元素数 | elems_per_thread | stride | DCache 工作集 | VF 调用开销 |
|---|---|---|---|---|---|
| 1 (=case2) | 8192 | 8 | 8 | 96KB | 最小 |
| **2 (=case4)** | **4096** | **4** | **4** | **48KB** | **中等** |
| 4 | 2048 | 2 | 2 | 24KB | 较高 |
| 8 | 1024 | 1 | 1 (=case3 的 stride!) | 12KB | 最高 |

理论上 STAGE_COUNT=8 可使 stride 降为 1（等同于 case3 的 coalesced 效果），但 8 次 VF 调用 + 7 次 barrier 的开销可能抵消收益。实验验证才能确定最优值。

---

## 九、Case5：增大 TILE_LENGTH —— 实测无效与根因分析

Case5 是原 case4（当前 case3 stride 方案）的 MaxTile 尝试，仅作为历史对照保留，不在当前样例实现中展示。

### 9.1 实测数据

| 指标 | case3 (DB+L2Bypass) | case5 (DB+L2Bypass) | 变化 |
|---|---|---|---|
| Task Duration | **458.8 us** | **460.0 us** | **+0.2% (无效果)** |
| vec_time | 305.7 us | 310.4 us | +1.5% |
| mte2_time | 444.5 us | 446.2 us | +0.4% |
| mte3_time | 108.9 us | 107.0 us | -1.7% |
| scalar_time | 3.3 us | **18.7 us** | **+5.7×** |
| icache_miss_rate | — | 0.004 | — |

**结论：case5 完全没有优化效果，Task Duration 与 case3 几乎一致（459.97 vs 458.84 us）。**

### 9.2 为什么增大 TILE_LENGTH 无效

当初的假设是：更大的 MTE2 burst 可能提升 DMA 带宽利用率（如同网络 MTU 越大协议开销占比越低）。实测结果否定了这个假设：

**① MTE2 在 32KB burst 时已达峰值带宽**

MTE2 引擎的 DMA 带宽在 32KB 的 DataCopy 粒度下已经接近硬件峰值。从 32KB → 36KB 的提升（+12.5%）不足以触发带宽台阶效应——MTE2 不是"burst 越大越高效"的简单曲线，而是在达到一定阈值后迅速饱和。32KB 已经过了这个阈值。

**② Remainder tile 引入了 scalar 开销**

scalar_time 从 3.3us 暴增到 18.7us（+5.7×），这是最大的异常信号。case5 的 remainder tile 处理引入了一个额外的迭代分支（`if constexpr (HAS_REMAINDER)` 中的 tile），每次循环都需要判断 tileIdx 是否到达 fullTiles 边界。此外 9216 不能整除带来的索引分散可能导致额外的标量运算开销。

```
case3: 128 个 tile，循环条件简单 (tileIdx < 128)
case5: 113 个 full tile + 1 个 remainder tile，额外的 if constexpr 分支
```

**③ vec/mte2 比例恒定的根本约束**

vec_time 和 mte2_time 均与数据量成线性关系。增大 TILE_LENGTH 同时增大单 tile 的计算量和搬移量，两者的比例完全不变。这就像同时放大矩形的长和宽——面积变了但长宽比不变。瓶颈依然是瓶颈。

```
case3: vec/mte2 ≈ 305.7 / 444.5 = 0.688
case5: vec/mte2 ≈ 310.4 / 446.2 = 0.696  ← 几乎相同
```

### 9.3 教训

在 MTE2 已达峰值带宽的前提下，单纯增大 tile 无法缓解 MTE2-bound。要突破这个瓶颈，必须改变 vec/mte2 的有效比例——要么减少 MTE2 搬移量（算子融合），要么让 Vec 引擎协助数据搬运（架构特性）。这两个方向都需要跨出单算子代码调优的范畴。

---

## 十、BLOCK_DIM=32：减少核数放大 SIMT 优化差异

### 10.1 动机

在原配置（BLOCK_DIM=64）下，case3 和 case4 的差距仅 2.7%（458.8 vs 471.1 us），case4 的 DCache 优化效果被高核数稀释了——每个 core 只处理 128 个 tile，DCache thrashing 的绝对时间不够显著。将 BLOCK_DIM 从 64 降至 32，**每个 core 处理的数据量翻倍（256 tile）**，DCache 压力翻倍，case 之间的差异将被放大。

### 10.2 实测数据（DB+L2Bypass, BLOCK_DIM=32）

| Case | Task Duration(us) | vec_time(us) | vec_ratio | scalar(us) | mte2_time(us) | mte2_ratio | mte3_time(us) | mte3_ratio |
|---|---|---|---|---|---|---|---|---|
| case1 (SIMD) | 1021.4 | 1015.0 | 0.995 | 6.0 | 329.1 | 0.322 | 83.2 | 0.082 |
| case2 (SIMT block) | 1060.6 | 1005.7 | 0.949 | 5.9 | 1051.6 | 0.992 | 81.3 | 0.077 |
| case3 (SIMT stride) | **616.1** | **609.2** | **0.990** | 5.8 | **309.0** | **0.502** | 89.5 | 0.145 |
| case4 (SIMT staged) | 819.8 | 813.4 | 0.993 | 9.7 | 421.4 | 0.514 | 87.2 | 0.106 |

### 10.3 核心发现：排名保持不变，但差距被放大

```
BLOCK_DIM=64 (原)               BLOCK_DIM=32 (新)
  case3: 458.8  🥇               case3: 616.1  🥇  ← 仍然第一
  case4: 471.1  🥈  gap=2.7%     case4: 819.8  🥈  gap=33.1% ← 差距暴增！
  case1: 527.3  🥉               case1: 1021.4 🥉
  case2: 545.5  4                case2: 1060.6 4
```

**排名顺序 case3 > case4 > case1 > case2 在两种 BLOCK_DIM 下完全一致。但 case3 对 case4 的领先优势从 2.7% 暴增到 33.1%（12 倍）。**

### 10.4 瓶颈反转：MTE2-bound → Vec-bound，揭示 case3 的真实计算优势

这是 BLOCK_DIM=32 实验最重要的发现——**瓶颈从 MTE2 反转到 Vec**：

```
                    BLOCK_DIM=64                    BLOCK_DIM=32
case3  vec_ratio    66.7%   (MTE2 是瓶颈)           99.0%   (Vec 是瓶颈！)
       mte2_ratio   97.0%                           50.2%

case4  vec_ratio    86.2%   (接近平衡)              99.3%   (Vec 是瓶颈！)
       mte2_ratio   85.7%                           51.4%
```

**BLOCK_DIM=64 时，case3 被 MTE2 瓶颈"压着打"**：计算再快也没用，Vec 引擎有 33% 时间在等 MTE2。case3 对 case4 的 vec_time 优势（305.7 vs 405.6，快 32.7%）被 MTE2 瓶颈掩盖，端到端仅快 2.7%。

**BLOCK_DIM=32 时，MTE2 瓶颈消失**：每 core 数据量翻倍后，vec_time 也翻倍（~2.0×），但 MTE2 有更长的 Vec 窗口来重叠执行，反而不再阻塞。case3 和 case4 的 mte2_ratio 都降到 ~50%——MTE2 不再是瓶颈。此时 **Vec 成为唯一瓶颈**，case3 对 case4 的 vec_time 优势（609.2 vs 813.4，快 33.5%）直接转化为端到端优势（616.1 vs 819.8，快 33.1%）。

```
BLOCK_DIM=64:  case3 带着手铐跑步（MTE2 拖后腿）
               case4 手脚自由但跑得慢（VF 调用开销大）
               结果：差距不大（2.7%）

BLOCK_DIM=32:  手铐解开（MTE2 不再阻塞）
               case3 的真实速度暴露 → 比 case4 快 33%
```

### 10.5 case3 vs case4 的 vec_time 差距溯源

两种 BLOCK_DIM 下，case4 的 vec_time 始终比 case3 慢 ~33%：

| | BLOCK_DIM=64 vec_time | BLOCK_DIM=32 vec_time | case4/case3 比例 |
|---|---|---|---|
| case3 | 305.7 us | 609.2 us | — |
| case4 | 405.6 us | 813.4 us | **1.327× / 1.335×** |

这个 ~33% 的差距来自 case4 相对 case3 的两个结构性劣势：

**① stride=4 vs stride=1**：case4 每线程 stride=4，warp 内 32 线程访问不连续（跨 128 元素），DCache line 利用率低于 case3 的 stride=1（跨 32 元素完美 coalesced）。

**② 双倍 VF 调用 + barrier 开销**：case4 每 tile 执行 2 次 `asc_vf_call` + 1 次 `DataSyncBarrier`，256 tiles 共 512 次 VF 调用；case3 仅 256 次。每次 VF 调用有固定的启动/同步开销。

vec_time 比例恒定（~1.33×）说明这是结构性差距，与数据量无关。

### 10.6 为什么 MTE2 瓶颈在 BLOCK_DIM=32 下消失了

这是一个违反直觉的现象——数据量翻倍，MTE2 反而不再阻塞：

```
BLOCK_DIM=64 (128 tiles):  Vec 窗口窄 (306us)，MTE2 需要 445us 搬完 128×64KB
                           → MTE2 来不及在 Vec 窗口内完成，成为瓶颈

BLOCK_DIM=32 (256 tiles):  Vec 窗口宽 (609us)，MTE2 只需 309us 搬完 256×64KB
                           → MTE2 有充足时间在 Vec 窗口内完成
```

Vec 窗口从 306us 扩到 609us（+2.0×），但 MTE2 的有效活跃时间从 445us 降到 309us（−30%）。这是因为 MTE2 与 Vec 的重叠效率在长 Vec 窗口中大幅提升——MTE2 可以更连续地运行，减少了引擎启动/停止的间隙损耗。

### 10.7 关键结论

1. **排名一致性**：case3 > case4 > case1 > case2 的排序在 BLOCK_DIM=64 和 BLOCK_DIM=32 下完全一致，说明这是由访存和计算模式决定的稳定性能关系，不是偶然。

2. **瓶颈决定可见度**：BLOCK_DIM=64 时 MTE2 瓶颈"遮蔽"了 case3 的真实计算优势；BLOCK_DIM=32 解除 MTE2 瓶颈后，case3 的 33% vec 优势完整曝光。**调优效果是否可见，取决于当前瓶颈在哪个引擎上。**

3. **case4 的价值仍在**：虽然 case4 始终慢于 case3，但它用分阶段策略解决了 case2 的 DCache 灾难（819.8 vs 1060.6，快 22.7%），是 case2→case3 中间的有效过渡方案。

4. **case3 是 floor_mod 的最优实现**，在所有 Block 数配置下均保持第一。
