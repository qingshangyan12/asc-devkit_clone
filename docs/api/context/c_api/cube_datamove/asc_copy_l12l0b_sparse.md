# asc_copy_l12l0b_sparse

## AI处理器支持情况

| AI处理器类型 | 是否支持  |
| :----------------------- | :------: |
| <term>Ascend 910C</term> |    √     |
| <term>Ascend 910B</term> |    √     |

## 功能说明

用于搬运存放在L1 Buffer里的512B大小的稠密权重矩阵到L0B Buffer里，同时读取128B大小的索引矩阵用于稠密矩阵的稀疏化。

索引矩阵在一个int8_t的地址中的排布是逆序排布的，例如：索引矩阵1 2 0 1 0 2 1 0，在地址中的排布为1 0 2 1 0 1 2 0，其中1 0 2 1（对应索引矩阵前四位1 2 0 1）拼成一个int8_t数据，0 1 2 0（对应索引矩阵后四位0 2 1 0）拼成一个int8_t数据。

## 函数原型

- 高维切分搬运
  ```c++
  __aicore__ inline void asc_copy_l12l0b_sparse(__cb__ int8_t* dst, __cbuf__ int8_t* src, __cbuf__ int8_t* index, const asc_copy_2d_config& config)
  ```

- 同步搬运
  ```c++
  __aicore__ inline void asc_copy_l12l0b_sparse_sync(__cb__ int8_t* dst, __cbuf__ int8_t* src, __cbuf__ int8_t* index, const asc_copy_2d_config& config)
  ```

## 参数说明

表1 参数说明
| 参数名 | 输入/输出 | 描述 |
| :--- | :--- | :--- |
| dst | 输出 | 目的L0B Buffer地址。 |
| src | 输入 | 源L1 Buffer地址。 |
| index | 输入 | 索引矩阵源L1 Buffer地址。 |
| config | 输入 | 在高维切分场景下使用的数据搬运配置参数。详细说明请参考[asc_copy_2d_config](../struct/asc_copy_2d_config.md) 。|

## 返回值说明

无

## 流水类型

PIPE_MTE1

## 约束说明

- dst、src的起始地址需要32字节对齐。
- config中的repeat为0表示不执行。
- config中的start_index不能小于0。
- 不支持转置功能。

## 调用示例

```cpp
__ubuf__ int8_t dst[256 * sizeof(int8_t)];
__ubuf__ int8_t src[256 * sizeof(int8_t)];
__ubuf__ int8_t index[64 * sizeof(int8_t)];
asc_copy_l12l0b_sparse(dst, src, index, config);
```
