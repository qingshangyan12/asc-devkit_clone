# 2.1 Shape / Stride / Coord 元组

TensorAPI 使用 C++ 的 `Std::tuple` 作为基础容器来表示 Shape（形状）、Stride（步长）、Coord（坐标）等概念。这些是构建 Layout 和 Tensor 的基石。

## 2.1.1 类型定义

```cpp
template <typename... Shapes>
using Shape  = Std::tuple<Shapes...>;

template <typename... Strides>
using Stride = Std::tuple<Strides...>;

template <typename... Layouts>
using Tile   = Std::tuple<Layouts...>;

template <typename... Coords>
using Coord  = Std::tuple<Coords...>;
```

## 2.1.2 平坦元组与分形元组

TensorAPI 支持两种层级深度的元组：

### 平坦元组 (Flat / 2D)

适用于行优先 (ND) 或列优先 (DN) 等简单布局：

```cpp
// Shape = (rows, cols)
auto shape  = MakeShape(32, 64);

// Stride = (64, 1)    （行优先）
auto stride = MakeStride(64, Std::Int<1>{});

// Coord = (3, 5)
auto coord  = MakeCoord(3, 5);
```

### 分形元组 (Fractal / 4D)

适用于 NZ、ZZ、ZN 等分形布局。Shape 和 Stride 各包含两个子元组，分别描述**内层块结构**和**外层块索引**：

```cpp
// 分形 Shape = ((inner_row, outer_row), (inner_col, outer_col))
// 例如 NZ Layout 的 shape:
auto shape = MakeShape(
    MakeShape(Std::Int<16>{},     row_blocks),   // 内行=16, 外行=块数
    MakeShape(C0Elements,         col_blocks)    // 内列=C0, 外列=块数
);
```

分形元组中的 `Std::Int<N>` 是编译期整数常量，使得大部分布局计算可以在编译期完成。

## 2.1.3 Std::Int 编译期常量

```cpp
template <intmax_t Value>
using Int = Std::integral_constant<intmax_t, Value>;
```

`Std::Int<N>` 是编译期整数，广泛用于 Shape 和 Stride 的构造。它与运行时整数 `int` 可以混合使用，形成编译期部分静态、运行期部分动态的混合 Layout。

```cpp
auto shape = MakeShape(
    Std::Int<16>{},    // 编译期常量：内层块大小为 16
    row_blocks         // 运行期变量：块数
);
```

## 2.1.4 工厂函数

| 函数 | 签名 | 说明 |
|------|------|------|
| `MakeShape` | `constexpr Shape<Ts...> MakeShape(const Ts&... t)` | 创建 Shape 元组 |
| `MakeStride` | `constexpr Stride<Ts...> MakeStride(const Ts&... t)` | 创建 Stride 元组 |
| `MakeCoord` | `constexpr Coord<Ts...> MakeCoord(const Ts&... t)` | 创建 Coord 元组 |
| `MakeTile` | `constexpr Tile<Ts...>  MakeTile(const Ts&... t)` | 创建 Tile 元组 |

## 2.1.5 嵌套深度

通过 `NestingDepthV<T>` 可以查询元组的嵌套深度：

```cpp
static_assert(NestingDepthV<Shape<int, int>>                  == 1);  // 平坦 2D
static_assert(NestingDepthV<Shape<Shape<int,int>,Shape<int,int>>> == 2);  // 分形 4D
```

平坦元组深度为 1，分形元组深度为 2。

## 2.1.6 空元组

对于需要占位但不需要实际数据的场景，TensorAPI 提供了 `EmptyShape<N>` / `EmptyStride<N>` / `EmptyCoord<N>`，它们生成所有元素均为 `Std::Int<0>` 的元组：

```cpp
using E = EmptyShape<4>;  // Shape<Std::Int<0>, Std::Int<0>, Std::Int<0>, Std::Int<0>>
```

## 2.1.7 坐标与地址映射

TensorAPI 通过 Layout 的 `Crd2Idx()` 机制自动完成从坐标到线性地址的映射，这是整个系统能实现"坐标式编程"的基础。详见 [2.2 Layout](02-layout.md)。
