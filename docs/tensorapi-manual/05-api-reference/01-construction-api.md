# 5.1 构造类 API

构造类 API 是所有 TensorAPI 程序的入口。它们用于创建 Shape、Stride、Layout、Tensor、Pointer 和 Atom 等基础对象。

## 5.1.1 Shape / Stride / Coord / Tile 构造

### MakeShape

```cpp
template <typename... Ts>
constexpr Shape<Ts...> MakeShape(const Ts&... t);
```

创建 Shape 元组。常用于构建平坦或分形 Shape。

| 参数 | 类型 | 说明 |
|------|------|------|
| `t...` | `const Ts&...` | 各维度的大小，可以是 `int` 或 `Std::Int<N>` |

**示例：**
```cpp
auto shape2d    = MakeShape(32, 64);                           // 平坦 (32, 64)
auto shapeFract = MakeShape(MakeShape(16, 2), MakeShape(16, 4)); // 分形 ((16,2),(16,4))
```

### MakeStride

```cpp
template <typename... Ts>
constexpr Stride<Ts...> MakeStride(const Ts&... t);
```

创建 Stride 元组。

**示例：**
```cpp
auto stride2d = MakeStride(64, Std::Int<1>{});  // 行优先 64×1
```

### MakeCoord

```cpp
template <typename... Ts>
constexpr Coord<Ts...> MakeCoord(const Ts&... t);
```

创建坐标元组，用于 Element 访问和 Tensor 切片。

**示例：**
```cpp
auto coord = MakeCoord(3, 5);    // 坐标 (3, 5)
auto val = tensor[coord];        // 按坐标访问
auto sub = tensor(coord);        // 按坐标切片
```

### MakeTile

```cpp
template <typename... Ts>
constexpr Tile<Ts...> MakeTile(const Ts&... t);
```

创建 Tile 元组，用于 Tensor 的 Tile 操作。

## 5.1.2 Layout 构造

### MakeLayout (Shape + Stride)

```cpp
template <typename T, typename U>
constexpr auto MakeLayout(const T& shape, const U& stride);
```

手动指定 Shape 和 Stride 创建 Layout。

**示例：**
```cpp
auto shape  = MakeShape(16, 32);
auto stride = MakeStride(32, Std::Int<1>{});
auto layout = MakeLayout(shape, stride);
```

### MakeLayout (Shape only, Auto-Stride)

```cpp
template <typename T>
constexpr auto MakeLayout(const T& shape);
```

仅指定 Shape，Stride 自动计算。规则：
- 平坦 Shape `(d0, d1, ..., dn)` → Stride = `(d1*...*dn, ..., dn, 1)`
- 分形 Shape `((r0,r1), (c0,c1))` → 分形 Stride

**示例：**
```cpp
auto layout = MakeLayout(MakeShape(16, 32));  // Stride 自动 = (32, 1)
```

### MakeFrameLayout (Pattern + TraitType)

```cpp
// 重载 1：由 TraitType 决定 C0
template <typename LayoutPattern, typename TraitType, typename... Args>
decltype(auto) MakeFrameLayout(const Args&... args);

// 重载 2：手动指定 C0 为 IntType::value
template <typename LayoutPattern, typename IntType, typename... Args>
decltype(auto) MakeFrameLayout(const Args&... args);
```

创建带有 Pattern 信息的 Layout。重载 1 使用 `TraitType::C0_ELEMENT` 决定 C0；重载 2 直接从 `IntType`（须为 `Std::Int<N>`）获取 C0。

**示例：**
```cpp
// NZ 分形, half 类型 (C0 = 16)
auto nzLayout = MakeFrameLayout<NZLayoutPtn, LayoutTraitDefault<half>>(16, 32);

// ND 平坦, 手动 C0 = 8
auto ndLayout = MakeFrameLayout<NDLayoutPtn, Std::Int<8>>(16, 32);

// 默认 TraitType
auto ndDefault = MakeFrameLayout<NDLayoutPtn>(16, 32);
```

## 5.1.3 Tensor 构造

### MakeTensor

```cpp
template <typename Iterator, typename... Args>
constexpr auto MakeTensor(const Iterator& iter, const Args&... args);
```

三种重载（编译期自动派发）：

| 形式 | args 数量 | 说明 |
|------|-----------|------|
| `MakeTensor(ptr, layout)` | 1 | 传入已有 Layout |
| `MakeTensor(ptr, shape, stride)` | 2 | 传入 Shape 和 Stride，内部调用 MakeLayout |
| `MakeTensor(ptr, shape)` | 1 + 非 Layout | 传入 Shape，自动计算 Stride |

**返回类型：** `LocalTensor<TensorAttribute<ViewEngine<Iterator>, LayoutType>>`

**示例：**
```cpp
auto tensor1 = MakeTensor(ptr, layout);
auto tensor2 = MakeTensor(ptr, shape, stride);
auto tensor3 = MakeTensor(ptr, shape);  // stride 自动
```

## 5.1.4 Pointer 构造

### MakeMemPtr (从原始指针)

```cpp
// 显式指定 Location
template <typename Hardware, typename... Args>
constexpr auto MakeMemPtr(Args... args);  // args = 已有指针

// 自动推断 Location
template <typename Iterator>
constexpr auto MakeMemPtr(const Iterator& iter);
```

**示例：**
```cpp
__gm__ half* rawPtr  = /* ... */;
auto gmPtr  = MakeMemPtr<Location::GM>(rawPtr);
auto autoPtr = MakeMemPtr(rawPtr);          // 自动推断为 GM
```

### MakeMemPtr (从字节偏移)

```cpp
template <typename Hardware, typename TraitOrType, typename... Args>
constexpr auto MakeMemPtr(Args... args);  // args = 字节偏移
```

**示例：**
```cpp
auto l1Ptr = MakeMemPtr<Location::L1, half>(offset_in_bytes);
```

## 5.1.5 Atom 构造

### MakeCopy

```cpp
template <typename... Args>
auto MakeCopy(const Args& ...traits);
```

创建 CopyAtom 用于数据搬运。

| 参数 | 说明 |
|------|------|
| `traits...` | 操作标签 (`CopyGM2L1{}` 等) 和可选的 Trait 参数 |

**返回类型：** `CopyAtom<CopyTraits<...>>`

**示例：**
```cpp
auto atom1 = MakeCopy(CopyGM2L1{});                          // 默认配置
auto atom2 = MakeCopy(CopyL12L0A{}, LoadDataTrait{true});   // 带配置
```

### MakeMmad

```cpp
template <typename... Args>
auto MakeMmad(const Args& ...traits);
```

创建 MmadAtom 用于矩阵乘加。

**返回类型：** `MmadAtom<MmadTraits<...>>`

**示例：**
```cpp
auto mmad = MakeMmad(MmadOperation{});
auto mmadWithParams = MakeMmad(MmadOperation{}, MmadParams{m, n, k});
```
