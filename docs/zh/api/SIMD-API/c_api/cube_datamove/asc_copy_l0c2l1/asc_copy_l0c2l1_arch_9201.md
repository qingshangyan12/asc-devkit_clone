# asc_copy_l0c2l1

## 产品支持情况

<!-- npu="960" id1 -->
- Ascend 960PR/Ascend 960DT：支持
<!-- end id1 -->
<!-- npu="950" id2 -->
- Ascend 950PR/Ascend 950DT：不支持
<!-- end id2 -->
<!-- npu="A3" id3 -->
- Atlas A3 训练系列产品/Atlas A3 推理系列产品：不支持
<!-- end id3 -->
<!-- npu="910b" id4 -->
- Atlas A2 训练系列产品/Atlas A2 推理系列产品：不支持
<!-- end id4 -->
<!-- npu="310b" id5 -->
- Atlas 200I/500 A2 推理产品：不支持
<!-- end id5 -->
<!-- npu="310p" id6 -->
- Atlas 推理系列产品AI Core：不支持
<!-- end id6 -->
<!-- npu="310p" id7 -->
- Atlas 推理系列产品Vector Core：不支持
<!-- end id7 -->
<!-- npu="910" id8 -->
- Atlas 训练系列产品：不支持
<!-- end id8 -->

## 功能说明

头文件路径为：`"c_api/cube_datamove/cube_datamove.h"`。

该接口用于将矩阵计算的结果从L0C Buffer搬运至L1 Buffer中，并且在搬运过程中支持与其它接口组合使用，配置多种随路能力，包括随路量化、随路ReLU、随路格式转换（Nz2ND/Nz2DN）以及随路通道拆分/通道合并等。（目前针对量化模式的支持尚不完全，具体支持情况请以[函数原型](#函数原型)中包含的接口为准。）

quant_pre_mode量化模式参数支持的枚举值如下：

- NoQuant：不开启量化功能。
- DEQF16：int32_t量化成half，scalar量化。
- VDEQF16：int32_t量化成half，tensor量化。
- REQ4：int32_t量化成int4b_t，scalar量化。
- VREQ4：int32_t量化成int4b_t，tensor量化。
- REQ8：int32_t量化成int8_t/uint8_t，scalar量化。
- VREQ8：int32_t量化成int8_t/uint8_t，tensor量化。
- QS322BF16_PRE：int32_t量化成bfloat16_t，scalar量化。
- VQS322BF16_PRE：int32_t量化成bfloat16_t，tensor量化。
- QF322F16_PRE：float量化成half，scalar量化。
- VQF322F16_PRE：float量化成half，tensor量化。
- QF322BF16_PRE：float量化成bfloat16_t，scalar量化。
- VQF322BF16_PRE：float量化成bfloat16_t，tensor量化。
- F322F16：float cast成half，cast mode为CAST_RINT模式。
- F322BF16：float cast成bfloat16_t，cast mode为CAST_RINT模式。
- QF322S4_PRE：float量化成int4b_t，scalar量化。
- VQF322S4_PRE：float量化成int4b_t，tensor量化。
- QF322B8_PRE：float量化成int8_t/uint8_t，scalar量化。
- VQF322B8_PRE：float量化成int8_t/uint8_t，tensor量化。
- QF322FP8_PRE：float量化成fp8_e4m3fn_t，scalar量化。
- VQF322FP8_PRE：float量化成fp8_e4m3fn_t，tensor量化。
- QF322HIF8_PRE：float量化成hifloat8_t(Half to Away Round)，scalar量化。
- VQF322HIF8_PRE：float量化成hifloat8_t(Half to Away Round)，tensor量化。
- QF322HIF8_PRE_HYBRID：float量化成hifloat8_t(Hybrid Round)，scalar量化。
- VQF322HIF8_PRE_HYBRID：float量化成hifloat8_t(Hybrid Round)，tensor量化。
- QF322F32_PRE：float量化成float，scalar量化，精度可以达到双千分之一，无法达到双万分之一。如果有双万分之一的精度要求，建议使用[AscendDeQuant](../../../adv_api/quantization/AscendDequant.md)高阶API。
- VQF322F32_PRE：float量化成float，tensor量化，精度可以达到双千分之一，无法达到双万分之一。如果有双万分之一的精度要求，建议使用[AscendDeQuant](../../../adv_api/quantization/AscendDequant.md)高阶API。

本接口仅在AIC上执行有效。

## 函数原型

### 模板原型（占位符形式）

```c
__aicore__ inline void asc_copy_l0c2l1(__cbuf__ <dst_dtype>* dst,
                                       __cc__ <src_dtype>* src,
                                       uint16_t n_size,
                                       uint16_t m_size,
                                       uint32_t dst_stride,
                                       uint16_t src_stride,
                                       uint8_t enable_clip_relu_pre,
                                       uint8_t unit_flag_mode,
                                       uint64_t quant_pre_mode,
                                       uint8_t relu_pre_mode,
                                       bool enable_channel_split,
                                       bool enable_nz2nd,
                                       uint64_t quant_post,
                                       uint8_t relu_post,
                                       bool clip_relu_post,
                                       uint8_t eltwise_op,
                                       uint8_t eltwise_antq_en,
                                       bool c0_pad_en)
__aicore__ inline void asc_copy_l0c2l1(__cbuf__ <dst_dtype>* dst,
                                       __cc__ <src_dtype>* src,
                                       uint16_t n_size,
                                       uint16_t m_size,
                                       uint32_t dst_stride,
                                       uint16_t src_stride,
                                       bool quant_pre_rnd,
                                       uint8_t enable_clip_relu_pre,
                                       uint8_t unit_flag_mode,
                                       uint64_t quant_pre_mode,
                                       uint8_t relu_pre_mode,
                                       bool enable_channel_split,
                                       bool enable_nz2nd,
                                       uint64_t quant_post,
                                       uint8_t relu_post,
                                       bool clip_relu_post,
                                       uint8_t eltwise_op,
                                       uint8_t eltwise_antq_en,
                                       bool c0_pad_en)
__aicore__ inline void asc_copy_l0c2l1(__cbuf__ <dst_dtype>* dst,
                                       __cc__ <src_dtype>* src,
                                       uint16_t n_size,
                                       uint16_t m_size,
                                       uint32_t dst_stride,
                                       uint16_t src_stride,
                                       uint8_t enable_clip_relu_pre,
                                       uint8_t unit_flag_mode,
                                       uint64_t quant_pre_mode,
                                       uint8_t relu_pre_mode,
                                       bool enable_channel_split,
                                       bool enable_nz2nd,
                                       uint64_t quant_post,
                                       uint8_t relu_post,
                                       bool clip_relu_post,
                                       uint8_t eltwise_op,
                                       uint8_t eltwise_antq_en,
                                       bool c0_pad_en,
                                       bool enable_nz2dn)
__aicore__ inline void asc_copy_l0c2l1(__cbuf__ void* dst,
                                       __cc__ <src_dtype>* src,
                                       uint16_t n_size,
                                       uint16_t m_size,
                                       uint32_t dst_stride,
                                       uint16_t src_stride,
                                       uint8_t l2_cache_mode,
                                       uint8_t enable_clip_relu_pre,
                                       uint8_t unit_flag_mode,
                                       uint64_t quant_pre_mode,
                                       uint8_t relu_pre_mode,
                                       bool enable_channel_split,
                                       bool enable_nz2nd,
                                       uint64_t quant_post,
                                       uint8_t relu_post,
                                       bool clip_relu_post,
                                       uint8_t eltwise_op,
                                       bool eltwise_antq_en,
                                       bool c0_pad_en,
                                       bool broadcast_en,
                                       bool enable_nz2dn)
__aicore__ inline void asc_copy_l0c2l1(__cbuf__ void* dst,
                                       __cc__ <src_dtype>* src,
                                       uint16_t n_size,
                                       uint16_t m_size,
                                       uint32_t dst_stride,
                                       uint16_t src_stride,
                                       bool quant_pre_rnd,
                                       uint8_t enable_clip_relu_pre,
                                       uint8_t unit_flag_mode,
                                       uint64_t quant_pre_mode,
                                       uint8_t relu_pre_mode,
                                       bool enable_channel_split,
                                       bool enable_nz2nd,
                                       uint64_t quant_post,
                                       uint8_t relu_post,
                                       bool clip_relu_post,
                                       uint8_t eltwise_op,
                                       bool eltwise_antq_en,
                                       bool c0_pad_en,
                                       bool broadcast_en,
                                       bool enable_nz2dn)
```

### dtype支持的数据类型

src dtype与dst dtype支持以下组合：

- `src_dtype`为`float`时：
    - 基础形式与`quant_pre_rnd`形式下，`dst_dtype`支持`half`、`bfloat16_t`、`int8_t`、`uint8_t`、`float`。
    - `enable_nz2dn`形式下，`dst_dtype`支持`bfloat16_t`、`int8_t`、`uint8_t`、`float`、`hifloat8_t`、`fp8_e4m3fn_t`。
    - `quant_pre_rnd`与`enable_nz2dn`组合形式下，`dst_dtype`仅支持`half`。
- `src_dtype`为`int32_t`时：
    - 基础形式与`quant_pre_rnd`形式下，`dst_dtype`支持`half`、`int8_t`、`uint8_t`、`int32_t`。
    - `enable_nz2dn`形式下，`dst_dtype`支持`half`、`bfloat16_t`、`int8_t`、`uint8_t`、`int32_t`。
- `dst`为`void*`的形式下，`src_dtype`支持`float`、`int32_t`，输出数据类型由`quant_pre_mode`确定的量化模式决定（如REQ4/VREQ4量化输出`int4b_t`类型）。

### 函数原型典型示例

```c
// 示例1：将float类型源矩阵搬运并转换为half类型。
__aicore__ inline void asc_copy_l0c2l1(__cbuf__ half* dst,
                                       __cc__ float* src,
                                       uint16_t n_size,
                                       uint16_t m_size,
                                       uint32_t dst_stride,
                                       uint16_t src_stride,
                                       uint8_t enable_clip_relu_pre,
                                       uint8_t unit_flag_mode,
                                       uint64_t quant_pre_mode,
                                       uint8_t relu_pre_mode,
                                       bool enable_channel_split,
                                       bool enable_nz2nd,
                                       uint64_t quant_post,
                                       uint8_t relu_post,
                                       bool clip_relu_post,
                                       uint8_t eltwise_op,
                                       uint8_t eltwise_antq_en,
                                       bool c0_pad_en)
// 示例2：dst为void*的形式，输出数据类型由quant_pre_mode确定。
__aicore__ inline void asc_copy_l0c2l1(__cbuf__ void* dst,
                                       __cc__ float* src,
                                       uint16_t n_size,
                                       uint16_t m_size,
                                       uint32_t dst_stride,
                                       uint16_t src_stride,
                                       uint8_t l2_cache_mode,
                                       uint8_t enable_clip_relu_pre,
                                       uint8_t unit_flag_mode,
                                       uint64_t quant_pre_mode,
                                       uint8_t relu_pre_mode,
                                       bool enable_channel_split,
                                       bool enable_nz2nd,
                                       uint64_t quant_post,
                                       uint8_t relu_post,
                                       bool clip_relu_post,
                                       uint8_t eltwise_op,
                                       bool eltwise_antq_en,
                                       bool c0_pad_en,
                                       bool broadcast_en,
                                       bool enable_nz2dn)
```

## 参数说明

**表1** 参数说明

| 参数名 | 输入/输出 | 描述 |
|:-----------------|:------|:-----------------------------------|
| dst | 输出 | 目的操作数的起始地址。数据格式为NZ、ND或DN格式，地址需要满足32字节对齐。 |
| src | 输入 | 源操作数的起始地址。数据格式为NZ格式，地址需要满足64字节对齐。 |
| n_size | 输入 | 源NZ矩阵在N方向上的大小，取值范围：[1, 4095]。 |
| m_size | 输入 | 源NZ矩阵在M方向上的大小。<br>&nbsp;&nbsp;&bull; 不开启NZ2ND功能，取值范围：[1, 65535]；<br>&nbsp;&nbsp;&bull; 开启NZ2ND功能，取值范围：[1, 8192]。 |
| dst_stride | 输入 | &nbsp;&nbsp;&bull; 若不开启NZ2ND功能，dst_stride表示目的NZ矩阵中相邻Z排布的起始地址偏移，取值不为0，单位：element。<br>&nbsp;&nbsp;&bull; 若开启NZ2ND/NZ2DN功能，dst_stride表示目的ND矩阵每一行中的元素个数，取值不为0，单位：element。 |
| src_stride | 输入 | 源NZ矩阵中相邻Z排布的起始地址偏移，取值范围：[0, 65535]，单位：C0_Size(16*sizeof(T)，T为src的数据类型)。 |
| quant_pre_rnd | 输入 | 预处理量化阶段的舍入模式控制。<br>&nbsp;&nbsp;&bull; `false`：关闭；<br>&nbsp;&nbsp;&bull; `true`：开启，舍入行为与`quant_pre_mode`确定的量化模式相关。 |
| l2_cache_mode | 输入 | 无效参数，用户无需关注，传入0即可。 |
| enable_clip_relu_pre | 输入 | 是否开启Clip ReLU，需搭配Normal ReLU一起使用，且需要开启量化功能，取值如下：<br>&nbsp;&nbsp;&bull; 0：不开启Clip ReLU；<br>&nbsp;&nbsp;&bull; 1：开启Clip ReLU（scalar 模式）。 |
| unit_flag_mode | 输入 | unit_flag是mmad类指令和矩阵搬出类指令细粒度的并行功能，开启该功能后，硬件每计算完一个分形，计算结果就会被搬出。取值说明如下：<br>&nbsp;&nbsp;&bull; 0：不开启unit_flag；<br>&nbsp;&nbsp;&bull; 2：开启unit_flag，硬件执行完指令之后，不复位单元标记位；<br>&nbsp;&nbsp;&bull; 3：开启unitFlag，硬件执行完指令之后，复位单元标记位。<br>开启该功能时，须将mmad类指令和矩阵搬出类指令的unit_flag值设置为2或3。 |
| quant_pre_mode | 输入 | 预处理阶段量化参数。取值见[功能说明](#功能说明)。 |
| relu_pre_mode | 输入 | 预处理阶段ReLU模式控制，取值如下：<br>&nbsp;&nbsp;&bull; 0：不开启ReLU；<br>&nbsp;&nbsp;&bull; 1：开启Normal ReLU；<br>&nbsp;&nbsp;&bull; 2：开启Scalar ReLU；<br>&nbsp;&nbsp;&bull; 3：开启Vector ReLU。 |
| enable_channel_split | 输入 | 是否开启通道拆分的功能。仅在src和dst都为float时才能开启Channel Split，且不能同时开启Channel Split和NZ2ND功能。<br>&nbsp;&nbsp;&bull; false：不开启；<br>&nbsp;&nbsp;&bull; true：开启。 |
| enable_nz2nd | 输入 | 是否开启NZ2ND功能。<br>&nbsp;&nbsp;&bull; false：不开启；<br>&nbsp;&nbsp;&bull; true：开启。 |
| quant_post | 输入 | 无效参数，用户无需关注，传入0即可。 |
| relu_post | 输入 | 无效参数，用户无需关注，传入0即可。 |
| clip_relu_post | 输入 | 无效参数，用户无需关注，传入false即可。 |
| eltwise_op | 输入 | 无效参数，用户无需关注，传入0即可。 |
| eltwise_antq_en | 输入 | 无效参数，用户无需关注，传入false或0即可。 |
| c0_pad_en | 输入 | 无效参数，用户无需关注，传入false即可。 |
| broadcast_en | 输入 | 预留参数，当前须设置为`false`。 |
| enable_nz2dn | 输入 | 是否开启NZ2DN功能。<br>&nbsp;&nbsp;&bull; false：不开启；<br>&nbsp;&nbsp;&bull; true：开启。 |

## 返回值说明

无

## 流水类型

PIPE_FIX

## 约束说明

### 通用约束

- 本接口仅在AIC上执行有效，非AIC调用直接返回。
- 各存储单元的空间大小和对齐要求请参考[存储单元说明](../../general_description_and_constraints.md#存储单元说明)。

### 随路转换约束

- 随路scalar量化场景下，须先调用asc_set_l0c_copy_prequant接口配置量化计算所需的缩放系数、偏移量和量化结果符号，再搭配本接口`quant_pre_mode`使用。
- NZ2ND/NZ2DN格式转换场景下，须先调用asc_set_l0c_copy_nz_para接口配置格式转换参数，再搭配本接口`enable_nz2nd`或`enable_nz2dn`使用。
- src与dst dtype组合需与`quant_pre_mode`量化模式匹配，否则会导致搬运结果不符合预期。
- `dst`为`void*`的形式下，`quant_pre_mode`确定的量化模式决定了实际输出数据类型，需按输出数据类型的大小进行地址对齐与空间预留。
- `enable_channel_split`仅在src和dst均为float时可开启，且不能与NZ2ND功能同时开启。

## 调用示例

```cpp
// dst和src分别对应目的操作数的输出地址和源操作数的输入地址
constexpr uint64_t total_length = 128 * 64;
__cbuf__ int32_t dst[total_length];
__cc__ int32_t src[total_length];
// 其余入参均已默认数值传入
uint16_t n_size = 64;
uint16_t m_size = 128;
uint32_t dst_stride = 2048;
uint16_t src_stride = 128;
uint8_t enable_clip_relu_pre = 0;
uint8_t unit_flag_mode = 0;
uint64_t quant_pre_mode = QuantMode_t::NoQuant;
uint8_t relu_pre_mode = 0;
bool enable_channel_split = false;
bool enable_nz2nd = false;
uint64_t quant_post = 0;
uint8_t relu_post = 0;
bool clip_relu_post = false;
uint8_t eltwise_op = 0;
uint8_t eltwise_antq_en = 0;
bool c0_pad_en = false;
asc_copy_l0c2l1(
    dst, src, n_size, m_size, dst_stride, src_stride, enable_clip_relu_pre, unit_flag_mode,
    quant_pre_mode, relu_pre_mode, enable_channel_split, enable_nz2nd, quant_post, relu_post,
    clip_relu_post, eltwise_op, eltwise_antq_en, c0_pad_en);
```

```cpp
// dst为void*的变体：源矩阵为float类型，随路量化输出int4b_t类型数据
constexpr uint64_t total_length = 128 * 64;
__cbuf__ int4b_t dst[total_length];
__cc__ float src[total_length];
uint16_t n_size = 64;
uint16_t m_size = 128;
uint32_t dst_stride = 2048;
uint16_t src_stride = 128;
uint8_t l2_cache_mode = 0;
uint8_t enable_clip_relu_pre = 0;
uint8_t unit_flag_mode = 0;
uint64_t quant_pre_mode = QuantMode_t::QF322S4_PRE;  // float随路量化成int4b_t
uint8_t relu_pre_mode = 0;
bool enable_channel_split = false;
bool enable_nz2nd = false;
uint64_t quant_post = 0;
uint8_t relu_post = 0;
bool clip_relu_post = false;
uint8_t eltwise_op = 0;
bool eltwise_antq_en = false;
bool c0_pad_en = false;
bool broadcast_en = false;
bool enable_nz2dn = false;
asc_copy_l0c2l1(reinterpret_cast<__cbuf__ void*>(dst), src, n_size, m_size, dst_stride,
    src_stride, l2_cache_mode, enable_clip_relu_pre, unit_flag_mode, quant_pre_mode, relu_pre_mode,
    enable_channel_split, enable_nz2nd, quant_post, relu_post, clip_relu_post, eltwise_op,
    eltwise_antq_en, c0_pad_en, broadcast_en, enable_nz2dn);
```
