cmake_minimum_required(VERSION 3.16)
find_package(ASC REQUIRED)
project(kernel_samples LANGUAGES ASC CXX)
add_executable(cube_asc_copy_l12l0b_trans_example c_api_asc_copy_l12l0b_trans.asc)
target_compile_options(cube_asc_copy_l12l0b_trans_example PRIVATE
    $<$<COMPILE_LANGUAGE:ASC>:--npu-arch=dav-3510>
)
