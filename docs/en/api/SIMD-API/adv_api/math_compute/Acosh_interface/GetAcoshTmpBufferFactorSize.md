# GetAcoshTmpBufferFactorSize

<!-- md-trans-meta sourceCommit=ae444a8b0a91a144a05d68a7cd8d1d0e84475d04 translatedAt=2026-10-19T01:58:45.273Z pushedAt=2026-10-29T02:15:02.461Z nl=2 -->

## Description

The computation of the Acosh API on the kernel side requires you to reserve/allocate temporary space. The maximum temporary space (maxTmpBuffer) and the space occupied by the input (inputSize \* typeSize) have the following relationship:

**maxTmpBuffer = maxLiveNodeCount \* inputSize \* typeSize + extraBuffer**

Here, maxLiveNodeCount indicates how many times the maximum temporary space is relative to the space occupied by the input, and extraBuffer indicates the size of the additional temporary space used.

This API is used to obtain maxLiveNodeCount and extraBuffer. With a fixed space size, the maximum number of elements that the operator can compute in a single call can be derived from maxLiveNodeCount and extraBuffer. In addition, the maxLiveNodeCount value obtained by this API may be 0. When performing the computation, you must check that this value is not 0 to avoid a division-by-zero error.

The following is an example:

The operator implementation needs to call the Acosh API. You reserve a space of currBuff size for it, use the GetAcoshTmpBufferFactorSize API to obtain the output values maxLiveNodeCount and extraBuffer, and then derive the maximum number of elements that the Acosh operator can compute in a single call as follows:

**currentShapeSize = \(currBuff - extraBuffer\) / maxLiveNodeCount / typeSize**

## Prototype

```
void GetAcoshTmpBufferFactorSize(const uint32_t typeSize, uint32_t& maxLiveNodeCount, uint32_t& extraBuf)
```

## Parameters

**Table 1**  Parameters

| Parameter | Input/Output | Description |
| --- | --- | --- |
| typeSize | Input | Size of the input data type, in bytes. For example, if the input data type is half, pass 2 here. |
| maxLiveNodeCount | Output | Maximum number of live nodes, indicating how many times the maximum temporary space is relative to the space occupied by the input. |
| extraBuf | Output | Size of the additional temporary space used, in bytes. |

## Return Value

None

## Constraints

When the currentShapeSize * typeSize derived from maxLiveNodeCount and extraBuf is smaller than 256B, currentShapeSize is rounded up to the value of 256B/typeSize.

## Examples

```
// Obtain the maxLiveNodeCount and extraBuffer of the Acosh operation with the half input type.
uint32_t typeSize = sizeof(half);
uint32_t maxLiveNodeCount = 0;
uint32_t extraBuffer = 0;

AscendC::GetAcoshTmpBufferFactorSize(typeSize, maxLiveNodeCount, extraBuffer);
```
