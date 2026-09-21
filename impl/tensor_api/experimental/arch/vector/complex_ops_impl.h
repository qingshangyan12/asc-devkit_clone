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
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_COMPLEX_OPS_IMPL_H
#endif

#ifndef IMPL_TENSOR_API_EXPERIMENTAL_ARCH_VECTOR_COMPLEX_OPS_IMPL_H
#define IMPL_TENSOR_API_EXPERIMENTAL_ARCH_VECTOR_COMPLEX_OPS_IMPL_H

#include "impl/tensor_api/experimental/arch/utils/reg_utils.h"

namespace asc {
namespace te {
namespace experimental {
namespace detail {

template <typename Tag>
__simd_callee__ inline reg_tensor<Tag> make_complex(
    const typename reg_tensor<Tag>::reg_type& real, const typename reg_tensor<Tag>::reg_type& imag,
    vector_bool mask)
{
    reg_tensor<Tag> dst;
    dst.real_ = real;
    dst.imag_ = imag;
    dst.mask = mask;
    return dst;
}

template <typename Tag>
__simd_callee__ inline reg_tensor<Tag> complex_add(const reg_tensor<Tag>& src0, const reg_tensor<Tag>& src1)
{
    static_assert(is_complex_tag_v<Tag>, "complex_add requires a complex tag");
    typename reg_tensor<Tag>::reg_type real_dst;
    typename reg_tensor<Tag>::reg_type imag_dst;
    asc_add(real_dst, src0.real_, src1.real_, src0.mask);
    asc_add(imag_dst, src0.imag_, src1.imag_, src0.mask);
    return make_complex<Tag>(real_dst, imag_dst, src0.mask);
}

template <typename Tag>
__simd_callee__ inline reg_tensor<Tag> complex_sub(const reg_tensor<Tag>& src0, const reg_tensor<Tag>& src1)
{
    static_assert(is_complex_tag_v<Tag>, "complex_sub requires a complex tag");
    typename reg_tensor<Tag>::reg_type real_dst;
    typename reg_tensor<Tag>::reg_type imag_dst;
    asc_sub(real_dst, src0.real_, src1.real_, src0.mask);
    asc_sub(imag_dst, src0.imag_, src1.imag_, src0.mask);
    return make_complex<Tag>(real_dst, imag_dst, src0.mask);
}

template <typename Tag>
__simd_callee__ inline reg_tensor<Tag> complex_adds(
    const reg_tensor<Tag>& src, const complex_scalar<complex_elem_t<Tag>>& scalar)
{
    static_assert(is_complex_tag_v<Tag>, "complex_adds requires a complex tag");
    typename reg_tensor<Tag>::reg_type real_dst;
    typename reg_tensor<Tag>::reg_type imag_dst;
    asc_add_scalar(real_dst, src.real_, scalar.real, src.mask);
    asc_add_scalar(imag_dst, src.imag_, scalar.imag, src.mask);
    return make_complex<Tag>(real_dst, imag_dst, src.mask);
}

template <typename Tag>
__simd_callee__ inline reg_tensor<Tag> complex_mul(const reg_tensor<Tag>& src0, const reg_tensor<Tag>& src1)
{
    static_assert(is_complex_tag_v<Tag>, "complex_mul requires a complex tag");
    // (a+bi)(c+di) = (ac-bd) + (ad+bc)i
    typename reg_tensor<Tag>::reg_type ac;
    typename reg_tensor<Tag>::reg_type bd;
    typename reg_tensor<Tag>::reg_type ad;
    typename reg_tensor<Tag>::reg_type bc;
    asc_mul(ac, src0.real_, src1.real_, src0.mask);
    asc_mul(bd, src0.imag_, src1.imag_, src0.mask);
    asc_mul(ad, src0.real_, src1.imag_, src0.mask);
    asc_mul(bc, src0.imag_, src1.real_, src0.mask);
    typename reg_tensor<Tag>::reg_type real_dst;
    typename reg_tensor<Tag>::reg_type imag_dst;
    asc_sub(real_dst, ac, bd, src0.mask);
    asc_add(imag_dst, ad, bc, src0.mask);
    return make_complex<Tag>(real_dst, imag_dst, src0.mask);
}

template <typename Tag>
__simd_callee__ inline reg_tensor<Tag> complex_muls(
    const reg_tensor<Tag>& src, const complex_scalar<complex_elem_t<Tag>>& scalar)
{
    static_assert(is_complex_tag_v<Tag>, "complex_muls requires a complex tag");
    typename reg_tensor<Tag>::reg_type real_s;
    typename reg_tensor<Tag>::reg_type imag_s;
    asc_duplicate_scalar(real_s, scalar.real, src.mask);
    asc_duplicate_scalar(imag_s, scalar.imag, src.mask);
    return complex_mul(src, make_complex<Tag>(real_s, imag_s, src.mask));
}

template <typename Tag>
__simd_callee__ inline reg_tensor<Tag> complex_div(const reg_tensor<Tag>& src0, const reg_tensor<Tag>& src1)
{
    static_assert(is_complex_tag_v<Tag>, "complex_div requires a complex tag");
    using Elem = complex_elem_t<Tag>;
    using Vec = typename reg_tensor<Tag>::reg_type;
    const vector_bool mask = src0.mask;

    Vec a = src0.real_;
    Vec b = src0.imag_;
    Vec c = src1.real_;
    Vec d = src1.imag_;

    Vec abs_c;
    Vec abs_d;
    asc_abs(abs_c, c, mask);
    asc_abs(abs_d, d, mask);

    Vec one;
    Vec zero;
    asc_duplicate_scalar(one, static_cast<Elem>(1), mask);
    asc_duplicate_scalar(zero, static_cast<Elem>(0), mask);

    vector_bool ge_mask;
    asc_ge(ge_mask, abs_c, abs_d, mask);

    vector_bool eq_c0;
    vector_bool eq_d0;
    asc_eq(eq_c0, abs_c, zero, mask);
    asc_eq(eq_d0, abs_d, zero, mask);
    vector_bool both_zero;
    asc_and(both_zero, eq_c0, eq_d0, mask);

    // abs_c >= abs_d: rat = d/c
    Vec rat_ge;
    Vec tmp;
    Vec scl;
    Vec real_ge;
    Vec imag_ge;
    asc_div(rat_ge, d, c, ge_mask);
    asc_mul(tmp, d, rat_ge, ge_mask);
    asc_add(tmp, c, tmp, ge_mask);
    asc_div(scl, one, tmp, ge_mask);
    asc_mul(tmp, b, rat_ge, ge_mask);
    asc_add(tmp, a, tmp, ge_mask);
    asc_mul(real_ge, tmp, scl, ge_mask);
    asc_mul(tmp, a, rat_ge, ge_mask);
    asc_sub(tmp, b, tmp, ge_mask);
    asc_mul(imag_ge, tmp, scl, ge_mask);

    // abs_c < abs_d: rat = c/d
    vector_bool lt_mask;
    asc_not(lt_mask, ge_mask, mask);
    Vec rat_lt;
    Vec real_lt;
    Vec imag_lt;
    asc_div(rat_lt, c, d, lt_mask);
    asc_mul(tmp, c, rat_lt, lt_mask);
    asc_add(tmp, d, tmp, lt_mask);
    asc_div(scl, one, tmp, lt_mask);
    asc_mul(tmp, a, rat_lt, lt_mask);
    asc_add(tmp, tmp, b, lt_mask);
    asc_mul(real_lt, tmp, scl, lt_mask);
    asc_mul(tmp, b, rat_lt, lt_mask);
    asc_sub(tmp, tmp, a, lt_mask);
    asc_mul(imag_lt, tmp, scl, lt_mask);

    Vec real_dst;
    Vec imag_dst;
    asc_select(real_dst, real_ge, real_lt, ge_mask);
    asc_select(imag_dst, imag_ge, imag_lt, ge_mask);
    asc_select(real_dst, zero, real_dst, both_zero);
    asc_select(imag_dst, zero, imag_dst, both_zero);
    return make_complex<Tag>(real_dst, imag_dst, mask);
}

template <typename Tag>
__simd_callee__ inline reg_tensor<complex_elem_t<Tag>> complex_abs(const reg_tensor<Tag>& src)
{
    static_assert(is_complex_tag_v<Tag>, "complex_abs requires a complex tag");
    using Elem = complex_elem_t<Tag>;
    using Vec = typename reg_tensor<Tag>::reg_type;
    Vec re2;
    Vec im2;
    Vec sum;
    asc_mul(re2, src.real_, src.real_, src.mask);
    asc_mul(im2, src.imag_, src.imag_, src.mask);
    asc_add(sum, re2, im2, src.mask);
    reg_tensor<Elem> dst;
    asc_sqrt(dst.reg, sum, src.mask);
    dst.mask = src.mask;
    return dst;
}

template <typename Tag>
__simd_callee__ inline reg_tensor<Tag> complex_fill_scalar(
    const complex_scalar<complex_elem_t<Tag>>& value, const reg_tensor<bool>& mask)
{
    static_assert(is_complex_tag_v<Tag>, "complex_fill_scalar requires a complex tag");
    typename reg_tensor<Tag>::reg_type real_dst;
    typename reg_tensor<Tag>::reg_type imag_dst;
    asc_duplicate_scalar(real_dst, value.real, mask.reg);
    asc_duplicate_scalar(imag_dst, value.imag, mask.reg);
    return make_complex<Tag>(real_dst, imag_dst, mask.reg);
}

template <typename Tag>
__simd_callee__ inline reg_tensor<Tag> complex_fill_reg(
    const reg_tensor<Tag>& src, const reg_tensor<bool>& mask)
{
    static_assert(is_complex_tag_v<Tag>, "complex_fill_reg requires a complex tag");
    typename reg_tensor<Tag>::reg_type real_dst;
    typename reg_tensor<Tag>::reg_type imag_dst;
    asc_duplicate(real_dst, src.real_, mask.reg);
    asc_duplicate(imag_dst, src.imag_, mask.reg);
    return make_complex<Tag>(real_dst, imag_dst, mask.reg);
}

template <typename Tag>
__simd_callee__ inline reg_tensor<Tag> complex_move(const reg_tensor<Tag>& src)
{
    static_assert(Std::is_same_v<Tag, complex_float>, "move only supports complex64 (complex_float)");
    return make_complex<Tag>(src.real_, src.imag_, src.mask);
}

template <typename Tag>
__simd_callee__ inline reg_tensor<Tag> complex_move_merging(
    const reg_tensor<Tag>& dst, const reg_tensor<Tag>& src, const reg_tensor<bool>& mask)
{
    static_assert(Std::is_same_v<Tag, complex_float>, "move only supports complex64 (complex_float)");
    using Elem = complex_elem_t<Tag>;
    reg_tensor<Tag> out = dst;
    auto src_real = src.real_;
    auto src_imag = src.imag_;
    copy_merging<Elem>(out.real_, src_real, mask.reg);
    copy_merging<Elem>(out.imag_, src_imag, mask.reg);
    out.mask = mask.reg;
    return out;
}

template <typename Elem, typename Pointer>
__simd_callee__ inline complex<Elem> load_complex_from_ptr(Pointer src)
{
    static_assert(Std::is_same_v<Elem, half> || Std::is_same_v<Elem, float>, "load_complex supports half/float");
    using Tag = typename complex_map<Elem>::type;
    typename reg_tensor<Tag>::reg_type real;
    typename reg_tensor<Tag>::reg_type imag;
    asc_loadalign_deintlv(real, imag, src);
    vector_bool mask;
    if constexpr (Std::is_same_v<Elem, half>) {
        mask = asc_create_mask_b16(PAT_ALL);
    } else {
        mask = asc_create_mask_b32(PAT_ALL);
    }
    return make_complex<Tag>(real, imag, mask);
}

template <typename Elem, typename Pointer>
__simd_callee__ inline void store_complex_to_ptr(Pointer dst, const complex<Elem>& src)
{
    static_assert(Std::is_same_v<Elem, half> || Std::is_same_v<Elem, float>, "store_complex supports half/float");
    if constexpr (Std::is_same_v<Elem, half>) {
        asc_storealign_intlv(dst, src.real_, src.imag_);
    } else {
        typename reg_tensor<Elem>::reg_type v0;
        typename reg_tensor<Elem>::reg_type v1;
        vector_bool m0;
        vector_bool m1;
        asc_intlv(v0, v1, src.real_, src.imag_);
        asc_intlv_b32(m0, m1, src.mask, src.mask);
        constexpr uint32_t vf_bytes = 256;
        constexpr uint32_t elem_per_reg = vf_bytes / sizeof(float);
        asc_storealign(dst, v0, m0);
        asc_storealign(dst + elem_per_reg, v1, m1);
    }
}

} // namespace detail
} // namespace experimental
} // namespace te
} // namespace asc

#endif // IMPL_TENSOR_API_EXPERIMENTAL_ARCH_VECTOR_COMPLEX_OPS_IMPL_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_COMPLEX_OPS_IMPL_H)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_COMPLEX_OPS_IMPL_H
#endif
