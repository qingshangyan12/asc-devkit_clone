if(NOT BUILD_OPEN_PROJECT OR (BUILD_OPEN_PROJECT AND KERNEL_MODE))
target_sources(ccl_kernel PRIVATE
        ins_all_to_all_sole_executor.cc
        ins_v2_all_to_all_sole_executor.cc
)
endif()