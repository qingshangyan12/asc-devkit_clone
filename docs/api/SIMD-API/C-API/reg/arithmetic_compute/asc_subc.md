# asc_subc

## 产品支持情况

<!-- npu="950" id1 -->
- Ascend 950PR/Ascend 950DT：支持
<!-- end id1 -->

## 功能说明

头文件路径：`"c_api/reg_compute/reg_vector.h"`。

根据mask，对源操作数src0、src1及输入进位carry_src进行按元素求差操作，i为元素索引，相减结果写入dst，同时将每个元素的进位结果写入carry。计算公式如下：

$$
\{dst0_i, dst1_i\} = src0_i - src1_i - carry\_src_i
$$

Carry flag（进位标志）用于表示加法进位或者减法无借位，若src0，src1，carry_src输入按位相减后最高位有借位，在carry（存放借位的掩码寄存器）中对应位置每4bit设置1，否则写0。

![](../../../../figures/asc_subc_1.png)

以int64_t类型数据计算-1 - 8 = -9为例，asc_sub/asc_subc接口的适用场景如下图：

![](../../../../figures/asc_subc_2.png)

## 函数原型

```cpp
__simd_callee__ inline void asc_subc(vector_bool& carry, vector_uint32_t& dst, vector_uint32_t src0, vector_uint32_t src1, vector_bool carry_src, vector_bool mask)
__simd_callee__ inline void asc_subc(vector_bool& carry, vector_int32_t& dst, vector_int32_t src0, vector_int32_t src1, vector_bool carry_src, vector_bool mask)
```

## 参数说明

| 参数名  | 输入/输出 | 描述 |
| :----- | :------- | :------- |
| carry | 输出 | 目的借位值（掩码寄存器）。 |
| dst | 输出 | 目的操作数（矢量数据寄存器）。 |
| src0 | 输入 | 源操作数0（矢量数据寄存器）。 |
| src1 | 输入 | 源操作数1（矢量数据寄存器）。 |
| carry_src | 输入 | 源借位值（矢量数据寄存器）。 |
| mask | 输入 | 源操作数掩码（掩码寄存器）。用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。 |

矢量数据寄存器和掩码寄存器的详细说明请参见[data_type_definition.md](../reg_data_types/data_type_definition.md)。

## 返回值说明

无

## 流水类型

PIPE_V

## 约束说明

- mask控制源操作数是否参与计算，源操作数不参与计算的元素在输出对应位置置零。

- 运算输出完整计算结果（包含进位位），硬件不会对输出进行饱和或截断。

## 调用示例

```cpp
vector_bool carry;
vector_uint32_t dst;
vector_uint32_t src0;
vector_uint32_t src1;
vector_bool carry_src = asc_create_mask_b32(PAT_ALLF);
vector_bool mask = asc_create_mask_b32(PAT_ALL);
asc_loadalign(src0, src0_addr); // src0_addr是外部输入的UB内存空间地址。
asc_loadalign(src1, src1_addr); // src1_addr是外部输入的UB内存空间地址。
// mask为源操作数掩码
asc_subc(carry, dst, src0, src1, carry_src, mask);
```
