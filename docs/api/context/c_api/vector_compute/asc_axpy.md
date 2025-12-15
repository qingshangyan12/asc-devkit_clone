# asc_axpy

## AI处理器支持情况

|AI处理器类型|是否支持|
| :------------ | :------------: |
| <term>Ascend 910C</term> | √ |
| <term>Ascend 910B</term> | √ |

## 功能说明

源操作数src中每个元素与标量value求积后和目的操作数dst中的对应元素相加计算公式如下:

$$
dst = src * value + dst
$$

## 函数原型

- 连续数据计算

  ```cpp
  __aicore__ inline void asc_axpy(_ubuf__ half* dst, _ubuf__ half* src, half value, uint32_t count)
  __aicore__ inline void asc_axpy(_ubuf__ float* dst, _ubuf__ float* src, float value, uint32_t count)
  ```

- 高维切分计算

  ```cpp
  __aicore__ inline void asc_axpy(_ubuf__ half* dst, _ubuf__ half* src, half value, const asc_unary_config& config)
  __aicore__ inline void asc_axpy(_ubuf__ float* dst, _ubuf__ float* src, float value, const asc_unary_config& config)
  ```

- 同步计算

  ```cpp
  __aicore__ inline void asc_axpy_sync(_ubuf__ half* dst, _ubuf__ half* src, half value, uint32_t count)
  __aicore__ inline void asc_axpy_sync(_ubuf__ float* dst, _ubuf__ float* src, float value, uint32_t count)
  ```

## 参数说明

表1 参数说明

| 参数名       | 输入/输出 | 描述               |
| --------- | ----- | ---------------- |
| dst       | 输出    | 目的操作数（向量）的起始地址。            |
| src | 输入    | 源操作数（向量）的起始地址。            |
| value | 输入    | 源操作数（标量）。       |
| count     | 输入    | 参与连续计算的元素个数。      |
| config    | 输入    | 在高维切分计算场景下使用的计算配置参数。<br/>详细说明请参考[asc_unary_config](../struct/asc_unary_config.md)。 |


## 返回值说明

无

## 流水类型

PIPE_TYPE_V

## 约束说明

- 操作数地址重叠约束请参考[通用地址重叠约束](../general_instruction.md#通用地址重叠约束)。
- dst、src的起始地址需要32字节对齐。

## 调用示例

```cpp
//total_length 指参与计算的数据长度
uint64_t total_length = 32;
half value = 10;
uint64_t offset = 0;
__ubuf__ half* src =  src[offset];
offset += total_length * sizeof(half);
__ubuf__ half* dst = dst[offset];
asc_axpy(dst, src, value, total_length);
```
