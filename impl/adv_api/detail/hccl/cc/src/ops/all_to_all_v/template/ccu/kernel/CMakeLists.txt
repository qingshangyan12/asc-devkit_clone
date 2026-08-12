# ----------------------------------------------------------------------------------------------------------
set(src_list
    ${CMAKE_CURRENT_SOURCE_DIR}/ccu_kernel_kfc_half_all_to_all_v_mesh1d.cc
)

target_sources(mc2_client PRIVATE
    ${src_list}
)
