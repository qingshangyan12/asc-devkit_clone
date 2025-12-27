# asc_reduce

## 产品支持情况

| 产品 | 是否支持  |
| :----------------------- | :------: |
| <term>Atlas A3 训练系列产品/Atlas A3 推理系列产品</term> |    √     |
| <term>Atlas A2 训练系列产品/Atlas A2 推理系列产品</term> |    √     |

## 功能说明

Reduce（归约）是将一组数据通过指定运算聚合（聚合指将将一组分散、多个的数据通过某种规则合并成一个或少数几个的操作）为单个、少量结果的核心操作，包括ReduceMax、ReduceMin和ReduceSum等。 \
以ReduceMax为例，其核心规则是从一组数据中找出最大值（将多个数据聚合为一个最大值）。

## 函数原型

- 前n个数据计算

    ```c++
    __aicore__ inline void asc_reduce(__ubuf__ uint16_t* dst, __ubuf__ uint16_t* src0, __ubuf uint16_t* src1, uint32_t count)
    __aicore__ inline void asc_reduce(__ubuf__ uint32_t* dst, __ubuf__ uint32_t* src0, __ubuf uint32_t* src1, uint32_t count)
    ```

- 高维切分计算

  ```c++
  __aicore__ inline void asc_reduce(__ubuf__ uint16_t* dst, __ubuf__ uint16_t* src0, __ubuf uint16_t* src1,
    uint16_t dst_repeat_stride, uint8_t src_block_stride, uint8_t src_repeat_stride, uint16_t reserved, uint8_t repeat)
  __aicore__ inline void asc_reduce(__ubuf__ uint32_t* dst, __ubuf__ uint32_t* src0, __ubuf uint32_t* src1,
    uint16_t dst_repeat_stride, uint8_t src_block_stride, uint8_t src_repeat_stride, uint16_t reserved, uint8_t repeat)
  ```

- 同步计算

  ```c++
  __aicore__ inline void asc_reduce_sync(__ubuf__ uint16_t* dst, __ubuf__ uint16_t* src0, __ubuf uint16_t* src1, uint32_t count)
  __aicore__ inline void asc_reduce_sync(__ubuf__ uint32_t* dst, __ubuf__ uint32_t* src0, __ubuf uint32_t* src1, uint32_t count)
  ```

## 参数说明

表1 参数说明

| 参数名       | 输入/输出 | 描述                                                                              |
|:----------| :-----|:--------------------------------------------------------------------------------|
| dst       | 输出 | 目的操作数的起始地址。                                                                     |
| src0、src1 | 输入 | 源操作数的起始地址。                                                                      |
| count     | 输入 | 参与计算的元素个数。                                                                      |
| dst_block_stride | 输入 | 目的操作数单次迭代内不同DataBlock间地址步长，默认值：1。|
| src_block_stride | 输入 | 源操作数单次迭代内不同DataBlock间地址步长，默认值：1。|
| dst_repeat_stride | 输入 | 目的操作数相邻迭代间相同DataBlock的地址步长，默认值：8。 |
| src_repeat_stride | 输入 | 源操作数相邻迭代间相同DataBlock的地址步长，默认值：8。 |
| repeat | 输入 | 迭代次数，默认值：1。 |

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
__ubuf__ uint_16* value0[total_length];
__ubuf__ uint_16* value1[total_length];
__ubuf__ uint_16* dst[total_length];
asc_reduce(dst, value0, value1, total_length);
```
