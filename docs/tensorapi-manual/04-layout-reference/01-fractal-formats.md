# 4.1 分形数据布局格式全解

NPU 的数据在片内存储时，并非按行或列的简单顺序排列，而是采用**分形 (Fractal) 布局**——将大矩阵分割为若干小块，每块内部按特定规则排列。TensorAPI 通过 13 种 Layout Pattern 完整覆盖了 NPU 所有的数据组织方式。

## 4.1.1 核心常量

| 常量 | 值 | 含义 |
|------|-----|------|
| `FRACTAL_FIXED` | 16 | 标准分形块边长 |
| `BLOCK_CUBE` | 16 | Cube 块大小 |
| `C0_SIZE` | 32 字节（或 fp4 时 64） | C0 维度的字节宽度 |
| `C0_ELEMENT` | `C0_SIZE / sizeof(T)` | C0 维度元素数（如 half=16, float=8） |

C0 是分形布局中的关键概念：它由 NPU 硬件总线的数据宽度决定。对于 half 类型（2 字节），C0 = 32/2 = 16；对于 float（4 字节），C0 = 32/4 = 8。

## 4.1.2 通用分形格式

### NZ Layout Pattern (`NZLayoutPtn`)

**最常用的分形格式**，用于 Cube 矩阵计算的 A 矩阵（L0A Buffer）。

数据组织方式：将矩阵切分为 16×C0 的块，每个块内部按行优先排列，块之间也按行优先排列。

```
原矩阵 (16×32, half类型, C0=16):
┌────────────────────────────────┐
│ a00 a01 ... a0,15 | a0,16 ... │  ← 第0行
│ a10 a11 ... a1,15 | a1,16 ... │  ← 第1行
│ ...                           │
│ a15,0 ... a15,15 | a15,16 ... │  ← 第15行
└────────────────────────────────┘

分形NZ存储 (2个块: Block[0,0] 和 Block[0,1]):
Block[0,0]: a00,a01,...,a0f  a10,a11,...,a1f  ...  a15,0,...,a15,15
Block[0,1]: a0,16,...,a0,1f  ...,a1f  ...  a15,16,...,a15,31
```

Shape 结构：
```
Shape = ((16, row_blocks), (C0, col_blocks))
      = ((内行=16, 外行=块数), (内列=C0, 外列=块数))
```

Stride 结构：
```
Stride[0] = (C0, C0 * 16)        — 行方向
Stride[1] = (1,  C0 * 对齐行数)    — 列方向
```

### ZN Layout Pattern (`ZNLayoutPtn`)

用于 Cube 矩阵计算的 B 矩阵（L0B Buffer）。与 NZ 类似，但行列交换。

Shape 结构：
```
Shape = ((C0, row_blocks), (16, col_blocks))
Stride[0] = (1,  C0 * 对齐列数)
Stride[1] = (C0, C0 * 16)
```

### ZZ Layout Pattern (`ZZLayoutPtn`)

一种特殊的分形布局，常用于需要交错存储的场景。

Shape 结构：
```
Shape = ((16, row_blocks), (C0, col_blocks))
Stride: 块间按行优先，行/列方向与 NZ 有所不同
```

## 4.1.3 平坦格式

### ND Layout Pattern (`NDLayoutPtn`)

**行优先 (Row-Major)** 连续存储，与传统 C 数组排列一致。

```
Shape  = (rows, cols)
Stride = (cols, 1)
地址   =  row * cols + col
```

适用于：GM 数据存储、Vector 通路的 ND 格式数据。

### DN Layout Pattern (`DNLayoutPtn`)

**列优先 (Column-Major)** 连续存储。NPU 内部某些格式要求按列排布。

```
Shape  = (rows, cols)
Stride = (1, rows)
地址   =  row + col * rows
```

## 4.1.4 扩展格式

### NDExt Layout Pattern (`NDExtLayoutPtn`)

扩展 ND 格式，采用 1×2 的内分形结构：

```
Shape  = ((1, rows), (1, cols))
Stride = ((0, cols), (0, 1))
```

### DNExt Layout Pattern (`DNExtLayoutPtn`)

扩展 DN 格式：

```
Shape  = ((1, rows), (1, cols))
Stride = ((0, 1), (0, rows))
```

## 4.1.5 Scale 格式（fp8 量化相关）

当使用 fp8 数据类型（`fp8_e5m2_t`, `fp8_e4m3fn_t`, `fp8_e8m0_t`）时，NPU 需要配套的 Scale 因子数据，这些 Scale 因子有独立的布局格式。

### NN Layout Pattern (`NNLayoutPtn`)

Scale 因子的标准分形格式：

```
Shape  = ((2, rows/2), (16, col_blocks))
C0 = 2（固定，因为每个 Scale 元素大小为 2 bytes 规格）
```

### ScaleA ND Layout Pattern (`ScaleANDLayoutPtn`)

A 矩阵 Scale 的 ND 格式：

```
Shape  = ((1, rows), (1, cols))
Stride = ((0, cols), (0, 1))
```

### ScaleA DN Layout Pattern (`ScaleADNLayoutPtn`)

A 矩阵 Scale 的 DN 格式：

```
Shape  = ((1, rows), (2, cols/2))
Stride = ((0, 2), (1, 2*rows))
```

### ScaleB ND Layout Pattern (`ScaleBNDLayoutPtn`)

B 矩阵 Scale 的 ND 格式：

```
Shape  = ((2, rows/2), (1, cols))
Stride = ((1, 2*cols), (0, 2))
```

### ScaleB DN Layout Pattern (`ScaleBDNLayoutPtn`)

B 矩阵 Scale 的 DN 格式。

## 4.1.6 TraitType 与 C0_ELEMENT

每种 Layout 都需要配套的 `TraitType` 来指定数据类型和 C0 大小。TensorAPI 提供了三种预定义 TraitType：

```cpp
// 标准类型：用于 half(float16)、float、int8 等
template <typename T = uint16_t, size_t C0 = 32 / sizeof(T)>
struct LayoutTraitDefault {
    using type = T;
    static constexpr auto C0_ELEMENT = Std::Int<C0>{};
};

// fp8 Scale 类型：C0 = 2
template <typename T = fp8_e8m0_t, size_t C0 = 2 / sizeof(T)>
struct LayoutTraitScale {
    using type = T;
    static constexpr auto C0_ELEMENT = Std::Int<C0>{};
};

// fp4 类型：C0 = 64
template <typename T = fp4x2_e2m1_t, size_t C0 = 64 / sizeof(T)>
struct LayoutTraitFP4 {
    using type = T;
    static constexpr auto C0_ELEMENT = Std::Int<C0>{};
};
```

## 4.1.7 Layout Pattern 速查表

| Pattern | 布局形态 | 典型用途 | C0 元素数 |
|---------|---------|---------|-----------|
| `NZLayoutPtn` | 16×C0 分形, 行优先块 | L0A (A 矩阵) | 32/sizeof(T) |
| `ZNLayoutPtn` | C0×16 分形, 列优先块 | L0B (B 矩阵) | 32/sizeof(T) |
| `ZZLayoutPtn` | 16×C0 分形, ZZ 排列 | L0A/L0B 特殊场景 | 32/sizeof(T) |
| `NDLayoutPtn` | 行优先平坦 | GM 数据, Vector 通路 | 无 C0 限制 |
| `DNLayoutPtn` | 列优先平坦 | GM 数据, Vector 通路 | 无 C0 限制 |
| `NDExtLayoutPtn` | 扩展 ND (1×2 内分形) | 扩展格式 | 无 C0 限制 |
| `DNExtLayoutPtn` | 扩展 DN (1×2 内分形) | 扩展格式 | 无 C0 限制 |
| `NNLayoutPtn` | NN 分形 | fp8 Scale B | 2 |
| `ScaleANDLayoutPtn` | ScaleA ND | fp8 Scale A (ND) | 2 |
| `ScaleADNLayoutPtn` | ScaleA DN | fp8 Scale A (DN) | 2 |
| `ScaleBNDLayoutPtn` | ScaleB ND | fp8 Scale B (ND) | 2 |
| `ScaleBDNLayoutPtn` | ScaleB DN | fp8 Scale B (DN) | 2 |
| `DefaultPtn` | 默认 | 无特定格式 | 默认值 |

## 4.1.8 MakeFrameLayout 使用示例

```cpp
// NZ 分形布局：16×32 的半精度矩阵
auto nzHalf = MakeFrameLayout<NZLayoutPtn, LayoutTraitDefault<half>>(16, 32);

// ND 平坦布局：任意尺寸的浮点矩阵
auto ndFloat = MakeFrameLayout<NDLayoutPtn>(rows, cols);

// NZ 分形布局：手动指定 C0=8（浮点类型，32/4=8）
auto nzFloat = MakeFrameLayout<NZLayoutPtn, Std::Int<8>>(16, 32);

// ScaleA ND 布局：fp8 Scale 因子
auto scaleA = MakeFrameLayout<ScaleANDLayoutPtn, LayoutTraitScale<>>(rows, cols);
```

## 4.1.9 选择 Layout Pattern 的指南

| 场景 | 推荐 Pattern |
|------|-------------|
| GM 上的输入/输出数据 | `NDLayoutPtn` |
| L1 上的暂存数据 (Cube通路) | `NZLayoutPtn` + 对应 `LayoutTraitDefault<T>` |
| L0A Buffer (A矩阵) | `NZLayoutPtn` / `ZZLayoutPtn` |
| L0B Buffer (B矩阵) | `ZNLayoutPtn` / `ZZLayoutPtn` |
| UB 上的暂存数据 (Vector通路) | `NDLayoutPtn` / `DNLayoutPtn` |
| fp8 Scale 数据 | `NNLayoutPtn` / 对应的 ScaleA/B 格式 |
| 通用、不关心格式 | `NDLayoutPtn` |
