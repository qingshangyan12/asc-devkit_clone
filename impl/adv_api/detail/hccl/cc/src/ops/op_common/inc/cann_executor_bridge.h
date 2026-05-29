/**
* Copyright (c) 2026 Huawei Technologies Co., Ltd.
* Executor桥接 - 让本地Executor调用CANN库的实现
*/
#ifndef CANN_EXECUTOR_BRIDGE_H
#define CANN_EXECUTOR_BRIDGE_H

#include "alg_param.h"
#include "alg_v2_template_base.h"
#include "log.h"

namespace hccl {
    class CollAlltoAllExecutor;
    class CollScatterExecutor;
    class CollAllGatherExecutor;
    class CollAllReduceExecutor;
    class CollReduceScatterExecutor;
    class CollBroadcastExecutor;
    
    class CollCommExecutor {
    public:
        HcclResult Orchestrate(void* param, void* resCtx);
    };
}

namespace mc2_ops_hccl {

class CannExecutorBridge {
public:
    static CannExecutorBridge& Instance();
    
    HcclResult OrchestrateAlltoAll(const OpParam& param, const AlgResourceCtxSerializable& resCtx);
    HcclResult OrchestrateScatter(const OpParam& param, const AlgResourceCtxSerializable& resCtx);
    HcclResult OrchestrateAllGather(const OpParam& param, const AlgResourceCtxSerializable& resCtx);
    HcclResult OrchestrateAllReduce(const OpParam& param, const AlgResourceCtxSerializable& resCtx);
    HcclResult OrchestrateReduceScatter(const OpParam& param, const AlgResourceCtxSerializable& resCtx);
    HcclResult OrchestrateBroadcast(const OpParam& param, const AlgResourceCtxSerializable& resCtx);
    
private:
    CannExecutorBridge() {}
    
    void* GetExecutorByOpType(HcclCMDType opType);
    void* ConvertParam(const OpParam& param);
    void* ConvertResCtx(const AlgResourceCtxSerializable& resCtx);
};

} // namespace mc2_ops_hccl

#endif