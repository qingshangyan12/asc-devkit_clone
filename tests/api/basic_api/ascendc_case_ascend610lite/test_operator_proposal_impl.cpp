/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include <gtest/gtest.h>
#include "kernel_operator.h"

using namespace AscendC;

namespace {
template <typename Func>
void ExpectUnsupportedProposalPath(Func func)
{
#if (defined(ASCENDC_CPU_DEBUG) && (ASCENDC_CPU_DEBUG == 1)) || defined(__NPU_HOST__) || defined(__ASCC_HOST__)
    EXPECT_DEATH({ func(); }, "");
#else
    func();
    SUCCEED();
#endif
}
} // namespace

TEST(TestDavM310ProposalImpl, Vmrgsort4UnsupportedPaths)
{
    ExpectUnsupportedProposalPath([] {
        __ubuf__ half* dst = nullptr;
        __ubuf__ half* addrArray[MRG_SORT_ELEMENT_LEN] = {};
        Vmrgsort4Cal(dst, addrArray, 0UL);
    });

    ExpectUnsupportedProposalPath([] {
        __ubuf__ half* dst = nullptr;
        __ubuf__ half* addrArray[MRG_SORT_ELEMENT_LEN] = {};
        Vmrgsort4Cal(dst, addrArray, 0UL, 0UL);
    });
}

TEST(TestDavM310ProposalImpl, VbitsortUnsupportedPaths)
{
    ExpectUnsupportedProposalPath([] {
        ProposalIntriParams params;
        VbitsortCal<half>(nullptr, nullptr, params);
    });

    ExpectUnsupportedProposalPath([] {
        ProposalIntriParams params;
        VbitsortCal<half>(nullptr, nullptr, nullptr, params);
    });
}

TEST(TestDavM310ProposalImpl, ConcatExtractAndResultUnsupportedPaths)
{
    ExpectUnsupportedProposalPath([] {
        ProposalIntriParams params;
        VconcatCal<half>(nullptr, nullptr, params);
    });

    ExpectUnsupportedProposalPath([] {
        ProposalIntriParams params;
        VextractCal<half>(nullptr, nullptr, params);
    });

    ExpectUnsupportedProposalPath([] {
        uint16_t result1 = 0;
        uint16_t result2 = 0;
        uint16_t result3 = 0;
        uint16_t result4 = 0;
        GetMrgSortResultImpl(result1, result2, result3, result4);
    });
}
