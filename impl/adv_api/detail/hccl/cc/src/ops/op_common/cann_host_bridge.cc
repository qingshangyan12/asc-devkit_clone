/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#include "cann_host_bridge.h"

#include <dlfcn.h>

#include <cstdlib>
#include <string>

#include "log.h"

namespace mc2_ops_hccl {
namespace cann_abi {
namespace {

constexpr const char* kDefaultCannLib = "libhccl.so";

// ops_hccl::ExecuteSelector::ExecuteSelector()
constexpr const char* kSymSelectorCtor = "_ZN8ops_hccl15ExecuteSelectorC1Ev";
// ops_hccl::ExecuteSelector::Run(OpParam&, TopoInfoWithNetLayerDetails*, std::string&) const
// libhccl.so 以旧 ABI(_GLIBCXX_USE_CXX11_ABI=0)编译, std::string mangle 为 Ss, 故此处用 RSs。
constexpr const char* kSymSelectorRun =
    "_ZNK8ops_hccl15ExecuteSelector3RunERNS_7OpParamEPNS_27TopoInfoWithNetLayerDetailsERSs";

// 函数指针类型。
// ExecuteSelector 构造: 无入参, 在调用方提供的内存上原地构造(类无数据成员)。
using SelectorCtorFn = void (*)(void* self);
// ExecuteSelector::Run const 成员函数: AArch64 下 this 作隐式首参。
using SelectorRunFn = HcclResult (*)(
    const void* self, mc2_ops_hccl::OpParam& param, mc2_ops_hccl::TopoInfoWithNetLayerDetails* topoInfo,
    std::string& algName);

struct CannSyms {
    void* handle = nullptr;
    SelectorCtorFn selectorCtor = nullptr;
    SelectorRunFn selectorRun = nullptr;
    bool ok = false;
};

// dlopen libhccl.so 并一次性解析全部符号。先 RTLD_NOLOAD 复用已加载实例, 失败再按名加载。
// 仅在进程内解析一次(局部 static)。
const CannSyms& LoadCannSyms()
{
    static CannSyms syms = []() -> CannSyms {
        CannSyms s;
        const char* libName = std::getenv("MC2_HOST_CANN_LIB");
        if (libName == nullptr) {
            libName = kDefaultCannLib;
        }
        s.handle = dlopen(libName, RTLD_NOW | RTLD_LOCAL | RTLD_NOLOAD);
        if (s.handle == nullptr) {
            s.handle = dlopen(libName, RTLD_NOW | RTLD_LOCAL);
        }
        if (s.handle == nullptr) {
            HCCL_ERROR("[HostCannBridge] dlopen %s failed: %s", libName, dlerror());
            return s;
        }
        s.selectorCtor = reinterpret_cast<SelectorCtorFn>(dlsym(s.handle, kSymSelectorCtor));
        s.selectorRun = reinterpret_cast<SelectorRunFn>(dlsym(s.handle, kSymSelectorRun));
        if (s.selectorCtor == nullptr || s.selectorRun == nullptr) {
            HCCL_ERROR(
                "[HostCannBridge] dlsym failed: SelectorCtor=%p SelectorRun=%p err=%s",
                reinterpret_cast<void*>(s.selectorCtor), reinterpret_cast<void*>(s.selectorRun), dlerror());
            return s;
        }
        s.ok = true;
        HCCL_RUN_INFO(
            "[HostCannBridge] loaded %s handle=%p SelectorCtor=%p SelectorRun=%p",
            libName, s.handle, reinterpret_cast<void*>(s.selectorCtor), reinterpret_cast<void*>(s.selectorRun));
        return s;
    }();
    return syms;
}

} // namespace
} // namespace cann_abi

HcclResult SelectViaCann(OpParam& ascParam, TopoInfoWithNetLayerDetails* ascTopo, std::string& algName)
{
    const cann_abi::CannSyms& syms = cann_abi::LoadCannSyms();
    if (!syms.ok) {
        HCCL_ERROR(
            "[HostCannBridge][Select] symbols not ready, fallback to local selector. opType[%u]",
            static_cast<u32>(ascParam.opType));
        return HCCL_E_NOT_SUPPORT;
    }
    if (ascTopo == nullptr) {
        HCCL_ERROR("[HostCannBridge][Select] null arg: ascTopo=%p", ascTopo);
        return HCCL_E_PARA;
    }

    // 1) 在栈 buffer 上原地构造 CANN ExecuteSelector(无数据成员; 取地址作隐式 this)。
    //    给足对齐与冗余空间, 防御 CANN 侧若新增成员。
    alignas(16) unsigned char selBuf[64] = {0};
    syms.selectorCtor(static_cast<void*>(selBuf));
    const void* self = static_cast<const void*>(selBuf);

    // 2) 调 CANN ExecuteSelector::Run 选算法(回写 ascParam 与 ascTopo, 选出 algName)。
    //    asc TopoInfoWithNetLayerDetails 与 CANN 布局已对齐, 可直接传 ascTopo 指针。
    HCCL_RUN_INFO(
        "[HostCannBridge][Select] before CANN ExecuteSelector::Run, opType=%u engine=%d "
        "opExecuteConfig=%d",
        static_cast<u32>(ascParam.opType), static_cast<int>(ascParam.engine),
        static_cast<int>(ascParam.opExecuteConfig));
    HcclResult ret = syms.selectorRun(self, ascParam, ascTopo, algName);
    if (ret != HCCL_SUCCESS) {
        HCCL_ERROR(
            "[HostCannBridge][Select] CANN ExecuteSelector::Run failed, ret=%d opType=%u", static_cast<int>(ret),
            static_cast<u32>(ascParam.opType));
        return ret;
    }
    if (algName.empty()) {
        HCCL_ERROR(
            "[HostCannBridge][Select] CANN selected empty algName, opType=%u", static_cast<u32>(ascParam.opType));
        return HCCL_E_NOT_SUPPORT;
    }

    HCCL_RUN_INFO(
        "[HostCannBridge][Select] CANN ExecuteSelector::Run success, opType=%u algName[%s] engine=%d "
        "opExecuteConfig=%d",
        static_cast<u32>(ascParam.opType), algName.c_str(), static_cast<int>(ascParam.engine),
        static_cast<int>(ascParam.opExecuteConfig));
    return HCCL_SUCCESS;
}

} // namespace mc2_ops_hccl
