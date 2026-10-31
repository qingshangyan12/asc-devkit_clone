# SetScaleAType

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-08T13:18:38.440Z pushedAt=2026-10-27T11:39:59.798Z nl=2 -->

## Description

In the MxMatmul scenario, this API sets the position, data format, and transpose information of the scaleA cube. These settings must be consistent with those on the kernel function (Kernel) side. If this API is not called, the scaleA cube information remains consistent with the A cube information set in [SetAType](SetAType.md).

## Prototype

```
int32_t SetScaleAType(TPosition scalePos, CubeFormat scaleType, bool isScaleTrans = false)
```

## Parameters

**Table 1**  Parameters

| Parameter | Input/Output | Description |
| --- | --- | --- |
| scalePos | Input | Memory logical position of the scaleA cube. <!-- npu="950" id1 --><br><br>For Ascend 950PR&950DT products, the scaleA cube can be set to **TPosition::GM**, **TPosition::VECOUT**, or **TPosition::TSCM**. <!-- end id1 --> |
| scaleType | Input | Physical layout format of the scaleA cube. <!-- npu="950" id2 --><br><br>For Ascend 950PR&950DT products, the scaleA cube can be set to **CubeFormat::ND** or **CubeFormat::NZ**. <!-- end id2 --> |
| isScaleTrans<a name="p2934103115919"></a> | Input | Whether the scaleA cube is transposed. Supported values are as follows:<br>**true**: The scaleA cube is transposed.<br>**false**: Default value. The scaleA cube is not transposed. |

## Return Value

-1 indicates a setting failure; 0 indicates a setting success.

## Constraints

None

## Examples

```
auto ascendcPlatform = platform_ascendc::PlatformAscendC(context->GetPlatformInfo());
matmul_tiling::MatmulApiTiling tiling(ascendcPlatform);
tiling.SetAType(matmul_tiling::TPosition::GM, matmul_tiling::CubeFormat::ND, matmul_tiling::DataType::DT_FLOAT16);
// Set the scaleA cube with the buffer position as GM, the data format as ND, and no transpose.
tiling.SetScaleAType(matmul_tiling::TPosition::GM, matmul_tiling::CubeFormat::ND, false);
```
