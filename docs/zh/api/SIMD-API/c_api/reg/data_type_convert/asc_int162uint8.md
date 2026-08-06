# asc_int162uint8

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

将src中int16_t类型的元素转换为uint8_t类型，并将结果写入dst。支持非饱和与饱和两种模式。

由于源操作数与目的操作数类型位宽比为2:1，写入数据时需要将一个VL大小的数据分为两部分，根据不同接口选择数据写入索引为奇数的位置或偶数的位置。

## 函数原型

```cpp
// 非饱和模式，结果写入dst各组内第0个位置，直接截断。mask每2bit为一组，仅组内LSB为1时选取对应位置的int16_t元素。
__simd_callee__ inline void asc_int162uint8(vector_uint8_t& dst,
                                            vector_int16_t src,
                                            vector_bool mask)
// 非饱和模式，结果写入dst各组内第1个位置，直接截断。mask每2bit为一组，仅组内LSB为1时选取对应位置的int16_t元素。
__simd_callee__ inline void asc_int162uint8_v2(vector_uint8_t& dst,
                                               vector_int16_t src,
                                               vector_bool mask)
// 饱和模式，超出[0, 255]范围的值钳位到边界。结果写入dst各组内第0个位置。mask每2bit为一组，仅组内LSB为1时选取对应位置的int16_t元素。
__simd_callee__ inline void asc_int162uint8_sat(vector_uint8_t& dst,
                                                vector_int16_t src,
                                                vector_bool mask)
// 饱和模式，超出[0, 255]范围的值钳位到边界。结果写入dst各组内第1个位置。mask每2bit为一组，仅组内LSB为1时选取对应位置的int16_t元素。
__simd_callee__ inline void asc_int162uint8_sat_v2(vector_uint8_t& dst,
                                                   vector_int16_t src,
                                                   vector_bool mask)
```

## 参数说明

**表1** 参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| dst | 输出 | 目的操作数（矢量数据寄存器）。 |
| src | 输入 | 源操作数（矢量数据寄存器）。 |
| mask | 输入 | 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。 |

矢量数据寄存器和掩码寄存器的详细说明请参见[reg数据类型定义](../reg_data_types/data_type_definition.md)。

## 返回值说明

无

## 约束说明

mask控制源操作数是否参与计算，源操作数不参与计算的元素在输出对应位置置零。


### 通用约束

- **本接口仅在AIV上生效，非AIV调用直接返回。**
- src的dtype必须为`int16_t`，dst的dtype必须为`uint8_t`，dtype固定不可混用，否则触发异常。
- mask为`vector_bool`类型掩码寄存器，需通过掩码设置接口预先赋值后再传入；未赋值的掩码寄存器内容不确定，会导致有效元素位置错误。
- 掩码比特位按元素大小步进过滤：每个`int16_t`元素占2字节，对应占用2个mask比特位，元素i的有效mask比特位于`i × 2`位置。
- 参与计算的元素个数由矢量长度（VL）决定，`int16_t`占2字节，单次处理源元素个数 = VL ÷ 2（如VL为256字节时为128个源元素）。
- 掩码为假的位置采用清零模式：dst对应位置写0，不保留src原值也不保留dst原值。
- `int16_t` → `uint8_t`为整数到整数的窄化转换：源16 bit补码截断为低8 bit后以无符号整数重新解释为`uint8_t`，无舍入模式；输入值域 [−2^15, 2^15−1] 远大于`uint8_t`可表示范围 [0, 255]，存在越界场景，按本接口饱和/非饱和变体的越界处理策略处理。
- 本接口采用分块位置选取源元素，仅转换源矢量寄存器中被分块位置选中的`int16_t`元素，结果从dst起始位置连续写入；未被选中的源元素不参与本次转换。
- 源与目的寄存器重叠时行为不保证：dst与src寄存器号重叠时，转换结果不保证正确性，硬件不报错但数据可能被覆盖。
- 本接口由接口封装实现类型转换，无对应底层指令名，行为约束以本接口原型参数与窄化截断语义为准。
- 本接口在Vector Function（`__simd_vf__`标记的函数）内调用，dst与src为Reg矢量计算寄存器，不涉及UB地址，无UB容量与地址对齐约束。

### 分块位置变体约束

- 本接口提供2个分块位置变体：不带`_v2`后缀的主原型使用偶数分块位置选取源矢量寄存器中的`int16_t`元素进行转换（即每两个相邻源元素取索引为偶数的一个），结果从目的矢量寄存器起始位置连续写入；带`_v2`后缀的变体使用奇数分块位置选取源矢量寄存器中索引为奇数的`int16_t`元素进行转换，结果从目的矢量寄存器起始位置连续写入。
- 同一组源、目、掩码参数下不同分块位置变体的参数列表完全一致，仅分块位置不同；偶数分块与奇数分块的结果独立写入，不会相互覆盖。

### 饱和变体约束

- 本接口提供2个饱和行为变体：不带`_sat`后缀的变体关闭饱和行为；带`_sat`后缀的变体开启饱和行为。饱和行为与分块位置独立组合，共4个变体。
- 非饱和变体（不带`_sat`后缀）在`int16_t`输入超出`uint8_t`可表示范围 [0, 255] 时，转换结果对源16 bit补码截断低8 bit后以无符号整数重新解释为`uint8_t`，发生回绕。例如输入256截断低8 bit为0；输入257回绕为1；输入−1的16 bit补码为0xFFFF，截断低8 bit为0xFF，回绕为255；输入−2回绕为254。硬件不报错但结果与数学期望不一致。
- 饱和变体（带`_sat`后缀）在`int16_t`输入超出`uint8_t`可表示范围 [0, 255] 时，转换结果饱和到`uint8_t`上下界（0或UINT8_MAX=255）：输入 < 0饱和到0，输入 > 255饱和到255，不触发异常。
- 同一组源、目、掩码参数下饱和与非饱和变体的参数列表完全一致，仅越界处理行为不同。

## 调用示例

```cpp
__simd_vf__ inline void int162uint8_vf(__ubuf__ uint8_t* dst_addr, __ubuf__ int16_t* src_addr, uint32_t count, uint16_t one_repeat_size, uint16_t repeat_time)
{
    vector_uint8_t dst;
    vector_int16_t src;
    vector_bool mask;
    for (uint16_t i = 0; i < repeat_time; ++i) {
        mask = asc_update_mask_b16(count);
        asc_loadalign(src, src_addr + i * one_repeat_size);
        asc_int162uint8(dst, src, mask);
        asc_storealign(dst_addr + i * one_repeat_size, dst, mask);
    }
}
```
