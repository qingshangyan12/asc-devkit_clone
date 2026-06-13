# -----------------------------------------------------------------------------------------------------------
# Copyright (c) 2026 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# -----------------------------------------------------------------------------------------------------------

set(ASC_COMM_TAG_ID master CACHE STRING "asc-comm git ref for mc2 components")
set(ASC_COMM_DEFAULT_AUTH_USER "aoe_public" CACHE STRING "default asc-comm git user when token authentication is enabled")
set(ASC_COMM_DEFAULT_AUTH_PASSWORD "KTZ21WLiB_fZv67g9kzGQiyc")
set(ASC_COMM_GIT_USER_FROM_TOKEN "")
set(ASC_COMM_GIT_USER "${ASC_COMM_DEFAULT_AUTH_USER}" CACHE STRING "asc-comm git user for https authentication" FORCE)
if(DEFINED ENV{ASC_COMM_GIT_TOKEN} AND NOT "$ENV{ASC_COMM_GIT_TOKEN}" STREQUAL "")
    set(ASC_COMM_GIT_PASSWORD "$ENV{ASC_COMM_GIT_TOKEN}")
elseif(DEFINED ENV{ASC_COMM_GIT_PASSWORD} AND NOT "$ENV{ASC_COMM_GIT_PASSWORD}" STREQUAL "")
    set(ASC_COMM_GIT_PASSWORD "$ENV{ASC_COMM_GIT_PASSWORD}")
elseif(DEFINED ASC_COMM_GIT_TOKEN AND NOT "${ASC_COMM_GIT_TOKEN}" STREQUAL "")
    set(ASC_COMM_GIT_PASSWORD "${ASC_COMM_GIT_TOKEN}")
elseif(DEFINED ASC_COMM_GIT_PASSWORD AND NOT "${ASC_COMM_GIT_PASSWORD}" STREQUAL "")
    set(ASC_COMM_GIT_PASSWORD "${ASC_COMM_GIT_PASSWORD}")
elseif(NOT "${ASC_COMM_DEFAULT_AUTH_PASSWORD}" STREQUAL "")
    set(ASC_COMM_GIT_PASSWORD "${ASC_COMM_DEFAULT_AUTH_PASSWORD}")
elseif(DEFINED ENV{GITCODE_TOKEN} AND NOT "$ENV{GITCODE_TOKEN}" STREQUAL "")
    set(ASC_COMM_GIT_PASSWORD "$ENV{GITCODE_TOKEN}")
elseif(DEFINED ENV{GITCODE_PASSWORD} AND NOT "$ENV{GITCODE_PASSWORD}" STREQUAL "")
    set(ASC_COMM_GIT_PASSWORD "$ENV{GITCODE_PASSWORD}")
elseif(DEFINED ENV{GIT_PASSWORD} AND NOT "$ENV{GIT_PASSWORD}" STREQUAL "")
    set(ASC_COMM_GIT_PASSWORD "$ENV{GIT_PASSWORD}")
elseif(DEFINED ENV{git_token} AND NOT "$ENV{git_token}" STREQUAL "")
    set(ASC_COMM_GIT_PASSWORD "$ENV{git_token}")
    if(DEFINED ENV{gitcode_name} AND NOT "$ENV{gitcode_name}" STREQUAL "")
        set(ASC_COMM_GIT_USER_FROM_TOKEN "$ENV{gitcode_name}")
    endif()
elseif(DEFINED ENV{access_token} AND NOT "$ENV{access_token}" STREQUAL "")
    set(ASC_COMM_GIT_PASSWORD "$ENV{access_token}")
    if(DEFINED ENV{user_name} AND NOT "$ENV{user_name}" STREQUAL "")
        set(ASC_COMM_GIT_USER_FROM_TOKEN "$ENV{user_name}")
    endif()
endif()
unset(ASC_COMM_GIT_TOKEN CACHE)
unset(ASC_COMM_GIT_PASSWORD CACHE)
if(DEFINED ASC_COMM_GIT_PASSWORD AND NOT "${ASC_COMM_GIT_PASSWORD}" STREQUAL ""
        AND "${ASC_COMM_GIT_USER}" STREQUAL "")
    if(NOT "${ASC_COMM_GIT_USER_FROM_TOKEN}" STREQUAL "")
        set(ASC_COMM_GIT_USER "${ASC_COMM_GIT_USER_FROM_TOKEN}")
    else()
        set(ASC_COMM_GIT_USER "${ASC_COMM_DEFAULT_AUTH_USER}")
    endif()
endif()
if(NOT "${ASC_COMM_GIT_USER}" STREQUAL "")
    set(ASC_COMM_GIT_USER "${ASC_COMM_GIT_USER}" CACHE STRING "asc-comm git user for https authentication" FORCE)
endif()
set(ASC_COMM_DEFAULT_GIT_URL "https://gitcode.com/cann/asc-comm.git")
if(DEFINED ASC_COMM_GIT_PASSWORD AND NOT "${ASC_COMM_GIT_PASSWORD}" STREQUAL ""
        AND NOT "${ASC_COMM_GIT_USER}" STREQUAL "")
    set(ASC_COMM_DEFAULT_GIT_URL "https://${ASC_COMM_GIT_USER}@gitcode.com/cann/asc-comm.git")
endif()
if(NOT DEFINED ASC_COMM_GIT_URL
        OR ASC_COMM_GIT_URL STREQUAL "https://gitcode.com/cann/asc-comm.git"
        OR ASC_COMM_GIT_URL MATCHES "^https://[^/@]+@gitcode\\.com/cann/asc-comm\\.git$")
    set(ASC_COMM_GIT_URL "${ASC_COMM_DEFAULT_GIT_URL}" CACHE STRING "asc-comm git url" FORCE)
else()
    set(ASC_COMM_GIT_URL "${ASC_COMM_GIT_URL}" CACHE STRING "asc-comm git url")
endif()
if(DEFINED ASC_COMM_GIT_PASSWORD AND NOT "${ASC_COMM_GIT_PASSWORD}" STREQUAL ""
        AND NOT "${ASC_COMM_GIT_USER}" STREQUAL "")
    set(ASC_COMM_GIT_ASKPASS "${CMAKE_BINARY_DIR}/asc_comm_git_askpass.sh")
    file(WRITE "${ASC_COMM_GIT_ASKPASS}"
"#!/bin/sh
case \"$1\" in
    *Username*) printf '%s\\n' \"$ASC_COMM_GIT_USER\" ;;
    *) printf '%s\\n' \"$ASC_COMM_GIT_PASSWORD\" ;;
esac
")
    file(CHMOD "${ASC_COMM_GIT_ASKPASS}"
        PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE
    )
    set(ENV{GIT_ASKPASS} "${ASC_COMM_GIT_ASKPASS}")
    set(ENV{ASC_COMM_GIT_USER} "${ASC_COMM_GIT_USER}")
    set(ENV{ASC_COMM_GIT_PASSWORD} "${ASC_COMM_GIT_PASSWORD}")
endif()
set(ENV{GIT_TERMINAL_PROMPT} "0")

function(asc_comm_checkout_source source_path)
    execute_process(
        COMMAND git fetch ${ASC_COMM_GIT_URL} ${ASC_COMM_TAG_ID} --depth=1
        WORKING_DIRECTORY ${source_path}
        TIMEOUT 20
        RESULT_VARIABLE FETCH_RESULT
        ERROR_VARIABLE FETCH_ERROR
        OUTPUT_QUIET
    )

    if(FETCH_RESULT EQUAL 0)
        set(checkout_ref FETCH_HEAD)
        message(STATUS "[ThirdPartyLib][asc-comm] fetched ${ASC_COMM_TAG_ID}")
    else()
        set(checkout_ref ${ASC_COMM_TAG_ID})
        message(WARNING "[ThirdPartyLib][asc-comm] Git fetch failed, checkout cached ${ASC_COMM_TAG_ID}: ${FETCH_ERROR}")
    endif()

    execute_process(
        COMMAND git checkout ${checkout_ref}
        WORKING_DIRECTORY ${source_path}
        RESULT_VARIABLE CHECKOUT_RESULT
        ERROR_VARIABLE CHECKOUT_ERROR
        OUTPUT_QUIET
    )
    if(NOT CHECKOUT_RESULT EQUAL 0)
        message(FATAL_ERROR "[ThirdPartyLib][asc-comm] Git checkout failed: ${CHECKOUT_ERROR}")
    endif()
endfunction()

if(EXISTS "${CANN_3RD_LIB_PATH}/asc-comm/.git")
    get_filename_component(ASC_COMM_SOURCE_PATH "${CANN_3RD_LIB_PATH}/asc-comm" REALPATH)
    message(STATUS "[ThirdPartyLib][asc-comm] Find source dir: ${ASC_COMM_SOURCE_PATH}")
    asc_comm_checkout_source(${ASC_COMM_SOURCE_PATH})
elseif(EXISTS "${PROJECT_SOURCE_DIR}/../asc-comm/.git")
    get_filename_component(ASC_COMM_SOURCE_PATH "${PROJECT_SOURCE_DIR}/../asc-comm" REALPATH)
    message(STATUS "[ThirdPartyLib][asc-comm] Find source dir: ${ASC_COMM_SOURCE_PATH}")
    asc_comm_checkout_source(${ASC_COMM_SOURCE_PATH})
else()
    include(FetchContent)
    FetchContent_Declare(
        asc-comm
        GIT_REPOSITORY ${ASC_COMM_GIT_URL}
        GIT_TAG ${ASC_COMM_TAG_ID}
        GIT_PROGRESS TRUE
        SOURCE_DIR ${CANN_3RD_LIB_PATH}/asc-comm
    )
    FetchContent_Populate(asc-comm)
    set(ASC_COMM_SOURCE_PATH ${CANN_3RD_LIB_PATH}/asc-comm)
endif()

# 对外伞头(tiling_api.h、kernel_api.h)按删除前布局以 "hccl/xxx.h"、"hcomm/xxx.h" 引用这些头，
# 但 asc-comm 中它们分别位于 include/aicore/mc2 与 include/aicore/hcomm。UT 直接基于源码树编译，
# 不经过打包安装流程，需要一个能解析上述前缀的 include 搜索根。这里在 build 目录下建立软链
# (hccl -> mc2, hcomm -> hcomm)，并把该目录缓存为全局变量，供 tests 下的 UT target 追加到
# 自身 include 路径。如此既不污染源码树，又与删除前的引用方式保持一致；软链目标内部的相对
# include(detail/...) 也因目录结构原样保留而自洽。
set(ASC_COMM_HEADER_LINK_DIR ${CMAKE_BINARY_DIR}/asc_comm_headers)
file(MAKE_DIRECTORY ${ASC_COMM_HEADER_LINK_DIR})
if(EXISTS ${ASC_COMM_SOURCE_PATH}/include/aicore/mc2)
    file(CREATE_LINK ${ASC_COMM_SOURCE_PATH}/include/aicore/mc2
        ${ASC_COMM_HEADER_LINK_DIR}/hccl SYMBOLIC)
endif()
if(EXISTS ${ASC_COMM_SOURCE_PATH}/include/aicore/hcomm)
    file(CREATE_LINK ${ASC_COMM_SOURCE_PATH}/include/aicore/hcomm
        ${ASC_COMM_HEADER_LINK_DIR}/hcomm SYMBOLIC)
endif()
set(ASC_COMM_HEADER_INCLUDE_DIR ${ASC_COMM_HEADER_LINK_DIR}
    CACHE PATH "include root exposing asc-comm headers as hccl/* and hcomm/*" FORCE)
message(STATUS "[ThirdPartyLib][asc-comm] Exposed hccl/hcomm headers at: ${ASC_COMM_HEADER_INCLUDE_DIR}")

# asc-comm 迁出后，devkit 不再构建 mc2，仅把从 asc-comm 迁移来的头文件按删除前的源码路径打入包，
# 保证包内对应路径与删除代码前保持一致。host 侧(非 device-only)才安装这些头。
option(ASC_COMM_SKIP_HOST_BUILD "Skip host-side asc-comm header install (for device-only builds)" OFF)

if(NOT ASC_COMM_SKIP_HOST_BUILD)
    # asc-comm 的 hccl(mc2)/hcomm 对外头使用同目录相对 include(如 hccl.h 内
    # #include "detail/impl/hccl_impl_def.h")，因此 detail 必须与对外头同目录安装到
    # asc/include/adv_api/hccl(该目录经包内软链映射为 tikcfw/lib/hccl 等)，否则编译找不到
    # detail 头。internal 仍单独安装到 hccl/internal。
    install(DIRECTORY ${ASC_COMM_SOURCE_PATH}/include/aicore/mc2/
        DESTINATION ${INSTALL_LIBRARY_DIR}/asc/include/adv_api/hccl
        COMPONENT asc-devkit
        FILES_MATCHING PATTERN "*.h"
        PATTERN "internal" EXCLUDE
    )
    # 同时按删除前的源码布局保留一份 detail 到 asc/impl/adv_api/detail/hccl，保持包内源码路径一致。
    install(DIRECTORY ${ASC_COMM_SOURCE_PATH}/include/aicore/mc2/detail/
        DESTINATION ${INSTALL_LIBRARY_DIR}/asc/impl/adv_api/detail/hccl
        COMPONENT asc-devkit
        FILES_MATCHING PATTERN "*.h"
    )
    install(DIRECTORY ${ASC_COMM_SOURCE_PATH}/include/aicore/mc2/internal/
        DESTINATION ${INSTALL_LIBRARY_DIR}/asc/include/adv_api/hccl/internal
        COMPONENT asc-devkit
        FILES_MATCHING PATTERN "*.h"
    )

    install(DIRECTORY ${ASC_COMM_SOURCE_PATH}/include/aicore/hcomm/
        DESTINATION ${INSTALL_LIBRARY_DIR}/asc/include/adv_api/hcomm
        COMPONENT asc-devkit
        FILES_MATCHING PATTERN "*.h"
    )
    install(DIRECTORY ${ASC_COMM_SOURCE_PATH}/include/aicore/hcomm/detail/
        DESTINATION ${INSTALL_LIBRARY_DIR}/asc/impl/adv_api/detail/hcomm
        COMPONENT asc-devkit
        FILES_MATCHING PATTERN "*.h"
    )
endif()
