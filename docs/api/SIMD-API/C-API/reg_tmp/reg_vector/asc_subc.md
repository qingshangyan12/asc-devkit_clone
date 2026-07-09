# asc_subc

## 产品支持情况

<!-- npu="950" id1 -->
- Ascend 950PR/Ascend 950DT：支持
<!-- end id1 -->

## 功能说明

头文件路径：`"c_api/reg_compute/reg_vector.h"`。

根据mask对源操作数src0、src1以及进位数据carry_src进行按元素相减的操作，将结果写入目的操作数dst。src0、src1、carry_src相减时如果产生借位，在carry中对应位置每4bit的最低位写0，否则写1。

计算公式如下：

$$
dst_i = src0_i - src1_i - carry\_src_i
$$

## 函数原型

<!-- npu="950" id2 -->
```cpp
__simd_callee__ inline void asc_subc(vector_bool& carry, vector_uint32_t& dst, vector_uint32_t src0, vector_uint32_t src1, vector_bool carry_src, vector_bool mask)

__simd_callee__ inline void asc_subc(vector_bool& carry, vector_int32_t& dst, vector_int32_t src0, vector_int32_t src1, vector_bool carry_src, vector_bool mask)
```
<!-- end id2 -->

## 参数说明

**表 2**  参数说明

<!-- npu="950" id3 -->
| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| `carry` | 输出 | 借位输出（掩码寄存器）。 |
| `dst` | 输出 | 目的操作数（矢量数据寄存器）。 |
| `src0` | 输入 | 源操作数0（矢量数据寄存器）。 |
| `src1` | 输入 | 源操作数1（矢量数据寄存器）。 |
| `carry_src` | 输入 | 借位输入（掩码寄存器）。 |
| `mask` | 输入 | 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。 |
<!-- end id3 -->

矢量数据寄存器和掩码寄存器的详细说明请参见[reg数据类型定义.md](../reg数据类型定义.md)。

## 数据类型支持

<!-- npu="950" id4 -->
矢量数据寄存器的目的操作数与源操作数的数据类型需要保持一致，支持的数据类型为：`vector_int32_t`、`vector_uint32_t`。掩码寄存器的借位输出支持的数据类型为：`vector_bool`。
<!-- end id4 -->

## 返回值说明

无

## 流水类型

PIPE_V

## 理论性能

| 矢量数据寄存器数据类型 | 理论并行度（element/cycle） | 
|---|---|
| `vector_int32_t` | <TODO> |
| `vector_uint32_t` | <TODO> |

## 约束说明

<!-- npu="950" id5 -->
<TODO>
<!-- end id5 -->

## 调用示例

<!-- npu="950" id6 -->
```cpp
vector_bool carry;
vector_uint32_t dst;
vector_uint32_t src0;
vector_uint32_t src1;
vector_bool carry_src;
vector_bool mask = asc_create_mask_b32(PAT_ALL);
asc_loadalign(src0, src0_addr);  // src0_addr是外部输入的UB内存空间地址
asc_loadalign(src1, src1_addr);  // src1_addr是外部输入的UB内存空间地址
asc_subc(carry, dst, src0, src1, carry_src, mask);
```
<!-- end id6 -->
