cmake_minimum_required(VERSION 3.16)
project(CApiHostHeaderChecker)

set(INCLUDE_PATHS ${ASCENDC_DIR})

if (TARGET tikcfw_headers)
    message(STATUS "tikcfw_headers target exists, gathering its include dirs")
    get_target_property(TIKCFW_HEADERS_INCLUDE_DIRS tikcfw_headers INTERFACE_INCLUDE_DIRECTORIES)
    message(STATUS "tikcfw_headers include dirs: ${TIKCFW_HEADERS_INCLUDE_DIRS}")
    if(TIKCFW_HEADERS_INCLUDE_DIRS)
        foreach(DIR ${TIKCFW_HEADERS_INCLUDE_DIRS})
            if(DIR MATCHES "^\\$<INSTALL_INTERFACE:.*>$")
                continue()
            endif()
            if(DIR MATCHES "^\\$<BUILD_INTERFACE:(.*)>$")
                list(APPEND INCLUDE_PATHS "${CMAKE_MATCH_1}")
            else()
                list(APPEND INCLUDE_PATHS "${DIR}")
            endif()
        endforeach()
    endif()
else()
    message(WARNING "tikcfw_headers target does not exist")
endif()

find_package(tikicpulib REQUIRED)
if(tikicpulib_FOUND)
    message(STATUS "tikicpulib found, adding include directories")
    message(STATUS "tikicpulib cmake path: ${tikicpulib_DIR}")
    get_target_property(TIKICPULIB_INCLUDE_DIRS tikicpulib_ascend910 INTERFACE_INCLUDE_DIRECTORIES)
    message(STATUS "tikicpulib include dirs: ${TIKICPULIB_INCLUDE_DIRS}")
    if(TIKICPULIB_INCLUDE_DIRS)
        list(APPEND INCLUDE_PATHS ${TIKICPULIB_INCLUDE_DIRS})
    endif()
endif()

# NPU checker configurations.  Keep the compiler target and C API architecture
# macros in sync so architecture-gated declarations are checked in the proper
# CCE environment.
set(CONFIGS_LIST
    "dav-c310-vec|3510|-D__DAV_C310__|-D__DAV_C310_VEC__"
    "dav-c310-cube|3510|-D__DAV_C310__|-D__DAV_C310_CUBE__"
    "dav-c220-vec|2201|-D__DAV_C220__|-D__DAV_C220_VEC__"
    "dav-c220-cube|2201|-D__DAV_C220__|-D__DAV_C220_CUBE__"
)

# ==========================================
# 测试文件
# ==========================================

set(ALL_TEST_FILES "")

file(GLOB_RECURSE ALL_TEST_FILES
    LIST_DIRECTORIES false
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

if(NOT ALL_TEST_FILES)
    message(FATAL_ERROR "No test files found to test.")
endif()

list(REMOVE_DUPLICATES ALL_TEST_FILES)
list(LENGTH ALL_TEST_FILES LEN)
message(STATUS "Total test files to test: ${LEN}")

# ==========================================
# 构建逻辑
# ==========================================

# 生成 Include 参数
set(INC_FLAGS "")
list(REMOVE_DUPLICATES INCLUDE_PATHS)
list(FILTER INCLUDE_PATHS EXCLUDE REGEX "^$")
foreach(INC ${INCLUDE_PATHS})
    list(APPEND INC_FLAGS "-I${INC}")
endforeach()
message(STATUS "INC_FLAGS: ${INC_FLAGS}")

# 函数: 为指定模式生成检查 targets
# 参数:  MODE_SUFFIX - 模式后缀 (如 "" 或 "_single")
#       MODE_DEFINES - 额外的宏定义列表
function(create_check_targets MODE_SUFFIX MODE_DEFINES)
    set(TARGET_VAR "CHECK_TARGETS_${NPU_MODE}${MODE_SUFFIX}")
    set(${TARGET_VAR})

    # 根据 NPU 模式设置编译器和参数
    set(COMPILER "bisheng")
    set(COMPILER_FLAGS -x cce -std=c++17 --cce-aicore-only -w)
    set(COMPILER_TYPE "NPU")

    message(STATUS "Creating ${COMPILER_TYPE}${MODE_SUFFIX} targets")

    # 遍历所有文件和所有配置
    foreach(TEST_FILE ${ALL_TEST_FILES})
        # 获取相对路径用于生成唯一的 Target 名字
        file(RELATIVE_PATH REL_TEST_FILE ${ASCENDC_DIR} ${TEST_FILE})

        # 清理文件名中的特殊字符
        string(REPLACE "/" "_" TEST_FILE_ID ${REL_TEST_FILE})
        string(REPLACE "." "_" TEST_FILE_ID ${TEST_FILE_ID})
        string(REPLACE "-" "_" TEST_FILE_ID ${TEST_FILE_ID})
        list(LENGTH CONFIGS_LIST CONFIGS_COUNT)
        math(EXPR CONFIGS_LAST "${CONFIGS_COUNT} - 1")
        foreach(IDX RANGE ${CONFIGS_LAST})
            list(GET CONFIGS_LIST ${IDX} CONFIG_STR)
            string(REPLACE "|" " " CONFIG_STR_SPACE "${CONFIG_STR}")
            separate_arguments(CONFIG_ITEMS UNIX_COMMAND "${CONFIG_STR_SPACE}")
            list(GET CONFIG_ITEMS 0 CONFIG_NAME)
            list(GET CONFIG_ITEMS 1 CONFIG_ARCH)
            list(LENGTH CONFIG_ITEMS CONFIG_ITEMS_LEN)
            if(CONFIG_ITEMS_LEN GREATER 2)
                list(SUBLIST CONFIG_ITEMS 2 -1 CONFIG_DEFINES)
            else()
                set(CONFIG_DEFINES "")
            endif()
            set(ARCH_FLAG "--cce-aicore-arch=${CONFIG_NAME}")

            # 创建子 Target
            set(TARGET_NAME "c_api_host_check_npu${MODE_SUFFIX}_${CONFIG_NAME}_${TEST_FILE_ID}")
            add_custom_target(${TARGET_NAME}
                COMMAND ${COMPILER} ${COMPILER_FLAGS} ${INC_FLAGS} ${MODE_DEFINES} ${CONFIG_DEFINES}
                    -fsyntax-only ${TEST_FILE} ${ARCH_FLAG}
                COMMENT "Checking C API host (${COMPILER_TYPE})${MODE_SUFFIX} ${CONFIG_NAME} ${REL_TEST_FILE}"
                VERBATIM
                USES_TERMINAL
            )

            list(APPEND ${TARGET_VAR} ${TARGET_NAME})
        endforeach()
    endforeach()

    # 创建汇总 Target
    set(RUN_ALL_TARGET "c_api_run_all_host_header_checks_npu${MODE_SUFFIX}")

    if(MODE_SUFFIX STREQUAL "")
        set(MODE_DESC "normal")
    else()
        string(REPLACE "_" " " MODE_DESC "${MODE_SUFFIX}")
        string(STRIP "${MODE_DESC}" MODE_DESC)
    endif()
    add_custom_target(${RUN_ALL_TARGET}
        DEPENDS ${${TARGET_VAR}}
        COMMENT "Running all C API host header checks (${COMPILER_TYPE} ${MODE_DESC} mode)..."
    )
endfunction()

# 模式 1: NPU 普通模式
set(NORMAL_DEFINES)
create_check_targets("" "${NORMAL_DEFINES}")

# 模式 2: NPU 单一头文件模式
set(SINGLE_DEFINES -DVERIFY_SINGLE_HEADER)
create_check_targets("_single" "${SINGLE_DEFINES}")

add_custom_target(c_api_run_all_host_header_checks ALL
    DEPENDS
        c_api_run_all_host_header_checks_npu
        c_api_run_all_host_header_checks_npu_single
    COMMENT "Running all C API host header checks (all modes, all architectures)..."
)
