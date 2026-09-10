/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include <cstdio>
#include <csignal>
#include <gtest/gtest.h>
#include <unistd.h>
#include "utils/debug/asc_assert.h"
#include "utils/debug/asc_printf.h"
#include "utils/debug/asc_dump.h"

#if defined(ASCENDC_CPU_DEBUG) && ASCENDC_CPU_DEBUG == 1 && __NPU_ARCH__ == 3510
namespace {
__aicore__ void CheckSimd(bool condition, bool useAlias, int& evaluations)
{
    if (useAlias) {
        ascendc_assert(condition, "value=%d text=%s real=%.1f", ++evaluations, "cpu", 1.5f);
    } else {
        assert(condition, "value=%d text=%s real=%.1f", ++evaluations, "cpu", 1.5f);
    }
}

__simd_vf__ void CheckSimdVf(bool condition, bool useAlias, int& evaluations)
{
    if (useAlias) {
        ascendc_assert(condition, "value=%d text=%s real=%.1f", ++evaluations, "cpu", 1.5f);
    } else {
        assert(condition, "value=%d text=%s real=%.1f", ++evaluations, "cpu", 1.5f);
    }
}
} // namespace

TEST(CpuAssertTest, SuccessfulChecksDoNotEvaluateMessageArguments)
{
    int conditions = 0;
    int evaluations = 0;
    assert((++conditions, true), "value=%d", ++evaluations);
    ascendc_assert((++conditions, true), "value=%d", ++evaluations);
    for (bool useAlias : {false, true}) {
        CheckSimd(true, useAlias, evaluations);
        CheckSimdVf(true, useAlias, evaluations);
    }
#if !defined(NDEBUG) && !(defined(ASCENDC_DUMP) && ASCENDC_DUMP == 0)
    EXPECT_EQ(conditions, 2);
#else
    EXPECT_EQ(conditions, 0);
#endif
    EXPECT_EQ(evaluations, 0);
}

TEST(CpuAssertTest, FailedChecksPrintArgumentsAndAbort)
{
    int evaluations = 0;
    for (bool useAlias : {false, true}) {
#if !defined(NDEBUG) && !(defined(ASCENDC_DUMP) && ASCENDC_DUMP == 0)
        EXPECT_EXIT({
            dup2(STDERR_FILENO, STDOUT_FILENO);
            CheckSimd(false, useAlias, evaluations);
        }, testing::KilledBySignal(SIGABRT), "Assertion .*value=1 text=cpu real=1[.]5");
        EXPECT_EXIT({
            dup2(STDERR_FILENO, STDOUT_FILENO);
            CheckSimdVf(false, useAlias, evaluations);
        }, testing::KilledBySignal(SIGABRT), "Assertion .*value=1 text=cpu real=1[.]5");
#else
        CheckSimd(false, useAlias, evaluations);
        CheckSimdVf(false, useAlias, evaluations);
#endif
    }
    EXPECT_EQ(evaluations, 0);
}

TEST(CpuAssertTest, FormatOnlySupportsRuntimeStringsAndPercentEscapes)
{
    const char* format = "progress=100%%";
    (void)format;
#if !defined(NDEBUG) && !(defined(ASCENDC_DUMP) && ASCENDC_DUMP == 0)
    EXPECT_EXIT({
        dup2(STDERR_FILENO, STDOUT_FILENO);
        assert(false, format);
    }, testing::KilledBySignal(SIGABRT), "Assertion .*progress=100%$");
    EXPECT_EXIT({
        dup2(STDERR_FILENO, STDOUT_FILENO);
        ascendc_assert(false, format);
    }, testing::KilledBySignal(SIGABRT), "Assertion .*progress=100%$");
#else
    assert(false, format);
    ascendc_assert(false, format);
#endif
}
#endif
