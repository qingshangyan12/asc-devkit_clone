# 内存指针技能文档

## 1. MakeGMmemPtr

### 功能概述
创建全局内存（GM）指针，用于访问 NPU 的全局内存。全局内存是 NPU 的主存储器，容量大但访问速度相对较慢。

### 函数原型
```cpp
template <typename Iterator>
__aicore__ inline constexpr auto MakeGMmemPtr(Iterator iter);
```

### 参数说明
- `iter`: 迭代器或指针，指向全局内存的起始地址

### 返回值
- 返回 GM 内存指针对象，类型为 `MemPtr<Hardware::GM, Iterator>`

### 约束条件
- `iter` 必须是有效的迭代器类型
- 地址必须在全局内存范围内

### 注意事项
- 全局内存容量大，适合存储大量数据
- 访问速度相对较慢，建议先将数据搬运到本地缓存
- 适合存储输入/输出数据、模型参数等
- 需要注意地址对齐要求

### 示例代码
```cpp
// 示例 1: 使用地址创建 GM 指针
uint64_t gm_addr = 0x10000000;
auto gmPtr = MakeGMmemPtr(gm_addr);

// 示例 2: 使用原始指针创建
half* gm_buffer = reinterpret_cast<half*>(0x10000000);
auto gmPtr2 = MakeGMmemPtr(gm_buffer);

// 示例 3: 创建张量
auto gmTensor = MakeTensor(gmPtr, MakeDNLayout<half>(128, 128));
```

---

## 2. MakeUBmemPtr

### 功能概述
创建统一缓冲区（UB）指针，用于访问 NPU 的统一缓冲区。UB 是一种灵活的本地存储器，可用于临时数据存储。

### 函数原型
```cpp
template <typename Iterator>
__aicore__ inline constexpr auto MakeUBmemPtr(Iterator iter);

template <typename T, typename U>
__aicore__ inline auto MakeUBmemPtr(const U& byteOffset);
```

### 参数说明
- `iter`: 迭代器或指针
- `T`: 数据类型（用于字节偏移版本）
- `byteOffset`: 字节偏移量

### 返回值
- 返回 UB 内存指针对象，类型为 `MemPtr<Hardware::UB, Iterator>`

### 约束条件
- 地址必须在 UB 范围内
- 字节偏移必须在有效范围内

### 注意事项
- UB 是灵活的本地存储器
- 适合存储临时数据和中间结果
- 支持字节偏移访问
- 容量有限，需要合理规划使用

### 示例代码
```cpp
// 示例 1: 使用指针创建
__ubuf__ half* ub_buffer = reinterpret_cast<__ubuf__ half*>(0);
auto ubPtr = MakeUBmemPtr(ub_buffer);

// 示例 2: 使用字节偏移创建
auto ubPtr2 = MakeUBmemPtr<half>(1024);  // 偏移 1024 字节

// 示例 3: 创建张量
auto ubTensor = MakeTensor(ubPtr, MakeNzLayout<half>(32, 32));
```

---

## 3. MakeL1memPtr

### 功能概述
创建 L1 缓存指针，用于访问 NPU 的 L1 缓存。L1 缓存是本地存储器，访问速度快，适合频繁访问的数据。

### 函数原型
```cpp
template <typename Iterator>
__aicore__ inline constexpr auto MakeL1memPtr(Iterator iter);

template <typename T, typename U>
__aicore__ inline auto MakeL1memPtr(const U& byteOffset);
```

### 参数说明
- `iter`: 迭代器或指针
- `T`: 数据类型（用于字节偏移版本）
- `byteOffset`: 字节偏移量

### 返回值
- 返回 L1 内存指针对象，类型为 `HardwareMemPtr<Hardware::L1, Iterator>`

### 约束条件
- 地址必须在 L1 缓存范围内
- 字节偏移必须在有效范围内

### 注意事项
- L1 缓存访问速度快
- 适合存储频繁访问的数据
- 容量有限（通常为 1MB）
- 建议用于存储激活值、中间结果等

### 示例代码
```cpp
// 示例 1: 使用指针创建
__cbuf__ half* l1_buffer = reinterpret_cast<__cbuf__ half*>(0);
auto l1Ptr = MakeL1memPtr(l1_buffer);

// 示例 2: 使用字节偏移创建
auto l1Ptr2 = MakeL1memPtr<half>(2048);  // 偏移 2048 字节

// 示例 3: 创建张量
auto l1Tensor = MakeTensor(l1Ptr, MakeNzLayout<half>(64, 64));
```

---

## 4. MakeL0AmemPtr

### 功能概述
创建 L0A 缓存指针，用于访问 NPU 的 L0A 缓存。L0A 是矩阵运算单元的输入缓存，专门用于存储矩阵 A 的数据。

### 函数原型
```cpp
template <typename Iterator>
__aicore__ inline constexpr auto MakeL0AmemPtr(Iterator iter);

template <typename T, typename U>
__aicore__ inline auto MakeL0AmemPtr(const U& byteOffset);
```

### 参数说明
- `iter`: 迭代器或指针
- `T`: 数据类型（用于字节偏移版本）
- `byteOffset`: 字节偏移量

### 返回值
- 返回 L0A 内存指针对象，类型为 `MemPtr<Hardware::L0A, Iterator>`

### 约束条件
- 地址必须在 L0A 缓存范围内
- 字节偏移必须在有效范围内

### 注意事项
- L0A 专用于矩阵乘法的输入矩阵 A
- 访问速度极快
- 容量较小，通常为 32KB
- 必须使用 NZ 或 ZN 格式布局
- 适合存储左操作数矩阵

### 示例代码
```cpp
// 示例 1: 使用指针创建
__ca__ half* l0a_buffer = reinterpret_cast<__ca__ half*>(0);
auto l0aPtr = MakeL0AmemPtr(l0a_buffer);

// 示例 2: 使用字节偏移创建
auto l0aPtr2 = MakeL0AmemPtr<half>(512);  // 偏移 512 字节

// 示例 3: 创建张量（必须使用 NZ/ZN 格式）
auto l0aTensor = MakeTensor(l0aPtr, MakeNzLayout<half>(16, 16));
```

---

## 5. MakeL0BmemPtr

### 功能概述
创建 L0B 缓存指针，用于访问 NPU 的 L0B 缓存。L0B 是矩阵运算单元的输入缓存，专门用于存储矩阵 B 的数据。

### 函数原型
```cpp
template <typename Iterator>
__aicore__ inline constexpr auto MakeL0BmemPtr(Iterator iter);

template <typename T, typename U>
__aicore__ inline auto MakeL0BmemPtr(const U& byteOffset);
```

### 参数说明
- `iter`: 迭代器或指针
- `T`: 数据类型（用于字节偏移版本）
- `byteOffset`: 字节偏移量

### 返回值
- 返回 L0B 内存指针对象，类型为 `MemPtr<Hardware::L0B, Iterator>`

### 约束条件
- 地址必须在 L0B 缓存范围内
- 字节偏移必须在有效范围内

### 注意事项
- L0B 专用于矩阵乘法的输入矩阵 B
- 访问速度极快
- 容量较小，通常为 32KB
- 必须使用 NZ 或 ZN 格式布局
- 适合存储右操作数矩阵

### 示例代码
```cpp
// 示例 1: 使用指针创建
__cb__ half* l0b_buffer = reinterpret_cast<__cb__ half*>(0);
auto l0bPtr = MakeL0BmemPtr(l0b_buffer);

// 示例 2: 使用字节偏移创建
auto l0bPtr2 = MakeL0BmemPtr<half>(512);  // 偏移 512 字节

// 示例 3: 创建张量（必须使用 NZ/ZN 格式）
auto l0bTensor = MakeTensor(l0bPtr, MakeNzLayout<half>(16, 16));
```

---

## 6. MakeL0CmemPtr

### 功能概述
创建 L0C 缓存指针，用于访问 NPU 的 L0C 缓存。L0C 是矩阵运算单元的输出缓存，专门用于存储矩阵运算的结果。

### 函数原型
```cpp
template <typename Iterator>
__aicore__ inline constexpr auto MakeL0CmemPtr(Iterator iter);

template <typename T, typename U>
__aicore__ inline auto MakeL0CmemPtr(const U& byteOffset);
```

### 参数说明
- `iter`: 迭代器或指针
- `T`: 数据类型（用于字节偏移版本）
- `byteOffset`: 字节偏移量

### 返回值
- 返回 L0C 内存指针对象，类型为 `MemPtr<Hardware::L0C, Iterator>`

### 约束条件
- 地址必须在 L0C 缓存范围内
- 字节偏移必须在有效范围内

### 注意事项
- L0C 专用于矩阵乘法的输出结果
- 访问速度极快
- 容量较小，通常为 32KB
- 必须使用 L0C 格式布局
- 适合存储矩阵运算结果

### 示例代码
```cpp
// 示例 1: 使用指针创建
__cc__ half* l0c_buffer = reinterpret_cast<__cc__ half*>(0);
auto l0cPtr = MakeL0CmemPtr(l0c_buffer);

// 示例 2: 使用字节偏移创建
auto l0cPtr2 = MakeL0CmemPtr<half>(512);  // 偏移 512 字节

// 示例 3: 创建张量（必须使用 L0C 格式）
auto l0cTensor = MakeTensor(l0cPtr, MakeL0CLayout(16, 16));
```

---

## 7. MakeBiasmemPtr

### 功能概述
创建偏置缓冲区（BIAS）指针，用于访问 NPU 的偏置缓冲区。BIAS 缓存专门用于存储偏置数据。

### 函数原型
```cpp
template <typename Iterator>
__aicore__ inline constexpr auto MakeBiasmemPtr(Iterator iter);

template <typename T, typename U>
__aicore__ inline auto MakeBiasmemPtr(const U& byteOffset);
```

### 参数说明
- `iter`: 迭代器或指针
- `T`: 数据类型（用于字节偏移版本）
- `byteOffset`: 字节偏移量

### 返回值
- 返回 BIAS 内存指针对象，类型为 `MemPtr<Hardware::BIAS, Iterator>`

### 约束条件
- 地址必须在 BIAS 缓存范围内
- 字节偏移必须在有效范围内

### 注意事项
- BIAS 专用于存储偏置数据
- 适合神经网络中的偏置项
- 容量有限，需要合理规划使用

### 示例代码
```cpp
// 示例 1: 使用指针创建
__biasbuf__ half* bias_buffer = reinterpret_cast<__biasbuf__ half*>(0);
auto biasPtr = MakeBiasmemPtr(bias_buffer);

// 示例 2: 使用字节偏移创建
auto biasPtr2 = MakeBiasmemPtr<half>(256);  // 偏移 256 字节

// 示例 3: 创建张量
auto biasTensor = MakeTensor(biasPtr, MakeDNLayout<half>(128, 1));
```

---

## 8. MakeFixbufmemPtr

### 功能概述
创建固定缓冲区（FIXBUF）指针，用于访问 NPU 的固定缓冲区。FIXBUF 是一种特殊的本地存储器，用于固定模式的数据访问。

### 函数原型
```cpp
template <typename Iterator>
__aicore__ inline constexpr auto MakeFixbufmemPtr(Iterator iter);

template <typename T, typename U>
__aicore__ inline auto MakeFixbufmemPtr(const U& byteOffset);
```

### 参数说明
- `iter`: 迭代器或指针
- `T`: 数据类型（用于字节偏移版本）
- `byteOffset`: 字节偏移量

### 返回值
- 返回 FIXBUF 内存指针对象，类型为 `MemPtr<Hardware::FIXBUF, Iterator>`

### 约束条件
- 地址必须在 FIXBUF 缓存范围内
- 字节偏移必须在有效范围内

### 注意事项
- FIXBUF 用于固定模式的数据访问
- 适合需要固定访问模式的数据
- 容量有限，需要合理规划使用

### 示例代码
```cpp
// 示例 1: 使用指针创建
__fbuf__ half* fixbuf_buffer = reinterpret_cast<__fbuf__ half*>(0);
auto fixbufPtr = MakeFixbufmemPtr(fixbuf_buffer);

// 示例 2: 使用字节偏移创建
auto fixbufPtr2 = MakeFixbufmemPtr<half>(512);  // 偏移 512 字节

// 示例 3: 创建张量
auto fixbufTensor = MakeTensor(fixbufPtr, MakeDNLayout<half>(64, 64));
```

---

## 内存类型对比

| 内存类型 | 容量 | 访问速度 | 用途 | 特殊要求 |
|---------|------|----------|------|---------|
| GM | 大（GB级） | 慢 | 主存储器，输入/输出数据 | 无 |
| UB | 中（MB级） | 中 | 临时数据，中间结果 | 灵活使用 |
| L1 | 小（1MB） | 快 | 频繁访问的数据 | 需要合理规划 |
| L0A | 很小（32KB） | 极快 | 矩阵乘法输入A | 必须使用NZ/ZN格式 |
| L0B | 很小（32KB） | 极快 | 矩阵乘法输入B | 必须使用NZ/ZN格式 |
| L0C | 很小（32KB） | 极快 | 矩阵乘法输出 | 必须使用L0C格式 |
| BIAS | 很小（KB级） | 快 | 偏置数据 | 神经网络偏置 |
| FIXBUF | 很小（KB级） | 快 | 固定模式访问 | 特殊访问模式 |

---

## 使用场景和最佳实践

### 1. 数据搬运流程

```cpp
// GM -> L1 -> L0A/L0B -> L0C -> L1 -> GM

// 从 GM 加载到 L1
auto gmPtr = MakeGMmemPtr(gm_addr);
auto l1Ptr = MakeL1memPtr<half>(0);
auto gmTensor = MakeTensor(gmPtr, MakeDNLayout<half>(128, 128));
auto l1Tensor = MakeTensor(l1Ptr, MakeNzLayout<half>(64, 64));

// 执行数据搬运
DataCopy(gmTensor, l1Tensor);

// 从 L1 加载到 L0A/L0B
auto l0aPtr = MakeL0AmemPtr<half>(0);
auto l0bPtr = MakeL0BmemPtr<half>(0);
auto l0aTensor = MakeTensor(l0aPtr, MakeNzLayout<half>(16, 16));
auto l0bTensor = MakeTensor(l0bPtr, MakeNzLayout<half>(16, 16));

// 执行数据搬运
DataCopy(l1Tensor, l0aTensor);
DataCopy(l1Tensor, l0bTensor);
```

### 2. 矩阵乘法示例

```cpp
// 准备输入数据
auto l0aPtr = MakeL0AmemPtr<half>(0);
auto l0bPtr = MakeL0BmemPtr<half>(0);
auto l0cPtr = MakeL0CmemPtr<half>(0);

auto l0aTensor = MakeTensor(l0aPtr, MakeNzLayout<half>(16, 16));
auto l0bTensor = MakeTensor(l0bPtr, MakeNzLayout<half>(16, 16));
auto l0cTensor = MakeTensor(l0cPtr, MakeL0CLayout(16, 16));

// 执行矩阵乘法
Matmul(l0aTensor, l0bTensor, l0cTensor);
```

### 3. 偏置添加示例

```cpp
// 准备偏置数据
auto biasPtr = MakeBiasmemPtr<half>(0);
auto biasTensor = MakeTensor(biasPtr, MakeDNLayout<half>(128, 1));

// 准备输出数据
auto l1Ptr = MakeL1memPtr<half>(0);
auto l1Tensor = MakeTensor(l1Ptr, MakeNzLayout<half>(64, 64));

// 添加偏置
Add(l1Tensor, biasTensor, l1Tensor);
```

### 4. 多缓冲区使用

```cpp
// 使用多个 UB 缓冲区进行流水线处理
auto ubPtr1 = MakeUBmemPtr<half>(0);
auto ubPtr2 = MakeUBmemPtr<half>(4096);
auto ubPtr3 = MakeUBmemPtr<half>(8192);

auto ubTensor1 = MakeTensor(ubPtr1, MakeNzLayout<half>(32, 32));
auto ubTensor2 = MakeTensor(ubPtr2, MakeNzLayout<half>(32, 32));
auto ubTensor3 = MakeTensor(ubPtr3, MakeNzLayout<half>(32, 32));

// 流水线处理
// Stage 1
Process(ubTensor1);
// Stage 2
Process(ubTensor2);
);
```

---

## 内存管理建议

### 1. 容量规划

```cpp
// 合理规划各层内存使用
constexpr size_t L0_SIZE = 32 * 1024;      // 32KB
constexpr size_t L1_SIZE = 1 * 1024 * 1024; // 1MB
constexpr size_t UB_SIZE = 2 * 1024 * 1024;  // 2MB

// 计算所需内存
constexpr size_t tensor_size = 64 * 64 * sizeof(half);  // 8KB
static_assert(tensor_size <= L0_SIZE, "Tensor too large for L0");
```

### 2. 地址对齐

```cpp
// 确保地址对齐
constexpr size_t ALIGNMENT = 32;  // 32字节对齐

uint64_t aligned_addr = (addr + ALIGNMENT - 1) / ALIGNMENT * ALIGNMENT;
auto alignedPtr = MakeGMmemPtr(aligned_addr);
```

### 3. 内存复用

```cpp
// 复用同一块内存
auto l1Ptr = MakeL1memPtr<half>(0);

// 第一阶段使用
auto tensor1 = MakeTensor(l1Ptr, MakeNzLayout<half>(32, 32));
Process(tensor1);

// 第二阶段复用
auto tensor2 = MakeTensor(l1Ptr, MakeNzLayout<half>(32, 32));
Process(tensor2);
```

### 4. 避免内存溢出

```cpp
// 检查内存边界
constexpr size_t MAX_L1_SIZE = 1 * 1024 * 1024;
constexpr size_t TENSOR_SIZE = 64 * 64 * sizeof(half);
constexpr size_t OFFSET = 512 * 1024;

static_assert(OFFSET + TENSOR_SIZE <= MAX_L1_SIZE, "Memory overflow");

auto l1Ptr = MakeL1memPtr<half>(OFFSET);
```

---

## 性能优化建议

### 1. 选择合适的内存类型

```cpp
// 频繁访问的小数据 -> L0A/L0B/L0C
auto l0aTensor = MakeTensor(MakeL0AmemPtr<half>(0), layout);

// 中等大小的频繁访问数据 -> L1
auto l1Tensor = MakeTensor(MakeL1memPtr<half>(0), layout);

// 大数据或不频繁访问 -> GM
auto gmTensor = MakeTensor(MakeGMmemPtr(addr), layout);

// 临时数据 -> UB
auto ubTensor = MakeTensor(MakeUBmemPtr<half>(0), layout);
```

### 2. 减少数据搬运

```cpp
// 尽量在本地内存完成计算
auto l1Ptr = MakeL1memPtr<half>(0);
auto l1Tensor = MakeTensor(l1Ptr, layout);

// 在 L1 中完成多个操作
Process1(l1Tensor);
Process2(l1Tensor);
Process3(l1Tensor);

// 最后再写回 GM
DataCopy(l1Tensor, gmTensor);
```

### 3. 使用合适的布局

```cpp
// L0A/L0B 必须使用 NZ/ZN 格式
auto l0aTensor = MakeTensor(l0aPtr, MakeNzLayout<half>(16, 16));

// L0C 必须使用 L0C 格式
auto l0cTensor = MakeTensor(l0cPtr, MakeL0CLayout(16, 16));

// GM/L1/UB 可以使用多种格式
auto l1Tensor = MakeTensor(l1Ptr, MakeDNLayout<half>(64, 64));
```

### 4. 利用字节偏移

```cpp
// 使用字节偏移快速创建指针
auto basePtr = MakeL1memPtr<half>(0);

// 快速创建多个指针
auto ptr1 = MakeL1memPtr<half>(0);
auto ptr2 = MakeL1memPtr<half>(1024);
auto ptr3 = MakeL1memPtr<half>(2048);
```
