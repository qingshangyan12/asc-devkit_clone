# asc_cast_deq

## AI处理器支持情况

| AI处理器类型 | 是否支持  |
| :----------------------- | :------: |
| <term>Ascend 910C</term> |    √     |
| <term>Ascend 910B</term> |    √     |

## 功能说明

对输入做量化并进行精度转换。计算公式如下：

$$
dst_i = Deq(Cast(src_i, dst_i))
$$

## 函数原型

- 前n个数据计算

```c++
__aicore__ inline void asc_cast_deq(__ubuf__ int8_t* dst, __ubuf__ int16_t* src, uint32_t count)
__aicore__ inline void asc_cast_deq(__ubuf__ uint8_t* dst, __ubuf__ uint16_t* src, uint32_t count)
```

- 高维切分计算

```c++
__aicore__ inline void asc_cast_deq(__ubuf__ int8_t* dst, __ubuf__ int16_t* src, const asc_binary_config& config)
__aicore__ inline void asc_cast_deq(__ubuf__ uint8_t* dst, __ubuf__ uint16_t* src, const asc_binary_config& config)
```

- 同步计算

```c++
__aicore__ inline void asc_rsqrt_cast_deq(__ubuf__ int8_t* dst, __ubuf__ int16_t* src, uint32_t count)
__aicore__ inline void asc_rsqrt_cast_deq(__ubuf__ uint8_t* dst, __ubuf__ uint16_t* src, uint32_t count)
```

## 参数说明

表1 参数说明

| 参数名 | 输入/输出 | 描述 |
| :----| :-----| :-----|
| dst | 输出 | 目的操作数。 |
| src  | 输入 | 源操作数。|
| count | 输入 | 参与计算的元素个数。 |
| config | 输入 | 在高维切分计算场景下使用的计算配置参数。详细说明请参考[asc_unary_config](../struct/asc_unary_config.md)。|

## 返回值说明

无

## 流水类型

PIPE_TYPE_V

## 约束说明

- dst、src的起始地址需要32字节对齐
- 操作数地址重叠约束请参考[通用地址重叠约束](../general_instruction.md#通用地址重叠约束)。

## 调用示例

```cpp
//total_length指参与计算的数据总长度
half value = 10;
__ubuf__ half* value[total_length];
__ubuf__ half* dst[total_length];
asc_cast_deq(dst, value, total_length);
```
