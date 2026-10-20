#!/usr/bin/env bash
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
