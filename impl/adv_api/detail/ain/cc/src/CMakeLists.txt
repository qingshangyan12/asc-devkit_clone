# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2026 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ----------------------------------------------------------------------------------------------------------

# ACL / runtime host packages (exposes acl_rt for aclrtMalloc/aclrtMemcpy/...).
find_cann_package(acl_rt REQUIRED)
find_cann_package(runtime REQUIRED)

add_library(ain_host SHARED
    ${CMAKE_CURRENT_SOURCE_DIR}/ain.cpp
)

target_include_directories(ain_host PRIVATE
    # Public interface header (adv_api/ain/ain_host.h) and adv_api/hccl/hccl.h.
    ${ASCENDC_INCLUDE_DIR}/adv_api
    # HCCL host internal headers: hccl_rank_graph.h / hccl_res.h / hccl_types.h.
    ${ASCENDC_INCLUDE_DIR}/adv_api/hccl/internal/hcomm/hccl
    # External hcomm headers (hcomm/hcomm_res.h, hcomm/hcomm_res_entity_defs.h) are provided by the
    # installed CANN package include path at build time.
    ${ASCEND_CANN_PACKAGE_PATH}/include
)

target_compile_options(ain_host PRIVATE
    -fno-common
    -fno-strict-aliasing
    -pipe
    $<$<CONFIG:Debug>:-g>
    $<$<CONFIG:Release>:-O3>
    -fstack-protector-all
)

target_link_options(ain_host PRIVATE
    -Wl,-z,relro
    -Wl,-z,now
    -Wl,-z,noexecstack
    -Wl,-Bsymbolic-functions
    $<$<CONFIG:Release>:-s>
)

# HCCL host library is linked by name (same approach detail/hccl/cc uses for `hccl`).
target_link_libraries(ain_host PRIVATE
    -Wl,--no-as-needed
    hccl
    acl_rt
    -Wl,--no-as-needed
)

install(TARGETS ain_host
    DESTINATION "${CMAKE_SYSTEM_PROCESSOR}-linux/lib64" COMPONENT asc-devkit
)
