/**
* Copyright (c) 2025 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/

/*!
 * \file tiling_api.h
 * \brief
 */

#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_TILING_API_H__
#endif

#ifndef LIB_TILING_API_H
#define LIB_TILING_API_H
#include "include/adv_api/matmul/matmul_tiling.h"
#include "include/adv_api/matmul/bmm_tiling.h"
#include "include/adv_api/activation/softmax_tiling.h"
#include "include/adv_api/activation/logsoftmax_tiling.h"
#include "include/adv_api/filter/dropout_tiling.h"
#include "include/adv_api/sort/sort_tiling_intf.h"
#include "include/adv_api/index/arithprogression_tiling.h"
#include "include/adv_api/quantization/ascend_dequant_tiling.h"
#include "include/adv_api/quantization/ascend_quant_tiling.h"
#include "include/adv_api/quantization/ascend_antiquant_tiling.h"
#include "include/adv_api/quantization/quantize_tiling.h"
#include "include/adv_api/quantization/antiquantize_tiling.h"
#include "include/adv_api/quantization/dequantize_tiling.h"
#include "include/adv_api/reduce/sum_tiling.h"
#include "include/adv_api/activation/silu_tiling.h"
#include "include/adv_api/activation/swish_tiling.h"
#include "include/adv_api/activation/gelu_tiling.h"
#include "include/adv_api/pad/pad_tiling.h"
#include "include/adv_api/normalization/rmsnorm_tiling.h"
#include "include/adv_api/normalization/deepnorm_tiling.h"
#include "include/adv_api/normalization/layernorm_tiling.h"
#include "include/adv_api/normalization/normalize_tiling.h"
#include "include/adv_api/normalization/batchnorm_tiling.h"
#include "include/adv_api/normalization/layernorm_grad_tiling.h"
#include "include/adv_api/normalization/layernorm_grad_beta_tiling.h"
#include "include/adv_api/normalization/welfordfinalize_tiling.h"
#include "include/adv_api/transpose/confusion_transpose_tiling.h"
#include "include/utils/tiling/platform/platform_ascendc.h"
#include "include/adv_api/sort/topk_tiling.h"
#include "include/adv_api/math/tanh_tiling.h"
#include "include/adv_api/activation/sigmoid_tiling.h"
#include "include/adv_api/math/frac_tiling.h"
#include "include/adv_api/math/acos_tiling.h"
#include "include/adv_api/math/asin_tiling.h"
#include "include/adv_api/math/fma_tiling.h"
#include "include/adv_api/math/rint_tiling.h"
#include "include/adv_api/math/sincos_tiling.h"
#include "include/adv_api/math/acosh_tiling.h"
#include "include/adv_api/math/asinh_tiling.h"
#include "include/adv_api/math/sin_tiling.h"
#include "include/adv_api/math/cos_tiling.h"
#include "include/adv_api/math/atan_tiling.h"
#include "include/adv_api/math/power_tiling.h"
#include "include/adv_api/math/log_tiling.h"
#include "include/adv_api/math/cosh_tiling.h"
#include "include/adv_api/math/clamp_tiling.h"
#include "include/adv_api/math/erf_tiling.h"
#include "include/adv_api/math/erfc_tiling.h"
#include "include/adv_api/math/round_tiling.h"
#include "include/adv_api/math/sinh_tiling.h"
#include "include/adv_api/activation/swiglu_tiling.h"
#include "include/adv_api/math/tan_tiling.h"
#include "include/adv_api/math/hypot_tiling.h"
#include "include/adv_api/select/selectwithbytesmask_tiling.h"
#include "include/adv_api/math/trunc_tiling.h"
#include "include/adv_api/activation/geglu_tiling.h"
#include "include/adv_api/math/lgamma_tiling.h"
#include "include/adv_api/math/digamma_tiling.h"
#include "include/adv_api/math/atanh_tiling.h"
#include "include/adv_api/math/xor_tiling.h"
#include "include/adv_api/math/sign_tiling.h"
#include "include/adv_api/reduce/mean_tiling.h"
#include "include/adv_api/math/bitwise_and_tiling.h"
#include "include/adv_api/math/logical_and_tiling.h"
#include "include/adv_api/math/logical_ands_tiling.h"
#include "include/adv_api/math/logical_not_tiling.h"
#include "include/adv_api/math/logical_or_tiling.h"
#include "include/adv_api/math/logical_ors_tiling.h"
#include "include/adv_api/math/exp_tiling.h"
#include "include/adv_api/math/axpy_tiling.h"
#include "include/adv_api/math/ceil_tiling.h"
#include "include/adv_api/math/floor_tiling.h"
#include "include/adv_api/math/is_inf_tiling.h"
#include "include/adv_api/math/is_nan_tiling.h"
#include "include/adv_api/math/bitwise_not_tiling.h"
#include "include/adv_api/math/bitwise_or_tiling.h"
#include "include/adv_api/math/bitwise_xor_tiling.h"
#include "include/adv_api/math/logical_xor_tiling.h"
#include "include/adv_api/activation/reglu_tiling.h"
#include "include/adv_api/pad/broadcast_tiling.h"
#include "include/adv_api/reduce/reduce_xor_sum_tiling.h"
#include "include/adv_api/reduce/reduce_tiling.h"
#include "include/adv_api/math/cumsum_tiling.h"
#include "include/adv_api/math/fmod_tiling.h"
#include "include/adv_api/math/where_tiling.h"
#include "include/adv_api/normalization/groupnorm_tiling.h"
#include "include/adv_api/transpose/transdata_tiling.h"
#include "include/adv_api/hccl/hccl_tilingdata.h"
#include "include/adv_api/hccl/hccl_tiling.h"
#include "include/adv_api/conv/conv3d/conv3d_tiling.h"
#endif // LIB_TILING_API_H

#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_TILING_API_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_TILING_API_H__
#endif