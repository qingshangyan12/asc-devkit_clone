# GetSortOffset

<!-- md-trans-meta sourceCommit=ae444a8b0a91a144a05d68a7cd8d1d0e84475d04 translatedAt=2026-10-08T11:24:34.919Z pushedAt=2026-10-27T11:51:12.125Z nl=2 -->

## Description

Obtains the corresponding offset (in bytes) in the Sort data based on the element position.

## Prototype

```
template <typename T>
__aicore__ inline uint32_t GetSortOffset(const uint32_t elemOffset)
```

## Parameters

**Table 1** Template parameters

| Parameter | Description |
| --- | --- |
| T | Data type of the operand. Supported data types: half and float. |

**Table 2** API parameter list

| Parameter | Input/Output | Description |
| --- | --- | --- |
| elemOffset | Input | Element position. |

## Return Value

Returns the corresponding offset in the Sort data.

## Constraints

None

## Examples

```
AscendC::GetSortOffset<half>(128);
```
