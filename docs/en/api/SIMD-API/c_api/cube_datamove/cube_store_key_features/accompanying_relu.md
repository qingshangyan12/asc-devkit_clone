# Accompanying ReLU

<!-- md-trans-meta sourceCommit=5df1d1c781b51fcb7826db7f2afab5f8bc453e93 translatedAt=2026-10-22T12:53:25.651Z pushedAt=2026-10-28T12:45:45.424Z nl=2 -->

## Description

The accompanying ReLU capability is supported during the move-out process of cube computation. The following accompanying ReLU capabilities are currently supported.

| `asc_relu_pre_mode` Enumeration Value | Encoding | Function and Configuration |
| --- | --- | --- |
| `NONE` | 0 | Disables activation. |
| `NORMAL` | 1 | Normal ReLU, with a coefficient of 0 on the negative half-axis. |
| `SCALAR` | 2 | The entire cube shares the activation coefficient, configured using [asc_set_l0c_copy_relu_alpha](../asc_set_l0c_copy_relu_alpha.md). |
| `VECTOR` | 3 | Each N channel has an independent activation coefficient. Move the coefficients in first, and then configure `relu_pre_addr` using [asc_set_l0c_copy_config](../asc_set_l0c_copy_config.md). |

For details about using it in combination with accompanying quantization, see [Accompanying Quantization and Accompanying ReLU Scenario Combination](accompanying_quantization_and_relu_scenario_combination.md).
