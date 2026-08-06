set(src_list
    ${CMAKE_CURRENT_SOURCE_DIR}/ins_temp_all_reduce_mesh_1D_one_shot.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/ins_temp_all_reduce_mesh_1D_two_shot_mesh_chunk.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/ins_temp_all_reduce_aicpu_reduce_nhr.cc
)

target_sources(mc2_client PRIVATE
    ${src_list}
)
