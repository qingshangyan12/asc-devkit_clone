# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ----------------------------------------------------------------------------------------------------------

if (securec_FOUND)
  message(STATUS "Package securec has been found.")
  return()
endif()

set(_cmake_targets_defined "")
set(_cmake_targets_not_defined "")
set(_cmake_expected_targets "")
foreach(_cmake_expected_target IN ITEMS c_sec c_sec_static c_sec_headers)
  list(APPEND _cmake_expected_targets "${_cmake_expected_target}")
  if(TARGET "${_cmake_expected_target}")
    list(APPEND _cmake_targets_defined "${_cmake_expected_target}")
  else()
    list(APPEND _cmake_targets_not_defined "${_cmake_expected_target}")
  endif()
endforeach()
unset(_cmake_expected_target)

if(_cmake_targets_defined STREQUAL _cmake_expected_targets)
  set(securec_FOUND TRUE)
  unset(_cmake_targets_defined)
  unset(_cmake_targets_not_defined)
  unset(_cmake_expected_targets)
  return()
endif()
unset(_cmake_targets_defined)
unset(_cmake_targets_not_defined)
unset(_cmake_expected_targets)

set(C_SEC_HEAD_SEARCH_PATHS
  ${ASCEND_DIR}/${SYSTEM_PREFIX}/include
  # ${TOP_DIR}/abl/libc_sec/include             # compile with ci
)

set(C_SEC_LIB_SEARCH_PATHS ${ASCEND_DIR}/${SYSTEM_PREFIX})

find_path(C_SEC_INCLUDE NAMES securec.h PATHS ${C_SEC_HEAD_SEARCH_PATHS})
if(WIN32)
  find_library(C_SEC_STATIC_LIBRARY NAMES libc_sec_static.lib)
  find_file(C_SEC_SHARED_DLL NAMES libc_sec.dll PATH_SUFFIXES lib)
  find_library(C_SEC_IMPLIB NAMES libc_sec.lib)
else()
  find_library(C_SEC_STATIC_LIBRARY NAMES libc_sec.a PATHS ${C_SEC_LIB_SEARCH_PATHS} PATH_SUFFIXES lib64)
  find_library(C_SEC_SHARED_LIBRARY NAMES libc_sec.so PATHS ${C_SEC_LIB_SEARCH_PATHS} PATH_SUFFIXES lib64)
endif()

include(FindPackageHandleStandardArgs)
if(WIN32)
  find_package_handle_standard_args(securec
    FOUND_VAR
      securec_FOUND
    REQUIRED_VARS
      C_SEC_INCLUDE
      C_SEC_STATIC_LIBRARY
      C_SEC_SHARED_DLL
      C_SEC_IMPLIB
    )
elseif("${ENABLE_SECUREC_SHARED}" STREQUAL "OFF")
  find_package_handle_standard_args(securec
    FOUND_VAR
      securec_FOUND
    REQUIRED_VARS
      C_SEC_INCLUDE
      C_SEC_STATIC_LIBRARY
    )
else()
  find_package_handle_standard_args(securec
    FOUND_VAR
      securec_FOUND
    REQUIRED_VARS
      C_SEC_INCLUDE
      C_SEC_STATIC_LIBRARY
      C_SEC_SHARED_LIBRARY
    )
endif()

if(securec_FOUND)
  set(C_SEC_INCLUDE_DIR ${C_SEC_INCLUDE})
  get_filename_component(C_SEC_LIBRARY_DIR ${C_SEC_STATIC_LIBRARY} DIRECTORY)

  if(NOT TARGET c_sec_headers)
    add_library(c_sec_headers INTERFACE IMPORTED)
  endif()
  target_include_directories(c_sec_headers INTERFACE ${C_SEC_INCLUDE})

  if(NOT TARGET c_sec_static)
    add_library(c_sec_static STATIC IMPORTED)
  endif()
  get_target_property(_c_sec_static_is_imported c_sec_static IMPORTED)
  if(_c_sec_static_is_imported)
    set_target_properties(c_sec_static PROPERTIES
      INTERFACE_INCLUDE_DIRECTORIES "${C_SEC_INCLUDE}"
      IMPORTED_LOCATION "${C_SEC_STATIC_LIBRARY}"
    )
  else()
    set_target_properties(c_sec_static PROPERTIES
      INTERFACE_INCLUDE_DIRECTORIES "${C_SEC_INCLUDE}"
    )
  endif()

  if(NOT TARGET c_sec)
    add_library(c_sec SHARED IMPORTED)
  endif()
  get_target_property(_c_sec_is_imported c_sec IMPORTED)
  if(WIN32)
    if(_c_sec_is_imported)
      set_target_properties(c_sec PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${C_SEC_INCLUDE}"
        IMPORTED_IMPLIB "${C_SEC_IMPLIB}"
        IMPORTED_LOCATION "${C_SEC_SHARED_DLL}"
      )
    else()
      set_target_properties(c_sec PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${C_SEC_INCLUDE}"
      )
    endif()
  else()
    if(_c_sec_is_imported)
      set_target_properties(c_sec PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${C_SEC_INCLUDE}"
        IMPORTED_LOCATION "${C_SEC_SHARED_LIBRARY}"
      )
    else()
      set_target_properties(c_sec PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${C_SEC_INCLUDE}"
      )
    endif()
  endif()
  unset(_c_sec_is_imported)
  unset(_c_sec_static_is_imported)
endif()
