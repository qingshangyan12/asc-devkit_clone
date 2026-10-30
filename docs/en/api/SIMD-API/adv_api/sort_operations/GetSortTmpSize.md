# GetSortTmpSize

<!-- md-trans-meta sourceCommit=4c68db07c483e21d3d63ceb9ef38c2963da2162f translatedAt=2026-10-08T11:24:48.596Z pushedAt=2026-10-27T11:51:20.102Z nl=2 -->

## Description

Obtains the temporary space size required by the Sort API.

## Prototype

```
uint32_t GetSortTmpSize(const platform_ascendc::PlatformAscendC& ascendcPlatform, const uint32_t elemCount, const uint32_t dataTypeSize)
```

## Parameters

**Table 1**  API parameters

| Parameter | Input/Output | Description |
| --- | --- | --- |
| ascendcPlatform | Input | Passes the hardware platform information. For the definition of **PlatformAscendC**, see [constructor and destructor](../../../Utils-API/platform_info/PlatformAscendC/constructor_and_destructor.md). |
| elemCount | Input | Number of input elements. |
| dataTypeSize | Input | Size of the input data (in bytes). |

## Return Value

Temporary space size required by the Sort API.

## Constraints

None

## Examples

```
fe::PlatFormInfos platform_info;
auto plat = platform_ascendc::PlatformAscendC(&platform_info);
const uint32_t elemCount = 128;
AscendC::GetSortTmpSize(plat, elemCount, 4);
```
