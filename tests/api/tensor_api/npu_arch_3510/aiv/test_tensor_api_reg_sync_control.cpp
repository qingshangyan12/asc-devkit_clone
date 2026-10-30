/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include <gtest/gtest.h>

#include "tensor_api/stub/cce_stub.h"
#include "tensor_api/experimental/arch/vector/reg_sync_control.h"

namespace {

using asc::te::experimental::mem_type;

#define CHECK_MEM_BAR_SUPPORTED(Src, Dst) \
    static_assert(asc::te::experimental::detail::is_mem_bar_supported<mem_type::Src, mem_type::Dst>::value)

CHECK_MEM_BAR_SUPPORTED(vec_store, vec_load);
CHECK_MEM_BAR_SUPPORTED(vec_load, vec_store);
CHECK_MEM_BAR_SUPPORTED(vec_store, vec_store);
CHECK_MEM_BAR_SUPPORTED(vec_store, scalar_load);
CHECK_MEM_BAR_SUPPORTED(vec_load, scalar_store);
CHECK_MEM_BAR_SUPPORTED(vec_store, scalar_store);
CHECK_MEM_BAR_SUPPORTED(scalar_store, vec_load);
CHECK_MEM_BAR_SUPPORTED(scalar_load, vec_store);
CHECK_MEM_BAR_SUPPORTED(scalar_store, vec_store);
CHECK_MEM_BAR_SUPPORTED(vec_all, vec_all);
CHECK_MEM_BAR_SUPPORTED(vec_all, scalar_all);
CHECK_MEM_BAR_SUPPORTED(scalar_all, vec_all);

#undef CHECK_MEM_BAR_SUPPORTED

static_assert(!asc::te::experimental::detail::is_mem_bar_supported<mem_type::vec_load, mem_type::vec_load>::value);

TEST(TensorApiVectorRegSyncControl3510, AllSupportedCombinations)
{
    asc::te::experimental::mem_bar<mem_type::vec_store, mem_type::vec_load>();
    asc::te::experimental::mem_bar<mem_type::vec_load, mem_type::vec_store>();
    asc::te::experimental::mem_bar<mem_type::vec_store, mem_type::vec_store>();
    asc::te::experimental::mem_bar<mem_type::vec_store, mem_type::scalar_load>();
    asc::te::experimental::mem_bar<mem_type::vec_load, mem_type::scalar_store>();
    asc::te::experimental::mem_bar<mem_type::vec_store, mem_type::scalar_store>();
    asc::te::experimental::mem_bar<mem_type::scalar_store, mem_type::vec_load>();
    asc::te::experimental::mem_bar<mem_type::scalar_load, mem_type::vec_store>();
    asc::te::experimental::mem_bar<mem_type::scalar_store, mem_type::vec_store>();
    asc::te::experimental::mem_bar<mem_type::vec_all, mem_type::vec_all>();
    asc::te::experimental::mem_bar<mem_type::vec_all, mem_type::scalar_all>();
    asc::te::experimental::mem_bar<mem_type::scalar_all, mem_type::vec_all>();
    SUCCEED();
}

} // namespace
