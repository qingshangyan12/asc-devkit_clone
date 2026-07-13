/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "c_api/cache_ctrl/cache_ctrl.h"

static void test_host_c_api_cache_ctrl_0()
{
    (void)static_cast<void(*)(__gm__ uint64_t* int64_t)>(&asc_datacache_preload);
    (void)static_cast<void(*)()>(&asc_dcci_entire_all);
    (void)static_cast<void(*)()>(&asc_dcci_entire_atomic);
    (void)static_cast<void(*)()>(&asc_dcci_entire_out);
    (void)static_cast<void(*)()>(&asc_dcci_entire_ub);
    (void)static_cast<void(*)(__gm__ void*)>(&asc_dcci_single);
    (void)static_cast<void(*)()>(&asc_dci);
    (void)static_cast<int64_t(*)()>(&asc_get_icache_preload_status);
    (void)static_cast<void(*)(const void* int64_t)>(&asc_icache_preload);
    (void)static_cast<void(*)(__ubuf__ void*)>(&asc_ub_dcci_single);
}

