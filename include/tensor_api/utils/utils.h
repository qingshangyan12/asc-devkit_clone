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
 * \file utils.h
 * \brief
 */
#ifndef INCLUDE_TENSOR_API_UTILS_UTILS_H
#define INCLUDE_TENSOR_API_UTILS_UTILS_H

#include "utils/base/sys_macros.h"
#include "utils/base/sys_constants.h"
#include "utils/common_types.h"

namespace asc {
namespace te {

using namespace AscendC;

enum class cache_mode : uint8_t {
    normal = 0,
    disable = 4,
    last = 5,
    persistent = 6,
    // Deprecated compatibility enumerators.
    CACHE_MODE_NORMAL = normal,
    CACHE_MODE_DISABLE = disable,
    CACHE_MODE_LAST = last,
    CACHE_MODE_PERSISTENT = persistent
};
using CacheMode = cache_mode;

enum class mmad_type : uint8_t {
    normal = 0,
    mx = 1,
    // Deprecated compatibility enumerators.
    NORMAL = normal,
    MX = mx
};
using MmadType = mmad_type;

struct mmad_trait {
    __aicore__ constexpr mmad_trait(){};

    __aicore__ constexpr mmad_trait(int32_t fm_offset_in, bool k_direction_align_in, bool cmatrix_source_in,
                                    bool disable_gemv_in, asc::te::mmad_type mmad_type_in) :
        fm_offset(fm_offset_in), k_direction_align(k_direction_align_in), cmatrix_source(cmatrix_source_in),
        disable_gemv(disable_gemv_in), mmad_type(mmad_type_in){};

    int32_t fm_offset = 0;
    bool k_direction_align = false;
    bool cmatrix_source = false;
    bool disable_gemv = true;
    asc::te::mmad_type mmad_type = asc::te::mmad_type::normal;
};

struct mmad_params {
    __aicore__ constexpr mmad_params(){};

    __aicore__ constexpr mmad_params(uint16_t m_in, uint16_t n_in, uint16_t k_in, uint8_t unit_flag_in,
                                     bool cmatrix_init_val_in) :
        m(m_in), n(n_in), k(k_in), unit_flag(unit_flag_in), cmatrix_init_val(cmatrix_init_val_in){};

    uint16_t m = 0;
    uint16_t n = 0;
    uint16_t k = 0;
    uint8_t unit_flag = 0;
    bool cmatrix_init_val = false;
};

enum class round_mode : uint8_t {
    default_round = 0,
    hybrid = 1,
    // Deprecated compatibility enumerators.
    DEFAULT = default_round,
    HYBRID = hybrid
};
using RoundMode = round_mode;

enum dual_dst_mode : uint8_t {
    disable = 0,
    split_m = 1,
    split_n = 2,
    // Deprecated compatibility enumerators.
    DUAL_DST_DISABLE = disable,
    DUAL_DST_SPLIT_M = split_m,
    DUAL_DST_SPLIT_N = split_n
};
using DualDstMode = dual_dst_mode;

struct fixpipe_params {
    __aicore__ constexpr fixpipe_params(){};

    __aicore__ constexpr fixpipe_params(uint8_t unit_flag_in, bool sub_block_id_in = false) :
        unit_flag(unit_flag_in), sub_block_id(sub_block_id_in)
    {}

    uint8_t unit_flag = 0;
    bool sub_block_id = false;
};

struct copy_gm_to_ub_params {
    __aicore__ constexpr copy_gm_to_ub_params() {}

    __aicore__ constexpr copy_gm_to_ub_params(uint8_t left_padding_count_in, uint8_t right_padding_count_in,
                                              bool enable_constant_pad_in = true) :
        left_padding_count(left_padding_count_in), right_padding_count(right_padding_count_in),
        enable_constant_pad(enable_constant_pad_in)
    {}

    uint8_t left_padding_count = 0;
    uint8_t right_padding_count = 0;
    bool enable_constant_pad = true;
};

template <typename T>
struct img2col_params {
    uint16_t m_extension = 0;
    uint16_t k_extension = 0;
    uint16_t m_start_pt = 0;
    uint16_t k_start_pt = 0;
    uint8_t pad_list[4] = {0, 0, 0, 0};
    uint8_t stride_w = 1;
    uint8_t stride_h = 1;
    uint8_t filter_w = 1;
    uint8_t filter_h = 1;
    uint8_t dilation_filter_w = 1;
    uint8_t dilation_filter_h = 1;
    bool filter_size_w = false;
    bool filter_size_h = false;
    bool transpose = false;
    bool f_matrix_ctrl = false;
    T pad_value = 0;
};

struct MmadTrait {
    __aicore__ constexpr MmadTrait(){};

    __aicore__ constexpr MmadTrait(int32_t fmOffsetIn, bool kDirectionAlignIn, bool cmatrixSourceIn, bool disableGemvIn,
                                   MmadType mmadTypeIn) :
        fmOffset(fmOffsetIn), kDirectionAlign(kDirectionAlignIn), cmatrixSource(cmatrixSourceIn),
        disableGemv(disableGemvIn), mmadType(mmadTypeIn){};

    int32_t fmOffset = 0;
    bool kDirectionAlign = false;
    bool cmatrixSource = false;
    bool disableGemv = true;
    MmadType mmadType = MmadType::NORMAL;
};

struct MmadParams {
    __aicore__ constexpr MmadParams(){};

    __aicore__ constexpr MmadParams(uint16_t mIn, uint16_t nIn, uint16_t kIn, uint8_t unitFlagIn,
                                    bool cmatrixInitValIn) :
        m(mIn), n(nIn), k(kIn), unitFlag(unitFlagIn), cmatrixInitVal(cmatrixInitValIn){};

    uint16_t m = 0;
    uint16_t n = 0;
    uint16_t k = 0;
    uint8_t unitFlag = 0;
    bool cmatrixInitVal = false;
};

struct FixpipeParams {
    __aicore__ constexpr FixpipeParams(){};

    __aicore__ constexpr FixpipeParams(uint8_t unitFlagIn, bool subBlockIdIn = false) :
        unitFlag(unitFlagIn), subBlockId(subBlockIdIn)
    {}

    uint8_t unitFlag = 0;
    bool subBlockId = false;
};

struct CopyGM2UBParams {
    __aicore__ constexpr CopyGM2UBParams() {}

    __aicore__ constexpr CopyGM2UBParams(uint8_t leftPaddingCountIn, uint8_t rightPaddingCountIn,
                                         bool enableConstantPadIn = true) :
        leftPaddingCount(leftPaddingCountIn), rightPaddingCount(rightPaddingCountIn),
        enableConstantPad(enableConstantPadIn)
    {}

    uint8_t leftPaddingCount = 0;
    uint8_t rightPaddingCount = 0;
    bool enableConstantPad = true;
};

template <typename T>
struct Img2ColParams {
    uint16_t mExtension = 0;
    uint16_t kExtension = 0;
    uint16_t mStartPt = 0;
    uint16_t kStartPt = 0;
    uint8_t padList[4] = {0, 0, 0, 0};
    uint8_t strideW = 1;
    uint8_t strideH = 1;
    uint8_t filterW = 1;
    uint8_t filterH = 1;
    uint8_t dilationFilterW = 1;
    uint8_t dilationFilterH = 1;
    bool filterSizeW = false;
    bool filterSizeH = false;
    bool transpose = false;
    bool fMatrixCtrl = false;
    T padValue = 0;
};

} // namespace te
} // namespace asc

namespace AscendC {
namespace Te = asc::te;
}
#endif // INCLUDE_TENSOR_API_UTILS_UTILS_H
