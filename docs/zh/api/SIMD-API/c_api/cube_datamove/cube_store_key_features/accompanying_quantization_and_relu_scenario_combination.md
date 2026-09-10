# 随路量化与随路ReLU场景组合<a name="ZH-CN_TOPIC_0000002538071282"></a>

矩阵搬出支持多种随路量化与随路ReLU的组合，常用的不激活/Normal ReLU组合参考下表，Scalar/Vector扩展组合见后文。

<!-- npu="950,A3,910b" id1 -->
随路量化与随路ReLU的组合情况如下表所示：

<!-- npu="A3,910b" id2 -->
**表1** 随路量化与随路ReLU的组合表（[NPU架构版本2201](../../../../../guide/programming_guide/language_extension/simd_builtin_keywords.md)）

| quant_pre / relu_pre | 0（不激活） | 1（Normal ReLU） |
| ------------------ | --------- | ------------ |
| &bull;REQ8<br>&bull;DEQF16<br>&bull;QF322B8_PRE | M1=QUANT_PRE\[31:0\]<br>M2=QUANT_PRE\[31:0\]<br>M1、M2均为量化参数 | M1=QUANT_PRE\[31:0\]<br>M2=0<br>M1为量化参数、M2为NORMAL系数 |
| &bull;VREQ8<br>&bull;VDEQF16<br>&bull;VQF322B8_PRE | M1=Quant_PRE_ADDR\[i\]\[31:0\]<br>M2=Quant_PRE_ADDR\[i\]\[31:0\]<br>M1、M2均为量化参数，i为原始矩阵的列索引 | M1=Quant_PRE_ADDR\[i\]\[31:0\]<br>M2=0<br>M1为量化参数，M2为NORMAL系数，i为原始矩阵的列索引 |

<!-- end id2 -->

<!-- npu="950" id3 -->
**表2** 随路量化与随路ReLU的组合表（[NPU架构版本3510](../../../../../guide/programming_guide/language_extension/simd_builtin_keywords.md)）

| quant_pre_mode / relu_pre_mode | NONE | NORMAL |
| --- | --- | --- |
| &bull;REQ8<br>&bull;DEQF16<br>&bull;QF322B8_PRE<br>&bull;QF322FP8_PRE<br>&bull;QF322HIF8_PRE<br>&bull;QF322HIF8_PRE_HYBRID<br>&bull;QS322BF16_PRE<br>&bull;QF322F16_PRE<br>&bull;QF322BF16_PRE<br>&bull;QF322F32_PRE | M1=QUANT_PRE\[31:13\]<br>M2=QUANT_PRE\[31:13\]<br>M1、M2均为量化参数 | M1=QUANT_PRE\[31:13\]<br>M2=0<br>M1为量化参数、M2为NORMAL系数 |
| &bull;VREQ8<br>&bull;VDEQF16<br>&bull;VQF322B8_PRE<br>&bull;VQF322FP8_PRE<br>&bull;VQF322HIF8_PRE<br>&bull;VQF322HIF8_PRE_HYBRID<br>&bull;VQS322BF16_PRE<br>&bull;VQF322F16_PRE<br>&bull;VQF322BF16_PRE<br>&bull;VQF322F32_PRE | M1=Quant_PRE_ADDR\[i\]\[31:13\]<br>M2=Quant_PRE_ADDR\[i\]\[31:13\]<br>M1、M2均为量化参数，i为原始矩阵的列索引 | M1=Quant_PRE_ADDR\[i\]\[31:13\]<br>M2=0<br>M1为量化参数，M2为NORMAL系数，i为原始矩阵的列索引 |

<!-- end id3 -->

注：M1为原始数据非负时使用的随路系数，M2为原始数据为负数时使用的随路系数。
<!-- end id1 -->

## C API扩展组合

<!-- npu="950" id4 -->

3510的`quant_pre_mode`选择正半轴量化系数来源，`relu_pre_mode`独立选择负半轴系数来源。定义Q为Scalar量化参数中的M1，Q[i]为量化区第i列的M1，R为Scalar激活系数，R[i]为激活区第i列系数：

| 量化系数来源 | NONE的M2 | NORMAL的M2 | SCALAR的M2 | VECTOR的M2 |
| --- | --- | --- | --- | --- |
| Scalar（M1=Q） | Q | 0 | R | R[i] |
| Vector（M1=Q[i]） | Q[i] | 0 | R | R[i] |

SCALAR/VECTOR是负半轴的独立系数选择，不会自动将激活系数与量化系数相乘。若想要标准Leaky ReLU在量化后具有比例α，应按实际算法准备匹配的负半轴系数。无量化和Cast场景不消费QUANT_PRE作为M1，不能把上表解释为这些模式也会自动乘Q。

3510的`REQ4/VREQ4`、`QF322S4_PRE/VQF322S4_PRE`也按相应Scalar/Vector方式准备量化参数，输出饱和范围与打包粒度为4位。基本运算步骤为：按源符号选择有效系数，执行模式要求的输入转换与缩放，再按相应模式执行偏移、舍入及目标类型转换。B8的Offset/Sign需按[量化参数位域](accompanying_quantization.md)处理，不能用一个未经舍入的`x*scale`式子代替全部模式。

<!-- end id4 -->

<!-- npu="910b,A3" id5 -->

2201的搬出整数参数`relu_pre=2/3`也分别选择Scalar/Vector负半轴系数；其Scalar配置使用`asc_set_l0c2gm_lrelu_alpha`，Vector地址使用`asc_set_l0c2gm_config`。带MCB右移的量化先按2201位域说明转换输入。

<!-- end id5 -->

<!-- npu="950" id6 -->

Clip使用独立开关`enable_clip_relu_pre`，与Normal ReLU及量化模式组合；不应把Clip当作`asc_relu_pre_mode`的新枚举值。配置与限制见[随路ReLU](accompanying_relu.md)。

<!-- end id6 -->
