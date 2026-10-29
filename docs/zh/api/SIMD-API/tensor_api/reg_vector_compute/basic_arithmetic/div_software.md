# div

## 产品支持情况

<!-- npu="950" id1 -->
- Ascend 950PR&950DT系列产品：支持
<!-- end id1 -->
<!-- npu="A3" id2 -->
- Atlas A3系列产品：不支持
<!-- end id2 -->
<!-- npu="910b" id3 -->
- Atlas A2系列产品：不支持
<!-- end id3 -->
<!-- npu="310b" id4 -->
- Atlas 200I/500 A2推理产品：不支持
<!-- end id4 -->
<!-- npu="310p" id5 -->
- Atlas推理系列产品AI Core：不支持
<!-- end id5 -->
<!-- npu="310p" id6 -->
- Atlas推理系列产品Vector Core：不支持
<!-- end id6 -->
<!-- npu="910" id7 -->
- Atlas训练系列产品：不支持
<!-- end id7 -->

## 功能说明

头文件路径：`"tensor_api/experimental/arch/vector/basic_arithmetic.h"`。

该接口根据src0携带的mask，对两个源操作数按元素相除，并通过编译期配置选择除法精度算法。计算公式如下：

$$
dst_i = \frac{src0_i}{src1_i}
$$

`operator/`直接使用硬件除法指令，不提供精度模式和算法配置；`div`支持通过模板参数配置除法精度和算法，并根据所选配置处理Subnormal。`INTRINSIC`模式的Subnormal处理遵循编译选项`--cce-ftz`；`PRECISION_0ULP_FTZ_TRUE`模式始终启用FTZ。

## 函数原型

```cpp
enum class div_algo {
    INTRINSIC,
    PRECISION_0ULP_FTZ_TRUE
};

template <bool precisionMode = false, div_algo algo = div_algo::INTRINSIC, typename T>
__simd_callee__ inline reg_tensor<T> div(
    const reg_tensor<T>& src0, const reg_tensor<T>& src1)
```

## 参数说明

**表1** 模板参数说明

| 参数名 | 描述 |
| --- | --- |
| precisionMode | 是否启用高精度除法路径。为`true`时使用差值补偿算法，仅支持`float`。精度说明请参考[关键特性说明](#关键特性说明)。 |
| algo | 除法算法。仅支持`INTRINSIC`和`PRECISION_0ULP_FTZ_TRUE`。后者要求`precisionMode=true`。算法与Subnormal模式的关系请参考[关键特性说明](#关键特性说明)。 |
| T | 操作数数据类型。支持的数据类型请参考[数据类型](#数据类型)。 |

**表2** 参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| src0 | 输入 | 被除数，类型为`reg_tensor<T>`。src0.reg保存矢量数据，src0.mask控制有效元素。 |
| src1 | 输入 | 除数，类型为`reg_tensor<T>`。src1.mask不参与本次计算。 |

## 数据类型

`precisionMode=false`且`algo=INTRINSIC`时支持：`int16_t`、`uint16_t`、`half`、`int32_t`、`uint32_t`、`float`。

`precisionMode=true`的两种模式暂仅支持`float`。

## 返回值说明

返回按元素相除的结果，类型为`reg_tensor<T>`。返回值mask与src0.mask相同；src0.mask对应位置为0时，返回值的对应元素置零。

## 约束说明

- `src0.mask`用于指定参与计算的有效元素。调用接口前需通过`with_mask`预先设置；mask为0的元素不参与计算，输出对应元素置零。
- `INTRINSIC`是否保留Subnormal由`--cce-ftz`决定；高精度模式不表示自动关闭FTZ。
- 使用整数类型时，除数不能为0，结果为按元素进行整数除法得到的商。
- 使用浮点类型时，除数为`0.0`或`-0.0`且被除数为非零有限值时，结果为带相应符号的`inf`；被除数和除数均为零时，结果为`nan`。
- 使用浮点类型时，被除数或除数为`nan`时，结果为`nan`；有限值除以`inf`或`-inf`时，结果为带相应符号的零；`inf`或`-inf`除以有限非零值时，结果为带相应符号的`inf`；`inf`或`-inf`除以`inf`或`-inf`时，结果为`nan`。
- 浮点运算结果溢出时返回带相应符号的`inf`；结果下溢为Subnormal时，按照所选算法及`--cce-ftz`配置处理。

## 关键特性说明

### 最大精度误差

对于浮点类型，支持的配置及最大精度误差如下。整数类型不涉及浮点精度误差和Subnormal处理。

| precisionMode | algo | 支持的浮点类型 | 最大精度误差 |
| --- | --- | --- | --- |
| `false`（默认值） | `div_algo::INTRINSIC`（默认值） | half、float | 1 ULP |
| `true` | `div_algo::INTRINSIC` | float | 0 ULP |
| `true` | `div_algo::PRECISION_0ULP_FTZ_TRUE` | float | 0 ULP |

`precisionMode=true`时通过差值补偿提高除法精度，但不表示自动保留Subnormal。Subnormal的处理方式由`algo`与编译选项共同决定。

### 配置Subnormal模式

FTZ（Flush-To-Zero）是一种浮点运算模式，当结果为[Subnormal](../../../basic_api/data_structures/builtin_data_types.md#p7381131713310)时，将其直接清零（近似为0），而非保留其精确的微小数值。上述精度误差应结合所选Subnormal模式理解；0 ULP不表示FTZ模式下仍保留Subnormal结果。

#### 默认算法

`div_algo::INTRINSIC`为默认算法，浮点类型的Subnormal处理受编译选项`--cce-ftz`控制：

- `--cce-ftz=true`（默认值）：采用FTZ模式。
- `--cce-ftz=false`：支持Subnormal数据计算并保留Subnormal结果。

上述规则同时适用于默认精度和高精度配置。两者的差别在于是否启用差值补偿，而不是是否启用FTZ。

#### 显式指定的算法

`div_algo::PRECISION_0ULP_FTZ_TRUE`要求`precisionMode=true`，仅支持float类型，始终采用FTZ模式，不受`--cce-ftz`取值影响。即使设置`--cce-ftz=false`，该算法也不保留Subnormal结果。

Tensor API当前仅支持上述三种配置，不支持直接指定基础API中的`DivAlgo::DIFF_COMPENSATION`、`DivAlgo::PRECISION_1ULP_FTZ_TRUE`或名称中包含`FTZ_FALSE`的算法。

#### 使用建议

- 一般场景可使用`div(src0, src1)`，在`--cce-ftz=true`（默认值）时采用硬件除法路径，避免高精度差值补偿的额外开销。
- 需要0 ULP精度且Subnormal处理跟随编译选项时，使用`div<true>(src0, src1)`。
- 需要0 ULP精度且始终采用FTZ模式时，使用`div<true, div_algo::PRECISION_0ULP_FTZ_TRUE>(src0, src1)`。
- 需要保留Subnormal结果时，设置`--cce-ftz=false`并使用`div_algo::INTRINSIC`；若同时要求0 ULP精度，使用`div<true>(src0, src1)`。

## 调用示例

```cpp
#include "tensor_api/tensor.h"
#include "tensor_api/experimental/arch/vector/basic_arithmetic.h"

using asc::te::experimental::div;
using asc::te::experimental::div_algo;

auto default_result = div(src0, src1);
auto precise_result = div<true>(src0, src1);
auto ftz_result = div<true, div_algo::PRECISION_0ULP_FTZ_TRUE>(src0, src1);
```
