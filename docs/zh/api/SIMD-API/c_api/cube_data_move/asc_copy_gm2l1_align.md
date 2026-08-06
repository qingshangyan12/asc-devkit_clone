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

对矩阵数据进行实时padding，完成padding后将数据从Global Memory搬运到L1 Buffer。需要与[asc_set_gm2l1_loop_size](./asc_set_gm2l1_loop_size.md)、[asc_set_gm2l1_loop1_stride](./asc_set_gm2l1_loop1_stride.md)、[asc_set_gm2l1_loop2_stride](./asc_set_gm2l1_loop2_stride.md)和[asc_set_gm2l1_pad](./asc_set_gm2l1_pad.md)配合使用。

## 函数原型

- 常规搬运

    ```cpp
    __aicore__ inline void asc_copy_gm2l1_align(__cbuf__ int8_t* dst, __gm__ int8_t* src, uint32_t n_burst, uint32_t len_burst, uint8_t left_padding_count, uint8_t right_padding_count, bool data_select_bit, uint8_t l2_cache_ctl, uint64_t burst_src_stride, uint32_t burst_dst_stride)
    __aicore__ inline void asc_copy_gm2l1_align(__cbuf__ uint8_t* dst, __gm__ uint8_t* src, uint32_t n_burst, uint32_t len_burst, uint8_t left_padding_count, uint8_t right_padding_count, bool data_select_bit, uint8_t l2_cache_ctl, uint64_t burst_src_stride, uint32_t burst_dst_stride)
    __aicore__ inline void asc_copy_gm2l1_align(__cbuf__ hifloat8_t* dst, __gm__ hifloat8_t* src, uint32_t n_burst, uint32_t len_burst, uint8_t left_padding_count, uint8_t right_padding_count, bool data_select_bit, uint8_t l2_cache_ctl, uint64_t burst_src_stride, uint32_t burst_dst_stride)
    __aicore__ inline void asc_copy_gm2l1_align(__cbuf__ int16_t* dst, __gm__ int16_t* src, uint32_t n_burst, uint32_t len_burst, uint8_t left_padding_count, uint8_t right_padding_count, bool data_select_bit, uint8_t l2_cache_ctl, uint64_t burst_src_stride, uint32_t burst_dst_stride)
    __aicore__ inline void asc_copy_gm2l1_align(__cbuf__ uint16_t* dst, __gm__ uint16_t* src, uint32_t n_burst, uint32_t len_burst, uint8_t left_padding_count, uint8_t right_padding_count, bool data_select_bit, uint8_t l2_cache_ctl, uint64_t burst_src_stride, uint32_t burst_dst_stride)
    __aicore__ inline void asc_copy_gm2l1_align(__cbuf__ half* dst, __gm__ half* src, uint32_t n_burst, uint32_t len_burst, uint8_t left_padding_count, uint8_t right_padding_count, bool data_select_bit, uint8_t l2_cache_ctl, uint64_t burst_src_stride, uint32_t burst_dst_stride)
    __aicore__ inline void asc_copy_gm2l1_align(__cbuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint32_t n_burst, uint32_t len_burst, uint8_t left_padding_count, uint8_t right_padding_count, bool data_select_bit, uint8_t l2_cache_ctl, uint64_t burst_src_stride, uint32_t burst_dst_stride)
    __aicore__ inline void asc_copy_gm2l1_align(__cbuf__ int32_t* dst, __gm__ int32_t* src, uint32_t n_burst, uint32_t len_burst, uint8_t left_padding_count, uint8_t right_padding_count, bool data_select_bit, uint8_t l2_cache_ctl, uint64_t burst_src_stride, uint32_t burst_dst_stride)
    __aicore__ inline void asc_copy_gm2l1_align(__cbuf__ uint32_t* dst, __gm__ uint32_t* src, uint32_t n_burst, uint32_t len_burst, uint8_t left_padding_count,uint8_t right_padding_count, bool data_select_bit, uint8_t l2_cache_ctl, uint64_t burst_src_stride, uint32_t burst_dst_stride)
    __aicore__ inline void asc_copy_gm2l1_align(__cbuf__ float* dst, __gm__ float* src, uint32_t n_burst, uint32_t len_burst, uint8_t left_padding_count, uint8_t right_padding_count, bool data_select_bit, uint8_t l2_cache_ctl, uint64_t burst_src_stride, uint32_t burst_dst_stride)
    ```

- 同步搬运

    ```cpp
    __aicore__ inline void asc_copy_gm2l1_align_sync(__cbuf__ int8_t* dst, __gm__ int8_t* src, uint32_t n_burst, uint32_t len_burst, uint8_t left_padding_count, uint8_t right_padding_count, bool data_select_bit, uint8_t l2_cache_ctl, uint64_t burst_src_stride, uint32_t burst_dst_stride)
    __aicore__ inline void asc_copy_gm2l1_align_sync(__cbuf__ uint8_t* dst, __gm__ uint8_t* src, uint32_t n_burst, uint32_t len_burst, uint8_t left_padding_count, uint8_t right_padding_count, bool data_select_bit, uint8_t l2_cache_ctl, uint64_t burst_src_stride, uint32_t burst_dst_stride)
    __aicore__ inline void asc_copy_gm2l1_align_sync(__cbuf__ hifloat8_t* dst, __gm__ hifloat8_t* src, uint32_t n_burst, uint32_t len_burst, uint8_t left_padding_count, uint8_t right_padding_count, bool data_select_bit, uint8_t l2_cache_ctl, uint64_t burst_src_stride, uint32_t burst_dst_stride)
    __aicore__ inline void asc_copy_gm2l1_align_sync(__cbuf__ int16_t* dst, __gm__ int16_t* src, uint32_t n_burst, uint32_t len_burst, uint8_t left_padding_count, uint8_t right_padding_count, bool data_select_bit, uint8_t l2_cache_ctl, uint64_t burst_src_stride, uint32_t burst_dst_stride)
    __aicore__ inline void asc_copy_gm2l1_align_sync(__cbuf__ uint16_t* dst, __gm__ uint16_t* src, uint32_t n_burst, uint32_t len_burst, uint8_t left_padding_count, uint8_t right_padding_count, bool data_select_bit, uint8_t l2_cache_ctl, uint64_t burst_src_stride, uint32_t burst_dst_stride)
    __aicore__ inline void asc_copy_gm2l1_align_sync(__cbuf__ half* dst, __gm__ half* src, uint32_t n_burst, uint32_t len_burst, uint8_t left_padding_count, uint8_t right_padding_count, bool data_select_bit, uint8_t l2_cache_ctl, uint64_t burst_src_stride, uint32_t burst_dst_stride)
    __aicore__ inline void asc_copy_gm2l1_align_sync(__cbuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint32_t n_burst, uint32_t len_burst, uint8_t left_padding_count, uint8_t right_padding_count, bool data_select_bit, uint8_t l2_cache_ctl, uint64_t burst_src_stride, uint32_t burst_dst_stride)
    __aicore__ inline void asc_copy_gm2l1_align_sync(__cbuf__ int32_t* dst, __gm__ int32_t* src, uint32_t n_burst, uint32_t len_burst, uint8_t left_padding_count, uint8_t right_padding_count, bool data_select_bit, uint8_t l2_cache_ctl, uint64_t burst_src_stride, uint32_t burst_dst_stride)
    __aicore__ inline void asc_copy_gm2l1_align_sync(__cbuf__ uint32_t* dst, __gm__ uint32_t* src, uint32_t n_burst, uint32_t len_burst, uint8_t left_padding_count,uint8_t right_padding_count, bool data_select_bit, uint8_t l2_cache_ctl, uint64_t burst_src_stride, uint32_t burst_dst_stride)
    __aicore__ inline void asc_copy_gm2l1_align_sync(__cbuf__ float* dst, __gm__ float* src, uint32_t n_burst, uint32_t len_burst, uint8_t left_padding_count, uint8_t right_padding_count, bool data_select_bit, uint8_t l2_cache_ctl, uint64_t burst_src_stride, uint32_t burst_dst_stride)
    ```

## 参数说明

**表1** 参数说明

| 参数名  | 输入/输出 | 描述 |
| :----- | :------- | :------- |
| dst | 输出 | 目的操作数（矢量）的起始地址。 |
| src | 输入 | 源操作数（矢量）的起始地址。 |
| n_burst | 输入 | 待搬运的连续传输数据块个数。 |
| len_burst | 输入 | 待搬运的每个连续传输数据块的长度，单位为字节。 |
| left_padding_count | 输入 | 数据左侧的padding元素数。对位宽为8的数据类型，该参数的最大值为32，对位宽为16的数据类型，该参数的最大值为16，对位宽为32的数据类型，该参数的最大值为8。 |
| right_padding_count | 输入 | 数据右侧的padding元素数。对位宽为8的数据类型，该参数的最大值为32，对位宽为16的数据类型，该参数的最大值为16，对位宽为32的数据类型，该参数的最大值为8。 |
| data_select_bit | 输入 | padding数据选择位，当前只支持设置为false，将padding值设置为数据块的第一个元素。 |
| l2_cache_ctl | 输入 | 配置数据在L2 Cache中的管理策略。取值说明请参见[表2](#table2)。 |
| burst_src_stride |输入| 源操作数相邻连续数据块的距离（前面一个数据块的头与后面一个数据块的头的间隔），单位为字节。<br>只搬运1个数据块，即n_burst=1时，可以将此参数设置为0。 |
| burst_dst_stride | 输入 | 目的操作数相邻连续数据块的距离（前面一个数据块的头与后面一个数据块的头的间隔），单位为字节。<br>只搬运1个数据块，即n_burst=1时，可以将此参数设置为0。 |

**表2** l2_cache_ctl取值说明 <a id="table2"></a>

| 取值 | 模式 | 含义 |
|------|------|------|
| 0    | NORMAL模式 | 启用L2 Cache，并且将分配的Cache Line标记为高替换优先级。|
| 1    | LAST模式 | &bull; 启用L2 Cache，并且将分配的Cache Line标记为低替换优先级。<br>&bull; **LAST模式功能，暂不支持。**|
| 2    | PERSISTENT模式 | &bull; 启用L2 Cache。已存入L2 Cache中的数据可能被替换，若需确保特定GlobalTensor的数据始终保留在L2 Cache中，可采用驻留模式。<br>&bull; 注意，被标记为驻留模式的Cache Line只能被其他同样被标记为驻留模式的Cache Line替换。<br>&bull; **PERSISTENT模式功能，暂不支持。**|
| 4    | DISABLE模式 | 不启用L2 Cache，每次都直接从GM中读取，并且保持已有Cache Line的状态不变。 |

## 返回值说明

无

## 流水类型

PIPE_MTE2

## 约束说明

- 各存储单元的空间大小和对齐要求请参考[存储单元说明](../general_description_and_constraints.md#存储单元说明)。
- 目的操作数不得存在地址重叠。对于存在地址重叠的操作，硬件将不返回任何警告或错误，也无法保证地址重叠数据的正确性。

### 通用约束

- 本接口仅在AIC上生效，非AIC调用直接返回。
- dst起始地址需32字节对齐（L1 Buffer端口对齐要求），否则触发异常。
- src起始地址需1字节对齐（Global Memory端口对齐要求），不满足触发异常。
- 如果需要执行多条本接口指令，且目的地址存在重叠，需要插入同步指令（`asc_sync_notify`和`asc_sync_wait`），保证多个指令串行化，防止出现异常数据。
- L1 Buffer容量上限：L1 Buffer总容量512KB，dst偏移+搬运大小不可越界，否则触发目的地址越界异常。当搬运规模`n_burst×len_burst`满足`L1 Buffer容量<n_burst×len_burst<2×L1 Buffer容量`时触发目的地址越界异常，当`n_burst×len_burst≥2×L1 Buffer容量`时触发异常。
- 源端数据占用的内存不可超出Global Memory容量，否则触发源地址越界异常。
- 当`burst_dst_stride`不等于`len_burst`时（即存在padding），`burst_dst_stride`需32字节对齐，否则触发异常，该约束即使`n_burst`为1仍生效。
- `len_burst`需满足dtype字节对齐：dtype为`int16_t`/`uint16_t`/`half`/`bfloat16_t`/`hifloat8_t`时需为2的倍数，dtype为`int32_t`/`uint32_t`/`float`时需为4的倍数，否则触发异常。

### 填充模式约束

- `left_padding_count×sizeof(dtype)`与`right_padding_count×sizeof(dtype)`均不可超过32字节，否则触发异常。具体上限：dtype为`int8_t`/`uint8_t`/`hifloat8_t`时填充元素数∈ [0, 32]，dtype为`int16_t`/`uint16_t`/`half`/`bfloat16_t`时∈ [0, 16]，dtype为`int32_t`/`uint32_t`/`float`时∈ [0, 8]。
- 当`left_padding_count`与`right_padding_count`均为0时，本接口处于normal padding或compact padding模式，不进行左右填充；其中至少一个非0时处于LP/RP模式，硬件强制采用常量填充，`data_select_bit`设为false无效，填充值需通过配套setter接口预先配置。
- LP/RP模式下，`left_padding_count×sizeof(dtype) +right_padding_count×sizeof(dtype) +len_burst`加上末尾对齐填充后需32字节对齐。
- 常量填充模式下，填充值需通过配套setter接口预先配置；未配置时填充值不确定。

### 字段截断约束

- `n_burst`传入>2^21−1的值会按位宽截断处理（如2^21→0、2^21+1→1），硬件不报错。截断后实际生效值偏小，导致实际搬运的连续数据块数小于预期，部分数据未被搬运。截断后若值为0会同时触发n_burst不可为0的异常。
- `len_burst`传入>2^21−1的值会按位宽截断处理（如2^21→0），硬件不报错。截断后实际生效值偏小，导致单次burst搬运字节数不足，需多次burst才能搬完同一块数据。截断后若值为0会同时触发len_burst不可为0的异常。
- `left_padding_count`传入>63的值会按位宽截断处理（如64→0、65→1），硬件不报错。截断后实际填充元素数小于预期，边界区域填充不足，部分应填充位置仍为旧数据。
- `right_padding_count`传入>63的值会按位宽截断处理（如64→0、65→1），硬件不报错。截断后实际填充元素数小于预期，边界区域填充不足，部分应填充位置仍为旧数据。
- `l2_cache_ctl`传入>2^4−1的值会按位宽截断处理（如2^4→0、2^4+1→1），硬件不报错。截断后实际生效值偏小，导致实际生效的缓存策略与预期不符，可能降低搬运命中率。截断后若值落在3、5~7、11、15仍会触发L2 Cache策略异常。
- `burst_src_stride`传入>2^40−1的值会按位宽截断处理，硬件不报错。截断后实际生效值偏小，导致相邻两次迭代的源端地址偏移量小于预期，后一次迭代读到错误位置，取到非预期数据，与前一次迭代数据重叠或错位。
- `burst_dst_stride`传入>2^21−1的值会按位宽截断处理（如2^21→0），硬件不报错。截断后实际生效值偏小，导致相邻两次迭代的目的端地址偏移量小于预期，后一次迭代写到错误位置，与前一次迭代数据重叠或错位。

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
constexpr uint8_t l2_cache_ctl = 4;
// 输入输出数据中两个相邻连续数据块之间的距离为64字节
constexpr uint64_t burst_src_stride = 64;
constexpr uint32_t burst_dst_stride = 64;
// src为从GM传入的地址指针
__cbuf__ half dst[256];
asc_copy_gm2l1_align(dst, src, n_burst, len_burst,  left_padding_count, right_padding_count, data_select_bit, l2_cache_ctl, burst_src_stride, burst_dst_stride);
```
