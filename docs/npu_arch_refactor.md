# `__NPU_ARCH__` 包含方式重构方案

> 扫描路径：`/home/q00954432/asc-devkit/include/simt_api`
> 创建时间：2026-05-19

---

## 背景与问题

`simt_api/` 下 7 个头文件共 9 处使用 `__NPU_ARCH__` 魔数，存在 3 种不同条件组合。每次新增或下线一个 arch，需逐一排查所有文件，极易遗漏。

### 现状分布

| 条件 | 出现位置 | 次数 |
|------|---------|------|
| `(__NPU_ARCH__ == 3510) \|\| (__NPU_ARCH__ == 5102)` | asc_bf16.h, asc_fp16.h, asc_fp8.h, device_atomic_functions.h, device_warp_functions.h, vector_functions.h, common_functions.h | 6 |
| `defined(__DAV_VEC__) && (__NPU_ARCH__ == 2201 \|\| __NPU_ARCH__ == 3510)` | common_functions.h | 1 |
| `__NPU_ARCH__ == 5102`（async_invoke arch 部分） | common_functions.h | 1 |

---

## 设计目标

新增一个 arch 时，**只修改一处**（`npu_arch_features.h`），其他所有头文件零改动。

---

## 方案：两层架构

### 第一层：新建 `simt_api/npu_arch_features.h`

唯一维护 `__NPU_ARCH__` 魔数的文件。将 arch 数值翻译为语义化能力宏。

**设计约束：**
- `__NPU_ARCH__` 由 CMakeLists.txt 通过 `-D` 传入，编译期确定，无顺序依赖问题
- 只做 arch 数值 → 能力符号的翻译，不组合任何业务宏（`SPLIT_CORE_VEC`、`ASCENDC_CPU_DEBUG` 等保留在各自使用处）
- 只需标准 include guard，不需要 `__ASCENDC_INCLUDE_INTERNAL_HEADERS__` 机制（此文件无 ABI 内容）
- 不 include 任何其他头文件

**文件内容：**

```c
#ifndef INCLUDE_SIMT_API_NPU_ARCH_FEATURES_H
#define INCLUDE_SIMT_API_NPU_ARCH_FEATURES_H

/*
 * Arch capability map for simt_api.
 *
 * When adding a new __NPU_ARCH__ value, consult the hardware spec and add it
 * to the appropriate capability macros below. Each macro's comment describes
 * the required hardware feature.
 *
 * Currently all SIMT capabilities share the same arch set (3510 || 5102).
 * To restrict a specific capability to a subset, extract it to its own #if block.
 *
 * __NPU_ARCH__ is set by CMakeLists.txt via compiler -D flag.
 */

/* All SIMT capabilities currently supported on the same arch set.
 * Split into separate #if blocks when a new arch supports only a subset. */
#if (__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102)
#  define __NPU_HAS_FP16__          /* asc_fp16.h: half/half2 ops */
#  define __NPU_HAS_BF16__          /* asc_bf16.h: bfloat16/bfloat16x2 ops */
#  define __NPU_HAS_FP8__           /* asc_fp8.h: fp8 conversion ops */
#  define __NPU_HAS_ATOMIC__        /* device_atomic_functions.h: asc_atomic_* */
#  define __NPU_HAS_WARP__          /* device_warp_functions.h: asc_shfl/ballot/etc. */
#  define __NPU_HAS_VECTOR_TYPES__  /* vector_functions.h: make_short2/int4/etc. */
#  define __NPU_HAS_VF_CALL__       /* common_functions.h: asc_vf_call template */
#endif

/* DAV vector core with split-core scheduling support (arch part only).
 * Business condition defined(__DAV_VEC__) stays in common_functions.h. */
#if (__NPU_ARCH__ == 2201) || (__NPU_ARCH__ == 3510)
#  define __NPU_DAV_VEC_ARCH__
#endif

/* Native async_invoke execution path (arch part only).
 * Combined with SPLIT_CORE_VEC / ASCENDC_CPU_DEBUG in common_functions.h. */
#if __NPU_ARCH__ == 5102
#  define __NPU_ASYNC_INVOKE_ARCH__
#endif

#endif  // INCLUDE_SIMT_API_NPU_ARCH_FEATURES_H
```

### 第二层：`device_types.h` 统一引入

在 `device_types.h` 中添加一行，所有 include `device_types.h` 的头文件自动获得能力宏：

```diff
  #include "utils/base/sys_macros.h"
+ #include "simt_api/npu_arch_features.h"
```

---

## 各文件具体改动

### `asc_fp16.h`

```diff
- #if (__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102)
+ #ifdef __NPU_HAS_FP16__
```

### `asc_bf16.h`

```diff
- #if (__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102)
+ #ifdef __NPU_HAS_BF16__
```

### `asc_fp8.h`（需要额外结构修复）

`asc_fp8.h` 是唯一将 `#include "simt_api/device_types.h"` 放在 arch guard **内部**的头文件，与其他头文件结构不一致。若不修复，`#ifdef __NPU_HAS_FP8__` 在 `device_types.h` 被 include 之前执行，宏未定义导致**静默失效**（编译不报错但 guard 永远不生效）。

修复方式：将 includes 移到 guard 外面，与其他头文件结构对齐：

```diff
+ #include "simt_api/asc_bf16.h"
+ #include "simt_api/device_types.h"

- #if (__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102)
+ #ifdef __NPU_HAS_FP8__

- #include "simt_api/asc_bf16.h"
- #include "simt_api/device_types.h"

  typedef enum __asc_fp8_interpretation_t { ... } __asc_fp8_interpretation_t;
  ...
  #endif
```

### `device_atomic_functions.h`

```diff
- #if (__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102)
+ #ifdef __NPU_HAS_ATOMIC__
```

### `device_warp_functions.h`

```diff
- #if (__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102)
+ #ifdef __NPU_HAS_WARP__
```

### `vector_functions.h`

```diff
- #if (__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102)
+ #ifdef __NPU_HAS_VECTOR_TYPES__
```

### `common_functions.h`（3 处）

```diff
  #ifndef SPLIT_CORE_VEC
- #if (defined(__DAV_VEC__) && (__NPU_ARCH__ == 2201 || __NPU_ARCH__ == 3510))
+ #if (defined(__DAV_VEC__) && defined(__NPU_DAV_VEC_ARCH__))
  #define SPLIT_CORE_VEC
  #endif
  #endif

- #if (__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102)
+ #ifdef __NPU_HAS_VF_CALL__

  template <auto funcPtr, typename... Args>
  __aicore__ inline void asc_vf_call(dim3 threadNums, Args &&...args)
  {
- #if (defined(__NPU_ARCH__) && __NPU_ARCH__ == 5102) || defined(SPLIT_CORE_VEC) || defined(ASCENDC_CPU_DEBUG)
+ #if defined(__NPU_ASYNC_INVOKE_ARCH__) || defined(SPLIT_CORE_VEC) || defined(ASCENDC_CPU_DEBUG)
      cce::async_invoke<funcPtr>(threadNums, args...);
  #endif
  }
  #endif
```

---

## 改动文件汇总

| 文件 | 改动类型 | 改动量 |
|------|---------|--------|
| `simt_api/npu_arch_features.h` | **新建** | 约 35 行 |
| `simt_api/device_types.h` | 新增 1 行 include | 1 行 |
| `simt_api/asc_fp16.h` | 替换 arch 条件 | 1 行 |
| `simt_api/asc_bf16.h` | 替换 arch 条件 | 1 行 |
| `simt_api/asc_fp8.h` | 移动 includes + 替换 arch 条件 | 3 行 |
| `simt_api/device_atomic_functions.h` | 替换 arch 条件 | 1 行 |
| `simt_api/device_warp_functions.h` | 替换 arch 条件 | 1 行 |
| `simt_api/vector_functions.h` | 替换 arch 条件 | 1 行 |
| `simt_api/common_functions.h` | 替换 3 处 arch 条件 | 3 行 |

---

## 新增 arch 操作流程

以新增 arch `6100` 为例：

**情形一：具备完整 SIMT 能力**，只需修改 `npu_arch_features.h` 一行，其他 8 个文件零改动：

```diff
- #if (__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102)
+ #if (__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102) || (__NPU_ARCH__ == 6100)
  #  define __NPU_HAS_FP16__
  ...
```

**情形二：只支持 FP16/BF16，不支持 FP8/warp 等**，将对应宏拆到独立 `#if` 块，其他 8 个文件仍零改动：

```c
#if (__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102) || (__NPU_ARCH__ == 6100)
#  define __NPU_HAS_FP16__
#  define __NPU_HAS_BF16__
#endif

#if (__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102)
#  define __NPU_HAS_FP8__
#  define __NPU_HAS_ATOMIC__
#  define __NPU_HAS_WARP__
#  define __NPU_HAS_VECTOR_TYPES__
#  define __NPU_HAS_VF_CALL__
#endif
```

---

## 设计边界说明

- `npu_arch_features.h` 只负责 **arch 数值 → 能力符号** 的映射，不感知业务宏
- `SPLIT_CORE_VEC`、`ASCENDC_CPU_DEBUG`、`__DAV_VEC__` 等业务/调试宏的组合逻辑**保留在各自使用处**，不下沉到 `npu_arch_features.h`
- 范围仅限 `simt_api/`；`utils/`、`adv_api/`、`basic_api/` 等目录的 `__NPU_ARCH__` 使用不在本次变更范围内
