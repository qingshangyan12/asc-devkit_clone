# Copyright (c) 2026 Huawei Technologies Co., Ltd.
# Licensed under the CANN Open Software License Agreement Version 2.0.

set(COLLECTIVE_DSL_TEST ascendc_ut_hccl_collective_dsl)
set(COLLECTIVE_DSL_DIR ${HCCL_CC_SRC_DIR}/ops/op_common/collective_dsl)

add_executable(${COLLECTIVE_DSL_TEST}
    collective_dsl_test.cc
    ${COLLECTIVE_DSL_DIR}/core/collective_plan.cc
    ${COLLECTIVE_DSL_DIR}/core/resource_plan.cc
    ${COLLECTIVE_DSL_DIR}/primitives/all_gather_primitives.cc
    ${COLLECTIVE_DSL_DIR}/algorithms/all_gather_algorithms.cc
    ${COLLECTIVE_DSL_DIR}/backends/trace_backend.cc
)

target_include_directories(${COLLECTIVE_DSL_TEST} PRIVATE
    ${COLLECTIVE_DSL_DIR}/core
    ${COLLECTIVE_DSL_DIR}/primitives
    ${COLLECTIVE_DSL_DIR}/algorithms
    ${COLLECTIVE_DSL_DIR}/backends
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
