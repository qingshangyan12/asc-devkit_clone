# asc_rsqrt

## AI处理器支持情况

| AI处理器类型 | 是否支持  |
| :----------------------- | :------: |
| <term>Ascend 910C</term> |    √     |
| <term>Ascend 910B</term> |    √     |

## 功能说明

按元素进行开方后取倒数的计算，计算公式如下：

$$
dst_i = 1/(sqrt(src_i))
$$

## 函数原型

- 前n个数据计算

  ```c++
  __aicore__ inline void asc_rsqrt(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
  __aicore__ inline void asc_rsqrt(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
  ```

- 高维切分计算

  ```c++
  __aicore__ inline void asc_rsqrt(__ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
  __aicore__ inline void asc_rsqrt(__ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
  ```

- 同步计算

  ```c++
  __aicore__ inline void asc_rsqrt_sync(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
  __aicore__ inline void asc_rsqrt_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
  ```

## 参数说明

表1 参数说明

| 参数名 | 输入/输出 | 描述                                                                            |
| :----| :-----|:------------------------------------------------------------------------------|
| dst | 输出 | 目的操作数起始地址。                                                                    |
| src  | 输入 | 源操作数起始地址。                                                                         |
| count | 输入 | 参与计算的元素个数。                                                                    |
| repeat | 输入 | 迭代次数，默认值：1。 |
| dst_block_stride | 输入 | 目的操作数单次迭代内不同DataBlock间地址步长，默认值：1。|
| src_block_stride | 输入 | 源操作数单次迭代内不同DataBlock间地址步长，默认值：1。|
| dst_repeat_stride | 输入 | 目的操作数相邻迭代间相同DataBlock的地址步长，默认值：8。 |
| src_repeat_stride | 输入 | 源操作数相邻迭代间相同DataBlock的地址步长，默认值：8。 |

## 返回值说明

无

## 流水类型

PIPE_TYPE_V

## 约束说明

- dst、src的起始地址需要32字节对齐。
- 操作数地址重叠约束请参考[通用地址重叠约束](../general_instruction.md#通用地址重叠约束)。

## 调用示例

```cpp
//total_length指参与计算的数据总长度
constexpr uint64_t total_length = 128;
__ubuf__ half* value[total_length];
__ubuf__ half* dst[total_length];
asc_rsqrt(dst, value, total_length);
```
