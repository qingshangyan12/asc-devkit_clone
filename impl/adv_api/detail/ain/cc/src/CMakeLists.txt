# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2026 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ----------------------------------------------------------------------------------------------------------

find_cann_package(acl_rt REQUIRED)
find_cann_package(hccl REQUIRED)

add_library(ain_host SHARED
    ${CMAKE_CURRENT_SOURCE_DIR}/ain.cpp
)

target_include_directories(ain_host PRIVATE
    # Repo include root only, so the public interface header resolves as adv_api/ain/ain_host.h.
    # Do NOT add ${ASCENDC_INCLUDE_DIR}/adv_api here: that would shadow the host HCCL C API headers
    # (hccl/hccl.h etc.) with the device-side kernel headers under adv_api/hccl.
    ${ASCENDC_INCLUDE_DIR}
    # Host HCCL/hcomm C API headers (hccl/hccl.h, hccl/hccl_types.h, hccl/hccl_res.h,
    # hccl/hccl_rank_graph.h, hcomm/hcomm_res.h, acl/*) come from the installed CANN package.
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

target_link_libraries(ain_host PRIVATE
    -Wl,--no-as-needed
    hccl
    acl_rt
    -Wl,--as-needed
)

install(TARGETS ain_host
    DESTINATION "${CMAKE_SYSTEM_PROCESSOR}-linux/lib64" COMPONENT asc-devkit
)
