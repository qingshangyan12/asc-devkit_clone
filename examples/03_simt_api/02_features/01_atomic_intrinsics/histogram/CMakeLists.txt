cmake_minimum_required(VERSION 3.16)

find_package(ASC REQUIRED)

project(histogram LANGUAGES ASC CXX)

add_executable(histogram
    histogram.asc
)

# ======================================================================================
# NPU 编译选项配置
#
# 说明：
#   - 需根据实际部署的 NPU 硬件架构选择对应的 `npu-arch` 参数。
# ======================================================================================
target_compile_options(histogram PRIVATE
    $<$<COMPILE_LANGUAGE:ASC>:--npu-arch=dav-3510 --enable-simt>
)

target_link_libraries(histogram PRIVATE
    m
)
