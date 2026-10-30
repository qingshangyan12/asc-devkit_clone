# asc_position_quarter_mode

<!-- md-trans-meta sourceCommit=d04c597734e437e5ee87a6dd3b47f91fe427e33f translatedAt=2026-10-23T00:03:49.466Z pushedAt=2026-10-28T12:47:03.102Z nl=2 -->

`asc_position_quarter_mode` represents the quarter position of the source or destination operand, used by the Reg type conversion API to select the data position. The corresponding `ASC_DISPERSE_*_QUARTER` constants can be passed directly as API parameters.

The header file path is `"c_api/defs/enum.h"`.

## Enum Class Definition

```cpp
enum class asc_position_quarter_mode {
    DISPERSE_FIRST_QUARTER = 0,
    DISPERSE_SECOND_QUARTER,
    DISPERSE_THIRD_QUARTER,
    DISPERSE_FOURTH_QUARTER
};
```

## Enum Value Details

**Table 1** Enum value description

| Enum Value | Value | Description | Corresponding Constant |
| --- | --- | --- | --- |
| DISPERSE_FIRST_QUARTER | 0 | Selects the first quarter position. | ASC_DISPERSE_FIRST_QUARTER |
| DISPERSE_SECOND_QUARTER | 1 | Selects the second quarter position. | ASC_DISPERSE_SECOND_QUARTER |
| DISPERSE_THIRD_QUARTER | 2 | Selects the third quarter position. | ASC_DISPERSE_THIRD_QUARTER |
| DISPERSE_FOURTH_QUARTER | 3 | Selects the fourth quarter position. | ASC_DISPERSE_FOURTH_QUARTER |
