# asc_hf32_round_mode

<!-- md-trans-meta sourceCommit=462c9a712209ad24f2d57d4b5f5dd0d416050b06 translatedAt=2026-10-23T00:02:04.778Z pushedAt=2026-10-28T12:47:03.093Z nl=2 -->

asc_hf32_round_mode is used to manage the rounding mode for converting FP32 to HF32 when HF32 mode is enabled for Mmad computation. It is used when calling [asc_set_hf32_round_mode](../../cube_compute/asc_set_hf32_round_mode.md).

## Enum Class Definition

```cpp
enum class asc_hf32_round_mode : uint8_t {
    NEAREST_AWAY = 0,
    NEAREST_EVEN = 1,
    NEAREST_ZERO = NEAREST_AWAY  // Compatible with the old name; the actual behavior is equivalent to NEAREST_AWAY.
};
```

## Enumeration Value Details

|Enumeration Value|HF32 Rounding Mode Management Policy|
|------|-----------|
| NEAREST_AWAY | FP32 is rounded to HF32 by rounding to the nearest value, with ties rounding away from zero. |
| NEAREST_EVEN | FP32 is rounded to HF32 by rounding to the nearest value, with ties rounding to even. |
| NEAREST_ZERO | Reserved for compatibility with the old implementation and not recommended. Its actual behavior is equivalent to `NEAREST_AWAY`. |
