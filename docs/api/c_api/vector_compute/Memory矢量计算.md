# Memory矢量计算

矢量计算类API，用于Unified Buffer上的矢量计算，单独使用时需要引入 `vector_compute.h`。

| API名称 | 说明 |
|----------|-----------|
| [asc_abs](asc_abs.md) | 按元素取绝对值。 |
| [asc_add](asc_add.md) | 按元素求和。 |
| [asc_add_relu](asc_add_relu.md) | 按元素求和，再进行Relu计算。 |
| [asc_add_scalar](asc_add_scalar.md) | 矢量内每个元素与标量求和。 |
| [asc_and](asc_and.md) | 执行矢量与运算。 |
| [asc_axpy](asc_axpy.md) | 源操作数src中每个元素与标量value求积后和目的操作数dst中的对应元素相加。 |
| [asc_bfloat162float](asc_bfloat162float.md) | 数据类型转换。将bfloat16_t类型的数据转换为float类型。 |
| [asc_bfloat162int32](asc_bfloat162int32.md) | 数据类型转换。将bfloat16_t类型的数据转换为int32_t类型。 |
| [asc_bitsort](asc_bitsort.md) | Score和Index分别存储在src0和src1中，按Score进行排序。 |
| [asc_brcb](asc_brcb.md) | 将源操作数中的每一个数填充到目的操作数的一个DataBlock中。 |
| [asc_copy](asc_copy.md) | 将数据从Unified Buffer搬运到Unified Buffer。支持高维切分。 |
| [asc_datablock_reduce_max](asc_datablock_reduce_max.md) | 对每个DataBlock内所有元素求最大值。 |
| [asc_datablock_reduce_min](asc_datablock_reduce_min.md) | 对每个DataBlock内所有元素求最小值。 |
| [asc_datablock_reduce_sum](asc_datablock_reduce_sum.md) | 对每个DataBlock内所有元素求和。 |
| [asc_deq_int162b8](asc_deq_int162b8.md) | 将int16_t类型转换为int8_t或uint8_t类型。 |
| [asc_deq_int322half](asc_deq_int322half.md) | 对输入的int32_t类型的数据按元素做量化并转换为half类型。 |
| [asc_div](asc_div.md) | 按元素求商。 |
| [asc_duplicate](asc_duplicate.md) | 将一个变量或立即数填充到一个矢量中。 |
| [asc_eq](asc_eq.md) | 比较src0与src1在对应索引位置的元素大小。 |
| [asc_eq_scalar](asc_eq_scalar.md) | 执行矢量与标量的比较运算。 |
| [asc_exp](asc_exp.md) | 按元素取自然指数。 |
| [asc_float2bfloat16](asc_float2bfloat16.md) | 数据类型转换。将float类型的数据转换为bfloat16_t类型。 |
| [asc_float2float](asc_float2float.md) | 数据类型转换。将float类型的数据转换为float类型。 |
| [asc_float2half](asc_float2half.md) | 数据类型转换。将float类型的数据转换为half类型。 |
| [asc_float2int16](asc_float2int16.md) | 将float类型数据转换为int16_t类型。 |
| [asc_float2int32](asc_float2int32.md) | 将float类型数据转换为int32_t类型。 |
| [asc_float2int64](asc_float2int64.md) | 将float类型数据转换为int64_t类型。 |
| [asc_fma](asc_fma.md) | 按元素将src0和src1相乘并和dst相加。 |
| [asc_gather](asc_gather.md) | 将源操作数按照给定的偏移按元素收集到目的操作数中。 |
| [asc_gather_datablock](asc_gather_datablock.md) | 根据偏移地址按照DataBlock的粒度将源操作数收集到目的操作数中。 |
| [asc_ge](asc_ge.md) | 逐元素比较src0 >= src1是否成立。 |
| [asc_ge_scalar](asc_ge_scalar.md) | 按元素判断src >= value是否成立。 |
| [asc_get_cmp_mask](asc_get_cmp_mask.md) | 获取Compare操作的比较结果。 |
| [asc_get_reduce_max_cnt](asc_get_reduce_max_cnt.md) | 获取执行asc_repeat_reduce_max操作后的最大值及索引。 |
| [asc_get_reduce_min_cnt](asc_get_reduce_min_cnt.md) | 获取执行asc_repeat_reduce_min操作后的最小值及索引。 |
| [asc_get_rsvd_count](asc_get_rsvd_count.md) | 获取GatherMask操作后剩余的元素数量。 |
| [asc_get_vms4_sr](asc_get_vms4_sr.md) | 获取执行asc_mrgsort4操作后的队列中已处理Region Proposal个数。 |
| [asc_gt](asc_gt.md) | 按元素比较两个矢量的大小关系。 |
| [asc_gt_scalar](asc_gt_scalar.md) | src中的每个元素逐个与标量value比较大小。 |
| [asc_half2float](asc_half2float.md) | 数据类型转换。将half类型的数据转换为float类型。 |
| [asc_half2int16](asc_half2int16.md) | 数据类型转换。将half类型的数据转换为int16_t类型。 |
| [asc_half2int4](asc_half2int4.md) | 数据类型转换。将half类型的数据转换为int4b_t类型。 |
| [asc_half2int8](asc_half2int8.md) | 数据类型转换。将half类型的数据转换为int8_t类型。 |
| [asc_half2int32](asc_half2int32.md) | 数据类型转换。将half类型的数据转换为int32_t类型。 |
| [asc_half2uint8](asc_half2uint8.md) | 将half类型数据转换为uint8_t类型，支持多种舍入模式。 |
| [asc_int162float](asc_int162float.md) | 数据类型转换。将int16_t类型的数据转换为float类型。 |
| [asc_int162half](asc_int162half.md) | 数据类型转换。将int16_t类型的数据转换为half类型。 |
| [asc_int322float](asc_int322float.md) | 将int32_t类型数据转换为float类型。 |
| [asc_int322int16](asc_int322int16.md) | 数据类型转换。将int32_t类型的数据转换为int16_t类型。 |
| [asc_int322int64](asc_int322int64.md) | 数据类型转换。将int32_t类型的数据转换为int64_t类型。 |
| [asc_int42half](asc_int42half.md) | 数据类型转换。将int4b_t类型的数据转换为half类型。 |
| [asc_int642float](asc_int642float.md) | 将int64_t类型数据转换为float类型。 |
| [asc_int642int32](asc_int642int32.md) | 数据类型转换。将int64_t类型的数据转换为int32_t类型。 |
| [asc_int82half](asc_int82half.md) | 数据类型转换。将int8_t类型的数据转换为half类型。 |
| [asc_le](asc_le.md) | 按元素判断src0 <= src1是否成立。 |
| [asc_le_scalar](asc_le_scalar.md) | 按元素判断src <= value是否成立。 |
| [asc_leakyrelu](asc_leakyrelu.md) | 执行矢量Leaky Relu运算。 |
| [asc_log](asc_log.md) | 按元素取自然对数。 |
| [asc_lt](asc_lt.md) | 按元素判断src0 < src1是否成立。 |
| [asc_lt_scalar](asc_lt_scalar.md) | 执行矢量中每个位置和标量比较。 |
| [asc_max](asc_max.md) | 按元素求最大值。 |
| [asc_max_scalar](asc_max_scalar.md) | 矢量内每个元素与标量求最大值。 |
| [asc_min](asc_min.md) | 按元素求最小值。 |
| [asc_min_scalar](asc_min_scalar.md) | 源操作数矢量逐元素与标量相比，取较小值。 |
| [asc_mrgsort4](asc_mrgsort4.md) | 将已经排好序的最多4条队列合并排列成1条队列。 |
| [asc_mul](asc_mul.md) | 按元素求积。 |
| [asc_mul_add](asc_mul_add.md) | 执行矢量的乘加运算。 |
| [asc_mul_add_relu](asc_mul_add_relu.md) | 按元素将src0和dst相乘并加上src1，再进行Relu计算。 |
| [asc_mul_cast_half2int8](asc_mul_cast_half2int8.md) | 按元素求积，并将结果转换为int8_t类型。 |
| [asc_mul_cast_half2uint8](asc_mul_cast_half2uint8.md) | 按元素求积，并将结果转换为uint8_t类型。 |
| [asc_mul_scalar](asc_mul_scalar.md) | 矢量内每个元素与标量求积。 |
| [asc_ne](asc_ne.md) | 按元素判断src0 != src1是否成立。 |
| [asc_ne_scalar](asc_ne_scalar.md) | 按元素判断是否不等于输入标量。 |
| [asc_not](asc_not.md) | 按元素做按位取反。 |
| [asc_or](asc_or.md) | 每对元素按位或运算。 |
| [asc_pair_reduce_sum](asc_pair_reduce_sum.md) | 对输入数据做归约操作，得到数据总和。 |
| [asc_rcp](asc_rcp.md) | 执行矢量的取倒数运算。 |
| [asc_reduce](asc_reduce.md) | 以内置固定模式对应的二进制从源操作数中选取元素写入目的操作数中。 |
| [asc_relu](asc_relu.md) | 按元素做线性整流Relu。 |
| [asc_repeat_reduce_max](asc_repeat_reduce_max.md) | 对每个Repeat内所有元素求最大值。 |
| [asc_repeat_reduce_min](asc_repeat_reduce_min.md) | 对每个Repeat内所有元素求最小值。 |
| [asc_repeat_reduce_sum](asc_repeat_reduce_sum.md) | 对每个Repeat内所有元素求和。 |
| [asc_rsqrt](asc_rsqrt.md) | 按元素进行开方后取倒数的计算。 |
| [asc_select](asc_select.md) | 根据掩码，从两个源操作数中选取元素，输出到目的操作数。 |
| [asc_set_cmp_mask](asc_set_cmp_mask.md) | 为Select操作设置用于选择的掩码。 |
| [asc_set_deq_scale](asc_set_deq_scale.md) | 设置DEQSCALAR寄存器的值。 |
| [asc_set_mask_count](asc_set_mask_count.md) | 设置Mask模式为Counter模式。 |
| [asc_set_mask_norm](asc_set_mask_norm.md) | 设置Mask模式为Normal模式。 |
| [asc_set_va_reg](asc_set_va_reg.md) | 用于设置transpose的地址。 |
| [asc_set_vector_mask](asc_set_vector_mask.md) | 设置Mask值。 |
| [asc_shiftleft](asc_shiftleft.md) | 对所有元素左移distance位。 |
| [asc_shiftright](asc_shiftright.md) | 对源操作数中的每个元素执行右移。 |
| [asc_sqrt](asc_sqrt.md) | 对元素进行开方。 |
| [asc_squeeze](asc_squeeze.md) | 通过比较掩码src1，将src0中的向量压缩成更短的向量。 |
| [asc_sub](asc_sub.md) | 按元素求差。 |
| [asc_sub_relu](asc_sub_relu.md) | 按元素求差，再进行Relu计算。 |
| [asc_sub_scalar](asc_sub_scalar.md) | 矢量内每个元素与标量求差。 |
| [asc_transto5hd](asc_transto5hd.md) | 数据格式转换，一般用于将NCHW格式转换成NC1HWC0格式。 |
| [asc_transpose](asc_transpose.md) | 用于实现16*16的二维矩阵数据块转置。 |
| [asc_uint82half](asc_uint82half.md) | 数据类型转换。将uint8_t类型的数据转换为half类型。 |
| [asc_vdeq_int162b8](asc_vdeq_int162b8.md) | 将int16_t类型转换为int8_t或uint8_t类型。 |
