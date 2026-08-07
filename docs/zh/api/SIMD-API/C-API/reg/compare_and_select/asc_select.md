# asc_select

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

根据mask的比特位值，从源操作数src0，src1中选择元素，得到目的操作数dst。选择的规则为：当mask的比特位为1时，从src0中选取对应位置的数；当mask的比特位为0时，从src1中选取对应位置的数。

计算公式如下：

$$
dst_i = (mask_i == 1)\ ?\ src0_i : src1_i
$$

```python
# 以half类型为例（b16位宽，256B共128个元素）
# src0: half[128]   (vector_half, 源操作数0)
# src1: half[128]   (vector_half, 源操作数1)
# dst:  half[128]   (vector_half, 目的操作数)
# mask: bool[128]   (vector_bool, 掩码寄存器，控制元素选择)
#
# 根据mask选择元素：mask为1时选取src0对应元素，mask为0时选取src1对应元素

def asc_select(dst, src0, src1, mask):
    for i in range(128):
        if mask[i]:
            dst[i] = src0[i]
        else:
            dst[i] = src1[i]
```

## 函数原型

### 模板原型

```c
__simd_callee__ inline void asc_select(vector_<dtype>& dst,
                                       vector_<dtype> src0,
                                       vector_<dtype> src1,
                                       vector_bool mask)
```

dtype支持的数据类型为`bool`、`uint8_t`、`int8_t`、`fp8_e4m3fn_t`、`hifloat8_t`、`fp8_e5m2_t`、`uint16_t`、`int16_t`、`half`、`bfloat16_t`、`uint32_t`、`int32_t`、`float`

### 典型示例

```c
// 示例：对half矢量数据寄存器执行按掩码选择
__simd_callee__ inline void asc_select(vector_half& dst,
                                       vector_half src0,
                                       vector_half src1,
                                       vector_bool mask)
```

## 参数说明

### 矢量数据寄存器按掩码选择

**表1** 参数说明

| 参数名 | 输入/输出 | 描述                           |
| ------ | --------- | ------------------------------ |
| dst    | 输出      | 目的操作数（矢量数据寄存器）。 |
| src0   | 输入      | 源操作数（矢量数据寄存器）。   |
| src1   | 输入      | 源操作数（矢量数据寄存器）。   |
| mask   | 输入      | 掩码寄存器，用于选择源操作数。 |

### 掩码寄存器按掩码选择

**表2** 参数说明

| 参数名 | 输入/输出 | 描述                           |
| ------ | --------- | ------------------------------ |
| dst    | 输出      | 目的操作数（掩码寄存器）。     |
| src0   | 输入      | 源操作数（掩码寄存器）。       |
| src1   | 输入      | 源操作数（掩码寄存器）。       |
| mask   | 输入      | 掩码寄存器，用于选择源操作数。 |

矢量数据寄存器和掩码寄存器的详细说明请参见[reg数据类型定义](../reg_data_types/data_type_definition.md)。

## 返回值说明

无

## 流水类型

PIPE_V

## 约束说明

- src0、src1和dst的数据类型需要保持一致。
- 掩码位为1时，dst取src0对应元素；为0时，dst取src1对应元素。

## 调用示例

```cpp
vector_half dst;
vector_half src0;
vector_half src1;
vector_bool mask = asc_create_mask_b16(PAT_ALL);
asc_loadalign(src0, src0_addr); // src0_addr是外部输入的UB内存空间地址。
asc_loadalign(src1, src1_addr); // src1_addr是外部输入的UB内存空间地址。
// mask为源操作数掩码
asc_select(dst, src0, src1, mask);
```
