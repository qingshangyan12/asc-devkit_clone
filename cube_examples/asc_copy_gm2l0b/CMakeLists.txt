cmake_minimum_required(VERSION 3.16)
find_package(ASC REQUIRED)
project(kernel_samples LANGUAGES ASC CXX)
add_executable(cube_asc_copy_gm2l0b_example c_api_asc_copy_gm2l0b.asc)
target_compile_options(cube_asc_copy_gm2l0b_example PRIVATE
    $<$<COMPILE_LANGUAGE:ASC>:--npu-arch=dav-2201>
)
