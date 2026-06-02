# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ----------------------------------------------------------------------------------------------------------

# copy std to highlevel_api impl
install(
  DIRECTORY std debug
  DESTINATION ${INSTALL_LIBRARY_DIR}/asc/impl/utils
  OPTIONAL FILES_MATCHING
  PATTERN "*.h"
  PATTERN "CMakeLists.txt" EXCLUDE)

install(
  DIRECTORY ${ASCENDC_INCLUDE_DIR}/utils/
  DESTINATION ${INSTALL_LIBRARY_DIR}/asc/include/utils/ OPTIONAL
  PATTERN "utils/stub" EXCLUDE
)

install(FILES common_types.h sys_constants.h sys_macros.h
    DESTINATION ${INSTALL_LIBRARY_DIR}/asc/impl/utils
)
