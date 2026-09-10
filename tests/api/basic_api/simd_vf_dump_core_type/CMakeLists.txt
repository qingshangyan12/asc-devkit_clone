# Copyright (c) 2026 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.

cmake_minimum_required(VERSION 3.16)
set(CMAKE_ASC_ARCHITECTURES dav-3510 CACHE STRING "Target architecture")
find_package(ASC REQUIRED)
project(simd_vf_dump_core_type LANGUAGES ASC CXX)
get_filename_component(SOURCE_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../../../.." ABSOLUTE)
add_executable(demo test_core_type.asc)
target_compile_features(demo PRIVATE cxx_std_17)
foreach(dir "" /include /include/basic_api /include/utils /impl /impl/basic_api)
    target_compile_options(demo PRIVATE "SHELL:-iquote ${SOURCE_ROOT}${dir}")
endforeach()
target_compile_options(demo PRIVATE --npu-arch=dav-3510 -O2 -fno-jump-tables)
set(ASC_SIMULATOR_LIB "" CACHE PATH "Empty for hardware, or simulator library directory")
option(ASC_SIMULATOR_PV "Use the PV model" ON)
if(ASC_SIMULATOR_LIB)
    target_link_directories(demo PRIVATE "${ASC_SIMULATOR_LIB}")
    if(ASC_SIMULATOR_PV)
        target_link_libraries(demo PRIVATE runtime_cmodel npu_drv_pvmodel m)
    else()
        target_link_libraries(demo PRIVATE runtime_camodel npu_drv_camodel m)
    endif()
    target_link_options(demo PRIVATE -Wl,--disable-new-dtags)
endif()
file(GLOB DEBUG_HEADERS CONFIGURE_DEPENDS "${SOURCE_ROOT}/impl/utils/debug/*.h" "${SOURCE_ROOT}/impl/utils/debug/npu_arch_3510/*.h")
set_property(SOURCE test_core_type.asc APPEND PROPERTY OBJECT_DEPENDS ${DEBUG_HEADERS} "${SOURCE_ROOT}/impl/utils/base/helpers_impl.h")
find_package(Python3 COMPONENTS Interpreter REQUIRED)
enable_testing()
add_test(NAME core_type_verifier COMMAND ${Python3_EXECUTABLE} -O ${CMAKE_CURRENT_SOURCE_DIR}/test_verify.py)
set_tests_properties(core_type_verifier PROPERTIES ENVIRONMENT "PYTHONDONTWRITEBYTECODE=1")
