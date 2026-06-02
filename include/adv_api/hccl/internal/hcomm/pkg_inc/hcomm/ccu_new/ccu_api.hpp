/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef CCU_API_HPP
#define CCU_API_HPP

#include "ccu_data_api_impl.h"
#include "ccu_variable.hpp"
#include "ccu_func.hpp"

namespace ccu {

// ==================== 资源创建 ====================

template <typename T>
inline T GetResByChannel(ChannelHandle /*channel*/, uint32_t /*index*/) {
    static_assert(sizeof(T) == 0,
        "ccu::GetResByChannel<T> is not specialized for this type T; "
        "currently supported: Variable.");
}
template <>
inline Variable GetResByChannel<Variable>(ChannelHandle channel, uint32_t varIndex) {
    Variable v{NoAllocTag{}};
    CCU_THROW_IF_FAILED(CcuVariableCreateByChannel(channel, varIndex, &v.handle),
        "CcuVariableCreateByChannel: failed");
    return v;
}

} // namespace ccu

#endif // CCU_API_HPP
