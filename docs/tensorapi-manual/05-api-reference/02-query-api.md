# 5.2 查询类 API

查询类 API 用于从 Layout 和 Tensor 中提取维度信息、计算索引、获取子结构。

## 5.2.1 Tensor 查询

### Engine()

```cpp
constexpr decltype(auto) Engine() const;
constexpr decltype(auto) Engine();
```

返回 Tensor 底层的 ViewEngine 引用。

### Layout()

```cpp
constexpr decltype(auto) Layout() const;
```

返回 Tensor 底层的 Layout 引用。

### Data()

```cpp
constexpr decltype(auto) Data() const;
constexpr decltype(auto) Data();
```

返回 `Engine().Begin()`，即指向数据起点的原始指针。

### Shape()

```cpp
constexpr decltype(auto) Shape() const;
```

返回 `Layout().Shape()`。

### Stride()

```cpp
constexpr decltype(auto) Stride() const;
```

返回 `Layout().Stride()`。

### Size()

```cpp
constexpr auto Size() const;
```

返回 `Layout().Size()`，即数据总元素数。

### Capacity()

```cpp
constexpr auto Capacity() const;
```

返回 `Layout().Capacity()`，即最大地址偏移 + 1（所需最小内存容量）。

**示例：**
```cpp
auto tensor   = MakeTensor(ptr, layout);
auto engine   = tensor.Engine();
auto shape    = tensor.Shape();
auto size     = tensor.Size();
auto ptr      = tensor.Data();
```

## 5.2.2 Layout 查询

### Shape<I...>()

```cpp
template <size_t... I>
constexpr decltype(auto) Shape();
```

提取 Shape 的指定维度。无参数时返回完整 Shape。

### Stride<I...>()

```cpp
template <size_t... I>
constexpr decltype(auto) Stride();
```

提取 Stride 的指定维度。

### operator()(coord)

```cpp
template <typename S>
constexpr auto operator()(const S& coord) const;
```

坐标 → 线性索引：`Crd2Idx(coord, Shape(), Stride())`。

**示例：**
```cpp
auto idx = layout(MakeCoord(3, 5));  // 3*32 + 5 = 101 (ND格式)
```

### Rank<I...>()

```cpp
template <size_t... I>
constexpr decltype(auto) Rank() const;
```

查询子维度的 rank（元素数）。返回 `Shape()` 中对应子元组的大小。

### Size<I...>()

```cpp
template <size_t... I>
constexpr decltype(auto) Size() const;
```

查询子维度的 Size（总元素数）。递归计算 `Shape()` 中所有元素的乘积。

### Get<I...>()

```cpp
template <size_t... I>
constexpr decltype(auto) Get();
```

提取子 Layout（包含子 Shape 和子 Stride）。

### Capacity()

```cpp
constexpr auto Capacity() const;
```

计算最大偏址 = `shape[i] * stride[i]` 的最大值（用于确定需要分配的内存大小）。

## 5.2.3 自由函数查询

### Crd2Idx

```cpp
template <typename T, typename U, typename S>
constexpr auto Crd2Idx(const T& coord, const U& shape, const S& stride);
```

将多元坐标转换为线性索引，无需 Layout 对象：

```cpp
auto idx = Crd2Idx(MakeCoord(3, 5), MakeShape(16, 32), MakeStride(32, 1));
// idx = 3*32 + 5 = 101
```

### GetShape<I...>() / GetStride<I...>()

```cpp
template <size_t... Is, typename LayoutType>
constexpr auto GetShape(const LayoutType& layout);    // = layout.Shape<Is...>()

template <size_t... Is, typename LayoutType>
constexpr auto GetStride(const LayoutType& layout);   // = layout.Stride<Is...>()
```

自由函数版本的 Shape/Stride 访问。

### Cosize<I...>()

```cpp
template <size_t... Is, typename LayoutType>
constexpr auto Cosize(const LayoutType& layout);
```

Coshape（即 Shape 每个维度对齐到 Stride 后的索引最大值）。计算方式：
```
Coshape = (shape[0] - 1) * |stride[0]| + (shape[1] - 1) * |stride[1]| + ...
```

### Rank<I...>() / Size<I...>() / Select<I...>() / Get<I...>()

```cpp
template <size_t... Is, typename LayoutType>
constexpr auto Rank(const LayoutType& layout);

template <size_t... Is, typename LayoutType>
constexpr auto Size(const LayoutType& layout);

template <size_t... Is, typename LayoutType>
constexpr auto Select(const LayoutType& layout);  // 选择指定维度

template <size_t... Is, typename LayoutType>
constexpr auto Get(const LayoutType& layout);     // 提取子 Layout
```

## 5.2.4 Get (Tuple 操作)

```cpp
// 单索引
template <size_t I, typename Tuple>
constexpr auto Get(Tuple&& t);

// 多索引（递归提取）
template <size_t I0, size_t I1, size_t... Is, typename Tuple>
constexpr auto Get(Tuple&& t);
```

从 Tuple（Shape、Stride、Coord 等）中提取元素：

```cpp
auto shape = MakeShape(16, 32);
auto first = Get<0>(shape);   // 16
auto all   = Get(shape);      // (16, 32)
```
