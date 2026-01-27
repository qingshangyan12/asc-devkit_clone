# asc_ne_scalar

## 产品支持情况

| 产品         | 是否支持 |
| :-----------------------| :-----:|
| <term>Atlas A3 训练系列产品/Atlas A3 推理系列产品</term> | √ |
| <term>Atlas A2 训练系列产品/Atlas A2 推理系列产品</term> | √ |

## 功能说明

src中的每个元素逐个与标量value比较大小，如果某个位置上的元素不等于value，则输出结果dst对应比特位为1，否则为0。

## 函数原型

```cpp
__simd_callee__ inline void asc_ne_scalar(vector_bool& dst, vector_uint8_t src, uint8_t value, vector_bool mask)
__simd_callee__ inline void asc_ne_scalar(vector_bool& dst, vector_int8_t src, int8_t value, vector_bool mask)
__simd_callee__ inline void asc_ne_scalar(vector_bool& dst, vector_uint16_t src, uint16_t value, vector_bool mask)
__simd_callee__ inline void asc_ne_scalar(vector_bool& dst, vector_int16_t src, int16_t value, vector_bool mask)
__simd_callee__ inline void asc_ne_scalar(vector_bool& dst, vector_uint32_t src, uint32_t value, vector_bool mask)
__simd_callee__ inline void asc_ne_scalar(vector_bool& dst, vector_int32_t src, int32_t value, vector_bool mask)
__simd_callee__ inline void asc_ne_scalar(vector_bool& dst, vector_half src, half value, vector_bool mask)
__simd_callee__ inline void asc_ne_scalar(vector_bool& dst, vector_float src, float value, vector_bool mask)
__simd_callee__ inline void asc_ne_scalar(vector_bool& dst, vector_bfloat16_t src, bfloat16_t value, vector_bool mask)
```
## 参数说明

| 参数名  | 输入/输出 | 描述 |
| :----- | :------- | :------- |
| dst | 输出 | 目的操作数（矢量）的起始地址。 |
| src | 输入 | 源操作数（矢量）的起始地址。 |
| value | 输入 | 源操作数（标量）。 |
| mask | 输入 | 用于按位控制参与计算的元素，bit位值为1表示参与计算，0表示不参与。 |


## 返回值说明

无

## 约束说明

- dst、src的起始地址需要32字节对齐。
- 操作数地址重叠约束请参考[通用地址重叠约束](../general_instruction.md#通用地址重叠约束)。

## 调用示例

```cpp
// 结果输出到目标地址中，固定为128个元素
constexpr uint32_t total_length = 128;
__ubuf__ uint8_t dst[total_length / 8];
__ubuf__ half src[total_length];
half scalar = 20;
uint8_t repeat = 1;
uint8_t dst_block_stride = 1;
uint8_t src_block_stride = 1;
uint8_t dst_repeat_stride = 8;
uint8_t src_repeat_stride = 8;
…… // 数据搬运及同步操作
asc_ne_scalar(dst, src, scalar, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
…… // 同步操作
```