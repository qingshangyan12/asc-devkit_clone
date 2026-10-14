// Copyright (c) 2026 Huawei Technologies Co., Ltd.
// Licensed under the CANN Open Software License Agreement Version 2.0.
#ifndef MC2_DSL_CCU_ALL_GATHER_SERVER_H
#define MC2_DSL_CCU_ALL_GATHER_SERVER_H
#include "ccu_all_gather_mesh.h"
#include "mailbox_protocol.h"
namespace mc2_ops_hccl { namespace dsl {
CcuResult BuildCcuAllGatherServer(ccu::Variable mailbox,ccu::Variable token,const ChannelHandle channels[],
    uint32_t channelCount,uint32_t rankSize,uint32_t rank,uint32_t capacity);
// HcommCcuKernelRegister 的编译期参数。channel 由真实 HComm 资源申请先行获取。
struct CcuServerBinding { const ChannelHandle* channels; uint32_t channelCount; uint32_t rankSize; uint32_t rank; uint32_t capacity; };
CcuResult CcuAllGatherServerEntry(const CcuServerBinding* binding);
} }
#endif
