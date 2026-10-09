# 随路量化

## 特性说明

矩阵搬出支持Cast和带系数的量化。

随路处理发生在L0C Buffer结果搬出的过程中，可在一次搬运中完成输出类型转换或按系数量化。C API通过[asc_copy_l0c2gm](../cube_compute_store/asc_copy_l0c2gm.md)、[asc_copy_l0c2l1](../cube_compute_store/asc_copy_l0c2l1.md)使用这些能力。应先确定源/目的数据类型、输出布局和搬出通路，再选择对应架构支持的量化模式；枚举定义见[asc_quant_mode](../../defs/enum/asc_quant_mode.md)。

<!-- npu="950" id10 -->
Ascend 950PR&950DT系列产品还可通过[asc_copy_l0c2ub](../cube_compute_store/asc_copy_l0c2ub.md)使用随路量化。
<!-- end id10 -->

量化模式与量化系数是两类参数：搬出接口选择处理方式，`QUANT_PRE`寄存器或Fixpipe Buffer提供该方式使用的系数。`NoQuant`不执行量化，Cast不读取量化系数，Scalar/Vector量化则需要预先准备对应参数。

<!-- npu="A3,910b" id11 -->
Atlas A3系列产品和Atlas A2系列产品的搬出接口使用`quant_pre`选择处理方式。
<!-- end id11 -->

<!-- npu="950" id12 -->
Ascend 950PR&950DT系列产品的搬出接口使用`quant_pre_mode`选择处理方式，系数由QUANT_PRE寄存器或Fixpipe Buffer提供。
<!-- end id12 -->

使用`asc_quant_mode::F322F16`或`asc_quant_mode::F322BF16`将float转换成half/bfloat16_t，无需量化系数，舍入模式采用CAST_RINT。

<!-- npu="950" id13 -->
Ascend 950PR&950DT系列产品中的`QF322F16_PRE`/`VQF322F16_PRE`和`QF322BF16_PRE`/`VQF322BF16_PRE`虽然也输出half/bfloat16_t，但属于带系数量化，需要配置Scalar/Vector参数，不能与上述Cast模式混用。
<!-- end id13 -->

带参数量化包括两种粒度：

- Scalar：整个C矩阵共享一个量化参数。
- Tensor/Vector：C矩阵每一列对应一个8B量化参数，参数shape为[N]。先用[asc_copy_l12fb](../cube_compute_load/asc_copy_l12fb.md)从L1 Buffer搬入量化区，再设置起始地址索引。量化地址索引为量化区字节地址除以128，不是元素下标；激活地址索引为激活区字节地址除以64。两个参数区分别寻址，起始地址与长度都不能超过对应区域。

<!-- npu="950" id1 -->
针对Ascend 950PR&950DT系列产品，Scalar系数使用[asc_set_l0c_copy_prequant(scale, offset, is_signed)](../cube_store_aux_config/asc_set_l0c_copy_prequant.md)配置，Vector地址使用[asc_set_l0c_copy_config](../cube_store_aux_config/asc_set_l0c_copy_config.md)配置。搬出调用的`quant_pre_mode`使用`asc_quant_mode`枚举，参数编码见[Ascend 950PR&950DT系列产品编码表](#quant-pre-3510)。
<!-- end id1 -->

<!-- npu="910b,A3" id2 -->
针对如下产品型号：

<!-- npu="A3" id4 -->
Atlas A3系列产品
<!-- end id4 -->
<!-- npu="910b" id5 -->
Atlas A2系列产品
<!-- end id5 -->

<!-- npu="A3,910b" id14 -->
Scalar系数通过[asc_set_l0c_copy_prequant](../cube_store_aux_config/asc_set_l0c_copy_prequant.md)传入打包后的`uint64_t config`，低32位必须是float的位模式，不能将浮点数数值转换成uint64_t；Vector地址使用[asc_set_l0c_copy_config](../cube_store_aux_config/asc_set_l0c_copy_config.md)配置。搬出调用的`quant_pre`使用整数，可显式转换`asc_quant_mode`枚举得到模式编码，参数编码见[Atlas A3系列产品和Atlas A2系列产品编码表](#quant-pre-2201)。
<!-- end id14 -->

<!-- end id2 -->

配置系数只写寄存器，不会自动选择搬出模式。下文模式名称均为`asc_quant_mode`的成员，调用时须按所选重载的参数类型传入。

Scalar和Vector模式使用相同架构下的量化参数编码，区别在于一个参数由整个矩阵共享，还是每列读取一个参数。参数中的缩放系数、偏移量和符号位分别按下文对应架构的编码表解释；`V`前缀表示Vector粒度，不表示参数数组的数据类型是float。每列应准备一个打包后的`uint64_t`参数。

**配置与调用顺序：**

1. 在当前架构的搬出接口中确认源/目的类型、量化模式和目标格式可组合使用。
2. Scalar模式：通过[asc_set_l0c_copy_prequant](../cube_store_aux_config/asc_set_l0c_copy_prequant.md)设置系数。

    <!-- npu="A3,910b" id15 -->
    Atlas A3系列产品和Atlas A2系列产品使用`uint64_t config`重载。
    <!-- end id15 -->

    <!-- npu="950" id16 -->
    Ascend 950PR&950DT系列产品可使用`scale, offset, is_signed`重载或原始编码重载。
    <!-- end id16 -->

3. Vector模式：将每列参数准备在L1 Buffer，经[asc_copy_l12fb](../cube_compute_load/asc_copy_l12fb.md)搬入Fixpipe Buffer的量化参数区，再用[asc_set_l0c_copy_config](../cube_store_aux_config/asc_set_l0c_copy_config.md)设置`quant_pre_addr`。若原始参数位于GM，先通过当前架构的[asc_copy_gm2l1](../cube_compute_load/asc_copy_gm2l1.md)搬入L1。
4. 根据输入数据及参数搬运的依赖关系完成[核内同步](../../sync/intra_core_sync/intra_core_sync.md)后，在L0C搬出接口中选择对应量化模式；若还需激活，结合[随路ReLU](accompanying_relu.md)配置。

模式、系数与结果布局分别控制数值处理和存储方式。输出int8_t/uint8_t且保持Nz布局时，还需考虑[Int8 Channel Merge](Int8-Channel-Merge.md)；转换为ND/DN时，按[NZ2ND](NZ2ND.md)或[NZ2DN](NZ2DN.md)说明设置布局参数。

当前支持的量化模式及其功能描述如下：

```text
DEQF16,                // DeQuant_Float16：int32_t反量化成half，scalar量化
VDEQF16,               // Vector_DeQuant_Float16：int32_t反量化成half，tensor量化
QF322B8_PRE,           // Quant_Float32_2_B8：float量化成int8_t/uint8_t，scalar量化
VQF322B8_PRE,          // Vector_Quant_Float32_2_B8：float量化成int8_t/uint8_t，tensor量化
REQ8,                  // ReQuant_int8：int32_t重量化成int8_t/uint8_t，scalar量化
VREQ8,                 // Vector_ReQuant_int8：int32_t重量化成int8_t/uint8_t，tensor量化
```

<!-- npu="950" id3 -->
除上述量化模式外，Ascend 950PR&950DT系列产品还额外支持以下量化模式：

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
<!-- end id3 -->

不同量化模式的量化算法详细介绍参见[随路量化与随路ReLU场景组合](accompanying_quantization_and_relu_scenario_combination.md)。

量化参数比特位含义见下表：

<!-- npu="A3,910b" id8 -->
<a id="quant-pre-2201"></a>

**表1** 量化参数QUANT\_PRE比特位含义映射表（[NPU架构版本2201](../../../../../guide/programming_guide/language_extension/simd_builtin_keywords.md)）

| 模式 | 比特位数 | 变量名 | 作用介绍 |
| ------ | ---------- | -------- | ---------- |
| &bull;(V)REQ8<br>&bull;(V)QF322B8_PRE<br>&bull;(V)DEQF16 | 0~31 | M1 | 32位数视为float，硬件以(1, 8, 10)的格式，即1个符号位、8个指数位和10个尾数位作为量化计算所需要乘的值。其中31位为符号位，23~30位为指数位，13~22位为尾数位。 |
| &bull;(V)REQ8<br>&bull;(V)QF322B8_PRE<br>&bull;(V)DEQF16 | 32~35 | N | 4位比特位，表示范围为\[1, 16\]（b'0000对应表示1，b'1111对应表示16）。<br>当模式为(V)REQ8时，MCB标志位置为1时，将输入的值进行右移N比特位。当模式为(V)QF322B8_PRE，N为无效变量。 |
| &bull;(V)REQ8<br>&bull;(V)QF322B8_PRE<br>&bull;(V)DEQF16 | 36 | MCB标志位 | Mode Control Bit。如果置为0，输入的int32_t会被直接转换为float，N为无效值。如果置为1，输入的int32_t会先右移N比特位，转变成int16_t，然后转换为float。当模式为(V)QF322B8_PRE时，此标志位为无效比特。 |
| &bull;(V)REQ8<br>&bull;(V)QF322B8_PRE<br>&bull;(V)DEQF16 | 37~45 | Offset | 9bit的整型数据，源数据乘以量化系数或者随路系数的计算结果，可以与Offset表示的整数值进行相加。若不使用offset，请置为0。<br>当模式为(V)DEQF16时，此变量为无效变量。 |
| &bull;(V)REQ8<br>&bull;(V)QF322B8_PRE<br>&bull;(V)DEQF16 | 46 | Sign标志位 | 如果置为1，表明量化结果是signed(int8)；如果置为0，表明量化结果是unsigned(uint8)。仅在(V)REQ8、(V)QF322B8_PRE中会用到。 |
| &bull;(V)REQ8<br>&bull;(V)QF322B8_PRE<br>&bull;(V)DEQF16 | 47~63 | - | 无效比特位，用户无需关注。 |
<!-- end id8 -->

<!-- npu="950" id9 -->
<a id="quant-pre-3510"></a>

**表2** 量化参数QUANT\_PRE比特位含义映射表（[NPU架构版本3510](../../../../../guide/programming_guide/language_extension/simd_builtin_keywords.md)）

| 模式 | 比特位数 | 变量名 | 作用介绍 |
| --- | --- | --- | --- |
| &bull;(V)REQ8<br>&bull;(V)QF322B8_PRE<br>&bull;(V)DEQF16<br>&bull;(V)QF322FP8_PRE<br>&bull;(V)QF322HIF8_PRE<br>&bull;(V)QF322HIF8_PRE_HYBRID<br>&bull;(V)QS322BF16_PRE<br>&bull;(V)QF322F16_PRE<br>&bull;(V)QF322BF16_PRE<br>&bull;(V)QF322F32_PRE<br>&bull;(V)REQ4<br>&bull;(V)QF322S4_PRE | 0~12 | - | 无效比特位，用户无需关注。 |
| 同第一行 | 13~31 | M1 | 数据类型视为float，硬件以(1, 8, 10)的格式，即1个符号位、8个指数位和10个尾数位作为量化计算所需要乘的值。其中31位为符号位，23~30位为指数位，13~22位为尾数位。其值不能为inf/nan。 |
| 同第一行 | 32~36 | - | 无效比特位，用户无需关注。 |
| 同第一行 | 37~45 | Offset | 9bit整形数据，源数据乘以量化系数或者随路系数的计算结果，可以与Offset表示的整数值进行相加。若不使用offset，请置为0。当模式为(V)REQ8、(V)QF322B8_PRE、(V)REQ4、(V)QF322S4_PRE时该变量生效。 |
| 同第一行 | 46 | Sign标志位 | 如果置为1，表明量化结果是signed(int8)；如果为置为0，表明量化结果是unsigned(uint8)。仅在(V)REQ8、(V)QF322B8_PRE中会用到。 |
| 同第一行 | 47~63 | - | 无效比特位，用户无需关注。 |
<!-- end id9 -->

**使用约束与参数精度：**

- 量化参数不能为inf、nan或非规格化数；配置前需检查缩放系数，具体限制见对应配置接口。
- 缩放系数按表中有效位参与计算，不保留完整FP32尾数精度。输出仍为float的量化模式也会受到该有效精度的影响。
- Offset、Sign只在表中列出的模式下生效。

  <!-- npu="A3,910b" id17 -->
  Atlas A3系列产品和Atlas A2系列产品的N/MCB只在表中列出的模式下生效。
  <!-- end id17 -->

  <!-- npu="950" id18 -->
  Ascend 950PR&950DT系列产品中32~36位无效，不能沿用Atlas A3系列产品和Atlas A2系列产品的右移配置。
  <!-- end id18 -->

- Vector量化的参数按原始C矩阵的N维列索引对应。分块搬出时，应同步检查参数区偏移、列数、对齐及占用范围。Fixpipe Buffer的量化区和激活区独立寻址，不能把元素下标直接传给地址索引参数。
- 不同搬出任务需要不同系数时，应在对应搬出执行前重新配置。`asc_set_l0c_copy_config`每次同时覆盖激活地址、量化地址和UnitFlag初始化参数，三者的含义见该接口的参数说明。

**使用示例：**

[asc_copy_l0c2gm样例](../../../../../../../examples/02_simd_c_api/03_c_api/03_matrix_compute/asc_copy_l0c2gm/README.md)的场景4、5分别展示Scalar和Vector量化。运行时按样例声明的产品支持范围选择环境。算法示意及量化与ReLU的配合见[随路量化与随路ReLU场景组合](accompanying_quantization_and_relu_scenario_combination.md)。
