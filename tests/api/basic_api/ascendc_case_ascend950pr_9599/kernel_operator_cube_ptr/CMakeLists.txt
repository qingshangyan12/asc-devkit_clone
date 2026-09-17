# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2026 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ----------------------------------------------------------------------------------------------------------

# Compile-only signature lock for the CUBE Basic API pointer overloads.
#
# The check runs through compile_check.sh because the ASC driver resolves AscendC headers from the *installed*
# asc-devkit first; -isystem / -iquote on the source tree either do not take effect or mix two trees.
# compile_check.sh builds a symlink "shadow" CANN root and points ASCEND_HOME_PATH at it, so the driver reads
# this repository's include/impl without touching the shared CANN installation.  See README.md.
#
# Requires:  source ${install_path}/cann/set_env.sh   (ccec + ASCEND_HOME_PATH), CANN >= 9.2.0.
#
# This target intentionally lives in its own directory instead of being globbed into
# ascendc_case_ascend950pr_9599_aic, so that it does not change the shared 950PR AIC test build.

cmake_minimum_required(VERSION 3.16)

project(kernel_operator_cube_ptr_check LANGUAGES NONE)

if(NOT DEFINED ASCENDC_DIR)
    get_filename_component(ASCENDC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../../../.." ABSOLUTE)
endif()

set(ARCH "all" CACHE STRING "NPU architecture to check the pointer overloads for: dav-3510 / dav-2201 / all")

find_program(BASH_BIN bash)
if(NOT BASH_BIN)
    message(FATAL_ERROR "bash not found; the signature check runs through compile_check.sh")
endif()

if(NOT DEFINED ENV{ASCEND_HOME_PATH})
    message(FATAL_ERROR "ASCEND_HOME_PATH is not set; run: source ${install_path}/cann/set_env.sh")
endif()

add_custom_target(cube_ptr_signature_lock ALL
    COMMAND ${BASH_BIN} "${CMAKE_CURRENT_SOURCE_DIR}/compile_check.sh" "${ARCH}" "${ASCENDC_DIR}"
    COMMENT "Checking CUBE pointer overload signatures for ${ARCH}"
    VERBATIM)