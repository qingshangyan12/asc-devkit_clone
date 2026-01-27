# asc_ne

## 产品支持情况

|产品|是否支持|
| :------------ | :------------: |
| Atlas A3 训练系列产品/Atlas A3 推理系列产品 | √ |
| Atlas A2 训练系列产品/Atlas A2 推理系列产品 | √ |

## 功能说明

按元素判断src0 != src1是否成立，若成立则输出结果为1，否则为0。每个元素的比较结果占1个bit。

## 函数原型


```cpp
__simd_callee__ inline void asc_ne(vector_bool& dst, vector_uint8_t src0, vector_uint8_t src1, vector_bool mask)
__simd_callee__ inline void asc_ne(vector_bool& dst, vector_int8_t src0, vector_int8_t src1, vector_bool mask)
__simd_callee__ inline void asc_ne(vector_bool& dst, vector_uint16_t src0, vector_uint16_t src1, vector_bool mask)
__simd_callee__ inline void asc_ne(vector_bool& dst, vector_int16_t src0, vector_int16_t src1, vector_bool mask)
__simd_callee__ inline void asc_ne(vector_bool& dst, vector_uint32_t src0, vector_uint32_t src1, vector_bool mask)
__simd_callee__ inline void asc_ne(vector_bool& dst, vector_int32_t src0, vector_int32_t src1, vector_bool mask)
__simd_callee__ inline void asc_ne(vector_bool& dst, vector_half src0, vector_half src1, vector_bool mask)
__simd_callee__ inline void asc_ne(vector_bool& dst, vector_float src0, vector_float src1, vector_bool mask)
```

## 参数说明

|参数名|输入/输出|描述|
| ------------ | ------------ | ------------ |
|dst| 输出 |目的操作数（矢量）的起始地址。|
|src0| 输入 |源操作数（矢量）的起始地址。|
|src1| 输入 |源操作数（矢量）的起始地址。|
| mask | 输入 | 用于按位控制参与计算的元素，bit位值为1表示参与计算，0表示不参与。 |

## 返回值说明

无

## 约束说明

- 操作数地址重叠约束请参考[通用地址重叠约束](../general_instruction.md#通用地址重叠约束)。
- dst、src的起始地址需要32字节对齐。
- dst按照小端顺序排序成二进制结果，对应src中相应位置的数据比较结果。


## 调用示例

```cpp
// 输入固定为128个元素
constexpr uint32_t total_length = 128;
__ubuf__ uint8_t dst[total_length / 8];
__ubuf__ half src0[total_length];
__ubuf__ half src1[total_length];
uint8_t repeat = 1;
uint8_t dst_block_stride = 1;
uint8_t src0_block_stride = 1;
uint8_t src1_block_stride = 1;
uint8_t dst_repeat_stride = 8;
uint8_t src0_repeat_stride = 8;
uint8_t src1_repeat_stride = 8;
…… // 数据搬运及同步操作

// 结果存入寄存器，通过寄存器获取比较结果
asc_ne(src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride, src0_repeat_stride, src1_repeat_stride);
…… // 同步操作
asc_get_cmp_mask(dst); // 读取结果

// 结果输出到目标地址中
asc_ne(dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride, src0_repeat_stride, src1_repeat_stride);
```