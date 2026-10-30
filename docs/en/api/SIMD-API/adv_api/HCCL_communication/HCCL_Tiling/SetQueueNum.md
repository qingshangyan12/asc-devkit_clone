# SetQueueNum

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-08T12:05:07.472Z pushedAt=2026-10-27T11:48:23.914Z nl=2 -->

## Description

Sets the number of BatchWrite communication queues on each core that sends tasks to the server.

## Prototype

```
uint32_t SetQueueNum(uint16_t num)
```

## Parameters

**Table 1**  Parameters

| Parameter | Input/Output | Description |
| --- | --- | --- |
| num | Input | Number of queues. The supported value range of the number of cores participating in communication * the number of queues is [0, 40]. For details about setting the number of cores participating in communication, see [SetCommBlockNum](SetCommBlockNum.md). |

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
uint32_t reduceType = 0; // BatchWrite has no reduction semantics. This is a placeholder.
AscendC::Mc2CcTilingConfig mc2CcTilingConfig(groupName, opType, algConfig, reduceType);
mc2CcTilingConfig.SetQueueNum(2U);
```
