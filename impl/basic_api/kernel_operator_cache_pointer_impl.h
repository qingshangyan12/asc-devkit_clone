/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef ASCENDC_MODULE_OPERATOR_CACHE_POINTER_IMPL_H
#define ASCENDC_MODULE_OPERATOR_CACHE_POINTER_IMPL_H
#include "utils/kernel_utils_dma_pointer.h"

#if !defined(ASCENDC_CPU_DEBUG) && ((__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 2201))
namespace AscendC {
template <
    typename Offset, typename S,
    typename Std::enable_if<
        DmaInternal::DmaTraits<S>::isPointer && DmaInternal::DmaCanUse<S, Hardware::GM>::value &&
            Std::is_same<DmaInternal::DmaElem<S>, uint64_t>::value &&
            (Std::is_same<Offset, int16_t>::value || Std::is_same<Offset, int64_t>::value),
        bool>::type>
__aicore__ inline void DataCachePreload(const S& src, const Offset cacheOffset)
{
    auto address = DmaInternal::GetUnderlyingPtr<Hardware::GM>(src);
#if __NPU_ARCH__ == 3510
    DataCachePreloadImpl(const_cast<__gm__ uint64_t*>(address), cacheOffset);
#else
    GlobalTensor<uint64_t> tensor;
    tensor.SetGlobalBuffer(const_cast<__gm__ uint64_t*>(address));
    DataCachePreload(tensor, cacheOffset);
#endif
}

template <
    typename T, CacheLine entireType, DcciDst dcciDst,
    typename Std::enable_if<
        DmaInternal::DmaTraits<__gm__ T*>::isPointer && DmaInternal::DmaTraits<__gm__ T*>::isWritable, bool>::type>
__aicore__ inline void DataCacheCleanAndInvalid(__gm__ T* dst)
{
    DcciGMImpl<T, entireType, dcciDst>(DmaInternal::GetUnderlyingPtr<Hardware::GM>(dst));
}

template <
    typename T, CacheLine entireType, DcciDst dcciDst,
    typename Std::enable_if<
        DmaInternal::DmaTraits<__ubuf__ T*>::isPointer && DmaInternal::DmaTraits<__ubuf__ T*>::isWritable, bool>::type>
__aicore__ inline void DataCacheCleanAndInvalid(__ubuf__ T* dst)
{
    DcciUBImpl<T, entireType, dcciDst>(dst);
}

template <
    typename T, CacheLine entireType,
    typename Std::enable_if<
        DmaInternal::DmaTraits<__gm__ T*>::isPointer && DmaInternal::DmaTraits<__gm__ T*>::isWritable, bool>::type>
__aicore__ inline void DataCacheCleanAndInvalid(__gm__ T* dst)
{
    DcciGMImpl<T, entireType>(DmaInternal::GetUnderlyingPtr<Hardware::GM>(dst));
}
} // namespace AscendC
#endif
#endif // ASCENDC_MODULE_OPERATOR_CACHE_POINTER_IMPL_H
