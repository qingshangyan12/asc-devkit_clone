add_subdirectory(kernel)

set(src_list
    ${CMAKE_CURRENT_SOURCE_DIR}/ccu_temp_kfc_all_reduce_mesh_1D_mem2mem.cc
)

target_sources(mc2_client PRIVATE
    ${src_list}
)
