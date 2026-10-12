# Copyright (c) 2026 Huawei Technologies Co., Ltd.
# Licensed under the CANN Open Software License Agreement Version 2.0.

target_sources(mc2_client PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/model/collective_plan.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/model/logical_resources.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/model/execution_route.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/primitives/all_gather_primitives.cc
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
    ${CMAKE_CURRENT_SOURCE_DIR}/model
    ${CMAKE_CURRENT_SOURCE_DIR}/primitives
    ${CMAKE_CURRENT_SOURCE_DIR}/algorithms
    ${CMAKE_CURRENT_SOURCE_DIR}/dispatch
    ${CMAKE_CURRENT_SOURCE_DIR}/adapters/ascend
    ${CMAKE_CURRENT_SOURCE_DIR}/engines/trace
    ${CMAKE_CURRENT_SOURCE_DIR}/trace
)
