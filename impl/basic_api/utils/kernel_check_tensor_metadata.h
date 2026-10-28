/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef ASCENDC_KERNEL_CHECK_TENSOR_METADATA_H
#define ASCENDC_KERNEL_CHECK_TENSOR_METADATA_H

#if ASCENDC_CPU_DEBUG
#include "../kernel_utils.h"

namespace AscendC {
namespace check {
namespace detail {

template <typename Params>
inline void InitializeCheckTensorPairMetadata(
    Params& params, uint32_t dstDtypeBytesIn, uint32_t srcDtypeBytesIn, uint64_t dstSizeIn, uint64_t srcSizeIn,
    uint8_t dstPosIn, uint8_t srcPosIn)
{
    params.dstDtypeBytes = dstDtypeBytesIn;
    params.srcDtypeBytes = srcDtypeBytesIn;
    params.dstSize = dstSizeIn;
    params.srcSize = srcSizeIn;
    params.dstLogicPos = dstPosIn;
    params.srcLogicPos = srcPosIn;
    params.dstPos = static_cast<uint8_t>(GetPhyType(static_cast<TPosition>(dstPosIn)));
    params.srcPos = static_cast<uint8_t>(GetPhyType(static_cast<TPosition>(srcPosIn)));
}

template <typename Params>
inline void InitializeCheckTensorPairSrc0Metadata(
    Params& params, uint32_t dstDtypeBytesIn, uint32_t src0DtypeBytesIn, uint64_t dstSizeIn, uint64_t src0SizeIn,
    uint8_t dstPosIn, uint8_t src0PosIn)
{
    params.dstDtypeBytes = dstDtypeBytesIn;
    params.src0DtypeBytes = src0DtypeBytesIn;
    params.dstSize = dstSizeIn;
    params.src0Size = src0SizeIn;
    params.dstLogicPos = dstPosIn;
    params.src0LogicPos = src0PosIn;
    params.dstPos = static_cast<uint8_t>(GetPhyType(static_cast<TPosition>(dstPosIn)));
    params.src0Pos = static_cast<uint8_t>(GetPhyType(static_cast<TPosition>(src0PosIn)));
}

template <typename Params>
inline void InitializeCheckTensorTripleMetadata(
    Params& params, uint32_t dstDtypeBytesIn, uint32_t src0DtypeBytesIn, uint32_t src1DtypeBytesIn, uint64_t dstSizeIn,
    uint64_t src0SizeIn, uint64_t src1SizeIn, uint8_t dstPosIn, uint8_t src0PosIn, uint8_t src1PosIn)
{
    params.dstDtypeBytes = dstDtypeBytesIn;
    params.src0DtypeBytes = src0DtypeBytesIn;
    params.src1DtypeBytes = src1DtypeBytesIn;
    params.dstSize = dstSizeIn;
    params.src0Size = src0SizeIn;
    params.src1Size = src1SizeIn;
    params.dstLogicPos = dstPosIn;
    params.src0LogicPos = src0PosIn;
    params.src1LogicPos = src1PosIn;
    params.dstPos = static_cast<uint8_t>(GetPhyType(static_cast<TPosition>(dstPosIn)));
    params.src0Pos = static_cast<uint8_t>(GetPhyType(static_cast<TPosition>(src0PosIn)));
    params.src1Pos = static_cast<uint8_t>(GetPhyType(static_cast<TPosition>(src1PosIn)));
}

template <typename Params>
inline void InitializeCheckTensorTripleSizeAndPosition(
    Params& params, uint64_t dstSizeIn, uint64_t src0SizeIn, uint64_t src1SizeIn, uint8_t dstPosIn, uint8_t src0PosIn,
    uint8_t src1PosIn)
{
    params.dstSize = dstSizeIn;
    params.src0Size = src0SizeIn;
    params.src1Size = src1SizeIn;
    params.dstLogicPos = dstPosIn;
    params.src0LogicPos = src0PosIn;
    params.src1LogicPos = src1PosIn;
    params.dstPos = static_cast<uint8_t>(GetPhyType(static_cast<TPosition>(dstPosIn)));
    params.src0Pos = static_cast<uint8_t>(GetPhyType(static_cast<TPosition>(src0PosIn)));
    params.src1Pos = static_cast<uint8_t>(GetPhyType(static_cast<TPosition>(src1PosIn)));
}

} // namespace detail
} // namespace check
} // namespace AscendC
#endif
#endif
