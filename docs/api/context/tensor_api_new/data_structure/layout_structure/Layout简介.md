# Layout简介

Layout\<Shape, Stride\> 数据结构是描述多维张量内存布局的基础模板类，通过编译时的形状（Shape）和步长（Stride）信息，实现逻辑坐标空间到一维内存地址空间的映射。借助模板元编程技术，该类在编译时完成计算和代码生成，从而降低运行时开销。

Layout 包含两个核心组成部分：

- **Shape**：定义数据的逻辑形状，例如二维矩阵的行数和列数或多维张量的各维度大小。
- **Stride**：定义各维度在内存中的步长，即同维度相邻元素在内存中的间隔（单位为元素），与 Shape 的维度信息一一对应。

例如，一个二维矩阵的 Shape 为 (4, 2)，Stride 为 (4, 1)，表示：

- 矩阵有 4 行、2 列。
- 列方向步长为 1，即每行中相邻元素间隔 1 个元素；行方向步长为 4，即相邻行的起始地址间隔 4 个元素。

**表 1**  线性地址视图

| 地址 | 0 | 1 | 2、3 | 4 | 5 | 6、7 | 8 | 9 | 10、11 | 12 | 13 |
|------|---|---|------|---|---|------|---|---|--------|----|----|
| 元素 | a00 | a01 | - | a10 | a11 | - | a20 | a21 | - | a30 | a31 |

**表 2**  矩阵逻辑视图

| 索引 | 列 0 | 列 1 |
|------|------|------|
| 行 0 | a00 (地址 0) | a01 (地址 1) |
| 行 1 | a10 (地址 4) | a11 (地址 5) |
| 行 2 | a20 (地址 8) | a21 (地址 9) |
| 行 3 | a30 (地址 12) | a31 (地址 13) |

## 需要包含的头文件

```cpp
#include "kernel_operator_layout.h"
```

## 原型定义

```cpp
template <typename T, typename U, typename Info = Std::ignore_t>
struct Layout : private Std::tuple<T, U>
{
    static constexpr auto depth = NestingDepthV<T>;
    static constexpr auto rank = Std::tuple_size_v<T>;

    __aicore__ inline constexpr Layout(const T& shape  = {}, const U& stride = {})
        : Std::tuple<T, U>(shape, stride)

    template <size_t... I>
    __aicore__ inline constexpr decltype(auto) Capacity() const

    __aicore__ inline constexpr decltype(auto) layout()
    __aicore__ inline constexpr decltype(auto) layout() const

    template <size_t... I>
    __aicore__ inline constexpr decltype(auto) Shape()
    template <size_t... I>
    __aicore__ inline constexpr decltype(auto) Shape() const

    template <size_t... I>
    __aicore__ inline constexpr decltype(auto) Stride()
    template <size_t... I>
    __aicore__ inline constexpr decltype(auto) Stride() const

    template <typename S>
    __aicore__ inline constexpr auto operator()(const S& coord) const

    template <size_t... I>
    __aicore__ inline constexpr decltype(auto) Rank() const

    template <size_t... I>
    __aicore__ inline constexpr decltype(auto) Size() const

    template <size_t... I>
    __aicore__ inline constexpr decltype(auto) Get()
    template <size_t... I>
    __aicore__ inline constexpr decltype(auto) Get() const
};
```

## 模板参数

| 参数名 | 描述 |
|--------|------|
| `ShapeType` | `Std::tuple` 结构类型，用于定义数据的逻辑形状，例如二维矩阵的行数和列数或多维张量的各维度大小。 |
| `StrideType` | `Std::tuple` 结构类型，用于定义各维度在内存中的步长，即同维度相邻元素在内存中的间隔（单位为元素），与 Shape 的维度信息一一对应。 |
| `Info` | 可选模板参数，默认为 `Std::ignore_t`。当 Layout 由 `MakeFrameLayout` 或 `MakePatternLayout` 构造时，该参数携带布局模式（`LayoutPattern`）和布局特征（`LayoutTrait`），如 ND / NZ / ZN / DN / ZNZ 排布模式及数据类型。通过 `GetLayoutPattern` 和 `GetLayoutTrait` 可在编译期提取 Info 中的类型信息。 |

## 成员函数

| 成员函数 | 描述 |
|----------|------|
| `Layout(const T& shape, const U& stride)` | 构造函数。用给定的 shape（形状）和 stride（步长）初始化 Layout 对象。shape 和 stride 必须为 `Std::tuple` 类型。 |
| `template<size_t... I> Capacity() const` | 计算该布局所需的最小容器容量。逐维度取 `shape[i] × stride[i]` 的最大值。传入模板参数 `I...` 可先选出子维度再计算。 |
| `layout()` | 返回 Layout 自身的引用（const / non-const 两个重载），用于统一接口中获取布局实例。 |
| `template<size_t... I> Shape()` | 提取 Layout 中 shape 部分的全部或指定维度（const / non-const 两个重载）。不指定模板参数 `I` 时返回完整 shape；传入 `I...` 时提取指定子维度。 |
| `template<size_t... I> Stride()` | 提取 Layout 中 stride 部分的全部或指定维度（const / non-const 两个重载）。用法与 `Shape()` 类似。 |
| `template<typename S> operator()(const S& coord) const` | 坐标到索引的映射运算符。传入多维逻辑坐标 `coord`，内部调用 `Crd2Idx` 返回该坐标在内存中的一维线性索引。 |
| `template<size_t... I> Rank() const` | 查询 Layout 的秩（维度数）。不指定模板参数 `I` 时返回顶层 tuple 的元素个数；传入 `I...` 时返回指定子维度的秩。 |
| `template<size_t... I> Size() const` | 查询 Layout 覆盖的元素总数。计算指定维度下 shape 各分量的乘积。不指定 `I` 时返回整张量元素总数。 |
| `template<size_t... I> Get()` | 从底层 `Std::tuple<T, U>` 中提取指定位置元素的引用（const / non-const 两个重载）。位置 0 对应 shape，位置 1 对应 stride。配合 `I...` 可进一步提取子 tuple 中的元素。 |
| `static constexpr auto depth` | shape 类型的嵌套深度（`NestingDepthV<T>`），用于编译期判断 Layout 是否为二维以上的嵌套结构。 |
| `static constexpr auto rank` | shape 类型的顶层维度数（`Std::tuple_size_v<T>`）。 |
