# asc_int162float

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

将src中int16_t类型的元素转换为float类型，并将结果写入dst。

由于源操作数与目的操作数类型位宽比为1:2，读取数据时需要将一个VL大小的数据分为两部分，根据不同接口选择输入数据索引为奇数的位置或偶数的位置。

## 函数原型

```cpp
// 取src的偶数索引元素（索引0, 2, 4, ...）进行转换，mask每2bit为一组、仅组内LSB为1时选取对应位置的int16_t元素。
__simd_callee__ inline void asc_int162float(vector_float& dst,
                                            vector_int16_t src,
                                            vector_bool mask)
// 取src的奇数索引元素（索引1, 3, 5, ...）进行转换，mask每2bit为一组、仅组内LSB为1时选取对应位置的int16_t元素。
__simd_callee__ inline void asc_int162float_v2(vector_float& dst,
                                               vector_int16_t src,
                                               vector_bool mask)
```

## 参数说明

**表1** 参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| dst | 输出 | 目的操作数（矢量数据寄存器）。 |
| src | 输入 | 源操作数（矢量数据寄存器）。 |
| mask | 输入 | 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。。 |

矢量数据寄存器和掩码寄存器的详细说明请参见[reg数据类型定义](../reg_data_types/data_type_definition.md)。

## 返回值说明

无

## 约束说明

mask控制源操作数是否参与计算，源操作数不参与计算的元素在输出对应位置置零。


### 通用约束

- **本接口仅在AIV上生效，非AIV调用直接返回。**
- src的dtype必须为`int16_t`，dst的dtype必须为`float`，dtype固定不可混用，否则触发异常。
- mask为`vector_bool`类型掩码寄存器，需通过掩码设置接口预先赋值后再传入；未赋值的掩码寄存器内容不确定，会导致有效元素位置错误。
- 掩码为假的位置采用清零模式：dst对应位置写0，不保留src原值也不保留dst原值。
- `int16_t`与`float`的转换属精度无损的短转长扩展：`int16_t`全部取值范围 [−2^15, 2^15−1] 均可在`float`尾数24 bit（含隐含最高位）中精确表示，转换无舍入、无饱和、无溢出；不触发异常。
- 本接口由接口封装实现类型转换，无对应底层指令名，行为约束以本接口原型参数与短转长扩展语义为准。
- 本接口在Vector Function（`__simd_vf__`标记的函数）内调用，dst与src为矢量数据寄存器，不涉及UB地址，无UB容量与地址对齐约束。

### 分块位置变体约束

- 本接口提供2个分块位置（偶数、奇数），对应接口名后缀`_v2`的变体使用奇数分块位置；不带`_vN`后缀的主原型使用偶数分块位置。
- 主原型仅转换源矢量寄存器中索引为偶数的`int16_t`元素，`asc_int162float_v2`仅转换索引为奇数的`int16_t`元素，两者配合可完成源矢量寄存器全部元素的转换；单独调用其中一个原型只处理一半元素，另一半元素不参与本次转换。
- 不同分块位置变体的源、目、掩码参数完全一致，仅分块位置不同；不同分块位置的结果可独立并行写入，不会相互覆盖。

## 调用示例

```cpp
__simd_vf__ inline void int162float_vf(__ubuf__ float* dst_addr, __ubuf__ int16_t* src_addr, uint32_t count, uint16_t one_repeat_size, uint16_t repeat_time)
{
    vector_float dst;
    vector_int16_t src;
    vector_bool mask;
    for (uint16_t i = 0; i < repeat_time; ++i) {
        mask = asc_update_mask_b16(count);
        asc_loadalign(src, src_addr + i * one_repeat_size);
        asc_int162float(dst, src, mask);
        asc_storealign(dst_addr + i * one_repeat_size, dst, mask);
    }
}
```
