set(GTEST_PARALLEL_TARGET "gtest-parallel")
set(DOWNLOAD_PATH ${CANN_3RD_LIB_PATH}/pkg)
set(SRC_PATH ${CANN_3RD_LIB_PATH}/${GTEST_PARALLEL_TARGET})

# 默认配置的boost不存在则下载
if (NOT EXISTS "${SRC_PATH}/${GTEST_PARALLEL_TARGET}/${GTEST_PARALLEL_TARGET}")
    set(DOWNLOAD_URL "https://gitcode.com/cann-src-third-party/gtest-parallel/releases/download/1.0/gtest-parallel.tar.gz")
    message(STATUS "Downloading ${GTEST_PARALLEL_TARGET} from ${DOWNLOAD_URL}")

    include(FetchContent)
    FetchContent_Declare(
        ${GTEST_PARALLEL_TARGET}
        URL ${DOWNLOAD_URL}
        URL_HASH SHA256=39fdd57f299f4484192ca6225badd2b3004955c96e3c88951b6d0799701a9e17
        DOWNLOAD_DIR ${DOWNLOAD_PATH}
        SOURCE_DIR ${SRC_PATH}  # 直接解压到此目录
        TLS_VERIFY OFF
    )
    FetchContent_MakeAvailable(${GTEST_PARALLEL_TARGET})
endif() 