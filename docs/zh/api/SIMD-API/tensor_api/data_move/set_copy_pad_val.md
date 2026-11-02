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
- 填充值仅在[GM到UB搬运](../reg_vector_compute/vector_compute_load/copy_gm_to_ub.md)的`gm_to_ub_params`参数中`enable_constant_pad`成员设置为`true`时被取用。
- 填充值配置对后续所有满足条件的GM到UB对齐搬运生效，直到再次调用本接口覆盖配置。

## 调用示例

以下示例从Global Memory读取每行24个int8_t数据，在每行左侧填充3个、右侧填充5个-9后写入Global Memory。示例展示set_copy_pad_val与gm_to_ub_params的配合使用。

```cpp
#include <cstdint>

#include "tensor_api/tensor.h"

constexpr uint32_t rows = 2;
constexpr uint32_t source_columns = 24;
constexpr uint32_t padded_columns = 32;
constexpr uint32_t padded_length = rows * padded_columns;
constexpr uint8_t left_padding = 3;
constexpr uint8_t right_padding = 5;
constexpr int8_t padding_value = -9;

__global__ __vector__ void padded_copy_kernel(__gm__ int8_t* src, __gm__ int8_t* dst)
{
    using namespace asc::te;
    __ubuf__ int8_t dst_buffer[padded_length];

    auto src_layout = make_frame_layout<nd_ext_layout_ptn, layout_trait_default<int8_t>>(rows, source_columns);
    auto dst_layout = make_frame_layout<nd_ext_layout_ptn, layout_trait_default<int8_t>>(rows, padded_columns);
    auto src_gm = make_tensor(make_mem_ptr<location::gm>(src), src_layout);
    auto dst_ub = make_tensor(make_mem_ptr(dst_buffer), dst_layout);
    auto dst_gm = make_tensor(make_mem_ptr<location::gm>(dst), dst_layout);

    set_copy_pad_val<int8_t>(padding_value);
    constexpr gm_to_ub_params padding_params{left_padding, right_padding, true};
    copy(make_copy(copy_gm_to_ub{}).with(padding_params), dst_ub, src_gm);
    copy(make_copy(copy_ub_to_gm{}), dst_gm, dst_ub);
}
```
