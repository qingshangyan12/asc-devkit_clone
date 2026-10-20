#include "basic_api/kernel_operator_vec_binary_intf.h"
#include "basic_api/kernel_operator_mm_intf.h"
#ifndef VECTOR_NEGATIVE_CASE
#define VECTOR_NEGATIVE_CASE 0
#endif
__aicore__ void CheckOperandConstraints(__ubuf__ float* dst, __ubuf__ float* src,
    __ubuf__ const float* readonly, __gm__ float* gm, __ubuf__ int32_t* integers,
    __cbuf__ half* l1, __ubuf__ half* ub)
{
#if VECTOR_NEGATIVE_CASE == 0
    AscendC::Add(dst, src, src, 32);
    AscendC::InitConstValueParams<half> params{};
    AscendC::Fill(l1, params);
#elif VECTOR_NEGATIVE_CASE == 1
    AscendC::Add(readonly, src, src, 32);
#elif VECTOR_NEGATIVE_CASE == 2
    AscendC::Add(gm, src, src, 32);
#elif VECTOR_NEGATIVE_CASE == 3
    AscendC::Add(dst, src, integers, 32);
#elif VECTOR_NEGATIVE_CASE == 4
    AscendC::InitConstValueParams<half> params{};
    AscendC::Fill(ub, params);
#elif VECTOR_NEGATIVE_CASE == 5
    AscendC::Add<int32_t>(dst, src, src, 32);
#endif
}
