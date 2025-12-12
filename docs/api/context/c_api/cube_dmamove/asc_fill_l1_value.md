
# asc_fill_l1_value

## AI处理器支持情况

| AI处理器类型         | 是否支持 |
| :-----------------------| :-----:|
| <term>Ascend 910C</term> | √ |
| <term>Ascend 910B</term> | √ |

### 功能说明

将L1 Buffer的Local Memory初始化为某一具体数值。

### 函数原型

- 常规计算

    ```cpp
    __aicore__ inline void asc_fill_l1_value(__cbuf__ half* dst, half value, const asc_fill_value_config& config)
    __aicore__ inline void asc_fill_l1_value(__cbuf__ half* dst, uint32_t value, const asc_fill_value_config& config)
    __aicore__ inline void asc_fill_l1_value(__cbuf__ float* dst, half value, const asc_fill_value_config& config)
    __aicore__ inline void asc_fill_l1_value(__cbuf__ float* dst, uint32_t value, const asc_fill_value_config& config)
    __aicore__ inline void asc_fill_l1_value(__cbuf__ int16_t* dst, half value, const asc_fill_value_config& config)
    __aicore__ inline void asc_fill_l1_value(__cbuf__ int16_t* dst, uint32_t value, const asc_fill_value_config& config)
    __aicore__ inline void asc_fill_l1_value(__cbuf__ int32_t* dst, half value, const asc_fill_value_config& config)
    __aicore__ inline void asc_fill_l1_value(__cbuf__ int32_t* dst, uint32_t value, const asc_fill_value_config& config)
    __aicore__ inline void asc_fill_l1_value(__cbuf__ uint16_t* dst, half value, const asc_fill_value_config& config)
    __aicore__ inline void asc_fill_l1_value(__cbuf__ uint16_t* dst, uint32_t value, const asc_fill_value_config& config)
    __aicore__ inline void asc_fill_l1_value(__cbuf__ uint32_t* dst, half value, const asc_fill_value_config& config)
    __aicore__ inline void asc_fill_l1_value(__cbuf__ uint32_t* dst, uint32_t value, const asc_fill_value_config& config)
    __aicore__ inline void asc_fill_l1_value(__cbuf__ bfloat16_t* dst, bfloat16_t value, const asc_fill_value_config& config)
    ```

- 同步计算

    ```cpp
    __aicore__ inline void asc_fill_l1_value_sync(__cbuf__ half* dst, half value, const asc_fill_value_config& config)
    __aicore__ inline void asc_fill_l1_value_sync(__cbuf__ half* dst, uint32_t value, const asc_fill_value_config& config)
    __aicore__ inline void asc_fill_l1_value_sync(__cbuf__ float* dst, half value, const asc_fill_value_config& config)
    __aicore__ inline void asc_fill_l1_value_sync(__cbuf__ float* dst, uint32_t value, const asc_fill_value_config& config)
    __aicore__ inline void asc_fill_l1_value_sync(__cbuf__ int16_t* dst, half value, const asc_fill_value_config& config)
    __aicore__ inline void asc_fill_l1_value_sync(__cbuf__ int16_t* dst, uint32_t value, const asc_fill_value_config& config)
    __aicore__ inline void asc_fill_l1_value_sync(__cbuf__ int32_t* dst, half value, const asc_fill_value_config& config)
    __aicore__ inline void asc_fill_l1_value_sync(__cbuf__ int32_t* dst, uint32_t value, const asc_fill_value_config& config)
    __aicore__ inline void asc_fill_l1_value_sync(__cbuf__ uint16_t* dst, half value, const asc_fill_value_config& config)
    __aicore__ inline void asc_fill_l1_value_sync(__cbuf__ uint16_t* dst, uint32_t value, const asc_fill_value_config& config)
    __aicore__ inline void asc_fill_l1_value_sync(__cbuf__ uint32_t* dst, half value, const asc_fill_value_config& config)
    __aicore__ inline void asc_fill_l1_value_sync(__cbuf__ uint32_t* dst, uint32_t value, const asc_fill_value_config& config)
    __aicore__ inline void asc_fill_l1_value_sync(__cbuf__ bfloat16_t* dst, bfloat16_t value, const asc_fill_value_config& config)
    ```

### 参数说明

| 参数名  | 输入/输出 | 描述 |
| :----- | :------- | :------- |
| dst | 输出 | 目的操作数（向量）的起始地址。 |
| value | 输入 | 源操作数（标量）。 |
| config | 输入 | 使用的初始化相关参数，详细说明请参考[asc_fill_value_config](../struct/asc_fill_value_config.md)。 |

### 返回值说明

无

### 流水类型

PIPE_TYPE_MTE1

### 约束说明

- dst的起始地址需要32字节对齐。
- 操作数地址重叠约束请参考[通用地址重叠约束](../general_instruction.md#通用地址重叠约束)。

### 调用示例

```cpp
uint64_t offset = 0;
half value = 1;
__cbuf__ half* dst = dst[offset];
asc_fill_l1_value(dst, value, config);
```