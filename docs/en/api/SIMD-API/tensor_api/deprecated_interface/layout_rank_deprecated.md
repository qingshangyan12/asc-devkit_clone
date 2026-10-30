# layout::Rank (Deprecated)

<!-- md-trans-meta sourceCommit=c891f0c5d5b2ac25c5683ff40810fa381d654ece translatedAt=2026-10-27T09:52:58.637Z pushedAt=2026-10-27T13:04:46.071Z nl=2 -->

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

**This API is deprecated. Use [layout::rank](../layout/layout.md#rank) instead.**

Returns the rank of the current Layout or of a specified sub-dimension.

## Function Prototype

```cpp
template <size_t... I>
__aicore__ inline constexpr decltype(auto) Rank() const;
```

## Parameters

**Table 1** Template parameter description

| Parameter | Input/Output | Description |
| :--- | :---: | :--- |
| I... | Input | Template parameter, indicating the sequence of sub-dimension indexes to be accessed. When not specified, the complete Layout is processed. |

## Return Value

Returns the number of top-level dimensions of the current Layout or the Shape of the specified sub-dimensions.

## Pipeline Type

None

## Constraints

`I...` must be a valid index in Layout.

## Example

```cpp
#include "tensor_api/tensor.h"

using namespace asc::te;

auto old_rank = layout.Rank();              // Deprecated usage.
auto new_rank = layout.rank();              // Replacement usage.
```
