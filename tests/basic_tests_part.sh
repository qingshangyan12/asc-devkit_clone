#!/bin/bash
# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ----------------------------------------------------------------------------------------------------------

set -e

test_one_targets=(
    ascendc_ut_basic_api_ascend910
    ascendc_ut_basic_api_ascend310p
    ascendc_ut_basic_api_ascend610
    ascendc_ut_basic_api_ascend310B1
    ascendc_ut_basic_api_ascend610Lite
    ascendc_ut_basic_api_ascend910B1_AIC
    ascendc_ut_basic_api_ascend910B1_AIV
    ascendc_ut_tiling_utils_ascend910
    ascendc_ut_tiling_utils_ascend310p
    ascendc_ut_tiling_utils_ascend610
    ascendc_ut_tiling_utils_ascend310B1
    ascendc_ut_tiling_utils_ascend610Lite
    ascendc_ut_tiling_utils_ascend910B1_AIC
    ascendc_ut_tiling_utils_ascend910B1_AIV
    ascendc_ut_tiling_utils_ascend910_9599_AIC
    ascendc_ut_tiling_utils_ascend910_9599_AIV_BASIC
    ascendc_ut_tiling_utils_ascend910_9599_AIV_FRAMEWORK
    ascendc_ut_tiling_utils_ascend910_9599_AIV_MICRO
    ascendc_ut_tiling_utils_ascend910_9599_AIV_SIMT
)

test_two_targets=(
    ascendc_ut_tpl_tiling_debug
    ascendc_ut_tpl_tiling_release
    ascendc_ut_aicpu_api
    ascendc_ut_std_api_ascend910B1
    ascendc_ut_std_api_ascend910_9599
    ascendc_ut_tiling_utils_mc62cm12aa_AIC
    ascendc_ut_tiling_utils_mc62cm12aa_BASIC
    ascendc_ut_tiling_utils_mc62cm12aa_FRAMEWORK
    ascendc_ut_tiling_utils_mc62cm12aa_MICRO
    ascendc_ut_tiling_utils_mc62cm12aa_SIMT
    ascendc_ut_basic_api_mc62cm12aa_AIC
    ascendc_ut_basic_api_mc62cm12aa_BASIC
    ascendc_ut_basic_api_mc62cm12aa_FRAMEWORK
    ascendc_ut_basic_api_mc62cm12aa_MICRO
    ascendc_ut_basic_api_mc62cm12aa_SIMT
)

test_three_targets=(
    ascendc_ut_c_api_ascend910B1_AIC
    ascendc_ut_c_api_ascend910B1_AIV
    ascendc_ut_basic_api_ascend910_9599_AIC
    ascendc_ut_basic_api_ascend910_9599_AIV_BASIC
    ascendc_ut_basic_api_ascend910_9599_AIV_FRAMEWORK
    ascendc_ut_basic_api_ascend910_9599_AIV_MICRO
    ascendc_ut_basic_api_ascend910_9599_AIV_SIMT
)