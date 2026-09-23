# neg

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

对源操作数逐元素取相反数：

$$
dst_i = -src_i
$$

## 函数原型

```cpp
template <typename T>
__simd_callee__ inline reg_tensor<T> neg(const reg_tensor<T>& src)
```

## 参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| T | 模板参数 | 支持 `int8_t`、`int16_t`、`half`、`int32_t`、`float`。 |
| src | 输入 | 源操作数，`src.mask` 控制参与计算的元素。 |

## 返回值说明

返回类型为 `reg_tensor<T>`，返回值 `mask` 与 `src.mask` 相同；无效元素在 zeroing 模式下置零。

## 约束说明

- `src.mask` 必须通过 `with_mask` 预先设置，否则参与计算的元素位置未定义。
- 整型数据采用非饱和截断处理。当有符号整型输入为最小负值时，结果保留原值不变；例如 `int8_t` 的 `-128` 取相反数后仍为 `-128`。
- `half` 和 `float` 取相反数通过翻转符号位实现：正数变为对应负数，负数变为对应正数，`+0.0` 变为 `-0.0`，`-0.0` 变为 `+0.0`，`+inf` 变为 `-inf`，`-inf` 变为 `+inf`；NaN 的结果仍为 NaN。

## 调用示例

```cpp
#include "tensor_api/tensor.h"
#include "tensor_api/experimental/arch/vector/basic_arithmetic.h"
#include "tensor_api/experimental/arch/vector/mask_reg_compute.h"

template <typename InputTensor, typename OutputTensor>
__simd_vf__ inline void neg_example(InputTensor input, OutputTensor output)
{
    auto src = asc::te::experimental::load(input, asc::te::make_coord(0))
                   .with_mask(asc::te::experimental::all_mask<float>());
    auto dst = asc::te::experimental::neg(src);
    asc::te::experimental::store(output, asc::te::make_coord(0), dst);
}
```
