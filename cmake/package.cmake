# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ----------------------------------------------------------------------------------------------------------

#### CPACK to package run #####
message(STATUS "System processor: ${CMAKE_SYSTEM_PROCESSOR}")
if (CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64")
    message(STATUS "Detected architecture: x86_64")
    set(ARCH x86_64)
elseif (CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64|arm64|arm")
    message(STATUS "Detected architecture: ARM64")
    set(ARCH aarch64)
else ()
    message(WARNING "Unknown architecture: ${CMAKE_SYSTEM_PROCESSOR}")
endif ()
# 打印路径
message(STATUS "CMAKE_INSTALL_PREFIX = ${CMAKE_INSTALL_PREFIX}")
message(STATUS "CMAKE_SOURCE_DIR = ${CMAKE_SOURCE_DIR}")
message(STATUS "CMAKE_CURRENT_SOURCE_DIR = ${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "CMAKE_BINARY_DIR = ${CMAKE_BINARY_DIR}")

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/third_party/makeself-fetch.cmake)

set(script_prefix ${CMAKE_SOURCE_DIR}/scripts/package/scripts/)
install(DIRECTORY ${script_prefix}/
    DESTINATION asc-devkit/script
    COMPONENT asc-devkit
    FILE_PERMISSIONS
    OWNER_READ OWNER_WRITE OWNER_EXECUTE  # 文件权限
    GROUP_READ GROUP_EXECUTE
    WORLD_READ WORLD_EXECUTE
    DIRECTORY_PERMISSIONS
    OWNER_READ OWNER_WRITE OWNER_EXECUTE  # 目录权限
    GROUP_READ GROUP_EXECUTE
    WORLD_READ WORLD_EXECUTE
)
set(SCRIPTS_FILES
    ${CANN_CMAKE_DIR}/scripts/install/check_version_required.awk
    ${CANN_CMAKE_DIR}/scripts/install/common_func.inc
    ${CANN_CMAKE_DIR}/scripts/install/common_interface.sh
    ${CANN_CMAKE_DIR}/scripts/install/common_interface.csh
    ${CANN_CMAKE_DIR}/scripts/install/common_interface.fish
    ${CANN_CMAKE_DIR}/scripts/install/version_compatiable.inc
    ${CMAKE_SOURCE_DIR}/scripts/package/common/py/merge_binary_info_config.py
)

install(FILES ${SCRIPTS_FILES}
    DESTINATION asc-devkit/script COMPONENT asc-devkit
)

set(COMMON_FILES
    ${CANN_CMAKE_DIR}/scripts/install/install_common_parser.sh
    ${CANN_CMAKE_DIR}/scripts/install/common_func_v2.inc
    ${CANN_CMAKE_DIR}/scripts/install/common_installer.inc
    ${CANN_CMAKE_DIR}/scripts/install/script_operator.inc
    ${CANN_CMAKE_DIR}/scripts/install/version_cfg.inc
)

set(PACKAGE_FILES
    ${COMMON_FILES}
    ${CANN_CMAKE_DIR}/scripts/install/multi_version.inc
)
set(LATEST_MANGER_FILES
    ${COMMON_FILES}
    ${CMAKE_SOURCE_DIR}/scripts/package/common/sh/common_func.inc
    ${CMAKE_SOURCE_DIR}/scripts/package/common/sh/version_compatiable.inc
    ${CMAKE_SOURCE_DIR}/scripts/package/common/sh/check_version_required.awk
)
set(CONF_FILES 
    ${CANN_CMAKE_DIR}/scripts/package/common/cfg/path.cfg
)
install(FILES ${CONF_FILES}
    DESTINATION asc-devkit/conf COMPONENT asc-devkit
)
install(FILES ${PACKAGE_FILES}
   DESTINATION asc-devkit/script COMPONENT asc-devkit
)
install(FILES ${LATEST_MANGER_FILES}
    DESTINATION latest_manager COMPONENT asc-devkit
)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/scripts/package/latest_manager/scripts/
    DESTINATION latest_manager COMPONENT asc-devkit
)
install(FILES ${CMAKE_BINARY_DIR}/version.asc-devkit.info
    DESTINATION asc-devkit
    RENAME version.info
    COMPONENT asc-devkit
)
install(FILES ${CMAKE_BINARY_DIR}/version.asc-devkit.info
    DESTINATION .
    RENAME version.info
    COMPONENT asc-devkit
)

if (NOT ENABLE_COV AND NOT ENABLE_UT) 
    set_cann_cpack_config(asc-devkit ENABLE_DEVICE ${ENABLE_DEVICE} SHARE_INFO_NAME devkit)
endif()
