cmake_minimum_required(VERSION 3.16)
find_package(ASC REQUIRED)
project(kernel_samples LANGUAGES ASC CXX)
add_executable(cube_asc_get_l0c2gm_unitflag c_api_asc_get_l0c2gm_unitflag.asc)
target_compile_options(cube_asc_get_l0c2gm_unitflag PRIVATE
    $<$<COMPILE_LANGUAGE:ASC>:--npu-arch=dav-2201>
)
