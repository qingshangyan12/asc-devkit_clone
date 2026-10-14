# Copyright (c) 2026 Huawei Technologies Co., Ltd.
# Licensed under the CANN Open Software License Agreement Version 2.0.

target_sources(mc2_client PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/core/program.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/primitives/exchange.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/algorithms/allgather/slices.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/algorithms/allgather/sole_mesh.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/algorithms/allgather/sole_nhr.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/algorithms/allgather/sequence_mesh_nhr.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/algorithms/allgather/concurrent_mesh_nhr.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/algorithms/allgather/parallel_mesh_nhr.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/algorithms/allgather/pipeline_mesh_nhr.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/model/collective_plan.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/model/logical_resources.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/model/execution_route.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/primitives/all_gather_primitives.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/primitives/all_gather_steps.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/engines/aicpu/aicpu_all_gather_executor.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/engines/aiv/aiv_program_builder.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/algorithms/all_gather_algorithms.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/dispatch/kfc_dispatch.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/adapters/ascend/ascend_plan_binding.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/adapters/ascend/aicpu_all_gather_binding.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/adapters/ascend/ccu_all_gather_binding.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/adapters/ascend/dsl_all_gather_sole_templates.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/engines/trace/trace_engine.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/trace/native_binding_trace.cc
)

target_include_directories(mc2_client PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/core
    ${CMAKE_CURRENT_SOURCE_DIR}/algorithms/allgather
    ${CMAKE_CURRENT_SOURCE_DIR}/model
    ${CMAKE_CURRENT_SOURCE_DIR}/primitives
    ${CMAKE_CURRENT_SOURCE_DIR}/algorithms
    ${CMAKE_CURRENT_SOURCE_DIR}/dispatch
    ${CMAKE_CURRENT_SOURCE_DIR}/adapters/ascend
    ${CMAKE_CURRENT_SOURCE_DIR}/engines/trace
    ${CMAKE_CURRENT_SOURCE_DIR}/trace
    ${CMAKE_CURRENT_SOURCE_DIR}/engines/aicpu
    ${CMAKE_CURRENT_SOURCE_DIR}/engines/aiv
    ${CMAKE_CURRENT_SOURCE_DIR}/engines/ccu
    ${CMAKE_CURRENT_SOURCE_DIR}/dispatch/ccu_server
)

if(MC2_CLIENT_ENABLE_CCU)
    target_sources(mc2_client PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/engines/ccu/ccu_all_gather_mesh.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/adapters/ascend/ccu_server_launch.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/dispatch/ccu_server/ccu_all_gather_server.cc)
endif()

# 真正的 Ascend C AIV kernel/fat object。显式目标避免改变原工程打包和 server。
find_program(DSL_BISHENG bisheng HINTS /usr/local/Ascend/cann/bin)
set(DSL_ASC_HOME "/usr/local/Ascend/cann" CACHE PATH "CANN root used to compile DSL AIV kernels")
if(DSL_BISHENG)
    add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/aiv_all_gather.o
        COMMAND ${CMAKE_COMMAND} -E env ASCEND_HOME_PATH=${DSL_ASC_HOME} ${DSL_BISHENG} -c --asc-aicore-lang
            ${CMAKE_CURRENT_SOURCE_DIR}/engines/aiv/aiv_all_gather.asc
            -o ${CMAKE_CURRENT_BINARY_DIR}/aiv_all_gather.o -std=c++17 -O2 --npu-arch=dav-3510
            -D__INTEL_LLVM_COMPILER=1 -DSYCL_LANGUAGE_VERSION=1
            -I${CMAKE_CURRENT_SOURCE_DIR}/engines/aiv -I${CMAKE_CURRENT_SOURCE_DIR}/dispatch/ccu_server
        DEPENDS engines/aiv/aiv_all_gather.asc engines/aiv/aiv_program.h dispatch/ccu_server/mailbox_protocol.h
        VERBATIM)
    add_custom_target(collective_dsl_aiv DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/aiv_all_gather.o)
endif()
