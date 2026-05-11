# 5.3 Tensor 操作 API

Tensor 操作 API 用于对已构造的 Tensor 进行元素访问、子区域操作、视图变换和缓存控制。

## 5.3.1 元素访问

### operator[]

```cpp
template <typename Coord>
constexpr decltype(auto) operator[](const Coord& coord);

template <typename Coord>
constexpr decltype(auto) operator[](const Coord& coord) const;
```

按坐标访问 Tensor 中单个元素。等价于 `Data()[Layout()(coord)]`。

**参数：**
| 参数 | 类型 | 说明 |
|------|------|------|
| `coord` | `const Coord&` | 坐标元组，维度须与 Layout 匹配 |

**返回：** 元素的引用（类型取决于 Engine 的 `reference`）

**示例：**
```cpp
auto tensor = MakeTensor(ptr, MakeFrameLayout<NDLayoutPtn>(16, 32));
half val = tensor[MakeCoord(3, 5)];  // 访问第 3 行第 5 列
tensor[MakeCoord(0, 7)] = 1.0_hf;    // 写入
```

## 5.3.2 切片操作

### operator() — 坐标切片

```cpp
// 单坐标
template <typename Coord>
constexpr decltype(auto) operator()(const Coord& coord);

// 多坐标（等价于 MakeCoord(c0, c1, cs...)）
template <typename Coord0, typename Coord1, typename... Coords>
constexpr decltype(auto) operator()(const Coord0& c0, const Coord1& c1, const Coords&... cs);
```

对 Tensor 进行切片，返回一个新的子 Tensor。内部流程：
1. `sliceEngine = Engine() + Layout()(coord)` — 指针偏移
2. `coordLayout = MakeCoordLayout(coord, Layout())` — 新 Layout (Shape' = Shape - coord)
3. 返回 `LocalTensor<TensorAttribute<新Engine, 新Layout>>`

**示例：**
```cpp
auto tensor   = MakeTensor(ptr, MakeFrameLayout<NDLayoutPtn>(16, 32));
auto subTensor = tensor(MakeCoord(4, 0));    // 从第 4 行开始
auto elem      = subTensor[MakeCoord(0, 5)];  // 相当于 tensor[4, 5]

// 多参数形式
auto sub2 = tensor(4, 0);                    // 等价于 MakeCoord(4, 0)
```

### Slice() — 命名切片

```cpp
template <typename Coord, typename Info>
constexpr decltype(auto) Slice(const Coord& coord, const Info& info);
```

与 `operator()` 类似，但额外限制切片形状不超过 `info` 指定的尺寸。内部使用 `MakeSliceLayout`：

- 计算 `realRow = min(剩余行数, info.row)` / `realCol = min(剩余列数, info.col)`
- 保留原始 Stride，仅缩小 Shape

**示例：**
```cpp
// 切片最大 8 行 16 列（实际切片不超过剩余尺寸）
auto slice = tensor.Slice(MakeCoord(0, 0), MakeShape(8, 16));
```

**注意：** 当源 Layout 为分形格式（depth=2）时，Slice 会自动处理分形维度的对齐。

## 5.3.3 视图变换

### Compose()

```cpp
template <typename... Layouts>
constexpr auto Compose(const Layouts&... layouts);
```

在同一份数据上叠加新的 Layout 视图。数据指针不变，只改变 Layout 解释方式。

**示例：**
```cpp
auto ndTensor = MakeTensor(ptr, ndLayout);
auto nzView   = ndTensor.Compose(nzLayout);  // 同一份数据用 NZ 格式解读
```

### Tile()

```cpp
template <typename... Layouts>
constexpr auto Tile(const Layouts&... layouts);
```

对数据进行分块（Tiling），返回新的 Tensor 视图。常用于将大块数据切分为可并行处理的小块。

**示例：**
```cpp
auto tiled = tensor.Tile(tileSubLayout);
```

## 5.3.4 L2 缓存控制

### SetL2CacheHint()

```cpp
constexpr void SetL2CacheHint(CacheMode mode);
```

为 Tensor 关联的 GM 内存设置 L2 缓存策略，影响后续对该内存区域的访问性能。

**CacheMode 枚举：**

| 值 | 含义 | 使用场景 |
|----|------|----------|
| `CACHE_MODE_NORMAL` | 正常缓存策略 | 默认值 |
| `CACHE_MODE_DISABLE` | 禁用 L2 缓存 | 数据仅使用一次 |
| `CACHE_MODE_LAST` | 标记为最后访问 | 数据后续不再使用 |
| `CACHE_MODE_PERSISTENT` | 持久缓存 | 数据会多次访问 |

**示例：**
```cpp
gmInputTensor.SetL2CacheHint(CacheMode::CACHE_MODE_NORMAL);
gmOutputTensor.SetL2CacheHint(CacheMode::CACHE_MODE_LAST);
```

## 5.3.5 类型别名

LocalTensor 对外暴露的成员类型：

| 类型别名 | 定义 | 说明 |
|----------|------|------|
| `iterator` | `EngineType::iterator` | 底层指针迭代器类型 |
| `valueType` | `EngineType::valueType` | 元素值类型（去 cv 限定） |
| `elementType` | `EngineType::elementType` | 元素类型（去引用） |
| `reference` | `EngineType::reference` | 元素引用类型（T&） |
| `engineType` | `EngineType` | ViewEngine 类型 |
| `layoutType` | `LayoutType` | Layout 类型 |

静态成员：`rank` = `LayoutType::rank`（维度数）
