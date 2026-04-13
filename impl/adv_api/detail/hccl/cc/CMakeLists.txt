# -----------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# -----------------------------------------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.16.0)
option(BUILD_OPEN_PROJECT "Build open hccl project." ON)

set(HCCL_CC_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(HCCL_CC_INCLUDE_DIR ${ASCENDC_DIR}/include/adv_api/hccl/internal)
message(STATUS "KERNEL_MODE=${KERNEL_MODE}")
message(STATUS "BUILD_OPEN_PROJECT=${BUILD_OPEN_PROJECT}")
message(STATUS "AARCH_MODE=${AARCH_MODE}")

find_program(CCAHE_PROGRAM ccache)
if(CCAHE_PROGRAM)
    set(CMAKE_C_COMPILER_LAUNCHER ${CCAHE_PROGRAM})
    set(CMAKE_CXX_COMPILER_LAUNCHER ${CCAHE_PROGRAM})
endif()

if((BUILD_OPEN_PROJECT AND KERNEL_MODE) OR AARCH_MODE)
    # need to switch
    set(CMAKE_SYSTEM_NAME Linux)
    set(CMAKE_SYSTEM_PROCESSOR aarch64)
    #Change the path to the absolute path of the cross compilation tool after the toolkit package is decompressed
    # set cross compile toolchain dir
    set(TOOLCHAIN_DIR ${CUSTOM_ASCEND_CANN_PACKAGE_PATH}/toolkit/toolchain/hcc/)
    #Specify cross compiler
    message(STATUS "TOOLCHAIN_DIR=${TOOLCHAIN_DIR}")
    if (NOT "x${TOOLCHAIN_DIR}" STREQUAL "x")
        if (NOT IS_DIRECTORY ${TOOLCHAIN_DIR})
            message(FATAL_ERROR "specify cross compile toolchain directory(${TOOLCHAIN_DIR}) is not exist")
        endif()
    endif()
    set(CMAKE_CXX_COMPILER ${TOOLCHAIN_DIR}/bin/aarch64-target-linux-gnu-g++)
    set(CMAKE_C_COMPILER   ${TOOLCHAIN_DIR}/bin/aarch64-target-linux-gnu-gcc)
endif()

set(CMAKE_C_FLAGS_RELEASE "")
set(CMAKE_CXX_FLAGS_RELEASE "")

set(CMAKE_CXX_COMPILE_OBJECT
    "<CMAKE_CXX_COMPILER> <DEFINES> -D__FILE__='\"$(notdir $(abspath <SOURCE>))\"' -Wno-builtin-macro-redefined <INCLUDES> <FLAGS> -o <OBJECT> -c <SOURCE>"
)
set(CMAKE_C_COMPILE_OBJECT
    "<CMAKE_C_COMPILER> <DEFINES> -D__FILE__='\"$(notdir $(abspath <SOURCE>))\"' -Wno-builtin-macro-redefined <INCLUDES> <FLAGS> -o <OBJECT> -c <SOURCE>"
)

if(ENABLE_CUSTOM)
    # 自定义算子打包
    if(NOT CUSTOM_OPS_NAME)
        message(FATAL_ERROR "Custom ops name is not set")
    endif()
    if(NOT CUSTOM_OPS_PATH)
        message(FATAL_ERROR "Custom ops path is not set")
    endif()
    if(NOT CUSTOM_OPS_VENDOR)
        message(FATAL_ERROR "Custom ops vendor is not set")
    endif()

    if(NOT EXISTS ${CUSTOM_OPS_PATH})
        message(FATAL_ERROR "Custom ops path does not exist: ${CUSTOM_OPS_PATH}")
    endif()
    if(NOT EXISTS "${CUSTOM_OPS_PATH}/CMakeLists.txt")
        message(FATAL_ERROR "CMakeLists.txt not found. Please ensure it's a CMake project: ${CUSTOM_OPS_PATH}")
    endif()

    message(STATUS "Custom ops name: ${CUSTOM_OPS_NAME}")
    message(STATUS "Custom ops vendor: ${CUSTOM_OPS_VENDOR}")
    message(STATUS "Custom ops path: ${CUSTOM_OPS_PATH}")

    set(BUILD_DEVICE_DIR ${CMAKE_SOURCE_DIR}/build_device)
    set(BUILD_DIR ${CMAKE_SOURCE_DIR}/build)
    set(CUSTOM_OPS_OPP_PATH "opp/vendors/${CUSTOM_OPS_VENDOR}")
    set(CUSTOM_OPS_OPP_INC_PATH "${CUSTOM_OPS_OPP_PATH}/include")
    set(CUSTOM_OPS_OPP_LIB_PATH "${CUSTOM_OPS_OPP_PATH}/lib64")
    set(CUSTOM_OPS_OPP_SCRIPTS_PATH "${CUSTOM_OPS_OPP_PATH}/scripts")
    set(CUSTOM_OPS_OPP_AICPU_CONFIG_PATH "${CUSTOM_OPS_OPP_PATH}/aicpu/config")
    set(CUSTOM_OPS_OPP_AICPU_KERNEL_PATH "${CUSTOM_OPS_OPP_PATH}/aicpu/kernel")

    include(${HCCL_CC_DIR}/cmake/config.cmake)
    include(${HCCL_CC_DIR}/cmake/func.cmake)

    # AICPU Device 构建目标
    add_custom_target(custom_aicpu)
    # CPack 打包
    pack_custom()
    # 添加自定义算子工程
    add_subdirectory(${CUSTOM_OPS_PATH})

elseif(BUILD_OPEN_PROJECT)
    # include(${HCCL_CC_DIR}/cmake/third_party/openssl.cmake)
    include(${HCCL_CC_DIR}/cmake/third_party/ubengine.cmake)
    include(${HCCL_CC_DIR}/cmake/third_party/rdma-core.cmake)
    include(${HCCL_CC_DIR}/cmake/third_party/json.cmake)
    include(${HCCL_CC_DIR}/cmake/hcomm_utils.cmake)
    include(${HCCL_CC_DIR}/cmake/config_hcomm.cmake)
    # include(${HCCL_CC_DIR}/cmake/package.cmake)
    include(${HCCL_CC_DIR}/cmake/func_hcomm.cmake)
    # include(${HCCL_CC_DIR}/version.cmake)
    if(ENABLE_OPEN_SRC)
 	    check_pkg_build_deps("hcomm")
    endif()
    add_version_info_targets()

    set(HCCL_HEAD
        ${HCCL_CC_INCLUDE_DIR}/hcomm/hccl/hccl_comm.h
        ${HCCL_CC_INCLUDE_DIR}/hcomm/hccl/hccl_types.h
        src/aicpu_kfc/pub_inc/hccn_rping.h
        ${HCCL_CC_INCLUDE_DIR}/hcomm/hccl/hccl_rank_graph.h
        ${HCCL_CC_INCLUDE_DIR}/hcomm/hccl/hccl_res.h
        ${HCCL_CC_INCLUDE_DIR}/hcomm/hccl/hccl_sym_win.h
    )

    set(HCOMM_HEAD
         ${HCCL_CC_INCLUDE_DIR}/hcomm/hcomm_primitives.h
    )

    # todo:未找到ccl_kenel.json?
    set(HCCL_JSON
        cmake/scripts/ccl_kernel.json
    )

    # if(DEVICE_MODE)
    #     set(CCL_KERNEL_TAR_DIR ${CMAKE_CURRENT_SOURCE_DIR}/build_device/ccl_kernel_tar_pkg/)

    #     message(STATUS "Detected CUSTOM_SIGN_SCRIPT: ${CUSTOM_SIGN_SCRIPT}")
    #     message(STATUS "Detected ENABLE_SIGN: ${ENABLE_SIGN}")

    #     if(KERNEL_MODE)
    #         set(CCL_KERNEL_TAR_LIBS
    #             ${CCL_KERNEL_TAR_DIR}/aicpu_kernels_device/libccl_kernel_plf.so
    #             ${CCL_KERNEL_TAR_DIR}/aicpu_kernels_device/libccl_kernel.so
    #         )
    #         pack_targets_and_files(
    #             OUTPUT_TARGET "generate_device_aicpu_package"
    #             OUTPUT "aicpu_hcomm.tar.gz"
    #             FILES ${CCL_KERNEL_TAR_LIBS}
    #             MANIFEST "bin_hash.cfg"
    #             TAR_ROOT_DIR "aicpu_kernels_device"
    #         )
    #         add_dependencies(generate_device_aicpu_package ccl_kernel)
    #         sign_file(
    #             INPUT "aicpu_hcomm.tar.gz"
    #             CONFIG "${CMAKE_CURRENT_SOURCE_DIR}/scripts/sign/hcomm_check_cfg.xml"
    #             RESULT_VAR "aicpu_hcomm_sign_file"
    #             DEPENDS generate_device_aicpu_package
    #         )
    #         add_dependencies(sign_aicpu_hcomm generate_device_aicpu_package)
    #     endif()
    # endif()

    # todo:是否需要保留
    # if (NOT DEVICE_MODE)
    #     message(STATUS "Detected INSTALL_LIBRARY_DIR: ${INSTALL_LIBRARY_DIR}")

    #     set(BUILD_DEVICE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/build_device/)
    #     set(BUILD_HCCD_DIR ${CMAKE_CURRENT_SOURCE_DIR}/build_hccd/)
    #     add_custom_target(install_tar DEPENDS
    #         ${BUILD_DEVICE_DIR}/signatures/cann-hcomm-compat.tar.gz
    #         ${BUILD_DEVICE_DIR}/signatures/aicpu_hcomm.tar.gz
    #         ${BUILD_HCCD_DIR}/signatures/cann-hccd-compat.tar.gz
    #         ${BUILD_DEVICE_DIR}/src/framework/libaicpu_custom.so
    #         ${BUILD_DEVICE_DIR}/src/framework/libaicpu_custom.json
    #     )
    #     install(FILES ${BUILD_DEVICE_DIR}/signatures/cann-hcomm-compat.tar.gz
    #         DESTINATION ${INSTALL_DEVICE_TAR_DIR} OPTIONAL
    #     )
    #     install(FILES ${BUILD_DEVICE_DIR}/signatures/aicpu_hcomm.tar.gz
    #         DESTINATION ${INSTALL_DEVICE_TAR_DIR} OPTIONAL
    #     )
    #     install(FILES ${BUILD_HCCD_DIR}/signatures/cann-hccd-compat.tar.gz
    #         DESTINATION ${INSTALL_DEVICE_TAR_DIR} OPTIONAL
    #     )
    #     install(FILES ${BUILD_DEVICE_DIR}/src/framework/libaicpu_custom.so
    #         DESTINATION ${INSTALL_CCL_KERNEL_JSON_DIR} OPTIONAL
    #     )
    #     install(FILES ${BUILD_DEVICE_DIR}/src/framework/libaicpu_custom.json
    #         DESTINATION ${INSTALL_CCL_KERNEL_JSON_DIR} OPTIONAL
    #     )

    #     add_custom_target(install_ccl_kernel DEPENDS
    #         ${BUILD_DEVICE_DIR}/src/framework/libccl_kernel.so
    #     )
    #     install(FILES ${BUILD_DEVICE_DIR}/src/framework/libccl_kernel.so
    #         DESTINATION ${INSTALL_LIBRARY_DIR} OPTIONAL
    #     )
    #     add_dependencies(install_tar hcomm)
    #     add_dependencies(install_ccl_kernel hcomm)
    # endif()

    # 重复配置
    # install(FILES
    #     ${HCCL_HEAD}
    #     ${HCCL_JSON}
    #     DESTINATION ${INSTALL_INCLUDE_DIR}/hccl/ OPTIONAL
    # )
    install(FILES
        ${HCOMM_HEAD}
        DESTINATION ${INSTALL_INCLUDE_DIR}/ OPTIONAL
    )
    install(FILES
        ${HCCL_CC_INCLUDE_DIR}/hcomm/hcomm_primitives.h
        DESTINATION ${INSTALL_INCLUDE_DIR}/hcomm/ OPTIONAL
    )
    install(FILES
        # todo: 没有这个文件？
        ${HCCL_CC_INCLUDE_DIR}/hcomm/hcomm_res.h
        ${HCCL_CC_INCLUDE_DIR}/hcomm/hcomm_res_defs.h
        DESTINATION ${INSTALL_INCLUDE_DIR}/hcomm/ OPTIONAL
    )
    # todo:这些file的作用
    install(CODE [[
file(MAKE_DIRECTORY "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/hcomm/include/hccl")
file(REMOVE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/hcomm/include/hcomm_primitives.h")
file(REMOVE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/hcomm/include/hccl/hcomm_primitives.h")
file(CREATE_LINK "../hcomm/hcomm_primitives.h" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/hcomm/include/hccl/hcomm_primitives.h" SYMBOLIC)
file(REMOVE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/hcomm/include/hcomm_res_defs.h")
file(REMOVE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/hcomm/include/hccl/hcomm_res_defs.h")
file(CREATE_LINK "../hcomm/hcomm_res_defs.h" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/hcomm/include/hccl/hcomm_res_defs.h" SYMBOLIC)
]])

    set(HCCL_PKG_HEAD
        # todo: 一些文件没有，添加了hccl_diag.h
        ${HCCL_CC_INCLUDE_DIR}/hcomm/pkg_inc/hccl/base.h
        ${HCCL_CC_INCLUDE_DIR}/hcomm/pkg_inc/hccl/hccl_ex.h
        ${HCCL_CC_INCLUDE_DIR}/hcomm/pkg_inc/hccl/hccl_one_sided_services.h
        ${HCCL_CC_INCLUDE_DIR}/hcomm/pkg_inc/hccl/hccl_res_expt.h
        ${HCCL_CC_INCLUDE_DIR}/hcomm/pkg_inc/hccl/hcom.h
        ${HCCL_CC_INCLUDE_DIR}/hcomm/pkg_inc/hccl/hccl_diag.h
        ${HCCL_CC_INCLUDE_DIR}/hcomm/pkg_inc/hccl/hcomm_diag.h
        ${HCCL_CC_INCLUDE_DIR}/hcomm/pkg_inc/hccl/dtype_common.h
        ${HCCL_CC_INCLUDE_DIR}/hcomm/pkg_inc/hccl/workflow.h
        ${HCCL_CC_INCLUDE_DIR}/hcomm/pkg_inc/hccl/hccl_inner.h
        ${HCCL_CC_INCLUDE_DIR}/hcomm/pkg_inc/hccl/hccl_ctrl_plane.h
    )
    install(FILES
        ${HCCL_PKG_HEAD}
        DESTINATION ${INSTALL_PKG_INCLUDE_DIR}/hccl/ OPTIONAL
    )
    set(HCCL_PKG_CCU_HEAD
        ${HCCL_CC_INCLUDE_DIR}/hcomm/pkg_inc/hcomm/ccu/ccu_assist_pub.h
        ${HCCL_CC_INCLUDE_DIR}/hcomm/pkg_inc/hcomm/ccu/ccu_common.h
        ${HCCL_CC_INCLUDE_DIR}/hcomm/pkg_inc/hcomm/ccu/ccu_condition_v1.h
        ${HCCL_CC_INCLUDE_DIR}/hcomm/pkg_inc/hcomm/ccu/ccu_kernel_resource.h
        ${HCCL_CC_INCLUDE_DIR}/hcomm/pkg_inc/hcomm/ccu/ccu_datatype_v1.h
        ${HCCL_CC_INCLUDE_DIR}/hcomm/pkg_inc/hcomm/ccu/ccu_funccall_v1.h
        ${HCCL_CC_INCLUDE_DIR}/hcomm/pkg_inc/hcomm/ccu/ccu_instr_info_v1.h
        ${HCCL_CC_INCLUDE_DIR}/hcomm/pkg_inc/hcomm/ccu/ccu_interface_assist_v1.h
        ${HCCL_CC_INCLUDE_DIR}/hcomm/pkg_inc/hcomm/ccu/ccu_kernel_arg.h
        ${HCCL_CC_INCLUDE_DIR}/hcomm/pkg_inc/hcomm/ccu/ccu_kernel_signature.h
        ${HCCL_CC_INCLUDE_DIR}/hcomm/pkg_inc/hcomm/ccu/ccu_kernel.h
        ${HCCL_CC_INCLUDE_DIR}/hcomm/pkg_inc/hcomm/ccu/ccu_loopblock_v1.h
        ${HCCL_CC_INCLUDE_DIR}/hcomm/pkg_inc/hcomm/ccu/ccu_loopcall_v1.h
        ${HCCL_CC_INCLUDE_DIR}/hcomm/pkg_inc/hcomm/ccu/ccu_loopgroupcall_v1.h
        ${HCCL_CC_INCLUDE_DIR}/hcomm/pkg_inc/hcomm/ccu/ccu_microcode_v1.h
        ${HCCL_CC_INCLUDE_DIR}/hcomm/pkg_inc/hcomm/ccu/ccu_operator_v1.h
        ${HCCL_CC_INCLUDE_DIR}/hcomm/pkg_inc/hcomm/ccu/ccu_rep_arg_v1.h
        ${HCCL_CC_INCLUDE_DIR}/hcomm/pkg_inc/hcomm/ccu/ccu_rep_base_v1.h
        ${HCCL_CC_INCLUDE_DIR}/hcomm/pkg_inc/hcomm/ccu/ccu_rep_block_v1.h
        ${HCCL_CC_INCLUDE_DIR}/hcomm/pkg_inc/hcomm/ccu/ccu_rep_context_v1.h
        ${HCCL_CC_INCLUDE_DIR}/hcomm/pkg_inc/hcomm/ccu/ccu_rep_funcblock_v1.h
        ${HCCL_CC_INCLUDE_DIR}/hcomm/pkg_inc/hcomm/ccu/ccu_rep_funccall_v1.h
        ${HCCL_CC_INCLUDE_DIR}/hcomm/pkg_inc/hcomm/ccu/ccu_rep_jump_v1.h
        ${HCCL_CC_INCLUDE_DIR}/hcomm/pkg_inc/hcomm/ccu/ccu_rep_jumplabel_v1.h
        ${HCCL_CC_INCLUDE_DIR}/hcomm/pkg_inc/hcomm/ccu/ccu_rep_loopblock_v1.h
        ${HCCL_CC_INCLUDE_DIR}/hcomm/pkg_inc/hcomm/ccu/ccu_rep_loopcall_v1.h
        ${HCCL_CC_INCLUDE_DIR}/hcomm/pkg_inc/hcomm/ccu/ccu_rep_reference_manager_v1.h
        ${HCCL_CC_INCLUDE_DIR}/hcomm/pkg_inc/hcomm/ccu/ccu_rep_type_v1.h
        ${HCCL_CC_INCLUDE_DIR}/hcomm/pkg_inc/hcomm/ccu/ccu_repeat_v1.h
        ${HCCL_CC_INCLUDE_DIR}/hcomm/pkg_inc/hcomm/ccu/ccu_res_repo.h
        ${HCCL_CC_INCLUDE_DIR}/hcomm/pkg_inc/hcomm/ccu/ccu_task_arg_v1.h
        ${HCCL_CC_INCLUDE_DIR}/hcomm/pkg_inc/hcomm/ccu/ccu_task_param_v1.h
        ${HCCL_CC_INCLUDE_DIR}/hcomm/pkg_inc/hcomm/ccu/hccl_ccu_res.h
    )
    install(FILES
        ${HCCL_PKG_CCU_HEAD}
        DESTINATION ${INSTALL_PKG_INCLUDE_DIR}/hcomm/ccu/ OPTIONAL
    )
    # todo:这个文件没有
    set(HCOMM_PKG_HEAD
        ${HCCL_CC_INCLUDE_DIR}/hcomm/pkg_inc/hcomm/hcomm_primitives_expt.h
    )
    install(FILES
        ${HCOMM_PKG_HEAD}
        DESTINATION ${INSTALL_PKG_INCLUDE_DIR}/hcomm/ OPTIONAL
    )

    include(${HCCL_CC_DIR}/cmake/config.cmake)
    include(${HCCL_CC_DIR}/cmake/func.cmake)
    include(${HCCL_CC_DIR}/version.cmake)
    if(ENABLE_OPEN_SRC)
        check_pkg_build_deps("hccl")
    endif()
    add_subdirectory(src/aicpu_kfc)
    add_subdirectory(src)

    if(DEVICE_MODE)
        set(CCL_KERNEL_TAR_DIR ${CMAKE_SOURCE_DIR}/build_device/ccl_kernel_tar_pkg/)
        if(KERNEL_MODE)
            set(CCL_KERNEL_TAR_LIBS
                ${CCL_KERNEL_TAR_DIR}/aicpu_kernels_device/libmc2_server.so
                ${CCL_KERNEL_TAR_DIR}/aicpu_kernels_device/libmc2_kernel_compat.so
                # hcomm
                ${CCL_KERNEL_TAR_DIR}/aicpu_kernels_device/libccl_kernel_plf.so
                ${CCL_KERNEL_TAR_DIR}/aicpu_kernels_device/libccl_kernel.so 
            )
            pack_targets_and_files(
                OUTPUT_TARGET "generate_device_aicpu_package"
                OUTPUT "mc2_server.tar.gz"
                FILES ${CCL_KERNEL_TAR_LIBS}
                MANIFEST "bin_hash.cfg"
                TAR_ROOT_DIR "aicpu_kernels_device"
            )
            add_dependencies(generate_device_aicpu_package mc2_server)
            # hcomm
            add_dependencies(generate_device_aicpu_package ccl_kernel)
            sign_file(
                INPUT "mc2_server.tar.gz"
                CONFIG "${HCCL_CC_DIR}/scripts/sign/hccl_check_cfg.xml"
                RESULT_VAR "mc2_server_sign_file"
                DEPENDS generate_device_aicpu_package
            )
            add_dependencies(sign_mc2_server generate_device_aicpu_package)
        endif()
    endif()

    if (NOT DEVICE_MODE)
        message(STATUS "Detected INSTALL_LIBRARY_DIR: ${INSTALL_LIBRARY_DIR}")

        set(BUILD_DEVICE_DIR ${CMAKE_SOURCE_DIR}/build_device/)
        add_custom_target(install_tar DEPENDS
            ${BUILD_DEVICE_DIR}/signatures/mc2_server.tar.gz
            ${BUILD_DEVICE_DIR}/aicpu_custom/libmc2_server.so
        )
        install(FILES ${BUILD_DEVICE_DIR}/signatures/mc2_server.tar.gz
            DESTINATION ${INSTALL_DEVICE_TAR_DIR} OPTIONAL
        )
        install(FILES ${BUILD_DEVICE_DIR}/aicpu_custom/libmc2_server.so
            DESTINATION ${INSTALL_AICPU_KERNEL_JSON_DIR} OPTIONAL
        )
        add_dependencies(install_tar mc2_client)
    endif()

    list(APPEND HCCL_HEAD
        ${HCCL_CC_INCLUDE_DIR}/hccl.h
        ${HCCL_CC_INCLUDE_DIR}/hccl_mc2.h
    )

    # set(HCCL_JSON
    #     # hccl json 待补充
    # )

    install(FILES
        ${HCCL_HEAD}
        ${HCCL_JSON}
        DESTINATION ${INSTALL_INCLUDE_DIR}/hccl/ OPTIONAL
    )
endif()
