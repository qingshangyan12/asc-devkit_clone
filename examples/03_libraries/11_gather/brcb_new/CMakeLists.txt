cmake_minimum_required(VERSION 3.16)

find_package(ASC REQUIRED)

project(kernel_samples LANGUAGES ASC CXX)

add_executable(demo
    brcb_new.asc
)

message(STATUS "[INFO] SIZE is ${SIZE}")
message(STATUS "[INFO] SCENARIO_NUM is ${SCENARIO_NUM}")
message(STATUS "[INFO] REPEAT_TIME is ${REPEAT_TIME}")
message(STATUS "[INFO] DST_BLK_STRIDE is ${DST_BLK_STRIDE}")
message(STATUS "[INFO] DST_REP_STRIDE is ${DST_REP_STRIDE}")

target_compile_definitions(demo PRIVATE
    SIZE=${SIZE}
    SCENARIO_NUM=${SCENARIO_NUM}
    REPEAT_TIME=${REPEAT_TIME}
    DST_BLK_STRIDE=${DST_BLK_STRIDE}
    DST_REP_STRIDE=${DST_REP_STRIDE}
)

target_compile_options(demo PRIVATE
    $<$<COMPILE_LANGUAGE:ASC>:--npu-arch=dav-2201>
)
