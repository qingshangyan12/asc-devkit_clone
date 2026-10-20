#!/usr/bin/env bash
# Copyright (c) 2026 ddplys.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.

set -euo pipefail
root=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/../../../.." && pwd)
logs=${1:-"${root}/tests/api/basic_api/vector_pointer/cpu_logs"}
mkdir -p "${logs}"
logs=$(cd "${logs}" && pwd)
: "${ASCEND_HOME_PATH:?Source the CANN environment first}"
cpu="${ASCEND_HOME_PATH}/tools/cpudebug"
model="${ASCEND_HOME_PATH}/tools/simulator/Ascend950PR_9599/lib"
flags=(-std=c++17 -D__NPU_ARCH__=3510 -D__DAV_C310__ -D__DAV_C310_VEC__
       -DASCENDC_CPU_DEBUG=1 -D__callee__= -D_GLIBCXX_USE_CXX11_ABI=0)
for dir in . include include/basic_api include/utils impl impl/basic_api; do
    flags+=(-iquote "${root}/${dir}")
done
command=(g++ "${flags[@]}" -I"${root}" -I"${cpu}/include"
         "${root}/tests/api/basic_api/vector_pointer/get_abs_addr_cpu.cpp"
         -L"${cpu}/lib64/Ascend950PR_9599" -L"${cpu}/lib64" -L"${model}"
         -Wl,--no-as-needed -ltikcpp_debug -lcpudebug -lcpudebug_cceprint -lcpudebug_npuchk
         -lcpudebug_stubreg -lpem_davinci -ldl -lpthread -o "${logs}/get_abs_addr_cpu")
printf '%q ' "${command[@]}" > "${logs}/command.txt"
printf '\n' >> "${logs}/command.txt"
"${command[@]}" > "${logs}/cpu_build.log" 2>&1
export LD_LIBRARY_PATH="${cpu}/lib64/Ascend950PR_9599:${cpu}/lib64:${model}:${LD_LIBRARY_PATH:-}"
"${logs}/get_abs_addr_cpu" > "${logs}/cpu_run.log" 2>&1
cat "${logs}/cpu_run.log"

# Parse the public comparison/selection header with the CPU_DEBUG architecture
# definitions used by LLT. Legacy products do not declare TypeUtils.
printf '#include "basic_api/kernel_operator_vec_cmpsel_intf.h"\n' > "${logs}/cmpsel_include.cpp"
for target in 1001:C100 2002:M200 2201:C220 3002:M300 3102:M310 3510:C310; do
    arch=${target%%:*}
    product=${target#*:}
    header_flags=(-std=c++17 -fsyntax-only "-D__NPU_ARCH__=${arch}" "-D__DAV_${product}__"
                  -DASCENDC_CPU_DEBUG=1 -D__callee__= -D_GLIBCXX_USE_CXX11_ABI=0)
    for dir in . include include/basic_api include/utils impl impl/basic_api; do
        header_flags+=(-iquote "${root}/${dir}")
    done
    command=(g++ "${header_flags[@]}" -I"${cpu}/include" "${logs}/cmpsel_include.cpp")
    printf '%q ' "${command[@]}" > "${logs}/header_${arch}_command.txt"
    printf '\n' >> "${logs}/header_${arch}_command.txt"
    "${command[@]}" > "${logs}/header_${arch}.log" 2>&1
    printf 'PASS public cmpsel header CPU_DEBUG arch=%s\n' "${arch}"
done
