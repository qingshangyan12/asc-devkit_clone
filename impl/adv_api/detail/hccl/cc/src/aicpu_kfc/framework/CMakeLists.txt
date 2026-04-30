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
    ${HCCL_BASE_DIR}/algorithm/base/inc
    ${HCCL_BASE_DIR}/algorithm/pub_inc
    ${HCCL_BASE_DIR}/pub_inc
    ${HCCL_BASE_DIR}/pub_inc/new
    ${HCCL_BASE_DIR}/algorithm/base/alg_aiv_template
    ${HCCL_BASE_DIR}/algorithm/base/alg_template
    ${HCCL_BASE_DIR}/algorithm/base/mc2_handler
    ${HCCL_BASE_DIR}/algorithm/base/alg_template/component
    ${HCCL_BASE_DIR}/algorithm/base/communicator
    ${HCCL_BASE_DIR}/algorithm/base/communicator/legacy
    ${HCCL_BASE_DIR}/algorithm/impl
    ${HCCL_BASE_DIR}/algorithm/impl/legacy
    ${HCCL_BASE_DIR}/algorithm/impl/inc
    ${HCCL_BASE_DIR}/algorithm/impl/task
    ${HCCL_BASE_DIR}/algorithm/impl/resource_manager
    ${HCCL_BASE_DIR}/algorithm/impl/coll_executor
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
message(STATUS "framework ASCEND_CANN_PACKAGE_PATH=${ASCEND_CANN_PACKAGE_PATH}")

set(ORION_HEAD_LIST)
FILE(GLOB_RECURSE sources_list LIST_DIRECTORIES TRUE ${HCCL_V2_CODE_ROOT})

foreach (dir ${sources_list})
    IF ((IS_DIRECTORY ${dir}) AND (${dir} MATCHES "aicpu_kfc/legacy") AND (NOT ${dir} MATCHES "aicpu_kfc/legacy/mdpi"))
        list(APPEND ORION_HEAD_LIST ${dir})
    endif()
endforeach()

if(NOT BUILD_OPEN_PROJECT OR (BUILD_OPEN_PROJECT AND KERNEL_MODE))
    set(CCL_KERNEL_INCLUDE_LIST
        ${HCCL_LOCAL_HEADER_DIRS}
        ${HCCL_HCOMM_HEADER_DIR}/hccl
        ${HCCL_BASE_DIR}/pub_inc
        ${TOP_DIR}/abl/atrace/inc/utrace
        ${HCCL_BASE_DIR}/algorithm/base/inc
        ${HCCL_BASE_DIR}/algorithm/pub_inc
        ${HCCL_BASE_DIR}/algorithm/base/alg_aiv_template
        ${HCCL_BASE_DIR}/algorithm/base/alg_template
        ${HCCL_BASE_DIR}/algorithm/base/alg_template/component
        ${HCCL_BASE_DIR}/algorithm/base/mc2_handler
        ${HCCL_BASE_DIR}/algorithm/base/communicator
        ${HCCL_BASE_DIR}/algorithm/impl
        ${HCCL_BASE_DIR}/algorithm/impl/inc
        ${HCCL_BASE_DIR}/algorithm/impl/legacy
        ${HCCL_BASE_DIR}/algorithm/impl/legacy/operator
        ${HCCL_BASE_DIR}/algorithm/base/communicator/legacy
        ${HCCL_BASE_DIR}/algorithm/impl/resource_manager
        ${HCCL_BASE_DIR}/algorithm/impl/task
        ${HCCL_BASE_DIR}/algorithm/impl/operator
        ${HCCL_BASE_DIR}/algorithm/impl/operator/registry
        ${HCCL_BASE_DIR}/algorithm/impl/coll_executor
        ${HCCL_BASE_DIR}/algorithm/impl/coll_executor/registry
        ${HCCL_BASE_DIR}/algorithm/impl/coll_executor/coll_send_receive
        ${HCCL_BASE_DIR}/algorithm/impl/coll_executor/coll_all_reduce
        ${HCCL_BASE_DIR}/algorithm/impl/coll_executor/coll_all_reduce/310P
        ${HCCL_BASE_DIR}/algorithm/impl/coll_executor/coll_all_gather
        ${HCCL_BASE_DIR}/algorithm/impl/coll_executor/coll_all_gather/310P
        ${HCCL_BASE_DIR}/algorithm/impl/coll_executor/coll_all_gather_v
        ${HCCL_BASE_DIR}/algorithm/impl/coll_executor/coll_reduce_scatter
        ${HCCL_BASE_DIR}/algorithm/impl/coll_executor/coll_reduce_scatter/310P
        ${HCCL_BASE_DIR}/algorithm/impl/coll_executor/coll_reduce_scatter_v
        ${HCCL_BASE_DIR}/algorithm/impl/coll_executor/coll_reduce_scatter_v/310P
        ${HCCL_BASE_DIR}/algorithm/impl/coll_executor/coll_all_to_all
        ${HCCL_BASE_DIR}/algorithm/impl/coll_executor/coll_scatter
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
    
        ${HCCL_BASE_DIR}/algorithm/base/alg_template/temp_all_gather
        ${HCCL_BASE_DIR}/algorithm/base/alg_template/temp_all_reduce
        ${HCCL_BASE_DIR}/algorithm/base/alg_template/temp_alltoall
        ${HCCL_BASE_DIR}/algorithm/base/alg_template/temp_alltoallv
        ${HCCL_BASE_DIR}/algorithm/base/alg_template/temp_broadcast
        ${HCCL_BASE_DIR}/algorithm/base/alg_template/temp_reduce
        ${HCCL_BASE_DIR}/algorithm/base/alg_template/temp_reduce_scatter
        ${HCCL_BASE_DIR}/algorithm/base/alg_template/temp_scatter
        ${HCCL_BASE_DIR}/algorithm/base/alg_template
        ${HCCL_BASE_DIR}/algorithm/pub_inc
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

        ${ASCEND_CANN_PACKAGE_PATH}/devlib/device/ # c_sec、mmpa、unified_dlog动态库搜索路径

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

        # runtime头文件
        ${ASCEND_CANN_PACKAGE_PATH}/include/

        # mmpa头文件
        ${ASCEND_CANN_PACKAGE_PATH}/include/acl/
        ${ASCEND_CANN_PACKAGE_PATH}/pkg_inc/mmpa/

        # 包间接口
        ${ASCEND_CANN_PACKAGE_PATH}/pkg_inc/
        ${ASCEND_CANN_PACKAGE_PATH}/pkg_inc/aicpu/
        ${ASCEND_CANN_PACKAGE_PATH}/pkg_inc/aicpu/common/
        ${ASCEND_CANN_PACKAGE_PATH}/pkg_inc/runtime/
        ${ASCEND_CANN_PACKAGE_PATH}/pkg_inc/runtime/runtime
        ${ASCEND_CANN_PACKAGE_PATH}/pkg_inc/profiling/
        ${ASCEND_CANN_PACKAGE_PATH}/pkg_inc/base/
        ${ASCEND_CANN_PACKAGE_PATH}/pkg_inc/dump/
        ${ASCEND_CANN_PACKAGE_PATH}/pkg_inc/trace/

        # driver头文件
        ${ASCEND_CANN_PACKAGE_PATH}/include/driver

        # ascendc
        ${ASCEND_CANN_PACKAGE_PATH}/include/ascendc/highlevel_api/
        ${ASCEND_CANN_PACKAGE_PATH}/pkg_inc/asc/hccl/internal/
        ${ASCEND_CANN_PACKAGE_PATH}/include/ascendc/

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


    function(add_mc2_server_ccl_object target_name)
        if(TARGET mc2_server)
            add_library(${target_name} OBJECT ${ARGN})
            set_target_properties(${target_name} PROPERTIES POSITION_INDEPENDENT_CODE ON)
            target_compile_definitions(${target_name} PRIVATE
                CCL_KERNEL_AICPU
                MC2_SERVER_ONLY
            )
            target_include_directories(${target_name} PRIVATE
                ${CCL_KERNEL_INCLUDE_LIST}
                ${ORION_HEAD_LIST}
            )
            if(TARGET json)
                target_link_libraries(${target_name} PRIVATE json)
                add_dependencies(${target_name} json)
            endif()
            target_sources(mc2_server PRIVATE $<TARGET_OBJECTS:${target_name}>)
            add_dependencies(mc2_server ${target_name})
        endif()
    endfunction()

    if(TARGET mc2_server)
        set(MC2_SERVER_CCL_KERNEL_STUB_SRC
            ${CMAKE_CURRENT_BINARY_DIR}/mc2_server_ccl_kernel_stub.cc)
        file(WRITE ${MC2_SERVER_CCL_KERNEL_STUB_SRC}
            "extern \"C\" void __mc2_ccl_kernel_stub(void) {}\n")
        add_library(mc2_server_ccl_kernel_stub SHARED
            ${MC2_SERVER_CCL_KERNEL_STUB_SRC})
        set_target_properties(mc2_server_ccl_kernel_stub PROPERTIES
            OUTPUT_NAME ccl_kernel
            LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/mc2_server_link_stub)
        target_link_options(mc2_server_ccl_kernel_stub PRIVATE
            -Wl,-soname,libccl_kernel.so)

        target_include_directories(mc2_server PRIVATE
            ${CCL_KERNEL_INCLUDE_LIST}
            ${ORION_HEAD_LIST}
        )
        target_link_directories(mc2_server PRIVATE
            ${HCCL_DEVICE_LIB_DIRS}
        )
        target_link_libraries(mc2_server PRIVATE
            -Wl,--no-as-needed
            ${ASCEND_CANN_PACKAGE_PATH}/devlib/device/libascend_hal.so
            mc2_server_ccl_kernel_stub
            ${HCCL_C_SEC_LINK}
            ${HCCL_UNIFIED_DLOG_LINK}
            ccl_kernel_plf
            ccl_kernel_plf_a
            ${HCCL_MMPA_LINK}
            -Wl,--as-needed
            -lrt
            -ldl
            -lpthread
        )
        if(TARGET json)
            target_link_libraries(mc2_server PRIVATE json)
            add_dependencies(mc2_server json)
        endif()
    endif()

    add_subdirectory(device)

endif()

add_subdirectory(${HCCL_BASE_DIR}/legacy ${CMAKE_CURRENT_BINARY_DIR}/legacy)

if(NOT BUILD_OPEN_PROJECT)


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

endif()
add_subdirectory(communicator)
add_subdirectory(next)

if(BUILD_OPEN_PROJECT AND KERNEL_MODE)
    if(BUILD_OPEN_PROJECT)
        message(STATUS "framework CMAKE_HOST_SYSTEM_PROCESSOR=${CMAKE_HOST_SYSTEM_PROCESSOR}")
        if(CMAKE_HOST_SYSTEM_PROCESSOR MATCHES "x86_64")
            message(STATUS "Host is x86_64")
            set(ASCEND_CANN_PACKAGE_ENV_PATH ${ASCEND_CANN_PACKAGE_PATH}/x86_64-linux)
        elseif(CMAKE_HOST_SYSTEM_PROCESSOR MATCHES "aarch64")
            message(STATUS "Host is ARM64")
            set(ASCEND_CANN_PACKAGE_ENV_PATH ${ASCEND_CANN_PACKAGE_PATH}/aarch64-linux)
        endif()
        message(STATUS "framework ASCEND_CANN_PACKAGE_ENV_PATH=${ASCEND_CANN_PACKAGE_ENV_PATH}")

        add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/libaicpu_custom.json
            COMMAND ${HI_PYTHON} ${HCCL_CC_DIR}/cmake/scripts/parser_ini.py ${HCCL_BASE_DIR}/../src/framework/device/framework/aicpu_custom.ini ${CMAKE_CURRENT_BINARY_DIR}/libaicpu_custom.json
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        )
        add_custom_target(aicpu_custom_json DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/libaicpu_custom.json)
        install(FILES ${CMAKE_CURRENT_BINARY_DIR}/libaicpu_custom.json
            DESTINATION ${INSTALL_CCL_KERNEL_JSON_DIR} OPTIONAL
        )
    endif()
endif()
