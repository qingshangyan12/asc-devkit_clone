# GetGeGLUTmpBufferFactorSize

<!-- md-trans-meta sourceCommit=ae444a8b0a91a144a05d68a7cd8d1d0e84475d04 translatedAt=2026-10-26T08:25:10.433Z pushedAt=2026-10-29T02:14:34.002Z nl=2 -->

## Description

The computation of the GeGLU API on the kernel side requires you to reserve/allocate temporary space. The maximum temporary space (maxTmpBuffer) and the space occupied by the input (inputSize \* typeSize) have the following relationship:

**maxTmpBuffer = maxLiveNodeCount \* inputSize \* typeSize + extraBuf**

Here, maxLiveNodeCount indicates how many times the maximum temporary space is the space occupied by the input, and extraBuf indicates the size of the additional temporary space used.

This API is used to obtain maxLiveNodeCount and extraBuf. With a fixed space size, you can use maxLiveNodeCount and extraBuf to calculate the maximum number of elements that the operator can compute in a single call.

The following is an example:

The operator implementation needs to call the GeGLU API. You reserve a buffer of currBuff size for it, and use the GetGeGLUTmpBufferFactorSize API to obtain the output values maxLiveNodeCount and extraBuf. The maximum number of elements that the GeGLU operator can compute in a single pass can then be derived as:

**currentShapeSize = \(currBuff - extraBuf\) / maxLiveNodeCount / typeSize**

## Prototype

```
void GetGeGLUTmpBufferFactorSize(const uint32_t typeSize, uint32_t& maxLiveNodeCount, uint32_t& extraBuf)
```

## Parameters

**Table 1**  Parameter list

| Parameter | Input/Output | Description |
| --- | --- | --- |
| typeSize | Input | Size of the input data type, in bytes. For example, if the input data type is half, pass 2 here. |
| maxLiveNodeCount | Output | Maximum number of live nodes, indicating how many times the maximum temporary space is the space occupied by the input. |
| extraBuf | Output | Size of the additional temporary space used, in bytes. |

## Return Value

None

## Constraints

When the currentShapeSize  \* typeSize derived from maxLiveNodeCount and extraBuf is less than 256B, currentShapeSize is rounded up to the value of 256B/typeSize.

## Examples

```
uint32_t typeSize = sizeof(half);
uint32_t maxLiveNodeCount = 0;
uint32_t extraBuf = 0;

AscendC::GetGeGLUTmpBufferFactorSize(typeSize, maxLiveNodeCount, extraBuf);
```
