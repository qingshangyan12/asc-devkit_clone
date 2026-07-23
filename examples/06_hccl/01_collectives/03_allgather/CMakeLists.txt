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

# 检查 ASCEND_HOME_PATH 环境变量
if(NOT DEFINED ENV{ASCEND_HOME_PATH})
    message(FATAL_ERROR "ASCEND_HOME_PATH is not set, please ensure CANN is properly installed and \
             source environment variables by running `source /path/to/Ascend/cann/set_env.sh`")
endif()

set(PROJECT_NAME "allgather" CACHE STRING "Project name")
project(${PROJECT_NAME} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 编译选项
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -fstack-protector-strong -fPIE -pie -O2 -s \
    -Wl,-z,relro -Wl,-z,now -Wl,-z,noexecstack -Wl,--copy-dt-needed-entries")

# 头文件和库文件路径
set(ASCEND_INC_DIR $ENV{ASCEND_HOME_PATH}/include)
set(ASCEND_LIB_DIR $ENV{ASCEND_HOME_PATH}/lib64)
# devkit 仓 mc2_client 库的安装路径
set(DEVKIT_LIB_DIR $ENV{ASCEND_HOME_PATH}/${CMAKE_SYSTEM_PROCESSOR}-linux/lib64)

# 编译目标
add_executable(${PROJECT_NAME}
    main.cc
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${ASCEND_INC_DIR}
)

target_link_directories(${PROJECT_NAME} PRIVATE
    ${ASCEND_LIB_DIR}
    ${DEVKIT_LIB_DIR}
)

target_link_libraries(${PROJECT_NAME}
    mc2_client
    hccl
    ascendcl
    pthread
)

# 安装目标
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)