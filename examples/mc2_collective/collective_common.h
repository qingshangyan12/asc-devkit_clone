/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software; you can redistribute it and/or modify it under the terms and conditions of
 * the CANN Open Software License Agreement Version 2.0.
 * Please refer to the License for details.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO, NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef MC2_COLLECTIVE_COMMON_H
#define MC2_COLLECTIVE_COMMON_H

#include <cstdint>

enum CollectiveOpType : uint32_t {
    COLLECTIVE_ALLGATHER = 0,
    COLLECTIVE_ALLREDUCE = 1,
    COLLECTIVE_ALLTOALL = 2,
    COLLECTIVE_ALLTOALLV = 3,
    COLLECTIVE_REDUCESCATTER = 4,
};

#endif
