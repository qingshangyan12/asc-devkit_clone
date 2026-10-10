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
 * \file conv2d_tiling_algorithm.h
 * \brief conv2d L1/L0 cut strategy and tiling algorithms
 */

#ifndef ADV_API_CONV_CONV2D_CONV2D_TILING_ALGORITHM_H
#define ADV_API_CONV_CONV2D_CONV2D_TILING_ALGORITHM_H

#include <cstdint>
#include "conv2d_tiling_util.h"

namespace Conv2dTilingApi {

#ifndef L1_ALIGN_SIZE
constexpr int L1_ALIGN_SIZE = 32;
#endif
#ifndef L0_PINGPONG
constexpr int L0_PINGPONG = 2;
#endif
#ifndef L1_PINGPONG
constexpr int L1_PINGPONG = 2;
#endif
#ifndef L1_PINGPONGPUNG
constexpr int L1_PINGPONGPUNG = 3;
#endif
#ifndef L1_PARTIAL_PINGPONG
constexpr int L1_PARTIAL_PINGPONG = 4; // 4 = 2(mingpong) * 2 (pingpong)
#endif
#ifndef L1_ELT_PINGPONG
constexpr int L1_ELT_PINGPONG = 2; // preload, ping pong
#endif

struct TilingL1BufferSize {
    int64_t inputSize = 0;
    int64_t filterSize = 0;
    int64_t outputSize = 0;
    int64_t eltwiseSize = 0;
    int64_t partialSize = 0;
};

struct L1SplitResult {
    int64_t l1FracH = 0;
    int64_t l1FracW = 0;
    int64_t l1FracCo = 0;
    int64_t l1FracCi = 0; // L1不切KhKw.
    int64_t l1FracHo = 0;
    int64_t l1FracWo = 0;
};

struct L0Frac {
    int64_t hi = 0;
    int64_t wi = 0;
    int64_t ci = 0;
    int64_t ho = 0;
    int64_t wo = 0;
    int64_t co = 0;
    int64_t kh = 0;
    int64_t kw = 0;
    int64_t k = 0;
};

class L1CutStrategy {
public:
    L1CutStrategy() {}
    virtual ~L1CutStrategy() = default;
    virtual int64_t CalcL1Split(const BufferInfo &bufferInfo, const Conv2dArgs &convArgs,
        optiling::TConv2DApiTiling &innerTiling) = 0;
    void InitL1Support(const Conv2dArgs &convArgs);
    int64_t CalcL1ParamSize(const Conv2dArgs &convArgs, int64_t fracCo);
    int64_t CalcFilterSize(const Conv2dArgs &convArgs, int64_t kH, int64_t kW, int64_t fracCi,
        int64_t fracCo, int64_t PingPongNum = 1);
    int64_t CalcInputSize(const Conv2dArgs &convArgs, int64_t H, int64_t W, int64_t fracCi, int64_t PingPongNum = 1);
    int64_t CalcEltSize(const Conv2dArgs &convArgs, int64_t Ho, int64_t Wo, int64_t fracCo);
    int64_t CalcOutputSize(const Conv2dArgs &convArgs, int64_t Ho, int64_t Wo, int64_t fracCo);
    int64_t CalcPartialSumSize(const Conv2dArgs &convArgs, int64_t Ho, int64_t Wo, int64_t fracCo);
    void SetTilingFlag(const Conv2dArgs &convArgs, uint32_t pBuffer, optiling::TConv2DApiTiling &innerTiling);
    bool isPartial = false;
    bool isL1PingPong = true;
    bool isFilterInL1 = true;
    bool isL1SupportDual = true;
    int64_t L0FracCo = 0;
    int64_t L0FracCi = 0;
    L1SplitResult l1SplitResult;
};

class L0CutStrategy {
public:
    L0CutStrategy() {}
    virtual ~L0CutStrategy() = default;
    virtual int64_t CalcL0Split(const BufferInfo &bufferInfo, const Conv2dArgs &convArgs) = 0;
    void InitL0AlignFrac(const Conv2dArgs &convArgs);
    int64_t maxN0_ = 0;
    int64_t minN0_ = 0;
    int64_t maxK0_ = 0;
    int64_t minK0_ = 0;
    bool isL0PingPong = true;
    L0Frac L0AlignFrac_ = {1, 1, 16, 1, 1, 16, 1, 1, 16};
    L0Frac L0MaxFrac_ = {4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096};
    L0Frac L0MinFrac_ = {1, 1, 16, 1, 1, 16, 1, 1, 16};
    L0Frac L0FracResult = {1, 1, 16, 1, 1, 16, 1, 1, 16};
};

class ConvTilingAlgorithm {
public:
    virtual ~ConvTilingAlgorithm() = default;
    void InitAlgo(const BufferInfo &bufferInfo, const Conv2dArgs &convArgs);
    virtual int64_t CalcTiling(const BufferInfo &bufferInfo, const Conv2dArgs &convArgs,
        optiling::TConv2DApiTiling &innerTilingResult) = 0;
    virtual const L1SplitResult &GetL1SplitResult() const = 0;
    virtual const L0Frac &GetL0SplitResult() const = 0;
};

class L1CutH : public L1CutStrategy {
public:
    int64_t CalcL1Split(const BufferInfo &bufferInfo, const Conv2dArgs &convArgs,
        optiling::TConv2DApiTiling &innerTilingResult) override;
    void CalcL1DataSize(const Conv2dArgs &convArgs, TilingL1BufferSize &L1DataSize, int64_t L1FracH, int64_t L1FracW,
        int64_t L1FracHo, int64_t L1FracWo, int64_t L1FracCi, int64_t FracCoL0);
};

class L1CutCo : public L1CutStrategy {
public:
    int64_t CalcL1Split(const BufferInfo &bufferInfo, const Conv2dArgs &convArgs,
        optiling::TConv2DApiTiling &innerTilingResult) override;
    void CalcL1DataSize(const Conv2dArgs &convArgs, TilingL1BufferSize &L1DataSize, int64_t L1FracCo, int64_t FracCoL0);
    bool IsL1CoSplitFit(const BufferInfo &bufferInfo, const Conv2dArgs &convArgs, int64_t curL0FracCo, int64_t l1Co,
        int64_t validSize);

private:
    struct L1CoBestSplit {
        int64_t l0FracCo = 0;
        int64_t l1Co = 0;
        bool found = false;
    };
    bool TryL1CoSplitFit(const BufferInfo &bufferInfo, const Conv2dArgs &convArgs, int64_t validSize,
        int64_t tryL0FracCo, int64_t tryL1Co, L1CoBestSplit &bestSplit);
};

class L1CutHWCo : public L1CutStrategy {
public:
    int64_t CalcL1Split(const BufferInfo &bufferInfo, const Conv2dArgs &convArgs,
        optiling::TConv2DApiTiling &innerTilingResult) override;
    int64_t GetL1OptimiFeatureSize(const BufferInfo &bufferInfo, const Conv2dArgs &convArgs, int64_t validSize,
        int64_t l1Co, int64_t curL0FracCo, int64_t &L1FracH, int64_t &L1FracW, int64_t &L1FracHOut,
        int64_t &L1FracWOut);
    bool TryL1HWCoSplit(const BufferInfo &bufferInfo, const Conv2dArgs &convArgs, int64_t curL0FracCo, int64_t l1Co,
        int64_t &L1FracH, int64_t &L1FracW, int64_t &L1FracHOut, int64_t &L1FracWOut, uint64_t &movInSize);
    static constexpr int64_t OUTPUTH_MINIMAL_HCUT = 4;
    static constexpr int64_t OUTPUTH_MINIMAL_WCUT = 2;

private:
    int64_t CalcL1FeatureSize(const Conv2dArgs &convArgs, int64_t filterSize, int64_t inH, int64_t inW, int64_t outH,
        int64_t outW, int64_t l1FracCi, int64_t curL0FracCo);

    struct L1HWCoBestSplit {
        int64_t l1Co = 0;
        int64_t l0FracCo = 0;
        int64_t l1FracH = 0;
        int64_t l1FracW = 0;
        int64_t l1FracHOut = 0;
        int64_t l1FracWOut = 0;
        uint64_t movInSize = UINT64_MAX;
    };
    bool TryL1HWCoSplitBest(const BufferInfo &bufferInfo, const Conv2dArgs &convArgs, int64_t tryL0FracCo,
        int64_t tryL1Co, L1HWCoBestSplit &bestSplit);
};

class L1CutHCiCo : public L1CutStrategy {
public:
    int64_t CalcL1Split(const BufferInfo &bufferInfo, const Conv2dArgs &convArgs,
        optiling::TConv2DApiTiling &innerTilingResult) override;
    void CalcSingleBaseJoint(const Conv2dArgs &convArgs, int64_t tryCi, int64_t FracCoL0, int64_t &baseL1FracH,
        int64_t &baseL1FracHo, int64_t &jointL1FracH, TilingL1BufferSize &baseDataSize,
        TilingL1BufferSize &jointDataSize);
    bool CalcL1JointNum(const BufferInfo &bufferInfo, const Conv2dArgs &convArgs, int64_t validSize,
        int64_t &outJointNum, const TilingL1BufferSize &baseDataSize, const TilingL1BufferSize &jointDataSize,
        int64_t baseL1FracHo);
    int64_t CalcMaxCiByL0B(const BufferInfo &bufferInfo, const Conv2dArgs &convArgs, int64_t fracCo);

private:
    void CalcL1DataSize(const Conv2dArgs &convArgs, TilingL1BufferSize &L1DataSize, int64_t L1FracH, int64_t L1FracW,
        int64_t L1FracHo, int64_t l1FracWo, int64_t L1FracCi, int64_t FracCoL0);

    struct L1HCiCoBestSplit {
        int64_t co = 0;
        int64_t ci = 0;
        int64_t jointNum = 0;
        int64_t baseHin = 0;
        int64_t baseHout = 0;
        int64_t jointHin = 0;
        TilingL1BufferSize baseDataSize;
        TilingL1BufferSize jointDataSize;
    };
    bool TrySingleBaseJoint(const BufferInfo &bufferInfo, const Conv2dArgs &convArgs, int64_t validSize, int64_t tryCo,
        int64_t tryCi, int64_t FracCoL0, L1HCiCoBestSplit &bestSplit);
};

class L0CutN : public L0CutStrategy {
public:
    int64_t CalcL0Split(const BufferInfo &bufferInfo, const Conv2dArgs &convArgs) override;
};

class L0CutKN : public L0CutStrategy {
public:
    int64_t CalcL0Split(const BufferInfo &bufferInfo, const Conv2dArgs &convArgs) override;
};

class L0CutKwN : public L0CutStrategy {
public:
    int64_t CalcL0Split(const BufferInfo &bufferInfo, const Conv2dArgs &convArgs) override;
};

class L0CutKhKwKN : public L0CutStrategy {
public:
    int64_t CalcL0Split(const BufferInfo &bufferInfo, const Conv2dArgs &convArgs) override;
};

class L0CutKNMaxN : public L0CutStrategy {
public:
    int64_t CalcL0Split(const BufferInfo &bufferInfo, const Conv2dArgs &convArgs) override;
};

class Tiling1CutHL0CutN : public ConvTilingAlgorithm, public L1CutH, public L0CutN {
public:
    int64_t CalcTiling(const BufferInfo &bufferInfo, const Conv2dArgs &convArgs,
        optiling::TConv2DApiTiling &innerTilingResult) override;
    const L1SplitResult &GetL1SplitResult() const override
    {
        return l1SplitResult;
    }
    const L0Frac &GetL0SplitResult() const override
    {
        return L0FracResult;
    }
};

class Tiling1CutHL0CutKN : public ConvTilingAlgorithm, public L1CutH, public L0CutKN {
public:
    int64_t CalcTiling(const BufferInfo &bufferInfo, const Conv2dArgs &convArgs,
        optiling::TConv2DApiTiling &innerTilingResult) override;
    const L1SplitResult &GetL1SplitResult() const override
    {
        return l1SplitResult;
    }
    const L0Frac &GetL0SplitResult() const override
    {
        return L0FracResult;
    }
};

class Tiling1CutHL0CutKwN : public ConvTilingAlgorithm, public L1CutH, public L0CutKwN {
public:
    int64_t CalcTiling(const BufferInfo &bufferInfo, const Conv2dArgs &convArgs,
        optiling::TConv2DApiTiling &innerTilingResult) override;
    const L1SplitResult &GetL1SplitResult() const override
    {
        return l1SplitResult;
    }
    const L0Frac &GetL0SplitResult() const override
    {
        return L0FracResult;
    }
};

class Tiling2CutCoL0CutKN : public ConvTilingAlgorithm, public L1CutCo, public L0CutKN {
public:
    int64_t CalcTiling(const BufferInfo &bufferInfo, const Conv2dArgs &convArgs,
        optiling::TConv2DApiTiling &innerTilingResult) override;
    const L1SplitResult &GetL1SplitResult() const override
    {
        return l1SplitResult;
    }
    const L0Frac &GetL0SplitResult() const override
    {
        return L0FracResult;
    }
};

class Tiling5CutHWCoL0CutKhKwKN : public ConvTilingAlgorithm, public L1CutHWCo, public L0CutKhKwKN {
public:
    int64_t CalcTiling(const BufferInfo &bufferInfo, const Conv2dArgs &convArgs,
        optiling::TConv2DApiTiling &innerTilingResult) override;
    const L1SplitResult &GetL1SplitResult() const override
    {
        return l1SplitResult;
    }
    const L0Frac &GetL0SplitResult() const override
    {
        return L0FracResult;
    }
};

class Tiling5CutHWCoL0CutKN : public ConvTilingAlgorithm, public L1CutHWCo, public L0CutKN {
public:
    int64_t CalcTiling(const BufferInfo &bufferInfo, const Conv2dArgs &convArgs,
        optiling::TConv2DApiTiling &innerTilingResult) override;
    const L1SplitResult &GetL1SplitResult() const override
    {
        return l1SplitResult;
    }
    const L0Frac &GetL0SplitResult() const override
    {
        return L0FracResult;
    }
};

class Tiling7CutHCiCoL0CutKN : public ConvTilingAlgorithm, public L1CutHCiCo, public L0CutKNMaxN {
public:
    int64_t CalcTiling(const BufferInfo &bufferInfo, const Conv2dArgs &convArgs,
        optiling::TConv2DApiTiling &innerTilingResult) override;
    const L1SplitResult &GetL1SplitResult() const override
    {
        return l1SplitResult;
    }
    const L0Frac &GetL0SplitResult() const override
    {
        return L0FracResult;
    }
};

} // namespace Conv2dTilingApi

#endif // ADV_API_CONV_CONV2D_CONV2D_TILING_ALGORITHM_H
