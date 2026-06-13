cmake_minimum_required(VERSION 3.16)

set(CMAKE_ASC_RUN_MODE "npu" CACHE STRING "Run mode: npu, cpu, sim")
set(CMAKE_ASC_ARCHITECTURES "dav-3510" CACHE STRING "NPU architecture: dav-2201, dav-3510")
option(ENABLE_TIMESTAMP "Build demo target with ENABLE_TIMESTAMP defined" OFF)

find_package(ASC REQUIRED)

project(kernel_samples LANGUAGES ASC CXX)

set(ASC_PROF_INCLUDE_DIRS
    # /usr/local/Ascend/cann-9.1.0/cann-9.1.0/include
    # /usr/local/Ascend/cann-9.1.0/cann-9.1.0/include/mmpa
    # /usr/local/Ascend/cann-9.1.0/cann-9.1.0//x86_64-linux/pkg_inc
    /home/yyc/pkgs/cann/include
    /home/yyc/pkgs/cann/include/mmpa
    /home/yyc/pkgs/cann/x86_64-linux/pkg_inc
)

function(add_asc_prof_executable target_name enable_timestamp)
    add_executable(${target_name}
        prof.asc
    )

    target_include_directories(${target_name} PRIVATE
        ${ASC_PROF_INCLUDE_DIRS}
    )

    target_compile_options(${target_name} PRIVATE
        -D__linux
        $<$<COMPILE_LANGUAGE:ASC>:--npu-arch=${CMAKE_ASC_ARCHITECTURES}>
    )

    if(${enable_timestamp})
        target_compile_definitions(${target_name} PRIVATE ENABLE_TIMESTAMP)
    endif()

    target_link_libraries(${target_name} PRIVATE
        /usr/local/Ascend/driver/lib64/driver/libascend_hal.so
    )
endfunction()

if(ENABLE_TIMESTAMP)
    add_asc_prof_executable(demo ON)
    add_asc_prof_executable(demo_no_timestamp OFF)
    set(ASC_PROF_TIMESTAMP_TARGET demo)
    set(ASC_PROF_NO_TIMESTAMP_TARGET demo_no_timestamp)
else()
    add_asc_prof_executable(demo OFF)
    add_asc_prof_executable(demo_timestamp ON)
    set(ASC_PROF_TIMESTAMP_TARGET demo_timestamp)
    set(ASC_PROF_NO_TIMESTAMP_TARGET demo)
endif()

add_custom_target(compare_timestamp
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/run_timestamp_compare.sh
        $<TARGET_FILE:${ASC_PROF_NO_TIMESTAMP_TARGET}>
        $<TARGET_FILE:${ASC_PROF_TIMESTAMP_TARGET}>
        ${CMAKE_CURRENT_BINARY_DIR}/compare_results
        ${CMAKE_CURRENT_SOURCE_DIR}/parse_data_bin.py
    DEPENDS ${ASC_PROF_NO_TIMESTAMP_TARGET} ${ASC_PROF_TIMESTAMP_TARGET}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Run asc_prof with and without ENABLE_TIMESTAMP, keeping results in separate directories"
)
