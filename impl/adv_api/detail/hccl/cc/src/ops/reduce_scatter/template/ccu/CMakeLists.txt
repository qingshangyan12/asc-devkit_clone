# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ----------------------------------------------------------------------------------------------------------
add_subdirectory(kernel)

if(NOT MC2_CLIENT_USE_ASCCOMM_CCU_DATAPLANE)
    set(src_list
        ${CMAKE_CURRENT_SOURCE_DIR}/ccu_temp_kfc_reduce_scatter_mesh_1D_mem2mem.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ccu_temp_kfc_reduce_scatter_nhr_1D_multi_jetty_mem2mem.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ccu_temp_kfc_reduce_scatter_mesh_1D_mem2mem_peer_only.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ccu_temp_kfc_reduce_scatter_nhr_1D_multi_jetty_mem2mem.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ccu_temp_kfc_reduce_scatter_nhr_1D_2die_mem2mem.cc
        # Legacy direct-launch CCU templates are retained but use APIs unavailable in the KFC-only mc2_client.
        # ${CMAKE_CURRENT_SOURCE_DIR}/ccu_temp_reduce_scatter_mesh_1D_mem2mem.cc
        # ${CMAKE_CURRENT_SOURCE_DIR}/ccu_temp_reduce_scatter_mesh_1D.cc
        # ${CMAKE_CURRENT_SOURCE_DIR}/ccu_temp_reduce_scatter_nhr_1D_mem2mem.cc
        # ${CMAKE_CURRENT_SOURCE_DIR}/ccu_temp_reduce_scatter_mesh_1D_2die_mem2mem.cc
        # ${CMAKE_CURRENT_SOURCE_DIR}/ccu_temp_reduce_scatter_nhr_1D_multi_jetty_mem2mem.cc
        # ${CMAKE_CURRENT_SOURCE_DIR}/ccu_temp_reduce_scatter_mesh2die.cc
    )

    target_sources(mc2_client PRIVATE
        ${src_list}
    )
else()
    # 实验构建（asc-comm 数据面）：ReduceScatter CCU 模板临时禁用。
endif()
