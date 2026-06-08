# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ----------------------------------------------------------------------------------------------------------
set(HCCL_CANN_VERSION_NUM 0)

if(BUILD_OPEN_PROJECT)
    message(STATUS "open TOP_DIR = ${TOP_DIR}")
    set(OPS_BASE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../)

    # 开源仓编译：从 CANN 包的 version 头解析 CANN_VERSION_NUM
    set(_hccl_cann_version_header "${ASCEND_CANN_PACKAGE_PATH}/include/version/cann_version.h")
    message(STATUS "Checking HCCL scoped CANN version header: ${_hccl_cann_version_header}")
    if(EXISTS "${_hccl_cann_version_header}")
        file(STRINGS "${_hccl_cann_version_header}" _hccl_cann_ver_line
            REGEX "^#define[ \t]+CANN_VERSION_NUM[ \t]+")
        if(_hccl_cann_ver_line)
            string(REGEX REPLACE "^#define[ \t]+CANN_VERSION_NUM[ \t]+(.+)$" "\\1"
                CANN_EXPR "${_hccl_cann_ver_line}")
            math(EXPR HCCL_CANN_VERSION_NUM "${CANN_EXPR}" OUTPUT_FORMAT DECIMAL)
        endif()
    endif()
else()
    message(STATUS "close TOP_DIR = ${TOP_DIR}")
    set(OPS_BASE_DIR ${TOP_DIR}/hccl/)
    set(INSTALL_AICPU_KERNEL_JSON_DIR ${INSTALL_LIBRARY_DIR})

    # 大工程编译：CANN 包路径不确定，改为从 hccl 自身的 version.cmake
    # (set_cann_package(hccl VERSION "X.Y.Z")) 解析 CANN_VERSION_NUM
    set(_hccl_version_cmake "${OPS_BASE_DIR}/version.cmake")
    message(STATUS "Checking HCCL version.cmake: ${_hccl_version_cmake}")
    if(EXISTS "${_hccl_version_cmake}")
        file(STRINGS "${_hccl_version_cmake}" _hccl_ver_pkg_line
            REGEX "set_cann_package[ \t]*\\([ \t]*hccl[ \t]+VERSION[ \t]+\"[0-9]+\\.[0-9]+\\.[0-9]+\"")
        if(_hccl_ver_pkg_line)
            string(REGEX MATCH "\"([0-9]+)\\.([0-9]+)\\.([0-9]+)\"" _ "${_hccl_ver_pkg_line}")
            if(NOT CMAKE_MATCH_1 STREQUAL "" AND NOT CMAKE_MATCH_2 STREQUAL "" AND NOT CMAKE_MATCH_3 STREQUAL "")
                math(EXPR HCCL_CANN_VERSION_NUM
                    "${CMAKE_MATCH_1} * 10000000 + ${CMAKE_MATCH_2} * 100000 + ${CMAKE_MATCH_3} * 1000")
            endif()
        endif()
    endif()
endif()

if(HCCL_CANN_VERSION_NUM EQUAL 0)
    message(WARNING "HCCL failed to resolve CANN_VERSION_NUM (got 0). "
        "Compatibility branches (e.g. HCCL_CANN_COMPAT_850) will fall back to the 9.0.0+ path.")
endif()

set(HCCL_CANN_COMPAT_850 OFF)
if(HCCL_CANN_VERSION_NUM GREATER 0 AND HCCL_CANN_VERSION_NUM LESS 90000000)
    set(HCCL_CANN_COMPAT_850 ON)
endif()

message(STATUS "HCCL scoped CANN_VERSION_NUM = ${HCCL_CANN_VERSION_NUM}")
message(STATUS "HCCL_CANN_COMPAT_850 = ${HCCL_CANN_COMPAT_850}")

function(hccl_apply_cann_compat target_name)
    if(NOT TARGET ${target_name})
        return()
    endif()
    target_compile_definitions(${target_name} PRIVATE CANN_VERSION_NUM=${HCCL_CANN_VERSION_NUM})
endfunction()

set(OPS_BASE_DIR ${HCCL_CC_DIR})

set(INCLUDE_LIST
    ${HCCL_CC_INCLUDE_DIR}
    ${HCCL_CC_INCLUDE_DIR}/hcomm
    ${HCCL_CC_INCLUDE_DIR}/hcomm/hccl
    ${HCCL_CC_INCLUDE_DIR}/hcomm/pkg_inc/hccl
    ${HCCL_CC_INCLUDE_DIR}/hcomm/pkg_inc/hcomm/ccu
    ${HCCL_CC_INCLUDE_DIR}/hcomm/pkg_inc
    ${CMAKE_CURRENT_SOURCE_DIR}/common
    ${CMAKE_CURRENT_SOURCE_DIR}/common/hcomm_dlsym
    ${CMAKE_CURRENT_SOURCE_DIR}/common/hcomm_dlsym/ccu

    ${CMAKE_CURRENT_SOURCE_DIR}/ops
    ${CMAKE_CURRENT_SOURCE_DIR}/ops/op_common
    ${CMAKE_CURRENT_SOURCE_DIR}/ops/op_common/inc

    ${CMAKE_CURRENT_SOURCE_DIR}/ops/op_common/executor
    ${CMAKE_CURRENT_SOURCE_DIR}/ops/op_common/executor/channel
    ${CMAKE_CURRENT_SOURCE_DIR}/ops/op_common/executor/registry

    ${CMAKE_CURRENT_SOURCE_DIR}/ops/op_common/selector

    ${CMAKE_CURRENT_SOURCE_DIR}/ops/op_common/template
    ${CMAKE_CURRENT_SOURCE_DIR}/ops/op_common/template/registry
    ${CMAKE_CURRENT_SOURCE_DIR}/ops/op_common/template/aicpu
    ${CMAKE_CURRENT_SOURCE_DIR}/ops/op_common/template/wrapper
    ${CMAKE_CURRENT_SOURCE_DIR}/ops/op_common/template/aiv
    # ${CMAKE_CURRENT_SOURCE_DIR}/ops/op_common/template/hostdpu
    ${CMAKE_CURRENT_SOURCE_DIR}/ops/op_common/template/ccu
    ${CMAKE_CURRENT_SOURCE_DIR}/ops/op_common/template/ccu/kernel

    ${CMAKE_CURRENT_SOURCE_DIR}/ops/op_common/topo
    ${CMAKE_CURRENT_SOURCE_DIR}/ops/interface_graph_mode

    ${CMAKE_CURRENT_SOURCE_DIR}/ops/reduce_scatter
    ${CMAKE_CURRENT_SOURCE_DIR}/ops/reduce_scatter/executor
    ${CMAKE_CURRENT_SOURCE_DIR}/ops/reduce_scatter/selector
    ${CMAKE_CURRENT_SOURCE_DIR}/ops/reduce_scatter/template
    ${CMAKE_CURRENT_SOURCE_DIR}/ops/reduce_scatter/template/aicpu
    ${CMAKE_CURRENT_SOURCE_DIR}/ops/reduce_scatter/template/aiv
    ${CMAKE_CURRENT_SOURCE_DIR}/ops/reduce_scatter/template/ccu
    ${CMAKE_CURRENT_SOURCE_DIR}/ops/reduce_scatter/template/ccu/kernel

    ${CMAKE_CURRENT_SOURCE_DIR}/ops/all_gather
    ${CMAKE_CURRENT_SOURCE_DIR}/ops/all_gather/executor
    ${CMAKE_CURRENT_SOURCE_DIR}/ops/all_gather/selector
    ${CMAKE_CURRENT_SOURCE_DIR}/ops/all_gather/template
    ${CMAKE_CURRENT_SOURCE_DIR}/ops/all_gather/template/aicpu
    ${CMAKE_CURRENT_SOURCE_DIR}/ops/all_gather/template/aiv
    ${CMAKE_CURRENT_SOURCE_DIR}/ops/all_gather/template/ccu
    ${CMAKE_CURRENT_SOURCE_DIR}/ops/all_gather/template/ccu/kernel
)


if(PRODUCT_SIDE STREQUAL "device")
    add_library(mc2_server SHARED
        ${CMAKE_CURRENT_SOURCE_DIR}/common/utils.cc
        # ${CMAKE_CURRENT_SOURCE_DIR}/common/adapter_acl.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/common/config_log.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/common/sal.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/common/log.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/common/adapter_error_manager_pub.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/common/alg_env_config.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/common/device_compat.cc

        ${CMAKE_CURRENT_SOURCE_DIR}/ops/op_common/omnipipe_data_slice_calc.cc
        
        ${CMAKE_CURRENT_SOURCE_DIR}/ops/op_common/exec_timeout_manager.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ops/op_common/executor/channel/channel.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ops/op_common/executor/channel/channel_request.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ops/op_common/executor/registry/coll_alg_exec_registry.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ops/op_common/executor/registry/coll_alg_v2_exec_registry.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ops/op_common/executor/executor_base.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ops/op_common/executor/executor_v2_base.cc

        ${CMAKE_CURRENT_SOURCE_DIR}/ops/op_common/template/alg_template_base.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ops/op_common/template/alg_v2_template_base.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ops/op_common/template/template_utils.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ops/op_common/template/aicpu/kernel_launch.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ops/op_common/template/aicpu/dfx/task_exception_fun.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ops/op_common/template/registry/alg_template_register.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ops/op_common/template/wrapper/alg_data_trans_wrapper.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ops/op_common/template/dpu/kernel_launch.cc


        ${CMAKE_CURRENT_SOURCE_DIR}/ops/op_common/topo/topo.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ops/op_common/topo/topo_match_1d.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ops/op_common/topo/topo_match_base.cc

        ${CMAKE_CURRENT_SOURCE_DIR}/ops/scatter/algo/scatter_comm_executor.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ops/scatter/algo/scatter_executor_base.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ops/scatter/algo/scatter_mesh_executor.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ops/scatter/algo/scatter_ring_executor.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ops/scatter/algo/scatter_single_executor.cc

        ${CMAKE_CURRENT_SOURCE_DIR}/ops/scatter/algo/template/nhr_base.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ops/scatter/algo/template/scatter_mesh.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ops/scatter/algo/template/scatter_nb.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ops/scatter/algo/template/scatter_nhr.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ops/scatter/algo/template/scatter_ring_direct.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ops/scatter/algo/template/scatter_ring.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ops/scatter/executor/ins_v2_scatter_sole_executor.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ops/scatter/executor/ins_v2_scatter_parallel_executor.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ops/scatter/template/aicpu/ins_temp_scatter_mesh_1D.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ops/scatter/template/aicpu/ins_temp_scatter_nhr.cc


        ${CMAKE_CURRENT_SOURCE_DIR}/ops/reduce_scatter/executor/ins_v2_reduce_scatter_sole_executor.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ops/reduce_scatter/executor/ins_reduce_scatter_parallel_executor.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ops/reduce_scatter/executor/ins_v2_reduce_scatter_sequence_executor_aicpu.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ops/reduce_scatter/template/aicpu/ins_temp_reduce_scatter_mesh_1D.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ops/reduce_scatter/template/aicpu/ins_temp_reduce_scatter_nhr.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ops/reduce_scatter/template/aicpu/ins_temp_reduce_scatter_mesh_1D_meshchunk.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ops/reduce_scatter/template/aicpu/ins_temp_reduce_scatter_aicpu_reduce_nhr.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ops/reduce_scatter/template/aicpu/ins_temp_reduce_scatter_mesh_1D_Z_axis_detour.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ops/reduce_scatter/template/aicpu/ins_temp_reduce_scatter_omnipipe_mesh_1d_dpu.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ops/reduce_scatter/template/aicpu/ins_temp_reduce_scatter_omnipipe_mesh_1D.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ops/reduce_scatter/template/aicpu/ins_temp_reduce_scatter_omnipipe_nhr.cc
 

        ${CMAKE_CURRENT_SOURCE_DIR}/ops/broadcast/executor/ins_v2_broadcast_parallel_executor.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ops/broadcast/executor/ins_v2_broadcast_sole_executor.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ops/broadcast/template/aicpu/ins_temp_broadcast_mesh_1D_two_shot.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ops/broadcast/template/aicpu/ins_temp_broadcast_nhr.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ops/broadcast/template/aicpu/ins_temp_allgather_mesh_1D_intra.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ops/broadcast/template/aicpu/ins_temp_scatter_mesh_1D_intra.cc

        ${CMAKE_CURRENT_SOURCE_DIR}/ops/all_gather/executor/ins_v2_all_gather_sole_executor.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ops/all_gather/executor/ins_v2_all_gather_parallel_executor.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ops/all_gather/executor/ins_v2_all_gather_sequence_executor_aicpu.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ops/all_gather/template/aicpu/ins_temp_all_gather_mesh_1D.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ops/all_gather/template/aicpu/ins_temp_all_gather_mesh_1D_Z_axis_detour.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ops/all_gather/template/aicpu/ins_temp_all_gather_nhr.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ops/all_gather/template/aicpu/ins_temp_all_gather_omnipipe_mesh_1D.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ops/all_gather/template/aicpu/ins_temp_all_gather_omnipipe_nhr_dpu.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ops/all_gather/template/aicpu/ins_temp_all_gather_omnipipe_nhr.cc

        ${CMAKE_CURRENT_SOURCE_DIR}/ops/reduce/executor/reduce_sole_executor.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ops/reduce/executor/reduce_parallel_executor.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ops/reduce/template/aicpu/reduce_mesh_1D.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ops/reduce/template/aicpu/reduce_mesh_1D_two_shot.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ops/reduce/template/aicpu/reduce_nhr.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ops/reduce/template/aicpu/reduce_aicpu_reduce_nhr.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ops/reduce/template/aicpu/ins_temp_gather_mesh_1D_intra.cc

        ${CMAKE_CURRENT_SOURCE_DIR}/ops/all_gather_v/executor/ins_v2_all_gather_v_sole_executor.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ops/all_gather_v/template/aicpu/ins_temp_all_gather_v_mesh_1D.cc

        ${CMAKE_CURRENT_SOURCE_DIR}/ops/all_to_all_v/executor/ins_v2_all_to_all_v_sole_executor.cc

        ${CMAKE_CURRENT_SOURCE_DIR}/ops/all_to_all_v/template/aicpu/ins_temp_all_to_all_v_mesh_1D.cc

        ${CMAKE_CURRENT_SOURCE_DIR}/ops/all_reduce/executor/ins_v2_all_reduce_sole_executor.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ops/all_reduce/executor/ins_v2_all_reduce_parallel_executor.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ops/all_reduce/executor/ins_v2_all_reduce_sequence_executor_aicpu.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ops/all_reduce/executor/ins_v2_all_reduce_two_shot_sole_executor.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ops/all_reduce/template/aicpu/ins_temp_all_reduce_mesh_1D_one_shot.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ops/all_reduce/template/aicpu/ins_temp_all_reduce_mesh_1D_two_shot.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ops/all_reduce/template/aicpu/ins_temp_all_reduce_nhr.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ops/all_reduce/template/aicpu/ins_temp_all_reduce_aicpu_reduce_nhr.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ops/all_reduce/template/aicpu/ins_temp_all_reduce_mesh_1D_two_shot_mesh_chunk.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ops/all_reduce/template/aicpu/ins_temp_reduce_scatter_mesh_1D_intra.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ops/all_reduce/template/aicpu/ins_temp_all_gather_mesh_1D_intra.cc

        ${CMAKE_CURRENT_SOURCE_DIR}/ops/send/executor/ins_send_executor.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ops/recv/executor/ins_recv_executor.cc

        ${CMAKE_CURRENT_SOURCE_DIR}/ops/reduce_scatter_v/executor/ins_v2_reduce_scatter_v_sole_executor.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ops/reduce_scatter_v/template/aicpu/ins_temp_reduce_scatter_v_mesh_1D.cc

        ${CMAKE_CURRENT_SOURCE_DIR}/ops/op_common/template/registry/alg_v2_template_register.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ops/batch_send_recv/executor/ins_v2_batch_send_recv_executor.cc

        ${CMAKE_CURRENT_SOURCE_DIR}/ops/recv/executor/ins_v2_recv_sole_executor.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ops/send/executor/ins_v2_send_sole_executor.cc

    )

    if(NOT HCCL_CANN_COMPAT_850)
        target_sources(scatter_aicpu_kernel PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/ops/send/executor/ins_send_dpu_executor.cc
            ${CMAKE_CURRENT_SOURCE_DIR}/ops/recv/executor/ins_recv_dpu_executor.cc
            ${CMAKE_CURRENT_SOURCE_DIR}/ops/send/template/host_nic/ins_temp_send_host_nic_dpu.cc
            ${CMAKE_CURRENT_SOURCE_DIR}/ops/recv/template/host_nic/ins_temp_recv_host_nic_dpu.cc
            ${CMAKE_CURRENT_SOURCE_DIR}/ops/op_common/template/wrapper/dpu_alg_data_trans_wrapper.cc
            ${CMAKE_CURRENT_SOURCE_DIR}/ops/op_common/topo/topo_match_multilevel.cc
            ${CMAKE_CURRENT_SOURCE_DIR}/ops/op_common/topo/topo_match_ubx.cc
            ${CMAKE_CURRENT_SOURCE_DIR}/ops/op_common/topo/topo_match_ubx_1d.cc
            ${CMAKE_CURRENT_SOURCE_DIR}/ops/op_common/topo/topo_match_pcie_mix.cc
            ${CMAKE_CURRENT_SOURCE_DIR}/ops/reduce_scatter/executor/ins_reduce_scatter_concurrent_executor.cc
            ${CMAKE_CURRENT_SOURCE_DIR}/ops/all_gather/executor/ins_v2_all_gather_concurrent_executor.cc
            ${CMAKE_CURRENT_SOURCE_DIR}/ops/all_to_all_v/executor/ins_v2_all_to_all_concurrent_executor.cc
            ${CMAKE_CURRENT_SOURCE_DIR}/ops/all_reduce/executor/ins_v2_all_reduce_concurrent_executor.cc
            ${CMAKE_CURRENT_SOURCE_DIR}/ops/reduce_scatter/executor/ins_v2_reduce_scatter_omnipipe_executor.cc
            ${CMAKE_CURRENT_SOURCE_DIR}/ops/all_gather/executor/ins_v2_all_gather_omnipipe_executor.cc
            ${CMAKE_CURRENT_SOURCE_DIR}/ops/all_reduce/executor/ins_v2_all_reduce_omnipipe_executor.cc
            ${CMAKE_CURRENT_SOURCE_DIR}/ops/reduce_scatter/executor/ins_v2_reduce_scatter_sequence_executor.cc
            ${CMAKE_CURRENT_SOURCE_DIR}/ops/reduce_scatter/template/aicpu/ins_temp_reduce_scatter_mesh_1d_dpu.cc
            ${CMAKE_CURRENT_SOURCE_DIR}/ops/broadcast/executor/ins_v2_broadcast_sequence_executor.cc
            ${CMAKE_CURRENT_SOURCE_DIR}/ops/broadcast/template/aicpu/ins_temp_allgather_nhr_dpu_inter.cc
            ${CMAKE_CURRENT_SOURCE_DIR}/ops/broadcast/template/aicpu/ins_temp_scatter_nhr_dpu_inter.cc
            ${CMAKE_CURRENT_SOURCE_DIR}/ops/all_gather/executor/ins_v2_all_gather_sequence_executor.cc
            ${CMAKE_CURRENT_SOURCE_DIR}/ops/all_gather/template/aicpu/ins_temp_all_gather_nhr_dpu.cc
            ${CMAKE_CURRENT_SOURCE_DIR}/ops/reduce/executor/ins_v2_reduce_sequence_executor.cc
            ${CMAKE_CURRENT_SOURCE_DIR}/ops/reduce/template/aicpu/ins_temp_gather_dpu_inter.cc
            ${CMAKE_CURRENT_SOURCE_DIR}/ops/all_to_all_v/template/aicpu/ins_temp_dpu_alltoall_mesh.cc
            ${CMAKE_CURRENT_SOURCE_DIR}/ops/all_reduce/executor/ins_v2_all_reduce_sequence_executor.cc
            ${CMAKE_CURRENT_SOURCE_DIR}/ops/all_reduce/template/aicpu/ins_temp_reduce_scatter_mesh_1D_dpu_inter.cc
            ${CMAKE_CURRENT_SOURCE_DIR}/ops/all_reduce/template/aicpu/ins_temp_all_gather_nhr_dpu_inter.cc
            ${CMAKE_CURRENT_SOURCE_DIR}/ops/send/template/ins_temp_send_dpu.cc
            ${CMAKE_CURRENT_SOURCE_DIR}/ops/recv/template/ins_temp_recv_dpu.cc
        )
    endif()

    target_include_directories(mc2_server PRIVATE
        ${INCLUDE_LIST}
    )

    target_compile_options(mc2_server PRIVATE
        $<$<CONFIG:Debug>:-g>
        $<$<CONFIG:Release>:-O3>
        -fvisibility=hidden
        -fvisibility-inlines-hidden
        -fstack-protector-all
    )

    target_link_options(mc2_server PRIVATE
        -Wl,-z,relro
        -Wl,-z,now
        -Wl,-z,noexecstack
        -Wl,-Bsymbolic
        $<$<CONFIG:Release>:-s>
    )

    target_compile_definitions(mc2_server PRIVATE
        -DAICPU_COMPILE
        MC2_SERVER_ONLY
    )

    target_link_libraries(mc2_server PRIVATE
        -Wl,--no-as-needed
        mc2_kernel_compat
        -Wl,--no-as-needed
        mmpa
        $<BUILD_INTERFACE:acl_rt_headers>
        $<BUILD_INTERFACE:error_manager_headers>
    )

    hccl_apply_cann_compat(mc2_server)
endif()

add_definitions(-DORION_MODE)

if(NOT KERNEL_MODE AND PRODUCT_SIDE STREQUAL "host")
    add_library(mc2_client SHARED)

    # 基于ini生成json文件
    SET(HCCL_CMAKE_DIR ${HCCL_CC_DIR}/cmake/)
    add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/libmc2_server.json
        COMMAND ${HI_PYTHON} ${HCCL_CMAKE_DIR}/scripts/parser_ini.py ${CMAKE_CURRENT_SOURCE_DIR}/ops/op_common/mc2_server.ini ${CMAKE_CURRENT_BINARY_DIR}/libmc2_server.json
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
    add_custom_target(aicpu_kernel_json DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/libmc2_server.json)
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/libmc2_server.json
        DESTINATION "opp/built-in/op_impl/aicpu/config" COMPONENT asc-devkit
    )
    add_dependencies(mc2_client aicpu_kernel_json)

    if(BUILD_OPEN_PROJECT)
        target_compile_definitions(mc2_client PRIVATE
            OPEN_BUILD_PROJECT
            $<$<STREQUAL:${PRODUCT_SIDE},host>:_GLIBCXX_USE_CXX11_ABI=0>
        )
    else()
        target_compile_definitions(mc2_client PRIVATE
            $<$<STREQUAL:${PRODUCT_SIDE},host>:_GLIBCXX_USE_CXX11_ABI=0>
        )
    endif()

    target_include_directories(mc2_client PRIVATE
        ${INCLUDE_LIST}
    )

    target_compile_definitions(mc2_client PRIVATE
        -DHOST_COMPILE
    )

    hccl_apply_cann_compat(mc2_client)

    target_compile_options(mc2_client PRIVATE
        -Werror
        -fno-common
        -fno-strict-aliasing
        -pipe
        -O3
        $<$<CONFIG:Debug>:-g>
        -std=c++14
        -fstack-protector-all
    )

    # libhccl
    target_link_options(mc2_client PRIVATE
        -Wl,-z,relro
        -Wl,-z,now
        -Wl,-z,noexecstack
        -Wl,-Bsymbolic-functions
        $<$<CONFIG:Release>:-s>
    )

    target_link_libraries(mc2_client PRIVATE
        $<BUILD_INTERFACE:runtime_headers>
        $<BUILD_INTERFACE:error_manager_headers>
        acl_rt
    )


    add_dependencies(mc2_client mc2_compat)

    # libhcomm must precede mc2_compat: mc2_compat exports weak fallback wrappers
    # that must not preempt real HCOMM symbols during dynamic symbol binding.
    if(BUILD_OPEN_PROJECT)
        set(MC2_CLIENT_HCOMM_STUB_SRC
            ${CMAKE_CURRENT_BINARY_DIR}/mc2_client_hcomm_stub.cc)
        file(WRITE ${MC2_CLIENT_HCOMM_STUB_SRC}
            "extern \"C\" void __mc2_hcomm_stub(void) {}\n")
        add_library(mc2_client_hcomm_stub SHARED
            ${MC2_CLIENT_HCOMM_STUB_SRC})
        set_target_properties(mc2_client_hcomm_stub PROPERTIES
            OUTPUT_NAME hcomm
            LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/mc2_client_link_stub)
        target_link_options(mc2_client_hcomm_stub PRIVATE
            -Wl,-soname,libhcomm.so)

        target_link_libraries(mc2_client PRIVATE
            -Wl,--no-as-needed
            mc2_client_hcomm_stub
            mc2_compat
            acl_rt
            c_sec
            unified_dlog
            mmpa
            -Wl,--no-as-needed
        )
    else()
        target_link_libraries(mc2_client PRIVATE
            $<BUILD_INTERFACE:slog_headers>
            $<BUILD_INTERFACE:msprof_headers>
            $<BUILD_INTERFACE:npu_runtime_headers>
            $<BUILD_INTERFACE:mmpa_headers>
            -Wl,--no-as-needed
            hcomm
            mc2_compat
            acl_rt
            c_sec
            unified_dlog
            mmpa
            -Wl,--no-as-needed
            ofed_headers
        )
    endif()

    install(TARGETS mc2_client
        DESTINATION "${CMAKE_SYSTEM_PROCESSOR}-linux/lib64" COMPONENT asc-devkit
    )
endif()

add_subdirectory(common)

if(PRODUCT_SIDE STREQUAL "host")
    add_subdirectory(ops)
endif()

if(DEVICE_MODE AND KERNEL_MODE)
    add_subdirectory(aicpu_kfc EXCLUDE_FROM_ALL)
endif()
