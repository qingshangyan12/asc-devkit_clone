cmake_minimum_required(VERSION 3.16)
find_package(ASC REQUIRED)
project(kernel_samples LANGUAGES ASC CXX)
add_executable(c_api_gt_example c_api_gt.asc)
target_compile_options(c_api_gt_example PRIVATE
    $<$<COMPILE_LANGUAGE:ASC>:--npu-arch=dav-3510>
)
