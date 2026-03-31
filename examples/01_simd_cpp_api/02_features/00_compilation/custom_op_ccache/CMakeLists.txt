# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ----------------------------------------------------------------------------------------------------------


cmake_minimum_required(VERSION 3.16.0)
project(custom_op_ccache_prj)

# ASCConfig.cmake is installed by the CANN toolkit and made discoverable via
# CMAKE_PREFIX_PATH only after sourcing set_env.sh. Give a clear error instead
# of the generic "could not find ASCConfig.cmake" message.
if(NOT DEFINED ENV{ASCEND_TOOLKIT_HOME})
    message(FATAL_ERROR
        "[ASC] CANN environment is not set up.\n"
        "Please run the following command before invoking cmake:\n"
        "  source /usr/local/Ascend/cann/set_env.sh\n"
        "Then re-run cmake.")
endif()
find_package(ASC REQUIRED)

set(ASCEND_COMPUTE_UNIT ascend910b)
set(package_name customize)

set(CMAKE_CXX_COMPILER_LAUNCHER "ccache")

npu_op_package(${package_name}
    TYPE RUN
    CONFIG
        ENABLE_SOURCE_PACKAGE True
        ENABLE_BINARY_PACKAGE True
        INSTALL_PATH ${CMAKE_BINARY_DIR}
)

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/op_host")
    add_subdirectory(op_host)
endif()

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/op_kernel")
    add_subdirectory(op_kernel)
endif()
