# Basic API贡献指南

## 概述

Basic API（基础API）是Ascend C编程框架中的指令级API层，直接封装昇腾AI处理器的硬件指令，使用`LocalTensor`抽象提供类型安全的接口。Basic API是高阶API的构建基础，开发者可以通过组合Basic API实现复杂的算法逻辑。

**Basic API的核心特点：**

- **指令级封装**：每个API对应一条或多条硬件指令
- **LocalTensor抽象**：使用`LocalTensor<T>`类型操作内存
- **模板化设计**：支持多种数据类型（half, float, int16_t, int32_t等）
- **双层接口**：Level 0（精细控制）和 Level 2（简化调用）
- **架构适配**：通过宏定义支持不同NPU架构

---

## 开发流程

### 1. 需求分析

- 明确API功能（如Add、Mul、Relu等）
- 确定支持的数据类型
- 分析硬件指令支持情况

### 2. API设计

- 定义函数原型（使用LocalTensor）
- 设计Level 0和Level 2接口
- 确定参数规范（mask, repeat, stride等）

### 3. 实现开发

- 编写接口声明（include/basic_api/）
- 实现核心逻辑（impl/basic_api/）
- 处理架构差异

### 4. 测试验证

- 编写单元测试
- 验证功能正确性
- 检查边界条件

### 5. 文档编写

- 完善API文档
- 提供使用示例
- 说明约束条件

---

## API介绍

### Level 0 vs Level 2接口

#### Level 0（精细控制）

```cpp
// 需要手动设置mask和repeat参数
template <typename T, bool isSetMask = true>
__aicore__ inline void Add(const LocalTensor<T>& dst,
                           const LocalTensor<T>& src0,
                           const LocalTensor<T>& src1,
                           uint64_t mask[],           // mask数组
                           const uint8_t repeatTime,  // repeat次数
                           const BinaryRepeatParams& repeatParams);  // stride参数
```

**适用场景**：

- 需要精细控制计算过程
- 非连续内存访问
- 性能优化

#### Level 2（简化调用）

```cpp
// 自动处理mask和repeat
template <typename T>
__aicore__ inline void Add(const LocalTensor<T>& dst,
                           const LocalTensor<T>& src0,
                           const LocalTensor<T>& src1,
                           const int32_t& count);  // 仅需元素个数
```

**适用场景**：

- 连续内存块计算
- 简化代码
- 快速开发

---

## 目录规划

### 目录结构

```text
asc-devkit-dev/
├── include/
│   └── basic_api/                    # Basic API头文件
│       ├── kernel_operator_common_intf.h        # 通用接口
│       ├── kernel_operator_vec_binary_intf.h    # 矢量二元运算
│       ├── kernel_operator_vec_unary_intf.h     # 矢量一元运算
│       ├── kernel_operator_data_copy_intf.h     # 数据搬运
│       ├── kernel_operator_fixpipe_intf.h       # Fixpipe
│       ├── kernel_operator_mm_intf.h            # 矩阵乘法
│       ├── kernel_operator_scalar_intf.h        # 标量运算
│       ├── kernel_operator_sys_var_intf.h       # 系统变量
│       ├── kernel_operator_atomic_intf.h        # 原子操作
│       ├── kernel_tensor.h                      # Tensor定义
│       └── kernel_struct_*.h                    # 参数结构体
│
├── impl/
│   └── basic_api/                    # Basic API实现
│       ├── dav_c100/                 # DAV-C100架构
│       │   ├── kernel_operator_vec_binary_impl.h
│       │   ├── kernel_operator_vec_unary_impl.h
│       │   └── ...
│       ├── dav_c220/                 # DAV-C220架构
│       │   ├── kernel_operator_vec_binary_impl.h
│       │   └── ...
│       └── CMakeLists.txt
│
├── tests/
│   └── api/
│       └── basic_api/                # Basic API测试
│           ├── tikcpp_case_common/   # 通用测试
│           │   └── test_operator_axpy.cpp
│           ├── tikcpp_case_ascend610/    # Atlas 610
│           │   ├── test_operator_vec_binary.cpp
│           │   └── test_operator_vec_unary.cpp
│           ├── tikcpp_case_ascend310p/   # Atlas 310P
│           │   └── ...
│           └── tikcpp_case_ascend910/    # Atlas 910
│               └── ...
│
└── docs/
    └── api/
        └── context/
            └── ...              # Basic API文档
```

### 文件命名规范

| 文件类型 | 命名规范 | 示例 |
| --------- | --------- | ------ |
| 接口头文件 | `kernel_operator_<category>_intf.h` | `kernel_operator_vec_binary_intf.h` |
| 实现文件 | `kernel_operator_<category>_impl.h` | `kernel_operator_vec_binary_impl.h` |
| 测试文件 | `test_operator_<category>.cpp` | `test_operator_vec_binary.cpp` |
| 文档文件 | `<api>.md` | `Add.md` |

### API分类

| 分类 | 说明 | 示例API |
| ------ | ------ | --------- |
| vec_binary | 矢量二元运算 | Add, Sub, Mul, Div, Max, Min |
| vec_unary | 矢量一元运算 | Relu, Exp, Cast, Abs |
| vec_reduce | 矢量归约 | Sum, Max, Mean |
| data_copy | 数据搬运 | DataCopy, LoadData |
| fixpipe | 流水线控制 | Fixpipe |
| mm | 矩阵乘法 | Mmad, Conv2D |
| scalar | 标量运算 | ToFloat |
| atomic | 原子操作 | AtomicAdd, AtomicCAS |

---

## 架构设计

### 实现层次

#### 层次1：接口声明层（include/basic_api/）

```cpp
// include/basic_api/kernel_operator_vec_binary_intf.h
#ifndef ASCENDC_MODULE_OPERATOR_VEC_BINARY_INTERFACE_H
#define ASCENDC_MODULE_OPERATOR_VEC_BINARY_INTERFACE_H

#include "kernel_tensor.h"
#include "kernel_struct_binary.h"

namespace AscendC {

// Add - Level 0
template <typename T, bool isSetMask = true>
__aicore__ inline void Add(const LocalTensor<T>& dst,
                           const LocalTensor<T>& src0,
                           const LocalTensor<T>& src1,
                           uint64_t mask[],
                           const uint8_t repeatTime,
                           const BinaryRepeatParams& repeatParams);

// Add - Level 2
template <typename T>
__aicore__ inline void Add(const LocalTensor<T>& dst,
                           const LocalTensor<T>& src0,
                           const LocalTensor<T>& src1,
                           const int32_t& count);

}  // namespace AscendC

#include "../../impl/basic_api/kernel_operator_vec_binary_intf_impl.h"
#endif
```

#### 层次2：指令实现层（impl/basic_api/）

```cpp
// impl/basic_api/dav_c220/kernel_operator_vec_binary_impl.h
#ifndef ASCENDC_MODULE_OPERATOR_VEC_BINARY_IMPL_H
#define ASCENDC_MODULE_OPERATOR_VEC_BINARY_IMPL_H

namespace AscendC {

// Add实现 - Level 2
template <typename T>
__aicore__ inline void AddImpl(__ubuf__ T* dst, __ubuf__ T* src0,
                               __ubuf__ T* src1, const int32_t& count) {
    if ASCEND_IS_AIV {
        // 1. 设置mask
        set_mask_count();
        set_vector_mask(0, count);

        // 2. 调用底层指令
        vadd(dst, src0, src1, 1,
             DEFAULT_BLK_STRIDE, DEFAULT_BLK_STRIDE, DEFAULT_BLK_STRIDE,
             DEFAULT_REPEAT_STRIDE, DEFAULT_REPEAT_STRIDE, DEFAULT_REPEAT_STRIDE);

        // 3. 恢复mask
        set_mask_norm();
        set_vector_mask(static_cast<uint64_t>(-1), static_cast<uint64_t>(-1));
    }
}

// Add实现 - Level 0
template <typename T, bool isSetMask = true>
__aicore__ inline void AddImpl(__ubuf__ T* dst, __ubuf__ T* src0,
                               __ubuf__ T* src1, const uint64_t mask[],
                               const uint8_t repeatTime,
                               const BinaryRepeatParams& repeatParams) {
    if ASCEND_IS_AIV {
        // 设置mask（如果需要）
        if (isSetMask) {
            AscendCUtils::SetMask<T, isSetMask>(mask[1], mask[0]);
        }

        // 调用底层指令
        vadd(dst, src0, src1, repeatTime,
             repeatParams.dstBlkStride, repeatParams.src0BlkStride, repeatParams.src1BlkStride,
             repeatParams.dstRepStride, repeatParams.src0RepStride, repeatParams.src1RepStride);
    }
}

}  // namespace AscendC

#endif
```

#### 层次3：接口封装层

```cpp
// impl/basic_api/kernel_operator_vec_binary_intf_impl.h
namespace AscendC {

// Level 2接口封装
template <typename T>
__aicore__ inline void Add(const LocalTensor<T>& dst,
                           const LocalTensor<T>& src0,
                           const LocalTensor<T>& src1,
                           const int32_t& count) {
    AddImpl<T>(dst.GetPtr(), src0.GetPtr(), src1.GetPtr(), count);
}

// Level 0接口封装
template <typename T, bool isSetMask = true>
__aicore__ inline void Add(const LocalTensor<T>& dst,
                           const LocalTensor<T>& src0,
                           const LocalTensor<T>& src1,
                           uint64_t mask[],
                           const uint8_t repeatTime,
                           const BinaryRepeatParams& repeatParams) {
    AddImpl<T, isSetMask>(dst.GetPtr(), src0.GetPtr(), src1.GetPtr(),
                          mask, repeatTime, repeatParams);
}

}  // namespace AscendC
```

### 架构适配

不同NPU架构的实现可能有差异，需要重新实现

---

## 开发示例：实现Axpy Basic API

### 需求分析

实现矢量乘加：`dst = src * scalar + dst`

- 支持数据类型：half, float
- 接口类型：Level 2（简化调用）
- 硬件支持：确认支持硬件

### 1. 查看现有API结构

Basic API使用`LocalTensor<T>`作为参数，Level 2接口仅需count参数：

```cpp
// 参考现有Add接口
template <typename T>
__aicore__ inline void Add(const LocalTensor<T>& dst,
                           const LocalTensor<T>& src0,
                           const LocalTensor<T>& src1,
                           const int32_t& count);
```

### 2. 接口设计

在 `include/basic_api/kernel_operator_vec_binary_intf.h` 中添加：

```cpp
/* **************************************************************************************************
 * Axpy                                             *
 * ************************************************************************************************* */
/*
 * @ingroup Axpy Level 2
 * @brief dst = dst + src * scalar
 * @param [out] dst output LocalTensor
 * @param [in] src input LocalTensor
 * @param [in] scalar scalar value
 * @param [in] count number Number of data involved in calculation
 */
template <typename T, typename U>
__aicore__ inline void Axpy(const LocalTensor<T>& dst,
                           const LocalTensor<U>& src,
                           const U scalar,
                           const int32_t& count);
```

### 3. 实现代码

参考其他接口实现

### 4. 接口封装

在 `impl/basic_api/kernel_operator_vec_binary_intf_impl.h` 中添加：

```cpp
template <typename T, typename U>
__aicore__ inline void Axpy(const LocalTensor<T>& dst,
                           const LocalTensor<U>& src,
                           const U scalar,
                           const int32_t& count) {
    AxpyImpl<T, U>(dst.GetPtr(), src.GetPtr(), scalar, count);
}
```

### 5. 测试代码

增加对应接口的测试代码

---

## 测试验证要求

### 测试类型

#### 1. 功能测试

验证API计算结果正确性

#### 2. 边界测试

```cpp
TEST_F(TestAxpy, BoundaryTest) {
    // 测试count=0, 1, 256, 257等边界值
    // 测试不同数据类型组合
    // 测试特殊值（NaN, Inf）
}
```

#### 3. 数据类型测试

```cpp
INSTANTIATE_TEST_CASE_P(TEST_AXPY_TYPES, AxpyTestsuite,
    ::testing::Values(
        BinaryTestParams { 256, 2, 2, main_axpy<half, half> },
        BinaryTestParams { 256, 4, 2, main_axpy<float, half> },
        BinaryTestParams { 256, 4, 4, main_axpy<float, float> }
    )
);
```

---

## 代码规范

### 命名规范

```cpp
// 函数名：大驼峰，首字母大写
void Add(...);
void Relu(...);
void Axpy(...);

// 参数名：小驼峰
LocalTensor<T> dstTensor;
int32_t elementCount;

// 宏定义：全大写+下划线
#define ASCENDC_ASSERT(cond, msg) ...

// 类型名：大驼峰
struct BinaryRepeatParams;
class LocalTensor;
```

### 代码风格

```cpp
// 1. 缩进：4空格
// 2. 大括号：K&R风格
// 3. 空格：操作符前后空格
// 4. 注释：Doxygen风格

/**
 * @brief 矢量加法运算
 * @param dst 目标LocalTensor
 * @param src0 源LocalTensor 0
 * @param src1 源LocalTensor 1
 * @param count 元素个数
 */
template <typename T>
__aicore__ inline void Add(const LocalTensor<T>& dst,
                           const LocalTensor<T>& src0,
                           const LocalTensor<T>& src1,
                           const int32_t& count) {
    // 参数验证
    ASCENDC_ASSERT(count > 0, "count must be positive");

    // 调用实现
    AddImpl<T>(dst.GetPtr(), src0.GetPtr(), src1.GetPtr(), count);
}
```

### 错误处理

```cpp
// 1. 参数验证（Debug模式）
ASCENDC_ASSERT(count > 0, "count must be greater than 0");
ASCENDC_ASSERT(dst != nullptr, "dst cannot be nullptr");
ASCENDC_ASSERT(src != nullptr, "src cannot be nullptr");

// 2. 类型检查
static_assert(SupportType<T, half, float, int16_t, int32_t>(),
              "Unsupported data type");

// 3. 架构检查
#if !defined(__NPU_ARCH__) || (__NPU_ARCH__ != 2201 && __NPU_ARCH__ != 3101)
    #error "Unsupported NPU architecture"
#endif
```

---
