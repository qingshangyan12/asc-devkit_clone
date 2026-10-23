# set_copy_pad_val

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

头文件路径为：`tensor_api/tensor.h`。

设置Global Memory到Unified Buffer对齐搬运时使用的常量填充值。该配置对后续满足条件的对齐搬运持续生效。

## 函数原型

```cpp
template <typename T>
__aicore__ inline void set_copy_pad_val(T pad_value)
```

## 参数说明

**表1** 参数说明

| 参数名 | 输入/输出 | 描述 |
| :--- | :--- | :--- |
| pad_value | 输入 | 常量填充值，类型为`T`。 |

## 数据类型

`T`支持`int8_t`、`uint8_t`、`int16_t`、`uint16_t`、`half`、`bfloat16_t`、`int32_t`、`uint32_t`和`float`。

## 返回值说明

无

## 约束说明

- 本接口仅在AIV上生效。
- 本接口只设置填充值，须先调用本接口，再执行对应的[GM到UB搬运](../reg_vector_compute/vector_compute_load/copy_gm_to_ub.md)。
- 填充值仅在搬运参数`enable_constant_pad`设置为`true`时被取用。
- 填充值配置对后续所有满足条件的GM到UB对齐搬运生效，直到再次调用本接口覆盖配置。
- `fp8_e8m0_t`、`fp8_e5m2_t`、`fp8_e4m3fn_t`、`fp4x2_e2m1_t`、`fp4x2_e1m2_t`和`hifloat8_t`类型需转换为本接口支持的数据类型后调用。

## 调用示例

```cpp
#include "tensor_api/tensor.h"

using namespace asc::te;

__aicore__ inline void set_copy_pad_val_example()
{
    constexpr gm_to_ub_params params{3, 5, true};
    set_copy_pad_val<int8_t>(-9);
    // 使用params执行GM到UB对齐搬运。
}
```
