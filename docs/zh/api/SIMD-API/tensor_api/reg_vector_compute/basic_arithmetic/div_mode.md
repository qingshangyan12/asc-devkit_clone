# div 模式除法

## 功能说明

头文件：`tensor_api/experimental/arch/vector/basic_arithmetic.h`。仅适用于 `__NPU_ARCH__ == 3510`。

```cpp
template <div_algorithm Algorithm = div_algorithm::intrinsic,
          bool PrecisionMode = false, typename T>
__simd_callee__ inline reg_tensor<T> div(
    const reg_tensor<T>& src0, const reg_tensor<T>& src1);
```

`div_algorithm` 定义在 `tensor_api/experimental/utils/reg_enum.h`。该接口将编译期模式映射到基础 API 的 `Reg::Div`，直接复用 3510 的单指令、差值补偿及 IEEE 754 subnormal 软仿路径，不重复维护边界值和舍入逻辑。

| Algorithm | 支持类型 | 3510 实现 |
| --- | --- | --- |
| `intrinsic` | `int16_t`、`uint16_t`、`int32_t`、`uint32_t`、`half`、`float` | 整数直接除法；浮点数根据编译时 FTZ 设置使用单指令或 subnormal 软仿 |
| `precision_1ulp_ftz_true` | 同上 | 单指令除法 |
| `diff_compensation` | `float` | 根据编译时 FTZ 设置选择差值补偿或 subnormal 软仿 |
| `precision_0ulp_ftz_true` | `float` | 差值补偿 |
| `precision_0ulp_ftz_false` | `float` | subnormal 规格化、差值补偿及结果舍入还原 |
| `precision_1ulp_ftz_false` | `half`、`float` | subnormal 规格化、单指令除法及结果还原 |

`PrecisionMode = true` 仅支持 `float`，沿用基础 API 高精度配置。即使 `Algorithm` 为 `intrinsic`，此设置仍会进入高精度路径。默认 `PrecisionMode = false`。`intrinsic` 和 `diff_compensation` 的浮点 FTZ 行为受编译时 `--cce-ftz` 设置影响；显式 FTZ 模式由 `Algorithm` 指定。

接口使用 `src0.mask` 控制有效元素，忽略 `src1.mask`，返回值的 mask 等于 `src0.mask`；未启用的元素置零。支持的类型以编译期检查为准。

```cpp
auto accurate = div<div_algorithm::precision_0ulp_ftz_false>(dividend, divisor);
auto fast = div<div_algorithm::precision_1ulp_ftz_true>(dividend, divisor);
```
