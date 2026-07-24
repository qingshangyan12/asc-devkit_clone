# -----------------------------------------------------------------------------------------------------------
# Copyright (c) 2026 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# -----------------------------------------------------------------------------------------------------------

include_guard(GLOBAL)

# aicpu 头文件在 asc-comm 的位置
set(ASC_COMM_AICPU_INCLUDE_RELATIVE_DIR "include/aicpu/hcomm")
set(ASC_COMM_SOURCE_PATH "" CACHE PATH "Path to asc-comm source tree")
set(ASC_COMM_URL "https://gitcode.com/cann/asc-comm.git" CACHE STRING "asc-comm git url")
set(ASC_COMM_TAG_ID "master" CACHE STRING "asc-comm git ref")
# 如果要拉取 pr 使用 refs/merge-requests/123/head 的 tag id
# set(ASC_COMM_TAG_ID refs/merge-requests/123/head CACHE STRING "asc-comm git ref for source checkout")

get_filename_component(_ASC_COMM_SOURCE_ROOT "${CMAKE_CURRENT_LIST_DIR}/../.." REALPATH)
set(ASC_COMM_SOURCE_CANDIDATES "")
if(ASC_COMM_SOURCE_PATH)
    list(APPEND ASC_COMM_SOURCE_CANDIDATES "${ASC_COMM_SOURCE_PATH}")
endif()
list(APPEND ASC_COMM_SOURCE_CANDIDATES "${_ASC_COMM_SOURCE_ROOT}/../asc-comm")
if(CANN_3RD_LIB_PATH)
    list(APPEND ASC_COMM_SOURCE_CANDIDATES "${CANN_3RD_LIB_PATH}/asc-comm")
endif()

foreach(source_candidate IN LISTS ASC_COMM_SOURCE_CANDIDATES)
    if(IS_DIRECTORY "${source_candidate}/${ASC_COMM_AICPU_INCLUDE_RELATIVE_DIR}")
        get_filename_component(ASC_COMM_SOURCE_PATH "${source_candidate}" REALPATH)
        message(STATUS "[ThirdPartyLib][asc-comm] Use local source dir: ${ASC_COMM_SOURCE_PATH}")
        break()
    endif()
endforeach()

if(NOT ASC_COMM_SOURCE_PATH)
    set(_ASC_COMM_DOWNLOAD_SOURCE_DIR "${CMAKE_BINARY_DIR}/_deps/asc-comm-src")

    message(STATUS "[ThirdPartyLib][asc-comm] No local source found, fetching ${ASC_COMM_TAG_ID} from ${ASC_COMM_URL}.")
    set(ENV{GIT_TERMINAL_PROMPT} "0")
    include(FetchContent)
    FetchContent_Declare(
        asc_comm
        GIT_REPOSITORY ${ASC_COMM_URL}
        GIT_TAG ${ASC_COMM_TAG_ID}
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE
        SOURCE_DIR ${_ASC_COMM_DOWNLOAD_SOURCE_DIR}
    )
    FetchContent_GetProperties(asc_comm)
    if(NOT asc_comm_POPULATED)
        FetchContent_Populate(asc_comm)
    endif()
    set(ASC_COMM_SOURCE_PATH ${_ASC_COMM_DOWNLOAD_SOURCE_DIR} CACHE PATH "Path to asc-comm source tree" FORCE)
endif()

set(ASC_COMM_AICPU_INCLUDE_DIR "${ASC_COMM_SOURCE_PATH}/${ASC_COMM_AICPU_INCLUDE_RELATIVE_DIR}")
if(NOT IS_DIRECTORY "${ASC_COMM_AICPU_INCLUDE_DIR}")
    message(FATAL_ERROR "[ThirdPartyLib][asc-comm] hcomm headers not found: ${ASC_COMM_AICPU_INCLUDE_DIR}")
endif()

# TODO: 目前 asc-comm include 目录没有清理干净，还有其余头文件，所以显式 hcomm_primitives.h, hcomm_res_defs.h
install(DIRECTORY "${ASC_COMM_AICPU_INCLUDE_DIR}/"
    DESTINATION ${INSTALL_LIBRARY_DIR}/asc/include/adv_api/asc-comm/aicpu/hcomm
    COMPONENT asc-devkit
    FILES_MATCHING
    PATTERN "hcomm_primitives.h"
    PATTERN "hcomm_res_defs.h"
)
