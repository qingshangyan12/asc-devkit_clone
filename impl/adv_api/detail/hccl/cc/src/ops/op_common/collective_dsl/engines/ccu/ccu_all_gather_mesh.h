// Copyright (c) 2026 Huawei Technologies Co., Ltd.
// Licensed under the CANN Open Software License Agreement Version 2.0.
#ifndef MC2_DSL_CCU_ALL_GATHER_MESH_H
#define MC2_DSL_CCU_ALL_GATHER_MESH_H
#include "ccu_kernel_alg_base.h"
namespace mc2_ops_hccl { namespace dsl {
// 保持 KFC sole Mesh 队列 ABI；goSize 为现有 client 协议保留，直搬路径不使用。
CcuResult BuildCcuAllGatherMesh(
    ccu::Variable input,ccu::Variable output,ccu::Variable token,ccu::Variable outputOffset,
    ccu::Variable bytes,ccu::Variable go0,ccu::Variable go1,ccu::Variable go2,ccu::Variable go3,
    ccu::Variable inputOffset,ccu::Variable repeatInverse,ccu::Variable inputStride,ccu::Variable outputStride,
    ccu::Variable tailBytes,ccu::Variable inplace,const ChannelHandle channels[],uint32_t channelCount,
    uint32_t rankSize,uint32_t rank);
} }
#endif
