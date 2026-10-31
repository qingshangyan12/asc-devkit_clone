# asc_unit_flag_mode

<!-- md-trans-meta sourceCommit=d04c597734e437e5ee87a6dd3b47f91fe427e33f translatedAt=2026-10-23T00:05:29.010Z pushedAt=2026-10-28T12:47:03.107Z nl=2 -->

`asc_unit_flag_mode` configures the UnitFlag behavior after a matrix computation or matrix move-out API is executed, and serves as the type of the `unit_flag_mode` parameter in the related API.

The header file path is `"c_api/defs/enum.h"`.

## Enum Class Definition

```cpp
enum class asc_unit_flag_mode : uint8_t {
    DISABLE = 0,
    ENABLE_KEEP = 2,
    ENABLE_UPDATE = 3
};
```

## Enumeration Value Details

**Table 1** Enumeration value description

| Enumeration Value | Value | Description |
| --- | --- | --- |
| DISABLE | 0 | Disables UnitFlag. |
| ENABLE_KEEP | 2 | Enables UnitFlag. The hardware does not change the unit flag after executing the instruction. |
| ENABLE_UPDATE | 3 | Enables UnitFlag. The hardware changes the unit flag after executing the instruction. |
