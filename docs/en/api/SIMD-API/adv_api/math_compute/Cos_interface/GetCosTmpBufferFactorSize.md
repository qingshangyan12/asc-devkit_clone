# GetCosTmpBufferFactorSize

<!-- md-trans-meta sourceCommit=7ef8ad7bdbdb0cef0f561d391759ecb8b4e4043a translatedAt=2026-10-19T02:35:00.222Z pushedAt=2026-10-29T02:15:02.616Z nl=2 -->

## Description

This API is used to obtain maxLiveNodeCount and extraBuf. When the space size is fixed, maxLiveNodeCount and extraBuf can be used to derive the maximum number of elements that the operator can compute at a time. maxLiveNodeCount indicates how many times the temporary space is the size of the space occupied by data volume of a single computation; extraBuf indicates the size of the additional temporary space used.

The following is an example of the derivation:

-   The operator implementation needs to call the Cos API. You reserve a space of currBuff size for it, and use the GetCosTmpBufferFactorSize API to obtain the output values maxLiveNodeCount and extraBuf. The maximum number of elements that the operator can compute at a time can then be derived as:

    **currentShapeSize = \(currBuff - extraBuf\) / maxLiveNodeCount / typeSize**

-   The operator implementation needs to call two kernel-side APIs, KernelIntf1 and KernelIntf2. Using the two sets of output values \(maxLiveNodeCount and extraBuf\) from two GetXxxTmpBufferFactorSize APIs (where Xxx represents the two high-level APIs to be called), together with the currently available temporary space, the maximum number of elements that can be computed at a time, currentShapeSize, is derived as:

    **currentShapeSize1 = \(currBuff - extraBuf1\) / maxLiveNodeCount1 / typeSize**

    **currentShapeSize2 = \(currBuff - extraBuf2\) / maxLiveNodeCount2 / typeSize**

    **currentShapeSize = min\(currentShapeSize1, currentShapeSize2\)****

Note that currBuff in the preceding text indicates the space available for API computation, from which the space for user input and output must be excluded. In addition, the maxLiveNodeCount value obtained by the API may be 0. When performing the computation, you must check that this value is not 0 to avoid a division-by-zero error.

## Prototype

```
void GetCosTmpBufferFactorSize(const uint32_t typeSize, uint32_t& maxLiveNodeCount, uint32_t& extraBuf)
```

```
void GetCosTmpBufferFactorSize(const CosConfig& config, const uint32_t typeSize, uint32_t& maxLiveNodeCount, uint32_t& extraBuf)
```

## Parameters

**Table 1**  Parameter List

| Parameter | Input/Output | Function |
| --- | --- | --- |
| config | Input | Cos API related configuration information. The configuration of this parameter must be consistent with that of the config template parameter of the Cos kernel function (Kernel) API. |
| typeSize | Input | Size of the input data type, in bytes. For example, if the input data type is half, pass 2 here. |
| maxLiveNodeCount | Output | Maximum number of live nodes, indicating how many times the temporary space is the space occupied by the data volume of a single computation. |
| extraBuf | Output | Size of the additional temporary space used, in bytes. |

## Return Value

None

## Constraints

When currentShapeSize \* typeSize derived from maxLiveNodeCount and extraBuf is less than 256B, currentShapeSize is rounded up to the value of 256B/typeSize.

## Examples

For the complete call example, see [More Examples](../more_examples.md).

```
uint32_t maxLiveNodeCount = 0;
uint32_t extraBuf = 0;
AscendC::GetCosTmpBufferFactorSize(typeSize, maxLiveNodeCount, extraBuf);
AscendC::CosConfig config;
config.algo = AscendC::CosAlgo::RADIAN_REDUCTION;
AscendC::GetCosTmpBufferFactorSize(config, typeSize, maxLiveNodeCount, extraBuf);
```
