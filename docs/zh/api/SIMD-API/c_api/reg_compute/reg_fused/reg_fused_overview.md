# Reg复合计算概述

Reg复合计算接口将两种或多种基础运算组合为一条Reg矢量计算指令，输入和输出均为矢量数据寄存器，通过`mask`控制参与计算的元素。与分别调用基础运算接口相比，复合计算可以减少指令数量，避免中间结果在寄存器之间的搬运。

所有Reg复合计算接口仅在AIV上生效，且需在Vector Function（`__simd_vf__`标记的函数）内调用；除`asc_exp_sub_half2float`等特殊接口外，输入与输出数据类型保持一致。

## 接口概览

**表1** Reg复合计算接口

| 接口名称 | 功能简述 | 计算公式 | 支持数据类型 |
| --- | --- | --- | --- |
| [asc_abs_sub](asc_abs_sub.md) | 按元素计算两个源操作数的差值绝对值。 | $dst_i = \lvert src0_i - src1_i \rvert$ | half、float |
| [asc_axpy](asc_axpy.md) | 源操作数与标量相乘后累加到目的操作数，结果写回`dst`。 | $dst_i = src_i \times value + dst_i$ | half、float |
| [asc_exp_sub](asc_exp_sub.md) | 计算两个源操作数差值的自然指数。`asc_exp_sub_half2float`用于half输入，读取偶数或奇数索引元素并升精度为float。 | float：$dst_i = e^{src0_i - src1_i}$<br>half：$dst_i = e^{float(src0_{2i}) - float(src1_{2i})}$（偶数索引）<br>$dst_i = e^{float(src0_{2i+1}) - float(src1_{2i+1})}$（奇数索引） | float；half输入（`half2float`变体，输出float） |
| [asc_fma](asc_fma.md) | 两个源操作数相乘后与第三个源操作数相加，结果通过返回值返回。 | $result_i = src0_i \times src1_i + src2_i$ | half、bfloat16_t、float |
| [asc_leakyrelu](asc_leakyrelu.md) | 负半轴斜率由标量`alpha`统一指定的Leaky ReLU。 | $dst_i = \begin{cases} src_i & src_i > 0 \\ src_i \times \alpha & src_i \leq 0 \end{cases}$ | half、float |
| [asc_madd](asc_madd.md) | 目的操作数与源操作数相乘后加另一个源操作数，结果写回`dst`。 | $dst_i = dst_i \times src0_i + src1_i$ | half、bfloat16_t、float |
| [asc_mul_scalar_float2half_rn](asc_mul_scalar_float2half_rn.md) | float源操作数与标量相乘，按ROUND舍入模式转换为half，并写入`dst`的偶数或奇数索引位置。 | $dst_i = round\_to\_half(src_i \times value)$ | float输入、half输出 |
| [asc_mula](asc_mula.md) | 两个源操作数相乘后累加到目的操作数，结果写回`dst`。 | $dst_i = src0_i \times src1_i + dst_i$ | int16_t、uint16_t、half、bfloat16_t、int32_t、uint32_t、float |
| [asc_prelu](asc_prelu.md) | 负半轴斜率由矢量`alpha`逐元素指定的PReLU。 | $dst_i = \begin{cases} src0_i & src0_i > 0 \\ src0_i \times \alpha_i & src0_i \leq 0 \end{cases}$ | half、float |
| [asc_relu](asc_relu.md) | 按元素执行ReLU，负值置0。 | $dst_i = (src_i \geq 0) ? src_i : 0$ | half、int32_t、float |

## 关键特性

复合计算在减少指令数量的同时，会改变中间结果精度、输出数据类型或输出布局。以下从算子实现角度说明这些特殊能力，以及易混接口之间的差异。

### 融合乘加

`asc_axpy`、`asc_madd`、`asc_mula`、`asc_fma`均完成乘加运算，区别在于累加项来源、操作数形式和结果写回方式。

**表2** 乘加类接口对比

| 接口名称 | 计算公式 | 累加项 | 操作数形式 | 结果写回方式 |
| --- | --- | --- | --- | --- |
| [asc_axpy](asc_axpy.md) | $dst_i = src_i \times value + dst_i$ | 原`dst` | 矢量×标量 | 写回`dst` |
| [asc_madd](asc_madd.md) | $dst_i = dst_i \times src0_i + src1_i$ | 原`dst` | 矢量×矢量 | 写回`dst` |
| [asc_mula](asc_mula.md) | $dst_i = src0_i \times src1_i + dst_i$ | 原`dst` | 矢量×矢量 | 写回`dst` |
| [asc_fma](asc_fma.md) | $result_i = src0_i \times src1_i + src2_i$ | `src2` | 矢量×矢量 | 通过返回值返回 |

- `asc_axpy`的乘数为标量，一条指令即可完成标量缩放与累加，不需要额外准备标量广播寄存器。
- `asc_madd`以`dst`作为一个乘数，`asc_mula`以`dst`作为加数。二者虽然都写回`dst`，但`dst`参与的运算位置不同，选择时需注意初始值的含义。
- `asc_fma`不读取`dst`，三个操作数均为输入，结果通过函数返回值返回，不修改已有寄存器。
- `asc_mula`额外支持int16_t、uint16_t、int32_t、uint32_t整型，其余三个接口仅支持浮点类型。
- `asc_axpy`、`asc_madd`、`asc_mula`和`asc_fma`均以融合乘加方式计算，乘积不单独舍入，只在最终结果处舍入一次。与先执行乘法再执行加法相比，可避免中间乘积的二次舍入误差；其中`asc_axpy`、`asc_madd`、`asc_mula`的硬件精度标准等效于`Fma(x, y, z)`，详见[Reg矢量计算接口精度标准汇总](../../../../appendix/reg_vector_compute_interface_precision_standard_summary.md)。

### 激活函数

`asc_relu`、`asc_leakyrelu`、`asc_prelu`的区别在于负半轴斜率的取值方式和来源。

**表3** 激活类接口对比

| 接口名称 | 负半轴斜率 | 斜率来源 | 计算公式 | 支持数据类型 |
| --- | --- | --- | --- | --- |
| [asc_relu](asc_relu.md) | 0 | 固定为0 | $dst_i = (src_i \geq 0) ? src_i : 0$ | half、int32_t、float |
| [asc_leakyrelu](asc_leakyrelu.md) | $\alpha$ | 标量立即数 | $dst_i = \begin{cases} src_i & src_i > 0 \\ src_i \times \alpha & src_i \leq 0 \end{cases}$ | half、float |
| [asc_prelu](asc_prelu.md) | $\alpha_i$ | 矢量数据寄存器逐元素指定 | $dst_i = \begin{cases} src0_i & src0_i > 0 \\ src0_i \times \alpha_i & src0_i \leq 0 \end{cases}$ | half、float |

- `asc_relu`将负半轴直接置0，且额外支持int32_t。
- `asc_leakyrelu`使用同一个标量`alpha`作为所有元素的负半轴斜率。
- `asc_prelu`允许每个元素使用不同的负半轴斜率。

### 计算差值

**表4** 差值类接口对比

| 接口名称 | 计算公式 | 输出数据类型 | 说明 |
| --- | --- | --- | --- |
| [asc_abs_sub](asc_abs_sub.md) | $dst_i = \lvert src0_i - src1_i \rvert$ | 与输入一致 | 输出差值的绝对值，输入和输出数据类型需一致。 |
| [asc_exp_sub](asc_exp_sub.md) | $dst_i = e^{src0_i - src1_i}$ | float | 输入为float，将差值作为自然指数的指数。 |
| [asc_exp_sub_half2float](asc_exp_sub.md) | 偶数索引：$dst_i = e^{float(src0_{2i}) - float(src1_{2i})}$<br>奇数索引：$dst_i = e^{float(src0_{2i+1}) - float(src1_{2i+1})}$ | float | 输入为half，按`src_pos`选择偶数或奇数索引元素，升精度后计算。 |

- `asc_abs_sub`在一条指令内完成减法和取绝对值，输出沿用输入类型，结果始终非负。
- `asc_exp_sub`在同一条指令内完成减法和自然指数运算，输出恒为正，但差值较大时结果可能超出float的可表示范围，差值较小时结果趋近于0，写算子时需要对输入差值范围或输出结果做保护。
- `asc_exp_sub_half2float`读取一个half矢量寄存器上的元素，只取偶数或奇数索引的元素参与计算，结果填入一个float数据类型的矢量寄存器。

处理连续且完整的half数据时，可先分别计算偶数索引和奇数索引的结果，再用[asc_intlv](../reg_permute_sel/asc_intlv.md)按元素交织，最后分两次搬出到连续地址，使输出顺序与输入一致。以下按每轮一个VL（128个half类型元素）循环处理：

```cpp
__simd_vf__ inline void exp_sub_half_vf(__ubuf__ float* dst, __ubuf__ half* src0,
                                        __ubuf__ half* src1, uint16_t repeat_times)
{
    vector_half src0_reg;
    vector_half src1_reg;
    vector_float even_reg;
    vector_float odd_reg;
    vector_float dst0_reg;
    vector_float dst1_reg;
    vector_bool mask_b16 = asc_create_mask_b16(PAT_ALL);
    vector_bool mask_b32 = asc_create_mask_b32(PAT_ALL);

    for (uint16_t i = 0; i < repeat_times; ++i) {
        asc_loadalign(src0_reg, src0 + i * 128);
        asc_loadalign(src1_reg, src1 + i * 128);
        asc_exp_sub_half2float(even_reg, src0_reg, src1_reg, mask_b16, ASC_POSITION_EVEN);
        asc_exp_sub_half2float(odd_reg, src0_reg, src1_reg, mask_b16, ASC_POSITION_ODD);
        asc_intlv(dst0_reg, dst1_reg, even_reg, odd_reg);
        asc_storealign(dst + i * 128, dst0_reg, mask_b32);
        asc_storealign(dst + i * 128 + 64, dst1_reg, mask_b32);
    }
}
```

### 标量乘法融合精度转换

[asc_mul_scalar_float2half_rn](asc_mul_scalar_float2half_rn.md)在一条指令内完成标量乘法以及float到half数据类型的ROUND模式舍入转换。由于float源与half目的的元素数比为2:1，单次调用只填充`dst`的偶数或奇数索引，另一半位置被置0。写算子时，连续处理完整float数据时，每次取一个VL量级的float类型数据，将偶数索引结果压缩搬出，即可得到连续的输出：

```cpp
__simd_vf__ inline void muls_cast_half_vf(__ubuf__ half* dst, __ubuf__ float* src,
                                          float value, uint16_t repeat_times)
{
    vector_float src_reg;
    vector_half dst_reg;
    vector_bool mask = asc_create_mask_b32(PAT_ALL);

    for (uint16_t i = 0; i < repeat_times; ++i) {
        asc_loadalign(src_reg, src + i * 64);
        asc_mul_scalar_float2half_rn(dst_reg, src_reg, value, mask, ASC_POSITION_EVEN);
        asc_storealign_pack((__ubuf__ int32_t*)dst + i * 32, (vector_int32_t)dst_reg, mask);
    }
}
```

## 通用约束

- Reg矢量计算C API通用约束请参见[通用约束](../overview.md#通用约束)。
- 接口仅在AIV上生效，需在Vector Function内调用；在非AIV上通过引用参数输出结果的接口直接返回，通过返回值输出结果的接口返回对应矢量类型的默认构造值。
- 接口将`mask`中为0的比特位对应的输出元素置0，`mask`需通过掩码设置接口预先赋值。
- 除`asc_exp_sub_half2float`、`asc_mul_scalar_float2half_rn`等接口外，源操作数与目的操作数的数据类型需保持一致。
- 本组接口仅Ascend 950PR&950DT系列产品支持。
