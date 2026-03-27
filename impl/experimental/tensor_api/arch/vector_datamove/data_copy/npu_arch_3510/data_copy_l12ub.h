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
 * \file data_copy_gm2l1.h
 * \brief
 */
#ifndef IMPL_TENSOR_API_ARCH_VECTOR_DATAMOVE_DATA_COPY_NPU_ARCH_3510_DATA_COPY_L12UB_H
#define IMPL_TENSOR_API_ARCH_VECTOR_DATAMOVE_DATA_COPY_NPU_ARCH_3510_DATA_COPY_L12UB_H

#include "include/experimental/tensor_api/utils/utils.h"
#include "impl/experimental/tensor_api/arch/vector_datamove/data_copy/npu_arch_3510/instruction.h"


namespace AscendC {
namespace Te {

class DataCopyL12UB3510 {
public:
    template <const DataCopyTrait& trait, typename T, typename U>
    __aicore__ inline void Run(const T& dst, const U& src) {
        Execute<trait>(dst, src);
    }

private:
    template <const DataCopyTrait& trait, typename T, typename U>
    __aicore__ inline static void Execute(const T& dst, const U& src) {
       
    }
};

} // namespace Te
} // namespace AscendC

#endif // IMPL_TENSOR_API_ARCH_VECTOR_DATAMOVE_DATA_COPY_NPU_ARCH_3510_DATA_COPY_L12UB_H