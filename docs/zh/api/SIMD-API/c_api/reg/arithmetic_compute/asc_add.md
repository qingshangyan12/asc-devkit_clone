# asc_add

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

该接口根据`mask`，对源操作数`src0`、`src1`进行按元素求和操作，将结果写入目的操作数`dst`。

Carry flag（进位/借位标志）用于表示加法进位或者减法无借位。该接口可以在`carry`（掩码寄存器）中标记每次加法是否产生进位，若`src0`，`src1`输入按位相加后最高位有进位，在`carry`中对应位置每4bit的最低位写1，否则写0。

**[无进位加法](#无进位加法)**：单目的操作数，矢量数据寄存器写入加法和，计算公式如下：

$$
dst_i = src0_i + src1_i
$$

**[带进位输出加法](#带进位输出加法)**：双目的操作数，矢量数据寄存器写入32位加法和的低32位，目的掩码寄存器写入每个元素加法产生的进位输出，计算公式如下：

$$
\{carry_i, dst_i\} = src0_i + src1_i
$$

![](../../figures/asc_add.png)

输出carry适用场景请参考[asc_addc功能说明](asc_addc.md#功能说明)。

本接口仅在AIV上执行有效，在AIC上调用为空操作。

## 函数原型

### 无进位加法

#### 模板原型（占位符形式）
```c
__simd_callee__ inline void asc_add(vector_<dtype>& dst,
                                    vector_<dtype> src0,
                                    vector_<dtype> src1,
                                    vector_bool mask)
```

#### dtype支持数据类型

dtype支持的数据类型为`int8_t`、`uint8_t`、`int16_t`、`uint16_t`、`half`、`bfloat16_t`、`int32_t`、`uint32_t`、`float`

#### 典型示例

```c
// 示例：float类型无进位加法
__simd_callee__ inline void asc_add(vector_float& dst,
                                    vector_float src0,
                                    vector_float src1,
                                    vector_bool mask)
```

### 带进位输出加法
```c
__simd_callee__ inline void asc_add(vector_bool& dst0,
                                    vector_uint32_t& dst1,
                                    vector_uint32_t src0,
                                    vector_uint32_t src1,
                                    vector_bool mask)
```

## 参数说明

### 无进位加法

**表1** 参数说明

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| dst | 输出 | 目的矢量数据寄存器，写入加法和。dtype须与src0、src1一致。 |
| src0 | 输入 | 第一源矢量数据寄存器，参与加法的第一操作数。dtype须与dst一致。 |
| src1 | 输入 | 第二源矢量数据寄存器，参与加法的第二操作数。dtype须与dst一致。 |
| mask | 输入 | 掩码寄存器，按比特位决定对应位置的元素是否参与计算。mask比特位为1的位置执行加法并将结果写入dst对应位置；mask比特位为0的位置不执行加法，dst对应位置写入零（ZEROING模式）。 |

### 带进位输出加法

**表2** 参数说明

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| carry | 输出 | 目的掩码寄存器，写入每个元素加法产生的进位输出。每2个元素的进位打包为1字节。 |
| dst | 输出 | 目的矢量数据寄存器，写入32位加法和的低32位。dtype须与src0、src1一致。 |
| src0 | 输入 | 第一源矢量数据寄存器，参与加法的第一操作数。dtype须与dst一致。 |
| src1 | 输入 | 第二源矢量数据寄存器，参与加法的第二操作数。dtype须与dst一致。 |
| mask | 输入 | 掩码寄存器，按比特位决定对应位置的元素是否参与计算。mask比特位为1的位置执行加法并将和写入dst对应位置、进位写入carry对应位置；mask比特位为0的位置不执行加法，dst对应位置写入零、carry对应进位写入零（ZEROING模式）。 |

矢量数据寄存器和掩码寄存器的详细说明请参见[reg数据类型定义](../reg_data_types/data_type_definition.md)。

## 返回值说明

无

## 约束说明

### 通用约束

- 本接口仅在AIV上生效，非AIV调用直接返回。
- 本接口在Vector Function（`__simd_vf__`标记的函数）内调用，src0、src1、dst为矢量数据寄存器，不涉及UB地址，无UB容量与地址对齐约束。
- src0、src1、dst三者dtype必须完全一致，否则触发数据类型异常。
- mask为 `vector_bool` 类型掩码寄存器，需通过掩码设置接口预先赋值后再传入；未赋值的掩码寄存器内容不确定，会导致有效元素位置错误。
- mask比特位为0的位置采用清零模式（ZEROING）：dst对应位置写0，不保留src原值也不保留dst原值。

### 无进位加法约束

- dtype仅支持int8_t、uint8_t、int16_t、uint16_t、half、bfloat16_t、int32_t、uint32_t、float共9种，其他dtype触发数据类型异常。
- 整型dtype加法溢出时按环绕（wrap-around）策略处理：有符号类型溢出回绕到对应dtype的最小值或最大值（MAX+1->MIN，MIN-1->MAX），无符号类型溢出回绕到0（UMAX+1->0）。
- 浮点dtype（half、bfloat16_t、float）加法按IEEE 754浮点加法语义执行，溢出与无效输入（如nan、Inf）的处理遵循浮点运算规则。

### 带进位输出加法约束

- dtype仅支持uint32_t，其他dtype触发数据类型异常。
- 运算输出完整计算结果（包含进位位），不受[asc_set_ctrl](../../sys_var/asc_set_ctrl.md)影响，硬件不会对输出进行饱和或截断。
- 进位输出按32位无符号加法产生：每个元素的进位为`(src0_i + src1_i) >> 32`取最高位1bit。
- 和值截断为32位：dst写入`(src0_i + src1_i) & 0xFFFFFFFF`，超出32位的部分不保留，仅通过carry的进位输出体现。

## 调用示例

- 无进位加法

  ```cpp
  __simd_vf__ inline void add_vf(__ubuf__ half* dst_addr, __ubuf__ half* src0_addr, __ubuf__ half* src1_addr, uint32_t count, int32_t one_repeat_size, uint16_t repeat_time)
  {
      vector_half src0;
      vector_half src1;
      vector_half dst;
      vector_bool mask;
      for (uint16_t i = 0; i < repeat_time; ++i) {
          mask = asc_update_mask_b16(count);
          asc_loadalign_postupdate(src0, src0_addr, one_repeat_size);
          asc_loadalign_postupdate(src1, src1_addr, one_repeat_size);
          asc_add(dst, src0, src1, mask);
          asc_storealign_postupdate(dst_addr, dst, one_repeat_size, mask);
      }
  }
  ```

- 带进位输出加法

  ```cpp
  __simd_vf__ inline void add_vf(__ubuf__ uint32_t* carry_addr, __ubuf__ uint32_t* dst_addr, __ubuf__ uint32_t* src0_addr, __ubuf__ uint32_t* src1_addr, uint32_t count, int32_t one_repeat_size, int32_t one_block_size, uint16_t repeat_time)
  {
      vector_uint32_t src0;
      vector_uint32_t src1;
      vector_uint32_t dst;
      vector_bool carry;
      vector_bool mask;
      for (uint16_t i = 0; i < repeat_time; ++i) {
          mask = asc_update_mask_b32(count);
          asc_loadalign_postupdate(src0, src0_addr, one_repeat_size);
          asc_loadalign_postupdate(src1, src1_addr, one_repeat_size);
          asc_add(carry, dst, src0, src1, mask);
          asc_storealign_postupdate(dst_addr, dst, one_repeat_size, mask);
          asc_storealign_postupdate(carry_addr, carry, one_block_size);
      }
  }
  ```
