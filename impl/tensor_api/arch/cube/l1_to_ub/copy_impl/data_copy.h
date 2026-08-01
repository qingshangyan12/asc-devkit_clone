/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#if !defined(ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#warning                                                                                                               \
    "impl/tensor_api/arch/cube/l1_to_ub/copy_impl/data_copy.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future. Please use "#include "tensor_api/tensor.h"" and use public functions or variables defined in interface headers files."
#define ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif

/*!
 * \file data_copy.h
 * \brief
 */
#ifndef IMPL_TENSOR_API_ARCH_CUBE_L1_TO_UB_COPY_IMPL_DATA_COPY_H
#define IMPL_TENSOR_API_ARCH_CUBE_L1_TO_UB_COPY_IMPL_DATA_COPY_H

#include "impl/tensor_api/utils/utils_impl.h"
#include "impl/tensor_api/arch/cube/l1_to_ub/copy_impl/instruction.h"

namespace asc {
namespace te {

class copy_l1_to_ubnd {
public:
    template <const copy_l1_to_ub_trait& trait, typename T, typename U>
    __aicore__ inline static void run(const T& dst, const U& src)
    {
        execute<trait>(dst, src);
    }

private:
    template <const copy_l1_to_ub_trait& trait, typename T, typename U>
    __aicore__ inline static void execute(const T& dst, const U& src)
    {
        using srcType = typename U::elementType;
        using dstType = typename T::elementType;

        auto dst_layout = dst.layout();
        auto src_layout = src.layout();

        uint16_t block_count = get_total_row_shape(src_layout);
        uint32_t block_len = Std::ceil_division(get_total_column_shape(src_layout), C0_ELEMENT<srcType>);

        uint32_t src_row_stride;
        uint32_t dst_row_stride;
        if constexpr (is_satisfied_ptn_format_v<U, NDLayoutPtn>) {
            src_row_stride = get_element<AttrInfo::Stride, AttrInfo::Row>(src_layout);
        } else {
            src_row_stride = get_element<AttrInfo::Stride, AttrInfo::Row, 1>(src_layout);
        }
        if constexpr (is_satisfied_ptn_format_v<T, NDLayoutPtn>) {
            dst_row_stride = get_element<AttrInfo::Stride, AttrInfo::Row>(dst_layout);
        } else {
            dst_row_stride = get_element<AttrInfo::Stride, AttrInfo::Row, 1>(dst_layout);
        }
        int64_t src_stride =
            Std::ceil_division(src_row_stride - get_total_column_shape(src_layout), C0_ELEMENT<srcType>);
        int64_t dst_stride =
            Std::ceil_division(dst_row_stride - get_total_column_shape(src_layout), C0_ELEMENT<dstType>);

        copy_cbuf_to_ubuf_instr::data_copy(dst, src, block_count, block_len, src_stride, dst_stride);
    }
};

class copy_l1_to_ubdn {
public:
    template <const copy_l1_to_ub_trait& trait, typename T, typename U>
    __aicore__ inline static void run(const T& dst, const U& src)
    {
        execute<trait>(dst, src);
    }

private:
    template <const copy_l1_to_ub_trait& trait, typename T, typename U>
    __aicore__ inline static void execute(const T& dst, const U& src)
    {
        using srcType = typename U::elementType;
        using dstType = typename T::elementType;

        auto dst_layout = dst.layout();
        auto src_layout = src.layout();

        uint16_t block_count = get_total_column_shape(src_layout);
        uint32_t block_len = Std::ceil_division(get_total_row_shape(src_layout), C0_ELEMENT<srcType>);

        uint32_t src_column_stride;
        uint32_t dst_column_stride;
        if constexpr (is_satisfied_ptn_format_v<U, DNLayoutPtn>) {
            src_column_stride = get_element<AttrInfo::Stride, AttrInfo::Column>(src_layout);
        } else {
            src_column_stride = get_element<AttrInfo::Stride, AttrInfo::Column, 1>(src_layout);
        }
        if constexpr (is_satisfied_ptn_format_v<T, DNLayoutPtn>) {
            dst_column_stride = get_element<AttrInfo::Stride, AttrInfo::Column>(dst_layout);
        } else {
            dst_column_stride = get_element<AttrInfo::Stride, AttrInfo::Column, 1>(dst_layout);
        }
        int64_t src_stride =
            Std::ceil_division(src_column_stride - get_total_row_shape(src_layout), C0_ELEMENT<srcType>);
        int64_t dst_stride =
            Std::ceil_division(dst_column_stride - get_total_row_shape(src_layout), C0_ELEMENT<dstType>);

        copy_cbuf_to_ubuf_instr::data_copy(dst, src, block_count, block_len, src_stride, dst_stride);
    }
};

class copy_l1_to_ubnz {
public:
    template <const copy_l1_to_ub_trait& trait, typename T, typename U>
    __aicore__ inline static void run(const T& dst, const U& src)
    {
        execute<trait>(dst, src);
    }

private:
    template <const copy_l1_to_ub_trait& trait, typename T, typename U>
    __aicore__ inline static void execute(const T& dst, const U& src)
    {
        using srcType = typename U::elementType;
        using dstType = typename T::elementType;

        auto dst_layout = dst.layout();
        auto src_layout = src.layout();

        uint16_t block_count = get_element<AttrInfo::Shape, AttrInfo::Column, 1>(src_layout);
        uint32_t block_len = get_total_row_shape(src_layout);
        int64_t src_stride =
            get_element<AttrInfo::Stride, AttrInfo::Column, 1>(src_layout) / C0_ELEMENT<srcType> - block_len;
        int64_t dst_stride =
            get_element<AttrInfo::Stride, AttrInfo::Column, 1>(dst_layout) / C0_ELEMENT<dstType> - block_len;

        copy_cbuf_to_ubuf_instr::data_copy(dst, src, block_count, block_len, src_stride, dst_stride);
    }
};

} // namespace te
} // namespace asc

#endif // IMPL_TENSOR_API_ARCH_CUBE_L1_TO_UB_COPY_IMPL_DATA_COPY_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif
