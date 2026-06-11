/**
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*!
 * \file conv_tiling_algorithm_base.h
 * \brief
 */
#ifndef ASCENDC_TIKCFW_TILING_CONV_TILING_ALGORITHM_BASE_H
#define ASCENDC_TIKCFW_TILING_CONV_TILING_ALGORITHM_BASE_H

#include "conv_tiling_base.h"

namespace conv_tiling {
enum class L1TilingMode {
    FULL_LOAD_BL1 = 0,
    FULL_LOAD_AL1,
    ALL_FULL_LOAD,
    NONE_FULL_LOAD,
    INVALID
};

struct PBufferParams {
    uint8_t pbAL1 = 1;
    uint8_t pbBL1 = 1;
    uint8_t pbAL0 = 1;
    uint8_t pbBL0 = 1;
    uint8_t pbCL0 = 1;
    uint64_t pBufferFlag = 0;
};

class ConvTilingAlgorithmBase {
public:
    explicit ConvTilingAlgorithmBase(ConvTilingBase *tilingIns);
    virtual ~ConvTilingAlgorithmBase() = default;
    virtual int64_t Process() = 0;

protected:
    virtual void SetL1TilingRes() = 0;
    virtual void SetL0TilingRes() = 0;

    uint64_t CalcAL0Size(uint64_t mL0, uint64_t kL0) const;
    uint64_t CalcBL0Size(uint64_t kL0, uint64_t nL0) const;
    uint64_t CalcCL0Size(uint64_t mL0, uint64_t nL0) const;
    uint64_t CalcBTSize(uint64_t nL0) const;
    uint64_t CalcFBSize(uint64_t nL0) const;
    uint64_t InferHiL1(uint64_t hoL1, int64_t hi) const;
    void SetPBufferRes();
    void PrintRanges(std::vector<uint64_t> inputRanges, std::string rangeName) const;
    bool CheckL0Buffer(uint64_t currmL0, uint64_t currkL0, uint64_t currnL0);
    ConvTilingBase* tilingIns_ = nullptr;
    PBufferParams dbValue;
    uint64_t fMapDTypeSize = 0;
    uint64_t weightDTypeSize = 0;
    uint64_t biasDTypeSize = 0;
    uint64_t quantScaleDtypeSize = 0;
};
} // namespace conv_tiling

#endif

