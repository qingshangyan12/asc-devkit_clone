# Copyright (c) 2026 Huawei Technologies Co., Ltd.
# CANN Open Software License Agreement Version 2.0

cmake_minimum_required(VERSION 3.16)

find_package(ASC REQUIRED)

project(kernel_samples LANGUAGES ASC CXX)

add_executable(c_api_abs_example
    c_api_abs.asc
)

# NPU 编译选项配置
# - 根据实际部署的 NPU 硬件架构选择对应的 npu-arch 参数
target_compile_options(c_api_abs_example PRIVATE
    $<$<COMPILE_LANGUAGE:ASC>:--npu-arch=dav-3510>
)
