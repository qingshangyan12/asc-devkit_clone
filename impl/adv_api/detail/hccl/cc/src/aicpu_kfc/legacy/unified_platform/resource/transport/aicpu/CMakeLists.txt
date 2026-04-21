# -----------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# -----------------------------------------------------------------------------------------------------------
target_sources(hccl_v2 PRIVATE
        mem_transport_lite.cc
        ub_transport_lite_impl.cc
        p2p_transport_lite_impl.cc
)

if(NOT BUILD_OPEN_PROJECT OR (BUILD_OPEN_PROJECT AND KERNEL_MODE))
target_sources(ccl_kernel PRIVATE
        mem_transport_lite.cc
        ub_transport_lite_impl.cc
        p2p_transport_lite_impl.cc
)
endif()

if(TARGET mc2_server)
add_mc2_server_ccl_object(mc2_server_legacy_transport_aicpu
        ${CMAKE_CURRENT_SOURCE_DIR}/mem_transport_lite.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ub_transport_lite_impl.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/p2p_transport_lite_impl.cc
)
endif()
