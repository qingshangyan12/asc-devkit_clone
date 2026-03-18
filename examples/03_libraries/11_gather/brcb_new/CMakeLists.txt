cmake_minimum_required(VERSION 3.16)

find_package(ASC REQUIRED)

project(kernel_samples LANGUAGES ASC CXX)

add_executable(demo
    brcb_new.asc
)

message(STATUS "[INFO] SIZE is ${SIZE}")
message(STATUS "[INFO] SCENARIO_NUM is ${SCENARIO_NUM}")

target_compile_definitions(demo PRIVATE
    SIZE=${SIZE}
    SCENARIO_NUM=${SCENARIO_NUM}
)

target_compile_options(demo PRIVATE
    $<$<COMPILE_LANGUAGE:ASC>:--npu-arch=dav-2201>
)
