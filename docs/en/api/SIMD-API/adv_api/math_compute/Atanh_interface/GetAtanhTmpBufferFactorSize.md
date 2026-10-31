# GetAtanhTmpBufferFactorSize

<!-- md-trans-meta sourceCommit=ae444a8b0a91a144a05d68a7cd8d1d0e84475d04 translatedAt=2026-10-19T02:11:47.281Z pushedAt=2026-10-29T02:15:02.514Z nl=2 -->

## Description

The computation of the Atanh API on the kernel side requires you to reserve/allocate temporary space. The maximum temporary space (maxTmpBuffer) and the space occupied by the input (inputSize \* typeSize) have the following relationship:

**maxTmpBuffer = maxLiveNodeCount \* inputSize \* typeSize + extraBuffer**

Here, maxLiveNodeCount indicates how many times the maximum temporary space is the space occupied by the input, and extraBuffer indicates the size of the additional temporary space used.

This API is used to obtain maxLiveNodeCount and extraBuffer. With a fixed space size, you can use maxLiveNodeCount and extraBuffer to calculate the maximum number of elements that the operator can compute in a single call.

The following is an example:

The operator implementation needs to call the Atanh API. You reserve a space of currBuff size for it, use the GetAtanhTmpBufferFactorSize API to obtain the output values maxLiveNodeCount and extraBuffer, and then calculate the maximum number of elements that the Atanh operator can compute in a single call as follows:

**currentShapeSize = \(currBuff - extraBuffer\) / maxLiveNodeCount / typeSize**

## Prototype

```
void GetAtanhTmpBufferFactorSize(const uint32_t typeSize, uint32_t& maxLiveNodeCount, uint32_t& extraBuf)
```

## Parameters

**Table 1**  Parameter List

| Parameter | Input/Output | Description |
| --- | --- | --- |
| typeSize | Input | Size of the input data type, in bytes. For example, if the input data type is half, pass 2 here. |
| maxLiveNodeCount | Output | Maximum number of live nodes, indicating how many times the maximum temporary space is the space occupied by the input. |
| extraBuf | Output | Size of the additional temporary space used, in bytes. |

## Return Value

None

## Constraints

When the currentShapeSize \* typeSize derived from maxLiveNodeCount and extraBuf is less than 256B, currentShapeSize is rounded up to 256B/typeSize.

## Examples

```
// Obtain the maxLiveNodeCount and extraBuffer of the Atanh operation whose input type is half.
uint32_t typeSize = sizeof(half);
uint32_t maxLiveNodeCount = 0;
uint32_t extraBuffer = 0;

AscendC::GetAtanhTmpBufferFactorSize(typeSize, maxLiveNodeCount, extraBuffer);
```
