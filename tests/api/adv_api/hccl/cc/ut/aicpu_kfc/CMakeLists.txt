find_package(Threads REQUIRED)

set(HCCL_BASE_DIR ${ASCENDC_DIR}/impl/adv_api/detail/hccl/cc/src/aicpu_kfc)

set(hccl_ut_mc2_server_kernel_list
    main.cc
    mc2_server_kernel_stub.cpp
    test_mc2_server_kernel.cpp
    ${HCCL_BASE_DIR}/framework/device/aicpu_kfc/aicpu_kfc_interface.cc
)

add_executable(hccl_ut_mc2_server_kernel ${hccl_ut_mc2_server_kernel_list})

add_dependencies(hccl_ut_mc2_server_kernel
    mc2_server
    ccl_kernel
)

target_include_directories(hccl_ut_mc2_server_kernel PRIVATE
    ${UT_COMMON_INCLUDE_LIST}
    $<TARGET_PROPERTY:ccl_kernel,INCLUDE_DIRECTORIES>
)

target_compile_options(hccl_ut_mc2_server_kernel PRIVATE
    -U_FORTIFY_SOURCE
    -Werror
    -fno-access-control
    $<$<COMPILE_LANGUAGE:CXX>:-std=c++14>
)

target_compile_definitions(hccl_ut_mc2_server_kernel PRIVATE
    AICPU_COMPILE
)

target_link_libraries(hccl_ut_mc2_server_kernel PRIVATE
    $<BUILD_INTERFACE:intf_llt_pub_basic>
    -Wl,--whole-archive
    -Wl,--no-whole-archive
    -lgcov
    -lrt
    -ldl
    Threads::Threads
)

run_llt_test(
    TARGET hccl_ut_mc2_server_kernel
    TASK_NUM 1
)
