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
 * \file conv3d_bp_filter_intf.h
 * \brief
 */

#if defined(__NPU_COMPILER_INTERNAL_PURE_SIMT__)
#error "conv3d_bp_filter_intf.h cannot be used with compile flag --enable-simt enabled."
#endif

#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_CONV3D_BP_FILTER_INTF_H__
#endif
#ifndef CONV3D_BP_FILTER_INTF_H
#define CONV3D_BP_FILTER_INTF_H

#include "../../../impl/adv_api/detail/conv_backprop/conv3d_backprop_filter/conv3d_bp_filter_func.h"
#include "../../../impl/adv_api/detail/conv_backprop/common/conv3d_bp_util.h"
#include "common/conv3d_bp_config_base.h"

namespace ConvBackpropApi {
template <class Config_, template <typename, class> class Impl>
struct Conv3DBpFilterIntf {
    using Config = Config_;
    using Ext = Impl<Conv3DBpFilterIntf, Config>;
    using SrcT = typename Config::SrcT;
    using DstT = typename Config::DstT;
    using L0cT = typename Config::L0cT;
    using ContextData = typename Ext::ContextData;

public:
    ContextData ctx;

public:
    __aicore__ inline Conv3DBpFilterIntf() {}

    __aicore__ inline void Init(const TConv3DBpFilterTiling* __restrict tiling)
    {
        using Local = typename Ext::Init;
        if constexpr (CHECK_FUN(Local, ConvBackpropFilterFunc, this, tiling)) {
            Local::call(this, tiling);
        }
    }

    __aicore__ inline void SetInput(const AscendC::GlobalTensor<SrcT>& input)
    {
        using Local = typename Ext::SetInput;
        if constexpr (CHECK_FUN(Local, ConvBackpropFilterFunc, this, input)) {
            Local::call(this, input);
        }
    }

    __aicore__ inline void SetGradOutput(const AscendC::GlobalTensor<SrcT>& gradOutput)
    {
        using Local = typename Ext::SetGradOutput;
        if constexpr (CHECK_FUN(Local, ConvBackpropFilterFunc, this, gradOutput)) {
            Local::call(this, gradOutput);
        }
    }

/**
 * @ingroup simd_api.adv_api.convolution_compute.conv3dbackpropfilter_kernel
 * @override_id SetSingleShape__u64_u64_u64
 * @archs 2201
 * @brief 设置Conv3DBackpropFilter在单核上计算的形状。用于运行时修改shape。
 * @param [in] singleCoreM 单核上M的大小，单位为元素个数。
 * @param [in] singleCoreN 单核上N的大小，单位为元素个数。
 * @param [in] singleCoreK 单核上K的大小，单位为元素个数。
 */
    __aicore__ inline void SetSingleShape(uint64_t singleCoreM, uint64_t singleCoreN, uint64_t singleCoreK)
    {
        using Local = typename Ext::SetSingleShape;
        if constexpr (CHECK_FUN(Local, ConvBackpropFilterFunc, this, singleCoreM, singleCoreN, singleCoreK)) {
            Local::call(this, singleCoreM, singleCoreN, singleCoreK);
        }
    }

/**
 * @ingroup simd_api.adv_api.convolution_compute.conv3dbackpropfilter_kernel
 * @override_id SetStartPosition__u32
 * @archs 2201
 * @brief 设置单核上GradOutput载入数据的起始位置。
 * @param [in] hoStartIdx 当前核GradOutput Height方向起始位置。
 */
    __aicore__ inline void SetStartPosition(uint32_t hoStartIdx)
    {
        using Local = typename Ext::SetStartPosition;
        if constexpr (CHECK_FUN(Local, ConvBackpropFilterFunc, this, hoStartIdx)) {
            Local::call(this, hoStartIdx);
        }
    }

    template <bool sync = true>
/**
 * @ingroup simd_api.adv_api.convolution_compute.conv3dbackpropfilter_kernel
 * @override_id Iterate__b
 * @archs 2201
 * @brief 每次调用Iterate时，会计算出一个baseM \* baseN的结果矩阵，并将该结果写入L0C Buffer。接口内部会维护迭代进度，每次调用后，矩阵的起始地址将进行偏移。如果传入的数据未对齐存在尾块，则在最后一次迭代中输出尾块的计算结果。本接口需与[GetTensorC](GetTensorC.md)接口配合使用；在调用本接口后，再调用GetTensorC接口，L0C Buffer中的数据将被写入目标地址。
 * @param [in] enPartialSum 预留参数，用户无需感知。
 */
    __aicore__ inline bool Iterate(bool enPartialSum = false)
    {
        using Local = typename Ext::template Iterate<sync>;
        if constexpr (CHECK_FUN(Local, ConvBackpropFilterFunc, this, enPartialSum)) {
            return Local::call(this, enPartialSum);
        }
    }

    template <bool sync = true>
    __aicore__ inline void GetTensorC(
        const AscendC::GlobalTensor<DstT>& output, uint8_t enAtomic = 1, bool enSequentialWrite = false)
    {
        using Local = typename Ext::template GetTensorC<sync>;
        if constexpr (CHECK_FUN(Local, ConvBackpropFilterFunc, this, output, enAtomic, enSequentialWrite)) {
            Local::call(this, output, enAtomic, enSequentialWrite);
        }
    }

/**
 * @ingroup simd_api.adv_api.convolution_compute.conv3dbackpropfilter_kernel
 * @override_id End__z
 * @archs 2201
 * @brief 在Conv3DBackpropFilter卷积反向计算完成后，必须调用一次End，以清除EventID并释放内部申请的临时内存。
 */
    __aicore__ inline void End()
    {
        using Local = typename Ext::End;
        if constexpr (CHECK_FUN(Local, ConvBackpropFilterFunc, this)) {
            Local::call(this);
        }
    }
};

} // namespace ConvBackpropApi

#endif // CONV3D_BP_FILTER_CONFIG_H

#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_CONV3D_BP_FILTER_INTF_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_CONV3D_BP_FILTER_INTF_H__
#endif
