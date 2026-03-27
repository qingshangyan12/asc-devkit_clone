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

find_package(ASC REQUIRED)

project(kernel_samples LANGUAGES ASC CXX)

add_executable(demo
    antiquantize_example.asc
)

# ======================================================================================
# NPU 编译选项配置
#
# 说明：
#   - 需根据实际部署的 NPU 硬件架构选择对应的 `npu-arch` 参数。
#   - dav-2201: 生成 golden 数据使用 float16 格式
#   - dav-3510: 生成 golden 数据使用 float32 格式
#   - 通过修改 NPU_ARCH 变量来选择架构，然后运行: cmake --build . --target gen_golden_data
# ======================================================================================
set(NPU_ARCH "dav-3510" CACHE STRING "NPU architecture (dav-2201 or dav-3510)")

if(NPU_ARCH STREQUAL "dav-2201")
    target_compile_options(demo PRIVATE
        $<$<COMPILE_LANGUAGE:ASC>:--npu-arch=dav-2201>
    )
elseif(NPU_ARCH STREQUAL "dav-3510")
    target_compile_options(demo PRIVATE
        $<$<COMPILE_LANGUAGE:ASC>:--npu-arch=dav-3510>
    )
else()
    message(FATAL_ERROR "Unsupported NPU_ARCH: ${NPU_ARCH}. Supported values: dav-2201, dav-3510")
endif()

message(STATUS "NPU Architecture: ${NPU_ARCH}")

add_custom_target(gen_golden_data
    COMMAND ${CMAKE_COMMAND} -E env NPU_ARCH=${NPU_ARCH} python3 ${CMAKE_CURRENT_SOURCE_DIR}/scripts/gen_data.py ${NPU_ARCH}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Generating golden data for ${NPU_ARCH}"
    VERBATIM
)