/**
* Copyright (c) 2026 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/

#include "kernel_operator_cache_intf.h"


//template <typename T, CacheLine entireType, DcciDst dcciDst>
//__aicore__ inline void DataCacheCleanAndInvalid(const GlobalTensor<T>& dst);
void HostTest_DataCacheCleanAndInvalid_1() {
    AscendC::GlobalTensor<float> dst;
    AscendC::DataCacheCleanAndInvalid<float, AscendC::CacheLine::ENTIRE_DATA_CACHE, AscendC::DcciDst::DCCI_DST_NONE>(dst);
}


//template <typename T, CacheLine entireType, DcciDst dcciDst>
//__aicore__ inline void DataCacheCleanAndInvalid(const LocalTensor<T>& dst);
void HostTest_DataCacheCleanAndInvalid_2() {
    AscendC::LocalTensor<float> dst;
    AscendC::DataCacheCleanAndInvalid<float, AscendC::CacheLine::ENTIRE_DATA_CACHE, AscendC::DcciDst::DCCI_DST_NONE>(dst);
}


//template <typename T, CacheLine entireType>
//__aicore__ inline void DataCacheCleanAndInvalid(const GlobalTensor<T>& dst);
void HostTest_DataCacheCleanAndInvalid_3() {
    AscendC::GlobalTensor<float> dst;
    AscendC::DataCacheCleanAndInvalid<float, AscendC::CacheLine::ENTIRE_DATA_CACHE>(dst);
}