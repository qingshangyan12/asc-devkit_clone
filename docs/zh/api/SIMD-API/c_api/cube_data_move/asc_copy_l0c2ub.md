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

该接口用于将矩阵计算的结果从L0C Buffer搬运至Unified Buffer（UB）中，并且在搬运过程中支持与其它接口组合使用，配置多种随路能力。

下图展示了随路量化、随路ReLU、随路格式转换、随路通道拆分以及随路通道合并的有效组合、中间数据类型和数据路径。下图中的F32-\>F16与F32-\>BF16为Cast，其余为随路scalar/tensor量化模式。

**图1** asc_copy_l0c2ub随路功能组合

![](../../../figures/C_API_L0C2UB_Function_Combination_950.png)

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
- QF322F32_PRE：float量化成float，scalar量化，精度可以达到双千分之一，无法达到双万分之一。如果有双万分之一的精度要求，建议使用[AscendDeQuant](../../adv_api/quantization/AscendDequant.md)高阶API。
- VQF322F32_PRE：float量化成float，tensor量化，精度可以达到双千分之一，无法达到双万分之一。如果有双万分之一的精度要求，建议使用[AscendDeQuant](../../adv_api/quantization/AscendDequant.md)高阶API。

## 函数原型

- 常规搬运

    ```cpp
    __aicore__ inline void asc_copy_l0c2ub(
        __ubuf__ bfloat16_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
        uint16_t src_stride, uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t enable_clip_relu_pre,
        uint8_t unit_flag_mode, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split,
        bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op,
        bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en, bool enable_nz2dn)
    __aicore__ inline void asc_copy_l0c2ub(
        __ubuf__ half* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
        uint16_t src_stride, uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t enable_clip_relu_pre,
        uint8_t unit_flag_mode, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split,
        bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op,
        bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en, bool enable_nz2dn)
    __aicore__ inline void asc_copy_l0c2ub(
        __ubuf__ fp8_e4m3fn_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
        uint16_t src_stride, uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t enable_clip_relu_pre,
        uint8_t unit_flag_mode, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split,
        bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op,
        bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en, bool enable_nz2dn)
    __aicore__ inline void asc_copy_l0c2ub(
        __ubuf__ hifloat8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
        uint16_t src_stride, uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t enable_clip_relu_pre,
        uint8_t unit_flag_mode, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split,
        bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op,
        bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en, bool enable_nz2dn)
    __aicore__ inline void asc_copy_l0c2ub(
        __ubuf__ int8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
        uint16_t src_stride, uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t enable_clip_relu_pre,
        uint8_t unit_flag_mode, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split,
        bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op,
        bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en, bool enable_nz2dn)
    __aicore__ inline void asc_copy_l0c2ub(
        __ubuf__ uint8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
        uint16_t src_stride, uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t enable_clip_relu_pre,
        uint8_t unit_flag_mode, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split,
        bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op,
        bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en, bool enable_nz2dn)
    __aicore__ inline void asc_copy_l0c2ub(
        __ubuf__ float* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
        uint16_t src_stride, uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t enable_clip_relu_pre,
        uint8_t unit_flag_mode, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split,
        bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op,
        bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en, bool enable_nz2dn)
    __aicore__ inline void asc_copy_l0c2ub(
        __ubuf__ bfloat16_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
        uint16_t src_stride, uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t enable_clip_relu_pre,
        uint8_t unit_flag_mode, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split,
        bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op,
        bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en, bool enable_nz2dn)
    __aicore__ inline void asc_copy_l0c2ub(
        __ubuf__ half* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
        uint16_t src_stride, uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t enable_clip_relu_pre,
        uint8_t unit_flag_mode, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split,
        bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op,
        bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en, bool enable_nz2dn)
    __aicore__ inline void asc_copy_l0c2ub(
        __ubuf__ int8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
        uint16_t src_stride, uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t enable_clip_relu_pre,
        uint8_t unit_flag_mode, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split,
        bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op,
        bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en, bool enable_nz2dn)
    __aicore__ inline void asc_copy_l0c2ub(
        __ubuf__ uint8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
        uint16_t src_stride, uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t enable_clip_relu_pre,
        uint8_t unit_flag_mode, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split,
        bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op,
        bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en, bool enable_nz2dn)
    __aicore__ inline void asc_copy_l0c2ub(
        __ubuf__ int32_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
        uint16_t src_stride, uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t enable_clip_relu_pre,
        uint8_t unit_flag_mode, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split,
        bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op,
        bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en, bool enable_nz2dn)
    __aicore__ inline void asc_copy_l0c2ub(
        __ubuf__ int4b_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
        uint16_t src_stride, uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t enable_clip_relu_pre,
        uint8_t unit_flag_mode, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split,
        bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op,
        bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en, bool enable_nz2dn)
    __aicore__ inline void asc_copy_l0c2ub(
        __ubuf__ int4b_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
        uint16_t src_stride, uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t enable_clip_relu_pre,
        uint8_t unit_flag_mode, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split,
        bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op,
        bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en, bool enable_nz2dn)
    ```

- 同步搬运

    ```cpp
    __aicore__ inline void asc_copy_l0c2ub_sync(
        __ubuf__ bfloat16_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
        uint16_t src_stride, uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t enable_clip_relu_pre,
        uint8_t unit_flag_mode, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split,
        bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op,
        bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en, bool enable_nz2dn)
    __aicore__ inline void asc_copy_l0c2ub_sync(
        __ubuf__ half* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
        uint16_t src_stride, uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t enable_clip_relu_pre,
        uint8_t unit_flag_mode, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split,
        bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op,
        bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en, bool enable_nz2dn)
    __aicore__ inline void asc_copy_l0c2ub_sync(
        __ubuf__ fp8_e4m3fn_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
        uint16_t src_stride, uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t enable_clip_relu_pre,
        uint8_t unit_flag_mode, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split,
        bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op,
        bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en, bool enable_nz2dn)
    __aicore__ inline void asc_copy_l0c2ub_sync(
        __ubuf__ hifloat8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
        uint16_t src_stride, uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t enable_clip_relu_pre,
        uint8_t unit_flag_mode, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split,
        bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op,
        bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en, bool enable_nz2dn)
    __aicore__ inline void asc_copy_l0c2ub_sync(
        __ubuf__ int8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
        uint16_t src_stride, uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t enable_clip_relu_pre,
        uint8_t unit_flag_mode, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split,
        bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op,
        bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en, bool enable_nz2dn)
    __aicore__ inline void asc_copy_l0c2ub_sync(
        __ubuf__ uint8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
        uint16_t src_stride, uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t enable_clip_relu_pre,
        uint8_t unit_flag_mode, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split,
        bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op,
        bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en, bool enable_nz2dn)
    __aicore__ inline void asc_copy_l0c2ub_sync(
        __ubuf__ float* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
        uint16_t src_stride, uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t enable_clip_relu_pre,
        uint8_t unit_flag_mode, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split,
        bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op,
        bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en, bool enable_nz2dn)
    __aicore__ inline void asc_copy_l0c2ub_sync(
        __ubuf__ bfloat16_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
        uint16_t src_stride, uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t enable_clip_relu_pre,
        uint8_t unit_flag_mode, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split,
        bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op,
        bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en, bool enable_nz2dn)
    __aicore__ inline void asc_copy_l0c2ub_sync(
        __ubuf__ half* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
        uint16_t src_stride, uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t enable_clip_relu_pre,
        uint8_t unit_flag_mode, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split,
        bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op,
        bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en, bool enable_nz2dn)
    __aicore__ inline void asc_copy_l0c2ub_sync(
        __ubuf__ int8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
        uint16_t src_stride, uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t enable_clip_relu_pre,
        uint8_t unit_flag_mode, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split,
        bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op,
        bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en, bool enable_nz2dn)
    __aicore__ inline void asc_copy_l0c2ub_sync(
        __ubuf__ uint8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
        uint16_t src_stride, uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t enable_clip_relu_pre,
        uint8_t unit_flag_mode, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split,
        bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op,
        bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en, bool enable_nz2dn)
    __aicore__ inline void asc_copy_l0c2ub_sync(
        __ubuf__ int32_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
        uint16_t src_stride, uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t enable_clip_relu_pre,
        uint8_t unit_flag_mode, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split,
        bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op,
        bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en, bool enable_nz2dn)
    __aicore__ inline void asc_copy_l0c2ub_sync(
        __ubuf__ int4b_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
        uint16_t src_stride, uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t enable_clip_relu_pre,
        uint8_t unit_flag_mode, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split,
        bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op,
        bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en, bool enable_nz2dn)
    __aicore__ inline void asc_copy_l0c2ub_sync(
        __ubuf__ int4b_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
        uint16_t src_stride, uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t enable_clip_relu_pre,
        uint8_t unit_flag_mode, uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split,
        bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op,
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
| dual_dst_ctrl | 输入 | 双目标模式控制参数。当同一AI Core内有一个Cube Core和两个Vector Core时，如果启用双目标模式控制，L0C Buffer中的M * N矩阵将被分成两半，并同时分别写入两个Vector Core各自的UB中，其中前半部分写入SUB BLOCK0，后半部分写入SUB BLOCK1，参数取值说明如下：<br>&nbsp;&nbsp;&bull; 0：单目标模式，将整个矩阵写入通过subBlockId参数配置的目标UB。<br>&nbsp;&nbsp;&bull; 1：双目标模式，按M维度拆分成形状为M / 2 \* N的两个矩阵，分别写入两个UB, M必须为2的倍数。<br>&nbsp;&nbsp;&bull; 2：双目标模式，按N维度拆分成形状为M \* N / 2的两个矩阵，分别写入两个UB, N须为32的倍数。<br>**dualDstCtrl仅支持在普通搬运模式（NZ2NZ）或NZ2ND搬运场景下使用，不支持其他随路功能场景。** |
| sub_blockid | 输入 | 启用单目标模式时用于指示目标UB的SUB BLOCK ID。取值为0或1，取值为0时写入SUB BLOCK0，为1时写入SUB BLOCK1。 |
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
| eltwise_antq_en | 输入 | 无效参数，用户无需关注，传入false即可。 |
| c0_pad_en | 输入 | 无效参数，用户无需关注，传入false即可。 |
| broadcast_en | 输入 | 无效参数，用户无需关注，传入false即可。 |
| enable_nz2dn | 输入 | 是否开启NZ2DN功能。<br>&nbsp;&nbsp;&bull; false：不开启；<br>&nbsp;&nbsp;&bull; true：开启。 |

## 返回值说明

无

## 流水类型

PIPE_FIX

## 约束说明

- 各存储单元的空间大小和对齐要求请参考[存储单元说明](../general_description_and_constraints.md#存储单元说明)。

### 通用约束

- 本接口仅在AIC上生效，非AIC调用直接返回。
- dst起始地址需32字节对齐（UB对齐要求），否则触发异常。
- src起始地址需64字节对齐（L0C Buffer对齐要求，`float`/`int32_t`源），否则触发异常。
- 如果需要执行多条本接口指令，且目的地址存在重叠，需要插入同步指令（`asc_sync_notify`和`asc_sync_wait`），保证多个指令串行化，防止出现异常数据。
- **UB容量上限**：UB总容量为256KB，默认预留6KB SIMD VF栈与2KB Ascend C预留空间后可用248KB；SIMD+SIMT混编时再划分32KB~128KB作Data Cache，可用容量进一步减少。dst偏移+搬运大小不可超过实际可用容量，否则触发写溢出异常。
- **L0C Buffer容量上限**：L0C Buffer总容量256KB，src偏移+搬运大小不可超过L0C Buffer容量，否则触发读溢出异常。
- 量化与激活模式中的M1/M2/M3/M4 scale参数不可为INF/NaN或非规格化数，否则结果错误。
- dtype仅支持源为`float`/`int32_t`（L0C Buffer端f16源在本芯片的Cube单元中不支持），dst dtype由重载决定，src与dst dtype组合需与`quant_pre`量化模式匹配，否则触发异常。

### 随路量化激活搬运模式约束

- `n_size`、`m_size`、`loop_dst_stride`需满足按dtype与功能模式（NZ2ND/NZ2DN/split_en等）的对齐约束（详见参数说明），违反约束触发异常。
- `loop_dst_stride`不可为0（即使对应loop size为0），否则触发异常。
- `clip_relu_pre`取值需为已定义模式（0/1之一），取值2/3触发异常。
- `unit_flag_ctl`取值1为保留值，触发异常；取值需为0/2/3之一。
- `quant_pre`取值需为已定义的合法量化模式，非法取值（含6、7、[17, 20]、[27, 30]、[35, 63] 等）触发异常。
- `relu_pre`取值需为已定义模式（0/1/2/3/4之一），取值5~7触发异常。
- `split_en`仅在dst dtype为`float`且`quant_pre`取值为无转换（0）或`float`→`float`量化模式（14/15）时合法，dst dtype非`float`或`quant_pre`取其他值时开启触发异常。
- `nz2nd_en`与`nz2dn_en`互斥，不可同时开启。
- `nz2nd_en`或`nz2dn_en`开启时，须先通过配套setter接口预先配置loop3 size（大于0）与loop3 stride等循环参数，再调用本接口，否则触发异常。
- `nz2dn_en`开启时loop0 src stride不可为0，否则触发异常。
- `nz2dn_en`开启且loop0 src stride不为1时，`unit_flag_ctl`须为0（关闭unit-flag）。
- `quant_post`、`relu_post`、`clip_relu_post`、`eltwise_op`、`eltwise_antq_en`、`c0_pad_en`、`dual_dst_ctl`、`sub_blockid`为保留参数，须填0/false，传入非0值硬件行为未定义。
- `n_size`传入>4095的值会截断处理（如4096→0、4097→1）。截断后实际生效值偏小，导致该维单次迭代搬运元素数不足，多出元素被丢弃且后续维度迭代起始地址错位。参数类型为`uint16_t`（0~2^16−1），截断后若值不满足按dtype的对齐约束仍会触发对齐异常。
- `clip_relu_pre`传入>3的值会截断处理。截断后实际生效值偏小，导致实际生效的Clip ReLU模式与预期不符，前处理激活行为错误。参数类型为`uint8_t`（0~255），截断后若值为2/3仍会触发保留值非法配置异常。
- `unit_flag_ctl`传入>3的值会截断处理。截断后实际生效值偏小，导致实际生效的同步机制与预期不符。参数类型为`uint8_t`（0~255），截断后若值为1仍会触发保留值非法配置异常。
- `quant_pre`传入>63的值会截断处理。截断后实际生效值偏小，导致实际生效的量化模式与预期不符，转换后的数据与预期不一致。参数类型为`uint64_t`（0~2^64−1），截断后若值落在非法取值集合仍会触发异常。
- `relu_pre`传入>7的值会截断处理。截断后实际生效值偏小，导致实际生效的ReLU模式与预期不符，前处理激活行为错误。参数类型为`uint8_t`（0~255），截断后若值为5~7仍会触发保留值非法配置异常。
- `dual_dst_ctl`传入>3的值会截断处理。该参数为保留参数，须填0。参数类型为`uint8_t`（0~255），
- `quant_post`传入>31的值会截断处理。该参数为保留参数，须填0。参数类型为`uint64_t`（0~2^64−1）。
- `relu_post`传入>7的值会截断处理。该参数为保留参数，须填0。参数类型为`uint8_t`（0~255），
- `eltwise_op`传入>7的值会截断处理。该参数为保留参数，须填0。参数类型为`uint8_t`（0~255），
- 开启`quant_pre`、`relu_pre`（scalar/vector模式）、`clip_relu_pre`等随路能力时，须先调用配套setter接口预先配置量化参数、ReLU alpha、Clip ReLU最大值等，再调用本接口，否则硬件读取未配置的参数导致结果错误。

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
