# SetCommEngine

<!-- md-trans-meta sourceCommit=ea8897b388b9bae7364d26841da4bdbbcadcfb65 translatedAt=2026-10-08T12:03:09.276Z pushedAt=2026-10-27T11:47:47.789Z nl=2 -->

## Description

Sets the communication engine used for communication tasks.

## Prototype

```
uint32_t SetCommEngine(uint8_t commEngine)
```

## Parameters

**Table 1**  Parameters

| Parameter | Input/Output | Description |
| --- | --- | --- |
| commEngine | Input | Communication engine. The value is of the uint8_t type. For the value range, see the description of the hcclOpExpansionMode parameter in [HcclCommConfig](https://gitcode.com/cann/hcomm/blob/9.2.0/docs/en/api_ref/comm_mgr_c/data_type_definition/HcclCommConfig.md#parameters). |

## Return Value

-   0 indicates success.
-   A non-zero value indicates failure.

## Constraints

None

## Examples

```
static ge::graphStatus AllToAllVCustomTilingFunc(gert::TilingContext* context)
{
    AllToAllVCustomV3TilingData* tiling = context->GetTilingData<AllToAllVCustomV3TilingData>();
    const std::string groupName = "testGroup";
    const std::string algConfig = "AlltoAll=level0:fullmesh;level1:pairwise";
    AscendC::Mc2CcTilingConfig mc2CcTilingConfig(groupName, HCCL_CMD_ALLTOALLV, algConfig, 0);
    mc2CcTilingConfig.SetCommEngine(1U);
    mc2CcTilingConfig.GetTiling(tiling->mc2InitTiling);
    mc2CcTilingConfig.GetTiling(tiling->mc2CcTiling);
    return ge::GRAPH_SUCCESS;
}
```
