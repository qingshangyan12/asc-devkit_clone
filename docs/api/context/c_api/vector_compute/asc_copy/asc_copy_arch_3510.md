
# asc_copy

## 产品支持情况

| 产品         | 是否支持 |
| :-----------------------| :-----:|
| Ascend 950PR/Ascend 950DT | √    |

### 功能说明

将数据从RegTensor搬运到MaskReg。

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
| part | 输入 | 数据搬运阶段的偏移量。 |

### 返回值说明

无

### 流水类型

PIPE_V

### 约束说明

无

### 调用示例

```cpp
int16_t part = 128;
vector_uint16_t src;
vector_bool dst;
asc_copy(dst, src, total_length);
```