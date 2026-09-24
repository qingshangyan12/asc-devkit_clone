# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2026 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ----------------------------------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.16)

set(CMAKE_ASC_ARCHITECTURES "dav-3510" CACHE STRING "NPU ARCH, e.g. dav-3510")
set(CMAKE_ASC_ENABLE_SIMT ON)

find_package(ASC REQUIRED)
project(kernel_samples LANGUAGES ASC CXX)

# 场景参数（通过命令行传入）
# SCENARIO_NUM: 用例编号（kernel与计算函数均以接口名hlog10/j0f命名，case版本由该宏隔开）
#   1 - hlog10：特殊值修正链前置，命中即提前return（基线）
#   2 - hlog10：正常值路径前置，修正链统一return覆盖temp（优化）
#   3 - j0f：所有分支命中即直接返回（基线）
#   4 - j0f：分段计算逐级覆盖result，全分支统一return（全谓词化）
#   5 - j0f：真跳转与谓词化混合，常规批（ax <= 8.0f为主）
#   6 - j0f：真跳转与谓词化混合，混合批
set(SCENARIO_NUM "1" CACHE STRING "Case type to run (1-6)")
if(NOT SCENARIO_NUM MATCHES "^[1-6]$")
    message(FATAL_ERROR "SCENARIO_NUM must be in [1, 6], but got ${SCENARIO_NUM}")
endif()

add_executable(predicate_optimization
    predicate_optimization.asc
)

target_compile_definitions(predicate_optimization PRIVATE
    SCENARIO_NUM=${SCENARIO_NUM}
)

target_link_libraries(predicate_optimization PRIVATE
    m
)
