/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

/* M510 (npu_arch_5102) specific: expdiff / VexpdifFloat gap.
 *
 * On 3510, `asc_exp_sub*` maps to the dedicated `vexpdif` instruction
 * (see npu_arch_3510/vector_compute/test_return_vconv.cpp). M510 does NOT have
 * this dedicated fp16 expdiff instruction (RFC 4.5).
 *
 * This file is a PLACEHOLDER pending the RFC 4.5 decision:
 *   1) Do not expose asc_exp_sub* on M510;
 *   2) Emulate via generic Exp (mock `vexp` instead of `vexpdif`); or
 *   3) Compile-time static_assert / remove declaration so callers fail fast.
 *
 * Once the decision is made, replace the placeholder below with the concrete
 * test (e.g. mock `vexp` and assert the emulation path, or assert a
 * static_assert that the API is unavailable on __NPU_ARCH__ == 5102). */

#include <gtest/gtest.h>

TEST(ExpSubM510Policy, PolicyDocumentedAndPendingDecision)
{
    // Placeholder: no functional assertion yet. Tracks RFC 4.5 decision.
    // Intentionally left as a documentation marker until the capi M510
    // expdiff strategy is finalized.
    SUCCEED();
}
