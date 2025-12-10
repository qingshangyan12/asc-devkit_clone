cmake_minimum_required(VERSION 3.16)

find_package(ASC REQUIRED HINTS $ENV{ASCEND_HOME_PATH}/lib64/cmake)

project(kernel_samples LANGUAGES ASC CXX)

add_executable(demo
    gather_and_adds.asc
)

target_compile_options(demo PRIVATE
    # 该NPU架构版本号暂未正式发布，后续可能会有变更，请关注正式版本发布信息
    $<$<COMPILE_LANGUAGE:ASC>:--npu-arch=dav-3101>
)

target_link_libraries(demo PRIVATE m)