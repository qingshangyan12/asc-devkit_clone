# asc_copy_gm2l1_align

## 产品支持情况

| 产品         | 是否支持 |
| :-----------------------| :-----:|
| Ascend 950PR/Ascend 950DT | √ |

## 功能说明

将矩阵数据从Global Memory (GM)搬运到Level 1 cache (L1)，并进行实时填充。数据在padding后被写入到Level 1 cache。

## 函数原型

- 常规计算

    ```cpp
    __aicore__ inline void asc_copy_gm2l1_align(__cbuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint32_t burst_num, uint32_t burst_len, uint8_t left_padding_count, int8_t right_padding_count, bool data_select_bit, uint8_t l2_cache_ctl, uint64_t burst_src_stride, uint32_t burst_dst_stride)
    __aicore__ inline void asc_copy_gm2l1_align(__cbuf__ half* dst, __gm__ half* src, uint32_t burst_num, uint32_t burst_len, uint8_t left_padding_count, uint8_t right_padding_count, bool data_select_bit, uint8_t l2_cache_ctl, uint64_t burst_src_stride, uint32_t burst_dst_stride)
    __aicore__ inline void asc_copy_gm2l1_align(__cbuf__ float* dst, __gm__ float* src, uint32_t burst_num, uint32_t burst_len, uint8_t left_padding_count, uint8_t right_padding_count, bool data_select_bit, uint8_t l2_cache_ctl, uint64_t burst_src_stride, uint32_t burst_dst_stride)
    __aicore__ inline void asc_copy_gm2l1_align(__cbuf__ hifloat8_t* dst, __gm__ hifloat8_t* src, uint32_t burst_num, uint32_t burst_len, uint8_t left_padding_count, uint8_t right_padding_count, bool data_select_bit, uint8_t l2_cache_ctl, uint64_t burst_src_stride, uint32_t burst_dst_stride)
    __aicore__ inline void asc_copy_gm2l1_align(__cbuf__ int16_t* dst, __gm__ int16_t* src, uint32_t burst_num, uint32_t burst_len, uint8_t left_padding_count, uint8_t right_padding_count, bool data_select_bit, uint8_t l2_cache_ctl, uint64_t burst_src_stride, uint32_t burst_dst_stride)
    __aicore__ inline void asc_copy_gm2l1_align(__cbuf__ int8_t* dst, __gm__ int8_t* src, uint32_t burst_num, uint32_t burst_len, uint8_t left_padding_count, uint8_t right_padding_count, bool data_select_bit, uint8_t l2_cache_ctl, uint64_t burst_src_stride, uint32_t burst_dst_stride)
    __aicore__ inline void asc_copy_gm2l1_align(__cbuf__ uint16_t* dst, __gm__ uint16_t* src, uint32_t burst_num, uint32_t burst_len, uint8_t left_padding_count,uint8_t right_padding_count, bool data_select_bit, uint8_t l2_cache_ctl, uint64_t burst_src_stride, uint32_t burst_dst_stride)
    __aicore__ inline void asc_copy_gm2l1_align(__cbuf__ uint32_t* dst, __gm__ uint32_t* src, uint32_t burst_num, uint32_t burst_len, uint8_t left_padding_count, uint8_t right_padding_count, bool data_select_bit, uint8_t l2_cache_ctl, uint64_t burst_src_stride, uint32_t burst_dst_stride)
    __aicore__ inline void asc_copy_gm2l1_align(__cbuf__ uint8_t* dst, __gm__ uint8_t* src, uint32_t burst_num, uint32_t burst_len, uint8_t left_padding_count, uint8_t right_padding_count, bool data_select_bit, uint8_t l2_cache_ctl, uint64_t burst_src_stride, uint32_t burst_dst_stride)
    ```

- 同步计算

    ```cpp
    __aicore__ inline void asc_copy_gm2l1_align_sync(__cbuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint32_t burst_num, uint32_t burst_len, uint8_t left_padding_count, uint8_t right_padding_count, bool data_select_bit, uint8_t l2_cache_ctl, uint64_t burst_src_stride, uint32_t burst_dst_stride)
    __aicore__ inline void asc_copy_gm2l1_align_sync(__cbuf__ half* dst, __gm__ half* src, uint32_t burst_num, uint32_t burst_len, uint8_t left_padding_count, uint8_t right_padding_count, bool data_select_bit, uint8_t l2_cache_ctl, uint64_t burst_src_stride, uint32_t burst_dst_stride)
    __aicore__ inline void asc_copy_gm2l1_align_sync(__cbuf__ float* dst, __gm__ float* src, uint32_t burst_num, uint32_t burst_len, uint8_t left_padding_count, uint8_t right_padding_count, bool data_select_bit, uint8_t l2_cache_ctl, uint64_t burst_src_stride, uint32_t burst_dst_stride)
    __aicore__ inline void asc_copy_gm2l1_align_sync(__cbuf__ hifloat8_t* dst, __gm__ hifloat8_t* src, uint32_t burst_num, uint32_t burst_len, uint8_t left_padding_count, uint8_t right_padding_count, bool data_select_bit, uint8_t l2_cache_ctl, uint64_t burst_src_stride, uint32_t burst_dst_stride)
    __aicore__ inline void asc_copy_gm2l1_align_sync(__cbuf__ int16_t* dst, __gm__ int16_t* src, uint32_t burst_num, uint32_t burst_len, uint8_t left_padding_count, uint8_t right_padding_count, bool data_select_bit, uint8_t l2_cache_ctl, uint64_t burst_src_stride, uint32_t burst_dst_stride)
    __aicore__ inline void asc_copy_gm2l1_align(__cbuf__ int32_t* dst, __gm__ int32_t* src, uint32_t burst_num, uint32_t burst_len, uint8_t left_padding_count, uint8_t right_padding_count, bool data_select_bit, uint8_t l2_cache_ctl, uint64_t burst_src_stride, uint32_t burst_dst_stride)
    __aicore__ inline void asc_copy_gm2l1_align_sync(__cbuf__ int32_t* dst, __gm__ int32_t* src, uint32_t burst_num, uint32_t burst_len, uint8_t left_padding_count, uint8_t right_padding_count, bool data_select_bit, uint8_t l2_cache_ctl, uint64_t burst_src_stride, uint32_t burst_dst_stride)
    __aicore__ inline void asc_copy_gm2l1_align_sync(__cbuf__ int8_t* dst, __gm__ int8_t* src, uint32_t burst_num, uint32_t burst_len, uint8_t left_padding_count, uint8_t right_padding_count, bool data_select_bit, uint8_t l2_cache_ctl, uint64_t burst_src_stride, uint32_t burst_dst_stride)
    __aicore__ inline void asc_copy_gm2l1_align_sync(__cbuf__ uint16_t* dst, __gm__ uint16_t* src, uint32_t burst_num, uint32_t burst_len, uint8_t left_padding_count, uint8_t right_padding_count, bool data_select_bit, uint8_t l2_cache_ctl, uint64_t burst_src_stride, uint32_t burst_dst_stride)
    __aicore__ inline void asc_copy_gm2l1_align_sync(__cbuf__ uint32_t* dst, __gm__ uint32_t* src, uint32_t burst_num, uint32_t burst_len, uint8_t left_padding_count,uint8_t right_padding_count, bool data_select_bit, uint8_t l2_cache_ctl, uint64_t burst_src_stride, uint32_t burst_dst_stride)
    __aicore__ inline void asc_copy_gm2l1_align_sync(__cbuf__ uint8_t* dst, __gm__ uint8_t* src, uint32_t burst_num, uint32_t burst_len, uint8_t left_padding_count, uint8_t right_padding_count, bool data_select_bit, uint8_t l2_cache_ctl, uint64_t burst_src_stride, uint32_t burst_dst_stride)
    ```

## 参数说明

| 参数名  | 输入/输出 | 描述 |
| :----- | :------- | :------- |
| dst | 输出 | 目的操作数（矢量）的起始地址。 |
| src | 输入 | 源操作数（矢量）的起始地址。 |
| burst_num | 输入 | 待搬运的连续传输数据块个数。 |
| burst_len | 输入 | 待搬运的每个连续传输数据块的长度，单位为DataBlock（32字节）。对16位数据，值为2的倍数，对32位数据，值为4的倍数。 |
| left_padding_count | 输入 | 数据左侧的padding元素数目，标记为left_pad，最大padding值为32B。 |
| right_padding_count | 输入 | 数据右侧的padding元素数目，标记为right_pad，最大padding值为32B。 |
| data_select_bit | 输入 | padding数据选择位，若为false，则padding值为数据块的第一个元素；若为true，则根据预设的常量来padding。 |
| l2_cache_ctl | 输入 | 配置数据在L2 Cache中的管理策略。取值说明如下：  <br>&bull; 0：DISABLE模式，适用于仅需访问一次的数据。 <br>&bull; 1：NORMAL模式，适用于重用模式未知或不极端的数据。 <br>&bull; 2：LAST模式，适用于高频重复访问的数据。 <br>&bull; 4：PERSISTENT模式，适用于需要长期驻留在缓存中的数据。 |
| burst_src_stride |输入| 输入数据中两个相邻连续数据块之间的距离。 |
| burst_dst_stride | 输入 | 输出数据中两个相邻连续数据块之间的距离。 |

## 返回值说明

无

## 流水类型

PIPE_MTE2

## 约束说明

- 注意burst_len不代表padding尺寸。
- 如果burst_dst_stride != burst_len，burst_dst_stride需要32B对齐。

## 调用示例

```cpp
__gm__ half src[256];
__cbuf__ half dst[256];
asc_copy_gm2l1_align(dst, src, 1, 4, 0, 0, false, 0, 0, 0);
```