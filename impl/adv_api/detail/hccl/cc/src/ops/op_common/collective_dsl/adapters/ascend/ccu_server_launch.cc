// Copyright (c) 2026 Huawei Technologies Co., Ltd.
// Licensed under the CANN Open Software License Agreement Version 2.0.
#include "ccu_server_launch.h"
#include "ccu_launch_dl.h"
namespace mc2_ops_hccl { namespace dsl {
CcuResult RegisterCcuAllGatherServer(CcuInsHandle ins,uint32_t die,const CcuServerBinding& binding,CcuKernelHandle& kernel)
{
    if(!binding.capacity||!binding.rankSize||binding.rank>=binding.rankSize||binding.channelCount+1!=binding.rankSize)return CCU_E_PARA;
    CCU_CHK_RET(HcommCcuKernelRegisterStart(ins));
    const void* args[]={&binding};
    const CcuResult ret=HcommCcuKernelRegister(ins,die,"DslCcuAllGatherServer",
        reinterpret_cast<const void*>(CcuAllGatherServerEntry),args,1,&kernel);
    const CcuResult finish=HcommCcuKernelRegisterEnd(ins);
    return ret==CCU_SUCCESS?finish:ret;
}
CcuResult LaunchCcuAllGatherServer(ThreadHandle thread,CcuKernelHandle kernel,uint64_t mailbox,uint64_t token)
{
    if(!mailbox)return CCU_E_PTR;
    const uint64_t args[]={mailbox,token};
    return HcommCcuKernelLaunch(thread,kernel,args,2);
}
} }
