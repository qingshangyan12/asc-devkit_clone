# C API Reg精度与低精度支持测试结果

依据[C_API_Reg精度与低精度支持方案设计.md](C_API_Reg精度与低精度支持方案设计.md)的6类测试用例，检查[reg_vector_compute_interface_precision_standard_summary.md](docs/zh/api/appendix/reg_vector_compute_interface_precision_standard_summary.md)当前工作区版本。

结论：**不能判定全部通过。**表3的19组浮点硬件路径对应正确，FP8广播和无软仿结论成立；表4有9个未带必要后缀的接口族名称，不能作为可调用的Reg接口名。表2还存在Int8/UInt8的Muls支持范围错误，表3的舍入模式和整数取负范围需要澄清。

检查基线：HEAD为`890b434fdb5b6a05c6172b41c9dc10a443cc83b2`，包含用户尚未提交的目标文档改动；目标架构为Ascend 950PR/Ascend 950DT（3510）。本次只新增本报告，未修改原文档或代码。

验证方式为仓内静态检查：公开头文件声明、Reg实现、Basic API的3510实现和对应API页面交叉核验。未运行CANN编译、反汇编、仿真或上板数值测试；“最终指令一致”指封装最终调用的编译器intrinsic一致，不等同于对生成机器码或ULP数值重新实测。精度数值以表1为既定基线，本报告验证其迁移条件。

## 测试用例结果

| 用例 | 结果 | 实际结果 |
| --- | --- | --- |
| 接口测试 | 不通过 | 表4共9种类型、98个类型—接口组合、55个不同名称；89个组合存在同名Reg声明，9个转换组合只存在带后缀的接口。按真实接口族展开后，类型及转换方向匹配。另有4个UInt8转换的饱和版本未列。 |
| FP8测试 | 通过 | 3种FP8类型×6类广播，共18组，全部直接调用vdup，无软件组合分支。 |
| 指令测试 | 通过（限定相同类型和模式） | 表3的19组均匹配表1的对应硬件路径。只匹配浮点精度涉及的重载、求和归约和匹配的布局，不能推广到Basic API默认软仿路径。 |
| 语义测试 | 部分通过 | madd/mula/axpy及三类求和的操作数关系正确；MulsCast的通用roundingmode、Neg未限定浮点、AbsSub的一元公式需要修正或澄清。 |
| 软仿测试 | 通过 | 表3对应C API实现无算法配置、补偿循环或Basic API转调，19行软仿列均为“-”。返回值重载转调同名引用输出重载，不属于软仿。 |
| 文档测试 | 结构检查通过，存在内容及写作问题 | 工作区与暂存区diff检查均返回0；四表列数和行数正确，标题编号连续，5个锚点唯一，页内链接有效。接口命名及两处中文逗号问题另列。 |

## 问题及建议

| 编码 | 位置 | 问题及证据 | 建议 |
| --- | --- | --- | --- |
| C001 | [表4第86—94行](docs/zh/api/appendix/reg_vector_compute_interface_precision_standard_summary.md#L86) | 9个转换接口族名不是Reg公开函数名。对应页面使用round_mode等占位符，而汇总表删掉了这些必要后缀。详见下一节逐项清单。注意Memory API存在同名asc_half2int8，不能作为Reg声明证据。 | 列出真实名称，或明确标注“接口族”并使用带后缀占位符、解释可取值。 |
| C002 | [表4第94行](docs/zh/api/appendix/reg_vector_compute_interface_precision_standard_summary.md#L94) | UInt8转换仅列非饱和名称，缺asc_int162uint8_sat、asc_int322uint8_sat、asc_uint162uint8_sat、asc_uint322uint8_sat。[饱和声明证据](include/c_api/reg_compute/reg_convert.h#L99)。现有名称并非类型错误，但若该表表达完整支持范围则不完整。 | 补充4个_sat接口，或明确每个名称代表同时包含饱和版本的接口族。 |
| C003 | [表2第43—44行](docs/zh/api/appendix/reg_vector_compute_interface_precision_standard_summary.md#L43) | 表2将Muls列入int8_t、uint8_t；[MulsImpl类型约束](impl/basic_api/reg_compute/dav_3510/kernel_reg_compute_vec_binary_scalar_impl.h#L148)和[Basic API类型说明](docs/zh/api/SIMD-API/basic_api/reg_vector_compute/basic_arithmetic/Muls.md#L67)均不支持这两种类型。 | 从表2两行移除Muls；表4不添加asc_mul_scalar。 |
| C004 | [表3第77行](docs/zh/api/appendix/reg_vector_compute_interface_precision_standard_summary.md#L77) | asc_mul_scalar_float2half_rn没有舍入模式参数，却保留cast_to_f16&lt;roundingmode&gt;，容易被理解为可选舍入。[两种位置重载](impl/c_api/reg_base_impl/reg_fused_intf_impl.h#L90)只改变PART_EVEN/PART_ODD。 | 明确为本接口固定舍入模式，不使用可配置占位符；单独写明float×float转half及EVEN/ODD。 |
| C005 | [表3第68行](docs/zh/api/appendix/reg_vector_compute_interface_precision_standard_summary.md#L68)、[表1第17行](docs/zh/api/appendix/reg_vector_compute_interface_precision_standard_summary.md#L17) | “符号位取反”仅适用于浮点；asc_neg还支持int8_t、int16_t、int32_t，整数取负并不是只翻转符号位。[整数及浮点行为](docs/zh/api/SIMD-API/c_api/reg_compute/reg_arith/asc_neg.md#L92)明确区分两者。 | 写为“half/float：符号位取反”，或在表前明确全部精度描述仅适用所列浮点重载。整数语义写为dst=-src。 |
| C006 | [表3第72行](docs/zh/api/appendix/reg_vector_compute_interface_precision_standard_summary.md#L72)、[表1第22行](docs/zh/api/appendix/reg_vector_compute_interface_precision_standard_summary.md#L22) | AbsSub(x)=Abs(Sub(x))省略第二个输入；两个封装实际均直接调用vabsdif，不能将“指令串行”理解为实现中发出独立Sub与Abs两条指令。 | 写为“精度等效于先减后取绝对值，AbsSub(x,y)=Abs(Sub(x,y))”。 |
| C007 | [第24行](docs/zh/api/appendix/reg_vector_compute_interface_precision_standard_summary.md#L24)、[第74行](docs/zh/api/appendix/reg_vector_compute_interface_precision_standard_summary.md#L74) | 中文“乘累加采用高精度计算,”使用英文逗号。 | 改为中文逗号。 |

本报告按用户请求交付分析，不执行上述修改。C001—C006涉及内容准确性或范围，C007为写作问题。

## 接口测试：转换名称、方向与边界

以下9个原名在Reg公开头文件和实现中均无精确同名函数；不是宏别名。文档页面名称不能直接当作可调用名称。表中花括号为报告简写，实际函数名需将其展开。

| 原表名称 | 实际公开名称或后缀集合 | 方向 | 证据 |
| --- | --- | --- | --- |
| `asc_bfloat162e2m1x2` | `asc_bfloat162e2m1x2_rd`、`asc_bfloat162e2m1x2_rn`、`asc_bfloat162e2m1x2_rna`、`asc_bfloat162e2m1x2_ru`、`asc_bfloat162e2m1x2_rz` | bfloat16_t → fp4x2_e2m1_t | [声明 L906](include/c_api/reg_compute/reg_convert.h#L906)；[实现 L3528](impl/c_api/reg_base_impl/reg_convert_intf_impl.h#L3528)；[API文档](docs/zh/api/SIMD-API/c_api/reg_compute/reg_convert/asc_bfloat162e2m1x2.md) |
| `asc_bfloat162e1m2x2` | `asc_bfloat162e1m2x2_rd`、`asc_bfloat162e1m2x2_rn`、`asc_bfloat162e1m2x2_rna`、`asc_bfloat162e1m2x2_ru`、`asc_bfloat162e1m2x2_rz` | bfloat16_t → fp4x2_e1m2_t | [声明 L846](include/c_api/reg_compute/reg_convert.h#L846)；[实现 L5875](impl/c_api/reg_base_impl/reg_convert_intf_impl.h#L5875)；[API文档](docs/zh/api/SIMD-API/c_api/reg_compute/reg_convert/asc_bfloat162e1m2x2.md) |
| `asc_float2e5m2` | `asc_float2e5m2_rn`、`asc_float2e5m2_rn_sat` | float → fp8_e5m2_t | [声明 L299](include/c_api/reg_compute/reg_convert.h#L299)；[实现 L608](impl/c_api/reg_base_impl/reg_convert_intf_impl.h#L608)；[API文档](docs/zh/api/SIMD-API/c_api/reg_compute/reg_convert/asc_float2e5m2.md) |
| `asc_float2e4m3` | `asc_float2e4m3_rn`、`asc_float2e4m3_rn_sat` | float → fp8_e4m3fn_t | [声明 L29](include/c_api/reg_compute/reg_convert.h#L29)；[实现 L5511](impl/c_api/reg_base_impl/reg_convert_intf_impl.h#L5511)；[API文档](docs/zh/api/SIMD-API/c_api/reg_compute/reg_convert/asc_float2e4m3.md) |
| `asc_float2hif8` | `asc_float2hif8_rh`、`asc_float2hif8_rh_sat`、`asc_float2hif8_rna`、`asc_float2hif8_rna_sat` | float → hifloat8_t | [声明 L602](include/c_api/reg_compute/reg_convert.h#L602)；[实现 L3928](impl/c_api/reg_base_impl/reg_convert_intf_impl.h#L3928)；[API文档](docs/zh/api/SIMD-API/c_api/reg_compute/reg_convert/asc_float2hif8.md) |
| `asc_half2hif8` | `asc_half2hif8_rh`、`asc_half2hif8_rh_sat`、`asc_half2hif8_rna`、`asc_half2hif8_rna_sat` | half → hifloat8_t | [声明 L4018](include/c_api/reg_compute/reg_convert.h#L4018)；[实现 L1178](impl/c_api/reg_base_impl/reg_convert_intf_impl.h#L1178)；[API文档](docs/zh/api/SIMD-API/c_api/reg_compute/reg_convert/asc_half2hif8.md) |
| `asc_half2int4x2` | `asc_half2int4x2_rd`、`asc_half2int4x2_rd_sat`、`asc_half2int4x2_rn`、`asc_half2int4x2_rn_sat`、`asc_half2int4x2_rna`、`asc_half2int4x2_rna_sat`、`asc_half2int4x2_ru`、`asc_half2int4x2_ru_sat`、`asc_half2int4x2_rz`、`asc_half2int4x2_rz_sat` | half → int4x2_t | [声明 L5430](include/c_api/reg_compute/reg_convert.h#L5430)；[实现 L7092](impl/c_api/reg_base_impl/reg_convert_intf_impl.h#L7092)；[API文档](docs/zh/api/SIMD-API/c_api/reg_compute/reg_convert/asc_half2int4x2.md) |
| `asc_half2int8` | `asc_half2int8_rd`、`asc_half2int8_rd_sat`、`asc_half2int8_rn`、`asc_half2int8_rn_sat`、`asc_half2int8_rna`、`asc_half2int8_rna_sat`、`asc_half2int8_ru`、`asc_half2int8_ru_sat`、`asc_half2int8_rz`、`asc_half2int8_rz_sat` | half → int8_t | [声明 L3700](include/c_api/reg_compute/reg_convert.h#L3700)；[实现 L1314](impl/c_api/reg_base_impl/reg_convert_intf_impl.h#L1314)；[API文档](docs/zh/api/SIMD-API/c_api/reg_compute/reg_convert/asc_half2int8.md) |
| `asc_half2uint8` | `asc_half2uint8_rd`、`asc_half2uint8_rd_sat`、`asc_half2uint8_rn`、`asc_half2uint8_rn_sat`、`asc_half2uint8_rna`、`asc_half2uint8_rna_sat`、`asc_half2uint8_ru`、`asc_half2uint8_ru_sat`、`asc_half2uint8_rz`、`asc_half2uint8_rz_sat` | half → uint8_t | [声明 L5229](include/c_api/reg_compute/reg_convert.h#L5229)；[实现 L1938](impl/c_api/reg_base_impl/reg_convert_intf_impl.h#L1938)；[API文档](docs/zh/api/SIMD-API/c_api/reg_compute/reg_convert/asc_half2uint8.md) |

实际窄化转换还须匹配舍入、饱和和位置选择：FP8 E5M2/E4M3FN只列RN及RN_SAT；HiFloat8有RH、RNA及其SAT版本；FP4有RD/RN/RNA/RU/RZ；half转Int4/Int8/UInt8有这5种舍入及SAT组合。位置标签选择EVEN/ODD或四分之一分组，不改变转换方向。报告不推荐旧的_v2/_v3/_v4兼容接口。

表4其余转换方向核对结果如下。所有箭头均按源类型到目标类型表示。

| 表4类型 | 已列转换方向 | 结果 |
| --- | --- | --- |
| fp4x2_e2m1_t | bfloat16_t ↔ fp4x2_e2m1_t | 方向正确；正向需加舍入后缀。 |
| fp4x2_e1m2_t | bfloat16_t ↔ fp4x2_e1m2_t | 方向正确；正向需加舍入后缀。 |
| fp8_e5m2_t | float ↔ fp8_e5m2_t | 方向正确；正向需加舍入/饱和后缀。 |
| fp8_e4m3fn_t | float ↔ fp8_e4m3fn_t | 方向正确；正向需加舍入/饱和后缀。 |
| fp8_e8m0_t | 无转换接口 | 与当前Reg C API声明一致，不能从Basic Cast推导出不存在的C接口。 |
| hifloat8_t | float ↔ hifloat8_t；half ↔ hifloat8_t | 方向正确；两个转入方向需加后缀。 |
| int4x2_t | half → int4x2_t；int4x2_t → bfloat16_t/half/int16_t | 方向正确；转入方向需加后缀。 |
| int8_t | half → int8_t；int8_t → half/int16_t/int32_t | 方向正确；转入方向需加后缀。 |
| uint8_t | half/int16_t/int32_t/uint16_t/uint32_t → uint8_t；uint8_t → half/uint16_t/uint32_t | 方向正确；half转入需加后缀，整数转入另有_sat版本。 |

不能从表2机械复制的差异：

- FP4和HiFloat8：当前C广播声明不包含这些类型，表4不列Duplicate是正确的。
- FP8 E8M0：Basic Cast有bfloat16_t双向软件实现，C侧没有对应Reg转换重载；表4只列广播正确。证据：[Basic E8M0转出](impl/basic_api/reg_compute/dav_3510/kernel_reg_compute_vec_vconv_impl.h#L58)、[Basic E8M0转入](impl/basic_api/reg_compute/dav_3510/kernel_reg_compute_vec_vconv_impl.h#L187)。
- Int8/UInt8：两侧Muls/asc_mul_scalar均不提供8位重载，表2基线应纠正。UInt8也不能增加有符号Abs/Neg。
- Basic低精度Duplicate带mask时将寄存器/标量按int8_t重解释后调用vdup，C侧直接使用FP8类型调用vdup；指令族与位宽操作对应，但编译器重载参数类型并非完全相同。Basic无mask版本调用vbr，不应混作本次六类带mask接口的对应重载。证据：[标量带mask](impl/basic_api/reg_compute/dav_3510/kernel_reg_compute_vec_duplicate_impl.h#L159)、[寄存器带mask](impl/basic_api/reg_compute/dav_3510/kernel_reg_compute_vec_duplicate_impl.h#L251)、[无mask](impl/basic_api/reg_compute/dav_3510/kernel_reg_compute_vec_duplicate_impl.h#L79)。

## FP8测试：18组广播

每格给出对应实现位置，全部已核对公开声明与API页面中的dtype取值。返回值便利重载（存在时）最终转调相同vdup路径。

| 接口 | vdup参数差异 | fp8_e5m2_t | fp8_e4m3fn_t | fp8_e8m0_t |
| --- | --- | --- | --- | --- |
| `asc_duplicate` | `dst, src, mask, POS_LOWEST, MODE_ZEROING` | [通过 L212](impl/c_api/reg_base_impl/reg_broadcast_intf_impl.h#L212) | [通过 L205](impl/c_api/reg_base_impl/reg_broadcast_intf_impl.h#L205) | [通过 L219](impl/c_api/reg_base_impl/reg_broadcast_intf_impl.h#L219) |
| `asc_duplicate_merge` | `dst, src, mask, POS_LOWEST, MODE_MERGING` | [通过 L716](impl/c_api/reg_base_impl/reg_broadcast_intf_impl.h#L716) | [通过 L709](impl/c_api/reg_base_impl/reg_broadcast_intf_impl.h#L709) | [通过 L723](impl/c_api/reg_base_impl/reg_broadcast_intf_impl.h#L723) |
| `asc_duplicate_highest` | `dst, src, mask, POS_HIGHEST, MODE_ZEROING` | [通过 L632](impl/c_api/reg_base_impl/reg_broadcast_intf_impl.h#L632) | [通过 L625](impl/c_api/reg_base_impl/reg_broadcast_intf_impl.h#L625) | [通过 L639](impl/c_api/reg_base_impl/reg_broadcast_intf_impl.h#L639) |
| `asc_duplicate_highest_merge` | `dst, src, mask, POS_HIGHEST, MODE_MERGING` | [通过 L801](impl/c_api/reg_base_impl/reg_broadcast_intf_impl.h#L801) | [通过 L793](impl/c_api/reg_base_impl/reg_broadcast_intf_impl.h#L793) | [通过 L808](impl/c_api/reg_base_impl/reg_broadcast_intf_impl.h#L808) |
| `asc_duplicate_scalar` | `dst, value, mask, MODE_ZEROING` | [通过 L44](impl/c_api/reg_base_impl/reg_broadcast_intf_impl.h#L44) | [通过 L37](impl/c_api/reg_base_impl/reg_broadcast_intf_impl.h#L37) | [通过 L51](impl/c_api/reg_base_impl/reg_broadcast_intf_impl.h#L51) |
| `asc_duplicate_scalar_merge` | `dst, value, mask, MODE_MERGING` | [通过 L380](impl/c_api/reg_base_impl/reg_broadcast_intf_impl.h#L380) | [通过 L373](impl/c_api/reg_base_impl/reg_broadcast_intf_impl.h#L373) | [通过 L387](impl/c_api/reg_base_impl/reg_broadcast_intf_impl.h#L387) |

## 指令测试：表1与表3逐行对应

下面只列精度标准涉及的浮点类型。C侧整数重载确实存在时，不据此套用ULP或浮点符号位结论。常规算术/乘加/归约采用ZEROING；ExpSub和MulsCast按相同PART位置匹配。

| Basic API | C API | 适用数据类型或方向 | 两侧最终intrinsic | 证据（Basic / C） | 结果 |
| --- | --- | --- | --- | --- | --- |
| `Exp` | `asc_exp` | half/float | `vexp` | [Basic L286](impl/basic_api/reg_compute/dav_3510/kernel_reg_compute_vec_unary_impl.h#L286) / [asc_exp L551](impl/c_api/reg_base_impl/reg_arith_intf_impl.h#L551) | 一致 |
| `Sqrt` | `asc_sqrt` | half/float | `vsqrt` | [Basic L405](impl/basic_api/reg_compute/dav_3510/kernel_reg_compute_vec_unary_impl.h#L405) / [asc_sqrt L445](impl/c_api/reg_base_impl/reg_arith_intf_impl.h#L445) | 一致 |
| `Add` | `asc_add` | half/bfloat16_t/float | `vadd` | [Basic L134](impl/basic_api/reg_compute/dav_3510/kernel_reg_compute_vec_binary_impl.h#L134) / [asc_add L265](impl/c_api/reg_base_impl/reg_arith_intf_impl.h#L265) | 一致 |
| `Sub` | `asc_sub` | half/bfloat16_t/float | `vsub` | [Basic L254](impl/basic_api/reg_compute/dav_3510/kernel_reg_compute_vec_binary_impl.h#L254) / [asc_sub L51](impl/c_api/reg_base_impl/reg_arith_intf_impl.h#L51) | 一致 |
| `Adds` | `asc_add_scalar` | half/bfloat16_t/float | `vadds` | [Basic L116](impl/basic_api/reg_compute/dav_3510/kernel_reg_compute_vec_binary_scalar_impl.h#L116) / [asc_add_scalar L593](impl/c_api/reg_base_impl/reg_arith_intf_impl.h#L593) | 一致 |
| `Div` | `asc_div` | half/float | `vdiv` | [Basic L1539](impl/basic_api/reg_compute/dav_3510/kernel_reg_compute_vec_binary_impl.h#L1539) / [asc_div L629](impl/c_api/reg_base_impl/reg_arith_intf_impl.h#L629) | 一致 |
| `Neg` | `asc_neg` | half/float | `vneg` | [Basic L741](impl/basic_api/reg_compute/dav_3510/kernel_reg_compute_vec_unary_impl.h#L741) / [asc_neg L188](impl/c_api/reg_base_impl/reg_arith_intf_impl.h#L188) | 一致 |
| `Ln/Log` | `asc_ln` / `asc_log` | half/float | `vln` | [Basic L524](impl/basic_api/reg_compute/dav_3510/kernel_reg_compute_vec_unary_impl.h#L524) / [asc_ln L1338](impl/c_api/reg_base_impl/reg_arith_intf_impl.h#L1338)；[asc_log L1352](impl/c_api/reg_base_impl/reg_arith_intf_impl.h#L1352) | 一致 |
| `Mul` | `asc_mul` | half/bfloat16_t/float | `vmul` | [Basic L395](impl/basic_api/reg_compute/dav_3510/kernel_reg_compute_vec_binary_impl.h#L395) / [asc_mul L487](impl/c_api/reg_base_impl/reg_arith_intf_impl.h#L487) | 一致 |
| `Muls` | `asc_mul_scalar` | half/float | `vmuls` | [Basic L179](impl/basic_api/reg_compute/dav_3510/kernel_reg_compute_vec_binary_scalar_impl.h#L179) / [asc_mul_scalar L537](impl/c_api/reg_base_impl/reg_arith_intf_impl.h#L537) | 一致 |
| `AbsSub` | `asc_abs_sub` | half/float | `vabsdif` | [Basic L77](impl/basic_api/reg_compute/dav_3510/kernel_reg_compute_vec_fused_impl.h#L77) / [asc_abs_sub L48](impl/c_api/reg_base_impl/reg_fused_intf_impl.h#L48) | 一致 |
| `ExpSub` | `asc_exp_sub` / `asc_exp_sub_half2float` | float → float；half → float | `vexpdif` | [Basic L105](impl/basic_api/reg_compute/dav_3510/kernel_reg_compute_vec_fused_impl.h#L105) / [asc_exp_sub L144](impl/c_api/reg_base_impl/reg_fused_intf_impl.h#L144)；[asc_exp_sub_half2float L108](impl/c_api/reg_base_impl/reg_fused_intf_impl.h#L108) | 一致 |
| `Axpy` | `asc_axpy` | half/float | `vaxpy` | [Basic L46](impl/basic_api/reg_compute/dav_3510/kernel_reg_compute_vec_ternary_scalar_impl.h#L46) / [asc_axpy L243](impl/c_api/reg_base_impl/reg_fused_intf_impl.h#L243) | 一致 |
| `MulDstAdd` | `asc_madd` | half/bfloat16_t/float | `vmadd` | [Basic L117](impl/basic_api/reg_compute/dav_3510/kernel_reg_compute_vec_fused_impl.h#L117) / [asc_madd L186](impl/c_api/reg_base_impl/reg_fused_intf_impl.h#L186) | 一致 |
| `MulAddDst` | `asc_mula` | half/bfloat16_t/float | `vmula` | [Basic L2068](impl/basic_api/reg_compute/dav_3510/kernel_reg_compute_vec_binary_impl.h#L2068) / [asc_mula L297](impl/c_api/reg_base_impl/reg_fused_intf_impl.h#L297) | 一致 |
| `MulsCast` | `asc_mul_scalar_float2half_rn` | float×float → half | `vmulscvt` | [Basic L46](impl/basic_api/reg_compute/dav_3510/kernel_reg_compute_vec_fused_impl.h#L46) / [asc_mul_scalar_float2half_rn L90](impl/c_api/reg_base_impl/reg_fused_intf_impl.h#L90) | 一致 |
| `Reduce<SUM>` | `asc_reduce_sum` | half/float，同类型输入输出 | `vcadd` | [Basic L54](impl/basic_api/reg_compute/dav_3510/kernel_reg_compute_vec_reduce_impl.h#L54) / [asc_reduce_sum L233](impl/c_api/reg_base_impl/reg_reduce_intf_impl.h#L233) | 一致 |
| `ReduceDataBlock<SUM>` | `asc_reduce_sum_datablock` | half/float，同类型输入输出 | `vcgadd` | [Basic L280](impl/basic_api/reg_compute/dav_3510/kernel_reg_compute_vec_reduce_impl.h#L280) / [asc_reduce_sum_datablock L247](impl/c_api/reg_base_impl/reg_reduce_intf_impl.h#L247) | 一致 |
| `PairReduceElem<SUM>` | `asc_pair_reduce_sum` | half/float | `vcpadd` | [Basic L323](impl/basic_api/reg_compute/dav_3510/kernel_reg_compute_vec_reduce_impl.h#L323) / [asc_pair_reduce_sum L149](impl/c_api/reg_base_impl/reg_reduce_intf_impl.h#L149) | 一致 |

模式限制：Exp/Sqrt/Ln/Log/Div应选Basic API的硬件路径，例如PRECISION_1ULP_FTZ_TRUE；不能把其INTRINSIC/default路径在不同__ASC_FTZ__配置下的行为一概等同C API。相关分支见[Exp](impl/basic_api/reg_compute/dav_3510/kernel_reg_compute_vec_unary_impl.h#L250)、[Sqrt](impl/basic_api/reg_compute/dav_3510/kernel_reg_compute_vec_unary_impl.h#L373)、[Ln/Log](impl/basic_api/reg_compute/dav_3510/kernel_reg_compute_vec_unary_impl.h#L500)、[Div](impl/basic_api/reg_compute/dav_3510/kernel_reg_compute_vec_binary_impl.h#L1500)。

表1共20组，删除无直接对应C API的Log2/Log10组后为19组；表3恰为19组，无组级缺行。换底公式只说明可组合实现，不承诺组合后的Log2/Log10仍满足单条vln的1ulp精度。

## 语义测试与软仿测试

| 重点接口 | 核对结果 |
| --- | --- |
| MulDstAdd / asc_madd | dst_new = dst_old × src0 + src1；两侧vmadd，映射正确。 |
| MulAddDst / asc_mula | dst_new = src0 × src1 + dst_old；两侧vmula，映射正确，不能与madd互换。 |
| Axpy / asc_axpy | dst_new = src × scalar + dst_old；两侧vaxpy。 |
| Reduce<SUM> / asc_reduce_sum | 全寄存器参与mask的元素求和，浮点结果写入首元素；对应vcadd。 |
| ReduceDataBlock<SUM> / asc_reduce_sum_datablock | 分datablock求和；对应vcgadd，不是全寄存器求和。 |
| PairReduceElem<SUM> / asc_pair_reduce_sum | 相邻元素成对求和；对应vcpadd。 |
| ExpSub | float输入对应asc_exp_sub；half输入、float输出对应asc_exp_sub_half2float，按EVEN/ODD匹配Basic RegLayout ZERO/ONE。 |
| MulsCast | 只支持float寄存器乘float标量并转half；位置选择与舍入选择不是同一个参数，见C004。 |

乘加公式证据：[Basic MulDstAdd](docs/zh/api/SIMD-API/basic_api/reg_vector_compute/composite_compute/MulDstAdd.md#L35)、[C madd](docs/zh/api/SIMD-API/c_api/reg_compute/reg_fused/asc_madd.md#L34)、[Basic MulAddDst](docs/zh/api/SIMD-API/basic_api/reg_vector_compute/composite_compute/MulAddDst.md#L35)、[C mula](docs/zh/api/SIMD-API/c_api/reg_compute/reg_fused/asc_mula.md#L34)。归约SUM分派见[Basic归约分派实现](impl/basic_api/reg_compute/kernel_reg_compute_vec_reduce_intf_impl.h)。

软仿检查逐函数读取表3的21个不同C API名称对应函数体，并检查算法配置、if constexpr、循环、补偿辅助函数和AscendC转调。直接实现只有AIV执行条件及intrinsic调用；返回值封装只创建结果寄存器并转调同名C重载。无精度补偿分支。表3的19个软仿列均为“-”，结论正确。此结论不扩展到其他未列出的C API。

## 文档测试

| 检查项 | 结果与证据 |
| --- | --- |
| git diff --check | 退出码0，无输出。 |
| git diff --cached --check | 退出码0，无输出，覆盖已有暂存改动。 |
| 表格列数 | 表1/表3均4列，表2/表4均2列；逐行一致。 |
| 数据行数 | 表1为20行，表2为9行，表3为19行，表4为9行；不计表头和分隔行。 |
| 标题 | 唯一一级标题；表1—表4编号连续，无重复表号。 |
| 锚点 | reg-vector-precision-summary、table-precision-standard、table-low-precision、table-c-api-precision-standard、table-c-api-low-precision，共5个，全部唯一且有含义。 |
| 链接 | 唯一页内链接#table-precision-standard可解析到本页实际锚点。 |
| 表格换行 | 使用br，未发现HTML表格或列拆分问题。 |
| 内容完整性 | 结构行数通过不代表内容通过；C001、C002、C003分别涉及真实名称、支持范围完整性、Basic基线准确性。 |
| 中文标点 | 第24、74行有英文逗号，见C007。 |
| API专属章节/头文件/示例 | 不适用：目标是附录汇总，不是单API参考页，不要求补齐完整原型及调用示例章节。 |
| 页面渲染 | 未执行浏览器渲染，以上仅为Markdown源码结构检查。 |

## 表4全部98个类型—接口组合证据

“通过”表示该类型的同名Reg接口存在、对应文档允许该dtype、声明和实现的类型/位置标签匹配。“族名需改”表示须按C001替换名称，表内证据指向真实带后缀的代表重载。转换实现最终调用vcvt或明确的vcvt_s42*/vcvt_f162s4，与Basic Cast的同类型、同舍入/饱和/布局分支一致。

Basic转换证据：[扩展转换](impl/basic_api/reg_compute/dav_3510/kernel_reg_compute_vec_vconv_impl.h#L37)、[整数窄化](impl/basic_api/reg_compute/dav_3510/kernel_reg_compute_vec_vconv_impl.h#L108)、[浮点窄化](impl/basic_api/reg_compute/dav_3510/kernel_reg_compute_vec_vconv_impl.h#L154)、[FP4窄化](impl/basic_api/reg_compute/dav_3510/kernel_reg_compute_vec_vconv_impl.h#L270)。Int8/UInt8算术对应vadd/vsub/vmax/vmin/vabs/vneg/vadds/vmaxs/vmins，比较对应vcmp_*和vcmps_*；Basic对应实现见[二元算术](impl/basic_api/reg_compute/dav_3510/kernel_reg_compute_vec_binary_impl.h#L134)、[一元算术](impl/basic_api/reg_compute/dav_3510/kernel_reg_compute_vec_unary_impl.h#L123)、[标量算术](impl/basic_api/reg_compute/dav_3510/kernel_reg_compute_vec_binary_scalar_impl.h#L116)、[矢量比较](impl/basic_api/reg_compute/dav_3510/kernel_reg_compute_vec_cmpsel_impl.h#L137)、[标量比较](impl/basic_api/reg_compute/dav_3510/kernel_reg_compute_vec_cmpsel_impl.h#L178)。

| # | 数据类型 | 表4名称 | 结果 | 声明 | 实现及最终指令 | 文档 |
| --- | --- | --- | --- | --- | --- | --- |
| 1 | fp4x2_e2m1_t | `asc_bfloat162e2m1x2` | 族名需改 | [声明 L906](include/c_api/reg_compute/reg_convert.h#L906) | [vcvt L3528](impl/c_api/reg_base_impl/reg_convert_intf_impl.h#L3528) | [文档](docs/zh/api/SIMD-API/c_api/reg_compute/reg_convert/asc_bfloat162e2m1x2.md) |
| 2 | fp4x2_e2m1_t | `asc_e2m1x22bfloat16` | 通过 | [声明 L125](include/c_api/reg_compute/reg_convert.h#L125) | [vcvt L4269](impl/c_api/reg_base_impl/reg_convert_intf_impl.h#L4269) | [文档](docs/zh/api/SIMD-API/c_api/reg_compute/reg_convert/asc_e2m1x22bfloat16.md) |
| 3 | fp4x2_e1m2_t | `asc_bfloat162e1m2x2` | 族名需改 | [声明 L846](include/c_api/reg_compute/reg_convert.h#L846) | [vcvt L5875](impl/c_api/reg_base_impl/reg_convert_intf_impl.h#L5875) | [文档](docs/zh/api/SIMD-API/c_api/reg_compute/reg_convert/asc_bfloat162e1m2x2.md) |
| 4 | fp4x2_e1m2_t | `asc_e1m2x22bfloat16` | 通过 | [声明 L253](include/c_api/reg_compute/reg_convert.h#L253) | [vcvt L6275](impl/c_api/reg_base_impl/reg_convert_intf_impl.h#L6275) | [文档](docs/zh/api/SIMD-API/c_api/reg_compute/reg_convert/asc_e1m2x22bfloat16.md) |
| 5 | fp8_e5m2_t | `asc_float2e5m2` | 族名需改 | [声明 L299](include/c_api/reg_compute/reg_convert.h#L299) | [vcvt L608](impl/c_api/reg_base_impl/reg_convert_intf_impl.h#L608) | [文档](docs/zh/api/SIMD-API/c_api/reg_compute/reg_convert/asc_float2e5m2.md) |
| 6 | fp8_e5m2_t | `asc_e5m22float` | 通过 | [声明 L279](include/c_api/reg_compute/reg_convert.h#L279) | [vcvt L5431](impl/c_api/reg_base_impl/reg_convert_intf_impl.h#L5431) | [文档](docs/zh/api/SIMD-API/c_api/reg_compute/reg_convert/asc_e5m22float.md) |
| 7 | fp8_e5m2_t | `asc_duplicate` | 通过 | [声明 L154](include/c_api/reg_compute/compute/reg_broadcast.h#L154) | [vdup L212](impl/c_api/reg_base_impl/reg_broadcast_intf_impl.h#L212) | [文档](docs/zh/api/SIMD-API/c_api/reg_compute/reg_broadcast/asc_duplicate.md) |
| 8 | fp8_e5m2_t | `asc_duplicate_merge` | 通过 | [声明 L226](include/c_api/reg_compute/compute/reg_broadcast.h#L226) | [vdup L716](impl/c_api/reg_base_impl/reg_broadcast_intf_impl.h#L716) | [文档](docs/zh/api/SIMD-API/c_api/reg_compute/reg_broadcast/asc_duplicate_merge.md) |
| 9 | fp8_e5m2_t | `asc_duplicate_highest` | 通过 | [声明 L202](include/c_api/reg_compute/compute/reg_broadcast.h#L202) | [vdup L632](impl/c_api/reg_base_impl/reg_broadcast_intf_impl.h#L632) | [文档](docs/zh/api/SIMD-API/c_api/reg_compute/reg_broadcast/asc_duplicate_highest.md) |
| 10 | fp8_e5m2_t | `asc_duplicate_highest_merge` | 通过 | [声明 L251](include/c_api/reg_compute/compute/reg_broadcast.h#L251) | [vdup L801](impl/c_api/reg_base_impl/reg_broadcast_intf_impl.h#L801) | [文档](docs/zh/api/SIMD-API/c_api/reg_compute/reg_broadcast/asc_duplicate_highest_merge.md) |
| 11 | fp8_e5m2_t | `asc_duplicate_scalar` | 通过 | [声明 L34](include/c_api/reg_compute/compute/reg_broadcast.h#L34) | [vdup L44](impl/c_api/reg_base_impl/reg_broadcast_intf_impl.h#L44) | [文档](docs/zh/api/SIMD-API/c_api/reg_compute/reg_broadcast/asc_duplicate_scalar.md) |
| 12 | fp8_e5m2_t | `asc_duplicate_scalar_merge` | 通过 | [声明 L82](include/c_api/reg_compute/compute/reg_broadcast.h#L82) | [vdup L380](impl/c_api/reg_base_impl/reg_broadcast_intf_impl.h#L380) | [文档](docs/zh/api/SIMD-API/c_api/reg_compute/reg_broadcast/asc_duplicate_scalar_merge.md) |
| 13 | fp8_e4m3fn_t | `asc_float2e4m3` | 族名需改 | [声明 L29](include/c_api/reg_compute/reg_convert.h#L29) | [vcvt L5511](impl/c_api/reg_base_impl/reg_convert_intf_impl.h#L5511) | [文档](docs/zh/api/SIMD-API/c_api/reg_compute/reg_convert/asc_float2e4m3.md) |
| 14 | fp8_e4m3fn_t | `asc_e4m32float` | 通过 | [声明 L410](include/c_api/reg_compute/reg_convert.h#L410) | [vcvt L802](impl/c_api/reg_base_impl/reg_convert_intf_impl.h#L802) | [文档](docs/zh/api/SIMD-API/c_api/reg_compute/reg_convert/asc_e4m32float.md) |
| 15 | fp8_e4m3fn_t | `asc_duplicate` | 通过 | [声明 L152](include/c_api/reg_compute/compute/reg_broadcast.h#L152) | [vdup L205](impl/c_api/reg_base_impl/reg_broadcast_intf_impl.h#L205) | [文档](docs/zh/api/SIMD-API/c_api/reg_compute/reg_broadcast/asc_duplicate.md) |
| 16 | fp8_e4m3fn_t | `asc_duplicate_merge` | 通过 | [声明 L224](include/c_api/reg_compute/compute/reg_broadcast.h#L224) | [vdup L709](impl/c_api/reg_base_impl/reg_broadcast_intf_impl.h#L709) | [文档](docs/zh/api/SIMD-API/c_api/reg_compute/reg_broadcast/asc_duplicate_merge.md) |
| 17 | fp8_e4m3fn_t | `asc_duplicate_highest` | 通过 | [声明 L200](include/c_api/reg_compute/compute/reg_broadcast.h#L200) | [vdup L625](impl/c_api/reg_base_impl/reg_broadcast_intf_impl.h#L625) | [文档](docs/zh/api/SIMD-API/c_api/reg_compute/reg_broadcast/asc_duplicate_highest.md) |
| 18 | fp8_e4m3fn_t | `asc_duplicate_highest_merge` | 通过 | [声明 L248](include/c_api/reg_compute/compute/reg_broadcast.h#L248) | [vdup L793](impl/c_api/reg_base_impl/reg_broadcast_intf_impl.h#L793) | [文档](docs/zh/api/SIMD-API/c_api/reg_compute/reg_broadcast/asc_duplicate_highest_merge.md) |
| 19 | fp8_e4m3fn_t | `asc_duplicate_scalar` | 通过 | [声明 L32](include/c_api/reg_compute/compute/reg_broadcast.h#L32) | [vdup L37](impl/c_api/reg_base_impl/reg_broadcast_intf_impl.h#L37) | [文档](docs/zh/api/SIMD-API/c_api/reg_compute/reg_broadcast/asc_duplicate_scalar.md) |
| 20 | fp8_e4m3fn_t | `asc_duplicate_scalar_merge` | 通过 | [声明 L80](include/c_api/reg_compute/compute/reg_broadcast.h#L80) | [vdup L373](impl/c_api/reg_base_impl/reg_broadcast_intf_impl.h#L373) | [文档](docs/zh/api/SIMD-API/c_api/reg_compute/reg_broadcast/asc_duplicate_scalar_merge.md) |
| 21 | fp8_e8m0_t | `asc_duplicate` | 通过 | [声明 L156](include/c_api/reg_compute/compute/reg_broadcast.h#L156) | [vdup L219](impl/c_api/reg_base_impl/reg_broadcast_intf_impl.h#L219) | [文档](docs/zh/api/SIMD-API/c_api/reg_compute/reg_broadcast/asc_duplicate.md) |
| 22 | fp8_e8m0_t | `asc_duplicate_merge` | 通过 | [声明 L228](include/c_api/reg_compute/compute/reg_broadcast.h#L228) | [vdup L723](impl/c_api/reg_base_impl/reg_broadcast_intf_impl.h#L723) | [文档](docs/zh/api/SIMD-API/c_api/reg_compute/reg_broadcast/asc_duplicate_merge.md) |
| 23 | fp8_e8m0_t | `asc_duplicate_highest` | 通过 | [声明 L204](include/c_api/reg_compute/compute/reg_broadcast.h#L204) | [vdup L639](impl/c_api/reg_base_impl/reg_broadcast_intf_impl.h#L639) | [文档](docs/zh/api/SIMD-API/c_api/reg_compute/reg_broadcast/asc_duplicate_highest.md) |
| 24 | fp8_e8m0_t | `asc_duplicate_highest_merge` | 通过 | [声明 L254](include/c_api/reg_compute/compute/reg_broadcast.h#L254) | [vdup L808](impl/c_api/reg_base_impl/reg_broadcast_intf_impl.h#L808) | [文档](docs/zh/api/SIMD-API/c_api/reg_compute/reg_broadcast/asc_duplicate_highest_merge.md) |
| 25 | fp8_e8m0_t | `asc_duplicate_scalar` | 通过 | [声明 L36](include/c_api/reg_compute/compute/reg_broadcast.h#L36) | [vdup L51](impl/c_api/reg_base_impl/reg_broadcast_intf_impl.h#L51) | [文档](docs/zh/api/SIMD-API/c_api/reg_compute/reg_broadcast/asc_duplicate_scalar.md) |
| 26 | fp8_e8m0_t | `asc_duplicate_scalar_merge` | 通过 | [声明 L84](include/c_api/reg_compute/compute/reg_broadcast.h#L84) | [vdup L387](impl/c_api/reg_base_impl/reg_broadcast_intf_impl.h#L387) | [文档](docs/zh/api/SIMD-API/c_api/reg_compute/reg_broadcast/asc_duplicate_scalar_merge.md) |
| 27 | hifloat8_t | `asc_float2hif8` | 族名需改 | [声明 L602](include/c_api/reg_compute/reg_convert.h#L602) | [vcvt L3928](impl/c_api/reg_base_impl/reg_convert_intf_impl.h#L3928) | [文档](docs/zh/api/SIMD-API/c_api/reg_compute/reg_convert/asc_float2hif8.md) |
| 28 | hifloat8_t | `asc_half2hif8` | 族名需改 | [声明 L4018](include/c_api/reg_compute/reg_convert.h#L4018) | [vcvt L1178](impl/c_api/reg_base_impl/reg_convert_intf_impl.h#L1178) | [文档](docs/zh/api/SIMD-API/c_api/reg_compute/reg_convert/asc_half2hif8.md) |
| 29 | hifloat8_t | `asc_hif82float` | 通过 | [声明 L642](include/c_api/reg_compute/reg_convert.h#L642) | [vcvt L1756](impl/c_api/reg_base_impl/reg_convert_intf_impl.h#L1756) | [文档](docs/zh/api/SIMD-API/c_api/reg_compute/reg_convert/asc_hif82float.md) |
| 30 | hifloat8_t | `asc_hif82half` | 通过 | [声明 L391](include/c_api/reg_compute/reg_convert.h#L391) | [vcvt L1836](impl/c_api/reg_base_impl/reg_convert_intf_impl.h#L1836) | [文档](docs/zh/api/SIMD-API/c_api/reg_compute/reg_convert/asc_hif82half.md) |
| 31 | int4x2_t | `asc_half2int4x2` | 族名需改 | [声明 L5430](include/c_api/reg_compute/reg_convert.h#L5430) | [vcvt_f162s4 L7092](impl/c_api/reg_base_impl/reg_convert_intf_impl.h#L7092) | [文档](docs/zh/api/SIMD-API/c_api/reg_compute/reg_convert/asc_half2int4x2.md) |
| 32 | int4x2_t | `asc_int4x22bfloat16` | 通过 | [声明 L5410](include/c_api/reg_compute/reg_convert.h#L5410) | [vcvt_s42bf16 L7452](impl/c_api/reg_base_impl/reg_convert_intf_impl.h#L7452) | [文档](docs/zh/api/SIMD-API/c_api/reg_compute/reg_convert/asc_int4x22bfloat16.md) |
| 33 | int4x2_t | `asc_int4x22half` | 通过 | [声明 L5390](include/c_api/reg_compute/reg_convert.h#L5390) | [vcvt_s42f16 L7488](impl/c_api/reg_base_impl/reg_convert_intf_impl.h#L7488) | [文档](docs/zh/api/SIMD-API/c_api/reg_compute/reg_convert/asc_int4x22half.md) |
| 34 | int4x2_t | `asc_int4x22int16` | 通过 | [声明 L5370](include/c_api/reg_compute/reg_convert.h#L5370) | [vcvt_s42s16 L7524](impl/c_api/reg_base_impl/reg_convert_intf_impl.h#L7524) | [文档](docs/zh/api/SIMD-API/c_api/reg_compute/reg_convert/asc_int4x22int16.md) |
| 35 | int8_t | `asc_half2int8` | 族名需改 | [声明 L3700](include/c_api/reg_compute/reg_convert.h#L3700) | [vcvt L1314](impl/c_api/reg_base_impl/reg_convert_intf_impl.h#L1314) | [文档](docs/zh/api/SIMD-API/c_api/reg_compute/reg_convert/asc_half2int8.md) |
| 36 | int8_t | `asc_int82half` | 通过 | [声明 L87](include/c_api/reg_compute/reg_convert.h#L87) | [vcvt L2960](impl/c_api/reg_base_impl/reg_convert_intf_impl.h#L2960) | [文档](docs/zh/api/SIMD-API/c_api/reg_compute/reg_convert/asc_int82half.md) |
| 37 | int8_t | `asc_int82int16` | 通过 | [声明 L163](include/c_api/reg_compute/reg_convert.h#L163) | [vcvt L2278](impl/c_api/reg_base_impl/reg_convert_intf_impl.h#L2278) | [文档](docs/zh/api/SIMD-API/c_api/reg_compute/reg_convert/asc_int82int16.md) |
| 38 | int8_t | `asc_int82int32` | 通过 | [声明 L105](include/c_api/reg_compute/reg_convert.h#L105) | [vcvt L6355](impl/c_api/reg_base_impl/reg_convert_intf_impl.h#L6355) | [文档](docs/zh/api/SIMD-API/c_api/reg_compute/reg_convert/asc_int82int32.md) |
| 39 | int8_t | `asc_add` | 通过 | [声明 L213](include/c_api/reg_compute/compute/reg_arith.h#L213) | [vadd L244](impl/c_api/reg_base_impl/reg_arith_intf_impl.h#L244) | [文档](docs/zh/api/SIMD-API/c_api/reg_compute/reg_arith/asc_add.md) |
| 40 | int8_t | `asc_sub` | 通过 | [声明 L30](include/c_api/reg_compute/compute/reg_arith.h#L30) | [vsub L30](impl/c_api/reg_base_impl/reg_arith_intf_impl.h#L30) | [文档](docs/zh/api/SIMD-API/c_api/reg_compute/reg_arith/asc_sub.md) |
| 41 | int8_t | `asc_max` | 通过 | [声明 L276](include/c_api/reg_compute/compute/reg_arith.h#L276) | [vmax L324](impl/c_api/reg_base_impl/reg_arith_intf_impl.h#L324) | [文档](docs/zh/api/SIMD-API/c_api/reg_compute/reg_arith/asc_max.md) |
| 42 | int8_t | `asc_min` | 通过 | [声明 L79](include/c_api/reg_compute/compute/reg_arith.h#L79) | [vmin L103](impl/c_api/reg_base_impl/reg_arith_intf_impl.h#L103) | [文档](docs/zh/api/SIMD-API/c_api/reg_compute/reg_arith/asc_min.md) |
| 43 | int8_t | `asc_abs` | 通过 | [声明 L136](include/c_api/reg_compute/compute/reg_arith.h#L136) | [vabs L202](impl/c_api/reg_base_impl/reg_arith_intf_impl.h#L202) | [文档](docs/zh/api/SIMD-API/c_api/reg_compute/reg_arith/asc_abs.md) |
| 44 | int8_t | `asc_neg` | 通过 | [声明 L116](include/c_api/reg_compute/compute/reg_arith.h#L116) | [vneg L167](impl/c_api/reg_base_impl/reg_arith_intf_impl.h#L167) | [文档](docs/zh/api/SIMD-API/c_api/reg_compute/reg_arith/asc_neg.md) |
| 45 | int8_t | `asc_add_scalar` | 通过 | [声明 L356](include/c_api/reg_compute/compute/reg_arith.h#L356) | [vadds L565](impl/c_api/reg_base_impl/reg_arith_intf_impl.h#L565) | [文档](docs/zh/api/SIMD-API/c_api/reg_compute/reg_arith/asc_add_scalar.md) |
| 46 | int8_t | `asc_max_scalar` | 通过 | [声明 L425](include/c_api/reg_compute/compute/reg_arith.h#L425) | [vmaxs L671](impl/c_api/reg_base_impl/reg_arith_intf_impl.h#L671) | [文档](docs/zh/api/SIMD-API/c_api/reg_compute/reg_arith/asc_max_scalar.md) |
| 47 | int8_t | `asc_min_scalar` | 通过 | [声明 L319](include/c_api/reg_compute/compute/reg_arith.h#L319) | [vmins L409](impl/c_api/reg_base_impl/reg_arith_intf_impl.h#L409) | [文档](docs/zh/api/SIMD-API/c_api/reg_compute/reg_arith/asc_min_scalar.md) |
| 48 | int8_t | `asc_eq` | 通过 | [声明 L172](include/c_api/reg_compute/compute/reg_compare.h#L172) | [vcmp_eq L534](impl/c_api/reg_base_impl/reg_compare_intf_impl.h#L534) | [文档](docs/zh/api/SIMD-API/c_api/reg_compute/reg_compare/asc_eq.md) |
| 49 | int8_t | `asc_ne` | 通过 | [声明 L392](include/c_api/reg_compute/compute/reg_compare.h#L392) | [vcmp_ne L660](impl/c_api/reg_base_impl/reg_compare_intf_impl.h#L660) | [文档](docs/zh/api/SIMD-API/c_api/reg_compute/reg_compare/asc_ne.md) |
| 50 | int8_t | `asc_lt` | 通过 | [声明 L248](include/c_api/reg_compute/compute/reg_compare.h#L248) | [vcmp_lt L282](impl/c_api/reg_base_impl/reg_compare_intf_impl.h#L282) | [文档](docs/zh/api/SIMD-API/c_api/reg_compute/reg_compare/asc_lt.md) |
| 51 | int8_t | `asc_le` | 通过 | [声明 L104](include/c_api/reg_compute/compute/reg_compare.h#L104) | [vcmp_le L156](impl/c_api/reg_base_impl/reg_compare_intf_impl.h#L156) | [文档](docs/zh/api/SIMD-API/c_api/reg_compute/reg_compare/asc_le.md) |
| 52 | int8_t | `asc_gt` | 通过 | [声明 L320](include/c_api/reg_compute/compute/reg_compare.h#L320) | [vcmp_gt L408](impl/c_api/reg_base_impl/reg_compare_intf_impl.h#L408) | [文档](docs/zh/api/SIMD-API/c_api/reg_compute/reg_compare/asc_gt.md) |
| 53 | int8_t | `asc_ge` | 通过 | [声明 L28](include/c_api/reg_compute/compute/reg_compare.h#L28) | [vcmp_ge L23](impl/c_api/reg_base_impl/reg_compare_intf_impl.h#L23) | [文档](docs/zh/api/SIMD-API/c_api/reg_compute/reg_compare/asc_ge.md) |
| 54 | int8_t | `asc_eq_scalar` | 通过 | [声明 L208](include/c_api/reg_compute/compute/reg_compare.h#L208) | [vcmps_eq L597](impl/c_api/reg_base_impl/reg_compare_intf_impl.h#L597) | [文档](docs/zh/api/SIMD-API/c_api/reg_compute/reg_compare/asc_eq_scalar.md) |
| 55 | int8_t | `asc_ne_scalar` | 通过 | [声明 L426](include/c_api/reg_compute/compute/reg_compare.h#L426) | [vcmps_ne L723](impl/c_api/reg_base_impl/reg_compare_intf_impl.h#L723) | [文档](docs/zh/api/SIMD-API/c_api/reg_compute/reg_compare/asc_ne_scalar.md) |
| 56 | int8_t | `asc_lt_scalar` | 通过 | [声明 L284](include/c_api/reg_compute/compute/reg_compare.h#L284) | [vcmps_lt L345](impl/c_api/reg_base_impl/reg_compare_intf_impl.h#L345) | [文档](docs/zh/api/SIMD-API/c_api/reg_compute/reg_compare/asc_lt_scalar.md) |
| 57 | int8_t | `asc_le_scalar` | 通过 | [声明 L140](include/c_api/reg_compute/compute/reg_compare.h#L140) | [vcmps_le L219](impl/c_api/reg_base_impl/reg_compare_intf_impl.h#L219) | [文档](docs/zh/api/SIMD-API/c_api/reg_compute/reg_compare/asc_le_scalar.md) |
| 58 | int8_t | `asc_gt_scalar` | 通过 | [声明 L354](include/c_api/reg_compute/compute/reg_compare.h#L354) | [vcmps_gt L471](impl/c_api/reg_base_impl/reg_compare_intf_impl.h#L471) | [文档](docs/zh/api/SIMD-API/c_api/reg_compute/reg_compare/asc_gt_scalar.md) |
| 59 | int8_t | `asc_ge_scalar` | 通过 | [声明 L64](include/c_api/reg_compute/compute/reg_compare.h#L64) | [vcmps_ge L86](impl/c_api/reg_base_impl/reg_compare_intf_impl.h#L86) | [文档](docs/zh/api/SIMD-API/c_api/reg_compute/reg_compare/asc_ge_scalar.md) |
| 60 | int8_t | `asc_duplicate` | 通过 | [声明 L150](include/c_api/reg_compute/compute/reg_broadcast.h#L150) | [vdup L198](impl/c_api/reg_base_impl/reg_broadcast_intf_impl.h#L198) | [文档](docs/zh/api/SIMD-API/c_api/reg_compute/reg_broadcast/asc_duplicate.md) |
| 61 | int8_t | `asc_duplicate_merge` | 通过 | [声明 L222](include/c_api/reg_compute/compute/reg_broadcast.h#L222) | [vdup L702](impl/c_api/reg_base_impl/reg_broadcast_intf_impl.h#L702) | [文档](docs/zh/api/SIMD-API/c_api/reg_compute/reg_broadcast/asc_duplicate_merge.md) |
| 62 | int8_t | `asc_duplicate_highest` | 通过 | [声明 L198](include/c_api/reg_compute/compute/reg_broadcast.h#L198) | [vdup L618](impl/c_api/reg_base_impl/reg_broadcast_intf_impl.h#L618) | [文档](docs/zh/api/SIMD-API/c_api/reg_compute/reg_broadcast/asc_duplicate_highest.md) |
| 63 | int8_t | `asc_duplicate_highest_merge` | 通过 | [声明 L246](include/c_api/reg_compute/compute/reg_broadcast.h#L246) | [vdup L786](impl/c_api/reg_base_impl/reg_broadcast_intf_impl.h#L786) | [文档](docs/zh/api/SIMD-API/c_api/reg_compute/reg_broadcast/asc_duplicate_highest_merge.md) |
| 64 | int8_t | `asc_duplicate_scalar` | 通过 | [声明 L30](include/c_api/reg_compute/compute/reg_broadcast.h#L30) | [vdup L30](impl/c_api/reg_base_impl/reg_broadcast_intf_impl.h#L30) | [文档](docs/zh/api/SIMD-API/c_api/reg_compute/reg_broadcast/asc_duplicate_scalar.md) |
| 65 | int8_t | `asc_duplicate_scalar_merge` | 通过 | [声明 L78](include/c_api/reg_compute/compute/reg_broadcast.h#L78) | [vdup L366](impl/c_api/reg_base_impl/reg_broadcast_intf_impl.h#L366) | [文档](docs/zh/api/SIMD-API/c_api/reg_compute/reg_broadcast/asc_duplicate_scalar_merge.md) |
| 66 | uint8_t | `asc_half2uint8` | 族名需改 | [声明 L5229](include/c_api/reg_compute/reg_convert.h#L5229) | [vcvt L1938](impl/c_api/reg_base_impl/reg_convert_intf_impl.h#L1938) | [文档](docs/zh/api/SIMD-API/c_api/reg_compute/reg_convert/asc_half2uint8.md) |
| 67 | uint8_t | `asc_int162uint8` | 通过 | [声明 L241](include/c_api/reg_compute/reg_convert.h#L241) | [vcvt L2312](impl/c_api/reg_base_impl/reg_convert_intf_impl.h#L2312) | [文档](docs/zh/api/SIMD-API/c_api/reg_compute/reg_convert/asc_int162uint8.md) |
| 68 | uint8_t | `asc_int322uint8` | 通过 | [声明 L189](include/c_api/reg_compute/reg_convert.h#L189) | [vcvt L984](impl/c_api/reg_base_impl/reg_convert_intf_impl.h#L984) | [文档](docs/zh/api/SIMD-API/c_api/reg_compute/reg_convert/asc_int322uint8.md) |
| 69 | uint8_t | `asc_uint162uint8` | 通过 | [声明 L93](include/c_api/reg_compute/reg_convert.h#L93) | [vcvt L1688](impl/c_api/reg_base_impl/reg_convert_intf_impl.h#L1688) | [文档](docs/zh/api/SIMD-API/c_api/reg_compute/reg_convert/asc_uint162uint8.md) |
| 70 | uint8_t | `asc_uint322uint8` | 通过 | [声明 L351](include/c_api/reg_compute/reg_convert.h#L351) | [vcvt L3028](impl/c_api/reg_base_impl/reg_convert_intf_impl.h#L3028) | [文档](docs/zh/api/SIMD-API/c_api/reg_compute/reg_convert/asc_uint322uint8.md) |
| 71 | uint8_t | `asc_uint82half` | 通过 | [声明 L794](include/c_api/reg_compute/reg_convert.h#L794) | [vcvt L1870](impl/c_api/reg_base_impl/reg_convert_intf_impl.h#L1870) | [文档](docs/zh/api/SIMD-API/c_api/reg_compute/reg_convert/asc_uint82half.md) |
| 72 | uint8_t | `asc_uint82uint16` | 通过 | [声明 L273](include/c_api/reg_compute/reg_convert.h#L273) | [vcvt L5841](impl/c_api/reg_base_impl/reg_convert_intf_impl.h#L5841) | [文档](docs/zh/api/SIMD-API/c_api/reg_compute/reg_convert/asc_uint82uint16.md) |
| 73 | uint8_t | `asc_uint82uint32` | 通过 | [声明 L169](include/c_api/reg_compute/reg_convert.h#L169) | [vcvt L4433](impl/c_api/reg_base_impl/reg_convert_intf_impl.h#L4433) | [文档](docs/zh/api/SIMD-API/c_api/reg_compute/reg_convert/asc_uint82uint32.md) |
| 74 | uint8_t | `asc_add` | 通过 | [声明 L211](include/c_api/reg_compute/compute/reg_arith.h#L211) | [vadd L237](impl/c_api/reg_base_impl/reg_arith_intf_impl.h#L237) | [文档](docs/zh/api/SIMD-API/c_api/reg_compute/reg_arith/asc_add.md) |
| 75 | uint8_t | `asc_sub` | 通过 | [声明 L28](include/c_api/reg_compute/compute/reg_arith.h#L28) | [vsub L23](impl/c_api/reg_base_impl/reg_arith_intf_impl.h#L23) | [文档](docs/zh/api/SIMD-API/c_api/reg_compute/reg_arith/asc_sub.md) |
| 76 | uint8_t | `asc_max` | 通过 | [声明 L274](include/c_api/reg_compute/compute/reg_arith.h#L274) | [vmax L317](impl/c_api/reg_base_impl/reg_arith_intf_impl.h#L317) | [文档](docs/zh/api/SIMD-API/c_api/reg_compute/reg_arith/asc_max.md) |
| 77 | uint8_t | `asc_min` | 通过 | [声明 L85](include/c_api/reg_compute/compute/reg_arith.h#L85) | [vmin L124](impl/c_api/reg_base_impl/reg_arith_intf_impl.h#L124) | [文档](docs/zh/api/SIMD-API/c_api/reg_compute/reg_arith/asc_min.md) |
| 78 | uint8_t | `asc_add_scalar` | 通过 | [声明 L358](include/c_api/reg_compute/compute/reg_arith.h#L358) | [vadds L572](impl/c_api/reg_base_impl/reg_arith_intf_impl.h#L572) | [文档](docs/zh/api/SIMD-API/c_api/reg_compute/reg_arith/asc_add_scalar.md) |
| 79 | uint8_t | `asc_max_scalar` | 通过 | [声明 L431](include/c_api/reg_compute/compute/reg_arith.h#L431) | [vmaxs L692](impl/c_api/reg_base_impl/reg_arith_intf_impl.h#L692) | [文档](docs/zh/api/SIMD-API/c_api/reg_compute/reg_arith/asc_max_scalar.md) |
| 80 | uint8_t | `asc_min_scalar` | 通过 | [声明 L321](include/c_api/reg_compute/compute/reg_arith.h#L321) | [vmins L416](impl/c_api/reg_base_impl/reg_arith_intf_impl.h#L416) | [文档](docs/zh/api/SIMD-API/c_api/reg_compute/reg_arith/asc_min_scalar.md) |
| 81 | uint8_t | `asc_eq` | 通过 | [声明 L176](include/c_api/reg_compute/compute/reg_compare.h#L176) | [vcmp_eq L527](impl/c_api/reg_base_impl/reg_compare_intf_impl.h#L527) | [文档](docs/zh/api/SIMD-API/c_api/reg_compute/reg_compare/asc_eq.md) |
| 82 | uint8_t | `asc_ne` | 通过 | [声明 L388](include/c_api/reg_compute/compute/reg_compare.h#L388) | [vcmp_ne L653](impl/c_api/reg_base_impl/reg_compare_intf_impl.h#L653) | [文档](docs/zh/api/SIMD-API/c_api/reg_compute/reg_compare/asc_ne.md) |
| 83 | uint8_t | `asc_lt` | 通过 | [声明 L244](include/c_api/reg_compute/compute/reg_compare.h#L244) | [vcmp_lt L275](impl/c_api/reg_base_impl/reg_compare_intf_impl.h#L275) | [文档](docs/zh/api/SIMD-API/c_api/reg_compute/reg_compare/asc_lt.md) |
| 84 | uint8_t | `asc_le` | 通过 | [声明 L100](include/c_api/reg_compute/compute/reg_compare.h#L100) | [vcmp_le L149](impl/c_api/reg_base_impl/reg_compare_intf_impl.h#L149) | [文档](docs/zh/api/SIMD-API/c_api/reg_compute/reg_compare/asc_le.md) |
| 85 | uint8_t | `asc_gt` | 通过 | [声明 L316](include/c_api/reg_compute/compute/reg_compare.h#L316) | [vcmp_gt L401](impl/c_api/reg_base_impl/reg_compare_intf_impl.h#L401) | [文档](docs/zh/api/SIMD-API/c_api/reg_compute/reg_compare/asc_gt.md) |
| 86 | uint8_t | `asc_ge` | 通过 | [声明 L40](include/c_api/reg_compute/compute/reg_compare.h#L40) | [vcmp_ge L44](impl/c_api/reg_base_impl/reg_compare_intf_impl.h#L44) | [文档](docs/zh/api/SIMD-API/c_api/reg_compute/reg_compare/asc_ge.md) |
| 87 | uint8_t | `asc_eq_scalar` | 通过 | [声明 L212](include/c_api/reg_compute/compute/reg_compare.h#L212) | [vcmps_eq L590](impl/c_api/reg_base_impl/reg_compare_intf_impl.h#L590) | [文档](docs/zh/api/SIMD-API/c_api/reg_compute/reg_compare/asc_eq_scalar.md) |
| 88 | uint8_t | `asc_ne_scalar` | 通过 | [声明 L424](include/c_api/reg_compute/compute/reg_compare.h#L424) | [vcmps_ne L716](impl/c_api/reg_base_impl/reg_compare_intf_impl.h#L716) | [文档](docs/zh/api/SIMD-API/c_api/reg_compute/reg_compare/asc_ne_scalar.md) |
| 89 | uint8_t | `asc_lt_scalar` | 通过 | [声明 L280](include/c_api/reg_compute/compute/reg_compare.h#L280) | [vcmps_lt L338](impl/c_api/reg_base_impl/reg_compare_intf_impl.h#L338) | [文档](docs/zh/api/SIMD-API/c_api/reg_compute/reg_compare/asc_lt_scalar.md) |
| 90 | uint8_t | `asc_le_scalar` | 通过 | [声明 L136](include/c_api/reg_compute/compute/reg_compare.h#L136) | [vcmps_le L212](impl/c_api/reg_base_impl/reg_compare_intf_impl.h#L212) | [文档](docs/zh/api/SIMD-API/c_api/reg_compute/reg_compare/asc_le_scalar.md) |
| 91 | uint8_t | `asc_gt_scalar` | 通过 | [声明 L352](include/c_api/reg_compute/compute/reg_compare.h#L352) | [vcmps_gt L464](impl/c_api/reg_base_impl/reg_compare_intf_impl.h#L464) | [文档](docs/zh/api/SIMD-API/c_api/reg_compute/reg_compare/asc_gt_scalar.md) |
| 92 | uint8_t | `asc_ge_scalar` | 通过 | [声明 L70](include/c_api/reg_compute/compute/reg_compare.h#L70) | [vcmps_ge L107](impl/c_api/reg_base_impl/reg_compare_intf_impl.h#L107) | [文档](docs/zh/api/SIMD-API/c_api/reg_compute/reg_compare/asc_ge_scalar.md) |
| 93 | uint8_t | `asc_duplicate` | 通过 | [声明 L148](include/c_api/reg_compute/compute/reg_broadcast.h#L148) | [vdup L191](impl/c_api/reg_base_impl/reg_broadcast_intf_impl.h#L191) | [文档](docs/zh/api/SIMD-API/c_api/reg_compute/reg_broadcast/asc_duplicate.md) |
| 94 | uint8_t | `asc_duplicate_merge` | 通过 | [声明 L220](include/c_api/reg_compute/compute/reg_broadcast.h#L220) | [vdup L695](impl/c_api/reg_base_impl/reg_broadcast_intf_impl.h#L695) | [文档](docs/zh/api/SIMD-API/c_api/reg_compute/reg_broadcast/asc_duplicate_merge.md) |
| 95 | uint8_t | `asc_duplicate_highest` | 通过 | [声明 L196](include/c_api/reg_compute/compute/reg_broadcast.h#L196) | [vdup L611](impl/c_api/reg_base_impl/reg_broadcast_intf_impl.h#L611) | [文档](docs/zh/api/SIMD-API/c_api/reg_compute/reg_broadcast/asc_duplicate_highest.md) |
| 96 | uint8_t | `asc_duplicate_highest_merge` | 通过 | [声明 L244](include/c_api/reg_compute/compute/reg_broadcast.h#L244) | [vdup L779](impl/c_api/reg_base_impl/reg_broadcast_intf_impl.h#L779) | [文档](docs/zh/api/SIMD-API/c_api/reg_compute/reg_broadcast/asc_duplicate_highest_merge.md) |
| 97 | uint8_t | `asc_duplicate_scalar` | 通过 | [声明 L28](include/c_api/reg_compute/compute/reg_broadcast.h#L28) | [vdup L23](impl/c_api/reg_base_impl/reg_broadcast_intf_impl.h#L23) | [文档](docs/zh/api/SIMD-API/c_api/reg_compute/reg_broadcast/asc_duplicate_scalar.md) |
| 98 | uint8_t | `asc_duplicate_scalar_merge` | 通过 | [声明 L76](include/c_api/reg_compute/compute/reg_broadcast.h#L76) | [vdup L359](impl/c_api/reg_base_impl/reg_broadcast_intf_impl.h#L359) | [文档](docs/zh/api/SIMD-API/c_api/reg_compute/reg_broadcast/asc_duplicate_scalar_merge.md) |

目标文件SHA-256：`98991bc3f324eb20e668d93884b400ea13257bfbf5be1c572dccf10e7869b170`。
