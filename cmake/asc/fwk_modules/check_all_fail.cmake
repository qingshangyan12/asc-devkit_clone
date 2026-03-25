# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ----------------------------------------------------------------------------------------------------------

set(ERROR_LOG_FILE "${OUT_DIR}/compile_error.log")

if(EXISTS "${ERROR_LOG_FILE}")
    execute_process(
        COMMAND grep -c "fail op-type:" "${ERROR_LOG_FILE}"
        OUTPUT_VARIABLE FAILED_COUNT
        ERROR_QUIET
    )

    if(NOT FAILED_COUNT)
        set(FAILED_COUNT 0)
    endif()

    message(STATUS "Found ${FAILED_COUNT} failed operators out of ${TYPE_COUNT} total")

    if(FAILED_COUNT EQUAL TYPE_COUNT)
        set_property(GLOBAL PROPERTY ${PROPERTY_NAME} TRUE)
        message(STATUS "Set global property ${PROPERTY_NAME} to TRUE")
    else()
        set_property(GLOBAL PROPERTY ${PROPERTY_NAME} FALSE)
        message(STATUS "Set global property ${PROPERTY_NAME} to FALSE (default)")
    endif()
else()
    set_property(GLOBAL PROPERTY ${PROPERTY_NAME} FALSE)
    message(STATUS "Set global property ${PROPERTY_NAME} to FALSE (no file)")
endif()
