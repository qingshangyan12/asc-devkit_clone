# asc_get_block_idx (Deprecated)

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-23T02:39:38.909Z pushedAt=2026-10-28T12:55:35.634Z nl=2 -->

## Applicable Products

<!-- npu="950" id1 -->
- Ascend 950PR&950DT products: Supported
<!-- end id1 -->
<!-- npu="A3" id2 -->
- Atlas A3 products: Supported
<!-- end id2 -->
<!-- npu="910b" id3 -->
- Atlas A2 products: Supported
<!-- end id3 -->
<!-- npu="310b" id4 -->
- Atlas 200I/500 A2 inference products: Not supported
<!-- end id4 -->
<!-- npu="310p" id5 -->
- Atlas inference product AI Core: Not supported
<!-- end id5 -->
<!-- npu="310p" id6 -->
- Atlas inference product Vector Core: Not supported
<!-- end id6 -->
<!-- npu="910" id7 -->
- Atlas training products: Not supported
<!-- end id7 -->

## Description

The header file path is `"c_api/utils/sys_var.h"`.

**This API is deprecated. Use the [built-in variable block_idx](../../../../guide/programming_guide/language_extension/simd_builtin_keywords.md#built-in-variables) together with the [asc_get_sub_block_num](../utils/sys_var/asc_get_sub_block_num.md) and [asc_get_sub_block_id](../utils/sys_var/asc_get_sub_block_id.md) APIs instead.**

Obtains the index of the current running core (logical core). When replacing it, calculate based on the logical position:

```cpp
int64_t blockIdx = block_idx * asc_get_sub_block_num() + asc_get_sub_block_id();  // Replace asc_get_block_idx().
__gm__ float* xGm = x + blockIdx * blockLength;
```

### Migration

- In non-Mix scenarios, you can directly use the built-in variable `block_idx`, whose value equals the return value of `asc_get_block_idx()`.
- In Mix scenarios (using the `__mix__` function execution space qualifier), the built-in variable `block_idx` only identifies the position of the current combination in the entire grid. The logical core index must be calculated together with sub-block information: `logic_idx = block_idx * sub_block_num + sub_block_id`, that is, `block_idx * asc_get_sub_block_num() + asc_get_sub_block_id()`. The value of this expression is consistent with the return value of the original API.
- Before use, see [Built-in Variable Description](../../../../guide/programming_guide/language_extension/simd_builtin_keywords.md#built-in-variables) to confirm the applicable scenarios.

## Prototype

```cpp
__aicore__ inline int64_t asc_get_block_idx()
```

## Parameters

None

## Return Value

Index of the currently running core.

## Pipeline Type

`PIPE_S`

## Constraints

None

## Examples

```cpp
int64_t blockIdx = block_idx * asc_get_sub_block_num() + asc_get_sub_block_id();  // Obtain the index of the currently running core, replacing asc_get_block_idx().
```
