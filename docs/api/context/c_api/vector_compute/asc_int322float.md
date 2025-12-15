# asc_int322float

## AI处理器支持情况

| AI处理器类型 | 是否支持 |
| :-----------| :-----:|
| Ascend 910C |   √   |
| Ascend 910B |   √   |

## 功能说明

将int32_t类型数据转换为float类型，支持多种舍入模式：

- RINT舍入模式：四舍六入五成双舍入。
- ROUND舍入模式：四舍五入舍入。
- FLOOR舍入模式：向负无穷舍入。
- CEIL舍入模式：向正无穷舍入。
- TRUNC舍入模式：向零舍入。

## 函数原型

- 前n个数据计算

  ```cpp
  // 在转换有精度损失时表示RINT舍入模式，不涉及精度损失时表示不舍入
  __aicore__ inline void asc_int322float(__ubuf__ float* dst, __ubuf__ int32_t* src, uint32_t count)
  // RINT舍入模式
  __aicore__ inline void asc_int322float_r(__ubuf__ float* dst, __ubuf__ int32_t* src, uint32_t count)
  // ROUND舍入模式
  __aicore__ inline void asc_int322float_a(__ubuf__ float* dst, __ubuf__ int32_t* src, uint32_t count)
  // FLOOR舍入模式
  __aicore__ inline void asc_int322float_f(__ubuf__ float* dst, __ubuf__ int32_t* src, uint32_t count)
  // CEIL舍入模式
  __aicore__ inline void asc_int322float_c(__ubuf__ float* dst, __ubuf__ int32_t* src, uint32_t count)
  // TRUNC舍入模式
  __aicore__ inline void asc_int322float_z(__ubuf__ float* dst, __ubuf__ int32_t* src, uint32_t count)
  ```

- 高维切分计算

  ```cpp
  // 在转换有精度损失时表示RINT舍入模式，不涉及精度损失时表示不舍入
  __aicore__ inline void asc_int322float(__ubuf__ float* dst, __ubuf__ int32_t* src, const asc_unary_config& config)
  // RINT舍入模式
  __aicore__ inline void asc_int322float_r(__ubuf__ float* dst, __ubuf__ int32_t* src, const asc_unary_config& config)
  // ROUND舍入模式
  __aicore__ inline void asc_int322float_a(__ubuf__ float* dst, __ubuf__ int32_t* src, const asc_unary_config& config)
  // FLOOR舍入模式
  __aicore__ inline void asc_int322float_f(__ubuf__ float* dst, __ubuf__ int32_t* src, const asc_unary_config& config)
  // CEIL舍入模式
  __aicore__ inline void asc_int322float_c(__ubuf__ float* dst, __ubuf__ int32_t* src, const asc_unary_config& config)
  // TRUNC舍入模式
  __aicore__ inline void asc_int322float_z(__ubuf__ float* dst, __ubuf__ int32_t* src, const asc_unary_config& config)
  ```

- 同步计算

    ```cpp
  // 在转换有精度损失时表示RINT舍入模式，不涉及精度损失时表示不舍入
  __aicore__ inline void asc_int322float_sync(__ubuf__ float* dst, __ubuf__ int32_t* src, uint32_t count)
  // RINT舍入模式
  __aicore__ inline void asc_int322float_r_sync(__ubuf__ float* dst, __ubuf__ int32_t* src, uint32_t count)
  // ROUND舍入模式
  __aicore__ inline void asc_int322float_a_sync(__ubuf__ float* dst, __ubuf__ int32_t* src, uint32_t count)
  // FLOOR舍入模式
  __aicore__ inline void asc_int322float_f_sync(__ubuf__ float* dst, __ubuf__ int32_t* src, uint32_t count)
  // CEIL舍入模式
  __aicore__ inline void asc_int322float_c_sync(__ubuf__ float* dst, __ubuf__ int32_t* src, uint32_t count)
  // TRUNC舍入模式
  __aicore__ inline void asc_int322float_z_sync(__ubuf__ float* dst, __ubuf__ int32_t* src, uint32_t count)
  ```

## 参数说明

表1 参数说明

| 参数名 | 输入/输出 | 描述 |
| :----| :-----| :-----|
| dst | 输出 | 目的操作数（矢量）的起始地址。 |
| src  | 输入 | 源操作数（矢量）的起始地址。|
| count | 输入 | 参与计算的元素个数。 |
| config | 输入 | 在高维切分计算场景下使用的计算配置参数，详细说明请参考[asc_unary_config](../struct/asc_unary_config.md)。|

## 返回值说明

无

## 流水类型

PIPE_TYPE_V

## 约束说明

- dst、src的起始地址需要32字节对齐。
- 操作数地址重叠约束请参考[通用地址重叠约束](../general_instruction.md#通用地址重叠约束)。

## 调用示例

```cpp
// total_length指参与计算的数据长度
const uint32_t total_length = 32;
__ubuf__ half dst[total_length];
__ubuf__ half src[total_length];
asc_int322float(dst, src, total_length);
```
