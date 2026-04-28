/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This file is a part of the CANN Open Software.
 * Licensed under CANN Open Software License Agreement.
 */


#include <gtest/gtest.h>
#include <type_traits>
#include <cmath>
#include "kernel_operator.h"
#include "simt_compiler_stub.h"

using namespace std;
using namespace AscendC;


template <typename T>
class KernelVectorAdd {
public:
    __aicore__ inline KernelVectorAdd() {}

    __aicore__ inline void Process(__gm__ T* out, __gm__ T* src0, __gm__ T* src1,
                                    const int mode) {
        // SIMT 计算逻辑
        // TODO: 实现 SIMT 计算
    }
};



class VectorAddTestsuite : public testing::Test,
                              public testing::WithParamInterface<int> {
protected:
    void SetUp() {}
    void TearDown() {}
};



TEST_P(VectorAddTestsuite, VectorAddTestCase) {
    int mode = GetParam();
    int fpByteSize = 4;
    int shapeSize = 128;

    uint8_t dstGm[shapeSize * fpByteSize] = {0};
    uint8_t src0Gm[shapeSize * fpByteSize] = {0};
    uint8_t src1Gm[shapeSize * fpByteSize] = {0};

    // 初始化输入数据
    for (int i = 0; i < shapeSize; i++) {
        reinterpret_cast<float*>(src0Gm)[i] = static_cast<float>(i);
        reinterpret_cast<float*>(src1Gm)[i] = static_cast<float>(i + 1);
    }

    // 执行 SIMT Kernel
    // TODO: 实现 SIMT 调用

    // 验证结果
    // TODO: 添加验证逻辑
}

