#ifndef TESTS_API_C_API_NPU_ARCH_3510_UTILS_REG_CONVERT_EXTENDED_SIGNATURES_H
#define TESTS_API_C_API_NPU_ARCH_3510_UTILS_REG_CONVERT_EXTENDED_SIGNATURES_H

#include "include/c_api/asc_simd.h"

namespace {

template <typename Signature>
constexpr bool HasConversionSignature(Signature)
{
    return true;
}

static_assert(HasConversionSignature<vector_half (*)(
    vector_float, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_float2half_rd));

static_assert(HasConversionSignature<vector_half (*)(
    vector_float, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_float2half_rd));

static_assert(HasConversionSignature<vector_half (*)(
    vector_float, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_float2half_rd_sat));

static_assert(HasConversionSignature<vector_half (*)(
    vector_float, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_float2half_rd_sat));

static_assert(HasConversionSignature<vector_half (*)(
    vector_float, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_float2half_rn));

static_assert(HasConversionSignature<vector_half (*)(
    vector_float, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_float2half_rn));

static_assert(HasConversionSignature<vector_half (*)(
    vector_float, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_float2half_rn_sat));

static_assert(HasConversionSignature<vector_half (*)(
    vector_float, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_float2half_rn_sat));

static_assert(HasConversionSignature<vector_half (*)(
    vector_float, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_float2half_rna));

static_assert(HasConversionSignature<vector_half (*)(
    vector_float, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_float2half_rna));

static_assert(HasConversionSignature<vector_half (*)(
    vector_float, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_float2half_rna_sat));

static_assert(HasConversionSignature<vector_half (*)(
    vector_float, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_float2half_rna_sat));

static_assert(HasConversionSignature<vector_half (*)(
    vector_float, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_float2half_ro));

static_assert(HasConversionSignature<vector_half (*)(
    vector_float, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_float2half_ro));

static_assert(HasConversionSignature<vector_half (*)(
    vector_float, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_float2half_ro_sat));

static_assert(HasConversionSignature<vector_half (*)(
    vector_float, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_float2half_ro_sat));

static_assert(HasConversionSignature<vector_half (*)(
    vector_float, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_float2half_ru));

static_assert(HasConversionSignature<vector_half (*)(
    vector_float, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_float2half_ru));

static_assert(HasConversionSignature<vector_half (*)(
    vector_float, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_float2half_ru_sat));

static_assert(HasConversionSignature<vector_half (*)(
    vector_float, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_float2half_ru_sat));

static_assert(HasConversionSignature<vector_half (*)(
    vector_float, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_float2half_rz));

static_assert(HasConversionSignature<vector_half (*)(
    vector_float, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_float2half_rz));

static_assert(HasConversionSignature<vector_half (*)(
    vector_float, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_float2half_rz_sat));

static_assert(HasConversionSignature<vector_half (*)(
    vector_float, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_float2half_rz_sat));

static_assert(HasConversionSignature<vector_int16_t (*)(
    vector_uint32_t, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_uint322int16));

static_assert(HasConversionSignature<vector_int16_t (*)(
    vector_uint32_t, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_uint322int16));

static_assert(HasConversionSignature<vector_int16_t (*)(
    vector_uint32_t, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_uint322int16_sat));

static_assert(HasConversionSignature<vector_int16_t (*)(
    vector_uint32_t, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_uint322int16_sat));

static_assert(HasConversionSignature<vector_fp8_e5m2_t (*)(
    vector_float, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>)>(asc_float2e5m2_rn));

static_assert(HasConversionSignature<vector_fp8_e5m2_t (*)(
    vector_float, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>)>(asc_float2e5m2_rn));

static_assert(HasConversionSignature<vector_fp8_e5m2_t (*)(
    vector_float, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>)>(asc_float2e5m2_rn));

static_assert(HasConversionSignature<vector_fp8_e5m2_t (*)(
    vector_float, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>)>(asc_float2e5m2_rn));

static_assert(HasConversionSignature<vector_fp8_e5m2_t (*)(
    vector_float, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>)>(asc_float2e5m2_rn_sat));

static_assert(HasConversionSignature<vector_fp8_e5m2_t (*)(
    vector_float, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>)>(asc_float2e5m2_rn_sat));

static_assert(HasConversionSignature<vector_fp8_e5m2_t (*)(
    vector_float, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>)>(asc_float2e5m2_rn_sat));

static_assert(HasConversionSignature<vector_fp8_e5m2_t (*)(
    vector_float, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>)>(asc_float2e5m2_rn_sat));

static_assert(HasConversionSignature<vector_float (*)(
    vector_half, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_half2float));

static_assert(HasConversionSignature<vector_float (*)(
    vector_half, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_half2float));

static_assert(HasConversionSignature<vector_float (*)(
    vector_fp8_e4m3fn_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>)>(asc_e4m32float));

static_assert(HasConversionSignature<vector_float (*)(
    vector_fp8_e4m3fn_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>)>(asc_e4m32float));

static_assert(HasConversionSignature<vector_float (*)(
    vector_fp8_e4m3fn_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>)>(asc_e4m32float));

static_assert(HasConversionSignature<vector_float (*)(
    vector_fp8_e4m3fn_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>)>(asc_e4m32float));

static_assert(HasConversionSignature<vector_float (*)(
    vector_int16_t, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_int162float));

static_assert(HasConversionSignature<vector_float (*)(
    vector_int16_t, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_int162float));

static_assert(HasConversionSignature<vector_int16_t (*)(
    vector_int32_t, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_int322int16));

static_assert(HasConversionSignature<vector_int16_t (*)(
    vector_int32_t, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_int322int16));

static_assert(HasConversionSignature<vector_int16_t (*)(
    vector_int32_t, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_int322int16_sat));

static_assert(HasConversionSignature<vector_int16_t (*)(
    vector_int32_t, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_int322int16_sat));

static_assert(HasConversionSignature<vector_uint8_t (*)(
    vector_int32_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>)>(asc_int322uint8));

static_assert(HasConversionSignature<vector_uint8_t (*)(
    vector_int32_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>)>(asc_int322uint8));

static_assert(HasConversionSignature<vector_uint8_t (*)(
    vector_int32_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>)>(asc_int322uint8));

static_assert(HasConversionSignature<vector_uint8_t (*)(
    vector_int32_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>)>(asc_int322uint8));

static_assert(HasConversionSignature<vector_uint8_t (*)(
    vector_int32_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>)>(asc_int322uint8_sat));

static_assert(HasConversionSignature<vector_uint8_t (*)(
    vector_int32_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>)>(asc_int322uint8_sat));

static_assert(HasConversionSignature<vector_uint8_t (*)(
    vector_int32_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>)>(asc_int322uint8_sat));

static_assert(HasConversionSignature<vector_uint8_t (*)(
    vector_int32_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>)>(asc_int322uint8_sat));

static_assert(HasConversionSignature<vector_float (*)(
    vector_bfloat16_t, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_bfloat162float));

static_assert(HasConversionSignature<vector_float (*)(
    vector_bfloat16_t, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_bfloat162float));

static_assert(HasConversionSignature<vector_hifloat8_t (*)(
    vector_half, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_half2hif8_rh));

static_assert(HasConversionSignature<vector_hifloat8_t (*)(
    vector_half, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_half2hif8_rh));

static_assert(HasConversionSignature<vector_hifloat8_t (*)(
    vector_half, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_half2hif8_rh_sat));

static_assert(HasConversionSignature<vector_hifloat8_t (*)(
    vector_half, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_half2hif8_rh_sat));

static_assert(HasConversionSignature<vector_hifloat8_t (*)(
    vector_half, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_half2hif8_rna));

static_assert(HasConversionSignature<vector_hifloat8_t (*)(
    vector_half, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_half2hif8_rna));

static_assert(HasConversionSignature<vector_hifloat8_t (*)(
    vector_half, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_half2hif8_rna_sat));

static_assert(HasConversionSignature<vector_hifloat8_t (*)(
    vector_half, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_half2hif8_rna_sat));

static_assert(HasConversionSignature<vector_int8_t (*)(
    vector_half, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_half2int8_rd));

static_assert(HasConversionSignature<vector_int8_t (*)(
    vector_half, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_half2int8_rd));

static_assert(HasConversionSignature<vector_int8_t (*)(
    vector_half, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_half2int8_rd_sat));

static_assert(HasConversionSignature<vector_int8_t (*)(
    vector_half, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_half2int8_rd_sat));

static_assert(HasConversionSignature<vector_int8_t (*)(
    vector_half, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_half2int8_rn));

static_assert(HasConversionSignature<vector_int8_t (*)(
    vector_half, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_half2int8_rn));

static_assert(HasConversionSignature<vector_int8_t (*)(
    vector_half, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_half2int8_rn_sat));

static_assert(HasConversionSignature<vector_int8_t (*)(
    vector_half, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_half2int8_rn_sat));

static_assert(HasConversionSignature<vector_int8_t (*)(
    vector_half, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_half2int8_rna));

static_assert(HasConversionSignature<vector_int8_t (*)(
    vector_half, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_half2int8_rna));

static_assert(HasConversionSignature<vector_int8_t (*)(
    vector_half, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_half2int8_rna_sat));

static_assert(HasConversionSignature<vector_int8_t (*)(
    vector_half, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_half2int8_rna_sat));

static_assert(HasConversionSignature<vector_int8_t (*)(
    vector_half, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_half2int8_ru));

static_assert(HasConversionSignature<vector_int8_t (*)(
    vector_half, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_half2int8_ru));

static_assert(HasConversionSignature<vector_int8_t (*)(
    vector_half, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_half2int8_ru_sat));

static_assert(HasConversionSignature<vector_int8_t (*)(
    vector_half, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_half2int8_ru_sat));

static_assert(HasConversionSignature<vector_int8_t (*)(
    vector_half, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_half2int8_rz));

static_assert(HasConversionSignature<vector_int8_t (*)(
    vector_half, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_half2int8_rz));

static_assert(HasConversionSignature<vector_int8_t (*)(
    vector_half, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_half2int8_rz_sat));

static_assert(HasConversionSignature<vector_int8_t (*)(
    vector_half, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_half2int8_rz_sat));

static_assert(HasConversionSignature<vector_uint32_t (*)(
    vector_int16_t, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_int162uint32));

static_assert(HasConversionSignature<vector_uint32_t (*)(
    vector_int16_t, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_int162uint32));

static_assert(HasConversionSignature<vector_uint8_t (*)(
    vector_uint16_t, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_uint162uint8));

static_assert(HasConversionSignature<vector_uint8_t (*)(
    vector_uint16_t, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_uint162uint8));

static_assert(HasConversionSignature<vector_uint8_t (*)(
    vector_uint16_t, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_uint162uint8_sat));

static_assert(HasConversionSignature<vector_uint8_t (*)(
    vector_uint16_t, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_uint162uint8_sat));

static_assert(HasConversionSignature<vector_float (*)(
    vector_hifloat8_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>)>(asc_hif82float));

static_assert(HasConversionSignature<vector_float (*)(
    vector_hifloat8_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>)>(asc_hif82float));

static_assert(HasConversionSignature<vector_float (*)(
    vector_hifloat8_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>)>(asc_hif82float));

static_assert(HasConversionSignature<vector_float (*)(
    vector_hifloat8_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>)>(asc_hif82float));

static_assert(HasConversionSignature<vector_half (*)(
    vector_hifloat8_t, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_hif82half));

static_assert(HasConversionSignature<vector_half (*)(
    vector_hifloat8_t, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_hif82half));

static_assert(HasConversionSignature<vector_half (*)(
    vector_uint8_t, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_uint82half));

static_assert(HasConversionSignature<vector_half (*)(
    vector_uint8_t, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_uint82half));

static_assert(HasConversionSignature<vector_uint32_t (*)(
    vector_uint16_t, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_uint162uint32));

static_assert(HasConversionSignature<vector_uint32_t (*)(
    vector_uint16_t, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_uint162uint32));

static_assert(HasConversionSignature<vector_uint8_t (*)(
    vector_half, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_half2uint8_rd));

static_assert(HasConversionSignature<vector_uint8_t (*)(
    vector_half, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_half2uint8_rd));

static_assert(HasConversionSignature<vector_uint8_t (*)(
    vector_half, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_half2uint8_rd_sat));

static_assert(HasConversionSignature<vector_uint8_t (*)(
    vector_half, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_half2uint8_rd_sat));

static_assert(HasConversionSignature<vector_uint8_t (*)(
    vector_half, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_half2uint8_rn));

static_assert(HasConversionSignature<vector_uint8_t (*)(
    vector_half, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_half2uint8_rn));

static_assert(HasConversionSignature<vector_uint8_t (*)(
    vector_half, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_half2uint8_rn_sat));

static_assert(HasConversionSignature<vector_uint8_t (*)(
    vector_half, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_half2uint8_rn_sat));

static_assert(HasConversionSignature<vector_uint8_t (*)(
    vector_half, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_half2uint8_rna));

static_assert(HasConversionSignature<vector_uint8_t (*)(
    vector_half, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_half2uint8_rna));

static_assert(HasConversionSignature<vector_uint8_t (*)(
    vector_half, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_half2uint8_rna_sat));

static_assert(HasConversionSignature<vector_uint8_t (*)(
    vector_half, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_half2uint8_rna_sat));

static_assert(HasConversionSignature<vector_uint8_t (*)(
    vector_half, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_half2uint8_ru));

static_assert(HasConversionSignature<vector_uint8_t (*)(
    vector_half, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_half2uint8_ru));

static_assert(HasConversionSignature<vector_uint8_t (*)(
    vector_half, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_half2uint8_ru_sat));

static_assert(HasConversionSignature<vector_uint8_t (*)(
    vector_half, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_half2uint8_ru_sat));

static_assert(HasConversionSignature<vector_uint8_t (*)(
    vector_half, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_half2uint8_rz));

static_assert(HasConversionSignature<vector_uint8_t (*)(
    vector_half, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_half2uint8_rz));

static_assert(HasConversionSignature<vector_uint8_t (*)(
    vector_half, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_half2uint8_rz_sat));

static_assert(HasConversionSignature<vector_uint8_t (*)(
    vector_half, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_half2uint8_rz_sat));

static_assert(HasConversionSignature<vector_int16_t (*)(
    vector_int8_t, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_int82int16));

static_assert(HasConversionSignature<vector_int16_t (*)(
    vector_int8_t, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_int82int16));

static_assert(HasConversionSignature<vector_uint8_t (*)(
    vector_int16_t, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_int162uint8));

static_assert(HasConversionSignature<vector_uint8_t (*)(
    vector_int16_t, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_int162uint8));

static_assert(HasConversionSignature<vector_uint8_t (*)(
    vector_int16_t, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_int162uint8_sat));

static_assert(HasConversionSignature<vector_uint8_t (*)(
    vector_int16_t, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_int162uint8_sat));

static_assert(HasConversionSignature<vector_int32_t (*)(
    vector_bfloat16_t, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_bfloat162int32_rn));

static_assert(HasConversionSignature<vector_int32_t (*)(
    vector_bfloat16_t, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_bfloat162int32_rn));

static_assert(HasConversionSignature<vector_int32_t (*)(
    vector_bfloat16_t, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_bfloat162int32_rn_sat));

static_assert(HasConversionSignature<vector_int32_t (*)(
    vector_bfloat16_t, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_bfloat162int32_rn_sat));

static_assert(HasConversionSignature<vector_int32_t (*)(
    vector_bfloat16_t, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_bfloat162int32_rna));

static_assert(HasConversionSignature<vector_int32_t (*)(
    vector_bfloat16_t, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_bfloat162int32_rna));

static_assert(HasConversionSignature<vector_int32_t (*)(
    vector_bfloat16_t, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_bfloat162int32_rna_sat));

static_assert(HasConversionSignature<vector_int32_t (*)(
    vector_bfloat16_t, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_bfloat162int32_rna_sat));

static_assert(HasConversionSignature<vector_int32_t (*)(
    vector_bfloat16_t, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_bfloat162int32_rd));

static_assert(HasConversionSignature<vector_int32_t (*)(
    vector_bfloat16_t, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_bfloat162int32_rd));

static_assert(HasConversionSignature<vector_int32_t (*)(
    vector_bfloat16_t, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_bfloat162int32_rd_sat));

static_assert(HasConversionSignature<vector_int32_t (*)(
    vector_bfloat16_t, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_bfloat162int32_rd_sat));

static_assert(HasConversionSignature<vector_int32_t (*)(
    vector_bfloat16_t, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_bfloat162int32_ru));

static_assert(HasConversionSignature<vector_int32_t (*)(
    vector_bfloat16_t, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_bfloat162int32_ru));

static_assert(HasConversionSignature<vector_int32_t (*)(
    vector_bfloat16_t, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_bfloat162int32_ru_sat));

static_assert(HasConversionSignature<vector_int32_t (*)(
    vector_bfloat16_t, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_bfloat162int32_ru_sat));

static_assert(HasConversionSignature<vector_int32_t (*)(
    vector_bfloat16_t, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_bfloat162int32_rz));

static_assert(HasConversionSignature<vector_int32_t (*)(
    vector_bfloat16_t, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_bfloat162int32_rz));

static_assert(HasConversionSignature<vector_int32_t (*)(
    vector_bfloat16_t, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_bfloat162int32_rz_sat));

static_assert(HasConversionSignature<vector_int32_t (*)(
    vector_bfloat16_t, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_bfloat162int32_rz_sat));

static_assert(HasConversionSignature<vector_float (*)(
    vector_int64_t, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_int642float_rn));

static_assert(HasConversionSignature<vector_float (*)(
    vector_int64_t, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_int642float_rn));

static_assert(HasConversionSignature<vector_float (*)(
    vector_int64_t, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_int642float_rna));

static_assert(HasConversionSignature<vector_float (*)(
    vector_int64_t, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_int642float_rna));

static_assert(HasConversionSignature<vector_float (*)(
    vector_int64_t, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_int642float_rd));

static_assert(HasConversionSignature<vector_float (*)(
    vector_int64_t, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_int642float_rd));

static_assert(HasConversionSignature<vector_float (*)(
    vector_int64_t, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_int642float_ru));

static_assert(HasConversionSignature<vector_float (*)(
    vector_int64_t, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_int642float_ru));

static_assert(HasConversionSignature<vector_float (*)(
    vector_int64_t, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_int642float_rz));

static_assert(HasConversionSignature<vector_float (*)(
    vector_int64_t, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_int642float_rz));

static_assert(HasConversionSignature<vector_half (*)(
    vector_int8_t, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_int82half));

static_assert(HasConversionSignature<vector_half (*)(
    vector_int8_t, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_int82half));

static_assert(HasConversionSignature<vector_int32_t (*)(
    vector_int16_t, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_int162int32));

static_assert(HasConversionSignature<vector_int32_t (*)(
    vector_int16_t, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_int162int32));

static_assert(HasConversionSignature<vector_uint8_t (*)(
    vector_uint32_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>)>(asc_uint322uint8));

static_assert(HasConversionSignature<vector_uint8_t (*)(
    vector_uint32_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>)>(asc_uint322uint8));

static_assert(HasConversionSignature<vector_uint8_t (*)(
    vector_uint32_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>)>(asc_uint322uint8));

static_assert(HasConversionSignature<vector_uint8_t (*)(
    vector_uint32_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>)>(asc_uint322uint8));

static_assert(HasConversionSignature<vector_uint8_t (*)(
    vector_uint32_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>)>(asc_uint322uint8_sat));

static_assert(HasConversionSignature<vector_uint8_t (*)(
    vector_uint32_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>)>(asc_uint322uint8_sat));

static_assert(HasConversionSignature<vector_uint8_t (*)(
    vector_uint32_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>)>(asc_uint322uint8_sat));

static_assert(HasConversionSignature<vector_uint8_t (*)(
    vector_uint32_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>)>(asc_uint322uint8_sat));

static_assert(HasConversionSignature<vector_int16_t (*)(
    vector_float, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_float2int16_rd));

static_assert(HasConversionSignature<vector_int16_t (*)(
    vector_float, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_float2int16_rd));

static_assert(HasConversionSignature<vector_int16_t (*)(
    vector_float, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_float2int16_rd_sat));

static_assert(HasConversionSignature<vector_int16_t (*)(
    vector_float, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_float2int16_rd_sat));

static_assert(HasConversionSignature<vector_int16_t (*)(
    vector_float, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_float2int16_rn));

static_assert(HasConversionSignature<vector_int16_t (*)(
    vector_float, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_float2int16_rn));

static_assert(HasConversionSignature<vector_int16_t (*)(
    vector_float, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_float2int16_rn_sat));

static_assert(HasConversionSignature<vector_int16_t (*)(
    vector_float, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_float2int16_rn_sat));

static_assert(HasConversionSignature<vector_int16_t (*)(
    vector_float, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_float2int16_rna));

static_assert(HasConversionSignature<vector_int16_t (*)(
    vector_float, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_float2int16_rna));

static_assert(HasConversionSignature<vector_int16_t (*)(
    vector_float, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_float2int16_rna_sat));

static_assert(HasConversionSignature<vector_int16_t (*)(
    vector_float, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_float2int16_rna_sat));

static_assert(HasConversionSignature<vector_int16_t (*)(
    vector_float, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_float2int16_ru));

static_assert(HasConversionSignature<vector_int16_t (*)(
    vector_float, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_float2int16_ru));

static_assert(HasConversionSignature<vector_int16_t (*)(
    vector_float, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_float2int16_ru_sat));

static_assert(HasConversionSignature<vector_int16_t (*)(
    vector_float, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_float2int16_ru_sat));

static_assert(HasConversionSignature<vector_int16_t (*)(
    vector_float, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_float2int16_rz));

static_assert(HasConversionSignature<vector_int16_t (*)(
    vector_float, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_float2int16_rz));

static_assert(HasConversionSignature<vector_int16_t (*)(
    vector_float, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_float2int16_rz_sat));

static_assert(HasConversionSignature<vector_int16_t (*)(
    vector_float, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_float2int16_rz_sat));

static_assert(HasConversionSignature<vector_fp4x2_e2m1_t (*)(
    vector_bfloat16_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>)>(asc_bfloat162e2m1x2_rd));

static_assert(HasConversionSignature<vector_fp4x2_e2m1_t (*)(
    vector_bfloat16_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>)>(asc_bfloat162e2m1x2_rd));

static_assert(HasConversionSignature<vector_fp4x2_e2m1_t (*)(
    vector_bfloat16_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>)>(asc_bfloat162e2m1x2_rd));

static_assert(HasConversionSignature<vector_fp4x2_e2m1_t (*)(
    vector_bfloat16_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>)>(asc_bfloat162e2m1x2_rd));

static_assert(HasConversionSignature<vector_fp4x2_e2m1_t (*)(
    vector_bfloat16_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>)>(asc_bfloat162e2m1x2_rn));

static_assert(HasConversionSignature<vector_fp4x2_e2m1_t (*)(
    vector_bfloat16_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>)>(asc_bfloat162e2m1x2_rn));

static_assert(HasConversionSignature<vector_fp4x2_e2m1_t (*)(
    vector_bfloat16_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>)>(asc_bfloat162e2m1x2_rn));

static_assert(HasConversionSignature<vector_fp4x2_e2m1_t (*)(
    vector_bfloat16_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>)>(asc_bfloat162e2m1x2_rn));

static_assert(HasConversionSignature<vector_fp4x2_e2m1_t (*)(
    vector_bfloat16_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>)>(asc_bfloat162e2m1x2_rna));

static_assert(HasConversionSignature<vector_fp4x2_e2m1_t (*)(
    vector_bfloat16_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>)>(asc_bfloat162e2m1x2_rna));

static_assert(HasConversionSignature<vector_fp4x2_e2m1_t (*)(
    vector_bfloat16_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>)>(asc_bfloat162e2m1x2_rna));

static_assert(HasConversionSignature<vector_fp4x2_e2m1_t (*)(
    vector_bfloat16_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>)>(asc_bfloat162e2m1x2_rna));

static_assert(HasConversionSignature<vector_fp4x2_e2m1_t (*)(
    vector_bfloat16_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>)>(asc_bfloat162e2m1x2_ru));

static_assert(HasConversionSignature<vector_fp4x2_e2m1_t (*)(
    vector_bfloat16_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>)>(asc_bfloat162e2m1x2_ru));

static_assert(HasConversionSignature<vector_fp4x2_e2m1_t (*)(
    vector_bfloat16_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>)>(asc_bfloat162e2m1x2_ru));

static_assert(HasConversionSignature<vector_fp4x2_e2m1_t (*)(
    vector_bfloat16_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>)>(asc_bfloat162e2m1x2_ru));

static_assert(HasConversionSignature<vector_fp4x2_e2m1_t (*)(
    vector_bfloat16_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>)>(asc_bfloat162e2m1x2_rz));

static_assert(HasConversionSignature<vector_fp4x2_e2m1_t (*)(
    vector_bfloat16_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>)>(asc_bfloat162e2m1x2_rz));

static_assert(HasConversionSignature<vector_fp4x2_e2m1_t (*)(
    vector_bfloat16_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>)>(asc_bfloat162e2m1x2_rz));

static_assert(HasConversionSignature<vector_fp4x2_e2m1_t (*)(
    vector_bfloat16_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>)>(asc_bfloat162e2m1x2_rz));

static_assert(HasConversionSignature<vector_hifloat8_t (*)(
    vector_float, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>)>(asc_float2hif8_rh));

static_assert(HasConversionSignature<vector_hifloat8_t (*)(
    vector_float, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>)>(asc_float2hif8_rh));

static_assert(HasConversionSignature<vector_hifloat8_t (*)(
    vector_float, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>)>(asc_float2hif8_rh));

static_assert(HasConversionSignature<vector_hifloat8_t (*)(
    vector_float, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>)>(asc_float2hif8_rh));

static_assert(HasConversionSignature<vector_hifloat8_t (*)(
    vector_float, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>)>(asc_float2hif8_rh_sat));

static_assert(HasConversionSignature<vector_hifloat8_t (*)(
    vector_float, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>)>(asc_float2hif8_rh_sat));

static_assert(HasConversionSignature<vector_hifloat8_t (*)(
    vector_float, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>)>(asc_float2hif8_rh_sat));

static_assert(HasConversionSignature<vector_hifloat8_t (*)(
    vector_float, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>)>(asc_float2hif8_rh_sat));

static_assert(HasConversionSignature<vector_hifloat8_t (*)(
    vector_float, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>)>(asc_float2hif8_rna));

static_assert(HasConversionSignature<vector_hifloat8_t (*)(
    vector_float, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>)>(asc_float2hif8_rna));

static_assert(HasConversionSignature<vector_hifloat8_t (*)(
    vector_float, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>)>(asc_float2hif8_rna));

static_assert(HasConversionSignature<vector_hifloat8_t (*)(
    vector_float, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>)>(asc_float2hif8_rna));

static_assert(HasConversionSignature<vector_hifloat8_t (*)(
    vector_float, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>)>(asc_float2hif8_rna_sat));

static_assert(HasConversionSignature<vector_hifloat8_t (*)(
    vector_float, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>)>(asc_float2hif8_rna_sat));

static_assert(HasConversionSignature<vector_hifloat8_t (*)(
    vector_float, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>)>(asc_float2hif8_rna_sat));

static_assert(HasConversionSignature<vector_hifloat8_t (*)(
    vector_float, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>)>(asc_float2hif8_rna_sat));

static_assert(HasConversionSignature<vector_bfloat16_t (*)(
    vector_fp4x2_e2m1_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>)>(asc_e2m1x22bfloat16));

static_assert(HasConversionSignature<vector_bfloat16_t (*)(
    vector_fp4x2_e2m1_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>)>(asc_e2m1x22bfloat16));

static_assert(HasConversionSignature<vector_bfloat16_t (*)(
    vector_fp4x2_e2m1_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>)>(asc_e2m1x22bfloat16));

static_assert(HasConversionSignature<vector_bfloat16_t (*)(
    vector_fp4x2_e2m1_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>)>(asc_e2m1x22bfloat16));

static_assert(HasConversionSignature<vector_uint32_t (*)(
    vector_uint8_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>)>(asc_uint82uint32));

static_assert(HasConversionSignature<vector_uint32_t (*)(
    vector_uint8_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>)>(asc_uint82uint32));

static_assert(HasConversionSignature<vector_uint32_t (*)(
    vector_uint8_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>)>(asc_uint82uint32));

static_assert(HasConversionSignature<vector_uint32_t (*)(
    vector_uint8_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>)>(asc_uint82uint32));

static_assert(HasConversionSignature<vector_uint16_t (*)(
    vector_uint32_t, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_uint322uint16));

static_assert(HasConversionSignature<vector_uint16_t (*)(
    vector_uint32_t, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_uint322uint16));

static_assert(HasConversionSignature<vector_uint16_t (*)(
    vector_uint32_t, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_uint322uint16_sat));

static_assert(HasConversionSignature<vector_uint16_t (*)(
    vector_uint32_t, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_uint322uint16_sat));

static_assert(HasConversionSignature<vector_bfloat16_t (*)(
    vector_float, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_float2bfloat16_rd));

static_assert(HasConversionSignature<vector_bfloat16_t (*)(
    vector_float, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_float2bfloat16_rd));

static_assert(HasConversionSignature<vector_bfloat16_t (*)(
    vector_float, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_float2bfloat16_rd_sat));

static_assert(HasConversionSignature<vector_bfloat16_t (*)(
    vector_float, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_float2bfloat16_rd_sat));

static_assert(HasConversionSignature<vector_bfloat16_t (*)(
    vector_float, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_float2bfloat16_rn));

static_assert(HasConversionSignature<vector_bfloat16_t (*)(
    vector_float, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_float2bfloat16_rn));

static_assert(HasConversionSignature<vector_bfloat16_t (*)(
    vector_float, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_float2bfloat16_rn_sat));

static_assert(HasConversionSignature<vector_bfloat16_t (*)(
    vector_float, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_float2bfloat16_rn_sat));

static_assert(HasConversionSignature<vector_bfloat16_t (*)(
    vector_float, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_float2bfloat16_rna));

static_assert(HasConversionSignature<vector_bfloat16_t (*)(
    vector_float, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_float2bfloat16_rna));

static_assert(HasConversionSignature<vector_bfloat16_t (*)(
    vector_float, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_float2bfloat16_rna_sat));

static_assert(HasConversionSignature<vector_bfloat16_t (*)(
    vector_float, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_float2bfloat16_rna_sat));

static_assert(HasConversionSignature<vector_bfloat16_t (*)(
    vector_float, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_float2bfloat16_ru));

static_assert(HasConversionSignature<vector_bfloat16_t (*)(
    vector_float, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_float2bfloat16_ru));

static_assert(HasConversionSignature<vector_bfloat16_t (*)(
    vector_float, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_float2bfloat16_ru_sat));

static_assert(HasConversionSignature<vector_bfloat16_t (*)(
    vector_float, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_float2bfloat16_ru_sat));

static_assert(HasConversionSignature<vector_bfloat16_t (*)(
    vector_float, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_float2bfloat16_rz));

static_assert(HasConversionSignature<vector_bfloat16_t (*)(
    vector_float, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_float2bfloat16_rz));

static_assert(HasConversionSignature<vector_bfloat16_t (*)(
    vector_float, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_float2bfloat16_rz_sat));

static_assert(HasConversionSignature<vector_bfloat16_t (*)(
    vector_float, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_float2bfloat16_rz_sat));

static_assert(HasConversionSignature<vector_int64_t (*)(
    vector_float, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_float2int64_rd));

static_assert(HasConversionSignature<vector_int64_t (*)(
    vector_float, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_float2int64_rd));

static_assert(HasConversionSignature<vector_int64_t (*)(
    vector_float, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_float2int64_rd_sat));

static_assert(HasConversionSignature<vector_int64_t (*)(
    vector_float, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_float2int64_rd_sat));

static_assert(HasConversionSignature<vector_int64_t (*)(
    vector_float, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_float2int64_rn));

static_assert(HasConversionSignature<vector_int64_t (*)(
    vector_float, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_float2int64_rn));

static_assert(HasConversionSignature<vector_int64_t (*)(
    vector_float, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_float2int64_rn_sat));

static_assert(HasConversionSignature<vector_int64_t (*)(
    vector_float, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_float2int64_rn_sat));

static_assert(HasConversionSignature<vector_int64_t (*)(
    vector_float, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_float2int64_rna));

static_assert(HasConversionSignature<vector_int64_t (*)(
    vector_float, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_float2int64_rna));

static_assert(HasConversionSignature<vector_int64_t (*)(
    vector_float, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_float2int64_rna_sat));

static_assert(HasConversionSignature<vector_int64_t (*)(
    vector_float, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_float2int64_rna_sat));

static_assert(HasConversionSignature<vector_int64_t (*)(
    vector_float, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_float2int64_ru));

static_assert(HasConversionSignature<vector_int64_t (*)(
    vector_float, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_float2int64_ru));

static_assert(HasConversionSignature<vector_int64_t (*)(
    vector_float, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_float2int64_ru_sat));

static_assert(HasConversionSignature<vector_int64_t (*)(
    vector_float, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_float2int64_ru_sat));

static_assert(HasConversionSignature<vector_int64_t (*)(
    vector_float, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_float2int64_rz));

static_assert(HasConversionSignature<vector_int64_t (*)(
    vector_float, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_float2int64_rz));

static_assert(HasConversionSignature<vector_int64_t (*)(
    vector_float, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_float2int64_rz_sat));

static_assert(HasConversionSignature<vector_int64_t (*)(
    vector_float, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_float2int64_rz_sat));

static_assert(HasConversionSignature<vector_int32_t (*)(
    vector_half, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_half2int32_rd));

static_assert(HasConversionSignature<vector_int32_t (*)(
    vector_half, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_half2int32_rd));

static_assert(HasConversionSignature<vector_int32_t (*)(
    vector_half, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_half2int32_rn));

static_assert(HasConversionSignature<vector_int32_t (*)(
    vector_half, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_half2int32_rn));

static_assert(HasConversionSignature<vector_int32_t (*)(
    vector_half, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_half2int32_rna));

static_assert(HasConversionSignature<vector_int32_t (*)(
    vector_half, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_half2int32_rna));

static_assert(HasConversionSignature<vector_int32_t (*)(
    vector_half, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_half2int32_ru));

static_assert(HasConversionSignature<vector_int32_t (*)(
    vector_half, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_half2int32_ru));

static_assert(HasConversionSignature<vector_int32_t (*)(
    vector_half, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_half2int32_rz));

static_assert(HasConversionSignature<vector_int32_t (*)(
    vector_half, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_half2int32_rz));

static_assert(HasConversionSignature<vector_float (*)(
    vector_fp8_e5m2_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>)>(asc_e5m22float));

static_assert(HasConversionSignature<vector_float (*)(
    vector_fp8_e5m2_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>)>(asc_e5m22float));

static_assert(HasConversionSignature<vector_float (*)(
    vector_fp8_e5m2_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>)>(asc_e5m22float));

static_assert(HasConversionSignature<vector_float (*)(
    vector_fp8_e5m2_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>)>(asc_e5m22float));

static_assert(HasConversionSignature<vector_fp8_e4m3fn_t (*)(
    vector_float, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>)>(asc_float2e4m3_rn));

static_assert(HasConversionSignature<vector_fp8_e4m3fn_t (*)(
    vector_float, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>)>(asc_float2e4m3_rn));

static_assert(HasConversionSignature<vector_fp8_e4m3fn_t (*)(
    vector_float, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>)>(asc_float2e4m3_rn));

static_assert(HasConversionSignature<vector_fp8_e4m3fn_t (*)(
    vector_float, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>)>(asc_float2e4m3_rn));

static_assert(HasConversionSignature<vector_fp8_e4m3fn_t (*)(
    vector_float, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>)>(asc_float2e4m3_rn_sat));

static_assert(HasConversionSignature<vector_fp8_e4m3fn_t (*)(
    vector_float, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>)>(asc_float2e4m3_rn_sat));

static_assert(HasConversionSignature<vector_fp8_e4m3fn_t (*)(
    vector_float, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>)>(asc_float2e4m3_rn_sat));

static_assert(HasConversionSignature<vector_fp8_e4m3fn_t (*)(
    vector_float, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>)>(asc_float2e4m3_rn_sat));

static_assert(HasConversionSignature<vector_int64_t (*)(
    vector_int32_t, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_int322int64));

static_assert(HasConversionSignature<vector_int64_t (*)(
    vector_int32_t, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_int322int64));

static_assert(HasConversionSignature<vector_uint16_t (*)(
    vector_int32_t, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_int322uint16));

static_assert(HasConversionSignature<vector_uint16_t (*)(
    vector_int32_t, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_int322uint16));

static_assert(HasConversionSignature<vector_uint16_t (*)(
    vector_int32_t, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_int322uint16_sat));

static_assert(HasConversionSignature<vector_uint16_t (*)(
    vector_int32_t, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_int322uint16_sat));

static_assert(HasConversionSignature<vector_int32_t (*)(
    vector_int64_t, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_int642int32));

static_assert(HasConversionSignature<vector_int32_t (*)(
    vector_int64_t, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_int642int32));

static_assert(HasConversionSignature<vector_int32_t (*)(
    vector_int64_t, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_int642int32_sat));

static_assert(HasConversionSignature<vector_int32_t (*)(
    vector_int64_t, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_int642int32_sat));

static_assert(HasConversionSignature<vector_uint16_t (*)(
    vector_uint8_t, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_uint82uint16));

static_assert(HasConversionSignature<vector_uint16_t (*)(
    vector_uint8_t, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_uint82uint16));

static_assert(HasConversionSignature<vector_fp4x2_e1m2_t (*)(
    vector_bfloat16_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>)>(asc_bfloat162e1m2x2_rd));

static_assert(HasConversionSignature<vector_fp4x2_e1m2_t (*)(
    vector_bfloat16_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>)>(asc_bfloat162e1m2x2_rd));

static_assert(HasConversionSignature<vector_fp4x2_e1m2_t (*)(
    vector_bfloat16_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>)>(asc_bfloat162e1m2x2_rd));

static_assert(HasConversionSignature<vector_fp4x2_e1m2_t (*)(
    vector_bfloat16_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>)>(asc_bfloat162e1m2x2_rd));

static_assert(HasConversionSignature<vector_fp4x2_e1m2_t (*)(
    vector_bfloat16_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>)>(asc_bfloat162e1m2x2_rn));

static_assert(HasConversionSignature<vector_fp4x2_e1m2_t (*)(
    vector_bfloat16_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>)>(asc_bfloat162e1m2x2_rn));

static_assert(HasConversionSignature<vector_fp4x2_e1m2_t (*)(
    vector_bfloat16_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>)>(asc_bfloat162e1m2x2_rn));

static_assert(HasConversionSignature<vector_fp4x2_e1m2_t (*)(
    vector_bfloat16_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>)>(asc_bfloat162e1m2x2_rn));

static_assert(HasConversionSignature<vector_fp4x2_e1m2_t (*)(
    vector_bfloat16_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>)>(asc_bfloat162e1m2x2_rna));

static_assert(HasConversionSignature<vector_fp4x2_e1m2_t (*)(
    vector_bfloat16_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>)>(asc_bfloat162e1m2x2_rna));

static_assert(HasConversionSignature<vector_fp4x2_e1m2_t (*)(
    vector_bfloat16_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>)>(asc_bfloat162e1m2x2_rna));

static_assert(HasConversionSignature<vector_fp4x2_e1m2_t (*)(
    vector_bfloat16_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>)>(asc_bfloat162e1m2x2_rna));

static_assert(HasConversionSignature<vector_fp4x2_e1m2_t (*)(
    vector_bfloat16_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>)>(asc_bfloat162e1m2x2_ru));

static_assert(HasConversionSignature<vector_fp4x2_e1m2_t (*)(
    vector_bfloat16_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>)>(asc_bfloat162e1m2x2_ru));

static_assert(HasConversionSignature<vector_fp4x2_e1m2_t (*)(
    vector_bfloat16_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>)>(asc_bfloat162e1m2x2_ru));

static_assert(HasConversionSignature<vector_fp4x2_e1m2_t (*)(
    vector_bfloat16_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>)>(asc_bfloat162e1m2x2_ru));

static_assert(HasConversionSignature<vector_fp4x2_e1m2_t (*)(
    vector_bfloat16_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>)>(asc_bfloat162e1m2x2_rz));

static_assert(HasConversionSignature<vector_fp4x2_e1m2_t (*)(
    vector_bfloat16_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>)>(asc_bfloat162e1m2x2_rz));

static_assert(HasConversionSignature<vector_fp4x2_e1m2_t (*)(
    vector_bfloat16_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>)>(asc_bfloat162e1m2x2_rz));

static_assert(HasConversionSignature<vector_fp4x2_e1m2_t (*)(
    vector_bfloat16_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>)>(asc_bfloat162e1m2x2_rz));

static_assert(HasConversionSignature<vector_bfloat16_t (*)(
    vector_fp4x2_e1m2_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>)>(asc_e1m2x22bfloat16));

static_assert(HasConversionSignature<vector_bfloat16_t (*)(
    vector_fp4x2_e1m2_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>)>(asc_e1m2x22bfloat16));

static_assert(HasConversionSignature<vector_bfloat16_t (*)(
    vector_fp4x2_e1m2_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>)>(asc_e1m2x22bfloat16));

static_assert(HasConversionSignature<vector_bfloat16_t (*)(
    vector_fp4x2_e1m2_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>)>(asc_e1m2x22bfloat16));

static_assert(HasConversionSignature<vector_int32_t (*)(
    vector_int8_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>)>(asc_int82int32));

static_assert(HasConversionSignature<vector_int32_t (*)(
    vector_int8_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>)>(asc_int82int32));

static_assert(HasConversionSignature<vector_int32_t (*)(
    vector_int8_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>)>(asc_int82int32));

static_assert(HasConversionSignature<vector_int32_t (*)(
    vector_int8_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>)>(asc_int82int32));

static_assert(HasConversionSignature<vector_bfloat16_t (*)(vector_half, vector_bool)>(asc_half2bfloat16_rd));

static_assert(HasConversionSignature<vector_bfloat16_t (*)(vector_half, vector_bool)>(asc_half2bfloat16_rn));

static_assert(HasConversionSignature<vector_bfloat16_t (*)(vector_half, vector_bool)>(asc_half2bfloat16_rna));

static_assert(HasConversionSignature<vector_bfloat16_t (*)(vector_half, vector_bool)>(asc_half2bfloat16_ru));

static_assert(HasConversionSignature<vector_bfloat16_t (*)(vector_half, vector_bool)>(asc_half2bfloat16_rz));

static_assert(HasConversionSignature<vector_half (*)(vector_bfloat16_t, vector_bool)>(asc_bfloat162half_rd));

static_assert(HasConversionSignature<vector_half (*)(vector_bfloat16_t, vector_bool)>(asc_bfloat162half_rd_sat));

static_assert(HasConversionSignature<vector_half (*)(vector_bfloat16_t, vector_bool)>(asc_bfloat162half_ru));

static_assert(HasConversionSignature<vector_half (*)(vector_bfloat16_t, vector_bool)>(asc_bfloat162half_ru_sat));

static_assert(HasConversionSignature<vector_half (*)(vector_bfloat16_t, vector_bool)>(asc_bfloat162half_rz));

static_assert(HasConversionSignature<vector_half (*)(vector_bfloat16_t, vector_bool)>(asc_bfloat162half_rz_sat));

static_assert(HasConversionSignature<vector_half (*)(vector_bfloat16_t, vector_bool)>(asc_bfloat162half_rn));

static_assert(HasConversionSignature<vector_half (*)(vector_bfloat16_t, vector_bool)>(asc_bfloat162half_rn_sat));

static_assert(HasConversionSignature<vector_half (*)(vector_bfloat16_t, vector_bool)>(asc_bfloat162half_rna));

static_assert(HasConversionSignature<vector_half (*)(vector_bfloat16_t, vector_bool)>(asc_bfloat162half_rna_sat));

static_assert(HasConversionSignature<vector_int16_t (*)(vector_half, vector_bool)>(asc_half2int16_rn));

static_assert(HasConversionSignature<vector_int16_t (*)(vector_half, vector_bool)>(asc_half2int16_rn_sat));

static_assert(HasConversionSignature<vector_int16_t (*)(vector_half, vector_bool)>(asc_half2int16_rna));

static_assert(HasConversionSignature<vector_int16_t (*)(vector_half, vector_bool)>(asc_half2int16_rna_sat));

static_assert(HasConversionSignature<vector_int16_t (*)(vector_half, vector_bool)>(asc_half2int16_rd));

static_assert(HasConversionSignature<vector_int16_t (*)(vector_half, vector_bool)>(asc_half2int16_rd_sat));

static_assert(HasConversionSignature<vector_int16_t (*)(vector_half, vector_bool)>(asc_half2int16_ru));

static_assert(HasConversionSignature<vector_int16_t (*)(vector_half, vector_bool)>(asc_half2int16_ru_sat));

static_assert(HasConversionSignature<vector_int16_t (*)(vector_half, vector_bool)>(asc_half2int16_rz));

static_assert(HasConversionSignature<vector_int16_t (*)(vector_half, vector_bool)>(asc_half2int16_rz_sat));

static_assert(HasConversionSignature<vector_float (*)(vector_int32_t, vector_bool)>(asc_int322float_rd));

static_assert(HasConversionSignature<vector_float (*)(vector_int32_t, vector_bool)>(asc_int322float_ru));

static_assert(HasConversionSignature<vector_float (*)(vector_int32_t, vector_bool)>(asc_int322float_rz));

static_assert(HasConversionSignature<vector_float (*)(vector_int32_t, vector_bool)>(asc_int322float_rn));

static_assert(HasConversionSignature<vector_float (*)(vector_int32_t, vector_bool)>(asc_int322float_rna));

static_assert(HasConversionSignature<vector_half (*)(vector_int16_t, vector_bool)>(asc_int162half_rd));

static_assert(HasConversionSignature<vector_half (*)(vector_int16_t, vector_bool)>(asc_int162half_rn));

static_assert(HasConversionSignature<vector_half (*)(vector_int16_t, vector_bool)>(asc_int162half_rna));

static_assert(HasConversionSignature<vector_half (*)(vector_int16_t, vector_bool)>(asc_int162half_ru));

static_assert(HasConversionSignature<vector_half (*)(vector_int16_t, vector_bool)>(asc_int162half_rz));

static_assert(HasConversionSignature<vector_int32_t (*)(vector_float, vector_bool)>(asc_float2int32_rd));

static_assert(HasConversionSignature<vector_int32_t (*)(vector_float, vector_bool)>(asc_float2int32_rd_sat));

static_assert(HasConversionSignature<vector_int32_t (*)(vector_float, vector_bool)>(asc_float2int32_ru));

static_assert(HasConversionSignature<vector_int32_t (*)(vector_float, vector_bool)>(asc_float2int32_ru_sat));

static_assert(HasConversionSignature<vector_int32_t (*)(vector_float, vector_bool)>(asc_float2int32_rz));

static_assert(HasConversionSignature<vector_int32_t (*)(vector_float, vector_bool)>(asc_float2int32_rz_sat));

static_assert(HasConversionSignature<vector_int32_t (*)(vector_float, vector_bool)>(asc_float2int32_rn));

static_assert(HasConversionSignature<vector_int32_t (*)(vector_float, vector_bool)>(asc_float2int32_rn_sat));

static_assert(HasConversionSignature<vector_int32_t (*)(vector_float, vector_bool)>(asc_float2int32_rna));

static_assert(HasConversionSignature<vector_int32_t (*)(vector_float, vector_bool)>(asc_float2int32_rna_sat));

static_assert(HasConversionSignature<vector_half (*)(vector_half, vector_bool)>(asc_ceil));

static_assert(HasConversionSignature<vector_bfloat16_t (*)(vector_bfloat16_t, vector_bool)>(asc_ceil));

static_assert(HasConversionSignature<vector_float (*)(vector_float, vector_bool)>(asc_ceil));

static_assert(HasConversionSignature<vector_half (*)(vector_half, vector_bool)>(asc_floor));

static_assert(HasConversionSignature<vector_bfloat16_t (*)(vector_bfloat16_t, vector_bool)>(asc_floor));

static_assert(HasConversionSignature<vector_float (*)(vector_float, vector_bool)>(asc_floor));

static_assert(HasConversionSignature<vector_half (*)(vector_half, vector_bool)>(asc_rint));

static_assert(HasConversionSignature<vector_bfloat16_t (*)(vector_bfloat16_t, vector_bool)>(asc_rint));

static_assert(HasConversionSignature<vector_float (*)(vector_float, vector_bool)>(asc_rint));

static_assert(HasConversionSignature<vector_half (*)(vector_half, vector_bool)>(asc_round));

static_assert(HasConversionSignature<vector_bfloat16_t (*)(vector_bfloat16_t, vector_bool)>(asc_round));

static_assert(HasConversionSignature<vector_float (*)(vector_float, vector_bool)>(asc_round));

static_assert(HasConversionSignature<vector_half (*)(vector_half, vector_bool)>(asc_trunc));

static_assert(HasConversionSignature<vector_bfloat16_t (*)(vector_bfloat16_t, vector_bool)>(asc_trunc));

static_assert(HasConversionSignature<vector_float (*)(vector_float, vector_bool)>(asc_trunc));

static_assert(HasConversionSignature<void (*)(
    vector_int4x2_t&, vector_half, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>)>(asc_half2int4x2_rd));

static_assert(HasConversionSignature<void (*)(
    vector_int4x2_t&, vector_half, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>)>(asc_half2int4x2_rd_sat));

static_assert(HasConversionSignature<void (*)(
    vector_int4x2_t&, vector_half, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>)>(asc_half2int4x2_rd));

static_assert(HasConversionSignature<void (*)(
    vector_int4x2_t&, vector_half, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>)>(asc_half2int4x2_rd_sat));

static_assert(HasConversionSignature<void (*)(
    vector_int4x2_t&, vector_half, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>)>(asc_half2int4x2_rd));

static_assert(HasConversionSignature<void (*)(
    vector_int4x2_t&, vector_half, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>)>(asc_half2int4x2_rd_sat));

static_assert(HasConversionSignature<void (*)(
    vector_int4x2_t&, vector_half, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>)>(asc_half2int4x2_rd));

static_assert(HasConversionSignature<void (*)(
    vector_int4x2_t&, vector_half, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>)>(asc_half2int4x2_rd_sat));

static_assert(HasConversionSignature<void (*)(
    vector_int4x2_t&, vector_half, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>)>(asc_half2int4x2_rn));

static_assert(HasConversionSignature<void (*)(
    vector_int4x2_t&, vector_half, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>)>(asc_half2int4x2_rn_sat));

static_assert(HasConversionSignature<void (*)(
    vector_int4x2_t&, vector_half, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>)>(asc_half2int4x2_rn));

static_assert(HasConversionSignature<void (*)(
    vector_int4x2_t&, vector_half, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>)>(asc_half2int4x2_rn_sat));

static_assert(HasConversionSignature<void (*)(
    vector_int4x2_t&, vector_half, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>)>(asc_half2int4x2_rn));

static_assert(HasConversionSignature<void (*)(
    vector_int4x2_t&, vector_half, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>)>(asc_half2int4x2_rn_sat));

static_assert(HasConversionSignature<void (*)(
    vector_int4x2_t&, vector_half, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>)>(asc_half2int4x2_rn));

static_assert(HasConversionSignature<void (*)(
    vector_int4x2_t&, vector_half, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>)>(asc_half2int4x2_rn_sat));

static_assert(HasConversionSignature<void (*)(
    vector_int4x2_t&, vector_half, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>)>(asc_half2int4x2_rna));

static_assert(HasConversionSignature<void (*)(
    vector_int4x2_t&, vector_half, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>)>(asc_half2int4x2_rna_sat));

static_assert(HasConversionSignature<void (*)(
    vector_int4x2_t&, vector_half, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>)>(asc_half2int4x2_rna));

static_assert(HasConversionSignature<void (*)(
    vector_int4x2_t&, vector_half, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>)>(asc_half2int4x2_rna_sat));

static_assert(HasConversionSignature<void (*)(
    vector_int4x2_t&, vector_half, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>)>(asc_half2int4x2_rna));

static_assert(HasConversionSignature<void (*)(
    vector_int4x2_t&, vector_half, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>)>(asc_half2int4x2_rna_sat));

static_assert(HasConversionSignature<void (*)(
    vector_int4x2_t&, vector_half, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>)>(asc_half2int4x2_rna));

static_assert(HasConversionSignature<void (*)(
    vector_int4x2_t&, vector_half, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>)>(asc_half2int4x2_rna_sat));

static_assert(HasConversionSignature<void (*)(
    vector_int4x2_t&, vector_half, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>)>(asc_half2int4x2_ru));

static_assert(HasConversionSignature<void (*)(
    vector_int4x2_t&, vector_half, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>)>(asc_half2int4x2_ru_sat));

static_assert(HasConversionSignature<void (*)(
    vector_int4x2_t&, vector_half, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>)>(asc_half2int4x2_ru));

static_assert(HasConversionSignature<void (*)(
    vector_int4x2_t&, vector_half, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>)>(asc_half2int4x2_ru_sat));

static_assert(HasConversionSignature<void (*)(
    vector_int4x2_t&, vector_half, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>)>(asc_half2int4x2_ru));

static_assert(HasConversionSignature<void (*)(
    vector_int4x2_t&, vector_half, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>)>(asc_half2int4x2_ru_sat));

static_assert(HasConversionSignature<void (*)(
    vector_int4x2_t&, vector_half, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>)>(asc_half2int4x2_ru));

static_assert(HasConversionSignature<void (*)(
    vector_int4x2_t&, vector_half, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>)>(asc_half2int4x2_ru_sat));

static_assert(HasConversionSignature<void (*)(
    vector_int4x2_t&, vector_half, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>)>(asc_half2int4x2_rz));

static_assert(HasConversionSignature<void (*)(
    vector_int4x2_t&, vector_half, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>)>(asc_half2int4x2_rz_sat));

static_assert(HasConversionSignature<void (*)(
    vector_int4x2_t&, vector_half, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>)>(asc_half2int4x2_rz));

static_assert(HasConversionSignature<void (*)(
    vector_int4x2_t&, vector_half, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>)>(asc_half2int4x2_rz_sat));

static_assert(HasConversionSignature<void (*)(
    vector_int4x2_t&, vector_half, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>)>(asc_half2int4x2_rz));

static_assert(HasConversionSignature<void (*)(
    vector_int4x2_t&, vector_half, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>)>(asc_half2int4x2_rz_sat));

static_assert(HasConversionSignature<void (*)(
    vector_int4x2_t&, vector_half, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>)>(asc_half2int4x2_rz));

static_assert(HasConversionSignature<void (*)(
    vector_int4x2_t&, vector_half, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>)>(asc_half2int4x2_rz_sat));

static_assert(HasConversionSignature<void (*)(
    vector_bfloat16_t&, vector_int4x2_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>)>(asc_int4x22bfloat16));

static_assert(HasConversionSignature<void (*)(
    vector_bfloat16_t&, vector_int4x2_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>)>(asc_int4x22bfloat16));

static_assert(HasConversionSignature<void (*)(
    vector_bfloat16_t&, vector_int4x2_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>)>(asc_int4x22bfloat16));

static_assert(HasConversionSignature<void (*)(
    vector_bfloat16_t&, vector_int4x2_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>)>(asc_int4x22bfloat16));

static_assert(HasConversionSignature<void (*)(
    vector_half&, vector_int4x2_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>)>(asc_int4x22half));

static_assert(HasConversionSignature<void (*)(
    vector_half&, vector_int4x2_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>)>(asc_int4x22half));

static_assert(HasConversionSignature<void (*)(
    vector_half&, vector_int4x2_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>)>(asc_int4x22half));

static_assert(HasConversionSignature<void (*)(
    vector_half&, vector_int4x2_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>)>(asc_int4x22half));

static_assert(HasConversionSignature<void (*)(
    vector_int16_t&, vector_int4x2_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>)>(asc_int4x22int16));

static_assert(HasConversionSignature<void (*)(
    vector_int16_t&, vector_int4x2_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>)>(asc_int4x22int16));

static_assert(HasConversionSignature<void (*)(
    vector_int16_t&, vector_int4x2_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>)>(asc_int4x22int16));

static_assert(HasConversionSignature<void (*)(
    vector_int16_t&, vector_int4x2_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>)>(asc_int4x22int16));

static_assert(HasConversionSignature<vector_int16_t (*)(
    vector_int4x2_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>)>(asc_int4x22int16));

static_assert(HasConversionSignature<vector_int16_t (*)(
    vector_int4x2_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>)>(asc_int4x22int16));

static_assert(HasConversionSignature<vector_int16_t (*)(
    vector_int4x2_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>)>(asc_int4x22int16));

static_assert(HasConversionSignature<vector_int16_t (*)(
    vector_int4x2_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>)>(asc_int4x22int16));

static_assert(HasConversionSignature<vector_half (*)(
    vector_int4x2_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>)>(asc_int4x22half));

static_assert(HasConversionSignature<vector_half (*)(
    vector_int4x2_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>)>(asc_int4x22half));

static_assert(HasConversionSignature<vector_half (*)(
    vector_int4x2_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>)>(asc_int4x22half));

static_assert(HasConversionSignature<vector_half (*)(
    vector_int4x2_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>)>(asc_int4x22half));

static_assert(HasConversionSignature<vector_bfloat16_t (*)(
    vector_int4x2_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>)>(asc_int4x22bfloat16));

static_assert(HasConversionSignature<vector_bfloat16_t (*)(
    vector_int4x2_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>)>(asc_int4x22bfloat16));

static_assert(HasConversionSignature<vector_bfloat16_t (*)(
    vector_int4x2_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>)>(asc_int4x22bfloat16));

static_assert(HasConversionSignature<vector_bfloat16_t (*)(
    vector_int4x2_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>)>(asc_int4x22bfloat16));

static_assert(HasConversionSignature<vector_int4x2_t (*)(
    vector_half, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>)>(asc_half2int4x2_rd));

static_assert(HasConversionSignature<vector_int4x2_t (*)(
    vector_half, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>)>(asc_half2int4x2_rd));

static_assert(HasConversionSignature<vector_int4x2_t (*)(
    vector_half, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>)>(asc_half2int4x2_rd));

static_assert(HasConversionSignature<vector_int4x2_t (*)(
    vector_half, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>)>(asc_half2int4x2_rd));

static_assert(HasConversionSignature<vector_int4x2_t (*)(
    vector_half, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>)>(asc_half2int4x2_rd_sat));

static_assert(HasConversionSignature<vector_int4x2_t (*)(
    vector_half, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>)>(asc_half2int4x2_rd_sat));

static_assert(HasConversionSignature<vector_int4x2_t (*)(
    vector_half, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>)>(asc_half2int4x2_rd_sat));

static_assert(HasConversionSignature<vector_int4x2_t (*)(
    vector_half, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>)>(asc_half2int4x2_rd_sat));

static_assert(HasConversionSignature<vector_int4x2_t (*)(
    vector_half, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>)>(asc_half2int4x2_rn));

static_assert(HasConversionSignature<vector_int4x2_t (*)(
    vector_half, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>)>(asc_half2int4x2_rn));

static_assert(HasConversionSignature<vector_int4x2_t (*)(
    vector_half, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>)>(asc_half2int4x2_rn));

static_assert(HasConversionSignature<vector_int4x2_t (*)(
    vector_half, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>)>(asc_half2int4x2_rn));

static_assert(HasConversionSignature<vector_int4x2_t (*)(
    vector_half, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>)>(asc_half2int4x2_rn_sat));

static_assert(HasConversionSignature<vector_int4x2_t (*)(
    vector_half, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>)>(asc_half2int4x2_rn_sat));

static_assert(HasConversionSignature<vector_int4x2_t (*)(
    vector_half, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>)>(asc_half2int4x2_rn_sat));

static_assert(HasConversionSignature<vector_int4x2_t (*)(
    vector_half, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>)>(asc_half2int4x2_rn_sat));

static_assert(HasConversionSignature<vector_int4x2_t (*)(
    vector_half, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>)>(asc_half2int4x2_rna));

static_assert(HasConversionSignature<vector_int4x2_t (*)(
    vector_half, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>)>(asc_half2int4x2_rna));

static_assert(HasConversionSignature<vector_int4x2_t (*)(
    vector_half, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>)>(asc_half2int4x2_rna));

static_assert(HasConversionSignature<vector_int4x2_t (*)(
    vector_half, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>)>(asc_half2int4x2_rna));

static_assert(HasConversionSignature<vector_int4x2_t (*)(
    vector_half, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>)>(asc_half2int4x2_rna_sat));

static_assert(HasConversionSignature<vector_int4x2_t (*)(
    vector_half, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>)>(asc_half2int4x2_rna_sat));

static_assert(HasConversionSignature<vector_int4x2_t (*)(
    vector_half, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>)>(asc_half2int4x2_rna_sat));

static_assert(HasConversionSignature<vector_int4x2_t (*)(
    vector_half, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>)>(asc_half2int4x2_rna_sat));

static_assert(HasConversionSignature<vector_int4x2_t (*)(
    vector_half, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>)>(asc_half2int4x2_ru));

static_assert(HasConversionSignature<vector_int4x2_t (*)(
    vector_half, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>)>(asc_half2int4x2_ru));

static_assert(HasConversionSignature<vector_int4x2_t (*)(
    vector_half, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>)>(asc_half2int4x2_ru));

static_assert(HasConversionSignature<vector_int4x2_t (*)(
    vector_half, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>)>(asc_half2int4x2_ru));

static_assert(HasConversionSignature<vector_int4x2_t (*)(
    vector_half, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>)>(asc_half2int4x2_ru_sat));

static_assert(HasConversionSignature<vector_int4x2_t (*)(
    vector_half, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>)>(asc_half2int4x2_ru_sat));

static_assert(HasConversionSignature<vector_int4x2_t (*)(
    vector_half, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>)>(asc_half2int4x2_ru_sat));

static_assert(HasConversionSignature<vector_int4x2_t (*)(
    vector_half, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>)>(asc_half2int4x2_ru_sat));

static_assert(HasConversionSignature<vector_int4x2_t (*)(
    vector_half, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>)>(asc_half2int4x2_rz));

static_assert(HasConversionSignature<vector_int4x2_t (*)(
    vector_half, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>)>(asc_half2int4x2_rz));

static_assert(HasConversionSignature<vector_int4x2_t (*)(
    vector_half, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>)>(asc_half2int4x2_rz));

static_assert(HasConversionSignature<vector_int4x2_t (*)(
    vector_half, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>)>(asc_half2int4x2_rz));

static_assert(HasConversionSignature<vector_int4x2_t (*)(
    vector_half, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>)>(asc_half2int4x2_rz_sat));

static_assert(HasConversionSignature<vector_int4x2_t (*)(
    vector_half, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>)>(asc_half2int4x2_rz_sat));

static_assert(HasConversionSignature<vector_int4x2_t (*)(
    vector_half, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>)>(asc_half2int4x2_rz_sat));

static_assert(HasConversionSignature<vector_int4x2_t (*)(
    vector_half, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>)>(asc_half2int4x2_rz_sat));

static_assert(HasConversionSignature<void (*)(
    vector_half&, vector_int16_t, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_half&, vector_int16_t, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_half&, vector_int16_t, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_half&, vector_int16_t, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_half&, vector_int16_t, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_bfloat16_t&, vector_int4x2_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_bfloat16_t&, vector_int4x2_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_bfloat16_t&, vector_int4x2_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_bfloat16_t&, vector_int4x2_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_half&, vector_int4x2_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_half&, vector_int4x2_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_half&, vector_int4x2_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_half&, vector_int4x2_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int16_t&, vector_int4x2_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int16_t&, vector_int4x2_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int16_t&, vector_int4x2_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int16_t&, vector_int4x2_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_fp4x2_e1m2_t&, vector_bfloat16_t, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN>,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_fp4x2_e1m2_t&, vector_bfloat16_t, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN>,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_fp4x2_e1m2_t&, vector_bfloat16_t, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN>,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_fp4x2_e1m2_t&, vector_bfloat16_t, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN>,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_fp4x2_e1m2_t&, vector_bfloat16_t, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA>,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_fp4x2_e1m2_t&, vector_bfloat16_t, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA>,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_fp4x2_e1m2_t&, vector_bfloat16_t, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA>,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_fp4x2_e1m2_t&, vector_bfloat16_t, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA>,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_fp4x2_e1m2_t&, vector_bfloat16_t, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD>,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_fp4x2_e1m2_t&, vector_bfloat16_t, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD>,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_fp4x2_e1m2_t&, vector_bfloat16_t, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD>,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_fp4x2_e1m2_t&, vector_bfloat16_t, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD>,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_fp4x2_e1m2_t&, vector_bfloat16_t, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU>,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_fp4x2_e1m2_t&, vector_bfloat16_t, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU>,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_fp4x2_e1m2_t&, vector_bfloat16_t, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU>,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_fp4x2_e1m2_t&, vector_bfloat16_t, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU>,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_fp4x2_e1m2_t&, vector_bfloat16_t, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ>,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_fp4x2_e1m2_t&, vector_bfloat16_t, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ>,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_fp4x2_e1m2_t&, vector_bfloat16_t, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ>,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_fp4x2_e1m2_t&, vector_bfloat16_t, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ>,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_fp4x2_e2m1_t&, vector_bfloat16_t, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN>,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_fp4x2_e2m1_t&, vector_bfloat16_t, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN>,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_fp4x2_e2m1_t&, vector_bfloat16_t, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN>,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_fp4x2_e2m1_t&, vector_bfloat16_t, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN>,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_fp4x2_e2m1_t&, vector_bfloat16_t, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA>,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_fp4x2_e2m1_t&, vector_bfloat16_t, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA>,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_fp4x2_e2m1_t&, vector_bfloat16_t, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA>,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_fp4x2_e2m1_t&, vector_bfloat16_t, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA>,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_fp4x2_e2m1_t&, vector_bfloat16_t, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD>,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_fp4x2_e2m1_t&, vector_bfloat16_t, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD>,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_fp4x2_e2m1_t&, vector_bfloat16_t, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD>,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_fp4x2_e2m1_t&, vector_bfloat16_t, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD>,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_fp4x2_e2m1_t&, vector_bfloat16_t, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU>,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_fp4x2_e2m1_t&, vector_bfloat16_t, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU>,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_fp4x2_e2m1_t&, vector_bfloat16_t, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU>,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_fp4x2_e2m1_t&, vector_bfloat16_t, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU>,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_fp4x2_e2m1_t&, vector_bfloat16_t, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ>,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_fp4x2_e2m1_t&, vector_bfloat16_t, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ>,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_fp4x2_e2m1_t&, vector_bfloat16_t, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ>,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_fp4x2_e2m1_t&, vector_bfloat16_t, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ>,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int4x2_t&, vector_half, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int4x2_t&, vector_half, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int4x2_t&, vector_half, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int4x2_t&, vector_half, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int4x2_t&, vector_half, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int4x2_t&, vector_half, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int4x2_t&, vector_half, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int4x2_t&, vector_half, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int4x2_t&, vector_half, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int4x2_t&, vector_half, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int4x2_t&, vector_half, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int4x2_t&, vector_half, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int4x2_t&, vector_half, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int4x2_t&, vector_half, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int4x2_t&, vector_half, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int4x2_t&, vector_half, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int4x2_t&, vector_half, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int4x2_t&, vector_half, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int4x2_t&, vector_half, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int4x2_t&, vector_half, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int4x2_t&, vector_half, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int4x2_t&, vector_half, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int4x2_t&, vector_half, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int4x2_t&, vector_half, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int4x2_t&, vector_half, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int4x2_t&, vector_half, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int4x2_t&, vector_half, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int4x2_t&, vector_half, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int4x2_t&, vector_half, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int4x2_t&, vector_half, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int4x2_t&, vector_half, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int4x2_t&, vector_half, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int4x2_t&, vector_half, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int4x2_t&, vector_half, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int4x2_t&, vector_half, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int4x2_t&, vector_half, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int4x2_t&, vector_half, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int4x2_t&, vector_half, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int4x2_t&, vector_half, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int4x2_t&, vector_half, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_float&, vector_half, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_float&, vector_half, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_float&, vector_fp8_e4m3fn_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_float&, vector_fp8_e4m3fn_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_float&, vector_fp8_e4m3fn_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_float&, vector_fp8_e4m3fn_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_bfloat16_t&, vector_fp4x2_e1m2_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_bfloat16_t&, vector_fp4x2_e1m2_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_bfloat16_t&, vector_fp4x2_e1m2_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_bfloat16_t&, vector_fp4x2_e1m2_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_bfloat16_t&, vector_fp4x2_e2m1_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_bfloat16_t&, vector_fp4x2_e2m1_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_bfloat16_t&, vector_fp4x2_e2m1_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_bfloat16_t&, vector_fp4x2_e2m1_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_float&, vector_bfloat16_t, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_float&, vector_bfloat16_t, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_float&, vector_fp8_e5m2_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_float&, vector_fp8_e5m2_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_float&, vector_fp8_e5m2_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_float&, vector_fp8_e5m2_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_float&, vector_hifloat8_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_float&, vector_hifloat8_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_float&, vector_hifloat8_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_float&, vector_hifloat8_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_float&, vector_int16_t, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_float&, vector_int16_t, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_half&, vector_hifloat8_t, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_half&, vector_hifloat8_t, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_half&, vector_int8_t, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_half&, vector_int8_t, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_half&, vector_uint8_t, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_half&, vector_uint8_t, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int16_t&, vector_int8_t, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int16_t&, vector_int8_t, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int32_t&, vector_int16_t, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int32_t&, vector_int16_t, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int32_t&, vector_int8_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int32_t&, vector_int8_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int32_t&, vector_int8_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int32_t&, vector_int8_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int64_t&, vector_int32_t, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int64_t&, vector_int32_t, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_uint16_t&, vector_uint8_t, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_uint16_t&, vector_uint8_t, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_uint32_t&, vector_int16_t, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_uint32_t&, vector_int16_t, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_uint32_t&, vector_uint16_t, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_uint32_t&, vector_uint16_t, vector_bool,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_uint32_t&, vector_uint8_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_uint32_t&, vector_uint8_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_uint32_t&, vector_uint8_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_uint32_t&, vector_uint8_t, vector_bool,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_bfloat16_t&, vector_float, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_bfloat16_t&, vector_float, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_bfloat16_t&, vector_float, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_bfloat16_t&, vector_float, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_bfloat16_t&, vector_float, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_bfloat16_t&, vector_float, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_bfloat16_t&, vector_float, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_bfloat16_t&, vector_float, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_bfloat16_t&, vector_float, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_bfloat16_t&, vector_float, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_bfloat16_t&, vector_float, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_bfloat16_t&, vector_float, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_bfloat16_t&, vector_float, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_bfloat16_t&, vector_float, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_bfloat16_t&, vector_float, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_bfloat16_t&, vector_float, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_bfloat16_t&, vector_float, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_bfloat16_t&, vector_float, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_bfloat16_t&, vector_float, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_bfloat16_t&, vector_float, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_bfloat16_t&, vector_half, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_bfloat16_t&, vector_half, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_bfloat16_t&, vector_half, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_bfloat16_t&, vector_half, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_bfloat16_t&, vector_half, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_float&, vector_int32_t, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_float&, vector_int32_t, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_float&, vector_int32_t, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_float&, vector_int32_t, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_float&, vector_int32_t, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_float&, vector_int64_t, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN>,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_float&, vector_int64_t, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN>,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_float&, vector_int64_t, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA>,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_float&, vector_int64_t, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA>,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_float&, vector_int64_t, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD>,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_float&, vector_int64_t, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD>,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_float&, vector_int64_t, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU>,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_float&, vector_int64_t, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU>,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_float&, vector_int64_t, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ>,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_float&, vector_int64_t, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ>,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_fp8_e4m3fn_t&, vector_float, vector_bool,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_fp8_e4m3fn_t&, vector_float, vector_bool,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_fp8_e4m3fn_t&, vector_float, vector_bool,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_fp8_e4m3fn_t&, vector_float, vector_bool,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_fp8_e4m3fn_t&, vector_float, vector_bool,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_fp8_e4m3fn_t&, vector_float, vector_bool,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_fp8_e4m3fn_t&, vector_float, vector_bool,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_fp8_e4m3fn_t&, vector_float, vector_bool,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_fp8_e5m2_t&, vector_float, vector_bool,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_fp8_e5m2_t&, vector_float, vector_bool,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_fp8_e5m2_t&, vector_float, vector_bool,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_fp8_e5m2_t&, vector_float, vector_bool,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_fp8_e5m2_t&, vector_float, vector_bool,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_fp8_e5m2_t&, vector_float, vector_bool,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_fp8_e5m2_t&, vector_float, vector_bool,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_fp8_e5m2_t&, vector_float, vector_bool,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_half&, vector_bfloat16_t, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_half&, vector_bfloat16_t, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_half&, vector_bfloat16_t, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_half&, vector_bfloat16_t, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_half&, vector_bfloat16_t, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_half&, vector_bfloat16_t, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_half&, vector_bfloat16_t, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_half&, vector_bfloat16_t, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_half&, vector_bfloat16_t, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_half&, vector_bfloat16_t, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_half&, vector_float, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_half&, vector_float, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_half&, vector_float, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_half&, vector_float, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_half&, vector_float, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_half&, vector_float, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_half&, vector_float, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_half&, vector_float, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_half&, vector_float, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_half&, vector_float, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_half&, vector_float, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_half&, vector_float, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_half&, vector_float, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_half&, vector_float, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_half&, vector_float, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_half&, vector_float, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_half&, vector_float, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_half&, vector_float, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_half&, vector_float, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_half&, vector_float, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_half&, vector_float, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RO>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_half&, vector_float, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RO>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_half&, vector_float, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RO>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_half&, vector_float, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RO>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_hifloat8_t&, vector_float, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_hifloat8_t&, vector_float, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_hifloat8_t&, vector_float, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_hifloat8_t&, vector_float, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_hifloat8_t&, vector_float, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_hifloat8_t&, vector_float, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_hifloat8_t&, vector_float, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_hifloat8_t&, vector_float, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_hifloat8_t&, vector_float, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RH>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_hifloat8_t&, vector_float, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RH>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_hifloat8_t&, vector_float, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RH>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_hifloat8_t&, vector_float, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RH>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_hifloat8_t&, vector_float, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RH>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_hifloat8_t&, vector_float, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RH>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_hifloat8_t&, vector_float, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RH>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_hifloat8_t&, vector_float, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RH>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_hifloat8_t&, vector_half, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_hifloat8_t&, vector_half, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_hifloat8_t&, vector_half, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_hifloat8_t&, vector_half, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_hifloat8_t&, vector_half, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RH>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_hifloat8_t&, vector_half, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RH>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_hifloat8_t&, vector_half, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RH>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_hifloat8_t&, vector_half, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RH>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int16_t&, vector_float, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int16_t&, vector_float, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int16_t&, vector_float, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int16_t&, vector_float, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int16_t&, vector_float, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int16_t&, vector_float, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int16_t&, vector_float, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int16_t&, vector_float, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int16_t&, vector_float, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int16_t&, vector_float, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int16_t&, vector_float, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int16_t&, vector_float, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int16_t&, vector_float, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int16_t&, vector_float, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int16_t&, vector_float, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int16_t&, vector_float, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int16_t&, vector_float, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int16_t&, vector_float, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int16_t&, vector_float, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int16_t&, vector_float, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int16_t&, vector_half, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int16_t&, vector_half, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int16_t&, vector_half, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int16_t&, vector_half, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int16_t&, vector_half, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int16_t&, vector_half, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int16_t&, vector_half, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int16_t&, vector_half, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int16_t&, vector_half, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int16_t&, vector_half, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int32_t&, vector_bfloat16_t, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int32_t&, vector_bfloat16_t, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int32_t&, vector_bfloat16_t, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int32_t&, vector_bfloat16_t, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int32_t&, vector_bfloat16_t, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int32_t&, vector_bfloat16_t, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int32_t&, vector_bfloat16_t, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int32_t&, vector_bfloat16_t, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int32_t&, vector_bfloat16_t, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int32_t&, vector_bfloat16_t, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int32_t&, vector_bfloat16_t, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int32_t&, vector_bfloat16_t, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int32_t&, vector_bfloat16_t, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int32_t&, vector_bfloat16_t, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int32_t&, vector_bfloat16_t, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int32_t&, vector_bfloat16_t, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int32_t&, vector_bfloat16_t, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int32_t&, vector_bfloat16_t, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int32_t&, vector_bfloat16_t, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int32_t&, vector_bfloat16_t, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int32_t&, vector_float, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int32_t&, vector_float, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int32_t&, vector_float, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int32_t&, vector_float, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int32_t&, vector_float, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int32_t&, vector_float, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int32_t&, vector_float, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int32_t&, vector_float, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int32_t&, vector_float, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int32_t&, vector_float, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int32_t&, vector_half, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN>,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int32_t&, vector_half, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN>,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int32_t&, vector_half, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA>,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int32_t&, vector_half, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA>,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int32_t&, vector_half, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD>,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int32_t&, vector_half, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD>,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int32_t&, vector_half, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU>,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int32_t&, vector_half, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU>,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int32_t&, vector_half, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ>,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int32_t&, vector_half, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ>,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int64_t&, vector_float, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int64_t&, vector_float, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int64_t&, vector_float, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int64_t&, vector_float, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int64_t&, vector_float, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int64_t&, vector_float, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int64_t&, vector_float, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int64_t&, vector_float, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int64_t&, vector_float, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int64_t&, vector_float, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int64_t&, vector_float, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int64_t&, vector_float, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int64_t&, vector_float, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int64_t&, vector_float, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int64_t&, vector_float, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int64_t&, vector_float, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int64_t&, vector_float, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int64_t&, vector_float, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int64_t&, vector_float, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int64_t&, vector_float, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int8_t&, vector_half, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int8_t&, vector_half, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int8_t&, vector_half, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int8_t&, vector_half, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int8_t&, vector_half, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int8_t&, vector_half, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int8_t&, vector_half, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int8_t&, vector_half, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int8_t&, vector_half, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int8_t&, vector_half, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int8_t&, vector_half, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int8_t&, vector_half, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int8_t&, vector_half, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int8_t&, vector_half, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int8_t&, vector_half, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int8_t&, vector_half, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int8_t&, vector_half, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int8_t&, vector_half, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int8_t&, vector_half, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int8_t&, vector_half, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_uint8_t&, vector_half, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_uint8_t&, vector_half, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_uint8_t&, vector_half, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_uint8_t&, vector_half, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_uint8_t&, vector_half, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_uint8_t&, vector_half, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_uint8_t&, vector_half, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_uint8_t&, vector_half, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_uint8_t&, vector_half, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_uint8_t&, vector_half, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_uint8_t&, vector_half, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_uint8_t&, vector_half, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_uint8_t&, vector_half, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_uint8_t&, vector_half, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_uint8_t&, vector_half, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_uint8_t&, vector_half, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_uint8_t&, vector_half, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_uint8_t&, vector_half, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_uint8_t&, vector_half, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_uint8_t&, vector_half, vector_bool,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ>,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int16_t&, vector_int32_t, vector_bool,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int16_t&, vector_int32_t, vector_bool,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int16_t&, vector_int32_t, vector_bool,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int16_t&, vector_int32_t, vector_bool,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int16_t&, vector_uint32_t, vector_bool,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int16_t&, vector_uint32_t, vector_bool,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int16_t&, vector_uint32_t, vector_bool,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int16_t&, vector_uint32_t, vector_bool,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int32_t&, vector_int64_t, vector_bool,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int32_t&, vector_int64_t, vector_bool,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int32_t&, vector_int64_t, vector_bool,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_int32_t&, vector_int64_t, vector_bool,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_uint16_t&, vector_int32_t, vector_bool,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_uint16_t&, vector_int32_t, vector_bool,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_uint16_t&, vector_int32_t, vector_bool,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_uint16_t&, vector_int32_t, vector_bool,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_uint16_t&, vector_uint32_t, vector_bool,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_uint16_t&, vector_uint32_t, vector_bool,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_uint16_t&, vector_uint32_t, vector_bool,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_uint16_t&, vector_uint32_t, vector_bool,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_uint8_t&, vector_int16_t, vector_bool,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_uint8_t&, vector_int16_t, vector_bool,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_uint8_t&, vector_int16_t, vector_bool,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_uint8_t&, vector_int16_t, vector_bool,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_uint8_t&, vector_int32_t, vector_bool,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_uint8_t&, vector_int32_t, vector_bool,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_uint8_t&, vector_int32_t, vector_bool,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_uint8_t&, vector_int32_t, vector_bool,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_uint8_t&, vector_int32_t, vector_bool,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_uint8_t&, vector_int32_t, vector_bool,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_uint8_t&, vector_int32_t, vector_bool,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_uint8_t&, vector_int32_t, vector_bool,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_uint8_t&, vector_uint16_t, vector_bool,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_uint8_t&, vector_uint16_t, vector_bool,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_uint8_t&, vector_uint16_t, vector_bool,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_uint8_t&, vector_uint16_t, vector_bool,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_uint8_t&, vector_uint32_t, vector_bool,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_uint8_t&, vector_uint32_t, vector_bool,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_uint8_t&, vector_uint32_t, vector_bool,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_uint8_t&, vector_uint32_t, vector_bool,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT>,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_uint8_t&, vector_uint32_t, vector_bool,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_uint8_t&, vector_uint32_t, vector_bool,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_uint8_t&, vector_uint32_t, vector_bool,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<void (*)(
    vector_uint8_t&, vector_uint32_t, vector_bool,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT>,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>)>(asc_cast));

static_assert(HasConversionSignature<half (*)(float)>(asc_float2half_ro));

}

#endif
