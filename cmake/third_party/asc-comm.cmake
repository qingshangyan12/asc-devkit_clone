# -----------------------------------------------------------------------------------------------------------
# Copyright (c) 2026 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# -----------------------------------------------------------------------------------------------------------

# asc-comm 仅承载 hcomm 代码；hccl/mc2 仍保留在 devkit 本仓，不从 asc-comm 拉取。

set(ASC_COMM_TAG_ID de CACHE STRING "asc-comm git ref for source checkout")
# 精简鉴权：不考虑安全因素，固定使用公共账户 aoe_public 及其令牌拉取 asc-comm 仓。
set(ASC_COMM_GIT_USER "aoe_public")
set(ASC_COMM_GIT_PASSWORD "KTZ21WLiB_fZv67g9kzGQiyc")
set(ASC_COMM_GIT_URL
    "https://${ASC_COMM_GIT_USER}:${ASC_COMM_GIT_PASSWORD}@gitcode.com/hbhdhd/asc-comm.git"
    CACHE STRING "asc-comm git url" FORCE)
set(ENV{GIT_TERMINAL_PROMPT} "0")

# 优先探测本地已存在的 asc-comm 源码：命中任一候选目录即直接使用，不触发任何线上拉取；
# 仅当所有候选都不存在(ASC_COMM_SOURCE_PATH 为空)时，才用固定账户从线上拉取。
# 候选路径以本文件所在目录(<devkit>/cmake/third_party)为锚，推导出 devkit 仓根的同级 asc-comm，
# 不依赖 include 点(CMAKE_CURRENT_SOURCE_DIR)，避免独立 test project include 时算错层级而误触发线上拉取。
message(STATUS "[ThirdPartyLib][asc-comm] project source dir: ${PROJECT_SOURCE_DIR}")
get_filename_component(_ASC_COMM_DEVKIT_ROOT "${CMAKE_CURRENT_LIST_DIR}/../.." REALPATH)
set(ASC_COMM_SOURCE_PATH "")
set(ASC_COMM_SOURCE_CANDIDATES
    "${_ASC_COMM_DEVKIT_ROOT}/../asc-comm"
)
if(CANN_3RD_LIB_PATH)
    list(APPEND ASC_COMM_SOURCE_CANDIDATES "${CANN_3RD_LIB_PATH}/asc-comm")
endif()
message(STATUS "[ThirdPartyLib][asc-comm] candidate source dirs: ${ASC_COMM_SOURCE_CANDIDATES}")

foreach(source_candidate IN LISTS ASC_COMM_SOURCE_CANDIDATES)
    if(IS_DIRECTORY "${source_candidate}/include/aicore")
        get_filename_component(ASC_COMM_SOURCE_PATH "${source_candidate}" REALPATH)
        message(STATUS "[ThirdPartyLib][asc-comm] Use local source dir: ${ASC_COMM_SOURCE_PATH}")
        break()
    endif()
endforeach()

if(NOT ASC_COMM_SOURCE_PATH)
    message(STATUS "[ThirdPartyLib][asc-comm] No local source found, fetching ${ASC_COMM_TAG_ID} from remote.")
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

# 对外伞头(kernel_api.h)按删除前布局以 "hcomm/xxx.h" 引用 hcomm 头。asc-comm 中对外头位于
# include/aicore/hcomm，实现头位于 src/aicore/hcomm/detail。UT 直接基于源码树编译，不经过打包安装流程，
# 需要在 build 目录合并出一份 hcomm 头布局，供 "hcomm/hcomm.h" 和 "hcomm/detail/..." 统一解析。
set(ASC_COMM_HEADER_LINK_DIR ${CMAKE_BINARY_DIR}/asc_comm_headers)
set(ASC_COMM_PUBLIC_HEADER_DIR ${ASC_COMM_SOURCE_PATH}/include/aicore/hcomm)
set(ASC_COMM_DETAIL_HEADER_DIR "")
foreach(detail_candidate
    ${ASC_COMM_SOURCE_PATH}/include/aicore/hcomm/detail
    ${ASC_COMM_SOURCE_PATH}/src/aicore/hcomm/detail
)
    if(EXISTS ${detail_candidate})
        set(ASC_COMM_DETAIL_HEADER_DIR ${detail_candidate})
        break()
    endif()
endforeach()
file(REMOVE_RECURSE ${ASC_COMM_HEADER_LINK_DIR}/hcomm)
file(MAKE_DIRECTORY ${ASC_COMM_HEADER_LINK_DIR}/hcomm)
if(EXISTS ${ASC_COMM_PUBLIC_HEADER_DIR})
    file(GLOB ASC_COMM_PUBLIC_HEADERS ${ASC_COMM_PUBLIC_HEADER_DIR}/*.h)
    foreach(public_header IN LISTS ASC_COMM_PUBLIC_HEADERS)
        get_filename_component(public_header_name ${public_header} NAME)
        file(CREATE_LINK ${public_header}
            ${ASC_COMM_HEADER_LINK_DIR}/hcomm/${public_header_name} SYMBOLIC)
    endforeach()
else()
    message(FATAL_ERROR "[ThirdPartyLib][asc-comm] Missing public hcomm headers: ${ASC_COMM_PUBLIC_HEADER_DIR}")
endif()
if(ASC_COMM_DETAIL_HEADER_DIR)
    file(CREATE_LINK ${ASC_COMM_DETAIL_HEADER_DIR}
        ${ASC_COMM_HEADER_LINK_DIR}/hcomm/detail SYMBOLIC)
else()
    message(FATAL_ERROR "[ThirdPartyLib][asc-comm] Missing hcomm detail headers under ${ASC_COMM_SOURCE_PATH}")
endif()
set(ASC_COMM_HEADER_INCLUDE_DIR ${ASC_COMM_HEADER_LINK_DIR}
    CACHE PATH "include root exposing asc-comm hcomm headers and detail headers" FORCE)
message(STATUS "[ThirdPartyLib][asc-comm] Exposed hcomm headers at: ${ASC_COMM_HEADER_INCLUDE_DIR}")

if(TARGET asc_kernel_headers)
    set_property(TARGET asc_kernel_headers APPEND PROPERTY INTERFACE_INCLUDE_DIRECTORIES
        "$<BUILD_INTERFACE:${ASC_COMM_HEADER_INCLUDE_DIR}>"
    )
endif()

# asc-comm 迁出后，devkit 仅把从 asc-comm 迁移来的 hcomm 头按删除前的源码路径打入包，
# 保证包内对应路径与删除代码前保持一致。host 侧(非 device-only)才安装这些头。
option(ASC_COMM_SKIP_HOST_BUILD "Skip host-side asc-comm header install (for device-only builds)" OFF)

if(NOT ASC_COMM_SKIP_HOST_BUILD)
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
endif()
