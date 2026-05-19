# API 接口设计规范

## 1. 三种 API 类型对比

| 维度 | 基础 API (basic_api) | 高阶 API (adv_api) | C API (c_api) |
|------|---------------------|-------------------|---------------|
| 函数命名 | 大驼峰：`Add`、`Mul` | 大驼峰：`Exp`、`Axpy` | `asc_` 前缀 + 小写：`asc_add` |
| 抽象层级 | 直接映射硬件指令 | 封装基础 API 实现算法 | 直接映射硬件指令 |
| 参数风格 | `LocalTensor<T>&` | `LocalTensor<T>&` + `sharedTmpBuffer` | `__ubuf__ T*` 指针 |
| 文档风格 | `@ingroup` / `@brief` | `\brief` / `\param` / `\note` | 简洁注释 |
| 公开入口 | `kernel_operator.h` | `kernel_api.h` | `asc_simd.h` |
| 实现位置 | `impl/basic_api/` | `impl/adv_api/detail/<category>/<api>/` | `impl/c_api/` |
| 命名空间 | `AscendC` | `AscendC` | 无（C 风格） |

---

## 2. 接口设计原则

### 2.1 输出参数优先

输出参数位于输入参数之前：

```cpp
// 正确：dst（输出）在前
void Add(const LocalTensor<T>& dst, const LocalTensor<T>& src0, const LocalTensor<T>& src1, ...);

// 正确：Tiling 接口中引用输出参数在最后
void GetAxpyMaxMinTmpSize(const ge::Shape& srcShape, const uint32_t typeSize,
    const bool isReuseSource, uint32_t& maxValue, uint32_t& minValue);
```

### 2.2 数据流模式支持

支持三种数据流模式：

1. **连续模式**：固定 count 参数
2. **高维切分模式**：repeat + block_stride + repeat_stride 参数
3. **同步模式**：`_sync` 后缀函数

### 2.3 参数命名一致性

相同语义的参数在不同 API 中保持一致的命名：

| 语义 | 参数名 |
|------|--------|
| 目的操作数 | `dstTensor` / `dst` |
| 源操作数 | `srcTensor` / `src0`、`src1` |
| 元素个数 | `calCount` / `count` |
| 临时缓存 | `sharedTmpBuffer` / `tmpBuffer` |
| 迭代次数 | `repeat` / `repeatTime` |
| 地址步长 | `block_stride` / `dstBlkStride` |
| Mask | `mask` / `mask0`、`mask1` |

---

## 3. 模板参数约定

### 3.1 类型参数

- 高阶 API：`T` 为 dst 类型，`U` 为 src 类型
- 基础 API：`T` 为数据类型

```cpp
// 高阶 API — T=dst, U=src
template <typename T, typename U, bool isReuseSource = false>
__aicore__ inline void Axpy(const LocalTensor<T>& dstTensor, const LocalTensor<U>& srcTensor, ...);

// 基础 API — T 统一
template <typename T>
__aicore__ inline void Add(const LocalTensor<T>& dst, const LocalTensor<T>& src0, ...);
```

### 3.2 模板参数声明

使用 `AlwaysBreakTemplateDeclarations: true`（clang-format 配置），每个模板参数占一行：

```cpp
template <
    typename T,
    typename U,
    bool isReuseSource = false
>
__aicore__ inline void Axpy(...);
```

### 3.3 预留参数

未来可能使用的参数设置默认值，命名以 `is` 开头表示布尔配置：

```cpp
bool isReuseSource = false
```

### 3.4 算法参数

使用明确的类型和有意义的名字：

```cpp
uint8_t taylorExpandLevel     // Taylor 展开级数
ReduceDim reduceDim           // 归约维度
EngineType engine             // 计算引擎
```

### 3.5 `static_assert` 类型校验

每个模板函数体首行应校验支持的数据类型：

```cpp
template <typename T>
__aicore__ inline void Exp(...)
{
    static_assert(SupportType<T, half, float>(),
        "current data type is not supported on current device!");
    // ...
}
```

---

## 4. `__aicore__` 及配套属性

### 4.1 放置顺序

```
[static] __aicore__ [constexpr] inline ReturnType FunctionName(args)
```

严格顺序：`static` → `__aicore__` → `constexpr` → `inline` → 返回类型

```cpp
// 正确的放置顺序
__aicore__ inline void Add(...);
constexpr __aicore__ static inline T Max();
static __aicore__ inline void Lock(MutexID id);
```

### 4.2 配套属性

| 属性 | 语义 | 示例 |
|------|------|------|
| `__gm__` | 全局内存地址空间 | `__gm__ uint8_t* ptr` |
| `__inout_pipe__(V)` | 流水线输入输出标注 | `__aicore__ inline __inout_pipe__(MTE2) void DataCopy(...)` |
| `__in_pipe__(V)` | 流水线输入标注 | `__aicore__ inline __in_pipe__(V) void Copy(...)` |
| `__out_pipe__(MTE3)` | 流水线输出标注 | 同上模式 |
| `__BLOCK_LOCAL__ __inline__` | Block 级线程局部变量 | `__BLOCK_LOCAL__ __inline__ TPipe* g_tPipePtr` |
| `__ubuf__` | Unified Buffer 地址空间 | `__ubuf__ half* dst` |

---

## 5. Tiling 接口设计

### 5.1 接口命名

Kernel 侧 `Axpy` ↔ Tiling 侧 `GetAxpyMaxMinTmpSize`：

```
Get<ApiName>MaxMinTmpSize
Get<ApiName>TmpBufferFactorSize
```

### 5.2 参数约定

**输入参数：**

- `const ge::Shape& srcShape` — 输入 shape 信息
- `const uint32_t typeSize` — 数据类型大小（字节）
- `const bool isReuseSource` — 预留参数（传 false）

**输出参数（引用）：**

- `uint32_t& maxValue` — 所需最大临时空间
- `uint32_t& minValue` — 所需最小临时空间

**返回值：** `void`

### 5.3 实现位置

- 声明：`include/adv_api/<category>/<api>_tiling.h`
- 实现：`impl/adv_api/tiling/<category>/<api>_tiling_impl.cpp`
- 注册：在 `impl/adv_api/tiling/CMakeLists.txt` 的 `tiling_api` target 中添加 `.cpp`

### 5.4 常量定义

Tiling 常量在实现文件中以 `constexpr` 定义：

```cpp
constexpr uint32_t AXPY_HALF_CALC_PROC = 4;
constexpr uint32_t AXPY_FLOAT_CALC_PROC = 1;
constexpr uint32_t AXPY_ONE_REPEAT_BYTE_SIZE = 256;
```

### 5.5 断言使用

Tiling 侧使用 `ASCENDC_HOST_ASSERT` 而非 `ASCENDC_ASSERT`：

```cpp
ASCENDC_HOST_ASSERT(inputSize > 0, return, "Input Shape size must be greater than 0.");
```

---

## 6. 公共头文件注册

### 6.1 高阶 API 注册

新增高阶 API 后，需要在以下文件中添加 include：

**`include/adv_api/kernel_api.h`：**

```cpp
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 2201 || __NPU_ARCH__ == 3510)
#include "math/axpy.h"
#endif
```

**`include/adv_api/tiling_api.h`：**

```cpp
#include "math/axpy_tiling.h"
```

### 6.2 基础 API 注册

新增基础 API 在对应的总入口中注册，例如 `kernel_operator.h`。

### 6.3 架构守卫

- 支持多架构的 API：在 `#if defined(__NPU_ARCH__)` 中列出所有支持的架构
- 仅支持特定架构的 API：在 include 时限制架构范围

---

## 7. 弃用标注

### 7.1 函数弃用

```cpp
[[deprecated("NOTICE: OldFunc has been deprecated and will be removed in the next version. "
             "Please use NewFunc instead!")]]
__aicore__ inline void OldFunc(...)
```

### 7.2 行注释弃用

```cpp
// NOTICE: InitOutput has been deprecated and will be removed in the next version.
// Please use Fill instead!
```

### 7.3 Impl 头文件禁止直接包含

```cpp
#pragma message("impl/basic_api/kernel_operator_vec_binary_intf_impl.h "
    "is an internal header file and must not be used directly. "
    "Please use \"#include \\\"basic_api/kernel_operator_vec_binary_intf.h\\\"\" "
    "and use public functions or variables defined in interface headers files.")
```

---

## 8. Interface 与 Implementation 分离

### 8.1 公开头文件 (`include/`)

- 仅包含声明和接口文档
- 在文件末尾 `#include` 对应的 impl 头
- `__aicore__` 标注

### 8.2 实现头文件 (`impl/`)

- 包含 `#pragma message` 警告
- 使用 `#if __NPU_ARCH__ == ...` 按架构分发到 `dav_<arch>/` 子目录
- 包含具体算法逻辑

### 8.3 架构分发模式

```cpp
#if __NPU_ARCH__ == 2201
#include "dav_c220/kernel_operator_common_impl.h"
#elif __NPU_ARCH__ == 3510
#include "dav_3510/kernel_operator_common_impl.h"
#endif
```

---

## 9. 接口实现模式

### 9.1 高阶 API 实现模板

```cpp
// include/adv_api/<category>/<api>.h
template <typename T, bool isReuseSource = false>
__aicore__ inline void ApiName(const LocalTensor<T>& dstTensor,
    const LocalTensor<T>& srcTensor, ...)
{
    ApiNameImpl<T, isReuseSource>(dstTensor, srcTensor, ...);
}

// impl/adv_api/detail/<category>/<api>/<api>_common_impl.h
template <typename T, bool isReuseSource>
__aicore__ inline void ApiNameImpl(const LocalTensor<T>& dstTensor,
    const LocalTensor<T>& srcTensor, ...)
{
    CHECK_FUNC_HIGHLEVEL_API(ApiName, (T, isReuseSource),
        (dstTensor, srcTensor, ...));
    // 实现逻辑...
}
```

### 9.2 算法函数与 Intrinsics 分离

复杂的 API 将对外接口、算法拆解、底层 Intrinsics 分离：

- `ApiName(...)` — 对外接口（public header）
- `ApiNameImpl(...)` — 算法参数拆解（impl header）
- `ApiNameSub(...)` — 分块循环逻辑（impl header）
- `ApiNameIntrinsicsImpl(...)` — 底层指令调用（impl header）
