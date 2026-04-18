# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ----------------------------------------------------------------------------------------------------------
function(run_llt_test)
    # 解析函数参数
    # 新增ENABLE_GTEST_PARALLEL参数，控制是否启用并行测试
    cmake_parse_arguments(LLT "" "TARGET;TASK_NUM;ENV_FILE;ENABLE_GTEST_PARALLEL" "" ${ARGN})

    # 检查必需的TARGET参数
    if(NOT LLT_TARGET)
        message(FATAL_ERROR "TARGET parameter is required for run_llt_test")
    endif()

    # 将TEST_MOD环境变量（逗号分隔的字符串）转换为CMake列表
    string(REPLACE "," ";" TARGET_LIST "${TEST_MOD}")
    # 移除可能的空项
    list(REMOVE_ITEM TARGET_LIST "")

    # 如果TEST_MOD不是"all"且当前目标不在目标列表中，则直接返回，不执行测试
    if (NOT "${TEST_MOD}" STREQUAL "all" AND NOT ${LLT_TARGET} IN_LIST TARGET_LIST)
        message(STATUS "Skipping test ${LLT_TARGET}, not in TEST_MOD: ${TEST_MOD}")
        return()
    endif()


    # 设置是否启用gtest_parallel的默认值
    if(NOT DEFINED LLT_ENABLE_GTEST_PARALLEL)
        set(LLT_ENABLE_GTEST_PARALLEL OFF)
    else()
        set(LLT_ENABLE_GTEST_PARALLEL ON)
    endif()

    # 检测gtest_parallel是否可用
    set(GTEST_PARALLEL_AVAILABLE OFF)
    set(GTEST_PARALLEL_COMMAND "")

    set(LLT_TASK_NUM 1)

    if(LLT_ENABLE_GTEST_PARALLEL)
        # 查找gtest_parallel可执行文件
        message("dddddddddddd ${CANN_3RD_LIB_PATH}")
        find_program(GTEST_PARALLEL_EXE gtest-parallel
            HINTS /usr/local/bin /usr/bin /opt/local/bin ${CANN_3RD_LIB_PATH}/gtest-parallel
            DOC "Path to gtest-parallel executable"
        )

        if(GTEST_PARALLEL_EXE)
            set(GTEST_PARALLEL_AVAILABLE ON)
            message(STATUS "Found gtest-parallel: ${GTEST_PARALLEL_EXE}")

            # 构建gtest_parallel命令
            # 基本参数：
            #   --workers: 指定并行任务数
            #   --output_dir: 测试输出目录
            #   --gtest_output: 设置Google Test输出格式
            set(TEST_OUTPUT_DIR ${CMAKE_BINARY_DIR}/test_results/${LLT_TARGET})
            file(MAKE_DIRECTORY ${TEST_OUTPUT_DIR})
            
            set(LLT_TASK_NUM 16)

            set(GTEST_PARALLEL_COMMAND
                ${GTEST_PARALLEL_EXE} $<TARGET_FILE:${LLT_TARGET}>
                --workers=${LLT_TASK_NUM}
                --output_dir=${TEST_OUTPUT_DIR}
                # --gtest_output=xml:${TEST_OUTPUT_DIR}/${LLT_TARGET}_results.xml
                --retry_failed=3  # 失败重试次数
            )
            message(WARNING "GTEST_PARALLEL_COMMAND "
                "${GTEST_PARALLEL_EXE} $<TARGET_FILE:${LLT_TARGET}>"
                "--workers=${LLT_TASK_NUM}"
                "--output_dir=${TEST_OUTPUT_DIR}"
                "--gtest_output=xml:${TEST_OUTPUT_DIR}/${LLT_TARGET}_results.xml"
                "--retry_failed=3"  # 失败重试次数
            )
        else()
            message(STATUS "gtest-parallel not found, falling back to serial execution")
        endif()
    endif()

    # 2. 构建测试执行命令
    # 根据是否启用gtest_parallel构建不同的测试执行命令
    if(GTEST_PARALLEL_AVAILABLE AND LLT_TASK_NUM GREATER 1)
        message(STATUS "Configuring ${LLT_TARGET} with gtest-parallel (workers=${LLT_TASK_NUM})")
        set(TEST_EXECUTION_COMMAND "${GTEST_PARALLEL_COMMAND}")
    else()
        # 串行执行
        message(STATUS "Configuring ${LLT_TARGET} for serial execution")
        set(TEST_EXECUTION_COMMAND "$<TARGET_FILE:${LLT_TARGET}>")
    endif()

    # 3. ASAN内存检测配置
    if (ENABLE_ASAN)
        message(STATUS "Configuring ASAN for target: ${LLT_TARGET}")

        # 获取当前C编译器的版本号
        execute_process(COMMAND ${CMAKE_C_COMPILER} -dumpversion OUTPUT_VARIABLE GCC_MAJOR)
        # 提取版本号中的数字部分
        string(REGEX MATCHALL "[0-9]+" GCC_MAJOR ${GCC_MAJOR})

        # 根据系统架构设置不同的ASAN库路径
        if("${CMAKE_HOST_SYSTEM_PROCESSOR}" STREQUAL "x86_64")
            # x86_64架构的ASAN库路径
            set(LD_PRELOAD_ "LD_PRELOAD=/usr/lib/gcc/x86_64-linux-gnu/${GCC_MAJOR}/libasan.so:/usr/lib/gcc/x86_64-linux-gnu/${GCC_MAJOR}/libstdc++.so")
        else()
            # aarch64架构的ASAN库路径
            set(LD_PRELOAD_ "LD_PRELOAD=/usr/lib/gcc/aarch64-linux-gnu/${GCC_MAJOR}/libasan.so:/usr/lib/gcc/aarch64-linux-gnu/${GCC_MAJOR}/libstdc++.so")
        endif()

        # 设置ASAN选项
        # detect_leaks=0: 禁用内存泄漏检测
        # halt_on_error=0: 发生错误时不停止程序
        set(ASAN_OPTIONS_ "ASAN_OPTIONS=detect_leaks=0:halt_on_error=0")

        # 添加自定义命令，在构建完成后运行测试
        add_custom_command(
            TARGET ${LLT_TARGET} POST_BUILD
            COMMAND export LD_LIBRARY_PATH=$ENV{LD_LIBRARY_PATH} && ${LD_PRELOAD_} && ulimit -s 32768 && ${ASAN_OPTIONS_} && ${TEST_EXECUTION_COMMAND}
            COMMENT "Run ${LLT_TARGET} with asan GTEST_PARALLEL_AVAILABLE=${GTEST_PARALLEL_AVAILABLE} (parallel workers=${LLT_TASK_NUM})"
        )
    else()

        # add_custom_command(
        #     TARGET ${LLT_TARGET} POST_BUILD
        #     COMMAND export LD_LIBRARY_PATH=$ENV{LD_LIBRARY_PATH} && $<TARGET_FILE:${LLT_TARGET}>
        #     COMMENT "Run ${LLT_TARGET}"
        # )

        # 不启用ASAN时，运行测试
        message(WARNING "DDDDDDDDDDD ${TEST_EXECUTION_COMMAND}")

        # file(GLOB MY_FILES LIST_DIRECTORIES false RELATIVE /usr/local/bin "*")
        # file(EXIST /usr/local/bin/gtest-parallel)
        add_custom_command(
            TARGET ${LLT_TARGET} POST_BUILD
            COMMAND export LD_LIBRARY_PATH=$ENV{LD_LIBRARY_PATH} && ${TEST_EXECUTION_COMMAND}
            # COMMAND export LD_LIBRARY_PATH=$ENV{LD_LIBRARY_PATH} && ${GTEST_PARALLEL_EXE} $<TARGET_FILE:${LLT_TARGET}>
            #     --workers=${LLT_TASK_NUM}
            #     --output_dir=${TEST_OUTPUT_DIR}
            #     --dump_json_test_results=${TEST_OUTPUT_DIR}/${LLT_TARGET}_results.json
            #     # --gtest_output=xml:${TEST_OUTPUT_DIR}/${LLT_TARGET}_results.xml
            #     --retry_failed=3  # 失败重试次数
            COMMENT "Run ${LLT_TARGET} GTEST_PARALLEL_AVAILABLE=${GTEST_PARALLEL_AVAILABLE} (parallel workers=${LLT_TASK_NUM})"
        )
    endif()

    # 4. GCOV覆盖率收集配置
    if(ENABLE_GCOV)
        message(STATUS "Configuring GCOV coverage collection for target: ${LLT_TARGET}")

        # 覆盖率数据收集目标名称
        set(_collect_coverage_data_target collect_coverage_data)

        # 获取构建目录的父目录路径
        get_filename_component(_ops_builtin_bin_path ${CMAKE_BINARY_DIR} DIRECTORY)

        # 获取CANN包路径的父目录真实路径
        get_filename_component(_pkg_path "${ASCEND_CANN_PACKAGE_PATH}/../" REALPATH)

        # 设置覆盖率报告相关路径
        set(_cov_report ${CMAKE_BINARY_DIR}/cov_report)
        set(_cov_html ${_cov_report})
        set(_cov_data ${_cov_report}/coverage.info)

        # 如果覆盖率收集目标还不存在，则创建它
        if (NOT TARGET ${_collect_coverage_data_target})
            add_custom_target(${_collect_coverage_data_target} ALL
                # 执行覆盖率收集脚本
                COMMAND bash ${GENERATE_CPP_COV} ${_ops_builtin_bin_path} ${_cov_data} ${_cov_html} ${_pkg_path}
                COMMENT "Run collect coverage data"
            )
        endif()

        # 添加依赖关系，确保测试在覆盖率收集之前运行
        add_dependencies(${_collect_coverage_data_target} ${LLT_TARGET})
    endif()

    # 5. 创建测试摘要输出目标（可选）
    if(GTEST_PARALLEL_AVAILABLE)
        set(TEST_SUMMARY_TARGET ${LLT_TARGET}_summary)
        add_custom_target(${TEST_SUMMARY_TARGET}
            COMMAND echo "Test Summary for ${LLT_TARGET}:"
            COMMAND echo "  Parallel workers: ${LLT_TASK_NUM}"
            COMMAND echo "  Output directory: ${CMAKE_BINARY_DIR}/test_results/${LLT_TARGET}"
            COMMAND echo "  XML results: ${CMAKE_BINARY_DIR}/test_results/${LLT_TARGET}/${LLT_TARGET}_results.xml"
            COMMAND echo "  gtest-parallel used: YES"
            COMMENT "Display test configuration summary for ${LLT_TARGET}"
        )
        add_dependencies(${TEST_SUMMARY_TARGET} ${LLT_TARGET})
    endif()
endfunction(run_llt_test)

function(run_python_llt_test)
    cmake_parse_arguments(PYTHON "" "TARGET;SRC_FILES_DIR;TEST_FILES_DIR;EXPORT_PYTHONPATH;COVERAGERC_DIR;ENV_FILE;ENV_PARAMS;PYVERSION;PRE_TEST_COMMAND;TASK_NUM" "DEPENDS" ${ARGN})
    add_custom_target(${PYTHON_TARGET} ALL DEPENDS ${CMAKE_INSTALL_PREFIX}/${PYTHON_TARGET}.timestamp)

    if(NOT DEFINED PYTHON_PYVERSION)
        set(PYTHON_PYVERSION python3)
    endif()

    if((NOT DEFINED PYTHON_EXPORT_PYTHONPATH) OR (PYTHON_EXPORT_PYTHONPATH STREQUAL ""))
        set(PY_EXPORT_PYTHONPATH \"\")
    else()
        set(PY_EXPORT_PYTHONPATH ${PYTHON_EXPORT_PYTHONPATH})
    endif()

    if((NOT DEFINED PYTHON_COVERAGERC_DIR) OR (PYTHON_COVERAGERC_DIR STREQUAL ""))
        set(PY_COVERAGERC_DIR \"\")
    else()
        set(PY_COVERAGERC_DIR ${PYTHON_COVERAGERC_DIR})
    endif()

    if((NOT DEFINED PYTHON_ENV_PARAMS) OR (PYTHON_ENV_PARAMS STREQUAL ""))
        set(PY_ENV_PARAMS \"\")
    else()
        set(PY_ENV_PARAMS ${PYTHON_ENV_PARAMS})
    endif()

    if((NOT DEFINED PYTHON_ENV_FILE) OR (PYTHON_ENV_FILE STREQUAL ""))
        set(PY_ENV_FILE \"\")
    else()
        set(PY_ENV_FILE ${PYTHON_ENV_FILE})
    endif()

    if((NOT DEFINED PYTHON_PRE_TEST_COMMAND) OR (PYTHON_PRE_TEST_COMMAND STREQUAL ""))
        set(PY_PRE_TEST_COMMAND "echo>null")
    else()
        set(PY_PRE_TEST_COMMAND ${PYTHON_PRE_TEST_COMMAND})
    endif()

    if((NOT DEFINED PYTHON_SRC_FILES_DIR) OR (PYTHON_SRC_FILES_DIR STREQUAL ""))
        message(FATAL_ERROR "NOT DEFINED PYTHON_SRC_FILES_DIR")
    else()
        set(PY_SRC_FILES_DIR ${PYTHON_SRC_FILES_DIR})
        string(REPLACE "," ";" PYTHON_SRC_FILES_DIR ${PY_SRC_FILES_DIR})
    endif()

    if((NOT DEFINED PYTHON_TEST_FILES_DIR) OR (PYTHON_TEST_FILES_DIR STREQUAL ""))
        message(FATAL_ERROR "NOT DEFINED PYTHON_TEST_FILES_DIR")
    else()
        set(PY_TEST_FILES_DIR ${PYTHON_TEST_FILES_DIR})
        string(REPLACE "," ";" PYTHON_TEST_FILES_DIR ${PY_TEST_FILES_DIR})
    endif()

    if((NOT DEFINED PYTHON_TARGET) OR (PYTHON_TARGET STREQUAL ""))
        message(FATAL_ERROR "NOT DEFINED PYTHON_TARGET")
    endif()

    if((NOT DEFINED LLT_RUN_MOD) OR (LLT_RUN_MOD STREQUAL ""))
        set(LLT_RUN_MOD single)
    endif()

    if(NOT DEFINED PYTHON_TASK_NUM)
        set(PYTHON_TASK_NUM 8)
    endif()
    if(DEFINED SKIP_EXECUTE)
        set(LLT_EXECUTE_COMMAND echo "skip execute ${PYTHON_TARGET}" )
    else()
        set(LLT_EXECUTE_COMMAND bash ${ASCENDC_DIR}/tests/cmake/scripts/python_llt_run_and_check.sh ${CMAKE_INSTALL_PREFIX} ${PYTHON_TARGET}
        ${PY_SRC_FILES_DIR} ${PY_TEST_FILES_DIR} ${PY_ENV_FILE} ${PY_ENV_PARAMS}
        ${PY_EXPORT_PYTHONPATH} ${PY_COVERAGERC_DIR} ${PYTHON_PYVERSION} "${LLT_RUN_MOD}" "${PYTHON_TASK_NUM}")
    endif()
    add_custom_command(
        OUTPUT ${CMAKE_INSTALL_PREFIX}/${PYTHON_TARGET}.timestamp
        COMMAND echo "PRE_TEST_COMMAND: ${PY_PRE_TEST_COMMAND}"
        COMMAND bash -c "${PY_PRE_TEST_COMMAND}"
        COMMAND echo "execute python_llt_run_and_check.sh starttime:"
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_INSTALL_PREFIX}/${PACKAGE}_report
        COMMAND ${LLT_EXECUTE_COMMAND}
        COMMAND echo "execute python_llt_run_and_check.sh end."
        COMMAND echo "build python ${PACKAGE} test successfully"
        COMMAND touch ${CMAKE_INSTALL_PREFIX}/${PYTHON_TARGET}.timestamp
        DEPENDS ${PYTHON_SRC_FILES_DIR} ${PYTHON_TEST_FILES_DIR} ${PYTHON_ENV_FILE} ${PYTHON_DEPENDS}
        WORKING_DIRECTORY ${ASCENDC_DIR}
    )

endfunction(run_python_llt_test)