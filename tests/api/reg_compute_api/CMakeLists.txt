# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ----------------------------------------------------------------------------------------------------------
project(ascendc_reg_compute_tests)
cmake_minimum_required(VERSION 3.16.0)
set(CMAKE_CXX_STANDARD 17)
find_package(tikicpulib REQUIRED)

add_subdirectory(ascendc_reg_header_checker)

set(MICRO_PRODUCT_TYPE_LIST_INIT ascend950pr_9599 kirin9050)
set(MICRO_PRODUCT_TYPE_LIST ${MICRO_PRODUCT_TYPE_LIST_INIT} CACHE STRING "Supported product types" FORCE)

set(ASCENDC_TEST_HEADER_FILES
    ../common
    ${ASCENDC_DIR}
    ${ASCENDC_DIR}/include
    ${ASCENDC_DIR}/include/basic_api
    ${ASCENDC_DIR}/include/basic_api/reg_compute
    ${ASCENDC_DIR}/include/utils
    ${ASCENDC_DIR}/impl
    ${ASCENDC_DIR}/impl/basic_api
    ${ASCENDC_DIR}/impl/basic_api/reg_compute
)

# # ================ reg_compute_api kernel ut ================
# ascend950pr_9599 micro test cases
file(GLOB ASCENDC_TEST_ascend950pr_9599_MICRO_CASE_SRC_FILES
    ascendc_case_ascend950pr_9599_reg_compute/*.cpp
)

# mc62cm12aa micro test cases
file(GLOB ASCENDC_TEST_mc62cm12aa_MICRO_CASE_SRC_FILES
    ascendc_case_mc62cm12aa_reg_compute/*.cpp
)

# kirin9050 micro test cases
file(GLOB ASCENDC_TEST_kirin9050_MICRO_CASE_SRC_FILES
    ascendc_case_kirin9050_reg_compute/*.cpp
)

# tikicpulib component name (ALIAS target) differs from product_type for some SoCs
set(TIKICPULIB_TARGET_NAME_kirin9050 "Kirin9050")

function(configure_reg_compute_ut_target ut_target product_type)
    target_compile_definitions(${ut_target} PRIVATE
        UT_TEST
        ASCENDC_OOM=1
        ASCENDC_DUMP=0
        $<$<STREQUAL:${product_type},ascend950pr_9599>:__NPU_ARCH__=3510;__DAV_C310__;__DAV_C310_VEC__>
        $<$<STREQUAL:${product_type},mc62cm12aa>:__NPU_ARCH__=5102>
        $<$<STREQUAL:${product_type},kirin9050>:__NPU_ARCH__=5101;__DAV_L510__;__DAV_L510_VEC__>
        __callee__=
    )

    target_include_directories(${ut_target} PRIVATE
        ${ASCENDC_TEST_HEADER_FILES}
        ${ASCEND_CANN_PACKAGE_PATH}/include/base/
        ${ASCEND_CANN_PACKAGE_PATH}/include/metadef/
    )

    target_compile_options(${ut_target} PRIVATE
        -fno-access-control
        -UASCENDC_DEBUG
    )
endfunction()

foreach(product_type ${MICRO_PRODUCT_TYPE_LIST})
    set(tikicpulib_lib "${product_type}")
    if(DEFINED TIKICPULIB_TARGET_NAME_${product_type})
        set(tikicpulib_lib "${TIKICPULIB_TARGET_NAME_${product_type}}")
    endif()

    if(TARGET tikicpulib::${tikicpulib_lib})
        add_executable(ascendc_ut_reg_compute_${product_type}
            ${ASCENDC_DIR}/tests/main_global.cpp
            ../common/k3_pvwrap.cpp
            ${ASCENDC_TEST_${product_type}_MICRO_CASE_SRC_FILES}
        )

        configure_reg_compute_ut_target(ascendc_ut_reg_compute_${product_type} ${product_type})

        target_link_libraries(ascendc_ut_reg_compute_${product_type} PRIVATE
            $<BUILD_INTERFACE:intf_llt_pub_basic>
            tikicpulib::${tikicpulib_lib}
            kernel_tiling
            -Wl,--no-as-needed
            error_manager
            mmpa
            c_sec
            -Wl,--as-needed
        )

        run_llt_test(
            TARGET ascendc_ut_reg_compute_${product_type}
            TASK_NUM 1
        )
    else()
        # tikicpulib::<SoC> not shipped with this CANN package (e.g. GA predates the SoC):
        # degrade to compile-level verification — same sources/definitions, no link, no run
        add_library(ascendc_ut_reg_compute_${product_type}_compile_only OBJECT
            ${ASCENDC_DIR}/tests/main_global.cpp
            ../common/k3_pvwrap.cpp
            ${ASCENDC_TEST_${product_type}_MICRO_CASE_SRC_FILES}
        )

        configure_reg_compute_ut_target(ascendc_ut_reg_compute_${product_type}_compile_only ${product_type})

        # host-side headers are SoC-independent: reuse the tikicpulib include set
        # (same extraction as ascendc_reg_header_checker) instead of the missing per-SoC target
        get_target_property(tikicpulib_host_include_dirs tikicpulib_ascend910 INTERFACE_INCLUDE_DIRECTORIES)
        if(NOT tikicpulib_host_include_dirs)
            set(tikicpulib_host_include_dirs "")
        endif()
        target_include_directories(ascendc_ut_reg_compute_${product_type}_compile_only SYSTEM PRIVATE
            ${tikicpulib_host_include_dirs}
        )

        # host-side compile definitions normally provided by the tikicpulib::<SoC> target
        target_compile_definitions(ascendc_ut_reg_compute_${product_type}_compile_only PRIVATE
            ASCENDC_CPU_DEBUG=1
            __CCE_KT_TEST__=1
        )

        target_link_libraries(ascendc_ut_reg_compute_${product_type}_compile_only PRIVATE
            $<BUILD_INTERFACE:intf_llt_pub_basic>
            kernel_tiling
            error_manager
            mmpa
            c_sec
        )
        # generate_kernel_headers only exists while kernel_tiling/kernel_tiling.h has not been
        # generated yet (tests/cmake/intf.cmake); guard to keep re-configure working afterwards
        if(TARGET generate_kernel_headers)
            add_dependencies(ascendc_ut_reg_compute_${product_type}_compile_only generate_kernel_headers)
        endif()

        add_custom_target(ascendc_ut_reg_compute_${product_type}
            DEPENDS ascendc_ut_reg_compute_${product_type}_compile_only
            COMMENT "ascendc_ut_reg_compute_${product_type}: compile-level verification only (tikicpulib::${tikicpulib_lib} not available in this CANN)"
        )
        message(STATUS "ascendc_ut_reg_compute_${product_type}: tikicpulib::${tikicpulib_lib} not available in this CANN, degraded to compile-level verification (no link, no run)")
    endif()
endforeach()
