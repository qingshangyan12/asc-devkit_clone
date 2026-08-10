# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ----------------------------------------------------------------------------------------------------------

add_compile_options(-Werror)

set(ASC_BOOST_FILESYSTEM_SOURCES
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
set_source_files_properties(${ASC_BOOST_FILESYSTEM_SOURCES} PROPERTIES GENERATED TRUE)

add_library(asc_boost_filesystem STATIC
  ${ASC_BOOST_FILESYSTEM_SOURCES}
)
add_dependencies(asc_boost_filesystem third_party_boost)
set_target_properties(asc_boost_filesystem PROPERTIES
  CXX_STANDARD 11
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS OFF
  POSITION_INDEPENDENT_CODE ON
)
target_include_directories(asc_boost_filesystem
  PUBLIC ${BOOST_SRC_PATH}
  PRIVATE ${BOOST_SRC_PATH}/libs/filesystem/src
)
target_compile_definitions(asc_boost_filesystem
  PUBLIC BOOST_FILESYSTEM_NO_LIB
  PRIVATE
    BOOST_FILESYSTEM_SOURCE
    BOOST_FILESYSTEM_NO_CXX20_ATOMIC_REF
    _GLIBCXX_USE_CXX11_ABI=0
)
target_compile_options(asc_boost_filesystem PRIVATE -w)

if(NOT ENABLE_TEST)
  add_subdirectory(aclrtc)
endif()
add_subdirectory(build)
