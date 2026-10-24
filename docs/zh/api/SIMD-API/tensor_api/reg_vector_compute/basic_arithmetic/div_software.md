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

与保留原有硬件路径的`operator/`不同，`div`用于显式选择基础Reg除法算法。`INTRINSIC`模式遵循当前设备编译的`--cce-ftz`设置；`PRECISION_0ULP_FTZ_TRUE`模式始终启用FTZ。

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
| precisionMode | 是否启用高精度除法路径。为`true`时仅支持`float`。 |
| algo | 除法算法。仅支持`INTRINSIC`和`PRECISION_0ULP_FTZ_TRUE`。后者要求`precisionMode=true`。 |
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

- src0.mask需预先设置；接口只使用src0.mask，不与src1.mask求交。
- 不支持标量混合、广播、复数、bfloat16和MERGING模式。
- `INTRINSIC`是否保留Subnormal由`--cce-ftz`决定；高精度模式不表示自动关闭FTZ。
- 使用整数类型时，除数不能为0。特殊值和边界值行为遵循基础Reg `Div`对应算法。

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
