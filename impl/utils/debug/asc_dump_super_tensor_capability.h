// Copyright (c) 2026 Huawei Technologies Co., Ltd.
// Licensed under the CANN Open Software License Agreement Version 2.0.
#ifndef IMPL_UTILS_DEBUG_ASC_DUMP_SUPER_TENSOR_CAPABILITY_H
#define IMPL_UTILS_DEBUG_ASC_DUMP_SUPER_TENSOR_CAPABILITY_H

namespace __asc_aicore {

// The second switch is supplied by the build integration only after the
// matching Runtime/collector protocol is selected. Keeping it separate from
// the protocol switch prevents an accidental type 11/12 enablement.
__aicore__ constexpr inline bool has_super_tensor_protocol()
{
#if defined(ASCENDC_DUMP_SUPER_TENSOR) && ASCENDC_DUMP_SUPER_TENSOR == 1 && \
    defined(ASCENDC_DUMP_SUPER_TENSOR_RUNTIME_CAPABILITY) && \
    ASCENDC_DUMP_SUPER_TENSOR_RUNTIME_CAPABILITY == 1
    return true;
#else
    return false;
#endif
}

} // namespace __asc_aicore
#endif // IMPL_UTILS_DEBUG_ASC_DUMP_SUPER_TENSOR_CAPABILITY_H
