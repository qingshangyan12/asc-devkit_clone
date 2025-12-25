# asc_set_va_reg

## AI处理器支持情况

|AI处理器类型|是否支持|
| :------------ | :------------: |
| <term>Ascend 910C</term> | √ |
| <term>Ascend 910B</term> | √ |

## 功能说明

将操作数地址序列与地址寄存器关联。

## 函数原型

```cpp
__aicore__ inline void asc_set_va_reg(ub_addr8_t addr, uint64_t* src_array)
```

## 参数说明

|参数名|输入/输出|描述|
| ------------ | ------------ | ------------ |
|addr|输出|地址寄存器。|
|src_array|输入|操作数地址序列。|

## 返回值说明

无

## 流水类型

PIPE_TYPE_V

## 约束说明

- 操作数地址对齐约束请参考[通用地址对齐约束](../general_instruction.md#通用地址对齐约束)。

## 调用示例

```cpp
__ubuf__ half dst_list[16];
__ubuf__ half src_list[16];
const int32_t VA_REG_ARRAY_LEN = 8;

asc_set_va_reg(VA0, dst_list);
asc_set_va_reg(VA1, dst_list + VA_REG_ARRAY_LEN);
asc_set_va_reg(VA2, src_list);
asc_set_va_reg(VA3, src_list + VA_REG_ARRAY_LEN);
```