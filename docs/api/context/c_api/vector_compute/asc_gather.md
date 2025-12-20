
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
    __aicore__ inline void asc_gather(__ubuf__ uint16_t* dst, __ubuf__ uint32_t* src, uint32_t offset_addr, uint16_t dst_repeat_stride, uint8_t repeat)
    __aicore__ inline void asc_gather(__ubuf__ uint32_t* dst, __ubuf__ uint32_t* src, uint32_t offset_addr, uint16_t dst_repeat_stride, uint8_t repeat)
    ```

- 同步计算

    ```cpp
    __aicore__ inline void asc_gather_sync(__ubuf__ uint16_t* dst, __ubuf__ uint32_t* src, uint32_t offset_addr, uint32_t count)
    __aicore__ inline void asc_gather_sync(__ubuf__ uint32_t* dst, __ubuf__ uint32_t* src, uint32_t offset_addr, uint32_t count)
    ```

### 参数说明

| 参数名  | 输入/输出 | 描述 |
| :----- | :------- | :------- |
| dst | 输出 | 目的操作数（矢量）的起始地址。 |
| src | 输入 | 源操作数（矢量）的起始地址。 |
| offset_addr | 输入 | 源操作数的每个元素对应的地址偏移，单位为Bytes。 |
| count | 输入 | 参与计算的元素个数。 |
| dst_repeat_stride | 输入 | 目的操作数相邻迭代间相同DataBlock的地址步长。 |
| repeat | 输入 | 迭代次数。 |

### 返回值说明

无

### 流水类型

PIPE_V

### 约束说明

- offset_addr取值应保证src元素类型位宽对齐。
- src加上offset_addr偏移后的地址不能超出Unified Buffer (UB)大小数据的范围。
- dst、src的起始地址需要32字节对齐。
- 操作数地址重叠约束请参考[通用地址重叠约束](../general_instruction.md#通用地址重叠约束)。

### 调用示例

```cpp
constexpr uint64_t total_length = 128;    // total_length指参与计算的数据长度
uint32_t offset_addr = 4;
__ubuf__ half src[total_length * sizeof(half)];
__ubuf__ half dst[total_length * sizeof(half)];
asc_gather(dst, src, offset_addr, total_length);    // 将src中的每个元素按照4 Bytes（half类型则偏移两个元素）的偏移收集到dst中
```