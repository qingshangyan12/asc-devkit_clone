# get_stride

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-27T10:05:08.214Z pushedAt=2026-10-27T13:04:46.093Z nl=2 -->

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

get_stride is used to extract the stride part from a layout:

- **Without specifying the template parameter Is...**: Returns the complete stride tuple.
- **With the template parameter Is... specified**: Selects the elements at the corresponding indexes from the top-level stride tuple. When a single index is specified, that element is returned; when multiple indexes are specified, a new tuple composed of these top-level elements is returned.

## Function Prototype

```cpp
template <size_t... Is, typename Layout>
__aicore__ inline constexpr auto get_stride(const Layout& layout)
```

## Parameters

**Table 1** Template parameter description

| Parameter | Input/Output | Description |
| -------- | ----------- | ------ |
| Is... | Input | Optional. Used to select one or more indexes in the top-level stride tuple. |
| Layout | Input | Type of the layout object. |

**Table 2**  Parameters

| Parameter | Input/Output | Description |
| -------- | ----------- | ------ |
| layout | Input | layout object. |

## Return Value

Returns the stride object (or its substructure) that describes the memory access stride.

## Constraints

Is... must be within the valid range of the top-level stride tuple.

## Example

```cpp
#include "tensor_api/tensor.h"

using namespace asc::te;

auto layout = make_layout(make_shape(10, 20, 30), make_stride(1, 10, 200));

// If no Is... is specified, returns the complete stride tuple.
auto full_stride = get_stride(layout);         // (1, 10, 200)

// If a single index is specified, returns the corresponding element.
auto s0 = get_stride<0>(layout);             // 1

// If multiple indexes are specified, composes a new tuple.
auto s01 = get_stride<0, 1>(layout);         // (1, 10)
```
