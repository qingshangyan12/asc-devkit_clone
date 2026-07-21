# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ----------------------------------------------------------------------------------------------------------

get_filename_component(_CUR_DIRNAME "${CMAKE_CURRENT_LIST_DIR}" NAME)
if(_CUR_DIRNAME STREQUAL "asc")
    set(ASC_BINARY_SCENE FALSE)
else()
    set(ASC_BINARY_SCENE TRUE)
endif()
unset(_CUR_DIRNAME)

if(ASC_BINARY_SCENE AND NOT DEFINED ENV{ASCEND_HOME_PATH})
    message(FATAL_ERROR "ERROR: ASCEND_HOME_PATH environment variable is not set! source set_env.sh in cann install directory first.")
else()
    if(NOT EXISTS "$ENV{ASCEND_HOME_PATH}")
        message(FATAL_ERROR "ERROR: ASCEND_HOME_PATH directory does not exist!")
    endif()
endif()

if(_ASC_MODULE_LOADED)
    unset(ASC_BINARY_SCENE)
    return()
endif()
set(_ASC_MODULE_LOADED FALSE)
if(ASC_BINARY_SCENE)
    include($ENV{ASCEND_HOME_PATH}/compiler/tikcpp/ascendc_kernel_cmake/fwk_modules/config.cmake)
    include($ENV{ASCEND_HOME_PATH}/compiler/tikcpp/ascendc_kernel_cmake/fwk_modules/func.cmake)
    include($ENV{ASCEND_HOME_PATH}/compiler/tikcpp/ascendc_kernel_cmake/fwk_modules/intf.cmake)
else()
    include(fwk_modules/config.cmake)
    include(fwk_modules/func.cmake)
    include(fwk_modules/intf.cmake)
endif()

# plugin support ASC language
if(ASC_BINARY_SCENE)
    list(APPEND CMAKE_MODULE_PATH "$ENV{ASCEND_HOME_PATH}/compiler/tikcpp/ascendc_kernel_cmake/asc_modules")
    include($ENV{ASCEND_HOME_PATH}/compiler/tikcpp/ascendc_kernel_cmake/asc_modules/FindASC.cmake)
else()
    list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/asc_modules")
    include(asc_modules/FindASC.cmake)
endif()
set(_ASC_MODULE_LOADED TRUE)
unset(ASC_BINARY_SCENE)
