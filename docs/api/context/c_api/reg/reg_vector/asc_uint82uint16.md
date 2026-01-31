# asc_uint82uint16

## 产品支持情况

| 产品     | 是否支持 |
| ----------- | :----: |
| Ascend 950PR/Ascend 950DT | √    |

## 功能说明

将vector_uint8_t类型的源操作数按照256B分为两部分，读取其中一部分元素，将其转换成vector_uint16_t类型并写入目的操作数。

- asc_uint82uint16：将源操作数的上半部分写入目的操作数。

- asc_uint82uint16_v2：将源操作数的下半部分写入目的操作数。

## 函数原型

```cpp
__simd_callee__ inline void asc_uint82uint16(vector_uint16_t& dst, vector_uint8_t src, vector_bool mask)
__simd_callee__ inline void asc_uint82uint16_v2(vector_uint16_t& dst, vector_uint8_t src, vector_bool mask)
```

## 参数说明

| 参数名       | 输入/输出 | 描述               |
| --------- | ----- | ---------------- |
| dst       | 输出    | 目的操作数。            |
| src | 输入    | 源操作数（矢量）的起始地址。            |
| mask     | 输入    | 用于指示在计算过程中哪些元素参与计算。        |

## 返回值说明

无

## 流水类型

PIPE_V

## 约束说明

- 操作数地址重叠约束请参考[通用地址重叠约束](../../general_instruction.md#通用地址重叠约束)。
- dst、src的起始地址需要32字节对齐。

## 调用示例

```cpp
vector_uint16_t dst;
vector_uint8_t src;
vector_bool mask;
asc_uint82uint16(dst, src, mask);    // 将src的上半部分转换成vector_uint16_t类型并写入dst
asc_uint82uint16_v2(dst, src, mask);    // 将src的下半部分转换成vector_uint16_t类型并写入dst
```