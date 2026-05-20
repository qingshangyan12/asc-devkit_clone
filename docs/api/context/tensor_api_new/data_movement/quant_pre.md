# 随路量化

随路量化是矩阵计算结果从 L0C 搬出到 GM 或 UB 过程中的数据转换能力。L0C 到 GM/UB 搬运根据是否传入 `quant` 参数自动选择不量化、直接 cast、scalar 量化或 tensor 量化模式。

## 调用形式

不传入 `quant` 参数时，接口根据源类型和目的类型选择不量化输出或直接 cast 输出：

```cpp
Copy(dst, src, fixpipeParams);
```

传入 `uint64_t` 类型的 `quant` 参数时，表示 scalar 量化。整个 C 矩阵共用同一个量化参数：

```cpp
uint64_t quant = 1;
Copy(dst, src, quant, fixpipeParams);
```

传入 Tensor API 张量作为 `quant` 参数时，表示 tensor 量化。量化参数通常位于 L1，元素类型为 `uint64_t`，每个 channel 维度对应一个量化参数：

```cpp
auto quant = MakeTensor(MakeMemPtr<Location::L1>(quantAddr),
    MakeFrameLayout<NDExtLayoutPtn, LayoutTraitDefault<uint64_t>>(1, n));
Copy(dst, src, quant, fixpipeParams);
```

## 支持模式

| 源类型 | 目的类型 | 不传 `quant` | 传入 scalar `quant` | 传入 tensor `quant` |
| :--- | :--- | :--- | :--- | :--- |
| `float` | `float` | 不量化输出 | `QF322F32_PRE` | `VQF322F32_PRE` |
| `int32_t` | `int32_t` | 不量化输出 | 不支持 | 不支持 |
| `float` | `half` | `F322F16` 直接 cast | `QF322F16_PRE` | `VQF322F16_PRE` |
| `float` | `bfloat16_t` | `F322BF16` 直接 cast | `QF322BF16_PRE` | `VQF322BF16_PRE` |
| `float` | `int8_t`、`uint8_t` | 不支持 | `QF322B8_PRE` | `VQF322B8_PRE` |
| `float` | `fp8_e4m3fn_t` | 不支持 | `QF322FP8_PRE` | `VQF322FP8_PRE` |
| `float` | `hifloat8_t` | 不支持 | `QF322HIF8_PRE` 或 `QF322HIF8_PRE_HYBRID` | `VQF322HIF8_PRE` 或 `VQF322HIF8_PRE_HYBRID` |
| `int32_t` | `half` | 不支持 | `DEQF16` | `VDEQF16` |
| `int32_t` | `bfloat16_t` | 不支持 | `QS322BF16_PRE` | `VQS322BF16_PRE` |
| `int32_t` | `int8_t`、`uint8_t` | 不支持 | `REQ8` | `VREQ8` |

`fp8_e4m3fn_t`、`hifloat8_t`、`QF322F32_PRE`、`QF322F16_PRE`、`QF322BF16_PRE`、`QS322BF16_PRE` 及对应 tensor 量化模式适用于 Ascend 950PR/Ascend 950DT。

## 量化参数

scalar 量化参数和 tensor 量化参数的元素类型均为 `uint64_t`。对常见 8 bit 量化、反量化和重量化模式，量化参数低位字段承载量化系数、偏移、signed/unsigned 标志等信息：

- `M1`：量化系数。硬件按 1 个符号位、8 个指数位和 10 个尾数位的格式参与计算。
- `N`：部分重量化模式中的右移位数。
- `MCB`：Mode Control Bit，控制 `int32_t` 输入是否先右移再转换。
- `Offset`：9 bit 整数偏移，可与量化计算结果相加。
- `Sign`：用于标识量化结果为 signed `int8_t` 或 unsigned `uint8_t`。

针对 Ascend 950PR/Ascend 950DT，`M1` 不能为 INF 或 NaN。

## 约束说明

- tensor 量化参数张量应位于 L1，元素类型为 `uint64_t`，地址要求 32 字节对齐。
- 量化参数不能为 INF、NaN 或非规格化数。
- `RoundMode::HYBRID` 仅在源类型为 `float`、目的类型为 `hifloat8_t` 的量化输出场景支持。
- 不传入 `quant` 时，仅支持不量化输出或 F32 到 F16/BF16 的直接 cast 输出。
