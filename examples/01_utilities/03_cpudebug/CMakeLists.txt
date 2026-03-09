# ----------------------------------------------------------------------------------------------------------
# This program is free software, you can redistribute it and/or modify it.
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This file is a part of the CANN Open Software.
# Licensed under CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ----------------------------------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.16)
find_package(ASC REQUIRED)
project(cpudebug)

add_executable(add ${CMAKE_CURRENT_SOURCE_DIR}/add.asc)
target_compile_options(add PRIVATE
    $<$<COMPILE_LANGUAGE:ASC>:--npu-arch=dav-${NPU_ARCH}>
    $<$<COMPILE_LANGUAGE:ASC>:--run-mode=cpu>
    -D __CCE_KT_TEST__=1
    -D ASCENDC_DEBUG=1
    -D __NPU_ARCH__=${NPU_ARCH}
    -g -O2 -std=c++17 -D_GLIBCXX_USE_CXX11_ABI=0 -Wall -Werror
)

target_include_directories(add PRIVATE
    ${ASCEND_INSTALL_PATH}/tools/tikicpulib/lib/include
)

target_link_options(add PRIVATE
    $<$<COMPILE_LANGUAGE:ASC>:--run-mode=cpu>
    -L${ASCEND_INSTALL_PATH}/tools/tikicpulib/lib
    -L${ASCEND_INSTALL_PATH}/tools/tikicpulib/lib/${SOC_VERSION}
    -L${ASCEND_INSTALL_PATH}/tools/simulator/${SOC_VERSION}/lib
    -L${ASCEND_INSTALL_PATH}/lib64
    -Wl,-rpath,${ASCEND_INSTALL_PATH}/tools/tikicpulib/lib
    -Wl,-rpath,${ASCEND_INSTALL_PATH}/tools/tikicpulib/lib/${SOC_VERSION}
    -Wl,-rpath,${ASCEND_INSTALL_PATH}/tools/simulator/${SOC_VERSION}/lib
    -Wl,-rpath,${ASCEND_INSTALL_PATH}/lib64
)

target_link_libraries(add PRIVATE
    -Wl,-Bdynamic,--no-as-needed
    m
    ascendc_acl_stub
    cpudebug_cceprint
    cpudebug_npuchk
    cpudebug_stubreg
    cpudebug
    pem_davinci
    c_sec
    stdc++
    -Wl,--as-needed    
)

install(TARGETS add
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)