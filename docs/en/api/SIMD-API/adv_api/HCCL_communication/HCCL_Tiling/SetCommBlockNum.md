# SetCommBlockNum

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-08T12:02:55.364Z pushedAt=2026-10-27T11:47:40.237Z nl=2 -->

## Description

Sets the number of cores participating in BatchWrite communication.

## Prototype

```
uint32_t SetCommBlockNum(uint16_t num)
```

## Parameters

**Table 1**  Parameters

| Parameter | Input/Output | Description |
| --- | --- | --- |
| num | Input | Number of cores. |

## Return Value

-   0 indicates that the setting is successful.
-   A non-zero value indicates that the setting fails.

## Constraints

<!-- npu="A3" id1 -->
This API takes effect only on Atlas A3 products when the communication type is HCCL\_CMD\_BATCH\_WRITE.
<!-- end id1 -->

## Examples

```
const char* groupName = "testGroup";
uint32_t opType = HCCL_CMD_BATCH_WRITE;
std::string algConfig = "BatchWrite=level0:fullmesh";
uint32_t reduceType = 0; // BatchWrite has no reduction semantics; this is a placeholder.
AscendC::Mc2CcTilingConfig mc2CcTilingConfig(groupName, opType, algConfig, reduceType);
mc2CcTilingConfig.SetCommBlockNum(24U);
```
