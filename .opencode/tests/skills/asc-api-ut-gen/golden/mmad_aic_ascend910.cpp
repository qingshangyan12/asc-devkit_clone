/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This file is a part of the CANN Open Software.
 * Licensed under CANN Open Software License Agreement.
 */


#include <gtest/gtest.h>
#include "kernel_operator.h"

using namespace std;
using namespace AscendC;


template <typename Src0T, typename Src1T, typename DstT, typename L1OutT>
class KernelMmad {
public:
    __aicore__ inline KernelMmad() {}

    __aicore__ inline void Init(__gm__ uint8_t* a, __gm__ uint8_t* b,
                                 __gm__ uint8_t* c,
                                 uint16_t mVal, uint16_t kVal, uint16_t nVal) {
        this->m = mVal;
        this->k = kVal;
        this->n = nVal;

        aGM.SetGlobalBuffer(reinterpret_cast<__gm__ Src0T*>(a));
        bGM.SetGlobalBuffer(reinterpret_cast<__gm__ Src1T*>(b));
        cGM.SetGlobalBuffer(reinterpret_cast<__gm__ DstT*>(c));

        pipe.InitBuffer(inQueueA1, 1, m * k * sizeof(Src0T));
        pipe.InitBuffer(inQueueB1, 1, k * n * sizeof(Src1T));
        pipe.InitBuffer(outQueueCO1, 1, m * n * sizeof(L1OutT));
    }

    __aicore__ inline void Process() {
        CopyIn();
        Compute();
        CopyOut();
    }

private:
    __aicore__ inline void CopyIn() {
        LocalTensor<Src0T> a1Local = inQueueA1.AllocTensor<Src0T>();
        LocalTensor<Src1T> b1Local = inQueueB1.AllocTensor<Src1T>();
        DataCopy(a1Local, aGM, m * k);
        DataCopy(b1Local, bGM, k * n);
        inQueueA1.EnQue(a1Local);
        inQueueB1.EnQue(b1Local);
    }

    __aicore__ inline void Compute() {
        LocalTensor<Src0T> a1Local = inQueueA1.DeQue<Src0T>();
        LocalTensor<Src1T> b1Local = inQueueB1.DeQue<Src1T>();
        LocalTensor<L1OutT> co1Local = outQueueCO1.AllocTensor<L1OutT>();

        Mmad(co1Local, a1Local, b1Local, {m, k, n}, {1, 1, 1});

        outQueueCO1.EnQue<L1OutT>(co1Local);
        inQueueA1.FreeTensor(a1Local);
        inQueueB1.FreeTensor(b1Local);
    }

    __aicore__ inline void CopyOut() {
        LocalTensor<L1OutT> co1Local = outQueueCO1.DeQue<L1OutT>();
        DataCopy(cGM, co1Local, m * n);
        outQueueCO1.FreeTensor(co1Local);
    }

    TPipe pipe;
    TQue<TPosition::A1, 1> inQueueA1;
    TQue<TPosition::B1, 1> inQueueB1;
    TQue<TPosition::CO1, 1> outQueueCO1;
    GlobalTensor<Src0T> aGM;
    GlobalTensor<Src1T> bGM;
    GlobalTensor<DstT> cGM;
    uint16_t m, k, n;
};



class TEST_MMAD : public testing::Test {
protected:
    void SetUp() {
        g_coreType = AscendC::AIC_TYPE;
    }
    void TearDown() {
        AscendC::CheckSyncState();
        g_coreType = AscendC::MIX_TYPE;
    }
};



TEST_F(TEST_MMAD, Mmad_Basic) {
    uint16_t m = 16;
    uint16_t n = 16;
    uint16_t k = 64;

    // 跳过非目标核心
    if ASCEND_IS_AIV {
        return;
    }

    // 分配内存
    uint8_t a[m * k * sizeof(half)] = {0};
    uint8_t b[k * n * sizeof(half)] = {0};
    uint8_t c[m * n * sizeof(float)] = {0};

    // 初始化输入数据
    for (int i = 0; i < m * k; i++) {
        reinterpret_cast<half*>(a)[i] = static_cast<half>(i % 10);
    }
    for (int i = 0; i < k * n; i++) {
        reinterpret_cast<half*>(b)[i] = static_cast<half>((i + 1) % 10);
    }

    // 执行 Kernel
    KernelMmad<half, half, float, float> op;
    op.Init(a, b, c, m, k, n);
    op.Process();

    // 验证结果
    for (int32_t i = 0; i < m * n; i++) {
        // TODO: 添加实际验证逻辑
        float val = reinterpret_cast<float*>(c)[i];
    }
}

