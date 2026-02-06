# asc_and

## 产品支持情况

| 产品     | 是否支持 |
| ----------- |:----:|
| Atlas A3 训练系列产品/Atlas A3 推理系列产品 | √    |
| Atlas A2 训练系列产品/Atlas A2 推理系列产品 | √    |

## 功能说明

执行矢量与运算。计算公式如下：

$$
dst_i = src0_i
$$

## 函数原型

- 前n个数据计算

    ```c++
    __aicore__ inline void asc_not(_ubuf__ int16_t* dst, _ubuf__ int16_t* src, uint32_t count)
    __aicore__ inline void asc_not(_ubuf__ uint16_t* dst, _ubuf__ uint16_t* src, uint32_t count)
    ```

## 参数说明

表1 参数说明

| 参数名       | 输入/输出 | 描述                |
| --------- | ----- | ----------------- |
| dst       | 输出    | 目的操作数。            |
| src0、src1 | 输入    | 源操作数（矢量）。类型为[矢量数据寄存器](../reg数据类型定义.md)。             |

## 返回值说明

无

## 流水类型

PIPE_V

## 约束说明

- 操作数地址重叠约束请参考[通用地址重叠约束](../general_instruction.md#通用地址重叠约束)。
- dst、src的需要32字节对齐。

## 调用示例

```c++
constexpr uint64_t total_length = 64;
//total_length 指参与计算的数据长度
__ubuf__ int16_t src[total_length];
__ubuf__ int16_t dst[total_length];
asc_and(dst, src0, src1, total_length);
```