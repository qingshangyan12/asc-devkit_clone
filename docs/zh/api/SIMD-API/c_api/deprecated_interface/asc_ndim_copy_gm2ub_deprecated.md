# asc_ndim_copy_gm2ub（废弃）

## 产品支持情况

<!-- npu="950" id1 -->
- Ascend 950PR/Ascend 950DT：支持
<!-- end id1 -->
<!-- npu="A3" id2 -->
- Atlas A3 训练系列产品/Atlas A3 推理系列产品：不支持
<!-- end id2 -->
<!-- npu="910b" id3 -->
- Atlas A2 训练系列产品/Atlas A2 推理系列产品：不支持
<!-- end id3 -->
<!-- npu="310b" id4 -->
- Atlas 200I/500 A2 推理产品：不支持
<!-- end id4 -->
<!-- npu="310p" id5 -->
- Atlas 推理系列产品AI Core：不支持
<!-- end id5 -->
<!-- npu="310p" id6 -->
- Atlas 推理系列产品Vector Core：不支持
<!-- end id6 -->
<!-- npu="910" id7 -->
- Atlas 训练系列产品：不支持
<!-- end id7 -->

## 功能说明

头文件路径为：`"c_api/vector_datamove/vector_datamove.h"`。

**入参`cache_mode`类型为`uint8_t`的原型已废弃，请使用[asc_ndim_copy_gm2ub](../vector_datamove/asc_ndim_copy_gm2ub.md)中入参类型为[asc_load_l2_cache_mode](../defs/enum/asc_load_l2_cache_mode.md)的原型替代。**

将多维数据从Global Memory（GM）搬运到Unified Buffer（UB），最多支持5个维度，并支持Padding。

## 函数原型

```c
__aicore__ inline void asc_ndim_copy_gm2ub(__ubuf__ <dtype>* dst,
                                           __gm__ <dtype>* src,
                                           uint32_t loop0_size,
                                           uint32_t loop1_size,
                                           uint32_t loop2_size,
                                           uint32_t loop3_size,
                                           uint32_t loop4_size,
                                           uint8_t loop0_lp_count,
                                           uint8_t loop0_rp_count,
                                           bool padding_mode,
                                           uint8_t cache_mode)
```

`dtype`取值为：`int8_t`、`uint8_t`、`fp4x2_e2m1_t`、`fp4x2_e1m2_t`、`fp8_e8m0_t`、`fp8_e5m2_t`、`fp8_e4m3fn_t`、`int16_t`、`uint16_t`、`half`、`bfloat16_t`、`int32_t`、`uint32_t`、`float`。

## 参数说明

| 参数名 | 输入/输出 | 描述 |
| :--- | :--- | :--- |
| dst | 输出 | 目的UB地址。 |
| src | 输入 | 源GM地址。 |
| loop0_size | 输入 | 0维处理的元素个数。 |
| loop1_size | 输入 | 1维处理的元素个数。 |
| loop2_size | 输入 | 2维处理的元素个数。 |
| loop3_size | 输入 | 3维处理的元素个数。 |
| loop4_size | 输入 | 4维处理的元素个数。 |
| loop0_lp_count | 输入 | 0维左侧需要补齐的元素个数。 |
| loop0_rp_count | 输入 | 0维右侧需要补齐的元素个数。 |
| padding_mode | 输入 | Padding值的填充方式。 |
| cache_mode | 输入 | L2 Cache管理策略。该`uint8_t`类型参数已废弃。 |

## 返回值说明

无

## 流水类型

PIPE_MTE2

## 约束说明

具体约束请参见替代接口[asc_ndim_copy_gm2ub](../vector_datamove/asc_ndim_copy_gm2ub.md)。

## 调用示例

```cpp
asc_ndim_copy_gm2ub(dst, src, loop0_size, loop1_size, loop2_size, loop3_size, loop4_size,
    loop0_lp_count, loop0_rp_count, padding_mode, asc_load_l2_cache_mode::NORMAL_FIRST_VICTIM);
```
