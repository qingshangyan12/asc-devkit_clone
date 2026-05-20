cmake_minimum_required(VERSION 3.16)
find_package(ASC REQUIRED)
project(kernel_samples LANGUAGES ASC CXX)
add_executable(cube_asc_set_gm2l1_loop_size c_api_asc_set_gm2l1_loop_size.asc)
target_compile_options(cube_asc_set_gm2l1_loop_size PRIVATE
    $<$<COMPILE_LANGUAGE:ASC>:--npu-arch=dav-3510>
)
