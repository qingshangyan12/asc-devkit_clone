# asc_copy_gm2l1_align_arch_9201

## 产品支持情况

<!-- npu="960" id1 -->
- Ascend 960PR/Ascend 960DT：支持
<!-- end id1 -->
<!-- npu="950" id2 -->
- Ascend 950PR/Ascend 950DT：不支持
<!-- end id2 -->
<!-- npu="A3" id3 -->
- Atlas A3 训练系列产品/Atlas A3 推理系列产品：不支持
<!-- end id3 -->
<!-- npu="910b" id4 -->
- Atlas A2 训练系列产品/Atlas A2 推理系列产品：不支持
<!-- end id4 -->
<!-- npu="310b" id5 -->
- Atlas 200I/500 A2 推理产品：不支持
<!-- end id5 -->
<!-- npu="310p" id6 -->
- Atlas 推理系列产品AI Core：不支持
<!-- end id6 -->
<!-- npu="310p" id7 -->
- Atlas 推理系列产品Vector Core：不支持
<!-- end id7 -->
<!-- npu="910" id8 -->
- Atlas 训练系列产品：不支持
<!-- end id8 -->

## 功能说明

头文件路径为：`"c_api/cube_datamove/cube_datamove.h"`。

将数据从Global Memory搬运到L1 Buffer，支持非32字节对齐搬运和左右Padding。本接口仅在AIC上执行有效。

## 函数原型

```cpp
__aicore__ inline void asc_copy_gm2l1_align(__cbuf__ <dtype>* dst,
                                            __gm__ <dtype>* src,
                                            uint32_t burst_count,
                                            uint32_t burst_len,
                                            uint8_t left_padding_count,
                                            uint8_t right_padding_count,
                                            bool enable_data_select,
                                            bool enable_pre_load_ignore_sync,
                                            asc_load_l2_cache_mode l2_cache_mode,
                                            uint64_t src_stride,
                                            uint32_t dst_stride)
```

`dtype`支持的数据类型为`int8_t`、`uint8_t`、`hifloat8_t`、`int16_t`、`uint16_t`、`half`、`bfloat16_t`、`int32_t`、`uint32_t`、`float`。

## 参数说明

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| dst | 输出 | 目的操作数，存储位置为L1 Buffer。起始地址需32字节对齐。 |
| src | 输入 | 源操作数，存储位置为Global Memory。起始地址需1字节对齐。 |
| burst_count | 输入 | 搬运的数据块个数。取值范围：[1, $2^{21}-1$]。 |
| burst_len | 输入 | 每个数据块的长度，单位为字节。取值范围：[1, $2^{21}-1$]。需满足`dtype`字节对齐。 |
| left_padding_count | 输入 | 每个数据块左侧填充的元素个数。填充字节数需满足`left_padding_count * sizeof(dtype) <= 32`。 |
| right_padding_count | 输入 | 每个数据块右侧填充的元素个数。填充字节数需满足`right_padding_count * sizeof(dtype) <= 32`。 |
| enable_data_select | 输入 | 填充数据来源选择。`false`表示使用数据块首元素，`true`表示使用预先配置的填充值。 |
| enable_pre_load_ignore_sync | 输入 | 是否开启预加载忽略同步功能。`false`表示关闭，`true`表示开启。 |
| l2_cache_mode | 输入 | 配置源端Global Memory数据在L2 Cache中的管理策略，类型和取值说明请参见[asc_load_l2_cache_mode](../../defs/enum/asc_load_l2_cache_mode.md)。 |
| src_stride | 输入 | 源端相邻数据块起始地址之间的间隔，单位为字节。取值范围：[0, $2^{40}-1$]。 |
| dst_stride | 输入 | 目的端相邻数据块起始地址之间的间隔，单位为字节。取值范围：[0, $2^{21}-1$]。 |

## 返回值说明

无

## 流水类型

PIPE_MTE2

## 约束说明

- 本接口非AIC调用直接返回。
- `burst_count`、`burst_len`和`dst_stride`超出21 bit取值范围时，高位会被截断；`src_stride`超出40 bit取值范围时，高位会被截断。
- `enable_data_select`设置为`true`或使用左右Padding时，需先调用[asc_set_gm2l1_pad](../cube_load_aux_config/asc_set_gm2l1_pad.md)配置填充值。
- 开启`enable_pre_load_ignore_sync`前，需确保本次搬运与前序访问相同地址的指令不存在数据依赖，否则可能产生异常数据。
- `dst`偏移量与搬运大小之和不可超出L1 Buffer容量。

## 调用示例

```cpp
#include "c_api/asc_simd.h"

__global__ __cube__ void CopyGm2l1AlignKernel(__gm__ half* src)
{
    asc_init();
    __cbuf__ half dst[256];
    // 搬运一个32字节数据块，不使用左右Padding和预加载忽略同步功能。
    asc_copy_gm2l1_align(dst, src, 1, 32, 0, 0, false, false,
        asc_load_l2_cache_mode::NORMAL_FIRST_VICTIM, 32, 32);
    asc_sync_pipe(PIPE_MTE2);
}
```
