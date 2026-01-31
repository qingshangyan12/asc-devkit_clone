# asc_and

## 产品支持情况

| 产品     | 是否支持 |
| ----------- |:----:|
| <term>Ascend 950PR/Ascend 950DT</term> | √ |

## 功能说明

执行矢量与运算。计算公式如下：

$$
dst_i = src0_i \quad \&  \quad src1_i
$$

## 函数原型
    ```c++
    __simd_callee__ inline void asc_and(_ubuf__ int16_t* dst, _ubuf__ int16_t* src0, _ubuf__ int16_t* src1, vector_bool mask)
    __simd_callee__ inline void asc_and(_ubuf__ uint16_t* dst, _ubuf__ uint16_t* src0, _ubuf__ uint16_t* src1, vector_bool mask)
    ```

## 参数说明

表1 参数说明

| 参数名       | 输入/输出 | 描述                |
| --------- | ----- | ----------------- |
| dst       | 输出    | 目的操作数。            |
| src0、src1 | 输入    | 源操作数（矢量）。类型为[矢量数据寄存器](../reg数据类型定义.md)。             |
| mask | 输入 | 源操作数掩码（矢量），类型为[掩码寄存器](../reg数据类型定义.md/#掩码寄存器)，用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。 |

## 返回值说明

无

## 流水类型

PIPE_V

## 约束说明

- 非饱和模式：输入数据超出目标数据范围时，会截断为目标数据格式，例如输入uint16_t值为256，输出uint8_t值为0。
- 饱和模式：输入数据超出目标数据范围时，返回输出类型的对应最值。

## 调用示例

```c++
constexpr uint64_t total_length = 64;
//total_length 指参与计算的数据长度
__ubuf__ int16_t src0[total_length];
__ubuf__ int16_t src1[total_length];
__ubuf__ int16_t dst[total_length];
asc_and(dst, src0, src1, total_length);
```