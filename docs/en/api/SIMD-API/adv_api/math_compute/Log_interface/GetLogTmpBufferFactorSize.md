# GetLogTmpBufferFactorSize

<!-- md-trans-meta sourceCommit=ae444a8b0a91a144a05d68a7cd8d1d0e84475d04 translatedAt=2026-10-19T03:18:24.150Z pushedAt=2026-10-29T02:15:02.810Z nl=2 -->

## Description

This API is used to obtain maxLiveNodeCount and extraBuf. With a fixed buffer size, maxLiveNodeCount and extraBuf can be used to derive the maximum number of elements that can be computed in a single pass. maxLiveNodeCount indicates how many times the temporary space size is relative to the buffer occupied by the data volume of a single computation. extraBuf indicates the size of the additional temporary space used.

The following is an example of the computation:

-   The operator implementation needs to call the Log API. You reserve a buffer of currBuff size for it. By using the GetLogTmpBufferFactorSize API to obtain the output values maxLiveNodeCount and extraBuf, you can derive the maximum number of elements that can be computed in a single pass as follows:

    **currentShapeSize = \(currBuff - extraBuf\) / maxLiveNodeCount / typeSize**

-   The operator implementation needs to call two kernel-side APIs, KernelIntf1 and KernelIntf2. Using the two sets of output values \(maxLiveNodeCount and extraBuf\) from the two GetXxxTmpBufferFactorSize APIs (where Xxx represents the two high-level APIs to be called) together with the currently available temporary space, you can derive the maximum number of elements that can be computed in a single pass, currentShapeSize, as follows:

    **currentShapeSize1 = \(currBuff - extraBuf1\) / maxLiveNodeCount1 / typeSize**

    **currentShapeSize2 = \(currBuff - extraBuf2\) / maxLiveNodeCount2 / typeSize**

    **currentShapeSize = min\(currentShapeSize1, currentShapeSize2\)****

Note that currBuff in the preceding text indicates the buffer available for the API computation, from which the space occupied by user input and output must be excluded. In addition, the maxLiveNodeCount value obtained by the API may be 0. When performing the computation, check that this value is not 0 to avoid a division-by-zero error.

## Prototype

```
void GetLogTmpBufferFactorSize(const uint32_t typeSize, uint32_t& maxLiveNodeCount, uint32_t& extraBuf)
```

```
void GetLog2TmpBufferFactorSize(const uint32_t typeSize, uint32_t& maxLiveNodeCount, uint32_t& extraBuf)
```

```
void GetLog10TmpBufferFactorSize(const uint32_t typeSize, uint32_t& maxLiveNodeCount, uint32_t& extraBuf)
```

## Parameters

**Table 1** Parameter list

| Parameter | Input/Output | Description |
| --- | --- | --- |
| typeSize | Input | Size of the input data type, in bytes. For example, if the input data type is half, pass 2 here. |
| maxLiveNodeCount | Output | Maximum number of live nodes, indicating how many times the temporary space is the size of the space occupied by the data volume of a single computation. |
| extraBuf | Output | Size of the additional temporary space used, in bytes. |

## Return Value

None

## Constraints

-   The current Log APIs (including Log2/Log10) do not require extraBuf. maxLiveNodeCount is currently required only when the src type of the Log2 API is half.
-   When the currentShapeSize * typeSize derived from extraBuf is less than 256B, currentShapeSize is rounded up to 256B/typeSize.

## Examples

For the complete call example, see [More Examples](../more_examples.md).

```
uint32_t maxLiveNodeCount = 0;
uint32_t extraBuf = 0;
uint32_t typeSize = sizeof(half);
AscendC::GetLogTmpBufferFactorSize(typeSize, maxLiveNodeCount, extraBuf);
```
