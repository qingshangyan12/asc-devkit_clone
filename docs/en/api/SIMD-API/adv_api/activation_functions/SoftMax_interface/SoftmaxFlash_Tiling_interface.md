# SoftmaxFlash Tiling API

<!-- md-trans-meta sourceCommit=7ef8ad7bdbdb0cef0f561d391759ecb8b4e4043a translatedAt=2026-10-26T08:48:44.582Z pushedAt=2026-10-29T02:14:34.083Z nl=2 -->

## Description

**Note: This API will be deprecated soon. Do not use it in new development.**

Used to obtain SoftmaxFlash Tiling parameters.

## Prototype

-   APIs for obtaining the minimum/maximum temporary space required by the kernel function (Kernel) computation

    ```
    uint32_t GetSoftMaxFlashMaxTmpSize(const AscendC::TensorShape& srcShape, const uint32_t dataTypeSize, const bool isUpdate, const bool isReuseSource)
    ```

    ```
    uint32_t GetSoftMaxFlashMinTmpSize(const AscendC::TensorShape& srcShape, const uint32_t dataTypeSize, const bool isUpdate, const bool isReuseSource)
    ```

-   Tiling computation API
    -   Computation API in the AscendC::optiling namespace

        ```
        void SoftMaxFlashTilingFunc(const AscendC::TensorShape& srcShape, const uint32_t dataTypeSize, const uint32_t localWorkSpaceSize, optiling::SoftMaxTiling& softmaxFlashTiling, const bool isUpdate = false)
        ```

    -   Computation API in the AscendC namespace

        ```
        void SoftMaxFlashTilingFunc(const AscendC::TensorShape& srcShape, const uint32_t dataTypeSize, const uint32_t localWorkSpaceSize, AscendC::tiling::SoftMaxTiling& softmaxFlashTiling, const bool isUpdate = false)
        ```

## Parameters

**Table 1**  SoftmaxFlash GetSoftMaxFlashMaxTmpSize/GetSoftMaxFlashMinTmpSize API parameter list

| Parameter | Input/Output | Description |
| --- | --- | --- |
| srcShape | Input | Input shape information of srcTensor. The parameter type is [AscendC::TensorShape](../../data_structures/TensorShape.md). |
| dataTypeSize | Input | Data type of maxTensor and sumTensor involved in the computation, for example, half=2. |
| isUpdate | Input | Whether to enable the refresh function, consistent with the SoftmaxFlash API on the kernel side. The default value is false. |
| isReuseSource | Input | Keep consistent with the kernel-side API configuration. |

**Table 2**  SoftmaxFlash SoftMaxFlashTilingFunc API parameter list

| Parameter | Input/Output | Description |
| --- | --- | --- |
| srcShape | Input | Shape information of the input srcTensor. The parameter type is [AscendC::TensorShape](../../data_structures/TensorShape.md). |
| dataTypeSize | Input | Data type of maxTensor and sumTensor involved in the computation, for example, half=2. |
| localWorkSpaceSize | Input | Remaining space available for the SoftmaxFlash API computation, in bytes. The value of localWorkSpaceSize must be greater than the minimum temporary space size required for computation, which is returned by the GetSoftMaxFlashMinTmpSize API. |
| isUpdate | Input | Whether to enable the refresh function. This parameter is consistent with the SoftmaxFlash API on the kernel side and defaults to false. |
| softmaxFlashTiling | Output | Outputs the tiling information required by the SoftmaxFlash API. Both the optiling::SoftMaxTiling form and the AscendC::tiling::SoftMaxTiling form are supported as input parameters. |

## Return Value

GetSoftMaxFlashMaxTmpSize returns the maximum temporary space size required for the SoftmaxFlash API to complete computation, in bytes.

GetSoftMaxFlashMinTmpSize returns the minimum temporary space size required for the SoftmaxFlash API to complete computation, in bytes.

## Constraints

None
