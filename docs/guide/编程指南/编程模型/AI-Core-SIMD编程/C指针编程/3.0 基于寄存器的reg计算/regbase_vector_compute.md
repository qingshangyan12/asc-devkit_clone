# Regbase API 矢量计算

## 1. 矢量计算概述

Regbase API的矢量计算在矢量数据寄存器之间执行，结果写入目的寄存器。掩码寄存器控制哪些元素参与计算。

### 1.1 计算特点

- 操作数：矢量数据寄存器
- 结果：矢量数据寄存器
- 掩码：参数传递，局部掩码
- 计算粒度：单次VL长度

---

## 2. 算术运算

### 2.1 基础运算

#### asc_add

```cpp
__simd_callee__ inline void asc_add(vector_half& dst, vector_half src0, vector_half src1,
                                     vector_bool mask);
```

**计算公式**：dst[i] = src0[i] + src1[i]

**支持的类型**：
- `vector_int8_t`, `vector_uint8_t`
- `vector_int16_t`, `vector_uint16_t`
- `vector_half`, `vector_bfloat16_t`
- `vector_int32_t`, `vector_uint32_t`, `vector_float`

#### asc_sub

```cpp
__simd_callee__ inline void asc_sub(vector_half& dst, vector_half src0, vector_half src1,
                                     vector_bool mask);
```

**计算公式**：dst[i] = src0[i] - src1[i]

#### asc_mul

```cpp
__simd_callee__ inline void asc_mul(vector_half& dst, vector_half src0, vector_half src1,
                                     vector_bool mask);
```

**计算公式**：dst[i] = src0[i] × src1[i]

#### asc_div

```cpp
__simd_callee__ inline void asc_div(vector_half& dst, vector_half src0, vector_half src1,
                                     vector_bool mask);
```

**计算公式**：dst[i] = src0[i] ÷ src1[i]

### 2.2 带进位运算

#### asc_addc

```cpp
__simd_callee__ inline void asc_addc(vector_bool& dst0, vector_int32_t& dst1,
                                      vector_int32_t src0, vector_int32_t src1,
                                      vector_bool mask);
```

**功能**：按元素执行带进位的加法运算。

**约束**：仅支持int32_t和uint32_t类型。

#### asc_subc

```cpp
__simd_callee__ inline void asc_subc(vector_half& dst, vector_half src0, vector_half src1,
                                      vector_bool mask);
```

**功能**：按元素执行带借位的减法运算。

### 2.3 标量运算

#### asc_add_scalar

```cpp
__simd_callee__ inline void asc_add_scalar(vector_half& dst, vector_half src, half value,
                                            vector_bool mask);
```

**计算公式**：dst[i] = src[i] + value

#### asc_mul_scalar

```cpp
__simd_callee__ inline void asc_mul_scalar(vector_half& dst, vector_half src, half value,
                                            vector_bool mask);
```

**计算公式**：dst[i] = src[i] × value

### 2.4 复合运算

#### asc_madd

```cpp
__simd_callee__ inline void asc_madd(vector_half& dst, vector_half src0, vector_half src1,
                                      vector_bool mask);
```

**计算公式**：dst[i] = src0[i] × src1[i] + dst[i]

#### asc_axpy

```cpp
__simd_callee__ inline void asc_axpy(vector_half& dst, vector_half src, half value,
                                      vector_bool mask);
```

**计算公式**：dst[i] = src[i] × value + dst[i]

---

## 3. 数学函数

### 3.1 指数与对数

#### asc_exp

```cpp
__simd_callee__ inline void asc_exp(vector_half& dst, vector_half src, vector_bool mask);
```

**计算公式**：dst[i] = e^src[i]

#### asc_ln

```cpp
__simd_callee__ inline void asc_ln(vector_half& dst, vector_half src, vector_bool mask);
```

**计算公式**：dst[i] = ln(src[i])

#### asc_exp_sub

```cpp
__simd_callee__ inline void asc_exp_sub(vector_half& dst, vector_half src0, vector_half src1,
                                         vector_bool mask);
```

**计算公式**：dst[i] = e^(src0[i] - src1[i])

### 3.2 开方运算

#### asc_sqrt

```cpp
__simd_callee__ inline void asc_sqrt(vector_half& dst, vector_half src, vector_bool mask);
```

**计算公式**：dst[i] = √src[i]

### 3.3 绝对值与取反

#### asc_abs

```cpp
__simd_callee__ inline void asc_abs(vector_half& dst, vector_half src, vector_bool mask);
```

**计算公式**：dst[i] = |src[i]|

#### asc_abs_sub

```cpp
__simd_callee__ inline void asc_abs_sub(vector_half& dst, vector_half src0, vector_half src1,
                                         vector_bool mask);
```

**计算公式**：dst[i] = |src0[i] - src1[i]|

#### asc_neg

```cpp
__simd_callee__ inline void asc_neg(vector_half& dst, vector_half src, vector_bool mask);
```

**计算公式**：dst[i] = -src[i]

---

## 4. 激活函数

### 4.1 ReLU

#### asc_relu

```cpp
__simd_callee__ inline void asc_relu(vector_half& dst, vector_half src, vector_bool mask);
```

**计算公式**：dst[i] = max(0, src[i])

#### asc_leakyrelu

```cpp
__simd_callee__ inline void asc_leakyrelu(vector_half& dst, vector_half src, half alpha,
                                           vector_bool mask);
```

**计算公式**：dst[i] = src[i] > 0 ? src[i] : α × src[i]

#### asc_prelu

```cpp
__simd_callee__ inline void asc_prelu(vector_half& dst, vector_half src0, vector_half src1,
                                       vector_bool mask);
```

**计算公式**：dst[i] = src0[i] > 0 ? src0[i] : src0[i] × src1[i]

---

## 5. 比较运算

比较运算生成掩码寄存器，每个元素的比较结果对应一个bit。

### 5.1 矢量比较

#### asc_eq

```cpp
__simd_callee__ inline void asc_eq(vector_bool& dst, vector_half src0, vector_half src1,
                                    vector_bool mask);
```

**功能**：逐元素比较是否相等。

#### asc_gt

```cpp
__simd_callee__ inline void asc_gt(vector_bool& dst, vector_half src0, vector_half src1,
                                    vector_bool mask);
```

**功能**：逐元素比较是否大于。

#### asc_lt

```cpp
__simd_callee__ inline void asc_lt(vector_bool& dst, vector_half src0, vector_half src1,
                                    vector_bool mask);
```

**功能**：逐元素比较是否小于。

#### asc_ge

```cpp
__simd_callee__ inline void asc_ge(vector_bool& dst, vector_half src0, vector_half src1,
                                    vector_bool mask);
```

**功能**：逐元素比较是否大于等于。

#### asc_le

```cpp
__simd_callee__ inline void asc_le(vector_bool& dst, vector_half src0, vector_half src1,
                                    vector_bool mask);
```

**功能**：逐元素比较是否小于等于。

#### asc_ne

```cpp
__simd_callee__ inline void asc_ne(vector_bool& dst, vector_half src0, vector_half src1,
                                    vector_bool mask);
```

**功能**：逐元素比较是否不等于。

### 5.2 标量比较

#### asc_eq_scalar

```cpp
__simd_callee__ inline void asc_eq_scalar(vector_bool& dst, vector_half src, half value,
                                           vector_bool mask);
```

**功能**：逐元素比较是否与标量相等。

---

## 6. 归约运算

### 6.1 全量归约

#### asc_reduce_sum

```cpp
__simd_callee__ inline void asc_reduce_sum(vector_half& dst, vector_half src, vector_bool mask);
```

**功能**：对所有参与计算的元素求和，结果保存在dst中。

#### asc_reduce_max

```cpp
__simd_callee__ inline void asc_reduce_max(vector_half& dst, vector_half src, vector_bool mask);
```

**功能**：对所有参与计算的元素求最大值。

#### asc_reduce_min

```cpp
__simd_callee__ inline void asc_reduce_min(vector_half& dst, vector_half src, vector_bool mask);
```

**功能**：对所有参与计算的元素求最小值。

### 6.2 DataBlock归约

#### asc_reduce_sum_datablock

```cpp
__simd_callee__ inline void asc_reduce_sum_datablock(vector_half& dst, vector_half src,
                                                      vector_bool mask);
```

**功能**：每个DataBlock内元素求和，结果依次保存在dst中。

### 6.3 Pair归约

#### asc_pair_reduce_sum

```cpp
__simd_callee__ inline void asc_pair_reduce_sum(vector_half& dst, vector_half src,
                                                 vector_bool mask);
```

**功能**：相邻两个（奇偶）元素求和，结果写入dst。

---

## 7. 数据类型转换

### 7.1 浮点类型转换

#### asc_half2float

```cpp
__simd_callee__ inline void asc_half2float(vector_float& dst, vector_half src);
```

#### asc_float2half

```cpp
__simd_callee__ inline void asc_float2half(vector_half& dst, vector_float src, vector_bool mask);
```

#### asc_bfloat162float

```cpp
__simd_callee__ inline void asc_bfloat162float(vector_float& dst, vector_bfloat16_t src);
```

### 7.2 整数类型转换

#### asc_int162float

```cpp
__simd_callee__ inline void asc_int162float(vector_float& dst, vector_int16_t src);
```

#### asc_float2int32

```cpp
__simd_callee__ inline void asc_float2int32(vector_int32_t& dst, vector_float src, vector_bool mask);
```

---

## 8. 数据操作

### 8.1 数据填充

#### asc_duplicate

```cpp
__simd_callee__ inline void asc_duplicate(vector_half& dst, vector_half src, vector_bool mask);
```

**功能**：将src的最低位元素填充到dst。

#### asc_duplicate_scalar

```cpp
__simd_callee__ inline void asc_duplicate_scalar(vector_half& dst, half value, vector_bool mask);
```

**功能**：将value填充到dst。

### 8.2 数据选择

#### asc_select

```cpp
__simd_callee__ inline void asc_select(vector_half& dst, vector_half src0, vector_half src1,
                                        vector_bool mask);
```

**功能**：根据mask从src0和src1中选择元素。

### 8.3 数据拷贝

#### asc_copy

```cpp
__simd_callee__ inline void asc_copy(vector_half& dst, vector_half src, vector_bool mask);
```

**功能**：完成数据在寄存器内的搬运。

### 8.4 生成递增序列

#### asc_arange

```cpp
__simd_callee__ inline void asc_arange(vector_half& dst, half value);
```

**功能**：以value为起始值，生成递增序列 [value, value+1, value+2, ...]

---

## 9. 逻辑运算

#### asc_and

```cpp
__simd_callee__ inline void asc_and(vector_bool& dst, vector_bool src0, vector_bool src1);
```

**功能**：掩码寄存器逻辑与运算。

#### asc_or

```cpp
__simd_callee__ inline void asc_or(vector_bool& dst, vector_bool src0, vector_bool src1);
```

**功能**：掩码寄存器逻辑或运算。

#### asc_not

```cpp
__simd_callee__ inline void asc_not(vector_bool& dst, vector_bool src);
```

**功能**：掩码寄存器逻辑非运算。

#### asc_xor

```cpp
__simd_callee__ inline void asc_xor(vector_bool& dst, vector_bool src0, vector_bool src1);
```

**功能**：掩码寄存器逻辑异或运算。

---

## 10. 位移运算

#### asc_shiftleft

```cpp
__simd_callee__ inline void asc_shiftleft(vector_int32_t& dst, vector_int32_t src0,
                                           vector_int32_t src1, vector_bool mask);
```

**功能**：左移运算，src1指定位移位数。

#### asc_shiftright

```cpp
__simd_callee__ inline void asc_shiftright(vector_int32_t& dst, vector_int32_t src0,
                                            vector_int32_t src1, vector_bool mask);
```

**功能**：右移运算。

---

## 11. 计算示例

### 11.1 基础运算

```cpp
vector_half src0, src1, dst;
vector_bool mask = asc_create_mask_b16(PAT_ALL);

asc_loadalign(src0, src0_addr);
asc_loadalign(src1, src1_addr);
asc_add(dst, src0, src1, mask);
asc_storealign(dst_addr, dst, mask);
```

### 11.2 归约运算

```cpp
vector_half src, dst;
vector_bool mask = asc_create_mask_b16(PAT_ALL);

asc_loadalign(src, src_addr);
asc_reduce_sum(dst, src, mask);
asc_storealign(dst_addr, dst, mask);  // dst中只有一个元素为总和
```

### 11.3 比较运算

```cpp
vector_half src0, src1;
vector_bool result, mask;

asc_loadalign(src0, src0_addr);
asc_loadalign(src1, src1_addr);
mask = asc_create_mask_b16(PAT_ALL);
asc_gt(result, src0, src1, mask);  // result为比较结果掩码
```

---

**文档版本**: 1.0  
**适用产品**: Ascend 950PR/Ascend 950DT