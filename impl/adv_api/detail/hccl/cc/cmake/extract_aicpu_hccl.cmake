# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2026 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ----------------------------------------------------------------------------------------------------------
#
# Extracts the device-side libscatter_aicpu_kernel.so and libhccl_kernel_compat.so
# from CANN's signed bundle so the device-side CMake targets (asc_ccl_kernel,
# asc_aicpu_custom) can link against them directly at build time.
#
# Provides:
#   - aicpu_hccl_extracted     custom target (depend on this from any consumer)
#   - AICPU_HCCL_EXTRACT_DIR   absolute path containing the two .so files
#   - AICPU_HCCL_SCATTER_LIB   full path to libscatter_aicpu_kernel.so
#   - AICPU_HCCL_KCOMPAT_LIB   full path to libhccl_kernel_compat.so
#
# Guard against double-include in case multiple device-side CMakeLists pull
# this in.
if(DEFINED _EXTRACT_AICPU_HCCL_INCLUDED)
    return()
endif()
set(_EXTRACT_AICPU_HCCL_INCLUDED TRUE)

if(DEFINED ENV{ASCEND_HOME_PATH})
    set(_ASCEND_HOME_PATH "$ENV{ASCEND_HOME_PATH}")
else()
    set(_ASCEND_HOME_PATH "/usr/local/Ascend")
endif()

set(_AICPU_HCCL_TAR
    "${_ASCEND_HOME_PATH}/opp/built-in/op_impl/aicpu/kernel/aicpu_hccl.tar.gz")

if(NOT EXISTS "${_AICPU_HCCL_TAR}")
    message(FATAL_ERROR
        "extract_aicpu_hccl.cmake: cannot find ${_AICPU_HCCL_TAR}. "
        "Install CANN and ensure ASCEND_HOME_PATH points to its root, "
        "or skip device-side targets if you don't need them.")
endif()

set(AICPU_HCCL_EXTRACT_DIR "${CMAKE_BINARY_DIR}/aicpu_hccl_extracted"
    CACHE INTERNAL "Directory holding the extracted device-side AICPU libs")
set(AICPU_HCCL_SCATTER_LIB "${AICPU_HCCL_EXTRACT_DIR}/libscatter_aicpu_kernel.so"
    CACHE INTERNAL "Extracted libscatter_aicpu_kernel.so path")
set(AICPU_HCCL_KCOMPAT_LIB "${AICPU_HCCL_EXTRACT_DIR}/libhccl_kernel_compat.so"
    CACHE INTERNAL "Extracted libhccl_kernel_compat.so path")

# HCCL_CC_DIR is defined by the asc-devkit top-level CMake and points at
# impl/adv_api/detail/hccl/cc. Fall back to a path relative to this .cmake
# file if it isn't set yet.
if(DEFINED HCCL_CC_DIR)
    set(_EXTRACT_SCRIPT "${HCCL_CC_DIR}/cmake/scripts/extract_aicpu_hccl.py")
else()
    set(_EXTRACT_SCRIPT "${CMAKE_CURRENT_LIST_DIR}/scripts/extract_aicpu_hccl.py")
endif()

if(NOT DEFINED HI_PYTHON)
    set(HI_PYTHON "python3")
endif()

add_custom_command(
    OUTPUT "${AICPU_HCCL_SCATTER_LIB}" "${AICPU_HCCL_KCOMPAT_LIB}"
    COMMAND ${CMAKE_COMMAND} -E make_directory "${AICPU_HCCL_EXTRACT_DIR}"
    COMMAND ${HI_PYTHON} "${_EXTRACT_SCRIPT}"
            --input "${_AICPU_HCCL_TAR}"
            --out-dir "${AICPU_HCCL_EXTRACT_DIR}"
    DEPENDS "${_EXTRACT_SCRIPT}" "${_AICPU_HCCL_TAR}"
    COMMENT "Extracting libscatter_aicpu_kernel.so from CANN aicpu_hccl.tar.gz"
    VERBATIM
)

add_custom_target(aicpu_hccl_extracted
    DEPENDS "${AICPU_HCCL_SCATTER_LIB}" "${AICPU_HCCL_KCOMPAT_LIB}")

message(STATUS "extract_aicpu_hccl: source       = ${_AICPU_HCCL_TAR}")
message(STATUS "extract_aicpu_hccl: extract dir  = ${AICPU_HCCL_EXTRACT_DIR}")
