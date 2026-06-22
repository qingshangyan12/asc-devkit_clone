# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ----------------------------------------------------------------------------------------------------------
set(ASCENDC_ADV_API_IMPL_DIR ${ASCENDC_IMPL_DIR}/adv_api)
set(ASCENDC_ADV_API_CMAKE_DIR ${ASCENDC_ADV_API_IMPL_DIR}/cmake)

#generate kernel_tiling/kernel_tiling.h
set(GEN_KERNEL_TILING_DATA_SCRIPT
    ${ASCENDC_ADV_API_CMAKE_DIR}/scripts/gen_kernel_tiling_data_def.py)
set(GENERATE_INSTALL_SCRIPT ${ASCENDC_ADV_API_CMAKE_DIR}/scripts/generate_install_script.sh)
set(TILING_DATA_DEF_DIR ${ASCENDC_INCLUDE_DIR}/adv_api)
set(KERNEL_TILING_DIR ${PROJECT_BINARY_DIR})
set(KERNEL_TILING_HEAD ${KERNEL_TILING_DIR}/kernel_tiling/kernel_tiling.h)

# asc-comm 迁出后，hccl 的 *tilingdata.h 不再位于 include/adv_api 下，但生成的 kernel_tiling.h
# 仍需包含其中的 Mc2InitTiling/Mc2CcTiling 等结构体(对外伞头 tiling_api.h -> hccl_tiling.h 依赖
# 这些全局类型)。asc-comm 的头由 cmake/third_party/asc-comm.cmake 通过软链暴露在
# ${CMAKE_BINARY_DIR}/asc_comm_headers/hccl，这里把该目录作为额外扫描源传给生成脚本。
# 软链在 configure 阶段(asc-comm.cmake)建立，早于本生成命令的 build 阶段执行。
set(ASC_COMM_TILING_SCAN_DIR ${CMAKE_BINARY_DIR}/asc_comm_headers/hccl)

if(NOT BUILD_OPEN_PROJECT)
    add_custom_command(
        OUTPUT ${KERNEL_TILING_HEAD}
        COMMAND ${HI_PYTHON} ${GEN_KERNEL_TILING_DATA_SCRIPT} ${TILING_DATA_DEF_DIR} ${KERNEL_TILING_HEAD}
        COMMAND mkdir -p ${TOP_DIR}/atc/opcompiler/ascendc_compiler/api/kernel_tiling
        COMMAND cp ${KERNEL_TILING_HEAD} ${TOP_DIR}/atc/opcompiler/ascendc_compiler/api/kernel_tiling
        DEPENDS ${GEN_KERNEL_TILING_DATA_SCRIPT}
    )
else()
    add_custom_command(
        OUTPUT ${KERNEL_TILING_HEAD}
        COMMAND ${HI_PYTHON} ${GEN_KERNEL_TILING_DATA_SCRIPT} ${TILING_DATA_DEF_DIR} ${KERNEL_TILING_HEAD} ${ASC_COMM_TILING_SCAN_DIR}
        DEPENDS ${GEN_KERNEL_TILING_DATA_SCRIPT}
    )
endif()

add_custom_target(gen_kernel_tiling ALL DEPENDS ${KERNEL_TILING_HEAD})

add_library(kernel_tiling_headers INTERFACE)
add_dependencies(kernel_tiling_headers gen_kernel_tiling)

target_include_directories(
  kernel_tiling_headers
  INTERFACE $<INSTALL_INTERFACE:include>
            $<INSTALL_INTERFACE:include/tikcpp>
            $<INSTALL_INTERFACE:include/tikcpp/tikcfw>
            $<INSTALL_INTERFACE:include/tikcpp/tikcfw/kernel_tiling>
            $<BUILD_INTERFACE:${KERNEL_TILING_DIR}>
            $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}>)

add_library(asc_kernel_headers INTERFACE)
target_include_directories(
  asc_kernel_headers
  INTERFACE $<BUILD_INTERFACE:${ASCENDC_DIR}>
            $<BUILD_INTERFACE:${ASCENDC_DIR}/include>
            $<BUILD_INTERFACE:${ASCENDC_DIR}/include/basic_api>
            $<BUILD_INTERFACE:${ASCENDC_DIR}/include/simt_api>
            $<BUILD_INTERFACE:${ASCENDC_DIR}/impl>
            $<BUILD_INTERFACE:${ASCENDC_DIR}/impl/basic_api>
            $<BUILD_INTERFACE:${ASCENDC_DIR}/impl/simt_api>
)

add_library(asc_host_headers INTERFACE)
target_include_directories(
  asc_host_headers
  INTERFACE $<BUILD_INTERFACE:${ASCENDC_DIR}/include/utils>
)
