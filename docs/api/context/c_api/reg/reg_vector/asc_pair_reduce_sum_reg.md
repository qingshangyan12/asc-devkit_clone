# asc_pa

## 产品支持情况

| 产品     | 是否支持 |
| ----------- |:----:|
| <term>Ascend 950PR/Ascend 950DT</term> | √ |

## 功能说明

执行矢量与运算。计算公式如下：

$$
dst_i = src0_i
$$

## 函数原型

- 前n个数据计算

    ```c++
    __simd_callee__ inline void asc_pair_reduce_sum(vector_half& dst, vector_half src, vector_bool mask)
    __simd_callee__ inline void asc_pair_reduce_sum(vector_float& dst, vector_float src, vector_bool mask)
    ```

## 参数说明

表1 参数说明

| 参数名       | 输入/输出 | 描述                |
| --------- | ----- | ----------------- |
| dst       | 输出    | 目的操作数。            |
| src | 输入    | 源操作数（矢量数据寄存器）。             |
| mask | 输入 | 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。 |

矢量数据寄存器和掩码寄存器的详细说明请参见[reg数据类型定义.md](../reg数据类型定义.md)。

## 返回值说明

无

## 流水类型

PIPE_V

## 约束说明

## 调用示例

```c++
vector_half dst;
vector_half src;
vector_bool mask;
asc_pair_reduce_sum(dst, src, mask);
```