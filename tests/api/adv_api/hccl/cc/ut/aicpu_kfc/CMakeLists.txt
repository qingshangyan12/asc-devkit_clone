cmake_minimum_required(VERSION 3.16.0)
option(BUILD_OPEN_PROJECT "Build open hcomm project." ON)
message(STATUS "DEVICE_MODE=${DEVICE_MODE}")
message(STATUS "BUILD_OPEN_PROJECT=${BUILD_OPEN_PROJECT}")
message(STATUS "AARCH_MODE=${AARCH_MODE}")

find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
    set(CMAKE_C_COMPILER_LAUNCHER ${CCACHE_PROGRAM})
    set(CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE_PROGRAM})
endif()

if((BUILD_OPEN_PROJECT AND DEVICE_MODE) OR AARCH_MODE)
    # need to switch
    set(CMAKE_SYSTEM_NAME Linux)
    set(CMAKE_SYSTEM_PROCESSOR aarch64)
    #Change the path to the absolute path of the cross compilation tool after the toolkit package is decompressed
    # set cross compile toolchain dir
    set(TOOLCHAIN_DIR ${CUSTOM_ASCEND_CANN_PACKAGE_PATH}/toolkit/toolchain/hcc/)
    message(STATUS "TOOLCHAIN_DIR: ${TOOLCHAIN_DIR}")
    if (NOT "x${TOOLCHAIN_DIR}" STREQUAL "x")
        if (NOT IS_DIRECTORY ${TOOLCHAIN_DIR})
            message(FATAL_ERROR "specify cross compile toolchain directory(${TOOLCHAIN_DIR}) is not exist")
        endif()
    endif()
    # endif()
    set(CMAKE_CXX_COMPILER ${TOOLCHAIN_DIR}/bin/aarch64-target-linux-gnu-g++)
    set(CMAKE_C_COMPILER   ${TOOLCHAIN_DIR}/bin/aarch64-target-linux-gnu-gcc)
endif()

# 获取host架构
execute_process(
    COMMAND uname -m
    OUTPUT_VARIABLE SYSTEM_ARCH
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
message(STATUS "当前系统架构: ${SYSTEM_ARCH}")
project(hcomm)

set(CMAKE_C_FLAGS_RELEASE "")
set(CMAKE_CXX_FLAGS_RELEASE "")

set(CMAKE_CXX_COMPILE_OBJECT
    "<CMAKE_CXX_COMPILER> <DEFINES> -D__FILE__='\"$(notdir $(abspath <SOURCE>))\"' -Wno-builtin-macro-redefined <INCLUDES> <FLAGS> -o <OBJECT> -c <SOURCE>"
)
set(CMAKE_C_COMPILE_OBJECT
    "<CMAKE_C_COMPILER> <DEFINES> -D__FILE__='\"$(notdir $(abspath <SOURCE>))\"' -Wno-builtin-macro-redefined <INCLUDES> <FLAGS> -o <OBJECT> -c <SOURCE>"
)

# 获取 CPU 核心数
if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    execute_process(
        COMMAND nproc
        OUTPUT_VARIABLE CPU_CORES
        OUTPUT_STRIP_TRAILING_WHITESPACE
    ) 
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    execute_process(
        COMMAND wmic cpu get NumberOfCores /value
        OUTPUT_VARIABLE CPU_CORES
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    # 解析输出
    string(REGEX REPLACE "NumberOfCores=([0-9]+)" "\\1" CPU_CORES "${CPU_CORES}")
else()
    # 默认设置为 1
    set(CPU_CORES 1)
endif()

math(EXPR JOBS "${CPU_CORES} * 2")
message(STATUS "Detected JOBS: ${JOBS}")
set(CMAKE_JOB_POOL_COMPILE "${JOBS}")
set(CMAKE_JOB_POOL_LINK "${JOBS}")

if(ENABLE_TEST)
    enable_testing()
    include(cmake/config.cmake)
    # include(cmake/function.cmake)
    include(cmake/intf_pub_gccnative.cmake)
    include(cmake/third_party/json.cmake)
    include(cmake/third_party/gtest.cmake)
    include(cmake/third_party/mockcpp.cmake)
    # include(cmake/third_party/rdma-core.cmake)
    # add_subdirectory(test)
    # return()

set(HCCL_CODE_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/../../../../../../../impl/adv_api/detail/hccl/cc/src/aicpu_kfc)
set(HCCL_HCOMM_HEADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../../../../../../include/adv_api/hccl/internal/hcomm)
set(HCCL_LLT_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

set(UT_COMMON_INCLUDE_LIST
    ${HCCL_HCOMM_HEADER_DIR}
    ${HCCL_HCOMM_HEADER_DIR}/hccl
    ${HCCL_HCOMM_HEADER_DIR}/pkg_inc
    ${HCCL_HCOMM_HEADER_DIR}/pkg_inc/hccl
    ${HCCL_HCOMM_HEADER_DIR}/pkg_inc/hcomm/ccu

    # src/pub_inc 目录下头文件
    ${HCCL_CODE_ROOT}/pub_inc
    ${HCCL_CODE_ROOT}/pub_inc/inner
    ${HCCL_CODE_ROOT}/pub_inc/aicpu
    ${HCCL_CODE_ROOT}/pub_inc/new

    # src/hccd 目录下头文件
    ${HCCL_CODE_ROOT}/hccd

    # src/common 目录下头文件
    ${HCCL_CODE_ROOT}/common/health
    ${HCCL_CODE_ROOT}/common/stream
    ${HCCL_CODE_ROOT}/common/launch_device
    ${HCCL_CODE_ROOT}/common/src
    ${HCCL_CODE_ROOT}/common/src/host
    ${HCCL_CODE_ROOT}/common/src/topo
    ${HCCL_CODE_ROOT}/common/debug/config
    ${HCCL_CODE_ROOT}/common/debug/profiling
    ${HCCL_CODE_ROOT}/common/debug/profiling/inc
    ${HCCL_CODE_ROOT}/common/debug/profiling/inc/host

    # src/framework 目录下头文件
    ${HCCL_CODE_ROOT}/framework/
    ${HCCL_CODE_ROOT}/framework/inc/
    ${HCCL_CODE_ROOT}/framework/inc/host
    ${HCCL_CODE_ROOT}/framework/cluster_maintenance/health/heartbeat/
    ${HCCL_CODE_ROOT}/framework/cluster_maintenance/detect/detect_connect_anomalies/
    ${HCCL_CODE_ROOT}/framework/cluster_maintenance/recovery/operator_retry
    ${HCCL_CODE_ROOT}/framework/cluster_maintenance/snapshot
    ${HCCL_CODE_ROOT}/framework/cluster_maintenance/aclgraph
    ${HCCL_CODE_ROOT}/framework/common/src
    ${HCCL_CODE_ROOT}/framework/common/src/host
    ${HCCL_CODE_ROOT}/framework/common/src/task
    ${HCCL_CODE_ROOT}/framework/common/src/topo
    ${HCCL_CODE_ROOT}/framework/common/src/exception
    ${HCCL_CODE_ROOT}/framework/common/src/config
    ${HCCL_CODE_ROOT}/framework/common/src/onesided_memory_management
    ${HCCL_CODE_ROOT}/framework/common/src/order_launch
    ${HCCL_CODE_ROOT}/framework/communicator/impl
    ${HCCL_CODE_ROOT}/framework/communicator/impl/zero_copy
    ${HCCL_CODE_ROOT}/framework/communicator/impl/one_sided_service
    ${HCCL_CODE_ROOT}/framework/communicator/impl/resource_manager
    ${HCCL_CODE_ROOT}/framework/communicator/impl/independent_op
    ${HCCL_CODE_ROOT}/framework/communicator/impl/independent_op/channel/
    ${HCCL_CODE_ROOT}/framework/communicator/impl/independent_op/channel/device
    ${HCCL_CODE_ROOT}/framework/communicator/impl/independent_op/rank_graph
    ${HCCL_CODE_ROOT}/framework/communicator/impl/independent_op/resource/engine
    ${HCCL_CODE_ROOT}/framework/hcom/
    ${HCCL_CODE_ROOT}/framework/hcom/gradient_segment
    ${HCCL_CODE_ROOT}/framework/op_base/src/
    ${HCCL_CODE_ROOT}/framework/group
    ${HCCL_CODE_ROOT}/framework/nslbdp/
    ${HCCL_CODE_ROOT}/framework/next/coll_comms/api_c_adpt

    # src/platform 目录下头文件
    ${HCCL_CODE_ROOT}/platform/
    ${HCCL_CODE_ROOT}/platform/common/
    ${HCCL_CODE_ROOT}/platform/common/unique/
    ${HCCL_CODE_ROOT}/platform/common/buffer_manager
    ${HCCL_CODE_ROOT}/platform/common/unfold_cache
    ${HCCL_CODE_ROOT}/platform/ping_mesh/
    ${HCCL_CODE_ROOT}/platform/inc/
    ${HCCL_CODE_ROOT}/platform/inc/adapter
    ${HCCL_CODE_ROOT}/platform/inc/adapter/host
    ${HCCL_CODE_ROOT}/platform/resource/
    ${HCCL_CODE_ROOT}/platform/resource/mem
    ${HCCL_CODE_ROOT}/platform/resource/notify/
    ${HCCL_CODE_ROOT}/platform/resource/rma_buffer
    ${HCCL_CODE_ROOT}/platform/resource/transport/
    ${HCCL_CODE_ROOT}/platform/resource/transport/device/
    ${HCCL_CODE_ROOT}/platform/resource/transport/host/
    ${HCCL_CODE_ROOT}/platform/resource/transport/heterog/
    ${HCCL_CODE_ROOT}/platform/resource/transport/onesided
    ${HCCL_CODE_ROOT}/platform/resource/transport/onesided/device
    ${HCCL_CODE_ROOT}/platform/resource/socket/
    ${HCCL_CODE_ROOT}/platform/resource/dispatcher_ctx
    ${HCCL_CODE_ROOT}/platform/task/
    ${HCCL_CODE_ROOT}/platform/aiv_communication/
    ${HCCL_CODE_ROOT}/platform/debug/
    ${HCCL_CODE_ROOT}/platform/debug/dfx/
    ${HCCL_CODE_ROOT}/platform/debug/dfx/trace/
    ${HCCL_CODE_ROOT}/platform/task/
    ${HCCL_CODE_ROOT}/platform/task/rtsq_interact/
    ${HCCL_CODE_ROOT}/platform/typical
    ${HCCL_CODE_ROOT}/platform/hccp/inc
    ${HCCL_CODE_ROOT}/platform/hccp/inc/network
    ${RDMA_CORE_INCLUDE_DIR}

    # hccl/algorithm 目录下头文件
    ${HCCL_CODE_ROOT}/algorithm/pub_inc
    ${HCCL_CODE_ROOT}/algorithm/base
    ${HCCL_CODE_ROOT}/algorithm/base/inc
    ${HCCL_CODE_ROOT}/algorithm/base/alg_template
    ${HCCL_CODE_ROOT}/algorithm/base/alg_template/temp_all_gather
    ${HCCL_CODE_ROOT}/algorithm/base/alg_template/temp_all_reduce
    ${HCCL_CODE_ROOT}/algorithm/base/alg_template/temp_alltoall
    ${HCCL_CODE_ROOT}/algorithm/base/alg_template/temp_alltoallv
    ${HCCL_CODE_ROOT}/algorithm/base/alg_template/temp_broadcast
    ${HCCL_CODE_ROOT}/algorithm/base/alg_template/temp_gather
    ${HCCL_CODE_ROOT}/algorithm/base/alg_template/temp_reduce
    ${HCCL_CODE_ROOT}/algorithm/base/alg_template/temp_reduce_scatter
    ${HCCL_CODE_ROOT}/algorithm/base/alg_template/temp_scatter
    ${HCCL_CODE_ROOT}/algorithm/base/alg_template/temp_send_recv
    ${HCCL_CODE_ROOT}/algorithm/base/alg_template/component
    ${HCCL_CODE_ROOT}/algorithm/base/alg_template/component/inc
    ${HCCL_CODE_ROOT}/algorithm/base/alg_template/inc_all_reduce_deter
    ${HCCL_CODE_ROOT}/algorithm/base/communicator
    ${HCCL_CODE_ROOT}/algorithm/base/mc2_handler
    ${HCCL_CODE_ROOT}/algorithm/impl
    ${HCCL_CODE_ROOT}/algorithm/impl/inc
    ${HCCL_CODE_ROOT}/algorithm/impl/legacy
    ${HCCL_CODE_ROOT}/algorithm/impl/legacy/operator
    ${HCCL_CODE_ROOT}/algorithm/base/communicator/legacy
    ${HCCL_CODE_ROOT}/algorithm/impl/task
    ${HCCL_CODE_ROOT}/algorithm/impl/operator
    ${HCCL_CODE_ROOT}/algorithm/impl/operator/registry
    ${HCCL_CODE_ROOT}/algorithm/impl/resource_manager
    ${HCCL_CODE_ROOT}/algorithm/impl/coll_executor
    ${HCCL_CODE_ROOT}/algorithm/impl/coll_executor/registry
    ${HCCL_CODE_ROOT}/algorithm/impl/coll_executor/coll_send_receive
    ${HCCL_CODE_ROOT}/algorithm/impl/coll_executor/coll_all_reduce
    ${HCCL_CODE_ROOT}/algorithm/impl/coll_executor/coll_all_reduce/310P
    ${HCCL_CODE_ROOT}/algorithm/impl/coll_executor/coll_all_to_all
    ${HCCL_CODE_ROOT}/algorithm/impl/coll_executor/coll_all_gather
    ${HCCL_CODE_ROOT}/algorithm/impl/coll_executor/coll_all_gather/310P
    ${HCCL_CODE_ROOT}/algorithm/impl/coll_executor/coll_all_gather_v
    ${HCCL_CODE_ROOT}/algorithm/impl/coll_executor/coll_all_gather_v/310P
    ${HCCL_CODE_ROOT}/algorithm/impl/coll_executor/coll_reduce_scatter
    ${HCCL_CODE_ROOT}/algorithm/impl/coll_executor//coll_reduce_scatter/310P
    ${HCCL_CODE_ROOT}/algorithm/impl/coll_executor/coll_reduce_scatter_v
    ${HCCL_CODE_ROOT}/algorithm/impl/coll_executor/coll_reduce_scatter_v/310P
    ${HCCL_CODE_ROOT}/algorithm/impl/coll_executor/coll_reduce
    ${HCCL_CODE_ROOT}/algorithm/impl/coll_executor/coll_scatter
    ${HCCL_CODE_ROOT}/algorithm/impl/coll_executor/coll_broadcast
    ${HCCL_CODE_ROOT}/algorithm/impl/coll_executor/coll_broadcast/310P

    ${HCCL_CODE_ROOT}/test/hccl_api_single_thread_test

    # hccl/operator目录下头文件
    ${HCCL_CODE_ROOT}/operator

    # proto.h头文件
    ${CMAKE_BINARY_DIR}/proto/hccl_llt

    # /src/framework/device/目录下头文件
    ${HCCL_CODE_ROOT}/framework/device/
    ${HCCL_CODE_ROOT}/framework/device/inc
    ${HCCL_CODE_ROOT}/framework/device/aicpu_kfc
    ${HCCL_CODE_ROOT}/framework/device/aicpu_kfc/inc
    ${HCCL_CODE_ROOT}/framework/device/aicpu_kfc/common
    ${HCCL_CODE_ROOT}/framework/device/common
    ${HCCL_CODE_ROOT}/framework/device/utils
    ${HCCL_CODE_ROOT}/framework/device/framework
    ${HCCL_CODE_ROOT}/framework/next/comms/comm_engine_res/threads

    # legacy include
    ${HCCL_CODE_ROOT}/legacy/common/
    ${HCCL_CODE_ROOT}/legacy/common/utils/
    ${HCCL_CODE_ROOT}/legacy/common/exception/
    ${HCCL_CODE_ROOT}/legacy/common/types/
    ${HCCL_CODE_ROOT}/legacy/service/collective/primitive
    ${HCCL_CODE_ROOT}/legacy/framework/
    ${HCCL_CODE_ROOT}/legacy/framework/resource_manager/socket/
    ${HCCL_CODE_ROOT}/legacy/framework/communicator/
    ${HCCL_CODE_ROOT}/legacy/framework/communicator/aicpu/
    ${HCCL_CODE_ROOT}/legacy/framework/communicator/aicpu/daemon
    ${HCCL_CODE_ROOT}/legacy/framework/communicator/aicpu/inc
    ${HCCL_CODE_ROOT}/legacy/framework/communicator/aicpu/daemon
    ${HCCL_CODE_ROOT}/legacy/framework/topo/
    ${HCCL_CODE_ROOT}/legacy/framework/topo/new_topo_builder/common/
    ${HCCL_CODE_ROOT}/legacy/framework/topo/new_topo_builder/rank_graph/
    ${HCCL_CODE_ROOT}/legacy/framework/topo/new_topo_builder/rank_table_info/
    ${HCCL_CODE_ROOT}/legacy/framework/env_config/
    ${HCCL_CODE_ROOT}/legacy/interface/
    ${HCCL_CODE_ROOT}/legacy/service/collective/
    ${HCCL_CODE_ROOT}/legacy/service/collective/primitive/
    ${HCCL_CODE_ROOT}/legacy/unified_platform/external_system/
    ${HCCL_CODE_ROOT}/legacy/unified_platform/resource/
    ${HCCL_CODE_ROOT}/legacy/unified_platform/resource/socket/
    ${HCCL_CODE_ROOT}/legacy/unified_platform/resource/transport/
    ${HCCL_CODE_ROOT}/legacy/unified_platform/resource/transport/aicpu/
    ${HCCL_CODE_ROOT}/legacy/unified_platform/resource/notify/aicpu/
    ${HCCL_CODE_ROOT}/legacy/unified_platform/resource/stream/aicpu/
    ${HCCL_CODE_ROOT}/legacy/unified_platform/resource/buffer/
    ${HCCL_CODE_ROOT}/legacy/unified_platform/resource/buffer/aicpu/
    ${HCCL_CODE_ROOT}/legacy/unified_platform/resource/connection/aicpu/
    ${HCCL_CODE_ROOT}/legacy/unified_platform/resource/connection/
    ${HCCL_CODE_ROOT}/legacy/unified_platform/pub_inc/
    ${HCCL_CODE_ROOT}/legacy/unified_platform/common/
    ${HCCL_CODE_ROOT}/legacy/unified_platform/
    ${HCCL_CODE_ROOT}/legacy/unified_platform/resource/task/
    ${HCCL_CODE_ROOT}/legacy/unified_platform/resource/notify/

    # tbe头文件
    ${HCCL_CODE_ROOT}/platform/legacy/hccl_tbe_task/
    ${HCCL_CODE_ROOT}/platform/legacy/common/
    ${HCCL_CODE_ROOT}/platform/legacy/graph_ctx_mgr/
    ${HCCL_CODE_ROOT}/platform/legacy/inc/

    # hcce头文件
    ${HCCL_CODE_ROOT}/engines/hcclengine/inc

    ${HCCL_LLT_ROOT}/stub
    ${HCCL_LLT_ROOT}/stub/legacy/
    ${HCCL_LLT_ROOT}/depends/pkg_inc/
    ${HCCL_LLT_ROOT}/depends/pkg_inc/aicpu/
    ${HCCL_LLT_ROOT}/depends/pkg_inc/aicpu/common/
    ${HCCL_LLT_ROOT}/depends/pkg_inc/runtime/
    ${HCCL_LLT_ROOT}/depends/pkg_inc/profiling/
    ${HCCL_LLT_ROOT}/depends/pkg_inc/base/
    ${HCCL_LLT_ROOT}/depends/include/base/context_builder
    ${HCCL_LLT_ROOT}/depends/pkg_inc/dump/
    ${HCCL_LLT_ROOT}/depends/pkg_inc/trace/
    ${HCCL_LLT_ROOT}/depends/pkg_inc/asc/include/adv_api/hccl/internal

    ${HCCL_LLT_ROOT}/depends/include/
    ${HCCL_LLT_ROOT}/depends/runtime/include/
    ${HCCL_LLT_ROOT}/depends/include/mmpa/
    ${HCCL_LLT_ROOT}/depends/include/driver/
    ${HCCL_LLT_ROOT}/depends/include/acl/
    ${HCCL_LLT_ROOT}/depends/include/ascendc/highlevel_api/
    ${HCCL_LLT_ROOT}/depends/include/hcomm-legacy/

    ${CANN_3RD_LIB_PATH}/json/include/
    ${CANN_3RD_LIB_PATH}/gtest/include/
    ${CANN_3RD_LIB_PATH}/mockcpp/include/
    ${CANN_3RD_LIB_PATH}/mockcpp/include/3rdparty/
    ${CANN_3RD_LIB_PATH}/boost/

    ${ASCEND_CANN_PACKAGE_PATH}/include/ascendc
    ${ASCEND_CANN_PACKAGE_PATH}/pkg_inc/asc/hccl/internal

    # next
    ${HCCL_CODE_ROOT}/framework/next/coll_comms
    ${HCCL_CODE_ROOT}/framework/next/coll_comms/communicator
    ${HCCL_CODE_ROOT}/framework/next/coll_comms/communicator/aicpu
    ${HCCL_CODE_ROOT}/framework/next/coll_comms/rank/
    ${HCCL_CODE_ROOT}/framework/next/coll_comms/rank_pairs/
    ${HCCL_CODE_ROOT}/framework/next/common/
    ${HCCL_CODE_ROOT}/framework/next/comms/endpoints/
    ${HCCL_CODE_ROOT}/framework/next/comms/endpoints/reged_mems/
    ${HCCL_CODE_ROOT}/framework/next/comms/endpoint_pairs/
    ${HCCL_CODE_ROOT}/framework/next/comms/endpoint_pairs/sockets/
    ${HCCL_CODE_ROOT}/framework/next/comms/endpoint_pairs/channels/host/
    ${HCCL_CODE_ROOT}/framework/next/comms/comm_engine_res/threads
    ${HCCL_CODE_ROOT}/framework/next/comms/endpoint_pairs/channels
    ${HCCL_CODE_ROOT}/framework/next/comms/endpoint_pairs/channels/aicpu/
    ${HCCL_CODE_ROOT}/framework/next/comms/api_c_adpt/
    ${HCCL_CODE_ROOT}/framework/next/coll_comms/communicator/ns_recovery
    ${HCCL_CODE_ROOT}/framework/next/coll_comms/communicator/ns_recovery/aicpu

    ${HCCL_CODE_ROOT}/legacy/interface/
    ${HCCL_CODE_ROOT}/framework/next/coll_comms/dfx
    ${HCCL_CODE_ROOT}/framework/next/coll_comms/dfx/profiling
    ${HCCL_CODE_ROOT}/framework/next/coll_comms/dfx/profiling/aicpu
    ${HCCL_CODE_ROOT}/framework/next/coll_comms/dfx/profiling/host
    ${HCCL_CODE_ROOT}/framework/next/coll_comms/dfx/taskException
    ${HCCL_CODE_ROOT}/framework/next/coll_comms/dfx/taskException/aicpu
    ${HCCL_CODE_ROOT}/framework/next/coll_comms/dfx/taskException/host
    ${HCCL_CODE_ROOT}/framework/device/framework
    ${HCCL_CODE_ROOT}/framework/next/comms/endpoint_pairs/channels/aicpu/device/
    ${HCCL_CODE_ROOT}/framework/dfx/task_exception
    ${HCCL_CODE_ROOT}/legacy/framework/dfx/
    ${HCCL_CODE_ROOT}/legacy/framework/dfx/common
    ${HCCL_CODE_ROOT}/legacy/framework/dfx/task_exception
    ${HCCL_CODE_ROOT}/legacy/framework/dfx/profiling
    ${HCCL_CODE_ROOT}/legacy/framework/dfx/aicpu
    ${HCCL_CODE_ROOT}/legacy/framework/dfx/aicpu/task_exception
    ${HCCL_CODE_ROOT}/legacy/framework/dfx/aicpu/profiling
    ${HCCL_CODE_ROOT}/legacy/framework/dfx/aicpu/common
    ${HCCL_CODE_ROOT}/legacy/framework/resource_manager/stream
    ${HCCL_CODE_ROOT}/legacy/unified_platform/resource/stream
    ${HCCL_CODE_ROOT}/legacy/interface/
    ${HCCL_CODE_ROOT}/legacy/framework/dfx/aicpu/profiling
    ${HCCL_CODE_ROOT}/legacy/unified_platform/ccu/dfx/
    ${HCCL_CODE_ROOT}/legacy/unified_platform/ccu/ccu_representation/reps/common/


    # ccu 头文件
    ${HCCL_CODE_ROOT}/framework/next/comms/adpt/
    ${HCCL_CODE_ROOT}/framework/next/comms/common/

    ${HCCL_CODE_ROOT}/framework/next/comms/ccu/pub_inc/
    ${HCCL_CODE_ROOT}/framework/next/comms/ccu/ccu_device/
    ${HCCL_CODE_ROOT}/framework/next/comms/ccu/ccu_device/ccu_comp/
    ${HCCL_CODE_ROOT}/framework/next/comms/ccu/ccu_device/ccu_comp/ccu_channel/
    ${HCCL_CODE_ROOT}/framework/next/comms/ccu/ccu_device/ccu_comp/ccu_channel/ccu_pfe/
    ${HCCL_CODE_ROOT}/framework/next/comms/ccu/ccu_device/ccu_comp/ccu_channel/ccu_channel_ctx_v1/
    ${HCCL_CODE_ROOT}/framework/next/comms/ccu/ccu_transport/
    ${HCCL_CODE_ROOT}/framework/next/comms/endpoint_pairs/channels/ccu/
    ${HCCL_CODE_ROOT}/framework/next/comms/endpoint_pairs/channels/aiv/
    ${HCCL_CODE_ROOT}/framework/next/comms/ccu/ccu_kernel/
    ${HCCL_CODE_ROOT}/framework/next/comms/ccu/ccu_representation/reps/arithmetic/
)

add_subdirectory(stub)
add_subdirectory(testcase)
