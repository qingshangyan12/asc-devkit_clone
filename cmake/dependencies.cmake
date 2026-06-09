# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ----------------------------------------------------------------------------------------------------------

set(CMAKE_PREFIX_PATH ${ASCEND_CANN_PACKAGE_PATH}/)

set(CMAKE_MODULE_PATH
  ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules
  ${CMAKE_MODULE_PATH}
)

foreach(_pkg securec mmpa metadef platform runtime acl_rt error_manager)
    if(NOT TARGET ${_pkg})
        find_cann_package(${_pkg} REQUIRED)
    endif()
endforeach()

if(NOT TARGET unified_dlog)
    find_cann_package(unified_dlog REQUIRED)
endif()

if(ENABLE_TEST)
  find_cann_package(graph REQUIRED)
  find_cann_package(pvmodel REQUIRED)
endif()
