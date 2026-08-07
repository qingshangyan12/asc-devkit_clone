# asc_reduce_max

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

根据mask对源操作数src进行归约最大值操作：将src中的最大值写入目的操作数dst的0位置，最大值在src中的索引写入目的操作数dst的1位置，dst的其他位置将被置为0。

对src中mask为1的元素求最大值，最大值写入dst的0位置，最大值索引写入dst的1位置，其余位置置0。

$$
\text{result} = \max\{src_i \mid mask_i = 1\}
$$

```python
# 以half类型为例(b16位宽, vector_half共128个元素; b32类型如float为64个元素)
# src: half[128]  (vector_half, 256B = 128个half元素)
# dst: half[128]  (vector_half, 归约结果)
# mask: bool[128] (b16位宽, 每元素1bit)
#
# 对mask为1的元素求最大值, 结果写入dst[0], 最大值索引写入dst[1], 其余位置置0
# 索引按dst数据类型存储, 读取时需用reinterpret_cast转换为整数类型
# 多个最大值时取最小索引; mask为0的元素视为-inf不参与比较

def asc_reduce_max(dst, src, mask):
    max_val = float('-inf')
    max_idx = 0
    for i in range(128):
        if mask[i] and src[i] > max_val:
            max_val = src[i]
            max_idx = i
    dst[0] = max_val                    # 最大值
    dst[1] = half(max_idx)              # 索引按dst类型存储, 需reinterpret_cast读取
    for i in range(2, 128):
        dst[i] = 0                      # 其余位置置0
```

## 函数原型

### 模板原型

```c
__simd_callee__ inline void asc_reduce_max(vector_<dtype>& dst,
                                           vector_<dtype> src,
                                           vector_bool mask)
```

dtype支持的数据类型为`int16_t`、`uint16_t`、`int32_t`、`uint32_t`、`half`、`float`

### 典型示例

```c
__simd_callee__ inline void asc_reduce_max(vector_int16_t& dst,
                                           vector_int16_t src,
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

## 流水类型

PIPE_V

## 约束说明

- 将最大值写入目的操作数的第一个数，将最大值的下标写入目的操作数的第二个数， 如果有多个最大值，第二个数为最小的下标，其余值都将被填充为0。
- 未被mask选中的元素被视为最小值（浮点数为-inf），如果一个repeat中所有元素都未被mask选中，最小值（浮点数为-inf）将被填充到目的操作数的第一个数，其余值都将被填充为0（包括下标）。
- 在比较中，遵循max(-0, +0) = +0。
- 如果输入数据存在nan，将nan写入目的操作数的第一个数，将第一个nan值的下标写入目的操作数的第二个数。

## 关键特性

**索引值需要强制类型转换**：

dst的索引按照dst的数据类型存储，比如dst为half类型时，索引按照half类型存储，因此读取索引需要使用  reinterpret\_cast方法转换到整数类型。若数据类型是half，需要使用reinterpret\_cast\<uint16_t\*\>；若数据类型是float，需要使用reinterpret\_cast\<uint32\_t\*\>。

**提取结果（值+索引）：**

归约产生两个有效结果，需要分别提取。

## 调用示例

```cpp
vector_half dst;
vector_half src;
vector_bool mask = asc_create_mask_b16(PAT_ALL);
asc_loadalign(src, src_addr); // src_addr是外部输入的UB内存空间地址。
asc_reduce_max(dst, src, mask);
```
