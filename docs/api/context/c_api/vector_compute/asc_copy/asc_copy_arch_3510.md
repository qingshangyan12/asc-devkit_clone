
# asc_copy

## 产品支持情况

| 产品         | 是否支持 |
| :-----------------------| :-----:|
| Ascend 950PR/Ascend 950DT | √    |

### 功能说明

将数据从Unified Buffer搬运到Unified Buffer。

### 函数原型

- 
```cpp
    __simd_callee__ inline void asc_copy(vector_bool& dst, vector_uint16_t src, int16_t part)
    __simd_callee__ inline void asc_copy(vector_bool& dst, vector_uint32_t src, int16_t part)
```

### 参数说明

| 参数名  | 输入/输出 | 描述 |
| :----- | :------- | :------- |
| dst | 输出 | 目的操作数（矢量）的起始地址。 |
| src | 输入 | 源操作数（矢量）的起始地址。 |
| part | 输入 | 。 |

### 返回值说明

无

### 流水类型

PIPE_V

### 约束说明

- dst、src的起始地址需要32字节对齐。
- 操作数地址重叠约束请参考[通用地址重叠约束](../general_instruction.md#通用地址重叠约束)。

### 调用示例

```cpp
//total_length 指参与计算的数据长度
constexpr uint32_t total_length = 128;
__ubuf__ half src[total_length];
__ubuf__ half dst[total_length];
asc_copy(dst, src, part);
```