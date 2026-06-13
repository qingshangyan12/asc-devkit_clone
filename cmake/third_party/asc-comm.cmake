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
set(ASC_COMM_COPY_FILE_SCRIPT ${CMAKE_CURRENT_LIST_DIR}/copy_file.cmake)

foreach(pkg
        acl_rt
        error_manager
        mmpa
        runtime
        securec
        unified_dlog)
    if(NOT TARGET ${pkg})
        find_cann_package(${pkg} MODULE REQUIRED)
    endif()
endforeach()

if(NOT TARGET json)
    add_cann_third_party(json)
endif()

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

# CUSTOM_SIGN_SCRIPT 由上层(build.sh / device 子工程)透传，其默认值可能指向尚未存在的
# 路径（例如固定写死的 build/asc-comm/...）。asc-comm 的签名脚本随源码一起发布，这里统一以
# 真实的 ASC_COMM_SOURCE_PATH 为准重新推导：仅当未显式指定一个真实存在的脚本时才覆写，
# 避免 sign_file() 的 DEPENDS 引用到不存在的文件导致 "No rule to make target"。
set(ASC_COMM_DEFAULT_SIGN_SCRIPT
    ${ASC_COMM_SOURCE_PATH}/src/aicore/mc2/scripts/sign/community_sign_build.py)
if(NOT DEFINED CUSTOM_SIGN_SCRIPT
        OR "${CUSTOM_SIGN_SCRIPT}" STREQUAL ""
        OR NOT EXISTS "${CUSTOM_SIGN_SCRIPT}")
    if(EXISTS "${ASC_COMM_DEFAULT_SIGN_SCRIPT}")
        set(CUSTOM_SIGN_SCRIPT "${ASC_COMM_DEFAULT_SIGN_SCRIPT}")
        message(STATUS "[ThirdPartyLib][asc-comm] Use sign script: ${CUSTOM_SIGN_SCRIPT}")
    else()
        set(CUSTOM_SIGN_SCRIPT "")
        message(STATUS "[ThirdPartyLib][asc-comm] Sign script not found, disable custom sign.")
    endif()
endif()

set(ASC_COMM_MC2_HOST_BUILD_DIR ${CMAKE_BINARY_DIR}/asc-comm-mc2-host-build)
set(ASC_COMM_MC2_HOST_INSTALL_DIR ${CMAKE_BINARY_DIR}/asc-comm-mc2-host-install)
set(ASC_COMM_MC2_DEVICE_BUILD_DIR ${CMAKE_BINARY_DIR}/asc-comm-mc2-device-build)
set(ASC_COMM_MC2_DEVICE_INSTALL_DIR ${CMAKE_BINARY_DIR}/asc-comm-mc2-device-install)
set(ASC_COMM_MC2_CLIENT_FILE
    ${ASC_COMM_MC2_HOST_INSTALL_DIR}/${CMAKE_SYSTEM_PROCESSOR}-linux/lib64/libmc2_client.so)
set(ASC_COMM_MC2_COMPAT_FILE
    ${ASC_COMM_MC2_HOST_INSTALL_DIR}/${CMAKE_SYSTEM_PROCESSOR}-linux/lib64/libmc2_compat.so)
set(ASC_COMM_MC2_SERVER_JSON_FILE
    ${ASC_COMM_MC2_HOST_INSTALL_DIR}/opp/built-in/op_impl/aicpu/config/libmc2_server.json)
set(ASC_COMM_MC2_SERVER_TAR_FILE
    ${ASC_COMM_MC2_DEVICE_INSTALL_DIR}/opp/built-in/op_impl/aicpu/kernel/mc2_server.tar.gz)
set(ASC_COMM_MC2_VARS
    DEVICE_MODE
    KERNEL_MODE
    PRODUCT_SIDE
    BUILD_OPEN_PROJECT
    ENABLE_BUILD_DEVICE
    ENABLE_TEST
    ASCENDC_DIR
    ASCCOMM_CANN_INCLUDE_DIRS
    HCCL_CC_DIR
    HCCL_CC_INCLUDE_DIR
    HCCL_PUBLIC_INCLUDE_DIR
    HCCL_LOCAL_HEADER_DIRS
)

option(ASC_COMM_SKIP_HOST_BUILD "Skip host-side asc-comm build (for device-only builds)" OFF)

if(DEFINED KERNEL_MODE AND KERNEL_MODE)
    set(ASC_COMM_PARENT_KERNEL_MODE TRUE)
else()
    set(ASC_COMM_PARENT_KERNEL_MODE FALSE)
endif()

foreach(var IN LISTS ASC_COMM_MC2_VARS)
    if(DEFINED ${var})
        set(ASC_COMM_SAVED_${var}_DEFINED TRUE)
        set(ASC_COMM_SAVED_${var} "${${var}}")
    else()
        set(ASC_COMM_SAVED_${var}_DEFINED FALSE)
    endif()
endforeach()

if(NOT ASC_COMM_SKIP_HOST_BUILD)
    set(BUILD_OPEN_PROJECT ON)
    set(ENABLE_BUILD_DEVICE OFF)
    set(ENABLE_TEST OFF)
    set(DEVICE_MODE FALSE)
    set(KERNEL_MODE OFF)
    set(PRODUCT_SIDE host)
    add_subdirectory(
        ${ASC_COMM_SOURCE_PATH}/src/aicore/mc2
        ${ASC_COMM_MC2_HOST_BUILD_DIR}
    )

    add_custom_command(
        OUTPUT
            ${ASC_COMM_MC2_CLIENT_FILE}
            ${ASC_COMM_MC2_COMPAT_FILE}
            ${ASC_COMM_MC2_SERVER_JSON_FILE}
        COMMAND ${CMAKE_COMMAND}
            -DMC2_CLIENT_INPUT=$<TARGET_FILE:mc2_client>
            -DMC2_CLIENT_OUTPUT=${ASC_COMM_MC2_CLIENT_FILE}
            -DMC2_COMPAT_INPUT=$<TARGET_FILE:mc2_compat>
            -DMC2_COMPAT_OUTPUT=${ASC_COMM_MC2_COMPAT_FILE}
            -DMC2_SERVER_JSON_INPUT=${ASC_COMM_MC2_HOST_BUILD_DIR}/src/libmc2_server.json
            -DMC2_SERVER_JSON_OUTPUT=${ASC_COMM_MC2_SERVER_JSON_FILE}
            -P ${ASC_COMM_COPY_FILE_SCRIPT}
        DEPENDS mc2_client mc2_compat aicpu_kernel_json
        VERBATIM
    )
    add_custom_target(asc_comm_mc2_client ALL
        DEPENDS
            ${ASC_COMM_MC2_CLIENT_FILE}
            ${ASC_COMM_MC2_COMPAT_FILE}
            ${ASC_COMM_MC2_SERVER_JSON_FILE}
    )
endif()

if(ASC_COMM_PARENT_KERNEL_MODE)
    set(DEVICE_MODE TRUE)
    set(KERNEL_MODE TRUE)
    set(PRODUCT_SIDE device)
    add_subdirectory(
        ${ASC_COMM_SOURCE_PATH}/src/aicore/mc2
        ${ASC_COMM_MC2_DEVICE_BUILD_DIR}
    )

    add_custom_command(
        OUTPUT ${ASC_COMM_MC2_SERVER_TAR_FILE}
        COMMAND ${CMAKE_COMMAND}
            -DMC2_SERVER_TAR_INPUT=${CMAKE_BINARY_DIR}/mc2_server.tar.gz
            -DMC2_SERVER_TAR_OUTPUT=${ASC_COMM_MC2_SERVER_TAR_FILE}
            -P ${ASC_COMM_COPY_FILE_SCRIPT}
        DEPENDS generate_device_aicpu_package
        VERBATIM
    )
    add_custom_target(asc_comm_mc2_server ALL
        DEPENDS ${ASC_COMM_MC2_SERVER_TAR_FILE}
    )
endif()

foreach(var IN LISTS ASC_COMM_MC2_VARS)
    if(ASC_COMM_SAVED_${var}_DEFINED)
        set(${var} "${ASC_COMM_SAVED_${var}}")
    else()
        unset(${var})
    endif()
endforeach()

if(NOT ASC_COMM_SKIP_HOST_BUILD)
    install(FILES ${ASC_COMM_MC2_CLIENT_FILE}
        DESTINATION ${CMAKE_SYSTEM_PROCESSOR}-linux/lib64
        COMPONENT asc-devkit
    )

    install(FILES ${ASC_COMM_MC2_COMPAT_FILE}
        DESTINATION ${CMAKE_SYSTEM_PROCESSOR}-linux/lib64
        COMPONENT asc-devkit
    )

    install(FILES ${ASC_COMM_MC2_SERVER_JSON_FILE}
        DESTINATION opp/built-in/op_impl/aicpu/config
        COMPONENT asc-devkit
    )

    set(HCCL_CC_INCLUDE_DIR ${ASC_COMM_SOURCE_PATH}/include/aicore/mc2/internal CACHE STRING "hccl cc internal include dir" FORCE)
    set(HCCL_HEAD
        ${HCCL_CC_INCLUDE_DIR}/hccl.h
        ${HCCL_CC_INCLUDE_DIR}/hccl_mc2.h
    )
    install(FILES
        ${HCCL_HEAD}
        DESTINATION ${CMAKE_SYSTEM_PROCESSOR}-linux/include/hccl
        COMPONENT asc-devkit
    )

    install(DIRECTORY ${ASC_COMM_SOURCE_PATH}/include/aicore/mc2/
        DESTINATION ${INSTALL_LIBRARY_DIR}/asc/include/adv_api/hccl
        COMPONENT asc-devkit
        FILES_MATCHING PATTERN "*.h"
        PATTERN "internal" EXCLUDE
        PATTERN "detail" EXCLUDE
    )
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
        PATTERN "detail" EXCLUDE
    )
    install(DIRECTORY ${ASC_COMM_SOURCE_PATH}/include/aicore/hcomm/detail/
        DESTINATION ${INSTALL_LIBRARY_DIR}/asc/impl/adv_api/detail/hcomm
        COMPONENT asc-devkit
        FILES_MATCHING PATTERN "*.h"
    )
endif()

if(ASC_COMM_PARENT_KERNEL_MODE)
    install(FILES ${ASC_COMM_MC2_SERVER_TAR_FILE}
        DESTINATION opp/built-in/op_impl/aicpu/kernel
        COMPONENT asc-devkit
    )
endif()
