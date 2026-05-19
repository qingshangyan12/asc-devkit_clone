# Cube C API Example CMakeLists.txt
# 使用 dav-2201 架构 (dav-3510 Cube C API 在 CANN 9.0.0-beta2 中尚未完整发布)
cmake_minimum_required(VERSION 3.16)
find_package(ASC REQUIRED)
project(kernel_samples LANGUAGES ASC CXX)
add_executable(cube_mmad_example cube_mmad_example.asc)
target_compile_options(cube_mmad_example PRIVATE
    $<$<COMPILE_LANGUAGE:ASC>:--npu-arch=dav-2201>
)
