/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

/*!
 * \file kernel_operator_data_copy_ptr_intf.h
 * \brief 搬运类接口（DataCopy / DataCopyPad / DataCopyL1ToUB / Copy）指针化扩展声明：
 *        在保留原有 Tensor 入参重载的前提下，新增地址空间限定裸指针
 *        （__ubuf__ / __gm__ / __cbuf__ / __cc__）入参重载，二者复用同一套底层 *Impl 实现，
 *        语义与数值结果完全一致。
 */

#if defined(__NPU_COMPILER_INTERNAL_PURE_SIMT__)
#error "kernel_operator_data_copy_ptr_intf.h cannot be used with compile flag --enable-simt enabled."
#endif

#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_DATA_COPY_PTR_INTF_H__
#endif
#ifndef ASCENDC_MODULE_OPERATOR_DATA_COPY_PTR_INTF_H
#define ASCENDC_MODULE_OPERATOR_DATA_COPY_PTR_INTF_H

#include "../../impl/basic_api/kernel_macros.h"
#include "../../impl/basic_api/common_types.h"
#include "kernel_struct_data_copy.h"
#include "kernel_tensor.h"

/*
 * 地址空间限定符（__ubuf__ / __gm__ / __cbuf__ / __cc__ / __fbuf__）仅在设备编译器
 * （ccec，定义 __CCE__）下真实生效；宿主机侧（CPU 仿真等，cpu_stub.hpp）这些宏展开为空，
 * 仅按地址空间区分的同形重载会塌缩为同一签名。因此宿主机侧每组同形重载仅保留一个代表
 * （统一保留 GM -> UB 搬入方向），其余方向为设备编译专用。
 */
#if defined(__CCE__)
#define ASCENDC_PTR_AS_REAL 1
#endif

namespace AscendC {
/* **************************************************************************************************
 * DataCopy 指针化扩展（Pointer 输入）                                        *
 * ************************************************************************************************* */
/*
 * @ingroup DataCopy Level 0
 * @brief datacopy from src to dst, applicable to vector data（GM -> UB 指针入参版本）
 * @param [out] dst output UB pointer (__ubuf__ T*)
 * @param [in] src input GM pointer (__gm__ T*)
 * @param [in] repeatParams.blockCount number of blocks
 * @param [in] repeatParams.blockLen Length of blocks
 * @param [in] repeatParams.srcGap src block gap
 * @param [in] repeatParams.dstGap dst block gap
 */
template <typename T>
__aicore__ inline void __inout_pipe__(MTE2)
    DataCopy(__ubuf__ T* dst, __gm__ T* src, const DataCopyParams& repeatParams);

/*
 * @ingroup DataCopy Level 2
 * @brief datacopy from src to dst（GM -> UB 指针入参版本）
 * @param [out] dst output UB pointer (__ubuf__ T*)
 * @param [in] src input GM pointer (__gm__ T*)
 * @param [in] count Number of operands
 */
template <typename T>
__aicore__ inline __inout_pipe__(MTE2) void DataCopy(__ubuf__ T* dst, __gm__ T* src, const uint32_t count);

/*
 * @ingroup DataCopy Level 1
 * @brief datacopy from src to dst（GM -> UB SliceInfo 指针入参版本）
 * @param [out] dst output UB pointer (__ubuf__ T*)
 * @param [in] src input GM pointer (__gm__ T*)
 * @param [in] dstSliceInfo[] ub slice info，指针路径要求 shapeValue 非 0（无 Tensor 形状可回退）
 * @param [in] srcSliceInfo[] gm slice info
 * @param [in] dimValue dim value also for length for dstSliceInfo[] and srcSliceInfo[]
 */
template <typename T>
__aicore__ inline __inout_pipe__(MTE2) void DataCopy(
    __ubuf__ T* dst, __gm__ T* src, const SliceInfo dstSliceInfo[], const SliceInfo srcSliceInfo[],
    const uint32_t dimValue = 1);

/*
 * @ingroup DataCopy Level 0
 * @brief format transform(such as nd2nz) during data load from OUT to UB（GM -> UB 指针入参版本）
 */
#if (__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102) || (__NPU_ARCH__ == 3113) || (__NPU_ARCH__ == 5101) || \
    (__NPU_ARCH__ == 5161) || (__NPU_ARCH__ == 5165) || (__NPU_ARCH__ == 5163)
template <typename T>
__aicore__ inline __inout_pipe__(MTE2) void DataCopy(__ubuf__ T* dst, __gm__ T* src, const Nd2NzParams& intriParams);
#endif

/*
 * @ingroup DataCopy Level 0
 * @brief datacopy from src to dst with N-dimensional DMA（GM -> UB 指针入参版本）
 */
#if (__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102)
template <typename T, uint8_t dim, const NdDmaConfig& config = kDefaultNdDmaConfig>
__aicore__ inline void DataCopy(__ubuf__ T* dst, __gm__ T* src, const MultiCopyParams<T, dim>& params);
#endif

/*
 * @ingroup DataCopy
 * @brief datacopy from src to dst, applicable to cube data（GM -> UB 指针入参版本）
 */
template <typename T>
__aicore__ inline __inout_pipe__(MTE2) void DataCopy(
    __ubuf__ T* dst, __gm__ T* src, const DataCopyParams& intriParams, const DataCopyEnhancedParams& enhancedParams);

#if defined(ASCENDC_PTR_AS_REAL)
/*
 * @ingroup DataCopy Level 0
 * @brief datacopy from src to dst, applicable to vector data（GM -> L1 指针入参版本）
 * @param [out] dst output L1 pointer (__cbuf__ T*)
 * @param [in] src input GM pointer (__gm__ T*)
 */
template <typename T>
__aicore__ inline __inout_pipe__(MTE2) void DataCopy(
    __cbuf__ T* dst, __gm__ T* src, const DataCopyParams& repeatParams);
#endif

/*
 * @ingroup DataCopy Level 0
 * @brief format transform(such as nd2nz) during data load from OUT to L1（GM -> L1 指针入参版本）
 * @par 说明：与 GM -> UB Nd2Nz 版仅地址空间限定不同，且 enableSmallC0 不参与函数形参，
 *           宿主机侧二者均塌缩为 DataCopy(T*, T*, const Nd2NzParams&) 且无法偏序，调用二义；
 *           故本重载为设备编译专用。
 */
#if (__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102) || (__NPU_ARCH__ == 3113) || (__NPU_ARCH__ == 5101) || \
    (__NPU_ARCH__ == 5161) || (__NPU_ARCH__ == 5165) || (__NPU_ARCH__ == 5163)
#if defined(ASCENDC_PTR_AS_REAL)
template <typename T, bool enableSmallC0 = false>
__aicore__ inline __inout_pipe__(MTE2) void DataCopy(__cbuf__ T* dst, __gm__ T* src, const Nd2NzParams& intriParams);
#endif
#endif

/*
 * @ingroup DataCopy Level 0
 * @brief format transform(such as dn2nz) during data load from OUT to L1（GM -> L1 指针入参版本）
 */
#if (__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102) || (__NPU_ARCH__ == 5101) || (__NPU_ARCH__ == 5161) || \
    (__NPU_ARCH__ == 5165) || (__NPU_ARCH__ == 5163)
template <typename T, bool enableSmallC0 = false>
__aicore__ inline __inout_pipe__(MTE2) void DataCopy(__cbuf__ T* dst, __gm__ T* src, const Dn2NzParams& intriParams);
#endif

#if defined(ASCENDC_PTR_AS_REAL)
/*
 * @ingroup DataCopy
 * @brief datacopy from src to dst, applicable to cube data（GM -> L1 指针入参版本）
 */
template <typename T>
__aicore__ inline __inout_pipe__(MTE2) void DataCopy(
    __cbuf__ T* dst, __gm__ T* src, const DataCopyParams& intriParams, const DataCopyEnhancedParams& enhancedParams);

/*
 * @ingroup DataCopy Level 0
 * @brief datacopy from src to dst, applicable to vector data（UB -> GM 指针入参版本）
 * @param [out] dst output GM pointer (__gm__ T*)
 * @param [in] src input UB pointer (__ubuf__ T*)
 */
template <typename T>
__aicore__ inline __inout_pipe__(MTE3) void DataCopy(
    __gm__ T* dst, __ubuf__ T* src, const DataCopyParams& repeatParams);

/*
 * @ingroup DataCopy Level 2
 * @brief datacopy from src to dst（UB -> GM 指针入参版本）
 */
template <typename T>
__aicore__ inline __inout_pipe__(MTE3) void DataCopy(__gm__ T* dst, __ubuf__ T* src, const uint32_t count);

/*
 * @ingroup DataCopy Level 1
 * @brief datacopy from src to dst（UB -> GM SliceInfo 指针入参版本）
 */
template <typename T>
__aicore__ inline __inout_pipe__(MTE3) void DataCopy(
    __gm__ T* dst, __ubuf__ T* src, const SliceInfo dstSliceInfo[], const SliceInfo srcSliceInfo[],
    const uint32_t dimValue = 1);
#endif

/*
 * @ingroup DataCopy Level 2
 * @brief datacopy from src to dst, nz2nd, applicable to simulated cube data（UB -> GM 指针入参版本）
 */
template <typename T>
__aicore__ inline __inout_pipe__(MTE3) void DataCopy(
    __gm__ T* dst, __ubuf__ T* src, const Nz2NdParamsFull& intriParams);

#if defined(ASCENDC_PTR_AS_REAL)
/*
 * @ingroup DataCopy
 * @brief datacopy from src to dst, applicable to cube data（UB -> GM 指针入参版本）
 */
template <typename T>
__aicore__ inline __inout_pipe__(MTE3) void DataCopy(
    __gm__ T* dst, __ubuf__ T* src, const DataCopyParams& intriParams, const DataCopyEnhancedParams& enhancedParams);

/*
 * @ingroup DataCopy Level 0
 * @brief datacopy from src to dst, applicable to vector data（L1 -> GM 指针入参版本）
 * @param [out] dst output GM pointer (__gm__ T*)
 * @param [in] src input L1 pointer (__cbuf__ T*)
 */
template <typename T>
__aicore__ inline __inout_pipe__(MTE3) void DataCopy(
    __gm__ T* dst, __cbuf__ T* src, const DataCopyParams& repeatParams);

/*
 * @ingroup DataCopy Level 0
 * @brief datacopy from src to dst, applicable to vector data（UB -> L1 指针入参版本）
 * @param [out] dst output L1 pointer (__cbuf__ T*)
 * @param [in] src input UB pointer (__ubuf__ T*)
 */
template <typename T>
__aicore__ inline void DataCopy(__cbuf__ T* dst, __ubuf__ T* src, const DataCopyParams& repeatParams);

/*
 * @ingroup DataCopy Level 2
 * @brief datacopy from src to dst（UB -> L1 指针入参版本）
 */
template <typename T>
__aicore__ inline void DataCopy(__cbuf__ T* dst, __ubuf__ T* src, const uint32_t count);

/*
 * @ingroup DataCopy Level 0
 * @brief format transform(such as nd2nz) during data load from UB to L1（UB -> L1 指针入参版本）
 */
template <typename T>
__aicore__ inline void DataCopy(__cbuf__ T* dst, __ubuf__ T* src, const Nd2NzParams& intriParams);
#endif

/*
 * @ingroup DataCopy Level 0
 * @brief datacopy from src to dst, applicable to vector data（UB -> UB 指针入参版本）
 * @param [out] dst output UB pointer (__ubuf__ T*)
 * @param [in] src input UB pointer (__ubuf__ T*)
 */
#if defined(ASCENDC_PTR_AS_REAL)
template <typename T>
__aicore__ inline void DataCopy(__ubuf__ T* dst, __ubuf__ T* src, const DataCopyParams& repeatParams);

/*
 * @ingroup DataCopy Level 2
 * @brief datacopy from src to dst（UB -> UB 指针入参版本）
 */
template <typename T>
__aicore__ inline void DataCopy(__ubuf__ T* dst, __ubuf__ T* src, const uint32_t count);
#endif

/*
 * @ingroup DataCopy Level 0
 * @brief fixpipe datacopy from L0C to GM（L0C -> GM 指针入参版本）
 * @param [out] dst output GM pointer (__gm__ T*)
 * @param [in] src input L0C pointer (__cc__ U*)
 */
template <typename T, typename U>
__aicore__ inline void DataCopy(__gm__ T* dst, __cc__ U* src, const DataCopyCO12DstParams& intriParams);

#if defined(ASCENDC_PTR_AS_REAL)
/*
 * @ingroup DataCopy Level 0
 * @brief datacopy from L0C to L1（L0C -> L1 指针入参版本）
 * @param [out] dst output L1 pointer (__cbuf__ T*)
 * @param [in] src input L0C pointer (__cc__ U*)
 */
template <typename T, typename U>
__aicore__ inline void DataCopy(__cbuf__ T* dst, __cc__ U* src, const DataCopyCO12DstParams& intriParams);
#endif

/*
 * @ingroup DataCopy Level 0
 * @brief datacopy from L1 to BiasTable C2（C1 -> C2 指针入参版本）
 * @param [out] dst output BiasTable C2 pointer，地址取自 C2 位置 Tensor（以 __cbuf__ 地址域表示）
 * @param [in] src input L1 pointer (__cbuf__ U*)
 * @param [in] repeatParams.blockLen 单位：Ascend 950 系列为 32B，其余型号为 64B
 */
template <typename T, typename U>
__aicore__ inline void DataCopy(__cbuf__ T* dst, __cbuf__ U* src, const DataCopyParams& repeatParams);

/*
 * @ingroup DataCopy Level 0
 * @brief datacopy from L1 to Fixpipe Buffer C2PIPE2GM（C1 -> C2PIPE2GM 指针入参版本）
 * @param [out] dst output fixpipe buffer pointer (__fbuf__ T*)
 * @param [in] src input L1 pointer (__cbuf__ T*)
 * @param [in] repeatParams.blockLen 单位：128B（与 Tensor 入参版本一致，底层按 64B 粒度下发）
 */
#if defined(__NPU_ARCH__) &&                                                                                 \
    ((__NPU_ARCH__ == 2201) || (__NPU_ARCH__ == 3002) || (__NPU_ARCH__ == 3102) || (__NPU_ARCH__ == 3510) || \
     (__NPU_ARCH__ == 5102) || (__NPU_ARCH__ == 3003) || (__NPU_ARCH__ == 3113) || (__NPU_ARCH__ == 5161) || \
     (__NPU_ARCH__ == 5165) || (__NPU_ARCH__ == 5163))
#if defined(ASCENDC_PTR_AS_REAL)
template <typename T>
__aicore__ inline void DataCopy(__fbuf__ T* dst, __cbuf__ T* src, const DataCopyParams& repeatParams);
#endif
#endif

/* **************************************************************************************************
 * DataCopyPad 指针化扩展（Pointer 输入）                                       *
 * ************************************************************************************************* */
/*
 * @ingroup DataCopyPad
 * @brief datacopy with padding from src to dst（GM -> UB 指针入参版本）
 */
#if (__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102)
template <typename T, PaddingMode mode = PaddingMode::Normal>
__aicore__ inline __inout_pipe__(MTE2) void DataCopyPad(
    __ubuf__ T* dst, __gm__ T* src, const DataCopyParams& dataCopyParams, const DataCopyPadParams& padParams);
#else
template <typename T>
__aicore__ inline __inout_pipe__(MTE2) void DataCopyPad(
    __ubuf__ T* dst, __gm__ T* src, const DataCopyParams& dataCopyParams, const DataCopyPadParams& padParams);
#endif

/*
 * @ingroup DataCopyPad
 * @brief datacopy with padding from src to dst（GM -> L1 指针入参版本）
 */
#if (__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102)
#if defined(ASCENDC_PTR_AS_REAL)
template <typename T, PaddingMode mode = PaddingMode::Normal>
__aicore__ inline __inout_pipe__(MTE2) void DataCopyPad(
    __cbuf__ T* dst, __gm__ T* src, const DataCopyParams& dataCopyParams, const DataCopyPadParams& padParams);
#endif
#endif

/*
 * @ingroup DataCopyPad
 * @brief datacopy with padding from src to dst（UB -> GM 指针入参版本）
 */
#if (__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102)
template <typename T, PaddingMode mode = PaddingMode::Normal>
__aicore__ inline __inout_pipe__(MTE3) void DataCopyPad(
    __gm__ T* dst, __ubuf__ T* src, const DataCopyParams& dataCopyParams);
#else
template <typename T>
__aicore__ inline __inout_pipe__(MTE3) void DataCopyPad(
    __gm__ T* dst, __ubuf__ T* src, const DataCopyParams& dataCopyParams);
#endif

/*
 * @ingroup DataCopyPad
 * @brief override DataCopyPad, use new param DataCopyExtParams（GM -> UB 指针入参版本）
 */
#if (__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102) || (__NPU_ARCH__ == 5101) || (__NPU_ARCH__ == 5161) || \
    (__NPU_ARCH__ == 5165) || (__NPU_ARCH__ == 5163)
template <typename T, PaddingMode mode = PaddingMode::Normal>
__aicore__ inline __inout_pipe__(MTE2) void DataCopyPad(
    __ubuf__ T* dst, __gm__ T* src, const DataCopyExtParams& dataCopyParams, const DataCopyPadExtParams<T>& padParams);
#else
template <typename T>
__aicore__ inline __inout_pipe__(MTE2) void DataCopyPad(
    __ubuf__ T* dst, __gm__ T* src, const DataCopyExtParams& dataCopyParams, const DataCopyPadExtParams<T>& padParams);
#endif

/*
 * @ingroup DataCopyPad
 * @brief override DataCopyPad, use new param DataCopyExtParams（UB -> GM 指针入参版本）
 */
#if (__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102) || (__NPU_ARCH__ == 5101) || (__NPU_ARCH__ == 5161) || \
    (__NPU_ARCH__ == 5165) || (__NPU_ARCH__ == 5163)
template <typename T, PaddingMode mode = PaddingMode::Normal>
__aicore__ inline __inout_pipe__(MTE3) void DataCopyPad(
    __gm__ T* dst, __ubuf__ T* src, const DataCopyExtParams& dataCopyParams);
#else
template <typename T>
__aicore__ inline __inout_pipe__(MTE3) void DataCopyPad(
    __gm__ T* dst, __ubuf__ T* src, const DataCopyExtParams& dataCopyParams);
#endif

/*
 * @ingroup DataCopyPad
 * @brief datacopy with padding and format transform(such as nd2nz) from UB to L1（UB -> L1 指针入参版本）
 */
template <typename T>
__aicore__ inline void DataCopyPad(
    __cbuf__ T* dst, __ubuf__ T* src, const DataCopyParams& dataCopyParams, const Nd2NzParams& nd2nzParams);

/*
 * @ingroup DataCopyPad
 * @brief override DataCopyPad with Nd2NzParams, use new param DataCopyExtParams（UB -> L1 指针入参版本）
 */
template <typename T>
__aicore__ inline void DataCopyPad(
    __cbuf__ T* dst, __ubuf__ T* src, const DataCopyExtParams& dataCopyParams, const Nd2NzParams& nd2nzParams);

/* **************************************************************************************************
 * DataCopyL1ToUB 指针化扩展（Pointer 输入）                                    *
 * ************************************************************************************************* */
/*
 * @ingroup DataCopyL1ToUB
 * @brief datacopy from L1 to UB（L1 -> UB 指针入参版本）
 * @param [out] dst output UB pointer (__ubuf__ T*)
 * @param [in] src input L1 pointer (__cbuf__ T*)
 */
template <typename T, uint8_t subBlockId = 0>
__aicore__ inline void DataCopyL1ToUB(__ubuf__ T* dst, __cbuf__ T* src, const uint32_t count);

template <typename T, uint8_t subBlockId = 0>
__aicore__ inline void DataCopyL1ToUB(__ubuf__ T* dst, __cbuf__ T* src, const DataCopyParams& repeatParams);

/* **************************************************************************************************
 * Copy 指针化扩展（Pointer 输入）                                          *
 * ************************************************************************************************* */
/*
 * @ingroup Copy Level 0
 * @brief datacopy from src to dst, applicable to vector data（UB -> UB 指针入参版本）
 */
// Copy::Level 0 - mask bit mode
template <typename T, bool isSetMask = true>
__ASC_USE_RESERVED_UBUF__(3510, "Copy is forbidden when compile option --cce-disable-asc-reserved-ubuf is enabled")
__aicore__ inline __inout_pipe__(V) void Copy(
    __ubuf__ T* dst, __ubuf__ T* src, const uint64_t mask[], const uint8_t repeatTime,
    const CopyRepeatParams& repeatParams);

// Copy::Level 0 - mask count mode
template <typename T, bool isSetMask = true>
__ASC_USE_RESERVED_UBUF__(3510, "Copy is forbidden when compile option --cce-disable-asc-reserved-ubuf is enabled")
__aicore__ inline __inout_pipe__(V) void Copy(
    __ubuf__ T* dst, __ubuf__ T* src, const uint64_t mask, const uint8_t repeatTime,
    const CopyRepeatParams& repeatParams);

#if (__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102)
// Copy::Level 2 - count mode
template <typename T, bool isSetMask = true>
__aicore__ inline __inout_pipe__(V) void Copy(__ubuf__ T* dst, __ubuf__ T* src, const uint32_t count);
#endif
} // namespace AscendC

#if defined(__NPU_ARCH__)
#include "../../impl/basic_api/kernel_operator_data_copy_ptr_impl.h"
#endif
#endif // ASCENDC_MODULE_OPERATOR_DATA_COPY_PTR_INTF_H
#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_DATA_COPY_PTR_INTF_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_DATA_COPY_PTR_INTF_H__
#endif
