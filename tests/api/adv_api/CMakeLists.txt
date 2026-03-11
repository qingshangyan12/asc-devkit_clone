# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ----------------------------------------------------------------------------------------------------------
project(ascendc_adv_api_tests)
cmake_minimum_required(VERSION 3.16.0)
set(CMAKE_CXX_STANDARD 17)
find_package(tikicpulib REQUIRED)

set(ASCENDC_ADV_API_INCLUDE ${ASCENDC_DIR}/include/adv_api)
set(ASCENDC_ADV_API_IMPL ${ASCENDC_DIR}/impl/adv_api)
set(ASCENDC_UTILS_INCLUDE ${ASCENDC_DIR}/include/utils)

set(PRODUCT_TYPE_LIST_INIT ascend610 ascend310p ascend910B1_AIC ascend910B1_AIV ascend950pr_9599_AIC ascend950pr_9599_AIV ascend310B1)
set(PRODUCT_TYPE_LIST ${PRODUCT_TYPE_LIST_INIT} CACHE STRING "Supported product types")

set(ASCENDC_DEP_HEADER_DIR
    ${ASCEND_CANN_PACKAGE_PATH}/include
    ${ASCEND_CANN_PACKAGE_PATH}/include/base
    ${ASCEND_CANN_PACKAGE_PATH}/pkg_inc/base
    ${CMAKE_BINARY_DIR}
    ${ASCENDC_UTILS_INCLUDE}
)

set(ASCENDC_TEST_HEADER_FILES
    ../common
    matmul/utils
    ${ASCENDC_DIR}
    ${ASCENDC_DIR}/include
    ${ASCENDC_DIR}/include/adv_api
    ${ASCENDC_DIR}/include/basic_api
    ${ASCENDC_DIR}/impl
    ${ASCENDC_DIR}/impl/basic_api
)

# # ================ adv_api kernel ut ================
# ascend610 test cases
file(GLOB ASCENDC_TEST_ascend610_CASE_SRC_FILES
    activation/sigmoid/test_operator_vec_sigmoid.cpp
)

# ascend310p test cases
file(GLOB ASCENDC_TEST_ASCEND310P_CASE_SRC_PART_FILES
    utils/init_global_memory/test_operator_init_global_memory.cpp
)
file(GLOB_RECURSE ASCENDC_TEST_ASCEND310P_CASE_SRC_FILES_API_CHECK api_check/math/*.cpp)
set(ASCENDC_TEST_ascend310p_CASE_SRC_FILES
    ${ASCENDC_TEST_ASCEND310P_CASE_SRC_PART_FILES}
    ${ASCENDC_TEST_ASCEND310P_CASE_SRC_FILES_API_CHECK}
)

# ascend910B1 aiv test cases
file(GLOB ASCENDC_TEST_ASCEND910B1_AIV_CASE_SRC_PART_FILES
    activation/geglu/test_operator_geglu.cpp
)
file(GLOB_RECURSE ASCENDC_TEST_ASCEND910B1_AIV_CASE_SRC_FILES_API_CHECK api_check/*.cpp)
set(ASCENDC_TEST_ascend910B1_AIV_CASE_SRC_FILES
    ${ASCENDC_TEST_ASCEND910B1_AIV_CASE_SRC_PART_FILES}
    ${ASCENDC_TEST_ASCEND910B1_AIV_CASE_SRC_FILES_API_CHECK}
)

# ascend910B1 aic test cases
file(GLOB ASCENDC_TEST_ascend910B1_AIC_CASE_SRC_FILES
    matmul/kfc/test_matmul_server_c220.cpp
)

# ascend950pr_9599 test cases
file(GLOB ASCENDC_TEST_ascend950pr_9599_AIC_CASE_SRC_FILES
    matmul/lib/test_matmul_client.cpp
)
file(GLOB ASCENDC_TEST_ascend950pr_9599_AIV_CASE_SRC_FILES
    hccl/test_hccl_950.cpp
    hcomm/test_hcomm_950.cpp
)

# ascend310B1 test cases
file(GLOB ASCENDC_TEST_ascend310B1_CASE_SRC_FILES
    activation/softmax/test_operator_softmax_v300.cpp
    matmul/test_operator_matmul_v300.cpp
    math/xor/test_operator_xor.cpp
    math/floor/test_operator_floor.cpp
    matmul/scheduler/test_scheduler_norm.cpp
)

foreach(product_type ${PRODUCT_TYPE_LIST})
    add_executable(ascendc_ut_adv_api_kernel_${product_type}
        ${ASCENDC_DIR}/tests/main_global.cpp
        $<$<STREQUAL:${product_type},ascend610>:../common/tik_pv_wrapper.cpp>
        $<$<STREQUAL:${product_type},ascend310p>:../common/tik_pv_wrapper.cpp>
        $<$<STREQUAL:${product_type},ascend910B1_AIC>:../common/k3_pvwrap.cpp>
        $<$<STREQUAL:${product_type},ascend910B1_AIV>:../common/k3_pvwrap.cpp>
        $<$<STREQUAL:${product_type},ascend950pr_9599_AIC>:../common/k3_pvwrap.cpp>
        $<$<STREQUAL:${product_type},ascend950pr_9599_AIV>:../common/k3_pvwrap.cpp>
        $<$<STREQUAL:${product_type},ascend310B1>:../common/k3_pvwrap.cpp>
        ${ASCENDC_TEST_${product_type}_CASE_SRC_FILES}
    )

    target_compile_definitions(ascendc_ut_adv_api_kernel_${product_type} PRIVATE
        UT_TEST
        ASCENDC_OOM=1
        ASCENDC_DUMP=0
        $<$<STREQUAL:${product_type},ascend310p>:__NPU_ARCH__=2002;__DAV_M200__>
        $<$<STREQUAL:${product_type},ascend610>:__NPU_ARCH__=2002;__DAV_M200__>
        $<$<STREQUAL:${product_type},ascend910B1_AIC>:__NPU_ARCH__=2201;__DAV_C220__;__DAV_C220_CUBE__;__DAV_CUBE__>
        $<$<STREQUAL:${product_type},ascend910B1_AIV>:__NPU_ARCH__=2201;__DAV_C220__;__DAV_C220_VEC__;__DAV_VEC__>
        $<$<STREQUAL:${product_type},ascend950pr_9599_AIC>:__NPU_ARCH__=3510;__DAV_C310__;__DAV_C310_CUBE__>
        $<$<STREQUAL:${product_type},ascend950pr_9599_AIV>:__NPU_ARCH__=3510;__DAV_C310__;__DAV_C310_VEC__>
        $<$<STREQUAL:${product_type},ascend310B1>:__NPU_ARCH__=3002;__DAV_M300__>
        __disable_kernel_type_autoinfer__=
    )

    target_include_directories(ascendc_ut_adv_api_kernel_${product_type} PRIVATE
        ${ASCENDC_TEST_HEADER_FILES}
        ${ASCENDC_DEP_HEADER_DIR}
    )

    target_compile_options(ascendc_ut_adv_api_kernel_${product_type} PRIVATE
        -fno-access-control
        -Werror
    )

    target_link_libraries(ascendc_ut_adv_api_kernel_${product_type} PRIVATE
        $<BUILD_INTERFACE:intf_llt_pub_basic>
        kernel_tiling
        $<$<STREQUAL:${product_type},ascend310p>:tikicpulib::ascend310p>
        $<$<STREQUAL:${product_type},ascend610>:tikicpulib::ascend610>
        $<$<STREQUAL:${product_type},ascend910B1_AIC>:tikicpulib::ascend910B1>
        $<$<STREQUAL:${product_type},ascend910B1_AIV>:tikicpulib::ascend910B1>
        $<$<STREQUAL:${product_type},ascend950pr_9599_AIC>:tikicpulib::ascend950pr_9599>
        $<$<STREQUAL:${product_type},ascend950pr_9599_AIV>:tikicpulib::ascend950pr_9599>
        $<$<STREQUAL:${product_type},ascend310B1>:tikicpulib::ascend310B1>
        -Wl,--no-as-needed
        error_manager
        mmpa
        c_sec
        -Wl,--as-needed
    )

    run_llt_test(
        TARGET ascendc_ut_adv_api_kernel_${product_type}
        TASK_NUM 1
    )
endforeach()

# ================ adv_api tiling ut ================
file(GLOB ASCENDC_TILING_TEST_SRC_FILES
    tiling/test_hccl_tiling.cpp
    tiling/test_matmul_api_tiling.cpp
    tiling/test_tiling_v310.cpp
    tiling/test_tiling.cpp
    tiling/conv/*.cpp
    tiling/conv_backprop/*.cpp
)

file(GLOB ASCENDC_TILING_SRC_FILES
    ${ASCENDC_ADV_API_IMPL}/tiling/activation/*.cpp
    ${ASCENDC_ADV_API_IMPL}/tiling/filter/*.cpp
    ${ASCENDC_ADV_API_IMPL}/tiling/index/*.cpp
    ${ASCENDC_ADV_API_IMPL}/tiling/math/*.cpp
    ${ASCENDC_ADV_API_IMPL}/tiling/matmul/*cpp
    ${ASCENDC_ADV_API_IMPL}/tiling/hccl/*.cpp
    ${ASCENDC_ADV_API_IMPL}/tiling/normalization/*.cpp
    ${ASCENDC_ADV_API_IMPL}/tiling/pad/*.cpp
    ${ASCENDC_ADV_API_IMPL}/tiling/quantization/*.cpp
    ${ASCENDC_ADV_API_IMPL}/tiling/reduce/*.cpp
    ${ASCENDC_ADV_API_IMPL}/tiling/select/*.cpp
    ${ASCENDC_ADV_API_IMPL}/tiling/sort/*.cpp
    ${ASCENDC_ADV_API_IMPL}/tiling/transpose/*.cpp
    ${ASCENDC_ADV_API_IMPL}/tiling/conv/*.cpp
    ${ASCENDC_ADV_API_IMPL}/tiling/conv_backprop/*.cpp
    ${ASCENDC_ADV_API_IMPL}/detail/host_log.cpp
)

# ascendc_tiling_utest
foreach(product_type ${PRODUCT_TYPE_LIST})
    add_executable(ascendc_ut_adv_api_tiling_${product_type}
        ${ASCENDC_DIR}/tests/main.cpp
        ../common/platform_stub.cpp
        ../common/ascendc_platform_stub.cpp
        ${ASCENDC_TILING_SRC_FILES}
        ${ASCENDC_TILING_TEST_SRC_FILES}
    )

    target_compile_definitions(ascendc_ut_adv_api_tiling_${product_type} PRIVATE
        UT_TEST
        $<$<STREQUAL:${product_type},ascend310p>:__NPU_ARCH__=2002>
        $<$<STREQUAL:${product_type},ascend610>:__NPU_ARCH__=2002>
        $<$<STREQUAL:${product_type},ascend910B1_AIC>:__NPU_ARCH__=2201>
        $<$<STREQUAL:${product_type},ascend910B1_AIV>:__NPU_ARCH__=2201>
        $<$<STREQUAL:${product_type},ascend950pr_9599_AIC>:__NPU_ARCH__=3510>
        $<$<STREQUAL:${product_type},ascend950pr_9599_AIV>:__NPU_ARCH__=3510>
        $<$<STREQUAL:${product_type},ascend310B1>:__NPU_ARCH__=3002>
    )

    target_include_directories(ascendc_ut_adv_api_tiling_${product_type} PRIVATE
        ../common/
        ${ASCENDC_TEST_HEADER_FILES}
        ${ASCENDC_DEP_HEADER_DIR}
        ${ASCENDC_TEST_${product_type}_HEADER_FILES}
        ${ASCENDC_ADV_API_INCLUDE}/
        ${ASCENDC_ADV_API_INCLUDE}/activation/
        ${ASCENDC_ADV_API_INCLUDE}/filter/
        ${ASCENDC_ADV_API_INCLUDE}/index/
        ${ASCENDC_ADV_API_INCLUDE}/math/
        ${ASCENDC_ADV_API_INCLUDE}/matmul/
        ${ASCENDC_ADV_API_INCLUDE}/normalization/
        ${ASCENDC_ADV_API_INCLUDE}/pad/
        ${ASCENDC_ADV_API_INCLUDE}/quantization/
        ${ASCENDC_ADV_API_INCLUDE}/reduce/
        ${ASCENDC_ADV_API_INCLUDE}/select/
        ${ASCENDC_ADV_API_INCLUDE}/sort/
        ${ASCENDC_ADV_API_INCLUDE}/transpose/
        ${ASCENDC_ADV_API_INCLUDE}/conv/
        ${ASCENDC_ADV_API_INCLUDE}/conv_backprop/
        ${ASCENDC_ADV_API_INCLUDE}/conv/conv3d/
    )

    target_link_directories(ascendc_ut_adv_api_tiling_${product_type} PRIVATE
        ${ASCEND_CANN_PACKAGE_PATH}/lib64
    )

    target_link_libraries(ascendc_ut_adv_api_tiling_${product_type} PRIVATE
        $<BUILD_INTERFACE:intf_llt_pub_basic>
        kernel_tiling
        register
        platform
        graph
        graph_base
        unified_dlog
        ascend_protobuf
        error_manager
        mmpa
        c_sec
        dl
    )

    run_llt_test(
        TARGET ascendc_ut_adv_api_tiling_${product_type}
        TASK_NUM 1
    )
endforeach()