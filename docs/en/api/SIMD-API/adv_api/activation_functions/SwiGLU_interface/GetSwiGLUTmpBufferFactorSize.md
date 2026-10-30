# GetSwiGLUTmpBufferFactorSize

<!-- md-trans-meta sourceCommit=ae444a8b0a91a144a05d68a7cd8d1d0e84475d04 translatedAt=2026-10-26T09:01:06.695Z pushedAt=2026-10-29T02:14:34.122Z nl=2 -->

## Description

The SwiGLU API on the kernel side requires you to reserve/allocate temporary space during computation. The maximum temporary space (maxTmpBuffer) and the input space (inputSize \* typeSize) have the following relationship:

**maxTmpBuffer = maxLiveNodeCount \* inputSize \* typeSize + extraBuffer**

Here, maxLiveNodeCount indicates how many times the maximum temporary space is the space occupied by the input, and extraBuffer indicates the size of the additional temporary space used.

This API is used to obtain maxLiveNodeCount and extraBuffer. With a fixed space size, you can use maxLiveNodeCount and extraBuffer to calculate the maximum number of elements that the operator can compute in a single call.

The following is an example:

The operator implementation needs to call the SwiGLU API. You reserve a space of currBuff size for it, use the GetSwiGLUTmpBufferFactorSize API to obtain the maxLiveNodeCount and extraBuffer output values, and then derive the maximum number of elements that the SwiGLU operator can compute in a single call as follows:

**currentShapeSize = \(currBuff - extraBuffer\) / maxLiveNodeCount / typeSize**

## Prototype

```
void GetSwiGLUTmpBufferFactorSize(const uint32_t typeSize, uint32_t& maxLiveNodeCount, uint32_t& extraBuffer)
```

## Parameters

**Table 1**  Parameter list

| API | Input/Output | Description |
| --- | --- | --- |
| typeSize | Input | Size of the input data type, in bytes. For example, if the input data type is half, pass 2 here. |
| maxLiveNodeCount | Output | Maximum number of live nodes, indicating how many times the maximum temporary space is the space occupied by the input. |
| extraBuffer | Output | Size of the additional temporary space used, in bytes. |

## Return Value

None

## Constraints

When the currentShapeSize * typeSize derived from maxLiveNodeCount and extraBuffer is less than 256B, currentShapeSize is rounded up to the value of 256B/typeSize.

## Examples

```
uint32_t typeSize = sizeof(half);
uint32_t maxLiveNodeCount = 0;
uint32_t extraBuffer = 0;

AscendC::GetSwiGLUTmpBufferFactorSize(typeSize, maxLiveNodeCount, extraBuffer);
```
