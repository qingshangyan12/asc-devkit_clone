# asc_set_va_reg

## 产品支持情况

|产品|是否支持|
| :------------ | :------------: |
| <term>Atlas A3 训练系列产品/Atlas A3 推理系列产品</term> | √ |
| <term>Atlas A2 训练系列产品/Atlas A2 推理系列产品</term> | √ |

## 功能说明

用于设置transpose的16个DataBlock地址，将操作数地址序列与地址寄存器关联，接口要求前8个和后8个地址序列与地址寄存器分别关联。

## 函数原型

```cpp
__aicore__ inline void asc_set_va_reg(ub_addr8_t addr, __ubuf__ int8_t* src_array[16])
__aicore__ inline void asc_set_va_reg(ub_addr8_t addr, __ubuf__ uint8_t* src_array[16])
__aicore__ inline void asc_set_va_reg(ub_addr8_t addr, __ubuf__ int16_t* src_array[16])
__aicore__ inline void asc_set_va_reg(ub_addr8_t addr, __ubuf__ uint16_t* src_array[16])
__aicore__ inline void asc_set_va_reg(ub_addr8_t addr, __ubuf__ half* src_array[16])
__aicore__ inline void asc_set_va_reg(ub_addr8_t addr, __ubuf__ int32_t* src_array[16])
__aicore__ inline void asc_set_va_reg(ub_addr8_t addr, __ubuf__ uint32_t* src_array[16])
__aicore__ inline void asc_set_va_reg(ub_addr8_t addr, __ubuf__ float* src_array[16])
```

## 参数说明

|参数名|输入/输出|描述|
| ------------ | ------------ | ------------ |
|addr|输出|地址寄存器，类型为ub_addr8_t，可取值为：<br>&bull; VA0<br>&bull; VA1<br>&bull; VA2<br>&bull; VA3<br>&bull; VA4<br>&bull; VA5<br>&bull; VA6<br>&bull; VA7<br>数字代表寄存器顺序，使用方法请参考调用示例|
|src_array|输入|操作数地址序列。|

## 返回值说明

无

## 流水类型

PIPE_V

## 约束说明

- 操作数地址对齐约束请参考[通用地址对齐约束](../general_instruction.md#通用地址对齐约束)。

## 调用示例

```cpp
const uint32_t SRC_BASE_ADDR = 0x80000000;
const uint32_t DST_BASE_ADDR = 0x80000100;
const uint32_t STEP = 32;
// src_list和dst_list是16个DataBlock地址的数组
__ubuf__ half* src_list[16] = {
    (__ubuf__ half*)(SRC_BASE_ADDR + 0 * STEP),
    (__ubuf__ half*)(SRC_BASE_ADDR + 1 * STEP),
    (__ubuf__ half*)(SRC_BASE_ADDR + 2 * STEP),
    (__ubuf__ half*)(SRC_BASE_ADDR + 3 * STEP),
    (__ubuf__ half*)(SRC_BASE_ADDR + 4 * STEP),
    (__ubuf__ half*)(SRC_BASE_ADDR + 5 * STEP),
    (__ubuf__ half*)(SRC_BASE_ADDR + 6 * STEP),
    (__ubuf__ half*)(SRC_BASE_ADDR + 7 * STEP),
    (__ubuf__ half*)(SRC_BASE_ADDR + 8 * STEP),
    (__ubuf__ half*)(SRC_BASE_ADDR + 9 * STEP),
    (__ubuf__ half*)(SRC_BASE_ADDR + 10 * STEP),
    (__ubuf__ half*)(SRC_BASE_ADDR + 11 * STEP),
    (__ubuf__ half*)(SRC_BASE_ADDR + 12 * STEP),
    (__ubuf__ half*)(SRC_BASE_ADDR + 13 * STEP),
    (__ubuf__ half*)(SRC_BASE_ADDR + 14 * STEP),
    (__ubuf__ half*)(SRC_BASE_ADDR + 15 * STEP)
};
__ubuf__ half* dst_list[16] = {
    (__ubuf__ half*)(DST_BASE_ADDR + 0 * STEP),
    (__ubuf__ half*)(DST_BASE_ADDR + 1 * STEP),
    (__ubuf__ half*)(DST_BASE_ADDR + 2 * STEP),
    (__ubuf__ half*)(DST_BASE_ADDR + 3 * STEP),
    (__ubuf__ half*)(DST_BASE_ADDR + 4 * STEP),
    (__ubuf__ half*)(DST_BASE_ADDR + 5 * STEP),
    (__ubuf__ half*)(DST_BASE_ADDR + 6 * STEP),
    (__ubuf__ half*)(DST_BASE_ADDR + 7 * STEP),
    (__ubuf__ half*)(DST_BASE_ADDR + 8 * STEP),
    (__ubuf__ half*)(DST_BASE_ADDR + 9 * STEP),
    (__ubuf__ half*)(DST_BASE_ADDR + 10 * STEP),
    (__ubuf__ half*)(DST_BASE_ADDR + 11 * STEP),
    (__ubuf__ half*)(DST_BASE_ADDR + 12 * STEP),
    (__ubuf__ half*)(DST_BASE_ADDR + 13 * STEP),
    (__ubuf__ half*)(DST_BASE_ADDR + 14 * STEP),
    (__ubuf__ half*)(DST_BASE_ADDR + 15 * STEP)
};
const int32_t VA_REG_ARRAY_LEN = 8;

// 接口要求前8个和后8个地址序列与地址寄存器分别关联
asc_set_va_reg(VA0, dst_list);
asc_set_va_reg(VA1, dst_list + VA_REG_ARRAY_LEN);
asc_set_va_reg(VA2, src_list);
asc_set_va_reg(VA3, src_list + VA_REG_ARRAY_LEN);
```