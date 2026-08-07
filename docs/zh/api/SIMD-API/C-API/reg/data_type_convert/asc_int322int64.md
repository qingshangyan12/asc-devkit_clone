# asc_int322int64

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

将vector_int32_t类型的源操作数按偶数/奇数索引分为两组，读取其中一组元素，将其转换成vector_int64_t类型并写入目的操作数。

- asc_int322int64：读取源操作数的偶数索引元素，转换后填满目的操作数。

- asc_int322int64_v2：读取源操作数的奇数索引元素，转换后填满目的操作数。

$$
dst_i = (int64\_t)src_i
$$

```python
# src: int32[64]  (vector_int32_t, 256B = 64个int32元素)
# dst: int64[32]  (vector_int64_t, 256B = 32个int64元素)
# mask: bool[64]  (按src的b32位宽)
#
# 宽化转换(2:1): 64个int32按偶数/奇数索引分为两组, 每次读取其中一组32个元素
# asc_int322int64    读取偶数索引 src[0,2,...,62], 转换后填满dst全部32个位置
# asc_int322int64_v2 读取奇数索引 src[1,3,...,63], 转换后填满dst全部32个位置

def asc_int322int64(dst, src, mask):
    for i in range(32):
        if mask[2 * i]:                  # 偶数索引位置参与计算
            dst[i] = int64(src[2 * i])   # int32 -> int64 扩展转换
        else:
            dst[i] = 0                   # mask未选中, 置零

def asc_int322int64_v2(dst, src, mask):
    for i in range(32):
        if mask[2 * i + 1]:              # 奇数索引位置参与计算
            dst[i] = int64(src[2 * i + 1])
        else:
            dst[i] = 0
```

## 函数原型

### 模板原型

```c
__simd_callee__ inline void asc_int322int64<v2_suffix>(vector_int64_t& dst,
                                                       vector_int32_t src,
                                                       vector_bool mask)
```

`<v2_suffix>`为空时，读取src的偶数索引位置元素；取值为`_v2`时，读取src的奇数索引位置元素。

src支持的数据类型为`int32_t`，dst支持的数据类型为`int64_t`

### 典型示例

```c
// 数据写入索引为偶数的位置
__simd_callee__ inline void asc_int322int64(vector_int64_t& dst,
                                            vector_int32_t src,
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

- src与dst的数据类型需要与函数原型匹配。
- 掩码位为0时，dst对应元素置0。

## 调用示例

```cpp
vector_int64_t dst;
vector_int32_t src;
vector_bool mask;
mask = asc_create_mask_b32(PAT_ALL);
asc_int322int64(dst, src, mask);    // 将src的偶数索引元素转换成vector_int64_t类型并写入dst
asc_int322int64_v2(dst, src, mask);    // 将src的奇数索引元素转换成vector_int64_t类型并写入dst
```
