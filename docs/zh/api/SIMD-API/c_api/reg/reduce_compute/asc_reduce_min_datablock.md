# asc_reduce_min_datablock

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

根据mask将每个DataBlock(32B)中的最小值依次保存在dst中的最低位。

```python
# 以half类型为例(b16位宽, vector_half共128个元素; b32类型如float为64个元素)
# src: half[128]  (vector_half, 256B = 128个half元素)
# dst: half[128]  (vector_half, 归约结果)
# mask: bool[128] (b16位宽, 每元素1bit)
#
# 按DataBlock(32B)分组归约: half类型每DataBlock含16个元素, 共8个DataBlock
# (b32类型每DataBlock含8个元素, 同样共8个DataBlock)
# 每个DataBlock独立求最小值, 结果连续写入dst[0..7], 其余位置置0
# 不输出索引; mask为0的元素视为+inf不参与比较

def asc_reduce_min_datablock(dst, src, mask):
    num_blocks = 8                      # 128元素 / 16元素每块 = 8块
    elems_per_block = 16                # 32B / 2B(half) = 16
    for b in range(num_blocks):
        min_val = float('inf')
        for j in range(elems_per_block):
            i = b * elems_per_block + j
            if mask[i] and src[i] < min_val:
                min_val = src[i]
        dst[b] = min_val                # 每块最小值连续写入dst低位
    for i in range(num_blocks, 128):
        dst[i] = 0                      # 其余位置置0
```

## 函数原型

### 模板原型（占位符形式）

本接口使用`<dtype>`表示支持的数据类型，模板原型如下：

```c
__simd_callee__ inline void asc_reduce_min_datablock(vector_<dtype>& dst,
                                                     vector_<dtype> src,
                                                     vector_bool mask)
```

#### dtype支持数据类型

dtype支持的数据类型为`int16_t`、`int32_t`、`uint16_t`、`uint32_t`、`half`、`float`

### 典型示例

```c
// 示例：对half矢量数据寄存器按数据块归约取最小值
__simd_callee__ inline void asc_reduce_min_datablock(vector_half& dst,
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

### 通用约束

- 在比较中，遵循min(-0, +0) = -0。
- 找到源操作数中每个DataBlock内的最小值，连续写入到目的操作数，目的操作数中的其它元素置0。
- 未被mask选中的元素被视为最大值（浮点数为+inf），如果一个DataBlock中所有元素都未被mask选中，最大值（浮点数为+inf）将被填充到目的操作数的指定位置。
- 仅输出最值，不输出index下标。

## 调用示例

```cpp
__simd_vf__ inline void reduce_min_datablock_vf(__ubuf__ half* src_addr, __ubuf__ half* dst_addr, uint32_t count, uint16_t one_repeat_size, uint16_t repeat_time)
{
    vector_half src, dst;
    vector_bool mask;
    for (uint16_t i = 0; i < repeat_time; ++i) {
        mask = asc_update_mask_b16(count);
        asc_loadalign_postupdate(src, src_addr, one_repeat_size);
        asc_reduce_min_datablock(dst, src, mask);
        asc_storealign_postupdate(dst_addr, dst, one_repeat_size, mask);
    }
}
```
