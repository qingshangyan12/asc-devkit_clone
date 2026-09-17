// Copyright (c) 2026 Huawei Technologies Co., Ltd.
// Licensed under the CANN Open Software License Agreement Version 2.0.

#define __aicore__
#include "impl/utils/debug/asc_dump_super_tensor_capability.h"

#if defined(ASCENDC_DUMP_SUPER_TENSOR_RUNTIME_CAPABILITY) && \
    ASCENDC_DUMP_SUPER_TENSOR_RUNTIME_CAPABILITY == 1
static_assert(__asc_aicore::has_super_tensor_protocol(), "runtime capability must enable the protocol");
#else
static_assert(!__asc_aicore::has_super_tensor_protocol(), "protocol must fail closed without runtime capability");
#endif

int main()
{
    return 0;
}
