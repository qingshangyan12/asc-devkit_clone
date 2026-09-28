# xpuops_exception_dump 构建脚本（CMake）
# 环境：需先 source CANN 环境（set_env.sh），ASCEND_TOOLKIT_HOME 生效
cmake_minimum_required(VERSION 3.10)
project(xpuops_exception_dump CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

if(NOT DEFINED ENV{ASCEND_TOOLKIT_HOME})
    message(FATAL_ERROR "ASCEND_TOOLKIT_HOME not set, please source CANN set_env.sh first")
endif()

set(ASCEND_TOOLKIT_HOME $ENV{ASCEND_TOOLKIT_HOME})

add_library(xpuops_exception_dump SHARED
    src/init.cpp
    src/exception_callback.cpp
    src/utils.cpp
)

target_include_directories(xpuops_exception_dump PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${ASCEND_TOOLKIT_HOME}/include
)

# 链接 AscendCL Runtime
find_library(ASCENDCL_LIB ascendcl PATHS ${ASCEND_TOOLKIT_HOME}/lib64 ${ASCEND_TOOLKIT_HOME}/lib)
find_library(RT_LIB rt)
target_link_libraries(xpuops_exception_dump PRIVATE ${ASCENDCL_LIB} ${RT_LIB})
