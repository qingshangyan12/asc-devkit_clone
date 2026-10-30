# GetGeluMaxMinTmpSize

<!-- md-trans-meta sourceCommit=7ef8ad7bdbdb0cef0f561d391759ecb8b4e4043a translatedAt=2026-10-26T08:28:47.043Z pushedAt=2026-10-29T02:14:34.022Z nl=2 -->

## Description

The Gelu, FasterGelu, and FasterGeluV2 APIs on the kernel side require you to reserve or allocate temporary space for computation. This API is used to obtain the maximum and minimum temporary space sizes to reserve or allocate on the host side. Based on this range, you can select an appropriate space size as a Tiling parameter to pass to the kernel side.

-   To ensure correct functionality, the reserved or allocated temporary space size must not be smaller than the minimum temporary space size.
-   Within the range from the minimum to the maximum temporary space size, as the temporary space increases, the computation performance of the kernel-side API improves to a certain extent. To achieve better performance, you can reserve or allocate space based on the actual memory usage.

## Prototype

-   Obtains the maximum temporary space size required for computation by the kernel-side API.

    ```
    uint32_t GetGeluMaxTmpSize(const AscendC::TensorShape& srcShape, const uint32_t typeSize)
    ```

-   Obtains the minimum temporary space size required for computation by the kernel-side API.

    ```
    uint32_t GetGeluMinTmpSize(const AscendC::TensorShape& srcShape, const uint32_t typeSize)
    ```

-   Obtains the maximum/minimum temporary space required for computation by the kernel-side API.

    ```
    void GetGeluMaxMinTmpSize(const AscendC::TensorShape& srcShape, const uint32_t typeSize, uint32_t& maxValue, uint32_t& minValue)
    ```

## Parameters

**Table 1**  Parameter list

| Parameter | Input/Output | Description |
| --- | --- | --- |
| srcShape | Input | Input shape information. The parameter type is [AscendC::TensorShape](../../data_structures/TensorShape.md). |
| typeSize | Input | Size of the input data type, in bytes. For example, if the input data type is half, pass 2 here. |
| maxValue | Output | Maximum temporary space size required for the Gelu, FasterGelu, and FasterGeluV2 APIs to complete computation. Space exceeding this value is not used by these APIs. Within the range from the minimum to maximum temporary space, as the temporary space increases, the computation performance of the kernel-side API improves to a certain extent. To achieve better performance, you can reserve or allocate space based on the actual memory usage. A maximum space size of 0 indicates that no temporary space is required for computation.<br>maxValue is only a reference value and may be larger than the remaining space of the Unified Buffer (UB). In this case, you need to select an appropriate temporary space size based on the remaining UB space. |
| minValue | Output | Minimum temporary space size required for the Gelu, FasterGelu, and FasterGeluV2 APIs to complete computation. To ensure correct functionality, the temporary space reserved or allocated during API computation must not be smaller than this value. A minimum space size of 0 indicates that no temporary space is required for computation. |

## Return Value

**GetGeluMaxTmpSize** returns the maximum temporary space size required for the kernel-side API to complete computation.

**GetGeluMinTmpSize** returns the minimum temporary space size required for the kernel-side API to complete computation.

**GetGeluMaxMinTmpSize** has no return value.

## Constraints

None

## Examples

```
// Input shape information is 1024; the operator input data type is half;
std::vector<int64_t> shape_vec = {1024};
AscendC::TensorShape srcShape(shape_vec);
uint32_t typeSize = 2;
uint32_t maxValue = 0;
uint32_t minValue = 0;
AscendC::GetGeluMaxMinTmpSize(srcShape, typeSize, maxValue, minValue);
```
