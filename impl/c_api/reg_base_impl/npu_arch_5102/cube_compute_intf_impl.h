/*
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * M510 (npu_arch_5102) capi shim.
 * See docs/zh/api/SIMD-API/c_api/rfc_capi_m510_adaptation.md (RFC 4.2 / 4.5).
 *
 * NOTE: the 3510 counterpart defines only fp4/fp8 MX matmul (asc_mmad_mx*),
 * which M510 does NOT support (user-confirmed; see RFC 4.5). Those functions
 * are therefore intentionally NOT provided for npu_arch_5102 (Drop).
 */

#ifndef IMPL_C_API_REG_BASE_IMPL_NPU_ARCH_5102_CUBE_COMPUTE_INTF_IMPL_H
#define IMPL_C_API_REG_BASE_IMPL_NPU_ARCH_5102_CUBE_COMPUTE_INTF_IMPL_H

/* M510 provides no fp4/fp8 MX matmul; see RFC 4.5. */

#endif
