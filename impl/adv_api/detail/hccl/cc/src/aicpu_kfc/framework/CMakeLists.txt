# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ----------------------------------------------------------------------------------------------------------
set(HCCL_PACKET_FLAG 1)

if(BUILD_OPEN_PROJECT)
    message(STATUS "open TOP_DIR = ${TOP_DIR}")
    set(HCCL_BASE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../)
    set(HCCL_OPEN_CODE_BASE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../)
    set(HCCL_THIRD_PARTY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../../../../../../../)
else()
    message(STATUS "close TOP_DIR = ${TOP_DIR}")
    set(HCCL_BASE_DIR ${TOP_DIR}/hcomm/src/)
    set(HCCL_OPEN_CODE_BASE_DIR ${TOP_DIR}/hcomm/src/)
    set(HCCL_THIRD_PARTY_DIR ${TOP_DIR})
    set(INSTALL_CCL_KERNEL_JSON_DIR ${INSTALL_LIBRARY_DIR})
    set(INSTALL_DPU_KERNEL_JSON_DIR ${INSTALL_LIBRARY_DIR})
endif()

set(HCCL_V2_CODE_ROOT ${ORION_REAL_PATH})

message(STATUS "HCCL_OPEN_CODE_BASE_DIR = ${HCCL_OPEN_CODE_BASE_DIR}")
message(STATUS "HCCL_BASE_DIR = ${HCCL_BASE_DIR}")

set(hccl_include_list
    ${HCCL_HCOMM_HEADER_DIR}/hccl
    ${HCCL_BASE_DIR}/pub_inc
    ${HCCL_BASE_DIR}/pub_inc/new
    ${HCCL_BASE_DIR}/framework/cluster_maintenance/health/heartbeat/
    ${HCCL_BASE_DIR}/framework/cluster_maintenance/recovery/operator_retry/
    ${HCCL_BASE_DIR}/framework/cluster_maintenance/detect/detect_connect_anomalies/
    ${HCCL_BASE_DIR}/framework/cluster_maintenance/snapshot/
    ${HCCL_BASE_DIR}/framework/cluster_maintenance/aclgraph/
    ${HCCL_BASE_DIR}/framework/common/src/
    ${HCCL_BASE_DIR}/framework/common/src/host
    ${HCCL_BASE_DIR}/framework/common/src/config
    ${HCCL_BASE_DIR}/framework/common/src/topo
    ${HCCL_BASE_DIR}/framework/common/src/task
    ${HCCL_BASE_DIR}/framework/common/src/exception
    ${HCCL_BASE_DIR}/framework/common/src/h2d_tlv
    ${HCCL_BASE_DIR}/framework/common/src/thread
    ${HCCL_BASE_DIR}/framework/common/src/h2d_dto
    ${HCCL_BASE_DIR}/framework/common/src/onesided_memory_management
    ${HCCL_BASE_DIR}/framework/common/src/mgr
    ${HCCL_BASE_DIR}/framework/communicator/impl/
    ${HCCL_BASE_DIR}/framework/communicator/impl/resource_manager
    ${HCCL_BASE_DIR}/framework/communicator/impl/one_sided_service
    ${HCCL_BASE_DIR}/framework/communicator/impl/zero_copy
    ${HCCL_BASE_DIR}/framework/communicator/impl/independent_op
    ${HCCL_BASE_DIR}/framework/communicator/impl/independent_op/rank_graph
    ${HCCL_BASE_DIR}/framework/communicator/impl/independent_op/resource/engine
    ${HCCL_BASE_DIR}/framework/communicator/impl/independent_op/channel
    ${HCCL_BASE_DIR}/framework/communicator/impl/independent_op/channel/device
    ${HCCL_BASE_DIR}/framework/hcom/
    ${HCCL_BASE_DIR}/framework/hcom/gradient_segment
    ${HCCL_BASE_DIR}/framework/inc/
    ${HCCL_BASE_DIR}/framework/
    ${HCCL_BASE_DIR}/framework/nslbdp/
    ${HCCL_BASE_DIR}/framework/op_base/src
    ${HCCL_BASE_DIR}/framework/group
    ${HCCL_BASE_DIR}/common/health/
    ${HCCL_BASE_DIR}/common/stream/
    ${HCCL_BASE_DIR}/common/launch_device/
    ${HCCL_BASE_DIR}/platform/resource/socket
    ${HCCL_BASE_DIR}/platform/inc
    ${HCCL_BASE_DIR}/platform/inc/adapter
    ${HCCL_BASE_DIR}/framework/next/comms/comm_engine_res/threads
    ${HCCL_BASE_DIR}/framework/next/comms/comm_engine_res/threads/slaves/
    ${HCCL_BASE_DIR}/framework/next/coll_comms/
    ${HCCL_BASE_DIR}/framework/next/coll_comms/communicator
    ${HCCL_BASE_DIR}/framework/next/coll_comms/rank
    ${HCCL_BASE_DIR}/framework/next/coll_comms/rank/comm_mems/
    ${HCCL_BASE_DIR}/framework/next/coll_comms/rank_pairs
    ${HCCL_BASE_DIR}/framework/next/coll_comms/dfx
    ${HCCL_BASE_DIR}/framework/next/coll_comms/dfx/profiling
    ${HCCL_BASE_DIR}/framework/next/coll_comms/dfx/profiling/aicpu
    ${HCCL_BASE_DIR}/framework/next/coll_comms/dfx/profiling/host
    ${HCCL_BASE_DIR}/framework/next/coll_comms/dfx/taskException
    ${HCCL_BASE_DIR}/framework/next/coll_comms/dfx/taskException/aicpu
    ${HCCL_BASE_DIR}/framework/next/coll_comms/dfx/taskException/host
    ${HCCL_BASE_DIR}/framework/next/comms/endpoint_pairs/channels
    ${HCCL_BASE_DIR}/framework/next/comms/endpoint_pairs/
    ${HCCL_BASE_DIR}/framework/next/comms/endpoint_pairs/channels/aicpu/
    ${HCCL_BASE_DIR}/framework/next/comms/endpoint_pairs/channels/aicpu/device/
    ${HCCL_BASE_DIR}/framework/next/comms/endpoints/server_socket/
    ${HCCL_BASE_DIR}/framework/next/comms/adpt
    ${HCCL_BASE_DIR}/framework/next/comms/ccu/pub_inc/
    ${HCCL_BASE_DIR}/framework/next/comms/ccu/ccu_device/
    ${HCCL_BASE_DIR}/framework/next/comms/ccu/ccu_device/ccu_comp
    ${HCCL_BASE_DIR}/framework/next/comms/ccu/ccu_device/ccu_comp/ccu_channel
    ${HCCL_BASE_DIR}/framework/next/comms/ccu/ccu_device/ccu_comp/ccu_channel/ccu_pfe
    ${HCCL_BASE_DIR}/framework/next/comms/ccu/ccu_device/ccu_comp/ccu_channel/ccu_channel_ctx_v1
    ${HCCL_BASE_DIR}/framework/next/comms/ccu/ccu_transport/
    ${HCCL_BASE_DIR}/framework/next/comms/ccu/ccu_kernel
)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

message(STATUS "framework KERNEL_MODE=${KERNEL_MODE}")
message(STATUS "framework BUILD_OPEN_PROJECT=${BUILD_OPEN_PROJECT}")


set(ORION_HEAD_LIST)
FILE(GLOB_RECURSE sources_list LIST_DIRECTORIES TRUE ${HCCL_V2_CODE_ROOT})

foreach (dir ${sources_list})
    IF ((IS_DIRECTORY ${dir}) AND (${dir} MATCHES "aicpu_kfc/legacy") AND (NOT ${dir} MATCHES "aicpu_kfc/legacy/mdpi"))
        list(APPEND ORION_HEAD_LIST ${dir})
    endif()
endforeach()

set(MC2_SERVER_ORION_HEAD_LIST
    ${HCCL_BASE_DIR}/legacy/common
    ${HCCL_BASE_DIR}/legacy/common/exception
    ${HCCL_BASE_DIR}/legacy/common/types
    ${HCCL_BASE_DIR}/legacy/common/utils
    ${HCCL_BASE_DIR}/legacy/framework/communicator
    ${HCCL_BASE_DIR}/legacy/framework/communicator/aicpu
    ${HCCL_BASE_DIR}/legacy/framework/dfx/aicpu/common
    ${HCCL_BASE_DIR}/legacy/framework/dfx/aicpu/profiling
    ${HCCL_BASE_DIR}/legacy/framework/dfx/common
    ${HCCL_BASE_DIR}/legacy/interface
    ${HCCL_BASE_DIR}/legacy/service/collective
    ${HCCL_BASE_DIR}/legacy/unified_platform/external_system
    ${HCCL_BASE_DIR}/legacy/unified_platform/pub_inc
    ${HCCL_BASE_DIR}/legacy/unified_platform/resource
    ${HCCL_BASE_DIR}/legacy/unified_platform/resource/buffer
    ${HCCL_BASE_DIR}/legacy/unified_platform/resource/buffer/aicpu
    ${HCCL_BASE_DIR}/legacy/unified_platform/resource/connection/aicpu
    ${HCCL_BASE_DIR}/legacy/unified_platform/resource/notify/aicpu
    ${HCCL_BASE_DIR}/legacy/unified_platform/resource/stream/aicpu
    ${HCCL_BASE_DIR}/legacy/unified_platform/resource/transport/aicpu
)

if(NOT BUILD_OPEN_PROJECT OR (BUILD_OPEN_PROJECT AND KERNEL_MODE))
    add_library(asc_ccl_kernel  SHARED)

    set(CCL_KERNEL_SRC_LIST

        # common
        ${HCCL_BASE_DIR}/common/debug/profiling/task_profiling.cc
        ${HCCL_BASE_DIR}/common/debug/profiling/task_exception_handler.cc
        ${HCCL_BASE_DIR}/common/debug/profiling/task_exception_handler_device.cc
        ${HCCL_BASE_DIR}/common/debug/profiling/profiler_base.cc
        ${HCCL_BASE_DIR}/common/debug/profiling/plugin_runner.cc
        ${HCCL_BASE_DIR}/common/debug/profiling/plugin_runner_device.cc
        ${HCCL_BASE_DIR}/common/debug/profiling/command_handle.cc
        ${HCCL_BASE_DIR}/common/debug/profiling/profiling_manager.cc
        ${HCCL_BASE_DIR}/common/debug/profiling/task_overflow.cc
        ${HCCL_BASE_DIR}/common/debug/profiling/profiler_manager_impl.cc
        ${HCCL_BASE_DIR}/common/debug/profiling/profiler_manager.cc
        ${HCCL_BASE_DIR}/common/debug/profiling/profiling_manager_pub.cc
        ${HCCL_BASE_DIR}/common/debug/profiling/adapter_prof.cc
        ${HCCL_BASE_DIR}/common/debug/profiling/dlprof_function.cc
        ${HCCL_BASE_DIR}/common/debug/profiling/dlrt_function.cc
        ${HCCL_BASE_DIR}/common/debug/config/config_log.cc
        ${HCCL_BASE_DIR}/common/health/calc_crc.cc
        ${HCCL_BASE_DIR}/common/health/rank_consistentcy_checker.cc
        ${HCCL_BASE_DIR}/common/stream/stream_utils_aicpu.cc

        #framework
        ${HCCL_BASE_DIR}/framework/communicator/impl/independent_op/hccl_independent_rank_graph.cc
        ${HCCL_BASE_DIR}/framework/communicator/impl/one_sided_service/i_hccl_one_sided_service.cc
    )
    target_sources(asc_ccl_kernel PRIVATE
        ${CCL_KERNEL_SRC_LIST}
    )

    target_compile_options(asc_ccl_kernel PRIVATE
        -Wfloat-equal
        -Wall
        -fno-common
        -fstack-protector-strong
        -fno-strict-aliasing
        -pipe
        -O3
        -std=c++14
    )

    target_link_options(asc_ccl_kernel PRIVATE
    -Wl,-z,relro
    -Wl,-z,now
    -Wl,-z,noexecstack
    -s
)
    
    target_compile_definitions(asc_ccl_kernel PRIVATE
        HCCD
        CCL_KERNEL_AICPU
    )

    set(CCL_KERNEL_INCLUDE_LIST
        ${HCCL_LOCAL_HEADER_DIRS}
        ${HCCL_HCOMM_HEADER_DIR}/hccl
        ${HCCL_BASE_DIR}/pub_inc
        ${TOP_DIR}/abl/atrace/inc/utrace
        ${HCCL_BASE_DIR}/common/health
        ${HCCL_BASE_DIR}/common/debug/profiling/inc
        ${HCCL_BASE_DIR}/common/debug/profiling/inc/aicpu
        ${HCCL_BASE_DIR}/common/debug/config/
        ${HCCL_BASE_DIR}/common/debug/
        ${HCCL_BASE_DIR}/common/stream
        ${HCCL_BASE_DIR}/common/launch_device
        ${HCCL_BASE_DIR}/framework/inc
        ${HCCL_BASE_DIR}/framework/cluster_maintenance/health/heartbeat/
        ${HCCL_BASE_DIR}/framework/cluster_maintenance/detect/detect_connect_anomalies/
        ${HCCL_BASE_DIR}/framework/cluster_maintenance/snapshot/
        ${HCCL_BASE_DIR}/framework/common/src
        ${HCCL_BASE_DIR}/framework/common/src/aicpu
        ${HCCL_BASE_DIR}/framework/common/src/config
        ${HCCL_BASE_DIR}/framework/common/src/task
        ${HCCL_BASE_DIR}/framework/common/src/topo
        ${HCCL_BASE_DIR}/framework/hcom
        ${HCCL_BASE_DIR}/framework/communicator/impl/
        ${HCCL_BASE_DIR}/framework/communicator/impl/one_sided_service/
        ${HCCL_BASE_DIR}/framework/communicator/impl/zero_copy
        ${HCCL_BASE_DIR}/framework/communicator/impl/resource_manager
        ${HCCL_BASE_DIR}/framework/op_base/src/
        ${HCCL_BASE_DIR}/framework/device/
        ${HCCL_BASE_DIR}/framework/device/aicpu_kfc
        ${HCCL_BASE_DIR}/framework/device/aicpu_kfc/inc
        ${HCCL_BASE_DIR}/framework/device/aicpu_kfc/dfx
        ${HCCL_BASE_DIR}/framework/device/common
        ${HCCL_BASE_DIR}/framework/device/inc
        ${HCCL_BASE_DIR}/framework/device/framework
        ${HCCL_BASE_DIR}/framework/cluster_maintenance/recovery/operator_retry
        ${HCCL_BASE_DIR}/framework/common/src/exception
        ${HCCL_BASE_DIR}/framework/communicator/impl/independent_op/
        ${HCCL_BASE_DIR}/framework/communicator/impl/independent_op/resource
        ${HCCL_BASE_DIR}/framework/communicator/impl/independent_op/thread
        ${HCCL_BASE_DIR}/framework/communicator/impl/independent_op/rank_graph
        ${HCCL_BASE_DIR}/framework/communicator/impl/independent_op/channel
        ${HCCL_BASE_DIR}/framework/communicator/impl/independent_op/channel/device
        ${HCCL_BASE_DIR}/framework/communicator/impl/independent_op/data_api
        ${HCCL_HCOMM_HEADER_DIR}/pkg_inc/
        ${HCCL_HCOMM_HEADER_DIR}/pkg_inc/hccl/

        ${HCCL_BASE_DIR}/platform/
        ${HCCL_BASE_DIR}/platform/inc/
        ${HCCL_BASE_DIR}/platform/inc/adapter
        ${HCCL_BASE_DIR}/platform/resource/transport/heterog/
        ${HCCL_BASE_DIR}/platform/resource/transport/host/
        ${HCCL_BASE_DIR}/platform/resource/transport/
        ${HCCL_BASE_DIR}/platform/resource/notify/
        ${HCCL_BASE_DIR}/platform/task
        ${HCCL_BASE_DIR}/platform/common
        ${HCCL_BASE_DIR}/platform/common/unique
        ${HCCL_BASE_DIR}/platform/common/unfold_cache


        ${RDMA_CORE_INCLUDE_DIR}
        ${HCCL_BASE_DIR}/platform/
        ${HCCL_BASE_DIR}/platform/inc/
        ${HCCL_BASE_DIR}/platform/inc/adapter
        ${HCCL_BASE_DIR}/platform/resource/transport/heterog/
        ${HCCL_BASE_DIR}/platform/resource/transport/host/
        ${HCCL_BASE_DIR}/platform/resource/transport/
        ${HCCL_BASE_DIR}/platform/resource/notify/
        ${HCCL_BASE_DIR}/platform/resource/dispatcher_ctx
        ${HCCL_BASE_DIR}/platform/task
        ${HCCL_BASE_DIR}/platform/common
        ${HCCL_BASE_DIR}/platform/common/unique
        ${hccl_include_list}
        ${HCCL_HCOMM_HEADER_DIR}
        ${HCCL_HCOMM_HEADER_DIR}/hccl
        ${HCCL_BASE_DIR}/../inc/hccl/hccl
        ${HCCL_HCOMM_HEADER_DIR}/pkg_inc/
        ${HCCL_HCOMM_HEADER_DIR}/pkg_inc/hccl/
        ${HCCL_BASE_DIR}/pub_inc
        ${HCCL_BASE_DIR}/pub_inc/aicpu

        ${HCCL_BASE_DIR}/framework/device/
        ${HCCL_BASE_DIR}/framework/device/common
        ${HCCL_BASE_DIR}/framework/device/inc
        ${HCCL_BASE_DIR}/framework/device/framework


        ${HCCL_HCOMM_HEADER_DIR}/externel_depends/tsch/
        ${RDMA_CORE_INCLUDE_DIR}
        ${THIRD_PARTY_NLOHMANN_PATH}

        

        ${HCCL_BASE_DIR}/platform/hccp/inc/network/
        ${HCCL_BASE_DIR}/platform/hccp/inc/
        ${HCCL_BASE_DIR}/framework/next/comms/endpoint_pairs/sockets/
        ${HCCL_BASE_DIR}/framework/next/comms/endpoints/
        ${HCCL_BASE_DIR}/framework/next/comms/comm_engine_res/threads
        ${HCCL_BASE_DIR}/framework/next/comms/comm_engine_res/threads/slaves/
        ${HCCL_BASE_DIR}/framework/next/coll_comms/communicator/aicpu
        ${HCCL_BASE_DIR}/framework/next/coll_comms/dfx
    )

    set(MC2_SERVER_CCL_INCLUDE_LIST ${CCL_KERNEL_INCLUDE_LIST})
    list(APPEND MC2_SERVER_CCL_INCLUDE_LIST
        ${ASCEND_CANN_PACKAGE_PATH}/${HCCL_HOST_ARCH_PREFIX}/pkg_inc/base
        ${ASCEND_CANN_PACKAGE_PATH}/${HCCL_HOST_ARCH_PREFIX}/pkg_inc/runtime
        ${ASCEND_CANN_PACKAGE_PATH}/${HCCL_HOST_ARCH_PREFIX}/pkg_inc/profiling
    )
    list(REMOVE_ITEM MC2_SERVER_CCL_INCLUDE_LIST
        ${HCCL_BASE_DIR}/framework/cluster_maintenance/health/heartbeat/
        ${HCCL_BASE_DIR}/framework/cluster_maintenance/detect/detect_connect_anomalies/
        ${HCCL_BASE_DIR}/framework/cluster_maintenance/snapshot/
        ${HCCL_BASE_DIR}/framework/cluster_maintenance/recovery/operator_retry
        ${HCCL_BASE_DIR}/framework/cluster_maintenance/recovery/operator_retry/
        ${HCCL_BASE_DIR}/framework/cluster_maintenance/aclgraph/
        ${HCCL_BASE_DIR}/framework/hcom
        ${HCCL_BASE_DIR}/framework/hcom/
        ${HCCL_BASE_DIR}/framework/hcom/gradient_segment
        ${HCCL_BASE_DIR}/framework/nslbdp/
        ${HCCL_BASE_DIR}/framework/group
        ${HCCL_BASE_DIR}/framework/op_base/src
        ${HCCL_BASE_DIR}/framework/op_base/src/
        ${HCCL_BASE_DIR}/platform/resource/socket
        ${HCCL_BASE_DIR}/platform/resource/transport/heterog/
        ${HCCL_BASE_DIR}/platform/resource/transport/host/
        ${HCCL_BASE_DIR}/platform/resource/transport/
        ${HCCL_BASE_DIR}/platform/resource/notify/
        ${HCCL_BASE_DIR}/platform/resource/dispatcher_ctx
        ${HCCL_BASE_DIR}/platform/common/unfold_cache
    )
    list(REMOVE_DUPLICATES MC2_SERVER_CCL_INCLUDE_LIST)

    target_include_directories(asc_ccl_kernel PRIVATE
        ${CCL_KERNEL_INCLUDE_LIST}
    )


    target_include_directories(asc_ccl_kernel PRIVATE ${ORION_HEAD_LIST})
    if(TARGET json)
        target_link_libraries(asc_ccl_kernel PRIVATE json)
        add_dependencies(asc_ccl_kernel json)
    endif()

    set(MC2_SERVER_CCL_OBJECT_ALLOWLIST
        mc2_server_aicpu_kfc_common
    )

    function(add_mc2_server_ccl_object target_name)
        if("${target_name}" IN_LIST MC2_SERVER_CCL_OBJECT_ALLOWLIST)
            add_mc2_server_object(${target_name} ${ARGN})
        else()
            add_mc2_server_detached_object(${target_name} ${ARGN})
        endif()
    endfunction()

    configure_mc2_server_minimal_target()

    add_subdirectory(device)

endif()

if(NOT BUILD_OPEN_PROJECT)
    # orion仓依赖
    FILE(GLOB_RECURSE sources_list LIST_DIRECTORIES TRUE ${TOP_DIR}/ace/comop/hccl/orion/*)
    foreach (dir ${sources_list})
        IF ((IS_DIRECTORY ${dir}))
            target_include_directories(asc_ccl_kernel PRIVATE ${dir})
        endif()
    endforeach()

    FILE(GLOB_RECURSE open_dir_list LIST_DIRECTORIES TRUE ${TOP_DIR}/ace/comop/hccl/whole/hccl_next/*)
    foreach (dir ${open_dir_list})
        IF ((IS_DIRECTORY ${dir}))
            target_include_directories(asc_ccl_kernel PRIVATE ${dir})
        endif()
    endforeach()
    target_include_directories(asc_ccl_kernel PRIVATE
        ${HCCL_HCOMM_HEADER_DIR}/hccl
        ${HCCL_BASE_DIR}/pub_inc
        ${HCCL_BASE_DIR}/pub_inc/inner
        ${HCCL_BASE_DIR}/pub_inc/aicpu
        ${HCCL_BASE_DIR}/pub_inc/new

        ${HCCL_BASE_DIR}/hccl/hccl_comm/wrapper/notify/

        ${TOP_DIR}/inc
        ${TOP_DIR}/inc/driver
        ${TOP_DIR}/metadef/inc/external
        ${TOP_DIR}/metadef/pkg_inc
        ${TOP_DIR}/metadef/pkg_inc
        ${TOP_DIR}/open_source/json/include
        ${TOP_DIR}/inc/aicpu/
        ${TOP_DIR}/atc/opcompiler/ascendc_compiler/api
        ${TOP_DIR}/atc/opcompiler/ascendc_compiler/api/impl
        ${TOP_DIR}/atc/opcompiler/ascendc_compiler/api/include
        ${TOP_DIR}/abl/adump/inc/adump
        ${TOP_DIR}/abl/atrace/inc/utrace
        ${TOP_DIR}/runtime/include/external
 	    ${TOP_DIR}/runtime/include/external/acl
 	    ${TOP_DIR}/runtime/pkg_inc
 	    ${TOP_DIR}/runtime/pkg_inc/runtime
 	    ${TOP_DIR}/runtime/pkg_inc/profiling
 	    ${TOP_DIR}/runtime/pkg_inc/trace
 	    ${TOP_DIR}/runtime/pkg_inc/base
 	    ${TOP_DIR}/runtime/pkg_inc/aicpu_sched
        ${TOP_DIR}/asc/asc-devkit
 	    ${TOP_DIR}/asc/asc-devkit/include/adv_api/hccl/internal

        # orion
        ${TOP_DIR}/ace/comop/inc

        ${HCCL_BASE_DIR}/platform/
        ${HCCL_BASE_DIR}/platform/inc/
        ${HCCL_BASE_DIR}/platform/inc/adapter
        ${HCCL_BASE_DIR}/platform/resource/transport/heterog/
        ${HCCL_BASE_DIR}/platform/resource/transport/host/
        ${HCCL_BASE_DIR}/platform/resource/transport/
        ${HCCL_BASE_DIR}/platform/resource/notify/
        ${HCCL_BASE_DIR}/platform/resource/dispatcher_ctx
        ${HCCL_BASE_DIR}/platform/task
        ${HCCL_BASE_DIR}/platform/common
        ${HCCL_BASE_DIR}/platform/common/unique
        ${HCCL_BASE_DIR}/platform/common/unfold_cache

        ${HCCL_BASE_DIR}/platform/hccp/inc
        ${HCCL_BASE_DIR}/platform/hccp/inc/network
        ${HCCL_HCOMM_HEADER_DIR}/pkg_inc/
        ${HCCL_HCOMM_HEADER_DIR}/pkg_inc/hccl/

        ${HCCL_BASE_DIR}/framework
 	    ${HCCL_BASE_DIR}/framework/common
 	    ${HCCL_BASE_DIR}/framework/common/src
        ${HCCL_BASE_DIR}/framework/common/src/mgr
        ${HCCL_BASE_DIR}/framework/communicator
        ${HCCL_BASE_DIR}/framework/communicator/impl/independent_op/resource/engine
    )
    if(TARGET mc2_server)
        target_include_directories(mc2_server PRIVATE
            ${HCCL_HCOMM_HEADER_DIR}/hccl
            ${HCCL_BASE_DIR}/pub_inc
            ${HCCL_BASE_DIR}/pub_inc/inner
            ${HCCL_BASE_DIR}/pub_inc/aicpu
            ${HCCL_BASE_DIR}/pub_inc/new

            ${HCCL_BASE_DIR}/hccl/hccl_comm/wrapper/notify/

            ${TOP_DIR}/inc
            ${TOP_DIR}/inc/driver
            ${TOP_DIR}/metadef/inc/external
            ${TOP_DIR}/metadef/pkg_inc
            ${TOP_DIR}/metadef/pkg_inc
            ${TOP_DIR}/open_source/json/include
            ${TOP_DIR}/inc/aicpu/
            ${TOP_DIR}/atc/opcompiler/ascendc_compiler/api
            ${TOP_DIR}/atc/opcompiler/ascendc_compiler/api/impl
            ${TOP_DIR}/atc/opcompiler/ascendc_compiler/api/include
            ${TOP_DIR}/abl/adump/inc/adump
            ${TOP_DIR}/abl/atrace/inc/utrace
            ${TOP_DIR}/runtime/include/external
            ${TOP_DIR}/runtime/include/external/acl
            ${TOP_DIR}/runtime/pkg_inc
            ${TOP_DIR}/runtime/pkg_inc/runtime
            ${TOP_DIR}/runtime/pkg_inc/profiling
            ${TOP_DIR}/runtime/pkg_inc/trace
            ${TOP_DIR}/runtime/pkg_inc/base
            ${TOP_DIR}/runtime/pkg_inc/aicpu_sched
            ${TOP_DIR}/asc/asc-devkit
            ${TOP_DIR}/asc/asc-devkit/include/adv_api/hccl/internal

            ${TOP_DIR}/ace/comop/inc

            ${HCCL_BASE_DIR}/platform/
            ${HCCL_BASE_DIR}/platform/inc/
            ${HCCL_BASE_DIR}/platform/inc/adapter
            ${HCCL_BASE_DIR}/platform/resource/transport/heterog/
            ${HCCL_BASE_DIR}/platform/resource/transport/host/
            ${HCCL_BASE_DIR}/platform/resource/transport/
            ${HCCL_BASE_DIR}/platform/resource/notify/
            ${HCCL_BASE_DIR}/platform/resource/dispatcher_ctx
            ${HCCL_BASE_DIR}/platform/task
            ${HCCL_BASE_DIR}/platform/common
            ${HCCL_BASE_DIR}/platform/common/unique
            ${HCCL_BASE_DIR}/platform/common/unfold_cache

            ${HCCL_BASE_DIR}/platform/hccp/inc
            ${HCCL_BASE_DIR}/platform/hccp/inc/network
            ${HCCL_HCOMM_HEADER_DIR}/pkg_inc/
            ${HCCL_HCOMM_HEADER_DIR}/pkg_inc/hccl/

            ${HCCL_BASE_DIR}/framework
            ${HCCL_BASE_DIR}/framework/common
            ${HCCL_BASE_DIR}/framework/common/src
            ${HCCL_BASE_DIR}/framework/common/src/mgr
            ${HCCL_BASE_DIR}/framework/communicator
            ${HCCL_BASE_DIR}/framework/communicator/impl/independent_op/resource/engine
        )
    endif()
    target_link_libraries(asc_ccl_kernel PRIVATE
        $<BUILD_INTERFACE:intf_pub_cxx14>
        $<BUILD_INTERFACE:mmpa_headers>
        $<BUILD_INTERFACE:msprof_headers>
        $<BUILD_INTERFACE:slog_headers>
        $<BUILD_INTERFACE:asc_hccl_headers>
        $<BUILD_INTERFACE:npu_runtime_headers>
        $<BUILD_INTERFACE:atrace_headers>
        $<BUILD_INTERFACE:kernel_tiling_headers>
        $<BUILD_INTERFACE:rdma_core_headers>
        -Wl,--no-as-needed
        ${HCCL_C_SEC_LINK}
        asc_ccl_kernel_plf
        ${HCCL_MMPA_LINK}
        -Wl,--as-needed
        -lrt
        -ldl
        -lpthread
        ofed_headers
        ascend_hal
        c_sec
        unified_dlog
    )

    install(TARGETS  asc_ccl_kernel
        LIBRARY DESTINATION ${INSTALL_LIBRARY_DIR}  OPTIONAL
    )
endif()
add_subdirectory(common)
add_subdirectory(communicator)
add_subdirectory(hcom)
add_subdirectory(op_base)
add_subdirectory(group)
add_subdirectory(cluster_maintenance)
add_subdirectory(nslbdp)
add_subdirectory(next)

if(NOT KERNEL_MODE)
    SET(HCCL_CMAKE_DIR ${HCCL_CC_DIR}/cmake/)
    message(STATUS "HCCL_CMAKE_DIR = ${HCCL_CMAKE_DIR}")
    add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/asc_ccl_kernel.json
        COMMAND ${HI_PYTHON} ${HCCL_CMAKE_DIR}/scripts/parser_ini.py ${CMAKE_CURRENT_SOURCE_DIR}/device/framework/asc_ccl_kernel.ini ${CMAKE_CURRENT_BINARY_DIR}/asc_ccl_kernel.json
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
    add_custom_target(ccl_kernel_json DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/asc_ccl_kernel.json)
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/asc_ccl_kernel.json
        DESTINATION ${INSTALL_CCL_KERNEL_JSON_DIR} OPTIONAL
    )

    add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/libaicpu_custom.json
        COMMAND ${HI_PYTHON} ${HCCL_CMAKE_DIR}/scripts/parser_ini.py ${CMAKE_CURRENT_SOURCE_DIR}/device/framework/asc_aicpu_custom.ini ${CMAKE_CURRENT_BINARY_DIR}/libaicpu_custom.json
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
    add_custom_target(asc_aicpu_custom_json DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/libaicpu_custom.json)
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/libaicpu_custom.json
        DESTINATION ${INSTALL_CCL_KERNEL_JSON_DIR} OPTIONAL
    )

    add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/libccl_dpu.json
        COMMAND ${HI_PYTHON} ${HCCL_CMAKE_DIR}/scripts/parser_ini.py ${CMAKE_CURRENT_SOURCE_DIR}/../legacy/framework/communicator/hostdpu/ccl_dpu.ini ${CMAKE_CURRENT_BINARY_DIR}/libccl_dpu.json
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
    add_custom_target(dpu_kernel_json DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/libccl_dpu.json)
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/libccl_dpu.json
        DESTINATION ${INSTALL_DPU_KERNEL_JSON_DIR} OPTIONAL
    )
endif()

if(BUILD_OPEN_PROJECT AND KERNEL_MODE)
    if(BUILD_OPEN_PROJECT)
        set(CMAKE_CXX_FLAGS "")
        target_compile_definitions(asc_ccl_kernel PRIVATE
            OPEN_BUILD_PROJECT
            -D_GLIBCXX_USE_CXX11_ABI=1) # 链接ascendalog需要LOG_CPP，链接slog不需要
 
        set(CCL_KERNEL_PLF_PATH
            -Wl,--whole-archive
            asc_ccl_kernel_plf_a
            -Wl,--no-whole-archive
            -Wl,-Bsymbolic
        )
        if(FULL_MODE)
            set(CCL_KERNEL_PLF_PATH asc_ccl_kernel_plf)
        endif()
        target_link_libraries(asc_ccl_kernel PRIVATE
            -Wl,--no-as-needed
            ascend_hal
            c_sec
            unified_dlog
            ${HCCL_C_SEC_LINK}
            ${HCCL_MMPA_LINK}
            ${CCL_KERNEL_PLF_PATH}
            -Wl,--as-needed
            -lrt
            -ldl
            -lpthread
        )

        add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/libaicpu_custom.json
            COMMAND ${HI_PYTHON} ${HCCL_CC_DIR}/cmake/scripts/parser_ini.py ${HCCL_BASE_DIR}/../src/framework/device/framework/asc_aicpu_custom.ini ${CMAKE_CURRENT_BINARY_DIR}/libaicpu_custom.json
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        )
        add_custom_target(asc_aicpu_custom_json DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/libaicpu_custom.json)
        install(FILES ${CMAKE_CURRENT_BINARY_DIR}/libaicpu_custom.json
            DESTINATION ${INSTALL_CCL_KERNEL_JSON_DIR} OPTIONAL
        )

    endif()
endif()

if(DEVICE_MODE AND KERNEL_MODE)
    set(CCL_KERNEL_TAR_DIR ${CMAKE_SOURCE_DIR}/build_device/ccl_kernel_tar_pkg/aicpu_kernels_device)
    add_custom_command(
        TARGET asc_ccl_kernel
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CCL_KERNEL_TAR_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:asc_ccl_kernel> ${CCL_KERNEL_TAR_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:asc_ccl_kernel_plf> ${CCL_KERNEL_TAR_DIR}
        COMMAND chmod 750 ${CCL_KERNEL_TAR_DIR}/lib*
        COMMENT "Copying libccl_kernel_plf.so libccl_kernel.so to ${CCL_KERNEL_TAR_DIR}"
    )

    set(AICPU_CUSTOM_COMPILE_DEFINITIONS
        HCCD
        CCL_KERNEL_AICPU
        OPEN_BUILD_PROJECT
        -D_GLIBCXX_USE_CXX11_ABI=1
    )
    set(AICPU_CUSTOM_LINK_LIBRARIES
        -Wl,--no-as-needed
        ascend_hal
        ${HCCL_C_SEC_LINK}
        ${HCCL_MMPA_LINK}
        -Wl,--whole-archive
        asc_ccl_kernel_plf_a
        -Wl,--no-whole-archive
        -Wl,--as-needed
        -lrt
        -ldl
        -lpthread
    )

    add_library(asc_aicpu_custom SHARED)
    get_target_property(CCL_KERNEL_ALL_SOURCES asc_ccl_kernel SOURCES)
    if(CCL_KERNEL_ALL_SOURCES)
        list(REMOVE_DUPLICATES CCL_KERNEL_ALL_SOURCES)
        target_sources(asc_aicpu_custom PRIVATE ${CCL_KERNEL_ALL_SOURCES})
    endif()
    target_compile_definitions(asc_aicpu_custom PRIVATE ${AICPU_CUSTOM_COMPILE_DEFINITIONS})
    target_compile_options(asc_aicpu_custom PRIVATE
        -Werror
        -Wfloat-equal
        -Wall
        -fno-common
        -fstack-protector-strong
        -fno-strict-aliasing
        -pipe
        -O3
        -std=c++14
    )
    target_link_options(asc_aicpu_custom PRIVATE
        -Wl,-z,relro
        -Wl,-z,now
        -Wl,-z,noexecstack
        -s
    )
    target_include_directories(asc_aicpu_custom PRIVATE
        ${CCL_KERNEL_INCLUDE_LIST}
        ${ORION_HEAD_LIST}
    )

    target_link_libraries(asc_aicpu_custom PRIVATE
        ${AICPU_CUSTOM_LINK_LIBRARIES}
        rdma_core_headers
        ascend_hal 
        c_sec 
        unified_dlog
    )

    add_dependencies(asc_aicpu_custom asc_ccl_kernel asc_ccl_kernel_plf_a asc_aicpu_custom_json)
    set_target_properties(asc_aicpu_custom PROPERTIES
        OUTPUT_NAME asc_aicpu_custom
        PREFIX "lib"
        SUFFIX ".so"
    )
    install(TARGETS asc_aicpu_custom
        LIBRARY DESTINATION ${INSTALL_CCL_KERNEL_JSON_DIR} OPTIONAL
    )
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/libaicpu_custom.json
        DESTINATION ${INSTALL_CCL_KERNEL_JSON_DIR} OPTIONAL
    )
endif()
