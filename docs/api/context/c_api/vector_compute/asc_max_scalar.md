# asc_max_scalar

## AI处理器支持情况

| AI处理器类型     | 是否支持 |
| ----------- | :----: |
| Ascend 910C | √    |
| Ascend 910B | √    |

## 功能说明

矢量src的逐个元素与标量value比较大小，接着按照对应的比特位将最大值存入dst中。计算公式如下：

$$
dst_i = max(src_i, value)
$$

## 函数原型

- 前n个数据计算

  ```cpp
    __aicore__ inline void asc_max_scalar(_ubuf__ int16_t* dst, _ubuf__ int16_t* src, int16_t value, uint32_t count)
    __aicore__ inline void asc_max_scalar(_ubuf__ half* dst, _ubuf__ half* src, half value, uint32_t count)
    __aicore__ inline void asc_max_scalar(_ubuf__ int32_t* dst, _ubuf__ int32_t* src, int32_t value, uint32_t count)
    __aicore__ inline void asc_max_scalar(_ubuf__ float* dst, _ubuf__ float* src, float value, uint32_t count)
  ```

- 高维切分计算

  ```cpp
    _aicore__ inline void asc_max_scalar(ubuf__ int16_t* dst, _ubuf__ int16_t* src, int16_t value, const asc_unary_config& config)
    __aicore__ inline void asc_max_scalar(_ubuf__ half* dst, _ubuf__ half* src, half value, const asc_unary_config& config)
    __aicore__ inline void asc_max_scalar(_ubuf__ int32_t* dst, _ubuf__ int32_t* src, int32_t value, const asc_unary_config& config)
    __aicore__ inline void asc_max_scalar(_ubuf__ float* dst, _ubuf__ float* src, float value, const asc_unary_config& config)
  ```

- 同步计算

  ```cpp
    __aicore__ inline void asc_max_scalar_sync(_ubuf__ int16_t* dst, _ubuf__ int16_t* src, int16_t value, uint32_t count)
    __aicore__ inline void asc_max_scalar_sync(_ubuf__ half* dst, _ubuf__ half* src, half value, uint32_t count)
    __aicore__ inline void asc_max_scalar_sync(_ubuf__ int32_t* dst, _ubuf__ int32_t* src, int32_t value, uint32_t count)
    __aicore__ inline void asc_max_scalar_sync(_ubuf__ float* dst, _ubuf__ float* src, float value, uint32_t count)
  ```

## 参数说明

| 参数名       | 输入/输出 | 描述               |
| --------- | ----- | ---------------- |
| dst       | 输出    | 目的操作数（矢量）的起始地址。            |
| src | 输入    | 源操作数（矢量）的起始地址。           |
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
half value = 10;
__ubuf__ half src[total_length];
__ubuf__ half dst[total_length];
asc_max_scalar(dst, src, value, total_length);
```