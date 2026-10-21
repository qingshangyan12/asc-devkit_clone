/*
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef CCU_FUNC_DL_HPP
#define CCU_FUNC_DL_HPP

// See ccu_variable_dl.hpp.  The stub already provides the Func abstraction
// required by the algorithm sources captured by this ST target.
#include "ccu_primitives_stub.h"
#include "ccu_types.h"

namespace AscendC {
namespace ccu {

inline CcuResult CallFunc(const Variable&, Array<Variable>& args)
{
    if (args.size() > CCU_FUNC_TASK_ARG_MAX) {
        return CCU_E_PARA;
    }
    std::vector<ResourceHandle> handles;
    for (uint32_t index = 0; index < args.size(); ++index) {
        handles.push_back(args[index].handle);
    }
    return HcclSim::CcuSt::CompilerContext::Current().CaptureFunction(handles) == Result::SUCCESS ? CCU_SUCCESS :
                                                                                                    CCU_E_INTERNAL;
}

inline CcuResult CallFunc(const Variable&, const std::vector<Variable>& args)
{
    if (args.size() > CCU_FUNC_TASK_ARG_MAX) {
        return CCU_E_PARA;
    }
    std::vector<ResourceHandle> handles;
    for (const auto& argument : args) {
        handles.push_back(argument.handle);
    }
    return HcclSim::CcuSt::CompilerContext::Current().CaptureFunction(handles) == Result::SUCCESS ? CCU_SUCCESS :
                                                                                                    CCU_E_INTERNAL;
}

} // namespace ccu
} // namespace AscendC

#endif // CCU_FUNC_DL_HPP
