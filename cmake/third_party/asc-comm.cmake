# -----------------------------------------------------------------------------------------------------------
# Copyright (c) 2026 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# -----------------------------------------------------------------------------------------------------------

# 当前默认拉取master分支上最新代码，后续正式开源为了构建稳定性，会拉取固定change id代码。
# 后续如果更新asc-comm代码需要同步更新至devkit run包需要更新change id
set(ASC_COMM_TAG_ID master CACHE STRING "asc-comm git ref for source checkout")
# 当前因为为私仓，精简鉴权：不考虑安全因素，固定使用公共账户 aoe_public 及其令牌拉取 asc-comm 仓。
# 正式上库此明文账户密码会删除
set(ASC_COMM_GIT_USER "aoe_public")
set(ASC_COMM_GIT_PASSWORD "KTZ21WLiB_fZv67g9kzGQiyc")
set(ASC_COMM_GIT_URL
    "https://${ASC_COMM_GIT_USER}:${ASC_COMM_GIT_PASSWORD}@gitcode.com/cann/asc-comm.git"
    CACHE STRING "asc-comm git url" FORCE)
set(ENV{GIT_TERMINAL_PROMPT} "0")

# asc-comm 与 asc-devkit 按同级目录放置。路径以本文件所在目录(<devkit>/cmake/third_party)为锚，
# 对于CI环境已经存在asc-comm代码, 不去拉取代码否则使用submodule方式拉取asc-comm仓代码
message(STATUS "[ThirdPartyLib][asc-comm] project source dir: ${PROJECT_SOURCE_DIR}")
get_filename_component(_ASC_COMM_DEVKIT_ROOT "${CMAKE_CURRENT_LIST_DIR}/../.." REALPATH)
set(ASC_COMM_SOURCE_DIR "${_ASC_COMM_DEVKIT_ROOT}/../asc-comm")
if(EXISTS "${ASC_COMM_SOURCE_DIR}")
    get_filename_component(ASC_COMM_SOURCE_PATH ${ASC_COMM_SOURCE_DIR} REALPATH)
    message(STATUS "[ThirdPartyLib][asc-comm] Find source dir: ${ASC_COMM_SOURCE_PATH}")
else()
    if(EXISTS "${CMAKE_BINARY_DIR}/_deps/asc-comm-subbuild")
        file(REMOVE_RECURSE ${CMAKE_BINARY_DIR}/_deps/asc-comm-subbuild)
    endif()
    include(FetchContent)

    FetchContent_Declare(
        asc-comm
        GIT_REPOSITORY ${ASC_COMM_GIT_URL}
        GIT_TAG ${ASC_COMM_TAG_ID}
        GIT_PROGRESS TRUE
        SOURCE_DIR ${ASC_COMM_SOURCE_DIR}
    )
    FetchContent_Populate(asc-comm)

    get_filename_component(ASC_COMM_SOURCE_PATH ${ASC_COMM_SOURCE_DIR} REALPATH)
endif()

set(ASC_COMM_PUBLIC_HEADER_DIR ${ASC_COMM_SOURCE_PATH}/include/aicore/hcomm)
set(ASC_COMM_DETAIL_HEADER_DIR ${ASC_COMM_SOURCE_PATH}/src/aicore/hcomm/detail)
if(NOT EXISTS ${ASC_COMM_PUBLIC_HEADER_DIR})
    message(FATAL_ERROR "[ThirdPartyLib][asc-comm] Missing public hcomm headers: ${ASC_COMM_PUBLIC_HEADER_DIR}")
endif()

# asc-comm 迁出后，devkit 仅把从 asc-comm 迁移来的 hcomm 头按删除前的源码路径打入包。
# asc-comm 的 hcomm 对外头使用 "hcomm/detail/..." 引用实现头，因此 detail 必须安装到
# asc/include/adv_api/hcomm/detail，否则编译找不到 detail 头。
install(DIRECTORY ${ASC_COMM_PUBLIC_HEADER_DIR}/
    DESTINATION ${INSTALL_LIBRARY_DIR}/asc/include/adv_api/hcomm
    COMPONENT asc-devkit
    FILES_MATCHING PATTERN "*.h"
)
install(DIRECTORY ${ASC_COMM_DETAIL_HEADER_DIR}/
    DESTINATION ${INSTALL_LIBRARY_DIR}/asc/include/adv_api/hcomm/detail
    COMPONENT asc-devkit
    FILES_MATCHING PATTERN "*.h"
)
# 同时按删除前的源码布局保留一份 detail 到 asc/impl/adv_api/detail/hcomm，保持包内源码路径一致。
install(DIRECTORY ${ASC_COMM_DETAIL_HEADER_DIR}/
    DESTINATION ${INSTALL_LIBRARY_DIR}/asc/impl/adv_api/detail/hcomm
    COMPONENT asc-devkit
    FILES_MATCHING PATTERN "*.h"
)
