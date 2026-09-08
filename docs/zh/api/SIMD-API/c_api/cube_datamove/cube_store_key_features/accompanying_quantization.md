# 随路量化<a name="ZH-CN_TOPIC_0000002568950983"></a>

## 特性说明

矩阵搬出支持Cast和带系数的量化。Cast使用`asc_quant_mode::F322F16`或`asc_quant_mode::F322BF16`将float转换成half/bfloat16_t，无需量化系数，舍入采用CAST_RINT。不开启转换时使用`asc_quant_mode::NoQuant`，并匹配源/目的类型。

带参数量化包括两种粒度：

- Scalar：整个C矩阵共享一个量化参数。3510优先用[asc_set_l0c_copy_prequant(scale, offset, is_signed)](../asc_set_l0c_copy_prequant.md)配置；2201的同名接口使用打包后的`uint64_t config`，低32位必须是float的位模式，不能把浮点值数值转换成uint64_t。
- Vector：C矩阵每一列（N方向）对应一个8B量化参数，参数shape为[N]。先用[asc_copy_l12fb](../asc_copy_l12fb/asc_copy_l12fb.md)从L1搬入量化区，再设置起始地址索引。3510使用[asc_set_l0c_copy_config](../asc_set_l0c_copy_config.md)，2201使用[asc_set_l0c2gm_config](../asc_set_l0c2gm_config.md)。量化地址索引为量化区字节地址除以128，不是元素下标；激活地址索引为激活区字节地址除以64。两个参数区分别寻址，起始地址与长度都不能超过对应区域。

配置系数只写寄存器，不会自动选择搬出模式；还需设置搬出调用的`quant_pre_mode`（2201参数名为`quant_pre`）。本文模式名称均为`asc_quant_mode`的成员。

当前支持的量化模式及其功能描述如下：

```text
DEQF16,                // DeQuant_Float16：int32_t反量化成half，scalar量化
VDEQF16,               // Vector_DeQuant_Float16：int32_t反量化成half，tensor量化
QF322B8_PRE,           // Quant_Float32_2_B8：float量化成int8_t/uint8_t，scalar量化
VQF322B8_PRE,          // Vector_Quant_Float32_2_B8：float量化成int8_t/uint8_t，tensor量化
REQ8,                  // ReQuant_int8：int32_t重量化成int8_t/uint8_t，scalar量化
VREQ8,                 // Vector_ReQuant_int8：int32_t重量化成int8_t/uint8_t，tensor量化
```
<!-- npu="950" id1 -->
除上述量化模式外，Ascend 950PR/Ascend 950DT产品还额外支持以下量化模式：

```text
QF322FP8_PRE,          // Quant_Float32_2_FP8: float量化成fp8_e4m3fn_t，scalar量化
VQF322FP8_PRE,         // Vector_Quant_Float32_2_FP8: float量化成fp8_e4m3fn_t，tensor量化
QF322HIF8_PRE,         // Quant_Float32_2_HIF8: float量化成hifloat8_t(Half to Away Round)，scalar量化
VQF322HIF8_PRE,        // Vector_Quant_Float32_2_HIF8: float量化成hifloat8_t(Half to Away Round)，tensor量化
QF322HIF8_PRE_HYBRID,  // Quant_Float32_2_HIF8_Hybrid: float量化成hifloat8_t(Hybrid Round)，scalar量化
VQF322HIF8_PRE_HYBRID, // Vector_Quant_Float32_2_HIF8_Hybrid: float量化成hifloat8_t(Hybrid Round)，tensor量化
QS322BF16_PRE,         // Quant_Int32_2_BFloat16: int32_t量化成bfloat16_t，scalar量化
VQS322BF16_PRE,        // Vector_Quant_Int32_2_BFloat16: int32_t量化成bfloat16_t，tensor量化
QF322F16_PRE,          // Quant_Float32_2_Float16: float量化成half，scalar量化
VQF322F16_PRE,         // Vector_Quant_Float32_2_Float16: float量化成half，tensor量化
QF322BF16_PRE,         // Quant_Float32_2_BFloat16: float量化成bfloat16_t，scalar量化
VQF322BF16_PRE,        // Vector_Quant_Float32_2_BFloat16: float量化成bfloat16_t，tensor量化
QF322F32_PRE,          // Quant_Float32_2_Float32: float量化成float，scalar量化，使用有限精度的量化系数，需按输入范围验证误差
VQF322F32_PRE,         // Vector_Quant_Float32_2_Float32: float量化成float，tensor量化，使用有限精度的量化系数，需按输入范围验证误差
```
<!-- end id1 -->

**C API补充模式与通路：**

| 模式 | 输入→输出 | 2201 | 3510 |
| --- | --- | --- | --- |
| `REQ4` / `VREQ4` | int32_t→int4b_t，Scalar/Vector | L0C→L1对应4位重载 | L0C→GM/L1/UB |
| `QF322S4_PRE` / `VQF322S4_PRE` | float→int4b_t，Scalar/Vector | L0C→L1对应4位重载 | L0C→GM/L1/UB |

3510 FP8量化输出类型为`fp8_e4m3fn_t`，不能因为矩阵输入支持`fp8_e5m2_t`就推定搬出也有同类型量化重载。HiFloat8的普通和HYBRID模式舍入不同，应与下游数值格式约定一致。4位输出的物理小分形和通道合并见[Int8 Channel Merge及位宽扩展](Int8-Channel-Merge.md)。

不同量化模式的量化算法详细介绍参见[随路量化与随路ReLU场景组合](accompanying_quantization_and_relu_scenario_combination.md)。

<!-- npu="950,A3,910b" id2 -->
量化参数比特位含义见下表：

<!-- npu="A3,910b" id3 -->
**表1** 量化参数QUANT\_PRE比特位含义映射表（[NPU架构版本2201](../../../../../guide/programming_guide/language_extension/simd_builtin_keywords.md)）<a id="zh-cn_topic_0000002547300781_table1273124313613"></a>

| 模式 | 比特位数 | 变量名 | 作用介绍 |
| ------ | ---------- | -------- | ---------- |
| &bull;(V)REQ8<br>&bull;(V)QF322B8_PRE<br>&bull;(V)DEQF16 | 0~31 | M1 | 32位数视为float，硬件以(1, 8, 10)的格式，即1个符号位、8个指数位和10个尾数位作为量化计算所需要乘的值。其中31位为符号位，23~30位为指数位，13~22位为尾数位。 |
| &bull;(V)REQ8<br>&bull;(V)QF322B8_PRE<br>&bull;(V)DEQF16 | 32~35 | N | 4位比特位，表示范围为\[1, 16\]（b'0000对应表示1，b'1111对应表示16）。<br>当模式为(V)REQ8时，MCB标志位置为1时，将输入的值进行右移N比特位。当模式为(V)QF322B8_PRE，N为无效变量。 |
| &bull;(V)REQ8<br>&bull;(V)QF322B8_PRE<br>&bull;(V)DEQF16 | 36 | MCB标志位 | Mode Control Bit。如果置为0，输入的int32_t会被直接转换为float，N为无效值。如果置为1，输入的int32_t会先右移N比特位，转变成int16_t，然后转换为float。当模式为(V)QF322B8_PRE时，此标志位为无效比特。 |
| &bull;(V)REQ8<br>&bull;(V)QF322B8_PRE<br>&bull;(V)DEQF16 | 37~45 | Offset | 9bit的整型数据，源数据乘以量化系数或者随路系数的计算结果，可以与Offset表示的整数值进行相加。若不使用offset，请置为0。<br>当模式为(V)DEQF16时，此变量为无效变量。 |
| &bull;(V)REQ8<br>&bull;(V)QF322B8_PRE<br>&bull;(V)DEQF16 | 46 | Sign标志位 | 如果置为1，表明量化结果是signed(int8)；如果置为0，表明量化结果是unsigned(uint8)。仅在(V)REQ8、(V)QF322B8_PRE中会用到。 |
| &bull;(V)REQ8<br>&bull;(V)QF322B8_PRE<br>&bull;(V)DEQF16 | 47~63 | - | 无效比特位，用户无需关注。 |

<!-- end id3 -->

<!-- npu="950" id4 -->
**表2** 量化参数QUANT\_PRE比特位含义映射表（[NPU架构版本3510](../../../../../guide/programming_guide/language_extension/simd_builtin_keywords.md)）<a id="zh-cn_topic_0000002547300781_table1273124313714"></a>

| 模式 | 比特位数 | 变量名 | 作用介绍 |
| --- | --- | --- | --- |
| &bull;(V)REQ8<br>&bull;(V)QF322B8_PRE<br>&bull;(V)DEQF16<br>&bull;(V)QF322FP8_PRE<br>&bull;(V)QF322HIF8_PRE<br>&bull;(V)QF322HIF8_PRE_HYBRID<br>&bull;(V)QS322BF16_PRE<br>&bull;(V)QF322F16_PRE<br>&bull;(V)QF322BF16_PRE<br>&bull;(V)QF322F32_PRE<br>&bull;(V)REQ4<br>&bull;(V)QF322S4_PRE | 0~12 | - | 无效比特位，用户无需关注。 |
| 同第一行 | 13~31 | M1 | 数据类型视为float，硬件以(1, 8, 10)的格式，即1个符号位、8个指数位和10个尾数位作为量化计算所需要乘的值。其中31位为符号位，23~30位为指数位，13~22位为尾数位。其值不能为inf/nan。 |
| 同第一行 | 32~36 | - | 无效比特位，用户无需关注。 |
| 同第一行 | 37~45 | Offset | 9bit整形数据，源数据乘以量化系数或者随路系数的计算结果，可以与Offset表示的整数值进行相加。若不使用offset，请置为0。当模式为(V)REQ8、(V)QF322B8_PRE、(V)REQ4、(V)QF322S4_PRE时该变量生效。 |
| 同第一行 | 46 | Sign标志位 | 如果置为1，表明量化结果是signed(int8)；如果为置为0，表明量化结果是unsigned(uint8)。仅在(V)REQ8、(V)QF322B8_PRE中会用到。 |
| 同第一行 | 47~63 | - | 无效比特位，用户无需关注。 |

<!-- end id4 -->
<!-- end id2 -->

## 使用示例

以下为3510的float→int8_t Scalar量化片段，系数为2、偏移为3、有符号输出，输出ND；`src`为已就绪的16×16 L0C矩阵，`dst`为足够大的GM输出。

```cpp
asc_set_l0c_copy_prequant(2.0f, 3, true);
asc_set_l0c_copy_nz_para(1, 0, 0);
asc_copy_l0c2gm(dst, src, 16, 16, 16, 16,
    asc_store_l2_cache_mode::NORMAL_FIRST_VICTIM, asc_unit_flag_mode::DISABLE,
    asc_quant_mode::QF322B8_PRE, asc_relu_pre_mode::NONE,
    false, true, false, false);
```

Vector量化将模式改为对应的V前缀枚举，并在搬出前完成系数搬入与地址配置。3510量化区4KB最多容纳512个8B系数；较大的N需要按实际容量分片，逐片更新数据和地址。2201量化区为2KB，可容纳256个8B系数。

QUANT_PRE中的M1是(1,8,10)有限精度浮点位模式。3510的低13位无效，写入`scale`后不会保留完整float精度。系数不能为Inf/NaN，使用接口约束允许的有限规格化值；Sign位控制B8有符号/无符号输出，不改变源矩阵的类型。
