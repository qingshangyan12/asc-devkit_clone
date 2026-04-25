add_subdirectory(new_topo_builder)

if(NOT BUILD_OPEN_PROJECT OR (BUILD_OPEN_PROJECT AND KERNEL_MODE))
target_sources(ccl_kernel PRIVATE
        virtual_topo.cc
)
endif()
