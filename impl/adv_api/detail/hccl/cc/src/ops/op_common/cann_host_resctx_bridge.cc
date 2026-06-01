/**
* Copyright (c) 2026 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/

// cann_host_resctx_bridge.cc — 见同名头文件说明。本阶段目标: 打通链路。
#include "cann_host_resctx_bridge.h"

#include <dlfcn.h>

#include <cstdlib>
#include <memory>
#include <string>

#include "log.h"

namespace mc2_ops_hccl {
namespace cann_abi {
namespace {

// host CANN 库名(可由环境变量覆盖, 便于指向特定路径的 libhccl.so)。
constexpr const char *kDefaultCannLib = "libhccl.so";
// 精确 mangled 名(实测自 libhccl.so 导出表, cann-9.1.0)。
// ops_hccl::CollAlgExecRegistryV2::Instance()
constexpr const char *kSymInstance = "_ZN8ops_hccl21CollAlgExecRegistryV28InstanceEv";
// ops_hccl::CollAlgExecRegistryV2::GetAlgExec(HcclCMDType, const std::string&)
constexpr const char *kSymGetAlgExec =
    "_ZN8ops_hccl21CollAlgExecRegistryV210GetAlgExecE11HcclCMDTypeRKNSt7__cxx11"
    "12basic_stringIcSt11char_traitsIcESaIcEEE";
// ops_hccl::HcclGetAlgRes(void*, OpParam&, unique_ptr<InsCollAlgBase>&,
//   TopoInfoWithNetLayerDetails*, unique_ptr<AlgResourceCtxSerializable>&, void**, bool&)
// 注意: 安装包(9.1.0)的 HcclGetAlgRes 为 7 参(无 resPack), 与 asc 源码一致。
constexpr const char *kSymHcclGetAlgRes =
    "_ZN8ops_hccl13HcclGetAlgResEPvRNS_7OpParamERSt10unique_ptrINS_14InsCollAlgBaseESt14default_"
    "deleteIS4_EEPNS_27TopoInfoWithNetLayerDetailsERS3_INS_26AlgResourceCtxSerializableES5_ISB_EEPS0_Rb";

// 与 CANN ops_hccl::InsCollAlgBase 虚函数槽位一致的抽象基类(到 CalcRes 为止即够,
// HcclGetAlgRes 只用 CalcAlgHierarchyInfo / CalcRes)。参数类型只需 ABI 兼容(引用=指针)。
// 槽位顺序: dtor, Describe, CalcAlgHierarchyInfo, CalcRes, ...(同 device bridge)。
class InsCollAlgBase {
public:
    virtual ~InsCollAlgBase() {}
    virtual std::string Describe() const = 0;
    virtual int CalcAlgHierarchyInfo(void *, void *, void *) = 0;
    virtual int CalcRes(void *, const void *, const void *, const void *, void *) = 0;
    virtual int Orchestrate(const void *, const void *) = 0;
};

// CANN 布局的 TopoInfoWithNetLayerDetails 镜像: 与 asc 唯一差异是 asc 多 bool Level1Hd
// (Level1Nhr 之后)。这里去掉该字段, 字段序与 CANN 安装包一致。base TopoInfo 与嵌套类型
// (Level0Shape/NetLayerDetails/TopoInstDetails/Level0MeshType)两 fork 逐字一致, 直接复用。
struct TopoInfoWithNetLayerDetails : public mc2_ops_hccl::TopoInfo {
    u32 topoLevelNums = 0;
    Level0Shape level0Topo;
    bool Level0Nhr{false};
    bool Level1Nhr{false};
    // (asc 此处有 bool Level1Hd, CANN 无)
    bool is2DieFullMesh{false};
    bool level0PcieMix{false};
    bool level0BigClosRange{false};
    u32 topoInstDetailsOfLayerSize = 0;
    Level0MeshType level0MeshType;
    NetLayerDetails netLayerDetails;
    std::vector<TopoInstDetails> topoInstDetailsOfLayer;
};

// 函数指针类型。
using InstanceFn = void *(*)();
using GetAlgExecFn =
    std::unique_ptr<InsCollAlgBase> (*)(void *self, HcclCMDType cmdType, const std::string &algName);
// HcclGetAlgRes: comm, param, executor(unique_ptr&), topoInfo*, resCtxHost(unique_ptr&),
//   resCtxSequence(void**), isResourceReused(bool&)。
using HcclGetAlgResFn = HcclResult (*)(void *comm, mc2_ops_hccl::OpParam &param,
    std::unique_ptr<InsCollAlgBase> &executor, TopoInfoWithNetLayerDetails *topoInfo,
    std::unique_ptr<mc2_ops_hccl::AlgResourceCtxSerializable> &resCtxHost, void **resCtxSequence,
    bool &isResourceReused);

struct CannSyms {
    void *handle = nullptr;
    InstanceFn instance = nullptr;
    GetAlgExecFn getAlgExec = nullptr;
    HcclGetAlgResFn hcclGetAlgRes = nullptr;
    bool ok = false;
};

// dlopen libhccl.so 并解析符号。先 RTLD_NOLOAD 复用已加载实例, 失败再按名加载。
// 仅在进程内解析一次(局部 static)。
const CannSyms &LoadCannSyms()
{
    static CannSyms syms = []() -> CannSyms {
        CannSyms s;
        const char *libName = std::getenv("MC2_HOST_RESCTX_CANN_LIB");
        if (libName == nullptr) {
            libName = kDefaultCannLib;
        }
        s.handle = dlopen(libName, RTLD_NOW | RTLD_LOCAL | RTLD_NOLOAD);
        if (s.handle == nullptr) {
            s.handle = dlopen(libName, RTLD_NOW | RTLD_LOCAL);
        }
        if (s.handle == nullptr) {
            HCCL_ERROR("[HostResCtxBridge] dlopen %s failed: %s", libName, dlerror());
            return s;
        }
        s.instance = reinterpret_cast<InstanceFn>(dlsym(s.handle, kSymInstance));
        s.getAlgExec = reinterpret_cast<GetAlgExecFn>(dlsym(s.handle, kSymGetAlgExec));
        s.hcclGetAlgRes = reinterpret_cast<HcclGetAlgResFn>(dlsym(s.handle, kSymHcclGetAlgRes));
        if (s.instance == nullptr || s.getAlgExec == nullptr || s.hcclGetAlgRes == nullptr) {
            HCCL_ERROR("[HostResCtxBridge] dlsym failed: Instance=%p GetAlgExec=%p HcclGetAlgRes=%p err=%s",
                       reinterpret_cast<void *>(s.instance), reinterpret_cast<void *>(s.getAlgExec),
                       reinterpret_cast<void *>(s.hcclGetAlgRes), dlerror());
            return s;
        }
        s.ok = true;
        HCCL_RUN_INFO("[HostResCtxBridge] loaded %s handle=%p Instance=%p GetAlgExec=%p HcclGetAlgRes=%p",
                      libName, s.handle, reinterpret_cast<void *>(s.instance),
                      reinterpret_cast<void *>(s.getAlgExec), reinterpret_cast<void *>(s.hcclGetAlgRes));
        return s;
    }();
    return syms;
}

// asc topoInfo → CANN 布局镜像(逐字段搬运, 跳过 asc 独有的 Level1Hd)。
void FillCannTopo(const mc2_ops_hccl::TopoInfoWithNetLayerDetails &src, TopoInfoWithNetLayerDetails &dst)
{
    // base TopoInfo 两 fork 逐字一致, 整体拷贝基类子对象。
    static_cast<mc2_ops_hccl::TopoInfo &>(dst) = static_cast<const mc2_ops_hccl::TopoInfo &>(src);
    // 派生字段逐一搬运(跳过 Level1Hd)。
    dst.topoLevelNums = src.topoLevelNums;
    dst.level0Topo = src.level0Topo;
    dst.Level0Nhr = src.Level0Nhr;
    dst.Level1Nhr = src.Level1Nhr;
    dst.is2DieFullMesh = src.is2DieFullMesh;
    dst.level0PcieMix = src.level0PcieMix;
    dst.level0BigClosRange = src.level0BigClosRange;
    dst.topoInstDetailsOfLayerSize = src.topoInstDetailsOfLayerSize;
    dst.level0MeshType = src.level0MeshType;
    dst.netLayerDetails = src.netLayerDetails;
    dst.topoInstDetailsOfLayer = src.topoInstDetailsOfLayer;
}

}  // namespace
}  // namespace cann_abi

HcclResult GetAlgResViaCann(HcclComm comm, OpParam &ascParam, TopoInfoWithNetLayerDetails *ascTopo,
                            const std::string &algName, void **resCtxSequence, u64 &ctxSize)
{
    const cann_abi::CannSyms &syms = cann_abi::LoadCannSyms();
    if (!syms.ok) {
        HCCL_ERROR("[HostResCtxBridge] symbols not ready, fallback to asc path. algName[%s]", algName.c_str());
        return HCCL_E_NOT_SUPPORT;
    }
    if (ascTopo == nullptr || resCtxSequence == nullptr) {
        HCCL_ERROR("[HostResCtxBridge] null arg: ascTopo=%p resCtxSequence=%p", ascTopo, resCtxSequence);
        return HCCL_E_PARA;
    }

    // 1) 取 CANN executor(隐式 this = Instance() 返回的 registry 地址)。
    void *self = syms.instance();
    std::unique_ptr<cann_abi::InsCollAlgBase> executor =
        syms.getAlgExec(self, ascParam.opType, algName);
    if (executor == nullptr) {
        HCCL_ERROR("[HostResCtxBridge] GetAlgExec returned null for opType=%u algName[%s]",
                   static_cast<u32>(ascParam.opType), algName.c_str());
        return HCCL_E_NOT_SUPPORT;
    }

    // 2) asc topoInfo → CANN 布局镜像(跳过 Level1Hd)。
    cann_abi::TopoInfoWithNetLayerDetails cannTopo;
    cann_abi::FillCannTopo(*ascTopo, cannTopo);

    // 3) 调 CANN HcclGetAlgRes 产 resCtx(内部建链 + memcpy 到 device, 回写 ctxSize)。
    //    resCtxHost 由 CANN 内部填充并 Serialize; 这里用 asc 的 AlgResourceCtxSerializable
    //    (布局已与 CANN 对齐)承接。
    std::unique_ptr<AlgResourceCtxSerializable> resCtxHost = std::make_unique<AlgResourceCtxSerializable>();
    bool isResourceReused = false;
    HCCL_RUN_INFO("[HostResCtxBridge] before CANN HcclGetAlgRes, opType=%u algName[%s] engine=%d",
                  static_cast<u32>(ascParam.opType), algName.c_str(), static_cast<int>(ascParam.engine));
    HcclResult ret = syms.hcclGetAlgRes(static_cast<void *>(comm), ascParam, executor, &cannTopo,
                                        resCtxHost, resCtxSequence, isResourceReused);
    if (ret != HCCL_SUCCESS) {
        HCCL_ERROR("[HostResCtxBridge] CANN HcclGetAlgRes failed, ret=%d algName[%s]",
                   static_cast<int>(ret), algName.c_str());
        return ret;
    }
    ctxSize = ascParam.ctxSize;
    HCCL_RUN_INFO("[HostResCtxBridge] CANN HcclGetAlgRes success, algName[%s] ctxSize=%llu resCtxSeq=%p reused=%d",
                  algName.c_str(), static_cast<unsigned long long>(ctxSize), *resCtxSequence, isResourceReused);
    return HCCL_SUCCESS;
}

}  // namespace mc2_ops_hccl
