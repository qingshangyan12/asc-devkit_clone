# asc_copy_gm2ub_align

## 产品支持情况

| 产品 | 是否支持  |
| :----------------------- | :------: |
| <cann-filter npu_type="A3"><term>Atlas A3 训练系列产品/Atlas A3 推理系列产品</term> |    √</cann-filter>     |
| <cann-filter npu_type="910b"><term>Atlas A2 训练系列产品/Atlas A2 推理系列产品</term> |    √</cann-filter>     |
| <cann-filter npu_type="950"><term>Ascend 950PR/Ascend 950DT</term>  | √ </cann-filter>|

## 功能说明

提供数据非对齐搬运的功能，将数据从Global Memory (GM) 搬运到 Unified Buffer (UB)，并支持8位/16位/32位数据类型搬运。

## 函数原型

<cann-filter npu_type="910b,A3">

- 前n个数据搬运

    ```cpp
    __aicore__ inline void asc_copy_gm2ub_align(__ubuf__ int8_t* dst, __gm__ int8_t* src, uint32_t size)
    __aicore__ inline void asc_copy_gm2ub_align(__ubuf__ uint8_t* dst, __gm__ uint8_t* src, uint32_t size)
    __aicore__ inline void asc_copy_gm2ub_align(__ubuf__ int16_t* dst, __gm__ int16_t* src, uint32_t size)
    __aicore__ inline void asc_copy_gm2ub_align(__ubuf__ uint16_t* dst, __gm__ uint16_t* src, uint32_t size)
    __aicore__ inline void asc_copy_gm2ub_align(__ubuf__ half* dst, __gm__ half* src, uint32_t size)
    __aicore__ inline void asc_copy_gm2ub_align(__ubuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint32_t size)
    __aicore__ inline void asc_copy_gm2ub_align(__ubuf__ int32_t* dst, __gm__ int32_t* src, uint32_t size)
    __aicore__ inline void asc_copy_gm2ub_align(__ubuf__ uint32_t* dst, __gm__ uint32_t* src, uint32_t size)
    __aicore__ inline void asc_copy_gm2ub_align(__ubuf__ float* dst, __gm__ float* src, uint32_t size)
    ```

- 高维切分搬运

    ```cpp
    __aicore__ inline void asc_copy_gm2ub_align(__ubuf__ int8_t* dst, __gm__ int8_t* src, uint16_t n_burst, uint32_t len_burst, uint8_t left_padding_num, uint8_t right_padding_num, uint32_t src_gap, uint32_t dst_gap)
    __aicore__ inline void asc_copy_gm2ub_align(__ubuf__ uint8_t* dst, __gm__ uint8_t* src, uint16_t n_burst, uint32_t len_burst, uint8_t left_padding_num, uint8_t right_padding_num, uint32_t src_gap, uint32_t dst_gap)
    __aicore__ inline void asc_copy_gm2ub_align(__ubuf__ int16_t* dst, __gm__ int16_t* src, uint16_t n_burst, uint32_t len_burst, uint8_t left_padding_num, uint8_t right_padding_num, uint32_t src_gap, uint32_t dst_gap)
    __aicore__ inline void asc_copy_gm2ub_align(__ubuf__ uint16_t* dst, __gm__ uint16_t* src, uint16_t n_burst, uint32_t len_burst, uint8_t left_padding_num, uint8_t right_padding_num, uint32_t src_gap, uint32_t dst_gap)
    __aicore__ inline void asc_copy_gm2ub_align(__ubuf__ half* dst, __gm__ half* src, uint16_t n_burst, uint32_t len_burst, uint8_t left_padding_num, uint8_t right_padding_num, uint32_t src_gap, uint32_t dst_gap)
    __aicore__ inline void asc_copy_gm2ub_align(__ubuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint16_t n_burst, uint32_t len_burst, uint8_t left_padding_num, uint8_t right_padding_num, uint32_t src_gap, uint32_t dst_gap)
    __aicore__ inline void asc_copy_gm2ub_align(__ubuf__ int32_t* dst, __gm__ int32_t* src, uint16_t n_burst, uint32_t len_burst, uint8_t left_padding_num, uint8_t right_padding_num, uint32_t src_gap, uint32_t dst_gap)
    __aicore__ inline void asc_copy_gm2ub_align(__ubuf__ uint32_t* dst, __gm__ uint32_t* src, uint16_t n_burst, uint32_t len_burst, uint8_t left_padding_num, uint8_t right_padding_num, uint32_t src_gap, uint32_t dst_gap)
    __aicore__ inline void asc_copy_gm2ub_align(__ubuf__ float* dst, __gm__ float* src, uint16_t n_burst, uint32_t len_burst, uint8_t left_padding_num, uint8_t right_padding_num, uint32_t src_gap, uint32_t dst_gap)
    ```

- 同步计算

    ```cpp
    __aicore__ inline void asc_copy_gm2ub_align_sync(__ubuf__ int8_t* dst, __gm__ int8_t* src, uint32_t size)
    __aicore__ inline void asc_copy_gm2ub_align_sync(__ubuf__ uint8_t* dst, __gm__ uint8_t* src, uint32_t size)
    __aicore__ inline void asc_copy_gm2ub_align_sync(__ubuf__ int16_t* dst, __gm__ int16_t* src, uint32_t size)
    __aicore__ inline void asc_copy_gm2ub_align_sync(__ubuf__ uint16_t* dst, __gm__ uint16_t* src, uint32_t size)
    __aicore__ inline void asc_copy_gm2ub_align_sync(__ubuf__ half* dst, __gm__ half* src, uint32_t size)
    __aicore__ inline void asc_copy_gm2ub_align_sync(__ubuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint32_t size)
    __aicore__ inline void asc_copy_gm2ub_align_sync(__ubuf__ int32_t* dst, __gm__ int32_t* src, uint32_t size)
    __aicore__ inline void asc_copy_gm2ub_align_sync(__ubuf__ uint32_t* dst, __gm__ uint32_t* src, uint32_t size)
    __aicore__ inline void asc_copy_gm2ub_align_sync(__ubuf__ float* dst, __gm__ float* src, uint32_t size)
    ```
</cann-filter>
<cann-filter npu_type="950">

```cpp
__aicore__ inline void asc_copy_gm2ub_align(__ubuf__ int8_t* dst, __gm__ int8_t* src, uint16_t n_burst, uint32_t len_burst, uint8_t left_padding_num, uint8_t right_padding_num, bool enable_constant_pad, uint8_t l2_cache_mode, uint64_t src_stride, uint32_t dst_stride)
__aicore__ inline void asc_copy_gm2ub_align(__ubuf__ uint8_t* dst, __gm__ uint8_t* src, uint16_t n_burst, uint32_t len_burst, uint8_t left_padding_num, uint8_t right_padding_num, bool enable_constant_pad, uint8_t l2_cache_mode, uint64_t src_stride, uint32_t dst_stride)
__aicore__ inline void asc_copy_gm2ub_align(__ubuf__ hifloat8_t* dst, __gm__ hifloat8_t* src, uint16_t n_burst, uint32_t len_burst, uint8_t left_padding_num, uint8_t right_padding_num, bool enable_constant_pad, uint8_t l2_cache_mode, uint64_t src_stride, uint32_t dst_stride)
__aicore__ inline void asc_copy_gm2ub_align(__ubuf__ fp8_e5m2_t* dst, __gm__ fp8_e5m2_t* src, uint16_t n_burst, uint32_t len_burst, uint8_t left_padding_num, uint8_t right_padding_num, bool enable_constant_pad, uint8_t l2_cache_mode, uint64_t src_stride, uint32_t dst_stride)
__aicore__ inline void asc_copy_gm2ub_align(__ubuf__ fp8_e4m3fn_t* dst, __gm__ fp8_e4m3fn_t* src, uint16_t n_burst, uint32_t len_burst, uint8_t left_padding_num, uint8_t right_padding_num, bool enable_constant_pad, uint8_t l2_cache_mode, uint64_t src_stride, uint32_t dst_stride)
__aicore__ inline void asc_copy_gm2ub_align(__ubuf__ int16_t* dst, __gm__ int16_t* src, uint16_t n_burst, uint32_t len_burst, uint8_t left_padding_num, uint8_t right_padding_num, bool enable_constant_pad, uint8_t l2_cache_mode, uint64_t src_stride, uint32_t dst_stride)
__aicore__ inline void asc_copy_gm2ub_align(__ubuf__ uint16_t* dst, __gm__ uint16_t* src, uint16_t n_burst, uint32_t len_burst, uint8_t left_padding_num, uint8_t right_padding_num, bool enable_constant_pad, uint8_t l2_cache_mode, uint64_t src_stride, uint32_t dst_stride)
__aicore__ inline void asc_copy_gm2ub_align(__ubuf__ half* dst, __gm__ half* src, uint16_t n_burst, uint32_t len_burst, uint8_t left_padding_num, uint8_t right_padding_num, bool enable_constant_pad, uint8_t l2_cache_mode, uint64_t src_stride, uint32_t dst_stride)
__aicore__ inline void asc_copy_gm2ub_align(__ubuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint16_t n_burst, uint32_t len_burst, uint8_t left_padding_num, uint8_t right_padding_num, bool enable_constant_pad, uint8_t l2_cache_mode, uint64_t src_stride, uint32_t dst_stride)
__aicore__ inline void asc_copy_gm2ub_align(__ubuf__ int32_t* dst, __gm__ int32_t* src, uint16_t n_burst, uint32_t len_burst, uint8_t left_padding_num, uint8_t right_padding_num, bool enable_constant_pad, uint8_t l2_cache_mode, uint64_t src_stride, uint32_t dst_stride)
__aicore__ inline void asc_copy_gm2ub_align(__ubuf__ uint32_t* dst, __gm__ uint32_t* src, uint16_t n_burst, uint32_t len_burst, uint8_t left_padding_num, uint8_t right_padding_num, bool enable_constant_pad, uint8_t l2_cache_mode, uint64_t src_stride, uint32_t dst_stride)
__aicore__ inline void asc_copy_gm2ub_align(__ubuf__ float* dst, __gm__ float* src, uint16_t n_burst, uint32_t len_burst, uint8_t left_padding_num, uint8_t right_padding_num, bool enable_constant_pad, uint8_t l2_cache_mode, uint64_t src_stride, uint32_t dst_stride)
```

</cann-filter>

## 参数说明

<cann-filter npu_type="910b,A3">

| 参数名 | 输入/输出 | 描述 |
| :--- | :--- | :--- |
| dst | 输出 | 目的UB的起始地址。 |
| src | 输入 | 源GM的起始地址。 |
| <cann-filter npu_type="910b,A3">size | 输入 | 搬运数据大小（字节）。 </cann-filter>|
| n_burst | 输入 | 待搬运的连续传输数据块个数。取值范围：[1, 4095]。 |
| len_burst | 输入 | 待搬运的每个连续传输数据块的长度，单位为字节。取值范围：[1, 2097151]。 |
| left_padding_num | 输入 | 连续搬运数据块左侧需要补充的数据范围，单位为元素个数，left_padding_num的字节数不能超过32。|
| right_padding_num | 输入 | 连续搬运数据块右侧需要补充的数据范围，单位为元素个数，right_padding_num的字节数不能超过32。|
| <cann-filter npu_type="910b,A3">src_gap | 输入 | 源操作数相邻连续数据块的间隔（前面一个数据块的尾与后面一个数据块的头的间隔）。<br>单位为DataBlock（32字节）。</cann-filter>|
| <cann-filter npu_type="910b,A3">dst_gap | 输入 | 目的操作数相邻连续数据块的间隔（前面一个数据块的尾与后面一个数据块的头的间隔）。<br>单位为DataBlock（32字节）。 </cann-filter>|
| <cann-filter npu_type="950">enable_constant_pad | 输入 | 配置非对齐填充值是否为常量。取值说明如下：  <br>&bull; true：填充值为接口[asc_set_gm2ub_pad](asc_set_gm2ub_pad.md)设置的值。 <br>&bull; false：填充值为输入数据的第一个值。 </cann-filter>|
| <cann-filter npu_type="950">l2_cache_mode | 输入 | 配置数据在L2Cache中的管理策略。取值说明如下：  <br>&bull; 0：DISABLE模式，适用于仅需访问一次的数据。 <br>&bull; 1：NORMAL模式，适用于重用模式未知或不极端的数据。 <br>&bull; 2：LAST模式，适用于高频重复访问的数据。 <br>&bull; 4：PERSISTENT模式，适用于需要长期驻留在缓存中的数据。 </cann-filter>|
| <cann-filter npu_type="950">src_stride | 输入 | 源操作数相邻连续数据块的间隔（前面一个数据块的尾与后面一个数据块的头的间隔）。<br>单位为字节。 </cann-filter>|
| <cann-filter npu_type="950">dst_stride | 输入 | 目的操作数相邻连续数据块的间隔（前面一个数据块的尾与后面一个数据块的头的间隔）。<br>单位为DataBlock（32字节）。 </cann-filter>|

</cann-filter>

## 返回值说明

无

## 流水类型

PIPE_MTE2

## 约束说明

<cann-filter npu_type="910b,A3">

- src的起始地址要求按照对应数据类型所占字节数对齐。
- dst的起始地址要求32字节对齐。
- 如果需要执行多条asc_copy_gm2ub_align指令，且asc_copy_gm2ub_align指令的目的地址存在重叠，需要插入同步指令，保证多个asc_copy_gm2ub_align指令的串行化，防止出现异常数据。
- 同步计算包含同步等待。
- left_padding_num、right_padding_num的字节数均不能超过32Bytes。

</cann-filter>
<cann-filter npu_type="950">

- src的起始地址要求按照对应数据类型所占字节数对齐。
- dst的起始地址要求32字节对齐。
- 如果需要执行多条asc_copy_gm2ub_align指令，且asc_copy_gm2ub_align指令的目的地址存在重叠，需要插入同步指令，保证多个asc_copy_gm2ub_align指令的串行化，防止出现异常数据。
- left_padding_num、right_padding_num的字节数均不能超过32Bytes。

</cann-filter>

## 调用示例

<cann-filter npu_type="910b,A3">

```cpp
// total_length指参与搬运的数据总长度。src是外部输入的GM内存。
constexpr uint64_t total_length = 128;
__ubuf__ uint8_t dst[total_length];
asc_copy_gm2ub_align(dst, src, total_length * sizeof(uint8_t));
```

</cann-filter>
<cann-filter npu_type="950">

```cpp
asc_set_gm2ub_loop_size(2, 2);
asc_set_gm2ub_loop1_stride(96, 128);
asc_set_gm2ub_loop2_stride(192, 288);
asc_copy_gm2ub_align(dst, src, 2, 48 * sizeof(int8_t), 0, 0, false, 0, 0, 0);
asc_set_gm2ub_loop_size(1, 1);
```

</cann-filter>
