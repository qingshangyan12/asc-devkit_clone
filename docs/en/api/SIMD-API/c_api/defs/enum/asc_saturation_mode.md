# asc_saturation_mode

<!-- md-trans-meta sourceCommit=2cfabc56ee194582a5ce4e70e566331ab4c978a5 translatedAt=2026-10-23T00:04:29.821Z pushedAt=2026-10-28T12:47:03.105Z nl=2 -->

asc_saturation_mode indicates the mode type on which saturation control acts. It is used when calling [asc_set_saturation_flag](../../spr/asc_set_saturation_flag.md) and [asc_get_saturation_flag](../../spr/asc_get_saturation_flag.md).

## Enum Class Specific Definition

```cpp
enum class asc_saturation_mode : uint8_t {
    FLOAT,
    FLOAT8,
    INT,
    CAST
};
```

## Enum Value Description

**Table 1** Enum value description

| Enum Value | Description |
| ------ | ----------- |
| FLOAT | Saturation control for floating-point computation and some floating-point precision conversion scenarios. Floating-point computation supports the half data type; floating-point precision conversion supports the hifloat8_t, fp8_e8m0_t, fp8_e5m2_t, fp8_e4m3fn_t, half, and bfloat16_t data types. |
| FLOAT8 | Saturation control for scenarios related to fp8_e8m0_t, fp8_e5m2_t, and fp8_e4m3fn_t. |
| INT | Saturation/truncation control for integer computation overflow. |
| CAST | Precision conversion saturation control for floating-point-to-integer or integer-to-integer conversion. It takes effect only when the saturation strategy is [asc_override_strategy::USE_GLOBAL](asc_override_strategy.md). |

For the behavioral differences between enabling and disabling saturation in each mode, see the description of the `enable_sat` parameter in [asc_set_saturation_flag](../../spr/asc_set_saturation_flag.md).
