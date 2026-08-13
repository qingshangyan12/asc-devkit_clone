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
#define ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_TENSOR_API_H
#endif

/*!
 * \file copy_op.h
 * \brief
 */
#ifndef INCLUDE_TENSOR_API_ARCH_CUBE_COPY_OP_H
#define INCLUDE_TENSOR_API_ARCH_CUBE_COPY_OP_H

#include "tensor_api/utils/utils.h"

namespace asc {
namespace te {

struct copy_gm_to_l1_trait {};
struct copy_l1_to_ub_trait {};
struct copy_l1_to_l0a_trait {};
struct copy_l1_to_l0b_trait {};
struct copy_l1_to_l0scalea_trait {};
struct copy_l1_to_l0scaleb_trait {};
struct copy_l1_to_fixbuf_trait {};
struct copy_l1_to_biastable_trait {};

struct copy_l0c_to_gm_trait {
    __aicore__ constexpr copy_l0c_to_gm_trait() = default;
    __aicore__ constexpr copy_l0c_to_gm_trait(asc::te::round_mode round_mode, bool enable_relu,
                                              bool enable_channel_split);

    asc::te::round_mode round_mode = asc::te::round_mode::default_round;
    bool enable_relu = false;
    bool enable_channel_split = false;
};

struct copy_l0c_to_ub_trait {
    __aicore__ constexpr copy_l0c_to_ub_trait() = default;
    __aicore__ constexpr copy_l0c_to_ub_trait(asc::te::round_mode round_mode, bool enable_relu,
                                              bool enable_channel_split, dual_dst_mode dual_dst_ctl);

    asc::te::round_mode round_mode = asc::te::round_mode::default_round;
    bool enable_relu = false;
    bool enable_channel_split = false;
    dual_dst_mode dual_dst_ctl = dual_dst_mode::disable;
};

struct copy_l0c_to_l1_trait {
    __aicore__ constexpr copy_l0c_to_l1_trait() = default;
    __aicore__ constexpr copy_l0c_to_l1_trait(asc::te::round_mode round_mode, bool enable_relu,
                                              bool enable_channel_split);

    asc::te::round_mode round_mode = asc::te::round_mode::default_round;
    bool enable_relu = false;
    bool enable_channel_split = false;
};

struct copy_gm_to_l1_trait_default {
    using trait_type = copy_gm_to_l1_trait;
    static constexpr const trait_type value = {};
};

struct copy_l1_to_ub_trait_default {
    using trait_type = copy_l1_to_ub_trait;
    static constexpr const trait_type value = {};
};

struct copy_l1_to_l0a_trait_default {
    using trait_type = copy_l1_to_l0a_trait;
    static constexpr const trait_type value = {};
};

struct copy_l1_to_l0b_trait_default {
    using trait_type = copy_l1_to_l0b_trait;
    static constexpr const trait_type value = {};
};

struct copy_l1_to_l0scalea_trait_default {
    using trait_type = copy_l1_to_l0scalea_trait;
    static constexpr const trait_type value = {};
};

struct copy_l1_to_l0scaleb_trait_default {
    using trait_type = copy_l1_to_l0scaleb_trait;
    static constexpr const trait_type value = {};
};

struct copy_l1_to_fixbuf_trait_default {
    using trait_type = copy_l1_to_fixbuf_trait;
    static constexpr const trait_type value = {};
};

struct copy_l1_to_biastable_trait_default {
    using trait_type = copy_l1_to_biastable_trait;
    static constexpr const trait_type value = {};
};

struct copy_l0c_to_gm_trait_default {
    using trait_type = copy_l0c_to_gm_trait;
    static constexpr const trait_type value = {};
};

struct copy_l0c_to_ub_trait_default {
    using trait_type = copy_l0c_to_ub_trait;
    static constexpr const trait_type value = {};
};

struct copy_l0c_to_l1_trait_default {
    using trait_type = copy_l0c_to_l1_trait;
    static constexpr const trait_type value = {};
};

struct copy_gm_to_l1 {
    template <typename TraitType, const TraitType& trait, typename... Args>
    __aicore__ inline static void copy(const Args&... args);

private:
    template <const copy_gm_to_l1_trait& trait, typename DstTensor, typename SrcTensor>
    __aicore__ inline static void data_copy_impl(const DstTensor& dst, const SrcTensor& src);
};

struct copy_l1_to_ub {
    template <typename TraitType, const TraitType& trait, typename... Args>
    __aicore__ inline static void copy(const Args&... args);

private:
    template <const copy_l1_to_ub_trait& trait, typename DstTensor, typename SrcTensor>
    __aicore__ inline static void data_copy_impl(const DstTensor& dst, const SrcTensor& src);
};

struct copy_l1_to_l0a {
    template <typename TraitType, const TraitType& trait, typename... Args>
    __aicore__ inline static void copy(const Args&... args);
};

struct copy_l1_to_l0b {
    template <typename TraitType, const TraitType& trait, typename... Args>
    __aicore__ inline static void copy(const Args&... args);

private:
    template <const copy_l1_to_l0b_trait& trait, typename DstTensor, typename SrcTensor>
    __aicore__ inline static void load_data(const DstTensor& dst, const SrcTensor& src);
};

struct copy_l1_to_l0scalea {
    template <typename TraitType, const TraitType& trait, typename... Args>
    __aicore__ inline static void copy(const Args&... args);

private:
    template <const copy_l1_to_l0scalea_trait& trait, typename DstTensor, typename SrcTensor>
    __aicore__ inline static void load_data(const DstTensor& dst, const SrcTensor& src);
};

struct copy_l1_to_l0scaleb {
    template <typename TraitType, const TraitType& trait, typename... Args>
    __aicore__ inline static void copy(const Args&... args);

private:
    template <const copy_l1_to_l0scaleb_trait& trait, typename DstTensor, typename SrcTensor>
    __aicore__ inline static void load_data(const DstTensor& dst, const SrcTensor& src);
};

struct copy_l1_to_fixbuf {
    template <typename TraitType, const TraitType& trait, typename... Args>
    __aicore__ inline static void copy(const Args&... args);

private:
    template <const copy_l1_to_fixbuf_trait& trait, typename DstTensor, typename SrcTensor>
    __aicore__ inline static void data_copy_impl(const DstTensor& dst, const SrcTensor& src);
};

struct copy_l1_to_biastable {
    template <typename TraitType, const TraitType& trait, typename... Args>
    __aicore__ inline static void copy(const Args&... args);

private:
    template <const copy_l1_to_biastable_trait& trait, typename DstTensor, typename SrcTensor>
    __aicore__ inline static void data_copy_impl(const DstTensor& dst, const SrcTensor& src);
};

struct copy_l0c_to_gm {
    template <typename TraitType, const TraitType& trait, typename... Args>
    __aicore__ inline static void copy(const Args&... args);
};

struct copy_l0c_to_ub {
    template <typename TraitType, const TraitType& trait, typename... Args>
    __aicore__ inline static void copy(const Args&... args);
};

struct copy_l0c_to_l1 {
    template <typename TraitType, const TraitType& trait, typename... Args>
    __aicore__ inline static void copy(const Args&... args);
};

} // namespace te
} // namespace asc

#include "impl/tensor_api/arch/cube/gm_to_l1/copy.h"
#include "impl/tensor_api/arch/cube/l1_to_ub/copy.h"
#include "impl/tensor_api/arch/cube/l1_to_l0a/copy.h"
#include "impl/tensor_api/arch/cube/l1_to_l0b/copy.h"
#include "impl/tensor_api/arch/cube/l1_to_l0scalea/copy.h"
#include "impl/tensor_api/arch/cube/l1_to_l0scaleb/copy.h"
#include "impl/tensor_api/arch/cube/l1_to_fb/copy.h"
#include "impl/tensor_api/arch/cube/l1_to_bt/copy.h"
#include "impl/tensor_api/arch/cube/l0c_to_gm/copy.h"
#include "impl/tensor_api/arch/cube/l0c_to_ub/copy.h"
#include "impl/tensor_api/arch/cube/l0c_to_l1/copy.h"

#endif // INCLUDE_TENSOR_API_ARCH_CUBE_COPY_OP_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_TENSOR_API_H)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_TENSOR_API_H
#endif
