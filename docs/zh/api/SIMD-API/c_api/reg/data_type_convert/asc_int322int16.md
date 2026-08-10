# asc_int322int16

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

根据mask将src中的每个int32_t类型元素转化为int16_t类型，支持饱和/非饱和模式，结果写入dst。参考伪代码如下：
```python
def asc_int322int16(dst, src, mask):
    for i in range(64):
        if mask[i]:
            dst[2 * i] = int16(src[i])   # int32 -> int16, 写入偶数索引
        else:
            dst[2 * i] = 0
    for i in range(64):
        dst[2 * i + 1] = 0               # 奇数索引置零

def asc_int322int16_v2(dst, src, mask):
    for i in range(64):
        dst[2 * i] = 0                   # 偶数索引置零
        if mask[i]:
            dst[2 * i + 1] = int16(src[i])  # int32 -> int16, 写入奇数索引
        else:
            dst[2 * i + 1] = 0
```
由于源操作数与目的操作数类型位宽比为2:1，写入数据时需要将一个VL大小的数据分为两部分，根据不同接口将结果写入dst的偶数索引或奇数索引位置。

关于舍入模式和饱和/非饱和模式的详细说明，请参见[舍入模式](../data_type_convert/rounding_mode.md)。

由于源操作数与目的操作数类型位宽比为2:1，写入数据时需要将一个VL大小的数据分为两部分，根据不同接口选择数据写入索引为奇数的位置或偶数的位置。

- asc_int322int16：非饱和模式，数据写入索引为偶数的位置。

- asc_int322int16_sat：饱和模式，数据写入索引为偶数的位置。

- asc_int322int16_v2：非饱和模式，数据写入索引为奇数的位置。

- asc_int322int16_sat_v2：饱和模式，数据写入索引为奇数的位置。

## 函数原型

### 模板原型（占位符形式）

本接口包含2种分块位置和2种饱和模式，共4个原型，模板原型如下：

```c
__simd_callee__ inline void asc_int322int16<sat_suffix><v2_suffix>(vector_int16_t& dst,
                                                                  vector_int32_t src,
                                                                  vector_bool mask)
```

其中：

- `<sat_suffix>`为空时表示非饱和模式，取值为`_sat`时表示饱和模式。
- `<v2_suffix>`为空时，结果写入dst的偶数索引位置；取值为`_v2`时，结果写入dst的奇数索引位置。

### 典型示例

```c
// 非饱和模式，数据写入索引为偶数的位置
__simd_callee__ inline void asc_int322int16(vector_int16_t& dst,
                                            vector_int32_t src,
                                            vector_bool mask)
```

## 参数说明

**表1** 参数说明

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
__simd_vf__ inline void int322int16_vf(__ubuf__ int32_t* src_addr, __ubuf__ int16_t* dst_addr, uint32_t count, uint16_t one_repeat_size, uint16_t repeat_time)
{
    vector_int32_t src;
    vector_int16_t dst;
    vector_bool mask;
    for (uint16_t i = 0; i < repeat_time; ++i) {
        mask = asc_update_mask_b32(count);
        asc_loadalign_postupdate(src, src_addr, one_repeat_size);
        asc_int322int16(dst, src, mask);
        asc_store(dst_addr, dst);
        dst_addr += one_repeat_size * 2;
    }
}
```
