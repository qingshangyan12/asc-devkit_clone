/**
* Copyright (c) 2026 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/

/*!
 * \file data_copy_routing_base.h
 * \brief
 */
#ifndef IMPL_TENSOR_API_ARCH_DATA_COPY_ROUTING_BASE_H
#define IMPL_TENSOR_API_ARCH_DATA_COPY_ROUTING_BASE_H

#include "include/experimental/tensor_api/arch/trait_struct.h"

namespace AscendC {
namespace Te {

class DataCopyIgnore {
public:
    template <const DataCopyTrait& trait, typename ...Args>
    __aicore__ inline void Run(const Args&... args) {}
};

template <Hardware dstTPos, Hardware srcTpos, uint32_t Version>
struct DataCopyTensor2Tensor {
    using type = DataCopyIgnore;
};

} // namespace Te
} // namespace AscendC

#endif // IMPL_TENSOR_API_ARCH_DATA_COPY_ROUTING_BASE_H