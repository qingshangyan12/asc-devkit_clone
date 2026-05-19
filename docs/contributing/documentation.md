# 文档与注释规范

> 本文档聚焦 AscendC 代码注释约定（Doxygen 风格、参数标注、弃用标记等）。通用 Markdown 写作规范（标题、列表、图片、表格、代码块、标点等）请参考 [CANN 文档写作规范](https://gitcode.com/cann/community/blob/master/contributor/docs/document_writing_specs.md)。

## 1. 注释风格总览

项目使用三种注释风格，适用于不同 API 类型：

| 风格 | 前缀符号 | 适用场景 | 示例位置 |
|------|---------|---------|---------|
| `\brief` / `\param` / `\note` | 反斜杠 `\` | 高阶 API 公开头文件 | `include/adv_api/math/sin.h` |
| `@ingroup` / `@brief` | @ 符号 | 基础 API 公开头文件 | `include/basic_api/kernel_operator_vec_binary_intf.h` |
| `ingroup` / `brief` | 无前缀 | impl 内部头文件 | `include/basic_api/kernel_operator_common_intf.h` |

---

## 2. 文件头注释

### 2.1 所有公开头文件

每个公开头文件必须以 `\file` 块开头：

```cpp
/*!
 * \file kernel_common.h
 * \brief
 */
```

- `\file` 后填文件名
- `\brief` 可填空（基础 API）或写一行功能描述（高阶 API）

### 2.2 高阶 API 文件头（含公式描述）

```cpp
/*!
 * \file sin.h
 * \brief Defines a series of interface used to do elementwise math sin calculation.
 * Formula: sin(x) = (-1)^k*sin(x0), sin(x) = xP(x)
 */
```

---

## 3. 函数/类注释

### 3.1 高阶 API 风格（`\brief` / `\param` / `\note`）

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

### 3.2 基础 API 风格（`@ingroup` / `@brief` + 分隔线）

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

### 3.3 C API 风格

```cpp
// ==========asc_add(half/float)==========
// 连续模式
__aicore__ inline void asc_add(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count);

// 配置模式
__aicore__ inline void asc_add(__ubuf__ half* dst, ...);
```

---

## 4. 参数标注约定

### 4.1 方向标注

| 标注 | 含义 |
|------|------|
| `\param [out]` | 输出参数 |
| `\param [in]` | 输入参数 |
| `\param [in/out]` | 输入输出参数 |

### 4.2 模板参数

```
\tparam T: target operand data type, supports half/float
\tparam U: source operand data type, supports half/float
\tparam isReuseSource: whether allows API to modify source data during calculation
```

### 4.3 参数描述格式

- 参数名与实际形参名一致
- 描述简洁，说明类型约束和取值范围
- 统一使用英文或中文，一个文件内保持一致

---

## 5. 特殊标注

### 5.1 `\note` 约束说明

```cpp
/*!
 * \note src/dst Tensor must be 32B aligned.
 * \note CalCount must be less than or equal to the element count of src/dst Tensor.
 * \note support data type: half and float
 */
```

用于：对齐要求、数据范围限制、支持的数据类型、性能注意事项

### 5.2 弃用标注

**方式一：`[[deprecated]]` 属性**

```cpp
[[deprecated("NOTICE: SetDumpWorkSpacePtr has been deprecated and will be removed "
             "in the next version. Please do not use it!")]]
__aicore__ inline __gm__ uint8_t* SetDumpWorkSpacePtr(__gm__ uint8_t* workspace)
```

**方式二：行注释**

```cpp
// NOTICE: InitOutput has been deprecated and will be removed in the next version.
// Please use Fill instead!
```

**方式三：`#pragma message`（仅限 impl 头文件）**

```cpp
#pragma message("impl/xxx.h is an internal header file and must not be used directly. "
    "Please use \"#include \\\"basic_api/xxx.h\\\"\" "
    "and use public functions or variables defined in interface headers files.")
```

### 5.3 功能分组线（基础 API）

```cpp
/* **************************************************************************************************
 * Add                                             *
 * ************************************************************************************************* */
```

- 80 列宽
- 中间写函数名
- 用于在大型接口文件中划分功能区块

---

## 6. 中英文约定

| 内容类型 | 语言 | 示例 |
|---------|------|------|
| 公开 API 注释（`\brief`、`\param`） | 英文 | `\brief compute Sin elementwisely` |
| impl 内部注释 | 中文 | `// 计算循环次数` |
| 日志消息 | 英文 | `"Tensor size mismatch"` |
| 断言消息 | 英文 | `"current data type is not supported"` |
| 文件/函数/变量名 | 英文 | `kernel_operator_vec_binary_intf.h` |
| 文档描述（.md） | 中文 | 本文档介绍... |

---

## 7. 文档文件 (.md)

### 7.1 API 文档组织

```
docs/
├── api/
│   └── context/
│       ├── c_api/
│       │   ├── vector_compute/
│       │   │   ├── asc_add.md
│       │   │   └── ...
│       │   ├── general_instruction.md
│       │   └── c_api_list.md
│       └── adv_api/
│           └── math/
│               ├── axpy.md
│               └── ...
├── guide/
│   └── ...
└── README.md
```

### 7.2 API 文档模板

每个 API 文档（`.md`）应包含：

1. API 名称和功能概述
2. 函数原型（含模板参数说明表格、接口参数说明表格）
3. 参数约束和取值范围
4. 支持的架构和数据类型
5. 使用示例代码
6. 注意事项
