# Copyright (c) 2026 Huawei Technologies Co., Ltd.
# Licensed under the CANN Open Software License Agreement Version 2.0.

set(COLLECTIVE_DSL_TEST ascendc_ut_hccl_collective_dsl)
set(COLLECTIVE_DSL_DIR ${HCCL_CC_SRC_DIR}/ops/op_common/collective_dsl)

add_executable(${COLLECTIVE_DSL_TEST}
    collective_dsl_test.cc
    ${COLLECTIVE_DSL_DIR}/model/collective_plan.cc
    ${COLLECTIVE_DSL_DIR}/model/logical_resources.cc
    ${COLLECTIVE_DSL_DIR}/model/execution_route.cc
    ${COLLECTIVE_DSL_DIR}/primitives/all_gather_primitives.cc
    ${COLLECTIVE_DSL_DIR}/algorithms/all_gather_algorithms.cc
    ${COLLECTIVE_DSL_DIR}/dispatch/kfc_dispatch.cc
    ${COLLECTIVE_DSL_DIR}/engines/trace/trace_engine.cc
    ${COLLECTIVE_DSL_DIR}/trace/native_binding_trace.cc
)

target_include_directories(${COLLECTIVE_DSL_TEST} PRIVATE
    ${COLLECTIVE_DSL_DIR}/model
    ${COLLECTIVE_DSL_DIR}/primitives
    ${COLLECTIVE_DSL_DIR}/algorithms
    ${COLLECTIVE_DSL_DIR}/dispatch
    ${COLLECTIVE_DSL_DIR}/engines/trace
    ${COLLECTIVE_DSL_DIR}/trace
)

target_compile_options(${COLLECTIVE_DSL_TEST} PRIVATE
    -std=c++14
    -Wall
    -Wextra
    -Werror
)

add_custom_command(
    TARGET ${COLLECTIVE_DSL_TEST} POST_BUILD
    COMMAND $<TARGET_FILE:${COLLECTIVE_DSL_TEST}>
    COMMENT "Run collective DSL algorithm, resource and trace checks"
)

# 新执行图测试不依赖旧 Plan 或硬件 mock。与主工程一起构建，命令可直接查看。
set(DATAFLOW_SOURCES
    ${COLLECTIVE_DSL_DIR}/core/program.cc
    ${COLLECTIVE_DSL_DIR}/primitives/exchange.cc
    ${COLLECTIVE_DSL_DIR}/algorithms/allgather/slices.cc
    ${COLLECTIVE_DSL_DIR}/algorithms/allgather/sole_mesh.cc
    ${COLLECTIVE_DSL_DIR}/algorithms/allgather/sole_nhr.cc
    ${COLLECTIVE_DSL_DIR}/algorithms/allgather/sequence_mesh_nhr.cc
    ${COLLECTIVE_DSL_DIR}/algorithms/allgather/concurrent_mesh_nhr.cc
    ${COLLECTIVE_DSL_DIR}/algorithms/allgather/parallel_mesh_nhr.cc
    ${COLLECTIVE_DSL_DIR}/algorithms/allgather/pipeline_mesh_nhr.cc
)
add_executable(ascendc_ut_hccl_dataflow dataflow_test.cc ${DATAFLOW_SOURCES}
    ${COLLECTIVE_DSL_DIR}/engines/sim/sim_engine.cc)
target_include_directories(ascendc_ut_hccl_dataflow PRIVATE
    ${COLLECTIVE_DSL_DIR}/core ${COLLECTIVE_DSL_DIR}/primitives
    ${COLLECTIVE_DSL_DIR}/algorithms/allgather ${COLLECTIVE_DSL_DIR}/engines/sim)
target_compile_options(ascendc_ut_hccl_dataflow PRIVATE -std=c++14 -Wall -Wextra -Werror)
add_custom_command(TARGET ascendc_ut_hccl_dataflow POST_BUILD
    COMMAND $<TARGET_FILE:ascendc_ut_hccl_dataflow>
    COMMENT "Validate explicit transfers and cross-rank dependencies with randomized schedules")
