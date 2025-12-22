# asc_transpose_to_5hd

## AI处理器支持情况

|AI处理器类型|是否支持|
| :------------ | :------------: |
| <term>Ascend 910C</term> | √ |
| <term>Ascend 910B</term> | √ |

## 功能说明

数据格式转换，一般用于将NCHW格式转换成NC1HWC0格式。特别的，也可以用于二维矩阵数据块的转置。本接口单次Repeat内可处理512Byte的数据（16个DataBlock），根据数据类型不同，支持不同shape的矩阵转置（比如数据类型为half时，单次Repeat可完成16*16大小的矩阵转置），同时还可以支持多次Repeat操作。

## 函数原型

```cpp
__aicore__ inline void asc_set_va_reg(ub_addr8_t addr, uint64_t* src_array)
__aicore__ inline void asc_transdata_to_5hd_b32(ub_addr8_t dst, ub_addr8_t src, uint8_t repeat,
    uint16_t dst_stride, uint16_t src_stride)
__aicore__ inline void asc_transdata_to_5hd_b16(ub_addr8_t dst, ub_addr8_t src, uint8_t repeat,
    uint16_t dst_stride, uint16_t src_stride)
__aicore__ inline void asc_transdata_to_5hd_b8(ub_addr8_t dst, ub_addr8_t src, uint8_t repeat,
    uint16_t dst_stride, uint16_t src_stride, bool dst_high_half, bool src_high_half)
```

## 参数说明

|参数名|输入/输出|描述|
| ------------ | ------------ | ------------ |
|addr|输出|地址寄存器。|
|src_array|输入|操作数地址序列。|
|dst|输出|目的地址寄存器。|
|src|输入|源地址寄存器。|
|repeat|输入|重复迭代次数， repeat∈[0,255]。<br>注意事项：<br>&bull; 当repeat为1时，目的操作数/源操作数的有效起始位置为dst/src序列输入的起始位置加上dst_stride/src_stride。如果要让目的操作数/源操作数的有效起始位置为dst/src序列输入的起始位置，需要将dst_stride/src_stride置为0。<br>&bull;当repeat大于1时，第一次repeat中目的操作数/源操作数的有效起始位置为dst/src序列输入的起始位置，第二次会加上dst_stride/src_stride。以此类推。|
|dst_stride|输入|相邻迭代间，目的操作数相同DataBlock地址Stride，单位：DataBlock。<br>相邻迭代间相同DataBlock的地址步长参数的详细说明请参考[repeatStride](../general_instruction.md#repeatStride)。|
|src_stride|输入|相邻迭代间，源操作数相同DataBlock地址Stride，单位：DataBlock。<br>相邻迭代间相同DataBlock的地址步长参数的详细说明请参考[repeatStride](../general_instruction.md#repeatStride)。|
|dst_high_half|输入|指定每个dst地址中的数据存储到DataBlock的高半部还是低半部，该配置只配置int8_t/uint8_t的数据类型。<br>支持的数据类型为bool，有以下两种取值：<br>&bull; true:表示存储于DataBlock的高半部。<br>&bull; false:表示存储于DataBlock的低半部。|
|src_high_half|输入|指定每个src地址中的数据存储到DataBlock的高半部还是低半部，该配置只配置int8_t/uint8_t的数据类型。<br>支持的数据类型为bool，有以下两种取值：<br>&bull; true:表示存储于DataBlock的高半部。<br>&bull;false:表示存储于DataBlock的低半部。|

## 返回值说明

无

## 流水类型

PIPE_TYPE_V

## 约束说明

- 操作数地址对齐约束请参考[通用地址对齐约束](../general_instruction.md#通用地址对齐约束)。
- 操作数地址重叠约束请参考[通用地址重叠约束](../general_instruction.md#通用地址重叠约束)。
- 进行NCHW格式到NC1HWC0格式的转换时，一般用法是将src/dst中的每个元素配置为每个HW平面的起点。
- 为了性能更优，数据类型位宽为8位时建议先固定dst_high_half/src_high_half，再HW方向Repeat后，再改变dst_high_half/src_high_half。

## 调用示例

```cpp
__ubuf__ half dst_list[16];
__ubuf__ half src_list[16];
const int32_t VA_REG_ARRAY_LEN = 8;
uint8_t repeat = 16;
uint16_t dst_stride = 16;
uint16_t src_stride = 1;

asc_set_va_reg(VA0, dst_list);
asc_set_va_reg(VA1, dst_list + VA_REG_ARRAY_LEN);
asc_set_va_reg(VA2, src_list);
asc_set_va_reg(VA3, src_list + VA_REG_ARRAY_LEN);

asc_transpose_to_5hd_b16(VA0, VA2, repeat, dst_stride, src_stride);
```