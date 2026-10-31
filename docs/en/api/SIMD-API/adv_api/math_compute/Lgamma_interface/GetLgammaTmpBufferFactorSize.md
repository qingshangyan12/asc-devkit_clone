# GetLgammaTmpBufferFactorSize

<!-- md-trans-meta sourceCommit=ae444a8b0a91a144a05d68a7cd8d1d0e84475d04 translatedAt=2026-10-19T03:16:39.389Z pushedAt=2026-10-29T02:15:02.800Z nl=2 -->

## Description

This API is used to obtain maxLiveNodeCount and extraBuf. With a fixed buffer size, maxLiveNodeCount and extraBuf can be used to derive the maximum number of elements that the operator can compute in a single call. maxLiveNodeCount indicates how many times the temporary space is the size of the space occupied by the data computed in a single call; extraBuf indicates the size of the additional temporary space used.

The derivation examples are as follows:

-   The operator implementation needs to call the Lgamma API. You reserve a buffer of currBuff size for it, and use the GetLgammaTmpBufferFactorSize API to obtain the output values maxLiveNodeCount and extraBuf. The maximum number of elements that the operator can compute in a single call can then be derived as:

    **currentShapeSize = \(currBuff - extraBuf\) / maxLiveNodeCount / typeSize**

-   The operator implementation needs to call two kernel-side APIs, KernelIntf1 and KernelIntf2. Using the two sets of output values \(maxLiveNodeCount and extraBuf\) from the two GetXxxTmpBufferFactorSize APIs (where Xxx is the two high-order APIs to be called) and the currently available temporary space, the maximum number of elements that can be computed in a single call, currentShapeSize, is derived as:

    **currentShapeSize1 = \(currBuff - extraBuf1\) / maxLiveNodeCount1 / typeSize**

    **currentShapeSize2 = \(currBuff - extraBuf2\) / maxLiveNodeCount2 / typeSize**

    **currentShapeSize = min\(currentShapeSize1, currentShapeSize2\)****

Note that currBuff in the preceding text indicates the buffer available for API computation, from which the user input and output buffers must be excluded. In addition, the maxLiveNodeCount value obtained by the API may be 0. When performing the computation, check that this value is not 0 to avoid a division-by-zero error.

## Prototype

```
void GetLgammaTmpBufferFactorSize(const uint32_t typeSize, uint32_t& maxLiveNodeCount, uint32_t& extraBuffer)
```

## Parameters

**Table 1** Parameter List

| Parameter | Input/Output | Function |
| --- | --- | --- |
| typeSize | Input | Size of the input data type, in bytes. For example, if the input data type is half, pass 2 here. |
| maxLiveNodeCount | Output | Maximum number of live nodes, indicating how many times the temporary space is the size of the space occupied by a single computation's data volume. |
| extraBuffer | Output | Size of the additional temporary space used, in bytes. |

## Return Value

None

## Constraints

When the currentShapeSize \* typeSize derived from maxLiveNodeCount and extraBuffer is less than 256B, currentShapeSize is rounded up to the value of 256B/typeSize.

## Examples

For the complete call example, see [More Examples](../more_examples.md).

```
uint32_t maxLiveNodeCount = 0;
uint32_t extraBuf = 0;
AscendC::GetLgammaTmpBufferFactorSize(typeSize, maxLiveNodeCount, extraBuf);
```
