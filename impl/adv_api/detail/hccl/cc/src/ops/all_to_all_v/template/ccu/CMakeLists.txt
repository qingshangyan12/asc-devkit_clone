# ----------------------------------------------------------------------------------------------------------
add_subdirectory(kernel)

set(src_list
    ${CMAKE_CURRENT_SOURCE_DIR}/ccu_temp_kfc_half_all_to_all_v_mesh_1D.cc
)

target_sources(mc2_client PRIVATE
    ${src_list}
)
