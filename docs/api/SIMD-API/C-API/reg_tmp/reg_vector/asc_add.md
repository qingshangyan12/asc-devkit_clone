# asc_add

## 产品支持情况

<!-- npu="950" id1 -->
- Ascend 950PR/Ascend 950DT：支持
<!-- end id1 -->

## 功能说明

头文件路径：`"c_api/reg_compute/reg_vector.h"`。

按照元素对应位置执行矢量加法运算，i为元素索引。计算过程为ZEROING模式（未被mask掩码的元素在目的操作数中置0）。

计算公式如下：

$$
dst_i = src0_i + src1_i
$$

执行带进位的加法运算时，可以在进位输出（掩码寄存器）中标记每次加法是否产生进位，若src0，src1输入按位相加后最高位有进位，在dst0中对应位置每4bit的最低位写1，否则写0。

![](../../../../figures/asc_add.png)

输出dst0适用场景请参考[asc_addc功能说明](asc_addc.md#功能说明)。

## 函数原型

<!-- npu="950" id2 -->
```cpp
// 不带进位的矢量加法
__simd_callee__ inline void asc_add(vector_uint8_t& dst, vector_uint8_t src0, vector_uint8_t src1, vector_bool mask)

__simd_callee__ inline void asc_add(vector_int8_t& dst, vector_int8_t src0, vector_int8_t src1, vector_bool mask)

__simd_callee__ inline void asc_add(vector_uint16_t& dst, vector_uint16_t src0, vector_uint16_t src1, vector_bool mask)

__simd_callee__ inline void asc_add(vector_int16_t& dst, vector_int16_t src0, vector_int16_t src1, vector_bool mask)

__simd_callee__ inline void asc_add(vector_half& dst, vector_half src0, vector_half src1, vector_bool mask)

__simd_callee__ inline void asc_add(vector_bfloat16_t& dst, vector_bfloat16_t src0, vector_bfloat16_t src1, vector_bool mask)

__simd_callee__ inline void asc_add(vector_int32_t& dst, vector_int32_t src0, vector_int32_t src1, vector_bool mask)

__simd_callee__ inline void asc_add(vector_uint32_t& dst, vector_uint32_t src0, vector_uint32_t src1, vector_bool mask)

__simd_callee__ inline void asc_add(vector_float& dst, vector_float src0, vector_float src1, vector_bool mask)

// 带进位的矢量加法
__simd_callee__ inline void asc_add(vector_bool& dst0, vector_int32_t& dst1, vector_int32_t src0, vector_int32_t src1, vector_bool mask)

__simd_callee__ inline void asc_add(vector_bool& dst0, vector_uint32_t& dst1, vector_uint32_t src0, vector_uint32_t src1, vector_bool mask)
```
<!-- end id2 -->

## 参数说明

**表 2**  参数说明

<!-- npu="950" id3 -->
| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| `dst` | 输出 | 目的操作数（矢量数据寄存器）。 |
| `dst0` | 输出 | 进位输出（掩码寄存器），仅进位版本有效。 |
| `dst1` | 输出 | 目的操作数（矢量数据寄存器），仅进位版本有效。 |
| `src0` | 输入 | 源操作数0（矢量数据寄存器）。 |
| `src1` | 输入 | 源操作数1（矢量数据寄存器）。 |
| `mask` | 输入 | 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。 |
<!-- end id3 -->

矢量数据寄存器和掩码寄存器的详细说明请参见[reg数据类型定义.md](../reg数据类型定义.md)。

## 数据类型支持

<!-- npu="950" id4 -->
### 不带进位的矢量加法

目的操作数与源操作数的数据类型需要保持一致。支持的数据类型为：`vector_int8_t`、`vector_uint8_t`、`vector_int16_t`、`vector_uint16_t`、`vector_half`、`vector_bfloat16_t`、`vector_int32_t`、`vector_uint32_t`、`vector_float`。

### 带进位的矢量加法

矢量数据寄存器的目的操作数与源操作数的数据类型需要保持一致，支持的数据类型为：`vector_int32_t`、`vector_uint32_t`。掩码寄存器的进位输出支持的数据类型为：`vector_bool`。
<!-- end id4 -->

## 返回值说明

无

## 流水类型

PIPE_V

## 约束说明

<!-- npu="950" id5 -->
- 不带进位的加法默认执行非饱和运算，此时相同位宽的有符号和无符号数据类型硬件实现完全相同。
- 带进位的加法运算仅支持int32_t和uint32_t类型。
- 带进位的加法运算需手动调用，接口内部不支持自动识别触发。
- 带进位的加法运算输出完整计算结果（包含进位位），硬件不会对输出进行饱和或截断。
<!-- end id5 -->

## 理论性能

### 不带进位的矢量加法

| 数据类型 | 理论并行度（element/cycle） | 
|---|---|
| `vector_int8_t` | 128 |
| `vector_uint8_t` | 128 |
| `vector_int16_t` | 64 |
| `vector_uint16_t` | 64 |
| `vector_half` | 128 |
| `vector_bfloat16_t` | 128 |
| `vector_int32_t` | 32 |
| `vector_uint32_t` | 32 |
| `vector_float` | 64 |

### 带进位的矢量加法

| 矢量数据寄存器数据类型 | 理论并行度（element/cycle） | 
|---|---|
| `vector_int32_t` | 32 |
| `vector_uint32_t` | 32 |

## 调用示例

<!-- npu="950" id6 -->
```cpp
vector_half dst;
vector_half src0;
vector_half src1;
vector_bool mask = asc_create_mask_b16(PAT_ALL);
asc_loadalign(src0, src0_addr);  // src0_addr是外部输入的UB内存空间地址
asc_loadalign(src1, src1_addr);  // src1_addr是外部输入的UB内存空间地址
asc_add(dst, src0, src1, mask);
```
<!-- end id6 -->
