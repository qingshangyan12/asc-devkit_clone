/**
* Copyright (c) 2025 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/

#ifndef IMPL_C_API_C_API_INSTR_IMPL_VECTOR_COMPUTE_SET_MASK_NORM_IMPL_NPU_ARCH_2201_ASC_TRANSDATA_TO_5HD_IMPL_H
#define IMPL_C_API_C_API_INSTR_IMPL_VECTOR_COMPUTE_SET_MASK_NORM_IMPL_NPU_ARCH_2201_ASC_TRANSDATA_TO_5HD_IMPL_H

#define asc_transdata_to_5hd_b8(dst, src, repeat, dst_stride, src_stride, dst_high_half, src_high_half) scatter_vnchwconv_b8(dst, src, repeat, dst_stride, src_stride, dst_high_half, src_high_half)

#define asc_transdata_to_5hd_b16(dst, src, repeat, dstStride, srcStride) scatter_vnchwconv_b16(dst, src, repeat, dstStride, srcStride)

#define asc_transdata_to_5hd_b32(dst, src, repeat, dstStride, srcStride) scatter_vnchwconv_b32(dst, src, repeat, dstStride, srcStride)

#endif