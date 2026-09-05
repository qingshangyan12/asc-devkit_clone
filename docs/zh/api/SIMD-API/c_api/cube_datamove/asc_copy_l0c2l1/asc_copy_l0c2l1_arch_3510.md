# asc_copy_l0c2l1

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

头文件路径为：`"c_api/cube_datamove/cube_datamove.h"`。

将矩阵计算结果从L0C Buffer搬运到L1 Buffer。搬运过程中可组合随路量化、ReLU、Nz2ND格式转换和通道拆分能力，具体支持的dtype组合及参数以函数原型为准。

下图展示了随路量化、随路ReLU、随路格式转换、随路通道拆分以及随路通道合并的有效组合、中间数据类型和数据路径。下图中的F32-\>F16与F32-\>BF16为Cast，其余为随路scalar/tensor量化模式。

**图1** asc_copy_l0c2l1随路功能组合

![](../../../../figures/C_API_L0C2L1_Function_Combination_950.png)

quant_pre_mode量化模式参数支持的枚举值如下：

- NoQuant：不开启量化功能
- DEQF16：int32_t量化成half， scalar量化
- VDEQF16：int32_t量化成half，tensor量化
- REQ4：int32_t量化成int4b_t，scalar量化
- VREQ4：int32_t量化成int4b_t，tensor量化
- REQ8：int32_t量化成int8_t/uint8_t，scalar量化
- VREQ8：int32_t量化成int8_t/uint8_t，tensor量化
- QS322BF16_PRE：int32_t量化成bfloat16_t，scalar量化
- VQS322BF16_PRE：int32_t量化成bfloat16_t，tensor量化
- QF322F16_PRE：float量化成half，scalar量化
- VQF322F16_PRE：float量化成half，tensor量化
- QF322BF16_PRE：float量化成bfloat16_t，scalar量化
- VQF322BF16_PRE：float量化成bfloat16_t，tensor量化
- F322F16：float cast成half，cast mode为CAST_RINT模式
- F322BF16：float cast成bfloat16_t，cast mode为CAST_RINT模式
- QF322S4_PRE：float量化成int4b_t，scalar量化
- VQF322S4_PRE：float量化成int4b_t，tensor量化
- QF322B8_PRE：float量化成int8_t/uint8_t，scalar量化
- VQF322B8_PRE：float量化成int8_t/uint8_t，tensor量化
- QF322FP8_PRE：float量化成fp8_e4m3fn_t，scalar量化
- VQF322FP8_PRE：float量化成fp8_e4m3fn_t，tensor量化
- QF322HIF8_PRE：float量化成hifloat8_t(Half to Away Round)，scalar量化
- VQF322HIF8_PRE：float量化成hifloat8_t(Half to Away Round)，tensor量化
- QF322HIF8_PRE_HYBRID：float量化成hifloat8_t(Hybrid Round)，scalar量化
- VQF322HIF8_PRE_HYBRID：float量化成hifloat8_t(Hybrid Round)，tensor量化
- QF322F32_PRE：float量化成float，scalar量化，精度可以达到双千分之一，无法达到双万分之一。如果有双万分之一的精度要求，建议使用[AscendDeQuant](../../../adv_api/quantization/AscendDequant.md)高阶API。
- VQF322F32_PRE：float量化成float，tensor量化，精度可以达到双千分之一，无法达到双万分之一。如果有双万分之一的精度要求，建议使用[AscendDeQuant](../../../adv_api/quantization/AscendDequant.md)高阶API。

本接口仅在AIC上执行有效。

## 函数原型

```c
// 占位符形式
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
__aicore__ inline void asc_copy_l0c2l1(__cbuf__ int4b_t* dst,
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
__aicore__ inline void asc_copy_l0c2l1_sync(__cbuf__ int4b_t* dst,
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
```

### dtype支持的数据类型

- 普通原型中，`src_dtype`为`int32_t`时，`dst_dtype`支持`half`、`int8_t`、`uint8_t`、`int32_t`；`src_dtype`为`float`时，`dst_dtype`支持`half`、`bfloat16_t`、`int8_t`、`uint8_t`、`float`。
- 带`l2_cache_mode`及同步版本的原型中，`dst_dtype`为`int4b_t`，`src_dtype`支持`int32_t`、`float`。

### 函数原型典型示例

```c
// 示例：int8_t类型的搬运
__aicore__ inline void asc_copy_l0c2l1(__cbuf__ int8_t* dst,
                                       __cc__ int32_t* src,
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
```

## 参数说明

**表1** 参数说明

| 参数名 | 输入/输出 | 描述 |
|:-----------------|:------|:-----------------------------------|
| dst | 输出 | 目的操作数，存储位置为L1 Buffer。数据格式为Nz、ND或DN格式，起始地址需32字节对齐。 |
| src | 输入 | 源操作数，存储位置为L0C Buffer。数据格式为Nz格式，起始地址需64字节对齐。 |
| n_size | 输入 | 源Nz矩阵在N方向上的大小，取值范围：[1, 4095]。 |
| m_size | 输入 | 源Nz矩阵在M方向上的大小。<br>&nbsp;&nbsp;&bull; 不开启Nz2ND功能，取值范围：[1, 65535]；<br>&nbsp;&nbsp;&bull; 开启Nz2ND功能，取值范围：[1, 8192]。 |
| dst_stride | 输入 | &nbsp;&nbsp;&bull; 若不开启Nz2ND功能，dst_stride表示目的Nz矩阵中相邻Z排布的起始地址偏移，取值不为0，单位：element。<br>&nbsp;&nbsp;&bull; 若开启Nz2ND/Nz2DN功能，dst_stride表示目的ND矩阵每一行中的元素个数，取值不为0，单位：element。 |
| src_stride | 输入 | 源Nz矩阵中相邻Z排布的起始地址偏移，取值范围：[0, 65535]，单位：C0_Size(16*sizeof(T)，T为src的数据类型)。 |
| l2_cache_mode | 输入 | 无效参数，用户无需关注，传入0即可。 |
| enable_clip_relu_pre | 输入 | 是否开启Clip ReLU，需搭配Normal ReLU一起使用，且需要开启量化功能，取值如下：<br>&nbsp;&nbsp;&bull; 0：不开启Clip ReLU；<br>&nbsp;&nbsp;&bull; 1：开启Clip ReLU（scalar 模式）。 |
| unit_flag_mode | 输入 | unit_flag是mmad类指令和矩阵搬出类指令细粒度的并行功能，开启该功能后，硬件每计算完一个分形，计算结果就会被搬出。取值说明如下：<br>&nbsp;&nbsp;&bull; 0：不开启unit_flag；<br>&nbsp;&nbsp;&bull; 2：开启unit_flag，硬件执行完指令之后，不复位单元标记位；<br>&nbsp;&nbsp;&bull; 3：开启unitFlag，硬件执行完指令之后，复位单元标记位。<br>开启该功能时，须将mmad类指令和矩阵搬出类指令的unit_flag值设置为2或3。 |
| quant_pre_mode | 输入 | 预处理阶段量化参数。取值见[功能说明](#功能说明)。 |
| relu_pre_mode | 输入 | 预处理阶段ReLU模式控制，取值如下：<br>&nbsp;&nbsp;&bull; 0：不开启ReLU；<br>&nbsp;&nbsp;&bull; 1：开启Normal ReLU；<br>&nbsp;&nbsp;&bull; 2：开启Scalar ReLU；<br>&nbsp;&nbsp;&bull; 3：开启Vector ReLU。 |
| enable_channel_split | 输入 | 是否开启通道拆分的功能。仅在src和dst都为float时才能开启Channel Split，且不能同时开启Channel Split和Nz2ND功能。<br>&nbsp;&nbsp;&bull; false：不开启；<br>&nbsp;&nbsp;&bull; true：开启。 |
| enable_nz2nd | 输入 | 是否开启Nz2ND功能。<br>&nbsp;&nbsp;&bull; false：不开启；<br>&nbsp;&nbsp;&bull; true：开启。 |
| quant_post | 输入 | 无效参数，用户无需关注，传入0即可。 |
| relu_post | 输入 | 无效参数，用户无需关注，传入0即可。 |
| clip_relu_post | 输入 | 无效参数，用户无需关注，传入false即可。 |
| eltwise_op | 输入 | 无效参数，用户无需关注，传入0即可。 |
| eltwise_antq_en | 输入 | 无效参数，用户无需关注，传入false或0即可。 |
| c0_pad_en | 输入 | 无效参数，用户无需关注，传入false即可。 |
| broadcast_en | 输入 | 无效参数，用户无需关注，传入false即可。 |
| enable_nz2dn | 输入 | 是否开启Nz2DN功能。<br>&nbsp;&nbsp;&bull; false：不开启；<br>&nbsp;&nbsp;&bull; true：开启。 |

## 返回值说明

无

## 流水类型

PIPE_FIX

## 约束说明

### 通用约束

- 本接口仅在AIC上生效，非AIC调用直接返回。
- `dst`起始地址需32字节对齐（L1 Buffer对齐要求），否则会导致搬运异常。
- `src`起始地址需64字节对齐（L0C Buffer对齐要求，`src`为`int32_t`或`float`时），否则会导致搬运异常。
- 如果本指令与其他指令存在目的地址重叠，需要插入同步指令（[asc_sync_notify](../../sync/asc_sync_notify.md)和[asc_sync_wait](../../sync/asc_sync_wait.md)），保证多个指令串行化，防止出现异常数据。
- 接口使用随路量化、随路ReLU/Clip ReLU、通道拆分、Nz2ND/Nz2DN等随路能力前，须先调用配套setter接口预先配置对应参数（量化系数、ReLU alpha、Clip ReLU上限、通道步长等），再调用本搬运接口，否则随路处理使用未初始化的配置值产生错误结果。
- L0C Buffer容量上限：L0C Buffer总容量256KB，`src`偏移加搬运大小不可越界，否则触发异常。
- L1 Buffer容量上限：L1 Buffer总容量512KB，`dst`偏移加搬运大小不可越界，否则触发异常。

### 随路转换约束

- `n_size`、`m_size`、`dst_stride`需根据dtype与功能模式确定对齐约束，详见参数说明，不满足对齐约束会导致搬运结果不符合预期。
- src与dst dtype组合需与`quant_pre_mode`量化模式匹配，否则会导致搬运结果不符合预期。
- `enable_channel_split`仅在输出dtype为`float`且输出为Nz格式时可设为true。
- 量化与激活模式中使用的量化系数不可为INF/NaN和非规格化数，否则会导致量化激活结果错误。
- 开启Nz2DN转换时，需通过[asc_set_l0c_copy_channel_para](asc_set_l0c_copy_channel_para.md)预先配置源矩阵步长，且源矩阵步长不可为0，否则会导致搬运异常。
- 开启Nz2DN转换时，仅当通过[asc_set_l0c_copy_channel_para](asc_set_l0c_copy_channel_para.md)配置源矩阵步长为1时，可同时开启UnitFlag功能。
- `enable_clip_relu_pre`设为Clip ReLU（标量模式）时需搭配`relu_pre_mode`与量化功能一起使用。

## 调用示例

```cpp
// 一个16×16 int32_t分形从L0C搬运到L1，关闭量化、激活和格式转换。
__cbuf__ int32_t dst[256];
__cc__ int32_t src[256];
// dst_stride表示目的Nz矩阵相邻Z排布的起始地址偏移；src_stride单位为64B。
asc_copy_l0c2l1(
    dst, src, 16, 16, 16, 1, 0, 0, static_cast<uint64_t>(QuantMode_t::NoQuant), 0,
    false, false, 0, 0, false, 0, 0, false);
```
