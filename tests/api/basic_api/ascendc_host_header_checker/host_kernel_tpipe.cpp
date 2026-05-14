/**
* Copyright (c) 2026 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/

#include "kernel_tpipe.h"

//template <typename T> __aicore__ inline __sync_noalias__ LocalTensor<T> AllocTensor();
void HostTestTQueBindAllocTensor1() {
    AscendC::TQueBind<AscendC::TPosition::VECIN, AscendC::TPosition::VECOUT, 8> que;
    AscendC::LocalTensor<float> tensor = que.AllocTensor<float>();
}

//template <typename T> __aicore__ inline __sync_noalias__ void AllocTensor(LocalTensor<T>& tensor);
void HostTestTQueBindAllocTensor2() {
    AscendC::TQueBind<AscendC::TPosition::VECIN, AscendC::TPosition::VECOUT, 8> que;
    AscendC::LocalTensor<float> tensor;
    que.AllocTensor<float>(tensor);
}

//template<TPosition bufPos, uint32_t bufIDSize> friend class TBufPoolExtImpl;
// Multiple constexpr members are architecture-specific
void HostTestTQueBindConfig() {
    AscendC::TQueBind<AscendC::TPosition::VECIN, AscendC::TPosition::VECOUT, 8> que;
}

//template<TPosition bufPos, uint32_t bufIDSize> friend class TBufPoolExtImpl;
void HostTestTQueFriend() {
    AscendC::TQue<AscendC::TPosition::VECIN, 8> que;
}

//template<TPosition bufPos, uint32_t bufIDSize> friend class TBufPoolExtImpl;
void HostTestTBufFriend() {
    AscendC::TBuf<AscendC::TPosition::LCM> buf;
}

//template <typename T> __aicore__ inline __sync_noalias__ LocalTensor<T> AllocTensor(int32_t num);
void HostTestTQueBindAllocTensorNum() {
    AscendC::TQueBind<AscendC::TPosition::VECIN, AscendC::TPosition::VECOUT, 8> que;
    int32_t num = 8;
    AscendC::LocalTensor<float> tensor = que.AllocTensor<float>(num);
}

//__aicore__ inline uint64_t GetNext(const int32_t len = 1);
void HostTestTQueBindGetNext() {
    AscendC::TQueBind<AscendC::TPosition::VECIN, AscendC::TPosition::VECOUT, 8> que;
    que.GetNext(1);
}

//template <TPosition pos, uint32_t bufIDSize = defaultBufIDSize>
//class TBufPool : public TBufPoolExtImpl<pos, bufIDSize> {
//    __aicore__ inline TBufPool() = default;
//    __aicore__ inline ~TBufPool();
//};
void HostTestTBufPool() {
    AscendC::TBufPool<AscendC::TPosition::VECIN> bufPool;
}

//template <typename IMPL, typename A, typename B, typename L0cT, class C, const auto &MM_CFG, typename>
//friend class Impl::Detail::CubeOutBuffer;
void HostTestTQueBindCubeOutBuffer() {
    AscendC::TQueBind<AscendC::TPosition::VECIN, AscendC::TPosition::VECOUT, 8> que;
}

//template <typename T>
//friend __aicore__ inline uint64_t GetTQueHeadAddr(const T& que);
void HostTestTQueGetHeadAddr() {
    AscendC::TQue<AscendC::TPosition::VECIN, 8> que;
}

//template <typename T>
//friend __aicore__ inline uint64_t GetTQueHeadAddr(const T& que);
void HostTestTQueBindGetHeadAddr() {
    AscendC::TQueBind<AscendC::TPosition::VECIN, AscendC::TPosition::VECOUT, 8> que;
}

//template <typename T> __aicore__ inline __sync_noalias__ LocalTensor<T> AllocTensor(int32_t num);
void HostTestTQueAllocTensorNum() {
    AscendC::TQue<AscendC::TPosition::VECIN, 8> que;
    int32_t num = 8;
    AscendC::LocalTensor<float> tensor = que.AllocTensor<float>(num);
}

//template<TPosition bufPos, uint32_t bufIDSize> friend class TBufPoolExtImpl;
void HostTestTBufPoolExtImpl() {
    AscendC::TBuf<AscendC::TPosition::LCM> buf;
}

//__aicore__ inline MutexID AllocMutexID();
void HostTestTPipeAllocMutexID() {
    AscendC::TPipe tpipe;
    AscendC::MutexID mutexID = tpipe.AllocMutexID();
}

//__aicore__ inline void ReleaseMutexID(MutexID id);
void HostTestTPipeReleaseMutexID() {
    AscendC::TPipe tpipe;
    AscendC::MutexID mutexID = tpipe.AllocMutexID();
    tpipe.ReleaseMutexID(mutexID);
}

//__aicore__ inline TBufId AllocTscmBufId();
void HostTestTPipeAllocTscmBufId() {
    AscendC::TPipe tpipe;
    AscendC::TBufId bufId = tpipe.AllocTscmBufId();
}

//__aicore__ inline int8_t AllocCrossSyncId();
void HostTestTPipeAllocCrossSyncId() {
    AscendC::TPipe tpipe;
    int8_t syncId = tpipe.AllocCrossSyncId();
}