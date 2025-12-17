# asc_sub_relu_cast

## AI处理器支持情况

| AI处理器类型     | 是否支持 |
| ----------- |:----:|
| Ascend 910C | √    |
| Ascend 910B | √    |

## 功能说明

执行矢量减运算，并将结果进行Relu和类型转换。计算公式如下：

$$
dst_i = Cast(ReLU(src0_i - src1_i))
$$

## 函数原型

- 前n个数据计算

```c++
__aicore__ inline void asc_sub_relu_cast(_ubuf__ int8_t* dst, _ubuf__ half* src0, _ubuf__ half* src1, uint32_t count)
__aicore__ inline void asc_sub_relu_cast(_ubuf__ half* dst, _ubuf__ float* src0, _ubuf__ float* src1, uint32_t count)
__aicore__ inline void asc_sub_relu_cast(_ubuf__ int8_t* dst, _ubuf__ int16_t* src0, _ubuf__ int16_t* src1, uint32_t count)
```

- 高维切分计算

```cpp
__aicore__ inline void asc_sub_relu_cast(_ubuf__ int8_t* dst, _ubuf__ half* src0, _ubuf__ half* src1, const asc_binary_config& config)
__aicore__ inline void asc_sub_relu_cast(_ubuf__ half* dst, _ubuf__ float* src0, _ubuf__ float* src1, const asc_binary_config& config)
__aicore__ inline void asc_sub_relu_cast(_ubuf__ int8_t* dst, _ubuf__ int16_t* src0, _ubuf__ int16_t* src1, const asc_binary_config& config)
```

- 同步计算

```cpp
__aicore__ inline void asc_sub_relu_cast_sync(_ubuf__ int8_t* dst, _ubuf__ half* src0, _ubuf__ half* src1, uint32_t count)
__aicore__ inline void asc_sub_relu_cast_sync(_ubuf__ half* dst, _ubuf__ float* src0, _ubuf__ float* src1, uint32_t count)
__aicore__ inline void asc_sub_relu_cast_sync(_ubuf__ int8_t* dst, _ubuf__ int16_t* src0, _ubuf__ int16_t* src1, uint32_t count)
```

## 参数说明


| 参数名       | 输入/输出 | 描述                |
| --------- | ----- | ----------------- |
| dst       | 输出    | 目的操作数（向量）的起始地址。            |
| src0、src1 | 输入    | 源操作数（向量）的起始地址。              |
| count     | 输入    | 参与计算的元素个数。        |
| config    | 输入    | 在高维切分计算场景下使用的计算配置参数。<br/>详细说明请参考[asc_binary_config](../struct/asc_binary_config.md)。 |

## 返回值说明

无

## 流水类型

PIPE_V

## 约束说明

- 操作数地址重叠约束请参考[通用地址重叠约束](../general_instruction.md#通用地址重叠约束)。
- dst、src的起始地址需要32字节对齐。

## 调用示例

```c++
//total_length 指参与计算的数据长度
const uint64_t offset = 0;
__ubuf__ int16_t src0[total_length];
__ubuf__ int16_t src1[total_length];
__ubuf__ int8_t dst[total_length];
asc_sub_relu_cast(dst, src0, src1, total_length);
```