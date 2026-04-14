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
project(custom_op_prj)

option(CUSTOM_OP_ENABLE_PARALLEL_RUN_PACKAGES
    "Build add_custom and leaky_relu_custom as two independent RUN packages."
    OFF
)
set(CUSTOM_OP_SELECTED_PROFILE "all" CACHE STRING "Build profile: all, add_custom, leaky_relu_custom.")

set(ASCEND_COMPUTE_UNIT ascend910b ascend910_93 ascend950)

if(CUSTOM_OP_ENABLE_PARALLEL_RUN_PACKAGES AND NOT CUSTOM_OP_PARALLEL_LEAF_BUILD)
    include(ExternalProject)
    set(INSTALL_TARGET package)

    foreach(profile IN ITEMS add_custom leaky_relu_custom)
        ExternalProject_Add(${profile}
            SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}
            CMAKE_CACHE_ARGS
                -DASCEND_COMPUTE_UNIT:STRING=${ASCEND_COMPUTE_UNIT}
            CMAKE_ARGS
                -DCUSTOM_OP_PARALLEL_LEAF_BUILD:BOOL=ON
                -DCUSTOM_OP_SELECTED_PROFILE:STRING=${profile}
                -DCUSTOM_OP_PACKAGE_NAME:STRING=${profile}
                -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_BINARY_DIR}/${profile}
            BUILD_COMMAND
                ${CMAKE_COMMAND} --build <BINARY_DIR> --target binary
            COMMAND
                ${CMAKE_COMMAND} --build <BINARY_DIR> --target ${INSTALL_TARGET}
            INSTALL_COMMAND ""
        )
    endforeach()
    return()
endif()

find_package(ASC REQUIRED)

if(CUSTOM_OP_PARALLEL_LEAF_BUILD)
    set(package_name ${CUSTOM_OP_PACKAGE_NAME})
    set(package_install_path ${CMAKE_INSTALL_PREFIX})
else()
    set(package_name customize)
    set(package_install_path ${CMAKE_BINARY_DIR})
endif()

npu_op_package(${package_name}
    TYPE RUN
    CONFIG
        INSTALL_PATH ${package_install_path}
)

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/framework")
    add_subdirectory(framework)
endif()

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/op_host")
    add_subdirectory(op_host)
endif()

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/op_kernel")
    add_subdirectory(op_kernel)
endif()
