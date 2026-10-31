# base_tensor::Capacity (Deprecated)

<!-- md-trans-meta sourceCommit=c891f0c5d5b2ac25c5683ff40810fa381d654ece translatedAt=2026-10-27T09:56:13.598Z pushedAt=2026-10-27T13:04:46.077Z nl=2 -->

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

**This API is deprecated. Use [base_tensor::capacity](../tensor/tensor.md#capacity) instead.**

Obtains the actual memory capacity occupied by the Tensor under the current Layout.

## Function Prototype

```cpp
__aicore__ inline constexpr auto Capacity() const;
```

## Parameters

None

## Return Value

Returns the result of `layout().capacity()`.

## Pipeline Type

None

## Constraints

None

## Example

```cpp
#include "tensor_api/tensor.h"

using namespace asc::te;

auto old_capacity = tensor.Capacity();       // Deprecated usage.
auto new_capacity = tensor.capacity();       // Replacement usage.
```
