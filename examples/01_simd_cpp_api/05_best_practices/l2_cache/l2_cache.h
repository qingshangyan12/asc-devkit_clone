/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

/* !
 * \file l2_cache.h
 * \brief L2 Cache mode best practices – kernel implementations
 *
 * 本文件包含本样例所需的全部核心 kernel 实现，包含两类算子：
 * 1. KernelDataCopyRepeat – GM→UB 重复搬运 kernel（Case 1-3）
 *    通过对比整块重复搬运和分片重复搬运，以及 L2 Cache 开启/关闭的差异，
 *    揭示对于需要多次重复读取的数据，应合理利用 L2 Cache 并控制单次工作集大小。
 *
 * 2. KernelAdd – 矩阵加法 kernel（Case 4-5）
 *    通过对比双缓冲场景下 L2 Cache 开启和 bypass 的差异，
 *    揭示对于只读取一次的流式数据，关闭 L2 Cache 可以减少不必要的 cache 写开销。
 *
 * Case 6-7 的流水线效果通过 main 函数先后调用两个 kernel 实现，不在此文件中体现。
 */

#ifndef L2_CACHE_H
#define L2_CACHE_H

#include "kernel_operator.h"

// ====================================================================
// 第一部分：GM→UB 重复搬运 Kernel（Case 1-3）
// 用于对比不同数据复用策略下 L2 Cache 的行为
// ====================================================================

template <uint32_t m, uint32_t n, uint32_t numBlocks, uint32_t singleCoreM, uint32_t tileM, uint32_t tileN>
class KernelDataCopyRepeat {
public:
    __aicore__ inline KernelDataCopyRepeat() {}

    /* !
     * \brief 初始化：设置 GlobalTensor，每个核处理 singleCoreM 行的数据
     * \param src 输入数据在 Global Memory 上的地址
     */
    __aicore__ inline void Init(__gm__ half* src)
    {
        srcGlobal.SetGlobalBuffer(src, m * n);
        uint32_t blockIdx = AscendC::GetBlockIdx();
        uint32_t mStart = blockIdx * singleCoreM;
        // 每个核只使用自己负责的 M 方向分片
        srcGlobal = srcGlobal[mStart * n];
        ubLocal = AscendC::LocalTensor<half>(AscendC::TPosition::VECIN, ubAddr, ubBufferSize);
    }

    /* !
     * \brief Case 1: N 方向切 4 份，每份连续重复搬运 4 次，L2 Cache 默认开启
     *
     * 设计意图：
     * - 输入矩阵为 [12288, 12288]，half 类型，单次工作集约 301.99MB
     * - L2 Cache 容量有限（A2/A3 约 192MB，950 约 128MB），整块矩阵无法完整保留
     * - 将 N 方向切为 4 份后，每份约 75.50MB，小于 L2 Cache 容量
     * - 在每个分片内连续重复 4 次：第 1 次从 GM 读取并写入 L2，第 2~4 次命中 L2
     * - 搬运总数据量与整块重复搬运相同（1207.96MB），但 L2 命中率显著更高
     */
    __aicore__ inline void ProcessSplitFourAndRepeat4Times()
    {
        static_assert(n % 4 == 0, "n must be divisible by 4 for split-4 scenario");
        constexpr uint32_t quarterN = n / 4;

        // 先按 N 方向切成 4 份，每份连续搬运 4 次，充分利用 L2 Cache 复用
        for (uint32_t splitIdx = 0; splitIdx < 4; splitIdx++) {
            RepeatCopy(splitIdx * quarterN, quarterN, 4);
        }
    }

    /* !
     * \brief Case 2: 同 Case 1，但调用 SetL2CacheHint 关闭 L2 Cache
     *
     * 设计意图：
     * - 与 Case 1 形成对照：搬运模式完全相同，唯一区别是关闭了 L2 Cache
     * - 由于数据需要重复读取 4 次，关闭 L2 后每次都要从 GM 重新读取
     * - 预期 L2 命中率降为 0%，MTE2 耗时显著增加，端到端性能劣化
     * - 证明：对于需要复用的数据，应保持 L2 Cache 开启
     */
    __aicore__ inline void ProcessSplitFourAndRepeat4TimesL2Disable()
    {
        // 关闭 L2 Cache 后，DataCopy 直接从 GM 读取到 UB，不会写入 L2
        // 后续的重复读取无法命中 L2 Cache，每次都需要重新从 GM 搬运
        srcGlobal.SetL2CacheHint(AscendC::CacheMode::CACHE_MODE_DISABLE);
        ProcessSplitFourAndRepeat4Times();
    }

    /* !
     * \brief Case 3: 整块矩阵连续重复搬运 4 次，L2 Cache 默认开启
     *
     * 设计意图：
     * - 与 Case 1 形成对照：搬运总数据量相同（都是 4 次完整矩阵读取）
     * - 但整块矩阵（301.99MB）大于 L2 Cache 容量，每次完整矩阵搬运都会
     *   导致 L2 Cache 中的旧数据被换出，命中率极低
     * - 预期 L2 命中率接近 0%，MTE2 耗时远高于 Case 1
     * - 证明：即使 L2 Cache 开启，如果单次工作集超出 L2 容量，也无法获得复用收益
     */
    __aicore__ inline void ProcessRepeatWhole4Times()
    {
        // 整块矩阵一次搬运完整的 N 方向，连续重复 4 次
        // 由于单次工作集（301.99MB）超出 L2 Cache 容量，
        // 第 2~4 轮的大部分数据需要重新从 GM 读取
        RepeatCopy(0, n, 4);
    }

private:
    /* !
     * \brief 在指定的 N 方向分片内，按 tile 粒度连续重复搬运指定次数
     *
     * \param nStart  本次分片在 N 方向的起始列索引
     * \param nCount  本次分片在 N 方向的列数
     * \param repeatTimes  连续重复搬运的次数
     */
    __aicore__ inline void RepeatCopy(uint32_t nStart, uint32_t nCount, uint32_t repeatTimes)
    {
        uint32_t blockIdx = AscendC::GetBlockIdx();
        uint32_t mStart = blockIdx * singleCoreM;

        // 按 tile 粒度切分 M/N 方向，使用 DataCopyPad 进行二维搬运
        uint32_t nLoopCount = nCount / tileN;
        constexpr uint32_t mLoopCount = singleCoreM / tileM;
        AscendC::DataCopyParams copyInParams{
            static_cast<uint16_t>(tileM),
            static_cast<uint16_t>(tileN * sizeof(half)),
            static_cast<uint16_t>((n - tileN) * sizeof(half)),
            0};
        AscendC::DataCopyPadParams padParams{false, 0, 0, 0};

        // 外层循环：重复次数；内层循环：按 M/N tile 遍历分片内的所有数据
        for (uint32_t repeatIdx = 0; repeatIdx < repeatTimes; repeatIdx++) {
            for (uint32_t mBlockIdx = 0; mBlockIdx < mLoopCount; mBlockIdx++) {
                uint32_t mIdx = mStart + mBlockIdx * tileM;
                for (uint32_t nBlockIdx = 0; nBlockIdx < nLoopCount; nBlockIdx++) {
                    uint32_t nIdx = nBlockIdx * tileN;
                    AscendC::DataCopyPad(
                        ubLocal, srcGlobal[mIdx * n + nStart + nIdx], copyInParams, padParams);
                }
            }
        }
    }

    static constexpr uint32_t ubBufferSize = tileM * tileN;   // UB 缓冲区大小（元素个数）
    static constexpr uint64_t ubAddr = 0;                     // UB 起始地址
    AscendC::GlobalTensor<half> srcGlobal;                     // 输入数据在 GM 上的视图
    AscendC::LocalTensor<half> ubLocal;                        // UB 上的本地张量
};


// ====================================================================
// 第二部分：矩阵加法 Kernel（Case 4-5）
// 用于对比流式数据（只读一次）的 L2 Cache 开启 vs Bypass
// ====================================================================

template <uint32_t totalM, uint32_t totalN, uint32_t splitM, uint32_t splitN, uint32_t dataCopyLen,
    uint32_t maxDataCopyLen>
class KernelAdd {
public:
    __aicore__ inline KernelAdd() = default;

    /* !
     * \brief 初始化：按 M 方向均匀切分，每个核计算一片连续的行
     *
     * \param x 输入矩阵 x 在 Global Memory 上的地址
     * \param y 输入矩阵 y 在 Global Memory 上的地址
     * \param z 输出矩阵 z 在 Global Memory 上的地址
     */
    __aicore__ inline void Init(__gm__ uint8_t* x, __gm__ uint8_t* y, __gm__ uint8_t* z)
    {
        static_assert(splitN == 1, "This optimized sample expects splitN == 1 for contiguous row-wise DataCopy.");
        uint32_t blockIdx = AscendC::GetBlockIdx();
        uint32_t blockIdxM = blockIdx;

        // 均匀切分：将余数行分配给前面几个核，确保各核负载均衡
        constexpr uint32_t baseCoreM = totalM / splitM;
        constexpr uint32_t remainderM = totalM % splitM;
        uint32_t actualCoreM;
        uint32_t startM;
        if (blockIdxM < remainderM) {
            actualCoreM = baseCoreM + 1;
            startM = blockIdxM * actualCoreM;
        } else {
            actualCoreM = baseCoreM;
            startM = remainderM * (baseCoreM + 1) + (blockIdxM - remainderM) * baseCoreM;
        }
        this->singleCoreM = actualCoreM;
        this->singleCoreN = totalN;
        this->totalElementsPerCore = this->singleCoreM * this->singleCoreN;

        // 每个核只设置自己负责的 M 方向连续分片
        xGm.SetGlobalBuffer((__gm__ half*)x + startM * totalN, this->totalElementsPerCore);
        yGm.SetGlobalBuffer((__gm__ half*)y + startM * totalN, this->totalElementsPerCore);
        zGm.SetGlobalBuffer((__gm__ half*)z + startM * totalN, this->totalElementsPerCore);
    }

    /* !
     * \brief Case 4: 多核均匀切分 + 双缓冲 + L2 Cache 默认开启
     *
     * 设计意图：
     * - Add 算子的输入 x、y 均为流式数据（每个元素只读取一次，不会复用）
     * - 使用 Ping-Pong 双缓冲实现数据搬运与向量计算的流水线并行
     * - L2 Cache 默认开启，MTE2 在搬运数据到 UB 的同时会将数据写入 L2 Cache
     * - 由于数据没有复用，写入 L2 Cache 的操作是纯浪费的，额外消耗了 L2 控制器带宽
     * - 同时，这些不会被再次访问的数据占据了 L2 Cache 空间，产生 cache 污染
     */
    __aicore__ inline void ProcessDoubleBuffer()
    {
        // 分配 Ping-Pong 双缓冲区，交替用于数据搬运和向量计算
        AscendC::LocalTensor<half> xPing(AscendC::TPosition::VECCALC, xAddrPing, maxDataCopyLen);
        AscendC::LocalTensor<half> yPing(AscendC::TPosition::VECCALC, yAddrPing, maxDataCopyLen);
        AscendC::LocalTensor<half> zPing(AscendC::TPosition::VECCALC, zAddrPing, maxDataCopyLen);
        AscendC::LocalTensor<half> xPong(AscendC::TPosition::VECCALC, xAddrPong, maxDataCopyLen);
        AscendC::LocalTensor<half> yPong(AscendC::TPosition::VECCALC, yAddrPong, maxDataCopyLen);
        AscendC::LocalTensor<half> zPong(AscendC::TPosition::VECCALC, zAddrPong, maxDataCopyLen);

        DoubleBufferLoop(xPing, yPing, zPing, xPong, yPong, zPong);
    }

    /* !
     * \brief Case 5: 多核均匀切分 + 双缓冲 + L2 Cache bypass
     *
     * 设计意图：
     * - 与 Case 4 形成对照：双缓冲流水线完全相同，唯一区别是对 x、y 输入禁用 L2 Cache
     * - x、y 是流式数据（只读一次），禁用 L2 Cache 后 MTE2 直接将数据从 GM 搬运到 UB，
     *   不再写入 L2 Cache，省去了不必要的 cache 写操作
     * - 预期 MTE2 耗时显著降低（约 30%），端到端性能明显提升
     * - 同时避免了流式数据对 L2 Cache 的污染，为后续其他算子的缓存复用腾出空间
     */
    __aicore__ inline void ProcessDoubleBufferL2Bypass()
    {
        // 对只读取一次的流式数据设置 L2 Cache bypass
        // 数据直接从 GM 搬运到 UB，不经过 L2 Cache，减少不必要的 cache 写开销
        xGm.SetL2CacheHint(AscendC::CacheMode::CACHE_MODE_DISABLE);
        yGm.SetL2CacheHint(AscendC::CacheMode::CACHE_MODE_DISABLE);
        ProcessDoubleBuffer();
    }

private:
    /* !
     * \brief 双缓冲流水线核心循环
     *
     * 原理：
     * - 将数据切分为多个 block，每个 block 大小为 dataCopyLen 个元素
     * - 使用 Ping-Pong 双缓冲区：Ping 计算时，Pong 搬运下一个 block 的数据
     * - 通过两个 Event ID（EVENT_ID0/EVENT_ID1）管理两套同步信号
     * - 实现数据搬运（MTE2/MTE3）与向量计算（Vector）的流水线并行
     */
    __aicore__ inline void DoubleBufferLoop(AscendC::LocalTensor<half>& xPing, AscendC::LocalTensor<half>& yPing,
        AscendC::LocalTensor<half>& zPing, AscendC::LocalTensor<half>& xPong,
        AscendC::LocalTensor<half>& yPong, AscendC::LocalTensor<half>& zPong)
    {
        uint32_t totalBlocks = AscendC::DivCeil(totalElementsPerCore, dataCopyLen);

        // 初始化所有同步信号，避免第一轮循环中的虚假依赖
        AscendC::SetFlag<AscendC::HardEvent::V_MTE2>(EVENT_ID0);
        AscendC::SetFlag<AscendC::HardEvent::V_MTE2>(EVENT_ID1);
        AscendC::SetFlag<AscendC::HardEvent::MTE3_V>(EVENT_ID0);
        AscendC::SetFlag<AscendC::HardEvent::MTE3_V>(EVENT_ID1);

        for (uint32_t loopIdx = 0; loopIdx < totalBlocks; loopIdx++) {
            uint32_t startElement = loopIdx * dataCopyLen;
            uint32_t remainElements = totalElementsPerCore - startElement;
            uint32_t curLen = remainElements > dataCopyLen ? dataCopyLen : remainElements;

            // Ping-Pong 切换：偶数轮用 Ping 缓冲和 EVENT_ID0，奇数轮用 Pong 和 EVENT_ID1
            int32_t eventID = ((loopIdx & 1) == 0 ? EVENT_ID0 : EVENT_ID1);
            AscendC::LocalTensor<half>& xLocal = ((loopIdx & 1) == 0 ? xPing : xPong);
            AscendC::LocalTensor<half>& yLocal = ((loopIdx & 1) == 0 ? yPing : yPong);
            AscendC::LocalTensor<half>& zLocal = ((loopIdx & 1) == 0 ? zPing : zPong);

            // 等待上一轮在相同缓冲区上的搬运完成（释放 GM→UB 通路）
            AscendC::WaitFlag<AscendC::HardEvent::V_MTE2>(eventID);

            // 从 GM 搬运当前 block 的 x、y 输入数据到 UB
            AscendC::DataCopy(xLocal, xGm[startElement], curLen);
            AscendC::DataCopy(yLocal, yGm[startElement], curLen);

            // 通知向量单元：GM→UB 搬运已完成，可以开始计算
            AscendC::SetFlag<AscendC::HardEvent::MTE2_V>(eventID);
            AscendC::WaitFlag<AscendC::HardEvent::MTE2_V>(eventID);

            // 等待上一轮在相同缓冲区上的 UB→GM 写出完成（释放 UB 空间）
            AscendC::WaitFlag<AscendC::HardEvent::MTE3_V>(eventID);

            // 向量计算：z = x + y
            AscendC::Add(zLocal, xLocal, yLocal, curLen);

            // 通知 MTE2：VECCALC 区域已释放，可以搬运下一轮数据
            AscendC::SetFlag<AscendC::HardEvent::V_MTE2>(eventID);

            // 通知 MTE3：VECCALC 区域的输出数据已就绪，可以写回 GM
            AscendC::SetFlag<AscendC::HardEvent::V_MTE3>(eventID);
            AscendC::WaitFlag<AscendC::HardEvent::V_MTE3>(eventID);

            // 将计算结果从 UB 写回 GM
            AscendC::DataCopy(zGm[startElement], zLocal, curLen);

            // 通知向量单元：UB→GM 写出已完成，可以复用缓冲区
            AscendC::SetFlag<AscendC::HardEvent::MTE3_V>(eventID);
        }

        // 等待所有尾部的同步信号，确保没有硬件死锁
        AscendC::WaitFlag<AscendC::HardEvent::V_MTE2>(EVENT_ID0);
        AscendC::WaitFlag<AscendC::HardEvent::V_MTE2>(EVENT_ID1);
        AscendC::WaitFlag<AscendC::HardEvent::MTE3_V>(EVENT_ID0);
        AscendC::WaitFlag<AscendC::HardEvent::MTE3_V>(EVENT_ID1);
    }

    // 双缓冲 UB 地址布局（每份缓冲区大小 = maxDataCopyLen * sizeof(half)）
    // 6 份缓冲区（xPing, yPing, zPing, xPong, yPong, zPong）总计 maxDataCopyLen * 12 字节
    static constexpr uint32_t xAddrPing = 0;
    static constexpr uint32_t yAddrPing = maxDataCopyLen * sizeof(half);
    static constexpr uint32_t zAddrPing = yAddrPing + maxDataCopyLen * sizeof(half);
    static constexpr uint32_t xAddrPong = zAddrPing + maxDataCopyLen * sizeof(half);
    static constexpr uint32_t yAddrPong = xAddrPong + maxDataCopyLen * sizeof(half);
    static constexpr uint32_t zAddrPong = yAddrPong + maxDataCopyLen * sizeof(half);

    AscendC::GlobalTensor<half> xGm;    // 输入 x 在 GM 上的视图
    AscendC::GlobalTensor<half> yGm;    // 输入 y 在 GM 上的视图
    AscendC::GlobalTensor<half> zGm;    // 输出 z 在 GM 上的视图
    uint32_t singleCoreM;               // 当前核处理的 M 方向行数
    uint32_t singleCoreN;               // N 方向列数（= totalN）
    uint32_t totalElementsPerCore;      // 当前核处理的元素总数
};

#endif // L2_CACHE_H
