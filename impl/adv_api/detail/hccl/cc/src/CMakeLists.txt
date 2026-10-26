# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ----------------------------------------------------------------------------------------------------------
set(OPS_BASE_DIR ${HCCL_CC_DIR})
option(MC2_CLIENT_ENABLE_CCU "Build CCU algorithm implementations into mc2_client" ON)

# asccomm 头路径（T1临接）：host 进程经 impl/adv_api 的 asc-comm.cmake 取得 ASC_COMM_SOURCE_PATH；
# device 是独立 cmake 进程，在 cmake/device 入口按 devkit 根相对探测 asc-comm 后展开
# ASCCOMM_PRIMITIVES_HDR（见 cmake/device/CMakeLists.txt）；本地 standalone 构建亦可经
# EXT_ARGS 显式传入。两者皆空时回落 ASCCOMM_PRIMITIVES_INCLUDE_DIR 环境变量（开发侧本地构建用）。
if(ASCCOMM_PRIMITIVES_HDR)
    # device 入口已展开，直接用
elseif(ASC_COMM_SOURCE_PATH)
    set(ASCCOMM_PRIMITIVES_HDR "${ASC_COMM_SOURCE_PATH}/include/aicpu/hcomm")
else()
    set(ASCCOMM_PRIMITIVES_HDR "$ENV{ASCCOMM_PRIMITIVES_INCLUDE_DIR}")
endif()

# 工单 60 附录 A：编排原语（asccomm_addr_notify_*）收敛为内部头，仅框架消费者可见。
# 私有头位于 <asc-comm>/src/aicpu/asccomm/primitives/api_c_adpt/，在 ASCCOMM_PRIMITIVES_HDR
# 基础上加第二路径；消费方（mc2_server_decoupler 编入 comm_kfc_aicpu_server.cc）需定义
# ASCCOMM_ENABLE_INTERNAL_APIS 宏，否则私有头内声明不可见（编译期即报缺声明）。
if(ASCCOMM_PRIMITIVES_HDR)
    get_filename_component(_ASCCOMM_SOURCE_DIR "${ASCCOMM_PRIMITIVES_HDR}/../../.." ABSOLUTE)
    set(ASCCOMM_PRIMITIVES_INTERNAL_HDR "${_ASCCOMM_SOURCE_DIR}/src/aicpu/asccomm/primitives/api_c_adpt")
else()
    set(ASCCOMM_PRIMITIVES_INTERNAL_HDR "")
endif()

set(INCLUDE_LIST
    ${HCCL_CC_INCLUDE_DIR}
    ${ASCENDC_DIR}
    ${HCCL_CC_INCLUDE_DIR}/hcomm
    ${HCCL_CC_INCLUDE_DIR}/hcomm/types
    ${HCCL_CC_INCLUDE_DIR}/hcomm/hccl
    ${HCCL_CC_INCLUDE_DIR}/hcomm/pkg_inc/hccl
    ${HCCL_CC_INCLUDE_DIR}/hcomm/ccu
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
    ${CMAKE_CURRENT_SOURCE_DIR}/ops/op_common/external_alg

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
    ${CMAKE_CURRENT_SOURCE_DIR}/ops/reduce_scatter/template/ccu
    ${CMAKE_CURRENT_SOURCE_DIR}/ops/reduce_scatter/template/ccu/kernel

    ${CMAKE_CURRENT_SOURCE_DIR}/ops/all_gather
    ${CMAKE_CURRENT_SOURCE_DIR}/ops/all_gather/executor
    ${CMAKE_CURRENT_SOURCE_DIR}/ops/all_gather/selector
    ${CMAKE_CURRENT_SOURCE_DIR}/ops/all_gather/template
    ${CMAKE_CURRENT_SOURCE_DIR}/ops/all_gather/template/aicpu
    ${CMAKE_CURRENT_SOURCE_DIR}/ops/all_gather/template/ccu
    ${CMAKE_CURRENT_SOURCE_DIR}/ops/all_gather/template/ccu/kernel

    ${CMAKE_CURRENT_SOURCE_DIR}/ops/all_to_all_v
    ${CMAKE_CURRENT_SOURCE_DIR}/ops/all_to_all_v/executor
    ${CMAKE_CURRENT_SOURCE_DIR}/ops/all_to_all_v/selector
    ${CMAKE_CURRENT_SOURCE_DIR}/ops/all_to_all_v/template
    ${CMAKE_CURRENT_SOURCE_DIR}/ops/all_to_all_v/template/ccu
    ${CMAKE_CURRENT_SOURCE_DIR}/ops/all_to_all_v/template/ccu/kernel
    ${CMAKE_CURRENT_SOURCE_DIR}/ops/all_to_all_v/template/aicpu
    ${CMAKE_CURRENT_SOURCE_DIR}/ops/all_to_all_v/template/ccu
    ${CMAKE_CURRENT_SOURCE_DIR}/ops/all_to_all_v/template/ccu/kernel

    ${CMAKE_CURRENT_SOURCE_DIR}/ops/all_reduce
    ${CMAKE_CURRENT_SOURCE_DIR}/ops/all_reduce/executor
    ${CMAKE_CURRENT_SOURCE_DIR}/ops/all_reduce/selector
    ${CMAKE_CURRENT_SOURCE_DIR}/ops/all_reduce/template
    ${CMAKE_CURRENT_SOURCE_DIR}/ops/all_reduce/template/aicpu
    ${CMAKE_CURRENT_SOURCE_DIR}/ops/all_reduce/template/ccu
    ${CMAKE_CURRENT_SOURCE_DIR}/ops/all_reduce/template/ccu/kernel

    ${CMAKE_CURRENT_SOURCE_DIR}/ops/kfc_server
    ${CMAKE_CURRENT_SOURCE_DIR}/ops/kfc_server/executor
    ${CMAKE_CURRENT_SOURCE_DIR}/ops/kfc_server/selector
    ${CMAKE_CURRENT_SOURCE_DIR}/ops/kfc_server/template
    ${CMAKE_CURRENT_SOURCE_DIR}/ops/kfc_server/template/ccu
    ${CMAKE_CURRENT_SOURCE_DIR}/ops/kfc_server/template/ccu/kernel
)

list(APPEND INCLUDE_LIST
    ${ASCEND_CANN_PACKAGE_PATH}/include
    ${ASCEND_CANN_PACKAGE_PATH}/include/acl

    # hcomm头文件
    ${ASCEND_CANN_PACKAGE_PATH}/include/ccu
    ${ASCEND_CANN_PACKAGE_PATH}/include/hccl
    ${ASCEND_CANN_PACKAGE_PATH}/include/hcomm
    ${ASCEND_CANN_PACKAGE_PATH}/include/hcomm/ccu

    # runtime头文件
    ${ASCEND_CANN_PACKAGE_PATH}/include/base/
    ${ASCEND_CANN_PACKAGE_PATH}/include/driver/
    ${ASCEND_CANN_PACKAGE_PATH}/include/dump/
    ${ASCEND_CANN_PACKAGE_PATH}/include/external/
    ${ASCEND_CANN_PACKAGE_PATH}/include/platform/

    # mmpa头文件
    ${ASCEND_CANN_PACKAGE_PATH}/pkg_inc/mmpa/

    # 包间接口
    ${ASCEND_CANN_PACKAGE_PATH}/pkg_inc/
    ${ASCEND_CANN_PACKAGE_PATH}/pkg_inc/hccl
    ${ASCEND_CANN_PACKAGE_PATH}/pkg_inc/hcomm/ccu
    ${ASCEND_CANN_PACKAGE_PATH}/pkg_inc/runtime/
    ${ASCEND_CANN_PACKAGE_PATH}/pkg_inc/aicpu/
    ${ASCEND_CANN_PACKAGE_PATH}/pkg_inc/runtime/runtime/
    ${ASCEND_CANN_PACKAGE_PATH}/pkg_inc/profiling/
    ${ASCEND_CANN_PACKAGE_PATH}/pkg_inc/base/
    ${ASCEND_CANN_PACKAGE_PATH}/pkg_inc/dump/
    ${ASCEND_CANN_PACKAGE_PATH}/pkg_inc/trace/
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

        ${CMAKE_CURRENT_SOURCE_DIR}/ops/op_common/executor/channel/channel.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ops/op_common/executor/channel/channel_request.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ops/op_common/executor/registry/coll_alg_exec_registry.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ops/op_common/executor/registry/coll_alg_v2_exec_registry.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ops/op_common/external_alg/alg_meta_registry.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ops/op_common/executor/executor_base.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ops/op_common/executor/executor_v2_base.cc

        ${CMAKE_CURRENT_SOURCE_DIR}/ops/op_common/template/alg_template_base.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ops/op_common/template/alg_v2_template_base.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ops/op_common/template/template_utils.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ops/op_common/template/aicpu/kernel_launch.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ops/op_common/template/aicpu/task_cache/mc2_aicpu_task_cache.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ops/op_common/template/aicpu/dfx/task_exception_fun.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ops/op_common/template/registry/alg_template_register.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ops/op_common/template/wrapper/alg_data_trans_wrapper.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ops/op_common/template/wrapper/dpu_alg_data_trans_wrapper.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ops/op_common/omnipipe_data_slice_calc.cc


        ${CMAKE_CURRENT_SOURCE_DIR}/ops/op_common/topo/topo.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ops/op_common/topo/topo_match_1d.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ops/op_common/topo/topo_match_base.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ops/op_common/topo/topo_match_multilevel.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ops/op_common/topo/topo_match_ubx.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ops/op_common/topo/topo_match_ubx_1d.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ops/op_common/topo/topo_match_pcie_mix.cc

        ${CMAKE_CURRENT_SOURCE_DIR}/ops/reduce_scatter/executor/ins_v2_reduce_scatter_sole_executor.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ops/reduce_scatter/executor/ins_reduce_scatter_parallel_executor.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ops/reduce_scatter/executor/ins_reduce_scatter_concurrent_executor.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ops/reduce_scatter/executor/ins_v2_reduce_scatter_omnipipe_executor.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ops/reduce_scatter/executor/ins_v2_reduce_scatter_sequence_executor_aicpu.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ops/reduce_scatter/executor/ins_v2_reduce_scatter_sequence_executor.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ops/reduce_scatter/template/aicpu/ins_temp_reduce_scatter_mesh_1D.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ops/reduce_scatter/template/aicpu/ins_temp_reduce_scatter_nhr.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ops/reduce_scatter/template/aicpu/ins_temp_reduce_scatter_mesh_1D_meshchunk.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ops/reduce_scatter/template/aicpu/ins_temp_reduce_scatter_mesh_1d_dpu.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ops/reduce_scatter/template/aicpu/ins_temp_reduce_scatter_aicpu_reduce_nhr.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ops/reduce_scatter/template/aicpu/ins_temp_reduce_scatter_mesh_1D_Z_axis_detour.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ops/reduce_scatter/template/aicpu/ins_temp_reduce_scatter_omnipipe_mesh_1D.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ops/reduce_scatter/template/aicpu/ins_temp_reduce_scatter_omnipipe_nhr.cc

        ${CMAKE_CURRENT_SOURCE_DIR}/ops/all_gather/executor/ins_v2_all_gather_sole_executor.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ops/all_gather/executor/ins_v2_all_gather_parallel_executor.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ops/all_gather/executor/ins_v2_all_gather_concurrent_executor.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ops/all_gather/executor/ins_v2_all_gather_omnipipe_executor.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ops/all_gather/template/aicpu/ins_temp_all_gather_mesh_1D.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ops/all_gather/template/aicpu/ins_temp_all_gather_mesh_1D_Z_axis_detour.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ops/all_gather/template/aicpu/ins_temp_all_gather_nhr.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ops/all_gather/executor/ins_v2_all_gather_sequence_executor.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ops/all_gather/executor/ins_v2_all_gather_sequence_executor_aicpu.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ops/all_gather/template/aicpu/ins_temp_all_gather_nhr_dpu.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ops/all_gather/template/aicpu/ins_temp_all_gather_omnipipe_mesh_1D.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ops/all_gather/template/aicpu/ins_temp_all_gather_omnipipe_nhr.cc

        ${CMAKE_CURRENT_SOURCE_DIR}/ops/all_to_all_v/executor/ins_v2_all_to_all_v_sole_executor.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ops/all_to_all_v/executor/ins_v2_all_to_all_concurrent_executor.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ops/all_to_all_v/template/aicpu/ins_temp_all_to_all_v_mesh_1D.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ops/all_to_all_v/template/aicpu/ins_temp_ubx_all_to_all_v_mesh_1D.cc

        ${CMAKE_CURRENT_SOURCE_DIR}/ops/all_reduce/executor/ins_v2_all_reduce_sole_executor.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ops/all_reduce/executor/ins_v2_all_reduce_parallel_executor.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ops/all_reduce/executor/ins_v2_all_reduce_omnipipe_executor.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ops/all_reduce/template/aicpu/ins_temp_all_reduce_mesh_1D_two_shot.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ops/all_reduce/template/aicpu/ins_temp_all_reduce_mesh_1D_one_shot.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ops/all_reduce/template/aicpu/ins_temp_all_reduce_mesh_1D_two_shot_mesh_chunk.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ops/all_reduce/template/aicpu/ins_temp_all_reduce_aicpu_reduce_nhr.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/ops/all_reduce/template/aicpu/ins_temp_all_reduce_nhr.cc

        ${CMAKE_CURRENT_SOURCE_DIR}/ops/op_common/template/registry/alg_v2_template_register.cc
    )

    target_include_directories(mc2_server PRIVATE
        ${INCLUDE_LIST}
        # TODO(T1临接): device 侧与 mc2_client 编译相同的已切 asccomm 源文件，头路径经 ASCCOMM_PRIMITIVES_HDR（见文件头）
        ${ASCCOMM_PRIMITIVES_HDR}
        # 工单 60 附录 A：编排原语私有头目录（mc2_server_decoupler 编入 comm_kfc_aicpu_server.cc 消费）
        ${ASCCOMM_PRIMITIVES_INTERNAL_HDR}
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
        ASCCOMM_ENABLE_INTERNAL_APIS
    )

    target_link_libraries(mc2_server PRIVATE
        -Wl,--no-as-needed
        mc2_kernel_compat
        mmpa
        $<BUILD_INTERFACE:acl_rt_headers>
        $<BUILD_INTERFACE:error_manager_headers>
    )
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
        # TODO(T1临接): 正式依赖治理走 FetchContent TAG_ID bump（Q12），此路径穿刺期联调用
        ${ASCCOMM_PRIMITIVES_HDR}
        # 工单 60 附录 A：编排原语私有头目录
        ${ASCCOMM_PRIMITIVES_INTERNAL_HDR}
    )

    target_compile_definitions(mc2_client PRIVATE
        -DHOST_COMPILE
        MC2_CLIENT_ENABLE_CCU=$<BOOL:${MC2_CLIENT_ENABLE_CCU}>
        ASCCOMM_ENABLE_INTERNAL_APIS
    )

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

        # asccomm 提供者登记（与 device mc2_server 同机制）：mc2_client 数据面已切到
        # asccomm_*，这些是对 libasccomm_aicpu.so 的 loose-U。host 侧此前靠
        # LD_PRELOAD libasccomm_aicpu.so 兜底；登记进 NEEDED 后由运行期解析提供者
        # （与 server 一致），消灭 preload 依赖。asc-comm.cmake 只 build asccomm_ccu，
        # 无 asccomm_aicpu target 可链，故用空壳桩登记 soname（真身由环境/runtime 供给）。
        if(NOT TARGET mc2_client_asccomm_stub)
            set(MC2_CLIENT_ASCCOMM_STUB_SRC
                ${CMAKE_CURRENT_BINARY_DIR}/mc2_client_asccomm_stub.cc)
            file(WRITE ${MC2_CLIENT_ASCCOMM_STUB_SRC}
                "extern \"C\" void __mc2_client_asccomm_stub(void) {}\n")
            add_library(mc2_client_asccomm_stub SHARED
                ${MC2_CLIENT_ASCCOMM_STUB_SRC})
            set_target_properties(mc2_client_asccomm_stub PROPERTIES
                OUTPUT_NAME asccomm_aicpu
                LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/mc2_client_link_stub)
            target_link_options(mc2_client_asccomm_stub PRIVATE
                -Wl,-soname,libasccomm_aicpu.so)
        endif()

        target_link_libraries(mc2_client PRIVATE
            -Wl,--no-as-needed
            mc2_client_hcomm_stub
            mc2_client_asccomm_stub
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
