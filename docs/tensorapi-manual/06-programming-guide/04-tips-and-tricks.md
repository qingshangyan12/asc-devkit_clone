# 6.4 技巧与最佳实践

本章总结 TensorAPI 编程中的常见技巧、注意事项和性能优化建议。

## 6.4.1 编译期与运行期的平衡

TensorAPI 大量使用模板元编程，目标是**编译期完成尽可能多的计算**。

### 优先使用 Std::Int<N>

在 Shape 和 Stride 中，能用 `Std::Int<N>` 就不要用运行期 `int`：

```cpp
// 推荐：编译期常量
auto shape = MakeShape(Std::Int<16>{}, Std::Int<32>{});

// 不推荐：运行期变量（除非维度的确是动态的）
int rows = 16;
auto shape = MakeShape(rows, 32);  // rows 是运行期变量
```

好处：
- `StaticLayoutSize` 可在编译期计算 Layout 容量
- 编译器能生成更优化的硬件指令

### 用 with() 而非修改原始 Atom

```cpp
// 推荐：保持 baseAtom 不变，通过 with() 创建新配置
auto baseAtom = MakeCopy(CopyL12L0A{});
auto transAtom = baseAtom.with(LoadDataTrait{true});
Copy(transAtom, l0aTensor, l1Tensor);

// 或者直接在 MakeCopy 中指定
auto transAtom2 = MakeCopy(CopyL12L0A{}, LoadDataTrait{true});
Copy(transAtom2, l0aTensor, l1Tensor);
```

## 6.4.2 对齐处理

### 维度对齐

Cube 操作要求维度对齐到块边界：

```cpp
constexpr int BLOCK = 16;
constexpr int C0    = 16;  // half 的 C0

// 对齐到 16 的倍数
int alignedM = Std::ceil_align(m, BLOCK);
int alignedK = Std::ceil_align(k, C0);
int alignedN = Std::ceil_align(n, C0);
```

### 分块数量计算

对于分形 Layout，需要通过 `Std::ceil_division` 计算分块数：

```cpp
// NZ Layout：行分成 m/16 个块，列分成 n/C0 个块
int rowBlocks = Std::ceil_division(alignedM, BLOCK);
int colBlocks = Std::ceil_division(alignedN, C0);
```

## 6.4.3 缓存策略优化

合理使用 `SetL2CacheHint()` 可以显著提升 GM 访问性能：

```cpp
// 输入：正常缓存（可能多次访问）
inputTensor.SetL2CacheHint(CacheMode::CACHE_MODE_NORMAL);

// 输出：标记为最后访问（后续不再使用）
outputTensor.SetL2CacheHint(CacheMode::CACHE_MODE_LAST);

// 只读一次的数据：禁缓存（节省缓存空间）
tempTensor.SetL2CacheHint(CacheMode::CACHE_MODE_DISABLE);
```

## 6.4.4 Tensor 视图的复用

Compose 和 Tile 不复制数据，只是改变"如何看同一块内存"：

```cpp
// 同一份 L1 数据，用不同 Layout 给不同的硬件单元
auto l1ND  = l1Tensor;                              // 给 Vector 用 (ND)
auto l1NZ  = l1ND.Compose(nzLayout);                // 给 Cube 用 (NZ)
// l1ND 和 l1NZ 指向同一块 L1 内存，无任何数据搬移！
```

这一特性可以用于：
- 同一块内存同时提供 Vector 和 Cube 的视图
- 在 Tiling 场景中用不同 Tile 视图访问不同子块

## 6.4.5 避免不必要的 Layout 转换

Layout Pattern 的切换应当发生在数据搬运阶段（Copy），而不是在计算阶段：

```cpp
// 推荐：GM (ND) → CopyGM2L1 → L1 (NZ)   — 搬运时自动转换
// 不推荐：先搬到 L1 (ND)，再手动 Compose 为 NZ
```

Copy 操作内部已针对各种格式转换进行了优化。依赖 Copy 内部的自动转换，而不要手动进行格式转换。

## 6.4.6 调试建议

### 利用编译期断言

TensorAPI 内置了大量 `static_assert`，编译时即可捕获错误：

- Shape 和 Stride 维度不匹配 → 编译错误
- Layout Pattern 与 TraitType 不兼容 → 编译错误
- 非法的数据搬运方向 → 编译错误

编译期报错是好事——比运行期崩溃容易定位得多。

### 分步验证

复杂的数据流可以分步验证：

1. 先验证 GM → L1 搬运是否正确
2. 再验证 L1 → L0A/L0B 搬运是否正确
3. 最后验证 MMAD + Fixpipe 写回是否正确

每一步使用简单的全 0 或全 1 数据来验证地址计算。

### 类型信息查询

在模板代码中可以利用以下 traits 进行调试：

```cpp
static_assert(IsLayoutV<decltype(myLayout)>, "myLayout is not a Layout");
static_assert(IsAttrTensorV<decltype(myTensor)>, "myTensor is not a LocalTensor");
static_assert(Std::is_same_v<GetLayoutPattern<decltype(myLayout)>, NZLayoutPtn>,
    "Layout pattern is not NZ");
```

## 6.4.7 性能优化清单

| 检查项 | 说明 |
|--------|------|
| ✓ 维度对齐 | M 对齐到 16，K/N 对齐到 C0 |
| ✓ 正确的 Layout Pattern | L0A=NZ, L0B=ZN, L0C=ZZ |
| ✓ 缓存策略 | 输入 NORMAL，输出 LAST/DISABLE |
| ✓ 避免空转 | Tiling 循环不处理超出范围的子块 |
| ✓ 数据类型匹配 | 避免隐式类型转换导致性能下降 |
| ✓ Fixpipe 参数 | 利用 enableRelu、roundMode 等减少后处理 |

## 6.4.8 常见错误与解决方案

| 错误 | 原因 | 解决 |
|------|------|------|
| `static_assert` 失败：Shape/Stride 不是 tuple | 未用 MakeShape/MakeStride 直接传了原始值 | 使用 `MakeShape()` / `MakeStride()` 包装 |
| 运行期地址越界 | Shape 未对齐导致容量不够 | 使用 `Std::ceil_align()` 对齐维度 |
| Copy 操作失败 | 源和目标 Tensor 的地址空间不匹配 | 检查 Location 标记是否与 Copy 操作方向一致 |
| MMAD 结果不正确 | Layout Pattern 错误 | 确认 L0A=NZ, L0B=ZN, L0C=ZZ |
| 数据搬运后格式异常 | Copy 时的 Layout Pattern 到目标 Layout Pattern 转换未自动处理 | 检查搬运双方的 Layout 是否兼容 |
