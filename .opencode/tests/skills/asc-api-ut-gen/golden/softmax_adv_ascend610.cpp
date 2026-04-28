/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This file is a part of the CANN Open Software.
 * Licensed under CANN Open Software License Agreement.
 */


#include <gtest/gtest.h>
#include "kernel_operator.h"

using namespace std;
using namespace AscendC;


template <typename T1, typename T2>
class KernelSoftmax {
public:
    __aicore__ inline KernelSoftmax() {}

    __aicore__ inline void Init(__gm__ uint8_t* srcGm, __gm__ uint8_t* dstGm,
                                 uint32_t height, uint32_t width) {
        this->height = height;
        this->width = width;

        srcGlobal.SetGlobalBuffer(reinterpret_cast<__gm__ T1*>(srcGm), height * width);
        dstGlobal.SetGlobalBuffer(reinterpret_cast<__gm__ T2*>(dstGm), height * width);

        pipe.InitBuffer(inQueueSrc, 1, height * width * sizeof(T1));
        pipe.InitBuffer(outQueueDst, 1, height * width * sizeof(T2));
    }

    __aicore__ inline void Process() {
        CopyIn();
        Compute();
        CopyOut();
    }

private:
    __aicore__ inline void CopyIn() {
        LocalTensor<T1> srcLocal = inQueueSrc.AllocTensor<T1>();
        DataCopy(srcLocal, srcGlobal, height * width);
        inQueueSrc.EnQue(srcLocal);
    }

    __aicore__ inline void Compute() {
        LocalTensor<T1> srcLocal = inQueueSrc.DeQue<T1>();
        LocalTensor<T2> dstLocal = outQueueDst.AllocTensor<T2>();

        // 调用高阶 API
        Softmax<T1, T2>(dstLocal, srcLocal, {height, width});

        outQueueDst.EnQue<T2>(dstLocal);
        inQueueSrc.FreeTensor(srcLocal);
    }

    __aicore__ inline void CopyOut() {
        LocalTensor<T2> dstLocal = outQueueDst.DeQue<T2>();
        DataCopy(dstGlobal, dstLocal, height * width);
        outQueueDst.FreeTensor(dstLocal);
    }

    TPipe pipe;
    TQue<TPosition::VECIN, 1> inQueueSrc;
    TQue<TPosition::VECOUT, 1> outQueueDst;
    GlobalTensor<T1> srcGlobal;
    GlobalTensor<T2> dstGlobal;
    uint32_t height, width;
};



struct SoftmaxTestParams {
    uint32_t typeSize;
    uint32_t height;
    uint32_t width;
    void (*cal_func)(uint8_t*, uint8_t*, uint32_t, uint32_t);
};



class SoftmaxTestsuite : public testing::Test,
                              public testing::WithParamInterface<SoftmaxTestParams> {
protected:
    void SetUp() { AscendC::SetGCoreType(2); }
    void TearDown() { AscendC::SetGCoreType(0); }
};



INSTANTIATE_TEST_CASE_P(TEST_SOFTMAX, SoftmaxTestsuite,
    ::testing::Values(
        SoftmaxTestParams { 2, 8, 128, main_Softmax<half> }
    ));



TEST_P(SoftmaxTestsuite, SoftmaxTestCase) {
    auto param = GetParam();

    uint32_t dataSize = param.height * param.width;

    // 分配内存
    uint8_t srcGm[dataSize * param.typeSize] = {0};
    uint8_t dstGm[dataSize * param.typeSize] = {0};

    // 初始化输入数据
    for (uint32_t i = 0; i < dataSize; i++) {
        srcGm[i] = static_cast<uint8_t>(i % 256);
    }

    // 执行测试
    param.cal_func(srcGm, dstGm, param.height, param.width);

    // 验证结果
    // TODO: 添加实际验证逻辑
}

