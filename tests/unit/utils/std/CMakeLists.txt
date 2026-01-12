# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ----------------------------------------------------------------------------------------------------------
project(ascendc_devkit_ut)
cmake_minimum_required(VERSION 3.14.1)

set(PRODUCT_TYPE_LIST
    ascend910 ascend610 ascend310p ascend910B1_AIC ascend910B1_AIV ascend310B1 ascend610Lite)

set(TIKCPP_UT_PRODUCT_TYPE_LIST ascend910 ascend610 ascend310p ascend910B1_AIC ascend910B1_AIV ascend310B1 ascend610Lite)
# set(TIKCPP_UT_PRODUCT_TYPE_LIST ascend910)

add_compile_definitions(ASCENDC_CPU_DEBUG=1 ASCENDC_DUMP=0)

message(STATUS "tikcpp_ut_test compile start")
set(CMAKE_CXX_STANDARD 17)

set(LOCAL_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(ASCENDC_SYS_DIR ${ASCEND_CANN_PACKAGE_PATH}/${CMAKE_SYSTEM_PROCESSOR}-linux)

############# gen tiling head file
set(tiling_script ${LOCAL_DIR}/scripts/gen_kernel_tiling_data_def.py)
set(TILING_GEN_DIR ${CMAKE_CURRENT_BINARY_DIR})
set(tiling_gen_file ${TILING_GEN_DIR}/kernel_tiling/kernel_tiling.h)
set(tiling_data_def_path ${ASCENDC_INCLUDE_DIR}/adv_api)

add_custom_command(OUTPUT ${tiling_gen_file}
    COMMAND python3  ${tiling_script} ${tiling_data_def_path} ${tiling_gen_file}
    DEPENDS ${tiling_script} ${tiling_data_def_path})

add_custom_target(gen_ut_kernel_tiling ALL
    DEPENDS ${tiling_gen_file})
##################################################

####################################################################
####################################################################
set(ASCENDC_TEST_HEADER_FILES
    ${ASCENDC_SYS_DIR}/include
    ${ASCENDC_SYS_DIR}/include/base
    ${ASCEND_CANN_PACKAGE_PATH}/tools/tikicpulib/lib/include
    ${LOCAL_DIR}/../common
    ${LOCAL_DIR}/common/
    ${LOCAL_DIR}/testcase/
    ${ASCENDC_DIR}
    ${ASCENDC_DIR}/include/
    ${ASCENDC_DIR}/include/basic_api/
    ${ASCENDC_DIR}/include/utils/
    ${ASCENDC_DIR}/include/c_api/
    ${ASCENDC_DIR}/impl/
    ${ASCENDC_DIR}/impl/c_api/
    ${ASCENDC_DIR}/impl/basic_api/
    ${ASCENDC_SYS_DIR}/tikcpp/tikcfw
    ${ASCENDC_SYS_DIR}/tikcpp/tikcfw/interface
    ${ASCENDC_SYS_DIR}/tikcpp/tikcfw/impl
    ${ASCENDC_SYS_DIR}/ascendc/include/highlevel_api/
    ${CMAKE_CURRENT_BINARY_DIR}/stub
    ${TILING_GEN_DIR}
)

set(ASCEND_DEP_LIB_DIR
    ${ASCENDC_SYS_DIR}/lib64/
    ${ASCEND_CANN_PACKAGE_PATH}/tools/tikicpulib/lib/
)

# common ut src files
file(GLOB ASCENDC_TEST_COMMON_SRC_FILES
    ${LOCAL_DIR}/common/common.cpp
)

# common test cases
file(GLOB ASCENDC_TEST_COMMON_CASE_SRC_FILES
    ${LOCAL_DIR}/testcase/common/*.cpp
    ${LOCAL_DIR}/testcase/tikcpp_case_common/*.cpp
)

# ascend910 test cases
file(GLOB ASCENDC_TEST_ascend910_CASE_SRC_FILES
    ${LOCAL_DIR}/testcase/basic_api/tikcpp_case_ascend910/*.cpp
)

# ascend610 test cases
file(GLOB ASCENDC_TEST_ascend610_CASE_SRC_FILES
    ${LOCAL_DIR}/testcase/basic_api/tikcpp_case_ascend610/*.cpp
)

# ascend310p test cases
file(GLOB ASCENDC_TEST_ascend310p_CASE_SRC_FILES
    ${LOCAL_DIR}/testcase/basic_api/tikcpp_case_ascend310p/*.cpp
)

# ascend910B1 aic c api test cases
file(GLOB ASCENDC_TEST_ascend910B1_AIC_CAPI_CASE_SRC_FILES
    ${LOCAL_DIR}/testcase/c_api/asc_2201/cube_compute/*.cpp
    ${LOCAL_DIR}/testcase/c_api/asc_2201/cube_datamove/*.cpp
)

# ascend910B1 aiv c api test cases
file(GLOB ASCENDC_TEST_ascend910B1_AIV_CAPI_CASE_SRC_FILES
    ${LOCAL_DIR}/testcase/c_api/asc_2201/atomic/*.cpp
    ${LOCAL_DIR}/testcase/c_api/asc_2201/cache_ctrl/*.cpp
    ${LOCAL_DIR}/testcase/c_api/asc_2201/sync/*.cpp
    ${LOCAL_DIR}/testcase/c_api/asc_2201/sys_var/*.cpp
    ${LOCAL_DIR}/testcase/c_api/asc_2201/vector_compute/*.cpp
    ${LOCAL_DIR}/testcase/c_api/asc_2201/vector_datamove/*.cpp
    ${LOCAL_DIR}/testcase/c_api/asc_2201/scalar_compute/*.cpp
    ${LOCAL_DIR}/testcase/c_api/asc_2201/cache_ctrl/*.cpp
)

# ascend910B1 aiv test cases
file(GLOB ASCENDC_TEST_ascend910B1_AIV_CASE_SRC_FILES
    ${LOCAL_DIR}/testcase/tikcpp_case_common/test_data_copy_slice.cpp
    ${LOCAL_DIR}/testcase/tikcpp_case_common/test_tscm_data.cpp
    ${LOCAL_DIR}/testcase/basic_api/tikcpp_case_ascend910b1/tikcpp_case_ascend910b1_aiv/*.cpp
)

# ascend910B1 aic test cases
file(GLOB ASCENDC_TEST_ascend910B1_AIC_CASE_SRC_FILES
    ${LOCAL_DIR}/testcase/basic_api/tikcpp_case_ascend910b1/tikcpp_case_ascend910b1_aic/*.cpp
    ${LOCAL_DIR}/testcase/matmul_api/tikcpp_case_ascend910b1/tikcpp_case_ascend910b1_aic/*.cpp
)

# ascend310B1 test cases
file(GLOB ASCENDC_TEST_ascend310B1_CASE_SRC_FILES
    ${LOCAL_DIR}/testcase/tikcpp_case_common/test_kernel_pop_stack_buffer.cpp
    ${LOCAL_DIR}/testcase/basic_api/tikcpp_case_ascend310b1/*.cpp
    ${LOCAL_DIR}/testcase/matmul_api/tikcpp_case_ascend310b1/*.cpp
)

# ascend610Lite test cases
file(GLOB ASCENDC_TEST_ascend610Lite_CASE_SRC_FILES
    ${LOCAL_DIR}/testcase/tikcpp_case_common/test_kernel_pop_stack_buffer.cpp
    ${LOCAL_DIR}/testcase/basic_api/tikcpp_case_ascend610lite/*.cpp
)

file(GLOB ASCENDC_API_CHECK_CASE_SRC_FILES
    ${LOCAL_DIR}/testcase/tikcpp_api_check/*.cpp
)

# ############################################ utils ut ############################################
set(ACLRTC_TEST_HEADERS_FILES
    ${LOCAL_DIR}/../common/
    ${ASCENDC_DIR}/tools/aclrtc/include
)

file(GLOB ACLRTC_SRC_FILES
    ${ASCENDC_DIR}/tools/aclrtc/src/*.cpp
)

file(GLOB ASCENDC_UTILS_TEST_SRC_FILES
    ${LOCAL_DIR}/cmath/*.cpp
)



file(GLOB TEST_ACLRTC_SRC_FILES
    ${LOCAL_DIR}/main.cpp
    ${LOCAL_DIR}/testcase/aclrtc/stub/*.cpp
    ${LOCAL_DIR}/testcase/aclrtc/*.cpp
)

add_executable(ascendc_utest_aclrtc
    ${ACLRTC_SRC_FILES}
    ${TEST_ACLRTC_SRC_FILES}
)
add_dependencies(ascendc_utest_aclrtc gen_ut_kernel_tiling)
target_compile_definitions(ascendc_utest_aclrtc PRIVATE
    UT_TEST
)
target_compile_definitions(ascendc_utest_aclrtc PRIVATE ASCENDC_CPU_DEBUG=1)
target_include_directories(ascendc_utest_aclrtc PRIVATE
    ${ACLRTC_TEST_HEADERS_FILES}
    ${ASCENDC_SYS_DIR}/include
    ${ASCENDC_SYS_DIR}/include/acl/
)
target_compile_options(ascendc_utest_aclrtc PRIVATE
    -g
    -Werror
)
target_link_directories(ascendc_utest_aclrtc PRIVATE
    ${ASCEND_DEP_LIB_DIR}
)
target_link_libraries(ascendc_utest_aclrtc PRIVATE
    $<BUILD_INTERFACE:intf_llt_pub_basic>
    -Wl,--no-as-needed
    c_sec
    dl
    -Wl,--as-needed
)

run_llt_test(
    TARGET ascendc_utest_aclrtc
    TASK_NUM 1
)