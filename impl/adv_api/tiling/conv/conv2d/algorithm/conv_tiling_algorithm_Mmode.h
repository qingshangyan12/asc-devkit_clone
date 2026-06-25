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
 * \file conv_tiling_algorithm_Mmode.h
 * \brief
 */

#ifndef ASCENDC_TIKCFW_TILING_CONV_TILING_ALGORITHM_MMODE_H
#define ASCENDC_TIKCFW_TILING_CONV_TILING_ALGORITHM_MMODE_H

#include <cstdint>
#include <unordered_map>
#include "../common/conv_tiling_algorithm_base.h"
#include "../common/conv_tiling_base.h"
using namespace conv_tiling;
namespace conv_tiling_algo_m {
struct L1TilingFlag {
    L1TilingMode abL1Mode = L1TilingMode::INVALID;
    IterateMNOrder iterateMNOrder = IterateMNOrder::INVALID;
    bool kAL1CanFullLoadFlag = false;
    bool kBL1CanFullLoadFlag = false;
    bool kABL1CanFullLoadFlag = false;
    bool isBiasFullLoad = false;
    bool isFixpFullLoad = false;
    bool isWeightBypass = false;
};

struct L1TilingIdx {
    uint64_t kAL1Idx = 0;
    uint64_t kBL1Idx = 0;
    uint64_t mAL1Idx = 0;
    uint64_t nBL1Idx = 0;

    void SetIdx(uint64_t kAl1, uint64_t kBl1, uint64_t mAL1, uint64_t nBL1)
    {
        kAL1Idx = kAl1;
        kBL1Idx = kBl1;
        mAL1Idx = mAL1;
        nBL1Idx = nBL1;
    }
};

struct L0TilingRange {
    std::vector<uint64_t> mL0Range;
    std::vector<uint64_t> kL0Range;
    std::vector<uint64_t> nL0Range;
};

struct L0TilingParams {
    uint64_t mL0 = 0;
    uint64_t kL0 = 0;
    uint64_t nL0 = 0;
};

struct L0TilingIdx {
    uint64_t mL0Idx = 0;
    uint64_t nL0Idx = 0;
    uint64_t kL0Idx = 0;
};

struct L1TilingRange {
    std::vector<uint64_t> mAL1ValueRange;
    std::vector<uint64_t> nBL1ValueRange;
    std::vector<uint64_t> kAL1Range;
    std::vector<uint64_t> kBL1Range;
};

struct L1TilingParams {
    uint64_t kAL1 = 0;
    uint64_t kBL1 = 0;
    uint64_t mAL1Value = 0;
    uint64_t nBL1Value = 0;
};

struct L1TilingCalc {
    uint64_t ci0HkWk = 0;
    uint64_t fmapFullLoadL1Size = 0;
    uint64_t weightFullLoadL1Size = 0;
    uint64_t fmapKL1FullLoadSize = 0;
    uint64_t weightKL1FullLoadSize = 0;
    uint64_t fmapMinLoadL1Size = 0;
    uint64_t weightMinLoadL1Size = 0;
    uint64_t biasMinLoadL1Size = 0;
    uint64_t fixpMinLoadL1Size = 0;
};

class ConvTilingAlgorithmMmode : public ConvTilingAlgorithmBase {
public:
    explicit ConvTilingAlgorithmMmode(ConvTilingBase *tilingIns) : ConvTilingAlgorithmBase(tilingIns) {};
    ~ConvTilingAlgorithmMmode() override {};
    int64_t Process() override;
    void SetL1TilingRes() override;
    void SetL0TilingRes() override;
private:
    // L1 Tiling
    int64_t GetL1Tiling();
    bool CheckMinL1Tiling();
    int64_t PreProcessingL1Tiling();
    bool CheckL1Buffer();
    int64_t InitCalcL1Params();

    void GetL1TilingRange();
    void InitL1TiLing();
    void InitABL1TilingMode();
    void CoreL1TilingDecision();
    void FmapL1FullLoadIter();
    void WeightL1FullLoadIter();
    void InitKL1LoadFlag();
    void L1NoFullLoadIter();
    void KAL1FullLoadIter();
    void KBL1FullLoadIter();
    uint64_t KABL1FullLoadIterN();
    uint64_t KABL1FullLoadIterM();
    void NoneKABL1FullLoadIter();
    void BiasL1TilingDecision();
    void GetKL0TilingDecision();
    void WeightByPassDecision();
    void UpdateL1DoubelBuffer();

     // L0 tiling
    int64_t GetL0Tiling();
    void InitPingPong();
    void GetL0TilingRange();
    void L0TilingDecision();
    uint64_t CalcL1SizeForL0Tiling(uint64_t currmL0, uint64_t currnL0, uint64_t reserveWeightSize = 0);
    void CheckL0CDoubleBuffer();
    bool IsKAL1orKBL1FullLoad();
    bool IsKAL1andKBL1FullLoad();

    L0TilingRange l0TilingRange;
    L0TilingParams l0TilingParams;
    L0TilingIdx l0TilingIdx;

    L1TilingRange l1TilingRange;
    L1TilingParams l1TilingParams;
    L1TilingCalc l1TilingCalc;
    L1TilingIdx l1TilingIdx;
    L1TilingFlag l1TilingFlag;
    unordered_map<L1TilingMode, L1TilingIdx> l1TilingInitMap;
};
} // namespace conv_tiling_algo_m

#endif
