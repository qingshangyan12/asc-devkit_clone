# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ----------------------------------------------------------------------------------------------------------
project(ascendc_utils_std_tests)
cmake_minimum_required(VERSION 3.16.0)
set(CMAKE_CXX_STANDARD 17)
find_package(tikicpulib REQUIRED)

set(ASCENDC_UT_PRODUCT_TYPE_LIST
    ascend910B1
    ascend910_9599
)

file(GLOB ASCENDC_UT_ascend910B1_CASE_SRC
    sequence/test_sequence.cpp
    tuple/test_operator_tuple.cpp
    type_traits/test_operator_conditional.cpp
    type_traits/test_operator_enable_if.cpp
    type_traits/test_operator_integral_constant.cpp
    type_traits/test_operator_is_base_of.cpp
    type_traits/test_operator_is_convertible.cpp
    type_traits/test_operator_is_one_of.cpp
    type_traits/test_operator_is_same.cpp
)

file(GLOB ASCENDC_UT_ascend910_9599_CASE_SRC
    cmath/test_abs.cpp
    cmath/test_sqrt.cpp
)

foreach(product_type ${ASCENDC_UT_PRODUCT_TYPE_LIST})
    add_executable(ascendc_ut_std_api_${product_type}
        ${ASCENDC_DIR}/tests/main_global.cpp
        ${ASCENDC_UT_${product_type}_CASE_SRC}
    )

    target_include_directories(ascendc_ut_std_api_${product_type} PRIVATE
        ${ASCENDC_DIR}
        ${ASCENDC_DIR}/impl
        $<$<STREQUAL:${product_type},ascend910_9599>:${ASCENDC_DIR}/impl/simt_api>
    )

    target_compile_definitions(ascendc_ut_std_api_${product_type} PRIVATE
        UT_TEST
        ASCENDC_OOM=1
        ASCENDC_DUMP=0
        $<$<STREQUAL:${product_type},ascend910B1>:__NPU_ARCH__=2201>
        $<$<STREQUAL:${product_type},ascend910_9599>:__NPU_ARCH__=3101>
        __disable_kernel_type_autoinfer__=
    )

    target_compile_options(ascendc_ut_std_api_${product_type} PRIVATE
        -fno-access-control
        -Werror
    )

    target_link_libraries(ascendc_ut_std_api_${product_type} PRIVATE
        $<BUILD_INTERFACE:intf_llt_pub_basic>
        tikicpulib::${product_type}
        -Wl,--no-as-needed
        error_manager
        mmpa
        c_sec
        -Wl,--as-needed
    )

    run_llt_test(
        TARGET ascendc_ut_std_api_${product_type}
        TASK_NUM 1
    )
endforeach()