# asc_exp

## 产品支持情况

<!-- npu="950" id1 -->
- Ascend 950PR/Ascend 950DT：支持
<!-- end id1 -->

## 功能说明

头文件路径：`"c_api/reg_compute/reg_vector.h"`。

按元素取自然指数，计算公式如下：
$$
dst_i = e^{src_i}
$$

## 函数原型

<!-- npu="950" id2 -->
```cpp
__simd_callee__ inline void asc_exp(vector_half& dst, vector_half src, vector_bool mask)

__simd_callee__ inline void asc_exp(vector_float& dst, vector_float src, vector_bool mask)
```
<!-- end id2 -->

## 参数说明

**表 2**  参数说明

<!-- npu="950" id3 -->
| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| `dst` | 输出 | 目的操作数（矢量数据寄存器）。 |
| `src` | 输入 | 源操作数（矢量数据寄存器）。 |
| `mask` | 输入 | 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。 |
<!-- end id3 -->

矢量数据寄存器和掩码寄存器的详细说明请参见[reg数据类型定义.md](../reg数据类型定义.md)。

## 数据类型支持

<!-- npu="950" id4 -->
目的操作数与源操作数的数据类型需要保持一致。支持的数据类型为：`vector_half`、`vector_float`。
<!-- end id4 -->

## 返回值说明

无

## 流水类型

PIPE_V

## 理论性能

| 数据类型 | 理论并行度（element/cycle） | 
|---|---|
| `vector_half` | <TODO> |
| `vector_float` | <TODO> |

## 约束说明

<!-- npu="950" id5 -->
<TODO>
<!-- end id5 -->

## 调用示例

<!-- npu="950" id6 -->
```cpp
vector_half dst;
vector_half src;
vector_bool mask = asc_create_mask_b16(PAT_ALL);
asc_loadalign(src, src_addr);  // src_addr是外部输入的UB内存空间地址
asc_exp(dst, src, mask);
```
<!-- end id6 -->
