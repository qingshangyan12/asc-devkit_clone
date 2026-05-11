# 2.2 Layout 布局描述

Layout 是 TensorAPI 最核心的类型，它描述了**多维数据在内存中的排布方式**。Layout 连接了逻辑坐标与物理地址，是整个编程模型的支柱。

## 2.2.1 Layout 结构定义

```cpp
template <typename T, typename U, typename Info = DefaultInfo>
struct Layout : private Std::tuple<T, U>
{
    static constexpr auto size  = StaticLayoutSize<T, U>::size;
    static constexpr auto depth = NestingDepthV<T>;          // 1 (flat) 或 2 (fractal)
    static constexpr auto rank  = Std::tuple_size_v<T>;      // 维度数

    constexpr Layout(const T& shape = {}, const U& stride = {});

    constexpr auto Capacity() const;         // 最大索引上限（shape·stride 的 max 值）
    constexpr auto operator()(coord) const;  // 坐标 → 线性地址（Crd2Idx）
};
```

Layout 内部存储一个 `Std::tuple<Shape, Stride>`，即一个 `(shape, stride)` 对。第三个模板参数 `Info` 用于嵌入 Layout Pattern 和 TraitType 信息，格式为 `Std::tuple<LayoutPattern, TraitType>`。

## 2.2.2 Shape 与 Stride 的关系

对于一个 Shape `(d0, d1, ..., dk)` 和 Stride `(s0, s1, ..., sk)`：

```
线性地址 = coord[0] * s0 + coord[1] * s1 + ... + coord[k] * sk
```

- **Shape** 描述每个维度有多少个元素
- **Stride** 描述每个维度相邻元素之间的地址步长

例如，行优先布局 (ND)：
```
Shape  = (rows, cols)
Stride = (cols, 1)
地址    = row * cols + col
```

列优先布局 (DN)：
```
Shape  = (rows, cols)
Stride = (1, rows)
地址    = row + col * rows
```

## 2.2.3 Info 参数：Layout Pattern 与 TraitType

`Info` 模板参数是 `Std::tuple<LayoutPattern, TraitType>`，它将数据类型 (TraitType) 和分形格式 (LayoutPattern) 嵌入到 Layout 的类型签名中：

```cpp
// ND Layout, half 类型
Layout<
    Shape<int, int>,                             // T: Shape
    Stride<int, Std::Int<1>>,                    // U: Stride
    Std::tuple<NDLayoutPtn, LayoutTraitDefault<half>>  // Info
>
```

通过 `GetLayoutPattern<T>` 和 `GetLayoutTrait<T>` 可以在编译期提取这些信息：

```cpp
using Pattern = GetLayoutPattern<decltype(layout)>;   // NDLayoutPtn
using Trait   = GetLayoutTrait<decltype(layout)>;      // LayoutTraitDefault<half>
```

## 2.2.4 Layout 的成员方法

### 查询方法

| 方法 | 返回内容 |
|------|----------|
| `Shape<I...>()` | 提取子 Shape（可选索引参数） |
| `Stride<I...>()` | 提取子 Stride |
| `Rank<I...>()` | 子元素维度数 |
| `Size<I...>()` | 子元素总元素数 |
| `Get<I...>()` | 提取子 Layout |
| `Capacity()` | 最大地址偏移 + 1 |
| `location()` | 返回自身的引用（用于接口一致性） |

### 坐标转换

| 方法 | 说明 |
|------|------|
| `operator()(const S& coord)` | 坐标 → 线性索引 (Crd2Idx) |

## 2.2.5 工厂函数

### MakeLayout

```cpp
// (1) 手动指定 Shape 和 Stride
auto layout = MakeLayout(shape, stride);

// (2) 仅给定 Shape，自动计算 Stride
auto layout = MakeLayout(shape);
```

自动计算 Stride 的规则：
- **平坦 Shape** `(d0, d1, ..., dk)`：Stride = `(d1*d2*...*dk, ..., d_{k-1}*dk, dk, 1)`
- **分形 Shape** `((r0, r1), (c0, c1))`：Stride[0][0] = 1, Stride[0][i] = r[i-1]*c[i-1]*Stride[0][i-1], Stride[1][i] = r[i]*Stride[0][i]

### MakeFrameLayout

用于创建带有 Layout Pattern 信息的分形布局：

```cpp
// 通用形式（由 TraitType 的 C0_ELEMENT 决定 C0）
auto layout = MakeFrameLayout<NZLayoutPtn, LayoutTraitDefault<half>>(rows, cols);

// 手动指定 C0
auto layout = MakeFrameLayout<NDLayoutPtn, Std::Int<8>>(rows, cols);
```

`MakeFrameLayout` 内部查表（`LayoutFormatSet` 这个 `TupleMap`）找到对应的 Maker 结构体，生成正确的 Shape 和 Stride。

## 2.2.6 Layout 信息的提取

| 函数/类型 | 说明 |
|-----------|------|
| `GetLayoutPattern<T>` | 提取 Layout 的 Pattern 类型 |
| `GetLayoutTrait<T>` | 提取 Layout 的 TraitType |
| `GetLayoutInfoT<T>` | 提取 `Std::tuple<Pattern, TraitType>` |
| `IsLayout<T>` / `IsLayoutV<T>` | 类型 trait：是否为 Layout |

## 2.2.7 子布局操作

### Get<I...>

提取指定维度的子 Layout：

```cpp
auto layout2d = MakeFrameLayout<NDLayoutPtn>(16, 32);
auto subRow   = Get<0>(layout2d);  // 提取第 0 维：Layout<Shape<int>, Stride<int>>
auto subCol   = Get<1>(layout2d);  // 提取第 1 维：Layout<Shape<int>, Stride<Int<1>>>
```

### Select<I...>

选择指定维度的 Layout，返回新的 Layout：

```cpp
auto layout3d = MakeLayout(MakeShape(a, b, c));
auto selected = Select<0, 2>(layout3d);  // 只保留第 0 和 2 维
```

### Rank<I...> / Size<I...>

查询维度数和元素总数：

```cpp
auto rank = Rank(layout);       // 返回维度数
auto size = Size(layout);       // 返回总元素数 = shape[0] * shape[1] * ...
```

### Capacity<I...>

返回布局所需的最小内存容量（字节偏移上限 + 1）：

```cpp
auto cap = Capacity(layout);    // 返回 shape[i] * stride[i] 的最大值
```

## 2.2.8 Crd2Idx 工具函数

```cpp
// 直接计算：给定 coord, shape, stride 返回线性索引
auto idx = Crd2Idx(coord, shape, stride);

// 通过 Layout 计算
auto idx = layout(myCoord);  // 等同于 layout.operator()(myCoord)
```

Crd2Idx 的核心算法：
- 平坦维度：`idx = coord[0]*stride[0] + coord[1]*stride[1] + ...`
- 分形维度：对每个子元组递归计算后求和

## 2.2.9 Layout 示例

```cpp
// === ND（行优先）Layout ===
auto ndLayout = MakeFrameLayout<NDLayoutPtn>(16, 32);
// 内部 Shape  = (16, 32)
// 内部 Stride = (32, 1)
// Crd2Idx(3,5) = 3*32 + 5 = 101

// === DN（列优先）Layout ===
auto dnLayout = MakeFrameLayout<DNLayoutPtn>(16, 32);
// Shape  = (16, 32)
// Stride = (1, 16)
// Crd2Idx(3,5) = 3*1 + 5*16 = 83

// === NZ（分形）Layout, half 类型 ===
auto nzLayout = MakeFrameLayout<NZLayoutPtn, LayoutTraitDefault<half>>(16, 32);
// 内部 Shape  = ((16, 1), (16, 2))       （因为 16/16=1, 32/16=2）
//              — 16行x32列, 内层块16x16, 外层块1x2
// Crd2Idx 先定位外层块位置，再定位块内偏移
```
