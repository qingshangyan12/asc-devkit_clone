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
 * \file kernel_operator_proposal_intf.h
 * \brief
 */

#if defined(__NPU_COMPILER_INTERNAL_PURE_SIMT__)
#error "kernel_operator_proposal_intf.h cannot be used with compile flag --enable-simt enabled."
#endif

#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_PROPOSAL_INTF_H__
#endif

#ifndef ASCENDC_MODULE_OPERATOR_PROPOSAL_INTERFACE_H
#define ASCENDC_MODULE_OPERATOR_PROPOSAL_INTERFACE_H

#include "../../impl/basic_api/kernel_macros.h"
#include "kernel_tensor.h"
#include "kernel_struct_proposal.h"

#if defined(ASCENDC_CPU_DEBUG) && ASCENDC_CPU_DEBUG == 1
#include <cstdint>
#include "stub_def.h"
#endif

namespace AscendC {
#pragma begin_pipe(V)
/* **************************************** MrgSort4 ****************************************** */
/*
 * @ingroup MrgSort4
 * @brief Arrange and merge up to four arranged potential queues into one queue
 * @param [out] dst output LocalTensor
 * @param [in] src input LocalTensor list
 * @param [in] filter input LocalTensor
 * @param [in] Params.elementLengths length of proposal list
 * @param [in] Params.ifExhaustedSuspension judge whether to stop after a queue is exhausted
 * @param [in] Params.validBit judge value is valid or not
 * @param [in] Params.repeatTimes repeat times
 */
template <typename T>
__aicore__ inline void MrgSort4(const LocalTensor<T>& dst, const MrgSortSrcList<T>& src, const MrgSort4Info& params);

/* **************************************** RpSort16 ****************************************** */
/*
 * @ingroup RpSort16
 * @brief Sort them according to the score field in the Region Proposals
 * @param [out] dst output LocalTensor
 * @param [in] src input LocalTensor
 * @param [in] repeatTime repeat times
 */
template <typename T>
/**
 * @ingroup simd_api.basic_api.memory_vector_compute.sort_and_merge_isasi
 * @override_id RpSort16__RCltg1x2_pT_RCltg1x2_pT_i32
 * @archs 2002 1001
 * @brief 根据Region Proposals中的score域对其进行排序（score大的排前面），每次排16个Region Proposals。
 */
__aicore__ inline void RpSort16(const LocalTensor<T>& dst, const LocalTensor<T>& src, const int32_t repeatTime);

/* **************************************** MrgSort ****************************************** */
/*
 * @ingroup MrgSort
 * @brief Arrange and merge up to four arranged potential queues into one queue
 * @param [out] dst output LocalTensor
 * @param [in] src input LocalTensor list
 * @param [in] filter input LocalTensor
 * @param [in] Params.elementLengths length of proposal list
 * @param [in] Params.ifExhaustedSuspension judge whether to stop after a queue is exhausted
 * @param [in] Params.validBit judge value is valid or not
 * @param [in] Params.repeatTimes repeat times
 */
template <typename T>
__aicore__ inline void MrgSort(const LocalTensor<T>& dst, const MrgSortSrcList<T>& src, const MrgSort4Info& params);

/* **************************************** Sort32 ****************************************** */
/*
 * @ingroup Sort32
 * @brief Sort 32 elements
 * @param [out] dst output LocalTensor
 * @param [in] src0 input LocalTensor
 * @param [in] src1 input LocalTensor
 * @param [in] repeatTime repeat times
 */
template <typename T>
/**
 * @ingroup simd_api.basic_api.memory_vector_compute.sort_and_merge_isasi
 * @override_id Sort32__RCltg1x2_pT_RCltg1x2_pT_RCltg1x3_u32_i32
 * @archs 3510 2201 3002
 * @brief Sort32接口实现一次迭代内对32个数的降序排列操作。
 */
__aicore__ inline void Sort32(
    const LocalTensor<T>& dst, const LocalTensor<T>& src0, const LocalTensor<uint32_t>& src1, const int32_t repeatTime);

/* **************************************** ProposalConcat ****************************************** */
/*
 * @ingroup ProposalConcat
 * @brief Combine continuous elements into corresponding positions in the Region Proposal
 * @param [out] dst output LocalTensor
 * @param [in] src input LocalTensor
 * @param [in] repeatTime repeat times
 * @param [in] modeNumbe Position parameter
 */
template <typename T>
/**
 * @ingroup simd_api.basic_api.memory_vector_compute.sort_and_merge_isasi
 * @override_id ProposalConcat__RCltg1x2_pT_RCltg1x2_pT_i32_i32
 * @archs 2002 1001
 * @brief 将连续元素合入Region Proposal内对应位置，每次迭代会将16个连续元素合入到16个Region Proposals的对应位置里。
 */
__aicore__ inline void ProposalConcat(
    const LocalTensor<T>& dst, const LocalTensor<T>& src, const int32_t repeatTime, const int32_t modeNumber);

/* **************************************** ProposalExtract ****************************************** */
/*
 * @ingroup ProposalExtract
 * @brief ProposalExtract and rearrange the individual elements in the corresponding position from the Region Proposals
 * @param [out] dst output LocalTensor
 * @param [in] src input LocalTensor
 * @param [in] repeatTime repeat times
 * @param [in] modeNumbe Position parameter
 */
template <typename T>
/**
 * @ingroup simd_api.basic_api.memory_vector_compute.sort_and_merge_isasi
 * @override_id ProposalExtract__RCltg1x2_pT_RCltg1x2_pT_i32_i32
 * @archs 2002 1001
 * @brief 与ProposalConcat功能相反，从Region Proposals内将相应位置的单个元素抽取后重排，每次迭代处理16个Region Proposals，抽取16个元素后连续排列。
 */
__aicore__ inline void ProposalExtract(
    const LocalTensor<T>& dst, const LocalTensor<T>& src, const int32_t repeatTime, const int32_t modeNumber);

/* **************************************** Concat ****************************************** */
/*
 * @ingroup Concat
 * @brief Combine continuous elements into corresponding positions
 * @param [out] concat output LocalTensor
 * @param [in] src input LocalTensor
 * @param [in] tmp tmp buffer
 * @param [in] repeatTime repeat times
 */
template <typename T>
/**
 * @ingroup simd_api.adv_api.sort_operations
 * @override_id Concat__Rltg1x2_pT_RCltg1x2_pT_RCltg1x2_pT_i32
 * @archs 3510 2201 2002
 * @brief 对数据进行预处理，将要排序的源操作数src一一对应的合入目标数据concat中，数据预处理完成后，可以进行[Sort](Sort.md)。
 * @param [out] concat 目的操作数。类型为[LocalTensor](../../basic_api/data_structures/LocalTensor/LocalTensor.md)，支持的TPosition为VECIN/VECCALC/VECOUT。LocalTensor的起始地址需要32字节对齐。
 * @param [in] src 源操作数。类型为[LocalTensor](../../basic_api/data_structures/LocalTensor/LocalTensor.md)，支持的TPosition为VECIN/VECCALC/VECOUT。LocalTensor的起始地址需要32字节对齐。源操作数的数据类型需要与目的操作数保持一致。
 * @param [in] tmp 临时空间。接口内部复杂计算时用于存储中间变量，由开发者提供，临时空间大小的获取方式请参考[GetConcatTmpSize](GetConcatTmpSize.md)。数据类型与源操作数保持一致。类型为[LocalTensor](../../basic_api/data_structures/LocalTensor/LocalTensor.md)，支持的TPosition为VECIN/VECCALC/VECOUT。LocalTensor的起始地址需要32字节对齐。
 * @param [in] repeatTime 重复迭代次数，int32_t类型，每次迭代处理16个元素，下次迭代跳至相邻的下一组16个元素。取值范围：repeatTime∈[0,255]。
 */
__aicore__ inline void Concat(
    LocalTensor<T>& concat, const LocalTensor<T>& src, const LocalTensor<T>& tmp, const int32_t repeatTime);

/* **************************************** Extract ****************************************** */
/*
 * @ingroup Extract
 * @brief Extract and rearrange the individual elements in the corresponding position
 * @param [out] dstValue output LocalTensor
 * @param [in] dstIndex output LocalTensor
 * @param [in] sorted input LocalTensor
 * @param [in] repeatTime repeat times
 */
template <typename T>
/**
 * @ingroup simd_api.adv_api.sort_operations
 * @override_id Extract__RCltg1x2_pT_RCltg1x3_u32_RCltg1x2_pT_i32
 * @archs 3510 2201 2002
 * @brief 处理Sort的结果数据，输出排序后的value和index。
 * @param [out] dstValue 目的操作数。类型为[LocalTensor](../../basic_api/data_structures/LocalTensor/LocalTensor.md)，支持的TPosition为VECIN/VECCALC/VECOUT。LocalTensor的起始地址需要32字节对齐。
 * @param [out] dstIndex 目的操作数。类型为[LocalTensor](../../basic_api/data_structures/LocalTensor/LocalTensor.md)，支持的TPosition为VECIN/VECCALC/VECOUT。LocalTensor的起始地址需要32字节对齐。此源操作数固定为uint32_t数据类型。
 * @param [in] sorted 源操作数。类型为[LocalTensor](../../basic_api/data_structures/LocalTensor/LocalTensor.md)，支持的TPosition为VECIN/VECCALC/VECOUT。LocalTensor的起始地址需要32字节对齐。源操作数的数据类型需要与目的操作数保持一致。
 * @param [in] repeatTime 重复迭代次数，int32_t类型。
 */
__aicore__ inline void Extract(
    const LocalTensor<T>& dstValue, const LocalTensor<uint32_t>& dstIndex, const LocalTensor<T>& sorted,
    const int32_t repeatTime);

/* **************************************** MrgSort ****************************************** */
/*
 * @ingroup MrgSort
 * @brief Arrange and merge up to four arranged potential queues into one queue
 * @param [out] dst output LocalTensor
 * @param [in] sortList input LocalTensor list
 * @param [in] elementCountList input LocalTensor list length
 * @param [in] sortedNum output sorted numbers
 * @param [in] validBit input valid bit
 * @param [in] repeatTime repeat times
 */
template <typename T, bool isExhaustedSuspension = false>
__aicore__ inline void MrgSort(
    const LocalTensor<T>& dst, const MrgSortSrcList<T>& sortList, const uint16_t elementCountList[4],
    uint32_t sortedNum[4], uint16_t validBit, const int32_t repeatTime);

/* ***************************************** Sort ****************************************** */
/*
 * @ingroup Sort
 * @brief Sort them according to the value
 * @param [out] dst output LocalTensor
 * @param [in] concat input LocalTensor
 * @param [in] index input LocalTensor
 * @param [in] tmp tmp buffer
 * @param [in] repeatTime repeat times
 */
template <typename T, bool isFullSort>
/**
 * @ingroup simd_api.adv_api.sort_operations
 * @override_id Sort__RCltg1x2_pT_RCltg1x2_pT_RCltg1x3_u32_Rltg1x2_pT_i32
 * @archs 3510 2201 2002 x90 9030
 * @brief 排序函数，按照数值大小进行降序排序。
 */
__aicore__ inline void Sort(
    const LocalTensor<T>& dst, const LocalTensor<T>& concat, const LocalTensor<uint32_t>& index, LocalTensor<T>& tmp,
    const int32_t repeatTime);

/* **************************************** GetSortOffset ****************************************** */
/*
 * @ingroup GetSortOffset
 * @brief get sort offset in the sorted struct
 * @param [in] elemOffset element number offer
 */
template <typename T>
/**
 * @ingroup simd_api.adv_api.sort_operations
 * @override_id GetSortOffset__u32
 * @brief 根据元素位置，获取Sort数据中的对应偏移量（单位为字节）。
 * @param [in] elemOffset 元素的位置。
 */
__aicore__ inline uint32_t GetSortOffset(const uint32_t elemOffset);

/* **************************************** GetSortLen ****************************************** */
/*
 * @ingroup GetSortLen
 * @brief get sort length in the sorted struct
 * @param [in] elemOffset element number ocountffer
 */
template <typename T>
/**
 * @ingroup simd_api.adv_api.sort_operations
 * @override_id GetSortLen__u32
 * @brief 根据元素数量，获取Sort数据的大小（单位为字节）。
 * @param [in] elemCount 输入元素个数。
 */
__aicore__ inline uint32_t GetSortLen(const uint32_t elemCount);
#pragma end_pipe
__aicore__ inline __inout_pipe__(S) void GetMrgSortResult(
    uint16_t& mrgSortList1, uint16_t& mrgSortList2, uint16_t& mrgSortList3, uint16_t& mrgSortList4);
} // namespace AscendC

#if defined(__NPU_ARCH__)
#include "../../impl/basic_api/kernel_operator_proposal_intf_impl.h"
#endif // ASCENDC_MODULE_OPERATOR_PROPOSAL_INTERFACE_H
#endif

#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_PROPOSAL_INTF_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_PROPOSAL_INTF_H__
#endif
