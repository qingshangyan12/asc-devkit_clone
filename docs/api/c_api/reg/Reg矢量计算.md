# Reg矢量计算

寄存器级别的矢量计算API，包括寄存器数据类型定义、Reg数据搬运和Reg矢量计算操作。

## 寄存器数据类型

| API名称 | 说明 |
|----------|-----------|
| [reg数据类型定义](reg数据类型定义.md) | Reg相关数据类型定义。 |

## Reg数据搬运

Reg数据搬运类API，位于 `reg_load/` 和 `reg_store/` 目录下。

| API名称 | 说明 |
|----------|-----------|
| [asc_loadalign](reg_load/asc_loadalign/asc_loadalign_v1.md) | 对齐数据搬运接口，从UB连续对齐搬入目的操作数，支持多种搬入模式。 |
| [asc_loadalign_postupdate](reg_load/asc_loadalign_postupdate.md) | 将数据从UB搬入MaskReg，使能post mod。 |
| [asc_storealign](reg_store/asc_storealign/asc_storealign_v1.md) | reg计算数据搬运接口，适用于从矢量数据寄存器连续对齐搬出到UB的场景。 |
| [asc_gather](reg_load/asc_gather.md) | 根据索引位置index将源操作数src按元素收集到目的操作数dst中。 |
| [asc_gather_datablock](reg_load/asc_gather_datablock.md) | 根据索引位置将源操作数按DataBlock收集到目的操作数中。 |
| [asc_get_mask_spr](reg_load/asc_get_mask_spr.md) | 从特殊寄存器SPR读取mask值。 |
| [asc_load](reg_load/asc_load.md) | reg计算数据搬运接口，支持从UB非32字节对齐的源地址搬运至矢量数据寄存器。 |
| [asc_loadunalign](reg_load/asc_loadunalign.md) | reg计算数据搬运接口，适用于从UB非32B对齐的起始地址连续搬入矢量数据寄存器的场景。 |
| [asc_loadunalign_pre](reg_load/asc_loadunalign_pre.md) | 用于在进行非对齐数据搬入前的初始化。 |
| [asc_scatter](reg_load/asc_scatter.md) | 数据散射操作。 |
| [asc_store](reg_store/asc_store.md) | reg计算数据搬运接口，适用于从矢量数据寄存器搬出到UB的场景。 |
| [asc_storeunalign](reg_store/asc_storeunalign.md) | reg计算数据搬运接口，适用于从矢量数据寄存器连续非32B对齐搬出到UB的场景。 |
| [asc_storeunalign_postupdate](reg_store/asc_storeunalign_postupdate.md) | reg计算数据搬运接口，适用于非对齐搬出并更新地址的场景。 |

## Reg矢量计算

Reg矢量计算类API，位于 `reg_vector/` 目录下。

| API名称 | 说明 |
|----------|-----------|
| [asc_abs](reg_vector/asc_abs.md) | 逐元素计算绝对值。 |
| [asc_abs_sub](reg_vector/asc_abs_sub.md) | 逐元素计算差的绝对值。 |
| [asc_add](reg_vector/asc_add.md) | 按元素执行加法运算。 |
| [asc_addc](reg_vector/asc_addc.md) | 按元素执行带进位的加法运算。 |
| [asc_add_scalar](reg_vector/asc_add_scalar.md) | 按元素执行矢量和标量的加法运算。 |
| [asc_and](reg_vector/asc_and.md) | 对掩码寄存器进行逻辑与运算。 |
| [asc_arange](reg_vector/asc_arange.md) | 以传入的value为起始值，生成递增/递减的索引。 |
| [asc_axpy](reg_vector/asc_axpy.md) | 按元素做乘加操作。 |
| [asc_bfloat162e1m2x2](reg_vector/asc_bfloat162e1m2x2.md) | 将bfloat16_t类型转换为fp4x2_e1m2_t类型。 |
| [asc_bfloat162e2m1x2](reg_vector/asc_bfloat162e2m1x2.md) | 将bfloat16_t类型转换为fp4x2_e2m1_t类型。 |
| [asc_bfloat162float](reg_vector/asc_bfloat162float.md) | 将bfloat16_t类型转换为float类型。 |
| [asc_bfloat162half](reg_vector/asc_bfloat162half.md) | 将bfloat16_t类型转换为half类型。 |
| [asc_bfloat162int32](reg_vector/asc_bfloat162int32.md) | 将bfloat16_t类型数据转换为int32_t类型。 |
| [asc_clear_ar_spr](reg_vector/asc_clear_ar_spr.md) | 对AR寄存器进行清理。 |
| [asc_copy](reg_vector/asc_copy.md) | 完成数据在寄存器内的搬运。 |
| [asc_create_iter_reg](reg_vector/asc_create_iter_reg.md) | 地址寄存器初始化。 |
| [asc_create_mask](reg_vector/asc_create_mask.md) | 根据入参生成相应的掩码寄存器。 |
| [asc_cumulative_histogram](reg_vector/asc_cumulative_histogram.md) | 对直方图数据进行累计统计。 |
| [asc_deintlv](reg_vector/asc_deintlv.md) | 将src0和src1中的元素解交织存入dst0和dst1中。 |
| [asc_div](reg_vector/asc_div.md) | 按元素执行除法运算。 |
| [asc_duplicate](reg_vector/asc_duplicate.md) | 根据mask将源操作数src的最低位元素填充到目的操作数dst。 |
| [asc_duplicate_scalar](reg_vector/asc_duplicate_scalar.md) | 根据mask将value填充到目的操作数dst。 |
| [asc_e1m2x22bfloat16](reg_vector/asc_e1m2x22bfloat16.md) | 将fp4x2_e1m2_t类型转换为bfloat16_t类型。 |
| [asc_e2m1x22bfloat16](reg_vector/asc_e2m1x22bfloat16.md) | 将fp4x2_e2m1_t类型转换为bfloat16_t类型。 |
| [asc_e4m32float](reg_vector/asc_e4m32float.md) | 将fp8_e4m3fn_t类型转换为float类型。 |
| [asc_e5m22float](reg_vector/asc_e5m22float.md) | 将vector_fp8_e5m2_t类型转换为vector_float类型。 |
| [asc_eq](reg_vector/asc_eq.md) | 对源操作数逐元素比较是否相等。 |
| [asc_eq_scalar](reg_vector/asc_eq_scalar.md) | 对源操作数逐元素比较是否与标量相等。 |
| [asc_exp](reg_vector/asc_exp.md) | 对源操作数逐元素计算指数。 |
| [asc_exp_sub](reg_vector/asc_exp_sub.md) | 将src0与src1相减，差值作为e的指数计算。 |
| [asc_float2bfloat16](reg_vector/asc_float2bfloat16.md) | 将float类型转换为bfloat16_t类型。 |
| [asc_float2e4m3](reg_vector/asc_float2e4m3.md) | 将float类型转换为fp8_e4m3fn_t类型。 |
| [asc_float2e5m2](reg_vector/asc_float2e5m2.md) | 将float类型数据转换为fp8_e5m2类型。 |
| [asc_float2half](reg_vector/asc_float2half.md) | 将float类型转换为half类型。 |
| [asc_float2hif8](reg_vector/asc_float2hif8.md) | 将float类型转换为hifloat8_t类型。 |
| [asc_float2int16](reg_vector/asc_float2int16.md) | 将float类型转换为int16_t类型。 |
| [asc_float2int32](reg_vector/asc_float2int32.md) | 将float类型转换为int32_t类型。 |
| [asc_float2int64](reg_vector/asc_float2int64.md) | 将float类型转换为int64_t类型。 |
| [asc_frequency_histogram](reg_vector/asc_frequency_histogram.md) | 对直方图数据进行频率统计。 |
| [asc_ge](reg_vector/asc_ge.md) | 对源操作数执行逐元素比较 (>=)。 |
| [asc_ge_scalar](reg_vector/asc_ge_scalar.md) | 对源操作数与标量执行逐元素比较 (>=)。 |
| [asc_gt](reg_vector/asc_gt.md) | 对源操作数执行逐元素比较 (>)。 |
| [asc_gt_scalar](reg_vector/asc_gt_scalar.md) | 对源操作数与标量执行逐元素比较 (>)。 |
| [asc_half2bf16](reg_vector/asc_half2bf16.md) | 将half类型数据转换为bfloat16_t类型。 |
| [asc_half2float](reg_vector/asc_half2float.md) | 将half类型数据转换为float类型。 |
| [asc_half2hif8](reg_vector/asc_half2hif8.md) | 将half类型转换为hifloat8_t类型。 |
| [asc_half2int16](reg_vector/asc_half2int16.md) | 将half类型数据转换为int16_t类型。 |
| [asc_half2int32](reg_vector/asc_half2int32.md) | 将half类型数据转换为int32_t类型。 |
| [asc_half2int4x2](reg_vector/asc_half2int4x2.md) | 将half类型数据转换为int4x2_t类型。 |
| [asc_half2int8](reg_vector/asc_half2int8.md) | 将half类型转换为int8_t类型。 |
| [asc_half2uint8](reg_vector/asc_half2uint8.md) | 将half类型转换为uint8_t类型。 |
| [asc_hif82float](reg_vector/asc_hif82float.md) | 将hifloat8_t类型数据转换为float类型。 |
| [asc_hif82half](reg_vector/asc_hif82half.md) | 将hifloat8_t类型数据转换为half类型。 |
| [asc_int162float](reg_vector/asc_int162float.md) | 将int16_t类型转换为float类型。 |
| [asc_int162half](reg_vector/asc_int162half.md) | 将int16_t类型转换为half类型。 |
| [asc_int162int32](reg_vector/asc_int162int32.md) | 将int16_t类型转换为int32_t类型。 |
| [asc_int162uint32](reg_vector/asc_int162uint32.md) | 将int16_t类型转换为uint32_t类型。 |
| [asc_int162uint8](reg_vector/asc_int162uint8.md) | 将int16_t类型转换为uint8_t类型。 |
| [asc_int322float](reg_vector/asc_int322float.md) | 将int32_t类型转换为float类型。 |
| [asc_int322int16](reg_vector/asc_int322int16.md) | 将int32_t类型转换为int16_t类型。 |
| [asc_int322int64](reg_vector/asc_int322int64.md) | 将int32_t类型转换为int64_t类型。 |
| [asc_int322uint16](reg_vector/asc_int322uint16.md) | 将int32_t类型转换为uint16_t类型。 |
| [asc_int322uint8](reg_vector/asc_int322uint8.md) | 将int32_t类型转换为uint8_t类型。 |
| [asc_int4x22bfloat16](reg_vector/asc_int4x22bfloat16.md) | 将int4x2_t类型转换为bfloat16_t类型。 |
| [asc_int4x22half](reg_vector/asc_int4x22half.md) | 将int4x2_t类型数据转换为half类型。 |
| [asc_int4x22int16](reg_vector/asc_int4x22int16.md) | 将int4x2_t类型转换为int16_t类型。 |
| [asc_int642float](reg_vector/asc_int642float.md) | 将int64_t类型数据转换为float类型。 |
| [asc_int642int32](reg_vector/asc_int642int32.md) | 将int64_t类型转换为int32_t类型。 |
| [asc_int82half](reg_vector/asc_int82half.md) | 将int8_t类型转换为half类型。 |
| [asc_int82int16](reg_vector/asc_int82int16.md) | 将int8_t类型转换为int16_t类型。 |
| [asc_int82int32](reg_vector/asc_int82int32.md) | 将int8_t类型转换为int32_t类型。 |
| [asc_intlv](reg_vector/asc_intlv.md) | 将src0和src1中的元素交织存入dst0和dst1中。 |
| [asc_le](reg_vector/asc_le.md) | 对源操作数执行逐元素比较 (<=)。 |
| [asc_leakyrelu](reg_vector/asc_leakyrelu.md) | 按元素执行Leaky ReLU操作。 |
| [asc_le_scalar](reg_vector/asc_le_scalar.md) | 对源操作数与标量执行逐元素比较 (<=)。 |
| [asc_ln](reg_vector/asc_ln.md) | 对源操作数逐元素计算自然对数。 |
| [asc_lt](reg_vector/asc_lt.md) | 对源操作数执行逐元素比较 (<)。 |
| [asc_lt_scalar](reg_vector/asc_lt_scalar.md) | 对源操作数与标量执行逐元素比较 (<)。 |
| [asc_madd](reg_vector/asc_madd.md) | 对源操作数执行逐元素乘法和加法。 |
| [asc_max](reg_vector/asc_max.md) | 按元素求最大值。 |
| [asc_max_scalar](reg_vector/asc_max_scalar.md) | 按元素求矢量和标量的最大值。 |
| [asc_mem_bar](reg_vector/asc_mem_bar.md) | Reg计算宏函数内不同流水线之间的同步指令。 |
| [asc_min](reg_vector/asc_min.md) | 按元素求最小值操作。 |
| [asc_min_scalar](reg_vector/asc_min_scalar.md) | 源操作数矢量内每个元素与标量比较，取较小值。 |
| [asc_mul](reg_vector/asc_mul.md) | 对源操作数进行乘法运算。 |
| [asc_mull](reg_vector/asc_mull.md) | 按元素相乘，将结果写入dst0，溢出部分写入dst1。 |
| [asc_muls](reg_vector/asc_muls.md) | 对源操作数和value进行乘法运算并转为half类型。 |
| [asc_mul_scalar](reg_vector/asc_mul_scalar.md) | 对源操作数、value进行按元素乘法操作。 |
| [asc_ne](reg_vector/asc_ne.md) | 对源操作数执行逐元素比较 (!=)。 |
| [asc_neg](reg_vector/asc_neg.md) | 根据mask对源操作数src进行取相反数操作。 |
| [asc_ne_scalar](reg_vector/asc_ne_scalar.md) | src中的每个元素逐个与标量value比较大小 (!=)。 |
| [asc_not](reg_vector/asc_not.md) | 执行矢量非运算。 |
| [asc_or](reg_vector/asc_or.md) | 对源操作数进行或运算。 |
| [asc_pack](reg_vector/asc_pack.md) | 将源操作数中的元素选取低N位写入目的操作数。 |
| [asc_pair_reduce_sum](reg_vector/asc_pair_reduce_sum.md) | 相邻两个元素求和。 |
| [asc_prelu](reg_vector/asc_prelu.md) | PReLU操作。 |
| [asc_reduce_max](reg_vector/asc_reduce_max.md) | 归约最大值操作。 |
| [asc_reduce_max_datablock](reg_vector/asc_reduce_max_datablock.md) | 找出DataBlock中的最大值。 |
| [asc_reduce_min](reg_vector/asc_reduce_min.md) | 归约最小值操作。 |
| [asc_reduce_min_datablock](reg_vector/asc_reduce_min_datablock.md) | 找出DataBlock中的最小值。 |
| [asc_reduce_sum](reg_vector/asc_reduce_sum.md) | 归约求和。 |
| [asc_reduce_sum_datablock](reg_vector/asc_reduce_sum_datablock.md) | 对每个DataBlock归约求和。 |
| [asc_relu](reg_vector/asc_relu.md) | 逐元素执行ReLU运算。 |
| [asc_select](reg_vector/asc_select.md) | 根据mask从src0、src1中选择元素。 |
| [asc_shiftleft](reg_vector/asc_shiftleft.md) | 按元素左移操作。 |
| [asc_shiftleft_scalar](reg_vector/asc_shiftleft_scalar.md) | 按标量左移操作。 |
| [asc_shiftright](reg_vector/asc_shiftright.md) | 按元素右移操作。 |
| [asc_shiftright_scalar](reg_vector/asc_shiftright_scalar.md) | 按标量右移操作。 |
| [asc_sqrt](reg_vector/asc_sqrt.md) | 逐元素计算平方根。 |
| [asc_squeeze](reg_vector/asc_squeeze.md) | 将src中被mask选择的有效元素依次复制到dst。 |
| [asc_sub](reg_vector/asc_sub.md) | 按元素执行减法运算。 |
| [asc_subc](reg_vector/asc_subc.md) | 按元素执行带借位的减法运算。 |
| [asc_truncate](reg_vector/asc_truncate.md) | 将浮点数元素截断到整数位。 |
| [asc_uint162uint32](reg_vector/asc_uint162uint32.md) | 将uint16_t类型转换为uint32_t类型。 |
| [asc_uint162uint8](reg_vector/asc_uint162uint8.md) | 将uint16_t类型转换为uint8_t类型。 |
| [asc_uint322int16](reg_vector/asc_uint322int16.md) | 将uint32_t类型转换为int16_t类型。 |
| [asc_uint322uint16](reg_vector/asc_uint322uint16.md) | 将uint32_t类型转换为uint16_t类型。 |
| [asc_uint322uint8](reg_vector/asc_uint322uint8.md) | 将uint32_t类型转换为uint8_t类型。 |
| [asc_uint82half](reg_vector/asc_uint82half.md) | 将uint8_t类型转换为half类型。 |
| [asc_uint82uint16](reg_vector/asc_uint82uint16.md) | 将uint8_t类型转换为uint16_t类型。 |
| [asc_uint82uint32](reg_vector/asc_uint82uint32.md) | 将uint8_t类型转换为uint32_t类型。 |
| [asc_unpack](reg_vector/asc_unpack.md) | 矢量解包操作。 |
| [asc_unsqueeze](reg_vector/asc_unsqueeze.md) | 根据mask进行解压缩。 |
| [asc_update_mask](reg_vector/asc_update_mask.md) | 根据value大小生成对应的掩码寄存器。 |
| [asc_xor](reg_vector/asc_xor.md) | 按元素异或操作。 |
