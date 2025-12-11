# asc_gt_scalar

## AI处理器支持情况

|AI处理器类型|是否支持|
| :------------ | :------------: |
| <term>Ascend 910C</term> | √ |
| <term>Ascend 910B</term> | √ |

## 功能说明

src0中的每个元素逐个与标量src1比较大小，如果某个位置上的元素大于src1，则输出结果dst上的对应比特位为1，否则为0。

## 函数原型

- 高维切分计算
  ```cpp
  __aicore__ inline void asc_gt_scalar(__ubuf__ uint8_t* dst, __ubuf__ half* src0, __ubuf__ half src1, const asc_unary_config& config)
  __aicore__ inline void asc_gt_scalar(__ubuf__ uint8_t* dst, __ubuf__ float* src0, __ubuf__ float src1, const asc_unary_config& config)
  ```

- 同步计算
  ```cpp
  __aicore__ inline void asc_gt_scalar_sync(__ubuf__ uint8_t* dst, __ubuf__ half* src0, __ubuf__ half src1, const asc_unary_config &config)
  __aicore__ inline void asc_gt_scalar_sync(__ubuf__ uint8_t* dst, __ubuf__ float* src0, __ubuf__ float src1, const asc_unary_config &config)
  ```

## 参数说明

| 参数名       | 输入/输出 | 描述                |
| --------- | ----- | ----------------- |
| dst       | 输出    | 目的操作数（矢量）的起始地址。 |
| src0       | 输入    | 源操作数（矢量）的起始地址。 |
| src1       | 输入    | 源操作数（标量）。          |
| config    | 输入    | 在高维切分计算场景下使用的计算配置参数。<br/>详细说明请参考[asc_unary_config](../struct/asc_unary_config.md)。 |

## 返回值说明

无

## 流水类型

PIPE_TYPE_V

## 约束说明

- 操作数地址重叠约束请参考[通用地址重叠约束](../general_instruction.md#通用地址重叠约束)。
- dst、src的起始地址需要32字节对齐。

## 调用示例

```cpp
__ubuf__ uint8_t dst[256 * sizeof(uint8_t)];
__ubuf__ half src0[256 * sizeof(half)];
half src1 = 1;
asc_gt_scalar(dst, src0, src1, 256);
```