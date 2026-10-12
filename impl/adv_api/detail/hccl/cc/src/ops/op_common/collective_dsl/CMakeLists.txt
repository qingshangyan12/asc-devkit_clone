# Copyright (c) 2026 Huawei Technologies Co., Ltd.
# Licensed under the CANN Open Software License Agreement Version 2.0.

target_sources(mc2_client PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/core/collective_plan.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/core/resource_plan.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/primitives/all_gather_primitives.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/algorithms/all_gather_algorithms.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/backends/aicpu_backend.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/backends/trace_backend.cc
)

target_include_directories(mc2_client PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/core
    ${CMAKE_CURRENT_SOURCE_DIR}/primitives
    ${CMAKE_CURRENT_SOURCE_DIR}/algorithms
    ${CMAKE_CURRENT_SOURCE_DIR}/backends
)
