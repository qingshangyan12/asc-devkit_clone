/*
* Copyright (c) 2025 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/

/*!
 *\file kernel_operator_mm_bitmode_impl.h
 *\brief
*/

#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#pragma message("impl/basic_api/dav_3510/kernel_operator_mm_bitmode_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file may be removed in the future. Please use \"#include \"basic_api/kernel_operator_intf.h\"\" and use public functions or variables defined in interface headers files.")
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_MM_BITMODE_IMPL_H__
#endif
#ifndef ASCENDC_MODULE_OPERATOR_MM_BITMODE_IMPL_H
#define ASCENDC_MODULE_OPERATOR_MM_BITMODE_IMPL_H
#include "kernel_struct_mm.h"

namespace AscendC {

struct MmadBitModeConfig0 {
    uint64_t m:12;
    uint64_t k:12;
    uint64_t n:12;
    uint64_t :19;
    uint64_t unitFlag:2;
    uint64_t :4;
    uint64_t disableGemv:1;
    uint64_t cmatrixSource:1;
    uint64_t cmatrixInitVal:1;

    __aicore__ inline void Init(const MmadParams& mmadParams_) {
        m = mmadParams_.m;
        k = mmadParams_.k;
        n = mmadParams_.n;
        unitFlag = mmadParams_.unitFlag;
        disableGemv = mmadParams_.disableGemv;
        cmatrixSource = mmadParams_.cmatrixSource;
        cmatrixInitVal = mmadParams_.cmatrixInitVal;
    }
    __aicore__ inline void SetM(uint16_t m_) {
        m = m_;
    };
    __aicore__ inline void SetK(uint16_t k_) {
        k = k_;
    };
    __aicore__ inline void SetN(uint16_t n_) {
        n = n_;
    };
    __aicore__ inline void SetUnitFlag(uint8_t unitFlag_) {
        unitFlag = unitFlag_;
    };
    __aicore__ inline void SetDisableGemv(bool disableGemv_) {
        disableGemv = disableGemv_;
    };
    __aicore__ inline void SetCmatrixSource(bool cmatrixSource_) {
        cmatrixSource = cmatrixSource_;
    };
    __aicore__ inline void SetCmatrixInitVal(bool cmatrixInitVal_) {
        cmatrixInitVal = cmatrixInitVal_;
    };
    __aicore__ inline uint16_t GetM() const {
        return m;
    };
    __aicore__ inline uint16_t GetK() const {
        return k;
    };
    __aicore__ inline uint16_t GetN() const {
        return n;
    };
    __aicore__ inline uint8_t GetUnitFlag() const {
        return unitFlag;
    };
    __aicore__ inline bool GetDisableGemv() const {
        return disableGemv;
    };
    __aicore__ inline bool GetCmatrixSource() const {
        return cmatrixSource;
    };
    __aicore__ inline bool GetCmatrixInitVal() const {
        return cmatrixInitVal;
    };
};
}  // namespace AscendC

#endif  // ASCENDC_MODULE_OPERATOR_MM_BITMODE_IMPL_H
#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_MM_BITMODE_IMPL_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_MM_BITMODE_IMPL_H__
#endif
