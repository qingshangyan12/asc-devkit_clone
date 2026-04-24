# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ----------------------------------------------------------------------------------------------------------

include(${CMAKE_ROOT}/Modules/CMakeDetermineCompiler.cmake)

# CMakeDetermineASCCompiler.cmake is used to initialize ASC-related variables.
# And this file will not be triggered again during incremental compilation.
message(STATUS "System processer: ${CMAKE_SYSTEM_PROCESSOR}")
if(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64")
    set(ASCEND_CANN_PACKAGE_LINUX_PATH "$ENV{ASCEND_HOME_PATH}/x86_64-linux")
    set(CMAKE_ASC_COMPILER_ARCHITECTURE_ID "x86_64")
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64|arm64|arm")
    set(ASCEND_CANN_PACKAGE_LINUX_PATH "$ENV{ASCEND_HOME_PATH}/aarch64-linux")
    set(CMAKE_ASC_COMPILER_ARCHITECTURE_ID "aarch64")
else()
    message(FATAL_ERROR "Unknown architecture: ${CMAKE_SYSTEM_PROCESSOR}")
endif()

set(_CMAKE_ASC_TOOLCHAIN_BIN_DIR "${ASCEND_CANN_PACKAGE_LINUX_PATH}/ccec_compiler/bin")

if(NOT CMAKE_ASC_COMPILER)
    find_program(CMAKE_ASC_COMPILER
        NAMES "bisheng"
        PATHS "${_CMAKE_ASC_TOOLCHAIN_BIN_DIR}" "$ENV{PATH}" "$ENV{ASCEND_HOME_PATH}"
        DOC "ASC Compiler")
else()
    _cmake_find_compiler_path(ASC)
endif()

mark_as_advanced(CMAKE_ASC_COMPILER)
message(STATUS "CMAKE_ASC_COMPILER: ${CMAKE_ASC_COMPILER}")

if(NOT _CMAKE_TOOLCHAIN_LOCATION)
    get_filename_component(_CMAKE_TOOLCHAIN_LOCATION "${CMAKE_ASC_COMPILER}" PATH)
endif()

set(_CMAKE_PROCESSING_LANGUAGE "ASC")
include(${CMAKE_ROOT}/Modules/CMakeFindBinUtils.cmake)
unset(_CMAKE_PROCESSING_LANGUAGE)

find_program(CMAKE_ASC_COMPILER_AR
    NAMES "llvm-ar" "ar"
    HINTS "${_CMAKE_TOOLCHAIN_LOCATION}" "${_CMAKE_ASC_TOOLCHAIN_BIN_DIR}"
    DOC "ASC compiler archiver")
if(NOT CMAKE_ASC_COMPILER_AR)
    set(CMAKE_ASC_COMPILER_AR "${CMAKE_AR}")
endif()
mark_as_advanced(CMAKE_ASC_COMPILER_AR)

find_program(CMAKE_ASC_COMPILER_LINKER
    NAMES "ld.lld" "ld"
    HINTS "${_CMAKE_TOOLCHAIN_LOCATION}" "${_CMAKE_ASC_TOOLCHAIN_BIN_DIR}"
    DOC "ASC compiler linker")
if(NOT CMAKE_ASC_COMPILER_LINKER)
    if(CMAKE_LINKER_LLD)
        set(CMAKE_ASC_COMPILER_LINKER "${CMAKE_LINKER_LLD}")
    else()
        set(CMAKE_ASC_COMPILER_LINKER "${CMAKE_LINKER}")
    endif()
endif()
mark_as_advanced(CMAKE_ASC_COMPILER_LINKER)

set(CMAKE_ASC_LLD_LINKER "${CMAKE_ASC_COMPILER_LINKER}")
mark_as_advanced(CMAKE_ASC_LLD_LINKER)

if(UNIX)
    set(CMAKE_ASC_OUTPUT_EXTENSION .o)
else()
    set(CMAKE_ASC_OUTPUT_EXTENSION .obj)
endif()

set(CMAKE_ASC_SOURCE_FILE_EXTENSIONS asc)
set(CMAKE_ASC_COMPILER_ENV_VAR "ASC")
set(CMAKE_ASC_STANDARD_DEFAULT 17)

set(CMAKE_ASC_COMPILER_WORKS 1)
message(STATUS "ASCEND_CANN_PACKAGE_LINUX_PATH: ${ASCEND_CANN_PACKAGE_LINUX_PATH}")
message(STATUS "CMAKE_ASC_COMPILER_AR: ${CMAKE_ASC_COMPILER_AR}")
message(STATUS "CMAKE_ASC_COMPILER_LINKER: ${CMAKE_ASC_COMPILER_LINKER}")

configure_file(${CMAKE_CURRENT_LIST_DIR}/CMakeASCCompiler.cmake.in
    ${CMAKE_PLATFORM_INFO_DIR}/CMakeASCCompiler.cmake
    @ONLY
)
