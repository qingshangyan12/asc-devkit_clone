# GetAsinhTmpBufferFactorSize

<!-- md-trans-meta sourceCommit=ae444a8b0a91a144a05d68a7cd8d1d0e84475d04 translatedAt=2026-10-19T02:05:33.633Z pushedAt=2026-10-29T02:15:02.488Z nl=2 -->

## Description

The Asinh API on the kernel side requires you to reserve/allocate temporary space during computation. The maximum temporary space (maxTmpBuffer) and the input space (inputSize \* typeSize) have the following relationship:

**maxTmpBuffer = maxLiveNodeCount \* inputSize \* typeSize + extraBuffer**

Here, maxLiveNodeCount indicates how many times the maximum temporary space is the input space, and extraBuffer indicates the size of the additional temporary space used.

This API is used to obtain maxLiveNodeCount and extraBuffer. With a fixed space size, you can use maxLiveNodeCount and extraBuffer to derive the maximum number of elements that the operator can compute in a single call. In addition, the maxLiveNodeCount value obtained by the API may be 0. You need to check that this value is non-zero during computation to avoid a division-by-zero error.

The following is an example:

The operator implementation needs to call the Asinh API. You reserve a space of currBuff size for it, use the GetAsinhTmpBufferFactorSize API to obtain the maxLiveNodeCount and extraBuffer output values, and then derive the maximum number of elements that the Asinh operator can compute in a single call as follows:

**currentShapeSize = \(currBuff - extraBuffer\) / maxLiveNodeCount / typeSize**

## Prototype

```
void GetAsinhTmpBufferFactorSize(const uint32_t typeSize, uint32_t& maxLiveNodeCount, uint32_t& extraBuf)
```

## Parameters

**Table 1**  Parameter list

| Parameter | Input/Output | Description |
| --- | --- | --- |
| typeSize | Input | Size of the input data type, in bytes. For example, if the input data type is half, pass 2 here. |
| maxLiveNodeCount | Output | Maximum number of live nodes, indicating how many times the maximum temporary space is the input space. |
| extraBuf | Output | Size of the additional temporary space used, in bytes. |

## Return Value

None

## Constraints

When the currentShapeSize  \* typeSize derived from maxLiveNodeCount and extraBuf is less than 256B, currentShapeSize is rounded up to the value of 256B/typeSize.

## Examples

```
// Obtain maxLiveNodeCount and extraBuffer for the Asinh operation with the input type half.
uint32_t typeSize = sizeof(half);
uint32_t maxLiveNodeCount = 0;
uint32_t extraBuffer = 0;

AscendC::GetAsinhTmpBufferFactorSize(typeSize, maxLiveNodeCount, extraBuffer);
```
