# asc_le_scalar

## AI处理器支持情况

|AI处理器类型|是否支持|
| :------------ | :------------: |
| Ascend 910C | √ |
| Ascend 910B | √ |

## 功能说明

按元素判断src <= value是否成立，若成立则输出结果为1，否则为0。每个元素的比较结果占1个bit。

## 函数原型

- 高维切分计算
    ```cpp
    __aicore__ inline void asc_le_scalar(__ubuf__ uint8_t* dst, __ubuf__ half* src, half value, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
    __aicore__ inline void asc_le_scalar(__ubuf__ uint8_t* dst, __ubuf__ float* src, float value, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
    ```

- 同步计算
    ```cpp
    __aicore__ inline void asc_le_scalar_sync(__ubuf__ uint8_t* dst, __ubuf__ half* src0, half value, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
    __aicore__ inline void asc_le_scalar_sync(__ubuf__ uint8_t* dst, __ubuf__ float* src, float value, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
    ```

## 参数说明

|参数名|输入/输出|描述|
| ------------ | ------------ | ------------ |
|dst|输出|目的操作数（矢量）的起始地址。|
|src|输入|源操作数（矢量）的起始地址。|
|value|输入|源操作数（标量）的起始地址。|
|config|输入|在高维切分场景下使用的计算配置参数。<br/>详细说明请参考[asc_unary_config](../struct/asc_unary_config.md)。|

## 返回值说明

无

## 流水类型

PIPE_TYPE_V

## 约束说明

- 操作数地址重叠约束请参考[通用地址重叠约束](../general_instruction.md#通用地址重叠约束)。
- dst、src的起始地址需要32字节对齐。
- dst按照小端顺序排序成二进制结果，对应src中相应位置的数据比较结果。
- 当结果输出到寄存器中时，固定处理256B的数据，config中的repeat字段不生效。

## 调用示例

```cpp
// 结果输出到寄存器中，固定为128个元素
uint32_t total_length = 128;
__ubuf__ half dst[total_length];
__ubuf__ half src[total_length];
half scalar = 20;
uint8_t repeat = 1;
uint8_t dst_block_stride = 1;
uint8_t src_block_stride = 1;
uint8_t dst_repeat_stride = 8;
uint8_t src_repeat_stride = 8;
…… // 数据搬运及同步操作
asc_le_scalar(dst, src, scalar, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
…… // 同步操作
```