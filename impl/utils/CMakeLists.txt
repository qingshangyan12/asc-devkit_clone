# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ----------------------------------------------------------------------------------------------------------

install(
  DIRECTORY std
  DESTINATION ${INSTALL_LIBRARY_DIR}/asc/impl/utils
  ${INSTALL_OPTIONAL} COMPONENT asc-devkit FILES_MATCHING
  PATTERN "*.h"
  PATTERN "CMakeLists.txt" EXCLUDE)

install(
  DIRECTORY ${ASCENDC_INCLUDE_DIR}/utils/
  DESTINATION ${INSTALL_LIBRARY_DIR}/asc/include/utils/ ${INSTALL_OPTIONAL} COMPONENT asc-devkit
)

install(FILES common_types.h sys_constants.h sys_macros.h
    DESTINATION ${INSTALL_LIBRARY_DIR}/asc/impl/utils ${INSTALL_OPTIONAL} COMPONENT asc-devkit
)
