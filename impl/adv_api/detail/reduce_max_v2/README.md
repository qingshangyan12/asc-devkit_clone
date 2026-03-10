# ReduceMax 算子实现文档

## 概述

本文档描述了 ReduceMax 算子的实现，该算子支持三种数据类型（`half`、`float`、`int32_t`）和两种归约模式（`AR`、`RA`）。

### 核心特性

✅ **多数据类型支持**
- `half`:   使用 `vcgmax` 指令（块内归约）
- `float`:   使用 `vcgmax` 指令（块内归约）
- `int32_t`: 使用 `vmax` 指令（二分归约）

✅ **多种归约模式**
- `Pattern::Reduce::AR` (All-to-Reduce): 在最后一维上归约
- `Pattern::Reduce::RA` (Reduce-to-All): 在第一维上归约

✅ **性能优化**
- `half/float`: 使用单条 `vcgmax` 指令完成块内归约
- `int32_t`: 使用二分归约策略，时间复杂度 O(log N)

---

## 文件结构

```
/home/zhangyike/asc-devkit/impl/adv_api/detail/reduce_max_v2/
├── reduce_max.h                 # 统一对外接口
├── reduce_max_impl.h            # 核心实现整合
├── reduce_max_v220_impl.h       # float/half 实现（迁移自 ge/compiler/.../api/）
├── reduce_max_vmax_impl.h       # int32_t 专用实现（新增）
├── reduce_max_common_util.h     # 通用工具函数
├── reduce.h                     # 通用归约函数
├── reduce_init.h                # 归约初始化
├── reduce_max_example.h         # 使用示例
├── reduce_max_vmax_example.h    # vmax 使用示例
└── README.md                    # 本文档
```

---

## 文件说明

### 1. `reduce_max.h` - 统一对外接口

**功能**: 提供统一的 `ReduceMax` 接口，根据数据类型自动选择实现

**核心函数**:
```cpp
template <typename T, typename pattern>
__aicore__ inline void ReduceMax(
    const LocalTensor<T>& dstTensor,
    const LocalTensor<T>& srcTensor,
    const LocalTensor<uint8_t>& tmpBuffer,
    const uint32_t srcShape[],
    bool isReuseSource = false)
```

**参数说明**:
- `dstTensor`: 目标张量，存储归约后的最大值
- `srcTensor`: 源张量，包含待归约的数据
- `tmpBuffer`: 临时缓冲区，用于中间计算
- `srcShape`: 源张量的形状信息 `[first, last]`
- `isReuseSource`: 是否原地操作（仅支持 RA 模式的 half/float）

**支持的数据类型**:
- `half`:   自动使用 `vcgmax` 指令
- `float`:   自动使用 `vcgmax` 指令
- `int32_t`: 自动使用 `vmax` 指令

---

### 2. `reduce_max_impl.h` - 核心实现整合

**功能**: 根据数据类型分发到不同的实现

**核心函数**:
```cpp
template <class T, class pattern, bool isReuseSource = false>
__aicore__ inline void ReduceMaxImpl(...)
```

**类型分发逻辑**:
```cpp
if constexpr (std::is_same_v<T, int32_t>) {
    // int32_t: 使用 vmax 实现
    ReduceMaxVMaxImpl<T>(...);
} else {
    // half/float: 使用 vcgmax 实现
    ReduceMaxV220Impl<T, pattern, isReuseSource>(...);
}
```

---

### 3. `reduce_max_v220_impl.h` - float/half 实现

**来源**: 从 `ge/compiler/graph/optimize/autofuse/ascendc/api/` 迁移

**功能**: 提供 `half` 和 `float` 数据类型的 ReduceMax 实现

**核心函数**:
```cpp
template <class T, class pattern, bool isReuseSource = false>
__aicore__ inline void ReduceMaxV220Impl(...)
```

**底层指令**: `vcgmax` (Vector Compare Greater Max)

**特点**:
- 单条指令完成块内归约
- 性能最优
- 仅支持 `half` 和 `float` 类型

---

### 4. `reduce_max_vmax_impl.h` - int32_t 专用实现

**功能**: 提供 `int32_t` 数据类型的 ReduceMax 实现

**核心函数**:
```cpp
template <typename T>
__aicore__ inline void ReduceMaxVMaxImpl(...)
```

**底层指令**: `vmax` (Vector Maximum)

**算法**: 二分归约（Binary Reduction）

**特点**:
- 支持任意长度数据
- 时间复杂度 O(log N)
- 处理非 2 的幂的剩余元素

---

### 5. `reduce_max_common_util.h` - 通用工具函数

**来源**: 从以下文件迁移并整合：
- `asc-devkit/impl/adv_api/detail/reduce/reduce_common_util_v220_impl.h`
- `asc-devkit/impl/adv_api/detail/reduce/reduce_common_util_impl.h`

**核心函数**:

| 函数名 | 功能 | 用途 |
|--------|------|------|
| `ApiMode` 枚举 | 归约模式枚举 | SUM/MIN/MAX/ANY/ALL |
| `FindClosestPowerOfTwo` | 查找最近的 2 的幂 | 二分归约辅助 |
| `ComputeMaskBit` | 计算 Mask 位图 | 控制 SIMD 指令 |
| `BlockReduceCompute` | 块级归约调度 | 根据 ApiMode 分发 |
| `WholeReduceCompute` | 整体归约调度 | 根据 ApiMode 分发 |
| `BlockReduceByLastAxis` | 最后一维归约（AR 模式） | 行级归约 |
| `BinaryReduceByFirstAxis` | 第一维归约（RA 模式） | 列级归约 |
| `DoReduceLessThanBlk` | 小于一个块的归约 | 小数据优化 |
| `DoReduceOneBlk` | 正好一个块的归约 | 特殊情况优化 |
| `DoReduceByBlk` | 多块归约 | 分块策略 |
| `DoReduceByRep` | 重复块归约 | 重复数据优化 |
| `DoLongLastReduce` | 长维度归约 | 大数据优化 |
| `GetReduceValOnRep` | 获取归约值 | 重复数据处理 |
| `AccValOnBlk` | 块内累加/比较 | 归约辅助 |
| `BlkReduceForLoop` | 块级归约循环 | 循环展开 |
| `ReduceParams` | 归约参数结构体 | 参数封装 |

---

### 6. `reduce_max_example.h` - 使用示例

**功能**: 提供完整的使用示例和 kernel 模板

**示例内容**:
- AR 模式示例（half/float/int32_t）
- RA 模式示例（half/float/int32_t）
- 完整的 kernel 示例

---

## 使用示例

### 示例 1: AR 模式 - half 类型

```cpp
#include "reduce_max.h"

// 输入: [32, 128] 张量
LocalTensor<half> srcTensor = AllocTensor<half>(32 * 128);
LocalTensor<half> dstTensor = AllocTensor<half>(32);
LocalTensor<uint8_t> tmpBuffer = AllocTensor<uint8_t>(1024);
uint32_t srcShape[2] = {32, 128};

// 执行 AR 模式归约
ReduceMax<half, Pattern::Reduce::AR>(
    dstTensor, srcTensor, tmpBuffer, srcShape, false);

// 结果: dstTensor[i] = max(srcTensor[i*128 : i*128+127])
```

---

### 示例 2: RA 模式 - float 类型

```cpp
#include "reduce_max.h"

// 输入: [16, 256] 张量
LocalTensor<float> srcTensor = AllocTensor<float>(16 * 256);
LocalTensor<float> dstTensor = AllocTensor<float>(256);
LocalTensor<uint8_t> tmpBuffer = AllocTensor<uint8_t>(2048);
uint32_t srcShape[2] = {16, 256};

// 执行 RA 模式归约
ReduceMax<float, Pattern::Reduce::RA>(
    dstTensor, srcTensor, tmpBuffer, srcShape, false);

// 结果: dstTensor[j] = max(srcTensor[j], srcTensor[j+256], ..., srcTensor[j+15*256])
```

---

### 示例 3: AR 模式 - int32_t 类型

```cpp
#include "reduce_max.h"

// 输入: [8, 64] 张量
LocalTensor<int32_t> srcTensor = AllocTensor<int32_t>(8 * 64);
LocalTensor<int32_t> dstTensor = AllocTensor<int32_t>(8);
LocalTensor<uint8_t> tmpBuffer = AllocTensor<uint8_t>(1024);
uint32_t srcShape[2] = {8, 64};

// 执行 AR 模式归约
ReduceMax<int32_t, Pattern::Reduce::AR>(
    dstTensor, srcTensor, tmpBuffer, srcShape, false);

// 结果: dstTensor[i] = max(srcTensor[i*64 : i*64+63])
```

---

### 示例 4: RA 模式 - int32_t 类型

```cpp
#include "reduce_max.h"

// 输入: [32, 128] 张量
LocalTensor<int32_t> srcTensor = AllocTensor<int32_t>(32 * 128);
LocalTensor<int32_t> dstTensor = AllocTensor<int32_t>(128);
LocalTensor<uint8_t> tmpBuffer = AllocTensor<uint8_t>(2048);
uint32_t srcShape[2] = {32, 128};

// 执行 RA 模式归约
ReduceMax<int32_t, Pattern::Reduce::RA>(
    dstTensor, srcTensor, tmpBuffer, srcShape, false);

// 结果: dstTensor[j] = max(srcTensor[j], srcTensor[j+128], ..., srcTensor[j+31*128])
```

---

## 归约模式详解

### AR 模式（All-to-Reduce）

**定义**: 在张量的最后一维上执行归约操作

**输入形状**: `[first, last]` → **输出形状**: `[first]`

**示例**:
```cpp
输入: [2, 4]
[[1, 5, 3, 8],   // 行 0
 [4, 2, 9, 6]]   // 行 1

输出: [2]
[8, 9]  // max(1,5,3,8)=8, max(4,2,9,6)=9
```

**归约方向**: `→`（在最后一维上）

**适用场景**:
- 批量数据的全局最大值池化
- 图像分类的 batch-level 归约
- 序列数据的每个时间步归约

---

### RA 模式（Reduce-to-All）

**定义**: 在张量的第一维上执行归约操作

**输入形状**: `[first, last]` → **输出形状**: `[last]`

**示例**:
```cpp
输入: [4, 2]
[[1, 5],   // 行 0
 [4, 2],   // 行 1
 [3, 9],   // 行 2
 [8, 6]]   // 行 3

输出: [2]
[8, 9]  // max(1,4,3,8)=8, max(5,2,9,6)=9
```

**归约方向**: `↓`（在第一维上，使用二分归约）

**适用场景**:
- 多层特征的全局最大值
- 深度网络的特征聚合
- 时间序列的多帧归约

---

## 算法对比

| 数据类型 | 指令 | 算法 | 时间复杂度 | 性能 |
|---------|------|------|-----------|------|
| half | vcgmax | 块内归约 | O(1) | ⭐⭐⭐⭐⭐ |
| float | vcgmax | 块内归约 | O(1) | ⭐⭐⭐⭐⭐ |
| int32_t | vmax | 二分归约 | O(log N) | ⭐⭐⭐⭐ |

**说明**:
- `vcgmax` 是单条指令完成块内归约，性能最优
- `vmax` 需要多条指令进行二分归约，但仍然非常高效

---

## 性能分析

### half/float（使用 vcgmax）

**优点**:
- 单条指令完成归约
- 硬件加速
- 无中间结果存储

**性能**: ⭐⭐⭐⭐⭐（最优）

---

### int32_t（使用 vmax）

**优点**:
- 支持任意长度
- 对数时间复杂度
- 处理非 2 的幂的剩余元素

**性能**: ⭐⭐⭐⭐（次优）

**优化策略**:
1. 二分归约：每次迭代数据量减半
2. 循环展开：减少循环开销
3. 流水线同步：合理使用 `PipeBarrier`

---

## 技术要点

### 1. 类型分发

使用 `if constexpr` 进行编译期类型检查：

```cpp
if constexpr (std::is_same_v<T, int32_t>) {
    // int32_t: 使用 vmax 实现
    ReduceMaxVMaxImpl<T>(...);
} else {
    // half/float: 使用 vcgmax 实现
    ReduceMaxV220Impl<T, ...>(...);
}
```

---

### 2. 二分归约（int32_t）

**原理**: 每次将数据对半比较，直到只剩 1 个元素

```
原始数据: [a0, a1, a2, a3, a4, a5, a6, a7]

第1轮: [max(a0,a4), max(a1,a5), max(a2,a6), max(a3,a7)]
第2轮: [max(第1轮前半), max(第1轮后半)]
第3轮: [max(all)]

输出: [max(all)]
```

---

### 3. 处理非 2 的幂的剩余元素

**问题**: 如果数据长度不是 2 的幂次，直接二分归约会丢失元素

**解决方案**: 将数据分为两部分
1. `2^k` 部分：使用二分归约
2. 剩余部分：单独处理，最后与主结果比较

```
原始数据: 13 个元素
2^k = 8, 剩余 = 5

步骤1: 对前8个元素二分归约 → max1
步骤2: 比较剩余5个元素 → max2
步骤3: max(max1, max2) → 最终结果
```

---

## 迁移记录

### 从 `ge/compiler/graph/optimize/autofuse/ascendc/api/` 复制的文件

| 原文件路径 | 目标文件 | 复制方式 |
|------------|-----------|---------|
| `reduce.h` | `reduce.h` | 完整复制 |
| `reduce_max.h` | `reduce_max.h` | 完整复制 |
| `reduce_max_impl.h` | `reduce_max_impl.h` | 完整复制 |
| `reduce_max_v220_impl.h` | `reduce_max_v220_impl.h` | 完整复制 |
| `reduce_max_vmax_impl.h` | `reduce_max_vmax_impl.h` | 完整复制 |
| `reduce_max_common_util.h` | `reduce_max_common_util.h` | 完整复制 |
| `reduce_init.h` | `reduce_init.h` | 完整复制 |
| `reduce_max_example.h` | `reduce_max_example.h` | 完整复制 |
| `reduce_max_vmax_example.h` | `reduce_max_vmax_example.h` | 完整复制 |

---

## 注意事项

1. **保留原始内容**
   - 所有代码文件均保留原始内容，未进行任何移除操作
   - 头文件包含路径保持原有结构

2. **保持函数名不变**
   - 复制的函数保持原有命名
   - 遵循现有命名规范

3. **添加清晰的注释**
   - 每个文件包含版权声明
   - 每个函数包含功能说明
   - 标注代码来源

---

## 总结

✅ **已完成的工作**:
1. 从源路径复制了 ReduceMax 相关的 9 个文件
2. 创建了目标文件夹 `reduce_max_v2`
3. 所有文件内容完整保留，未做任何移除

✅ **技术亮点**:
1. 类型安全：使用 `if constexpr` 进行编译期类型检查
2. 性能优化：`half/float` 使用单条 `vcgmax` 指令
3. 算法高效：`int32_t` 使用二分归约，时间复杂度 O(log N)

✅ **代码质量**:
1. 清晰的注释和文档
2. 完整的使用示例
3. 保留原始代码内容

---

**文档版本**: v1.0  
**创建日期**: 2026-03-10  
**来源路径**: `/home/zhangyike/ge/compiler/graph/optimize/autofuse/ascendc/api/`  
**目标路径**: `/home/zhangyike/asc-devkit/impl/adv_api/detail/reduce_max_v2/`
