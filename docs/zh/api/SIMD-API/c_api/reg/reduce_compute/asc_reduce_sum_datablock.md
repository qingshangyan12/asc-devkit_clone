# asc_reduce_sum_datablock

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

头文件路径：`"c_api/reg_compute/reg_vector.h"`。

将每个DataBlock（32B）中参与计算的元素相加，并将最终的计算结果依次保存在dst的最低位。

```python
# 以half类型为例(b16位宽, vector_half共128个元素; b32类型如float为64个元素)
# src: half[128]  (vector_half, 256B = 128个half元素)
# dst: half[128]  (vector_half, 归约结果)
# mask: bool[128] (b16位宽, 每元素1bit)
#
# 按DataBlock(32B)分组归约: half类型每DataBlock含16个元素, 共8个DataBlock
# (b32类型每DataBlock含8个元素, 同样共8个DataBlock)
# 每个DataBlock内独立求和, 结果连续写入dst[0..7], 其余位置置0
# 不输出索引; mask为0的元素视为0不参与累加
# 注: 整型(int16/uint16)求和时dst会提升精度到int32/uint32以避免溢出

def asc_reduce_sum_datablock(dst, src, mask):
    num_blocks = 8                      # 128元素 / 16元素每块 = 8块
    elems_per_block = 16                # 32B / 2B(half) = 16
    for b in range(num_blocks):
        sum_val = 0
        for j in range(elems_per_block):
            i = b * elems_per_block + j
            if mask[i]:
                sum_val = sum_val + src[i]
        dst[b] = sum_val                # 每块求和值连续写入dst低位
    for i in range(num_blocks, 128):
        dst[i] = 0                      # 其余位置置0
```

## 函数原型

### 模板原型

```c
__simd_callee__ inline void asc_reduce_sum_datablock(vector_<dst_dtype>& dst,
                                                     vector_<src_dtype> src,
                                                     vector_bool mask)
```

dtype支持的数据类型为`int16_t`、`int32_t`、`uint16_t`、`uint32_t`、`half`、`float`

### 典型示例

```c
// 示例：half类型树形归约求和
__simd_callee__ inline void asc_reduce_sum_datablock(vector_half& dst,
                                                     vector_half src,
                                                     vector_bool mask)
```

## 参数说明

**表1** 参数说明

| 参数名 | 输入/输出 | 描述                                     |
| ------ | --------- | ---------------------------------------- |
| dst    | 输出      | 目的操作数（矢量数据寄存器）。           |
| src    | 输入      | 源操作数（矢量数据寄存器）。             |
| mask   | 输入      | 掩码寄存器，用于控制各元素是否参与归约。 |

矢量数据寄存器和掩码寄存器的详细说明请参见[reg数据类型定义](../reg_data_types/data_type_definition.md)。

## 返回值说明

无

## 约束说明

- 指令内累加顺序采用二叉树累加方式，在每个DataBlock（32B）内两两相加逐层归约求和，结果连续写入到目的操作数，目的操作数中的其它元素置0。
- 当DataBlock中的元素均不参与计算（mask全为0）时，将0写入dst对应位置（对于浮点数则为+0）。
- 对于输入为uint16_t/int16_t类型的情况，会提升精度到uint32_t/int32_t进行计算。

## 关键特性

**asc_reduce_sum_datablock累加顺序**：

以二叉树累加的方式计算每个DataBlock内的和。

以half类型的数据求和为例，在每个DataBlock内有16个数，通过二叉树的方式，两两相加，计算过程如下图所示：

1. data1和data2相加得到data01，data3和data4相加得到data02，……，data13和data14相加得到data07，data15和data16相加得到data08；
2. data01和data02相加得到data001，data03和data04相加得到data002，……，data07和data08相加得到data004；
3. 以此类推，得到目的操作数为1个half类型的数据sum。

**图1** asc_reduce_sum_datablock示意图
![asc_reduce_sum_datablock示意图](../../../../figures/reg_reduce_datablock_index.png "asc_reduce_sum_datablock示意图")

## 调用示例

```cpp
__simd_vf__ inline void reduce_sum_datablock_vf(__ubuf__ half* src_addr, __ubuf__ half* dst_addr, uint32_t count, uint16_t one_repeat_size, uint16_t repeat_time)
{
    vector_half src, dst;
    vector_bool mask;
    for (uint16_t i = 0; i < repeat_time; ++i) {
        mask = asc_update_mask_b16(count);
        asc_loadalign_postupdate(src, src_addr, one_repeat_size);
        asc_reduce_sum_datablock(dst, src, mask);
        asc_storealign_postupdate(dst_addr, dst, one_repeat_size, mask);
    }
}
```
