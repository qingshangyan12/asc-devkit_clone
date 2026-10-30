# asc_dual_dst_mode

<!-- md-trans-meta sourceCommit=d04c597734e437e5ee87a6dd3b47f91fe427e33f translatedAt=2026-10-23T00:01:58.770Z pushedAt=2026-10-28T12:47:03.091Z nl=2 -->

`asc_dual_dst_mode` specifies the dual-destination split mode for moving data out of the L0C Buffer, and serves as the type of the `dual_dst_ctrl` parameter in the matrix move-out API.

The header file path is `"c_api/defs/enum.h"`.

## Enum Class Definition

```cpp
enum class asc_dual_dst_mode : uint8_t {
    DUAL_DST_DISABLE = 0,
    DUAL_DST_SPLIT_M = 1,
    DUAL_DST_SPLIT_N = 2
};
```

## Enumeration Value Details

**Table 1** Enumeration value description

| Enumeration Value | Value | Description |
| --- | --- | --- |
| DUAL_DST_DISABLE | 0 | Disables dual-destination mode. |
| DUAL_DST_SPLIT_M | 1 | Enables dual-destination mode and splits data along the M dimension. |
| DUAL_DST_SPLIT_N | 2 | Enables dual-destination mode and splits data along the N dimension. |
