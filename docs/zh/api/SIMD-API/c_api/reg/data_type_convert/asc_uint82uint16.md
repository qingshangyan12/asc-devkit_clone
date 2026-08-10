# asc_uint82uint16

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

头文件路径：`"c_api/reg_compute/reg_convert.h"`。

将uint8_t类型数据转为uint16_t类型。

由于源操作数与目的操作数类型位宽比为1:2，读取数据时需要将源操作数按偶数/奇数索引分为两组，每次读取其中一组元素。

```python
# src: uint8[256]   (vector_uint8_t, 256B = 256个uint8元素)
# dst: uint16[128]  (vector_uint16_t, 256B = 128个uint16元素)
# mask: bool[256]   (按src的b8位宽)
#
# 宽化转换(2:1): 256个uint8按偶数/奇数索引分为两组, 每次读取其中一组128个元素
# asc_uint82uint16    读取偶数索引 src[0,2,...,254], 转换后填满dst全部128个位置
# asc_uint82uint16_v2 读取奇数索引 src[1,3,...,255], 转换后填满dst全部128个位置

def asc_uint82uint16(dst, src, mask):
    for i in range(128):
        if mask[2 * i]:                  # 偶数索引位置参与计算
            dst[i] = uint16(src[2 * i])  # uint8 -> uint16 扩展转换
        else:
            dst[i] = 0                   # mask未选中, 置零

def asc_uint82uint16_v2(dst, src, mask):
    for i in range(128):
        if mask[2 * i + 1]:              # 奇数索引位置参与计算
            dst[i] = uint16(src[2 * i + 1])
        else:
            dst[i] = 0
```

## 函数原型

### 模板原型（占位符形式）

本接口包含2种分块位置，模板原型如下：

```c
__simd_callee__ inline void asc_uint82uint16<v2_suffix>(vector_uint16_t& dst,
                                                        vector_uint8_t src,
                                                        vector_bool mask)
```

`<v2_suffix>`为空时，读取src的偶数索引位置；取值为`_v2`时，读取src的奇数索引位置。

src支持的数据类型为`uint8_t`，dst支持的数据类型为`uint16_t`

### 典型示例

```c
// 数据读取索引为偶数的位置
__simd_callee__ inline void asc_uint82uint16(vector_uint16_t& dst,
                                             vector_uint8_t src,
                                             vector_bool mask)
```

## 参数说明

**表1**参数说明

| 参数名 | 输入/输出 | 描述                                                                                                             |
| ------ | --------- | ---------------------------------------------------------------------------------------------------------------- |
| dst    | 输出      | 目的操作数（矢量数据寄存器）。                                                                                   |
| src    | 输入      | 源操作数（矢量数据寄存器）。                                                                                     |
| mask   | 输入      | 掩码寄存器，用于控制各元素是否参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。 |

矢量数据寄存器和掩码寄存器的详细说明请参见[reg数据类型定义](../reg_data_types/data_type_definition.md)。

## 返回值说明

无

## 流水类型

PIPE_V

## 约束说明

### 通用约束

- src与dst的数据类型需要与函数原型匹配。
- 掩码位为0时，dst对应元素置0。

## 调用示例

```cpp
vector_uint16_t dst;
vector_uint8_t src;
vector_bool mask;
mask = asc_create_mask_b8(PAT_ALL);
asc_uint82uint16(dst, src, mask);    // 取src的偶数索引元素进行转换并写入dst
asc_uint82uint16_v2(dst, src, mask);    // 取src的奇数索引元素进行转换并写入dst
```
