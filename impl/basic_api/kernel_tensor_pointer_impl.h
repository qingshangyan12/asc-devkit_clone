/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef ASCENDC_TENSOR_POINTER_IMPL_H
#define ASCENDC_TENSOR_POINTER_IMPL_H
#if defined(__NPU_ARCH__) && !defined(ASCENDC_CPU_DEBUG)
namespace AscendC {
namespace Impl {
template <typename S>
__aicore__ inline void FillLocalTensorAddrWithOffset(TBuffAddr& address, uint64_t src, uint32_t offset,
    TPosition sourcePos, Hardware expected, uint32_t sourceSize)
{
    static_assert(!Std::is_const<S>::value, "SetAddrWithOffset requires a writable source");
    ASCENDC_ASSERT(GetPhyType(sourcePos) == expected, {
        KERNEL_LOG(KERNEL_ERROR, "SetAddrWithOffset source position does not match pointer space");
    });
    ASCENDC_ASSERT(uint64_t(offset) * sizeof(S) <= sourceSize, {
        KERNEL_LOG(KERNEL_ERROR, "SetAddrWithOffset offset exceeds source size");
    });
    address.logicPos = static_cast<int8_t>(sourcePos);
    address.dataLen = sourceSize;
    address.bufferAddr = src + uint64_t(offset) * sizeof(S);
}
}

template <typename T>
template <typename S>
__aicore__ inline void LocalTensor<T>::SetAddrWithOffset(__cbuf__ S* src, uint32_t offset,
    TPosition sourcePos, uint32_t sourceSize)
{
    Impl::FillLocalTensorAddrWithOffset<S>(this->address_, (uint64_t)src, offset, sourcePos, Hardware::L1, sourceSize);
}
template <typename T>
template <typename S>
__aicore__ inline void LocalTensor<T>::SetAddrWithOffset(__ca__ S* src, uint32_t offset,
    TPosition sourcePos, uint32_t sourceSize)
{
    Impl::FillLocalTensorAddrWithOffset<S>(this->address_, (uint64_t)src, offset, sourcePos, Hardware::L0A, sourceSize);
}
template <typename T>
template <typename S>
__aicore__ inline void LocalTensor<T>::SetAddrWithOffset(__cb__ S* src, uint32_t offset,
    TPosition sourcePos, uint32_t sourceSize)
{
    Impl::FillLocalTensorAddrWithOffset<S>(this->address_, (uint64_t)src, offset, sourcePos, Hardware::L0B, sourceSize);
}
template <typename T>
template <typename S>
__aicore__ inline void LocalTensor<T>::SetAddrWithOffset(__cc__ S* src, uint32_t offset,
    TPosition sourcePos, uint32_t sourceSize)
{
    Impl::FillLocalTensorAddrWithOffset<S>(this->address_, (uint64_t)src, offset, sourcePos, Hardware::L0C, sourceSize);
}
template <typename T>
template <typename S>
__aicore__ inline void LocalTensor<T>::SetAddrWithOffset(__ubuf__ S* src, uint32_t offset,
    TPosition sourcePos, uint32_t sourceSize)
{
    Impl::FillLocalTensorAddrWithOffset<S>(this->address_, (uint64_t)src, offset, sourcePos, Hardware::UB, sourceSize);
}
template <typename T>
template <typename S>
__aicore__ inline void LocalTensor<T>::SetAddrWithOffset(__biasbuf__ S* src, uint32_t offset,
    TPosition sourcePos, uint32_t sourceSize)
{
    Impl::FillLocalTensorAddrWithOffset<S>(this->address_, (uint64_t)src, offset, sourcePos, Hardware::BIAS, sourceSize);
}
template <typename T>
template <typename S>
__aicore__ inline void LocalTensor<T>::SetAddrWithOffset(__fbuf__ S* src, uint32_t offset,
    TPosition sourcePos, uint32_t sourceSize)
{
    Impl::FillLocalTensorAddrWithOffset<S>(this->address_, (uint64_t)src, offset, sourcePos, Hardware::FIXBUF, sourceSize);
}
} // namespace AscendC
#endif
#endif
