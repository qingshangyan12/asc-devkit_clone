cmake_minimum_required(VERSION 3.16)

set(CMAKE_ASC_ARCHITECTURES "dav-3510" CACHE STRING "NPU architecture")
set(CASE_SOURCE "" CACHE FILEPATH "C API example source")
set(CASE_NAME "c_api_example" CACHE STRING "C API example target name")

if(CASE_SOURCE STREQUAL "")
    message(FATAL_ERROR "CASE_SOURCE must point to one generated .asc example")
endif()

find_package(ASC REQUIRED)
project(c_api_call_example LANGUAGES ASC CXX)

add_executable(${CASE_NAME} ${CASE_SOURCE})
target_include_directories(${CASE_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${CASE_NAME} PRIVATE m)
target_compile_options(${CASE_NAME} PRIVATE
    $<$<COMPILE_LANGUAGE:ASC>:--npu-arch=${CMAKE_ASC_ARCHITECTURES}>
    $<$<COMPILE_LANGUAGE:ASC>:-Wno-deprecated-declarations>
)
