# ND2NZ Bank 冲突调优样例

## 概述

本样例以 `8192 × 8192` half ND 矩阵到全局紧凑 NZ 布局的转换为例，介绍基于静态 Tensor 方式编程时，UB 上向量写阶段的 bank 冲突如何产生、如何观测、如何规避。整个调优过程分为两个步骤（case 1-2），通过切换 `dstNzC0Stride` 这一个参数对比性能。

## 支持的产品
- Ascend 950PR/Ascend 950DT
- Atlas A3 训练系列产品/Atlas A3 推理系列产品
- Atlas A2 训练系列产品/Atlas A2 推理系列产品

## 目录结构介绍

```text
├── bank_conflict_nd2nz
│   ├── figures
│   ├── scripts
│   │   ├── gen_data.py            // 输入数据和真值数据生成脚本文件
│   │   └── verify_result.py       // 真值对比文件
│   ├── CMakeLists.txt             // 编译工程文件
│   ├── data_utils.h               // 数据读入写出函数
│   ├── bank_conflict_nd2nz.asc    // Ascend C 样例实现（含 2 个 case）
│   └── bank_conflict_nd2nz_3510.h // dav-3510 Regbase ND2NZ 实现
```

## 样例描述

**样例功能**：

  样例实现的是固定 shape 为 `8192 × 8192` 的 half ND 矩阵到全局紧凑 NZ 布局的转换。

  ND → NZ 转换的本质是一次大规模向量重排写：把 tile 内每行的 8 个 C0 块（每个 C0 块 = 16 half = 32B = 1 个 DataBlock）"撒"到 NZ 布局里 8 个不同 C0 列的对应行槽里。`dstNzC0Stride` 控制 UB 内相邻 C0 列的落点间距，是 bank 冲突的核心旋钮：

  - x：输入，shape `[8192, 8192]`，half，ND 行优先布局
  - z：输出，shape `[8192, 8192]`，half，全局紧凑 NZ 布局

**分块与分核**：

`8192 × 8192` 输入按 N / D 两个方向切成 `64 × 64 = 4096` 个 tile（每 tile `128 × 128` half），tile 按行优先编号 `tileId = tileRow * 64 + tileCol`：

```text
         tileCol →   0      1      2     ...    62      63
   tileRow ↓       ┌──────┬──────┬──────┬─────┬──────┬──────┐
       0           │  #0  │  #1  │  #2  │ ... │ #62  │ #63  │
       1           │ #64  │ #65  │ #66  │ ... │ #126 │ #127 │
       2           │ #128 │ #129 │ #130 │ ... │ #190 │ #191 │
       ...         │                  ......                │
      63           │#4032 │#4033 │ ...  │ ... │ ...  │#4095 │
                   └──────┴──────┴──────┴─────┴──────┴──────┘
```

`tileId` 按顺序平均分给各核（核内再用 `tileRow = tileId / 64`、`tileCol = tileId % 64` 还原坐标，进而算出 GM 起始地址 `tileRow * 128 * 8192 + tileCol * 128`）：

**关键参数**：

| 参数 | 值 | 含义 |
|:---:|:---:|:---|
| `tileH` | 128 | 单次搬运 tile  N 方向行数 |
| `tileW` | 128 | 单次搬运 tile  D 方向 half 数 |
| `C0_ELEMS` | 16 | 固定布局参数：一个 C0 块包含的 half 元素数，也是一个 DataBlock 的 half 元素数，不是可调 tiling 参数 |
| `vecLenHalf` | 128 | tiling 参数：单条向量指令一次处理的 half 元素数 |
| `tileC0Cols` | 8 | 派生参数：单次搬运 tile  D 方向 C0 列数，等于 `tileW / C0_ELEMS` |
| `dstNzNStride` | 1 | 单次搬运 UB 内 N 方向步长，以 DataBlock 为单位 |
| `dstNzC0Stride` | 128 (S1) / 129 (S2) | 单次搬运 UB 内相邻 C0 列起始位置的间距，以 DataBlock 为单位。对应 `Copy()` 接口 `CopyRepeatParams` 中的 `dstStride` —— 同一迭代内 datablock 的地址步长。本样例通过切换 128 / 129 来对比 bank 冲突场景。 |
| `vecLenDbs` | 8 | 派生参数：单条向量指令一次处理的 DataBlock 数，等于 `vecLenHalf / C0_ELEMS` |



> **术语说明 · DataBlock**
>
> DataBlock 是**一条 NPU 矢量计算指令处理的数据单元**，大小为 **32 字节**（half 类型下即 16 个 half）；一条矢量指令一次 Repeat 可同时处理多个 DataBlock。后文所有 stride 类参数（`dstNzNStride` / `dstNzC0Stride`）取值都以 DataBlock 为单位；行文中的"32B 块"指的就是 DataBlock。

**ND → NZ 数据排布**：

ND和NZ的数据排布如图所示

<img src="figures/nd2nz.png" width="50%">

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

---

### Atlas A2 系列产品（dav-2201）

> 编译时通过 `-DCMAKE_ASC_ARCHITECTURES=dav-2201` 选择 Atlas A2 。

#### UB Bank 结构与冲突类型

Atlas A2 系列产品上，Unified Buffer 总大小 192KB，划分为 48 个 bank（每个 bank 128 行 × 32B = 4KB）；这 48 个 bank 进一步组织为 **16 个 bank group**，每个 bank group 包含 3 个 bank。Vector 计算单元每拍（一个指令周期）能从每个 bank group 中读取或写入一行数据。当一拍内的多个读 / 写操作落到同一个 bank 或 bank group 时，硬件无法在一个周期内全部处理，需要排队等待，即 bank 冲突。

**图：Atlas A2 UB Bank 结构示意图**（箭头方向表示内存排布顺序）

<img src="figures/ubBankStruct2201.png" width="80%">

bank 冲突主要分为三类：

- **读写冲突**：读操作和写操作同时尝试访问同一个 bank；
- **写写冲突**：多个写操作同时尝试访问同一个 bank group；
- **读读冲突**：多个读操作同时尝试访问同一个 bank group。

> 详细规格、地址级示例可参考 [`避免bank冲突（NPU架构版本2201）.md`](../../../../../../docs/guide/算子实践参考/SIMD算子性能优化/内存访问/避免UB的bank冲突/避免bank冲突（NPU架构版本2201）.md)。

本样例的 ND→NZ 转换主要触发**写写冲突**：向量写 NZ 时一拍要落 8 个 DataBlock，落点的 bank group 由 `dstNzC0Stride` 决定。下面对比两种 case。

#### Case 1: 紧凑排放

**实现方式**：编译时 `-DSCENARIO_NUM=1`。

代码中每次取 128×128 的 `Tile` 进行 ND→NZ 重排，转置后的 8 个 C0 列在 UB 里**依次紧密排开、一列接一列首尾相连**，这是 NZ 布局的"自然写法"。

**关键代码**：

**ND2NZ 实现**：

vcopy 的 `Copy()` 自带 repeat，单条 Copy 覆盖 tileH=128 行 × 8 个 C0 列。一行 8 个 C0 列要 1 次 Copy：

```cpp
AscendC::CopyRepeatParams copyParams;
copyParams.dstStride     = dstNzC0Stride;    // = 128，向量写列间距
copyParams.srcStride     = 1;
copyParams.dstRepeatSize = dstNzNStride;
copyParams.srcRepeatSize = tileW / C0_ELEMS;

for (uint32_t k = 0; k < tileW / vecLenHalf; ++k) {
    AscendC::Copy(nzBuf[k * vecLenDbs * dstNzC0Stride * C0_ELEMS],
                  ndBuf[k * vecLenHalf],
                  static_cast<uint64_t>(vecLenHalf),
                  static_cast<uint8_t>(tileH),
                  copyParams);
}
```

一拍取的 8 个 datablock 数据在 NZ 矩阵中的排布如下图所示，原本相邻的两个 datablock 在 NZ 排布中起始地址需间隔 `128 * 32B`：

<img src="figures/datand2nzS1.png" width="80%">

**Ping-Pong 流水**：

样例在 `Process` 中开启双缓冲机制，UB 上同时分配 `ndPing/ndPong` 和 `nzPing/nzPong` 两套 buffer，相邻 tile 交替使用两组 buffer。每组 buffer 使用独立 event：

- `V_MTE2`：表示当前 `ndBuf` 已被向量写消费完，MTE2 可以复用该输入 buffer；
- `MTE3_V`：表示当前 `nzBuf` 已完成 UB→GM 写回，Vec 可以复用该输出 buffer；
- `MTE2_V` 和 `V_MTE3`：连接当前 tile 的 GM→UB、向量写、UB→GM 三段流水。

当前实现参考 `gelu_high_performance` 样例采用标准 double buffer 流水：每轮使用当前 ping/pong buffer 执行 `CopyIn(tile i) -> Compute(tile i) -> CopyOut(tile i)`。`eventID` 与当前 buffer 绑定，偶数 tile 使用 `EVENT_ID0`，奇数 tile 使用 `EVENT_ID1`。这样当前 tile 的 CopyIn 与 Compute 保持清晰的 producer-consumer 关系，上一 tile 的 MTE3 写回可以和下一 tile 的 MTE2/Vec 执行重叠。为了让 2201/3510、S1/S2 全部满足 UB 容量约束，当前 tile 宽度固定为 `128`。

下面的时间轴只表示 pipe 间相对重叠关系，宽度不代表精确 cycle：

```text
Time:       0        1        2        3        4        5
            |--------|--------|--------|--------|--------|-------->

MTE2:       |<- T0 ->|        |<- T1 ->|        |<- T2 ->|
Vec:                 |<- T0 ->|        |<- T1 ->|        |<- T2 ->|
MTE3:                         |<- T0 ->|        |<- T1 ->|

Buffer:     ping      ping     ping/pong  pong     ping/pong
            load T0   comp T0  out T0 /   comp T1  out T1 /
                                load T1             load T2
```

展开到循环就是：

```text
每轮主体:
  CopyIn(tile i):   Wait(V_MTE2) + MTE2(GM -> cur ndBuf) + SetFlag(MTE2_V)
  Compute(tile i):  Wait(MTE2_V) + Wait(MTE3_V) + Vec(cur ndBuf -> cur nzBuf)
                    SetFlag(V_MTE2), cur ndBuf 可复用
  CopyOut(tile i):  Wait(V_MTE3) + MTE3(cur nzBuf -> GM) + SetFlag(MTE3_V)
```

**UB 上的摆放顺序**：4 个 buf 按 `[ndPing | ndPong | nzPing | nzPong]` 顺序摆放（**而不是直觉上的 `[ndPing | nzPing | ndPong | nzPong]`**）。在 Atlas A2 上：

- ndPing、ndPong 累计 64 KB，落进 UB 第一组 bank（每个 bank group 的第 1 个子 bank，bank id 0–15）；
- nzPing、nzPong 累计约 64 KB，落进第二组 bank（每个 bank group 的第 2 个子 bank，bank id 16–31）。

这样 V scatter 一拍内对 `ndBuf` 的读和对 `nzBuf` 的写**落到不同的 bank**，避开 "读写冲突：读和写同时访问同一个 bank"。

> 下文 Case 2 / Ascend 950 各 case 的 Ping-Pong 流水实现完全一致，不再重复展开。

**冲突原理**：

UB 的 16 个 bank group 各自可以并行接收写入；地址按 32B 一块为单位顺序循环分配：第 1 个 32B 块属于 bank group 0、第 2 个属于 bank group 1、……、第 17 个又回到 bank group 0 重新一轮。**一拍内多个写如果都要去同一个 bank group，就只能排队**。

单条 vcopy 一拍要把 8 个 32B 块写到 UB，相邻两个落点之间隔 `dstNzC0Stride = 128` 个 32B 块。**128 刚好是 16 的整数倍**——每跳 128 步都会回到同一个 bank group。结果是 8 个写全部挤到 bank group 0，硬件只能依次处理，单条 vcopy 从 1 拍拉长为 **8 拍**，aiv_vec_time 显著膨胀。

**S1 落点示意图**：

```text
单条 copy() 的 8 个写在 UB 上的落点：
  写 #0 → 第   0 个 32B 块  → bank group 0
  写 #1 → 第 128 个 32B 块  → bank group 0   ◀ 同一 bank group
  写 #2 → 第 256 个 32B 块  → bank group 0   ◀ 同一 bank group
  写 #3 → 第 384 个 32B 块  → bank group 0   ◀ 同一 bank group
  写 #4 → 第 512 个 32B 块  → bank group 0   ◀ 同一 bank group
  写 #5 → 第 640 个 32B 块  → bank group 0   ◀ 同一 bank group
  写 #6 → 第 768 个 32B 块  → bank group 0   ◀ 同一 bank group
  写 #7 → 第 896 个 32B 块  → bank group 0   ◀ 同一 bank group
```

下图中蓝色部分表示一拍从 NDBUF 中读取数据，红色部分表示一拍写入 NZBUF 的数据，可以看到 8 个 datablock 全部落入同一个 bank group：

<img src="figures/conflictAtlas.png" width="80%">

```
  bank group:    0       1       2       ……         13     14      15
              ┌──────┬──────┬──────┬────────────┬──────┬──────┬──────┐
              │ ●●●●●│      │      │            │      │      │      │
              │ ●●●  │      │      │            │      │      │      │
              └──────┴──────┴──────┴────────────┴──────┴──────┴──────┘
                 ↑
                 8 个写全堆在 bank group 0 → 写写冲突
```

**性能数据**（Atlas A2 系列产品，48 核）：

| Task Duration(μs) | aiv_time(μs) | aiv_vec_time(μs) | aiv_vec_ratio | aiv_scalar_time(μs) | aiv_scalar_ratio | aiv_mte2_time(μs) | aiv_mte2_ratio | aiv_mte3_time(μs) | aiv_mte3_ratio |
|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|
| 174.32 | 155.51 | **47.856** | 0.273 | 4.716 | 0.027 | 61.046 | 0.348 | 57.284 | 0.327 |

**优化效果分析**：
- `aiv_vec_time = 47.856 μs`，是 Case 2 的 ~7.3 倍。
- 冲突点在 Vec 写阶段：vcopy 的 8 个 32B 块全打 bank group 0。
- 读侧（vcopy 的源）紧凑无冲突，不是瓶颈。

#### Case 2: 每个 C0 列偏移 1 个 DataBlock

**实现方式**：编译时 `-DSCENARIO_NUM=2`。

在 Case 1 紧凑排放的基础上，**让每个 C0 列的起始位置比前一列多偏移 1 个 DataBlock**，向量写落点便能在不同 bank group 间轮转。

**关键代码**：
```cpp
constexpr uint32_t dstNzC0Stride = (SCENARIO == 1) ? tileH : (tileH + 1);
```

Ping-Pong 流水与 Case 1 完全一致，参见上文。

**规避原理**：

把 `dstNzC0Stride` 从 128 改成 129，相邻两个落点之间隔 129 个 32B 块。每跳 129 步，相当于在 16 个 bank group 里走完一整轮（16 步）后又额外多迈 1 步，落点依次进入 bank group 0、1、2、……、7，**8 个写分散到 8 个不同 bank group**，硬件可以并发处理，单条 vcopy **1 拍**完成。

每个 C0 列尾部多 1 行（1 个 32B 块），原本相邻的两个 32B 块在 NZ 排布中起始地址需间隔 `129 * 32B`，在 UB 中正好错开一个 bank。

<img src="figures/datand2nzS2.png" width="100%">

**S2 落点示意图**：

```text
单条 copy() 的 8 个写在 UB 上的落点：
  写 #0 → 第   0 个 32B 块  → bank group 0
  写 #1 → 第 129 个 32B 块  → bank group 1
  写 #2 → 第 258 个 32B 块  → bank group 2
  写 #3 → 第 387 个 32B 块  → bank group 3
  写 #4 → 第 516 个 32B 块  → bank group 4
  写 #5 → 第 645 个 32B 块  → bank group 5
  写 #6 → 第 774 个 32B 块  → bank group 6
  写 #7 → 第 903 个 32B 块  → bank group 7
```

<img src="figures/s2bank2201.png" width="100%">

```
  bank group:    0       1       2       3       4       5       6       7    …       15
              ┌──────┬──────┬──────┬──────┬──────┬──────┬──────┬──────┬───────────────┐
              │  ●   │  ●   │  ●   │  ●   │  ●   │  ●   │  ●   │  ●   │               │
              └──────┴──────┴──────┴──────┴──────┴──────┴──────┴──────┴───────────────┘
                 ↑      ↑      ↑      ↑      ↑      ↑      ↑      ↑
                 8 个写散到 8 个不同 bank groups → 无冲突 → 1 拍完成
```

**UB 占用变化**：

`nzBuf` 占用 DataBlock 数 = `(tileH - 1) * dstNzNStride + (tileC0Cols - 1) * dstNzC0Stride + 1`

| 场景 | DataBlock 数 | UB 占用 |
|:---:|:---:|:---:|
| S1 (stride=128) | 1024 | 32 KB（紧凑） |
| S2 (stride=129) | 1031 | ≈32.2 KB（含 7 段 32B 偏移位） |

这些偏移位在 UB→GM 的 MTE3 阶段由 `outParams.srcStride = dstNzC0Stride - tileH` 跳过，不会写回 GM。

双缓冲后，每套 buffer 包含一个 `ndBuf` 和一个 `nzBuf`。S1 总 UB 占用约为 `2 × (32 KB + 32 KB) = 128 KB`，S2 总 UB 占用约为 `2 × (32 KB + 32.2 KB) = 128.4 KB`，满足 Atlas A2 系列产品 192 KB UB 的约束。

**性能数据**（Atlas A2 系列产品，48 核）：

| Task Duration(μs) | aiv_time(μs) | aiv_vec_time(μs) | aiv_vec_ratio | aiv_scalar_time(μs) | aiv_scalar_ratio | aiv_mte2_time(μs) | aiv_mte2_ratio | aiv_mte3_time(μs) | aiv_mte3_ratio |
|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|
| 177.26 | 166.24 | **6.528** | 0.039 | 2.264 | 0.014 | 70.39 | 0.423 | 82.344 | 0.495 |

**优化效果分析**：
- `aiv_vec_time` 从 47.856 → 6.528 μs（提升 **7.33×**），与"延迟从 8 拍降到 1 拍"的理论几乎吻合。
- 代价是每 tile 多占用 7 × 32B = 224B 的 UB 空间（约 0.7% 浪费），完全可接受。

#### 性能对比与理论分析（Atlas A2 系列产品）

| Case | dstNzC0Stride | 向量写落点模式 | Task Duration(μs) | aiv_vec_time(μs) | 理论vector耗时(μs) | 读带宽(TB/S) | 写带宽(TB/S) |
|:---:|:---:|:---|:---:|:---:|:---:|:---:|:---:|
| 1 | 128 | 8 个 DataBlock 全打 bank group 0 | 182.16 | **47.856** | 47.233 | 2.199 | 2.343 |
| 2 | 129 | 8 个 DataBlock 散到 bank groups 0~7 | 177.26 | **6.528** | 5.904 | 1.907 | 1.630 |

表中的"理论vector耗时"只统计 UB 上向量写阶段，不包含 GM→UB、UB→GM 搬运、事件同步和 scalar 调度开销。Atlas A2 系列产品上，单条 vcopy 每拍处理 `128` 个 half，主频按 `1.85GHz`、核数按 `48` 计算。无冲突基础耗时为：

$$
T_{\text{base}} = \frac{M \times N}{128 \times 1.85 \times 10^9 \times 48}
                = \frac{8192 \times 8192}{1.13664 \times 10^{13}}
                \approx 5.904\mu s
$$

Case 1 中 `dstNzC0Stride = 128`，8 个写落到同一个 bank group，单条向量写从 1 拍拉长到 8 拍，因此：

$$
T_{\text{S1}} = 8 \times T_{\text{base}} \approx 47.233\mu s
$$

Case 2 中 `dstNzC0Stride = 129`，8 个写均匀分散到不同 bank group，因此：

$$
T_{\text{S2}} = 1 \times T_{\text{base}} \approx 5.904\mu s
$$

可以看到实测 `aiv_vec_time` 分别为 `47.856μs` 和 `6.528μs`，与理论值非常接近。端到端 Task Duration 提升较小（仅 2.7%），是因为整体仍包含 MTE2/MTE3 搬运和同步开销，bank 冲突优化主要体现在 Vec 子流水。

---

### Ascend 950PR/Ascend 950DT（dav-3510）

> 编译时通过 `-DCMAKE_ASC_ARCHITECTURES=dav-3510` 选择 Ascend 950PR/DT 后端。

#### UB Bank 结构与冲突类型

Ascend 950PR/Ascend 950DT 上，Unified Buffer 总大小 256KB，划分为 16 个 bank（每个 bank 512 行 × 32B = 16KB）；这 16 个 bank 进一步组织为 **8 个 bank group**，每个 bank group 包含 2 个 bank。Vector 计算单元每拍能从每个 bank group 中读取或写入一行数据。

**图：Ascend 950 UB Bank 结构示意图**（箭头方向表示内存排布顺序）

<img src="figures/ubBankStruct3510.png" width="80%">

bank 冲突主要分为三类：

- **读写冲突**：读操作和写操作同时尝试访问同一个 bank；
- **写写冲突**：多个写操作同时尝试访问同一个 bank group；
- **读读冲突**：两个读操作同时尝试访问同一个 bank，或两个以上读操作同时尝试访问同一个 bank group。

> 950 的 bank group 上有两组读口和写口，因此两次读操作访问同一个 bank group 的**不同** bank 时不会冲突，这是与 2201 的主要差异。详细规格、地址级示例可参考 [`避免bank冲突（NPU架构版本3510）.md`](../../../../../../docs/guide/算子实践参考/SIMD算子性能优化/内存访问/避免UB的bank冲突/避免bank冲突（NPU架构版本3510）.md)。

本样例的 ND→NZ 转换主要触发**写写冲突**：向量写 NZ 时一拍要落 8 个 DataBlock，落点的 bank group 由 `dstNzC0Stride` 决定。下面对比两种 case。

#### Case 1: 紧凑排放

**实现方式**：编译时 `-DSCENARIO_NUM=1`。

代码中每次取 128×128 的 `Tile` 进行 ND→NZ 重排，转置后的 8 个 C0 列在 UB 里**依次紧密排开、一列接一列首尾相连**，这是 NZ 布局的"自然写法"。

**关键代码**：
```cpp
constexpr uint32_t dstNzC0Stride = (SCENARIO == 1) ? tileH : (tileH + 1);
```

Ping-Pong 流水的实现与 Atlas A2 Case 1 完全相同（参见上文 [Atlas A2 → Case 1 → Ping-Pong 流水](#case-1-紧凑排放)）。

**ND2NZ 实现**：

950 没有 vcopy/Copy 的 repeat 机制，手写 `__simd_vf__`，外层向量列组、内层行循环：

```cpp
for (uint16_t k = 0; k < vecsPerRow; ++k) {
    const uint32_t dstColGroupOffset = k * vecLenDbs * dstNzC0Stride * C0_ELEMS;

    for (uint16_t r = 0; r < tileH; ++r) {
        AscendC::Reg::LoadAlign<half>(reg, srcAddr + r * tileW + k * vecLenHalf);

        __ubuf__ half* dstRowAddr = dstAddr + dstColGroupOffset + r * C0_ELEMS;
        AscendC::Reg::StoreAlign<half, AscendC::Reg::DataCopyMode::DATA_BLOCK_COPY>(
            dstRowAddr, reg, dstNzC0Stride, mask);
    }
}
```

一拍取的 8 个 datablock 数据在 NZ 矩阵中的排布如下图所示，原本相邻的两个 datablock 在 NZ 排布中起始地址需间隔 `128 * 32B`：

<img src="figures/datand2nz.png" width="80%">

**冲突原理**：

UB 的 8 个 bank group 各自可以并行接收写入；地址按 32B 一块为单位顺序循环分配：第 1 个 32B 块属于 bank group 0、第 2 个属于 bank group 1、……、第 9 个又回到 bank group 0 重新一轮。**一拍内多个写如果都要去同一个 bank group，就只能排队**。

单条 `Reg::StoreAlign<DATA_BLOCK_COPY>` 一拍要把 8 个 32B 块写到 UB，相邻两个落点之间隔 `dstNzC0Stride = 128` 个 32B 块。**128 刚好是 8 的整数倍**——每跳 128 步都会回到同一个 bank group。结果是 8 个写全部挤到 bank group 0，硬件只能依次处理，单条 StoreAlign 从 1 拍拉长为 **8 拍**，aiv_vec_time 显著膨胀。

**S1 落点示意图**：

```text
单条 StoreAlign 的 8 个写在 UB 上的落点：
  写 #0 → 第   0 个 32B 块  → bank group 0
  写 #1 → 第 128 个 32B 块  → bank group 0   ◀ 同一 bank group
  写 #2 → 第 256 个 32B 块  → bank group 0   ◀ 同一 bank group
  写 #3 → 第 384 个 32B 块  → bank group 0   ◀ 同一 bank group
  写 #4 → 第 512 个 32B 块  → bank group 0   ◀ 同一 bank group
  写 #5 → 第 640 个 32B 块  → bank group 0   ◀ 同一 bank group
  写 #6 → 第 768 个 32B 块  → bank group 0   ◀ 同一 bank group
  写 #7 → 第 896 个 32B 块  → bank group 0   ◀ 同一 bank group
```

```
  bank group:    0       1       2       3       4       5       6       7
              ┌──────┬──────┬──────┬──────┬──────┬──────┬──────┬──────┐
              │ ●●●●●│      │      │      │      │      │      │      │
              │ ●●●  │      │      │      │      │      │      │      │
              └──────┴──────┴──────┴──────┴──────┴──────┴──────┴──────┘
                 ↑
                 8 个写全堆在 bank group 0 → 写写冲突
```

**性能数据**（Ascend 950PR/DT，64 核）：

| Task Duration(μs) | aiv_time(μs) | aiv_vec_time(μs) | aiv_vec_ratio | aiv_scalar_time(μs) | aiv_scalar_ratio | aiv_mte2_time(μs) | aiv_mte2_ratio | aiv_mte3_time(μs) | aiv_mte3_ratio |
|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|
| 155.51 | 150.13 | **44.426** | 0.296 | 1.551 | 0.01 | 70.06 | 0.467 | 32.396 | 0.216 |

**优化效果分析**：
- `aiv_vec_time = 44.426 μs`，是 Case 2 的 ~4.7 倍。
- 冲突点在 Vec 写阶段：VEC_STORE 流水内每条 StoreAlign 都因 bank-group 冲突排队 8 拍。
- 读侧（VEC_LOAD 读 ndBuf）紧凑无冲突，不是瓶颈。

**性能瓶颈定位**：
> ⚠️ **bank 冲突在 Vec 子流水内部**
>
> 在 Regbase 架构下，`Reg::LoadAlign` 走 **VEC_LOAD** 子流水、`Reg::StoreAlign` 走 **VEC_STORE** 流水，不在 MTE 上。常规 MTE 类 bank-conflict 计数器看不到，需要看 vec 流水或单指令周期数。

#### Case 2: 每个 C0 列偏移 1 个 DataBlock

**实现方式**：编译时 `-DSCENARIO_NUM=2`。

在 Case 1 紧凑排放的基础上，**让每个 C0 列的起始位置比前一列多偏移 1 个 DataBlock**，向量写落点便能在不同 bank group 间轮转。

**关键代码**：
```cpp
constexpr uint32_t dstNzC0Stride = (SCENARIO == 1) ? tileH : (tileH + 1);
```

Ping-Pong 流水与 Atlas A2 Case 1 完全一致，参见上文。

**规避原理**：

把 `dstNzC0Stride` 从 128 改成 129，相邻两个落点之间隔 129 个 32B 块。每跳 129 步，相当于在 8 个 bank group 里走完一整轮（8 步）后又额外多迈 1 步，落点依次进入 bank group 0、1、2、……、7，**8 个写正好打满 8 个 bank group**，硬件可以并发处理，单条 StoreAlign **1 拍**完成。

每个 C0 列尾部多 1 行（1 个 32B 块），原本相邻的两个 32B 块在 NZ 排布中起始地址需间隔 `129 * 32B`，在 UB 中正好错开一个 bank。

<img src="figures/datand2nzS2.png" width="100%">

**S2 落点示意图**：

```text
单条 StoreAlign 的 8 个写在 UB 上的落点：
  写 #0 → 第   0 个 32B 块  → bank group 0
  写 #1 → 第 129 个 32B 块  → bank group 1
  写 #2 → 第 258 个 32B 块  → bank group 2
  写 #3 → 第 387 个 32B 块  → bank group 3
  写 #4 → 第 516 个 32B 块  → bank group 4
  写 #5 → 第 645 个 32B 块  → bank group 5
  写 #6 → 第 774 个 32B 块  → bank group 6
  写 #7 → 第 903 个 32B 块  → bank group 7
```

```
  bank group:    0       1       2       3       4       5       6       7
              ┌──────┬──────┬──────┬──────┬──────┬──────┬──────┬──────┐
              │  ●   │  ●   │  ●   │  ●   │  ●   │  ●   │  ●   │  ●   │
              └──────┴──────┴──────┴──────┴──────┴──────┴──────┴──────┘
                 ↑      ↑      ↑      ↑      ↑      ↑      ↑      ↑
                 8 个写均匀分散到 8 个 bank groups → 无冲突 → 1 拍完成
```

**UB 占用变化**：

`nzBuf` 占用 DataBlock 数 = `(tileH - 1) * dstNzNStride + (tileC0Cols - 1) * dstNzC0Stride + 1`

| 场景 | DataBlock 数 | UB 占用 |
|:---:|:---:|:---:|
| S1 (stride=128) | 1024 | 32 KB（紧凑） |
| S2 (stride=129) | 1031 | ≈32.2 KB（含 7 段 32B 偏移位） |

这些偏移位在 UB→GM 的 MTE3 阶段由 `outParams.srcStride = dstNzC0Stride - tileH` 跳过，不会写回 GM。

双缓冲后，每套 buffer 包含一个 `ndBuf` 和一个 `nzBuf`。S1 总 UB 占用约为 `2 × (32 KB + 32 KB) = 128 KB`，S2 总 UB 占用约为 `2 × (32 KB + 32.2 KB) = 128.4 KB`，满足 Ascend 950 系列产品 256 KB UB 的约束。

**性能数据**（Ascend 950PR/DT，64 核）：

| Task Duration(μs) | aiv_time(μs) | aiv_vec_time(μs) | aiv_vec_ratio | aiv_scalar_time(μs) | aiv_scalar_ratio | aiv_mte2_time(μs) | aiv_mte2_ratio | aiv_mte3_time(μs) | aiv_mte3_ratio |
|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|
| 142.151 | 141.27 | **9.406** | 0.064 | 1.507 | 0.011 | 86.995 | 0.616 | 41.492 | 0.294 |

**优化效果分析**：
- `aiv_vec_time` 从 44.426 → 9.406 μs（提升 **4.72×**）。Reg 路径每次向量写包含 1 拍 `LoadAlign` 和 1 拍 `StoreAlign`；冲突只放大 `StoreAlign`，因此理论加速比约为 `(1 + 8) / (1 + 1) = 4.5×`，与实测接近。
- 代价是每 tile 多占用 7 × 32B = 224B 的 UB 空间（约 0.7% 浪费），完全可接受。

#### 性能对比与理论分析（Ascend 950PR/Ascend 950DT）

| Case | dstNzC0Stride | 向量写落点模式 | Task Duration(μs) | aiv_vec_time(μs) | 理论vector耗时(μs) | 读带宽(TB/S) | 写带宽(TB/S) |
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

`LoadAlign` 从 `ndBuf` 连续读取，不触发冲突，按 1 拍计；`StoreAlign` 写 `nzBuf` 时受 `dstNzC0Stride` 影响。Case 1 的 Store 写冲突为 8 拍，Case 2 的 Store 写无冲突为 1 拍。

每次 `LoadAlign` 后立即用同一个 `reg` 执行 `StoreAlign`，Store 对 Load 存在数据依赖，下一轮循环也复用同一个 `reg`，因此本样例中按 `Load + Store` 的等效拍数计算。这里的 $1_{\text{Load}}$ 和 $1_{\text{Store}}$ 表示每组 `128` 个 half 数据对应的等效拍数，$T_{\text{base}}$ 已经表示全量数据按 1 拍/组处理一遍的耗时。因此：

$$
T_{\text{S1}} = (1_{\text{Load}} + 8_{\text{Store}}) \times T_{\text{base}} \approx 44.684\mu s
$$

$$
T_{\text{S2}} = (1_{\text{Load}} + 1_{\text{Store}}) \times T_{\text{base}} \approx 9.930\mu s
$$

实测 `aiv_vec_time` 分别为 `44.426μs` 和 `9.406μs`，同样与理论值接近。如果按 Load/Store 理想完全重叠估算，Case 2 会接近 $1 \times T_{\text{base}} = 4.965\mu s$，与实测差距较大；当前实测更符合 `Load + Store` 的等效关键路径模型。950 上整体提升不是 8×，是因为无冲突场景仍包含 `LoadAlign + StoreAlign` 两类 Vec 指令开销，理论加速比为 `9 / 2 = 4.5×`，实测为 `4.72×`。

---

### 带宽计算说明

本样例只有一个输入矩阵和一个输出矩阵，读写数据量均为：

$$
D = 8192 \times 8192 \times sizeof(half) = 134217728\text{ Byte}
$$

各表中的有效读/写带宽按 profiler 中对应流水耗时估算：

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

> 💡 **stride 错开 1 个 DataBlock 即可消除 bank 冲突**
>
> 1. 当向量写的 stride 是 bank-group 周期（2201 为 16、3510 为 8）的整数倍时触发 bank 冲突；
> 2. 把 stride 改成"周期 + 1"（或任意与周期互质的偏移）即可规避，代价只是每段尾部多占 1 个 DataBlock 的位置。

| 优化手段 | 核心原理 | 适用场景 |
|:---|:---:|:---|
| stride + 1 偏移 | 让向量写落点的 bank-group 索引轮转，避免 1 拍内多次访问同一 group | ND2NZ / NZ2ND / 转置 / 任何 stride 为 bank-group 周期整数倍的向量写 |

---

## 编译运行

- 切换 Case

  在 cmake 编译时通过 `-DSCENARIO_NUM=N` 指定要编译的 case：

  ```bash
  cmake -DSCENARIO_NUM=1 -DCMAKE_ASC_ARCHITECTURES=dav-2201 ..   # 编译 case 1（可替换为 1 或 2）
  ```

  各 case 说明：
  - `1`: `dstNzC0Stride = 128`，紧凑排放（触发 bank 冲突）
  - `2`: `dstNzC0Stride = 129`，每个 C0 列偏移 1 个 DataBlock（规避 bank 冲突）

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
  cmake -DSCENARIO_NUM=1 -DCMAKE_ASC_RUN_MODE=cpu -DCMAKE_ASC_ARCHITECTURES=dav-2201 ..;make -j;   # CPU 调试模式
  cmake -DSCENARIO_NUM=1 -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-2201 ..;make -j;  # NPU 仿真模式
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
