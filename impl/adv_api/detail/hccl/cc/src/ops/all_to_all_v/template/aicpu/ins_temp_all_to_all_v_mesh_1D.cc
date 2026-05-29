/**
* Copyright (c) 2026 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/
//
// Forwards every member function of mc2_ops_hccl::InsTempAlltoAllVMesh1D to the
// matching ops_hccl::InsTempAlltoAllVMesh1D symbol shipped inside CANN
// (libhccl.so on host, libscatter_aicpu_kernel.so on device aicpu side).
//
// The library may be missing at install time, so all forwarding goes through
// the lazy DlHcclLib loader. Repeated calls cache the resolved function
// pointer in a static; if the library is missing or the symbol can't be
// found, calls return an error code (or, for void/pure-data getters, fall
// back to a no-op).
//
// ABI assumption: mc2_ops_hccl::InsTempAlltoAllVMesh1D and
// ops_hccl::InsTempAlltoAllVMesh1D have the same memory layout and vtable
// shape -- the same assumption already made in
// coll_alg_v2_exec_registry.cc. The header keeps the field declarations in
// sync with the upstream class; this .cc only forwards behavior.
//
#include "ins_temp_all_to_all_v_mesh_1D.h"
#include "dl_hccl_lib.h"
#include "log.h"

namespace mc2_ops_hccl {

namespace {

// Mangled symbols extracted from libhccl.so / libscatter_aicpu_kernel.so via
// `nm -D | grep InsTempAlltoAllVMesh1D`. If the upstream class signature
// changes these need to be regenerated.
constexpr const char *kSymCtor =
    "_ZN8ops_hccl22InsTempAlltoAllVMesh1DC1ERKNS_7OpParamEjRKSt6vectorIS4_IjSaIjEESaIS6_EE";
constexpr const char *kSymDtor =
    "_ZN8ops_hccl22InsTempAlltoAllVMesh1DD1Ev";
constexpr const char *kSymKernelRun =
    "_ZN8ops_hccl22InsTempAlltoAllVMesh1D9KernelRunERKNS_7OpParamE"
    "RKNS_18TemplateDataParamsERNS_16TemplateResourceE";
constexpr const char *kSymCalcRes =
    "_ZN8ops_hccl22InsTempAlltoAllVMesh1D7CalcResEPvRKNS_7OpParamE"
    "PKNS_27TopoInfoWithNetLayerDetailsERNS_18AlgResourceRequestE";
constexpr const char *kSymCalcScratch =
    "_ZN8ops_hccl22InsTempAlltoAllVMesh1D19CalcScratchMultipleENS_10BufferTypeES1_";
constexpr const char *kSymGetNotifyMain =
    "_ZN8ops_hccl22InsTempAlltoAllVMesh1D21GetNotifyIdxMainToSubERSt6vectorIjSaIjEE";
constexpr const char *kSymGetNotifySub =
    "_ZN8ops_hccl22InsTempAlltoAllVMesh1D21GetNotifyIdxSubToMainERSt6vectorIjSaIjEE";

// Function-pointer typedefs. Each takes a `void *self` first parameter --
// the Itanium C++ ABI lays out non-static member functions exactly like
// free functions whose first argument is `this`, so dlsym + this cast works
// without invoking pointer-to-member machinery.
using FnCtor = void (*)(void * /*self*/, const OpParam &, u32,
                        const std::vector<std::vector<u32>> &);
using FnDtor = void (*)(void * /*self*/);
using FnKernelRun = HcclResult (*)(void * /*self*/, const OpParam &,
                                   const TemplateDataParams &, const TemplateResource &);
using FnCalcRes = HcclResult (*)(void * /*self*/, HcclComm, const OpParam &,
                                 const TopoInfoWithNetLayerDetails *,
                                 AlgResourceRequest &);
using FnCalcScratch = u64 (*)(void * /*self*/, BufferType, BufferType);
using FnGetNotify = void (*)(void * /*self*/, std::vector<u32> &);

template <typename Fn>
Fn ResolveCached(const char *sym, Fn &cache)
{
    if (cache != nullptr) {
        return cache;
    }
    void *p = DlHcclLib::GetInstance().Resolve(sym);
    if (p == nullptr) {
        return nullptr;
    }
    cache = reinterpret_cast<Fn>(p);
    return cache;
}

}  // namespace

InsTempAlltoAllVMesh1D::InsTempAlltoAllVMesh1D(const OpParam &param, const u32 rankId,
    const std::vector<std::vector<u32>> &subCommRanks)
{
    static FnCtor fn = nullptr;
    auto resolved = ResolveCached(kSymCtor, fn);
    if (resolved == nullptr) {
        HCCL_ERROR("[InsTempAlltoAllVMesh1D] ctor symbol unavailable; object left "
                   "default-constructed and unusable");
        return;
    }
    resolved(this, param, rankId, subCommRanks);
}

InsTempAlltoAllVMesh1D::~InsTempAlltoAllVMesh1D()
{
    static FnDtor fn = nullptr;
    auto resolved = ResolveCached(kSymDtor, fn);
    if (resolved == nullptr) {
        // Library missing -- assume the object was never fully constructed
        // either, so leaking the (zero-initialized) members is acceptable.
        return;
    }
    resolved(this);
}

HcclResult InsTempAlltoAllVMesh1D::KernelRun(const OpParam &param,
                                             const TemplateDataParams &tempAlgParams,
                                             const TemplateResource &templateResource)
{
    static FnKernelRun fn = nullptr;
    auto resolved = ResolveCached(kSymKernelRun, fn);
    if (resolved == nullptr) {
        HCCL_ERROR("[InsTempAlltoAllVMesh1D::KernelRun] HCCL library unavailable");
        return HCCL_E_NOT_SUPPORT;
    }
    return resolved(this, param, tempAlgParams, templateResource);
}

HcclResult InsTempAlltoAllVMesh1D::CalcRes(HcclComm comm, const OpParam &param,
                                           const TopoInfoWithNetLayerDetails *topoInfo,
                                           AlgResourceRequest &resourceRequest)
{
    static FnCalcRes fn = nullptr;
    auto resolved = ResolveCached(kSymCalcRes, fn);
    if (resolved == nullptr) {
        HCCL_ERROR("[InsTempAlltoAllVMesh1D::CalcRes] HCCL library unavailable");
        return HCCL_E_NOT_SUPPORT;
    }
    return resolved(this, comm, param, topoInfo, resourceRequest);
}

u64 InsTempAlltoAllVMesh1D::CalcScratchMultiple(BufferType inBuffType, BufferType outBuffType)
{
    static FnCalcScratch fn = nullptr;
    auto resolved = ResolveCached(kSymCalcScratch, fn);
    if (resolved == nullptr) {
        HCCL_WARNING("[InsTempAlltoAllVMesh1D::CalcScratchMultiple] HCCL library "
                     "unavailable; returning 0");
        return 0;
    }
    return resolved(this, inBuffType, outBuffType);
}

void InsTempAlltoAllVMesh1D::GetNotifyIdxMainToSub(std::vector<u32> &notifyIdxMianToSub)
{
    static FnGetNotify fn = nullptr;
    auto resolved = ResolveCached(kSymGetNotifyMain, fn);
    if (resolved == nullptr) {
        HCCL_WARNING("[InsTempAlltoAllVMesh1D::GetNotifyIdxMainToSub] HCCL library "
                     "unavailable; leaving output untouched");
        return;
    }
    resolved(this, notifyIdxMianToSub);
}

void InsTempAlltoAllVMesh1D::GetNotifyIdxSubToMain(std::vector<u32> &notifyIdxSubToMain)
{
    static FnGetNotify fn = nullptr;
    auto resolved = ResolveCached(kSymGetNotifySub, fn);
    if (resolved == nullptr) {
        HCCL_WARNING("[InsTempAlltoAllVMesh1D::GetNotifyIdxSubToMain] HCCL library "
                     "unavailable; leaving output untouched");
        return;
    }
    resolved(this, notifyIdxSubToMain);
}

}  // namespace mc2_ops_hccl
