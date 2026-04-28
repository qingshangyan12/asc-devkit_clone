/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This file is a part of the CANN Open Software.
 * Licensed under CANN Open Software License Agreement.
 */


#include <gtest/gtest.h>
#include "kernel_operator.h"

using namespace std;
using namespace AscendC;


template <typename T, Src1T>
class KernelAdd {
public:
    __aicore__ inline KernelAdd() {}

    __aicore__ inline void Init(__gm__ uint8_t* dstGm, __gm__ uint8_t* src0Gm,
                                 __gm__ uint8_t* src1Gm, uint32_t dataSize) {
        this->dataSize = dataSize;

        dstGlobal.SetGlobalBuffer(reinterpret_cast<__gm__ T*>(dstGm), dataSize);
        src0Global.SetGlobalBuffer(reinterpret_cast<__gm__ T*>(src0Gm), dataSize);
        src1Global.SetGlobalBuffer(reinterpret_cast<__gm__ Src1T*>(src1Gm), dataSize);

        pipe.InitBuffer(inQueueSrc0, 1, dataSize * sizeof(T));
        pipe.InitBuffer(inQueueSrc1, 1, dataSize * sizeof(Src1T));
        pipe.InitBuffer(outQueueDst, 1, dataSize * sizeof(T));
    }

    __aicore__ inline void Process() {
        CopyIn();
        Compute();
        CopyOut();
    }

private:
    __aicore__ inline void CopyIn() {
        LocalTensor<T> src0Local = inQueueSrc0.AllocTensor<T>();
        LocalTensor<Src1T> src1Local = inQueueSrc1.AllocTensor<Src1T>();
        DataCopy(src0Local, src0Global, dataSize);
        DataCopy(src1Local, src1Global, dataSize);
        inQueueSrc0.EnQue(src0Local);
        inQueueSrc1.EnQue(src1Local);
    }

    __aicore__ inline void Compute() {
        LocalTensor<T> src0Local = inQueueSrc0.DeQue<T>();
        LocalTensor<Src1T> src1Local = inQueueSrc1.DeQue<Src1T>();
        LocalTensor<T> dstLocal = outQueueDst.AllocTensor<T>();

        Add(dstLocal, src0Local, src1Local, dataSize);

        outQueueDst.EnQue<T>(dstLocal);
        inQueueSrc0.FreeTensor(src0Local);
        inQueueSrc1.FreeTensor(src1Local);
    }

    __aicore__ inline void CopyOut() {
        LocalTensor<T> dstLocal = outQueueDst.DeQue<T>();
        DataCopy(dstGlobal, dstLocal, dataSize);
        outQueueDst.FreeTensor(dstLocal);
    }

    TPipe pipe;
    TQue<TPosition::VECIN, 1> inQueueSrc0;
    TQue<TPosition::VECIN, 1> inQueueSrc1;
    TQue<TPosition::VECOUT, 1> outQueueDst;
    GlobalTensor<T> dstGlobal;
    GlobalTensor<T> src0Global;
    GlobalTensor<Src1T> src1Global;
    uint32_t dataSize;
};



template <typename T, typename Src1T = T>
__aicore__ inline void main_Add(uint8_t* dstGm, uint8_t* src0Gm,
                                        uint8_t* src1Gm, uint32_t dataSize) {
    KernelAdd<T, Src1T> op;
    op.Init(dstGm, src0Gm, src1Gm, dataSize);
    op.Process();
}



struct AddTestParams {
    uint32_t data_size;
    uint32_t data_bit_size;
    void (*cal_func)(uint8_t*, uint8_t*, uint8_t*, uint32_t);
};



class AddTestsuite : public testing::Test,
                              public testing::WithParamInterface<AddTestParams> {
protected:
    void SetUp() { AscendC::SetGCoreType(2); }
    void TearDown() { AscendC::SetGCoreType(0); }
};



INSTANTIATE_TEST_CASE_P(TEST_ADD, AddTestsuite,
    ::testing::Values(
        AddTestParams { 128, 2, main_Add<half> },
        AddTestParams { 256, 4, main_Add<float> }
    ));



TEST_P(AddTestsuite, AddTestCase) {
    auto param = GetParam();

    // 分配 GM 内存
    uint8_t dstGm[param.data_size * param.data_bit_size] = {0};
    uint8_t src0Gm[param.data_size * param.data_bit_size] = {0};
    uint8_t src1Gm[param.data_size * param.data_bit_size] = {0};

    // 初始化输入数据
    for (uint32_t i = 0; i < param.data_size; i++) {
        src0Gm[i] = static_cast<uint8_t>(i % 256);
        src1Gm[i] = static_cast<uint8_t>((i + 1) % 256);
    }

    // 执行测试
    param.cal_func(dstGm, src0Gm, src1Gm, param.data_size);

    // 验证结果
    for (uint32_t i = 0; i < param.data_size; i++) {
        // TODO: 添加实际验证逻辑
        // EXPECT_NEAR(...);
    }
}

