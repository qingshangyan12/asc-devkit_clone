# Tensor API 教程

本教程面向初次接触 Tensor API 的开发者，以通俗易懂的方式介绍 Tensor API 的核心概念、架构设计和使用方法。阅读本教程后，你将能够独立使用 Tensor API 完成张量描述、数据搬运和矩阵计算等操作。

关于每个接口的完整参数说明和约束条件，请参考对应的 [API 参考文档](./tensor_api.md)。

## 目录

- [1. 为什么需要 Tensor API](#1-为什么需要-tensor-api)
- [2. NPU 存储架构概览](#2-npu-存储架构概览)
- [3. 快速上手：四步走](#3-快速上手四步走)
- [4. Layout 详解](#4-layout-详解)
- [5. Tensor 操作](#5-tensor-操作)
- [6. 数据搬入（Cube 单元）](#6-数据搬入cube-单元)
- [7. 矩阵乘法 Mmad](#7-矩阵乘法-mmad)
- [8. 数据搬出（Cube 单元）](#8-数据搬出cube-单元)
- [9. Vector 单元操作](#9-vector-单元操作)
- [10. 三种调用风格对比](#10-三种调用风格对比)
- [11. 编译期与运行时](#11-编译期与运行时)
- [12. 常见注意事项](#12-常见注意事项)

---

## 1. 为什么需要 Tensor API

在昇腾 NPU 上编写算子时，开发者面临两大痛点：

1. **地址计算复杂**：NPU 使用分形布局（如 NZ、ZN）存储矩阵数据，手动计算每个元素的内存偏移既繁琐又容易出错。
2. **搬运指令繁多**：不同存储层级之间的数据搬运需要调用不同的底层指令（如 `asc_copy_gm2l1_align`、`asc_copy_l12l0a` 等），参数多且格式转换逻辑复杂。

**Tensor API 的核心思想**是：让开发者用"坐标"而非"地址"来访问数据，用"操作类型"而非"底层指令"来描述计算。

打个比方：传统方式就像你手动查地图算经纬度来导航，而 Tensor API 就像你告诉导航软件"我要从 A 到 B"，它自动帮你规划路线。

---

## 2. NPU 存储架构概览

### 2.1 存储层级

昇腾 NPU 的 AI Core 内部有多级存储，可以类比为一个工厂的物流体系：

| 存储位置 | 限定符 | Location 标签 | 类比 | 说明 |
|:---:|:---:|:---:|:---|:---|
| **GM** | `__gm__` | `Location::GM` | 外部仓库 | 全局内存（DRAM），容量大但访问慢 |
| **L1** | `__cbuf__` | `Location::L1` | 中转站 | L1 Buffer，片上共享缓存，矩阵搬运的中转站 |
| **L0A** | `__ca__` | `Location::L0A` | 车间 A 原料区 | Cube 单元 A 矩阵输入寄存器 |
| **L0B** | `__cb__` | `Location::L0B` | 车间 B 原料区 | Cube 单元 B 矩阵（Filter）输入寄存器 |
| **L0C** | `__cc__` | `Location::L0C` | 车间成品区 | Cube 单元累加器/输出寄存器 |
| **UB** | `__ubuf__` | `Location::UB` | 加工车间 | Vector 单元统一缓冲区，用于逐元素运算 |
| **BIAS** | `__biasbuf__` | `Location::BIAS` | 辅料区 | Bias 表，矩阵乘加运算中的偏置数据 |
| **FIXBUF** | `__fbuf__` | `Location::FIXBUF` | 后处理车间 | Fixpipe 缓冲区，用于量化/反量化等后处理 |

### 2.2 数据流全景

一次典型的矩阵乘法（MatMul）的数据流如下：

```
                    Cube 单元（矩阵计算通路）
                    ┌───────────────────────────────┐
                    │                               │
  GM ──→ L1 ──→ L0A ─┐                              │
  (外存)  (中转)  (A矩阵) ├──→ L0C ──→ GM / UB       │
         ├─→ L0B ─┘   (累加器)   (写回外存/后处理)   │
         │  (B矩阵)                                  │
         ├─→ BIAS (偏置)                             │
         └─→ FIXBUF (后处理)                         │
                    └───────────────────────────────┘

                    Vector 单元（逐元素计算通路）
                    ┌───────────────────────────────┐
  GM ←──→ UB ←──→ L1                                │
  (外存)  (计算)  (中转)                              │
                    └───────────────────────────────┘
```

### 2.3 Cube 核与 Vector 核

NPU 的 AI Core 分为两种核心类型：

- **Cube 核（AIC）**：负责矩阵乘法（Mmad）和矩阵数据搬运，使用 L1/L0A/L0B/L0C 存储。
- **Vector 核（AIV）**：负责逐元素运算（加减乘除、类型转换等），使用 UB 存储。

在代码中通过 `AscendC::SetGCoreType(1)` 切换到 Cube 核，`AscendC::SetGCoreType(2)` 切换到 Vector 核。

---

## 3. 快速上手：四步走

Tensor API 的使用遵循固定的四步模式。下面以"将 GM 中的矩阵搬入 L1 并转为 NZ 格式"为例：

### 第一步：构造内存指针

告诉 Tensor API "数据在哪里"。`MakeMemPtr` 通过模板参数标记存储位置：

```cpp
#include "tensor_api/tensor.h"
using namespace AscendC::Te;

// gmAddr 是 __gm__ half* 类型，MakeMemPtr 自动识别 GM 位置
auto gmPtr = MakeMemPtr(gmAddr);

// 显式指定存储位置为 L1
auto l1Ptr = MakeMemPtr<Location::L1>(l1Buf);
```

### 第二步：构造数据布局

告诉 Tensor API "数据长什么样"。`MakeFrameLayout` 根据布局格式和维度自动构造 Layout：

```cpp
// GM 侧：M×K 的 ND 格式（行优先）
auto gmLayout = MakeFrameLayout<NDExtLayoutPtn>(M, K);

// L1 侧：M×K 的 NZ 格式（分形布局），数据类型为 half
auto l1Layout = MakeFrameLayout<NZLayoutPtn, half>(M, K);
```

### 第三步：构造张量

将指针和布局绑定成一个完整的张量：

```cpp
auto gmTensor = MakeTensor(gmPtr, gmLayout);   // GlobalTensor
auto l1Tensor = MakeTensor(l1Ptr, l1Layout);   // LocalTensor
```

### 第四步：执行操作

使用 `Copy` 或 `Mmad` 执行硬件操作：

```cpp
auto copyAtom = MakeCopy(CopyGM2L1{}, CopyGM2L1TraitDefault{});
Copy(copyAtom, l1Tensor, gmTensor);
```

### 完整示例

```cpp
#include "tensor_api/tensor.h"
using namespace AscendC::Te;

constexpr uint32_t M = 32;
constexpr uint32_t K = 64;

__aicore__ inline void CopyGmToL1(__gm__ half* gmAddr)
{
    __cbuf__ half l1Buf[M * K];

    auto gmA = MakeTensor(MakeMemPtr(gmAddr), MakeFrameLayout<NDExtLayoutPtn>(M, K));
    auto l1A = MakeTensor(MakeMemPtr(l1Buf), MakeFrameLayout<NZLayoutPtn, half>(M, K));

    auto copyGm2L1 = MakeCopy(CopyGM2L1{}, CopyGM2L1TraitDefault{});
    Copy(copyGm2L1, l1A, gmA);
}
```

就这么简单！四步完成了从 GM 到 L1 的数据搬运，并且自动完成了 ND→NZ 的格式转换。

---

## 4. Layout 详解

### 4.1 什么是 Layout

Layout 是 Tensor API 中最核心的概念，它描述了"多维数据在内存中如何排列"。

可以把 Layout 想象成一张**地图**：
- **Shape**（形状）：告诉你矩阵有多大（几行几列）
- **Stride**（步长）：告诉你沿着每个方向走一步，内存地址要跳多远

给定一个坐标 `(row, col)`，Layout 能自动算出它在内存中的偏移量：`offset = row * stride_row + col * stride_col`。

### 4.2 基本 Layout

最简单的二维 Layout 有两种：

- **ND（行优先）**：`Shape(row, col)`，`Stride(col, 1)` — 同一行的元素在内存中连续存放
- **DN（列优先）**：`Shape(row, col)`，`Stride(1, row)` — 同一列的元素在内存中连续存放

```cpp
// 行优先：4行8列
auto ndLayout = MakeLayout(MakeShape(4, 8), MakeStride(8, _1{}));

// 也可以只给 Shape，Stride 自动推导为行优先
auto autoLayout = MakeLayout(MakeShape(4, 8));
// 等价于 Stride(8, 1)
```

### 4.3 层次化表述法

当遇到分形布局（如 NZ、ZN）时，简单的二维 Shape/Stride 不够用了。Tensor API 采用**层次化表述法**——Shape 和 Stride 中的每个元素本身也可以是一个元组，用来描述"内层小矩阵"和"外层大矩阵"两层结构。

```
Shape  = ((内层行数, 外层行数), (内层列数, 外层列数))
Stride = ((内层行步长, 外层行步长), (内层列步长, 外层列步长))
```

形象地说，分形布局就像把一个大矩阵切成一个个固定大小的小方块（内层），这些小方块再按 Z 字形或 N 字形排列（外层）。

### 4.4 十二种 Layout Pattern

Tensor API 支持 12 种 Layout Pattern，可以分为四组：

#### 平面布局（无分形）

| Pattern | 说明 | 典型用途 |
|:---:|:---|:---|
| `NDLayoutPtn` | 行优先，Shape(row, col)，Stride(col, 1) | GM 侧普通矩阵 |
| `DNLayoutPtn` | 列优先，Shape(row, col)，Stride(1, row) | GM 侧转置矩阵 |

#### 扩展布局（带外层分组）

| Pattern | 说明 | 典型用途 |
|:---:|:---|:---|
| `NDExtLayoutPtn` | 行优先扩展，内层 1×1，外层行优先 | GM 侧矩阵（推荐） |
| `DNExtLayoutPtn` | 列优先扩展，内层 1×1，外层列优先 | GM 侧转置矩阵 |

> **NDExt vs ND**：NDExt 在层次化表述法中多了一层 1×1 的内层，使 Layout 结构统一为两层，便于后续格式转换时保持一致性。大多数场景推荐使用 NDExt/DNExt。

#### 分形布局（Cube 单元专用）

| Pattern | 内层形状 | 外层组织 | 典型用途 |
|:---:|:---|:---|:---|
| `NZLayoutPtn` | 16 × C0 | Z 字形 | L0A（A 矩阵）、L0C（输出） |
| `ZNLayoutPtn` | C0 × 16 | N 字形 | L0B（B 矩阵） |
| `ZZLayoutPtn` | 16 × C0 | Z 字形（列连续） | ScaleA 数据 |
| `NNLayoutPtn` | 2 × 16 | N 字形 | ScaleB 数据（仅 fp8_e8m0_t） |

> 其中 `C0 = 32Byte / sizeof(T)`，例如 half 类型时 C0=16，float 类型时 C0=8。

#### Scale 布局（MX 量化专用）

| Pattern | 说明 | 典型用途 |
|:---:|:---|:---|
| `ScaleANDLayoutPtn` | ScaleA 的 ND 形式 | MX 矩阵中 A 的 Scale 数据 |
| `ScaleADNLayoutPtn` | ScaleA 的 DN 形式 | MX 矩阵中 A 的 Scale 数据 |
| `ScaleBNDLayoutPtn` | ScaleB 的 ND 形式 | MX 矩阵中 B 的 Scale 数据 |
| `ScaleBDNLayoutPtn` | ScaleB 的 DN 形式 | MX 矩阵中 B 的 Scale 数据 |

> Scale 布局仅支持 `fp8_e8m0_t` 数据类型。

### 4.5 使用 MakeFrameLayout

`MakeFrameLayout` 是构造 Layout 最常用的方式，它根据 Pattern 和维度自动计算正确的 Shape 和 Stride：

```cpp
// ND 格式，8行16列
auto ndLayout = MakeFrameLayout<NDLayoutPtn>(8, 16);

// NZ 格式，M行K列，数据类型 half（自动推导 C0=16）
auto nzLayout = MakeFrameLayout<NZLayoutPtn, half>(M, K);

// ZN 格式，K行N列，数据类型 float（自动推导 C0=8）
auto znLayout = MakeFrameLayout<ZNLayoutPtn, float>(K, N);

// 使用 LayoutTrait 指定数据类型
auto nzLayout2 = MakeFrameLayout<NZLayoutPtn, LayoutTraitDefault<bfloat16_t>>(M, K);

// FP4 类型使用 LayoutTraitFP4
auto nzLayout3 = MakeFrameLayout<NZLayoutPtn, LayoutTraitFP4>(M, K);
```

### 4.6 Layout 查询

构造好 Layout 后，可以查询其各种属性：

```cpp
auto layout = MakeFrameLayout<NZLayoutPtn, half>(64, 128);

auto s = layout.Size();       // 元素总数
auto c = layout.Capacity();   // 内存容量（含对齐填充）
auto r = layout.Rank();       // 维度数
auto shape = layout.Shape();  // Shape 元组
auto stride = layout.Stride();// Stride 元组

// 获取子维度
auto rowShape = GetShape<0>(layout);   // 行方向的 Shape
auto colStride = GetStride<1>(layout); // 列方向的 Stride
```

---

## 5. Tensor 操作

### 5.1 创建张量

张量（Tensor）= 内存指针（Engine）+ 数据布局（Layout）。

根据存储位置不同，`MakeTensor` 返回两种类型：
- **GlobalTensor**：数据在 GM（外存），支持 L2 Cache 提示
- **LocalTensor**：数据在片上存储（L1/L0A/L0B/L0C/UB/BIAS/FIXBUF）

```cpp
// GM 张量（GlobalTensor）
auto gmTensor = MakeTensor(MakeMemPtr(gmAddr), MakeFrameLayout<NDExtLayoutPtn>(M, K));

// L1 张量（LocalTensor）
auto l1Tensor = MakeTensor(MakeMemPtr<Location::L1>(l1Buf), MakeFrameLayout<NZLayoutPtn, half>(M, K));

// L0A 张量
auto l0aTensor = MakeTensor(MakeMemPtr<Location::L0A>(l0aBuf), MakeFrameLayout<NZLayoutPtn, half>(M, K));

// UB 张量（自动识别 __ubuf__ 限定符）
auto ubTensor = MakeTensor(MakeMemPtr(ubBuf), MakeFrameLayout<NDLayoutPtn>(N));
```

> **便捷写法**：当指针已经带有存储限定符（如 `__gm__`、`__cbuf__`）时，`MakeMemPtr` 可以自动识别位置，不需要显式指定 `Location::X`。

### 5.2 坐标索引

使用 `operator[]` 通过坐标访问单个元素：

```cpp
auto tensor = MakeTensor(MakeMemPtr(gmAddr), MakeFrameLayout<NDExtLayoutPtn>(M, K));

// 访问第 2 行第 3 列的元素
auto val = tensor[MakeCoord(2, 3)];
```

### 5.3 子张量

使用 `operator()` 获取从某个坐标开始的子张量视图（不拷贝数据，只移动指针）：

```cpp
// 从 (1, 1) 开始的子张量
auto subTensor = tensor(MakeCoord(1, 1));
```

### 5.4 切片

`Slice` 获取指定坐标起点和大小的子张量：

```cpp
// 从 (1, 2) 开始，取 3 行 4 列的子张量
auto sliced = Slice(tensor, MakeCoord(1, 2), MakeShape(3, 4));

// 也可以用成员函数形式
auto sliced2 = tensor.Slice(MakeCoord(1, 2), MakeShape(3, 4));
```

### 5.5 L2 Cache 提示

仅 GlobalTensor 支持设置 L2 Cache 策略：

```cpp
gmTensor.SetL2CacheHint(CacheMode::CACHE_MODE_NORMAL);    // 正常缓存
gmTensor.SetL2CacheHint(CacheMode::CACHE_MODE_DISABLE);   // 禁用缓存
gmTensor.SetL2CacheHint(CacheMode::CACHE_MODE_LAST);      // 最后使用
gmTensor.SetL2CacheHint(CacheMode::CACHE_MODE_PERSISTENT);// 持久缓存
```

### 5.6 张量属性查询

```cpp
auto shape = tensor.Shape();     // Shape
auto stride = tensor.Stride();   // Stride
auto size = tensor.Size();       // 元素总数
auto cap = tensor.Capacity();    // 内存容量
auto data = tensor.Data();       // 底层数据指针
auto engine = tensor.Engine();   // Engine 对象
auto layout = tensor.Layout();   // Layout 对象
```

---

## 6. 数据搬入（Cube 单元）

数据搬入是将矩阵数据从外部存储（GM）经 L1 中转到 Cube 寄存器（L0A/L0B）的过程。Tensor API 的 `Copy` 接口会根据源和目的张量的存储位置和 Layout 自动选择正确的硬件指令。

### 6.1 GM → L1（CopyGM2L1）

将数据从 GM 搬入 L1 Buffer，支持随路格式转换（如 ND→NZ）。

```cpp
__gm__ half* gmAddr = ...;
__cbuf__ half l1Buf[M * K];

auto gmA = MakeTensor(MakeMemPtr(gmAddr), MakeFrameLayout<NDExtLayoutPtn>(M, K));
auto l1A = MakeTensor(MakeMemPtr(l1Buf), MakeFrameLayout<NZLayoutPtn, half>(M, K));

auto copyGm2L1 = MakeCopy(CopyGM2L1{}, CopyGM2L1TraitDefault{});
Copy(copyGm2L1, l1A, gmA);  // 自动完成 ND→NZ 格式转换
```

**支持的格式转换**：ND→ND、ND→NZ、ND→ZN、DN→NZ、DN→ZN、NZ→NZ、ZN→ZN，以及 Scale 数据搬入（ScaleAND→ZZ、ScaleADN→ZZ、ZZ→ZZ、ScaleBND→NN、ScaleBDN→NN、NN→NN）。

**支持的数据类型**：fp4x2、int8/uint8、hifloat8、fp8_e5m2/fp8_e4m3fn、int16/uint16、half、bfloat16_t、int32/uint32、float、int64/uint64（具体组合参见 [gm_to_l1 参考文档](./data_movement/gm_to_l1.md)）。

### 6.2 L1 → L0A（CopyL12L0A）

将 A 矩阵从 L1 搬入 L0A 寄存器。

```cpp
__cbuf__ half l1Buf[...] = ...;
__ca__ half l0aBuf[M * K];

auto l1A = MakeTensor(MakeMemPtr(l1Buf), MakeFrameLayout<NZLayoutPtn, half>(M, K));
auto l0aA = MakeTensor(MakeMemPtr(l0aBuf), MakeFrameLayout<NZLayoutPtn, half>(M, K));

auto copyL12L0A = MakeCopy(CopyL12L0A{}, CopyL12L0ATraitDefault{});
Copy(copyL12L0A, l0aA, l1A);
```

- **非转置**：L1 为 NZ → L0A 为 NZ
- **转置**：L1 为 ZN → L0A 为 NZ

### 6.3 L1 → L0B（CopyL12L0B）

将 B 矩阵（Filter）从 L1 搬入 L0B 寄存器。

```cpp
__cbuf__ half l1Buf[...] = ...;
__cb__ half l0bBuf[K * N];

auto l1B = MakeTensor(MakeMemPtr(l1Buf), MakeFrameLayout<ZNLayoutPtn, half>(K, N));
auto l0bB = MakeTensor(MakeMemPtr(l0bBuf), MakeFrameLayout<ZNLayoutPtn, half>(K, N));

auto copyL12L0B = MakeCopy(CopyL12L0B{}, CopyL12L0BTraitDefault{});
Copy(copyL12L0B, l0bB, l1B);
```

- **非转置**：L1 为 ZN → L0B 为 ZN
- **转置**：L1 为 NZ → L0B 为 ZN

### 6.4 L1 → L0ScaleA（CopyL12L0ScaleA）

将 A 矩阵的 Scale 数据从 L1 搬入 L0ScaleA，用于 MX 量化矩阵计算。

```cpp
auto l1ScaleA = MakeTensor(
    MakeMemPtr<Location::L1>(l1ScaleBuf),
    MakeFrameLayout<ZZLayoutPtn, fp8_e8m0_t>(scaleRows, scaleCols));
auto l0ScaleA = MakeTensor(
    MakeMemPtr<Location::L0ScaleA, fp8_e8m0_t>(addr / 16),
    MakeFrameLayout<ZZLayoutPtn, fp8_e8m0_t>(scaleRows, scaleCols));

auto copyAtom = MakeCopy(CopyL12L0ScaleA{}, CopyL12L0ScaleATraitDefault{});
Copy(copyAtom, l0ScaleA, l1ScaleA);
```

### 6.5 L1 → L0ScaleB（CopyL12L0ScaleB）

将 B 矩阵的 Scale 数据从 L1 搬入 L0ScaleB。用法与 L0ScaleA 类似，Layout 使用 `NNLayoutPtn`。

### 6.6 L1 → BiasTable（CopyL12BT）

将偏置数据从 L1 搬入 BiasTable Buffer。

```cpp
auto l1Bias = MakeTensor(MakeMemPtr(l1BiasBuf), MakeFrameLayout<NDLayoutPtn>(1, N));
auto btBias = MakeTensor(MakeMemPtr<Location::BIAS>(biasBuf), MakeFrameLayout<NDLayoutPtn>(1, N));

auto copyAtom = MakeCopy(CopyL12BT{}, CopyL12BTTraitDefault{});
Copy(copyAtom, btBias, l1Bias);
```

### 6.7 L1 → Fixpipe（CopyL12FB）

将数据从 L1 搬入 Fixpipe Buffer，用于后处理（量化/反量化）。

```cpp
auto l1Data = MakeTensor(MakeMemPtr(l1Buf), MakeFrameLayout<NDExtLayoutPtn>(rows, cols));
auto fbData = MakeTensor(MakeMemPtr<Location::FIXBUF>(fbBuf), MakeFrameLayout<NDExtLayoutPtn>(rows, cols));

auto copyAtom = MakeCopy(CopyL12FB{}, CopyL12FBTraitDefault{});
Copy(copyAtom, fbData, l1Data);
```

---

## 7. 矩阵乘法 Mmad

### 7.1 基本概念

Mmad（Matrix Multiply-Add）是 Cube 单元的核心计算操作，执行 `C += A × B`（或 `C = A × B`，由初始化标志控制）。

- **A 矩阵（fm）**：位于 L0A，使用 NZ 布局
- **B 矩阵（filter）**：位于 L0B，使用 ZN 布局
- **C 矩阵（dst）**：位于 L0C，使用 NZ 布局

### 7.2 基本用法

```cpp
// 1. 准备 L0A、L0B、L0C 张量
auto l0aTensor = MakeTensor(MakeMemPtr<Location::L0A>(l0aBuf),
    MakeFrameLayout<NZLayoutPtn, half>(M, K));
auto l0bTensor = MakeTensor(MakeMemPtr<Location::L0B>(l0bBuf),
    MakeFrameLayout<ZNLayoutPtn, half>(K, N));
auto l0cTensor = MakeTensor(MakeMemPtr<Location::L0C>(l0cBuf),
    MakeFrameLayout<NZLayoutPtn, LayoutTraitDefault<>>(M, N));

// 2. 配置 MmadParams
MmadParams para;
para.m = M;
para.n = N;
para.k = K;
para.unitFlag = 0;
para.cmatrixInitVal = true;  // true: C = A×B; false: C += A×B

// 3. 构造 MmadAtom 并执行
auto mmadAtom = MakeMmad(MmadOperation{}, MmadTraitDefault{}).with(para);
Mmad(mmadAtom, l0cTensor, l0aTensor, l0bTensor);
```

### 7.3 MmadParams 参数说明

| 字段 | 类型 | 说明 |
|:---:|:---:|:---|
| `m` | uint16_t | A 矩阵行数 / C 矩阵行数 |
| `n` | uint16_t | B 矩阵列数 / C 矩阵列数 |
| `k` | uint16_t | A 矩阵列数 / B 矩阵行数 |
| `unitFlag` | uint8_t | 计算单元标志，通常为 0 |
| `cmatrixInitVal` | bool | `true` 表示初始化 C 为 0 后累加（C = A×B）；`false` 表示在 C 原有值上累加（C += A×B） |

### 7.4 带 Bias 的 Mmad

可以在矩阵乘法的同时加上偏置：

```cpp
// Bias 在 BIAS 存储区
auto biasTensor = MakeTensor(MakeMemPtr<Location::BIAS>(biasBuf),
    MakeFrameLayout<NDLayoutPtn>(1, N));

Mmad(mmadAtom, l0cTensor, l0aTensor, l0bTensor, biasTensor);
// 计算结果：C = A×B + Bias
```

### 7.5 多次累加

可以连续调用 Mmad 将多个分块的结果累加到同一个 L0C 张量：

```cpp
// 第一次：C = A0 × B0
para.cmatrixInitVal = true;
auto atom1 = MakeMmad(MmadOperation{}, MmadTraitDefault{}).with(para);
Mmad(atom1, l0cTensor, l0aTensor0, l0bTensor0);

// 第二次：C += A1 × B1
para.cmatrixInitVal = false;
auto atom2 = MakeMmad(MmadOperation{}, MmadTraitDefault{}).with(para);
Mmad(atom2, l0cTensor, l0aTensor1, l0bTensor1);
```

### 7.6 支持的数据类型

| A 矩阵（L0A）数据类型 | B 矩阵（L0B）数据类型 | C 矩阵（L0C）数据类型 |
|:---:|:---:|:---:|
| half | half | float |
| half | half | int32_t |
| bfloat16_t | bfloat16_t | float |
| float | float | float |
| int8_t | int8_t | int32_t |
| hifloat8_t | hifloat8_t | float |
| hifloat8_t | hifloat8_t | int32_t |
| fp8_e5m2_t | fp8_e5m2_t | float |
| fp8_e4m3fn_t | fp8_e4m3fn_t | float |

> 完整的数据类型矩阵（含 MX 模式）请参考 [mmad_computation 参考文档](./mmad_computation/mmad_computation.md)。

---

## 8. 数据搬出（Cube 单元）

矩阵计算完成后，需要将结果从 L0C 搬出到 GM 或 UB。

### 8.1 L0C → GM（CopyL0C2GM）

将计算结果从 L0C 写回 GM，支持随路量化、Relu、Channel Split 等后处理。

```cpp
__cc__ float l0cBuf[M * N];
__gm__ half* gmAddr = ...;

auto l0cTensor = MakeTensor(MakeMemPtr(l0cBuf),
    MakeFrameLayout<NZLayoutPtn, LayoutTraitDefault<float, _16>>(M, N));
auto gmTensor = MakeTensor(MakeMemPtr(gmAddr),
    MakeFrameLayout<NDExtLayoutPtn>(M, N));

auto copyAtom = MakeCopy(CopyL0C2GM{}, CopyL0C2GMTraitDefault{});
Copy(copyAtom, gmTensor, l0cTensor);
```

#### 带 FixpipeParams 的参数化搬运

```cpp
FixpipeParams params;
params.unitFlag = 0;
params.subBlockId = false;

auto copyAtom = MakeCopy(CopyL0C2GM{}).with(params);
Copy(copyAtom, gmTensor, l0cTensor);
```

#### 带量化的搬运

```cpp
// 标量量化
uint64_t quantScale = ...;
Copy(copyAtom, gmTensor, l0cTensor, quantScale);

// 张量量化
auto quantTensor = MakeTensor(...);
Copy(copyAtom, gmTensor, l0cTensor, quantTensor);
```

#### CopyL0C2GMTrait 配置

```cpp
CopyL0C2GMTrait trait;
trait.roundMode = RoundMode::DEFAULT;     // 舍入模式
trait.enableRelu = false;                 // 是否启用 Relu
trait.enableChannelSplit = false;         // 是否启用 Channel Split
```

### 8.2 L0C → UB（CopyL0C2UB）

将计算结果从 L0C 搬入 UB，供 Vector 单元做后续逐元素运算。

```cpp
__cc__ float l0cBuf[M * N];
__ubuf__ half ubBuf[M * N];

auto l0cTensor = MakeTensor(MakeMemPtr(l0cBuf),
    MakeFrameLayout<NZLayoutPtn, LayoutTraitDefault<float, _16>>(M, N));
auto ubTensor = MakeTensor(MakeMemPtr(ubBuf),
    MakeFrameLayout<NDExtLayoutPtn>(M, N));

auto copyAtom = MakeCopy(CopyL0C2UB{}, CopyL0C2UBTraitDefault{});
Copy(copyAtom, ubTensor, l0cTensor);
```

#### DualDst 双目标模式

L0C→UB 支持双目标写入，可以在一次搬运中同时输出到两个 UB 区域：

```cpp
CopyL0C2UBTrait trait;
trait.dualDstCtl = DUAL_DST_SPLIT_M;  // 按 M 维度拆分
// 或 DUAL_DST_SPLIT_N                   // 按 N 维度拆分
```

---

## 9. Vector 单元操作

Vector 单元（AIV 核）负责逐元素运算，使用 UB 作为工作存储。

### 9.1 GM ↔ UB 搬运

```cpp
// GM → UB
auto gmTensor = MakeTensor(MakeMemPtr(gmAddr), MakeFrameLayout<NDExtLayoutPtn>(rows, cols));
auto ubTensor = MakeTensor(MakeMemPtr(ubBuf), MakeFrameLayout<NDExtLayoutPtn>(rows, cols));

auto copyGm2Ub = MakeCopy(CopyGM2UB{}, CopyGM2UBTraitDefault{});
Copy(copyGm2Ub, ubTensor, gmTensor);

// UB → GM
auto copyUb2Gm = MakeCopy(CopyUB2GM{}, CopyUB2GMTraitDefault{});
Copy(copyUb2Gm, gmTensor, ubTensor);
```

### 9.2 UB → L1 搬运

```cpp
auto copyUb2L1 = MakeCopy(CopyUB2L1{}, CopyUB2L1TraitDefault{});
Copy(copyUb2L1, l1Tensor, ubTensor);
```

### 9.3 L1 → UB 搬运

```cpp
auto copyL12Ub = MakeCopy(CopyL12UB{}, CopyL12UBTraitDefault{});
Copy(copyL12Ub, ubTensor, l1Tensor);
```

### 9.4 Transform 向量运算

`Transform` 是 Vector 单元的统一计算入口，根据传入的运算类型自动分发到二元运算或一元运算：

#### 二元运算（三个操作数：dst = src0 op src1）

```cpp
auto xUB = MakeTensor(MakeMemPtr(xBuf), MakeFrameLayout<NDLayoutPtn>(_1{}, Int<TILE_LENGTH>{}));
auto yUB = MakeTensor(MakeMemPtr(yBuf), MakeFrameLayout<NDLayoutPtn>(_1{}, Int<TILE_LENGTH>{}));
auto zUB = MakeTensor(MakeMemPtr(zBuf), MakeFrameLayout<NDLayoutPtn>(_1{}, Int<TILE_LENGTH>{}));

// 逐元素加法：z = x + y
Transform<Inst::Add>(zUB, xUB, yUB);

// 逐元素乘法：z = x * y
Transform<Inst::Mul>(zUB, xUB, yUB);
```

支持的二元运算：`Add`、`Sub`、`Mul`、`Div`、`Madd`、`Min`、`Max`、`And`、`Or`、`Xor`、`Select`、`AbsSub`、`ShiftLeft`、`ShiftRight`、`ExpSubEven`、`ExpSubOdd`、`Prelu`。

#### 一元运算（两个操作数：dst = op(src)）

```cpp
// 类型转换：uint8 → uint16
Transform<Inst::U82U16>(dstTensor, srcTensor);

// 向上取整
Transform<Inst::Ceil>(dstTensor, srcTensor);
```

---

## 10. 三种调用风格对比

Tensor API 提供了三种等价的方式来构造和执行操作，开发者可以根据场景选择：

### 风格一：MakeCopy 工厂函数（推荐）

最简洁，适合大多数场景：

```cpp
auto atom = MakeCopy(CopyGM2L1{}, CopyGM2L1TraitDefault{});
Copy(atom, dst, src);
```

### 风格二：CopyAtom 直接模板构造

更显式，适合需要精确控制模板参数的场景：

```cpp
CopyAtom<CopyTraits<CopyGM2L1, CopyGM2L1TraitDefault>>{}.Call(dst, src);
```

### 风格三：Copy 自由函数

一行完成构造和执行：

```cpp
Copy(CopyAtom<CopyTraits<CopyGM2L1, CopyGM2L1TraitDefault>>{}, dst, src);
```

### .with() 参数化

当需要传入运行时参数（如 `MmadParams`、`FixpipeParams`）时，使用 `.with()` 方法：

```cpp
// Mmad 参数化
auto atom = MakeMmad(MmadOperation{}, MmadTraitDefault{}).with(para);
Mmad(atom, dst, fm, filter);

// Copy 参数化
auto atom = MakeCopy(CopyL0C2GM{}).with(fixpipeParams);
Copy(atom, dst, src);
```

> Mmad 操作由于必须传入 `MmadParams`，通常总是使用 `.with()` 模式。

---

## 11. 编译期与运行时

Tensor API 大量使用 C++ 模板元编程，许多信息在编译期就已确定。理解编译期与运行时的边界对正确使用 API 很重要。

### 11.1 编译期常量

`Std::Int<N>{}` 是编译期整型常量，值保存在类型中。Tensor API 预定义了一系列快捷别名：

```cpp
using _0 = Std::Int<0>;
using _1 = Std::Int<1>;
using _16 = Std::Int<16>;
// ... 直到 _4096
```

使用编译期常量可以让编译器做更多优化：

```cpp
// 编译期 Shape：编译器知道确切大小
auto layout = MakeFrameLayout<NDLayoutPtn>(_1{}, Int<2048>{});

// 运行时 Shape：值在运行时确定
auto layout = MakeFrameLayout<NDLayoutPtn>(rows, cols);
```

### 11.2 Layout Pattern 和 Trait 必须在编译期确定

```cpp
// 正确：Pattern 和 Trait 是模板参数，编译期确定
auto layout = MakeFrameLayout<NZLayoutPtn, half>(M, K);

// 错误：不能在运行时选择 Pattern
// auto layout = MakeFrameLayout<runtimePattern, half>(M, K);  // 编译错误
```

### 11.3 维度可以是运行时值

Shape 的行数和列数可以是运行时变量，Tensor API 会自动处理：

```cpp
uint32_t M = getM();  // 运行时确定
uint32_t K = getK();
auto layout = MakeFrameLayout<NZLayoutPtn, half>(M, K);  // 正确
```

---

## 12. 常见注意事项

### 12.1 地址对齐要求

| 通路 | 源地址对齐 | 目的地址对齐 |
|:---:|:---:|:---:|
| GM → L1 | 1B | 32B |
| L1 → L0A/L0B | 32B | 32B |
| L0C → GM | 32B | 1B |
| GM ↔ UB | 1B | 32B |

### 12.2 数据类型限制

- **Scale 布局**（ScaleAND/ScaleADN/ScaleBND/ScaleBDN）仅支持 `fp8_e8m0_t`
- **NN 布局**仅支持 `fp8_e8m0_t`
- **FP4 类型**（fp4x2_e2m1_t/fp4x2_e1m2_t）需使用 `LayoutTraitFP4`
- 源和目的张量的数据类型通常需要匹配（部分通路支持类型转换，如 L1→BiasTable）

### 12.3 存储位置约束

- `CopyGM2L1` 要求目的张量在 `Location::L1`，源张量在 `Location::GM`
- `Mmad` 要求 A 在 `Location::L0A`，B 在 `Location::L0B`，C 在 `Location::L0C`
- 存储位置不匹配会导致编译期 `static_assert` 错误

### 12.4 FP4 特殊处理

FP4 类型（fp4x2_e2m1_t/fp4x2_e1m2_t）每个字节存储 2 个元素，指针索引时需要右移 1 位。Tensor API 内部自动处理了这个转换，但构造 Layout 时需注意 C0_ELEMENT 的值为 64（而非 32）。

### 12.5 L0ScaleA/L0ScaleB 指针构造

Scale 指针需要除以 16：

```cpp
auto ptr = MakeMemPtr<Location::L0ScaleA, fp8_e8m0_t>(
    reinterpret_cast<uint64_t>(rawPtr) / 16);
```

### 12.6 搬运字节数对齐

- GM→L1 连续搬运：搬运字节数需 32B 对齐，未对齐时向下取整
- 高维切分搬运：搬运长度、源步长和目的步长均按 32B 对齐

### 12.7 目的区域不可重叠

搬运到 L1 Buffer 的目的数据区域不能重叠，否则不保证写入顺序。

---

## 参考资源

- [Tensor API 概览](./README.md)
- [完整接口列表](./tensor_api_list.md)
- [Layout 和层次化表述法](./data_structure/layout_structure/Layout和层次化表述法.md)
- [测试用例](../../../../tests/api/tensor_api)
- [示例代码](../../../../examples/01_simd_cpp_api/02_features/05_tensor_api)
