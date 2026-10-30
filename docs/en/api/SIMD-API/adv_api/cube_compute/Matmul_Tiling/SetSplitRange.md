# SetSplitRange

<!-- md-trans-meta sourceCommit=ae444a8b0a91a144a05d68a7cd8d1d0e84475d04 translatedAt=2026-10-08T13:22:32.998Z pushedAt=2026-10-26T07:25:13.573Z -->

## Description

Sets the maximum and minimum values of **baseM**/**baseN**/**baseK**. Currently, **Tiling** does not support this function.

## Prototype

```
int32_t SetSplitRange(int32_t maxBaseM = -1, int32_t maxBaseN = -1, int32_t maxBaseK = -1, int32_t minBaseM = -1, int32_t minBaseN = -1, int32_t minBaseK = -1)
```

## Parameters

**Table 1**  Parameters

| Parameter | Input/Output | Description |
| --- | --- | --- |
| maxBaseM | Input | Sets the maximum **baseM** value. Default value: -1. -1 indicates that the specified maximum **baseM** value is not set, and the value is calculated by the **Tiling** function automatically. |
| maxBaseN | Input | Sets the maximum **baseN** value. Default value: -1. -1 indicates that the specified maximum **baseN** value is not set, and the value is calculated by the **Tiling** function automatically. |
| maxBaseK | Input | Sets the maximum **baseK** value. Default value: -1. -1 indicates that the specified maximum **baseK** value is not set, and the value is calculated by the **Tiling** function automatically. |
| minBaseM | Input | Sets the minimum **baseM** value. Default value: -1. -1 indicates that the specified minimum **baseM** value is not set, and the value is calculated by the **Tiling** function automatically. |
| minBaseN | Input | Sets the minimum **baseN** value. Default value: -1. -1 indicates that the specified minimum **baseN** value is not set, and the value is calculated by the **Tiling** function automatically. |
| minBaseK | Input | Sets the minimum **baseK** value. Default value: -1. -1 indicates that the specified minimum **baseK** value is not set, and the value is calculated by the **Tiling** function automatically. |

## Return Value

-1 indicates setting failure; 0 indicates setting success.

## Constraints

If **baseM**/**baseN**/**baseK** are not aligned to C0\_size, the value is aligned to C0\_size when computing **Tiling**. Note that the half/bfloat16\_t data type has a C0\_size of 16, the float data type has a C0\_size of 8, the int8\_t data type has a C0\_size of 32, and the int4b\_t data type has a C0\_size of 64.
