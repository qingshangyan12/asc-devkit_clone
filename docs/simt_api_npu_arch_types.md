# simt_api NPU_ARCH 条件隔离的 struct / enum 汇总

扫描路径：`/home/q00954432/asc-devkit/include/simt_api`

扫描对象：所有头文件中被 `#if`/`#elif` 含 `__NPU_ARCH__` 的条件编译块所包裹的 `struct` 和 `enum` 定义。

---

## 汇总表

| 类型 | 名称 | 所在文件 | 行号 | NPU_ARCH 条件 | 成员 |
|------|------|----------|------|---------------|------|
| `typedef enum` | `__asc_fp8_interpretation_t` | `asc_fp8.h` | 24–27 | `(__NPU_ARCH__ == 3510) \|\| (__NPU_ARCH__ == 5102)` | `__ASC_E4M3`, `__ASC_E5M2` |
| `typedef enum` | `__asc_saturation_t` | `asc_fp8.h` | 29–32 | `(__NPU_ARCH__ == 3510) \|\| (__NPU_ARCH__ == 5102)` | `__ASC_NOSAT`, `__ASC_SATFINITE` |

---

## 详细说明

### `asc_fp8.h`

条件块：`#if (__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102)`（第 19 行）

#### `__asc_fp8_interpretation_t`（第 24–27 行）

```c
typedef enum __asc_fp8_interpretation_t {
    __ASC_E4M3,    // FP8 E4M3 格式
    __ASC_E5M2,    // FP8 E5M2 格式
} __asc_fp8_interpretation_t;
```

用途：指定 FP8 数据的解释方式（E4M3 或 E5M2），供 `__asc_cvt_float2_to_fp8x2()` 等转换函数使用。

#### `__asc_saturation_t`（第 29–32 行）

```c
typedef enum __asc_saturation_t {
    __ASC_NOSAT,       // 不做饱和处理
    __ASC_SATFINITE,   // 饱和到有限值
} __asc_saturation_t;
```

用途：控制浮点数转 FP8 时的饱和行为，供 `__asc_cvt_float2_to_fp8x2()` 使用。

---

## 各文件扫描结果

| 文件 | 含 `__NPU_ARCH__` 条件 | 条件块内有 struct/enum |
|------|-----------------------|------------------------|
| `asc_fp8.h` | ✅ `(== 3510) \|\| (== 5102)` | ✅ **2 个 enum** |
| `asc_fp16.h` | ✅ `(== 3510) \|\| (== 5102)` | ❌ 仅函数声明和宏定义 |
| `asc_bf16.h` | ✅ `(== 3510) \|\| (== 5102)` | ❌ 仅函数声明和宏定义 |
| `device_atomic_functions.h` | ✅ `(== 3510) \|\| (== 5102)` | ❌ 仅函数声明 |
| `device_warp_functions.h` | ✅ `(== 3510) \|\| (== 5102)` | ❌ 仅函数声明 |
| `vector_functions.h` | ✅ `(== 3510) \|\| (== 5102)` | ❌ 仅函数声明 |
| `common_functions.h` | ✅ `(== 3510) \|\| (== 5102)` 等 | ❌ 仅模板函数 |
| `cpp/kernel_simt_intf.h` | ✅ `(== 5102)` 和 `(== 3510) \|\| (== 5102)` | ❌ 仅 `#include` 指令 |
| `cpp/kernel_simt_utils.h` | ✅ `(== 5102) \|\| ...` | ❌ 仅模板函数 |
| `device_types.h` | ❌ 无 | — |
| `device_functions.h` | ❌ 无 | — |
| `device_sync_functions.h` | ❌ 无 | — |
| `math_constants.h` | ❌ 无 | — |
| `math_functions.h` | ❌ 无 | — |
| `asc_simt.h` | ❌ 无 | — |
| `cpp/kernel_simt_atomic_intf.h` | ❌ 无 | — |
| `cpp/kernel_simt_bessel_intf.h` | ❌ 无 | — |
| `cpp/kernel_simt_cast_intf.h` | ❌ 无 | — |
| `cpp/kernel_simt_cmp_intf.h` | ❌ 无 | — |
| `cpp/kernel_simt_common_intf.h` | ❌ 无 | — |
| `cpp/kernel_simt_math_intf.h` | ❌ 无 | — |
| `cpp/kernel_simt_print_intf.h` | ❌ 无 | — |
| `cpp/kernel_simt_transcendental_intf.h` | ❌ 无 | — |
| `cpp/kernel_simt_warp_level_intf.h` | ❌ 无 | — |

---

---

## `utils/` 头文件中 `__NPU_ARCH__` 条件隔离内容

扫描路径：`/home/q00954432/asc-devkit/include/utils`

扫描对象：所有含 `__NPU_ARCH__` 的条件编译块。

---

### 汇总表

| 文件 | 行号 | `__NPU_ARCH__` 条件 | 隔离内容 |
|------|------|---------------------|---------|
| `debug/asc_assert.h` | 26–32 | `== 3510 \|\| == 5102` | SIMT assert impl；`__asc_simt_vf::__trap()` |
| `debug/asc_assert.h` | 35–37 | `== 2002 \|\| == 2201 \|\| == 3510`（外层需 `!__NPU_COMPILER_INTERNAL_PURE_SIMT__`） | AICore assert impl |
| `debug/asc_time.h` | 25–31 | `== 3510 \|\| == 5102` | `__asc_simt_vf::clock()`；SIMT time impl |
| `debug/asc_time.h` | 34–36 | `== 2201 \|\| == 3510`（外层需 `!__NPU_COMPILER_INTERNAL_PURE_SIMT__`） | AICore time impl |
| `debug/asc_time.h` | 45–51 | `== 3510`（外层需 `!__NPU_COMPILER_INTERNAL_PURE_SIMT__`） | `asc_mark_stamp` 模板 |
| `debug/asc_dump.h` | 23–25 | `== 2201 \|\| == 3510` | AICore dump impl |
| `debug/asc_printf.h` | 38–40 | `== 3510 \|\| == 5102`（外层需 `!__CHECK_FEATURE_AT_PRECOMPILE`） | SIMT printf impl |
| `debug/asc_printf.h` | 45–47 | `== 2002 \|\| == 2201 \|\| == 3510`（外层需 `!__NPU_COMPILER_INTERNAL_PURE_SIMT__` 且 `!__CHECK_FEATURE_AT_PRECOMPILE`） | AICore printf impl |
| `tiling/template_argument.h` | 100–132 | `== 2201` / `== 3002` / `== 3510` / `== 5102`（外层需 `ASCENDC_TPL_KERNEL`） | 各架构数据类型宏启用 |

---

### `debug/asc_assert.h`

#### `#if (__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102)`（第 26–32 行）

```cpp
#if (__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102)
#include "impl/utils/debug/asc_assert_simt_impl.h"

namespace __asc_simt_vf {
__SIMT_DEVICE_FUNCTIONS_DECL__ inline void __trap();
} // namespace __asc_simt_vf
#endif
```

引入 SIMT assert 实现，并声明 `__asc_simt_vf::__trap()`（SIMT 线程异常终止）。

#### `#ifndef __NPU_COMPILER_INTERNAL_PURE_SIMT__` + `#if (__NPU_ARCH__ == 2002) || (__NPU_ARCH__ == 2201) || (__NPU_ARCH__ == 3510)`（第 34–38 行）

非 pure-SIMT 编译器且目标架构为 2002 / 2201 / 3510 时，引入 `asc_aicore_assert_impl.h`。

---

### `debug/asc_time.h`

#### `#if (__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102)`（第 25–31 行）

```cpp
#if (__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102)
namespace __asc_simt_vf {
__SIMT_DEVICE_FUNCTIONS_DECL__ inline uint64_t clock(void);
} // namespace __asc_simt_vf

#include "impl/utils/debug/asc_time_simt_impl.h"
#endif
```

声明 `__asc_simt_vf::clock()`（读取 SIMT 线程时钟计数器），并引入 SIMT 时间实现。

#### `#ifndef __NPU_COMPILER_INTERNAL_PURE_SIMT__` 内的两处 `__NPU_ARCH__` 条件（第 33–56 行）

- `#if (__NPU_ARCH__ == 2201) || (__NPU_ARCH__ == 3510)`（第 34 行）：引入 `asc_aicore_time_impl.h`
- `#if __NPU_ARCH__ == 3510`（第 45 行）：额外声明 `asc_mark_stamp` 模板：

```cpp
template<pipe_t pipe = PIPE_S>
__aicore__ inline void asc_mark_stamp(uint16_t idx);

template<pipe_t pipe = PIPE_S, uint16_t idx>
__aicore__ inline void asc_mark_stamp();
```

---

### `debug/asc_dump.h`

#### `#if (__NPU_ARCH__ == 2201) || (__NPU_ARCH__ == 3510)`（第 23–25 行）

```cpp
#if (__NPU_ARCH__ == 2201) || (__NPU_ARCH__ == 3510)
#include "impl/utils/debug/asc_aicore_dump_impl.h"
#endif
```

引入 AICore dump 实现。其余 dump 函数声明无条件存在（不受 `__NPU_ARCH__` 控制）。

---

### `debug/asc_printf.h`

#### `#if (__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102)`（第 38–40 行）

```cpp
#if (__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102)
#include "impl/utils/debug/asc_printf_simt_impl.h"
#endif
```

引入 SIMT printf 实现（位于 `#ifndef __CHECK_FEATURE_AT_PRECOMPILE` 内）。

#### `#ifndef __NPU_COMPILER_INTERNAL_PURE_SIMT__` + `#if (__NPU_ARCH__ == 2002) || (__NPU_ARCH__ == 2201) || (__NPU_ARCH__ == 3510)`（第 43–47 行）

引入 `asc_aicore_printf_impl.h`（额外需 `!__CHECK_FEATURE_AT_PRECOMPILE`）。

---

### `tiling/template_argument.h`

仅在 `#elif defined(ASCENDC_TPL_KERNEL)` 上下文内有效（第 97 行起）。

#### 各架构数据类型启用矩阵（第 100–132 行）

| 数据类型宏 | 2201 | 3002 | 3510 | 5102 |
|-----------|:----:|:----:|:----:|:----:|
| `ASC_INNER_DATATYPE_ENABLE_INT4` | ✅（所有架构无条件） | ✅ | ✅ | ✅ |
| `ASC_INNER_DATATYPE_ENABLE_BF16` | ✅ | ✅ | ✅ | ✅ |
| `ASC_INNER_DATATYPE_ENABLE_HIFLOAT8` | | ✅ | ✅ | ✅ |
| `ASC_INNER_DATATYPE_ENABLE_FLOAT8_E5M2` | | ✅ | ✅ | ✅ |
| `ASC_INNER_DATATYPE_ENABLE_FLOAT8_E4M3FN` | | ✅ | ✅ | ✅ |
| `ASC_INNER_DATATYPE_ENABLE_FLOAT4_E2M1` | | ✅ | ✅ | ✅ |
| `ASC_INNER_DATATYPE_ENABLE_FLOAT4_E1M2` | | ✅ | ✅ | ✅ |
| `ASC_INNER_DATATYPE_ENABLE_COMPLEX64` | | | ✅ | ✅ |
| `ASC_INNER_DATATYPE_ENABLE_COMPLEX32` | | | ✅ | ✅ |
| `ASC_INNER_DATATYPE_ENABLE_UINT1` | | | | ✅ |
| `ASC_INNER_DATATYPE_ENABLE_INT2` | | | | ✅ |

注：`ASCENDC_CPU_DEBUG` 定义时会额外 `#undef` 掉所有 `FLOAT4_*` 和 `FLOAT8_*`（第 134–138 行）。

这些宏进一步控制 `TypeFromId<DT_*>` 的模板特化（第 161–193 行），只有对应宏已定义时该特化才存在。

---

### 各文件扫描结果

| 文件 | 含 `__NPU_ARCH__` 条件 | 条件 |
|------|-----------------------|------|
| `debug/asc_assert.h` | ✅ | `== 3510\|\|5102`；`== 2002\|\|2201\|\|3510` |
| `debug/asc_time.h` | ✅ | `== 3510\|\|5102`；`== 2201\|\|3510`；`== 3510` |
| `debug/asc_dump.h` | ✅ | `== 2201\|\|3510` |
| `debug/asc_printf.h` | ✅ | `== 3510\|\|5102`；`== 2002\|\|2201\|\|3510` |
| `tiling/template_argument.h` | ✅ | `== 2201`；`== 3002`；`== 3510`；`== 5102` |
| 其余文件 | ❌ 无 | — |

---

> 生成时间：2026-05-14  
> 扫描版本：全量扫描，逐文件人工核实  
> 2026-05-15 补充：`utils/` 目录全部 `__NPU_ARCH__` 条件块

---

---

## 将 `__NPU_ARCH__` 隔离内容移到宏外部的风险分析

**背景**：`__NPU_ARCH__` 在设备编译（`.asc` kernel）时被设置为目标架构值；host 编译时通常未定义（等效为 0）。移到宏外部意味着内容对 host 可见，同时对所有设备架构可见。

**关键约束**：以下符号在 host 编译器（标准 C++）中不存在：

| 符号 | 来源 | host 上的问题 |
|------|------|--------------|
| `__simt_callee__` | `__SIMT_DEVICE_FUNCTIONS_DECL__` 展开值 | 未定义 attribute → 编译错误 |
| `__aicore__` | `__SIMT_DEVICE_FUNCTIONS_DECL__` 展开值（pure-SIMT 模式） | 未定义 attribute → 编译错误 |
| `__gm__` | 设备端地址空间限定符 | host 不认识 → 编译错误 |
| `float2` | 设备端内建类型 | host 未定义 → 编译错误 |
| `hifloat8x2_t`、`float8_e4m3x2_t` 等 | 设备端 fp8 类型 | host 未定义 → 编译错误 |

---

### 无风险（可安全移到宏外，host 可见）

以下内容是纯 C 类型定义，不含任何设备 attribute、地址空间限定符或设备专属类型。

| 内容 | 所在文件 | 理由 |
|------|---------|------|
| `typedef enum __asc_fp8_interpretation_t { __ASC_E4M3, __ASC_E5M2 }` | `simt_api/asc_fp8.h` 第 24–27 行 | 纯 C enum，成员只是整数常量，无任何设备依赖 |
| `typedef enum __asc_saturation_t { __ASC_NOSAT, __ASC_SATFINITE }` | `simt_api/asc_fp8.h` 第 29–32 行 | 同上 |
| `typedef unsigned short int __asc_fp8x2_storage_t` | `simt_api/asc_fp8.h` 第 34 行 | 只是 `unsigned short int` 的别名，与架构无关 |

**移动方式**：将这三行提到 `asc_fp8.h` 中 `#if (__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102)` 之前即可，无需其他改动。

---

### 有风险（不可直接移到宏外）

#### 1. `#include "impl/..."` 系列（极高风险）

| 内容 | 文件 | 风险原因 |
|------|------|---------|
| `#include "impl/simt_api/asc_fp8_impl.h"` | `asc_fp8.h` | impl 内部包含 `__SIMT_DEVICE_FUNCTIONS_DECL__`、`float2`、`RoundingSaturation` 等设备专属符号；即使 impl 自身有内层 `__NPU_ARCH__` 守卫，`#include` 展开仍会触发头文件处理，引入无法在 host 解析的依赖 |
| `#include "impl/utils/debug/asc_assert_simt_impl.h"` | `asc_assert.h` | impl 包含 `__SIMT_DEVICE_FUNCTIONS_DECL__`、`__gm__`、设备函数体（含非法地址访问 `*((uint8_t __gm__*)-1) = 0`） |
| `#include "impl/utils/debug/asc_time_simt_impl.h"` | `asc_time.h` | 同类，含设备时钟指令实现 |
| `#include "impl/utils/debug/asc_printf_simt_impl.h"` | `asc_printf.h` | 同类，含设备 printf 实现 |
| `#include "impl/utils/debug/asc_aicore_dump_impl.h"` | `asc_dump.h` | 同类，含 AICore dump 实现 |

#### 2. 使用 `__SIMT_DEVICE_FUNCTIONS_DECL__` 的函数声明（高风险）

`__SIMT_DEVICE_FUNCTIONS_DECL__` 展开为 `__simt_callee__`（混合模式）或 `__aicore__`（pure-SIMT 模式），二者均为编译器私有 attribute，在标准 C++ host 编译中未定义。

| 内容 | 文件 | 额外风险 |
|------|------|---------|
| `__asc_simt_vf::__trap()` 声明 | `asc_assert.h` 第 30 行 | attribute 未定义；impl 无 host 实现 → 链接失败 |
| `__asc_simt_vf::clock()` 声明 | `asc_time.h` 第 27 行 | 同上；`clock` 与 `<ctime>` 的 `std::clock` 存在命名冲突风险 |
| `asc_fp8.h` 中所有函数声明（`__float22hif82_rna` 等） | `asc_fp8.h` 第 36–62 行 | 参数类型使用 `float2`、`hifloat8x2_t` 等 host 不存在的类型，声明本身在 host 即无法编译 |

#### 3. `tiling/template_argument.h` 中的架构数据类型启用宏（高风险）

这些 `#define ASC_INNER_DATATYPE_ENABLE_*` 宏在 `ASCENDC_TPL_KERNEL` 上下文内被后续 `TypeFromId` 模板特化消费。若移到各架构的 `__NPU_ARCH__` 条件块外：

- **对其他设备架构**：会为不支持该数据类型的架构（如 2201）启用 `complex64`、`uint1b_t` 等，而这些类型在 2201 上未定义 → 编译错误
- **对 host**：`bfloat16_t`、`int4b_t` 等设备专属类型同样在 host 未定义 → 编译错误
- 即使只移动 `#define` 本身（不移动 `TypeFromId` 特化），也会导致使用方代码在不支持的架构上错误地认为某类型可用

---

### 汇总

| 内容 | 风险等级 | 可移动 |
|------|---------|--------|
| `__asc_fp8_interpretation_t` enum | 无风险 | ✅ 可直接移到宏外 |
| `__asc_saturation_t` enum | 无风险 | ✅ 可直接移到宏外 |
| `__asc_fp8x2_storage_t` typedef | 无风险 | ✅ 可直接移到宏外 |
| 所有 `#include "impl/..."` | 极高风险 | ❌ 不可移动 |
| 带 `__SIMT_DEVICE_FUNCTIONS_DECL__` 的函数声明 | 高风险 | ❌ 不可移动 |
| 参数含设备类型（`float2` 等）的函数声明 | 高风险 | ❌ 不可移动 |
| `ASC_INNER_DATATYPE_ENABLE_*` 宏及 `TypeFromId` 特化 | 高风险 | ❌ 不可移动 |

---

> 2026-05-15 补充：host 可见性风险分析
