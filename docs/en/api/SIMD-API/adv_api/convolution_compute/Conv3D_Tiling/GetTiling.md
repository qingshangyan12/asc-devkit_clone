# GetTiling

<!-- md-trans-meta sourceCommit=8d4511303e7599d86c4c8f565e5b0a328eb60b1e translatedAt=2026-10-26T09:18:55.547Z pushedAt=2026-10-29T02:14:34.185Z nl=2 -->

## Description

Obtains the tiling parameters.

## Prototype

```
int64_t GetTiling(optiling::TConv3DApiTiling& tiling)
```

```
int64_t GetTiling(AscendC::tiling::TConv3DApiTiling& tiling)
```

## Parameters

| Parameter | Input/Output | Description |
| --- | --- | --- |
| tiling | Output | Tiling structure of Conv3D, used to store the final tiling result. For details about the TConv3DApiTiling structure, see [TConv3DApiTiling Structure](TConv3DApiTiling_struct.md). |

## Return Value

If the return value is not -1, the tiling computation succeeds and you can use the values in the tiling structure. If the return value is -1, the tiling computation fails and the tiling result cannot be used.

## Constraints

Before calling the GetTiling API, you must call SetOrgInputShape, SetOrgWeightShape, SetSingleWeightShape, and SetSingleOutputShape.

## Examples

```
// Instantiate the Conv3d API.
auto ascendcPlatform = platform_ascendc::PlatformAscendC(context->GetPlatformInfo());
Conv3dTilingApi::Conv3dTiling conv3dApiTiling(ascendcPlatform);
conv3dApiTiling.SetOrgInputShape(orgCi, orgDi, orgHi, orgWi);
conv3dApiTiling.SetOrgWeightShape(cout, kd, kh, kw);
conv3dApiTiling.SetSingleWeightShape(singleCi, singleKd, singleKh, singleKw);
conv3dApiTiling.SetSingleOutputShape(singleCo, singleDo, singleM);
...
conv3dApiTiling.GetTiling(tilingData.conv3ApiTilingData);
```
