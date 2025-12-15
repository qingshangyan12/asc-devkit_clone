# asc_int642float

## AI处理器支持情况

| AI处理器类型 | 是否支持  |
| :----------------------- | :------: |
| <term>Ascend 910C</term> |    √     |
| <term>Ascend 910B</term> |    √     |

## 功能说明

将int64_t类型数据转换为float类型。

## 函数原型

- 前n个数据计算

    ```c++
    __aicore__ inline void int642float(__ubuf__ float* dst, __ubuf__ int64_t* src, uint32_t count)
    ```

- 高维切分计算

    ```c++
   __aicore__ inline void int642float(__ubuf__ float* dst, __ubuf__ int64_t* src, const asc_unary_config& config)
    ```

- 同步转换

    ```c++
    __aicore__ inline void int642float_sync(__ubuf__ float* dst, __ubuf__ int64_t* src, uint32_t count)
    ```

## 参数说明

| 参数名    | 输入/输出 | 描述                |
| ------ | ----- | ----------------- |
| dst    | 输出    | 目的操作数。            |
| src    | 输入    | 源操作数。             |
| count  | 输入    | 参与连续计算的元素个数。      |
| config | 输入    | 在高维切分计算场景下使用的计算配置参数。<br/>详细说明请参考[asc_unary_config](../struct/asc_unary_config.md)。|

## 返回值说明

无

## 流水类型

PIPE_TYPE_V

## 约束说明

- dst、src的起始地址需要32字节对齐。
- 操作数地址重叠约束请参考[通用地址重叠约束](../general_instruction.md#通用地址重叠约束)。

## 调用示例

```cpp
//total_length指参与转换的数据总长度
uint64_t offset = 0;
__ubuf__ int64_t* src = src[offset];
offset += total_length * sizeof(float);
__ubuf__ float* dst = dst[offset];
asc_int642float(dst, src, total_length);
```