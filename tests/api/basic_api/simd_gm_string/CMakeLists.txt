cmake_minimum_required(VERSION 3.16)
project(simd_gm_consumer_tests LANGUAGES CXX)

# Standalone target for hosts whose top-level SDK test configuration is unavailable.
# Supply existing dependencies: this project neither downloads nor installs them.
set(CANN_ROOT "$ENV{ASCEND_HOME_PATH}" CACHE PATH "CANN installation containing CPU intrinsic headers")
set(GTEST_ROOT "" CACHE PATH "GoogleTest installation prefix")
set(MOCKCPP_ROOT "" CACHE PATH "mockcpp installation prefix")
set(BOOST_ROOT "" CACHE PATH "Boost header root")
set(TEST_CXX11_ABI "0" CACHE STRING "libstdc++ ABI used by GoogleTest and mockcpp")

find_path(CPU_STUB_INCLUDE stub_fun.h HINTS "${CANN_ROOT}"
    PATH_SUFFIXES tools/cpudebug/include toolkit/tools/tikicpulib/lib/include tools/tikicpulib/lib/include REQUIRED)
find_path(GTEST_INCLUDE gtest/gtest.h HINTS "${GTEST_ROOT}" PATH_SUFFIXES include REQUIRED)
find_library(GTEST_LIBRARY gtest HINTS "${GTEST_ROOT}" PATH_SUFFIXES lib lib64 REQUIRED)
find_library(GTEST_MAIN_LIBRARY gtest_main HINTS "${GTEST_ROOT}" PATH_SUFFIXES lib lib64 REQUIRED)
find_path(MOCKCPP_INCLUDE mockcpp/mockcpp.hpp HINTS "${MOCKCPP_ROOT}" PATH_SUFFIXES include REQUIRED)
find_library(MOCKCPP_LIBRARY mockcpp HINTS "${MOCKCPP_ROOT}" PATH_SUFFIXES lib lib64 REQUIRED)
find_path(BOOST_INCLUDE boost/typeof/typeof.hpp HINTS "${BOOST_ROOT}" PATH_SUFFIXES include REQUIRED)
find_package(Threads REQUIRED)

get_filename_component(REPO_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../../../.." ABSOLUTE)
add_executable(simd_gm_consumer_tests device_stubs.cpp
    ../ascendc_case_ascend950pr_9599/ascendc_case_ascend950pr_9599_aiv_basic/test_simd_vf_debug_publish.cpp )
target_compile_features(simd_gm_consumer_tests PRIVATE cxx_std_17)
target_compile_definitions(simd_gm_consumer_tests PRIVATE
    ASCENDC_CPU_DEBUG=1 ASCENDC_DUMP=0 ASCENDC_OOM=1 CFG_BUILD_DEBUG UT_TEST
    _GLIBCXX_USE_CXX11_ABI=${TEST_CXX11_ABI} __CCE_AICORE__=310 __CCE_KT_TEST__=1
    __DAV_C310_VEC__ __DAV_C310__ __NPU_ARCH__=3510 __callee__= __disable_kernel_type_autoinfer__=)
target_compile_options(simd_gm_consumer_tests PRIVATE -O0 -fno-access-control)
target_include_directories(simd_gm_consumer_tests PRIVATE
    "${REPO_ROOT}" "${REPO_ROOT}/include" "${REPO_ROOT}/include/basic_api"
    "${REPO_ROOT}/include/utils" "${REPO_ROOT}/impl" "${REPO_ROOT}/impl/basic_api"
    "${REPO_ROOT}/tests/api/basic_api/common" "${REPO_ROOT}/tests/api/common")
target_include_directories(simd_gm_consumer_tests SYSTEM PRIVATE
    "${CPU_STUB_INCLUDE}" "${GTEST_INCLUDE}" "${MOCKCPP_INCLUDE}" "${BOOST_INCLUDE}")
target_link_libraries(simd_gm_consumer_tests PRIVATE
    "${GTEST_MAIN_LIBRARY}" "${GTEST_LIBRARY}" "${MOCKCPP_LIBRARY}" Threads::Threads)
enable_testing()
add_test(NAME simd_gm_consumer COMMAND simd_gm_consumer_tests)
