# asc_copy_l0c2ub

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

将矩阵计算结果从L0C Buffer搬运至Unified Buffer，搬运过程中可同步支持随路量化、随路激活、随路格式转换（NZ2ND/NZ2DN）等能力组合。

下图展示了随路量化、随路ReLU、随路格式转换、随路通道拆分以及随路通道合并的有效组合、中间数据类型和数据路径。下图中的F32-\>F16与F32-\>BF16为Cast，其余为随路scalar/tensor量化模式。

**图1** asc_copy_l0c2ub随路功能组合

![](../../../figures/C_API_L0C2UB_Function_Combination_950.png)

本接口支持多种随路能力的组合，需通过配套接口预先配置量化参数、激活参数、通道参数等寄存器，再调用本接口完成搬运。

- NZ2ND格式转换场景下，需通过[asc_set_l0c2gm_nz2nd](../../cube_compute/asc_set_l0c2gm_nz2nd.md)预先配置格式转换参数，并且需要搭配本接口`enable_nz2nd`使用；
- NZ2DN格式转换场景下，需通过[asc_set_l0c2gm_nz2nd](../../cube_compute/asc_set_l0c2gm_nz2nd.md)、[asc_set_l0c2gm_channel_para](../asc_set_l0c2gm_channel_para.md)预先配置格式转换参数，并且需要搭配本接口`enable_nz2dn`使用；
- 随路scalar量化模式下，需通过[asc_set_l0c_copy_prequant](../asc_set_l0c_copy_prequant.md)设置随路scalar量化参数，并且需要搭配本接口`quant_pre_mode`使用;
- 随路tensor量化模式下，需通过[asc_set_l0c2gm_config](../../cube_compute/asc_set_l0c2gm_config.md)设置随路tensor量化使用tensor的起始地址，其中量化tensor的每个元素都代表一个量化参数，并且需要搭配本接口`quant_pre_mode`使用;
- 随路激活模式下，需通过[asc_set_l0c2gm_relu_alpha](../asc_set_l0c2gm_relu_alpha.md)、[asc_set_l0c2gm_lrelu_alpha](../asc_set_l0c2gm_lrelu_alpha.md)预先配置ReLU/Leaky ReLU激活参数，并且需要搭配本接口`enable_clip_relu_pre`与`relu_pre_mode`使用；

`quant_pre_mode`量化模式参数支持的枚举值如下：

- NoQuant：不开启量化功能。
- DEQF16：int32_t量化成half， scalar量化。
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
- QF322F32_PRE：float量化成float，scalar量化，精度可以达到双千分之一，无法达到双万分之一。
- VQF322F32_PRE：float量化成float，tensor量化，精度可以达到双千分之一，无法达到双万分之一。

本接口仅在AIC上执行有效，非AIC调用直接返回。

## 函数原型

### 模板原型（占位符形式）

```cpp
__aicore__ inline void asc_copy_l0c2ub(__ubuf__ <dst_dtype>* dst,
                                       __cc__ <src_dtype>* src,
                                       uint16_t n_size,
                                       uint16_t m_size,
                                       uint32_t dst_stride,
                                       uint16_t src_stride,
                                       uint8_t dual_dst_ctrl,
                                       bool sub_blockid,
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
__aicore__ inline void asc_copy_l0c2ub_sync(__ubuf__ <dst_dtype>* dst,
                                            __cc__ <src_dtype>* src,
                                            uint16_t n_size,
                                            uint16_t m_size,
                                            uint32_t dst_stride,
                                            uint16_t src_stride,
                                            uint8_t dual_dst_ctrl,
                                            bool sub_blockid,
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

- `src_dtype`为`int32_t`时，`dst_dtype`支持`int4b_t`、`int8_t`、`uint8_t`、`half`、`bfloat16_t`、`int32_t`。
- `src_dtype`为`float`时，`dst_dtype`支持`int4b_t`、`int8_t`、`uint8_t`、`hifloat8_t`、`fp8_e4m3fn_t`、`half`、`bfloat16_t`、`float`。

### 典型示例

```cpp
// 示例：将float类型数据转换为half类型后搬运。
__aicore__ inline void asc_copy_l0c2ub(__ubuf__ half* dst,
                                       __cc__ float* src,
                                       uint16_t n_size,
                                       uint16_t m_size,
                                       uint32_t dst_stride,
                                       uint16_t src_stride,
                                       uint8_t dual_dst_ctrl,
                                       bool sub_blockid,
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
|---|---|---|
| `dst` | 输出 | 目的操作数，存储位置为UB。目的地址需32字节对齐。 |
| `src` | 输入 | 源操作数，存储位置为L0C Buffer。源地址需64字节对齐。 |
| `n_size` | 输入 | 源Nz矩阵在N方向上的大小，取值范围：[1, $2^12−1$]。<br> 对于Nz输出场景：<br>&nbsp;&nbsp;&bull; 若`enable_channel_split`设置为`true`开启channelSplit功能，`n_size`必须为8的倍数。<br>&nbsp;&nbsp;&bull; 若不开启channelSplit功能，`n_size`必须为16的倍数。<br> 对于ND输出场景：<br>&nbsp;&nbsp;&bull; n_size*sizeof(dst_dtype)必须为32的倍数，若`dst_dtype`设置为`b4`类型, `n_size`必须为64的倍数。 |
| `m_size` | 输入 | 源Nz矩阵在M方向上的大小，取值范围：[1, $2^16−1$]。<br> 对于DN输出场景：<br>&nbsp;&nbsp;&bull; m_size*sizeof(dst_dtype)必须为32的倍数，若`dst_dtype`设置为`b4`类型, `m_size`必须为64的倍数。 |
| `dst_stride` | 输入 | 目的矩阵步长，取值范围：[1, $2^32−1$]，需32字节对齐。<br>&nbsp;&nbsp;&bull; 若不开启NZ2ND功能，dst_stride表示目的Nz矩阵中相邻Z排布的起始地址偏移，单位：元素。<br>&nbsp;&nbsp;&bull; 若开启NZ2ND/NZ2DN功能，dst_stride表示目的ND/DN矩阵每一行中的元素个数，单位：元素，对于不同`dst_type`的对齐约束如下：`b4`输出需为64的倍数，`b8`输出需为32的倍数，`b16`输出需为16的倍数，`b32`输出需为8的倍数。 |
| `src_stride` | 输入 | 源Nz矩阵中相邻Z排布的起始地址偏移，单位64字节(16*sizeof(src_dtype))，取值范围：[0, $2^16−1$]。 |
| `dual_dst_ctrl` | 输入 | 双目标模式控制参数。当同一AI Core内有一个Cube Core和两个Vector Core时，如果启用双目标模式控制，L0C Buffer中的M * N矩阵将被分成两半，并同时分别写入两个Vector Core各自的UB中，其中前半部分写入SUB BLOCK0，后半部分写入SUB BLOCK1，参数取值说明如下：<br>&nbsp;&nbsp;&bull; 0：单目标模式，将整个矩阵写入通过sub_blockid参数配置的目标UB。<br>&nbsp;&nbsp;&bull; 1：双目标模式，按M维度拆分成形状为M / 2 \* N的两个矩阵，分别写入两个UB, M必须为2的倍数。<br>&nbsp;&nbsp;&bull; 2：双目标模式，按N维度拆分成形状为M \* N / 2的两个矩阵，分别写入两个UB, N须为32的倍数。<br>**dual_dst_ctrl仅支持在普通搬运模式（NZ2NZ）或NZ2ND搬运场景下使用，不支持其他随路功能场景。** |
| `sub_blockid` | 输入 | 启用单目标模式时用于指示目标UB的SUB BLOCK ID。取值为0或1，取值为0时写入SUB BLOCK0，为1时写入SUB BLOCK1。 |
| `enable_clip_relu_pre` | 输入 | 是否开启Clip ReLU，需搭配Normal ReLU一起使用，且需要开启量化功能，取值如下：<br>&nbsp;&nbsp;&bull; `0`：不开启Clip ReLU；<br>&nbsp;&nbsp;&bull; `1`：开启Clip ReLU（scalar 模式）。 |
| `unit_flag_mode` | 输入 | UnitFlag是mmad类指令和矩阵搬出类指令细粒度的并行功能，开启该功能后，硬件每计算完一个分形，计算结果就会被搬出。取值说明如下：<br>&nbsp;&nbsp;&bull; `0`：不开启UnitFlag；<br>&nbsp;&nbsp;&bull; `2`：开启UnitFlag，硬件执行完指令之后，不复位单元标记位；<br>&nbsp;&nbsp;&bull; `3`：开启UnitFlag，硬件执行完指令之后，复位单元标记位。<br>开启该功能时，须将mmad类指令和矩阵搬出类指令的UnitFlag值设置为2或3。 |
| `quant_pre_mode` | 输入 | 预处理阶段量化模式。取值见[功能说明](#功能说明)。 |
| `relu_pre_mode` | 输入 | 预处理阶段ReLU模式控制，取值如下：<br>&nbsp;&nbsp;&bull; 0：不开启ReLU；<br>&nbsp;&nbsp;&bull; `1`：开启Normal ReLU；<br>&nbsp;&nbsp;&bull; `2`：开启Scalar ReLU；<br>&nbsp;&nbsp;&bull; `3`：开启Vector ReLU。 |
| `enable_channel_split` | 输入 | 是否开启通道拆分的功能。仅在src和dst都为float时，且输出为Nz才能开启Channel Split，且不能同时开启Channel Split。<br>&nbsp;&nbsp;&bull; `false`：不开启；<br>&nbsp;&nbsp;&bull; `true`：开启。 |
| `enable_nz2nd` | 输入 | NZ2ND格式转换使能。<br>&nbsp;&nbsp;&bull; `false`：关闭NZ2ND转换；<br>&nbsp;&nbsp;&bull; `true`：开启NZ2ND转换。 |
| quant_post | 输入 | 预留参数。为后续的功能做保留，开发者暂时无需关注，填0即可。 |
| relu_post | 输入 | 预留参数。为后续的功能做保留，开发者暂时无需关注，填0即可。 |
| clip_relu_post | 输入 | 预留参数。为后续的功能做保留，开发者暂时无需关注，填0即可。 |
| eltwise_op | 输入 | 预留参数。为后续的功能做保留，开发者暂时无需关注，填0即可。 |
| eltwise_antq_en | 输入 | 预留参数。为后续的功能做保留，开发者暂时无需关注，填false即可。 |
| c0_pad_en | 输入 | 预留参数。为后续的功能做保留，开发者暂时无需关注，填false即可。 |
| broadcast_en | 输入 | 预留参数。为后续的功能做保留，开发者暂时无需关注，填false即可。 |
| `enable_nz2dn` | 输入 | NZ2DN格式转换使能。<br>&nbsp;&nbsp;&bull; `false`：关闭NZ2DN转换；<br>&nbsp;&nbsp;&bull; `true`：开启NZ2DN转换。 |

## 返回值说明

无

## 流水类型

PIPE_FIX

## 约束说明


### 通用约束

- 本接口仅在AIC上生效，非AIC调用直接返回。
- dst起始地址需32字节对齐（UB对齐要求），否则触发异常。
- src起始地址需64字节对齐（L0C Buffer对齐要求），否则触发异常。
- 如果本指令与其他指令存在目的地址重叠，需要插入同步指令（[asc_sync_notify](../sync/asc_sync_notify.md)和[asc_sync_wait](../sync/asc_sync_wait.md)），保证多个指令串行化，防止出现异常数据。
- UB容量上限：UB总容量为256KB，默认预留6KB SIMD VF栈与2KB Ascend C预留空间后可用248KB；SIMD与SIMT混编时再划分32KB~128KB作Data Cache，可用容量进一步减少。dst偏移量与搬运大小之和不可超过实际可用容量，否则触发写溢出异常。
- L0C Buffer容量上限：L0C Buffer总容量256KB，src偏移量与搬运大小之和不可超过L0C Buffer容量，否则触发读溢出异常。

### 随路转换约束

- `n_size`、`m_size`、`dst_stride`需根据dtype与功能模式确定对齐约束，详见参数说明，不满足对齐约束会导致搬运结果不符合预期。
- src与dst dtype组合需与`quant_pre_mode`量化模式匹配，否则会导致搬运结果不符合预期。
- `dual_dst_ctrl`仅支持在普通搬运模式（NZ2NZ）或NZ2ND搬运场景下使用，不支持其他随路功能场景。
- `enable_channel_split`仅在输出dtype为`float`且输出为NZ格式时可设为true。
- 量化与激活模式中使用的量化系数不可为INF/NaN和非规格化数，否则会导致量化激活结果错误。
- 开启NZ2DN转换时，需通过[asc_set_l0c2gm_channel_para](../../sys_var/asc_set_l0c2gm_channel_para.md)预先配置源矩阵步长，且源矩阵步长不可为0，否则会导致搬运异常。
- 开启NZ2DN转换时，仅当通过[asc_set_l0c2gm_channel_para](../../sys_var/asc_set_l0c2gm_channel_para.md)配置源矩阵步长为1时，可同时开启unitFlag功能。
- `enable_clip_relu_pre`设为Clip ReLU（标量模式）时需搭配`relu_pre_mode`与量化功能一起使用。

## 调用示例

```cpp
// dst和src分别对应目的操作数的输出地址和源操作数的输入地址
constexpr uint64_t total_length = 128 * 64;
__ubuf__ float dst[total_length];
__cc__ float src[total_length];
uint16_t n_size =   64;
uint16_t m_size = 128;
uint32_t dst_stride = 2048;
uint16_t src_stride = 128;
uint8_t dual_dst_ctrl = 0;
bool sub_blockid = false;
uint8_t enable_clip_relu_pre = 0;
uint8_t unit_flag_mode = 0;
uint64_t quant_pre_mode = QuantMode_t::NoQuant;
uint8_t relu_pre_mode = 0;
bool enable_channel_split = false;
bool enable_nz2nd = false;
bool enable_nz2dn = false;
uint64_t quant_post = 0;  // 无效参数
uint8_t relu_post = 0;  // 无效参数
bool clip_relu_post = false;  // 无效参数
uint8_t eltwise_op = 0;  // 无效参数
bool eltwise_antq_en = false;  // 无效参数
bool c0_pad_en = false;  // 无效参数
bool broadcast_en = false;  // 无效参数
asc_copy_l0c2ub(dst, src, n_size, m_size, dst_stride, src_stride, dual_dst_ctrl, sub_blockid,
    enable_clip_relu_pre, unit_flag_mode, quant_pre_mode, relu_pre_mode, enable_channel_split,
    enable_nz2nd, quant_post, relu_post, clip_relu_post, eltwise_op, eltwise_antq_en,
    c0_pad_en, broadcast_en, enable_nz2dn);
```
