# asc_get_block_num (Deprecated)

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-23T02:40:20.499Z pushedAt=2026-10-28T12:55:35.637Z nl=2 -->

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

The header file path is: `"c_api/utils/sys_var.h"`.

**This API is deprecated. Use the [built-in variable block_num](../../../../guide/programming_guide/language_extension/simd_builtin_keywords.md#built-in-variables) instead.**

Obtains the number of cores configured for the current task, for use in multi-core logic control within the code. It is recommended to use the built-in variable `block_num` directly in the code:

```cpp
uint32_t blockLength = C_API_TOTAL_LENGTH / block_num;  // Replaces asc_get_block_num()
```

### Migration Notes

The built-in variable `block_num` holds the number of cores configured for the current task, which is the same as the value returned by the original API. You can use it directly as a replacement. Before using it, see [Built-in Variable Description](../../../../guide/programming_guide/language_extension/simd_builtin_keywords.md#built-in-variables) to confirm the applicable scenarios.

## Prototype

```cpp
__aicore__ inline int64_t asc_get_block_num()
```

## Parameters

None

## Return Value

Returns the number of cores configured for the current task.

## Pipeline Type

`PIPE_S`

## Constraints

None

## Examples

```cpp
uint32_t blockLength = C_API_TOTAL_LENGTH / block_num;  // Obtain the number of cores configured for the current task, replacing asc_get_block_num().
```
