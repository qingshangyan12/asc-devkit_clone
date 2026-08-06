# asc_int162half

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

将src中int16_t类型的元素转换为half类型（16位浮点数），并将结果写入dst。源操作数和目的操作数位宽相同，转换后元素数量不变。支持5种舍入模式，关于舍入模式的详细说明请参见[舍入模式与饱和模式](rounding_mode.md)。

## 函数原型

```cpp
// 向负无穷方向舍入（rd）
__simd_callee__ inline void asc_int162half_rd(vector_half& dst,
                                              vector_int16_t src,
                                              vector_bool mask)
// 四舍六入五成双舍入（rn）
__simd_callee__ inline void asc_int162half_rn(vector_half& dst,
                                              vector_int16_t src,
                                              vector_bool mask)
// 四舍五入舍入（rna）
__simd_callee__ inline void asc_int162half_rna(vector_half& dst,
                                               vector_int16_t src,
                                               vector_bool mask)
// 向正无穷方向舍入（ru）
__simd_callee__ inline void asc_int162half_ru(vector_half& dst,
                                              vector_int16_t src,
                                              vector_bool mask)
// 向零方向舍入（rz）
__simd_callee__ inline void asc_int162half_rz(vector_half& dst,
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
- src的dtype必须为`int16_t`，dst的dtype必须为`half`，dtype固定不可混用，否则触发异常。
- mask为`vector_bool`类型掩码寄存器，需通过掩码设置接口预先赋值后再传入；未赋值的掩码寄存器内容不确定，会导致有效元素位置错误。
- 掩码比特位按元素大小步进过滤：每个`int16_t`元素占2字节，对应占用2个mask比特位，元素i的有效mask比特位于`i × 2`位置。
- 参与计算的元素个数由矢量长度（VL）决定，`int16_t`占2字节，单次处理元素个数 = VL ÷ 2（如VL为256字节时为128个元素）。
- 掩码为假的位置采用清零模式：dst对应位置写0，不保留src原值也不保留dst原值。
- `int16_t`全体值均在`half`可表示范围内，转换不会触发越界处理；但`half`尾数10 bit仅能精确表示绝对值不超过2048（2^11）的整数，绝对值在(2048, 32767]区间的`int16_t`值转换时需按选定舍入模式舍入到`half`最近可表示值，舍入误差不超过0.5 ULP，不触发异常。
- 源与目的寄存器重叠时行为不保证：dst与src寄存器号重叠时，转换结果不保证正确性，硬件不报错但数据可能被覆盖。
- 本接口在Vector Function（`__simd_vf__`标记的函数）内调用，dst与src为Reg矢量计算寄存器，不涉及UB地址，无UB容量与地址对齐约束。
- 本接口由接口封装实现类型转换，无对应底层指令名，行为约束以本接口原型参数与整数到半精度浮点转换语义为准。

### 舍入模式变体约束

- 舍入模式通过接口名后缀选择，5种后缀对应的舍入语义如下：`_rd`向负无穷舍入（向下取整）；`_rn`向最近且偶数舍入（四舍六入五凑偶，IEEE 754 roundTiesToEven）；`_rna`向远离零舍入（四舍五入，IEEE 754 roundTiesToAway）；`_ru`向正无穷舍入（向上取整）；`_rz`向零舍入（截断取整）。
- 舍入作用于`int16_t`转换为`half`时的尾数低位补全：`int16_t`整数值先按`half`编码格式分解为符号 + 指数 + 尾数，绝对值超过2048的整数尾数低位超出`half` 10 bit尾数范围，超出部分按舍入模式处理。各舍入模式下舍入边界值（如0.5 ULB处）的取整方向不同，但均不触发异常。
- 不同舍入模式变体的源、目、掩码参数完全一致，仅舍入方向不同；绝对值不超过2048的整数在5种舍入模式下结果完全一致（精确表示，无舍入误差），差异仅体现在绝对值超过2048的整数上。

## 调用示例

```cpp
__simd_vf__ inline void int162half_vf(__ubuf__ half* dst_addr, __ubuf__ int16_t* src_addr, uint32_t count, uint16_t one_repeat_size, uint16_t repeat_time)
{
    vector_half dst;
    vector_int16_t src;
    vector_bool mask;
    for (uint16_t i = 0; i < repeat_time; ++i) {
        mask = asc_update_mask_b16(count);
        asc_loadalign(src, src_addr + i * one_repeat_size);
        asc_int162half_rd(dst, src, mask);
        asc_storealign(dst_addr + i * one_repeat_size, dst, mask);
    }
}
```
