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
 * \file kernel_operator_proposal_impl.h
 * \brief
 */
#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#pragma message( \
    "impl/basic_api/dav_5161/kernel_operator_proposal_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file may be removed in the future. Please use \"#include \"basic_api/kernel_tensor.h\"\" and use public functions or variables defined in interface headers files.")
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_PROPOSAL_IMPL_H__
#endif
#ifndef ASCENDC_MODULE_OPERATOR_PROPOSAL_IMPL_H
#define ASCENDC_MODULE_OPERATOR_PROPOSAL_IMPL_H

#include "../kernel_log.h"
#include "../kernel_macros.h"
#include "../../../include/basic_api/kernel_tensor.h"
#include "../../../include/basic_api/kernel_operator_block_sync_intf.h"
#include "../../../include/basic_api/kernel_struct_proposal.h"
#include "../utils/kernel_utils_ceil_oom_que.h"
#include "../utils/kernel_utils_constants.h"
#include "../utils/kernel_utils_struct_param.h"

#if defined(ASCENDC_CPU_DEBUG) && ASCENDC_CPU_DEBUG == 1
#include <cstdint>
#include "stub_def.h"
#include "stub_fun.h"
#endif // ASCENDC_CPU_DEBUG

namespace AscendC {
constexpr uint32_t singleSortElementCountArch5161 = 32;
constexpr uint32_t regionProposalDataSize = 8;
template <typename T>
[[deprecated("NOTICE: MrgSort4 is not deprecated. Currently, MrgSort4 is an unsupported API on current device."
             "Please check your code!")]] __aicore__ inline void
Vmrgsort4Cal(__ubuf__ T* dstLocal, __ubuf__ T* addrArray[MRG_SORT_ELEMENT_LEN], uint64_t config)
{
    ASCENDC_REPORT_NOT_SUPPORT(false, "MrgSort4");
}

template <typename T>
[[deprecated("NOTICE: RpSort16 is not deprecated. Currently, RpSort16 is an unsupported API on current device."
             "Please check your code!")]] __aicore__ inline void
VbitsortCal(__ubuf__ T* dstLocal, __ubuf__ T* srcLocal, const ProposalIntriParams& intriParams)
{
    ASCENDC_REPORT_NOT_SUPPORT(false, "RpSort16");
}

template <typename T>
__aicore__ inline void VbitsortCal(
    __ubuf__ T* dstLocal, __ubuf__ T* src0Local, __ubuf__ uint32_t* src1Local, const ProposalIntriParams& intriParams)
{
    static_assert(SupportType<T, float, half>(), "current data type is not supported on current device");
    uint64_t config = static_cast<uint64_t>(intriParams.repeat) << 56;
    vbs(dstLocal, src0Local, src1Local, config);
}

template <typename T>
__aicore__ inline void Vmrgsort4Cal(
    __ubuf__ T* dstLocal, __ubuf__ T* addrArray[MRG_SORT_ELEMENT_LEN], uint64_t src1, uint64_t config)
{
    vmrgsort4(dstLocal, addrArray, src1, config);
}

template <typename T>
__aicore__ inline void MrgSortCal(
    const LocalTensor<T>& dstLocal, const MrgSortSrcList<T>& sortList, const uint16_t elementCountList[4],
    uint32_t sortedNum[4], uint16_t validBit, const int32_t repeatTime)
{
    static_assert(SupportType<T, float, half>(), "MrgSort only support half / float.");
    MrgSort4Info mrgSortInfo(elementCountList, false, validBit, static_cast<uint16_t>(repeatTime));
#if ASCENDC_CPU_DEBUG
    if (!CheckFunProposal(dstLocal, sortList, mrgSortInfo, Internal::REGION_PROPOSAL_ELEMENT_NUM, "MrgSort")) {
        ASSERT(false && "check MrgSort instr failed");
    }
#endif
    uint64_t config = 0;
    config |= (mrgSortInfo.repeatTimes & 0xFF);
    config |= (uint64_t(mrgSortInfo.validBit & 0xF) << 8);
    config |= (uint64_t(mrgSortInfo.ifExhaustedSuspension & 0x1) << 12);

    uint64_t src1 = 0;
    src1 |= (uint64_t(mrgSortInfo.elementLengths[0] & 0xFFFF));
    src1 |= (uint64_t(mrgSortInfo.elementLengths[1] & 0xFFFF) << 16);
    src1 |= (uint64_t(mrgSortInfo.elementLengths[2] & 0xFFFF) << 32);
    src1 |= (uint64_t(mrgSortInfo.elementLengths[3] & 0xFFFF) << 48);

    __ubuf__ T* addrArray[MRG_SORT_ELEMENT_LEN] = {
        (__ubuf__ T*)sortList.src1.GetPhyAddr(), (__ubuf__ T*)sortList.src2.GetPhyAddr(),
        (__ubuf__ T*)sortList.src3.GetPhyAddr(), (__ubuf__ T*)sortList.src4.GetPhyAddr()};

    Vmrgsort4Cal((__ubuf__ T*)dstLocal.GetPhyAddr(), addrArray, src1, config);
}

__aicore__ inline void GetMrgSortResultImpl(
    uint16_t& mrgSortList1, uint16_t& mrgSortList2, uint16_t& mrgSortList3, uint16_t& mrgSortList4)
{
    int64_t mrgSortResult = get_vms4_sr();
    constexpr uint64_t resMask = 0xFFFF;
    // VMS4_SR[15:0], number of finished region proposals in list0
    mrgSortList1 = static_cast<uint64_t>(mrgSortResult) & resMask;
    constexpr uint64_t sortList2Bit = 16;
    // VMS4_SR[31:16], number of finished region proposals in list1
    mrgSortList2 = (static_cast<uint64_t>(mrgSortResult) >> sortList2Bit) & resMask;
    constexpr uint64_t sortList3Bit = 32;
    // VMS4_SR[47:32], number of finished region proposals in list2
    mrgSortList3 = (static_cast<uint64_t>(mrgSortResult) >> sortList3Bit) & resMask;
    constexpr uint64_t sortList4Bit = 48;
    // VMS4_SR[63:48], number of finished region proposals in list3
    mrgSortList4 = (static_cast<uint64_t>(mrgSortResult) >> sortList4Bit) & resMask;
}

} // namespace AscendC
#endif // ASCENDC_MODULE_OPERATOR_PROPOSAL_IMPL_H
#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_PROPOSAL_IMPL_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_PROPOSAL_IMPL_H__
#endif
