# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2026 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ----------------------------------------------------------------------------------------------------------

# 本样例只构建 host 侧 Exception Dump 回调库（无设备侧 kernel），
# 产物输出到 runtime/exception_dump_public.so，供 Python ctypes 加载。
cmake_minimum_required(VERSION 3.16)

project(utils_exception_dump LANGUAGES CXX)

set(ASCEND_HOME_PATH "$ENV{ASCEND_HOME_PATH}" CACHE PATH "CANN installation root (source the CANN environment first)")
if(NOT ASCEND_HOME_PATH)
    message(FATAL_ERROR "ASCEND_HOME_PATH is not set; source <CANN>/bin/setenv.bash or ascend-toolkit/set_env.sh first")
endif()

add_library(exception_dump_public SHARED runtime/exception_dump_public.cpp)

target_compile_features(exception_dump_public PRIVATE cxx_std_11)
target_include_directories(exception_dump_public PRIVATE
    ${ASCEND_HOME_PATH}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/runtime
)
target_link_directories(exception_dump_public PRIVATE ${ASCEND_HOME_PATH}/lib64)
target_link_libraries(exception_dump_public PRIVATE acl_rt ascendcl)

set_target_properties(exception_dump_public PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/runtime
    PREFIX ""
    OUTPUT_NAME exception_dump_public
)
