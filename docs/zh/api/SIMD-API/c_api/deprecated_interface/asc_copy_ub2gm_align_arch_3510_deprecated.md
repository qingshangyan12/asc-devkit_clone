# asc_copy_ub2gm_align（废弃）

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

**入参`l2_cache_mode`类型为`uint8_t`的高维切分搬运原型已废弃，请使用[asc_copy_ub2gm_align](../vector_datamove/asc_copy_ub2gm_align/asc_copy_ub2gm_align_arch_3510.md)中入参类型为[asc_store_l2_cache_mode](../defs/enum/asc_store_l2_cache_mode.md)的原型替代。**

将数据从Unified Buffer（UB）搬运到Global Memory（GM），支持高维切分数据搬运。

## 函数原型

```c
__aicore__ inline void asc_copy_ub2gm_align(__gm__ <dtype>* dst,
                                            __ubuf__ <dtype>* src,
                                            uint32_t burst_count,
                                            uint32_t burst_len,
                                            uint8_t l2_cache_mode,
                                            uint64_t dst_stride,
                                            uint32_t src_stride)
```

`dtype`取值为：`int8_t`、`uint8_t`、`hifloat8_t`、`fp8_e5m2_t`、`fp8_e4m3fn_t`、`int16_t`、`uint16_t`、`half`、`bfloat16_t`、`int32_t`、`uint32_t`、`float`。

## 参数说明

| 参数名 | 输入/输出 | 描述 |
| :--- | :--- | :--- |
| dst | 输出 | 目的GM的起始地址，需要1字节对齐。 |
| src | 输入 | 源UB的起始地址，需要32字节对齐。 |
| burst_count | 输入 | 待搬运的连续传输数据块个数。 |
| burst_len | 输入 | 每个连续传输数据块的长度，单位为字节。 |
| l2_cache_mode | 输入 | L2 Cache管理策略。该`uint8_t`类型原型已废弃。 |
| dst_stride | 输入 | 目的操作数相邻数据块首地址的间隔，单位为字节。 |
| src_stride | 输入 | 源操作数相邻数据块首地址的间隔，单位为字节。 |

## 返回值说明

无

## 流水类型

PIPE_MTE3

## 约束说明

- 本接口仅在AIV上生效。
- 其他数据搬运约束请参见替代接口[asc_copy_ub2gm_align](../vector_datamove/asc_copy_ub2gm_align/asc_copy_ub2gm_align_arch_3510.md)。

## 调用示例

```cpp
asc_copy_ub2gm_align(dst, src, burst_count, burst_len,
    asc_store_l2_cache_mode::NORMAL_FIRST_VICTIM, dst_stride, src_stride);
```
