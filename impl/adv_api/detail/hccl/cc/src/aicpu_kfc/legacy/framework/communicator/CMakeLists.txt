if(NOT BUILD_OPEN_PROJECT OR (BUILD_OPEN_PROJECT AND KERNEL_MODE))
target_sources(ccl_kernel PRIVATE
    hccl_params_pub.cc
)
endif()

add_subdirectory(aicpu)
