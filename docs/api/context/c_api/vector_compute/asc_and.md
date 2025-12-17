# asc_and

## AI处理器支持情况

| AI处理器类型     | 是否支持 |
| ----------- |:----:|
| Ascend 910C | √    |
| Ascend 910B | √    |

## 功能说明

执行矢量与运算。计算公式如下：

$$
dst_i = src0_i \quad \&  \quad src1_i
$$

## 函数原型

- 前n个数据计算

    ```c++
    __aicore__ inline void asc_and(_ubuf__ int16_t* dst, _ubuf__ int16_t* src0, uint32_t count)
    ```

- 高维切分计算

    ```cpp
    _aicore__ inline void asc_and(_ubuf__ int16_t* dst, _ubuf__ int16_t* src0, _ubuf__ int16_t* src1, const asc_binary_config& config)
    ```

- 同步计算

    ```cpp
    __aicore__ inline void asc_and_sync(_ubuf__ int16_t* dst, _ubuf__ int16_t* src0, uint32_t count)
    ```

## 参数说明

表1 参数说明

| 参数名       | 输入/输出 | 描述                |
| --------- | ----- | ----------------- |
| dst       | 输出    | 目的操作数。            |
| src0、src1 | 输入    | 源操作数。             |
| count     | 输入    | 参与计算的元素个数。        |
| config    | 输入    | 在高维切分计算场景下使用的计算配置参数。<br/>详细说明请参考[asc_binary_config](../struct/asc_binary_config.md)。 |

## 返回值说明

无

## 流水类型

PIPE_V

## 约束说明

- 操作数地址重叠约束请参考[通用地址重叠约束](../general_instruction.md#通用地址重叠约束)。
- dst、src的起始地址需要32字节对齐。

## 调用示例

```c++
#define TOTAL_LENGTH 64
//TOTAL_LENGTH 指参与计算的数据长度
__ubuf__ int16_t src0[TOTAL_LENGTH];
__ubuf__ int16_t src1[TOTAL_LENGTH];
__ubuf__ int16_t dst[TOTAL_LENGTH];
asc_and(dst, src0, src1, TOTAL_LENGTH);
```