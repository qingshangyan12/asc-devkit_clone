# asc_ge

## AI处理器支持情况

| AI处理器类型 | 是否支持  |
| :----------------------- | :------: |
| <term>Ascend 910C</term> |    √     |
| <term>Ascend 910B</term> |    √     |

## 功能说明

Ge（greater than or equal to），逐元素比较src0 >= src1是否成立，成立则输出结果为1，否则输出结果为0，每个元素的比较结果占一个bit。计算公式如下：

$$
dst = scr0 >= scr1
$$

## 函数原型
- 
- 高维切分计算

    ```c++
    __aicore__ inline void asc_ge(__ubuf__ half src0, __ubuf__ half src1, uint8_t repeat, uint8_t dst_block_stride,
        uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, int8_t src0_repeat_stride,
        int8_t src1_repeat_stride);
    __aicore__ inline void asc_ge(__ubuf__ float src0, __ubuf__ float src1, uint8_t repeat, uint8_t dst_block_stride,
        uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, int8_t src0_repeat_stride,
        int8_t src1_repeat_stride);
    __aicore__ inline void asc_ge(__ubuf__ half dst, __ubuf__ half src0, __ubuf__ half src1, uint8_t repeat, 
        uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, 
        int8_t src0_repeat_stride, int8_t src1_repeat_stride);
    __aicore__ inline void asc_ge(__ubuf__ float dst, __ubuf__ float src0, __ubuf__ float src1, uint8_t repeat, 
        uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, 
        int8_t src0_repeat_stride, int8_t src1_repeat_stride);
    ```

- 同步计算

    ```c++
    __aicore__ inline void asc_ge_sync(__ubuf__ half src0, __ubuf__ half src1, __ubuf__ half* src1, uint8_t repeat, 
        uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, 
        int8_t src0_repeat_stride, int8_t src1_repeat_stride);
    __aicore__ inline void asc_ge_sync(__ubuf__ float src0, __ubuf__ float src1, __ubuf__ half* src1, uint8_t repeat, 
        uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, 
        int8_t src0_repeat_stride, int8_t src1_repeat_stride);
    __aicore__ inline void asc_ge_sync(__ubuf__ half dst, __ubuf__ half src0, __ubuf__ half src1, __ubuf__ half* src1, 
        uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, 
        int8_t src0_repeat_stride, int8_t src1_repeat_stride);
    __aicore__ inline void asc_ge_sync(__ubuf__ float dst, __ubuf__ float src0, __ubuf__ float src1, __ubuf__ half* src1, 
        uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, 
        int8_t src0_repeat_stride, int8_t src1_repeat_stride);
    ```

## 参数说明

表1 参数说明

| 参数名       | 输入/输出 | 描述                                                                              |
|:----------| :-----|:--------------------------------------------------------------------------------|
| dst       | 输出 | 目的操作数的起始地址。                                                                     |
| src0、src1 | 输入 | 源操作数的起始地址。                                                                      |
| count     | 输入 | 参与计算的元素个数。                                                                      |
| repeat | 输入 | 迭代次数，默认值：1。 |
| dst_block_stride | 输入 | 目的操作数单次迭代内不同DataBlock间地址步长，默认值：1。 |
| src0_block_stride | 输入 | 源操作数0单次迭代内不同DataBlock间地址步长，默认值：1。 |
| src1_block_stride | 输入 | 源操作数1单次迭代内不同DataBlock间地址步长，默认值：1。 |
| dst_repeat_stride | 输入 | 目的操作数相邻迭代间相同DataBlock的地址步长，默认值：8。 |
| src0_repeat_stride | 输入 | 源操作数0相邻迭代间相同DataBlock的地址步长，默认值：8。 |
| src1_repeat_stride | 输入 | 源操作数1相邻迭代间相同DataBlock的地址步长，默认值：8。 |

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
__ubuf__ half* value0[total_length];
__ubuf__ half* value1[total_length];
__ubuf__ half* dst[total_length];
asc_ge(dst, value0, value1, total_length);
```
