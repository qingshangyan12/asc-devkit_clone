/**
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*!
 * \file conv_util.h
 * \brief
 */

#ifndef CONV_UTIL_H
#define CONV_UTIL_H

#include "kernel_utils.h"

namespace conv {
using namespace AscendC;

// [zj]ToDo

#if defined(__DAV_L510__)
    const static uint64_t L0A_SIZE = 0;
    const static uint64_t L0B_SIZE = 131072;
    const static uint64_t L0A_HALF_SIZE = 0;
    const static uint64_t L0B_HALF_SIZE = 65536;
#else
    const static uint64_t L0A_SIZE = 65536;
    const static uint64_t L0B_SIZE = 65536;
    const static uint64_t L0A_HALF_SIZE = 32768;
    const static uint64_t L0B_HALF_SIZE = 32768;
#endif
const static uint64_t C0_SIZE = 32;
const static uint64_t BT_SIZE = 64; // [zj]B
const static uint32_t FB_QUANT_PRE_SIZE = 128;
const static uint32_t FB_RELU_PRE_SIZE = 64;
const static uint32_t FB_RELU_POST_SIZE = 64;
const static uint32_t FB_QUANT_POST_SIZE = 128;
const static uint32_t FB_ELT_ANTIQ_SIZE = 64;
const static uint64_t PAD_IDX_T = 2;
const static uint64_t PAD_IDX_B = 3;
const static uint64_t PAD_IDX_L = 0;
const static uint64_t PAD_IDX_R = 1;
const static uint64_t MAX_PAD_R = 255;
const static uint64_t W_TAIL_NUM = 2;
const static uint64_t AL1_DB_IDX = 0x08;
const static uint64_t AL1_DB_OFFSET = 3;
const static uint64_t PADDING_ALIGN_SIZE = 32;
const static uint32_t BLOCK_L0_N = 16;
const static uint32_t BLOCK_L0_M = 16;
const static uint32_t BT_BLOCK_SIZE = 32;
const static uint32_t FB_BLOCK_SIZE = 32;
const static uint8_t C04_CIN_SIZE  = 4;
const static uint8_t ROUND_MODE_RINT = 1;
const static uint8_t ROUND_MODE_HALFTOAWAY = 2;
const static uint8_t ROUND_MODE_HYBRID = 3;
const static uint8_t DOUBLE_BUF = 2;
// Opt Group
const static uint16_t REG_SIZE = 256;
const static uint16_t CO0_LOOP_TIMES = 2;
const static uint16_t B8_CO0_LOOP_TIMES = 4;
const static uint8_t VEC_NUM = 2;
const static uint8_t VEC_ID_MAX = 16;
const static uint8_t CV_SYNC_ID_MTE3_MTE1 = 0;
const static uint8_t CV_SYNC_ID_MTE1_MTE3 = 1;
const static uint8_t CV_SYUC_ID_V_ROTATE = 2;
const static uint8_t NDDMA_DIMS = 3;
const static uint8_t NDDMA_LOOP0_INDEX = 0;
const static uint8_t NDDMA_LOOP1_INDEX = 1;
const static uint8_t NDDMA_LOOP2_INDEX = 2;
const static uint8_t CV_ENHANCE_MODE = 4;

// Load3d parameters offset
const static uint8_t MSTEP_OFFSET = 16;
const static uint8_t POSM_OFFSET = 48;
const static uint8_t POSK_OFFSET = 32;
const static uint8_t STRIDEH_OFFSET = 6;
const static uint8_t KERNELW_OFFSET = 12;
const static uint8_t KERNELH_OFFSET = 20;
const static uint8_t DILATIONW_OFFSET = 28;
const static uint8_t DILATIONH_OFFSET = 36;
const static uint8_t CIN_OFFSET = 48;
const static uint64_t DEQ_SCALAR_ONE = 1065353216;
static constexpr IsResetLoad3dConfig CONV_LOAD3DV2_DEFAULT_CONFIG = {false, false};

static __aicore__ inline uint64_t AlignB(uint64_t a, uint64_t b)
{
    return ((a + b - 1) / b) * b;
}

static __aicore__ inline uint64_t CeilDIV(uint64_t a, uint64_t b)
{
    return (a + b - 1) / b;
}

}

#endif  // CONV_UTIL_H