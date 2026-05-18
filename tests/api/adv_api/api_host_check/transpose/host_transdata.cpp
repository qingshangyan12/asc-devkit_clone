/**
* Copyright (c) 2026 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/
#include "transpose/transdata.h"
#include "kernel_struct_data_copy.h"

__aicore__ void TestTransData0() {
    AscendC::LocalTensor<half> dstTensor;
    AscendC::LocalTensor<half> srcTensor;
    AscendC::LocalTensor<uint8_t> sharedTmpBuffer;
    AscendC::Layout srcLayout = AscendC::MakeLayout(AscendC::MakeShape(1, 1, 1, 1, 1), AscendC::MakeStride());
    AscendC::Layout dstLayout = AscendC::MakeLayout(AscendC::MakeShape(1, 1, 1, 1, 1, 1, 1), AscendC::MakeStride());
    AscendC::TransDataParams<decltype(srcLayout), decltype(dstLayout)> params = {srcLayout, dstLayout};
    static constexpr AscendC::TransDataConfig config = {AscendC::DataFormat::NCDHW, AscendC::DataFormat::FRACTAL_Z_3D};
    AscendC::TransData<config>(dstTensor, srcTensor, sharedTmpBuffer, params);
}

__aicore__ void TestTransData1() {
    AscendC::LocalTensor<half> dstTensor;
    AscendC::LocalTensor<half> srcTensor;
    AscendC::Layout srcLayout = AscendC::MakeLayout(AscendC::MakeShape(1, 1, 1, 1, 1), AscendC::MakeStride());
    AscendC::Layout dstLayout = AscendC::MakeLayout(AscendC::MakeShape(1, 1, 1, 1, 1, 1, 1), AscendC::MakeStride());
    AscendC::TransDataParams<decltype(srcLayout), decltype(dstLayout)> params = {srcLayout, dstLayout};
    static constexpr AscendC::TransDataConfig config = {AscendC::DataFormat::NCDHW, AscendC::DataFormat::FRACTAL_Z_3D};
    AscendC::TransData<config>(dstTensor, srcTensor, params);
}

