
# asc_add_relu_cast_f322f16

## AI处理器支持情况

| AI处理器类型     | 是否支持 |
| ----------- |:----:|
| Ascend 910C | √    |
| Ascend 910B | √    |

### 功能说明

按元素求和，结果和0对比取较大值，并将结果从float类型转换为half类型。计算公式如下：

$$
dst_i = Cast(max((src0_i+src1_i), 0),half)
$$

### 函数原型

- 前n个数据计算

    ```cpp
   __aicore__ inline void asc_add_relu_cast_f322f16(__ubuf__ half* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count)
    ```

- 高维切分计算

    ```cpp
    __aicore__ inline void asc_add_relu_cast_f322f16(__ubuf__ half* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
    ```

- 同步计算

    ```cpp
    __aicore__ inline void asc_add_relu_cast_f322f16_sync(__ubuf__ half* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count)
    ```

### 参数说明

| 参数名  | 输入/输出 | 描述 |
| :----- | :------- | :------- |
| dst | 输出 | 目的操作数（矢量）的起始地址。 |
| src0 | 输入 | 源操作数（矢量）的起始地址。 |
| src1 | 输入 | 源操作数（矢量）的起始地址。 |
| count | 输入 | 参与计算的元素个数。 |
| dst_block_stride | 输入 | 目的操作数单次迭代内不同DataBlock间地址步长，默认值：1。 |
| src0_block_stride | 输入 | 源操作数0单次迭代内不同DataBlock间地址步长，默认值：1。 |
| src1_block_stride | 输入 | 源操作数1单次迭代内不同DataBlock间地址步长，默认值：1。 |
| dst_repeat_stride | 输入 | 目的操作数相邻迭代间相同DataBlock的地址步长，默认值：8。 |
| src0_repeat_stride | 输入 | 源操作数0相邻迭代间相同DataBlock的地址步长，默认值：8。 |
| src1_repeat_stride | 输入 | 源操作数1相邻迭代间相同DataBlock的地址步长，默认值：8。 |
| repeat | 输入 | 迭代次数，默认值：1。 |

### 返回值说明

无

### 流水类型

PIPE_TYPE_V

### 约束说明

- dst、src0、src1的起始地址需要32字节对齐。
- 操作数地址重叠约束请参考[通用地址重叠约束](../general_instruction.md#通用地址重叠约束)。

### 调用示例

```cpp
// total_length指参与计算的数据长度
const uint32_t total_length = 32;
__ubuf__ half dst[total_length];
__ubuf__ half src0[total_length];
__ubuf__ half src1[total_length];
asc_add_relu_cast_f322f16(dst, src0, src1, total_length);
```