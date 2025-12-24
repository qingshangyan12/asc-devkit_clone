# asc_eq

## AI处理器支持情况

|AI处理器类型|是否支持|
| :------------ | :------------: |
| <term>Ascend 910C</term> | √ |
| <term>Ascend 910B</term> | √ |

## 功能说明

比较src0与src1在对应索引位置的元素大小。若比较结果为真，则输出结果的对应比特位设为1，否则设为0。

## 函数原型

- 高维切分计算
  ```cpp
  // 比较结果存入寄存器
  __aicore__ inline void asc_eq(__ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat,
    uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride,
    uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
  __aicore__ inline void asc_eq(__ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat,
    uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride,
    uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
  // 比较结果存入dst中
  __aicore__ inline void asc_eq(__ubuf__ uint8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat,
    uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride,
    uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
  __aicore__ inline void asc_eq(__ubuf__ uint8_t* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat,
    uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride,
    uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
  __aicore__ inline void asc_eq(__ubuf__ uint8_t* dst, __ubuf__ int32_t* src0, __ubuf__ int32_t* src1, uint8_t repeat,
    uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride,
    uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
  ```

- 同步计算
  ```cpp
  // 比较结果存入寄存器
  __aicore__ inline void asc_eq_sync(__ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat,
    uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride,
    uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
  __aicore__ inline void asc_eq_sync(__ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat,
    uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride,
    uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
  // 比较结果存入dst中
  __aicore__ inline void asc_eq_sync(__ubuf__ uint8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat,
    uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride,
    uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
  __aicore__ inline void asc_eq_sync(__ubuf__ uint8_t* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat,
    uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride,
    uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
  __aicore__ inline void asc_eq_sync(__ubuf__ uint8_t* dst, __ubuf__ int32_t* src0, __ubuf__ int32_t* src1,uint8_t repeat,
    uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride,
    uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
  ```

## 参数说明

|参数名|输入/输出|描述|
| ------------ | ------------ | ------------ |
|dst|输出|目的操作数（矢量）的起始地址。|
|src0、src1|输入|源操作数（矢量）的起始地址。|
| repeat | 输入 | 迭代次数。 |
| dst_block_stride | 输入 | 目的操作数单次迭代内不同DataBlock间地址步长。 |
| src0_block_stride、src1_block_stride | 输入 | 源操作数单次迭代内不同DataBlock间地址步长。 |
| dst_repeat_stride | 输入 | 目的操作数相邻迭代间相同DataBlock的地址步长。 |
| src0_repeat_stride、src1_repeat_stride | 输入 | 源操作数相邻迭代间相同DataBlock的地址步长。 |

## 返回值说明

无

## 流水类型

PIPE_V

## 约束说明

- 使用寄存器形式的接口时，需要配合[asc_get_cmp_mask](asc_get_cmp_mask.md)获取比较结果。
- 操作数地址重叠约束请参考[通用地址重叠约束](../general_instruction.md#通用地址重叠约束)。
- dst、src0、src1的起始地址需要32字节对齐。

## 调用示例

```cpp
// total_length指参与计算的数据总长度
constexpr int total_length = 256;
__ubuf__ half src0[total_length];
__ubuf__ half src1[total_length];
asc_unary_config config;
// 通过寄存器获取比较结果
asc_eq(src0, src1, config);
// 通过dst获取比较结果
__ubuf__ half dst[total_length];
asc_eq(dst, src0, src1, config);
// src0 : [1, 2, 3, ..., 256]
// src1 : [256, 255, 254, ..., 1]
// dst : [0, ..., 1, ..., 0]
```