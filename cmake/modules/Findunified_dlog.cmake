# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ----------------------------------------------------------------------------------------------------------

if (unified_dlog_FOUND)
    message(STATUS "Package unified_dlog has been found.")
    return()
endif()

set(_cmake_targets_defined "")
set(_cmake_targets_not_defined "")
set(_cmake_expected_targets "")
foreach(_cmake_expected_target IN ITEMS unified_dlog unified_dlog_headers)
    list(APPEND _cmake_expected_targets "${_cmake_expected_target}")
    if(TARGET "${_cmake_expected_target}")
        list(APPEND _cmake_targets_defined "${_cmake_expected_target}")
    else()
        list(APPEND _cmake_targets_not_defined "${_cmake_expected_target}")
    endif()
endforeach()
unset(_cmake_expected_target)

if(_cmake_targets_defined STREQUAL _cmake_expected_targets)
    set(unified_dlog_FOUND TRUE)
    unset(_cmake_targets_defined)
    unset(_cmake_targets_not_defined)
    unset(_cmake_expected_targets)
    return()
endif()
unset(_cmake_targets_defined)
unset(_cmake_targets_not_defined)
unset(_cmake_expected_targets)

find_path(unified_dlog_INCLUDE_DIR
    NAMES base/dlog_pub.h
    PATHS
        "${ASCEND_CANN_PACKAGE_ENV_PATH}"
        "${ASCEND_CANN_PACKAGE_PATH}"
    PATH_SUFFIXES pkg_inc
    NO_CMAKE_SYSTEM_PATH
    NO_CMAKE_FIND_ROOT_PATH)

find_library(unified_dlog_SHARED_LIBRARY
    NAMES libunified_dlog.so
    PATH_SUFFIXES lib64
    NO_CMAKE_SYSTEM_PATH
    NO_CMAKE_FIND_ROOT_PATH)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(unified_dlog
    FOUND_VAR
        unified_dlog_FOUND
    REQUIRED_VARS
        unified_dlog_INCLUDE_DIR
        unified_dlog_SHARED_LIBRARY
)

if(unified_dlog_FOUND)
    set(unified_dlog_INCLUDE_DIR "${unified_dlog_INCLUDE_DIR}")
    include(CMakePrintHelpers)
    message(STATUS "Variables in unified_dlog module:")
    cmake_print_variables(unified_dlog_INCLUDE_DIR)
    cmake_print_variables(unified_dlog_SHARED_LIBRARY)

    if(NOT TARGET unified_dlog_headers)
        add_library(unified_dlog_headers INTERFACE IMPORTED)
    endif()
    set_target_properties(unified_dlog_headers PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${unified_dlog_INCLUDE_DIR};${unified_dlog_INCLUDE_DIR}/base"
    )

    if(NOT TARGET unified_dlog)
        add_library(unified_dlog SHARED IMPORTED)
    endif()
    get_target_property(_unified_dlog_is_imported unified_dlog IMPORTED)
    if(_unified_dlog_is_imported)
        set_target_properties(unified_dlog PROPERTIES
            INTERFACE_COMPILE_DEFINITIONS "PROCESS_LOG"
            INTERFACE_INCLUDE_DIRECTORIES "${unified_dlog_INCLUDE_DIR};${unified_dlog_INCLUDE_DIR}/base"
            IMPORTED_LOCATION "${unified_dlog_SHARED_LIBRARY}"
        )
    else()
        set_target_properties(unified_dlog PROPERTIES
            INTERFACE_COMPILE_DEFINITIONS "PROCESS_LOG"
            INTERFACE_INCLUDE_DIRECTORIES "${unified_dlog_INCLUDE_DIR};${unified_dlog_INCLUDE_DIR}/base"
        )
    endif()

    include(CMakePrintHelpers)
    cmake_print_properties(TARGETS unified_dlog
        PROPERTIES INTERFACE_COMPILE_DEFINITIONS INTERFACE_INCLUDE_DIRECTORIES IMPORTED_LOCATION
    )
    cmake_print_properties(TARGETS unified_dlog_headers
        PROPERTIES INTERFACE_INCLUDE_DIRECTORIES
    )
    unset(_unified_dlog_is_imported)
endif()

# Cleanup temporary variables.
set(unified_dlog_INCLUDE_DIR)
