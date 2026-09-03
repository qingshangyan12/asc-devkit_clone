# asc_copy_gm2l1_align

## 产品支持情况

<!-- npu="950" id1 -->
- Ascend 950PR/Ascend 950DT：支持
<!-- end id1 -->
<!-- npu="A3" id2 -->
- Atlas A3 训练系列产品/Atlas A3 推理系列产品：不支持
<!-- end id2 -->
<!-- npu="910b" id3 -->
- Atlas A2 训练系列产品/Atlas A2 推理系列产品：不支持
<!-- end id3 -->
<!-- npu="310b" id4 -->
- Atlas 200I/500 A2 推理产品：不支持
<!-- end id4 -->
<!-- npu="310p" id5 -->
- Atlas 推理系列产品AI Core：不支持
<!-- end id5 -->
<!-- npu="310p" id6 -->
- Atlas 推理系列产品Vector Core：不支持
<!-- end id6 -->
<!-- npu="910" id7 -->
- Atlas 训练系列产品：不支持
<!-- end id7 -->

## 功能说明

对矩阵数据进行实时padding，完成padding后将数据从Global Memory搬运到L1 Buffer。需要与[asc_set_gm2l1_loop_size](asc_set_gm2l1_loop_size.md)、[asc_set_gm2l1_loop1_stride](asc_set_gm2l1_loop1_stride.md)、[asc_set_gm2l1_loop2_stride](asc_set_gm2l1_loop2_stride.md)和[asc_set_gm2l1_pad](asc_set_gm2l1_pad.md)配合使用。

## 函数原型

- 常规搬运

    ```cpp
    __aicore__ inline void asc_copy_gm2l1_align(__cbuf__ T* dst, __gm__ T* src, uint32_t burst_count,
        uint32_t burst_len, uint8_t left_padding_count, uint8_t right_padding_count, bool enable_data_select,
        asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride, uint32_t dst_stride)
    ```

## 参数说明

**表1** 参数说明

| 参数名  | 输入/输出 | 描述 |
| :----- | :------- | :------- |
| dst | 输出 | 目的操作数（矢量）的起始地址。 |
| src | 输入 | 源操作数（矢量）的起始地址。 |
| burst_count | 输入 | 待搬运的连续传输数据块个数。 |
| burst_len | 输入 | 待搬运的每个连续传输数据块的长度，单位为字节。 |
| left_padding_count | 输入 | 数据左侧的padding元素数。对位宽为8的数据类型，该参数的最大值为32，对位宽为16的数据类型，该参数的最大值为16，对位宽为32的数据类型，该参数的最大值为8。 |
| right_padding_count | 输入 | 数据右侧的padding元素数。对位宽为8的数据类型，该参数的最大值为32，对位宽为16的数据类型，该参数的最大值为16，对位宽为32的数据类型，该参数的最大值为8。 |
| enable_data_select | 输入 | padding数据选择位。 |
| l2_cache_mode | 输入 | 使用[asc_load_l2_cache_mode](../defs/enum/asc_load_l2_cache_mode.md)配置L2 Cache管理策略。 |
| src_stride |输入| 源操作数相邻连续数据块的距离，单位为字节。 |
| dst_stride | 输入 | 目的操作数相邻连续数据块的距离，单位为字节。 |

**表2** `asc_load_l2_cache_mode`枚举值说明 <a id="table2"></a>

| 枚举值 | 数值 | 含义 |
|------|------|------|
| `asc_load_l2_cache_mode::NORMAL_FIRST_VICTIM` | 0 | 启用L2 Cache，并将分配的Cache Line标记为高替换优先级。 |
| `asc_load_l2_cache_mode::NORMAL_LAST_VICTIM` | 1 | 启用L2 Cache，并将分配的Cache Line标记为低替换优先级。 |
| `asc_load_l2_cache_mode::NORMAL_PERSISTENT` | 2 | 启用L2 Cache，并将分配的Cache Line标记为驻留模式。 |
| `asc_load_l2_cache_mode::NOTALLOC_KEEP` | 4 | 不分配L2 Cache Line，并保持已有Cache Line状态不变。 |
| `asc_load_l2_cache_mode::NOTALLOC_CLEAN` | 5 | 不分配L2 Cache Line，并清除已有Cache Line。 |
| `asc_load_l2_cache_mode::NOTALLOC_DROP` | 6 | 不分配L2 Cache Line，并丢弃已有Cache Line。 |

## 返回值说明

无

## 流水类型

PIPE_MTE2

## 约束说明

- 各存储单元的空间大小和对齐要求请参考[存储单元说明](../general_description_and_constraints.md#存储单元说明)。
- 目的操作数不得存在地址重叠。对于存在地址重叠的操作，硬件将不返回任何警告或错误，也无法保证地址重叠数据的正确性。

## 调用示例

```cpp
// 待搬运的连续传输数据块个数为2
constexpr uint32_t n_burst = 2;
// 待搬运的每个连续传输数据块的长度为64个字节
constexpr uint32_t len_burst = 64;
// 数据左右侧padding的元素数为0
constexpr uint8_t left_padding_count = 0;
constexpr uint8_t right_padding_count = 0;
// padding值取数据块的第一个元素
constexpr bool data_select_bit = false;
// L2 Cache采用DISABLE模式
constexpr asc_load_l2_cache_mode l2_cache_mode = asc_load_l2_cache_mode::NOTALLOC_KEEP;
// 输入输出数据中两个相邻连续数据块之间的距离为64字节
constexpr uint64_t burst_src_stride = 64;
constexpr uint32_t burst_dst_stride = 64;
// src为从GM传入的地址指针
__cbuf__ half dst[256];
asc_copy_gm2l1_align(dst, src, n_burst, len_burst, left_padding_count, right_padding_count, data_select_bit,
    l2_cache_mode, burst_src_stride, burst_dst_stride);
```
