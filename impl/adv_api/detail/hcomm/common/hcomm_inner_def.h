/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

/*!
 * \file hcomm_inner_def.h
 * \brief
 */
#ifndef IMPL_HCOMM_HCOMM_INNER_DEF_H
#define IMPL_HCOMM_HCOMM_INNER_DEF_H

#include <cstdint>

namespace AscendC {

struct RdmaSqContext {
    uint32_t qpn;
    uint64_t sqVa;
    uint32_t wqeSize;
    uint32_t depth;
    uint64_t headAddr;
    uint64_t tailAddr;
    uint8_t sl;
    uint64_t dbVa;
    int8_t dbModel;
};

struct SqContext {
    uint32_t type;
    RdmaSqContext rdmaSqContext;
};

struct ChannelHandle {
    uint32_t sqNum;
    SqContext* sqContext;
};

struct hns_roce_rc_sq_wqe {
    uint32_t byte_4;
    uint32_t msg_len;
    uint32_t immtdata;
    uint32_t byte_16;
    uint32_t byte_20;
    uint32_t rkey;
    uint64_t remoteVA;
};


} // namespace AscendC
#endif // IMPL_HCOMM_HCOMM_INNER_DEF_H