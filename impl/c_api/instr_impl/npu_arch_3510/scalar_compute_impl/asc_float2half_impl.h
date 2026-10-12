#ifndef IMPL_C_API_INSTR_IMPL_NPU_ARCH_3510_SCALAR_COMPUTE_IMPL_ASC_FLOAT2HALF_IMPL_H
#define IMPL_C_API_INSTR_IMPL_NPU_ARCH_3510_SCALAR_COMPUTE_IMPL_ASC_FLOAT2HALF_IMPL_H

#include "impl/c_api/instr_impl/npu_arch_3510/utils_impl.h"

__aicore__ inline half asc_float2half_ro_impl(float value) { return conv_f322f16o(value); }

#endif
