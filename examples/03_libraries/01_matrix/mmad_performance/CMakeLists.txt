cmake_minimum_required(VERSION 3.16)
project(MicroBenchMark_Vec)

if(NOT DEFINED BINARY_OP_DIR)
    set(BINARY_OP_DIR vec_op)
endif()

if(SOC_VERSION STREQUAL "Ascend910_9599")
    set(CHIP_TYPE 1)
else()
    set(CHIP_TYPE 0)
endif()

set(COMMON_HEADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/${BINARY_OP_DIR})
message(STATUS "BINARY_OP_DIR from cmd = ${BINARY_OP_DIR}")

set(ASCEND_PRODUCT_TYPE "Ascend910_9592" CACHE STRING "SOC VERSION")
set(CMAKE_CONFIG_DIR "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(RUN_MODE "npu" CACHE STRING "cpu/sim/npu")
set(SOC_VERSION "Ascend310P3" CACHE STRING "system on chip type")
set(ASCEND_CANN_PACKAGE_PATH "/usr/local/Ascend/ascend-toolkit/latest" CACHE STRING "CANN Package Dir")
set(ASC_CMAKE_PATH $ENV{ASCEND_HOME_PATH}/tools/tikcpp/ascendc_kernel_cmake)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DCHIP_TYPE=${CHIP_TYPE}")

list(APPEND CMAKE_MODULE_PATH ${ASC_CMAKE_PATH})
include(${ASC_CMAKE_PATH}/asc_config.cmake)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Build Type: Release/Debug" FORCE)
endif()

if(CMAKE_INSTALL_PREFIX STREQUAL /usr/local)
    set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_LIST_DIR}/out" CACHE STRING "PATH to install" FORCE)
endif()

if(NOT DEFINED ENV{CMAKE_PREFIX_PATH})
    set(CMAKE_PREFIX_PATH ${ASCEND_CANN_PACKAGE_PATH}/tools/tikicpulib/lib/cmake)
endif()

file(GLOB KERNEL_FILES
    ${COMMON_HEADER_DIR}/kernel.cpp
)

if("${RUN_MODE}" STREQUAL "cpu")
    include(${CMAKE_CONFIG_DIR}/cpu_lib.cmake)
elseif("${RUN_MODE}" STREQUAL "sim" OR "${RUN_MODE}" STREQUAL "npu")
    include(${CMAKE_CONFIG_DIR}/npu_lib.cmake)
    ascendc_compile_definitions(ascendc_kernels_${RUN_MODE} PUBLIC USE_SYS_COUNT_RECORD=1)
    ascendc_compile_definitions(ascendc_kernels_${RUN_MODE} PRIVATE CHIP_TYPE=${CHIP_TYPE})
else()
    message("invalid RUN_MODE: ${RUN_MODE}")
endif()
add_executable(ascendc_kernels_bbit ${COMMON_HEADER_DIR}/main.cpp)

set(TARGET_ARCH "x86_64")
if("${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "aarch64" OR
   "${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "armv8" OR
   "${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "arm64")
    set(TARGET_ARCH "aarch64")
endif()

target_include_directories(ascendc_kernels_bbit PRIVATE
    # ${COMMON_HEADER_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${ASCEND_CANN_PACKAGE_PATH}/tools/tikicpulib/lib/include
    ${ASCEND_CANN_PACKAGE_PATH}/${TARGET_ARCH}-linux/include/experiment/msprof
    ${ASCEND_CANN_PACKAGE_PATH}/${TARGET_ARCH}-linux/include/experiment/runtime
)

target_compile_options(ascendc_kernels_bbit PRIVATE
    $<BUILD_INTERFACE:$<$<STREQUAL:${RUN_MODE},cpu>:-g>>
    -O2 -std=c++17 -D_GLIBCXX_USE_CXX11_ABI=0 -Wall -Werror
)
set(LINK_DIRS
    ${ASCEND_CANN_PACKAGE_PATH}/tools/tikicpulib/lib
)
if(SOC_VERSION STREQUAL "Ascend910B1")
    list(INSERT LINK_DIRS 0 ${ASCEND_CANN_PACKAGE_PATH}/compiler/lib64)
    list(INSERT LINK_DIRS 1 ${ASCEND_CANN_PACKAGE_PATH}/${TARGET_ARCH}-linux/devlib)
endif()
target_link_directories(ascendc_kernels_bbit PRIVATE ${LINK_DIRS})

target_compile_definitions(ascendc_kernels_bbit PRIVATE
    SOC_VERSION="${SOC_VERSION}"
)

target_compile_definitions(ascendc_kernels_bbit PRIVATE USE_SYS_COUNT_RECORD=1)

set(LIBRARY_LIBS "")
if(SOC_VERSION STREQUAL "Ascend910B1")
    list(APPEND LIBRARY_LIBS
        # opp_registry
        error_manager
        ascend_hal
        runtime
        ascend_dump
    )
endif()
list(APPEND LIBRARY_LIBS
    $<BUILD_INTERFACE:$<$<OR:$<STREQUAL:${RUN_MODE},npu>,$<STREQUAL:${RUN_MODE},sim>>:host_intf_pub>>
    $<BUILD_INTERFACE:$<$<STREQUAL:${RUN_MODE},cpu>:ascendcl>>
    ascendc_kernels_${RUN_MODE}
    platform
    tikicpulib_stubreg
)
target_link_libraries(ascendc_kernels_bbit PRIVATE ${LIBRARY_LIBS})

install(TARGETS ascendc_kernels_bbit
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
