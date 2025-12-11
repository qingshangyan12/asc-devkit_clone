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
  __aicore__ inline void asc_eq(__ubuf__ half* src0, __ubuf__ half* src1, const asc_unary_config& config)
  __aicore__ inline void asc_eq(__ubuf__ float* src0, __ubuf__ float* src1, const asc_unary_config& config)
  // 比较结果存入dst中
  __aicore__ inline void asc_eq(__ubuf__ uint8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, const asc_unary_config& config)
  __aicore__ inline void asc_eq(__ubuf__ uint8_t* dst, __ubuf__ float* src0, __ubuf__ float* src1, const asc_unary_config& config)
  __aicore__ inline void asc_eq(__ubuf__ uint8_t* dst, __ubuf__ int32_t* src0, __ubuf__ int32_t* src1, const asc_unary_config& config)
```

- 同步计算
```cpp
  // 比较结果存入寄存器
  __aicore__ inline void asc_eq_sync(__ubuf__ half* src0, __ubuf__ half* src1, const asc_unary_config& config)
  __aicore__ inline void asc_eq_sync(__ubuf__ float* src0, __ubuf__ float* src1, const asc_unary_config& config)
  // 比较结果存入dst中
  __aicore__ inline void asc_eq_sync(__ubuf__ uint8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, const asc_unary_config& config)
  __aicore__ inline void asc_eq_sync(__ubuf__ uint8_t* dst, __ubuf__ float* src0, __ubuf__ float* src1, const asc_unary_config& config)
  __aicore__ inline void asc_eq_sync(__ubuf__ uint8_t* dst, __ubuf__ int32_t* src0, __ubuf__ int32_t* src1, const asc_unary_config& config)
```

## 参数说明

|参数名|输入/输出|描述|
| ------------ | ------------ | ------------ |
|dst|输出|目的操作数(矢量)的起始地址。|
|src0,src1|输入|源操作数(矢量)的起始地址。|
|config|输入|在非连续场景下使用的计算配置参数。<br/>详细说明请参考[asc_unary_config](../struct/asc_unary_config.md)。|

## 返回值说明

无

## 流水类型

PIPE_TYPE_V

## 约束说明

- 操作数地址重叠约束请参考[通用地址重叠约束](../general_instruction.md#通用地址重叠约束)。
- dst、src0、src1的起始地址需要32字节对齐。

## 调用示例

```cpp
// total_length指参与计算的数据总长度
__ubuf__ half src0[total_length];
__ubuf__ half src1[total_length];
asc_unary_config config;
// 通过寄存器获取比较结果
asc_eq(src0, src1, config);
// 通过dst获取比较结果
__ubuf__ half dst[total_length];
asc_eq(dst, src0, src1, config);
```