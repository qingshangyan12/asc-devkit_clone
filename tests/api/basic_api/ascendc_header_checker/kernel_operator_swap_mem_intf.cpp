#ifndef VERIFY_SINGLE_HEADER
#include "kernel_operator.h"
#else
#include "kernel_operator_swap_mem_intf.h"
#endif

// __aicore__ inline void SetSysWorkSpacePtr(__gm__ uint8_t* workspace)
extern "C" __global__ __aicore__ void KernelTestSetSysWorkSpacePtr1() {
    __gm__ uint8_t *workspace;
    SetSysWorkSpacePtr(workspace);
}

// __aicore__ inline __gm__ uint8_t* __gm__ GetSysWorkSpacePtr()
extern "C" __global__ __aicore__ void KernelTestGetSysWorkSpacePtr1() {
    __gm__ uint8_t *workspace = GetSysWorkSpacePtr();
}

