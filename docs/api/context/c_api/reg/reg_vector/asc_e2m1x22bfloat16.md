# asc_e2m1x22bfloat16

## 产品支持情况

| 产品     | 是否支持 |
| ----------- | :----: |
| <term>Ascend 950PR/Ascend 950DT</term> | √ |

## 功能说明

将fp4x2_e2m1_t类型转化为bfloat16_t类型。

## 函数原型

```cpp
// 写入索引0的位置
__simd_callee__ inline void asc_e2m1x22bfloat16(vector_bfloat16_t& dst, vector_fp4x2_e2m1_t src, vector_bool mask)
// 写入索引1的位置
__simd_callee__ inline void asc_e2m1x22bfloat16_v2(vector_bfloat16_t& dst, vector_fp4x2_e2m1_t src, vector_bool mask)
// 写入索引2的位置
__simd_callee__ inline void asc_e2m1x22bfloat16_v3(vector_bfloat16_t& dst, vector_fp4x2_e2m1_t src, vector_bool mask)
// 写入索引3的位置
__simd_callee__ inline void asc_e2m1x22bfloat16_v4(vector_bfloat16_t& dst, vector_fp4x2_e2m1_t src, vector_bool mask)
```

## 参数说明

| 参数名  | 输入/输出 | 描述 |
| :----- | :------- | :------- |
| dst | 输出 | 目的操作数（矢量数据寄存器）。 |
| src | 输入 | 源操作数（矢量数据寄存器）。 |
| mask | 输入 | 源操作数掩码（矢量寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。 |

矢量数据寄存器和掩码寄存器的详细说明请参见[reg数据类型定义.md](../reg数据类型定义.md)。

## 返回值说明

无

## 流水类型

PIPE_V

## 约束说明

对于位宽为4比1的数据类型转换，写入时，未写入3个相邻位置置0，例如写入4*N位时，4*N+1、4*N+2、4*N+3位置置0，写入4*N+1位时，4*N、4*N+2、4*N+3位置置0。

## 调用示例

```cpp
vector_fp4x2_e2m1_t src;
vector_bfloat16_t dst;
vector_bool mask;
asc_e2m1x22bfloat16(dst, src, mask);
```
