# asc_mul

## 产品支持情况

<!-- npu="950" id1 -->
- Ascend 950PR/Ascend 950DT：支持
<!-- end id1 -->

## 功能说明

头文件路径：`"c_api/reg_compute/reg_vector.h"`。

对源操作数src0和src1进行乘法运算，将结果写入目的操作数dst。计算公式如下：

$$
dst_i = src0_i * src1_i
$$

## 函数原型

```cpp
__simd_callee__ inline void asc_mul(vector_int16_t& dst, vector_int16_t src0, vector_int16_t src1, vector_bool mask)
__simd_callee__ inline void asc_mul(vector_uint16_t& dst, vector_uint16_t src0, vector_uint16_t src1, vector_bool mask)
__simd_callee__ inline void asc_mul(vector_half& dst, vector_half src0, vector_half src1, vector_bool mask)
__simd_callee__ inline void asc_mul(vector_bfloat16_t& dst, vector_bfloat16_t src0, vector_bfloat16_t src1, vector_bool mask)
__simd_callee__ inline void asc_mul(vector_int32_t& dst, vector_int32_t src0, vector_int32_t src1, vector_bool mask)
__simd_callee__ inline void asc_mul(vector_uint32_t& dst, vector_uint32_t src0, vector_uint32_t src1, vector_bool mask)
__simd_callee__ inline void asc_mul(vector_float& dst, vector_float src0, vector_float src1, vector_bool mask)
```

## 参数说明

| 参数名  | 输入/输出 | 描述 |
| :----- | :------- | :------- |
| dst | 输出 | 目的操作数（矢量数据寄存器）。<br>类型为[vector\_\[data\_type\]](../reg_data_types/data_type_definition.md#reg数据类型定义)，data_type的取值范围请参考[数据类型](#数据类型)。 |
| src0 | 输入 | 源操作数（矢量数据寄存器）。<br>类型为[vector\_\[data\_type\]](../reg_data_types/data_type_definition.md#reg数据类型定义)，data_type的取值范围请参考[数据类型](#数据类型)。 |
| src1 | 输入 | 源操作数（矢量数据寄存器）。<br>类型为[vector\_\[data\_type\]](../reg_data_types/data_type_definition.md#reg数据类型定义)，data_type的取值范围请参考[数据类型](#数据类型)。 |
| mask | 输入 | 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。<br>类型为[vector\_bool](../reg_data_types/data_type_definition.md#掩码寄存器)。 |

## 数据类型

目的操作数和源操作数的数据类型需要保持一致。支持的数据类型为：int16_t、uint16_t、half、bfloat16_t、int32_t、uint32_t、float。

## 返回值说明

无

## 流水类型

PIPE_V

## 约束说明

- mask控制源操作数是否参与计算，源操作数不参与计算的元素在输出对应位置置零。

- 默认执行非饱和运算，此时相同位宽的有符号和无符号数据类型硬件实现完全相同。

## 调用示例

```cpp
constexpr uint32_t total_length = 128;
__ubuf__ half src0_addr[total_length];
__ubuf__ half src1_addr[total_length];

vector_half src0;
vector_half src1;
vector_half dst;
vector_bool mask = asc_create_mask_b16(PAT_ALL);
asc_loadalign(src0, src0_addr);
asc_loadalign(src1, src1_addr);
asc_mul(dst, src0, src1, mask);
```
