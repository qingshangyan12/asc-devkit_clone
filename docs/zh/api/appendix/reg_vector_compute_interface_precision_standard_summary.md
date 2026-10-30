# Reg矢量计算接口精度标准汇总<a name="reg-vector-precision-summary"></a>

本节汇总介绍Basic API和C API的Reg矢量计算接口精度标准，本节内容仅针对如下型号生效：

Ascend 950PR&950DT系列产品

**表1**  Basic API Reg矢量计算API精度标准<a id="table-precision-standard"></a>

| 接口类型 | 接口 | 硬件指令精度标准 | 软仿精度标准 |
| --- | --- | --- | --- |
| 基础算术 | `Exp` | 1ulp, not support denormalized numbers | 1ulp, support denormalized numbers |
| 基础算术 | `Sqrt` | 1ulp, not support denormalized numbers | &bull; float: 0ulp <br>&bull; half/float: 1ulp, support denormalized numbers |
| 基础算术 | `Add` | &bull; half/float: 0ulp, support denormalized numbers<br>&bull; bfloat16_t: Add(x, y) = rint_to_bf16(Add(cast_to_f32(x), cast_to_f32(y))) | 0ulp, support denormalized numbers |
| 基础算术 | `Sub` | Sub(x, y) = Add(x, Neg(y)) | 0ulp, support denormalized numbers |
| 基础算术 | `Adds` | &bull; half/float: 0ulp, support denormalized numbers<br>&bull; bfloat16_t: Add(x, y) = rint_to_bf16(Add(cast_to_f32(x), cast_to_f32(y))) | 0ulp, support denormalized numbers |
| 基础算术 | `Div` | 1ulp, not support denormalized numbers | &bull; complex64: 0ulp, not support denormalized numbers <br>&bull; float: 0ulp, support denormalized numbers <br>&bull; half/float: 1ulp, support denormalized numbers |
| 基础算术 | `Neg` | 符号位取反 | - |
| 基础算术 | `Ln/Log` | 1ulp, not support denormalized numbers | 1ulp, support denormalized numbers |
| 基础算术 | `Log2/Log10` | - | &bull; 1ulp, not support denormalized numbers<br>&bull; 1ulp, support denormalized numbers |
| 基础算术 | `Mul` | &bull; half/float: 0ulp, support denormalized numbers<br>&bull; bfloat16_t: Mul(x, y) = rint_to_bf16(Mul(cast_to_f32(x), cast_to_f32(y))) | - |
| 基础算术 | `Muls` | half/float: 0ulp, support denormalized numbers | - |
| 复合指令 | `AbsSub` | 精度等效于先减后取绝对值，AbsSub(x) = Abs(Sub(x)) | - |
| 复合指令 | `ExpSub` | &bull; 输入half，输出float：<br>ExpSub(x, y) = Exp(Sub(cast_to_f32(x), cast_to_f32(y)))<br>&bull; 输入float，输出float：<br>ExpSub(x, y) = Exp(Sub(x, y)) | - |
| 复合指令 | `Axpy` | 结果等效于：乘累加采用高精度计算，同比Fma(x, y, z) | - |
| 复合指令 | `MulDstAdd` | 结果等效于：乘累加采用高精度计算，同比Fma(x, y, z) | - |
| 复合指令 | `MulAddDst` | 结果等效于：乘累加采用高精度计算，同比Fma(x, y, z) | - |
| 复合指令 | `MulsCast` | MulsCast(x, y) = cast_to_f16&lt;roundingmode&gt;(x * y) | - |
| 归约计算 | `Reduce` | half/float: 0ulp, 采用二叉树结构的成对累加策略 | - |
| 归约计算 | `ReduceDataBlock` | half/float: 0ulp, 采用二叉树结构的成对累加策略 | - |
| 归约计算 | `PairReduceElem` | half/float: 0ulp | - |

**表2**  Basic API低精度指令支持情况<a id="table-low-precision"></a>

| 数据类型 | 接口 |
| --- | --- |
| fp4x2_e2m1_t | `Cast`、`Duplicate`（软仿） |
| fp4x2_e1m2_t | `Cast`、`Duplicate`（软仿） |
| fp8_e5m2_t | `Cast`、`Duplicate`（软仿） |
| fp8_e4m3fn_t | `Cast`、`Duplicate`（软仿） |
| fp8_e8m0_t | `Cast`、`Duplicate`（软仿） |
| hifloat8_t | `Cast`、`Duplicate`（软仿） |
| int4x2_t | `Cast` |
| int8_t | `Cast`、`Add`、`Sub`、`Max`、`Min`、`Compare`、`Abs`、`Neg`、`Adds`、`Maxs`、`Mins`、`Compares`、`Duplicate` |
| uint8_t | `Cast`、`Add`、`Sub`、`Max`、`Min`、`Compare`、`Adds`、`Maxs`、`Mins`、`Compares`、`Duplicate` |

**表3**  C API Reg矢量计算API精度标准<a id="table-c-api-precision-standard"></a>

| 接口类型 | 接口 | 硬件指令精度标准 | 软仿精度标准 |
| --- | --- | --- | --- |
| 基础算术 | `asc_exp` | 1ulp, not support denormalized numbers | - |
| 基础算术 | `asc_sqrt` | 1ulp, not support denormalized numbers | - |
| 基础算术 | `asc_add` | &bull; half/float: 0ulp, support denormalized numbers<br>&bull; bfloat16_t: Add(x, y) = rint_to_bf16(Add(cast_to_f32(x), cast_to_f32(y))) | - |
| 基础算术 | `asc_sub` | Sub(x, y) = Add(x, Neg(y)) | - |
| 基础算术 | `asc_add_scalar` | &bull; half/float: 0ulp, support denormalized numbers<br>&bull; bfloat16_t: Add(x, y) = rint_to_bf16(Add(cast_to_f32(x), cast_to_f32(y))) | - |
| 基础算术 | `asc_div` | 1ulp, not support denormalized numbers | - |
| 基础算术 | `asc_neg` | half/float：符号位取反 | - |
| 基础算术 | `asc_ln`/`asc_log` | 1ulp, not support denormalized numbers | - |
| 基础算术 | `asc_mul` | &bull; half/float: 0ulp, support denormalized numbers<br>&bull; bfloat16_t: Mul(x, y) = rint_to_bf16(Mul(cast_to_f32(x), cast_to_f32(y))) | - |
| 基础算术 | `asc_mul_scalar` | half/float: 0ulp, support denormalized numbers | - |
| 复合指令 | `asc_abs_sub` | 精度等效于先减后取绝对值，AbsSub(x) = Abs(Sub(x)) | - |
| 复合指令 | `asc_exp_sub`/`asc_exp_sub_half2float` | &bull; 输入half，输出float：<br>ExpSub(x, y) = Exp(Sub(cast_to_f32(x), cast_to_f32(y)))<br>&bull; 输入float，输出float：<br>ExpSub(x, y) = Exp(Sub(x, y)) | - |
| 复合指令 | `asc_axpy` | 结果等效于：乘累加采用高精度计算，同比Fma(x, y, z) | - |
| 复合指令 | `asc_madd` | 结果等效于：乘累加采用高精度计算，同比Fma(x, y, z) | - |
| 复合指令 | `asc_mula` | 结果等效于：乘累加采用高精度计算，同比Fma(x, y, z) | - |
| 复合指令 | `asc_mul_scalar_float2half_rn` | float×float转half，固定采用RN舍入；结果为 `cast_to_f16_rn(x * y)` | - |
| 归约计算 | `asc_reduce_sum` | half/float: 0ulp, 采用二叉树结构的成对累加策略 | - |
| 归约计算 | `asc_reduce_sum_datablock` | half/float: 0ulp, 采用二叉树结构的成对累加策略 | - |
| 归约计算 | `asc_pair_reduce_sum` | half/float: 0ulp | - |

说明：[表1](#table-precision-standard)中的Log2/Log10当前无对应的C API，因此未在表3中列出。

C API需要计算Log2或Log10时，可调用`asc_ln`，再使用换底公式并通过`asc_mul_scalar`乘以倒数常量：

$$
\log_2(x) = \frac{\ln(x)}{\ln(2)} = \ln(x) \times \frac{1}{\ln(2)}
$$

$$
\log_{10}(x) = \frac{\ln(x)}{\ln(10)} = \ln(x) \times \frac{1}{\ln(10)}
$$

**表4**  C API低精度指令支持情况<a id="table-c-api-low-precision"></a>

| 数据类型 | 接口 |
| --- | --- |
| fp4x2_e2m1_t | `asc_bfloat162e2m1x2_<round_mode>`、`asc_e2m1x22bfloat16` |
| fp4x2_e1m2_t | `asc_bfloat162e1m2x2_<round_mode>`、`asc_e1m2x22bfloat16` |
| fp8_e5m2_t | `asc_float2e5m2_rn<sat_mode>`、`asc_e5m22float`、`asc_duplicate`、`asc_duplicate_merge`、`asc_duplicate_highest`、`asc_duplicate_highest_merge`、`asc_duplicate_scalar`、`asc_duplicate_scalar_merge` |
| fp8_e4m3fn_t | `asc_float2e4m3_rn<sat_mode>`、`asc_e4m32float`、`asc_duplicate`、`asc_duplicate_merge`、`asc_duplicate_highest`、`asc_duplicate_highest_merge`、`asc_duplicate_scalar`、`asc_duplicate_scalar_merge` |
| fp8_e8m0_t | `asc_duplicate`、`asc_duplicate_merge`、`asc_duplicate_highest`、`asc_duplicate_highest_merge`、`asc_duplicate_scalar`、`asc_duplicate_scalar_merge` |
| hifloat8_t | `asc_float2hif8_<round_mode><sat_mode>`、`asc_half2hif8_<round_mode><sat_mode>`、`asc_hif82float`、`asc_hif82half` |
| int4x2_t | `asc_half2int4x2_<round_mode><sat_mode>`、`asc_int4x22bfloat16`、`asc_int4x22half`、`asc_int4x22int16` |
| int8_t | 转换：`asc_half2int8_<round_mode><sat_mode>`、`asc_int82half`、`asc_int82int16`、`asc_int82int32`<br>算术：`asc_add`、`asc_sub`、`asc_max`、`asc_min`、`asc_abs`、`asc_neg`、`asc_add_scalar`、`asc_max_scalar`、`asc_min_scalar`<br>比较：`asc_eq`、`asc_ne`、`asc_lt`、`asc_le`、`asc_gt`、`asc_ge`、`asc_eq_scalar`、`asc_ne_scalar`、`asc_lt_scalar`、`asc_le_scalar`、`asc_gt_scalar`、`asc_ge_scalar`<br>广播：`asc_duplicate`、`asc_duplicate_merge`、`asc_duplicate_highest`、`asc_duplicate_highest_merge`、`asc_duplicate_scalar`、`asc_duplicate_scalar_merge` |
| uint8_t | 转换：`asc_half2uint8_<round_mode><sat_mode>`、`asc_int162uint8_<sat_mode>`、`asc_int322uint8_<sat_mode>`、`asc_uint162uint8_<sat_mode>`、`asc_uint322uint8_<sat_mode>`、`asc_uint82half`、`asc_uint82uint16`、`asc_uint82uint32`<br>算术：`asc_add`、`asc_sub`、`asc_max`、`asc_min`、`asc_add_scalar`、`asc_max_scalar`、`asc_min_scalar`<br>比较：`asc_eq`、`asc_ne`、`asc_lt`、`asc_le`、`asc_gt`、`asc_ge`、`asc_eq_scalar`、`asc_ne_scalar`、`asc_lt_scalar`、`asc_le_scalar`、`asc_gt_scalar`、`asc_ge_scalar`<br>广播：`asc_duplicate`、`asc_duplicate_merge`、`asc_duplicate_highest`、`asc_duplicate_highest_merge`、`asc_duplicate_scalar`、`asc_duplicate_scalar_merge` |

其中：

- `asc_bfloat162e2m1x2_<round_mode>`和`asc_bfloat162e1m2x2_<round_mode>`的源数据类型为`bfloat16_t`，`<round_mode>`可取：`rd`（FLOOR，向下取整）、`rn`（RINT，按RINT规则舍入）、`rna`（ROUND，舍入到最近整数）、`ru`（CEIL，向上取整）和`rz`（TRUNC，向零截断）。
- `asc_half2int4x2_<round_mode><sat_mode>`、`asc_half2int8_<round_mode><sat_mode>`和`asc_half2uint8_<round_mode><sat_mode>`的源数据类型为`half`，`<round_mode>`可取：`rd`（FLOOR，向下取整）、`rn`（RINT，按RINT规则舍入）、`rna`（ROUND，舍入到最近整数）、`ru`（CEIL，向上取整）和`rz`（TRUNC，向零截断）。
- `asc_float2e5m2_rn<sat_mode>`和`asc_float2e4m3_rn<sat_mode>`固定使用`rn`舍入。
- `asc_float2hif8_<round_mode><sat_mode>`和`asc_half2hif8_<round_mode><sat_mode>`的`<round_mode>`可取`rh`（HIFLOAT8专用舍入模式）和`rna`（ROUND，舍入到最近整数）。
- `<sat_mode>`可为空（非饱和）或取`_sat`（饱和）。`asc_int162uint8`、`asc_int322uint8`、`asc_uint162uint8`和`asc_uint322uint8`仅支持不带后缀和带`_sat`后缀两种形式，不支持`<round_mode>`。
