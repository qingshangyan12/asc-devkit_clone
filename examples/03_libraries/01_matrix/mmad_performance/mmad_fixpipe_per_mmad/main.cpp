#include <iostream>
#include <cstdlib>
#include <cstdint>
#include "acl/acl.h"
#include "acl/acl_base.h"  // 昇腾CANN内置的 float16 类型定义

constexpr uint32_t repeatTime = 1000;

void DoMatMul(uint32_t blockNum, void* stream, uint8_t* a, uint8_t* b, uint8_t* c, uint64_t* outTime, uint16_t m, uint16_t n, uint16_t k, uint32_t scenarioNum, uint32_t repeatTime);

int32_t main(int32_t argc, char *argv[])
{
    if (argc != 5) {
        std::cerr << "Usage: " << argv[0] << " <scenarioNum> <M> <N> <K>" << std::endl;
        std::cerr << "Example: " << argv[0] << " 6 64 32 128" << std::endl;
        return 1;
    }

    int32_t scenarioNum = 0;
    int32_t M = 0;
    int32_t N = 0;
    int32_t K = 0;

    try {
        scenarioNum = std::atoi(argv[1]);
        M = std::atoi(argv[2]);
        N = std::atoi(argv[3]);
        K = std::atoi(argv[4]);

        if (scenarioNum <= 0 || M <= 0 || N <= 0 || K <= 0) {
            throw std::invalid_argument("参数必须为正整数");
        }
    } catch (const std::exception& e) {
        std::cerr << "fail: " << e.what() << std::endl;
        return 1;
    }
    
    size_t aFileSize = 0;
    size_t bFileSize = 0;
    size_t cFileSize = 0;

    if (scenarioNum <= 4) {
        aFileSize = M * K * sizeof(int8_t);
        bFileSize = K * N * sizeof(int8_t);
        cFileSize = M * N * sizeof(int32_t);
    } else if (scenarioNum <= 8) {
        aFileSize = M * K * sizeof(aclFloat16);
        bFileSize = K * N * sizeof(aclFloat16);
        cFileSize = M * N * sizeof(float);
    } else {
        aFileSize = M * K * sizeof(float);
        bFileSize = K * N * sizeof(float);
        cFileSize = M * N * sizeof(float);
    }
    uint32_t numBlocks = 1;

    aclInit(nullptr);
    int32_t deviceId = 0;
    aclrtSetDevice(deviceId);
    aclrtStream stream = nullptr;
    aclrtCreateStream(&stream);

    uint8_t *aDevice;
    aclrtMalloc((void **)&aDevice, aFileSize, ACL_MEM_MALLOC_HUGE_FIRST);
    uint8_t *bDevice;
    aclrtMalloc((void **)&bDevice, bFileSize, ACL_MEM_MALLOC_HUGE_FIRST);
    uint8_t *cDevice;
    aclrtMalloc((void **)&cDevice, cFileSize, ACL_MEM_MALLOC_HUGE_FIRST);

    uint64_t *timeHost;
    uint64_t *timeDevice;
    size_t timeSize = repeatTime * sizeof(uint64_t);
    aclrtMallocHost((void **)(&timeHost), timeSize);
    aclrtMalloc((void **)&timeDevice, timeSize, ACL_MEM_MALLOC_HUGE_FIRST);

    // 执行
    DoMatMul(numBlocks, stream, aDevice, bDevice, cDevice, timeDevice, M, N, K, scenarioNum, repeatTime);
    aclrtSynchronizeStream(stream);
    DoMatMul(numBlocks, stream, aDevice, bDevice, cDevice, timeDevice, M, N, K, scenarioNum, repeatTime);
    aclrtSynchronizeStream(stream);

    // 验证性能
    // aclrtMemcpy(timeHost, timeSize, timeDevice, timeSize, ACL_MEMCPY_DEVICE_TO_HOST);
    // float durationNsSum = 0;
    // float durationNsMin = std::numeric_limits<float>::max();
    // float durationNsMax = std::numeric_limits<float>::lowest();
    // for (uint32_t i = 0; i < numBlocks; i++) {
    //     for (uint32_t j = 0; j < repeatTime; j++) {
    //         uint64_t deltaBlockCycles = timeHost[j];
    //         float durationNs = (float)deltaBlockCycles * 20;
    //         if (durationNs < 0.1) durationNs = 0.1;
    //         durationNsSum += durationNs;
    //         durationNsMin = std::min(durationNsMin, durationNs);
    //         durationNsMax = std::max(durationNsMax, durationNs);
    //     }
    // }
    // std::cout << "durationNsAvg: " << durationNsSum / numBlocks / repeatTime << std::endl;
    // std::cout << "durationNsMin: " << durationNsMin << std::endl;
    // std::cout << "durationNsMax: " << durationNsMax << std::endl;

    aclrtFree(aDevice);
    aclrtFree(bDevice);
    aclrtFree(cDevice);
    aclrtFree(timeDevice);
    aclrtFreeHost(timeHost);

    aclrtDestroyStream(stream);
    aclrtResetDevice(deviceId);
    aclFinalize();
    return 0;
}