# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ----------------------------------------------------------------------------------------------------------
if(POLICY CMP0135)
    cmake_policy(SET CMP0135 NEW)
endif()
set(GENERATE_CPP_COV ${CMAKE_CURRENT_LIST_DIR}/scripts/generate_cpp_cov.sh)

find_package(Python3 COMPONENTS Interpreter)
add_cann_third_party(gtest)
add_cann_third_party(boost)
add_cann_third_party(json)
include(${CMAKE_CURRENT_LIST_DIR}/third_party/mockcpp.cmake)

# Host-side shim for address-space qualifiers used by impl headers (e.g. __biasbuf__ in
# kernel_operator_dump_tensor_impl.h). CANN packages predating those qualifiers ship a
# stub_def.h without them; provide an empty definition, mirroring how __cbuf__/__fbuf__
# are defined there. Skipped when the CANN package already provides the qualifier.
set(_asc_stub_def_hint "${ASCEND_CANN_PACKAGE_PATH}/tools/tikicpulib/lib/include/stub_def.h")
if(EXISTS "${_asc_stub_def_hint}")
    file(READ "${_asc_stub_def_hint}" _asc_stub_def_content)
    if(NOT "${_asc_stub_def_content}" MATCHES "__biasbuf__")
        add_compile_definitions(__biasbuf__=)
        message(STATUS "CANN stub_def.h lacks __biasbuf__ host shim; defining __biasbuf__ as empty for host UT compilation")
    endif()
    unset(_asc_stub_def_content)
else()
    message(WARNING "stub_def.h not found at ${_asc_stub_def_hint}; __biasbuf__ host shim not applied — if impl headers use this qualifier, host UT compilation will fail")
endif()
unset(_asc_stub_def_hint)
