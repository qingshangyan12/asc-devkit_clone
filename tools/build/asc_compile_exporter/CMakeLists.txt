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

set(ASC_COMPILE_EXPORTER_BOOST_FILESYSTEM_SOURCES
  ${BOOST_SRC_PATH}/libs/filesystem/src/codecvt_error_category.cpp
  ${BOOST_SRC_PATH}/libs/filesystem/src/directory.cpp
  ${BOOST_SRC_PATH}/libs/filesystem/src/exception.cpp
  ${BOOST_SRC_PATH}/libs/filesystem/src/operations.cpp
  ${BOOST_SRC_PATH}/libs/filesystem/src/path.cpp
  ${BOOST_SRC_PATH}/libs/filesystem/src/path_traits.cpp
  ${BOOST_SRC_PATH}/libs/filesystem/src/portability.cpp
  ${BOOST_SRC_PATH}/libs/filesystem/src/unique_path.cpp
  ${BOOST_SRC_PATH}/libs/filesystem/src/utf8_codecvt_facet.cpp
)
set_source_files_properties(${ASC_COMPILE_EXPORTER_BOOST_FILESYSTEM_SOURCES} PROPERTIES GENERATED TRUE)

add_library(asc_compile_exporter_boost_filesystem STATIC
  ${ASC_COMPILE_EXPORTER_BOOST_FILESYSTEM_SOURCES}
)
add_dependencies(asc_compile_exporter_boost_filesystem third_party_boost)
set_target_properties(asc_compile_exporter_boost_filesystem PROPERTIES
  CXX_STANDARD 11
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS OFF
  POSITION_INDEPENDENT_CODE ON
)
target_include_directories(asc_compile_exporter_boost_filesystem
  PUBLIC ${BOOST_SRC_PATH}
  PRIVATE ${BOOST_SRC_PATH}/libs/filesystem/src
)
target_compile_definitions(asc_compile_exporter_boost_filesystem
  PUBLIC BOOST_FILESYSTEM_NO_LIB
  PRIVATE
    BOOST_FILESYSTEM_SOURCE
    BOOST_FILESYSTEM_NO_CXX20_ATOMIC_REF
    _GLIBCXX_USE_CXX11_ABI=0
)
target_compile_options(asc_compile_exporter_boost_filesystem PRIVATE -w)

set(ASC_COMPILE_EXPORTER_PUBLIC_INTERFACE intf_pub)
if(TARGET intf_pub_base)
  set(ASC_COMPILE_EXPORTER_PUBLIC_INTERFACE intf_pub_base)
endif()

set(ASC_COMPILE_EXPORTER_GENERATED_DIR
  "${CMAKE_BINARY_DIR}/generated/asc_compile_exporter"
  CACHE INTERNAL "Generated headers for asc_compile_exporter"
)
set(ASCENDC_MANIFEST_ABI_HEADER
  "${CMAKE_CURRENT_SOURCE_DIR}/../../aclrtc/specialization/ascendc_manifest_abi.h"
)
set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS "${ASCENDC_MANIFEST_ABI_HEADER}")
file(READ "${ASCENDC_MANIFEST_ABI_HEADER}" ASCENDC_MANIFEST_ABI_HEADER_CONTENT)
file(MAKE_DIRECTORY "${ASC_COMPILE_EXPORTER_GENERATED_DIR}")
configure_file(
  manifest_abi_header_content.h.in
  "${ASC_COMPILE_EXPORTER_GENERATED_DIR}/manifest_abi_header_content.h"
  @ONLY
)

add_executable(asc_compile_exporter
  asc_compile_exporter.cpp
  build_workspace.cpp
  bundle_source_generator.cpp
  collected_manifest_repository.cpp
  manifest_bundle_compiler.cpp
  ../common/file_utils.cpp
  ../common/process_runner.cpp
)

set_target_properties(asc_compile_exporter PROPERTIES
  CXX_STANDARD 11
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS OFF
)

target_compile_options(asc_compile_exporter PRIVATE -Wextra -Wfloat-equal -fno-common -fpie -Werror)

if(BUILD_OPEN_PROJECT)
  target_include_directories(asc_compile_exporter PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../common
    ${ASC_COMPILE_EXPORTER_GENERATED_DIR}
  )
else()
  target_include_directories(asc_compile_exporter PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${ASC_COMPILE_EXPORTER_GENERATED_DIR}
    ${TOP_DIR}/abl/libc_sec/include
    ${TOP_DIR}/abl/mmpa/inc
    ${TOP_DIR}/abl/slog/inc/toolchain
    ${TOP_DIR}/asc/asc-devkit/tools/build/common
  )
endif()

target_link_libraries(asc_compile_exporter PRIVATE
  asc_compile_exporter_boost_filesystem
  $<BUILD_INTERFACE:${ASC_COMPILE_EXPORTER_PUBLIC_INTERFACE}>
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
