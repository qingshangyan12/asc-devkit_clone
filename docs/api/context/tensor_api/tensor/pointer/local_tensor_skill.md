# 本地张量技能文档

## 文档概述

本文档介绍 Ascend C 中本地张量（LocalTensor）的使用方法和相关 API。本地张量是算子开发中的核心数据结构，用于描述和操作 NPU 本地存储器中的数据。

### 主要特性

- **轻量级设计**: 张量对象仅包含布局和迭代器信息，不持有数据所有权
- **灵活布局**: 支持多种内存布局格式（NZ, ZN, DN, ND, L0C 等）
- **多维访问**: 提供直观的多维坐标访问接口
- **分块操作**: 支持张量分块和视图操作，提高数据访问效率
- **类型安全**: 编译时类型检查，确保类型安全

### 适用场景

- 算子开发中的数据描述和访问
- NPU 本地存储器（L1, L0, UB 等）的数据操作
- 矩阵运算和向量计算
- 数据搬运和转换操作
- 分块计算和并行处理

### 相关文档

- [全局张量文档](../global_tensor/global_tensor_skill.md)
- [布局 API 文档](../layout/layout_skill.md)
- [内存管理文档](../../memory/memory_skill.md)

---

## 1. MakeTensor

### 功能概述
创建本地张量对象，用于描述和操作 NPU 本地存储器中的数据。本地张量结合了数据指针和布局信息，提供了高效的张量操作接口。

### 函数原型
```cpp
template <typename Iterator, typename... Args>
__aicore__ inline constexpr auto MakeTensor(const Iterator& iter, const Args&... args);
```

### 参数说明
- `iter`: 数据迭代器/指针，指向张量数据的起始位置
- `args...`: 布局参数，可以是：
  - 一个已创建的布局对象
  - 形状参数（用于自动创建布局）

### 返回值
- 返回 `LocalTensor<TensorAttribute<Engine, Layout>>` 类型的张量对象

### 约束条件
- `iter` 必须是可解引用的迭代器类型
- `args...` 不能是可解引用类型（必须是布局参数）
- 如果提供多个参数，将自动创建布局

### 注意事项
- 支持多种内存类型（GM, UB, L1, L0A, L0B, L0C, BIAS, FIXBUF）
- 张量对象是轻量级的，主要包含布局和迭代器信息
- 支持编译时常量和运行时布局
- 提供了丰富的访问和操作接口

### 示例代码
```cpp
// 示例 1: 使用指针和布局创建张量
auto ptr = MakeL1memPtr<half>(0);
auto layout = MakeNzLayout<half>(32, 32);
auto tensor = MakeTensor(ptr, layout);

// 示例 2: 使用指针和形状创建张量（自动计算步幅）
auto tensor2 = MakeTensor(ptr, MakeShape(32, 32));

// 示例 3: 使用分形布局
auto fractalLayout = MakeNzLayout<half>(64, 64);
auto tensor3 = MakeTensor(ptr, fractalLayout);

// 示例 4: 使用 GM 内存
auto gmPtr = MakeGMmemPtr<half>(gm_addr);
auto gmTensor = MakeTensor(gmPtr, MakeDNLayout<half>(128, 128));
```

---

## LocalTensor 类成员函数

### 1. Tensor() - 获取张量对象

### 功能概述
获取张量对象本身。

### 函数原型
```cpp
__aicore__ inline constexpr decltype(auto) Tensor() const;
```

### 返回值
- 返回张量对象的常量引用

### 示例代码
```cpp
auto tensor = MakeTensor(ptr, layout);
auto& self = tensor.Tensor();
```

---

### 2. Engine() - 获取引擎对象

### 功能概述
获取张量的引擎对象（包含数据迭代器信息）。

### 函数原型
```cpp
__aicore__ inline constexpr decltype(auto) Engine() const;
__aicore__ inline constexpr decltype(auto) Engine();
```

### 返回值
- 返回引擎对象的引用

### 注意事项
- 引擎对象包含数据迭代器和类型信息

### 示例代码
```cpp
auto tensor = MakeTensor(ptr, layout);
auto engine = tensor.Engine();
auto iter = engine.Begin();  // 获取数据迭代器
```

---

### 3. Layout() - 获取布局对象

### 功能概述
获取张量的布局对象。

### 函数原型
```cpp
__aicore__ inline constexpr decltype(auto) Layout() const;
```

### 返回值
- 返回布局对象的常量引用

### 示例代码
```cpp
auto tensor = MakeTensor(ptr, layout);
auto tensorLayout = tensor.Layout();
auto shape = tensorLayout.Shape();
auto stride = tensorLayout.Stride();
```

---

### 4. Data() - 获取数据指针

### 功能概述
获取张量数据的起始指针/迭代器。

### 函数原型
```cpp
__aicore__ inline constexpr decltype(auto) Data() const;
__aicore__ inline constexpr decltype(auto) Data();
```

### 返回值
- 返回数据迭代器的引用

### 示例代码
```cpp
auto tensor = MakeTensor(ptr, layout);
auto dataPtr = tensor.Data();
```

---

### 5. Shape() - 获取形状信息

### 功能概述
获取张量的形状信息。

### 函数原型
```cpp
__aicore__ inline constexpr decltype(auto) Shape() const;
```

### 返回值
- 返回形状元组

### 示例代码
```cpp
auto tensor = MakeTensor(ptr, layout);
auto shape = tensor.Shape();
// shape 可能是 MakeShape(32, 32) 或分形形状
```

---

### 6. Stride() - 获取步幅信息

### 功能概述
获取张量的步幅信息。

### 函数原型
```cpp
__aicore__ inline constexpr decltype(auto) Stride() const;
```

### 返回值
- 返回步幅元组

### 示例代码
```cpp
auto tensor = MakeTensor(ptr, layout);
auto stride = tensor.Stride();
```

---

### 7. Size() - 获取元素总数

### 功能概述
获取张量的元素总数。

### 函数原型
```cpp
__aicore__ inline constexpr auto Size() const;
```

### 返回值
- 返回张量的元素总数

### 示例代码
```cpp
auto tensor = MakeTensor(ptr, layout);
auto size = tensor.Size();  // 例如 1024
```

---

### 8. Capacity() - 获取总容量

### 功能概述
获取张量的总容量（考虑步幅）。

### 函数原型
```cpp
__aicore__ inline constexpr auto Capacity() const;
```

### 返回值
- 返回张量的总容量

### 注意事项
- 容量可能大于实际元素数量（非连续存储时）

### 示例代码
```cpp
auto tensor = MakeTensor(ptr, layout);
auto capacity = tensor.Capacity();
```

---

### 9. operator[] - 坐标访问

### 功能概述
使用坐标元组访问张量元素。

### 函数原型
```cpp
template <typename Coord>
__aicore__ inline constexpr decltype(auto) operator[](const Coord& coord);

template <typename Coord>
__aicore__ inline constexpr decltype(auto) operator[](const Coord& coord) const;
```

### 参数说明
- `coord`: 坐标元组

### 返回值
- 返回对应位置的元素引用

### 注意事项
- 对于 b4 类型数据，会自动进行位移处理
- 坐标必须在有效范围内

### 示例代码
```cpp
auto tensor = MakeTensor(ptr, layout);
auto coord = MakeCoord(3, 5);
auto value = tensor[coord];  // 获取 (3, 5) 位置的元素
tensor[coord] = 1.0f;       // 设置 (3, 5) 位置的元素
```

---

### 10. operator() - 多参数坐标访问

### 功能概述
使用多个坐标参数访问张量元素。

### 函数原型
```cpp
template <typename Coord>
__aicore__ inline constexpr decltype(auto) operator()(const Coord& coord);

template <typename Coord>
__aicore__ inline constexpr decltype(auto) operator()(const Coord& coord) const;

template <typename Coord0, typename Coord1, typename... Coords>
__aicore__ inline constexpr decltype(auto) operator()(const Coord0& c0, const Coord1& c1, const Coords&... cs);

template <typename Coord0, typename Coord1, typename... Coords>
__aicore__ inline constexpr decltype(auto) operator()(const Coord0& c0, const Coord1& c1, const Coords&... cs) const;
```

### 参数说明
- `coord`: 坐标元组
- `c0, c1, cs...`: 多个坐标参数

### 返回值
- 返回对应位置的元素引用

### 注意事项
- 多参数版本会自动组合成坐标元组
- 空信息参数用于分块操作

### 示例代码
```cpp
auto tensor = MakeTensor(ptr, layout);

// 使用坐标元组
auto coord = MakeCoord(3, 5);
auto value1 = tensor(coord);

// 使用多个参数
auto value2 = tensor(3, 5);  // 等价于 tensor(MakeCoord(3, 5))
```

---

### 11. operator() - 分块访问

### 功能概述
使用坐标和分块信息访问张量的子块。

### 函数原型
```cpp
template <typename Coord, typename InfoType>
__aicore__ inline constexpr decltype(auto) operator()(const Coord& coord, const InfoType& info);

template <typename Coord, typename InfoType>
__aicore__ inline constexpr decltype(auto) operator()(const Coord& coord, const InfoType& info) const;
```

### 参数说明
- `coord`: 起始坐标
- `info`: 分块信息（可以是分块形状或分块张量）

### 返回值
- 返回分块张量对象

### 注意事项
- 自动计算分块的边界
- 考虑源张量和目标张量的大小限制

### 示例代码
```cpp
auto tensor = MakeTensor(ptr, MakeNzLayout<half>(64, 64));
auto coord = MakeCoord(16, 16);
auto tileShape = MakeShape(MakeShape(16, 16), MakeShape(2, 2));

// 使用分块形状
auto tileTensor = tensor(coord, tileShape);

// 使用分块张量
auto smallTensor = MakeTensor(smallPtr, MakeNzLayout<half>(32, 32));
auto tileTensor2 = tensor(coord, smallTensor);
```

---

### 12. Compose() - 组合布局

### 功能概述
组合多个布局，创建新的张量视图。

### 函数原型
```cpp
template <typename... Layouts>
__aicore__ inline constexpr auto Compose(const Layouts&... layouts);

template <typename... Layouts>
__aicore__ inline constexpr auto Compose(const Layouts&... layouts) const;
```

### 参数说明
- `layouts...`: 要组合的布局

### 返回值
- 返回组合后的新张量对象

### 注意事项
- 保持原始数据指针不变
- 创建新的布局视图

### 示例代码
```cpp
auto tensor = MakeTensor(ptr, layout);
auto layout1 = MakeShape(16, 16);
auto layout2 = MakeShape(2, 2);
auto composedTensor = tensor.Compose(layout1, layout2);
```

---

### 13. Tile() - 分块操作

### 功能概述
对张量进行分块操作。

### 函数原型
```cpp
template <typename... Layouts>
__aicore__ inline constexpr auto Tile(const Layouts&... layouts);

template <typename... Layouts>
__aicore__ inline constexpr auto Tile(const Layouts&... layouts) const;
```

### 参数说明
- `layouts...`: 分块布局参数

### 返回值
- 返回分块后的新张量对象

### 注意事项
- 用于创建张量的分块视图
- 不复制数据，只是创建新的布局视图

### 示例代码
```cpp
auto tensor = MakeTensor(ptr, MakeNzLayout<half>(64, 64));
auto tileLayout1 = MakeShape(16, 16);
auto tileLayout2 = MakeShape(4, 4);
auto tiledTensor = tensor.Tile(tileLayout1, tileLayout2);
```

---

## 张量类型定义

### LocalTensor

### 功能概述
本地张量的主要类型定义，结合了引擎和布局信息。

### 类型成员
```cpp
template <typename EngineType, typename LayoutType>
struct LocalTensor<TensorAttribute<EngineType, LayoutType>> {
    using iterator = typename EngineType::iterator;      // 迭代器类型
    using valueType = typename EngineType::valueType;      // 值类型
    using elementType = typename EngineType::elementType;  // 元素类型
    using reference = typename EngineType::reference;      // 引用类型
    using engineType = EngineType;                         // 引擎类型
    using layoutType = LayoutType;                        // 布局类型
    
    static constexpr int rank = LayoutType::rank;         // 张量维度数
};
```

### 注意事项
- `iterator`: 数据访问的迭代器类型
- `valueType`: 去除 cv 限定符的值类型
- `elementType`: 去除引用的元素类型
- `reference`: 元素的引用类型
- `rank`: 张量的维度数（编译时常量）

### 示例代码
```cpp
auto tensor = MakeTensor(ptr, layout);
using TensorType = decltype(tensor);

// 使用类型成员
using ElemType = TensorType::elementType;  // 例如 half
constexpr int rank = TensorType::rank;      // 例如 2
```

---

## 使用示例

### 示例 1: 基本张量创建和访问

```cpp
// 创建 L1 内存指针
auto l1Ptr = MakeL1memPtr<half>(0);

// 创建 NZ 布局
auto nzLayout = MakeNzLayout<half>(32, 32);

// 创建张量
auto tensor = MakeTensor(l1Ptr, nzLayout);

// 访问元素
auto coord = MakeCoord(5, 10);
auto value = tensor[coord];
tensor[coord] = 2.0f;

// 获取张量信息
auto size = tensor.Size();      // 1024
auto capacity = tensor.Capacity();  // 1024
auto shape = tensor.Shape();    // (32, 32)
```

### 示例 2: 分块操作

```cpp
// 创建大张量
auto bigTensor = MakeTensor(
    MakeL1memPtr<half>(0),
    MakeNzLayout<half>(64, 64)
);

// 创建小张量
auto smallTensor = MakeTensor(
    MakeL1memPtr<half>(4096),
    MakeNzLayout<half>(32, 32)
);

// 从大张量中提取分块
auto coord = MakeCoord(16, 16);
auto tileTensor = bigTensor(coord, smallTensor);

// 访问分块元素
auto tileValue = tileTensor(MakeCoord(0, 0));
```

### 示例 3: 多维张量操作

```cpp
// 创建多维张量
auto multiDimTensor = MakeTensor(
    MakeL1memPtr<half>(0),
    MakeLayout(MakeShape(8, 8, 8))
);

// 访问多维元素
auto value = multiDimTensor(2, 3, 4);

// 使用坐标元组
auto coord = MakeCoord(2, 3, 4);
auto value2 = multiDimTensor(coord);
```

### 示例 4: GM 内存张量

```cpp
// 创建 GM 内存张量
auto gmTensor = MakeTensor(
    MakeGMmemPtr<half>(gm_addr),
    MakeDNLayout<half>(128, 128)
);

// 访问 GM 数据
auto gmValue = gmTensor(10, 20);
```

### 示例 5: 张量视图组合

```cpp
// 创建原始张量
auto originalTensor = MakeTensor(
    MakeL1memPtr<half>(0),
    MakeNzLayout<half>(64, 64)
);

// 创建组合视图
auto composedTensor = originalTensor.Compose(
    MakeShape(16, 16),
    MakeShape(4, 4)
);

// 组合视图仍然访问原始数据
auto composedValue = composedTensor(1, 1);
```

---

## 性能优化建议

1. **使用编译时常量布局**
   ```cpp
   // 推荐：编译时常量
   constexpr auto layout = MakeNzLayout<half, Std::Int<32>, Std::Int<32>>(...);
   
   // 避免：运行时布局（在尺寸已知时）
   auto layout = MakeNzLayout<half>(32, 32);
   ```

2. **选择合适的布局格式**
   - 矩阵运算：使用 NZ/ZN 格式
   - 通用数据：使用 DN/ND 格式
   - L0 缓存：使用 L0C 格式

3. **避免频繁创建张量对象**
   - 张量对象是轻量级的，但频繁创建仍有开销
   - 重用张量对象

4. **使用分块访问**
   - 大数据访问时使用分块操作
   - 提高缓存利用率

5. **选择合适的内存类型**
    - 频繁访问：使用 L1/L0 缓存
    - 大数据存储：使用 GM 内存
    - 临时数据：使用 UB 缓冲区

---

## 常见问题解答（FAQ）

### Q1: LocalTensor 和 GlobalTensor 有什么区别？

**A**: 
- **LocalTensor**: 用于描述 NPU 本地存储器（L1, L0, UB 等）中的数据，访问速度快但容量有限
- **GlobalTensor**: 用于描述全局内存（GM）中的数据，容量大但访问速度较慢
- 通常需要通过数据搬运 API 在两者之间传输数据

### Q2: 如何选择合适的布局格式？

**A**: 
- **NZ/ZN**: 矩阵运算，特别是与矩阵乘法相关的操作
- **DN/ND**: 通用数据存储和访问
- **L0C**: L0 缓存专用格式，用于矩阵乘法结果
- **C1**: 一维数据，适合向量运算

### Q3: 张量对象会复制数据吗？

**A**: 
不会。LocalTensor 是轻量级的视图对象，仅包含布局和迭代器信息，不持有数据所有权。复制张量对象只是复制视图信息，不会复制底层数据。

### Q4: 如何处理多维张量的访问？

**A**: 
使用 `operator()` 或 `operator[]` 配合坐标元组：
```cpp
// 多参数访问
auto value = tensor(i, j, k);

// 坐标元组访问
auto coord = MakeCoord(i, j, k);
auto value = tensor[coord];
```

### Q5: 分块操作的性能优势是什么？

**A**: 
- 提高缓存命中率
- 支持并行计算
- 减少数据搬运开销
- 适合分块算法实现

### Q6: 编译时常量布局和运行时布局有什么区别？

**A**: 
- **编译时常量**: 使用 `MakeLayout<T, Int...>()`，编译时确定，性能更好
- **运行时布局**: 使用 `MakeLayout(dim1, dim2)`，运行时确定，灵活性更高
- 在尺寸已知时优先使用编译时常量布局

### Q7: 如何确保张量访问的安全性？

**A**: 
- 确保坐标在有效范围内
- 检查张量大小和容量
- 使用编译时类型检查
- 避免越界访问

### Q8: 张量对象可以在函数间传递吗？

**A**: 
可以。由于张量对象是轻量级的，传递开销很小。但需要注意：
- 确保底层数据在传递期间保持有效
- 避免跨核传递本地存储器指针
- 全局内存张量可以安全传递

---

## 错误处理和注意事项

### 常见错误

1. **越界访问**
   - **现象**: 访问超出张量范围的坐标
   - **解决**: 使用 `Shape()` 检查边界，确保坐标有效

2. **类型不匹配**
   - **现象**: 布局类型与数据类型不匹配
   - **解决**: 确保布局和指针使用相同的数据类型

3. **内存不足**
   - **现象**: 分配的内存大小不足
   - **解决**: 检查 `Capacity()` 和 `Size()`，确保内存足够

4. **布局不兼容**
   - **现象**: 操作需要特定布局格式
   - **解决**: 使用正确的布局格式或进行布局转换

### 最佳实践

1. **使用编译时常量**
   ```cpp
   constexpr auto layout = MakeNzLayout<half, Int<32>, Int<32>>();
   ```

2. **检查张量大小**
   ```cpp
   if (tensor.Size() < requiredSize) {
       // 处理大小不足的情况
   }
   ```

3. **使用类型别名**
   ```cpp
   using TensorType = LocalTensor<TensorAttribute<L1MemPtr, NzLayout<half>>>;
   ```

4. **重用张量对象**
   ```cpp
   // 避免频繁创建
   auto tensor = MakeTensor(ptr, layout);
   for (int i = 0; i < N; ++i) {
       // 重用 tensor
   }
   ```

---

## 相关 API

### 布局 API
- `MakeNzLayout`: 创建 NZ 布局
- `MakeDNLayout`: 创建 DN 布局
- `MakeLayout`: 创建通用布局

### 内存 API
- `MakeL1memPtr`: 创建 L1 内存指针
- `MakeGMmemPtr`: 创建 GM 内存指针
- `MakeUBmemPtr`: 创建 UB 内存指针

### 形状 API
- `MakeShape`: 创建形状对象
- `MakeCoord`: 创建坐标对象

---

## 版本信息

- **文档版本**: 1.0
- **API 版本**: Ascend C 22.0
- **最后更新**: 2026-03-24

---

## 参考资源

- [Ascend C 编程指南](https://www.hiascend.com/document)
- [算子开发最佳实践](https://www.hiascend.com/developer)
- [NPU 架构手册](https://www.hiascend.com/hardware)
