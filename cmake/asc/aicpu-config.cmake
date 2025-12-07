# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ----------------------------------------------------------------------------------------------------------
if(NOT DEFINED ENV{ASCEND_HOME_PATH})
    set(POSSIBLE_PATHS "/usr/local/Ascend/cann" "${HOME}/Ascend/cann")
    
    message(FATAL_ERROR "
    ================================================================================
    ERROR: ASCEND_HOME_PATH environment variable is not set!
    
    This variable is required to find CANN package.
    
    Possible solutions:
        Source the environment setup script: source <ascend_install_path>/set_env.sh
    
    Common installation locations:
    ${POSSIBLE_PATHS}
    ================================================================================
    ")
else()
    if(NOT EXISTS "$ENV{ASCEND_HOME_PATH}")
        message(FATAL_ERROR "ERROR: ASCEND_HOME_PATH directory does not exist!")
    endif()
endif()

list(APPEND CMAKE_MODULE_PATH "$ENV{ASCEND_HOME_PATH}/compiler/tikcpp/ascendc_kernel_cmake/aicpu_modules")