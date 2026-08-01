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
    "impl/tensor_api/arch/cube/gm_to_l1/routing.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future. Please use "#include "tensor_api/tensor.h"" and use public functions or variables defined in interface headers files."
#define ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif

/*!
 * \file routing.h
 * \brief
 */
#ifndef IMPL_TENSOR_API_ARCH_CUBE_GM_TO_L1_ROUTING_H
#define IMPL_TENSOR_API_ARCH_CUBE_GM_TO_L1_ROUTING_H

#include "impl/tensor_api/arch/cube/gm_to_l1/copy_impl/dn2nz.h"
#include "impl/tensor_api/arch/cube/gm_to_l1/copy_impl/dn2zn.h"
#include "impl/tensor_api/arch/cube/gm_to_l1/copy_impl/nd2nd.h"
#include "impl/tensor_api/arch/cube/gm_to_l1/copy_impl/nd2nz.h"
#include "impl/tensor_api/arch/cube/gm_to_l1/copy_impl/nd2zn.h"
#include "impl/tensor_api/arch/cube/gm_to_l1/copy_impl/nc1hwc02nc1hwc0.h"
#include "impl/tensor_api/arch/cube/gm_to_l1/copy_impl/nchw2nc1hwc0.h"
#include "impl/tensor_api/arch/cube/gm_to_l1/copy_impl/ncdhw2ndc1hwc0.h"
#include "impl/tensor_api/arch/cube/gm_to_l1/copy_impl/nhwc2nc1hwc0.h"
#include "impl/tensor_api/arch/cube/gm_to_l1/copy_impl/nz2nz.h"
#include "impl/tensor_api/arch/cube/gm_to_l1/copy_impl/zn2zn.h"
#include "impl/tensor_api/arch/cube/gm_to_l1/copy_impl/scalea_nd2zz.h"
#include "impl/tensor_api/arch/cube/gm_to_l1/copy_impl/scalea_dn2zz.h"
#include "impl/tensor_api/arch/cube/gm_to_l1/copy_impl/scalea_zz2zz.h"
#include "impl/tensor_api/arch/cube/gm_to_l1/copy_impl/scaleb_nd2nn.h"
#include "impl/tensor_api/arch/cube/gm_to_l1/copy_impl/scaleb_dn2nn.h"
#include "impl/tensor_api/arch/cube/gm_to_l1/copy_impl/scaleb_nn2nn.h"

namespace asc {
namespace te {

class copy_gm_to_l1_ignore {
public:
    template <const copy_gm_to_l1_trait& trait, typename T, typename U>
    __aicore__ inline static void run(const T& dst, const U& src)
    {
        static_assert(!Std::is_same_v<T, T>, "copy_gm_to_l1: unsupported layout pattern combination.");
    }
};

template <uint32_t Version, typename DstLayoutPtn, typename SrcLayoutPtn>
struct copy_gm_to_l1_routing {
    using type = copy_gm_to_l1_ignore;
};

template <uint32_t Version>
struct copy_gm_to_l1_routing<Version, NDExtLayoutPtn, NDExtLayoutPtn> {
    using type = copy_gm_to_cbuf_align_v2_nd;
};

template <uint32_t Version>
struct copy_gm_to_l1_routing<Version, NDLayoutPtn, NDLayoutPtn> {
    using type = copy_gm_to_cbuf_align_v2_nd;
};

template <uint32_t Version>
struct copy_gm_to_l1_routing<Version, NZLayoutPtn, NDExtLayoutPtn> {
    using type = copy_gm_to_cbuf_multi_nd_to_nz;
};

template <uint32_t Version>
struct copy_gm_to_l1_routing<Version, NZLayoutPtn, NDLayoutPtn> {
    using type = copy_gm_to_cbuf_multi_nd_to_nz;
};

template <uint32_t Version>
struct copy_gm_to_l1_routing<Version, ZNLayoutPtn, NDExtLayoutPtn> {
    using type = copy_gm_to_cbuf_multi_nd_to_zn;
};

template <uint32_t Version>
struct copy_gm_to_l1_routing<Version, ZNLayoutPtn, NDLayoutPtn> {
    using type = copy_gm_to_cbuf_multi_nd_to_zn;
};

template <uint32_t Version>
struct copy_gm_to_l1_routing<Version, NZLayoutPtn, DNExtLayoutPtn> {
    using type = copy_gm_to_cbuf_multi_dn_to_nz;
};

template <uint32_t Version>
struct copy_gm_to_l1_routing<Version, NZLayoutPtn, DNLayoutPtn> {
    using type = copy_gm_to_cbuf_multi_dn_to_nz;
};

template <uint32_t Version>
struct copy_gm_to_l1_routing<Version, ZNLayoutPtn, DNExtLayoutPtn> {
    using type = copy_gm_to_cbuf_multi_dn_to_zn;
};

template <uint32_t Version>
struct copy_gm_to_l1_routing<Version, ZNLayoutPtn, DNLayoutPtn> {
    using type = copy_gm_to_cbuf_multi_dn_to_zn;
};

template <uint32_t Version>
struct copy_gm_to_l1_routing<Version, NZLayoutPtn, NZLayoutPtn> {
    using type = copy_gm_to_cbuf_align_v2_nz;
};

template <uint32_t Version>
struct copy_gm_to_l1_routing<Version, ZNLayoutPtn, ZNLayoutPtn> {
    using type = copy_gm_to_cbuf_align_v2_zn;
};

template <uint32_t Version>
struct copy_gm_to_l1_routing<Version, ZZLayoutPtn, ScaleANDLayoutPtn> {
    using type = copy_gm_to_cbuf_scale_a_nd_to_zz;
};

template <uint32_t Version>
struct copy_gm_to_l1_routing<Version, ZZLayoutPtn, ScaleADNLayoutPtn> {
    using type = copy_gm_to_cbuf_scale_a_dn_to_zz;
};

template <uint32_t Version>
struct copy_gm_to_l1_routing<Version, ZZLayoutPtn, ZZLayoutPtn> {
    using type = copy_gm_to_cbuf_scale_a_zz2_zz;
};

template <uint32_t Version>
struct copy_gm_to_l1_routing<Version, NNLayoutPtn, ScaleBNDLayoutPtn> {
    using type = copy_gm_to_cbuf_scale_b_nd_to_nn;
};

template <uint32_t Version>
struct copy_gm_to_l1_routing<Version, NNLayoutPtn, ScaleBDNLayoutPtn> {
    using type = copy_gm_to_cbuf_scale_b_dn_to_nn;
};

template <uint32_t Version>
struct copy_gm_to_l1_routing<Version, NNLayoutPtn, NNLayoutPtn> {
    using type = copy_gm_to_cbuf_scale_b_nn2_nn;
};

template <uint32_t Version>
struct copy_gm_to_l1_routing<Version, NC1HWC0LayoutPtn, NC1HWC0LayoutPtn> {
    using type = copy_gm_to_cbuf_nc1hwc0_to_nc1hwc0;
};

template <uint32_t Version>
struct copy_gm_to_l1_routing<Version, NC1HWC0LayoutPtn, NHWCLayoutPtn> {
    using type = copy_gm_to_cbuf_nhwc_to_nc1hwc0;
};

template <uint32_t Version>
struct copy_gm_to_l1_routing<Version, NC1HWC0LayoutPtn, NCHWLayoutPtn> {
    using type = copy_gm_to_cbuf_nchw_to_nc1hwc0;
};

template <uint32_t Version>
struct copy_gm_to_l1_routing<Version, NDC1HWC0LayoutPtn, NCDHWLayoutPtn> {
    using type = copy_gm_to_cbuf_ncdhw_to_ndc1hwc0;
};

} // namespace te
} // namespace asc

#endif // IMPL_TENSOR_API_ARCH_CUBE_GM_TO_L1_ROUTING_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif
