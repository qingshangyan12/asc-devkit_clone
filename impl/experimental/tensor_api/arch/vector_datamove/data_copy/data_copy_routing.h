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
 * \file data_copy_routing.h
 * \brief
 */
#ifndef IMPL_TENSOR_API_ARCH_VECTOR_DATAMOVE_DATA_COPY_DATA_COPY_ROUTING_H
#define IMPL_TENSOR_API_ARCH_VECTOR_DATAMOVE_DATA_COPY_DATA_COPY_ROUTING_H

#include "impl/experimental/tensor_api/arch/data_copy_routing_base.h"

#include "impl/experimental/tensor_api/arch/vector_datamove/data_copy/npu_arch_3510/data_copy_gm2ub.h"
#include "impl/experimental/tensor_api/arch/vector_datamove/data_copy/npu_arch_3510/data_copy_ub2gm.h"

#include "impl/experimental/tensor_api/arch/vector_datamove/data_copy/npu_arch_3510/data_copy_ub2l1.h"


namespace AscendC {
namespace Te {

template <>
struct DataCopyTensor2Tensor<Hardware::UB, Hardware::GM, ArchVersion::V3510> {
    using type = DataCopyGM2UB3510;
};

template <>
struct DataCopyTensor2Tensor<Hardware::GM, Hardware::UB, ArchVersion::V3510> {
    using type = DataCopyUB2GM3510;
};

template <>
struct DataCopyTensor2Tensor<Hardware::L1, Hardware::UB, ArchVersion::V3510> {
    using type = DataCopyUB2L13510;
};


} // namespace Te
} // namespace AscendC

#endif // IMPL_TENSOR_API_ARCH_VECTOR_DATAMOVE_DATA_COPY_DATA_COPY_ROUTING_H