# IsBasicBlockInSoftMax

<!-- md-trans-meta sourceCommit=ae444a8b0a91a144a05d68a7cd8d1d0e84475d04 translatedAt=2026-10-26T08:41:03.220Z pushedAt=2026-10-29T02:14:34.067Z nl=2 -->

## Description

Determines whether a SoftMaxTiling structure conforms to the basic block characteristics.

## Prototype

-   Compute API in the AscendC::optiling namespace

    ```
    bool IsBasicBlockInSoftMax(optiling::SoftMaxTiling& tiling, const uint32_t dataTypeSize = 2)
    ```

-   Compute API in the AscendC namespace

    ```
    bool IsBasicBlockInSoftMax(AscendC::tiling::SoftMaxTiling& tiling, const uint32_t dataTypeSize = 2)
    ```

## Parameters

**Table 1**  Parameter list

| Parameter | Input/Output | Description |
| --- | --- | --- |
| tiling | Input | SoftMaxTiling structure to be determined, which supports the optiling::SoftMaxTiling form and the AscendC::tiling::SoftMaxTiling form. |
| dataTypeSize | Input | Data type size of the srcTensor involved in the computation, for example, half=2. |

## Return Value

-   Returns true if the SoftMaxTiling structure conforms to the basic block Tiling feature.
-   Returns false if the SoftMaxTiling structure does not conform to the basic block Tiling feature.

## Constraints

None
