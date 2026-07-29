find_package(ASC REQUIRED)
project(kernel_samples LANGUAGES ASC CXX)
add_executable(demo dcci_entire_all.asc)
target_compile_options(demo PRIVATE
    $<$<COMPILE_LANGUAGE:ASC>:--npu-arch=${CMAKE_ASC_ARCHITECTURES}>
)
