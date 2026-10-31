# GetOffsetC

<!-- md-trans-meta sourceCommit=ae444a8b0a91a144a05d68a7cd8d1d0e84475d04 translatedAt=2026-10-08T12:23:15.908Z pushedAt=2026-10-17T11:37:44.484Z -->

## Description

Reserved API for future functionality.

Obtains the position of the current tile in the entire C cube for this computation.

## Prototype

```
__aicore__ inline MatrixOffset GetOffsetC()
```

## Parameters

None

## Return Value

The **MatrixOffset** structure is as follows:

```
struct MatrixOffset {
    int32_t offset;
    int32_t row, col;
    int32_t height, width;
};
```

## Constraints

None

