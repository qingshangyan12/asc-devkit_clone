# GetDequantizeTmpBufferFactorSize

<!-- md-trans-meta sourceCommit=ae444a8b0a91a144a05d68a7cd8d1d0e84475d04 translatedAt=2026-10-26T10:45:17.142Z pushedAt=2026-10-29T02:14:34.529Z nl=2 -->

## Description

This API is used to obtain maxLiveNodeCount and extraBuf. Given a fixed buffer size, you can use maxLiveNodeCount and extraBuf to derive the maximum number of elements that the operator can compute in a single pass. maxLiveNodeCount indicates how many times the temporary space size exceeds the space occupied by the data volume of a single computation; extraBuf indicates the size of the additional temporary space used.

The following examples show how to derive the value:

-   The operator implementation needs to call the Dequantize API. You reserve a space of currBuff size for it, and use the GetDequantizeTmpBufferFactorSize API to obtain the output values maxLiveNodeCount and extraBuf. The maximum number of elements that the operator can compute in a single pass can then be derived as:

    **currentShapeSize = \(currBuff - extraBuf\) / maxLiveNodeCount / typeSize**

-   The operator implementation needs to call two kernel-side APIs, KernelIntf1 and KernelIntf2. Using the two sets of output values \(maxLiveNodeCount and extraBuf\) from two GetXxxTmpBufferFactorSize APIs (where Xxx represents the two high-level APIs to be called), together with the currently available temporary space, the maximum number of elements **currentShapeSize** that can be computed in a single pass is derived as:

    **currentShapeSize1 = \(currBuff - extraBuf1\) / maxLiveNodeCount1 / typeSize**

    **currentShapeSize2 = \(currBuff - extraBuf2\) / maxLiveNodeCount2 / typeSize**

    **currentShapeSize = min\(currentShapeSize1, currentShapeSize2\)****

Note that currBuff in the preceding text indicates the space available for API computation, from which the space for user input and output must be excluded. In addition, the maxLiveNodeCount value obtained by the API may be 0. When performing the computation, you must check that this value is non-zero to avoid a division-by-zero error.

## Prototype

```
void GetDequantizeTmpBufferFactorSize(const AscendC::TensorShape& srcShape, uint32_t& maxLiveNodeCnt, uint32_t& extraBuf)
```

## Parameters

**Table 1**  Parameter list

| Parameter | Input/Output | Description |
| --- | --- | --- |
| srcShape | Input | Shape information of the input srcTensor. The parameter type is [AscendC::TensorShape](../data_structures/TensorShape.md). |
| maxLiveNodeCount | Output | Maximum number of live nodes, indicating how many times the temporary space is the space occupied by the data volume of a single computation. |
| extraBuf | Output | Size of the additional temporary space used, in bytes. |

## Return Value

None

## Constraints

When the currentShapeSize  \* typeSize derived from maxLiveNodeCount and extraBuf is less than 256B, currentShapeSize is rounded up to the value of 256B/typeSize.

## Examples

```
std::vector<int64_t> shapeVec = {2, 1024};
AscendC::TensorShape srcShape(shapeVec);
uint32_t maxLiveNodeCount = 0;
uint32_t extraBuf = 0;
AscendC::GetDequantizeTmpBufferFactorSize(srcShape, maxLiveNodeCount, extraBuf);
```
