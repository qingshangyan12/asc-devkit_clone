# SoftMax/SimpleSoftMax Tiling

<!-- md-trans-meta sourceCommit=7ef8ad7bdbdb0cef0f561d391759ecb8b4e4043a translatedAt=2026-10-26T08:44:59.686Z pushedAt=2026-10-29T02:14:34.080Z nl=2 -->

## Description

Used to obtain the SoftMax/SimpleSoftMax Tiling parameters.

## Prototype

-   API for obtaining the maximum/minimum temporary space required by the kernel API for computation

    ```
    uint32_t GetSoftMaxMaxTmpSize(const AscendC::TensorShape& srcShape, const uint32_t dataTypeSize, const bool isReuseSource)
    ```

    ```
    uint32_t GetSoftMaxMinTmpSize(const AscendC::TensorShape& srcShape, const uint32_t dataTypeSize, const bool isReuseSource)
    ```

-   Tiling computation API
    -   Computation API in the AscendC::optiling namespace

        ```
        void SoftMaxTilingFunc(const AscendC::TensorShape& srcShape, const uint32_t dataTypeSize, const uint32_t localWorkSpaceSize, optiling::SoftMaxTiling& softmaxTiling)
        ```

    -   Computation API in the AscendC namespace

        ```
        void SoftMaxTilingFunc(const AscendC::TensorShape& srcShape, const uint32_t dataTypeSize, const uint32_t localWorkSpaceSize, AscendC::tiling::SoftMaxTiling& softmaxTiling)
        ```

## Parameters

**Table 1**  SoftMax/SimpleSoftMax GetSoftMaxMaxTmpSize/GetSoftMaxMinTmpSize API parameter list

| Parameter  | Input/Output | Description |
| --- | --- | --- |
| srcShape | Input | Input shape information of srcTensor. The parameter type is [AscendC::TensorShape](../../data_structures/TensorShape.md). |
| dataTypeSize | Input | Data type of max and sum involved in the computation, for example, half=2. |
| isReuseSource | Input | Keep consistent with the configuration on the kernel side. |

**Table 2**  SoftMax/SimpleSoftMax SoftMaxTilingFunc API parameter list

| Parameter  | Input/Output | Description |
| --- | --- | --- |
| srcShape | Input | Input shape information of srcTensor. The parameter type is [AscendC::TensorShape](../../data_structures/TensorShape.md). |
| dataTypeSize | Input | Data type of max and sum involved in the computation, for example, half=2. |
| localWorkSpaceSize | Input | Remaining space available for the SoftMax API computation. The unit is Byte. The value of localWorkSpaceSize must be greater than the minimum temporary space size required for computation returned by the GetSoftMaxMinTmpSize API. |
| softmaxTiling | Output | Outputs the tiling information required by the SoftMax API. Supports the optiling::SoftMaxTiling form and the AscendC::tiling::SoftMaxTiling form. |

## Return Value

GetSoftMaxMaxTmpSize returns the maximum temporary space size required for the SoftMax/SimpleSoftMax API to complete computation. The unit is Byte.

GetSoftMaxMinTmpSize returns the minimum temporary space size required for the SoftMax/SimpleSoftMax API to complete computation. The unit is Byte.

## Constraints

None
