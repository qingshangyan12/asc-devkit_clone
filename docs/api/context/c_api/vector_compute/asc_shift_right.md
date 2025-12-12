
# asc_shift_right

## AI处理器支持情况

| AI处理器类型         | 是否支持 |
| :-----------------------| :-----:|
| <term>Ascend 910C</term> | √ |
| <term>Ascend 910B</term> | √ |

### 功能说明

对src中的每个元素执行右移，右移大小为value。

### 函数原型

- 前n个数据计算

    ```cpp
    __aicore__ inline void asc_shift_right(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, int16_t value, uint32_t count, bool round)
    __aicore__ inline void asc_shift_right(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t value, uint32_t count, bool round)
    __aicore__ inline void asc_shift_right(__ubuf__ uint16_t* dst, __ubuf__ uint16_t* src, uint16_t value, uint32_t count, bool round)
    __aicore__ inline void asc_shift_right(__ubuf__ uint32_t* dst, __ubuf__ uint32_t* src, uint32_t value, uint32_t count, bool round)
    ```

- 高维切分计算

    ```cpp
    __aicore__ inline void asc_shift_right(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, int16_t value, const asc_unary_config& config, bool round)
    __aicore__ inline void asc_shift_right(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t value, const asc_unary_config& config, bool round)
    __aicore__ inline void asc_shift_right(__ubuf__ uint16_t* dst, __ubuf__ uint16_t* src, uint16_t value, const asc_unary_config& config, bool round)
    __aicore__ inline void asc_shift_right(__ubuf__ uint32_t* dst, __ubuf__ uint32_t* src, uint32_t value, const asc_unary_config& config, bool round)
    ```

- 同步计算

    ```cpp
    __aicore__ inline void asc_shift_right_sync(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, int16_t value, uint32_t count, bool round)
    __aicore__ inline void asc_shift_right_sync(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t value, uint32_t count, bool round)
    __aicore__ inline void asc_shift_right_sync(__ubuf__ uint16_t* dst, __ubuf__ uint16_t* src, uint16_t value, uint32_t count, bool round)
    __aicore__ inline void asc_shift_right_sync(__ubuf__ uint32_t* dst, __ubuf__ uint32_t* src, uint32_t value, uint32_t count, bool round)
    ```

### 参数说明

| 参数名  | 输入/输出 | 描述 |
| :----- | :------- | :------- |
| dst | 输出 | 目的操作数（向量）的起始地址。 |
| src | 输入 | 源操作数（向量）的起始地址。 |
| value | 输入 | 源操作数（标量）。 |
| count | 输入 | 参与计算的元素个数。 |
| config | 输入 | 在高维切分计算场景下使用的计算配置参数, 详细说明请参考[asc_unary_config](../struct/asc_unary_config.md)。 |
| round | 输入 | 右移时是否开启舍入。 |

### 返回值说明

无

### 流水类型

PIPE_TYPE_V

### 约束说明

- dst、src的起始地址需要32字节对齐。
- 操作数地址重叠约束请参考[通用地址重叠约束](../general_instruction.md#通用地址重叠约束)。

### 调用示例

```cpp
// total_length指参与计算的数据长度
uint64_t total_length = 128;
uint64_t offset = 0;
int16_t scalar_val = 1;
bool round = true;
__ubuf__ int16_t* src = src[offset];
offset += total_length * sizeof(half);
__ubuf__ int16_t* dst = dst[offset];
asc_shift_right(dst, src, scalar_val, total_length, round);
```