/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

/*!
 * \file hccl_symbol_loader.h
 * \brief
 */

#pragma once
#include <unordered_map>
#include <string>
#include <mutex>
#include <dlfcn.h>
#include "../../detail/host_log.h"

namespace HcclApi {
class HcclSymbolLoader {
public:
    static HcclSymbolLoader& GetInstance()
    {
        static HcclSymbolLoader instance;
        return instance;
    }

private:
    std::unordered_map<std::string, void*> libMap_{};
    std::unordered_map<std::string, void*> symbolMap_{};
    mutable std::mutex mtx_;
    ~HcclSymbolLoader()
    {
        for (auto& it : libMap_) {
            if (it.second == nullptr) {
                continue;
            }
            (void)dlclose(it.second);
        }
    }
};
} // namespace HcclApi