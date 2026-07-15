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
 * \file ain_impl_def.h
 * \brief Ain implementation definition
 */

#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#pragma message(                                                                                             \
    "impl/adv_api/detail/ain/impl/ain_impl_def.h is an internal header file and must not be used directly. " \
    "Please use public interface headers.")
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_AIN_IMPL_DEF_H__
#endif

#ifndef IMPL_ADV_API_DETAIL_AIN_IMPL_AIN_IMPL_DEF_H
#define IMPL_ADV_API_DETAIL_AIN_IMPL_AIN_IMPL_DEF_H

#include <cstddef>
#include <cstdint>
#include "hcomm_res.h"
#include "../../../../basic_api/kernel_utils.h"

typedef enum {
    COMM_MEM_TYPE_INVALID = -1,
    COMM_MEM_TYPE_DEVICE = 0,
    COMM_MEM_TYPE_HOST = 1,
} CommMemType;

typedef struct {
    CommMemType type;
    void *addr;
    uint64_t size;
} CommMem;

namespace hcomm {
typedef struct {
    CommAbiHeader header;
    uint64_t memsNum;
    CommMem *mems; // 长度是memberNum
    uint64_t reserved[6];
} HcommWindow;

typedef struct {
    HcommWindow window; // 内部实际是ComMem[merberNum], 如果memberId是selfMemberId，则是本地内存，否则是远端内存
    CommMem     shadowMem; // 申请一块(signalCount + counterCount + barrierCount) * sizeof(uint64_t) * memberNum的device侧内存
    uint32_t signalCount;   // 在创建team的时候，申请一块(signalCount + counterCount + barrierCount) * sizeof(uint64_t) * memberNum的device侧内存
    uint32_t counterCount;
    uint32_t barrierCount;
    uint64_t workspaceSize; // (signalCount + counterCount + barrierCount) * sizeof(uint64_t) * memberNum
    uint64_t reserved[6];
} HcommTeamWorkSpace;

typedef struct {
    CommAbiHeader header;
    CommEngine engine;             /* COMM_ENGINE_AICPU_TS / CCU 等 */
    uint32_t memberNum;
    uint32_t selfMemberId;
    uint64_t **channels;            // 2维数组，第一维长度是teamMemberNums，第二维长度是每个成员的channel数量channelNumPerMember
    uint64_t *channelNumPerMember;   //长度为memberNum，表示每个成员的channel数量
    uint32_t *worldTeamIds;
    HcommTeamWorkSpace workspace;
    uint64_t reserved[6];
} HcommTeam;

enum AinSignalType {
    AIN_SIGNAL_TYPE_NONE,
    AIN_SIGNAL_TYPE_VA,
    AIN_SIGNAL_TYPE_INDEXED,
};

} // namespace hcomm

namespace AscendC {

enum class SymmetricMemoryMode { HCCS = 0, URMA = 1 };

typedef struct {
    uint64_t signalOffset;
    AinCommSymWindow signalWindow = nullptr;
} AinSignalDescriptor;

typedef struct {
    AinSignal signalId;
} AinBarrierSignalInc;

}  // namespace AscendC

#endif  // IMPL_ADV_API_DETAIL_AIN_IMPL_AIN_IMPL_DEF_H

#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_AIN_IMPL_DEF_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_AIN_IMPL_DEF_H__
#endif
