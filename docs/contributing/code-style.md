# C++ 代码规范

## 1. 命名规范

### 1.1 文件命名

所有文件使用 **snake_case**（小写字母 + 下划线）。

| 后缀 | 用途 | 示例 |
|------|------|------|
| `_intf.h` | 接口声明 | `kernel_operator_vec_binary_intf.h` |
| `_impl.h` | 实现定义 | `kernel_operator_vec_binary_intf_impl.h` |
| `_intf_impl.h` | 接口 + 架构分发实现 | `kernel_operator_common_intf_impl.h` |
| `_tiling.h` | Tiling 接口声明 | `axpy_tiling.h` |
| `_tiling_impl.cpp` | Tiling 实现 | `axpy_tiling_impl.cpp` |
| `_struct_*.h` | 类型/结构体定义 | `kernel_struct_binary.h` |
| `_common_impl.h` | 通用算法实现 | `exp_common_impl.h` |
| `_check.h` | 参数校验 | `exp_check_common.h` |

### 1.2 函数命名

**基础 API（`basic_api/`）：** 大驼峰（PascalCase）

```
Add, Sub, Mul, DataCopy, SetVectorMask, PipeBarrier
```

**C API（`c_api/`）：** 小写字母 + 下划线，`asc_` 前缀

```
asc_add, asc_axpy, asc_axpy_sync
```

**高阶 API（`adv_api/`）：** 大驼峰（PascalCase）

```
Axpy, Exp, LayerNorm, GroupedMatmul
```

同步版本加 `_sync` 后缀：

```
asc_add_sync, AddSync
```

### 1.3 宏命名

全大写字母 + 下划线。

| 类型 | 前缀 | 示例 |
|------|------|------|
| Include guard | `ASCENDC_` | `ASCENDC_KERNEL_COMMON_H` |
| 内部哨兵 | `__ASCENDC_INCLUDE_INTERNAL_HEADERS__` | 仅限 guard 体系使用 |
| 通用功能宏 | `ASCENDC_` | `ASCENDC_ASSERT`、`ASCENDC_CHECK_VALUE_RANGE` |
| 日志宏 | `KERNEL_LOG_` | `KERNEL_LOG_KERNEL_ERROR` |
| 常量/枚举宏 | 全大写 | `DT_FLOAT`、`QUE_MAX_DEPTH`、`FORMAT_NCHW` |

**规则：**
- 禁止自定义 `__` 双下划线前缀（C++ 标准保留）
- `__ASCENDC_INCLUDE_INTERNAL_HEADERS__` 和 `__UNDEF_*` 是唯一的例外

### 1.4 类型/结构体命名

大驼峰（PascalCase）：

```
LocalTensor, GlobalTensor, TPipe, TQue, FixpipeConfig, DivConfig
```

### 1.5 变量命名

小驼峰（camelCase）：

```
dstTensor, srcTensor, calCount, stackSize, sharedTmpBuffer, repeatTime
```

全局配置常量用小写 `k` 前缀或全大写：

```
kDefaultNdDmaConfig, DEFAULT_BINARY_CONFIG, QUE_MAX_DEPTH
```

### 1.6 模板参数命名

单字母大写或 CamelCase：

```
T, U, isReuseSource, taylorExpandLevel, reduceDim, engine
```

---

## 2. Header Guard 规范

### 2.1 公开头文件（三层 guard 体系）

每个公开头文件必须有如下三层保护：

```cpp
// Layer 1: 内部头文件哨兵（防止递归 include）
#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_<FILENAME_UPPER>__

// Layer 2: 传统 include guard
#ifndef ASCENDC_<MODULE>_H
#define ASCENDC_<MODULE>_H

// ... 文件内容 ...

#endif  // ASCENDC_<MODULE>_H

// Layer 3: 哨兵清理
#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_<FILENAME_UPPER>__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_<FILENAME_UPPER>__
#endif
#endif
```

**Guard 宏命名公式：**

```
文件名: kernel_common.h
  → Layer 2 guard: ASCENDC_KERNEL_COMMON_H
  → Layer 1 undef: __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_COMMON_H__

文件名: kernel_operator_vec_binary_intf.h
  → Layer 2 guard: ASCENDC_MODULE_OPERATOR_VEC_BINARY_INTERFACE_H
  → Layer 1 undef: __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_VEC_BINARY_INTF_H__
```

**命名规则：** `ASCENDC_` + 模块路径（斜杠变下划线，全大写） + `_H`

### 2.2 实现头文件（额外 `#pragma message` 警告）

包含 `impl/` 中的实现在公开头文件底部时，实现头文件本身应添加：

```cpp
#pragma message("impl/basic_api/kernel_operator_vec_binary_intf_impl.h "
    "is an internal header file and must not be used directly. "
    "Functions or variables defined in this file may be removed in the future. "
    "Please use \"#include \\\"basic_api/kernel_operator_vec_binary_intf.h\\\"\" "
    "and use public functions or variables defined in interface headers files.")
```

### 2.3 C API 头文件

C API 使用独立的哨兵命名空间：

```cpp
#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_C_API_H
#endif
```

### 2.4 禁止 `#pragma once`

本项目不使用 `#pragma once`。所有头文件使用 `#ifndef` / `#define` / `#endif` 风格。

---

## 3. Include 规范

### 3.1 引号 vs 尖括号

| 符号 | 用途 | 示例 |
|------|------|------|
| `""` | 项目内部头文件 | `#include "kernel_tensor.h"` |
| `<>` | 标准库头文件（仅限 `ASCENDC_CPU_DEBUG`） | `#include <cstdint>` |

### 3.2 Include 路径的四种模式

| 模式 | 示例 | 适用场景 |
|------|------|---------|
| 裸文件名 | `#include "kernel_tensor.h"` | 同目录或依赖的头文件 |
| 子目录路径 | `#include "reg_compute/kernel_reg_compute_utils.h"` | 同模块不同子目录 |
| `include/` 前缀绝对路径 | `#include "include/adv_api/kernel_api.h"` | 跨模块引用 |
| `../../impl/` 相对回溯 | `#include "../../impl/basic_api/kernel_operator_vec_binary_intf_impl.h"` | 接口头引用实现头 |

### 3.3 Include 顺序

按以下顺序组织 include，每组之间用空行分隔：

1. **项目核心依赖**
   ```cpp
   #include "kernel_macros.h"
   #include "kernel_tensor.h"
   ```

2. **同模块结构/类型头**
   ```cpp
   #include "kernel_struct_binary.h"
   #include "kernel_struct_unary.h"
   ```

3. **架构条件 include**
   ```cpp
   #if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510 || __NPU_ARCH__ == 5102)
   #include "reg_compute/kernel_reg_compute_utils.h"
   #endif
   ```

4. **CPU debug 条件 include（标准库）**
   ```cpp
   #if defined(ASCENDC_CPU_DEBUG) && ASCENDC_CPU_DEBUG == 1
   #include <cstdint>
   #include "stub_def.h"
   #endif
   ```

5. **Impl 头引用（置底，在 Layer 2 guard 的 `#endif` 之前）**
   ```cpp
   #include "../../impl/basic_api/kernel_operator_vec_binary_intf_impl.h"

   #endif  // ASCENDC_<MODULE>_H
   ```
   即 impl include 位于三层 guard 体系中 Layer 2 传统 guard 的闭合 `#endif` 紧上方，在 Layer 3 哨兵清理逻辑之前。

### 3.4 公共入口头文件

高阶 API 统一入口文件必须包含 `__NPU_ARCH__` 守卫：

```cpp
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 2201 || __NPU_ARCH__ == 3510)
#include "math/axpy.h"
#endif
```

---

## 4. Namespace 规范

### 4.1 主命名空间

所有公开 API 在 `namespace AscendC { }` 下。

```cpp
namespace AscendC {
__aicore__ inline void Add(...);
}  // namespace AscendC
```

### 4.2 子命名空间

| 子命名空间 | 用途 |
|-----------|------|
| `AscendC::Std` | 标准库兼容层 |
| `AscendC::Simt` | SIMT 编程模型 |
| `AscendC::Te` | Tensor Engine |
| `AscendC::Reg` | Register Compute |

### 4.3 命名空间闭合注释

闭合大括号后必须添加注释：

```cpp
}  // namespace AscendC
} // namespace AscendC::Std
```

### 4.4 禁止 `using namespace`

头文件中禁止使用 `using namespace`。

### 4.5 `#pragma begin_pipe` / `#pragma end_pipe`

流水线函数区块用以下 pragma 环绕：

```cpp
#pragma begin_pipe(V)
namespace AscendC {
    // vector pipeline functions...
}  // namespace AscendC
#pragma end_pipe
```

---

## 5. 宏规范

### 5.1 函数式宏

使用 `do { ... } while (0)` 包裹多语句宏：

```cpp
#define ASCENDC_ASSERT(cond, behavior)                          \
    do {                                                        \
        if (!(cond)) {                                          \
            behavior;                                           \
            AscendC::KernelRaise::GetInstance().Raise(SIGABRT); \
        }                                                       \
    } while (0)
```

### 5.2 日志宏

```cpp
#define KERNEL_LOG(level, format, ...) KERNEL_LOG_##level(format, ##__VA_ARGS__)
```

日志级别：`KERNEL_DEBUG`、`KERNEL_INFO`、`KERNEL_WARN`、`KERNEL_ERROR`

### 5.3 断言与校验宏

| 宏 | 用途 |
|----|------|
| `ASCENDC_ASSERT(cond, behavior)` | 断言失败时执行 behavior 并 abort |
| `ASCENDC_ASSERT_WARNING(cond, behavior)` | 断言失败时仅执行 behavior，不 abort |
| `ASCENDC_CHECK_VALUE_RANGE(v, lo, hi, name, msg)` | 参数值范围校验 |
| `ASCENDC_CHECK_TENSOR_PTR_ALIGN(ptr, pos, align, name, msg)` | 地址对齐校验 |
| `ASCENDC_REPORT_NOT_SUPPORT(cond, msg)` | 不支持条件报告 |
| `ASCENDC_REPORT_OVERFLOW_MEM(cond)` | 内存溢出报告 |

### 5.4 架构条件宏

```cpp
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510 || __NPU_ARCH__ == 5102)
    // 架构特定代码
#endif

#if defined(ASCENDC_CPU_DEBUG) && ASCENDC_CPU_DEBUG == 1
    // CPU 调试代码
#endif

#if (defined(UT_TEST) || defined(ST_TEST))
    // 测试代码
#endif
```

### 5.5 宏命名前缀约定

- `ASCENDC_`：所有功能宏的统一前缀
- `KERNEL_LOG_`：日志宏前缀
- `__ASCENDC_INCLUDE_INTERNAL_HEADERS__`：仅此哨兵宏使用 `__` 前缀

---

## 6. constexpr 与编译期编程

### 6.1 全局配置常量

```cpp
constexpr FixpipeConfig CFG_NZ_FIX = {CO2Layout::NZ, false, true};
constexpr FixpipeConfig CFG_NZ = {CO2Layout::NZ, false};
constexpr DivConfig DEFAULT_DIV_CONFIG = {DivAlgo::INTRINSIC};
constexpr BinaryConfig DEFAULT_BINARY_CONFIG = {1};
```

### 6.2 成员函数上的 constexpr + `__aicore__`

`constexpr` 在 `__aicore__` 之前（推荐的统一写法）：

```cpp
constexpr __aicore__ static inline T Max()
constexpr __aicore__ static inline T Min()
constexpr __aicore__ inline int16_t GetDataBlockSizeInBytes()
```

> **注意：** 少数遗留代码中可能存在 `__aicore__` 在 `constexpr` 之前的写法，新代码应统一将 `constexpr` 放在最前面。

### 6.3 `if constexpr` 编译期分支

```cpp
if constexpr (sizeof(U) == sizeof(float)) {
    Axpy<T, U>(dstTensor, srcTensor, scalarValue, calCount);
} else {
    AxpySub<T, U, isReuseSource>(dstTensor, srcTensor, scalarValue, sharedTmpBuffer, calCount);
}
```

```cpp
if constexpr (AscendC::Std::is_same<T, uint8_t>::value) {
    // ...
} else if constexpr (AscendC::Std::is_same<T, float>::value) {
    // ...
}
```

### 6.4 `static_assert` 类型约束

```cpp
static_assert(SupportType<T, uint8_t, int8_t, uint16_t, int16_t, half, float>(),
    "current data type is not supported on current device!");
```

### 6.5 `static constexpr` 类成员

```cpp
static constexpr TQueConfig config = GetTQueConfig(mask);
static constexpr TPosition srcPosition = src;
static constexpr Hardware srcHardType = GetPhyType(src);
```

---

## 7. 错误处理规范

### 7.1 `ASCENDC_ASSERT`

函数开头进行参数校验：

```cpp
__aicore__ inline void Add(const LocalTensor<T>& dst, const LocalTensor<T>& src0,
    const LocalTensor<T>& src1, ...)
{
    ASCENDC_ASSERT((dst.GetSize() == src0.GetSize()), {
        KERNEL_LOG(KERNEL_ERROR, "Tensor size mismatch: dst=%u, src0=%u",
            dst.GetSize(), src0.GetSize());
    });
    // ...
}
```

### 7.2 `CHECK_FUNC_HIGHLEVEL_API`

高阶 API 实现函数中，在函数体首行调用：

```cpp
template <typename T, typename U, bool isReuseSource>
__aicore__ inline void AxpyImpl(...)
{
    CHECK_FUNC_HIGHLEVEL_API(
        Axpy, (T, U, isReuseSource), (dstTensor, srcTensor, scalarValue, sharedTmpBuffer, calCount));
    // ...
}
```

### 7.3 `KERNEL_LOG` 日志级别选择

| 级别 | 使用场景 |
|------|---------|
| `KERNEL_DEBUG` | 开发调试信息 |
| `KERNEL_INFO` | 常规运行时信息 |
| `KERNEL_WARN` | 潜在问题警告 |
| `KERNEL_ERROR` | 错误条件 |

### 7.4 `#pragma message` 弃用警告

实现在 impl 头中禁止直接包含的警告：

```cpp
#pragma message("impl/xxx.h is an internal header file and must not be used directly. Please use include/xxx.h.")
```

---

## 8. 格式化

### 8.1 clang-format 配置

项目根目录 `.clang-format` 配置了统一的代码风格。关键配置项：

| 配置项 | 值 | 说明 |
|--------|-----|------|
| BasedOnStyle | Google | 基于 Google C++ Style |
| ColumnLimit | 120 | 每行最多 120 字符 |
| IndentWidth | 4 | 缩进 4 空格 |
| UseTab | Never | 不使用 Tab |
| PointerAlignment | Left | 指针符号靠左 |
| BreakBeforeBraces: AfterFunction | true | 函数大括号换行 |
| BreakBeforeBraces: AfterControlStatement | false | 控制流大括号不换行 |
| SortIncludes | false | 不自动排序 include |
| AccessModifierOffset | -4 | 访问修饰符缩进 |
| AlwaysBreakTemplateDeclarations | true | 模板声明每个参数一行 |

### 8.2 自动格式化

pre-commit hook 会自动运行 clang-format：

```yaml
# .pre-commit-config.yaml
- repo: https://github.com/pre-commit/mirrors-clang-format
  rev: v16.0.0
  hooks:
    - id: clang-format
      types_or: [c++, c]
```

提交前确保格式化通过。也可以手动运行：

```bash
clang-format -i --style=file <file>
```

---

## 9. 代码注释规范

### 9.1 注释风格总览

项目使用三种注释风格，适用于不同 API 类型：

| 风格 | 前缀符号 | 适用场景 | 示例位置 |
|------|---------|---------|---------|
| `\brief` / `\param` / `\note` | 反斜杠 `\` | 高阶 API 公开头文件 | `include/adv_api/math/sin.h` |
| `@ingroup` / `@brief` | @ 符号 | 基础 API 公开头文件 | `include/basic_api/kernel_operator_vec_binary_intf.h` |
| `ingroup` / `brief` | 无前缀 | impl 内部头文件 | `include/basic_api/kernel_operator_common_intf.h` |

### 9.2 文件头注释

每个公开头文件必须以 `\file` 块开头：

```cpp
/*!
 * \file kernel_common.h
 * \brief
 */
```

- `\file` 后填文件名
- `\brief` 可填空（基础 API）或写一行功能描述（高阶 API）

高阶 API 可包含公式描述：

```cpp
/*!
 * \file sin.h
 * \brief Defines a series of interface used to do elementwise math sin calculation.
 * Formula: sin(x) = (-1)^k*sin(x0), sin(x) = xP(x)
 */
```

### 9.3 函数/类注释

**高阶 API 风格（`\brief` / `\param` / `\note`）**

```cpp
/*!
 * \ingroup Sin
 * \brief compute Sin elementwisely
 *
 * \tparam T: half/float
 * \tparam isReuseSource: whether allows API to modify source data during calculation
 *
 * \param [out] dstTensor: output LocalTensor
 * \param [in] srcTensor: input LocalTensor
 * \param [in] sharedTmpBuffer: extra temporary shared space for intermediate
 * \param [in] calCount: number of elements to be calculated
 *
 * \note src/dst Tensor must be 32B aligned
 * \note The calculation may allocate temporary space through sharedTmpBuffer
 */
```

**要素：**
- `\ingroup` + 功能组名（首行）
- `\brief` + 功能简述
- `\tparam` + 模板参数说明（每个参数一行）
- `\param [out]` / `\param [in]` + 参数说明
- `\note` + 约束和注意事项

**基础 API 风格（`@ingroup` / `@brief` + 分隔线）**

```cpp
/* **************************************************************************************************
 * Add                                             *
 * ************************************************************************************************* */

/*
 * @ingroup Add Level 0
 * @brief dst = src0 + src1
 *
 * @param [out] dst output LocalTensor
 * @param [in] src0 input LocalTensor
 * @param [in] src1 input LocalTensor
 * @param [in] mask[]/mask mask array/count
 * @param [in] repeatTime repeat times
 * @param [in] repeatParams.dstBlkStride dst block stride
 * @param [in] repeatParams.src0BlkStride src0 block stride
 * @param [in] repeatParams.src1BlkStride src1 block stride
 * @param [in] repeatParams.dstRepStride dst repeat stride
 * @param [in] repeatParams.src0RepStride src0 repeat stride
 * @param [in] repeatParams.src1RepStride src1 repeat stride
 */
```

**要素：**
- `*` 分隔线，中间写函数名
- `@ingroup` + 分组 + Level（首行）
- `@brief` + 公式
- `@param [out]` / `@param [in]` + 参数说明
- 嵌套参数用 `.` 分隔（如 `repeatParams.dstBlkStride`）

**C API 风格**

```cpp
// ==========asc_add(half/float)==========
// 连续模式
__aicore__ inline void asc_add(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count);

// 配置模式
__aicore__ inline void asc_add(__ubuf__ half* dst, ...);
```

### 9.4 参数标注约定

**方向标注**

| 标注 | 含义 |
|------|------|
| `\param [out]` | 输出参数 |
| `\param [in]` | 输入参数 |
| `\param [in/out]` | 输入输出参数 |

**模板参数**

```
\tparam T: target operand data type, supports half/float
\tparam U: source operand data type, supports half/float
\tparam isReuseSource: whether allows API to modify source data during calculation
```

**参数描述格式**
- 参数名与实际形参名一致
- 描述简洁，说明类型约束和取值范围
- 统一使用英文或中文，一个文件内保持一致

### 9.5 特殊标注

**`\note` 约束说明**

```cpp
/*!
 * \note src/dst Tensor must be 32B aligned.
 * \note CalCount must be less than or equal to the element count of src/dst Tensor.
 * \note support data type: half and float
 */
```

用于：对齐要求、数据范围限制、支持的数据类型、性能注意事项

**弃用标注**

方式一 `[[deprecated]]` 属性：

```cpp
[[deprecated("NOTICE: SetDumpWorkSpacePtr has been deprecated and will be removed "
             "in the next version. Please do not use it!")]]
__aicore__ inline __gm__ uint8_t* SetDumpWorkSpacePtr(__gm__ uint8_t* workspace)
```

方式二 行注释：

```cpp
// NOTICE: InitOutput has been deprecated and will be removed in the next version.
// Please use Fill instead!
```

**功能分组线（基础 API）**

```cpp
/* **************************************************************************************************
 * Add                                             *
 * ************************************************************************************************* */
```

- 80 列宽，中间写函数名
- 用于在大型接口文件中划分功能区块

### 9.6 中英文约定

| 内容类型 | 语言 | 示例 |
|---------|------|------|
| 公开 API 注释（`\brief`、`\param`） | 英文 | `\brief compute Sin elementwisely` |
| impl 内部注释 | 中文 | `// 计算循环次数` |
| 日志消息 | 英文 | `"Tensor size mismatch"` |
| 断言消息 | 英文 | `"current data type is not supported"` |
| 文件/函数/变量名 | 英文 | `kernel_operator_vec_binary_intf.h` |
| 文档描述（.md） | 中文 | 本文档介绍... |

### 9.7 头文件级别约束

以下约束适用于所有新增头文件，不仅仅是废弃标记。

**`#pragma message` 内部头文件自标识**

所有 `impl/` 下的实现头文件必须添加 `#pragma message` 警告，告知用户此为内部文件不可直接包含：

```cpp
#pragma message("impl/xxx.h is an internal header file and must not be used directly. "
    "Please use \"#include \\\"basic_api/xxx.h\\\"\" "
    "and use public functions or variables defined in interface headers files.")
```

后续新增的头文件级别约束将在此处统一描述。
