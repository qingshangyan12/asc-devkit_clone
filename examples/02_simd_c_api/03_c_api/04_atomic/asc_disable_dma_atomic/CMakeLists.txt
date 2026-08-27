cmake_minimum_required(VERSION 3.16)

find_package(ASC REQUIRED)

project(asc_disable_dma_atomic LANGUAGES ASC CXX)

add_executable(asc_disable_dma_atomic_example
    asc_disable_dma_atomic.asc
)

target_include_directories(asc_disable_dma_atomic_example BEFORE PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/../../../../../include
    ${CMAKE_CURRENT_LIST_DIR}/../../../../../impl
)

target_compile_options(asc_disable_dma_atomic_example PRIVATE
    $<$<COMPILE_LANGUAGE:ASC>:--npu-arch=dav-2201>
)
