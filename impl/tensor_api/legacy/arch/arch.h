/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef IMPL_TENSOR_API_LEGACY_ARCH_ARCH_H
#define IMPL_TENSOR_API_LEGACY_ARCH_ARCH_H

namespace asc {
namespace te {

template <QuantMode_t QuantMode>
using IsScalarQuantMode = is_scalar_quant_mode<QuantMode>;

template <QuantMode_t QuantMode>
using IsVectorQuantMode = is_vector_quant_mode<QuantMode>;

template <QuantMode_t QuantMode>
using IsDirectQuantMode = is_direct_quant_mode<QuantMode>;

using LayoutPatternCheckSet = layout_pattern_check_set;

template <typename TensorType>
inline constexpr bool IsL0COutSrcBatchLayoutV = is_l0c_out_src_batch_layout_v<TensorType>;

template <typename TensorType>
inline constexpr bool IsL0COutBatchedLayoutV = is_l0c_out_batched_layout_v<TensorType>;

template <typename TensorType>
inline constexpr bool IsL0COutNDFormatV = is_l0c_out_nd_format_v<TensorType>;

template <typename TensorType>
inline constexpr bool IsL0COutDNFormatV = is_l0c_out_dn_format_v<TensorType>;

template <typename TensorType>
inline constexpr bool IsL0COutNZFormatV = is_l0c_out_nz_format_v<TensorType>;

template <typename TensorType>
inline constexpr bool IsL0COutNC1HWC0V = is_l0c_out_nc1hwc0_v<TensorType>;

template <typename DstTensorType, typename SrcTensorType>
inline constexpr bool IsL0COutBatchNZ2NZV = is_l0c_out_batch_nz2nz_v<DstTensorType, SrcTensorType>;

template <typename Info1, typename Info2, size_t Dim, typename T>
__aicore__ inline constexpr decltype(auto) GetElement(const T& layout)
{
    return get_element<Info1, Info2, Dim>(layout);
}

template <typename Info1, typename Info2, typename T>
__aicore__ inline constexpr decltype(auto) GetElement(const T& layout)
{
    return get_element<Info1, Info2>(layout);
}

template <typename LayoutType>
__aicore__ inline static constexpr uint32_t GetTotalColumnShape(const LayoutType& layout)
{
    return get_total_column_shape(layout);
}

template <typename LayoutType>
__aicore__ inline static constexpr uint32_t GetTotalRowShape(const LayoutType& layout)
{
    return get_total_row_shape(layout);
}

template <typename LayoutType>
__aicore__ inline static constexpr decltype(auto) GetRowStride(const LayoutType& layout)
{
    return get_row_stride(layout);
}

template <typename LayoutType>
__aicore__ inline static constexpr decltype(auto) GetColumnStride(const LayoutType& layout)
{
    return get_column_stride(layout);
}

template <typename T>
__aicore__ inline void CheckOneLayoutPattern()
{
    check_one_layout_pattern<T>();
}

template <typename... Args>
__aicore__ inline void CheckLayoutPattern()
{
    check_layout_pattern<Args...>();
}

template <typename T, typename LayoutType>
__aicore__ inline static constexpr uint32_t GetL0COutNDStride(const LayoutType& layout)
{
    return get_l0c_out_nd_stride<T>(layout);
}

template <typename T, typename LayoutType>
__aicore__ inline static constexpr uint32_t GetL0COutDNStride(const LayoutType& layout)
{
    return get_l0c_out_dn_stride<T>(layout);
}

template <typename T, typename U, typename DstLayout, typename SrcLayout>
__aicore__ inline static constexpr l0c_out_copy_params MakeL0COutCopyParams(const DstLayout& dst_layout,
                                                                            const SrcLayout& src_layout)
{
    return make_l0c_out_copy_params<T, U>(dst_layout, src_layout);
}

template <round_mode Mode, typename DstType, typename SrcType>
__aicore__ inline constexpr QuantMode_t GetVectorQuantMode()
{
    return get_vector_quant_mode<Mode, DstType, SrcType>();
}

template <round_mode Mode, typename DstType, typename SrcType>
__aicore__ inline constexpr QuantMode_t GetScalarQuantMode()
{
    return get_scalar_quant_mode<Mode, DstType, SrcType>();
}

template <round_mode Mode, typename DstType, typename SrcType>
__aicore__ inline constexpr QuantMode_t GetCastQuantMode()
{
    return get_cast_quant_mode<Mode, DstType, SrcType>();
}

template <round_mode Mode, typename T, typename U, typename S = void>
__aicore__ inline constexpr QuantMode_t GetQuantMode()
{
    return get_quant_mode<Mode, T, U, S>();
}

template <typename T, typename U, typename V>
__aicore__ inline static constexpr bool CheckVectorQuantBatchConsistency()
{
    return check_vector_quant_batch_consistency<T, U, V>();
}

__aicore__ inline auto AllocFbTempBuf(const uint16_t& calculate_n_size)
{
    return alloc_fb_temp_buf(calculate_n_size);
}

template <typename T>
__aicore__ inline void SetFpc(const __fbuf__ T* deq_tensor_temp_buf)
{
    set_fpc(deq_tensor_temp_buf);
}

__aicore__ inline void InsertSync()
{
    insert_sync();
}

template <typename T, typename U>
__aicore__ inline static void EmitSetRegister(const U& src_layout, uint32_t batch_num, uint32_t dst_batch_stride,
                                              uint32_t src_batch_stride)
{
    emit_set_register<T>(src_layout, batch_num, dst_batch_stride, src_batch_stride);
}

template <typename T, typename U>
__aicore__ inline static void EmitSetRegister(const U& src_layout, uint64_t quant, uint32_t batch_num,
                                              uint32_t dst_batch_stride, uint32_t src_batch_stride)
{
    emit_set_register<T>(src_layout, quant, batch_num, dst_batch_stride, src_batch_stride);
}

template <typename T, typename U>
__aicore__ inline static void SetRegisterImpl(const T& dst, const U& src)
{
    set_register_impl(dst, src);
}

template <typename T, typename U>
__aicore__ inline static void SetRegisterImpl(const T& dst, const U& src, uint64_t quant)
{
    set_register_impl(dst, src, quant);
}

template <const copy_gm_to_l1_trait& Trait, typename CopyOp, typename T, typename U>
__aicore__ inline void RunGmToL1Batched(const T& dst, const U& src)
{
    run_gm_to_l1_batched<Trait, CopyOp>(dst, src);
}

template <typename T>
__aicore__ inline void SetMTE2NzPara(const T& parameter)
{
    set_mte2_nz_para(parameter);
}

__aicore__ inline static void SetCtrlForhifloat8()
{
    set_ctrl_for_hifloat8();
}

__aicore__ inline static void ResetCtrlForhifloat8()
{
    reset_ctrl_for_hifloat8();
}

} // namespace te
} // namespace asc

#endif // IMPL_TENSOR_API_LEGACY_ARCH_ARCH_H
