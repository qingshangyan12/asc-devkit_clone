set(src_list
    ${CMAKE_CURRENT_SOURCE_DIR}/ccu_kernel_kfc_all_reduce_mesh1d_mem2mem.cc
)

target_sources(mc2_client PRIVATE
    ${src_list}
)
