set(HCCL_HCOMM_HEADER_DIR ${HCCL_CC_INCLUDE_DIR}/hcomm)
set(HCCL_BASE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(ORION_REAL_PATH ${CMAKE_CURRENT_SOURCE_DIR}/legacy)
set(HCOMM_REAL_PATH ${CMAKE_CURRENT_SOURCE_DIR}/..)

#组件间对外头文件目录
add_library(hccl_headers INTERFACE)
target_include_directories(hccl_headers INTERFACE
    $<BUILD_INTERFACE:${TOP_DIR}/hcomm/include>
    $<BUILD_INTERFACE:${TOP_DIR}/hcomm/include/hccl>
    $<INSTALL_INTERFACE:include>
    $<INSTALL_INTERFACE:include/hccl>
    $<INSTALL_INTERFACE:include/hccl/external>
    $<INSTALL_INTERFACE:include/hccl/external/hccl>
)

# 创建 ccl_kernel_plf共享库
add_library(ccl_kernel_plf  SHARED)
# 创建 ccl_kernel_plf_a静态库
add_library(ccl_kernel_plf_a  STATIC)

# 指定ccl_kernel_plf所需包含文件搜索路径
if(BUILD_OPEN_PROJECT)
    include(${HCCL_CC_DIR}/cmake/hcomm_utils.cmake)
    include(${HCCL_CC_DIR}/cmake/third_party/json.cmake)
    include(${HCCL_CC_DIR}/cmake/third_party/openssl.cmake)
    include(${HCCL_CC_DIR}/cmake/third_party/rdma-core.cmake)

    target_compile_definitions(ccl_kernel_plf PRIVATE
        OPEN_BUILD_PROJECT)

    target_compile_definitions(ccl_kernel_plf_a PRIVATE
        OPEN_BUILD_PROJECT)

    target_include_directories(ccl_kernel_plf PRIVATE
        ${HCCL_HCOMM_HEADER_DIR}
        ${HCCL_HCOMM_HEADER_DIR}/hccl
        ${HCCL_HCOMM_HEADER_DIR}/pkg_inc
        ${HCCL_HCOMM_HEADER_DIR}/pkg_inc/hccl
        ${HCCL_BASE_DIR}/pub_inc
        ${HCCL_BASE_DIR}/pub_inc/aicpu
        ${HCCL_BASE_DIR}/pub_inc/inner
        ${HCCL_BASE_DIR}/pub_inc/new
        ${HCCL_BASE_DIR}/platform/hccp/inc
        ${HCCL_BASE_DIR}/platform/hccp/inc/network
        # todo: 这个文件在什么地方
        ${RDMA_CORE_INCLUDE_DIR}

        # runtime头文件
        ${ASCEND_CANN_PACKAGE_PATH}/include/

        # mmpa头文件
        ${ASCEND_CANN_PACKAGE_PATH}/pkg_inc/mmpa/

        # 包间接口
        ${ASCEND_CANN_PACKAGE_PATH}/pkg_inc/
        ${ASCEND_CANN_PACKAGE_PATH}/pkg_inc/aicpu/
        ${ASCEND_CANN_PACKAGE_PATH}/pkg_inc/aicpu/common/
        ${ASCEND_CANN_PACKAGE_PATH}/pkg_inc/runtime/
        ${ASCEND_CANN_PACKAGE_PATH}/pkg_inc/profiling/
        ${ASCEND_CANN_PACKAGE_PATH}/pkg_inc/base/
        ${ASCEND_CANN_PACKAGE_PATH}/pkg_inc/dump/
        ${ASCEND_CANN_PACKAGE_PATH}/pkg_inc/trace/

        # driver头文件
        ${ASCEND_CANN_PACKAGE_PATH}/include/driver

        # 临时依赖头文件，待删除
        ${HCCL_HCOMM_HEADER_DIR}/externel_depends/tsch/
        # todo
        ${RDMA_CORE_INCLUDE_DIR}
        # todo:是否需要指定
        ${THIRD_PARTY_NLOHMANN_PATH}
    )

    target_include_directories(ccl_kernel_plf_a PRIVATE
        ${HCCL_HCOMM_HEADER_DIR}
        ${HCCL_HCOMM_HEADER_DIR}/hccl
        ${HCCL_HCOMM_HEADER_DIR}/pkg_inc
        ${HCCL_HCOMM_HEADER_DIR}/pkg_inc/hccl
        ${HCCL_HCOMM_HEADER_DIR}
        ${HCCL_HCOMM_HEADER_DIR}
        ${HCCL_BASE_DIR}/pub_inc
        ${HCCL_BASE_DIR}/pub_inc/aicpu
        ${HCCL_BASE_DIR}/pub_inc/inner
        ${HCCL_BASE_DIR}/pub_inc/new
        ${HCCL_BASE_DIR}/platform/hccp/inc
        ${HCCL_BASE_DIR}/platform/hccp/inc/network
        ${RDMA_CORE_INCLUDE_DIR}

        # runtime头文件
        ${ASCEND_CANN_PACKAGE_PATH}/include/

        # mmpa头文件
        ${ASCEND_CANN_PACKAGE_PATH}/pkg_inc/mmpa/

        # driver头文件
        ${ASCEND_CANN_PACKAGE_PATH}/include/driver

        # 包间接口
        ${ASCEND_CANN_PACKAGE_PATH}/pkg_inc/
        ${ASCEND_CANN_PACKAGE_PATH}/pkg_inc/aicpu/
        ${ASCEND_CANN_PACKAGE_PATH}/pkg_inc/aicpu/common/
        ${ASCEND_CANN_PACKAGE_PATH}/pkg_inc/runtime/
        ${ASCEND_CANN_PACKAGE_PATH}/pkg_inc/profiling/
        ${ASCEND_CANN_PACKAGE_PATH}/pkg_inc/base/
        ${ASCEND_CANN_PACKAGE_PATH}/pkg_inc/dump/
        ${ASCEND_CANN_PACKAGE_PATH}/pkg_inc/trace/

        # 临时依赖头文件，待删除
        ${HCCL_HCOMM_HEADER_DIR}/externel_depends/tsch/
        ${RDMA_CORE_INCLUDE_DIR}

        ${THIRD_PARTY_NLOHMANN_PATH}
    )

    target_link_directories(ccl_kernel_plf PRIVATE
        ${ASCEND_CANN_PACKAGE_PATH}/devlib/device/ # c_sec、mmpa、unified_dlog动态库搜索路径
    )

    target_link_directories(ccl_kernel_plf_a PRIVATE
        ${ASCEND_CANN_PACKAGE_PATH}/devlib/device/ # c_sec、mmpa、unified_dlog动态库搜索路径
    )
# todo:删除了非open project的定义
endif()

if(TARGET json)
    target_link_libraries(ccl_kernel_plf json)
    target_link_libraries(ccl_kernel_plf_a json)
    add_dependencies(ccl_kernel_plf json)
    add_dependencies(ccl_kernel_plf_a json)
endif()

if(BUILD_OPEN_PROJECT)
    add_dependencies(ccl_kernel_plf hccl_legacy)
endif()

target_compile_definitions(ccl_kernel_plf PRIVATE
    $<$<STREQUAL:${PRODUCT_SIDE},host>:_GLIBCXX_USE_CXX11_ABI=0>
)

target_compile_definitions(ccl_kernel_plf_a PRIVATE
    $<$<STREQUAL:${PRODUCT_SIDE},host>:_GLIBCXX_USE_CXX11_ABI=0>
)

# platform头文件
set(hccl_plf_include_list
    ${CMAKE_CURRENT_SOURCE_DIR}/platform
    ${CMAKE_CURRENT_SOURCE_DIR}/platform/inc
    ${CMAKE_CURRENT_SOURCE_DIR}/platform/inc/adapter
    ${CMAKE_CURRENT_SOURCE_DIR}/platform/inc/adapter/host
    ${CMAKE_CURRENT_SOURCE_DIR}/platform/task
    ${CMAKE_CURRENT_SOURCE_DIR}/platform/common/
    ${CMAKE_CURRENT_SOURCE_DIR}/platform/common/buffer_manager
    ${CMAKE_CURRENT_SOURCE_DIR}/platform/common/unfold_cache
    ${CMAKE_CURRENT_SOURCE_DIR}/platform/common/p2p_mgmt
    ${CMAKE_CURRENT_SOURCE_DIR}/platform/common/unique
    ${CMAKE_CURRENT_SOURCE_DIR}/platform/common/misc/network_manager
    ${CMAKE_CURRENT_SOURCE_DIR}/platform/common/misc/gradient_segment
    ${CMAKE_CURRENT_SOURCE_DIR}/platform/resource/transport
    ${CMAKE_CURRENT_SOURCE_DIR}/platform/resource/transport/device
    ${CMAKE_CURRENT_SOURCE_DIR}/platform/resource/transport/host
    ${CMAKE_CURRENT_SOURCE_DIR}/platform/resource/transport/heterog
    ${CMAKE_CURRENT_SOURCE_DIR}/platform/resource/transport/onesided
    ${CMAKE_CURRENT_SOURCE_DIR}/platform/resource/transport/onesided/device
    ${CMAKE_CURRENT_SOURCE_DIR}/platform/resource/mem
    ${CMAKE_CURRENT_SOURCE_DIR}/platform/resource/dispatcher_ctx
    ${CMAKE_CURRENT_SOURCE_DIR}/platform/resource/notify
    ${CMAKE_CURRENT_SOURCE_DIR}/platform/resource/socket
    ${CMAKE_CURRENT_SOURCE_DIR}/platform/resource/stream
    ${CMAKE_CURRENT_SOURCE_DIR}/platform/resource/rma_buffer
    ${CMAKE_CURRENT_SOURCE_DIR}/platform/resource/
    ${CMAKE_CURRENT_SOURCE_DIR}/platform/remote_access
    ${CMAKE_CURRENT_SOURCE_DIR}/platform/task
    ${CMAKE_CURRENT_SOURCE_DIR}/platform/tbe_vector_reduce
    ${CMAKE_CURRENT_SOURCE_DIR}/platform/aiv_communication    
)

# 设置ccl_kernel_plf 包含文件列表
set(ccl_kernel_plf_include_list
    # todo这个头文件又是什么
    ${include_list}
    ${HCCL_BASE_DIR}/hccl/hccl_comm/wrapper/notify/
    ${HCCL_BASE_DIR}/common/debug/profiling/inc/aicpu
    ${HCCL_BASE_DIR}/framework/common/src/aicpu
    ${hccl_plf_include_list}
)

# 指定ccl_kernel_plf所需头文件搜索路径
target_include_directories(ccl_kernel_plf PRIVATE
    ${ccl_kernel_plf_include_list}
)
# 指定ccl_kernel_plf_a所需头文件搜索路径
target_include_directories(ccl_kernel_plf_a PRIVATE
    ${ccl_kernel_plf_include_list}
)

# 给ccl_kernel_plf添加编译选项
target_compile_options(ccl_kernel_plf PRIVATE
    -Werror
    -Wall
    -fno-common
    -fno-strict-aliasing
    -pipe
    -O3
    -std=c++14
    -D_FORTIFY_SOURCE=2 -O2
    -fstack-protector-all
)

# 给ccl_kernel_plf_a添加编译选项
target_compile_options(ccl_kernel_plf_a PRIVATE
    -Werror
    -Wall
    -fno-common
    -fno-strict-aliasing
    -pipe
    -O3
    -std=c++14
    -ftrapv
    -D_FORTIFY_SOURCE=2 -O2
    -fPIC
    -fstack-protector-all
)
# 为ccl_kernel_plf添加编译定义
target_compile_definitions(ccl_kernel_plf PRIVATE
    HCCD
    CCL_KERNEL
    CCL_KERNEL_AICPU
    USE_AICORE_REDUCESUM
    USE_AICORE_GATHERV2
    USE_AICORE_GATHERV2_INFER
)
# 为ccl_kernel_plf_a添加编译定义
target_compile_definitions(ccl_kernel_plf_a PRIVATE
    HCCD
    CCL_KERNEL
    CCL_KERNEL_AICPU
    USE_AICORE_REDUCESUM
    USE_AICORE_GATHERV2
    USE_AICORE_GATHERV2_INFER
)
# 向ccl_kernel_plf添加链接器选项
target_link_options(ccl_kernel_plf PRIVATE
    -Wl,-z,relro,-z,now,-z,noexecstack
    -Wl,-Bsymbolic
    -Wl,--exclude-libs,ALL
    -s
)
# 向ccl_kernel_plf_a添加链接器选项
target_link_options(ccl_kernel_plf_a PRIVATE
    -Wl,-z,relro,-z,now,-z,noexecstack
    -Wl,-Bsymbolic
    -Wl,--exclude-libs,ALL
)

# 将库文件链接到目标ccl_kernel_plf  ccl_kernel_plf_a
if(BUILD_OPEN_PROJECT)
    target_link_libraries(ccl_kernel_plf
        -Wl,--no-as-needed
        c_sec
        aicpu_sharder
        mmpa
        -Wl,--as-needed
        -lrt
        -ldl
        -lpthread
    )

    target_link_libraries(ccl_kernel_plf_a
        -Wl,--no-as-needed
        c_sec
        aicpu_sharder
        mmpa
        -Wl,--as-needed
        -lrt
        -ldl
        -lpthread
    )
# todo:删除了非open的配置
endif()

add_subdirectory(framework)
add_subdirectory(algorithm)
add_subdirectory(platform)
add_subdirectory(common)

# 指定ccl_kernel_plf构建完成后安装到指定的目标位置
install(TARGETS  ccl_kernel_plf
    LIBRARY DESTINATION ${INSTALL_LIBRARY_DIR}  OPTIONAL
)
# 设置ccl_kernel_plf_a输出文件的名字
set_target_properties(ccl_kernel_plf_a
    PROPERTIES
    OUTPUT_NAME ccl_kernel_plf
)
# 指定ccl_kernel_plf_a构建完成后安装到指定的目标位置
install(TARGETS  ccl_kernel_plf_a
    ARCHIVE DESTINATION ${INSTALL_LIBRARY_DIR}  OPTIONAL
)
