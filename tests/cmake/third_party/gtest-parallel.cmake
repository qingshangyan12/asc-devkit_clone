set(GTEST_PARALLEL_TARGET "gtest-parallel")
set(DOWNLOAD_PATH ${CANN_3RD_LIB_PATH}/pkg)
set(SRC_PATH ${CANN_3RD_LIB_PATH}/${GTEST_PARALLEL_TARGET})

# 默认配置的boost不存在则下载
if (NOT EXISTS "${SRC_PATH}/${GTEST_PARALLEL_TARGET}/${GTEST_PARALLEL_TARGET}")
    set(DOWNLOAD_URL "https://github.com/google/gtest-parallel/archive/refs/heads/master.zip")
    message(STATUS "Downloading ${GTEST_PARALLEL_TARGET} from ${DOWNLOAD_URL}")

    include(FetchContent)
    FetchContent_Declare(
        ${GTEST_PARALLEL_TARGET}
        URL ${DOWNLOAD_URL}
        # URL_HASH SHA256=f55c340aa49763b1925ccf02b2e83f35fdcf634c9d5164a2acb87540173c741d
        DOWNLOAD_DIR ${DOWNLOAD_PATH}
        SOURCE_DIR ${SRC_PATH}  # 直接解压到此目录
        TLS_VERIFY OFF
    )
    FetchContent_MakeAvailable(${GTEST_PARALLEL_TARGET})
endif() 