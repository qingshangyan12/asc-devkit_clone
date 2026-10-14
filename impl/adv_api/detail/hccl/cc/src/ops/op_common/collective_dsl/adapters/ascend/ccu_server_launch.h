// Copyright (c) 2026 Huawei Technologies Co., Ltd.
// Licensed under the CANN Open Software License Agreement Version 2.0.
#ifndef MC2_DSL_CCU_SERVER_LAUNCH_H
#define MC2_DSL_CCU_SERVER_LAUNCH_H
#include "ccu_all_gather_server.h"
namespace mc2_ops_hccl { namespace dsl {
CcuResult RegisterCcuAllGatherServer(CcuInsHandle ins,uint32_t die,const CcuServerBinding& binding,CcuKernelHandle& kernel);
CcuResult LaunchCcuAllGatherServer(ThreadHandle thread,CcuKernelHandle kernel,uint64_t mailbox,uint64_t token);
} }
#endif
