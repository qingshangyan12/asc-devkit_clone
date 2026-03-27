cmake_minimum_required(VERSION 3.10)
project(antiquantize_example CXX C)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -Wall")

find_package(soc REQUIRED)

include_directories(${SOC_SDK_PATH}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_executable(antiquantize_demo 
    antiquantize_example.asc
)

target_link_libraries(antiquantize_demo 
    ${SOC_SDK_PATH}/lib/libascendcl.so
    ${SOC_SDK_PATH}/lib/libascend_kernel.so
)

set_target_properties(antiquantize_demo PROPERTIES
    CXX_STANDARD 14
    LINKER_LANGUAGE CXX
)