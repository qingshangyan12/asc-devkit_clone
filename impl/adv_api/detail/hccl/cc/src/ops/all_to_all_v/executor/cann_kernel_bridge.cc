/**
* Copyright (c) 2026 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/

// cann_kernel_bridge.cc — 见同名头文件说明。本阶段目标: 打通链路。
#include "cann_kernel_bridge.h"

#include <dlfcn.h>

#include <cstdlib>
#include <cstring>
#include <memory>
#include <string>
#include <vector>

#include "log.h"

namespace mc2_ops_hccl {
namespace cann_abi {
namespace {

// CANN device kernel 默认库名(运行时由 KFC 释放到 device rootfs)。
constexpr const char *kDefaultCannLib = "libscatter_aicpu_kernel.so";
// 精确 mangled 名(实测自 libscatter_aicpu_kernel.so 导出表)。
constexpr const char *kSymInstance = "_ZN8ops_hccl21CollAlgExecRegistryV28InstanceEv";
constexpr const char *kSymGetAlgExec =
    "_ZN8ops_hccl21CollAlgExecRegistryV210GetAlgExecE11HcclCMDTypeRKNSt7__cxx11"
    "12basic_stringIcSt11char_traitsIcESaIcEEE";

// Instance(): 静态成员，返回 registry 引用 → 取其地址(void*)。
using InstanceFn = void *(*)();
// GetAlgExec(HcclCMDType, const std::string&): 非静态成员。
// 隐式 this 作第一个整型参数; 返回 unique_ptr(non-trivial) 由编译器按 AArch64
// 间接返回(x8)约定自动处理，无需手工传 sret 指针。
using GetAlgExecFn =
    std::unique_ptr<InsCollAlgBase> (*)(void *self, HcclCMDType cmdType, const std::string &algName);

struct CannSyms {
    void *handle = nullptr;
    InstanceFn instance = nullptr;
    GetAlgExecFn getAlgExec = nullptr;
    bool ok = false;
};

// dlopen CANN kernel 并解析符号。先 RTLD_NOLOAD 取已加载句柄，失败再按名加载。
// 仅在进程内解析一次(局部 static)。
const CannSyms &LoadCannSyms()
{
    static CannSyms syms = [] () -> CannSyms {
        CannSyms s;
        const char *libName = std::getenv("MC2_ALLTOALL_CANN_LIB");
        if (libName == nullptr) {
            libName = kDefaultCannLib;
        }
        // 优先复用已加载实例(RTLD_LOCAL 降低符号冲突面)。
        s.handle = dlopen(libName, RTLD_NOW | RTLD_LOCAL | RTLD_NOLOAD);
        if (s.handle == nullptr) {
            s.handle = dlopen(libName, RTLD_NOW | RTLD_LOCAL);
        }
        if (s.handle == nullptr) {
            HCCL_ERROR("[CannBridge] dlopen %s failed: %s", libName, dlerror());
            return s;
        }
        s.instance = reinterpret_cast<InstanceFn>(dlsym(s.handle, kSymInstance));
        s.getAlgExec = reinterpret_cast<GetAlgExecFn>(dlsym(s.handle, kSymGetAlgExec));
        if (s.instance == nullptr || s.getAlgExec == nullptr) {
            HCCL_ERROR("[CannBridge] dlsym failed: Instance=%p GetAlgExec=%p err=%s",
                       reinterpret_cast<void *>(s.instance), reinterpret_cast<void *>(s.getAlgExec),
                       dlerror());
            return s;
        }
        s.ok = true;
        HCCL_RUN_INFO("[CannBridge] loaded %s handle=%p Instance=%p GetAlgExec=%p", libName, s.handle,
                      reinterpret_cast<void *>(s.instance), reinterpret_cast<void *>(s.getAlgExec));
        return s;
    }();
    return syms;
}

// asc OpParam → CANN 布局 OpParam(逐字段)。本阶段以"过 GetAlgExec + 进入
// Orchestrate"为目标: opType/algName 必须正确; 其余字段尽力对齐。
// 变长区指针(all2AllVDataDes.* 指向调用方内存)随 ascParam 生命周期有效，直接沿用。
void FillCannOpParam(const mc2_ops_hccl::OpParam &src, OpParam &dst)
{
    dst.hcclComm = src.hcclComm;
    std::memcpy(dst.tag, src.tag, TAG_LENGTH);
    std::memcpy(dst.algTag, src.algTag, ALG_TAG_LENGTH);
    std::memcpy(dst.fastLaunchTag, src.fastLaunchTag, ALG_TAG_LENGTH);
    std::memcpy(dst.fallbackTag, src.fallbackTag, ALG_MAX_LENGTH);
    std::memcpy(dst.commName, src.commName, COMM_INDENTIFIER_MAX_LENGTH);
    std::memcpy(dst.commModeTag, src.commModeTag, TAG_LENGTH);
    dst.stream = src.stream;
    dst.inputPtr = src.inputPtr;
    dst.inputSize = src.inputSize;
    dst.outputPtr = src.outputPtr;
    dst.outputSize = src.outputSize;
    dst.hcclBuff = src.hcclBuff;
    dst.reduceType = src.reduceType;
    dst.root = src.root;
    dst.userRank = src.userRank;
    dst.sendRecvRemoteRank = src.sendRecvRemoteRank;
    dst.opMode = src.opMode;
    dst.enableDetour = src.enableDetour;
    dst.isMc2 = src.isMc2;
    dst.deviceType = src.deviceType;
    dst.engine = src.engine;
    dst.algType = src.algType;
    std::memcpy(dst.algTypeStr, src.algTypeStr, ALG_MAX_LENGTH);
    // union: 两 fork 布局一致，整体拷贝(覆盖所有 *DataDes 形态)。
    std::memcpy(&dst.all2AllVDataDes, &src.all2AllVDataDes, sizeof(src.all2AllVDataDes));
    dst.opType = src.opType;
    dst.isZeroCopy = src.isZeroCopy;
    std::memcpy(dst.algName, src.algName, OP_ALG_LENGTH);
    dst.commOpExpansionMode = src.commOpExpansionMode;
    dst.opExecuteConfig = src.opExecuteConfig;
    dst.numBlocksLimit = src.numBlocksLimit;
    dst.isAivClearEnable = src.isAivClearEnable;
    dst.ctxSize = src.ctxSize;
    dst.resCtx = src.resCtx;
    dst.opThread = src.opThread;
    dst.aicpuRecordCpuIdx = src.aicpuRecordCpuIdx;
    dst.dataCount = src.dataCount;
    // asc 的内嵌字段 → CANN 的 opConfig。
    dst.opConfig.multipleDimensionSplitRatio = src.multipleDimensionSplitRatio;
    dst.varMemSize = src.varMemSize;
    // 注: varData 柔性数组本阶段不重布置(对触达 KernelRun 非必需)。
}

// asc resCtx → CANN 布局 resCtx(逐字段)。差异仅多一个 bool，填 false。
void FillCannResCtx(const mc2_ops_hccl::AlgResourceCtxSerializable &src, AlgResourceCtxSerializable &dst)
{
    dst.algType = src.algType;
    dst.algHierarchyInfo = src.algHierarchyInfo;
    dst.cclMem = src.cclMem;
    dst.notifyNumOnMainThread = src.notifyNumOnMainThread;
    dst.slaveThreadNum = src.slaveThreadNum;
    dst.notifyNumPerThread = src.notifyNumPerThread;
    dst.aivCommInfoPtr = src.aivCommInfoPtr;
    dst.threads = src.threads;
    dst.unfoldThread = src.unfoldThread;
    dst.channels = src.channels;
    dst.isHcommBatchTransferOnThreadSupported = false;
    dst.commInfoPtr = src.commInfoPtr;
    dst.npu2DpuShmemPtr = src.npu2DpuShmemPtr;
    dst.dpu2NpuShmemPtr = src.dpu2NpuShmemPtr;
    dst.ccuKernelNum = src.ccuKernelNum;
    dst.ccuKernels = src.ccuKernels;
    dst.topoInfoSeqSize = src.topoInfoSeqSize;
    dst.topoInfo = src.topoInfo;
}

}  // namespace
}  // namespace cann_abi

HcclResult LaunchAlltoAllViaCann(const OpParam &ascParam, const AlgResourceCtxSerializable &ascResCtx)
{
    const cann_abi::CannSyms &syms = cann_abi::LoadCannSyms();
    if (!syms.ok) {
        HCCL_ERROR("[CannBridge] symbols not ready, fallback to asc path. algName[%s]", ascParam.algName);
        return HCCL_E_NOT_SUPPORT;
    }

    // 转换为 CANN 布局(逐字段，非 reinterpret，避免 ABI 错位)。
    cann_abi::OpParam cannParam{};
    cann_abi::FillCannOpParam(ascParam, cannParam);
    cann_abi::AlgResourceCtxSerializable cannResCtx{};
    cann_abi::FillCannResCtx(ascResCtx, cannResCtx);

    // GetAlgExec(opType, algName)。隐式 this = Instance() 返回的 registry 地址。
    void *self = syms.instance();
    HCCL_RUN_INFO("[CannBridge] Instance()=%p, calling GetAlgExec opType=%u algName[%s]", self,
                  static_cast<u32>(cannParam.opType), cannParam.algName);
    std::unique_ptr<cann_abi::InsCollAlgBase> executor =
        syms.getAlgExec(self, cannParam.opType, std::string(cannParam.algName));
    if (executor == nullptr) {
        HCCL_ERROR("[CannBridge] GetAlgExec returned null for opType=%u algName[%s]",
                   static_cast<u32>(cannParam.opType), cannParam.algName);
        return HCCL_E_NOT_SUPPORT;
    }

    HCCL_RUN_INFO("[CannBridge] executor=%p, calling Orchestrate (will reach CANN KernelRun)",
                  reinterpret_cast<void *>(executor.get()));
    HcclResult ret = executor->Orchestrate(cannParam, cannResCtx);
    HCCL_RUN_INFO("[CannBridge] CANN Orchestrate returned %d for algName[%s]", static_cast<int>(ret),
                  cannParam.algName);
    return ret;
}

}  // namespace mc2_ops_hccl
