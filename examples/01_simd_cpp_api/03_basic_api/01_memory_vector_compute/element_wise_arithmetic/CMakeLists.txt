# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2026 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ----------------------------------------------------------------------------------------------------------


cmake_minimum_required(VERSION 3.16)

set(CMAKE_ASC_RUN_MODE "npu" CACHE STRING "Run mode: npu, cpu, sim")
set(CMAKE_ASC_ARCHITECTURES "dav-2201" CACHE STRING "NPU architecture: dav-2201, dav-3510")
option(BUILD_VECTOR_POINTER_SAMPLES "Build VECTOR pointer examples for dav-3510 NPU" OFF)

if(BUILD_VECTOR_POINTER_SAMPLES AND
   (NOT CMAKE_ASC_ARCHITECTURES STREQUAL "dav-3510" OR NOT CMAKE_ASC_RUN_MODE STREQUAL "npu"))
    message(FATAL_ERROR "VECTOR pointer examples require CMAKE_ASC_ARCHITECTURES=dav-3510 and CMAKE_ASC_RUN_MODE=npu")
endif()

find_package(ASC REQUIRED)

project(kernel_samples LANGUAGES ASC CXX)

add_executable(demo
    element_wise_arithmetic.asc
)

target_compile_options(demo PRIVATE
    $<$<COMPILE_LANGUAGE:ASC>:--npu-arch=${CMAKE_ASC_ARCHITECTURES}>
)

if(BUILD_VECTOR_POINTER_SAMPLES)
    get_filename_component(ASC_DEVKIT_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../../../../.." ABSOLUTE)
    enable_testing()
    add_custom_target(vector_pointer_samples)
    foreach(suite elementwise conversion layout compare index transpose sort special)
        set(sample_source "vector_pointer_${suite}_regression.asc")
        if(suite STREQUAL "elementwise")
            set(sample_source "vector_pointer_regression.asc")
        endif()
        set(sample_target "vector_pointer_${suite}")
        add_executable(${sample_target} ${sample_source})
        target_compile_options(${sample_target} PRIVATE
            $<$<COMPILE_LANGUAGE:ASC>:--npu-arch=${CMAKE_ASC_ARCHITECTURES}>
            $<$<COMPILE_LANGUAGE:ASC>:-std=c++17>
            $<$<COMPILE_LANGUAGE:ASC>:-O2>
        )
        # Prefer this checkout's Basic API headers over the installed SDK headers.
        foreach(include_dir . include include/basic_api include/utils impl impl/basic_api)
            target_compile_options(${sample_target} PRIVATE "-iquote${ASC_DEVKIT_ROOT}/${include_dir}")
        endforeach()
        target_include_directories(${sample_target} PRIVATE "${ASC_DEVKIT_ROOT}")
        target_link_libraries(${sample_target} PRIVATE ascendcl m)
        add_dependencies(vector_pointer_samples ${sample_target})
        add_test(NAME ${sample_target} COMMAND $<TARGET_FILE:${sample_target}>)
        # All examples use device 0.
        set_tests_properties(${sample_target} PROPERTIES RUN_SERIAL TRUE)
    endforeach()
endif()
