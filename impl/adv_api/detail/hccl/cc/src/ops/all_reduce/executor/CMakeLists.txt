set(src_list
    ${CMAKE_CURRENT_SOURCE_DIR}/ins_v2_all_reduce_sole_executor.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/ins_v2_all_reduce_parallel_executor.cc
)

target_sources(mc2_client PRIVATE
    ${src_list}
)
