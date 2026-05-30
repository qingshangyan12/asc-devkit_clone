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

}  // namespace
}  // namespace cann_abi

HcclResult LaunchViaCann(const OpParam &ascParam, const AlgResourceCtxSerializable &ascResCtx)
{
    const cann_abi::CannSyms &syms = cann_abi::LoadCannSyms();
    if (!syms.ok) {
        HCCL_ERROR("[CannBridge] symbols not ready, fallback to asc path. algName[%s]", ascParam.algName);
        return HCCL_E_NOT_SUPPORT;
    }

    // OpParam / AlgResourceCtxSerializable 布局已与 CANN 对齐，直接传入，无需逐字段转换。
    // GetAlgExec(opType, algName)。隐式 this = Instance() 返回的 registry 地址。
    void *self = syms.instance();
    std::unique_ptr<cann_abi::InsCollAlgBase> executor =
        syms.getAlgExec(self, ascParam.opType, std::string(ascParam.algName));
    if (executor == nullptr) {
        HCCL_ERROR("[CannBridge] GetAlgExec returned null for opType=%u algName[%s]",
                   static_cast<u32>(ascParam.opType), ascParam.algName);
        return HCCL_E_NOT_SUPPORT;
    }

    return executor->Orchestrate(ascParam, ascResCtx);
}

}  // namespace mc2_ops_hccl
