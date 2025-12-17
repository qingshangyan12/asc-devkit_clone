
# asc_gather

## AI处理器支持情况

| AI处理器类型         | 是否支持 |
| :-----------------------| :-----:|
| <term>Ascend 910C</term> | √ |
| <term>Ascend 910B</term> | √ |

### 功能说明

将src按照给定的偏移按元素收集到dst中。

### 函数原型

- 前n个数据计算

    ```cpp
    __aicore__ inline void asc_gather(__ubuf__ uint16_t* dst, __ubuf__ uint32_t* src, uint32_t offset_addr, uint32_t count)
    __aicore__ inline void asc_gather(__ubuf__ uint32_t* dst, __ubuf__ uint32_t* src, uint32_t offset_addr, uint32_t count)
    ```

- 高维切分计算

    ```cpp
    __aicore__ inline void asc_gather(__ubuf__ uint16_t* dst, __ubuf__ uint32_t* src, const asc_gather_config& config)
    __aicore__ inline void asc_gather(__ubuf__ uint32_t* dst, __ubuf__ uint32_t* src, const asc_gather_config& config)
    ```

- 同步计算

    ```cpp
    __aicore__ inline void asc_gather_sync(__ubuf__ uint16_t* dst, __ubuf__ uint32_t* src, uint32_t offset_addr, uint32_t count)
    __aicore__ inline void asc_gather_sync(__ubuf__ uint32_t* dst, __ubuf__ uint32_t* src, uint32_t offset_addr, uint32_t count)
    ```

### 参数说明

| 参数名  | 输入/输出 | 描述 |
| :----- | :------- | :------- |
| dst | 输出 | 目的操作数（向量）的起始地址。 |
| src | 输入 | 源操作数（向量）的起始地址。 |
| offset_addr | 输入 | 源操作数的每个元素对应的地址偏移。 |
| count | 输入 | 参与计算的元素个数。 |
| config | 输入 | 指令使用的计算配置参数, 详细说明请参考[asc_gather_config](../struct/asc_gather_config.md)。 |

### 返回值说明

无

### 流水类型

PIPE_V

### 约束说明

- dst、src的起始地址需要32字节对齐。
- 操作数地址重叠约束请参考[通用地址重叠约束](../general_instruction.md#通用地址重叠约束)。

### 调用示例

```cpp
// total_length指参与计算的数据长度
uint64_t total_length = 128;
uint64_t offset = 0;
uint32_t offset_addr = 1;
__ubuf__ half* src = src[offset];
offset += total_length * sizeof(half);
__ubuf__ half* dst = dst[offset];
asc_gather(dst, src, offset_addr, total_length);
```