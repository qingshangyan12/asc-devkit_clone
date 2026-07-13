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

if (NOT DEFINED PACKAGE_TYPE)
    set(PACKAGE_TYPE "run")
endif()
string(TOLOWER "${PACKAGE_TYPE}" PACKAGE_TYPE)
set(_ASC_DEVKIT_SUPPORTED_PACKAGE_TYPES run rpm deb)
if (NOT PACKAGE_TYPE IN_LIST _ASC_DEVKIT_SUPPORTED_PACKAGE_TYPES)
    message(FATAL_ERROR "Invalid PACKAGE_TYPE=${PACKAGE_TYPE}. Supported values are: run, rpm, deb.")
endif()

if (ARCH STREQUAL "x86_64")
    set(DEB_ARCH "amd64")
elseif (ARCH STREQUAL "aarch64")
    set(DEB_ARCH "arm64")
else()
    set(DEB_ARCH "${ARCH}")
endif()

add_cann_third_party(makeself-fetch)

set(script_prefix ${CMAKE_CURRENT_SOURCE_DIR}/scripts/package/scripts/)
install(DIRECTORY ${script_prefix}/
    DESTINATION share/info/asc-devkit/script
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
    ${CANN_CMAKE_DIR}/scripts/package/merge_binary_info_config.py
)

install(FILES ${SCRIPTS_FILES}
    DESTINATION share/info/asc-devkit/script COMPONENT asc-devkit
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

set(CONF_FILES
    ${CANN_CMAKE_DIR}/scripts/package/cfg/path.cfg
)
install(FILES ${CONF_FILES}
    DESTINATION ${CMAKE_SYSTEM_PROCESSOR}-linux/conf COMPONENT asc-devkit
)
install(FILES ${PACKAGE_FILES}
   DESTINATION share/info/asc-devkit/script COMPONENT asc-devkit
)
install(FILES ${CMAKE_BINARY_DIR}/version.asc-devkit.info
    DESTINATION share/info/asc-devkit
    RENAME version.info
    COMPONENT asc-devkit
)

# ============= CPack =============
set(CPACK_PACKAGE_NAME "${PROJECT_NAME}")
set(CPACK_PACKAGE_VERSION "${CANN_VERSION_asc-devkit_VERSION}")
string(REGEX MATCH "^([0-9]+)\\.([0-9]+)\\.([0-9]+)" _ASC_DEVKIT_VERSION_MATCH "${CPACK_PACKAGE_VERSION}")
if (_ASC_DEVKIT_VERSION_MATCH)
    set(CPACK_PACKAGE_VERSION_MAJOR "${CMAKE_MATCH_1}")
    set(CPACK_PACKAGE_VERSION_MINOR "${CMAKE_MATCH_2}")
    set(CPACK_PACKAGE_VERSION_PATCH "${CMAKE_MATCH_3}")
endif()
set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${CMAKE_SYSTEM_NAME}-${ARCH}")

# 安装到目标位置
install(DIRECTORY ${MAKESELF_PATH}
        DESTINATION ${INSTALL_LIBRARY_DIR}/tikcpp/ascendc_kernel_cmake/fwk_modules/util
        FILE_PERMISSIONS
            OWNER_READ OWNER_EXECUTE
            GROUP_READ GROUP_EXECUTE
        COMPONENT asc-devkit
        PATTERN ".github" EXCLUDE
        PATTERN ".gitignore" EXCLUDE
        PATTERN ".gitmodules" EXCLUDE
        PATTERN "test" EXCLUDE
)

if (NOT ENABLE_COV AND NOT ENABLE_UT)
    if (PACKAGE_TYPE STREQUAL "run")
        set_cann_cpack_config(asc-devkit OUTPUT "${CMAKE_SOURCE_DIR}/build_out" NO_CLEAN)
    else()
        set(CPACK_COMPONENTS_ALL asc-devkit)
        set(CPACK_PACKAGE_DIRECTORY "${CMAKE_SOURCE_DIR}/build_out")
        set(CPACK_PACKAGING_INSTALL_PREFIX "/usr/local/Ascend/cann")
        set(CPACK_PACKAGE_CONTACT "Huawei Technologies Co., Ltd.")
        set(CPACK_PACKAGE_VENDOR "Huawei Technologies Co., Ltd.")
        set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Ascend C development kit")

        if (PACKAGE_TYPE STREQUAL "rpm")
            set(CPACK_GENERATOR RPM)
            set(CPACK_RPM_COMPONENT_INSTALL ON)
            set(CPACK_RPM_PACKAGE_ARCHITECTURE "${ARCH}")
            set(CPACK_RPM_PACKAGE_RELEASE 1)
            set(CPACK_RPM_FILE_NAME RPM-DEFAULT)
        elseif (PACKAGE_TYPE STREQUAL "deb")
            set(CPACK_GENERATOR DEB)
            set(CPACK_DEB_COMPONENT_INSTALL ON)
            set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "${DEB_ARCH}")
            set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Huawei Technologies Co., Ltd.")
            set(CPACK_DEBIAN_FILE_NAME DEB-DEFAULT)
        endif()
        include(CPack)
    endif()
endif()
