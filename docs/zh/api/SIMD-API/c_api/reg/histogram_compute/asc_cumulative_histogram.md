# asc_cumulative_histogram

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

该接口用于对直方图数据进行累计统计。

如下图所示，在低位模式（BIN0）下，目的寄存器dst会统计源寄存器src中落在低位区间[0, 127]的数据分布情况；在高位模式（BIN1）下，目的寄存器dst则会统计src中落在高位区间[128, 255]的数据分布情况。dst中的第n个元素表示src中从0到n的所有数值在对应区间中出现的总频率。最终，统计结果会在目的寄存器原始数据的基础上进行累加。

![累计统计](../../../../figures/asc_cumulative_histogram.png)

本接口仅在AIV上执行有效，在AIC上调用为空操作。

## 函数原型

```c
__simd_callee__ inline void asc_cumulative_histogram_bin0(vector_uint16_t& dst,
                                                          vector_uint8_t src,
                                                          vector_bool mask)
__simd_callee__ inline void asc_cumulative_histogram_bin1(vector_uint16_t& dst,
                                                          vector_uint8_t src,
                                                          vector_bool mask)
```

dtype支持的数据类型为`bool`、`uint8_t`、`uint16_t`。

## 参数说明

**表1** 参数说明

| 参数名  | 输入/输出 | 描述 |
| :----- | :------- | :------- |
| dst | 输出 | 目的操作数（矢量数据寄存器）。 |
| src | 输入 | 源操作数（矢量数据寄存器）。 |
| mask | 输入 | 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。 |

矢量数据寄存器和掩码寄存器的详细说明请参见[reg数据类型定义](../reg_data_types/data_type_definition.md)。

## 返回值说明

无

## 流水类型

PIPE_V

## 约束说明

### 通用约束

- 本接口仅在AIV上生效，非AIV调用直接返回。
- 本接口在Vector Function（`__simd_vf__`标记的函数）内调用，dst与src为矢量数据寄存器，不涉及UB地址，无UB容量与地址对齐约束。
- dst的dtype必须为`uint16_t`，src的dtype必须为`uint8_t`，mask必须为`vector_bool`掩码寄存器；其他dtype触发数据类型异常。
- mask的第j个比特位控制`src[j]`是否参与统计：比特位为1时参与统计，为0时忽略该元素。
- 本次统计结果累加到dst对应元素的原值上，多次调用可累积跨批次数据。dst元素的最大值为65535，使用时需注意累加溢出。

### BIN0模式约束

对dst的第i个元素（i ∈ {0,1,...,127}），本次调用累加mask有效且满足`src[j] ≤ i`的源元素个数。

### BIN1模式约束

对dst的第i个元素（i ∈ {0,1,...,127}），本次调用累加mask有效且满足`128 <= src[j] ≤ 128 + i`的源元素个数。

## 调用示例

```cpp
__simd_vf__ inline void cumulative_histogram_vf(__ubuf__ uint16_t* dst_addr, __ubuf__ uint8_t* src_addr, uint32_t count, uint16_t one_repeat_size, uint16_t repeat_time)
{
    vector_uint16_t dst0, dst1;  // dst0用于低位统计，dst1用于高位统计
    vector_uint8_t src;
    vector_bool mask_b8 = asc_create_mask_b8(PAT_ALL);
    vector_bool mask_b16 = asc_create_mask_b16(PAT_ALL);

    // 初始化dst为0，从零开始统计频率
    asc_duplicate_scalar(dst0, (uint16_t)0);
    asc_duplicate_scalar(dst1, (uint16_t)0);

    for (uint16_t i = 0; i < repeat_time; ++i) {
        asc_loadalign_postupdate(src, src_addr, one_repeat_size);
        asc_cumulative_histogram_bin0(dst0, src, mask_b8);
        asc_cumulative_histogram_bin1(dst1, src, mask_b8);
    }

    // 此时dst0和dst1包含两批数据的累积统计结果
    asc_storealign_postupdate(dst_addr, dst0, one_repeat_size, mask_b16);
    asc_storealign_postupdate(dst_addr, dst1, one_repeat_size, mask_b16);
}
```
