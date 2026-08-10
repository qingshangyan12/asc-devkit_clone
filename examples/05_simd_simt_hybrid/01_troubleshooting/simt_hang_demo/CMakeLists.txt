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

set(CMAKE_ASC_ARCHITECTURES "dav-3510" CACHE STRING "NPU architecture: dav-3510")
option(BISECT_PRINTF "Enable printf-based hang bisection instrumentation" OFF)
# 让进程真正保持卡死（无超时同步），以便在msdebug中按CTRL+C手动中断并回显中断位置。
option(HANG_FOREVER "Block forever on stream sync so the hang can be interrupted in msdebug" OFF)
# 以-O0 -g构建，使算子二进制带调试信息，msdebug才能回显文件名和行号。
option(MSDEBUG_BUILD "Build with debug info (-g -O0) for msdebug" OFF)

find_package(ASC REQUIRED)

project(sync_hang_demo LANGUAGES ASC CXX)

add_executable(demo
    sync_hang_demo.asc
)

message(STATUS "[INFO] BISECT_PRINTF is ${BISECT_PRINTF}")
message(STATUS "[INFO] HANG_FOREVER is ${HANG_FOREVER}")
message(STATUS "[INFO] MSDEBUG_BUILD is ${MSDEBUG_BUILD}")

target_compile_definitions(demo PRIVATE
    BISECT_PRINTF=$<BOOL:${BISECT_PRINTF}>
    HANG_FOREVER=$<BOOL:${HANG_FOREVER}>
)

target_compile_options(demo PRIVATE
    $<$<COMPILE_LANGUAGE:ASC>:--npu-arch=${CMAKE_ASC_ARCHITECTURES}>
)

if(MSDEBUG_BUILD)
    target_compile_options(demo PRIVATE
        $<$<COMPILE_LANGUAGE:ASC>:-g>
        $<$<COMPILE_LANGUAGE:ASC>:-O0>
    )
endif()
