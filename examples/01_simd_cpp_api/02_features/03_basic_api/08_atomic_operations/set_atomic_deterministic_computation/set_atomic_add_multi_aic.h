/* *
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

/* !
 * \file set_atomic_add_multi_aic.h
 * \brief 多个AIC串行执行AtomicAdd操作
 */
#include "acl/acl.h"
#include "kernel_operator.h"
#include "data_utils.h"

template <typename dst_T, typename fmap_T, typename weight_T, typename dstCO1_T> class KernelSetAtomicAddMultiAIC {
public:
    __aicore__ inline KernelSetAtomicAddMultiAIC()
    {
        aSize = m * k;
        bSize = k * n;
        cSize = m * n;
        nBlocks = n / 16;
    }
    __aicore__ inline void Init(
        __gm__ uint8_t *a0, __gm__ uint8_t *b0,
        __gm__ uint8_t *a1, __gm__ uint8_t *b1,
        __gm__ uint8_t *a2, __gm__ uint8_t *b2,
        __gm__ uint8_t *a3, __gm__ uint8_t *b3,
        __gm__ uint8_t *c,
        __gm__ uint8_t *syncFlag)
    {
        aGM[0].SetGlobalBuffer((__gm__ fmap_T *)a0);
        bGM[0].SetGlobalBuffer((__gm__ weight_T *)b0);
        aGM[1].SetGlobalBuffer((__gm__ fmap_T *)a1);
        bGM[1].SetGlobalBuffer((__gm__ weight_T *)b1);
        aGM[2].SetGlobalBuffer((__gm__ fmap_T *)a2);
        bGM[2].SetGlobalBuffer((__gm__ weight_T *)b2);
        aGM[3].SetGlobalBuffer((__gm__ fmap_T *)a3);
        bGM[3].SetGlobalBuffer((__gm__ weight_T *)b3);
        cGM.SetGlobalBuffer((__gm__ dstCO1_T *)c);
        syncFlagGM.SetGlobalBuffer((__gm__ int32_t *)syncFlag);
        blockIdx = AscendC::GetBlockIdx();
    }
    __aicore__ inline void Process()
    {
        __gm__ int32_t* syncAddr = const_cast<__gm__ int32_t*>(syncFlagGM.GetPhyAddr());
        // 预期的4个核执行的顺序是核0-->核2-->核3-->核1
        // 核0负责在原子累加之前，以src0Local初始化GM；核2、3、1负责按顺序做原子累加
        if (blockIdx == 0) {
            ProcessKernel(aGM[0], bGM[0]);

            // 等待前一个核全部任务执行完毕后，才允许 Scalar 单元向全局内存信号量写入1。
            AscendC::SetFlag<AscendC::HardEvent::FIX_S>(EVENT_ID0);
            AscendC::WaitFlag<AscendC::HardEvent::FIX_S>(EVENT_ID0);

            AscendC::WriteGmByPassDCache<int32_t>(syncAddr + blockIdx * 32, 1);
        } else if (blockIdx == 1) {
            WaitForPreCore(3, syncAddr);

            // 等待 Scalar 单元检测到信号量更新为1后，当前核再启动后续任务执行。
            AscendC::SetFlag<AscendC::HardEvent::S_MTE2>(EVENT_ID0);
            AscendC::WaitFlag<AscendC::HardEvent::S_MTE2>(EVENT_ID0);

            ProcessKernel(aGM[1], bGM[1]);
        } else if (blockIdx == 2) {
            WaitForPreCore(0, syncAddr);

            // 等待 Scalar 单元检测到信号量更新为1后，当前核再启动后续任务执行。
            AscendC::SetFlag<AscendC::HardEvent::S_MTE2>(EVENT_ID0);
            AscendC::WaitFlag<AscendC::HardEvent::S_MTE2>(EVENT_ID0);

            ProcessKernel(aGM[2], bGM[2]);

            // 等待前一个核全部任务执行完毕后，才允许 Scalar 单元向全局内存信号量写入1。
            AscendC::SetFlag<AscendC::HardEvent::FIX_S>(EVENT_ID0);
            AscendC::WaitFlag<AscendC::HardEvent::FIX_S>(EVENT_ID0);

            AscendC::WriteGmByPassDCache<int32_t>(syncAddr + blockIdx * 32, 1);
        } else if (blockIdx == 3) {
            WaitForPreCore(2, syncAddr);

            // 等待 Scalar 单元检测到信号量更新为1后，当前核再启动后续任务执行。
            AscendC::SetFlag<AscendC::HardEvent::S_MTE2>(EVENT_ID0);
            AscendC::WaitFlag<AscendC::HardEvent::S_MTE2>(EVENT_ID0);

            ProcessKernel(aGM[3], bGM[3]);
            
            // 等待前一个核全部任务执行完毕后，才允许 Scalar 单元向全局内存信号量写入1。
            AscendC::SetFlag<AscendC::HardEvent::FIX_S>(EVENT_ID0);
            AscendC::WaitFlag<AscendC::HardEvent::FIX_S>(EVENT_ID0);

            AscendC::WriteGmByPassDCache<int32_t>(syncAddr + blockIdx * 32, 1);
        }
    }

    __aicore__ inline void WaitForPreCore(int32_t preblockIdx, __gm__ int32_t* syncAddr)
    {
        while (true) {
            int32_t value = AscendC::ReadGmByPassDCache<int32_t>(syncAddr + preblockIdx * 32);
            if (value == 1) {
                AscendC::WriteGmByPassDCache<int32_t>(syncAddr + preblockIdx * 32, 0);
                break;
            }
        }
    }

    __aicore__ inline void ProcessKernel(AscendC::GlobalTensor<fmap_T>& aGM_i, AscendC::GlobalTensor<weight_T>& bGM_i)
    {
        AscendC::LocalMemAllocator<AscendC::Hardware::L1> l1Allocator;
        AscendC::LocalTensor<fmap_T> a1Local = l1Allocator.Alloc<fmap_T>(aSize);
        AscendC::LocalTensor<weight_T> b1Local = l1Allocator.Alloc<weight_T>(bSize);
        AscendC::LocalMemAllocator<AscendC::Hardware::L0A> l0AAllocator;
        AscendC::LocalTensor<fmap_T> a2Local = l0AAllocator.Alloc<fmap_T>(aSize);
        AscendC::LocalMemAllocator<AscendC::Hardware::L0B> l0BAllocator;
        AscendC::LocalTensor<weight_T> b2Local = l0BAllocator.Alloc<weight_T>(bSize);
        AscendC::LocalMemAllocator<AscendC::Hardware::L0C> l0CAllocator;
        AscendC::LocalTensor<dst_T> c1Local = l0CAllocator.Alloc<dst_T>(cSize);
        CopyIn(a1Local, b1Local, aGM_i, bGM_i);
        SplitA(a1Local, a2Local);
        SplitB(b1Local, b2Local);
        Compute(a2Local, b2Local, c1Local);
        CopyOut(c1Local);
    }

private:
    __aicore__ inline void CopyIn(AscendC::LocalTensor<fmap_T>& a1Local, AscendC::LocalTensor<weight_T>& b1Local,
        AscendC::GlobalTensor<fmap_T>& aGM_i, AscendC::GlobalTensor<weight_T>& bGM_i)
    {
        AscendC::Nd2NzParams dataCopyA1Params;
        dataCopyA1Params.ndNum = 1;
        dataCopyA1Params.nValue = m;
        dataCopyA1Params.dValue = k;
        dataCopyA1Params.srcNdMatrixStride = 0;
        dataCopyA1Params.srcDValue = k;
        dataCopyA1Params.dstNzC0Stride = m;
        dataCopyA1Params.dstNzNStride = 1;
        dataCopyA1Params.dstNzMatrixStride = 0;
        AscendC::DataCopy(a1Local, aGM_i, dataCopyA1Params);

        AscendC::Nd2NzParams dataCopyB1Params;
        dataCopyB1Params.ndNum = 1;
        dataCopyB1Params.nValue = k;
        dataCopyB1Params.dValue = n;
        dataCopyB1Params.srcNdMatrixStride = 0;
        dataCopyB1Params.srcDValue = n;
        dataCopyB1Params.dstNzC0Stride = k;
        dataCopyB1Params.dstNzNStride = 1;
        dataCopyB1Params.dstNzMatrixStride = 0;
        AscendC::DataCopy(b1Local, bGM_i, dataCopyB1Params);
    }
    __aicore__ inline void SplitA(AscendC::LocalTensor<fmap_T>& a1Local, AscendC::LocalTensor<fmap_T>& a2Local)
    {
        // 循环内依赖：先"DataCopy(PIPE_MTE2)写a1Local、b1Local"，后 "LoadData(PIPE_MTE1)读a1Local、b1Local"
        AscendC::SetFlag<AscendC::HardEvent::MTE2_MTE1>(EVENT_ID0);
        AscendC::WaitFlag<AscendC::HardEvent::MTE2_MTE1>(EVENT_ID0);

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 2201)
        AscendC::LoadData2DParams loadL0AParams;
        loadL0AParams.repeatTimes = aSize * sizeof(fmap_T) / 512;
        loadL0AParams.srcStride = 1;
        loadL0AParams.ifTranspose = false;
        AscendC::LoadData(a2Local, a1Local, loadL0AParams);
#elif defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
        AscendC::LoadData2DParamsV2 loadDataParams;
        loadDataParams.mStartPosition = 0;
        loadDataParams.kStartPosition = 0;
        loadDataParams.mStep = AscendC::DivCeil(m, 16);
        loadDataParams.kStep = AscendC::DivCeil(k * sizeof(fmap_T), 32);
        loadDataParams.srcStride = AscendC::DivCeil(m, 16);
        loadDataParams.dstStride = AscendC::DivCeil(m, 16);
        loadDataParams.sid = 0;
        loadDataParams.ifTranspose = false;
        AscendC::LoadData(a2Local, a1Local, loadDataParams);
#endif
    }
    __aicore__ inline void SplitB(AscendC::LocalTensor<weight_T>& b1Local, AscendC::LocalTensor<weight_T>& b2Local)
    {
        AscendC::LoadData2dTransposeParams loadDataParams;
        loadDataParams.startIndex = 0;
        nBlockSize = 16;
        loadDataParams.repeatTimes = n / nBlockSize;
        loadDataParams.srcStride = 1;
        loadDataParams.dstGap = 0;
        loadDataParams.dstFracGap = n / nBlockSize - 1;
        AscendC::LoadDataWithTranspose(b2Local, b1Local, loadDataParams);
           
    }
    __aicore__ inline void Compute(AscendC::LocalTensor<fmap_T>& a2Local, AscendC::LocalTensor<weight_T>& b2Local, AscendC::LocalTensor<dst_T>& c1Local)
    {
        // 循环内依赖：先"LoadData(PIPE_MTE2)写a2Local和b2Local"，后 "Mmad(PIPE_M)读a2Local和b2Local"
        AscendC::SetFlag<AscendC::HardEvent::MTE2_M>(EVENT_ID0);
        AscendC::WaitFlag<AscendC::HardEvent::MTE2_M>(EVENT_ID0);

        AscendC::MmadParams mmadParams;
        mmadParams.m = m;
        mmadParams.n = n;
        mmadParams.k = k;
        AscendC::Mmad(c1Local, a2Local, b2Local, mmadParams);
    }
    __aicore__ inline void CopyOut(AscendC::LocalTensor<dst_T>& c1Local)
    {
        // 循环内依赖：先"Mmad(PIPE_M)写c1Local"，后 "Fixpipe(PIPE_M)读c1Local"
        AscendC::SetFlag<AscendC::HardEvent::M_FIX>(EVENT_ID0);
        AscendC::WaitFlag<AscendC::HardEvent::M_FIX>(EVENT_ID0);

        AscendC::FixpipeParamsV220 fixpipeParams;
        fixpipeParams.nSize = n;
        fixpipeParams.mSize = m;
        fixpipeParams.srcStride = m;
        fixpipeParams.dstStride = n;

        fixpipeParams.ndNum = 1;
        fixpipeParams.srcNdStride = 0;
        fixpipeParams.dstNdStride = 0;

        // 对L0C-->GM搬出的数据，启用原子累加
        AscendC::SetAtomicAdd<dst_T>();
        AscendC::Fixpipe(cGM, c1Local, fixpipeParams);
    }

private:
    AscendC::GlobalTensor<fmap_T> aGM[4];
    AscendC::GlobalTensor<weight_T> bGM[4];
    AscendC::GlobalTensor<dst_T> cGM;
    AscendC::GlobalTensor<int32_t> syncFlagGM;
    uint16_t m = 16, k = 16, n = 32;
    uint8_t nBlockSize = 16;
    uint16_t c0Size = 16;
    uint32_t aSize, bSize, cSize, nBlocks;
    int32_t blockIdx = -1;
};