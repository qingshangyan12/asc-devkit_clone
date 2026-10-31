# GetRintTmpBufferFactorSize

<!-- md-trans-meta sourceCommit=4c68db07c483e21d3d63ceb9ef38c2963da2162f translatedAt=2026-10-19T03:41:10.464Z pushedAt=2026-10-29T02:15:02.918Z nl=2 -->

## Description

This API is used to obtain maxLivedNodeCount and extraBuf. When the space size is fixed, maxLivedNodeCount and extraBuf can be used to derive the maximum number of elements that the operator can compute in a single computation. maxLivedNodeCount indicates how many times the temporary space is the size of the space occupied by the data of a single computation. extraBuf indicates the size of the additional temporary space used.

The following shows an example of the computation:

-   The operator implementation needs to call the Rint API. The developer reserves space of the currBuff size for it. By using the GetRintTmpBufferFactorSize API to obtain the output values maxLivedNodeCount and extraBuf, the maximum number of elements that the operator can compute in a single computation can be derived as follows:

    **currentShapeSize = \(currBuff - extraBuf\) / maxLivedNodeCount / typeSize**

-   The operator implementation needs to call two kernel-side APIs, KernelIntf1 and KernelIntf2. By using the two sets of output values \(maxLivedNodeCount and extraBuf\) from two GetXxxTmpBufferFactorSize APIs (where Xxx represents the two high-level APIs to be called) and the currently available temporary space, the maximum number of elements **currentShapeSize** that can be computed in a single computation is derived as follows:

    **currentShapeSize1 = \(currBuff - extraBuf1\) / maxLivedNodeCount1 / typeSize**

    **currentShapeSize2 = \(currBuff - extraBuf2\) / maxLivedNodeCount2 / typeSize**

    **currentShapeSize = min\(currentShapeSize1, currentShapeSize2\)****

Note that currBuff in the preceding text indicates the space available for API computation, from which the space for user input and output must be excluded. In addition, the maxLivedNodeCount value obtained by the API may be 0. During computation, you need to check that this value is not 0 to avoid a division-by-zero error.

## Prototype

```
void GetRintTmpBufferFactorSize(const platform_ascendc::PlatformAscendC& ascendcPlatform, const uint32_t typeSize, uint32_t& maxLivedNodeCount, uint32_t& extraBuf)
```

## Parameters

**Table 1**  Parameter list

| Parameter | Input/Output | Description |
| --- | --- | --- |
| ascendcPlatform | Input | Platform information input. For the definition of PlatformAscendC, see [Constructors and Destructors](../../../../Utils-API/platform_info/PlatformAscendC/constructor_and_destructor.md). |
| typeSize | Input | Size of the input data type, in bytes. For example, if the input data type is half, pass 2 here. |
| maxLivedNodeCount | Output | Maximum number of live nodes, indicating how many times the temporary space is the space occupied by the data volume of a single computation. |
| extraBuf | Output | Size of the additional temporary space used, in bytes. |

## Return Value

None

## Constraints

When currentShapeSize * typeSize derived from maxLivedNodeCount and extraBuf is less than 256B, currentShapeSize is rounded up to the value of 256B/typeSize.

## Examples

For the complete call example, see [More Examples](../more_examples.md).

```
uint32_t maxLivedNodeCount = 0;
uint32_t extraBuf = 0;
auto plat = platform_ascendc::PlatformAscendC(context->GetPlatformInfo());
AscendC::GetRintTmpBufferFactorSize(plat, typeSize, maxLivedNodeCount, extraBuf);
```
