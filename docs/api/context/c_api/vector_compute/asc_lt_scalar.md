# asc_lt_scalar

## AI处理器支持情况

| AI处理器类型         | 是否支持 |
| :-----------------------| :-----:|
| Ascend 910C| √ |
| Ascend 910B| √ |

### 功能说明

执行矢量中每个位置和标量比较，如果值小于标量值则为1，否则为0，结果为每个bit位按小端序排布。

### 函数原型

- 高维切分计算

    ```cpp
    __aicore__ inline void asc_lt_scalar(__ubuf__ uint8_t* dst, __ubuf__ half* src, half value, const asc_unary_config& config)
    __aicore__ inline void asc_lt_scalar(__ubuf__ uint8_t* dst, __ubuf__ int32_t* src, int32_t value, const asc_unary_config& config)
    __aicore__ inline void asc_lt_scalar(__ubuf__ uint8_t* dst, __ubuf__ float* src, float value, const asc_unary_config& config)
    ```

- 同步计算

    ```cpp
    __aicore__ inline void asc_lt_scalar_sync(__ubuf__ uint8_t* dst, __ubuf__ half* src, half value, const asc_unary_config& config)
    __aicore__ inline void asc_lt_scalar_sync(__ubuf__ uint8_t* dst, __ubuf__ int32_t* src, int32_t value, const asc_unary_config& config)
    __aicore__ inline void asc_lt_scalar_sync(__ubuf__ uint8_t* dst, __ubuf__ float* src, float value, const asc_unary_config& config)
    ```

### 参数说明

| 参数名  | 输入/输出 | 描述 |
| :----- | :------- | :------- |
| dst | 输出 | 目的操作数（向量）的起始地址。 |
| src | 输入 | 源操作数（向量）的起始地址。 |
| value | 输入 | 源操作数（标量）。 |
| config | 输入 | 在高维切分计算场景下使用的计算配置参数, 详细说明请参考[asc_unary_config](../struct/asc_unary_config.md)。 |

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
const uint64_t total_length = 64
uint64_t offset = 0;
half scalar_val = 1;
__ubuf__ half* src = src[offset];
offset += total_length * sizeof(half);
__ubuf__ uint8_t* dst = dst[offset];
asc_lt_scalar(dst, src, scalar_val, config);
```