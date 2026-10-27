cmake_minimum_required(VERSION 3.16)
set(CMAKE_ASC_ARCHITECTURES "dav-3510" CACHE STRING "NPU architecture")
find_package(ASC REQUIRED)
project(simd_vf_printf_gm_sample LANGUAGES ASC CXX)
get_filename_component(DEFAULT_ASC_DEVKIT_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/../../../../.." ABSOLUTE)
set(ASC_DEVKIT_SOURCE "${DEFAULT_ASC_DEVKIT_SOURCE}" CACHE PATH "Checkout under test")
if(NOT EXISTS "${ASC_DEVKIT_SOURCE}/impl/utils/debug/asc_simd_gm_printf_impl.h")
    message(FATAL_ERROR "ASC_DEVKIT_SOURCE must contain the GM printf implementation")
endif()
add_executable(demo simd_vf_printf.asc)
target_include_directories(demo BEFORE PRIVATE
    "${ASC_DEVKIT_SOURCE}" "${ASC_DEVKIT_SOURCE}/include"
    "${ASC_DEVKIT_SOURCE}/include/basic_api" "${ASC_DEVKIT_SOURCE}/include/c_api")
# bisheng prepends toolkit -I paths; -iquote makes quoted SDK includes use the checkout.
target_compile_options(demo PRIVATE
    "-iquote${ASC_DEVKIT_SOURCE}" "-iquote${ASC_DEVKIT_SOURCE}/include"
    "-iquote${ASC_DEVKIT_SOURCE}/include/basic_api" "-iquote${ASC_DEVKIT_SOURCE}/include/c_api"
    $<$<COMPILE_LANGUAGE:ASC>:--npu-arch=${CMAKE_ASC_ARCHITECTURES}>)
