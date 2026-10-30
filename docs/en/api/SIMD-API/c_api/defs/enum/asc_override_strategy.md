# asc_override_strategy

<!-- md-trans-meta sourceCommit=2cfabc56ee194582a5ce4e70e566331ab4c978a5 translatedAt=2026-10-23T00:03:17.222Z pushedAt=2026-10-28T12:47:03.099Z nl=2 -->

asc_override_strategy represents the saturation control strategy and is used when calling [asc_set_saturation_strategy](../../spr/asc_set_saturation_strategy.md) and [asc_get_saturation_strategy](../../spr/asc_get_saturation_strategy.md). This strategy determines whether the CAST-type global saturation configuration takes effect.

## Enum Class Definition

```cpp
enum class asc_override_strategy : uint8_t {
    USE_API = 0,
    USE_GLOBAL = 1
};
```

## Enum Value Details

**Table 1** Enum value description

| Enum Value | Description |
| ------ | ----------- |
| USE_API | Configures saturation per instruction/API path. In this case, the global `CAST` saturation configured through [asc_set_saturation_flag](../../spr/asc_set_saturation_flag.md) does not take effect. |
| USE_GLOBAL | Uses the global saturation configuration. In this case, the `CAST` mode configured through [asc_set_saturation_flag](../../spr/asc_set_saturation_flag.md) takes effect in precision conversion. |

Typical usage: `USE_GLOBAL` works with CAST saturation for float→int32 dynamic quantization. See the call example in [asc_set_saturation_strategy](../../spr/asc_set_saturation_strategy.md) and the [Cast example](../../../../../../../examples/02_simd_c_api/03_c_api/02_reg_vector_compute/cast). `USE_API` works with per-instruction saturation. See the `SatMode` and the configuration table for global or per-instruction saturation mode in [Cast](../../../basic_api/reg_vector_compute/type_conversion/Cast.md).
