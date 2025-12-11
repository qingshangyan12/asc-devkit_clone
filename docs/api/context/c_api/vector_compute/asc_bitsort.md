# asc_bitsort

## AI处理器支持情况

|AI处理器类型|是否支持|
| :------------ | :------------: |
| <term>Ascend 910C</term> | √ |
| <term>Ascend 910B</term> | √ |

## 功能说明

位图排序函数，一次迭代可以完成32个元素的排序，数据按如下描述结构进行保存：

Score和Index分别存储在src0和src1中，按Score进行排序（Score大的排前面），排序后的Score与其对应的Index一起以（Score，Index）的结构存储在dst中。

## 函数原型

- 前n个数据计算
  ```cpp
  __aicore__ inline void asc_bitsort(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ uint32_t* src1, uint32_t count)
  __aicore__ inline void asc_bitsort(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ uint32_t* src1, uint32_t count)
  ```

- 高维切分计算
  ```cpp
  __aicore__ inline void asc_bitsort(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ uint32_t* src1, const asc_binary_config& config)
  __aicore__ inline void asc_bitsort(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ uint32_t* src1, const asc_binary_config& config)
  ```

- 同步计算
  ```cpp
  __aicore__ inline void asc_bitsort_sync(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ uint32_t* src1, const asc_binary_config& config)
  __aicore__ inline void asc_bitsort_sync(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ uint32_t* src1, const asc_binary_config& config)
  ```

## 参数说明

| 参数名       | 输入/输出 | 描述                |
| --------- | ----- | ----------------- |
| dst       | 输出    | 目的操作数（矢量）的起始地址。 |
| src0、src1 | 输入    | 源操作数（矢量）的起始地址。 |
| count     | 输入    | 参与计算的元素个数。        |
| config    | 输入    | 在高维切分计算场景下使用的计算配置参数。<br/>详细说明请参考[asc_binary_config](../struct/asc_binary_config.md)。 |

## 返回值说明

无

## 流水类型

PIPE_TYPE_V

## 约束说明

- 操作数地址重叠约束请参考[通用地址重叠约束](../general_instruction.md#通用地址重叠约束)。
- dst、src的起始地址需要32字节对齐。

## 调用示例

```cpp
__ubuf__ half dst[256 * sizeof(half)];
__ubuf__ half src0[256 * sizeof(half)];
__ubuf__ half src1[256 * sizeof(uint32_t)];
asc_bitsort(dst, src0, src1, 256);
```