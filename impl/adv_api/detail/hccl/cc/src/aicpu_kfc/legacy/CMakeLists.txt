# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ----------------------------------------------------------------------------------------------------------
IF (DEFINED ENV{HCCL_LOCAL_BUILD})
    # 定义后续需要使用的变量
    cmake_minimum_required(VERSION 3.20.5)
    project(hccl)
    set(TOP_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../../..)
ENDIF ()

set(HCCL_V2_CODE_ROOT ${ORION_REAL_PATH})

# 自动找到项目内所有头文件
FILE(GLOB_RECURSE sources_list LIST_DIRECTORIES TRUE ${HCCL_V2_CODE_ROOT}/*)

foreach (dir ${sources_list})
    IF ((IS_DIRECTORY ${dir} ) AND (${dir} MATCHES "aicpu_kfc/legacy") AND (NOT ${dir} MATCHES "aicpu_kfc/legacy/mdpi"))
    endif ()
endforeach ()

add_subdirectory(common)
add_subdirectory(framework)
add_subdirectory(unified_platform)
add_subdirectory(interface)
