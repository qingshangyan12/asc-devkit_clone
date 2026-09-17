/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef ASCENDC_DUMP_TENSOR_POINTER_IMPL_H
#define ASCENDC_DUMP_TENSOR_POINTER_IMPL_H
#include "kernel_cube_pointer_traits.h"
namespace AscendC {
#if __NPU_ARCH__ == 3510 && !defined(ASCENDC_CPU_DEBUG)
namespace Impl {
template <typename P>
__aicore__ inline void DumpCubePointer(const P& input, uint32_t desc, uint32_t count,
    const uint32_t* shape, uint32_t shapeDim)
{
#if !(defined(ASCENDC_DUMP) && ASCENDC_DUMP == 0) || defined(ASCENDC_ACC_DUMP)
    constexpr Hardware space = CubeOperandTraits<P>::space;
    static_assert(space == Hardware::GM || space == Hardware::UB || space == Hardware::L1 ||
        space == Hardware::L0C || space == Hardware::BIAS || space == Hardware::FIXBUF,
        "Unsupported DumpTensor address space");
    uint64_t ctrlValue = get_ctrl();
    set_atomic_none();
    auto ptr = GetUnderlyingPtr(input);
    if constexpr (space == Hardware::GM) {
        __asc_aicore::asc_dump_gm(ptr, desc, count, shape, shapeDim);
    } else {
        dcci((__gm__ uint64_t*)g_sysPrintFifoSpace, cache_line_t::ENTIRE_DATA_CACHE, dcci_dst_t::CACHELINE_OUT);
        if (g_sysPrintFifoSpace != nullptr) {
            if constexpr (space == Hardware::UB) {
                __asc_aicore::asc_dump_ubuf(ptr, desc, count, shape, shapeDim);
            } else if ASCEND_IS_AIC {
                if constexpr (space == Hardware::L1) {
                    __asc_aicore::asc_dump_l1buf(ptr, desc, count, shape, shapeDim);
                } else if constexpr (space == Hardware::L0C) {
                    __asc_aicore::asc_dump_cbuf(ptr, desc, count, shape, shapeDim);
                } else {
                    __asc_aicore::asc_dump(ptr, desc, count, shape, shapeDim);
                }
            }
        }
    }
    set_ctrl(ctrlValue);
#endif
}
} // namespace Impl
template <typename P, typename Std::enable_if<Impl::CubeOperandTraits<P>::isPointer, bool>::type = true>
__aicore__ inline void DumpTensor(const P& input, uint32_t desc, uint32_t dumpSize)
{
#if !(defined(ASCENDC_DUMP) && ASCENDC_DUMP == 0)
    Impl::DumpCubePointer(input, desc, dumpSize, nullptr, 0);
#endif
}
template <typename P, typename Std::enable_if<Impl::CubeOperandTraits<P>::isPointer, bool>::type = true>
__aicore__ inline void DumpTensor(const P& input, uint32_t desc, uint32_t dumpSize, const ShapeInfo& shapeInfo)
{
#if !(defined(ASCENDC_DUMP) && ASCENDC_DUMP == 0)
    Impl::DumpCubePointer(input, desc, dumpSize, shapeInfo.shape, shapeInfo.shapeDim);
#endif
}
template <typename T, typename P,
    typename Std::enable_if<Impl::CubeOperandTraits<P>::isPointer &&
        Std::is_same<T, Impl::CubeElement<P>>::value, bool>::type = true>
__aicore__ inline void DumpTensor(const P& input, uint32_t desc, uint32_t dumpSize)
{
    DumpTensor<P>(input, desc, dumpSize);
}
template <typename T, typename P,
    typename Std::enable_if<Impl::CubeOperandTraits<P>::isPointer &&
        Std::is_same<T, Impl::CubeElement<P>>::value, bool>::type = true>
__aicore__ inline void DumpTensor(const P& input, uint32_t desc, uint32_t dumpSize, const ShapeInfo& shapeInfo)
{
    DumpTensor<P>(input, desc, dumpSize, shapeInfo);
}
template <typename P, typename Std::enable_if<Impl::CubeOperandTraits<P>::isPointer, bool>::type = true>
__aicore__ inline void DumpAccChkPoint(const P& input, uint32_t index, uint32_t countOff, uint32_t dumpSize)
{
#if !(defined(ASCENDC_DUMP) && ASCENDC_DUMP == 0) || defined(ASCENDC_ACC_DUMP)
    auto ptr = Impl::GetUnderlyingPtr(input);
    if constexpr (IsHalfByteDataType<typename Std::remove_const<Impl::CubeElement<P>>::type>()) {
        ptr += countOff / INT4_TWO;
    } else {
        ptr += countOff;
    }
    Impl::DumpCubePointer(ptr, index, dumpSize, nullptr, 0);
#endif
}
template <typename T, typename P,
    typename Std::enable_if<Impl::CubeOperandTraits<P>::isPointer &&
        Std::is_same<T, Impl::CubeElement<P>>::value, bool>::type = true>
__aicore__ inline void DumpAccChkPoint(const P& input, uint32_t index, uint32_t countOff, uint32_t dumpSize)
{
    DumpAccChkPoint<P>(input, index, countOff, dumpSize);
}
#endif
} // namespace AscendC
#endif
