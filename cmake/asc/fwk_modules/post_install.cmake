# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ----------------------------------------------------------------------------------------------------------

if(DEFINED ${CMAKE_CURRENT_BINARY_DIR}/binary)
    set(ERROR_LOG_FILE "${CMAKE_CURRENT_BINARY_DIR}/binary/compile_error.log")
    if(EXISTS "${ERROR_LOG_FILE}")
        file(READ "${ERROR_LOG_FILE}" ERROR_CONTENT)
        message(STATUS "")
        message(STATUS "========================================")
        message(STATUS "  Compilation Error Summary")
        message(STATUS "========================================")
        message(STATUS "")
        message(STATUS "${ERROR_CONTENT}")
        message(STATUS "")
        message(STATUS "========================================")
    else()
        message(STATUS "No compilation errors found in ${ERROR_LOG_FILE}")
    endif()
else()
    message(STATUS "${CMAKE_CURRENT_BINARY_DIR}/binary is not found, skipping error log check")
endif()
