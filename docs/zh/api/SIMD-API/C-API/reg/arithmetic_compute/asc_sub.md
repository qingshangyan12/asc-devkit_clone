# asc_sub

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

头文件路径：`"c_api/reg_compute/reg_vector.h"`。

按照元素对应位置执行矢量减法运算，i为元素索引。

计算公式如下：

$$
dst_i = src0_i - src1_i
$$

执行带借位的减法运算时，可以在借位输出（掩码寄存器）中标记每次减法是否产生借位，若src0，src1输入按位相减后最高位有借位，在carry中对应位置每4bit的最低位写0，否则写1。

![](../../figures/asc_sub.png)

输出carry适用场景请参考[asc_subc功能说明](asc_subc.md#功能说明)。

## 函数原型

- 不带借位数据的接口

    ```cpp
    __simd_callee__ inline void asc_sub(vector_uint8_t& dst, vector_uint8_t src0, vector_uint8_t src1, vector_bool mask)
    __simd_callee__ inline void asc_sub(vector_int8_t& dst, vector_int8_t src0, vector_int8_t src1, vector_bool mask)
    __simd_callee__ inline void asc_sub(vector_uint16_t& dst, vector_uint16_t src0, vector_uint16_t src1, vector_bool mask)
    __simd_callee__ inline void asc_sub(vector_int16_t& dst, vector_int16_t src0, vector_int16_t src1, vector_bool mask)
    __simd_callee__ inline void asc_sub(vector_half& dst, vector_half src0, vector_half src1, vector_bool mask)
    __simd_callee__ inline void asc_sub(vector_bfloat16_t& dst, vector_bfloat16_t src0, vector_bfloat16_t src1, vector_bool mask)
    __simd_callee__ inline void asc_sub(vector_uint32_t& dst, vector_uint32_t src0, vector_uint32_t src1, vector_bool mask)
    __simd_callee__ inline void asc_sub(vector_int32_t& dst, vector_int32_t src0, vector_int32_t src1, vector_bool mask)
    __simd_callee__ inline void asc_sub(vector_float& dst, vector_float src0, vector_float src1, vector_bool mask)
    ```

- 带借位数据的接口

    ```cpp
    __simd_callee__ inline void asc_sub(vector_bool& carry, vector_uint32_t& dst, vector_uint32_t src0, vector_uint32_t src1, vector_bool mask)
    __simd_callee__ inline void asc_sub(vector_bool& carry, vector_int32_t& dst, vector_int32_t src0, vector_int32_t src1, vector_bool mask)
    ```

## 参数说明

**表1** 参数说明

| 参数名  | 输入/输出 | 描述 |
| :----- | :------- | :------- |
| carry | 输出 | 目的借位值（[掩码寄存器](../reg_data_types/data_type_definition.md#掩码寄存器)） |
| dst | 输出 | 目的操作数（[矢量数据寄存器](../reg_data_types/data_type_definition.md#reg数据类型定义)）。 |
| src0 | 输入 | 源操作数0（[矢量数据寄存器](../reg_data_types/data_type_definition.md#reg数据类型定义)）。 |
| src1 | 输入 | 源操作数1（[矢量数据寄存器](../reg_data_types/data_type_definition.md#reg数据类型定义)）。 |
| mask | 输入 | 源操作数掩码（[掩码寄存器](../reg_data_types/data_type_definition.md#掩码寄存器)）。mask用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。 |

## 返回值说明

无

## 约束说明

- mask控制源操作数是否参与计算，源操作数不参与计算的元素在输出对应位置置零。

- 不带借位数据的接口

  默认执行非饱和运算，此时相同位宽的有符号和无符号数据类型硬件实现完全相同。

- 带借位数据的接口
  - 仅支持int32_t和uint32_t类型。
  - 需手动调用，接口内部不支持自动识别触发。
  - 运算输出完整计算结果（包含借位位），硬件不会对输出进行饱和或截断。

## 调用示例

```cpp
__simd_vf__ inline void sub_vf(__ubuf__ half* dst_addr, __ubuf__ half* src0_addr, __ubuf__ half* src1_addr, uint32_t count, uint32_t one_repeat_size, uint16_t one_block_size, uint16_t repeat_time)
{
    vector_half src0;
    vector_half src1;
    vector_half dst;
    vector_bool mask;
    for (uint16_t i = 0; i < repeat_time; ++i) {
        mask = asc_update_mask_b16(count);
        asc_loadalign_postupdate(src0, src0_addr, one_repeat_size);
        asc_loadalign_postupdate(src1, src1_addr, one_repeat_size);
        asc_sub(dst, src0, src1, mask);
        asc_storealign_postupdate(dst_addr, dst, one_block_size, mask);
    }
}
```
