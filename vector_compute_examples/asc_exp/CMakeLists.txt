cmake_minimum_required(VERSION 3.16)
find_package(ASC REQUIRED)
project(kernel_samples LANGUAGES ASC CXX)
add_executable(c_api_exp_example c_api_exp.asc)
target_compile_options(c_api_exp_example PRIVATE
    $<$<COMPILE_LANGUAGE:ASC>:--npu-arch=dav-3510>
)
target_link_options(c_api_exp_example PRIVATE -lm)
