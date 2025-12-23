/**
* Copyright (c) 2025 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/

#include "../utils/test_conv_instr_utils.h"

// ==========asc_half2int4(NA/a/c/f/r/z)==========
TEST_VECTOR_COMPUTE_CONV_INSTR(ConvH2I4, asc_half2int4, vconv_f162s4, void, half);
TEST_VECTOR_COMPUTE_CONV_INSTR(ConvH2I4a, asc_half2int4_a, vconv_f162s4a, void, half);
TEST_VECTOR_COMPUTE_CONV_INSTR(ConvH2I4c, asc_half2int4_c, vconv_f162s4c, void, half);
TEST_VECTOR_COMPUTE_CONV_INSTR(ConvH2I4f, asc_half2int4_f, vconv_f162s4f, void, half);
TEST_VECTOR_COMPUTE_CONV_INSTR(ConvH2I4r, asc_half2int4_r, vconv_f162s4r, void, half);
TEST_VECTOR_COMPUTE_CONV_INSTR(ConvH2I4z, asc_half2int4_z, vconv_f162s4z, void, half);