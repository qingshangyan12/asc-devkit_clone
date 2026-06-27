# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ----------------------------------------------------------------------------------------------------------
project(ascendc_tests)
cmake_minimum_required(VERSION 3.16.0)

add_compile_options(-Werror)

set(CMAKE_CXX_STANDARD 17)

if (BUILD_OPEN_PROJECT)
    include(cmake/config.cmake)
    include(cmake/intf.cmake)
    include(cmake/func.cmake)
    # hcomm 已迁出至 asc-comm，UT 编译期需 asc-comm 暴露 hcomm 头供伞头(kernel_api.h)解析。
    # 仅设置 ASC_COMM_HEADER_INCLUDE_DIR，跳过 host 安装(由打包流程负责)。
    set(ASC_COMM_SKIP_HOST_BUILD ON CACHE BOOL "skip host asc-comm install in tests" FORCE)
    include(${CMAKE_CURRENT_SOURCE_DIR}/../cmake/third_party/asc-comm.cmake)
endif()

add_subdirectory(api)
add_subdirectory(tools)
add_subdirectory(python)
