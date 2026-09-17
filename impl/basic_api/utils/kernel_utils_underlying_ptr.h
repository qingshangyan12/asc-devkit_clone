/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

/*!
 * \file kernel_utils_underlying_ptr.h
 * \brief Shared operand normalization helper for the Basic API pointer-ization extension.
 *
 *        GetUnderlyingPtr() resolves a Basic API operand to its underlying hardware pointer:
 *          - a bare hardware pointer (__gm__ / __cbuf__ / __ca__ / __cb__ / __cc__ / __biasbuf__ / __ubuf__ ...)
 *            is passed through unchanged, so the operand address space is preserved;
 *          - a LocalTensor / GlobalTensor operand is resolved through GetPhyAddr().
 *
 *        This keeps one public API entry point usable with both calling conventions, and gives the CUBE
 *        booklet and the sibling VECTOR booklet one shared extraction primitive.
 */

#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#pragma message( \
    "impl/basic_api/utils/kernel_utils_underlying_ptr.h is an internal header file and must not be used directly. Functions or variables defined in this file may be removed in the future. Please use \"#include \"basic_api/kernel_operator_mm_intf.h\"\" and use public functions or variables defined in interface headers files.")
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_UTILS_UNDERLYING_PTR_H__
#endif

#ifndef ASCENDC_UTILS_UNDERLYING_PTR_H
#define ASCENDC_UTILS_UNDERLYING_PTR_H

#include "../../../include/basic_api/kernel_tensor.h"
#include "../../utils/std/type_traits/is_pointer.h"

namespace AscendC {

/*!
 * \brief Compile-time predicate: is the deduced operand type a bare hardware pointer?
 * \note Address-space qualified pointers are ordinary pointer types for the type system, so the Tp*
 *       partial specialization of Std::is_pointer covers __gm__ / __cbuf__ / __ca__ / __cb__ / __cc__ /
 *       __biasbuf__ / __ubuf__ and friends.
 */
template <typename Tp>
using IsRawHwPtr = Std::is_pointer<Tp>;

template <typename Tp>
constexpr bool isRawHwPtrV = Std::is_pointer_v<Tp>;

/*!
 * \brief Resolve an operand to its underlying hardware pointer.
 * \param [in] val LocalTensor / GlobalTensor operand, or a bare hardware pointer.
 * \return the underlying hardware pointer. For a bare pointer the exact incoming type (including its
 *         address space) is preserved by the deduced return type.
 */
template <typename Tp>
__aicore__ inline auto GetUnderlyingPtr(const Tp& val)
{
    if constexpr (isRawHwPtrV<Tp>) {
        return val;
    } else {
        return val.GetPhyAddr();
    }
}

} // namespace AscendC

#endif // ASCENDC_UTILS_UNDERLYING_PTR_H
#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_UTILS_UNDERLYING_PTR_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_UTILS_UNDERLYING_PTR_H__
#endif