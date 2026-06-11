/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2026-2026. All rights reserved.
 * Description:
 */

#ifndef CONV2D_H
#define CONV2D_H

#include "kernel_operator.h" // [zj]TODO: #include "kernel_basic_intf.h"
#include "kernel_tiling/kernel_tiling.h"
// [zj]TODO: 黄蓝归一
#include "lib/conv/conv2d/conv2d_api.h"

using namespace AscendC;
using namespace Conv2dApi;

#define mDim hoDim

constexpr uint8_t SINGLE_BLOCK_SIZE = 32;
constexpr uint8_t M0 = 16;
constexpr uint8_t N0 = 16;

struct DimDataToFill {
    __aicore__ __forceinline__ DimDataToFill(uint64_t& singleCoreDim_, uint64_t& dimIdxStart_, bool& isDimTail_) :
        singleCoreDim(singleCoreDim_), dimIdxStart(dimIdxStart_), isDimTail(isDimTail_) {}

    uint64_t& singleCoreDim;
    uint64_t& dimIdxStart;
    bool& isDimTail;
};

template<int8_t FmapTiling, int8_t WeightTiling, int8_t L1PingPong, int8_t L0PingPong,
         int8_t OutputOrder, int8_t IterOrder, int8_t Bl1ByPass, int8_t GroupType>
struct Param : public Conv2dParam {
    __aicore__ inline Param()
    {
    }
    constexpr static int8_t fmapTiling = FmapTiling;
    constexpr static int8_t weightTiling = WeightTiling;
    constexpr static int8_t l1PingPong = L1PingPong;
    constexpr static int8_t l0PingPong = L0PingPong;
    constexpr static int8_t outputOrder = OutputOrder;
    constexpr static int8_t iterOrder = IterOrder;
    constexpr static int8_t bl1ByPass = Bl1ByPass;
    constexpr static int8_t groupType = GroupType;
};

template <class FMAP_TYPE, class WEIGHT_TYPE, class OUTPUT_TYPE, class BIAS_TYPE,
          class preQuant_TYPE, class PRE_RELU_TYPE, class PRE_CLIP_RELU_TYPE,
          class ELTWISE_TYPE, class ANTI_QUANT_TYPE,
          class POST_QUANT_TYPE, class POST_RELU_TYPE, class POST_CLIP_RELU_TYPE,
          class CONV_CFG>
class Conv2dBase {
public:
    __aicore__ inline Conv2dBase()
    {
    }

    __aicore__ inline void RunConv2dKernel(GM_ADDR x, GM_ADDR filter, GM_ADDR bias,
        GM_ADDR preQuant, GM_ADDR preRelu, GM_ADDR preClipRelu,
        GM_ADDR eltwise, GM_ADDR antiQuant,
        GM_ADDR postQuant, GM_ADDR postRelu, GM_ADDR postClipRelu,
        GM_ADDR y, const Conv2DTilingData& conv2dTilingData, TPipe* tPipe)
    {
        if (likely(Conv2dKernelInit(x, filter, bias,
                                    preQuant, preRelu, preClipRelu,
                                    eltwise, antiQuant,
                                    postQuant, postRelu, postClipRelu,
                                    y, conv2dTilingData, tPipe))) {
            Conv2dKernelImpl(conv2dTilingData);
        }
    }

protected:
    __aicore__ inline bool Conv2dKernelInit(GM_ADDR x, GM_ADDR filter, GM_ADDR bias,
                                            GM_ADDR preQuant, GM_ADDR preRelu, GM_ADDR preClipRelu,
                                            GM_ADDR eltwise, GM_ADDR antiQuant,
                                            GM_ADDR postQuant, GM_ADDR postRelu, GM_ADDR postClipRelu,
                                            GM_ADDR y, const Conv2DTilingData &conv2dTilingData, TPipe *tPipe)
    {
        conv2d.Init(&(conv2dTilingData.conv2dApiTiling), tPipe);

        if (unlikely(!InitSingleCoreData(conv2dTilingData))) {
            return false;
        }

        InitBuffer(x, filter, bias,
                   preQuant, preRelu, preClipRelu,
                   eltwise, antiQuant,
                   postQuant, postRelu, postClipRelu,
                   y, conv2dTilingData);

        return true;
    }

    __aicore__ inline bool InitSingleCoreData(const Conv2DTilingData &conv2dTilingData)
    {
        if constexpr (isMMode) {
            DimDataToFill batchToFill(singleCoreBatch, batchIdxStart, isBatchDimTail);
            bool isRealDim = CalcDimData(conv2dTilingData.conv2dRunInfo.mDim * conv2dTilingData.conv2dRunInfo.nDim,
                                         conv2dTilingData.conv2dRunInfo.batchDim, conv2dTilingData.conv2dRunInfo.batch,
                                         conv2dTilingData.conv2dRunInfo.batch, batchToFill);
            if (unlikely(!isRealDim)) {
                return false;
            }

            DimDataToFill nToFill(singleCoreN, nIdxStart, isNDimTail);
            isRealDim = CalcDimData(conv2dTilingData.conv2dRunInfo.mDim, conv2dTilingData.conv2dRunInfo.nDim,
                                    AlignB(conv2dTilingData.conv2dRunInfo.cout, N0),
                                    conv2dTilingData.conv2dRunInfo.cout, nToFill);
            if (unlikely(!isRealDim)) {
                return false;
            }

            DimDataToFill mToFill(singleCoreM, mIdxStart, isMDimTail);
            uint64_t totalM = conv2dTilingData.conv2dRunInfo.hout * conv2dTilingData.conv2dRunInfo.wout;
            isRealDim = CalcDimData(1, conv2dTilingData.conv2dRunInfo.mDim, AlignB(totalM, M0), totalM, mToFill);
            if (unlikely(!isRealDim)) {
                return false;
            }
        } else {
            DimDataToFill batchToFill(singleCoreBatch, batchIdxStart, isBatchDimTail);
            bool isRealDim = CalcDimData(conv2dTilingData.conv2dRunInfo.hoDim * conv2dTilingData.conv2dRunInfo.nDim,
                                         conv2dTilingData.conv2dRunInfo.batchDim, conv2dTilingData.conv2dRunInfo.batch,
                                         conv2dTilingData.conv2dRunInfo.batch, batchToFill);
            if (unlikely(!isRealDim)) {
                return false;
            }

            DimDataToFill nToFill(singleCoreN, nIdxStart, isNDimTail);
            isRealDim = CalcDimData(conv2dTilingData.conv2dRunInfo.hoDim, conv2dTilingData.conv2dRunInfo.nDim,
                                    AlignB(conv2dTilingData.conv2dRunInfo.cout, N0),
                                    conv2dTilingData.conv2dRunInfo.cout, nToFill);
            if (unlikely(!isRealDim)) {
                return false;
            }

            DimDataToFill hoToFill(singleCoreHo, hoIdxStart, isHoDimTail);
            isRealDim = CalcDimData(1, conv2dTilingData.conv2dRunInfo.hoDim, conv2dTilingData.conv2dRunInfo.hout,
                                    conv2dTilingData.conv2dRunInfo.hout, hoToFill);
            if (unlikely(!isRealDim)) {
                return false;
            }
        }

        return true;
    }

    __aicore__ inline bool CalcDimData(const uint32_t &blockPerDim, const uint32_t &dim, const uint64_t &wholeDim,
                                       const uint64_t &realWholeDim, DimDataToFill &curStruct)
    {
        const uint32_t dimIdx = (get_block_idx() / blockPerDim) % dim;
        const uint64_t maxDimPerCore = CeilDiv(wholeDim, dim);
        const uint64_t realDim = CeilDiv(realWholeDim, maxDimPerCore);

        if (unlikely(dimIdx >= realDim)) {
            return false;
        }

        curStruct.isDimTail = (dimIdx == (realDim - 1));
        curStruct.singleCoreDim = !curStruct.isDimTail ? maxDimPerCore : realWholeDim - (realDim - 1) * maxDimPerCore;
        curStruct.dimIdxStart = dimIdx * maxDimPerCore;
        return true;
    }

    __aicore__ inline void InitBuffer(GM_ADDR x, GM_ADDR filter, GM_ADDR bias,
                                      GM_ADDR preQuant, GM_ADDR preRelu, GM_ADDR preClipRelu,
                                      GM_ADDR eltwise, GM_ADDR antiQuant,
                                      GM_ADDR postQuant, GM_ADDR postRelu, GM_ADDR postClipRelu,
                                      GM_ADDR y, const Conv2DTilingData &conv2dTilingData)
    {
        CalcStartAddr(conv2dTilingData);
        InitBufferCommon(x, filter, bias,
                         preQuant, preRelu, preClipRelu,
                         eltwise, antiQuant,
                         postQuant, postRelu, postClipRelu,
                         y, conv2dTilingData);
    }

    __aicore__ inline void CalcStartAddr(const Conv2DTilingData &conv2dTilingData)
    {
        if constexpr (isMMode) {
            CalcStartAddrMMode(conv2dTilingData);
        } else {
            CalcStartAddrHWode(conv2dTilingData);
        }

        if (conv2dTilingData.conv2dApiTiling.hasBias) {
            biasStartAddr = nIdxStart;
        }

        if (conv2dTilingData.conv2dApiTiling.hasPreQuant) {
            preQuantStartAddr = nIdxStart;
        }

        if (conv2dTilingData.conv2dApiTiling.hasPreRelu) {
            preReluStartAddr = nIdxStart;
        }

        if (conv2dTilingData.conv2dApiTiling.hasPreClipRelu) {
            preClipReluStartAddr = nIdxStart;
        }

        if (conv2dTilingData.conv2dApiTiling.hasEltwise) {
            eltwiseStartAddr = conv2dTilingData.conv2dApiTiling.eltwiseBrcEn ? nIdxStart : outputStartAddr;
        }

        if (conv2dTilingData.conv2dApiTiling.hasAntiQuant) {
            antiQuantStartAddr = nIdxStart;
        }

        if (conv2dTilingData.conv2dApiTiling.hasPostQuant) {
            postQuantStartAddr = nIdxStart;
        }

        if (conv2dTilingData.conv2dApiTiling.hasPostRelu) {
            postReluStartAddr = nIdxStart;
        }

        if (conv2dTilingData.conv2dApiTiling.hasPostClipRelu) {
            postClipReluStartAddr = nIdxStart;
        }
    }

    __aicore__ inline void CalcStartAddrMMode(const Conv2DTilingData &conv2dTilingData)
    {
        // [zj]TODO: only support NCHW、NC1HWC0，other format to do
        singleCoreHiStartPos = 0;
        uint64_t hwIn = conv2dTilingData.conv2dRunInfo.hin * conv2dTilingData.conv2dRunInfo.win;
        if constexpr (FMAP_FORMAT == ConvFormat::NC1HWC0) {
            fmapOneBatchSize = AlignB(conv2dTilingData.conv2dRunInfo.cin, c0Fmap) * hwIn;
            fmapStartAddr = batchIdxStart * fmapOneBatchSize + mIdxStart * c0Fmap;
        } else if constexpr (FMAP_FORMAT == ConvFormat::NCHW) {
            fmapOneBatchSize = conv2dTilingData.conv2dRunInfo.cin * hwIn;
            fmapStartAddr = batchIdxStart * fmapOneBatchSize + mIdxStart;
        }

        if constexpr (WEIGHT_FORMAT == ConvFormat::FRACTAL_Z) {
            weightStartAddr = nIdxStart * AlignB(conv2dTilingData.conv2dRunInfo.cin, c0Fmap) *
                              conv2dTilingData.conv2dRunInfo.kh * conv2dTilingData.conv2dRunInfo.kw;
        }

        uint64_t hwOut = conv2dTilingData.conv2dRunInfo.hout * conv2dTilingData.conv2dRunInfo.wout;
        if constexpr (OUTPUT_FORMAT == ConvFormat::NC1HWC0) {
            outputOneBatchSize = AlignB(conv2dTilingData.conv2dRunInfo.cout, c0Output) * hwOut;
            outputStartAddr = batchIdxStart * outputOneBatchSize + nIdxStart / c0Output * hwOut * c0Output +
                              mIdxStart * c0Output + nIdxStart % c0Output;
        } else if constexpr (OUTPUT_FORMAT == ConvFormat::NCHW) {
            outputOneBatchSize = conv2dTilingData.conv2dRunInfo.cout * hwOut;
            outputStartAddr = batchIdxStart * outputOneBatchSize + nIdxStart * hwOut + mIdxStart;
        }
    }

    __aicore__ inline void CalcStartAddrHWode(const Conv2DTilingData &conv2dTilingData)
    {
        // [zj]TODO: only support NCHW、NC1HWC0，other format to do
        uint64_t hwIn = conv2dTilingData.conv2dRunInfo.hin * conv2dTilingData.conv2dRunInfo.win;
        int64_t hiStartPosTmp = (int64_t)(hoIdxStart * conv2dTilingData.conv2dRunInfo.strideH) -
                                (int64_t)conv2dTilingData.conv2dRunInfo.padTop;
        singleCoreHiStartPos = hiStartPosTmp < 0 ? 0 : hiStartPosTmp;
        if constexpr (FMAP_FORMAT == ConvFormat::NC1HWC0) {
            fmapOneBatchSize = AlignB(conv2dTilingData.conv2dRunInfo.cin, c0Fmap) * hwIn;
            fmapStartAddr = batchIdxStart * fmapOneBatchSize +
                            singleCoreHiStartPos * conv2dTilingData.conv2dRunInfo.win * c0Fmap;
        } else if constexpr (FMAP_FORMAT == ConvFormat::NCHW) {
            fmapOneBatchSize = conv2dTilingData.conv2dRunInfo.cin * hwIn;
            fmapStartAddr = batchIdxStart * fmapOneBatchSize +
                            singleCoreHiStartPos * conv2dTilingData.conv2dRunInfo.win;
        }

        if constexpr (WEIGHT_FORMAT == ConvFormat::FRACTAL_Z) {
            weightStartAddr = nIdxStart * AlignB(conv2dTilingData.conv2dRunInfo.cin, c0Fmap) *
                              conv2dTilingData.conv2dRunInfo.kh * conv2dTilingData.conv2dRunInfo.kw;
        }

        uint64_t hwOut = conv2dTilingData.conv2dRunInfo.hout * conv2dTilingData.conv2dRunInfo.wout;
        if constexpr (OUTPUT_FORMAT == ConvFormat::NC1HWC0) {
            outputOneBatchSize = AlignB(conv2dTilingData.conv2dRunInfo.cout, c0Output) * hwOut;
            outputStartAddr = batchIdxStart * outputOneBatchSize + nIdxStart / c0Output * hwOut * c0Output +
                              hoIdxStart * conv2dTilingData.conv2dRunInfo.wout * c0Output + nIdxStart % c0Output;
        } else if constexpr (OUTPUT_FORMAT == ConvFormat::NCHW) {
            outputOneBatchSize = conv2dTilingData.conv2dRunInfo.cout * hwOut;
            outputStartAddr = batchIdxStart * outputOneBatchSize + nIdxStart * hwOut +
                              hoIdxStart * conv2dTilingData.conv2dRunInfo.wout;
        }
    }

    __aicore__ inline void InitBufferCommon(GM_ADDR x, GM_ADDR filter, GM_ADDR bias,
                                            GM_ADDR preQuant, GM_ADDR preRelu, GM_ADDR preClipRelu,
                                            GM_ADDR eltwise, GM_ADDR antiQuant,
                                            GM_ADDR postQuant, GM_ADDR postRelu, GM_ADDR postClipRelu,
                                            GM_ADDR y, const Conv2DTilingData &conv2dTilingData)
    {
        fmapGm.SetGlobalBuffer(reinterpret_cast<__gm__ FMAP_T *>(x + fmapStartAddr * sizeof(FMAP_T)));
        filterGm.SetGlobalBuffer(reinterpret_cast<__gm__ WEIGHT_T *>(filter + weightStartAddr * sizeof(WEIGHT_T)));
        outputGm.SetGlobalBuffer(reinterpret_cast<__gm__ OUTPUT_T *>(y + outputStartAddr * sizeof(OUTPUT_T)));

        if (conv2dTilingData.conv2dApiTiling.hasBias) {
            biasGm.SetGlobalBuffer(
                reinterpret_cast<__gm__ BIAS_T *>(bias + biasStartAddr * sizeof(BIAS_T)));
        }

        if (conv2dTilingData.conv2dApiTiling.hasPreQuant) {
            preQuantGm.SetGlobalBuffer(
                reinterpret_cast<__gm__ PRE_QUANT_T *>(preQuant + preQuantStartAddr * sizeof(PRE_QUANT_T)));
        }

        if (conv2dTilingData.conv2dApiTiling.hasPreRelu) {
            preReluGm.SetGlobalBuffer(
                reinterpret_cast<__gm__ PRE_RELU_T *>(preRelu + preReluStartAddr * sizeof(PRE_RELU_T)));
        }

        if (conv2dTilingData.conv2dApiTiling.hasPreClipRelu) {
            preClipReluGm.SetGlobalBuffer(
                reinterpret_cast<__gm__ PRE_CLIP_RELU_T *>(preClipRelu + preClipReluStartAddr * sizeof(PRE_CLIP_RELU_T)));
        }

        if (conv2dTilingData.conv2dApiTiling.hasEltwise) {
            eltwiseGm.SetGlobalBuffer(
                reinterpret_cast<__gm__ ELTWISE_T *>(eltwise + eltwiseStartAddr * sizeof(ELTWISE_T)));
        }

        if (conv2dTilingData.conv2dApiTiling.hasAntiQuant) {
            antiQuantGm.SetGlobalBuffer(
                reinterpret_cast<__gm__ ANTI_QUANT_T *>(eltAntiQuant + antiQuantStartAddr * sizeof(ANTI_QUANT_T)));
        }

        if (conv2dTilingData.conv2dApiTiling.hasPostQuant) {
            postQuantGm.SetGlobalBuffer(
                reinterpret_cast<__gm__ POST_QUANT_T *>(quantPost + postQuantStartAddr * sizeof(POST_QUANT_T)));
        }

        if (conv2dTilingData.conv2dApiTiling.hasPostRelu) {
            postReluGm.SetGlobalBuffer(
                reinterpostt_cast<__gm__ POST_RELU_T *>(postRelu + postReluStartAddr * sizeof(POST_RELU_T)));
        }

        if (conv2dTilingData.conv2dApiTiling.hasPostClipRelu) {
            postClipReluGm.SetGlobalBuffer(
                reinterpostt_cast<__gm__ POST_CLIP_RELU_T *>(postClipRelu + postClipReluStartAddr * sizeof(POST_CLIP_RELU_T)));
        }
    }

    // Conv2d op kerenle impl intf
    __aicore__ inline void Conv2dKernelImpl(const Conv2DTilingData &conv2dTilingData)
    {
        if constexpr (isMMode) {
            if (unlikely(isNDimTail || isMDimTail)) {
                conv2d.SetSingleOutputShape(singleCoreN, singleCoreM);
            }
            conv2d.SetFmapStartPosition(mIdxStart);
        } else {
            if (unlikely(isNDimTail || isHoDimTail)) {
                conv2d.SetSingleOutputShape(singleCoreN, singleCoreHo, conv2dTilingData.conv2dRunInfo.wout);
            }
            conv2d.SetFmapStartPosition(singleCoreHiStartPos, 0);
        }

        conv2d.SetWeight(filterGm);

        if (conv2dTilingData.conv2dApiTiling.hasBias) {
            conv2d.SetBias(biasGm);
        }

        if (conv2dTilingData.conv2dApiTiling.hasPreQuant) {
            conv2d.SetPreQuant(preQuantGm);
        }

        if (conv2dTilingData.conv2dApiTiling.hasPreRelu) {
            conv2d.SetPreRelu(preReluGm);
        }

        if (conv2dTilingData.conv2dApiTiling.hasPreClipRelu) {
            conv2d.SetPreClipRelu(preClipReluGm);
        }

        if (conv2dTilingData.conv2dApiTiling.hasEltwise) {
            conv2d.SetEltwise(eltwiseGm);
        }

        if (conv2dTilingData.conv2dApiTiling.hasAntiQuant) {
            conv2d.SetAntiQuant(antiQuantGm);
        }

        if (conv2dTilingData.conv2dApiTiling.hasPostQuant) {
            conv2d.SetPostQuant(postQuantGm);
        }

        if (conv2dTilingData.conv2dApiTiling.hasPostRelu) {
            conv2d.SetPostRelu(postReluGm);
        }

        if (conv2dTilingData.conv2dApiTiling.hasPostClipRelu) {
            conv2d.SetPostClipRelu(postClipReluGm);
        }

        conv2d.SetSingleBatchNum(singleCoreBatch, fmapOneBatchSize, outputOneBatchSize); // [zj]TODO: check if need
        conv2d.SetFmap(fmapGm);
        conv2d.IterateAll(outputGm);
        conv2d.End();
    }

private:
    __aicore__ inline uint64_t AlignB(const uint64_t numA, const uint64_t numB)
    {
        return ((numA + numB - 1) / numB) * numB;
    }

    __aicore__ inline uint64_t CeilDiv(const uint64_t numA, const uint64_t numB)
    {
        return (numA + numB - 1) / numB;
    }

    __aicore__ inline int64_t Max(const int64_t numA, const int64_t numB)
    {
        return numA > numB ? numA : numB;
    }

public:
    // Get dtype
    using FMAP_T = typename FMAP_TYPE::T;
    using WEIGHT_T = typename WEIGHT_TYPE::T;
    using OUTPUT_T = typename OUTPUT_TYPE::T;
    using BIAS_T = typename BIAS_TYPE::T;
    using PRE_QUANT_T = typename PRE_QUANT_TYPE::T;
    using PRE_RELU_T = typename PRE_RELU_TYPE::T;
    using PRE_CLIP_RELU_T = typename PRE_CLIP_RELU_TYPE::T;
    using ELTWISE_T = typename ELTWISE_TYPE::T;
    using ANTI_QUANT_T = typename ANTI_QUANT_TYPE::T;
    using POST_QUANT_T = typename POST_QUANT_TYPE::T;
    using POST_RELU_T = typename POST_RELU_TYPE::T;
    using POST_CLIP_RELU_T = typename POST_CLIP_RELU_TYPE::T;

    // Conv2d API
    Conv2d<FMAP_TYPE, WEIGHT_TYPE, OUTPUT_TYPE, OUTPUT_TYPE, BIAS_TYPE,
           PRE_QUANT_TYPE, PRE_RELU_TYPE, PRE_CLIP_RELU_TYPE,
           ELTWISE_TYPE, ANTI_QUANT_TYPE,
           POST_QUANT_TYPE, POST_RELU_TYPE, POST_CLIP_RELU_TYPE, CONV_CFG> conv2d;

    // Input and output tensor declare
    GlobalTensor<FMAP_T> fmapGm;
    GlobalTensor<WEIGHT_T> filterGm;
    GlobalTensor<OUTPUT_T> outputGm;
    GlobalTensor<BIAS_T> biasGm;
    GlobalTensor<PRE_QUANT_T> preQuantGm;
    GlobalTensor<PRE_RELU_T> preReluGm;
    GlobalTensor<PRE_RELU_T> preClipReluGm;
    GlobalTensor<ELTWISE_T> eltwiseGm;
    GlobalTensor<ANTI_QUANT_T> antiQuantGm;
    GlobalTensor<POST_QUANT_T> postQuantGm;
    GlobalTensor<POST_RELU_T> postReluGm;
    GlobalTensor<POST_CLIP_RELU_T> postClipReluGm;

    uint64_t batchIdxStart;
    uint64_t nIdxStart;
    uint64_t mIdxStart;
    uint64_t hoIdxStart;

    // Single core data
    uint64_t singleCoreBatch;
    uint64_t singleCoreN;
    uint64_t singleCoreM;
    uint64_t singleCoreHo;
    int64_t singleCoreHiStartPos;

    bool isBatchDimTail;
    bool isNDimTail;
    bool isMDimTail;
    bool isHoDimTail;

    uint64_t fmapOneBatchSize;
    uint64_t outputOneBatchSize;

    constexpr static ConvFormat FMAP_FORMAT = FMAP_TYPE::format;
    constexpr static ConvFormat WEIGHT_FORMAT = WEIGHT_TYPE::format;
    constexpr static ConvFormat OUTPUT_FORMAT = OUTPUT_TYPE::format;

    constexpr static bool isMMode = CONV_CFG::outputOrder == static_cast<int8_t>(ConvOutputOrder::M_MODE);

    uint64_t fmapStartAddr;
    uint64_t weightStartAddr;
    uint64_t outputStartAddr;
    uint64_t biasStartAddr;
    uint64_t preQuantStartAddr;
    uint64_t preReluStartAddr;
    uint64_t preClipReluStartAddr;
    uint64_t eltwiseStartAddr;
    uint64_t antiQuantStartAddr;
    uint64_t postQuantStartAddr;
    uint64_t postReluStartAddr;
    uint64_t postClipReluStartAddr;

    constexpr static uint64_t c0Fmap = SINGLE_BLOCK_SIZE / sizeof(FMAP_T);
    constexpr static uint64_t c0Weight = SINGLE_BLOCK_SIZE / sizeof(WEIGHT_T);
    constexpr static uint64_t c0Output = SINGLE_BLOCK_SIZE / sizeof(OUTPUT_T);
};

#endif // CONV2D_H