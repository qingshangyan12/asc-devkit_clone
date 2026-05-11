# 2.3 LocalTensor

LocalTensor 是 TensorAPI 中"用户直接操作"的核心类型。它将一个 **Engine（内存引擎）** 和一个 **Layout（布局描述）** 绑定在一起，形成对某段内存区域的结构化访问接口。

## 2.3.1 类型结构

```cpp
// 标记类型（幽灵类型，用于模板特化）
template <typename EngineType, typename LayoutType>
struct TensorAttribute {};

// 主模板：空定义
template <typename T>
struct LocalTensor {};

// 实际使用的特化
template <typename EngineType, typename LayoutType>
struct LocalTensor<TensorAttribute<EngineType, LayoutType>> {
    using iterator    = typename EngineType::iterator;
    using valueType   = typename EngineType::valueType;
    using elementType = typename EngineType::elementType;
    using reference   = typename EngineType::reference;

    using engineType  = EngineType;
    using layoutType  = LayoutType;

    static constexpr int rank = LayoutType::rank;

    // 内部存储: Std::tuple<layoutType, engineType>
};
```

LocalTensor 继承自 `Std::tuple<layoutType, engineType>`，这意味着它本质上就是 `{layout, engine}` 的轻量包装。

## 2.3.2 构造

### MakeTensor

```cpp
template <typename Iterator, typename... Args>
constexpr auto MakeTensor(const Iterator& iter, const Args&... args);
```

两种重载形式：

```cpp
// (1) 传入指针 + Layout 对象
auto tensor = MakeTensor(ptr, layout);

// (2) 传入指针 + Shape + Stride（内部调用 MakeLayout）
auto tensor = MakeTensor(ptr, shape, stride);

// (3) 传入指针 + Shape（内部自动计算 Stride）
auto tensor = MakeTensor(ptr, shape);
```

返回类型为 `LocalTensor<TensorAttribute<ViewEngine<Iterator>, LayoutType>>`。

## 2.3.3 访问器方法

### 获取底层组件

| 方法 | 返回类型 | 说明 |
|------|----------|------|
| `Engine()` | `EngineType&` | 返回底层内存引擎引用 |
| `Layout()` | `LayoutType&` | 返回底层 Layout 引用 |
| `Data()` | `iterator` | 返回 `Engine().Begin()` 即原始指针 |

### 形状信息

| 方法 | 返回内容 |
|------|----------|
| `Shape()` | `Layout().Shape()` |
| `Stride()` | `Layout().Stride()` |
| `Size()` | `Layout().Size()` （总元素数） |
| `Capacity()` | `Layout().Capacity()`（内存容量） |

## 2.3.4 元素访问：operator[]

```cpp
template <typename Coord>
constexpr decltype(auto) operator[](const Coord& coord);

template <typename Coord>
constexpr decltype(auto) operator[](const Coord& coord) const;
```

通过坐标访问 Tensor 中的单个元素。内部流程：

```
Data()[Layout()(coord)]
    │        │
    │        └─ Crd2Idx(coord, Shape, Stride) → 线性索引
    └─ Engine().Begin() → 原始指针
```

示例：

```cpp
auto tensor = MakeTensor(ptr, layout);
auto value  = tensor[MakeCoord(3, 5)];  // 访问第 3 行第 5 列
```

## 2.3.5 子区域操作：operator()

```cpp
// 单坐标切片
template <typename Coord>
constexpr decltype(auto) operator()(const Coord& coord);

// 多参数切片（等价于 MakeCoord(c0,c1,cs...)）
template <typename Coord0, typename Coord1, typename... Coords>
constexpr decltype(auto) operator()(const Coord0& c0, const Coord1& c1, const Coords&... cs);
```

`operator()` 用于**对 Tensor 进行切片**，返回一个新的子 Tensor。内部流程：

```
1. 创建偏移后的 Engine:  Engine + Layout(coord)
2. 创建新的 Layout:      MakeCoordLayout(coord, 原Layout)
                        → Shape' = Shape - coord, Stride' = Stride
3. 返回:                 LocalTensor<TensorAttribute<新Engine, 新Layout>>
```

示例：

```cpp
auto tensor   = MakeTensor(ptr, MakeFrameLayout<NDLayoutPtn>(16, 32));
auto subTensor = tensor(MakeCoord(4, 0));   // 从第 4 行开始
auto elem      = subTensor[MakeCoord(0, 5)]; // 相当于原 tensor[4, 5]

// 多参数等价写法
auto sub2 = tensor(4, 0);  // 等同于 MakeCoord(4, 0)
```

## 2.3.6 命名切片：Slice()

```cpp
template <typename Coord, typename Info>
constexpr decltype(auto) Slice(const Coord& coord, const Info& info);
```

`Slice()` 与 `operator()` 类似，但额外限制切片形状不超过指定大小：

```cpp
// 切片最大 8 行 16 列（实际切片不超过剩余尺寸）
auto slice = tensor.Slice(MakeCoord(0, 0), MakeShape(8, 16));
```

内部使用 `MakeSliceLayout`，通过 `min(剩余Shape, 请求Shape)` 来计算实际的切片尺寸。

## 2.3.7 Compose 与 Tile

### Compose

```cpp
template <typename... Layouts>
constexpr auto Compose(const Layouts&... layouts);
```

在同一份数据上叠加新的 Layout 视图：

```cpp
auto original = MakeTensor(ptr, ndLayout);
auto composed = original.Compose(nzLayout);  // 同一份数据，不同的 Layout
```

### Tile

```cpp
template <typename... Layouts>
constexpr auto Tile(const Layouts&... layouts);
```

对数据进行重新分块/重塑：

```cpp
auto tiled = tensor.Tile(tileLayout);  // 新的 Tile 视图
```

## 2.3.8 L2 缓存控制

```cpp
constexpr void SetL2CacheHint(CacheMode mode);
```

为 Tensor 关联的 Global Memory 设定 L2 缓存策略。支持的 CacheMode：

| 值 | 含义 |
|----|------|
| `CACHE_MODE_NORMAL` | 正常缓存 |
| `CACHE_MODE_DISABLE` | 禁用缓存 |
| `CACHE_MODE_LAST` | 仅最后使用缓存 |
| `CACHE_MODE_PERSISTENT` | 持久缓存 |

内部调用 `Engine().SetCacheMode(mode)`。

## 2.3.9 类型 Traits

```cpp
template <typename T>
struct IsAttrTensor : Std::false_type {};

template <typename Engine, typename Layout>
struct IsAttrTensor<LocalTensor<TensorAttribute<Engine, Layout>>> : Std::true_type {};

template <typename T>
constexpr bool IsAttrTensorV = IsAttrTensor<Std::remove_cvref_t<T>>::value;
```

`IsAttrTensor<T>` 用于在模板编程中判断一个类型是否为 LocalTensor。
