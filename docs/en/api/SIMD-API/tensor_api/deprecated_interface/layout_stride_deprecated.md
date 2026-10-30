# layout::Stride (Deprecated)

<!-- md-trans-meta sourceCommit=c891f0c5d5b2ac25c5683ff40810fa381d654ece translatedAt=2026-10-27T09:55:03.294Z pushedAt=2026-10-27T13:04:46.074Z nl=2 -->

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

The header file path is `tensor_api/tensor.h`.

**This API is deprecated. Use [layout::stride](../layout/layout.md#stride) instead.**

Obtains the Stride object in Layout or the Stride substructure corresponding to a specified sub-dimension.

## Function Prototype

```cpp
template <size_t... I>
__aicore__ inline constexpr decltype(auto) Stride();

template <size_t... I>
__aicore__ inline constexpr decltype(auto) Stride() const;
```

## Parameters

**Table 1** Template parameter description

| Parameter | Input/Output | Description |
| :--- | :---: | :--- |
| I... | Input | Template parameter, indicating the sequence of sub-dimension indexes to be accessed. When not specified, the complete Layout is processed. |

## Return Value

Returns a reference to the Stride object or its substructure; the const overload returns a constant reference.

## Pipeline Type

None

## Constraints

`I...` must be a valid index in Stride.

## Example

```cpp
#include "tensor_api/tensor.h"

using namespace asc::te;

auto& old_stride = layout.Stride();         // Deprecated usage.
auto& new_stride = layout.stride();         // Replacement usage.
```
