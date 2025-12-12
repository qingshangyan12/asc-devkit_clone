# asc_uint82half

## AI处理器支持情况

| AI处理器类型 | 是否支持  |
| :----------------------- | :------: |
| <term>Ascend 910C</term> |    √     |
| <term>Ascend 910B</term> |    √     |

## 功能说明

将uint8类型数据转换为half类型。

## 函数原型

- 前n个数据计算

    ```c++
    __aicore__ inline void asc_uint82half(__ubuf__ half* dst, __ubuf__ uint8_t* src, uint32_t count)
    ```

- 高维切分计算

    ```c++
    __aicore__ inline void asc_uint82half(__ubuf__ half* dst, __ubuf__ uint8_t* src, const asc_unary_config& config)
    ```

- 同步转换

    ```c++
    __aicore__ inline void asc_uint82half_sync(__ubuf__ half* dst, __ubuf__ uint8_t* src, uint32_t count)
    ```

## 参数说明

表1 参数说明
| 参数名 | 输入/输出 | 描述 |
| :--- | :--- | :--- |
| dst | 输出 | 目的操作数（向量）的起始地址。 |
| src | 输入 | 源操作数（向量）的起始地址。 |
| count | 输入 | 参与计算的元素个数。 |
| config | 输入 | 在高维切分场景下使用的计算配置参数。详细说明请参考[asc_unary_config](../struct/asc_unary_config.md) 。 |

## 返回值说明

无

## 流水类型

PIPE_TYPE_V

## 约束说明

- dst、src的起始地址需要32字节对齐。
- 操作数地址重叠约束请参考[通用地址重叠约束](../general_instruction.md#通用地址重叠约束)。

## 调用示例

```cpp
//total_length指参与转换的数据总长度
uint64_t total_length = 128;
uint64_t offset = 0;
__ubuf__ uint8_t* src = src[offset];
offset += total_length * sizeof(half);
__ubuf__ half* dst = dst[offset];
asc_uint82half(dst, src, total_length);
```
