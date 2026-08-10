# asc_reduce_min

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

根据mask对源操作数src进行归约最小值操作：将src中的最小值写入目的操作数dst的0位置，最小值在src中的索引写入目的操作数dst的1位置，dst的其他位置将被置为0。如果有多个最小值，第二个数为最小的下标，其余值都将被填充为0。参考伪代码：
```python
def asc_reduce_min(dst, src, mask):
    min_val = float('inf')
    min_idx = 0
    for i in range(128):
        if mask[i] and src[i] < min_val:
            min_val = src[i]
            min_idx = i
    dst[0] = min_val                    # 最小值
    dst_bits[1] = unsigned_bits(min_idx) # 写入索引的原始位模式
    for i in range(2, 128):
        dst[i] = 0                      # 其余位置置0
```

计算公式如下：

$$
dst_0 = \min\{src_i \mid mask_i = 1\} \\
dst_1 = \argmin\{src_i \mid mask_i = 1\}
$$

## 函数原型

### 模板原型（占位符形式）

```c
__simd_callee__ inline void asc_reduce_min(vector_<dtype>& dst,
                                           vector_<dtype> src,
                                           vector_bool mask)
```

#### dtype支持数据类型

dtype支持的数据类型为`int16_t`、`int32_t`、`uint16_t`、`uint32_t`、`half`、`float`

### 典型示例

```c
// 示例：对half矢量数据寄存器整向量归约取最小值
__simd_callee__ inline void asc_reduce_min(vector_half& dst,
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

- 未被mask选中的元素被视为最大值（浮点数为+inf），如果一个repeat中所有元素都未被mask选中，最大值(浮点数为+inf)将被填充到目的操作数的第一个数，其余值都将被填充为0（包括下标）。
- 在比较中，遵循min(-0, +0) = -0。
- 如果输入数据存在nan，将nan写入目的操作数的第一个数，将第一个nan值的下标写入目的操作数的第二个数。

## 关键特性

**索引值需要强制类型转换**：

索引不是dst数据类型的数值，而是按dst元素位宽保存的无符号整数原始位模式。读取half或int16_t/uint16_t结果中的索引时，应按`uint16_t`位模式解释；读取float或int32_t/uint32_t结果中的索引时，应按`uint32_t`位模式解释。

**提取结果（值+索引）：**

归约产生两个有效结果，需要分别提取。

## 调用示例

```cpp
__simd_vf__ inline void reduce_min_vf(__ubuf__ half* src_addr, __ubuf__ half* dst_addr, uint32_t count, uint16_t one_repeat_size, uint16_t repeat_time)
{
    vector_half src, dst;
    vector_bool mask;
    for (uint16_t i = 0; i < repeat_time; ++i) {
        mask = asc_update_mask_b16(count);
        asc_loadalign_postupdate(src, src_addr, one_repeat_size);
        asc_reduce_min(dst, src, mask);
        asc_storealign_postupdate(dst_addr, dst, one_repeat_size, mask);
    }
}
```
