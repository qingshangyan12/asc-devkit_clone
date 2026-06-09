# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2026 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ----------------------------------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.16.0)
include(../fetch_cann_cmake.cmake)
project(ascendc_tiling_api)

init_cann_project(PRODUCT_SIDE "device" PREPEND_MODULE_PATH)
add_cann_target_options()

set(HI_PYTHON "python3" CACHE STRING "python executor")
set(ASCENDC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../..)
set(ASCENDC_INCLUDE_DIR ${ASCENDC_DIR}/include)
set(ASCENDC_IMPL_DIR ${ASCENDC_DIR}/impl)
set(INSTALL_LIBRARY_DIR ${CMAKE_HOST_SYSTEM_PROCESSOR}-linux/lib64/device)
get_filename_component(ASC_DEVKIT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../.." ABSOLUTE)

foreach(_pkg aicpu_sharder metadef ascend_hal securec runtime mmpa acl_rt atrace error_manager)
    if(NOT TARGET ${_pkg})
        find_cann_package(${_pkg} REQUIRED)
    endif()
endforeach()

if(NOT TARGET tilingdata_base)
    find_cann_package(tilingdata_base REQUIRED OBJECTS)
endif()

if(NOT TARGET unified_dlog)
    find_cann_package(unified_dlog REQUIRED)
endif()

include(${ASCENDC_DIR}/impl/adv_api/gen_kernel_tiling.cmake)

set(DEVICE_MODE TRUE)
set(KERNEL_MODE TRUE)

add_subdirectory(../../impl/aicpu_api impl/aicpu_api)
add_subdirectory(../../impl/adv_api/tiling impl/adv_api/tiling)
add_subdirectory(../../impl/adv_api/detail/hccl/cc impl/adv_api/detail/hccl/cc)
