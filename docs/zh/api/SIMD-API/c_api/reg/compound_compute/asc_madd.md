# asc_madd

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

## 功能说明

头文件路径为：`"c_api/reg_compute/reg_vector.h"`。

根据mask对目的操作数dst、源操作数src0和src1执行逐元素融合乘加运算：将dst中的原有元素与src0中的对应元素相乘，再与src1中的对应元素相加，并将结果写回dst。计算公式如下：

$$
dst_i = dst_i \times src0_i + src1_i
$$

mask未筛选的元素在dst中置零。本接口在硬件层面通过单条指令完成融合乘加运算，精度高于分别执行乘法和加法运算。

## 函数原型

```c
__simd_callee__ inline void asc_madd(vector_half& dst,
                                     vector_half src0,
                                     vector_half src1,
                                     vector_bool mask)

__simd_callee__ inline void asc_madd(vector_float& dst,
                                     vector_float src0,
                                     vector_float src1,
                                     vector_bool mask)
```

操作数支持的数据类型为`half`和`float`。

## 参数说明

**表1** 参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| dst | 输入/输出 | 目的操作数（矢量数据寄存器）。输入时保存参与乘法运算的原始数据，输出时保存融合乘加结果。数据类型需要与src0、src1保持一致。 |
| src0 | 输入 | 源操作数（矢量数据寄存器），参与乘法运算。数据类型需要与dst、src1保持一致。 |
| src1 | 输入 | 源操作数（矢量数据寄存器），参与加法运算。数据类型需要与dst、src0保持一致。 |
| mask | 输入 | 源操作数元素操作的有效指示（掩码寄存器）。mask筛选的元素参与融合乘加运算，未筛选的元素在dst中置零。 |

矢量数据寄存器和掩码寄存器的详细说明请参见[reg数据类型定义](../reg_data_types/data_type_definition.md)。

## 返回值说明

无

## 约束说明

- 本接口为Reg矢量API，只能在使用`__simd_vf__`标记的VF函数内调用，不支持在`__aicore__`函数中直接调用；计算在AIV上执行。
- dst、src0和src1的数据类型需要保持一致，仅支持`half`和`float`。其他数据类型组合无法匹配函数重载，编译不通过。
- dst在运算中既作为输入又作为输出：接口先读取dst中的原始数据参与乘法，再将融合乘加结果写回dst。本接口支持dst与src0或src1使用同一寄存器；同一寄存器的数据依赖由硬件保序，无需额外插入同步指令。
- dst、src0、src1和mask均为寄存器，不涉及UB地址和地址对齐。
- 本接口与前后Reg数据搬运接口之间，如果不同寄存器访问同一UB地址且存在写后读或写后写依赖，需要调用[asc_mem_bar](../sync_control/asc_mem_bar.md)进行同步。
- 本接口不修改全局寄存器的值。
- 使用mask前，需要通过掩码设置或搬入接口完成初始化；未初始化的掩码寄存器内容不确定。
- mask未筛选的元素在dst中置零。
- 输入包含NaN或Inf时，结果遵循浮点融合乘加规则。例如，`NaN × 有限数 + 有限数`和`Inf × 0 + 有限数`的结果均为NaN。

## 调用示例

```cpp
vector_half dst;
vector_half src0;
vector_half src1;
vector_bool mask = asc_create_mask_b16(PAT_ALL);
asc_loadalign(dst, dst_addr); // dst_addr是外部输入的UB内存空间地址。
asc_loadalign(src0, src0_addr); // src0_addr是外部输入的UB内存空间地址。
asc_loadalign(src1, src1_addr); // src1_addr是外部输入的UB内存空间地址。
asc_madd(dst, src0, src1, mask);
```
