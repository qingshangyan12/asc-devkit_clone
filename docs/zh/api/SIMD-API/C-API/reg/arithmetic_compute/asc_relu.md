# asc_relu

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

根据mask对源操作数src进行按元素relu操作，将结果写入目的操作数dst。

计算公式如下：

$$
dst_i = (src_i >= 0) ? src_i : 0
$$

```python
# src: half[128]  (vector_half, 256B = 128个half元素; 以half为例, b32类型为64元素)
# dst: half[128]  (vector_half)
# mask: bool[128] (b16位宽, 每元素1bit)
#
# 逐元素ReLU: 负数置0, 非负数保持, mask为0的元素置零(MODE_ZEROING)

def asc_relu(dst, src, mask):
    for i in range(128):
        if mask[i]:
            dst[i] = max(src[i], 0)
        else:
            dst[i] = 0
```

## 函数原型

### 模板原型

```c
__simd_callee__ inline void asc_relu(vector_<dtype>& dst, vector_<dtype> src, vector_bool mask);
```

dtype支持的数据类型为`half`、`int32_t`、`float`

### 典型示例

```c
// 示例：对half矢量寄存器执行ReLU
__simd_callee__ inline void asc_relu(vector_half& dst, vector_half src, vector_bool mask);
```

## 参数说明

**表1** 参数说明
| 参数名 | 输入/输出 | 描述                                                                                                             |
| :----- | :-------- | :--------------------------------------------------------------------------------------------------------------- |
| dst    | 输出      | 目的操作数（矢量数据寄存器）。                                                                                   |
| src    | 输入      | 源操作数（矢量数据寄存器）。                                                                                     |
| mask   | 输入      | 掩码寄存器，用于控制各元素是否参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。 |

矢量数据寄存器和掩码寄存器的详细说明请参见[reg数据类型定义](../reg_data_types/data_type_definition.md)。

## 返回值说明

无

## 流水类型

PIPE_V

## 约束说明

- dst与src的数据类型需要保持一致。
- 掩码位为0时，dst对应元素置0。
- 浮点输入（half/float）按符号位判定：负数（含-0、-inf）写0，正数与+0原样输出，+inf保留，nan输入返回标准nan。
- 整数输入（int32_t）按算术符号判定：负数写0，非负数原样输出。

## 调用示例

```cpp
vector_half dst;
vector_half src;
vector_bool mask = asc_create_mask_b16(PAT_ALL);
asc_loadalign(src, src_addr); // src_addr是外部输入的UB内存空间地址。
// mask为源操作数掩码
asc_relu(dst, src, mask);
```
