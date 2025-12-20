# asc_lt

## AI处理器支持情况

| AI处理器类型         | 是否支持 |
| :-----------------------| :-----:|
| <term>Ascend 910C</term> | √ |
| <term>Ascend 910B</term> | √ |

### 功能说明

按元素判断src0 < src1是否成立，若成立则输出结果dst上的对应比特位为1，否则为0。

### 函数原型


- 高维切分计算

  ```cpp
  //结果存入寄存器
  __aicore__ inline void asc_lt(__ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
  __aicore__ inline void asc_lt(__ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
  // 结果输出到目标地址中
  __aicore__ inline void asc_lt(__ubuf__ uint8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
  __aicore__ inline void asc_lt(__ubuf__ uint8_t* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
  ```

- 同步计算

  ```cpp
  //结果存入寄存器
  __aicore__ inline void asc_lt_sync(__ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
  __aicore__ inline void asc_lt_sync(__ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
  // 结果输出到目标地址中
  __aicore__ inline void asc_lt_sync(__ubuf__ uint8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
  __aicore__ inline void asc_lt_sync(__ubuf__ uint8_t* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
  ```

### 参数说明

| 参数名  | 输入/输出 | 描述 |
| ------------ | ------------ | ------------ |
| dst | 输出 | 目的操作数（矢量）的起始地址。 |
| src0 | 输入 | 源操作数（矢量）的起始地址。 |
| src1 | 输入 | 源操作数（矢量）的起始地址。 |
| repeat | 输入    | 迭代次数，默认值：1。 |
| dst_block_stride | 输入    | 目的操作数单次迭代内不同DataBlock间地址步长，默认值：1。 |
| src0_block_stride | 输入    | 源操作数0单次迭代内不同DataBlock间地址步长，默认值：1。 |
| src1_block_stride | 输入    | 源操作数1单次迭代内不同DataBlock间地址步长，默认值：1。 |
| dst_repeat_stride | 输入    | 目的操作数相邻迭代间相同DataBlock的地址步长，默认值：8。 |
| src0_repeat_stride | 输入    | 源操作数0相邻迭代间相同DataBlock的地址步长，默认值：8。 |
| src1_repeat_stride | 输入    | 源操作数1相邻迭代间相同DataBlock的地址步长，默认值：8。 |

### 返回值说明

无

### 流水类型

PIPE_TYPE_V

### 约束说明

- 操作数地址重叠约束请参考[通用地址重叠约束](../general_instruction.md#通用地址重叠约束)。
- dst、src的起始地址需要32字节对齐。
- dst或输出到寄存器中的结果均按照小端顺序排序成二进制结果，其中的每个bit对应src中相应位置的元素的比较结果。
- 当结果输出到寄存器中时，固定处理256B的数据，config中的repeat字段不生效。

### 调用示例

```cpp
constexpr uint64_t total_length = 128;    // total_length指参与计算的数据长度
__ubuf__ half src0[total_length * sizeof(half)];
__ubuf__ half src1[total_length * sizeof(half)];
// 通过寄存器获取比较结果
asc_lt(src0, src1, 1, 1, 1, 1, 8, 8, 8);
// 通过dst获取比较结果
__ubuf__ uint8_t dst[total_length * sizeof(uint8_t)];
asc_lt(dst, src0, src1, 1, 1, 1, 1, 8, 8, 8);
```



