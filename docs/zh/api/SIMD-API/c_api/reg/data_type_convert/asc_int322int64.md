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

根据mask将src中的每个int32_t类型元素转换为int64_t类型，结果写入dst。伪代码如下：
```python
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

由于源操作数与目的操作数类型位宽比为1:2，读取数据时需要将一个VL大小的数据分为两部分，根据不同接口选择输入数据索引为奇数的位置或偶数的位置。

- asc_int322int64：取src的偶数索引元素（索引0, 2, 4, ...）进行转换。

- asc_int322int64_v2：取src的奇数索引元素（索引1, 3, 5, ...）进行转换。


## 函数原型

### 模板原型（占位符形式）

本接口包含2种分块位置，模板原型如下：

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

## 约束说明

### 通用约束

- src与dst的数据类型需要与函数原型匹配。
- mask掩码位为0时，dst对应元素置0。

## 调用示例

```cpp
__simd_vf__ inline void int322int64_vf(__ubuf__ int32_t* src_addr, __ubuf__ int64_t* dst_addr, uint32_t count, uint16_t one_repeat_size, uint16_t one_block_size, uint16_t repeat_time)
{
    vector_int32_t src;
    vector_int64_t dst;
    vector_bool mask;
    for (uint16_t i = 0; i < repeat_time; ++i) {
        mask = asc_update_mask_b32(count);
        asc_loadalign_postupdate(src, src_addr, one_repeat_size);
        asc_int322int64(dst, src, mask);
        asc_store(dst_addr, dst);
        dst_addr += one_block_size;
    }
}
```