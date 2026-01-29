# asc_div

## 产品支持情况

| 产品 | 是否支持  |
| :-----------| :------: |
| Ascend 950PR/Ascend 950DT | √ |

## 功能说明

按元素求商。计算公式如下：

$$
dst_i = src0_i ÷ src1_i
$$

## 函数原型

```cpp
__simd_callee__ inline void asc_div(vector_half &dst, vector_half src0, vector_half src1, vector_bool mask)
__simd_callee__ inline void asc_div(vector_float &dst, vector_float src0, vector_float src1, vector_bool mask)
```
## 参数说明

| 参数名 | 输入/输出 | 描述 |
| :--- | :--- | :--- |
| dst | 输出 | 目的操作数（矢量）的起始地址 |
| src0 | 输入 | 源操作数（矢量）的起始地址 |
| src1 | 输入 | 源操作数（矢量）的起始地址 |
| mask | 输入 | 用于按位控制参与计算的元素，bit位值为1表示参与计算，0表示不参与。 |

## 返回值说明

无

## 约束说明

- dst、src0、src1的起始地址需要32字节对齐。
- 操作数地址重叠约束请参考[通用地址重叠约束](../general_instruction.md#通用地址重叠约束)。
- 注意除0错误。

## 调用示例

```cpp
vector_half dst;
vector_half src0;
vector_half src1;
vector_bool mask;
asc_div(dst, src0, src1, mask);
```
