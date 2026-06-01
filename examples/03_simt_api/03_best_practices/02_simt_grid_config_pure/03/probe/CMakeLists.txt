cmake_minimum_required(VERSION 3.16)
find_package(ASC REQUIRED)
project(probe_gather LANGUAGES ASC CXX)

# 参数化探针：元素总数 TOTAL、核数 NB、每block线程数 TPB 均为编译宏。
# 用于对不同 (TOTAL, NB) 组合扫描 msprof Task Duration。
set(TOTAL "131072" CACHE STRING "elements")
set(NB "64" CACHE STRING "blocks (logical cores)")
set(TPB "2048" CACHE STRING "threads per block")

add_executable(probe_gather probe_gather.asc)
target_compile_definitions(probe_gather PRIVATE TOTAL=${TOTAL} NB=${NB} TPB=${TPB})
target_compile_options(probe_gather PRIVATE
    $<$<COMPILE_LANGUAGE:ASC>:--npu-arch=dav-3510 --enable-simt>
)
