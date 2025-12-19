# asc_gt_scalar

## AI处理器支持情况

|AI处理器类型|是否支持|
| :------------ | :------------: |
| <term>Ascend 910C</term> | √ |
| <term>Ascend 910B</term> | √ |

## 功能说明

src中的每个元素逐个与标量value比较大小，如果某个位置上的元素大于value，则输出结果dst上的对应比特位为1，否则为0。

## 函数原型

- 高维切分计算
  ```cpp
  __aicore__ inline void asc_gt_scalar(__ubuf__ uint8_t* dst, __ubuf__ half* src, half value, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
  __aicore__ inline void asc_gt_scalar(__ubuf__ uint8_t* dst, __ubuf__ float* src, float value, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
  ```

- 同步计算
  ```cpp
  __aicore__ inline void asc_gt_scalar_sync(__ubuf__ uint8_t* dst, __ubuf__ half* src, half value, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
  __aicore__ inline void asc_gt_scalar_sync(__ubuf__ uint8_t* dst, __ubuf__ float* src, float value, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
  ```

## 参数说明

| 参数名       | 输入/输出 | 描述                |
| --------- | ----- | ----------------- |
| dst       | 输出    | 目的操作数（矢量）的起始地址。 |
| src       | 输入    | 源操作数（矢量）的起始地址。 |
| value       | 输入    | 源操作数（标量）。          |
| repeat | 输入    | 迭代次数，默认值：1。 |
| dst_block_stride | 输入    | 目的操作数单次迭代内不同DataBlock间地址步长，默认值：1。 |
| src_block_stride | 输入    | 源操作数单次迭代内不同DataBlock间地址步长，默认值：1。 |
| dst_repeat_stride | 输入    | 目的操作数相邻迭代间相同DataBlock的地址步长，默认值：8。 |
| src_repeat_stride | 输入    | 源操作数相邻迭代间相同DataBlock的地址步长，默认值：8。 |

## 返回值说明

无

## 流水类型

PIPE_V

## 约束说明

- 操作数地址重叠约束请参考[通用地址重叠约束](../general_instruction.md#通用地址重叠约束)。
- dst、src的起始地址需要32字节对齐。

## 调用示例

```cpp
__ubuf__ uint8_t dst[256 * sizeof(uint8_t)];
__ubuf__ half src[256 * sizeof(half)];
half value = 1;
asc_gt_scalar(dst, src, value, 1, 1, 1, 8, 8);
```