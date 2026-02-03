#ifndef VERIFY_SINGLE_HEADER
#include "kernel_operator.h"
#else
#include "kernel_operator_determine_compute_sync_intf.h"
#endif

// __aicore__ inline void InitDetermineComputeWorkspace(GlobalTensor<int32_t>& gmWorkspace, LocalTensor<int32_t>& ubWorkspace);
extern "C" __global__ __aicore__ void KernelTestInitDetermineComputeWorkspace1() {
    AscendC::GlobalTensor<int32_t> gmWorkspace;
    AscendC::LocalTensor<int32_t> ubWorkspace;
    AscendC::InitDetermineComputeWorkspace(gmWorkspace, ubWorkspace);
}

// __aicore__ inline void WaitPreBlock(GlobalTensor<int32_t>& gmWorkspace, LocalTensor<int32_t>& ubWorkspace);
extern "C" __global__ __aicore__ void KernelTestWaitPreBlock1() {
    AscendC::GlobalTensor<int32_t> gmWorkspace;
    AscendC::LocalTensor<int32_t> ubWorkspace;
    AscendC::WaitPreBlock(gmWorkspace, ubWorkspace);
}

// __aicore__ inline void NotifyNextBlock(GlobalTensor<int32_t>& gmWorkspace, LocalTensor<int32_t>& ubWorkspace);
extern "C" __global__ __aicore__ void KernelTestNotifyNextBlock1() {
    AscendC::GlobalTensor<int32_t> gmWorkspace;
    AscendC::LocalTensor<int32_t> ubWorkspace;
    AscendC::NotifyNextBlock(gmWorkspace, ubWorkspace);
}
