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

set(CMAKE_ASC_ARCHITECTURES "dav-2201" CACHE STRING "NPU architecture: dav-2201, dav-3510")

find_package(ASC REQUIRED)
project(triple_chevron_notation LANGUAGES ASC CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

set(COMMON_COMPILE_OPTIONS
    -Wall
    -Wextra
    -Wno-unused-parameter
)

set(PROJECT_OUTPUT_DIR "${CMAKE_SOURCE_DIR}/output")
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set(OPP_OS_TYPE "windows")
else()
    set(OPP_OS_TYPE "linux")
endif()

string(TOLOWER "${CMAKE_SYSTEM_PROCESSOR}" CMAKE_SYSTEM_PROCESSOR_LOWER)
if(CMAKE_SYSTEM_PROCESSOR_LOWER MATCHES "^(aarch64|arm64)$")
    set(OPP_CPU_TYPE "aarch64")
elseif(CMAKE_SYSTEM_PROCESSOR_LOWER MATCHES "^(x86_64|amd64)$")
    set(OPP_CPU_TYPE "x86_64")
else()
    set(OPP_CPU_TYPE "${CMAKE_SYSTEM_PROCESSOR_LOWER}")
endif()

set(CUSTOM_OP_OUTPUT_DIR "${PROJECT_OUTPUT_DIR}/op_graph/lib/${OPP_OS_TYPE}/${OPP_CPU_TYPE}")
set(CUSTOM_OP_INCLUDE_DIR "${PROJECT_OUTPUT_DIR}/op_graph/include")
file(MAKE_DIRECTORY "${CUSTOM_OP_OUTPUT_DIR}" "${CUSTOM_OP_INCLUDE_DIR}")

set(OP_PROTO_HEADER_SOURCE_FILE "${CMAKE_SOURCE_DIR}/add_custom_proto.h")
configure_file("${OP_PROTO_HEADER_SOURCE_FILE}" "${CUSTOM_OP_INCLUDE_DIR}/add_custom_proto.h" COPYONLY)

set(ASCEND_HOME_PATH_OVERRIDE "" CACHE PATH "Optional ASCEND_HOME_PATH override")
if(ASCEND_HOME_PATH_OVERRIDE)
    set(ASCEND_HOME_PATH "${ASCEND_HOME_PATH_OVERRIDE}")
else()
    set(ASCEND_HOME_PATH "$ENV{ASCEND_HOME_PATH}")
endif()

if(ASCEND_HOME_PATH)
    message(STATUS "ASCEND_HOME_PATH: ${ASCEND_HOME_PATH}")
else()
    message(WARNING "ASCEND_HOME_PATH is empty. Configure succeeds, but compilation requires a valid CANN toolkit path.")
endif()

add_library(add_custom_op SHARED
    add_custom.asc
)
target_compile_options(add_custom_op PRIVATE
    $<$<COMPILE_LANGUAGE:ASC>:--npu-arch=${CMAKE_ASC_ARCHITECTURES}>
)
target_compile_definitions(add_custom_op PRIVATE _GLIBCXX_USE_CXX11_ABI=0)
if(ASCEND_HOME_PATH)
    target_include_directories(add_custom_op PRIVATE
        "${ASCEND_HOME_PATH}/include"
        "${ASCEND_HOME_PATH}/include/graph"
        "${ASCEND_HOME_PATH}/include/register"
        "${ASCEND_HOME_PATH}/include/external"
    )
    target_link_directories(add_custom_op PRIVATE "${ASCEND_HOME_PATH}/lib64")
    target_link_libraries(add_custom_op PRIVATE
        -Wl,--no-as-needed
        ascendcl
        ascendc_runtime
        lowering
        register
        gert
        dl
        -Wl,--as-needed
    )
endif()
set_target_properties(add_custom_op PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${CUSTOM_OP_OUTPUT_DIR}"
    RUNTIME_OUTPUT_DIRECTORY "${CUSTOM_OP_OUTPUT_DIR}"
    ARCHIVE_OUTPUT_DIRECTORY "${CUSTOM_OP_OUTPUT_DIR}"
)

add_executable(triple_chevron_graph
    graph.cpp
)
target_compile_options(triple_chevron_graph PRIVATE ${COMMON_COMPILE_OPTIONS})
target_compile_definitions(triple_chevron_graph PRIVATE _GLIBCXX_USE_CXX11_ABI=0)
if(ASCEND_HOME_PATH)
    target_include_directories(triple_chevron_graph PRIVATE
        "${CMAKE_SOURCE_DIR}"
        "${ASCEND_HOME_PATH}/include"
        "${ASCEND_HOME_PATH}/include/graph"
        "${ASCEND_HOME_PATH}/include/ge"
        "${ASCEND_HOME_PATH}/opp/built-in/op_proto/inc"
    )
    target_link_directories(triple_chevron_graph PRIVATE "${ASCEND_HOME_PATH}/lib64")
    target_link_libraries(triple_chevron_graph PRIVATE
        graph
        ge_runner
        ge_compiler
        ascendcl
        graph_base
    )
endif()
set_target_properties(triple_chevron_graph PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")

install(FILES "${OP_PROTO_HEADER_SOURCE_FILE}" DESTINATION "${CUSTOM_OP_INCLUDE_DIR}")
