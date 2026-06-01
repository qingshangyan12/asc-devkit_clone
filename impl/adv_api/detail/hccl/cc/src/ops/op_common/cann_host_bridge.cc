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
#include <memory>
#include <new>
#include <string>

#include "log.h"

namespace mc2_ops_hccl {
namespace cann_abi {
namespace {

constexpr const char* kDefaultCannLib = "libhccl.so";

// ops_hccl::CollAlgExecRegistryV2::Instance()
constexpr const char* kSymInstance = "_ZN8ops_hccl21CollAlgExecRegistryV28InstanceEv";
// ops_hccl::CollAlgExecRegistryV2::GetAlgExec(HcclCMDType, const std::string&)
// libhccl.so 以旧 ABI(_GLIBCXX_USE_CXX11_ABI=0)编译, std::string mangle 为 Ss, 故此处用 RKSs
// 而非 __cxx11 版本(与本 TU 的 -D_GLIBCXX_USE_CXX11_ABI=0 及 kSymSelectorRun 的 RSs 一致)。
constexpr const char* kSymGetAlgExec = "_ZN8ops_hccl21CollAlgExecRegistryV210GetAlgExecE11HcclCMDTypeRKSs";
// ops_hccl::HcclGetAlgRes(void*, OpParam&, unique_ptr<InsCollAlgBase>&,
//   TopoInfoWithNetLayerDetails*, unique_ptr<AlgResourceCtxSerializable>&, void**, bool&)
constexpr const char* kSymHcclGetAlgRes =
    "_ZN8ops_hccl13HcclGetAlgResEPvRNS_7OpParamERSt10unique_ptrINS_14InsCollAlgBaseESt14default_"
    "deleteIS4_EEPNS_27TopoInfoWithNetLayerDetailsERS3_INS_26AlgResourceCtxSerializableES5_ISB_EEPS0_Rb";
// ops_hccl::ExecuteSelector::ExecuteSelector()
constexpr const char* kSymSelectorCtor = "_ZN8ops_hccl15ExecuteSelectorC1Ev";
// ops_hccl::ExecuteSelector::Run(OpParam&, TopoInfoWithNetLayerDetails*, std::string&) const
constexpr const char* kSymSelectorRun =
    "_ZNK8ops_hccl15ExecuteSelector3RunERNS_7OpParamEPNS_27TopoInfoWithNetLayerDetailsERSs";

// 与 CANN ops_hccl::InsCollAlgBase 虚函数槽位一致的抽象基类(到 CalcRes 为止即够,
// HcclGetAlgRes 只用 CalcAlgHierarchyInfo / CalcRes)。参数类型只需 ABI 兼容(引用=指针)。
// 槽位顺序: dtor, Describe, CalcAlgHierarchyInfo, CalcRes, ...(同 device bridge)。
class InsCollAlgBase {
public:
    virtual ~InsCollAlgBase() {}
    virtual std::string Describe() const = 0;
    virtual int CalcAlgHierarchyInfo(void*, void*, void*) = 0;
    virtual int CalcRes(void*, const void*, const void*, const void*, void*) = 0;
    virtual int Orchestrate(const void*, const void*) = 0;
};

// 函数指针类型。
using InstanceFn = void* (*)();
using GetAlgExecFn = std::unique_ptr<InsCollAlgBase> (*)(void* self, HcclCMDType cmdType, const std::string& algName);
// HcclGetAlgRes: comm, param, executor(unique_ptr&), topoInfo*, resCtxHost(unique_ptr&),
//   resCtxSequence(void**), isResourceReused(bool&)。
using HcclGetAlgResFn = HcclResult (*)(
    void* comm, mc2_ops_hccl::OpParam& param, std::unique_ptr<InsCollAlgBase>& executor,
    mc2_ops_hccl::TopoInfoWithNetLayerDetails* topoInfo,
    std::unique_ptr<mc2_ops_hccl::AlgResourceCtxSerializable>& resCtxHost, void** resCtxSequence,
    bool& isResourceReused);
// ExecuteSelector 构造: 无入参, 在调用方提供的内存上原地构造(类无数据成员)。
using SelectorCtorFn = void (*)(void* self);
// ExecuteSelector::Run const 成员函数: AArch64 下 this 作隐式首参。
using SelectorRunFn = HcclResult (*)(
    const void* self, mc2_ops_hccl::OpParam& param, mc2_ops_hccl::TopoInfoWithNetLayerDetails* topoInfo,
    std::string& algName);

struct CannSyms {
    void* handle = nullptr;
    InstanceFn instance = nullptr;
    GetAlgExecFn getAlgExec = nullptr;
    HcclGetAlgResFn hcclGetAlgRes = nullptr;
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
        s.instance = reinterpret_cast<InstanceFn>(dlsym(s.handle, kSymInstance));
        s.getAlgExec = reinterpret_cast<GetAlgExecFn>(dlsym(s.handle, kSymGetAlgExec));
        s.hcclGetAlgRes = reinterpret_cast<HcclGetAlgResFn>(dlsym(s.handle, kSymHcclGetAlgRes));
        s.selectorCtor = reinterpret_cast<SelectorCtorFn>(dlsym(s.handle, kSymSelectorCtor));
        s.selectorRun = reinterpret_cast<SelectorRunFn>(dlsym(s.handle, kSymSelectorRun));
        if (s.instance == nullptr || s.getAlgExec == nullptr || s.hcclGetAlgRes == nullptr ||
            s.selectorCtor == nullptr || s.selectorRun == nullptr) {
            HCCL_ERROR(
                "[HostCannBridge] dlsym failed: Instance=%p GetAlgExec=%p HcclGetAlgRes=%p "
                "SelectorCtor=%p SelectorRun=%p err=%s",
                reinterpret_cast<void*>(s.instance), reinterpret_cast<void*>(s.getAlgExec),
                reinterpret_cast<void*>(s.hcclGetAlgRes), reinterpret_cast<void*>(s.selectorCtor),
                reinterpret_cast<void*>(s.selectorRun), dlerror());
            return s;
        }
        s.ok = true;
        HCCL_RUN_INFO(
            "[HostCannBridge] loaded %s handle=%p Instance=%p GetAlgExec=%p HcclGetAlgRes=%p "
            "SelectorCtor=%p SelectorRun=%p",
            libName, s.handle, reinterpret_cast<void*>(s.instance), reinterpret_cast<void*>(s.getAlgExec),
            reinterpret_cast<void*>(s.hcclGetAlgRes), reinterpret_cast<void*>(s.selectorCtor),
            reinterpret_cast<void*>(s.selectorRun));
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

HcclResult GetAlgResViaCann(
    HcclComm comm, OpParam& ascParam, TopoInfoWithNetLayerDetails* ascTopo, const std::string& algName,
    void** resCtxSequence, u64& ctxSize)
{
    const cann_abi::CannSyms& syms = cann_abi::LoadCannSyms();
    if (!syms.ok) {
        HCCL_ERROR("[HostCannBridge][ResCtx] symbols not ready, fallback to asc path. algName[%s]", algName.c_str());
        return HCCL_E_NOT_SUPPORT;
    }
    if (ascTopo == nullptr || resCtxSequence == nullptr) {
        HCCL_ERROR("[HostCannBridge][ResCtx] null arg: ascTopo=%p resCtxSequence=%p", ascTopo, resCtxSequence);
        return HCCL_E_PARA;
    }

    // 1) 取 CANN executor(隐式 this = Instance() 返回的 registry 地址)。
    void* self = syms.instance();
    std::unique_ptr<cann_abi::InsCollAlgBase> executor = syms.getAlgExec(self, ascParam.opType, algName);
    if (executor == nullptr) {
        HCCL_ERROR(
            "[HostCannBridge][ResCtx] GetAlgExec returned null for opType=%u algName[%s]",
            static_cast<u32>(ascParam.opType), algName.c_str());
        return HCCL_E_NOT_SUPPORT;
    }

    // 2) 调 CANN HcclGetAlgRes 产 resCtx(内部建链 + memcpy 到 device, 回写 ctxSize)。
    //    asc TopoInfoWithNetLayerDetails 与 CANN 布局已对齐, 可直接传 ascTopo 指针。
    std::unique_ptr<AlgResourceCtxSerializable> resCtxHost = std::make_unique<AlgResourceCtxSerializable>();
    bool isResourceReused = false;
    HCCL_RUN_INFO(
        "[HostCannBridge][ResCtx] before CANN HcclGetAlgRes, opType=%u algName[%s] engine=%d",
        static_cast<u32>(ascParam.opType), algName.c_str(), static_cast<int>(ascParam.engine));
    HcclResult ret = syms.hcclGetAlgRes(
        static_cast<void*>(comm), ascParam, executor, ascTopo, resCtxHost, resCtxSequence, isResourceReused);
    if (ret != HCCL_SUCCESS) {
        HCCL_ERROR(
            "[HostCannBridge][ResCtx] CANN HcclGetAlgRes failed, ret=%d algName[%s]", static_cast<int>(ret),
            algName.c_str());
        return ret;
    }
    ctxSize = ascParam.ctxSize;
    HCCL_RUN_INFO(
        "[HostCannBridge][ResCtx] CANN HcclGetAlgRes success, algName[%s] ctxSize=%llu resCtxSeq=%p "
        "reused=%d",
        algName.c_str(), static_cast<unsigned long long>(ctxSize), *resCtxSequence, isResourceReused);
    return HCCL_SUCCESS;
}

} // namespace mc2_ops_hccl