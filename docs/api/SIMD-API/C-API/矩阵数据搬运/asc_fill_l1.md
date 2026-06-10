# asc_fill_l1

## 产品支持情况

| 产品         | 是否支持 |
| :-----------------------| :-----:|
|<cann-filter npu_type = "950"> Ascend 950PR/Ascend 950DT | �?/cann-filter> |
| <term>Atlas A3 训练系列产品/Atlas A3 推理系列产品</term> | �?|
| <term>Atlas A2 训练系列产品/Atlas A2 推理系列产品</term> | �?|

## 功能说明

将value的二进制值赋值给dst，对L1 Buffer的Local Memory进行初始化�?

## 函数原型

- 常规计算

    ```cpp
    __aicore__ inline void asc_fill_l1(__cbuf__ int16_t* dst, half value, const asc_fill_value_config& config)
    __aicore__ inline void asc_fill_l1(__cbuf__ int16_t* dst, uint32_t value, const asc_fill_value_config& config)
    __aicore__ inline void asc_fill_l1(__cbuf__ uint16_t* dst, half value, const asc_fill_value_config& config)
    __aicore__ inline void asc_fill_l1(__cbuf__ uint16_t* dst, uint32_t value, const asc_fill_value_config& config)
    __aicore__ inline void asc_fill_l1(__cbuf__ half* dst, half value, const asc_fill_value_config& config)
    __aicore__ inline void asc_fill_l1(__cbuf__ half* dst, uint32_t value, const asc_fill_value_config& config)
    __aicore__ inline void asc_fill_l1(__cbuf__ bfloat16_t* dst, bfloat16_t value, const asc_fill_value_config& config)
    __aicore__ inline void asc_fill_l1(__cbuf__ int32_t* dst, half value, const asc_fill_value_config& config)
    __aicore__ inline void asc_fill_l1(__cbuf__ int32_t* dst, uint32_t value, const asc_fill_value_config& config)
    __aicore__ inline void asc_fill_l1(__cbuf__ uint32_t* dst, half value, const asc_fill_value_config& config)
    __aicore__ inline void asc_fill_l1(__cbuf__ uint32_t* dst, uint32_t value, const asc_fill_value_config& config)
    __aicore__ inline void asc_fill_l1(__cbuf__ float* dst, half value, const asc_fill_value_config& config)
    __aicore__ inline void asc_fill_l1(__cbuf__ float* dst, uint32_t value, const asc_fill_value_config& config)
    __aicore__ inline void asc_fill_l1(__cbuf__ bfloat16_t* dst, half value, const asc_fill_value_config& config)
    __aicore__ inline void asc_fill_l1(__cbuf__ bfloat16_t* dst, uint32_t value, const asc_fill_value_config& config)
    ```

- 同步计算

    ```cpp
    __aicore__ inline void asc_fill_l1_sync(__cbuf__ int16_t* dst, half value, const asc_fill_value_config& config)
    __aicore__ inline void asc_fill_l1_sync(__cbuf__ int16_t* dst, uint32_t value, const asc_fill_value_config& config)
    __aicore__ inline void asc_fill_l1_sync(__cbuf__ uint16_t* dst, half value, const asc_fill_value_config& config)
    __aicore__ inline void asc_fill_l1_sync(__cbuf__ uint16_t* dst, uint32_t value, const asc_fill_value_config& config)
    __aicore__ inline void asc_fill_l1_sync(__cbuf__ half* dst, half value, const asc_fill_value_config& config)
    __aicore__ inline void asc_fill_l1_sync(__cbuf__ half* dst, uint32_t value, const asc_fill_value_config& config)
    __aicore__ inline void asc_fill_l1_sync(__cbuf__ bfloat16_t* dst, bfloat16_t value, const asc_fill_value_config& config)
    __aicore__ inline void asc_fill_l1_sync(__cbuf__ int32_t* dst, half value, const asc_fill_value_config& config)
    __aicore__ inline void asc_fill_l1_sync(__cbuf__ int32_t* dst, uint32_t value, const asc_fill_value_config& config)
    __aicore__ inline void asc_fill_l1_sync(__cbuf__ uint32_t* dst, half value, const asc_fill_value_config& config)
    __aicore__ inline void asc_fill_l1_sync(__cbuf__ uint32_t* dst, uint32_t value, const asc_fill_value_config& config)
    __aicore__ inline void asc_fill_l1_sync(__cbuf__ float* dst, half value, const asc_fill_value_config& config)
    __aicore__ inline void asc_fill_l1_sync(__cbuf__ float* dst, uint32_t value, const asc_fill_value_config& config)
    __aicore__ inline void asc_fill_l1_sync(__cbuf__ bfloat16_t* dst, half value,const asc_fill_value_config& config)
    __aicore__ inline void asc_fill_l1_sync(__cbuf__ bfloat16_t* dst, uint32_t value, const asc_fill_value_config& config)
    ```

## 参数说明

| 参数�? | 输入/输出 | 描述 |
| :----- | :------- | :------- |
| dst | 输出 | 目的操作数（矩阵）的起始地址�?|
| value | 输入 | 源操作数（标量）�?|
| config | 输入 | 使用的初始化相关参数，详细说明请参考[asc_fill_value_config](../数据结构/asc_fill_value_config.md)�?|

## 返回值说�?

�?

## 流水类型

PIPE_MTE1

## 约束说明

- dst的起始地址需�?2字节对齐�?
- 操作数地址重叠约束请参考[通用地址重叠约束](../通用说明和约�?md#通用地址重叠约束)�?
- 若dst和value的数据类型不相同，则需要根据dst的数据类型对value的值进行转换，具体请参考调用示例�?

## 调用示例

```cpp
constexpr uint64_t total_length = 128;    // total_length指参与搬运的数据总长�?
asc_fill_value_config config;

// dst和value数据类型不相�?
__cbuf__ float dst0[total_length];
uint32_t value0 = 0x3F800000;  // float类型�?的二进制表示�?x3F800000
config.repeat = 1;
config.blk_num = total_length * sizeof(float) / 32;
config.dst_gap = 0;
asc_fill_l1(dst0, value0, config);    // 将dst0�?28个float类型的元素初始化�?

// dst和value数据类型相同
__cbuf__ half dst1[total_length];
half value1 = 1;  // 不用做二进制转换，直接填1即可
config.repeat = 1;
config.blk_num = total_length * sizeof(half) / 32;
config.dst_gap = 0;
asc_fill_l1(dst1, value1, config);    // 将dst1�?28个half类型的元素初始化�?
```
