/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef CCU_DATA_API_IMPL_H
#define CCU_DATA_API_IMPL_H

#ifdef __cplusplus
#include <cstdbool>
#else
#include <stdbool.h>
#endif

#include "hccl_types.h"
#include "ccu_types.h"
#include "hcomm_primitives.h"

#ifdef __cplusplus
extern "C" {
#endif

extern CcuResult CcuVariableAlloc(CcuVariableHandle *varHandle);
extern CcuResult CcuAddressAlloc(CcuAddressHandle *addrHandle);
extern CcuResult CcuEventAlloc(CcuEventHandle *eventHandle);
extern CcuResult CcuBufferAlloc(CcuBufferHandle *bufHandle);
extern CcuResult CcuLocalAddrAlloc(CcuLocalAddrHandle *localAddrHandle, CcuAddressHandle *addrHandle, CcuVariableHandle *tokenHandle);
extern CcuResult CcuRemoteAddrAlloc(CcuRemoteAddrHandle *remoteAddrHandle, CcuAddressHandle *addrHandle, CcuVariableHandle *tokenHandle);

extern CcuResult CcuBlockVariableAlloc(CcuVariableHandle *varHandles, uint32_t count);
extern CcuResult CcuBlockEventAlloc(CcuEventHandle *eventHandles, uint32_t count);
extern CcuResult CcuBlockBufferAlloc(CcuBufferHandle *bufHandles, uint32_t count);

extern CcuResult CcuVariableCreateByChannel(ChannelHandle channel, uint32_t varIndex, CcuVariableHandle *varHandle);

extern CcuResult CcuVariableAssignImm(CcuVariableHandle resVar, uint64_t immediate);
extern CcuResult CcuVariableAssignVar(CcuVariableHandle dstVarHandle, CcuVariableHandle srcVarHandle);
extern CcuResult CcuVariableAddVarToVar(CcuVariableHandle resVar, CcuVariableHandle varA, CcuVariableHandle varB);

extern CcuResult CcuAddressAssignImm(CcuAddressHandle addr, uint64_t immediate);
extern CcuResult CcuAddressAssignAddr(CcuAddressHandle dstAddrHandle, CcuAddressHandle srcAddrHandle);
extern CcuResult CcuAddressAssignVar(CcuAddressHandle addr, CcuVariableHandle var);
extern CcuResult CcuAddressAddVarToAddr(CcuAddressHandle resAddr, CcuAddressHandle lhsAddr, CcuVariableHandle rhsVar);
extern CcuResult CcuAddressAddAddrToAddr(CcuAddressHandle resAddr, CcuAddressHandle addrA, CcuAddressHandle addrB);
extern CcuResult CcuAddressAddAssignVar(CcuAddressHandle addr, CcuVariableHandle var);

extern CcuResult CcuLoadArg(CcuVariableHandle varHandle, uint32_t argId);
extern CcuResult CcuLoadVar(uint64_t addr, CcuVariableHandle varHandle, uint32_t num);
extern CcuResult CcuLoadVarFromVarAddr(CcuVariableHandle addrHandle, CcuVariableHandle varHandle, uint32_t num);
extern CcuResult CcuStoreVar(uint64_t addr, CcuVariableHandle varHandle, uint32_t num);
extern CcuResult CcuStoreVarToVarAddr(CcuVariableHandle addrHandle, CcuVariableHandle varHandle, uint32_t num);

extern CcuResult CcuEventRecord(CcuEventHandle eventHandle, uint16_t mask);
extern CcuResult CcuEventWait(CcuEventHandle eventHandle, uint16_t mask);
extern CcuResult CcuNotifyRecord(ChannelHandle channel, uint32_t remoteNotifyIdx, uint16_t mask);
extern CcuResult CcuNotifyWait(ChannelHandle channel, uint32_t localNotifyIdx, uint16_t mask);
extern CcuResult CcuWriteVariableWithNotify(ChannelHandle channel, CcuVariableHandle varHandle, uint32_t remoteVarIdx, uint32_t remoteNotifyIdx, uint16_t mask);
extern CcuResult CcuLocalNotifyRecord(const char *notifyTag, uint16_t mask);
extern CcuResult CcuLocalNotifyWait(const char *notifyTag, uint16_t mask);

extern CcuResult CcuLocalCopyMemToMem(CcuLocalAddrHandle dst, CcuLocalAddrHandle src, CcuVariableHandle len, CcuEventHandle event, uint16_t mask);
extern CcuResult CcuLocalCopyMemToBuffer(CcuBufferHandle dst, CcuLocalAddrHandle src, CcuVariableHandle len, CcuEventHandle event, uint16_t mask);
extern CcuResult CcuLocalCopyBufferToMem(CcuLocalAddrHandle dst, CcuBufferHandle src, CcuVariableHandle len, CcuEventHandle event, uint16_t mask);

extern CcuResult CcuLocalMemReduce(CcuLocalAddrHandle dst, CcuLocalAddrHandle src, CcuVariableHandle len, HcclDataType dataType, HcclReduceOp opType, CcuEventHandle event, uint16_t mask);
extern CcuResult CcuLocalBufferReduce(CcuBufferHandle* buffers, uint32_t count, HcclDataType dataType, HcclDataType outputDataType, HcclReduceOp opType, CcuVariableHandle len, CcuEventHandle event, uint16_t mask);

extern CcuResult CcuReadMemToMem(ChannelHandle channel, CcuLocalAddrHandle localHandle, CcuRemoteAddrHandle remoteHandle, CcuVariableHandle len, CcuEventHandle event, uint16_t mask);
extern CcuResult CcuReadMemToBuffer(ChannelHandle channel, CcuBufferHandle localHandle, CcuRemoteAddrHandle remoteHandle, CcuVariableHandle len, CcuEventHandle event, uint16_t mask);
extern CcuResult CcuReadMemToMemReduce(ChannelHandle channel, CcuLocalAddrHandle localHandle, CcuRemoteAddrHandle remoteHandle, CcuVariableHandle len, HcclDataType dataType, HcclReduceOp opType, CcuEventHandle event, uint16_t mask);
extern CcuResult CcuWriteMemToMem(ChannelHandle channel, CcuRemoteAddrHandle remoteHandle, CcuLocalAddrHandle localHandle, CcuVariableHandle len, CcuEventHandle event, uint16_t mask);
extern CcuResult CcuWriteBufferToMem(ChannelHandle channel, CcuRemoteAddrHandle remoteHandle, CcuBufferHandle localHandle, CcuVariableHandle len, CcuEventHandle event, uint16_t mask);
extern CcuResult CcuWriteMemToMemReduce(ChannelHandle channel, CcuRemoteAddrHandle remoteHandle, CcuLocalAddrHandle localHandle, CcuVariableHandle len, HcclDataType dataType, HcclReduceOp opType, CcuEventHandle event, uint16_t mask);

extern CcuResult CcuIfBegin(CcuVariableHandle var, uint64_t immediate, CcuConditionType condType, const char *label);
extern CcuResult CcuIfElse(const char *label);
extern CcuResult CcuIfEnd(const char *label);
extern CcuResult CcuFlushPendingIfs();
extern CcuResult CcuWhileBegin(CcuVariableHandle var, uint64_t immediate, CcuConditionType condType, const char *label);
extern CcuResult CcuWhileEnd(const char *label);
extern CcuResult CcuDoWhileBegin(const char *label);
extern CcuResult CcuDoWhileEnd(CcuVariableHandle var, uint64_t immediate, CcuConditionType condType, const char *label);

extern CcuResult CcuFuncBlockLookup(const void *funcPtr, uint64_t *outHandle);
extern CcuResult CcuFuncBlockBegin(const void *funcPtr, uint64_t *outHandle);
extern CcuResult CcuFuncBlockEnd(uint64_t handle);
extern CcuResult CcuFuncDefineInArg(uint64_t handle, CcuVariableHandle formal);
extern CcuResult CcuFuncCall(uint64_t handle, const CcuVariableHandle *inArgs, uint32_t numIn);

extern void _CcuIfStackPush(const char *label);
extern void _CcuIfStackMarkBodyDone();
extern const char *_CcuIfStackPopForElse();

extern void _CcuDoWhileStackPush(const char *label);
extern const char *_CcuDoWhileStackPopForWhile();

extern CcuResult CcuLoopCreate(CcuLoop *loop);
extern CcuResult _CcuLoopBodyEnter(CcuLoop loop);
extern CcuResult _CcuLoopBodyExit(CcuLoop loop);
extern CcuResult CcuLoopGroupCreate(CcuLoopGroup *group, uint32_t maxLoopNum, const CcuLoopGroupConfig *config);
extern CcuResult CcuLoopGroupCreateFromVar(CcuLoopGroup *group, uint32_t maxLoopNum, CcuVariableHandle parallelVar, CcuVariableHandle offsetVar);
extern CcuResult CcuLoopGroupAddLoop(CcuLoopGroup group, CcuLoop loop, const CcuLoopConfig *config);
extern CcuResult CcuLoopGroupAddLoopFromVar(CcuLoopGroup group, CcuLoop loop, CcuVariableHandle loopParamVar);

#ifdef __cplusplus
}
#endif

#endif