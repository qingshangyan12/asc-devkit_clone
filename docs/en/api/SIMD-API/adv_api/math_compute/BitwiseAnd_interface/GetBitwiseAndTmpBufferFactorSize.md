# GetBitwiseAndTmpBufferFactorSize

<!-- md-trans-meta sourceCommit=4c68db07c483e21d3d63ceb9ef38c2963da2162f translatedAt=2026-10-19T02:17:35.376Z pushedAt=2026-10-29T02:15:02.540Z nl=2 -->

## Description

This API is used to obtain maxLivedNodeCount and extraBuf. With a fixed space size, maxLivedNodeCount and extraBuf can be used to derive the maximum number of elements that can be computed in a single pass. maxLivedNodeCount indicates how many times the temporary space is the size of the space occupied by the data volume of a single computation; extraBuf indicates the size of the additional temporary space used.

The following shows an example of the computation:

-   The operator implementation needs to call the BitwiseAnd API. You reserve a space of currBuff size for it, and use the GetBitwiseAndTmpBufferFactorSize API to obtain the output values maxLivedNodeCount and extraBuf. The maximum number of elements that can be computed in a single pass can then be derived as follows:

    **currentShapeSize = \(currBuff - extraBuf\) / maxLivedNodeCount / typeSize**

-   The operator implementation needs to call two kernel-side APIs, KernelIntf1 and KernelIntf2. Using the two sets of output values \(maxLivedNodeCount and extraBuf\) from the two GetXxxTmpBufferFactorSize APIs (where Xxx represents the two high-level APIs to be called) and the currently available temporary space, the maximum number of elements that can be computed in a single pass, currentShapeSize, is derived as follows:

    **currentShapeSize1 = \(currBuff - extraBuf1\) / maxLivedNodeCount1 / typeSize**

    **currentShapeSize2 = \(currBuff - extraBuf2\) / maxLivedNodeCount2 / typeSize**

    **currentShapeSize = min\(currentShapeSize1, currentShapeSize2\)****

Note that currBuff in the preceding text indicates the space available for API computation, from which the space for user input and output must be excluded. In addition, the maxLivedNodeCount value obtained by the API may be 0. When performing the computation, check that this value is not 0 to avoid a division-by-zero error.

## Prototype

```
void GetBitwiseAndTmpBufferFactorSize(const platform_ascendc::PlatformAscendC& ascendcPlatform, const uint32_t typeSize, uint32_t& maxLivedNodeCount, uint32_t& extraBuf)
```

## Parameters

**Table 1**  Parameter list

| Parameter | Input/Output | Description |
| --- | --- | --- |
| ascendcPlatform | Input | Platform information input. For the definition of PlatformAscendC, see [Constructors and Destructors](../../../../Utils-API/platform_info/PlatformAscendC/constructor_and_destructor.md). |
| typeSize | Input | Size of the input data type, in bytes. For example, if the input data type is uint16_t, pass 2 here. |
| maxLivedNodeCount | Output | Maximum number of live nodes, indicating the ratio of the temporary space to the space occupied by the data volume of a single computation. |
| extraBuf | Output | Size of the additional temporary space used, in bytes. |

## Return Value

None

## Constraints

When the currentShapeSize  \* typeSize derived from maxLivedNodeCount and extraBuf is less than 256B, currentShapeSize is rounded up to the value of 256B/typeSize.

## Examples

For the complete call example, see [More Examples](../more_examples.md).

```
uint32_t maxLivedNodeCount = 0;
uint32_t extraBuf = 0;
auto plat = platform_ascendc::PlatformAscendC(context->GetPlatformInfo());
AscendC::GetBitwiseAndTmpBufferFactorSize(plat, typeSize, maxLivedNodeCount, extraBuf);
```
