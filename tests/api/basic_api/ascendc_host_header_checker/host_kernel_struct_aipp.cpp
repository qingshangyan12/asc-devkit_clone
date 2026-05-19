/**
* Copyright (c) 2026 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/

#include "kernel_struct_aipp.h"


//template <typename T> struct AippParams {
//    AippPaddingParams<T> paddingParams;
//    AippSwapParams swapParams;
//    AippSingleLineParams singleLineParams;
//    AippDataTypeConvParams dtcParams;
//    AippChannelPaddingParams<T> cPaddingParams;
//    AippColorSpaceConvParams cscParams;
//};
void HostTest_AippParams() {
    AscendC::AippParams<float> aippParams;
    aippParams.paddingParams.paddingMode = 0;
    aippParams.paddingParams.paddingValueCh0 = 0.0f;
    aippParams.swapParams.isSwapRB = false;
    aippParams.singleLineParams.isSingleLineCopy = false;
    aippParams.dtcParams.dtcMeanCh0 = 0;
    aippParams.cPaddingParams.cPaddingMode = 0;
    aippParams.cPaddingParams.cPaddingValue = 0.0f;
    aippParams.cscParams.isEnableCsc = false;
}