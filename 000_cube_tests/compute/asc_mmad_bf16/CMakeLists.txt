cmake_minimum_required(VERSION 3.16)
find_package(ASC REQUIRED)
project(kernel_samples LANGUAGES ASC CXX)
add_executable(cube_asc_mmad_bf16 c_api_asc_mmad_bf16.asc)
target_compile_options(cube_asc_mmad_bf16 PRIVATE
    $<$<COMPILE_LANGUAGE:ASC>:--npu-arch=dav-3510>
)
