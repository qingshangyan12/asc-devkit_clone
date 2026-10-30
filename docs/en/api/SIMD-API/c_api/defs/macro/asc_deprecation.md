# ASC_DEPRECATED

<!-- md-trans-meta sourceCommit=d04c597734e437e5ee87a6dd3b47f91fe427e33f translatedAt=2026-10-23T00:06:13.770Z pushedAt=2026-10-28T12:47:03.111Z nl=2 -->

## Description

The header file path is `"c_api/defs/macro.h"`.

`ASC_DEPRECATED` adds the C++ `[[deprecated]]` attribute to C API declarations. When you compile code that calls a deprecated API, the compiler generates a warning that includes the version in which the API was deprecated, the planned removal date, and the recommended replacement API.

This macro only adds the deprecation attribute and warning message. It does not prevent the deprecated API from being compiled, nor does it automatically remove the API after the planned removal date.

## Macro Definition

```cpp
#define ASC_DEPRECATED(version, removal_date, replacement)                                                  \
    [[deprecated("Deprecated since " #version ", Will be removed after " removal_date ", Use " #replacement \
                 " instead.")]]
```

## Parameters

| Parameter | Description |
| --- | --- |
| version | CANN version in which the API starts to be deprecated. The macro converts this parameter to a string. Do not add double quotation marks when passing the parameter, for example, `9.2.0`. |
| removal_date | Planned removal date of the API. This parameter must be a string in the `"YYYY/MM/DD"` format, for example, `"2027/09/07"`. |
| replacement | Recommended replacement API or alternative solution. The macro converts this parameter to a string. Do not add double quotation marks when passing the parameter. If there are multiple replacement APIs, you can pass a complete description of the alternative solution. |

The format of the compiler warning message is as follows:

```text
Deprecated since <version>, Will be removed after <removal_date>, Use <replacement> instead.
```

## Constraints

- When using this macro, place it before the declaration to be marked as deprecated.
- `version` and `replacement` are converted to strings through the preprocessor `#` operator, so do not add double quotation marks to these parameters. `removal_date` is directly concatenated into the warning message and must be enclosed in double quotation marks.
- `removal_date` is used only to generate the warning message and does not control the actual removal time of the API.

## Examples

The following code uses this macro to mark the [asc_muls](../../deprecated_interface/asc_muls_deprecated.md) API declaration and recommends using the [asc_mul_scalar_float2half_rn](../../reg_compute/reg_fused/asc_mul_scalar_float2half_rn.md) API instead.

```cpp
#include "c_api/defs/macro.h"

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_mul_scalar_float2half_rn)
__simd_callee__ inline void asc_muls(vector_half& dst, vector_float src, float value, vector_bool mask);
```

When `asc_muls` is called, the compiler generates the following warning message:

```text
Deprecated since 9.2.0, Will be removed after 2027/09/07, Use asc_mul_scalar_float2half_rn instead.
```
