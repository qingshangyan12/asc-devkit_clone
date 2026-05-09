# ND2NZ Bank 冲突调优样例

## 概述

本样例以 `8192 × 8192` half ND 矩阵到全局紧凑 NZ 布局的转换为例，介绍基于静态 Tensor 方式编程时，UB 上 scatter 写阶段的 bank 冲突如何产生、如何观测、如何规避。整个调优过程分为两个步骤（case 1-2），通过切换 `dstNzC0Stride` 这一个参数对比性能。

**优化路径**：
- Case 1：`dstNzC0Stride = TILE_H = 128`（与 bank-group 周期同模，触发冲突）
- Case 2：`dstNzC0Stride = TILE_H + 1 = 129`（错开 1 个 DataBlock，规避冲突）

两个架构共用同一份代码，只在 Vec 侧 scatter 的 API 上有差异；冲突机制完全同构：
- Atlas A2/A3 (`dav-2201`)：scatter 走 `AscendC::Copy()` (vcopy)，bank-group 周期 = 16 DataBlock
- Ascend 950PR/DT (`dav-3510`)：scatter 走 `Reg::LoadAlign` + `Reg::StoreAlign<DATA_BLOCK_COPY>`，bank-group 周期 = 8 DataBlock

`128` 是两个架构 bank-group 周期的公倍数，所以两种硬件下都会触发冲突；`129` 同时不被两个周期整除，所以都规避。

## 支持的产品
- Ascend 950PR/Ascend 950DT
- Atlas A3 训练系列产品/Atlas A3 推理系列产品
- Atlas A2 训练系列产品/Atlas A2 推理系列产品

## 目录结构介绍

```text
├── bank_conflict_nd2nz
│   ├── scripts
│   │   ├── gen_data.py            // 输入数据和真值数据生成脚本文件
│   │   └── verify_result.py       // 真值对比文件
│   ├── CMakeLists.txt             // 编译工程文件
│   ├── data_utils.h               // 数据读入写出函数
│   └── bank_conflict_nd2nz.asc    // Ascend C 样例实现（含 2 个 case）
```

## 样例描述

**样例功能**：

  样例实现的是固定 shape 为 `8192 × 8192` 的 half ND 矩阵到全局紧凑 NZ 布局的转换。

  ND → NZ 转换的本质是一次大规模 scatter：把每行的 16 个 C0 块（每块 32B）"撒"到 NZ 布局里 16 个不同 C0 列的对应行槽里。dstNzC0Stride 控制 UB 内相邻 C0 列的落点间距，是 bank 冲突的核心旋钮：

  - x：输入，shape `[8192, 8192]`，half，ND 行优先布局
  - z：输出，shape `[8192, 8192]`，half，全局紧凑 NZ 布局
  - tile：固定 `128 × 256`，共 `64 × 32 = 2048` 个 tile
  - 核函数名：`bank_conflict_nd2nz`

**关键参数**：

| 参数 | 值 | 含义 |
|:---:|:---:|:---|
| `TILE_H` | 128 | tile 内 N 方向行数 |
| `TILE_W` | 256 | tile 内 D 方向 half 数 |
| `TILE_C0_COLS` | 16 | tile 内 D 方向 C0 列数 |
| `DST_NZ_N_STRIDE` | 1 | UB 内 N 方向步长（DataBlock） |
| `DST_NZ_C0_STRIDE` | 128 (S1) / 129 (S2) | UB 内 C0 列间步长（DataBlock） |
| `SCATTER_LEN_DBS` | 8 | 单条 Vec scatter 指令处理的 DataBlock 数 |

**ND → NZ scatter 数据流**：

```text
源 ndBuf（一行 16 个 C0 块紧凑相邻，DataBlock 索引 0..15）：
┌──┬──┬──┬──┬──┬──┬──┬──┬──┬──┬──┬──┬──┬──┬──┬──┐
│c0│c1│c2│c3│c4│c5│c6│c7│c8│c9│cA│cB│cC│cD│cE│cF│
└──┴──┴──┴──┴──┴──┴──┴──┴──┴──┴──┴──┴──┴──┴──┴──┘
└─ 一次 scatter (8 块)─┘└──── 第二次 scatter ────┘

目标 nzBuf（NZ 紧凑布局 + 列尾空洞，按 DST_NZ_C0_STRIDE 散开）：
       DataBlock 索引 →
       0    1   ...  127    128   129  ...  255    256 ...
列 0:  [c0][r1][r2]...[r127] .                                    │
列 1:                       │ [c1][r1][r2]...[r127] .             │  S2 (stride=129)
列 2:                                                │ [c2]...    │  时每段尾部
       ↑                    ↑                       ↑               有 1 块空洞
       第 0 列起点          第 1 列起点              第 2 列起点
       (DB 0)               (DB DST_NZ_C0_STRIDE)   (DB 2*stride)

scatter 的本质 = 源连续读、目的按大 stride 写；落点间距由 DST_NZ_C0_STRIDE 控制。
```

## 样例实现

### 性能指标说明

**表 1 AI Core 性能指标字段说明表**
| 字段名 | 字段含义 |
|:---:|:---|
| Task Duration(μs)|Task整体耗时，包含调度到加速器的时间、加速器上的执行时间以及响应结束时间。|
| aiv_time|Task在AI Vector Core上的理论执行时间，单位为μs。|
| aiv_vec_time(μs) | vec类型指令（向量类运算指令）耗时，单位μs。 |
| aiv_vec_ratio | vec类型指令（向量类运算指令）的cycle数在total cycle数中的占用比。 |
| aiv_scalar_time(μs) | scalar类型指令（标量类运算指令）耗时，单位μs。 |
| aiv_scalar_ratio | scalar类型指令（标量类运算指令）的cycle数在total cycle数中的占用比。 |
| aiv_mte2_time(μs) | mte2类型指令（GM->UB搬运类指令）耗时，单位μs。 |
| aiv_mte2_ratio | mte2类型指令（GM->UB搬运类指令）的cycle数在total cycle数中的占用比。 |
| aiv_mte3_time(μs) | mte3类型指令（UB->GM搬运类指令）耗时，单位μs。 |
| aiv_mte3_ratio | mte3类型指令（UB->GM搬运类指令）的cycle数在total cycle数中的占用比。 |

### Case 1: dstNzC0Stride = 128（触发 Bank 冲突）

**实现方式**：编译时 `-DSCENARIO_NUM=1`。

代码中 `DST_NZ_C0_STRIDE` 取 `TILE_H = 128`，让 UB 内 tile-NZ 布局紧凑无空洞。这是 NZ 布局的"自然写法"，但会触发 bank 冲突。

**关键代码**：
```cpp
constexpr uint32_t DST_NZ_C0_STRIDE = (SCENARIO == 1) ? TILE_H : (TILE_H + 1);
```

**架构特化的 scatter 实现**：

- `dav-2201`：vcopy 的 `Copy()` 自带 repeat，单条 Copy 覆盖 TILE_H=128 行 × 8 个 C0 列。一行 16 个 C0 列要 2 次 Copy。

  ```cpp
  AscendC::CopyRepeatParams copyParams;
  copyParams.dstStride     = DST_NZ_C0_STRIDE;    // = 128，scatter 列间距
  copyParams.srcStride     = 1;
  copyParams.dstRepeatSize = DST_NZ_N_STRIDE;
  copyParams.srcRepeatSize = TILE_W / C0_ELEMS;

  for (uint32_t k = 0; k < SCATTERS_PER_ROW; ++k) {
      AscendC::Copy(nzBuf[k * SCATTER_LEN_DBS * DST_NZ_C0_STRIDE * C0_ELEMS],
                    ndBuf[k * SCATTER_LEN_HALF],
                    static_cast<uint64_t>(SCATTER_LEN_HALF),
                    static_cast<uint8_t>(TILE_H),
                    copyParams);
  }
  ```

- `dav-3510`：950 没有 vcopy/Copy 的 repeat 机制，手写 `__simd_vf__`，外层 scatter 列组、内层行循环。

  ```cpp
  for (uint16_t k = 0; k < SCATTERS_PER_ROW; ++k) {
      for (uint16_t r = 0; r < TILE_H; ++r) {
          AscendC::Reg::LoadAlign<half>(reg, srcAddr + r * TILE_W + k * SCATTER_LEN_HALF);
          AscendC::Reg::StoreAlign<half, AscendC::Reg::DataCopyMode::DATA_BLOCK_COPY>(
              dstRowAddr, reg, DST_NZ_C0_STRIDE, mask);
      }
  }
  ```

**冲突原理**：

单条 Vec scatter 指令一次写 `SCATTER_LEN_DBS = 8` 个 32B 块到 UB，落点间距 = `DST_NZ_C0_STRIDE` 个 DataBlock。一个写命中哪个 bank-group：

```
bank_group_id = (落点 DataBlock 索引) % bank_groups
```

当 `DST_NZ_C0_STRIDE % bank_groups == 0` 时，8 个落点全部落到同一个 bank group：

| 架构 | bank groups | bank-group 周期 | 128 % bank_groups | 结果 |
|:---:|:---:|:---:|:---:|:---:|
| 2201 | 16 | 16 DataBlock | 128 % 16 = 0 | 8 个写全打同一 bank group，**8 拍**完成 1 个 repeat 的写入 |
| 3510 | 8 | 8 DataBlock | 128 % 8 = 0 | 同上，**8 拍**完成 1 条 StoreAlign |

理论上单条指令延迟从 1 拍拉长为 8 拍，aiv_vec_time 显著膨胀。

**S1 落点示意图**（以 950 为例，8 bank groups，stride=128）：

```text
单条 StoreAlign 的 8 个写在 UB 上的落点（DataBlock 索引）：
  写 #0 → DB   0    bank_group = 0 % 8 = 0
  写 #1 → DB 128    bank_group = 128 % 8 = 0   ◀ 冲突
  写 #2 → DB 256    bank_group = 256 % 8 = 0   ◀ 冲突
  写 #3 → DB 384    bank_group = 384 % 8 = 0   ◀ 冲突
  写 #4 → DB 512    bank_group = 512 % 8 = 0   ◀ 冲突
  写 #5 → DB 640    bank_group = 640 % 8 = 0   ◀ 冲突
  写 #6 → DB 768    bank_group = 768 % 8 = 0   ◀ 冲突
  写 #7 → DB 896    bank_group = 896 % 8 = 0   ◀ 冲突

  bank group:    0       1       2       3       4       5       6       7
              ┌──────┬──────┬──────┬──────┬──────┬──────┬──────┬──────┐
              │ ●●●●●│      │      │      │      │      │      │      │
              │ ●●●  │      │      │      │      │      │      │      │
              └──────┴──────┴──────┴──────┴──────┴──────┴──────┴──────┘
                 ↑
                 8 个写全堆在 bank group 0 → 排队 8 拍才能写完
```
**性能数据**（Atlas A2 系列产品，48 核）：

| Task Duration(μs) | aiv_time(μs) | aiv_vec_time(μs) | aiv_vec_ratio | aiv_scalar_time(μs) | aiv_scalar_ratio | aiv_mte2_time(μs) | aiv_mte2_ratio | aiv_mte3_time(μs) | aiv_mte3_ratio |
|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|
| 182.16 | 175.4| **47.856** | 0.273 | 4.716 | 0.027 | 61.046 | 0.348 | 57.284| 0.327|

**性能数据**（Ascend 950PR/DT，64 核）：

| Task Duration(μs) | aiv_time(μs) | aiv_vec_time(μs) | aiv_vec_ratio | aiv_scalar_time(μs) | aiv_scalar_ratio | aiv_mte2_time(μs) | aiv_mte2_ratio | aiv_mte3_time(μs) | aiv_mte3_ratio |
|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|
| 150.952 | 150.13 | **44.426** | 0.296 | 1.551 | 0.01 | 70.06 | 0.467 | 32.396 | 0.216|

**优化效果分析**：
- 两个架构下 `aiv_vec_time` 均显著高于规避场景：2201 = 47.856 μs（vs Case 2 的 6.528），3510 = 44.426 μs（vs Case 2 的 9.406）。
- 冲突点在 Vec 写阶段：2201 上 vcopy 的 8 个 32B 块全打 bank group 0；3510 上 VEC_STORE 流水内每条 StoreAlign 都因 bank-group 冲突排队 8 拍。
- 读侧（2201 vcopy 的源、3510 VEC_LOAD 读 ndBuf）紧凑无冲突，不是瓶颈。

**性能瓶颈定位**：
> ⚠️ **bank 冲突在 Vec 子流水内部**
>
> 在 Regbase 架构下，`Reg::LoadAlign` 走 **VEC_LOAD** 子流水、`Reg::StoreAlign` 走 **VEC_STORE** 子流水（出处：`docs/api/context/LocalMemBar.md`），不在 MTE 上。常规 MTE 类 bank-conflict 计数器看不到，需要看 vec 流水或单指令周期数。

---

### Case 2: dstNzC0Stride = 129（规避 Bank 冲突）

**实现方式**：编译时 `-DSCENARIO_NUM=2`。

代码中 `DST_NZ_C0_STRIDE` 取 `TILE_H + 1 = 129`，每个 C0 列尾部多出 1 个 DataBlock 的空洞，scatter 落点在 bank group 间轮转。

**关键代码**：源码完全一样，只是宏 `SCENARIO=2`，编译期推导出不同的 `DST_NZ_C0_STRIDE`。

**规避原理**：

`129 % 8 = 1`、`129 % 16 = 1`，落点 bank-group 索引序列在两个架构下分别是：

| 架构 | bank groups | 落点 bank-group 序列 | 结果 |
|:---:|:---:|:---|:---:|
| 2201 | 16 | 0, 1, 2, ..., 7（8 个落点，每个占一个 group） | 8 个写并发，**1 拍**完成 |
| 3510 | 8 | 0, 1, 2, ..., 7（8 个落点，每个占一个 group） | 同上，**1 拍**完成 1 条 StoreAlign |

**S2 落点示意图**（以 950 为例，8 bank groups，stride=129）：

```text
单条 StoreAlign 的 8 个写在 UB 上的落点（DataBlock 索引）：
  写 #0 → DB   0    bank_group =   0 % 8 = 0
  写 #1 → DB 129    bank_group = 129 % 8 = 1
  写 #2 → DB 258    bank_group = 258 % 8 = 2
  写 #3 → DB 387    bank_group = 387 % 8 = 3
  写 #4 → DB 516    bank_group = 516 % 8 = 4
  写 #5 → DB 645    bank_group = 645 % 8 = 5
  写 #6 → DB 774    bank_group = 774 % 8 = 6
  写 #7 → DB 903    bank_group = 903 % 8 = 7

  bank group:    0       1       2       3       4       5       6       7
              ┌──────┬──────┬──────┬──────┬──────┬──────┬──────┬──────┐
              │  ●   │  ●   │  ●   │  ●   │  ●   │  ●   │  ●   │  ●   │
              │      │      │      │      │      │      │      │      │
              └──────┴──────┴──────┴──────┴──────┴──────┴──────┴──────┘
                 ↑      ↑      ↑      ↑      ↑      ↑      ↑      ↑
                 8 个写均匀分散到 8 个 bank groups → 1 拍并发完成
```

**UB 占用变化**：

`nzBuf` 占用 DataBlock 数 = `(TILE_H - 1) * DST_NZ_N_STRIDE + (TILE_C0_COLS - 1) * DST_NZ_C0_STRIDE + 1`

| 场景 | DataBlock 数 | UB 占用 |
|:---:|:---:|:---:|
| S1 (stride=128) | 2048 | 64 KB（紧凑） |
| S2 (stride=129) | 2063 | ≈64.5 KB（含 15 个 32B 空洞） |

空洞在 UB→GM 的 MTE3 阶段由 `outParams.srcStride = DST_NZ_C0_STRIDE - TILE_H` 跳过，不写回 GM。

**性能数据**（Atlas A2 系列产品，48 核）：

| Task Duration(μs) | aiv_time(μs) | aiv_vec_time(μs) | aiv_vec_ratio | aiv_scalar_time(μs) | aiv_scalar_ratio | aiv_mte2_time(μs) | aiv_mte2_ratio | aiv_mte3_time(μs) | aiv_mte3_ratio |
|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|
| 177.26 | 166.24| **6.528** | 0.039 | 2.264 | 0.014 | 70.39 | 0.423 | 82.344 | 0.495|

**性能数据**（Ascend 950PR/DT，64 核）：

| Task Duration(μs) | aiv_time(μs) | aiv_vec_time(μs) | aiv_vec_ratio | aiv_scalar_time(μs) | aiv_scalar_ratio | aiv_mte2_time(μs) | aiv_mte2_ratio | aiv_mte3_time(μs) | aiv_mte3_ratio |
|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|
| 142.151 | 141.27 | **9.406** | 0.064 | 1.507 | 0.011 | 86.995 | 0.616 | 41.492 | 0.294|

**优化效果分析**：
- 2201：`aiv_vec_time` 从 47.856 → 6.528 μs（提升 **7.33×**），与"延迟从 8 拍降到 1 拍"的理论几乎吻合。
- 3510：`aiv_vec_time` 从 44.426 → 9.406 μs（提升 **4.72×**）。Reg 路径每次 scatter 包含 1 拍 `LoadAlign` 和 1 拍 `StoreAlign`；冲突只放大 `StoreAlign`，因此理论加速比约为 `(1 + 8) / (1 + 1) = 4.5×`，与实测接近。
- 代价是每 tile 多出 15 × 32B = 480B 的 UB 空洞（约 0.7% 浪费），完全可接受。

**性能优化建议**：
> 💡 **stride 错开 1 个 DataBlock 即可消除 bank 冲突**
>
> 1. 当 scatter 的 stride 是 bank-group 周期（2201 为 16、3510 为 8）的整数倍时必触发冲突；
> 2. 把 stride 改成"周期 + 1"（或任意与周期互质的偏移）即可规避，代价只是每段尾部 1 个 DataBlock 的空洞；
> 3. UB 空洞由 MTE3 的 `srcStride` 跳过，不影响 GM 输出正确性。

---

## 性能对比总结

### Atlas A2 系列性能对比

| Case | dstNzC0Stride | scatter 落点模式 | Task Duration(μs) | aiv_vec_time(μs) | 理论vector耗时(μs) | 读带宽(TB/S) | 写带宽(TB/S) |
|:---:|:---:|:---|:---:|:---:|:---:|:---:|:---:|
| 1 | 128 | 8 个 DataBlock 全打 bank group 0 | 182.16 | **47.856** | 47.233 | 2.199 | 2.343 |
| 2 | 129 | 8 个 DataBlock 散到 bank groups 0~7 | 177.26 | **6.528** | 5.904 | 1.907 | 1.630 |

表中的“理论vector耗时”只统计 UB 上 Vec scatter 阶段，不包含 GM→UB、UB→GM 搬运、事件同步和 scalar 调度开销。Atlas A2 系列产品上，单条 vcopy 每拍处理 `128` 个 half，主频按 `1.85GHz`、核数按 `48` 计算。无冲突基础耗时为：

$$
T_{\text{base}} = \frac{M \times N}{128 \times 1.85 \times 10^9 \times 48}
                = \frac{8192 \times 8192}{1.13664 \times 10^{13}}
                \approx 5.904\mu s
$$

Case 1 中 `dstNzC0Stride = 128`，8 个写落到同一个 bank group，单条 scatter 从 1 拍拉长到 8 拍，因此：

$$
T_{\text{S1}} = 8 \times T_{\text{base}} \approx 47.233\mu s
$$

Case 2 中 `dstNzC0Stride = 129`，8 个写均匀分散到不同 bank group，因此：

$$
T_{\text{S2}} = 1 \times T_{\text{base}} \approx 5.904\mu s
$$

可以看到实测 `aiv_vec_time` 分别为 `47.856μs` 和 `6.528μs`，与理论值非常接近。端到端 Task Duration 提升较小（仅 2.7%），是因为整体仍包含 MTE2/MTE3 搬运和同步开销，bank 冲突优化主要体现在 Vec 子流水。

### Ascend 950PR/Ascend 950DT 性能对比

| Case | dstNzC0Stride | scatter 落点模式 | Task Duration(μs) | aiv_vec_time(μs) | 理论vector耗时(μs) | 读带宽(TB/S) | 写带宽(TB/S) |
|:---:|:---:|:---|:---:|:---:|:---:|:---:|:---:|
| 1 | 128 | 8 个 DataBlock 全打 bank group 0 | 150.952 | **44.426** | 44.684 | 1.916 | 4.143 |
| 2 | 129 | 8 个 DataBlock 散到 bank groups 0~7 | 142.151 | **9.406** | 9.930 | 1.543 | 3.235 |

Ascend 950PR/Ascend 950DT 上，Reg 路径每处理一组 `128` 个 half 数据，需要执行一次 `LoadAlign` 和一次 `StoreAlign<DATA_BLOCK_COPY>`。参考 Reg 样例的理论性能计算方式，先分析 VF 内部指令的等效吞吐：

| 指令 | 数据类型 | 无冲突并行度 | Case 1 等效并行度 | Case 2 等效并行度 |
|:---:|:---:|:---:|:---:|:---:|
| `LoadAlign` | half | 256 bytes/cycle | 256 bytes/cycle | 256 bytes/cycle |
| `StoreAlign<DATA_BLOCK_COPY>` | half | 256 bytes/cycle | 32 bytes/cycle | 256 bytes/cycle |

其中 `StoreAlign` 一次写 8 个 DataBlock。Case 1 中 8 个写落到同一个 bank group，写入串行化为 8 拍，等效并行度从 `256 bytes/cycle` 降为 `32 bytes/cycle`；Case 2 中 8 个写分散到不同 bank group，保持 `256 bytes/cycle`。

**Step 1：计算基础吞吐**

单条 `LoadAlign` 或无冲突 `StoreAlign` 每拍处理 `128` 个 half，即 `256 bytes/cycle`。主频按 `1.65GHz`、核数按 `64` 计算，单遍处理全量数据的基础耗时为：

$$
T_{\text{base}} = \frac{M \times N}{128 \times 1.65 \times 10^9 \times 64}
                = \frac{8192 \times 8192}{1.35168 \times 10^{13}}
                \approx 4.965\mu s
$$

**Step 2：计算两种场景的理论vector耗时**

`LoadAlign` 从 `ndBuf` 连续读取，不触发该 scatter 写冲突，按 1 拍计；`StoreAlign` 写 `nzBuf` 时受 `dstNzC0Stride` 影响。Case 1 的 Store 写冲突为 8 拍，Case 2 的 Store 写无冲突为 1 拍。

需要注意的是，这里并不是说硬件上的 VEC_LOAD 和 VEC_STORE 完全不能并行，而是按当前实现的等效关键路径估算：代码每次 `LoadAlign` 后立即用同一个 `reg` 执行 `StoreAlign`，Store 对 Load 存在数据依赖，下一轮循环也复用同一个 `reg`，因此本样例中按 `Load + Store` 的等效拍数计算。这里的 $1_{\text{Load}}$ 和 $1_{\text{Store}}$ 表示每组 `128` 个 half 数据对应的等效拍数，$T_{\text{base}}$ 已经表示全量数据按 1 拍/组处理一遍的耗时。因此：

$$
T_{\text{S1}} = (1_{\text{Load}} + 8_{\text{Store}}) \times T_{\text{base}} \approx 44.684\mu s
$$

$$
T_{\text{S2}} = (1_{\text{Load}} + 1_{\text{Store}}) \times T_{\text{base}} \approx 9.930\mu s
$$

实测 `aiv_vec_time` 分别为 `44.426μs` 和 `9.406μs`，同样与理论值接近。如果按 Load/Store 理想完全重叠估算，Case 2 会接近 $1 \times T_{\text{base}} = 4.965\mu s$，与实测差距较大；当前实测更符合 `Load + Store` 的等效关键路径模型。950 上整体提升不是 8×，是因为无冲突场景仍包含 `LoadAlign + StoreAlign` 两类 Vec 指令开销，理论加速比为 `9 / 2 = 4.5×`，实测为 `4.72×`。

**Step 3：Reg 指令周期反推**

950 场景下共有 `64` 个 AIV 核，每核处理 `32` 个 tile；每个 tile 有 `2 × 128` 组 Reg scatter。因此单核 Reg 指令数量为：

$$
N_{\text{instr}} = N_{\text{tile}} \times N_{\text{scatter}} \times N_{\text{row}} \times N_{\text{op}}
                 = 32 \times 2 \times 128 \times 2
                 = 16384
$$

式中：
- $N_{\text{tile}} = 32$：每核处理的 tile 数；
- $N_{\text{scatter}} = 2$：每行 16 个 C0 块，需要 2 次 scatter；
- $N_{\text{row}} = 128$：每个 tile 的行数；
- $N_{\text{op}} = 2$：每组 scatter 包含 `LoadAlign` 和 `StoreAlign` 两条 Reg 访存指令。

本样例不是 Gelu 那类多条 Reg 计算指令的 VF 融合双发场景，核心瓶颈是 `StoreAlign` 写 UB 时的 bank 冲突。因此这里不使用 IPC 评价 VF 融合效率，只按实测 `aiv_vec_time` 反推单核 vector cycle，并与理论 cycle 对比：

| Case | aiv_vec_time(μs) | 实测单核 vector cycles | 理论单核 vector cycles | 单核 Reg 指令数 |
|:---:|:---:|:---:|:---:|:---:|
| 1 | 44.426 | 73303 | 73728 | 16384 |
| 2 | 9.406 | 15520 | 16384 | 16384 |

其中：

$$
Cycles_{\text{vec}} = T_{\text{vec}} \times f
$$

理论单核 vector cycles 的计算方式为：

$$
Cycles_{\text{S1}} = N_{\text{tile}} \times N_{\text{scatter}} \times N_{\text{row}} \times (1_{\text{Load}} + 8_{\text{Store}})
                   = 32 \times 2 \times 128 \times 9 = 73728
$$

$$
Cycles_{\text{S2}} = N_{\text{tile}} \times N_{\text{scatter}} \times N_{\text{row}} \times (1_{\text{Load}} + 1_{\text{Store}})
                   = 32 \times 2 \times 128 \times 2 = 16384
$$

可以看到实测 cycle 与理论 cycle 接近，说明 Case 1 的 vec 耗时主要来自 `StoreAlign` 写 bank 冲突导致的 8 拍串行化；Case 2 消除写冲突后，cycle 数基本回到 `LoadAlign + StoreAlign` 的正常访存指令开销。

本样例只有一个输入矩阵和一个输出矩阵，读写数据量均为：

$$
D = 8192 \times 8192 \times sizeof(half) = 134217728\text{ Byte}
$$

表中的有效读/写带宽按 profiler 中对应流水耗时估算：

$$
BW_{\text{read}} = \frac{D}{T_{\text{mte2}}}, \quad
BW_{\text{write}} = \frac{D}{T_{\text{mte3}}}
$$

例如 Atlas A2 Case 2：

$$
BW_{\text{read}} = \frac{134217728}{70.39 \times 10^{-6}}
                 \approx 1.907 \times 10^{12}\text{ B/s}
                 \approx 1.907\text{ TB/S}
$$

这里的带宽是按样例 profiler 时间反推的有效带宽，用于 case 间对比，不等同于单一物理通路的峰值带宽。

### 优化要点总结

| 优化手段 | 核心原理 | 适用场景 |
|:---|:---:|:---|
| stride + 1 偏移 | 让 scatter 落点的 bank-group 索引轮转，避免 1 拍内多次访问同一 group | ND2NZ / NZ2ND / 转置 / 任何 stride 为 bank-group 周期整数倍的 scatter |

---

## 编译运行

- 切换 Case

  在 cmake 编译时通过 `-DSCENARIO_NUM=N` 指定要编译的 case：

  ```bash
  cmake -DSCENARIO_NUM=1 -DCMAKE_ASC_ARCHITECTURES=dav-2201 ..   # 编译 case 1（可替换为 1 或 2）
  ```

  各 case 说明：
  - `1`: `dstNzC0Stride = 128`，触发 bank 冲突
  - `2`: `dstNzC0Stride = 129`，规避 bank 冲突

- 配置环境变量
  请根据当前环境上 CANN 开发套件包的[安装方式](../../../../../../docs/quick_start.md#prepare&install)，选择对应配置环境变量的命令。
  - 默认路径，root 用户安装 CANN 软件包
    ```bash
    source /usr/local/Ascend/cann/set_env.sh
    ```

  - 默认路径，非 root 用户安装 CANN 软件包
    ```bash
    source $HOME/Ascend/cann/set_env.sh
    ```

  - 指定路径 install_path，安装 CANN 软件包
    ```bash
    source ${install_path}/cann/set_env.sh
    ```

- 样例执行

  ```bash
  mkdir -p build && cd build   # 创建并进入build目录
  cmake -DSCENARIO_NUM=1 -DCMAKE_ASC_ARCHITECTURES=dav-2201 ..;make -j;  # 编译工程，默认npu模式
  python3 ../scripts/gen_data.py   # 生成测试输入数据
  ./demo                           # 执行（使用编译时指定的case）
  python3 ../scripts/verify_result.py output/output.bin output/golden.bin   # 验证输出结果是否正确，确认算法逻辑正确
  ```

  使用 CPU 调试 或 NPU 仿真 模式时，添加 `-DCMAKE_ASC_RUN_MODE=cpu` 或 `-DCMAKE_ASC_RUN_MODE=sim`：

  ```bash
  cmake -DSCENARIO_NUM=1 -DCMAKE_ASC_RUN_MODE=cpu -DCMAKE_ASC_ARCHITECTURES=dav-2201 ..   # CPU 调试模式
  cmake -DSCENARIO_NUM=1 -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-2201 ..   # NPU 仿真模式
  ```

  > **注意：** 切换编译模式或场景前需清理 cmake 缓存，可在 build 目录下执行 `rm CMakeCache.txt` 后重新 cmake。

- 编译选项说明

  | 选项 | 可选值 | 说明 |
  |------|--------|------|
  | `CMAKE_ASC_RUN_MODE` | `npu`（默认）、`cpu`、`sim` | 运行模式：NPU 运行、CPU 调试、NPU 仿真 |
  | `CMAKE_ASC_ARCHITECTURES` | `dav-2201`（默认）、`dav-3510` | NPU 架构：`dav-2201` 对应 Atlas A2/A3 训练/推理系列，`dav-3510` 对应 Ascend 950PR/Ascend 950DT |
  | `SCENARIO_NUM` | `1`（默认）、`2` | bank 冲突场景编号 |

  执行结果如下，说明精度对比成功：
  ```bash
  test pass!
  ```

## 性能分析

使用 `msprof` 工具获取详细性能数据：

```bash
msprof ./demo
```

当前目录下会生成 `PROF_` 前缀的文件夹，`mindstudio_profiler_output` 目录保存 Host 和各 Device 的性能数据汇总：

```text
PROF_xxxx_XXXXXX
├── device_{id}
└── host
└── mindstudio_profiler_log
└── mindstudio_profiler_output    # 保存 Host 和各 Device 的性能数据汇总
    ├── msprof_*.json
    ├── xx_*.csv
    └── README.txt
```

查看具体的性能分析结果：
```bash
cat ./PROF_*/mindstudio_profiler_output/op_summary_*.csv
```
