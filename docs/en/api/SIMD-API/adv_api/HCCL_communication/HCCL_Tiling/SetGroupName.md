# SetGroupName

<!-- md-trans-meta sourceCommit=ae444a8b0a91a144a05d68a7cd8d1d0e84475d04 translatedAt=2026-10-08T12:04:20.271Z pushedAt=2026-10-27T11:48:04.268Z nl=2 -->

## Description

Sets the communication domain where the communication task resides.

## Prototype

```
uint32_t SetGroupName(const std::string& groupName)
```

## Parameters

**Table 1**  Parameters

| Parameter | Input/Output | Description |
| --- | --- | --- |
| groupName | Input | Communication domain where the current communication task resides. String type, with a maximum supported length of 128 bytes. |

## Return Value

-   0 indicates that the setting is successful.
-   A non-zero value indicates that the setting failed.

## Constraints

None

## Examples

For a call example of this API, see [Examples](SetOpType.md#examples).
