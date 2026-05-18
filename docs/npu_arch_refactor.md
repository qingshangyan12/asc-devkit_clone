# `__NPU_ARCH__` 包含方式重构方案

## 背景与问题

`simt_api/` 下的头文件中，`__NPU_ARCH__` 魔数散落在 7 个文件的 9 处，存在 3 种不同的条件组合。每次新增或下线一个 arch，需要逐一排查每个文件，极易遗漏。

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

```c
/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * ...
 */

#ifndef INCLUDE_SIMT_API_NPU_ARCH_FEATURES_H
#define INCLUDE_SIMT_API_NPU_ARCH_FEATURES_H

/*
 * Arch capability map for simt_api.
 *
 * When adding a new __NPU_ARCH__ value, consult the hardware spec and add it
 * to the appropriate capability macros below. Each macro's comment describes
 * the required hardware feature.
 *
 * __NPU_ARCH__ is set by CMakeLists.txt via compiler -D flag.
 */

/* Full SIMT execution model: warp/ballot/shfl, atomic ops, fp16/bf16/fp8 */
#if (__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102)
#  define __NPU_SIMT_ENABLED__
#endif

/* DAV vector core with split-core scheduling support (arch part only) */
#if (__NPU_ARCH__ == 2201) || (__NPU_ARCH__ == 3510)
#  define __NPU_DAV_VEC_ARCH__
#endif

/* Native async_invoke execution path (arch part only) */
#if __NPU_ARCH__ == 5102
#  define __NPU_ASYNC_INVOKE_ARCH__
#endif

#endif  // INCLUDE_SIMT_API_NPU_ARCH_FEATURES_H
```

**设计约束：**
- 只做 arch 数值 → 能力符号的翻译，不组合任何业务宏（`SPLIT_CORE_VEC`、`ASCENDC_CPU_DEBUG` 等保留在各自的使用处）
- 只需标准 include guard，不需要 `__ASCENDC_INCLUDE_INTERNAL_HEADERS__` 机制（此文件无 ABI 内容）
- 不 include 任何其他头文件

### 第二层：`device_types.h` 统一引入

在 `device_types.h` 中添加一行，使所有 include `device_types.h` 的头文件自动获得能力宏：

```c
// device_types.h（在 sys_macros.h include 之后添加）
#include "utils/base/sys_macros.h"
#include "simt_api/npu_arch_features.h"   // ← 新增
```

---

## 各文件具体改动

### 1. `asc_bf16.h`、`asc_fp16.h`、`device_atomic_functions.h`、`device_warp_functions.h`、`vector_functions.h`

这 5 个文件结构相同，改动一致：将 arch 条件替换为能力宏。

```diff
- #if (__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102)
+ #ifdef __NPU_SIMT_ENABLED__
```

### 2. `common_functions.h`

涉及 3 处改动，业务逻辑组合保持不变：

```diff
  #ifndef SPLIT_CORE_VEC
- #if (defined(__DAV_VEC__) && (__NPU_ARCH__ == 2201 || __NPU_ARCH__ == 3510))
+ #if (defined(__DAV_VEC__) && defined(__NPU_DAV_VEC_ARCH__))
  #define SPLIT_CORE_VEC
  #endif
  #endif

  #ifndef __NPU_COMPILER_INTERNAL_PURE_SIMT__
  using dim3 = cce::dim3;
- #if (__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102)
+ #ifdef __NPU_SIMT_ENABLED__

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

### 3. `asc_fp8.h`（需要结构性修复）

**问题：** `asc_fp8.h` 是唯一一个将 `#include "simt_api/device_types.h"` 放在 arch guard **内部**的头文件，与其他头文件结构不一致。直接替换 guard 会导致 `__NPU_SIMT_ENABLED__` 在定义之前被使用，静默失效。

**修复：** 将 includes 移到 guard 外面，与其他头文件对齐。

```diff
+ #include "simt_api/asc_bf16.h"
+ #include "simt_api/device_types.h"

- #if (__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102)
+ #ifdef __NPU_SIMT_ENABLED__

- #include "simt_api/asc_bf16.h"
- #include "simt_api/device_types.h"

  typedef enum __asc_fp8_interpretation_t { ... } __asc_fp8_interpretation_t;
  ...

  #endif
```

---

## 改动文件汇总

| 文件 | 改动类型 | 改动量 |
|------|---------|--------|
| `simt_api/npu_arch_features.h` | **新建** | 约 20 行 |
| `simt_api/device_types.h` | 新增 1 行 include | 1 行 |
| `simt_api/asc_bf16.h` | 替换 arch 条件 | 1 行 |
| `simt_api/asc_fp16.h` | 替换 arch 条件 | 1 行 |
| `simt_api/asc_fp8.h` | 移动 includes + 替换 arch 条件 | 3 行 |
| `simt_api/device_atomic_functions.h` | 替换 arch 条件 | 1 行 |
| `simt_api/device_warp_functions.h` | 替换 arch 条件 | 1 行 |
| `simt_api/vector_functions.h` | 替换 arch 条件 | 1 行 |
| `simt_api/common_functions.h` | 替换 3 处 arch 条件 | 3 行 |

---

## 新增 arch 的操作流程（改后）

以新增 arch `6100`（具备完整 SIMT 能力）为例，只需修改 `npu_arch_features.h`：

```diff
- #if (__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102)
+ #if (__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102) || (__NPU_ARCH__ == 6100)
  #  define __NPU_SIMT_ENABLED__
  #endif
```

其他 8 个文件零改动。

---

## 设计边界说明

- `npu_arch_features.h` 只负责 **arch 数值 → 能力符号** 的映射，不感知业务宏
- `SPLIT_CORE_VEC`、`ASCENDC_CPU_DEBUG`、`__DAV_VEC__` 等业务/调试宏的组合逻辑**保留在各自使用处**，不下沉到 `npu_arch_features.h`
- 范围仅限 `simt_api/`，`adv_api/`、`basic_api/` 等目录的 `__NPU_ARCH__` 使用不在本次变更范围内
