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

# Resolve the CANN install root in priority order:
#   1. cmake variable ASCEND_CANN_PACKAGE_PATH (the canonical knob this repo
#      uses; cmake/config.cmake makes it mandatory and the device
#      ExternalProject in impl/adv_api/CMakeLists.txt forwards it)
#   2. env ASCEND_HOME_PATH (the standard CANN env var)
#   3. /usr/local/Ascend (last-resort default)
if(DEFINED ASCEND_CANN_PACKAGE_PATH AND NOT "${ASCEND_CANN_PACKAGE_PATH}" STREQUAL "")
    set(_ASCEND_HOME_PATH "${ASCEND_CANN_PACKAGE_PATH}")
elseif(DEFINED ENV{ASCEND_HOME_PATH} AND NOT "$ENV{ASCEND_HOME_PATH}" STREQUAL "")
    set(_ASCEND_HOME_PATH "$ENV{ASCEND_HOME_PATH}")
else()
    set(_ASCEND_HOME_PATH "/usr/local/Ascend")
endif()

set(_AICPU_HCCL_TAR
    "${_ASCEND_HOME_PATH}/opp/built-in/op_impl/aicpu/kernel/aicpu_hccl.tar.gz")

if(NOT EXISTS "${_AICPU_HCCL_TAR}")
    # Diagnostic: walk down each level of the expected path and report what's
    # actually there so we can see at which step it diverges from expectation.
    message(STATUS "extract_aicpu_hccl: tar not found, dumping path layout")
    message(STATUS "  ASCEND_CANN_PACKAGE_PATH (cmake) = '${ASCEND_CANN_PACKAGE_PATH}'")
    message(STATUS "  ASCEND_HOME_PATH (env)           = '$ENV{ASCEND_HOME_PATH}'")
    message(STATUS "  resolved _ASCEND_HOME_PATH       = '${_ASCEND_HOME_PATH}'")
    message(STATUS "  expected _AICPU_HCCL_TAR         = '${_AICPU_HCCL_TAR}'")

    set(_DIAG_LEVELS
        "${_ASCEND_HOME_PATH}"
        "${_ASCEND_HOME_PATH}/opp"
        "${_ASCEND_HOME_PATH}/opp/built-in"
        "${_ASCEND_HOME_PATH}/opp/built-in/op_impl"
        "${_ASCEND_HOME_PATH}/opp/built-in/op_impl/aicpu"
        "${_ASCEND_HOME_PATH}/opp/built-in/op_impl/aicpu/kernel"
    )
    foreach(_DIAG_DIR IN LISTS _DIAG_LEVELS)
        if(IS_DIRECTORY "${_DIAG_DIR}")
            file(GLOB _DIAG_ENTRIES RELATIVE "${_DIAG_DIR}" "${_DIAG_DIR}/*")
            list(LENGTH _DIAG_ENTRIES _DIAG_COUNT)
            message(STATUS "  [DIR ] ${_DIAG_DIR} (${_DIAG_COUNT} entries):")
            foreach(_DIAG_ENTRY IN LISTS _DIAG_ENTRIES)
                if(IS_DIRECTORY "${_DIAG_DIR}/${_DIAG_ENTRY}")
                    message(STATUS "         <d> ${_DIAG_ENTRY}")
                else()
                    message(STATUS "         <f> ${_DIAG_ENTRY}")
                endif()
            endforeach()
        elseif(EXISTS "${_DIAG_DIR}")
            message(STATUS "  [FILE] ${_DIAG_DIR} (exists, not a directory)")
        else()
            message(STATUS "  [MISS] ${_DIAG_DIR}")
        endif()
    endforeach()

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
