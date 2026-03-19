# -----------------------------------------------------------------------------------------------------------
# Copyright (c) 2026 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# -----------------------------------------------------------------------------------------------------------

set_package(asc-devkit VERSION "9.0.0")

set_build_dependencies(runtime "CUR_MAJOR_MINOR_VER")
set_build_dependencies(metadef "CUR_MAJOR_MINOR_VER")

<<<<<<< HEAD:examples/02_features/04_micro_api/move_successive_unalign/CMakeLists.txt
find_package(ASC REQUIRED)

project(kernel_samples LANGUAGES ASC CXX)

add_executable(demo
    move_successive_unalign.asc
)

target_link_libraries(demo PRIVATE
    tiling_api
    register
    platform
    m
)

target_compile_options(demo PRIVATE
    # 该NPU架构版本号暂未正式发布，后续可能会有变更，请关注正式版本发布信息
    $<$<COMPILE_LANGUAGE:ASC>:--npu-arch=dav-3510>
)
=======
set_run_dependencies(runtime "CUR_MAJOR_MINOR_VER")
set_run_dependencies(ge-executor "CUR_MAJOR_MINOR_VER")
set_run_dependencies(metadef "CUR_MAJOR_MINOR_VER")
set_run_dependencies(opbase "CUR_MAJOR_MINOR_VER")
set_run_dependencies(bisheng-compiler "CUR_MAJOR_MINOR_VER")
set_run_dependencies(tbe-tik "CUR_MAJOR_MINOR_VER")
>>>>>>> upstream/master:version.cmake
