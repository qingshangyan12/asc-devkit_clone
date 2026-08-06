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

该接口用于将矩阵计算的结果从L0C Buffer搬运至L1 Buffer中，并且在搬运过程中支持与其它接口组合使用，配置多种随路能力。（目前针对量化模式的支持尚不完全，具体支持情况请以[函数原型](#函数原型)中包含的接口为准）

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

## 函数原型

```cpp
__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ half* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode, uint64_t quant_pre_mode,
    uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, uint8_t eltwise_antq_en, bool c0_pad_en)
__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ bfloat16_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode, uint64_t quant_pre_mode,
    uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, uint8_t eltwise_antq_en, bool c0_pad_en)
__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ int8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode, uint64_t quant_pre_mode,
    uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, uint8_t eltwise_antq_en, bool c0_pad_en)
__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ uint8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode, uint64_t quant_pre_mode,
    uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, uint8_t eltwise_antq_en, bool c0_pad_en)
__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ float* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode, uint64_t quant_pre_mode,
    uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, uint8_t eltwise_antq_en, bool c0_pad_en)
__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ half* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode, uint64_t quant_pre_mode,
    uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, uint8_t eltwise_antq_en, bool c0_pad_en)
__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ int8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode, uint64_t quant_pre_mode,
    uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, uint8_t eltwise_antq_en, bool c0_pad_en)
__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ uint8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode, uint64_t quant_pre_mode,
    uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, uint8_t eltwise_antq_en, bool c0_pad_en)
__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ int32_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode, uint64_t quant_pre_mode,
    uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, uint8_t eltwise_antq_en, bool c0_pad_en)
__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ int4b_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    uint64_t quant_post, uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op,
    bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en, bool enable_nz2dn)
__aicore__ inline void asc_copy_l0c2l1_sync(
    __cbuf__ int4b_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    uint64_t quant_post, uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op,
    bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en, bool enable_nz2dn)
__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ int4b_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    uint64_t quant_post, uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op,
    bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en, bool enable_nz2dn)
__aicore__ inline void asc_copy_l0c2l1_sync(
    __cbuf__ int4b_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    uint64_t quant_post, uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op,
    bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en, bool enable_nz2dn)
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
| broadcast_en | 输入 | 无效参数，用户无需关注，传入false即可。 |
| enable_nz2dn | 输入 | 是否开启NZ2DN功能。<br>&nbsp;&nbsp;&bull; false：不开启；<br>&nbsp;&nbsp;&bull; true：开启。 |

## 返回值说明

无

## 流水类型

PIPE_FIX

## 约束说明

- 各存储单元的空间大小和对齐要求请参考[存储单元说明](../../general_description_and_constraints.md#存储单元说明)。

### 通用约束

- 本接口仅在AIC上生效，非AIC调用直接返回。
- dst起始地址需32字节对齐（L1 Buffer对齐要求），否则会导致搬运异常。
- src起始地址需64字节对齐（L0C Buffer对齐要求，src为`float`或`int32_t`时），否则会导致搬运异常。
- 如果需要执行多条本接口指令，且目的地址存在重叠，需要插入同步指令（`asc_sync_notify`和`asc_sync_wait`），保证多个指令串行化，防止出现异常数据。
- 接口使用随路量化、随路ReLU/Clip ReLU、通道拆分、NZ2ND/NZ2DN等随路能力前，须先调用配套setter接口预先配置对应参数（量化系数、ReLU alpha、Clip ReLU上限、通道步长等），再调用本搬运接口，否则随路处理使用未初始化的配置值产生错误结果。
- L0C Buffer容量上限：L0C Buffer总容量256KB，src偏移+搬运大小不可越界，否则触发异常。
- L1 Buffer容量上限：L1 Buffer总容量512KB，dst偏移+搬运大小不可越界，否则触发异常。

### 分形搬运模式约束

- n_size不可为0，否则触发异常。
- m_size不可为0，否则触发异常。
- dst_stride即使循环次数为0也不可为0，否则触发异常；同时需按dst dtype满足32字节对齐（`half`/`int16_t`需为16的倍数，`int8_t`/`uint8_t`需为32的倍数，`float`/`int32_t`需为8的倍数）。
- n_size需按dst dtype与`nz2nd_en`状态满足对齐约束：非NZ2ND模式下`half`/`int16_t`/`int8_t`/`uint8_t`需为16的倍数、`float`/`int32_t`需为8的倍数；NZ2ND模式下`int8_t`/`uint8_t`需为32的倍数、`float`/`int32_t`需为8的倍数、`half`/`int16_t`需为16的倍数，否则触发异常。
- m_size在`nz2dn_en`为true时需按dst dtype满足对齐约束（`half`/`int16_t`需为16的倍数，`int8_t`/`uint8_t`需为32的倍数，`float`/`int32_t`需为8的倍数），否则触发异常。
- `channel_split`为true时仅dst dtype为`float`且`quant_pre`取值与`float`输出匹配（不开启量化或`float`→`float`量化前处理模式）时合法，否则触发异常。
- `quant_pre`取值需与src dtype匹配，且不可取保留值（如6、7、17~20、27~30、35~63等），否则触发异常。
- `relu_pre`不可取4~7保留值，否则触发异常。
- `clip_relu_pre`不可取2/3保留值，否则触发异常。
- `quant_post`、`relu_post`、`clip_relu_post`、`eltwise_op`、`eltwise_antq_en`、`c0_pad_en`为保留参数，填0即可，传入非0值不生效。
- n_size传入>4095的值会截断处理（如4096→0、4097→1）。截断后实际生效值偏小，导致该维单次迭代搬运元素数不足，多出元素被丢弃且后续维度迭代起始地址错位。参数类型为`uint16_t`（0~2^16−1），截断后若值为0会同时触发n_size不可为0的异常。
- clip_relu_pre传入>3的值会截断处理。截断后实际生效值偏小，导致实际生效的预处理Clip ReLU模式与预期不符。参数类型为`uint8_t`（0~255），截断后若落在2/3仍会触发clip_relu_pre不可取保留值的异常。
- unit_flag_mode传入>3的值会截断处理。截断后实际生效值偏小，导致实际生效的unit-flag模式与预期不符。参数类型为`uint8_t`（0~255），截断后若值为1仍会触发unit_flag_mode保留值异常。
- quant_pre传入>63的值会截断处理。截断后实际生效值偏小，导致实际生效的预处理量化模式与预期不符，可能产生错误的量化结果。参数类型为`uint64_t`（0~2^64−1），截断后若落在保留值仍会触发quant_pre保留值异常。
- relu_pre传入>7的值会截断处理。截断后实际生效值偏小，导致实际生效的预处理ReLU模式与预期不符。参数类型为`uint8_t`（0~255），截断后若落在4~7仍会触发relu_pre保留值异常。
- quant_post传入>31的值会截断处理。参数类型为`uint64_t`（0~2^64−1），该参数为保留参数，截断后行为无意义。
- relu_post传入>7的值会截断处理。参数类型为`uint8_t`（0~255），该参数为保留参数，截断后行为无意义。
- eltwise_op传入>7的值会截断处理。参数类型为`uint8_t`（0~255），该参数为保留参数，截断后行为无意义。

### 通用搬运模式约束

- n_size不可为0，否则触发异常。
- m_size不可为0，否则触发异常。
- dst_stride即使循环次数为0也不可为0，否则触发异常；同时需按dst dtype满足32字节对齐（`float`/`int32_t`需为8的倍数）。
- n_size需按dst dtype与`nz2nd_en`状态满足对齐约束：`float`/`int32_t`需为8的倍数（非NZ2ND与NZ2ND模式均同），否则触发异常。
- m_size在`nz2dn_en`为true时需为8的倍数（dst dtype为`float`/`int32_t`），否则触发异常。
- `channel_split`为true时仅dst dtype为`float`且`quant_pre`取值与`float`输出匹配（不开启量化或`float`→`float`量化前处理模式）时合法，否则触发异常。
- `quant_pre`取值需与src dtype匹配，且不可取保留值（如6、7、17~20、27~30、35~63等），否则触发异常。
- `relu_pre`不可取4~7保留值，否则触发异常。
- `clip_relu_pre`不可取2/3保留值，否则触发异常。
- `nz2nd_en`与`nz2dn_en`互斥，同时使能触发异常。
- `quant_post`、`relu_post`、`clip_relu_post`、`eltwise_op`、`eltwise_antq_en`、`c0_pad_en`为保留参数，填0即可，传入非0值不生效。
- l2_cache_ctl支持0、1、2、4，未定义值会导致结果异常。其中取值1（LAST模式）暂不支持，取值2（驻留模式）尚在开发中暂不支持、计划于Ascend 950PR/Ascend 950DT上提供支持。
- n_size传入>4095的值会截断处理（如4096→0、4097→1）。截断后实际生效值偏小，导致该维单次迭代搬运元素数不足，多出元素被丢弃且后续维度迭代起始地址错位。参数类型为`uint16_t`（0~2^16−1），截断后若值为0会同时触发n_size不可为0的异常。
- clip_relu_pre传入>3的值会截断处理。截断后实际生效值偏小，导致实际生效的预处理Clip ReLU模式与预期不符。参数类型为`uint8_t`（0~255），截断后若落在2/3仍会触发clip_relu_pre不可取保留值的异常。
- unit_flag_mode传入>3的值会截断处理。截断后实际生效值偏小，导致实际生效的unit-flag模式与预期不符。参数类型为`uint8_t`（0~255），截断后若值为1仍会触发unit_flag_mode保留值异常。
- quant_pre传入>63的值会截断处理。截断后实际生效值偏小，导致实际生效的预处理量化模式与预期不符，可能产生错误的量化结果。参数类型为`uint64_t`（0~2^64−1），截断后若落在保留值仍会触发quant_pre保留值异常。
- relu_pre传入>7的值会截断处理。截断后实际生效值偏小，导致实际生效的预处理ReLU模式与预期不符。参数类型为`uint8_t`（0~255），截断后若落在4~7仍会触发relu_pre保留值异常。
- quant_post传入>31的值会截断处理。参数类型为`uint64_t`（0~2^64−1），该参数为保留参数，截断后行为无意义。
- relu_post传入>7的值会截断处理。参数类型为`uint8_t`（0~255），该参数为保留参数，截断后行为无意义。
- eltwise_op传入>7的值会截断处理。参数类型为`uint8_t`（0~255），该参数为保留参数，截断后行为无意义。

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
uint64_t quant_post = 0;  // 无效参数
uint8_t relu_post = 0;  // 无效参数
bool clip_relu_post = false;  // 无效参数
uint8_t eltwise_op = 0;  // 无效参数
uint8_t eltwise_antq_en = 0;  // 无效参数
bool c0_pad_en = false;  // 无效参数
asc_copy_l0c2l1(
    dst, src, n_size, m_size, dst_stride, src_stride, enable_clip_relu_pre, unit_flag_mode,
    quant_pre_mode, relu_pre_mode, enable_channel_split, enable_nz2nd, quant_post, relu_post,
    clip_relu_post, eltwise_op, eltwise_antq_en, c0_pad_en);
```
