cmake_minimum_required(VERSION 3.16)

set(CMAKE_ASC_ARCHITECTURES "dav-2201")

find_package(ASC REQUIRED)

project(aclgraph_chevron_add_demo LANGUAGES ASC CXX)

add_executable(demo
    add.asc
)

# target_compile_options(demo PRIVATE
#     $<$<COMPILE_LANGUAGE:ASC>:--npu-arch=${CMAKE_ASC_ARCHITECTURES}>
# )
