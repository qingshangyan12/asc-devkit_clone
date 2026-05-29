/**
* Copyright (c) 2026 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/
#ifndef MC2_OPS_HCCL_DL_HCCL_LIB_H
#define MC2_OPS_HCCL_DL_HCCL_LIB_H

#include <mutex>
#include <dlfcn.h>

namespace mc2_ops_hccl {

// Lazy loader for the CANN-shipped HCCL implementation library:
//   - host  side (mc2_client / MC2_SERVER_ONLY undefined): libhccl.so
//   - device side (asc_aicpu_custom / mc2_server, AICPU_COMPILE defined):
//                                                      libscatter_aicpu_kernel.so
//
// The library may be absent at install time; callers must tolerate
// GetHandle() == nullptr and fail their op-specific path gracefully.
class DlHcclLib {
public:
    static DlHcclLib &GetInstance();
    ~DlHcclLib();

    // Returns a non-null dlopen handle on success, nullptr if the library
    // could not be loaded. Repeated calls reuse the cached handle.
    void *GetHandle();

    // Resolves a mangled symbol within the loaded library. Returns nullptr
    // when the library is unloaded or the symbol is missing; callers should
    // log and return an error code in that case.
    void *Resolve(const char *mangled);

private:
    DlHcclLib() = default;
    DlHcclLib(const DlHcclLib &) = delete;
    DlHcclLib &operator=(const DlHcclLib &) = delete;

    void *handle_{nullptr};
    bool tried_{false};
    std::mutex mu_;
};

}  // namespace mc2_ops_hccl

#endif  // MC2_OPS_HCCL_DL_HCCL_LIB_H
