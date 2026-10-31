# make_stride

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-27T10:14:30.887Z pushedAt=2026-10-27T13:04:46.103Z nl=2 -->

## Applicable Products

<!-- npu="950" id1 -->
- Ascend 950PR&950DT products: Supported
<!-- end id1 -->
<!-- npu="A3" id2 -->
- Atlas A3 products: Not supported
<!-- end id2 -->
<!-- npu="910b" id3 -->
- Atlas A2 products: Not supported
<!-- end id3 -->
<!-- npu="310b" id4 -->
- Atlas 200I/500 A2 inference products: Not supported
<!-- end id4 -->
<!-- npu="310p" id5 -->
- AI Core in Atlas inference products: Not supported
<!-- end id5 -->
<!-- npu="310p" id6 -->
- Vector Core in Atlas inference products: Not supported
<!-- end id6 -->
<!-- npu="910" id7 -->
- Atlas training products: Not supported
<!-- end id7 -->

## Description

Constructs a stride object that describes the stride of each dimension in memory. It supports passing multiple stride values and also supports passing nested stride substructures to construct hierarchical strides.

## Function Prototype

```cpp
template <typename FirstStride, typename... Strides>
__aicore__ inline constexpr stride<FirstStride, Strides...> make_stride(
    const FirstStride& first_stride, const Strides&... strides)
```

## Parameters

**Table 1** Template parameter description

| Parameter | Input/Output | Description |
|--------|-----------|------|
| FirstStride | Input | Type of the first stride element, which can be an integer variable, a `Std::Int` integer constant, or a nested stride substructure type. |
| Strides... | Input | Types of the remaining stride elements. |

**Table 2**  Parameters

| Parameter | Input/Output | Description |
|--------|-----------|------|
| first_stride | Input | Stride or substructure of the first dimension. |
| strides... | Input | Strides or substructures of the remaining dimensions. |

## Return Value

Returns a `stride<FirstStride, Strides...>` object.

## Constraints

- The input parameters cannot be empty.
- Each parameter must be a non-negative integer.
- Supported data types include integer types such as size_t and int, or the Std::Int type.

## Example

```cpp
#include "tensor_api/tensor.h"

using namespace asc::te;

// Construct a regular stride.
auto stride = make_stride(1, 100, 200);

// Construct a hierarchical stride.
auto fractal_stride = make_stride(make_stride(1, 16), make_stride(32, 512));

auto stride0 = Std::get<0>(stride);                    // stride0 = 1
auto inner_stride = Std::get<1>(Std::get<0>(fractal_stride)); // inner_stride = 16
```
