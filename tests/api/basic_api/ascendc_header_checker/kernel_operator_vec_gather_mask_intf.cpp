#ifndef VERIFY_SINGLE_HEADER
#include "kernel_operator.h"
#else
#include "kernel_operator_vec_gather_mask_intf.h"
#endif

#if __NPU_ARCH__ != 1001
// __aicore__ inline void GatherMask(const LocalTensor<T>& dst, const LocalTensor<T>& src0, const LocalTensor<U>& src1Pattern, const bool reduceMode, const uint32_t mask, const GatherMaskParams& gatherMaskParams, uint64_t& rsvdCnt);
extern "C" __global__ __aicore__ void KernelTestGatherMask1() {
    AscendC::LocalTensor<uint16_t> dst;
    AscendC::LocalTensor<uint16_t> src0;
    AscendC::LocalTensor<uint16_t> src1Pattern;
    bool reduceMode = true;
    uint32_t mask = 0;
    AscendC::GatherMaskParams gatherMaskParams;
    uint64_t rsvdCnt = 0xFFFFFFFFFFFFFFFF;
    AscendC::GatherMask(dst, src0, src1Pattern, reduceMode, mask, gatherMaskParams, rsvdCnt);
}

// __aicore__ inline void GatherMask(const LocalTensor<T>& dst, const LocalTensor<T>& src0, const uint8_t src1Pattern, const bool reduceMode, const uint32_t mask, const GatherMaskParams& gatherMaskParams, uint64_t& rsvdCnt);
extern "C" __global__ __aicore__ void KernelTestGatherMask2() {
    AscendC::LocalTensor<uint16_t> dst;
    AscendC::LocalTensor<uint16_t> src0;
    uint8_t src1Pattern = 1;
    bool reduceMode = true;
    uint32_t mask = 0;
    AscendC::GatherMaskParams gatherMaskParams;
    uint64_t rsvdCnt = 0xFFFFFFFFFFFFFFFF;
    AscendC::GatherMask(dst, src0, src1Pattern, reduceMode, mask, gatherMaskParams, rsvdCnt);
}
#endif
