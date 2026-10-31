# LogSoftMax Tiling

<!-- md-trans-meta sourceCommit=7ef8ad7bdbdb0cef0f561d391759ecb8b4e4043a translatedAt=2026-10-26T08:31:56.347Z pushedAt=2026-10-29T02:14:34.029Z nl=2 -->

## Description

The computation of the LogSoftMax API on the kernel side requires you to reserve/allocate temporary space. The following APIs are used to obtain the maximum and minimum temporary space sizes to reserve/allocate on the host side. Based on this range, you select an appropriate space size and call the LogSoftMaxTilingFunc function to obtain parameters such as reduceSize and splitSize, which are passed to the kernel side as Tiling parameters.

-   To ensure correct functionality, the reserved or allocated temporary space size must not be smaller than the minimum temporary space size.
-   Within the range from the minimum to the maximum temporary space size, as the temporary space increases, the computation performance of the kernel-side API improves to a certain extent. To achieve better performance, you can reserve or allocate space based on the actual memory usage.

## Prototype

-   API for obtaining the maximum/minimum temporary space required for kernel-side API computation

    ```
    uint32_t GetLogSoftMaxMaxTmpSize(const AscendC::TensorShape srcShape, const uint32_t dataTypeSize, const bool isReuseSource)
    ```

    ```
    uint32_t GetLogSoftMaxMinTmpSize(const AscendC::TensorShape srcShape, const uint32_t dataTypeSize, const bool isReuseSource)
    ```

-   Tiling computation API

    ```
    void LogSoftMaxTilingFunc(const AscendC::TensorShape srcShape, const uint32_t dataTypeSize, const uint32_t localWorkSpaceSize, optiling::LogSoftMaxTiling& softmaxTiling)
    ```

    ```
    void LogSoftMaxTilingFunc(const AscendC::TensorShape srcShape, const uint32_t dataTypeSize, const uint32_t localWorkSpaceSize, AscendC::tiling::LogSoftMaxTiling& softmaxTiling)
    ```

## Parameters

**Table 1**  Parameter list of the GetLogSoftMaxMaxTmpSize/GetLogSoftMaxMinTmpSize API

| Parameter | Input/Output | Description |
| --- | --- | --- |
| srcShape | Input | Input shape information. The parameter type is [AscendC::TensorShape](../../data_structures/TensorShape.md). |
| dataTypeSize | Input | Size of the input data type, in bytes. For example, if the input data type is half, pass 2 here. |
| isReuseSource | Input | Whether to reuse the space of the source operand input, consistent with the LogSoftMax API. |

**Table 2**  Parameter list of the LogSoftMaxTilingFunc API

| Parameter | Input/Output | Description |
| --- | --- | --- |
| srcShape | Input | Input shape information. The parameter type is [AscendC::TensorShape](../../data_structures/TensorShape.md). |
| dataTypeSize | Input | Size of the input data type, in bytes. For example, if the input data type is half, pass 2 here. |
| localWorkSpaceSize | Input | Input temporary space size. |
| softmaxTiling | Output | Tiling parameters passed to the kernel side. |

## Return Value

The GetLogSoftMaxMaxTmpSize/GetLogSoftMaxMinTmpSize API returns the maximum/minimum temporary space.

The LogSoftMaxTilingFunc API has no return value.

## Constraints

None

## Examples

```
// shape: Input shape information.
// dtypesize: Size of the input data type, in bytes.

// Define an instance of the Tiling parameter struct for the LogSoftMax operator.
AscendC::tiling::LogSoftMaxTiling tilingData;
// Whether to reuse the space of the source operand input, consistent with the LogSoftMax API.
bool isReuseSource = false;

// Obtain the maximum temporary space required for computation.
const uint32_t tmpsize = AscendC::GetLogSoftMaxMaxTmpSize(shape, dtypesize, isReuseSource);
// Compute the optimal tiling strategy and fill it into the tilingData struct for use by the kernel.
AscendC::LogSoftMaxTilingFunc(shape, dtypesize, tmpsize, tilingData);
```
