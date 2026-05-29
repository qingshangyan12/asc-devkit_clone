/**
* Copyright (c) 2026 Huawei Technologies Co., Ltd.
* Executor桥接实现 - 让本地Executor调用CANN库的实现
*/
#include "cann_executor_bridge.h"

namespace hccl {
    struct OpParam;
    struct AlgResourceResponse;
    
    class CollAlltoAllExecutor {
    public:
        HcclResult Orchestrate(OpParam& param, AlgResourceResponse& resCtx);
    };
    
    class CollScatterExecutor {
    public:
        HcclResult Orchestrate(OpParam& param, AlgResourceResponse& resCtx);
    };
    
    class CollAllGatherExecutor {
    public:
        HcclResult Orchestrate(OpParam& param, AlgResourceResponse& resCtx);
    };
    
    class CollAllReduceExecutor {
    public:
        HcclResult Orchestrate(OpParam& param, AlgResourceResponse& resCtx);
    };
    
    class CollReduceScatterExecutor {
    public:
        HcclResult Orchestrate(OpParam& param, AlgResourceResponse& resCtx);
    };
    
    class CollBroadcastExecutor {
    public:
        HcclResult Orchestrate(OpParam& param, AlgResourceResponse& resCtx);
    };
}

namespace mc2_ops_hccl {

CannExecutorBridge& CannExecutorBridge::Instance()
{
    static CannExecutorBridge instance;
    return instance;
}

HcclResult CannExecutorBridge::OrchestrateAlltoAll(const OpParam& param, const AlgResourceCtxSerializable& resCtx)
{
    HCCL_INFO("[CannExecutorBridge][OrchestrateAlltoAll] calling CANN hccl::CollAlltoAllExecutor");
    HCCL_INFO("[CannExecutorBridge] param.opType=%u, param.algName=%s, param.inputPtr=%p, param.outputPtr=%p",
        static_cast<u32>(param.opType), param.algName, param.inputPtr, param.outputPtr);
    
    hccl::CollAlltoAllExecutor executor;
    hccl::OpParam& cannParam = reinterpret_cast<hccl::OpParam&>(const_cast<OpParam&>(param));
    hccl::AlgResourceResponse& cannResCtx = reinterpret_cast<hccl::AlgResourceResponse&>(const_cast<AlgResourceCtxSerializable&>(resCtx));
    
    HcclResult ret = executor.Orchestrate(cannParam, cannResCtx);
    
    if (ret != HCCL_SUCCESS) {
        HCCL_ERROR("[CannExecutorBridge][OrchestrateAlltoAll] CANN returned error[%d]", ret);
        return ret;
    }
    
    HCCL_INFO("[CannExecutorBridge][OrchestrateAlltoAll] CANN orchestrate succeeded");
    return HCCL_SUCCESS;
}

HcclResult CannExecutorBridge::OrchestrateScatter(const OpParam& param, const AlgResourceCtxSerializable& resCtx)
{
    HCCL_INFO("[CannExecutorBridge][OrchestrateScatter] calling CANN hccl::CollScatterExecutor");
    
    hccl::CollScatterExecutor executor;
    hccl::OpParam& cannParam = reinterpret_cast<hccl::OpParam&>(const_cast<OpParam&>(param));
    hccl::AlgResourceResponse& cannResCtx = reinterpret_cast<hccl::AlgResourceResponse&>(const_cast<AlgResourceCtxSerializable&>(resCtx));
    
    HcclResult ret = executor.Orchestrate(cannParam, cannResCtx);
    
    if (ret != HCCL_SUCCESS) {
        HCCL_ERROR("[CannExecutorBridge][OrchestrateScatter] CANN returned error[%d]", ret);
        return ret;
    }
    
    HCCL_INFO("[CannExecutorBridge][OrchestrateScatter] CANN orchestrate succeeded");
    return HCCL_SUCCESS;
}

HcclResult CannExecutorBridge::OrchestrateAllGather(const OpParam& param, const AlgResourceCtxSerializable& resCtx)
{
    HCCL_INFO("[CannExecutorBridge][OrchestrateAllGather] calling CANN hccl::CollAllGatherExecutor");
    
    hccl::CollAllGatherExecutor executor;
    hccl::OpParam& cannParam = reinterpret_cast<hccl::OpParam&>(const_cast<OpParam&>(param));
    hccl::AlgResourceResponse& cannResCtx = reinterpret_cast<hccl::AlgResourceResponse&>(const_cast<AlgResourceCtxSerializable&>(resCtx));
    
    HcclResult ret = executor.Orchestrate(cannParam, cannResCtx);
    
    if (ret != HCCL_SUCCESS) {
        HCCL_ERROR("[CannExecutorBridge][OrchestrateAllGather] CANN returned error[%d]", ret);
        return ret;
    }
    
    HCCL_INFO("[CannExecutorBridge][OrchestrateAllGather] CANN orchestrate succeeded");
    return HCCL_SUCCESS;
}

HcclResult CannExecutorBridge::OrchestrateAllReduce(const OpParam& param, const AlgResourceCtxSerializable& resCtx)
{
    HCCL_INFO("[CannExecutorBridge][OrchestrateAllReduce] calling CANN hccl::CollAllReduceExecutor");
    
    hccl::CollAllReduceExecutor executor;
    hccl::OpParam& cannParam = reinterpret_cast<hccl::OpParam&>(const_cast<OpParam&>(param));
    hccl::AlgResourceResponse& cannResCtx = reinterpret_cast<hccl::AlgResourceResponse&>(const_cast<AlgResourceCtxSerializable&>(resCtx));
    
    HcclResult ret = executor.Orchestrate(cannParam, cannResCtx);
    
    if (ret != HCCL_SUCCESS) {
        HCCL_ERROR("[CannExecutorBridge][OrchestrateAllReduce] CANN returned error[%d]", ret);
        return ret;
    }
    
    HCCL_INFO("[CannExecutorBridge][OrchestrateAllReduce] CANN orchestrate succeeded");
    return HCCL_SUCCESS;
}

HcclResult CannExecutorBridge::OrchestrateReduceScatter(const OpParam& param, const AlgResourceCtxSerializable& resCtx)
{
    HCCL_INFO("[CannExecutorBridge][OrchestrateReduceScatter] calling CANN hccl::CollReduceScatterExecutor");
    
    hccl::CollReduceScatterExecutor executor;
    hccl::OpParam& cannParam = reinterpret_cast<hccl::OpParam&>(const_cast<OpParam&>(param));
    hccl::AlgResourceResponse& cannResCtx = reinterpret_cast<hccl::AlgResourceResponse&>(const_cast<AlgResourceCtxSerializable&>(resCtx));
    
    HcclResult ret = executor.Orchestrate(cannParam, cannResCtx);
    
    if (ret != HCCL_SUCCESS) {
        HCCL_ERROR("[CannExecutorBridge][OrchestrateReduceScatter] CANN returned error[%d]", ret);
        return ret;
    }
    
    HCCL_INFO("[CannExecutorBridge][OrchestrateReduceScatter] CANN orchestrate succeeded");
    return HCCL_SUCCESS;
}

HcclResult CannExecutorBridge::OrchestrateBroadcast(const OpParam& param, const AlgResourceCtxSerializable& resCtx)
{
    HCCL_INFO("[CannExecutorBridge][OrchestrateBroadcast] calling CANN hccl::CollBroadcastExecutor");
    
    hccl::CollBroadcastExecutor executor;
    hccl::OpParam& cannParam = reinterpret_cast<hccl::OpParam&>(const_cast<OpParam&>(param));
    hccl::AlgResourceResponse& cannResCtx = reinterpret_cast<hccl::AlgResourceResponse&>(const_cast<AlgResourceCtxSerializable&>(resCtx));
    
    HcclResult ret = executor.Orchestrate(cannParam, cannResCtx);
    
    if (ret != HCCL_SUCCESS) {
        HCCL_ERROR("[CannExecutorBridge][OrchestrateBroadcast] CANN returned error[%d]", ret);
        return ret;
    }
    
    HCCL_INFO("[CannExecutorBridge][OrchestrateBroadcast] CANN orchestrate succeeded");
    return HCCL_SUCCESS;
}

}