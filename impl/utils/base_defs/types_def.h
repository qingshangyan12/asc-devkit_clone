/**
* Copyright (c) 2025 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/

/*!
 * \file types_def.h
 * \brief used to store definitions of public structures, unions, and classes
 */
#ifndef IMPL_UTILS_BASE_DEFS_TYPES_DEF_H
#define IMPL_UTILS_BASE_DEFS_TYPES_DEF_H

struct BaseTlv {  // TLV头部定义
    unsigned short type;
    unsigned short len;
};

struct BinaryMetaVersion {
    BaseTlv head;      // B_TYPE_BIN_VERSION = 0
    uint32_t version;  // version info
};

namespace AscendC {
// Hardware
enum class Hardware : uint8_t { GM, UB, L1, L0A, L0B, L0C, BIAS, FIXBUF, MAX };
// TPosition
enum class TPosition : uint8_t {
    GM,
    A1,
    A2,
    B1,
    B2,
    C1,
    C2,
    CO1,
    CO2,
    VECIN,
    VECOUT,
    VECCALC,
    LCM = VECCALC,
    SPM,
    SHM = SPM,
    TSCM,
    C2PIPE2GM,
    C2PIPE2LOCAL,
    MAX,
};

}

using Hardware = AscendC::Hardware; // for capi

#endif