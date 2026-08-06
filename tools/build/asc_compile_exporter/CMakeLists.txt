# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ----------------------------------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.16.0)
project(asc_compile_exporter)

add_executable(asc_compile_exporter
  asc_compile_exporter.cpp
  manifest_generator.cpp
)

target_compile_options(asc_compile_exporter PRIVATE -Wextra -Wfloat-equal -fno-common -fpie -Werror -DNO_OPERATOR_IMPL)

if(BUILD_OPEN_PROJECT)
  target_include_directories(asc_compile_exporter PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../common
  )
else()
  target_include_directories(asc_compile_exporter PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${TOP_DIR}/abl/libc_sec/include
    ${TOP_DIR}/abl/mmpa/inc
    ${TOP_DIR}/abl/slog/inc/toolchain
    ${TOP_DIR}/asc/asc-devkit/tools/build/common
  )
endif()

target_link_libraries(asc_compile_exporter PRIVATE
  $<BUILD_INTERFACE:intf_pub>
  $<BUILD_INTERFACE:mmpa_headers>
  $<BUILD_INTERFACE:c_sec_headers>
  metadef_headers
  metadef
  dl
  unified_dlog
  mmpa
  error_manager
  json
)

target_compile_options(asc_compile_exporter PRIVATE
  -std=c++17
)

target_link_options(asc_compile_exporter PRIVATE
    -pie
    $<$<CONFIG:Release>:-s>
)

install(TARGETS asc_compile_exporter
  RUNTIME DESTINATION tools/asc_compile_exporter ${INSTALL_OPTIONAL}
  COMPONENT asc-devkit
  PERMISSIONS
    OWNER_READ OWNER_EXECUTE
    GROUP_READ GROUP_EXECUTE
    WORLD_READ WORLD_EXECUTE
)
