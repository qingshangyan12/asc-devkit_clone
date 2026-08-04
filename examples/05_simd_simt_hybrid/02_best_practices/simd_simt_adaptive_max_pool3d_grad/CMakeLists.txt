# ----------------------------------------------------------------------------------------------------------
# This program is free software, you can redistribute it and/or modify it.
# Copyright (c) 2026 Huawei Technologies Co., Ltd.
# This file is a part of the CANN Open Software.
# Licensed under CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ----------------------------------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.16)

set(CMAKE_ASC_ARCHITECTURES "dav-3510" CACHE STRING "NPU architecture")

# grad/argmax的D/H/W取值，三者相同。改变该值即可复现README中的性能对比表格。
set(OUTPUT_DHW "8" CACHE STRING "D/H/W of grad/argmax, one of 1/2/4/8/16")

if(NOT OUTPUT_DHW MATCHES "^(1|2|4|8|16)$")
    message(FATAL_ERROR "OUTPUT_DHW must be one of 1, 2, 4, 8, 16.")
endif()

find_package(ASC REQUIRED)
project(adaptive_max_pool3d_grad_simd_simt_hybrid LANGUAGES ASC)

add_executable(adaptive_max_pool3d_grad adaptive_max_pool3d_grad_host.asc)

target_compile_definitions(adaptive_max_pool3d_grad PRIVATE OUTPUT_DHW=${OUTPUT_DHW})

math(EXPR GRAD_PER_NC_PLANE "${OUTPUT_DHW} * ${OUTPUT_DHW} * ${OUTPUT_DHW}")
message(STATUS "[INFO] OUTPUT_DHW is ${OUTPUT_DHW}, each NC plane scatters ${GRAD_PER_NC_PLANE} gradients")
message(STATUS "[INFO] Runtime mode is selected by ./adaptive_max_pool3d_grad <simd|scalar|hybrid|hybrid_ub> [profile]")
