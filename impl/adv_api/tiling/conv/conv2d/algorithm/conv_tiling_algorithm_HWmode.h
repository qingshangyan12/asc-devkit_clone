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
 * \file conv_tiling_algorithm_HWmode.h
 * \brief
 */

#ifndef ASCENDC_TIKCFW_TILING_CONV_TILING_ALGORITHM_HWMODE_H
#define ASCENDC_TIKCFW_TILING_CONV_TILING_ALGORITHM_HWMODE_H

#include <cstdint>
#include <unordered_map>
#include "../common/conv_tiling_algorithm_base.h"
#include "../common/conv_tiling_base.h"

using namespace conv_tiling;
namespace conv_tiling_algo_hw {
struct L1TilingParams {
    uint64_t kAL1; // without khkw
    uint64_t kBL1;
    uint64_t hoAL1;
    uint64_t woAL1;
    uint64_t nBL1;
};

struct L0TilingParams {
    uint64_t hoL0Index = 0;
    uint64_t woL0Index = 0;
    uint64_t kL0Index = 0;
    uint64_t nL0Index = 0;
 
    uint64_t hoL0 = 0;
    uint64_t woL0 = 0;
    uint64_t kL0 = 0;
    uint64_t nL0 = 0;
};

struct L1TilingRange {
    std::vector<uint64_t> hoAL1Range;
    std::vector<uint64_t> woAL1Range;
    std::vector<uint64_t> nBL1Range;
    std::vector<uint64_t> kAL1Range;
    std::vector<uint64_t> kBL1Range;

    std::vector<uint64_t> hoAL1StrictRange;
    std::vector<uint64_t> woAL1StrictRange;
    std::vector<uint64_t> nBL1StrictRange;
    std::vector<uint64_t> kAL1StrictRange;
    std::vector<uint64_t> kBL1StrictRange;
};

struct L0TilingRange {
    std::vector<uint64_t> hoL0Range;
    std::vector<uint64_t> woL0Range;
    std::vector<uint64_t> kL0Range;
    std::vector<uint64_t> nL0Range;
};

struct L1TilingCalc {
    uint64_t aL1MaxSize = 0;
    uint64_t bL1MaxSize = 0;
    uint64_t biasL1MaxSize = 0;
    uint64_t fixpParamsL1MaxSize = 0;
    uint64_t hiAL1Min = 0;
    uint64_t wiAL1Min = 0;
    uint64_t hoAL1Min = 0;
    uint64_t woAL1Min = 0;
    uint64_t aL1Minsize = 0;
    uint64_t bL1Minsize = 0;
    uint64_t biasL1MinSize = 0;
    uint64_t fixpParamsL1MinSize = 0;
    uint64_t kAL1MaxSize = 0;
    uint64_t kBL1MaxSize = 0;
    uint64_t woAL1MaxSize = 0;
    uint64_t nBL1MaxSize = 0;
    uint64_t ci0HkWk = 0;
    uint64_t c04KSizeAlign = 0;
};

struct L1TilingInit {
    uint64_t kAL1Init;
    uint64_t kBL1Init;
    uint64_t hoAL1Init;
    uint64_t woAL1Init;
    uint64_t nBL1Init;
};

struct L1TilingFlag {
    L1TilingMode abL1Mode = L1TilingMode::INVALID;
    IterateMNOrder iterateMNOrder = IterateMNOrder::INVALID;
    bool isBiasFullLoad = false;
    bool isFixpParamsFullLoad = false;
    bool isWoAL1FullLoad = false;
    bool useWoAL1SpareRange = false;
    bool usehoAL1SpareRange = false;
    bool isWoL1MustFullLoadFlagC04 = true;
};

class ConvTilingAlgorithmHWmode : public ConvTilingAlgorithmBase {
public:
    explicit ConvTilingAlgorithmHWmode(ConvTilingBase *tilingIns) : ConvTilingAlgorithmBase(tilingIns)
    {
        this->minBurstNum = MIN_BURST_SIZE / DTYPE_SIZE_TAB.at(tilingIns_->descInfo.fMapType.dtype);
    };
    ~ConvTilingAlgorithmHWmode() override {};
    int64_t Process() override;
    void SetL1TilingRes() override;
    void SetL0TilingRes() override;
private:
    // L1 tiling
    void InitPingPong();
    uint64_t CalcCurL1Size(const L1TilingParams &inputTiling, uint64_t biasFixpParamNB);
    int64_t GetL1Tiling();
    void InitCalcL1Params();
    void InitCalcL1ParamsC04Mode();
    void GetL1TilingRange();
    void InitL1TiLing();
    void CoreL1TilingDecision();
    void CoreL1TilingNoneFullLoadDecision();
    bool IsAL1FullFload(const uint64_t currHoAL1, const uint64_t currWoAL1, const uint64_t currKAL1) const;
    bool IsBL1FullFload(const uint64_t currNBL1, const uint64_t currKBL1) const;
    void UpdateAL1DoubelBuffer();
    void UpdateBiasFixpParamsL1Fullload();
    void UpdateAL1DoubleBufferFirst();
    void UpdateAL1DoubleBufferSecond();
    int64_t CheckMinL1Tiling();
    uint64_t GetFixpParamsL1FullLoadSize() const;
    uint64_t GetFixpParamsL1MinLoadSize() const;
    int64_t InferWiL1(uint64_t inputWoL1, int64_t wi) const;
    uint32_t GetWeightBandWidthCoef() const;

    bool CheckBL1FullLoad();
    bool CheckAL1FullLoad();
    bool CheckABL1FullLoad();
    bool IsBL1FullLoadFirst();
    bool IsKBL1FullLoadFirst();
    void KAL1FullLoadFirst(uint64_t &tmpKAL1, uint64_t &tmpKBL1, uint64_t &tmpHoAL1,
                           uint64_t &tmpWoAL1, uint64_t &tmpNBL1);
    void KBL1FullLoadFirst(uint64_t &tmpKAL1, uint64_t &tmpKBL1, uint64_t &tmpHoAL1,
                           uint64_t &tmpWoAL1, uint64_t &tmpNBL1);
    bool CheckKL1FullLoad(uint64_t kAL1, uint64_t kBL1);
    bool CheckWoL0FullLoadC04Mode() const;

    void RestrictRange(const std::vector<uint64_t> &inputRange, std::vector<uint64_t> &strictRange,
                       uint64_t restriction) const;
    void InitABL1TilingMode();

    void IterKAL1(uint64_t &kAL1, uint64_t kBL1, uint64_t hoAL1, uint64_t woAL1, uint64_t nBL1);
    void IterKBL1(uint64_t kAL1, uint64_t &kBL1, uint64_t hoAL1, uint64_t woAL1, uint64_t nBL1);
    void IterNBL1(uint64_t kAL1, uint64_t kBL1, uint64_t hoAL1, uint64_t woAL1, uint64_t &nBL1);
    void IterHoAL1(uint64_t &hoAL1Res, const std::vector<uint64_t> &inputHoAL1Range, L1TilingParams &inputTiling);
    void IterWoAL1(uint64_t &woAL1Res, const std::vector<uint64_t> &inputWoAL1Range, L1TilingParams &inputTiling);
    void IterHoWoAL1(uint64_t kAL1, uint64_t kBL1, uint64_t &hoAL1, uint64_t &woAL1, uint64_t nBL1);
    void IterKABL1(uint64_t& tmpKAL1, uint64_t& tmpKBL1, uint64_t tmpHoAL1, uint64_t tmpWoAL1, uint64_t tmpNBL1);
    void GetCaseStatus();
    void GetHoWoNL0Tiling();
    void GetHoWoNL0TilingRange();
    void HoWoNL0TilingDecision();
    void UpdateL0TilingParamsIndex(bool& indexOverflowTag);
    void SetHoL0ForC04();
    void GetKL0Tiling();
    uint64_t CalcL1SizeForL0Tiling(uint64_t currHoL0, uint64_t currWoL0, uint64_t currNL0);
     // L0 tiling
    bool UpdateNL0();
    bool UpdateHoL0WoL0(uint64_t& value, uint64_t& index, std::vector<uint64_t>& range);

    void CheckL0DoubleBuffer();
    void Printl1TilingCalc();
    L1TilingCalc l1TilingCalc;
    L1TilingInit l1TilingInit;
    unordered_map<L1TilingMode, L1TilingInit> l1TilingInitMap;
    L1TilingRange l1TilingRange;
    L1TilingParams l1Params;
    L1TilingFlag l1Flags;
    L0TilingRange l0TilingRange;
    L0TilingParams l0Params;

    uint32_t minBurstNum = 0;
};

} // namespace conv_tiling_algo_hw

#endif