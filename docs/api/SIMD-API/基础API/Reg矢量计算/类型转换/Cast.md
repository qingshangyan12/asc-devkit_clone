# Cast<a name="ZH-CN_TOPIC_0000001956827113"></a>

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

 |
| <cann-filter npu-type="A3"> Atlas A3 训练系列产品/Atlas A3 推理系列产品 | x </cann-filter> |
| <cann-filter npu-type="910b"> Atlas A2 训练系列产品/Atlas A2 推理系列产品 | x </cann-filter> |
| <cann-filter npu-type="310b"> Atlas 200I/500 A2 推理产品 | x </cann-filter> |
| <cann-filter npu-type="310p"> Atlas 推理系列产品AI Core | x </cann-filter> |
| <cann-filter npu-type="310p"> Atlas 推理系列产品Vector Core | x </cann-filter> |
| <cann-filter npu-type="910"> Atlas 训练系列产品 | x </cann-filter> |

## 功能说明<a name="section618mcpsimp"></a>

头文件路径：`"basic_api/reg_compute/kernel_reg_compute_vec_vconv_intf.h"`。

Cast用于数据类型精度转换，将源操作数数据类型转换成目的操作数数据类型，能够实现[浮点转整数](#表6-浮点转整数)、[浮点转浮点](#表7-浮点转浮点)、[整数转浮点](#表8-整数转浮点)、[整数转整数](#表9-整数转整数)的数据类型转换。


- [RegLayout](../数据类型/RegLayout.md)：源操作数和目的操作数位宽不同时，单条指令计算量以位宽更大的数据类型为准，RegLayout用于控制位宽小的元素在寄存器中的排布方式。
- [SatMode](../数据类型/SatMode.md)：用于设置饱和与不饱和模式。
- [MaskMergeMode](../数据类型/MaskMergeMode.md)：用于指定写入寄存器数据模式，mask未选择的元素在dst中置零或保留dst原值。
- [RoundMode](../数据类型/RoundMode.md)：用于设置舍入模式。

## 函数原型<a name="section620mcpsimp"></a>

```cpp
template <typename T = DefaultType, typename U = DefaultType, const CastTrait& trait = castTrait, typename S, typename V>
__simd_callee__ inline void Cast(S& dstReg, V& srcReg, MaskReg& mask);
```

## 参数说明<a name="section622mcpsimp"></a>

**表 1**  模板参数说明

| 参数名 | 描述 |
| --- | --- |
| T | 目的操作数的数据类型。支持的数据类型请参考[数据类型](#数据类型)。 |
| U | 源操作数的数据类型。支持的数据类型请参考[数据类型](#数据类型)。 |
| trait | CastTrait类型，类型转换模式结构体。包括[RegLayout](../数据类型/RegLayout.md)、[SatMode](../数据类型/SatMode.md)、[MaskMergeMode](../数据类型/MaskMergeMode.md)、[RoundMode](../数据类型/RoundMode.md)。<br><pre><code>struct CastTrait {<br>    RegLayout layoutMode = RegLayout::UNKNOWN;<br>    SatMode satMode = SatMode::UNKNOWN;<br>    MaskMergeMode mrgMode = MaskMergeMode::UNKNOWN;<br>    RoundMode roundMode = RoundMode::UNKNOWN;<br>};</code></pre>使能SatMode生效需与SetCtrlSpr配合使用。不饱和模式和饱和模式的具体配置请参考[表5](#表5)。<br>注：SetCtrlSpr需在SIMD_VF外调用。 |
| S | 目的操作数的[RegTensor](../寄存器数据类型/RegTensor.md)类型，例如RegTensor&lt;float&gt;，由编译器自动推导，用户不需要填写。 |
| V | 源操作数的[RegTensor](../寄存器数据类型/RegTensor.md)类型，例如RegTensor&lt;int32_t&gt;，由编译器自动推导，用户不需要填写。 |

**表 2**  参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| dstReg | 输出 | 目的操作数。<br>类型为[RegTensor](../寄存器数据类型/RegTensor.md)。 |
| srcReg | 输入 | 源操作数。<br>类型为[RegTensor](../寄存器数据类型/RegTensor.md)。 |
| mask | 输入 | 源操作数元素操作的有效指示，详细说明请参考[MaskReg](../寄存器数据类型/MaskReg.md)。<br>注：mask会按照输入来筛选，请参考MaskReg和RegLayout作用下的Cast类型转换过程。 |

## 数据类型

**表 3**  数据类型组合情况

| src | dst |
| --- | --- |
| int4x2_t | int16_t、half、bfloat16_t |
| int8_t | int16_t、half、int32_t |
| uint8_t | uint16_t、half、uint32_t |
| fp4x2_e2m1_t | bfloat16_t |
| fp4x2_e1m2_t | bfloat16_t |
| hifloat8_t | half、float |
| fp8_e8m0_t | bfloat16_t |
| fp8_e5m2_t | float |
| fp8_e4m3fn_t | float |
| int16_t | int4x2_t、uint8_t、half、int32_t、uint32_t、float |
| uint16_t | uint8_t、uint32_t |
| half | int4x2_t、int8_t、uint8_t、hifloat8_t、int16_t、bfloat16_t、int32_t、float |
| bfloat16_t | fp4x2_e2m1_t、fp4x2_e1m2_t、fp8_e8m0_t、half、int32_t、float |
| int32_t | uint8_t、int16_t、float、int64_t |
| uint32_t | uint8_t、int16_t、uint16_t |
| float | hifloat8_t、fp8_e5m2_t、fp8_e4m3fn_t、int16_t、half、bfloat16_t、int32_t、int64_t |
| int64_t | int32_t、float |

## 返回值说明<a name="section640mcpsimp"></a>

无

## 约束说明<a name="section633mcpsimp"></a>

-   该指令需配合SetCtrlSpr指令使用，通过设置寄存器的值控制Cast的不饱和模式和饱和模式。
-   舍入模式仅在可能导致精度损失且支持该舍入模式的转换中生效，具体支持情况请参考上方表格。

-   不同类型转换的不饱和与饱和模式的描述有所区别，全局或单指令生效配置方法也不同，具体内容参考[表4 不同类型转换场景的不饱和模式和饱和模式](#表4-不同类型转换场景的不饱和模式和饱和模式)、[表5 饱和模式全局或单指令生效配置表](#表5-饱和模式全局或单指令生效配置表)。
-   浮点数转浮点数：
    -   当输出类型float32时，只支持不饱和模式。
    -   不饱和模式：当输出类型为fp8_e4m3fn_t时，由于fp8_e4m3fn_t没有inf表示格式，所以输出为nan。
    -   饱和模式：当输出类型为fp8_e5m2_t/fp8_e4m3fn_t时，输入nan，默认输出为0。如果CTRL\[50\] = 1'b1，则输出为nan。
    -   fp4x2_e2m1_t/fp4x2_e1m2_t数据类型没有inf和nan的定义。对于bfloat16到float4的数据类型转换，输入bfloat16类型的值为inf或超出fp4x2_e2m1_t/fp4x2_e1m2_t数据最值范围时，会返回对应符号的fp4x2_e2m1_t/fp4x2_e1m2_t最值；输入nan时，fp4x2_e2m1_t/fp4x2_e1m2_t输出0。
    -   对于fp8_e8m0_t类型：

        输入bfloat16_t +/-inf或绝对值超出fp8_e8m0_t类型最值，则返回fp8_e8m0_t最大值0b11111110；

        输入bfloat16_tnan输出fp8_e8m0_t nan = 0b11111111。

-   整数转整数

    对于窄数据类型例如int16_t\(2Byte\)转宽数据类型uint32_t\(4Byte\)，只支持饱和模式，输入负数会被饱和成0。

-   特别的，int4x2_t/fp4x2_e2m1_t/fp4x2_e1m2_t和b16之间的转换，指令会以每2个元素为一对进行读写，大转小时mask有效位以偶数位为准。

    图1和图2展示了MaskReg和RegLayout同时作用时fp4x2_e2m1_t和bfloat16_t之间的转换。

    **图 1**  fp4x2_e2m1_t到bfloat16_t转换过程  
    
    ![](../../../../figures/reg_cast_1.png)

    **图 2**  bfloat16_t到fp4x2_e2m1_t转换过程  
    ![](../../../../figures/reg_cast_2.png)

## 关键特性说明

**1、不同类型转换场景下的不饱和模式和饱和模式描述如下：**

<a id="表4-不同类型转换场景的不饱和模式和饱和模式"></a>
**表 4**  不同类型转换场景的不饱和模式和饱和模式

| 场景 | 不饱和模式 | 饱和模式 |
| --- | --- | --- |
| 浮点转整数 | 输入数据超过输出类型最值时，结果被截断为目标格式的数据宽度（保留最低有效位），例如输入half值为257，输出uint8_t值为1；输入为+/-inf时，则返回输出类型的对应最值；输入为nan时，返回0。 | 输入数据超过输出类型最值时，返回输出类型的对应最值，例如输入half值为257，输出uint8值为255，输入half值为-inf，输出uint8_t值为0；输入为nan时，返回0。 |
| 浮点转浮点 | 输入数据为nan时，输出为nan；输入+/-inf时，输出为+/-inf。 | 输入为nan时，输出为0；输入数据超过输出类型最值时，返回输出类型的对应最值。 |
| 整数转浮点 | 不支持不饱和模式 | 输入为nan时，输出为0；输入数据超过输出类型最值时，返回输出类型的对应最值。该场景默认饱和模式，无需配置。 |
| 整数转整数 | 输入数据会截断为目标数据宽度，例如，输入int32_t值为256，输出uint8_t值为0。 | 输入数据超出目标数据范围，会饱和为目标数据最值。 |

**2、不饱和模式和饱和模式全局或单指令生效配置表如下：**

<a id="表5-饱和模式全局或单指令生效配置表"></a>
**表 5**  饱和模式全局或单指令生效配置表

| 全局使能位 | 控制位 | 功能描述 |
| --- | --- | --- |
| CTRL[60] = 1'b0 | Reg矢量计算Cast API的trait模板参数中satMode设置为SatMode::NO_SAT。 | 单指令非饱和模式。 |
| CTRL[60] = 1'b0 | Reg矢量计算Cast API的trait模板参数中satMode设置为SatMode::SAT。 | 单指令饱和模式。 |
| CTRL[60] = 1'b1 | CTRL[48] = 1'b1 | 全局非饱和模式（浮点数计算和浮点数精度转换）。 |
| CTRL[60] = 1'b1 | CTRL[48] = 1'b0 | 全局饱和模式（浮点数计算和浮点数精度转换）。 |
| CTRL[60] = 1'b1 | CTRL[59] = 1'b1 | 全局非饱和模式（浮点数转整数或整数转整数时的精度转换）。 |
| CTRL[60] = 1'b1 | CTRL[59] = 1'b0 | 全局饱和模式（浮点数转整数或整数转整数时的精度转换）。 |

3、不同类型转换支持的CastTrait类型取值不同，具体支持情况请参考表6~表9：

<a id="表6-浮点转整数"></a>
**表 6**  浮点转整数

| src dtype | dst dtype | layoutMode | satMode | mrgMode | roundMode |
| --- | --- | --- | --- | --- | --- |
| half | int4x2_t | RegLayout::ZERO, RegLayout::ONE, RegLayout::TWO, RegLayout::THREE | SatMode::NO_SAT, SatMode::SAT | MaskMergeMode::ZEROING | RoundMode::CAST_RINT, RoundMode::CAST_ROUND, RoundMode::CAST_FLOOR, RoundMode::CAST_CEIL, RoundMode::CAST_TRUNC |
| half | int8_t | RegLayout::ZERO, RegLayout::ONE | SatMode::NO_SAT, SatMode::SAT | MaskMergeMode::ZEROING | RoundMode::CAST_RINT, RoundMode::CAST_ROUND, RoundMode::CAST_FLOOR, RoundMode::CAST_CEIL, RoundMode::CAST_TRUNC |
| half | uint8_t | RegLayout::ZERO, RegLayout::ONE | SatMode::NO_SAT, SatMode::SAT | MaskMergeMode::ZEROING | RoundMode::CAST_RINT, RoundMode::CAST_ROUND, RoundMode::CAST_FLOOR, RoundMode::CAST_CEIL, RoundMode::CAST_TRUNC |
| half | int16_t | RegLayout::UNKNOWN | SatMode::NO_SAT, SatMode::SAT | MaskMergeMode::ZEROING | RoundMode::CAST_RINT, RoundMode::CAST_ROUND, RoundMode::CAST_FLOOR, RoundMode::CAST_CEIL, RoundMode::CAST_TRUNC |
| half | int32_t | RegLayout::ZERO, RegLayout::ONE | SatMode::UNKNOWN | MaskMergeMode::ZEROING | RoundMode::CAST_RINT, RoundMode::CAST_ROUND, RoundMode::CAST_FLOOR, RoundMode::CAST_CEIL, RoundMode::CAST_TRUNC |
| bfloat16_t | int32_t | RegLayout::ZERO, RegLayout::ONE | SatMode::NO_SAT, SatMode::SAT | MaskMergeMode::ZEROING | RoundMode::CAST_RINT, RoundMode::CAST_ROUND, RoundMode::CAST_FLOOR, RoundMode::CAST_CEIL, RoundMode::CAST_TRUNC |
| float | int16_t | RegLayout::ZERO, RegLayout::ONE | SatMode::NO_SAT, SatMode::SAT | MaskMergeMode::ZEROING | RoundMode::CAST_RINT, RoundMode::CAST_ROUND, RoundMode::CAST_FLOOR, RoundMode::CAST_CEIL, RoundMode::CAST_TRUNC |
| float | int32_t | RegLayout::UNKNOWN | SatMode::NO_SAT, SatMode::SAT | MaskMergeMode::ZEROING | RoundMode::CAST_RINT, RoundMode::CAST_ROUND, RoundMode::CAST_FLOOR, RoundMode::CAST_CEIL, RoundMode::CAST_TRUNC |
| float | int64_t | RegLayout::ZERO, RegLayout::ONE | SatMode::NO_SAT, SatMode::SAT | MaskMergeMode::ZEROING | RoundMode::CAST_RINT, RoundMode::CAST_ROUND, RoundMode::CAST_FLOOR, RoundMode::CAST_CEIL, RoundMode::CAST_TRUNC |

<a id="表7-浮点转浮点"></a>
**表 7**  浮点转浮点

| src dtype | dst dtype | layoutMode | satMode | mrgMode | roundMode |
| --- | --- | --- | --- | --- | --- |
| hifloat8_t | half | RegLayout::ZERO, RegLayout::ONE | SatMode::UNKNOWN | MaskMergeMode::ZEROING | RoundMode::UNKNOWN |
| hifloat8_t | float | RegLayout::ZERO, RegLayout::ONE, RegLayout::TWO, RegLayout::THREE | SatMode::UNKNOWN | MaskMergeMode::ZEROING | RoundMode::UNKNOWN |
| fp8_e4m3fn_t | bfloat16_t | RegLayout::ZERO, RegLayout::ONE | SatMode::UNKNOWN | MaskMergeMode::ZEROING | RoundMode::CAST_RINT, RoundMode::CAST_ROUND, RoundMode::CAST_TRUNC |
| fp8_e5m2_t | bfloat16_t | RegLayout::ZERO, RegLayout::ONE | SatMode::UNKNOWN | MaskMergeMode::ZEROING | RoundMode::CAST_RINT, RoundMode::CAST_ROUND, RoundMode::CAST_TRUNC |
| fp8_e4m3fn_t | float | RegLayout::ZERO, RegLayout::ONE, RegLayout::TWO, RegLayout::THREE | SatMode::UNKNOWN | MaskMergeMode::ZEROING | RoundMode::CAST_RINT, RoundMode::CAST_ROUND, RoundMode::CAST_TRUNC |
| fp8_e5m2_t | float | RegLayout::ZERO, RegLayout::ONE, RegLayout::TWO, RegLayout::THREE | SatMode::UNKNOWN | MaskMergeMode::ZEROING | RoundMode::CAST_RINT, RoundMode::CAST_ROUND, RoundMode::CAST_TRUNC |
| fp8_e8m0_t | bfloat16_t | RegLayout::ZERO, RegLayout::ONE | SatMode::UNKNOWN | MaskMergeMode::ZEROING | RoundMode::UNKNOWN |
| fp4x2_e2m1_t | bfloat16_t | RegLayout::ZERO, RegLayout::ONE, RegLayout::TWO, RegLayout::THREE | SatMode::UNKNOWN | MaskMergeMode::ZEROING | RoundMode::UNKNOWN |
| fp4x2_e1m2_t | bfloat16_t | RegLayout::ZERO, RegLayout::ONE, RegLayout::TWO, RegLayout::THREE | SatMode::UNKNOWN | MaskMergeMode::ZEROING | RoundMode::UNKNOWN |
| half | hifloat8_t | RegLayout::ZERO, RegLayout::ONE | SatMode::NO_SAT, SatMode::SAT | MaskMergeMode::ZEROING | RoundMode::CAST_ROUND,  RoundMode::HYBRID |
| half | bfloat16_t | RegLayout::UNKNOWN | SatMode::UNKNOWN | MaskMergeMode::ZEROING | RoundMode::CAST_RINT, RoundMode::CAST_ROUND, RoundMode::CAST_FLOOR, RoundMode::CAST_CEIL, RoundMode::CAST_TRUNC |
| bfloat16_t | half | RegLayout::UNKNOWN | SatMode::NO_SAT, SatMode::SAT | MaskMergeMode::ZEROING | RoundMode::CAST_RINT, RoundMode::CAST_ROUND, RoundMode::CAST_FLOOR, RoundMode::CAST_CEIL, RoundMode::CAST_TRUNC |
| bfloat16_t | fp4x2_e2m1_t | RegLayout::ZERO, RegLayout::ONE, RegLayout::TWO, RegLayout::THREE | SatMode::UNKNOWN | MaskMergeMode::ZEROING | RoundMode::CAST_RINT, RoundMode::CAST_ROUND, RoundMode::CAST_TRUNC |
| bfloat16_t | fp4x2_e1m2_t | RegLayout::ZERO, RegLayout::ONE, RegLayout::TWO, RegLayout::THREE | SatMode::UNKNOWN | MaskMergeMode::ZEROING | RoundMode::CAST_RINT, RoundMode::CAST_ROUND, RoundMode::CAST_TRUNC |
| bfloat16_t | fp8_e8m0_t | RegLayout::ZERO, RegLayout::ONE | SatMode::UNKNOWN | MaskMergeMode::ZEROING | RoundMode::UNKNOWN |
| float | hifloat8_t | RegLayout::ZERO, RegLayout::ONE, RegLayout::TWO, RegLayout::THREE | SatMode::NO_SAT, SatMode::SAT | MaskMergeMode::ZEROING | RoundMode::CAST_ROUND,  RoundMode::HYBRID |
| float | fp8_e4m3fn_t | RegLayout::ZERO, RegLayout::ONE, RegLayout::TWO, RegLayout::THREE | SatMode::NO_SAT, SatMode::SAT | MaskMergeMode::ZEROING | RoundMode::CAST_RINT |
| float | fp8_e5m2_t | RegLayout::ZERO, RegLayout::ONE, RegLayout::TWO, RegLayout::THREE | SatMode::NO_SAT, SatMode::SAT | MaskMergeMode::ZEROING | RoundMode::CAST_RINT |
| float | half | RegLayout::ZERO, RegLayout::ONE | SatMode::NO_SAT, SatMode::SAT | MaskMergeMode::ZEROING | RoundMode::CAST_ODD, RoundMode::CAST_RINT, RoundMode::CAST_ROUND, RoundMode::CAST_FLOOR, RoundMode::CAST_CEIL, RoundMode::CAST_TRUNC |
| float | bfloat16_t | RegLayout::ZERO, RegLayout::ONE | SatMode::NO_SAT, SatMode::SAT | MaskMergeMode::ZEROING | RoundMode::CAST_RINT, RoundMode::CAST_ROUND, RoundMode::CAST_FLOOR, RoundMode::CAST_CEIL, RoundMode::CAST_TRUNC |

<a id="表8-整数转浮点"></a>
**表 8**  整数转浮点

| src dtype | dst dtype | layoutMode | satMode | mrgMode | roundMode |
| --- | --- | --- | --- | --- | --- |
| int4x2_t | half | RegLayout::ZERO, RegLayout::ONE, RegLayout::TWO, RegLayout::THREE | SatMode::UNKNOWN | MaskMergeMode::ZEROING | RoundMode::UNKNOWN |
| int4x2_t | bfloat16_t | RegLayout::ZERO, RegLayout::ONE, RegLayout::TWO, RegLayout::THREE | SatMode::UNKNOWN | MaskMergeMode::ZEROING | RoundMode::UNKNOWN |
| int8_t | half | RegLayout::ZERO, RegLayout::ONE | SatMode::UNKNOWN | MaskMergeMode::ZEROING | RoundMode::UNKNOWN |
| uint8_t | half | RegLayout::ZERO, RegLayout::ONE | SatMode::UNKNOWN | MaskMergeMode::ZEROING | RoundMode::UNKNOWN |
| int16_t | half | RegLayout::UNKNOWN | SatMode::UNKNOWN | MaskMergeMode::ZEROING | RoundMode::CAST_RINT, RoundMode::CAST_ROUND, RoundMode::CAST_FLOOR, RoundMode::CAST_CEIL, RoundMode::CAST_TRUNC |
| int16_t | float | RegLayout::ZERO, RegLayout::ONE | SatMode::UNKNOWN | MaskMergeMode::ZEROING | RoundMode::UNKNOWN |
| int32_t | float | RegLayout::UNKNOWN | SatMode::UNKNOWN | MaskMergeMode::ZEROING | RoundMode::CAST_RINT, RoundMode::CAST_ROUND, RoundMode::CAST_FLOOR, RoundMode::CAST_CEIL, RoundMode::CAST_TRUNC |
| int64_t | float | RegLayout::ZERO, RegLayout::ONE | SatMode::UNKNOWN | MaskMergeMode::ZEROING | RoundMode::CAST_RINT, RoundMode::CAST_ROUND, RoundMode::CAST_FLOOR, RoundMode::CAST_CEIL, RoundMode::CAST_TRUNC |

<a id="表9-整数转整数"></a>
**表 9**  整数转整数

| src dtype | dst dtype | layoutMode | satMode | mrgMode | roundMode |
| --- | --- | --- | --- | --- | --- |
| int4x2_t | int16_t | RegLayout::ZERO, RegLayout::ONE, RegLayout::TWO, RegLayout::THREE | SatMode::UNKNOWN | MaskMergeMode::ZEROING | RoundMode::UNKNOWN |
| int8_t | int16_t | RegLayout::ZERO, RegLayout::ONE | SatMode::UNKNOWN | MaskMergeMode::ZEROING | RoundMode::UNKNOWN |
| int8_t | int32_t | RegLayout::ZERO, RegLayout::ONE, RegLayout::TWO, RegLayout::THREE | SatMode::NO_SAT, SatMode::SAT | MaskMergeMode::ZEROING | RoundMode::UNKNOWN |
| uint8_t | uint16_t | RegLayout::ZERO, RegLayout::ONE | SatMode::UNKNOWN | MaskMergeMode::ZEROING | RoundMode::UNKNOWN |
| uint8_t | uint32_t | RegLayout::ZERO, RegLayout::ONE, RegLayout::TWO, RegLayout::THREE | SatMode::UNKNOWN | MaskMergeMode::ZEROING | RoundMode::UNKNOWN |
| int16_t | int4x2_t | RegLayout::ZERO, RegLayout::ONE, RegLayout::TWO, RegLayout::THREE | SatMode::NO_SAT, SatMode::SAT | MaskMergeMode::ZEROING | RoundMode::UNKNOWN |
| int16_t | uint8_t | RegLayout::ZERO, RegLayout::ONE | SatMode::NO_SAT, SatMode::SAT | MaskMergeMode::ZEROING | RoundMode::UNKNOWN |
| int16_t | int32_t | RegLayout::ZERO, RegLayout::ONE | SatMode::UNKNOWN | MaskMergeMode::ZEROING | RoundMode::UNKNOWN |
| int16_t | uint32_t | RegLayout::ZERO, RegLayout::ONE | SatMode::UNKNOWN | MaskMergeMode::ZEROING | RoundMode::UNKNOWN |
| uint16_t | uint8_t | RegLayout::ZERO, RegLayout::ONE | SatMode::NO_SAT, SatMode::SAT | MaskMergeMode::ZEROING | RoundMode::UNKNOWN |
| uint16_t | uint32_t | RegLayout::ZERO, RegLayout::ONE | SatMode::UNKNOWN | MaskMergeMode::ZEROING | RoundMode::UNKNOWN |
| int32_t | uint8_t | RegLayout::ZERO, RegLayout::ONE, RegLayout::TWO, RegLayout::THREE | SatMode::NO_SAT, SatMode::SAT | MaskMergeMode::ZEROING | RoundMode::UNKNOWN |
| int32_t | int16_t | RegLayout::ZERO, RegLayout::ONE | SatMode::NO_SAT, SatMode::SAT | MaskMergeMode::ZEROING | RoundMode::UNKNOWN |
| int32_t | uint16_t | RegLayout::ZERO, RegLayout::ONE | SatMode::NO_SAT, SatMode::SAT | MaskMergeMode::ZEROING | RoundMode::UNKNOWN |
| uint32_t | uint8_t | RegLayout::ZERO, RegLayout::ONE, RegLayout::TWO, RegLayout::THREE | SatMode::NO_SAT, SatMode::SAT | MaskMergeMode::ZEROING | RoundMode::UNKNOWN |
| uint32_t | int16_t | RegLayout::ZERO, RegLayout::ONE | SatMode::NO_SAT, SatMode::SAT | MaskMergeMode::ZEROING | RoundMode::UNKNOWN |
| uint32_t | uint16_t | RegLayout::ZERO, RegLayout::ONE | SatMode::NO_SAT, SatMode::SAT | MaskMergeMode::ZEROING | RoundMode::UNKNOWN |
| uint32_t | int32_t | RegLayout::UNKNOWN | SatMode::NO_SAT, SatMode::SAT | MaskMergeMode::ZEROING | RoundMode::UNKNOWN |
| int64_t | int32_t | RegLayout::ZERO, RegLayout::ONE | SatMode::NO_SAT, SatMode::SAT | MaskMergeMode::ZEROING | RoundMode::UNKNOWN |

**表 10**  浮点转整数转换规则

| 转换模式 | 转换规则介绍 |
| --- | --- |
| CAST_RINT | 就近舍入，距离相同时向偶数进位。<br>举例：<br>输入3.3，输出3<br>输入5.9，输出6<br>输入5.5，输出6，因为6是偶数，5是奇数，所以向6舍入<br>输入4.5，输出4，4是偶数<br>输入-2.4，输出2<br>输入-3.6，输出-4 |
| CAST_ROUND | 就近舍入，距离相同时向远离0的数值进位。<br>举例：<br>输入3.3，输出3<br>输入5.9，输出6<br>输入5.5，输出6，因为6相比5，距离0更远，所以向6舍入<br>输入-2.4，输出2<br>输入-3.6，输出-4<br>输入-6.5，输出-7， 因为-7相比-6，距离0更远 |
| CAST_FLOOR | 向负无穷方向舍入。<br>举例：<br>输入3.2，输出3<br>输入7.9，输出7<br>输入-4.6，输出-5<br>输入-3.1，输出-4<br>相比输入，输出更接近负无穷 |
| CAST_CEIL | 向正无穷方向舍入。<br>举例：<br>输入3.2，输出4<br>输入7.9，输出8<br>输入-4.6，输出-4<br>输入-3.1，输出-3<br>相比输入，输出更接近正无穷 |
| CAST_TRUNC | 向0方向舍入。<br>举例：<br>输入3.2，输出3<br>输入7.9，输出7<br>输入-4.6，输出-4<br>输入-3.1，输出-3<br>相比输入，输出更接近0 |

**表 11**  浮点转浮点转换规则

| src dtype | dst dtype | 舍入规则 |
| --- | --- | --- |
| float | half | CAST_ODD，向奇数方向舍入。<br>half有尾数位10bit，float有尾数位23bit，float转half舍弃精度13bit，float16最低位标记为y，float舍弃精度y后面的数<br>float: +/-a.xxxxxxxxx00000100000000，如果y的bit位为0偶数，则进位，得到half: a.xxxxxxxxx1<br>float: +/-a.xxxxxxxxx11000100000000，如果y的bit位为1奇数，则不进位，得到half: a.xxxxxxxxx1<br>举例<br>输入float: 123.23333，二进制为<br>sign: 0, exponent: 10000101, mantissa: 11101100111011101110111<br>float_exponet = 0b10000101 = 133<br>half_exponet - 15 = 133 - 127<br>half_exponet  = 21 = 0b10101<br>float_mantissa第14位为1奇数，所以不进位，舍弃后面13位<br>11101100111011101110111<br>得到half_mantissa = 1110110011<br>所以half二进制为<br>sign: 0, exponent: 10101, mantissa: 1110110011<br>half = 123.2 |
| float | bfloat16_t | float二进制规则为<br>sign: 1bit, exponent: 8bit mantissa: 23bit<br>bfloat16_t二进制规则为<br>sign: 1bit, exponent: 8bit, mantissa: 7bit<br>CAST_RINT：就近舍入，距离相等时向偶数进位<br>举例<br>1、float32_mantissa: 00100000010000010000000<br>舍弃精度小于0b1000000000000000，等效于[0, 0.5)，则不进位<br>得到bfloat16_mantissa: 0010000<br>2、float32_mantissa: 00110001000000000000000<br>舍弃精度等于0b1000000000000000，bf16最低位为偶数时不进位<br>得到bfloat16_mantissa: 0011000<br>3、float32_mantissa: 00110011000000000000000<br>舍弃精度等于0b1000000000000000，bf16最低位为奇数时进位<br>得到bfloat16_mantissa: 0011010<br>4、float32_mantissa: 00100001010000010000000<br>舍弃精度大于0b1000000000000000，等效于(0.5, 1)，则进位<br>得到bfloat16_mantissa: 0010001<br>CAST_ROUND：就近舍入，距离相等时向远离0方向进位<br>举例<br>1、float32_mantissa: 00100000010000010000000<br>舍弃精度小于0b1000000000000000，等效于[0, 0.5)，则不进位<br>得到bfloat16_mantissa: 0010000<br>2、float32_mantissa: 00110001000000000000000<br>舍弃精度等于0b1000000000000000，进位后离0越远，所以进位<br>得到bfloat16_mantissa: 0011001<br>3、float32_mantissa: 00100001010000010000000<br>舍弃精度大于0b1000000000000000，等效于(0.5, 1)，则进位<br>得到bfloat16_mantissa: 0010001<br>CAST_FLOOR：向负无穷方向舍入<br>1、float32_mantissa: 00100000010000010000000，float32值正数时不进位，得到bfloat16_mantissa: 0010000<br>2、float32_mantissa: 00110001000000000000000<br>float32 值负数时进位，得到bfloat16_mantissa: 0011001<br>CAST_CEIL：向正无穷方向舍入<br>1、float32_mantissa: 00100000010000010000000，float32值正数时进位，得到bfloat16_mantissa: 0010001<br>2、float32_mantissa: 00110001000000000000000<br>float32 值负数时不进位，得到bfloat16_mantissa: 0011000<br>CAST_TRUNC：向0方向舍入<br>1、float32_mantissa: 00100000010000010000000，都不进位，直接舍弃红色精度，得到bfloat16_mantissa: 001000<br>完整举例：输入float32 205.75，二进制为<br>sign: 0, exponent: 10000110, mantissa: 10011011100000000000000<br>CAST_RINT模式<br>float32_mantissa = 0b10011011100000000000000<br>舍弃精度大于0b1000000000000000，等效于(0.5, 1)，则进位<br>得到bfloat16_mantinssa = 0b1001110<br>所以bfloat16二进制为<br>sign: 0, exponent: 10000110, mantissa: 1001110<br>bfloat16 = 206 |
| bfloat16_t | half | bfloat16_t二进制规则为<br>sign: 1bit, exponent: 8bit, mantissa: 7bit<br>float16二进制规则为<br>sign: 1bit, exponent: 5bit, mantissa: 10bit<br>尾数位相差3位<br>当bfloat16_exponent = exponent-127 &gt;= -14时<br>因为bfloat16精度低，float16精度高，所以bfloat16到float16精度不变<br>例1：bfloat16_mantissa = 0b1011000 -&gt;<br>float16_maintissa = 0b1011000000, 二进制位不发生进位，只需要再低位补3个0<br>当bfloat16_exponent = exponent-127 &lt; -14时<br>例2：当exponent = 108， bfloat16_exponent = exponent-127 = -19&lt; -14时，因为float16_exponent = exponent - 14 最小为-14，无法表示，所以指数需要+5保持和float16_exponent相等，同时尾数位需要除以2的5次方，因为bf16计算公式 = s * (2^(e-127))*(man) =<br>s * (2^(e-127+5))*(man*2^(-5))，指数位乘以一个数，尾数位需要除以相同大小的数，保证最终值不变。假设bfloat16_mantissa = 0b1011011<br>bfloat16 mantissa计算公式为 1+ man/128，用例二进制表示为<br>0b1.1011011，指数位乘以2的5次方时，尾数要除以2的5次，小数点向左移动5位，所以bfloat16_mantissa = 0b0.000011011011，尾数为12位，因为float16只有10bit尾数位，所以在以下CAST模式中需要舍弃最低2位，并根据舍入模式决定是否舍入，当前例子的舍弃精度中间值为0b10<br>CAST_RINT：就近舍入，距离相等时向偶数进位<br>当bfloat16_mantissa尾数小于等于10位时，不需要舍入；<br>当bfloat16_mantissa尾数大于10位时，类似例2的例子<br>1）被舍弃的精度小于舍弃精度中间值时，不进位，<br>例bfloat16_mantissa = 0b0.00001101100xxx<br>0xxx &lt; 1000 输出float16_mantissa = 0b0.0000110110<br>2）被舍弃的精度大于舍弃精度中间值时，进位<br>例bfloat16_mantissa = 0b0.00001101100xxx<br>1x1x &gt; 1000 输出float16_mantissa = 0b0.0000110111<br>3）被舍弃的精度等于舍弃精度中间值时，第10位为偶数时不进位<br>例bfloat16_mantissa = 0b0.00001101100xxx<br>0 % 2 == 0 输出float16_mantissa = 0b0.0000110110<br>4) 被舍弃的精度等于舍弃精度中间值时，第10位为奇数时进位<br>例bfloat16_mantissa = 0b0.00001100010xxx<br>1 % 2 == 1 输出float16_mantissa = 0b0.0000110001<br>CAST_ROUND：就近舍入，距离相等时向远离0方向进位<br>当bfloat16_mantissa尾数小于等于10位时，不需要进位；<br>当bfloat16_mantissa尾数大于10位时，类似例2的例子<br>1）被舍弃的精度小于舍弃精度中间值时，不进位，<br>例bfloat16_mantissa = 0b0.00001101100xxx<br>0xxx &lt; 1000 输出float16_mantissa = 0b0.0000110110<br>2）被舍弃的精度大于舍弃精度中间值时，进位<br>例bfloat16_mantissa = 0b0.00001101100xxx<br>1x1x &gt; 1000 输出float16_mantissa = 0b0.0000110111<br>3) 被舍弃的精度等于舍弃精度中间值时，进位远离0，所以进位<br>例bfloat16_mantissa = 0b0.00001101100xxx<br>输出float16_mantissa = 0b0.0000110111<br>CAST_FLOOR：向负无穷方向舍入<br>当bfloat16_mantissa尾数小于等于10位时，不需要进位；<br>当bfloat16_mantissa尾数大于10位时，类似例2的例子<br>1）符号位S为0，既正数时不舍入，<br>例bfloat16_mantissa = 0b0.00001101100xxx<br>输出float16_mantissa = 0b0.0000110110<br>2）符号位S为1，既负数时舍入<br>例bfloat16_mantissa = 0b0.00001101100xxx<br>输出float16_mantissa = 0b0.0000110111<br>CAST_CEIL：向正无穷方向舍入<br>当bfloat16_mantissa尾数小于等于10位时，不需要舍入；<br>当bfloat16_mantissa尾数大于10位时，类似例2的例子<br>1）符号位S为0，既正数时舍入，<br>例bfloat16_mantissa = 0b0.00001101100xxx<br>输出float16_mantissa = 0b0.0000110111<br>2）符号位S为1，既负数时不舍入<br>例bfloat16_mantissa = 0b0.00001101100xxx<br>输出float16_mantissa = 0b0.0000110110<br>CAST_TRUNC：向0方向舍入<br>当bfloat16_mantissa尾数小于等于10位时，不需要舍入；<br>当bfloat16_mantissa尾数大于10位时，类似例2的例子<br>1）直接舍弃多余精度，不舍入<br>例bfloat16_mantissa = 0b0.00001101100xxx<br>输出float16_mantissa = 0b0.0000110110<br>完整举例：<br>输入bfloat16为2.90573e-06，二进制为<br>sign: 0, exponent: 01101100, mantissa: 1000011<br>bfloat16_exponent = 108-127 = -19 &lt; -14，float16_exponent最小为-14，所以bfloat16_exponent += 5，bfloat16_mantissa小数点左移<br>5位，1.1000011 -&gt; 0.000011000011<br>CAST_RINT模式<br>舍弃精度11大于舍弃精度中间值10，所以进位<br>bfloat16_mantissa = 0.0000110001<br>最终float16二进制为<br>sign: 0, exponent: 00000, mantissa = 0000110001<br>按照计算公式十进制表示为<br>float16=0.00000293 |
| half | bfloat16_t | half二进制规则为<br>sign: 1bit, exponent: 5bit, mantissa: 10bit<br>bfloat16_t二进制规则为<br>sign: 1bit,exponent: 8bit, mantissa: 7bit<br>CAST_RINT：就近舍入，距离相同时向偶数舍入<br>1）当舍弃精度小于舍弃精度中间值100时，不舍入<br>例half_mantinssa = 0111010011<br>输出bfloat16_mantissa = 0111010<br>2）当舍弃精度大于舍弃精度中间值100时，舍入<br>例half_mantinssa = 0111010111<br>输出bfloat16_mantissa = 0111011<br>3）当舍入精度等于舍弃精度中间值100时，bfloat16最低位为偶数，不舍入<br>例half_mantinssa = 0111010100<br>输出bfloat16_mantissa = 0111010<br>4）当舍入精度等于舍弃精度中间值100时，bfloat16最低位为奇数，<br>舍入<br>例half_mantinssa = 0111010100<br>输出bfloat16_mantissa = 0111011<br>CAST_ROUND：就近舍入，距离相同时远离0舍入<br>1）当舍弃精度小于舍弃精度中间值100时，不舍入<br>例half_mantinssa = 0111010011<br>输出bfloat16_mantissa = 0111010<br>2）当舍弃精度大于舍弃精度中间值100时，舍入<br>例half_mantinssa = 0111010111<br>输出bfloat16_mantissa = 0111011<br>3）当舍入精度等于舍弃精度中间值100时，进位远离0，所以舍入<br>例half_mantinssa = 0111010100<br>输出bfloat16_mantissa = 0111011<br>CAST_FLOOR：向负无穷方向舍入<br>1）当输入值为正数时，不舍入<br>例half_mantinssa = 0111010011<br>输出bfloat16_mantissa = 0111010<br>2）当输入值为负数时，舍入<br>例half_mantinssa = 0111010011<br>输出bfloat16_mantissa = 0111011<br>CAST_CEIL：向正无穷方向舍入<br>1）当输入值为正数时，舍入<br>例half_mantinssa = 0111010011<br>输出bfloat16_mantissa = 0111011<br>2）当输入值为负数时，不舍入<br>例half_mantinssa = 0111010011<br>输出bfloat16_mantissa = 0111010<br>CAST_TRUNC：向0方向舍入<br>直接舍弃多余精度<br>例half_mantinssa = 0111010011<br>输出bfloat16_mantissa = 0111010<br>完整举例：<br>输入half 0.131<br>二进制为sign: 0, exponent: 01100, mantissa: 0000110001<br>half_exponent = 12-15 = bfloat16_exp - 127<br>bfloat16_exp = 124 = 0b1111100<br>CAST_ROUND模式下<br>half_mantissa = 0000110001<br>舍弃精度小于舍弃中间精度100，不进位，所以得到<br>bfloat16_mantissa = 0000110<br>所以bfloat16二进制为<br>sign: 0, exponent: 1111100, mantissa: 0000110<br>按照计算公式十进制表示为<br>bfloat16=0.130859 |
| float | fp8_e4m3fn_t | float二进制规则为<br>sign: 1bit, exponent: 8bit mantissa: 23bit<br>fp8_e4m3fn_t二进制规则为<br>sign: 1bit, exponent: 4bit, mantissa: 3bit<br>需要舍弃20bit，舍弃精度中间值为0b10000000000000000000<br>CAST_RINT：就近舍入，距离相同时向偶数舍入<br>1）当舍弃精度小于舍弃精度中间值时，不舍入<br>例float_mantinssa = 01100100110000000000000<br>输出fp8_e4m3fn_t_mantissa = 011<br>2）当舍弃精度大于舍弃精度中间值时，舍入<br>例float_mantinssa= 01110100110000000000000<br>输出fp8_e4m3fn_t_mantissa  = 100<br>3）当舍入精度等于舍弃精度中间值时，fp8_e4m3fn_t最低位为偶数，不舍入<br>例float_mantinssa= 01010000000000000000000<br>输出fp8_e4m3fn_t_mantissa = 010<br>4）当舍入精度等于舍弃精度中间值时，fp8_e4m3fn_t最低位为奇数，<br>舍入<br>例float_mantinssa= 01110000000000000000000<br>输出fp8_e4m3fn_t_mantissa = 100<br>完整例子：<br>输入float 1.233<br>二进制为<br>sign: 0, exponent: 01111111, mantissa: 00111011101001011110010<br>float_exponent = 127-127 = fp8_e4m3fn_t_exp - 7<br>fp8_e4m3fn_t_exp = 7 = 0b0111<br>CAST_RINT模式下<br>float_mantissa = 00111011101001011110010<br>舍弃精度大于舍弃中间精度，进位，所以得到<br>fp8_e4m3fn_t_mantissa = 010<br>所以fp8_e4m3fn_t二进制为<br>sign: 0, exponent: 0111, mantissa: 010<br>按照计算公式十进制表示为<br>fp8_e4m3fn_t=1.25 |
| float | fp8_e5m2_t | float二进制规则为<br>sign: 1bit, exponent: 8bit mantissa: 23bit<br>fp8_e5m2_t二进制规则为<br>sign: 1bit, exponent: 5bit, mantissa: 2bit<br>需要舍弃21bit，舍弃精度中间值为0b100000000000000000000<br>CAST_RINT：就近舍入，距离相同时向偶数舍入<br>1）当舍弃精度小于舍弃精度中间值时，不舍入<br>例float_mantinssa = 01000100110000000000000<br>输出fp8_e5m2_t_mantissa = 01<br>2）当舍弃精度大于舍弃精度中间值时，舍入<br>例float_mantinssa= 01101001100000000000000<br>输出fp8_e5m2_t_mantissa  = 10<br>3）当舍入精度等于舍弃精度中间值时，fp8_e5m2_t最低位为偶数，不舍入<br>例float_mantinssa= 00100000000000000000000<br>输出fp8_e5m2_t_mantissa = 00<br>4）当舍入精度等于舍弃精度中间值时，fp8_e5m2_t最低位为奇数，<br>舍入<br>例float_mantinssa= 0110000000000000000000<br>输出fp8_e5m2_t_mantissa = 10<br>完整例子：<br>输入float32 1.233<br>二进制为<br>sign: 0, exponent: 01111111, mantissa: 00111011101001011110010<br>float_exponent = 127-127 = fp8_e5m2_t_exp - 15<br>fp8_e5m2_t_exp = 15 = 0b01111<br>CAST_RINT模式下<br>float_mantissa = 00111011101001011110010<br>舍弃精度大于舍弃中间精度，进位，所以得到<br>fp8_e5m2_t_mantissa = 01<br>所以fp8_e5m2_t二进制为<br>sign: 0, exponent: 01111, mantissa: 01<br>按照计算公式十进制表示为<br>fp8_e5m2_t=1.25 |
| bfloat16_t | fp4x2_e2m1_t | bfloat16二进制规则为<br>sign: 1bit, exponent: 8bit, mantissa: 7bit<br>fp4x2_e2m1_t二进制规则为<br>sign: 1bit, exponent: 2bit, mantissa: 1bit<br>尾数位相差6位<br>当bfloat16_exponent = exponent-127 &lt; 0时<br>例1：当exponent = 124, bfloat16_exponent = exponent-127 = -3 &lt; 0时，因为float4_e2m1_exponent = exponent - 1 最小为0，无法表示，所以指数需要+3保持和float4_e2m1_exponent相等，同时尾数位需要除以2的3次方，因为bf16计算公式 = s * (2^(e-127))*(man) =<br>s * (2^(e-127+3))*(man*2^(-3))，指数位乘以一个数，尾数位需要除以相同大小的数，保证最终值不变。假设bfloat16_mantissa = 0b1011011<br>bfloat16 mantissa计算公式为 1+ man/128，用例二进制表示为<br>0b1.1011011，指数位乘以2的3次方时，尾数要除以2的3次，小数点向左移动3位，所以bfloat16_mantissa = 0b0.0011011011，因为float4_e2m1只有1bit尾数位，所以在以下CAST模式中需要舍弃最低9位，并根据舍入模式决定是否舍入，当前例子的舍弃精度中间值为0b100000000<br>CAST_RINT：就近舍入，距离相等时向偶数进位<br>1）被舍弃的精度小于舍弃精度中间值时，不进位，<br>例bfloat16_mantissa = 0b0.0000110xxx<br>000110xxx &lt；舍弃精度中间值 输出float4_e2m1_mantissa = 0b0.0<br>2）被舍弃的精度大于舍弃精度中间值时，进位<br>例bfloat16_mantissa = 0b0.0100110xxx<br>100110xxx &gt；舍弃精度中间值 输出float4_e2m1_mantissa = 0b0.1<br>3） 被舍弃的精度等于舍弃精度中间值时，第1位为偶数时不进位<br>例bfloat16_mantissa = 0b0.0100000xxx<br>0 % 2 == 0 输出float4_e2m1_mantissa = 0b0.0<br>4） 被舍弃的精度等于舍弃精度中间值时，第1位为奇数时进位<br>例bfloat16_mantissa = 0b0.1100000xxx<br>1 % 2 == 1 输出float4_e2m1_mantissa = 0b0.1<br>CAST_ROUND：就近舍入，距离相等时向远离0方向进位<br>1）被舍弃的精度小于舍弃精度中间值时，不进位，<br>例bfloat16_mantissa = 0b0.0000110xxx<br>000110xxx &lt；舍弃精度中间值 输出float4_e2m1_mantissa = 0b0.0<br>2）被舍弃的精度大于舍弃精度中间值时，进位<br>例bfloat16_mantissa = 0b0.0100110xxx<br>100110xxx &gt；舍弃精度中间值 输出float4_e2m1_mantissa = 0b0.1<br>3） 被舍弃的精度等于舍弃精度中间值时，进位远离0，所以进位<br>例bfloat16_mantissa = 0b0.0100000xxx<br>输出float4_e2m1_mantissa = 0b0.1<br>CAST_FLOOR：向负无穷方向舍入<br>1）输入值正数时，不进位，<br>例bfloat16_mantissa = 0b0.0000110xxx<br>输出float4_e2m1_mantissa = 0b0.0<br>2）输入值负数时，进位<br>例bfloat16_mantissa = 0b0.0100110xxx<br>输出float4_e2m1_mantissa = 0b0.1<br>CAST_CEIL：向正无穷方向舍入<br>1）输入值正数时，进位，<br>例bfloat16_mantissa = 0b0.0000110xxx<br>输出float4_e2m1_mantissa = 0b0.1<br>2）输入值负数时，不进位<br>例bfloat16_mantissa = 0b0.0100110xxx<br>输出float4_e2m1_mantissa = 0b0.0<br>CAST_TRUNC：向0方向舍入<br>1）直接舍弃多余精度，不进位<br>例bfloat16_mantissa = 0b0.0100110xxx<br>输出float4_e2m1_mantissa = 0b0.0<br>完整举例：<br>输入bfloat16为0.761719，二进制为<br>sign: 0, exponent: 01111110, mantissa: 1000011<br>bfloat16_exponent = 126-127 = -1 &lt; 0，float16_exponent最小为0，所以bfloat16_exponent += 1，bfloat16_mantissa小数点左移<br>1位，1.1000011 -&gt; 0.11000011<br>CAST_RINT模式<br>舍弃精度1000011大于舍弃精度中间值1000000，所以进位<br>float4_e2m1_mantissa = 0.1<br>最终float4_e2m1二进制为<br>sign: 0, exponent: 00, mantissa = 1<br>按照计算公式十进制表示为<br>float4_e2m1=0.5 |
| bfloat16_t | fp4x2_e1m2_t | bfloat16二进制规则为<br>sign: 1bit, exponent: 8bit, mantissa: 7bit<br>fp4x2_e1m2_t二进制规则为<br>sign: 1bit, exponent: 1bit, mantissa: 2bit<br>尾数位相差5位<br>当bfloat16_exponent = exponent-127 &lt; 0时，bfloat16_mantissa需要发生位移<br>例1：当exponent = 124， bfloat16_exponent = exponent-127 = -3 &lt; 0时，因为float4_e1m2_exponent = exponent - 1 最小为0，无法表示，所以指数需要+3保持和float4_e2m1_exponent相等，同时尾数位需要除以2的3次方，因为bf16计算公式 = s * (2^(e-127))*(man) =<br>s * (2^(e-127+3))*(man*2^(-3))，指数位乘以一个数，尾数位需要除以相同大小的数，保证最终值不变。假设bfloat16_mantissa = 0b1011011<br>bfloat16 mantissa计算公式为 1+ man/128，用例二进制表示为<br>0b1.1011011，指数位乘以2的3次方时，尾数要除以2的3次，小数点向左移动3位，所以bfloat16_mantissa = 0b0.0011011011，因为float4_e2m1只有2bit尾数位，所以在以下CAST模式中需要舍弃最低8位，并根据舍入模式决定是否舍入，当前例子的舍弃精度中间值为0b10000000<br>CAST_RINT：就近舍入，距离相等时向偶数进位<br>1）被舍弃的精度小于舍弃精度中间值时，不进位，<br>例bfloat16_mantissa = 0b0.0000011xxx<br>00011xxx &lt；舍弃精度中间值 输出float4_e1m2_mantissa = 0b0.00<br>2）被舍弃的精度大于舍弃精度中间值时，进位<br>例bfloat16_mantissa = 0b0.0010011xxx<br>10011xxx &gt；舍弃精度中间值 输出float4_e1m2_mantissa = 0b0.01<br>3） 被舍弃的精度等于舍弃精度中间值时，第2位为偶数时不进位<br>例bfloat16_mantissa = 0b0.0010000xxx<br>0 % 2 == 0 输出float4_e1m2_mantissa = 0b0.00<br>4） 被舍弃的精度等于舍弃精度中间值时，第2位为奇数时进位<br>例bfloat16_mantissa = 0b0.0110000xxx<br>1 % 2 == 1 输出float4_e1m2_mantissa = 0b0.10<br>CAST_ROUND：就近舍入，距离相等时向远离0方向进位<br>1）被舍弃的精度小于舍弃精度中间值时，不进位，<br>例bfloat16_mantissa = 0b0.0000110xxx<br>00110xxx &lt；舍弃精度中间值 输出float4_e1m2_mantissa = 0b0.00<br>2）被舍弃的精度大于舍弃精度中间值时，进位<br>例bfloat16_mantissa = 0b0.0010110xxx<br>10110xxx &gt；舍弃精度中间值 输出float4_e1m2_mantissa = 0b0.01<br>3） 被舍弃的精度等于舍弃精度中间值时，进位远离0，所以进位<br>例bfloat16_mantissa = 0b0.0010000xxx<br>输出float4_e1m2_mantissa = 0b0.01<br>CAST_FLOOR：向负无穷方向舍入<br>1）输入值正数时，不进位，<br>例bfloat16_mantissa = 0b0.0000110xxx<br>输出float4_e1m2_mantissa = 0b0.00<br>2）输入值负数时，进位<br>例bfloat16_mantissa = 0b0.00100110xxx<br>输出float4_e1m2_mantissa = 0b0.01<br>CAST_CEIL：向正无穷方向舍入<br>1）输入值正数时，进位，<br>例bfloat16_mantissa = 0b0.0000110xxx<br>输出float4_e1m2_mantissa = 0b0.01<br>2）输入值负数时，不进位<br>例bfloat16_mantissa = 0b0.0010110xxx<br>输出float4_e1m2_mantissa = 0b0.00<br>CAST_TRUNC：向0方向舍入<br>1）直接舍弃多余精度，不进位<br>例bfloat16_mantissa = 0b0.0010110xxx<br>输出float4_e1m2_mantissa = 0b0.00<br>完整举例：<br>输入bfloat16为0.761719，二进制为<br>sign: 0, exponent: 01111110, mantissa: 1000011<br>bfloat16_exponent = 126-127 = -1 &lt; 0，float16_exponent最小为0，所以bfloat16_exponent += 1，bfloat16_mantissa小数点左移<br>1位，1.1000011 -&gt; 0.11000011<br>CAST_RINT模式<br>舍弃精度000011小于舍弃精度中间值100000，所以不进位<br>float4_e2m1_mantissa = 0.11<br>最终float4_e2m1二进制为<br>sign: 0, exponent: 0, mantissa = 11<br>按照计算公式十进制表示为<br>float4_e2m1=0.75 |
| float | hifloat8_t | 参考表12。 |

**表 12**  float转hifloat8_t类型转换规则

| VCVTFF: F322HiF8 |
| --- |
| tmp2[31 : 0] = f32_src_data[i][31 : 0];<br>Ev = tmp2[30 : 23] - 8'b01111111, thr = tmp2[13 : 0];<br>if (Ev == 128 && tmp2[22 : 0] != 23'b0) then<br>tmp3[7 : 0] = HiF8NAN(8'b10000000);<br>else if ((Ev == 128 && tmp2[22 : 0] == 23'b0) \|\| (Ev > 15)) then<br>tmp3[7 : 0] = (tmp2[31] == 1'b0) ? HiF8+INF (8'b01101111) : HiF8-INF (8'b11101111);<br>else if (Ev < -23) then<br>tmp3[7 : 0] = HiF8ZERO (8'b00000000);<br>else if (Ev == -23) then<br>if ((Half To Away Round) \|\| (Hybrid Round && {1’b1, tmp2[22 : 10]} >= thr)) then<br>tmp3[7 : 0] = (tmp2[31] == 1'b0) ? 8'b00000001 : 8'b10000001; // min subnormal<br>else<br>tmp3[7 : 0] = HiF8ZERO (8'b00000000);<br>end if<br>else<br>if (Ev == 0) then<br>M = tmp2[22 : 20], T A_bit = tmp2[19], frac = tmp2[19 : 6];<br>else if (Ev == ±1) then<br>M = tmp2[22 : 20], T A_bit = tmp2[19], frac = tmp2[19 : 6];<br>else if (Ev == ±[2, 3]) then<br>M = tmp2[22 : 20], T A_bit = tmp2[19], frac = tmp2[19:6];<br>else if (Ev == ±[4, 7]) then<br>M = tmp2[22 : 21], T A_bit = tmp2[20], frac = tmp2[20 : 7];<br>else if (Ev == ±[8, 15]) then<br>M = tmp2[22], T A_bit = tmp2[21], frac = tmp2[21 : 8];<br>else if (Ev == [-16, -22]) then<br>M = Ev + 23, T A_bit = tmp2[22], frac = tmp2[22 : 9]; // subnormal<br>end if<br>if (Ev == ±[0, 3]) then<br>if (T A_bit == 1’b1) then<br>M_tmp = M + 1, Ev = Ev + carry of M_tmp, M = (carry of M_tmp) ? 0 : M_tmp;<br>end if<br>else if (Ev == ±[4, 15]) then<br>if (HALF To Away Round && T A_bit == 1’b1) \|\| (Hybrid Round && frac >= thr) then<br>M_tmp = M + 1, Ev = Ev + carry of M_tmp, M = (carry of M_tmp) ? 0 : M_tmp;<br>end if<br>else if (Ev == [-16, -22]) then<br>if ((Half To Away Round) && T A_bit == 1'b1) \|\| ((Hybrid Round && frac >= thr) then<br>M_tmp = Ev + 23, Ev = Ev + 1, M = (Ev == -15) ? 0 : M_tmp;<br>end if<br>end if<br>encode {tmp3[31], Ev, M} to tmp3[7 : 0] following HiF encoding rule;<br>end if<br>result[i][7 : 0] = saturation(tmp3[7 : 0]) according to control bit; |

**表 13**  half转hifloat8_t类型转换规则

| VCVTFF: F162HiF8 |
| --- |
| tmp2[15 : 0] = f16_src_data[i][15 : 0];<br>Ev = tmp2[14 : 10] - 5'b01111, thr = {tmp2[0], 1’b1};<br>if (Ev == 16 && tmp2[9 : 0] != 10'b0) then<br>tmp3[7 : 0] = HiF8NAN(8'b10000000);<br>else if (Ev == 16 && tmp2[9:0] == 10'b0) && (Ev > 15) then<br>tmp3[7 : 0] = (tmp2[31] == 1'b0) ? HiF8+INF(8'b01101111) : HiF8-INF(8'b11101111);<br>else if (Ev < -23) then<br>tmp3[7 : 0] = HiF8ZERO(8'b00000000);<br>else if (Ev == -23) then<br>if ((Half To Away Round) \|\| (Hybrid Round && {1’b1, tmp2[9]} >= thr)) then<br>tmp3[7 : 0] = (tmp2[31] == 1'b0) ? 8'b00000001 : 8'b10000001; // min subnormal<br>else<br>tmp3[7 : 0] = HiF8ZERO(8'b00000000);<br>end if<br>else<br>if (Ev == 0) then<br>M = tmp2[9 : 7], T A_bit = tmp2[6], frac = tmp2[6 : 5];<br>else if (Ev == ±1) then<br>M = tmp2[9 : 7], T A_bit = tmp2[6], frac = tmp2[6 : 5];<br>else if (Ev == ±[2, 3]) then<br>M = tmp2[9 : 7], T A_bit = tmp2[6], frac = tmp2[6 : 5];<br>else if (Ev == ±[4, 7]) then<br>M = tmp2[9 : 8], T A_bit = tmp2[7], frac = tmp2[7 : 6];<br>else if (Ev == ±[8, 15]) then<br>M = tmp2[9], T A_bit = tmp2[8], frac = tmp2[8 : 7];<br>else if (Ev == [-16, -22]) then<br>M = Ev + 23, T A_bit = tmp2[9], frac = tmp2[9 : 8]; // subnormal<br>end if<br>if (Ev == ±[0, 3]) then<br>if (T A_bit == 1’b1) then<br>M_tmp = M + 1, Ev = Ev + carry of M_tmp, M = (carry of M_tmp) ? 0 : M_tmp;<br>end if<br>else if (Ev == ±[4, 15]) then<br>if (HALF To Away Round && T A_bit == 1’b1) \|\| (Hybrid Round && frac >= thr) then<br>M_tmp = M + 1, Ev = Ev + carry of M_tmp, M = (carry of M_tmp) ? 0 : M_tmp;<br>end if<br>else if (Ev == [-16, -22]) then<br>if ((Half To Away Round) && T A_bit == 1'b1) \|\| (Hybrid Round && frac >= thr) then<br>M_tmp = Ev + 23, Ev = Ev + 1, M = (Ev == -15) ? 0 : M_tmp;<br>end if<br>end if<br>encode {tmp3[31], Ev, M} to tmp3[7 : 0] following HiF encoding rule;<br>end if<br>result[i][7 : 0] = saturation(tmp3[7 : 0]) according to control bit; |

## 调用示例<a name="section642mcpsimp"></a>

-   场景1：位宽小转大，以half→int32_t为例

    ```cpp
    __simd_vf__ inline void CastVFF162S32(__ubuf__ half* xAddr, __ubuf__ int32_t* yAddr,
        uint32_t repeatTimes, uint32_t oneRepeatSize)
    {
        AscendC::Reg::MaskReg mask = AscendC::Reg::CreateMask<int32_t, AscendC::Reg::MaskPattern::ALL>();
        AscendC::Reg::RegTensor<half> xReg;
        AscendC::Reg::RegTensor<int32_t> yReg;
        static constexpr AscendC::Reg::CastTrait castTrait = {
            AscendC::Reg::RegLayout::ZERO,
            AscendC::Reg::SatMode::UNKNOWN,
            AscendC::Reg::MaskMergeMode::ZEROING,
            AscendC::RoundMode::CAST_FLOOR
        };
        for (uint16_t i = 0; i < repeatTimes; ++i) {
            AscendC::Reg::LoadAlign<half, AscendC::Reg::PostLiteral::POST_MODE_UPDATE,
                AscendC::Reg::LoadDist::DIST_UNPACK_B16>(xReg, xAddr, oneRepeatSize);
            AscendC::Reg::Cast<int32_t, half, castTrait>(yReg, xReg, mask);
            AscendC::Reg::StoreAlign<int32_t, AscendC::Reg::PostLiteral::POST_MODE_UPDATE>(
                yAddr, yReg, oneRepeatSize, mask);
        }
    }
    ```

-   场景2：位宽大转小，以float→int16_t为例

    ```cpp
    __simd_vf__ inline void CastVFF322S16(__ubuf__ float* xAddr, __ubuf__ int16_t* yAddr,
        uint32_t repeatTimes, uint32_t oneRepeatSize)
    {
        AscendC::Reg::MaskReg mask = AscendC::Reg::CreateMask<float, AscendC::Reg::MaskPattern::ALL>();
        AscendC::Reg::RegTensor<float> xReg;
        AscendC::Reg::RegTensor<int16_t> yReg;
        static constexpr AscendC::Reg::CastTrait castTrait = {
            AscendC::Reg::RegLayout::ZERO,
            AscendC::Reg::SatMode::SAT,
            AscendC::Reg::MaskMergeMode::ZEROING,
            AscendC::RoundMode::CAST_ROUND
        };
        for (uint16_t i = 0; i < repeatTimes; ++i) {
            AscendC::Reg::LoadAlign<float, AscendC::Reg::PostLiteral::POST_MODE_UPDATE>(xReg, xAddr, oneRepeatSize);
            AscendC::Reg::Cast<int16_t, float, castTrait>(yReg, xReg, mask);
            AscendC::Reg::StoreAlign<int16_t, AscendC::Reg::PostLiteral::POST_MODE_UPDATE,
                AscendC::Reg::StoreDist::DIST_PACK_B32>(yAddr, yReg, oneRepeatSize, mask);
        }
    }
    ```
