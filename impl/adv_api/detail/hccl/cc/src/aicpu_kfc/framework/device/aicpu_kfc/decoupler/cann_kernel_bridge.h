/**
* Copyright (c) 2026 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/

// cann_kernel_bridge.h
//
// 本阶段目标: 打通链路。通过 dlopen CANN 的 libscatter_aicpu_kernel.so，借用其
// ops_hccl::CollAlgExecRegistryV2::GetAlgExec + 返回 executor 的 Orchestrate，
// 使 alltoall 场景由 CANN 内部派发到 ops_hccl::InsTempAlltoAllVMesh1D::KernelRun。
//
// 由于 CANN 包未安装内部头，这里手写 CANN 布局的镜像类型(cann_abi 命名空间)。
// 镜像嵌套在 mc2_ops_hccl 内，使其成员类型名(OpMode/AlgType/ChannelInfo/HcclMem...)
// 与 asc alg_param.h 中的解析完全一致；仅按 CANN 的字段顺序重新声明 OpParam /
// AlgResourceCtxSerializable，并提供一个虚函数槽位与 CANN 一致的抽象基类用于虚调用。
#ifndef MC2_OPS_HCCL_CANN_KERNEL_BRIDGE_H
#define MC2_OPS_HCCL_CANN_KERNEL_BRIDGE_H

#include <string>
#include "alg_param.h"

namespace mc2_ops_hccl {
namespace cann_abi {

// 对应 CANN alg_param.h 的 DevAicpuOpConfig
struct DevAicpuOpConfig {
    u32 execTimeout = 0;
    double multipleDimensionSplitRatio = 0.8;
};

// CANN 布局的 OpParam。与 asc OpParam 仅两处差异:
//  - asc 在 algType 后内嵌 double multipleDimensionSplitRatio; CANN 无此字段。
//  - CANN 在 dataCount 后多 DevAicpuOpConfig opConfig; asc 无。
// 其余字段名/类型/顺序完全一致(成员类型经 mc2_ops_hccl 命名空间解析)。
struct OpParam {
    void* hcclComm;
    char tag[TAG_LENGTH] = "";
    char algTag[ALG_TAG_LENGTH] = "";
    char fastLaunchTag[ALG_TAG_LENGTH] = "";
    char fallbackTag[ALG_MAX_LENGTH] = "";
    char commName[COMM_INDENTIFIER_MAX_LENGTH] = "";
    char commModeTag[TAG_LENGTH] = "";
    aclrtStream stream;
    void* inputPtr = nullptr;
    u64 inputSize = 0;
    void* outputPtr = nullptr;
    u64 outputSize = 0;
    HcclMem hcclBuff;
    HcclReduceOp reduceType = HcclReduceOp::HCCL_REDUCE_RESERVED;
    u32 root = INVALID_VALUE_RANKID;
    u32 userRank = INVALID_VALUE_RANKID;
    u32 sendRecvRemoteRank = INVALID_VALUE_RANKID;
    OpMode opMode;
    bool   enableDetour{false};
    bool   isMc2{false};
    DevType deviceType = DevType::DEV_TYPE_COUNT;
    CommEngine engine = CommEngine::COMM_ENGINE_RESERVED;
    AlgType algType;
    char algTypeStr[ALG_MAX_LENGTH] = "";
    union {
        struct {
            u64 count;
            HcclDataType dataType;
            HcclDataType outputType;
            u64 strideCount;
        } DataDes = {0, HCCL_DATA_TYPE_RESERVED, HCCL_DATA_TYPE_RESERVED, 0};
        struct {
            HcclDataType sendType;
            HcclDataType recvType;
            u64 sendCount;
            u64 recvCount;
        } all2AllDataDes;
        struct {
            void* counts;
            void* displs;
            HcclDataType dataType;
        } vDataDes;
        struct {
            HcclDataType sendType;
            HcclDataType recvType;
            void* sendCounts;
            void* recvCounts;
            void* sdispls;
            void* rdispls;
        } all2AllVDataDes;
        struct {
            HcclDataType sendType;
            HcclDataType recvType;
            void* sendCountMatrix;
        } all2AllVCDataDes;
        struct {
            HcclSendRecvItem* sendRecvItemsPtr;
            u32 itemNum;
        } batchSendRecvDataDes;
    };
    HcclCMDType opType = HcclCMDType::HCCL_CMD_INVALID;
    bool isZeroCopy = false;
    char algName[OP_ALG_LENGTH] = "";
    HcclOpExpansionMode commOpExpansionMode = HcclOpExpansionMode::HCCL_OP_EXPANSION_MODE_INVALID;
    OpExecuteConfig opExecuteConfig;
    u32 numBlocksLimit = 0;
    bool isAivClearEnable = false;
    u64 ctxSize = 0;
    void* resCtx = nullptr;
    ThreadHandle opThread = 0;
    u32 aicpuRecordCpuIdx = 0;
    u32 dataCount = 0;
    DevAicpuOpConfig opConfig;   // CANN 比 asc 多出的字段
    u64 varMemSize{0};
    u8 varData[0];
};

// CANN 布局的 AlgResourceCtxSerializable。与 asc 仅一处差异:
// CANN 在 channels 之后多一个 bool isHcommBatchTransferOnThreadSupported。
// 仅保留数据成员(布局需与 CANN 一致); 不需要 Serialize/DeSerialize。
struct AlgResourceCtxSerializable {
    AlgType algType;
    AlgHierarchyInfoForAllLevel algHierarchyInfo;
    HcclMem cclMem;
    u32 notifyNumOnMainThread;
    u32 slaveThreadNum;
    std::vector<u32> notifyNumPerThread;
    void* aivCommInfoPtr = nullptr;
    std::vector<ThreadHandle> threads;
    ThreadHandle unfoldThread = 0;
    std::vector<std::vector<ChannelInfo>> channels;
    bool isHcommBatchTransferOnThreadSupported = false;   // CANN 多出的字段
    void* commInfoPtr = nullptr;
    void *npu2DpuShmemPtr = nullptr;
    void *dpu2NpuShmemPtr = nullptr;
    std::vector<u32> ccuKernelNum;
    std::vector<CcuKernelHandle> ccuKernels;
    u32 topoInfoSeqSize = 0;
    TopoInfoWithNetLayerDetails topoInfo;
};

// 与 CANN ops_hccl::InsCollAlgBase 虚函数槽位一致的抽象基类。
// 仅需到 Orchestrate 为止的槽位顺序正确(dtor, Describe, CalcAlgHierarchyInfo,
// CalcRes, Orchestrate)，即可通过 vtable 正确派发 Orchestrate。后续虚函数无关。
// 参数类型只需 ABI 兼容(引用=指针)，签名细节不影响 vtable 槽位。
class InsCollAlgBase {
public:
    virtual ~InsCollAlgBase() {}
    virtual std::string Describe() const = 0;                       // slot: Describe
    virtual int CalcAlgHierarchyInfo(void*, void*, void*) = 0;      // slot: CalcAlgHierarchyInfo
    virtual int CalcRes(void*, const void*, const void*, const void*, void*) = 0; // slot: CalcRes
    virtual HcclResult Orchestrate(const OpParam &param,
                                   const AlgResourceCtxSerializable &resCtx) = 0; // slot: Orchestrate
};

}  // namespace cann_abi

// 旁路入口: 经 dlopen 调 CANN 的 GetAlgExec + Orchestrate。
// 入参为 asc 布局的 OpParam / AlgResourceCtxSerializable(调用方已 DeSerialize)。
// 返回 HCCL_SUCCESS 表示已成功派发到 CANN executor; 其它表示失败(调用方应回退)。
HcclResult LaunchViaCann(const OpParam &ascParam, const AlgResourceCtxSerializable &ascResCtx);

}  // namespace mc2_ops_hccl

#endif  // MC2_OPS_HCCL_CANN_KERNEL_BRIDGE_H
