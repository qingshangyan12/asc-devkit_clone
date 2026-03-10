#ifndef __ASCEND_KERNEL__
#define __ASCEND_KERNEL__
#endif
 
#include "kernel_operator.h"

using namespace AscendC;

constexpr static uint16_t LIMIT_MNSIZE = 10;

template <class T, class U>
class KernelMmad {
public:
    __aicore__ inline KernelMmad(uint16_t mSize, uint16_t nSize, uint16_t kSize, uint32_t repeat) {
        m = mSize;
        n = nSize;
        k = kSize;
        repeatTime = repeat;
        //左矩阵分形的shape
        fractalShape[0] = 16;
        fractalShape[1] = 32 / sizeof(T);
        //右矩阵的shape:[32 / sizeof(T), 16]
        //左、右矩阵分形的size，单位是元素数目
        fractalSize = 16 * fractalShape[1];
        if constexpr (sizeof(T) == 2) {
            fractalNum = 1;
        } else {
            fractalNum = 2;
        }
        // 对齐后的shape
        // 计算不同场景下，A矩阵、B矩阵各个方向对齐的shape参数
        if constexpr (IsSameType<T, int8_t>::value && IsSameType<U, int32_t>::value) {
            // L1上，A矩阵的对齐
            // GM上A矩阵的shape为[m,k]
            mAlignL1 = CeilAlign(m, fractalShape[0]); // 高度
            kAlignL1 = CeilAlign(k, fractalShape[1]); // 宽度
            aSizeAlignL1 = mAlignL1 * kAlignL1;
            // L0上，A矩阵的对齐
            // 由于L0上a矩阵也是Z排布
            mAlignL0 = CeilAlign(m, fractalShape[0]); // 高度
            kAlignL0 = CeilAlign(k, fractalShape[1]); // 宽度
            aSizeAlignL0 = mAlignL0 * kAlignL0;

            // L1上，B矩阵的对齐
            // L1上, b矩阵是Z排布
            nAlignL1 = CeilAlign(n, fractalShape[0]); // 高度
            kAlignL1 = CeilAlign(k, fractalShape[1]); // 宽度
            bSizeAlignL1 = nAlignL1 * kAlignL1;
            // L0上，B矩阵的对齐
            // L0上, b矩阵是N排布
            kAlignL0 = CeilAlign(k, fractalShape[1]); // 高度
            nAlignL0 = CeilAlign(n, fractalShape[0]); // 宽度
            bSizeAlignL0 = kAlignL0 * nAlignL0;

        } else if constexpr (IsSameType<T, half>::value && IsSameType<U, float>::value) {
            // L1上，A矩阵的对齐
            mAlignL1 = CeilAlign(m, fractalShape[0]); // 高度
            kAlignL1 = CeilAlign(k, fractalShape[1]); // 宽度
            aSizeAlignL1 = mAlignL1 * kAlignL1;
            // L0上，A矩阵的对齐
            // 由于L0上a矩阵也是Z排布
            mAlignL0 = CeilAlign(m, fractalShape[0]); // 高度
            kAlignL0 = CeilAlign(k, fractalShape[1]); // 宽度
            aSizeAlignL0 = mAlignL0 * kAlignL0;

            // L1上，B矩阵的对齐
            // L1上, b矩阵是Z排布
            // GM上A矩阵的shape为[n,k]
            nAlignL1 = CeilAlign(n, fractalShape[0]); // 高度
            kAlignL1 = CeilAlign(k, fractalShape[1]); // 宽度
            bSizeAlignL1 = nAlignL1 * kAlignL1;
            // L0上，B矩阵的对齐
            // L0上, b矩阵是N排布
            kAlignL0 = CeilAlign(k, fractalShape[1]); // 高度
            nAlignL0 = CeilAlign(n, fractalShape[0]); // 宽度
            bSizeAlignL0 = kAlignL0 * nAlignL0;
        } else if constexpr (IsSameType<T, float>::value && IsSameType<U, float>::value) {
            // L1上，A矩阵的对齐
            mAlignL1 = CeilAlign(m, fractalShape[0]); // 高度
            kAlignL1 = CeilAlign(k, fractalShape[1]); // 宽度
            aSizeAlignL1 = mAlignL1 * kAlignL1;
            // L0上，A矩阵的对齐
            // 由于L0上a矩阵也是Z排布
            mAlignL0 = CeilAlign(m, fractalShape[0]); // 高度
            kAlignL0 = CeilAlign(k, fractalShape[1]); // 宽度
            aSizeAlignL0 = mAlignL0 * kAlignL0;
        
            // L1上，B矩阵的对齐
            // L1上, b矩阵是Z排布
            nAlignL1 = CeilAlign(n, fractalShape[0]); // 高度
            kAlignL1 = CeilAlign(k, fractalShape[1]); // 宽度
            bSizeAlignL1 = nAlignL1 * kAlignL1;
            // L0上，B矩阵的对齐
            // L0上, b矩阵是N排布
            kAlignL0 = CeilAlign(k, fractalShape[1]); // 高度
            nAlignL0 = CeilAlign(n, fractalShape[0]); // 宽度
            bSizeAlignL0 = kAlignL0 * nAlignL0;
        }
        // C矩阵无视数据类型和a、b是否转置，m,n都向16对齐
        cSizeAlignL0 = CeilAlign(m, fractalShape[0]) * CeilAlign(n, fractalShape[0]);
    }

    __aicore__ inline void Init(GM_ADDR a, GM_ADDR b, GM_ADDR c, GM_ADDR outTime, TPipe* pipeIn)
    {
        pipe = pipeIn;
        aGM.SetGlobalBuffer((__gm__ T *)a);
        bGM.SetGlobalBuffer((__gm__ T *)b);
        cGM.SetGlobalBuffer((__gm__ U *)c);
        timeGMAddr = (__gm__ uint64_t*)outTime;
        pipe->InitBuffer(inQueueA1, 1, aSizeAlignL1 * sizeof(T));
        pipe->InitBuffer(inQueueA2, 1, aSizeAlignL0 * sizeof(T));
        pipe->InitBuffer(inQueueB1, 1, bSizeAlignL1 * sizeof(T));
        pipe->InitBuffer(inQueueB2, 1, bSizeAlignL0 * sizeof(T));
        pipe->InitBuffer(outQueueCO1, 1, cSizeAlignL0 * sizeof(U));
    }
    __aicore__ inline void Process()
    {
        for (uint32_t i = 0; i < repeatTime; i++) {
            Compute();
            CopyOut();
        }
        timeGMAddr[repeatTime] = endCycle - startCycle;
    }

    __aicore__ inline void FlashTimeRecord()
    {
        GlobalTensor<uint64_t> gm;
        DataCacheCleanAndInvalid<uint64_t, CacheLine::ENTIRE_DATA_CACHE, DcciDst::CACHELINE_OUT>(gm);
    }

private:
    __aicore__ inline uint16_t CeilDivision(uint16_t numerator, uint16_t denominator) 
    {
        return (numerator + denominator - 1) / denominator;
    }

    __aicore__ inline uint16_t CeilAlign(uint16_t numerator, uint16_t denominator) 
    {
        return (numerator + denominator - 1) / denominator * denominator;
    }

    __aicore__ inline void Compute()
    {
        LocalTensor<T> a2Local = inQueueA2.AllocTensor<T>();
        LocalTensor<T> b2Local = inQueueB2.AllocTensor<T>();
        LocalTensor<U> c1Local = outQueueCO1.AllocTensor<U>();
        MmadParams mmadParams;
        // 左矩阵Height
        mmadParams.m = m;
        // 右矩阵width
        mmadParams.n = n;
        // 左矩阵Width、右矩阵Height
        mmadParams.k = k;

        pipe_barrier(PIPE_ALL);
        startCycle = get_sys_cnt();
        pipe_barrier(PIPE_ALL);

        Mmad(c1Local, a2Local, b2Local, mmadParams);
        if ((m / 16) * (n / 16) < LIMIT_MNSIZE) {
            AscendC::PipeBarrier<PIPE_M>();
        }
        pipe_barrier(PIPE_ALL);
        endCycle = get_sys_cnt();
        pipe_barrier(PIPE_ALL);

        outQueueCO1.EnQue<U>(c1Local);
        inQueueA2.FreeTensor(a2Local);
        inQueueB2.FreeTensor(b2Local);
    }

    __aicore__ inline void CopyOut()
    {
        LocalTensor<U> c1Local = outQueueCO1.DeQue<U>();
        FixpipeParamsV220 fixpipeParams;
        // 源NZ矩阵在N方向上的大小。
        fixpipeParams.nSize = n;
        // 源NZ矩阵在M方向上的大小。
        fixpipeParams.mSize = m;

        // 源NZ矩阵中相邻Z排布的起始地址偏移
        // 源操作数来源于L0c，因此m只需要向16对齐，与数据类型无关
        fixpipeParams.srcStride = CeilAlign(m, fractalShape[0]);
        fixpipeParams.dstStride = n;
        // 源NZ矩阵的数目，也就是传输ND矩阵的数目
        fixpipeParams.ndNum = 1;
        // 不同NZ矩阵起始地址之间的间隔，当ndNum配置为1时，配置为0即可，不生效。
        fixpipeParams.srcNdStride = 0;
        // 不同ND矩阵起始地址之间的间隔，当ndNum配置为1时，配置为0即可，不生效。
        fixpipeParams.dstNdStride = 0;
        Fixpipe(cGM, c1Local, fixpipeParams);
        outQueueCO1.FreeTensor(c1Local);
    }

private:
    TPipe* pipe;
    TQue<TPosition::A1, 1> inQueueA1;
    TQue<TPosition::A2, 1> inQueueA2;
    TQue<TPosition::B1, 1> inQueueB1;
    TQue<TPosition::B2, 1> inQueueB2;
    TQue<TPosition::CO1, 1> outQueueCO1;

    GlobalTensor<T> aGM;
    GlobalTensor<T> bGM;
    GlobalTensor<U> cGM;
    __gm__ uint64_t* timeGMAddr;

    uint32_t repeatTime;

    uint16_t m = 0, k = 0, n = 0;

    uint16_t mAlignL1 = 0, kAlignL1 = 0, nAlignL1 = 0;
    uint16_t mAlignL0 = 0, kAlignL0 = 0, nAlignL0 = 0;

    uint16_t aSizeAlignL1, bSizeAlignL1;
    uint16_t aSizeAlignL0, bSizeAlignL0, cSizeAlignL0;
    uint16_t fractalShape[2] = {0, 0};
    uint16_t fractalSize = 0;
    uint16_t fractalNum = 0;

    uint64_t startCycle = 0;
    uint64_t endCycle = 0;
};

extern "C" __global__ __aicore__ void mmad_custom(GM_ADDR a, GM_ADDR b, GM_ADDR c, GM_ADDR outTime, uint16_t m, uint16_t n, uint16_t k, uint32_t scenarioNum, uint32_t repeatTime)
{
    KERNEL_TASK_TYPE_DEFAULT(KERNEL_TYPE_AIC_ONLY);
    TPipe pipe;

    if (scenarioNum == 1) {
        KernelMmad<int8_t, int32_t> op(m, n, k, repeatTime);
        op.Init(a, b, c, outTime, &pipe);
        op.Process();
        op.FlashTimeRecord(); 
    } else if (scenarioNum == 2) {
        KernelMmad<half, float> op(m, n, k, repeatTime);
        op.Init(a, b, c, outTime, &pipe);
        op.Process();
        op.FlashTimeRecord();
    } else if (scenarioNum == 3) {
        KernelMmad<float, float> op(m, n, k, repeatTime);
        op.Init(a, b, c, outTime, &pipe);
        op.Process();
        op.FlashTimeRecord();
    }
    pipe_barrier(PIPE_ALL);
}

#ifndef ASCEND_CPU_DEBUG
void DoMatMul(uint32_t blockNum, void* stream, uint8_t* a, uint8_t* b, uint8_t* c, uint64_t* outTime, uint16_t m, uint16_t n, uint16_t k, uint32_t scenarioNum, uint32_t repeatTime)
{
    mmad_custom<<<blockNum, nullptr, stream>>>(a, b, c, outTime, m, n, k, scenarioNum, repeatTime);
}

#endif