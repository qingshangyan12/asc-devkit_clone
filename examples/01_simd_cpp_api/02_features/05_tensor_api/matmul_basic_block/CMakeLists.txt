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
    matmul_basic_block.asc
)

set_target_properties(demo PROPERTIES 
    LINK_FLAGS "-Wl,--disable-new-dtags"  ## 由于仿真库会依赖当前目录的其他非链接的so，需要开启RPATH传递依赖目录
)

include_directories(
    /home/hyolee/asc-devkit_1681
    $ENV{ASCEND_HOME_PATH}/x86_64-linux/asc
)

# ${INSTALL_DIR}请替换为CANN软件安装后文件存储路径。以root用户安装为例，安装后文件默认存储路径为：/usr/local/Ascend/cann。
target_link_directories(demo PRIVATE
    $ENV{ASCEND_HOME_PATH}/tools/simulator/dav_3510/lib  ##仿真库所在的目录，其中dav_3510目录名称与芯片版本相关 
)

target_link_libraries(demo PRIVATE
    runtime_camodel 
    npu_drv_camodel  ## 仿真编译需要链接的so
    m
)



# target_link_libraries(demo PRIVATE
#     tiling_api
#     register
#     platform
#     m
#     dl
# )

target_compile_options(demo PRIVATE
    $<$<COMPILE_LANGUAGE:ASC>:--npu-arch=dav-3510>
)



# find_package(ASC REQUIRED)
# project(kernel_samples LANGUAGES ASC CXX)

# add_executable(demo
#     add_custom.asc
# )

# set_target_properties(demo PROPERTIES 
#     LINK_FLAGS "-Wl,--disable-new-dtags"  ## 由于仿真库会依赖当前目录的其他非链接的so，需要开启RPATH传递依赖目录
# )

# target_link_libraries(demo PRIVATE
#     runtime_camodel npu_drv_camodel  ## 仿真编译需要链接的so
#     m
# )

# # ${INSTALL_DIR}请替换为CANN软件安装后文件存储路径。以root用户安装为例，安装后文件默认存储路径为：/usr/local/Ascend/cann。
# target_link_directories(demo PRIVATE
#     ${INSTALL_DIR}/tools/simulator/dav_3510/lib  ##仿真库所在的目录，其中dav_3510目录名称与芯片版本相关 
# )

# target_compile_options(demo PRIVATE
#     $<$<COMPILE_LANGUAGE:ASC>: --npu-arch=dav-3510>
# )