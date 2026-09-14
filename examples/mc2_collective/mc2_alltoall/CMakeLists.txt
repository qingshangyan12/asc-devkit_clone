# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2026 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ----------------------------------------------------------------------------------------------------------


cmake_minimum_required(VERSION 3.16)

# 本样例位于asc-devkit仓内，默认使用仓内头文件和仓内构建的libmc2_client.so，
# 可通过-DDEVKIT_PATH指定其它asc-devkit仓路径，或-DMC2_CLIENT_LIB指定已编译的库。
set(DEVKIT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/.." CACHE PATH "asc-devkit repository root")
set(MC2_CLIENT_LIB "" CACHE FILEPATH "prebuilt libmc2_client.so exporting Mc2* host APIs")

set(CMAKE_ASC_ARCHITECTURES "dav-3510" CACHE STRING "NPU architecture: dav-3510")

find_package(ASC REQUIRED)

project(kernel_samples LANGUAGES ASC CXX)

add_executable(demo
    all_to_all.asc
    main.asc
)

# 仓内include目录提供adv_api/hccl/hccl_mc2.h等头文件，优先于CANN自带头文件
if(EXISTS "${DEVKIT_PATH}/include/adv_api/hccl/hccl_mc2.h")
    list(APPEND DEMO_INCLUDE_DIRS "${DEVKIT_PATH}/include")
endif()
list(APPEND DEMO_INCLUDE_DIRS
    $ENV{ASCEND_HOME_PATH}/include
    $ENV{ASCEND_HOME_PATH}/include/hcomm
)
target_include_directories(demo PRIVATE ${DEMO_INCLUDE_DIRS})

target_compile_options(demo PRIVATE
    $<$<COMPILE_LANGUAGE:ASC>:--npu-arch=${CMAKE_ASC_ARCHITECTURES}>
)

# MC2客户端库解析优先级：
# 1. 显式指定的MC2_CLIENT_LIB；
# 2. asc-devkit仓构建产物${DEVKIT_PATH}/build/impl/adv_api/detail/hccl/cc/src/libmc2_client.so
#    （需先在仓内执行: mkdir -p build && cd build && cmake .. && make mc2_client）；
# 3. CANN自带的libmc2_client.so（适用于CANN包已内置Mc2*接口的场景）。
if(MC2_CLIENT_LIB STREQUAL "" AND EXISTS "${DEVKIT_PATH}/build/impl/adv_api/detail/hccl/cc/src/libmc2_client.so")
    set(MC2_CLIENT_LIB "${DEVKIT_PATH}/build/impl/adv_api/detail/hccl/cc/src/libmc2_client.so")
endif()

if(EXISTS "${DEVKIT_PATH}/include/adv_api/hccl/hccl_mc2.h" AND MC2_CLIENT_LIB STREQUAL "")
    message(FATAL_ERROR
        "DEVKIT_PATH=${DEVKIT_PATH} provides adv_api/hccl/hccl_mc2.h but no prebuilt libmc2_client.so was found. "
        "Build it first: cd ${DEVKIT_PATH} && mkdir -p build && cd build && cmake .. && make mc2_client, "
        "or pass -DMC2_CLIENT_LIB=<path-to-libmc2_client.so> explicitly.")
endif()

target_link_directories(demo PRIVATE
    $ENV{ASCEND_HOME_PATH}/lib64
)

if(NOT MC2_CLIENT_LIB STREQUAL "")
    get_filename_component(MC2_CLIENT_LIB_DIR "${MC2_CLIENT_LIB}" DIRECTORY)
    # 仓内构建的libmc2_client.so与CANN lib64中同名旧库冲突：
    # 使用DT_RPATH(优先于LD_LIBRARY_PATH)并将样例目录置前，保证运行时优先加载正确版本。
    set_target_properties(demo PROPERTIES BUILD_RPATH "\$ORIGIN")
    target_link_options(demo PRIVATE
        "-Wl,--disable-new-dtags"
        "-Wl,-rpath,${MC2_CLIENT_LIB_DIR}"
    )
    # 将库拷贝到可执行文件目录，使demo脱离asc-devkit构建目录后仍可运行
    add_custom_command(TARGET demo POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${MC2_CLIENT_LIB}" "$<TARGET_FILE_DIR:demo>/libmc2_client.so"
    )
    target_link_libraries(demo PRIVATE
        "${MC2_CLIENT_LIB}"
        hccl
        hcomm
        pthread
        dl
    )
else()
    target_link_libraries(demo PRIVATE
        mc2_client
        hccl
        hcomm
        pthread
        dl
    )
endif()
