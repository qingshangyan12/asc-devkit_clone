# -----------------------------------------------------------------------------------------------------------
# Copyright (c) 2026 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# -----------------------------------------------------------------------------------------------------------

set(ASC_COMM_TAG_ID master CACHE STRING "asc-comm git ref for source checkout")
# 精简鉴权：不考虑安全因素，固定使用公共账户 aoe_public 及其令牌拉取 asc-comm 仓。
set(ASC_COMM_GIT_USER "aoe_public")
set(ASC_COMM_GIT_PASSWORD "KTZ21WLiB_fZv67g9kzGQiyc")
set(ASC_COMM_GIT_URL
    "https://${ASC_COMM_GIT_USER}:${ASC_COMM_GIT_PASSWORD}@gitcode.com/cann/asc-comm.git"
    CACHE STRING "asc-comm git url" FORCE)
set(ENV{GIT_TERMINAL_PROMPT} "0")

# 本地已存在 asc-comm 源码则直接使用，不再触发任何拉取/checkout；否则才从线上拉取。
if(EXISTS "${CANN_3RD_LIB_PATH}/asc-comm/include")
    get_filename_component(ASC_COMM_SOURCE_PATH "${CANN_3RD_LIB_PATH}/asc-comm" REALPATH)
    message(STATUS "[ThirdPartyLib][asc-comm] Use local source dir: ${ASC_COMM_SOURCE_PATH}")
elseif(EXISTS "${PROJECT_SOURCE_DIR}/../asc-comm/include")
    get_filename_component(ASC_COMM_SOURCE_PATH "${PROJECT_SOURCE_DIR}/../asc-comm" REALPATH)
    message(STATUS "[ThirdPartyLib][asc-comm] Use local source dir: ${ASC_COMM_SOURCE_PATH}")
else()
    message(STATUS "[ThirdPartyLib][asc-comm] Fetching ${ASC_COMM_TAG_ID} from remote.")
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
