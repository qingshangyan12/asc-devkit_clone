# GetReduceXorSumMaxMinTmpSize

<!-- md-trans-meta sourceCommit=7ef8ad7bdbdb0cef0f561d391759ecb8b4e4043a translatedAt=2026-10-19T04:26:51.846Z pushedAt=2026-10-29T02:15:03.128Z nl=2 -->

## Description

The ReduceXorSum API on the kernel side requires you to reserve or allocate temporary space for computation. This API is used to obtain the maximum and minimum temporary space sizes to reserve or allocate on the host side. Based on this range, you can select an appropriate space size as a Tiling parameter to pass to the kernel side.

-   To ensure correct functionality, the reserved or applied temporary space size must not be smaller than the minimum temporary space size.
-   Within the range from the minimum to the maximum temporary space size, as the temporary space increases, the computation performance of the kernel-side API improves to a certain extent. To achieve better performance, you can reserve or allocate space based on the actual memory usage. For this API, **the maximum temporary space is currently equal to the minimum temporary space**.

## Prototype

```
void GetReduceXorSumMaxMinTmpSize(const AscendC::TensorShape& srcShape, const uint32_t typeSize, const bool isReuseSource, uint32_t& maxValue, uint32_t& minValue)
```

## Parameters

**Table 1**  API parameter list

| Parameter | Input/Output | Description |
| --- | --- | --- |
| srcShape | Input | Input shape information. The parameter type is [AscendC::TensorShape](../../data_structures/TensorShape.md). |
| typeSize | Input | Size of the input data type, in bytes. For example, if the input data type is int16_t, pass 2 here. |
| isReuseSource | Input | Whether to reuse the space of the source operand, consistent with the ReduceXorSum API. |
| maxValue | Output | Maximum temporary space size required for the ReduceXorSum API to complete the computation. Space exceeding this value is not used by this API.<br> Note: <br>maxValue is for reference only and may be larger than the remaining space of the Unified Buffer (UB). In this case, you need to select an appropriate temporary space size based on the remaining UB space. |
| minValue | Output | Minimum temporary space size required for the ReduceXorSum API to complete the computation. To ensure correct functionality, the temporary space reserved or allocated during API computation must not be smaller than this value. |

## Return Value

None

## Constraints

None

## Examples

```
// The input shape information is 1024. The input data type of the operator is int16_t. Modifying the source operand is not allowed.
std::vector<int64_t> shape_vec = {1024};
AscendC::TensorShape shape(shape_vec);
uint32_t maxValue = 0;
uint32_t minValue = 0;
uint32_t typeSize = sizeof(int16_t);
AscendC::GetReduceXorSumMaxMinTmpSize(shape, typeSize, false, maxValue, minValue);
```
