# Reg矢量计算接口精度标准汇总检查确认

检查对象：[reg_vector_compute_interface_precision_standard_summary.md](reg_vector_compute_interface_precision_standard_summary.md)

检查日期：2026年10月30日。

## 用例1：表3、表4与表1、表2的一一对应关系

**确认结果：通过。**

- 表1列出Basic API的基础算术、复合指令和归约计算；表3分别使用对应的C API命名映射：`Exp`→`asc_exp`、`Sqrt`→`asc_sqrt`、`Add`→`asc_add`、`Sub`→`asc_sub`、`Adds`→`asc_add_scalar`、`Div`→`asc_div`、`Neg`→`asc_neg`、`Ln/Log`→`asc_ln/asc_log`、`Mul`→`asc_mul`、`Muls`→`asc_mul_scalar`，以及复合指令和归约接口对应项。证据见[表1第7-30行](reg_vector_compute_interface_precision_standard_summary.md#L7)和[表3第46-68行](reg_vector_compute_interface_precision_standard_summary.md#L46)。
- 表1中的`Log2/Log10`没有直接对应的C API，文档已在[第70-80行](reg_vector_compute_interface_precision_standard_summary.md#L70)明确说明，并给出通过`asc_ln`和`asc_mul_scalar`换底计算的方案；这属于已说明的接口缺失，不是一一对应遗漏。
- 表2与表4均按低精度数据类型逐行组织；表4将Basic API接口改为对应的C API接口，转换、算术、比较和广播能力保持一致。证据见[表2第32-44行](reg_vector_compute_interface_precision_standard_summary.md#L32)和[表4第82-94行](reg_vector_compute_interface_precision_standard_summary.md#L82)。
- 对照`include/c_api/reg_compute/compute/reg_arith.h`、`reg_fused.h`、`reg_reduce.h`、`reg_broadcast.h`和`reg_convert.h`的声明，表3、表4列出的接口均落在对应的C API Reg矢量计算类别中，底层指令类别一致。

## 用例2：表4接口与函数原型

**确认结果：通过。**

已对表4中的接口逐项去重，并在C API公开头文件中检索函数原型：

- 转换类接口均可在[reg_convert.h第24行起](../../../../include/c_api/reg_compute/reg_convert.h#L24)找到对应声明，包括`asc_bfloat162e2m1x2_*`、`asc_e2m1x22bfloat16`、`asc_float2e5m2_*`、`asc_e5m22float`、`asc_float2e4m3_*`、`asc_e4m32float`、HIFLOAT8转换、INT4/INT8/UINT8转换接口。
- 算术和复合类接口可在[reg_arith.h第28行起](../../../../include/c_api/reg_compute/compute/reg_arith.h#L28)和[reg_fused.h第40行起](../../../../include/c_api/reg_compute/compute/reg_fused.h#L40)找到声明。
- 比较类接口可在`include/c_api/reg_compute/compute/reg_compare.h`中找到`asc_eq`、`asc_ne`、`asc_lt`、`asc_le`、`asc_gt`、`asc_ge`及对应Scalar原型。
- 广播类接口可在`include/c_api/reg_compute/compute/reg_broadcast.h`中找到`asc_duplicate`、`asc_duplicate_merge`、`asc_duplicate_highest`、`asc_duplicate_highest_merge`、`asc_duplicate_scalar`和`asc_duplicate_scalar_merge`原型。

占位符也与原型形式一致：`<round_mode>`和`<sat_mode>`表示文档中随后[第98-102行](reg_vector_compute_interface_precision_standard_summary.md#L98)说明的合法后缀组合，不是缺失的函数名。

## 用例3：表3软仿列

**确认结果：通过，表3最后一列标记为“-”正确。**

检查范围为表3列出的23个C API接口及其对应的C API声明和实现头文件。结果如下：

- `include/c_api/reg_compute`和`include/c_api/composite`中未发现这些接口的软仿分支、软仿实现标识或`soft`/`simulator`相关条件分支。
- 相关头文件中的条件编译仅用于C API头文件保护、纯SIMT编译禁止和`__NPU_ARCH__`架构差异，例如[reg_arith.h第11-24行](../../../../include/c_api/reg_compute/compute/reg_arith.h#L11)；这些不是软仿分支。
- 仓内C API文档中出现的“软仿实现”位于其他接口（例如Gather或Scalar原子接口），不属于表3的23个接口，不能据此为表3增加软仿精度说明。

因此表3中`asc_exp`至`asc_pair_reduce_sum`对应的软仿精度列均可填写“-”。

## 用例4：文档规范检查

**确认结果：逐项通过。**

以下按文档检查技能的15项清单逐项检查，检查对象为汇总文档本身：

1. **标点符号与空格：通过。** 未发现中文与英文、数字之间的违规空格；公式、代码和Markdown语法中的必要字符已排除。
2. **Markdown格式：通过。** 标题、表格、锚点和说明段落格式正确。
3. **代码块：通过。** 本文无围栏代码块；行内接口名和参数均使用反引号。
4. **图片：不适用。** 文档无图片引用。
5. **图表编号：通过。** 表1至表4连续编号，表格标题与表格内容对应。
6. **引用与锚点：通过。** 文档内表格锚点可解析，引用目标为当前文档中的有效锚点。
7. **列表：通过。** 列表标记、缩进和空格使用正确。
8. **芯片过滤标签：不适用。** 文档只说明Ascend 950PR&950DT系列产品，未使用芯片过滤标签。
9. **数据类型：通过。** 数据类型采用仓内标准写法，低精度类型顺序和名称一致。
10. **API章节结构：不适用。** 本文是精度标准汇总文档，不是单个API内容页。
11. **术语：通过。** Basic API、C API、Reg矢量计算、软仿、硬件指令等术语前后一致。
12. **错别字和语病：通过。** 通读全文未发现影响语义的错别字或语病。
13. **头文件路径：不适用。** 本文不是单个API内容页，不要求“头文件路径为”章节。
14. **变量一致性：不适用。** 本文没有“调用示例”及结果示例变量对照场景。
15. **调用示例注释：不适用。** 本文没有调用示例代码块。

## 总体确认

四个验收用例均通过：表3、表4的接口映射与底层指令类别一致；表4列出的接口均能在C API公开头文件中找到函数原型；表3的C API均未发现软仿分支，软仿精度列使用“-”正确；文档规范检查逐项通过或按适用性标记为不适用。
