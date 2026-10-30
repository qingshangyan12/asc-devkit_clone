# asc_l13d_repeat_direction

<!-- md-trans-meta sourceCommit=d04c597734e437e5ee87a6dd3b47f91fe427e33f translatedAt=2026-10-23T00:02:20.767Z pushedAt=2026-10-28T12:47:03.094Z nl=2 -->

`asc_l13d_repeat_direction` indicates the repeat iteration direction of 3D img2col data movement, and serves as the `repeat_direction` parameter type of the [asc_set_l13d_rpt](../../cube_datamove/asc_set_l13d_rpt.md) and [asc_set_l13d_rpt_b](../../cube_datamove/asc_set_l13d_rpt_b.md) APIs.

The header file path is `"c_api/defs/enum.h"`.

## Enum Class Definition

```cpp
enum class asc_l13d_repeat_direction : uint8_t {
    M_DIRECTION = 0,
    K_DIRECTION = 1
};
```

## Enum Value Details

**Table 1** Enum value description

| Enum Value | Value | Repeat Iteration Direction | `repeat_stride` Unit |
| --- | --- | --- | --- |
| M_DIRECTION | 0 | Iterates along the height (M) direction. | 16 elements. |
| K_DIRECTION | 1 | Iterates along the width (K) direction. | `32 / sizeof(data_type)` elements, where `data_type` is the data type of the source operand of the 3D data movement API. |
