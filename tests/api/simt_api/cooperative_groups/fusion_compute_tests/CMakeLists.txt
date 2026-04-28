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

project(cooperative_groups_fusion LANGUAGES ASC CXX)

add_executable(cg_thread_block_fusion
    thread_block_fusion.asc
)

add_executable(cg_thread_block_tile_fusion
    thread_block_tile_fusion.asc
)

add_executable(cg_coalesced_group_fusion
    coalesced_group_fusion.asc
)

add_executable(cg_shuffle_functions_fusion
    shuffle_functions_fusion.asc
)

add_executable(cg_vote_functions_fusion
    vote_functions_fusion.asc
)

target_compile_options(cg_thread_block_fusion PRIVATE
    $<$<COMPILE_LANGUAGE:ASC>:--npu-arch=dav-3510>
    $<BUILD_INTERFACE:$<$<STREQUAL:${RUN_MODE},sim>:-g -O0>>
)

target_include_directories(cg_thread_block_fusion PRIVATE
    $<$<COMPILE_LANGUAGE:ASC>:${ASCENDC_DIR}/include ${ASCENDC_DIR}>
)

target_compile_options(cg_thread_block_tile_fusion PRIVATE
    $<$<COMPILE_LANGUAGE:ASC>:--npu-arch=dav-3510>
    $<BUILD_INTERFACE:$<$<STREQUAL:${RUN_MODE},sim>:-g -O0>>
)

target_include_directories(cg_thread_block_tile_fusion PRIVATE
    $<$<COMPILE_LANGUAGE:ASC>:${ASCENDC_DIR}/include ${ASCENDC_DIR}>
)

target_compile_options(cg_coalesced_group_fusion PRIVATE
    $<$<COMPILE_LANGUAGE:ASC>:--npu-arch=dav-3510>
    $<BUILD_INTERFACE:$<$<STREQUAL:${RUN_MODE},sim>:-g -O0>>
)

target_include_directories(cg_coalesced_group_fusion PRIVATE
    $<$<COMPILE_LANGUAGE:ASC>:${ASCENDC_DIR}/include ${ASCENDC_DIR}>
)

target_compile_options(cg_shuffle_functions_fusion PRIVATE
    $<$<COMPILE_LANGUAGE:ASC>:--npu-arch=dav-3510>
    $<BUILD_INTERFACE:$<$<STREQUAL:${RUN_MODE},sim>:-g -O0>>
)

target_include_directories(cg_shuffle_functions_fusion PRIVATE
    $<$<COMPILE_LANGUAGE:ASC>:${ASCENDC_DIR}/include ${ASCENDC_DIR}>
)

target_compile_options(cg_vote_functions_fusion PRIVATE
    $<$<COMPILE_LANGUAGE:ASC>:--npu-arch=dav-3510>
    $<BUILD_INTERFACE:$<$<STREQUAL:${RUN_MODE},sim>:-g -O0>>
)

target_include_directories(cg_vote_functions_fusion PRIVATE
    $<$<COMPILE_LANGUAGE:ASC>:${ASCENDC_DIR}/include ${ASCENDC_DIR}>
)

target_link_directories(cg_thread_block_fusion PRIVATE
    $<BUILD_INTERFACE:$<$<STREQUAL:${RUN_MODE},sim>:$ENV{ASCEND_HOME_PATH}/tools/simulator/${SOC_VERSION}/lib>>
)

target_link_directories(cg_thread_block_tile_fusion PRIVATE
    $<BUILD_INTERFACE:$<$<STREQUAL:${RUN_MODE},sim>:$ENV{ASCEND_HOME_PATH}/tools/simulator/${SOC_VERSION}/lib>>
)

target_link_directories(cg_coalesced_group_fusion PRIVATE
    $<BUILD_INTERFACE:$<$<STREQUAL:${RUN_MODE},sim>:$ENV{ASCEND_HOME_PATH}/tools/simulator/${SOC_VERSION}/lib>>
)

target_link_directories(cg_shuffle_functions_fusion PRIVATE
    $<BUILD_INTERFACE:$<$<STREQUAL:${RUN_MODE},sim>:$ENV{ASCEND_HOME_PATH}/tools/simulator/${SOC_VERSION}/lib>>
)

target_link_directories(cg_vote_functions_fusion PRIVATE
    $<BUILD_INTERFACE:$<$<STREQUAL:${RUN_MODE},sim>:$ENV{ASCEND_HOME_PATH}/tools/simulator/${SOC_VERSION}/lib>>
)

target_link_libraries(cg_thread_block_fusion PRIVATE
    m
    $<BUILD_INTERFACE:$<$<STREQUAL:${RUN_MODE},sim>:runtime_camodel>>
    $<BUILD_INTERFACE:$<$<STREQUAL:${RUN_MODE},sim>:npu_drv>>
)

target_link_libraries(cg_thread_block_tile_fusion PRIVATE
    m
    $<BUILD_INTERFACE:$<$<STREQUAL:${RUN_MODE},sim>:runtime_camodel>>
    $<BUILD_INTERFACE:$<$<STREQUAL:${RUN_MODE},sim>:npu_drv>>
)

target_link_libraries(cg_coalesced_group_fusion PRIVATE
    m
    $<BUILD_INTERFACE:$<$<STREQUAL:${RUN_MODE},sim>:runtime_camodel>>
    $<BUILD_INTERFACE:$<$<STREQUAL:${RUN_MODE},sim>:npu_drv>>
)

target_link_libraries(cg_shuffle_functions_fusion PRIVATE
    m
    $<BUILD_INTERFACE:$<$<STREQUAL:${RUN_MODE},sim>:runtime_camodel>>
    $<BUILD_INTERFACE:$<$<STREQUAL:${RUN_MODE},sim>:npu_drv>>
)

target_link_libraries(cg_vote_functions_fusion PRIVATE
    m
    $<BUILD_INTERFACE:$<$<STREQUAL:${RUN_MODE},sim>:runtime_camodel>>
    $<BUILD_INTERFACE:$<$<STREQUAL:${RUN_MODE},sim>:npu_drv>>
)