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

将数据从Global Memory搬运到L1 Buffer，搬运过程中对数据进行实时padding，使目的端按32字节对齐方式存储非对齐数据。源数据位于Global Memory，目的地址位于L1 Buffer，数据通路为GM->L1 Buffer。

本接口支持normal padding、compact padding、LP/RP三种填充模式，并支持与loop模式组合使用。填充数据的来源由`data_select_bit`决定（首元素填充或常量填充）。

常量填充模式下，填充值需通过[asc_set_gm2l1_pad](asc_set_gm2l1_pad.md)预先配置；loop模式下，loop1/loop2的搬运次数与源/目步长需通过[asc_set_gm2l1_loop_size](asc_set_gm2l1_loop_size.md)、[asc_set_gm2l1_loop1_stride](asc_set_gm2l1_loop1_stride.md)、[asc_set_gm2l1_loop2_stride](asc_set_gm2l1_loop2_stride.md)预先配置。

本接口仅在AIC上执行有效，在AIV上调用为空操作。

## 函数原型

### 模板原型（占位符形式）

```cpp
__aicore__ inline void asc_copy_gm2l1_align(__cbuf__ <dtype>* dst,
                                            __gm__ <dtype>* src,
                                            uint32_t n_burst,
                                            uint32_t len_burst,
                                            uint8_t left_padding_count,
                                            uint8_t right_padding_count,
                                            bool data_select_bit,
                                            uint8_t l2_cache_ctl,
                                            uint64_t burst_src_stride,
                                            uint32_t burst_dst_stride)
__aicore__ inline void asc_copy_gm2l1_align_sync(__cbuf__ <dtype>* dst,
                                                 __gm__ <dtype>* src,
                                                 uint32_t n_burst,
                                                 uint32_t len_burst,
                                                 uint8_t left_padding_count,
                                                 uint8_t right_padding_count,
                                                 bool data_select_bit,
                                                 uint8_t l2_cache_ctl,
                                                 uint64_t burst_src_stride,
                                                 uint32_t burst_dst_stride)
```

### dtype支持的数据类型

dtype支持的数据类型为`int8_t`、`uint8_t`、`hifloat8_t`、`int16_t`、`uint16_t`、`half`、`bfloat16_t`、`int32_t`、`uint32_t`、`float`。

### 典型示例

```cpp
// 示例：int8_t类型的常规搬运。
__aicore__ inline void asc_copy_gm2l1_align(__cbuf__ int8_t* dst,
                                            __gm__ int8_t* src,
                                            uint32_t n_burst,
                                            uint32_t len_burst,
                                            uint8_t left_padding_count,
                                            uint8_t right_padding_count,
                                            bool data_select_bit,
                                            uint8_t l2_cache_ctl,
                                            uint64_t burst_src_stride,
                                            uint32_t burst_dst_stride)
```

## 参数说明

**表1** 参数说明

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| dst | 输出 | 目的操作数，存储位置为L1 Buffer。目的地址需32字节对齐。 |
| src | 输入 | 源操作数，存储位置为GM。源地址需1字节对齐。 |
| n_burst | 输入 | 待搬运的连续数据块个数。取值范围 [1, 2^21−1]。 |
| len_burst | 输入 | 每个连续数据块的长度，单位字节。取值范围 [1, 2^21−1]。需满足dtype字节对齐：dtype为`hifloat8_t`、`int16_t`、`uint16_t`、`half`、`bfloat16_t`时len_burst需为2的倍数，dtype为`int32_t`、`uint32_t`、`float`时len_burst需为4的倍数。 |
| left_padding_count | 输入 | 左侧填充元素个数。填充字节数需满足`left_padding_count × sizeof(dtype) ≤ 32`字节，对应dtype上限：dtype为`int8_t`、`uint8_t`、`hifloat8_t`时取值范围 [0, 32]，dtype为`int16_t`、`uint16_t`、`half`、`bfloat16_t`时取值范围 [0, 16]，dtype为`int32_t`、`uint32_t`、`float`时取值范围 [0, 8]。 |
| right_padding_count | 输入 | 右侧填充元素个数。填充字节数需满足`right_padding_count × sizeof(dtype) ≤ 32`字节，对应dtype上限：dtype为`int8_t`、`uint8_t`、`hifloat8_t`时取值范围 [0, 32]，dtype为`int16_t`、`uint16_t`、`half`、`bfloat16_t`时取值范围 [0, 16]，dtype为`int32_t`、`uint32_t`、`float`时取值范围 [0, 8]。 |
| `data_select_bit` | 输入 | 填充数据来源选择。<br>&nbsp;&nbsp;&bull; `false`：首元素填充模式，填充数据取每个burst的首元素；<br>&nbsp;&nbsp;&bull; `true`：常量填充模式，填充数据取配套setter接口预先配置的填充值。当`left_padding_count`与`right_padding_count`至少有一个非0时本参数才生效，且常量填充模式下硬件强制使用常量填充值，`data_select_bit`设为`false`无效。 |
| l2_cache_ctl | 输入 | 配置数据在L2 Cache中的管理策略。取值说明请参见[表2](#table2)。取值范围 [0, 2^4−1]。 |
| burst_src_stride | 输入 | 相邻连续数据块在源端（Global Memory）的步长，单位字节。取值范围 [0, 2^40−1]。 |
| burst_dst_stride | 输入 | 相邻连续数据块在目的端（L1 Buffer）的步长，单位字节。取值范围 [0, 2^21−1]。当`burst_dst_stride`不等于`len_burst`时（即存在padding），`burst_dst_stride`需32字节对齐，否则触发异常（即使`n_burst`为1）。 |

**表2** l2_cache_ctl取值说明 <a id="table2"></a>

| 取值 | 模式 | 含义 |
|------|------|------|
| 0 | Normal模式 | 启用L2 Cache，并将分配的Cache Line标记为默认替换优先级。 |
| 1 | Normal模式 | 启用L2 Cache，并将分配的Cache Line标记为默认替换优先级。 |
| 2 | Normal模式 | 启用L2 Cache，并将分配的Cache Line标记为默认替换优先级。 |
| 4 | not-alloc模式 | 不在L2 Cache中分配Cache Line，保持已有Cache Line状态不变。 |
| 8 | write-back-home模式 | 写回归属策略。 |
| 9 | write-back-home模式 | 写回归属策略。 |
| 10 | write-back-home模式 | 写回归属策略。 |
| 12 | write-through-share模式 | 写透共享策略。 |
| 13 | write-through-share模式 | 写透共享策略。 |
| 14 | write-through-share模式 | 写透共享策略。 |

取值3、5、6、7、11、15为保留值或未定义值，传入触发L2 cache策略异常。

## 返回值说明

无

## 流水类型

PIPE_MTE2

## 约束说明

- 各存储单元的空间大小和对齐要求请参考[存储单元说明](../general_description_and_constraints.md#存储单元说明)。
- 目的操作数不得存在地址重叠。对于存在地址重叠的操作，无法保证地址重叠数据的正确性。

### 通用约束

- 本接口仅在AIC上生效，非AIC调用直接返回。
- `dst`起始地址需32字节对齐（L1 Buffer对齐要求），否则触发异常。
- `src`起始地址需1字节对齐（Global Memory对齐要求），不满足触发异常。
- 如果本指令与其他指令存在目的地址重叠，需要插入同步指令（[asc_sync_notify](../sync/asc_sync_notify.md)和[asc_sync_wait](../sync/asc_sync_wait.md)），保证多个指令串行化，防止出现异常数据。
- L1 Buffer容量上限：L1 Buffer总容量512KB，dst偏移+搬运大小不可越界，否则触发目的地址越界异常。
- 源端数据占用的内存不可超出Global Memory容量，否则触发源地址越界异常。
- `len_burst`需满足dtype字节对齐：dtype为`int16_t`、`uint16_t`、`half`、`bfloat16_t`时需为2的倍数，dtype为`int32_t`、`uint32_t`、`float`时需为4的倍数，否则触发异常。
- `n_burst`取值范围为[1, 2^21−1]，超出取值范围的值会被截断，导致搬运结果不符合预期。
- `len_burst`取值范围为[1, 2^21−1]，超出取值范围的值会被截断，导致搬运结果不符合预期。
- `burst_src_stride`取值范围为[0, 2^40−1]，超出取值范围的值会被截断，导致搬运结果不符合预期。
- `burst_dst_stride`取值范围为[0, 2^21−1]，超出取值范围的值会被截断，导致搬运结果不符合预期。
- `data_select_bit`设置为true时，或者左右填充模式（`left_padding_count`和`right_padding_count`任意不为0）时，须先调用[asc_set_gm2l1_pad](asc_set_gm2l1_pad.md)配置填充值。

### Normal填充模式约束

- 当`burst_dst_stride`不等于`len_burst`时，`burst_dst_stride`需32字节对齐，否则触发异常，该约束即使`n_burst`为1仍生效。
- 填充数据的数据量为`32 - len_burst % 32`，单位为字节；填充的值取决于`data_select_bit`设置。若设置`data_select_bit`为false，将padding值设置为每次burst的第一个元素；若设置为true，padding值为[asc_set_gm2l1_pad](asc_set_gm2l1_pad.md)设置的值。

### Compact填充模式约束

- 当`burst_dst_stride`等于`len_burst`时，`n_burst *len_burst`非32字节对齐，则触发Compact填充模式。
- 填充数据的数据量为`32 - n_burst * len_burst % 32`，单位为字节；填充的值取决于`data_select_bit`设置。若设置`data_select_bit`为false，将padding值设置为每次burst的第一个元素；若设置为true，padding值为[asc_set_gm2l1_pad](asc_set_gm2l1_pad.md)设置的值。

### 左右填充模式约束

- `left_padding_count`和`right_padding_count`的取值范围随dtype变化：dtype为`int8_t`、`uint8_t`或`hifloat8_t`时，取值范围为[0, 32]；dtype为`int16_t`、`uint16_t`、`half`或`bfloat16_t`时，取值范围为[0, 16]；dtype为`int32_t`、`uint32_t`或`float`时，取值范围为[0, 8]。超出取值范围会导致搬运填充结果不符合预期。
- 左右填充模式下，`left_padding_count×sizeof(dtype) + right_padding_count×sizeof(dtype) + len_burst`加上末尾对齐填充后需32字节对齐。

### 循环填充模式约束

- 循环填充模式必须配合[asc_set_gm2l1_loop_size](asc_set_gm2l1_loop_size.md)、[asc_set_gm2l1_loop1_stride](asc_set_gm2l1_loop1_stride.md)、[asc_set_gm2l1_loop2_stride](asc_set_gm2l1_loop2_stride.md)使用，通过上述接口配置循环次数和步长。
- 循环填充模式`loop1 dst stride`与`loop2 dst stride`必须32字节对齐，该约束即使loop size为1仍生效。
- 循环填充模式不能和左右填充模式同时开启。

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
