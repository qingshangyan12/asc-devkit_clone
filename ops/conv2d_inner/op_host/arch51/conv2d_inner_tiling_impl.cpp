/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2026-2026. All rights reserved.
 * Description:
 */

#include "conv2d_inner_tiling_impl.h"

namespace optiling {
bool Conv2DInnerTiling::IsCapable()
{
    // 待补充实现
    return true;
}

// 1、获取平台信息比如CoreNum、UB/L1/L0C资源大小
uint32_t Conv2DInnerTiling::GetPlatformInfo()
{
    // 待补充实现
    return npucl::SUCCESS;
}

// 2、获取INPUT/OUTPUT/ATTR信息
uint32_t Conv2DInnerTiling::GetShapeAttrsInfo()
{
    // 待补充实现
    return npucl::SUCCESS;
}

// 3、计算数据切分TilingData
uint32_t Conv2DInnerTiling::DoOpTiling()
{
    // 待补充实现
    conv2DInnerTilingData_.conv2dRunInfo.set_hi(0);
    conv2DInnerTilingData_.conv2dRunInfo.set_wi(0);
    conv2DInnerTilingData_.conv2dRunInfo.set_ho(0);
    conv2DInnerTilingData_.conv2dRunInfo.set_wo(0);
    conv2DInnerTilingData_.conv2dRunInfo.set_batch(0);
    conv2DInnerTilingData_.conv2dRunInfo.set_ci(0);
    conv2DInnerTilingData_.conv2dRunInfo.set_co(0);
    conv2DInnerTilingData_.conv2dRunInfo.set_kernelH(0);
    conv2DInnerTilingData_.conv2dRunInfo.set_kernelW(0);
    conv2DInnerTilingData_.conv2dRunInfo.set_batchDim(0);
    conv2DInnerTilingData_.conv2dRunInfo.set_coDim(0);
    conv2DInnerTilingData_.conv2dRunInfo.set_hoDim(0);
    conv2DInnerTilingData_.conv2dRunInfo.set_strideH(0);
    conv2DInnerTilingData_.conv2dRunInfo.set_padTop(0);
    conv2DInnerTilingData_.conv2dRunInfo.set_groupDim(0);
    conv2DInnerTilingData_.conv2dRunInfo.set_groups(0);
    return npucl::SUCCESS;
}

// 4、计算高阶API的TilingData
uint32_t Conv2DInnerTiling::DoLibApiTiling()
{
    // 待补充实现
    conv2DInnerTilingData_.conv2dApiTiling.set_orgHi(0);
    conv2DInnerTilingData_.conv2dApiTiling.set_orgWi(0);
    conv2DInnerTilingData_.conv2dApiTiling.set_oriHixWi(0);
    conv2DInnerTilingData_.conv2dApiTiling.set_orgHo(0);
    conv2DInnerTilingData_.conv2dApiTiling.set_orgWo(0);
    conv2DInnerTilingData_.conv2dApiTiling.set_singleCoreHo(0);
    conv2DInnerTilingData_.conv2dApiTiling.set_singleCoreWo(0);
    conv2DInnerTilingData_.conv2dApiTiling.set_orgCi(0);
    conv2DInnerTilingData_.conv2dApiTiling.set_orgCo(0);
    conv2DInnerTilingData_.conv2dApiTiling.set_singleCoreCi(0);
    conv2DInnerTilingData_.conv2dApiTiling.set_singleCoreCo(0);
    conv2DInnerTilingData_.conv2dApiTiling.set_kernelH(0);
    conv2DInnerTilingData_.conv2dApiTiling.set_kernelW(0);
    conv2DInnerTilingData_.conv2dApiTiling.set_kernelHxkernelW(0);
    conv2DInnerTilingData_.conv2dApiTiling.set_strideH(0);
    conv2DInnerTilingData_.conv2dApiTiling.set_strideW(0);
    conv2DInnerTilingData_.conv2dApiTiling.set_dilationH(0);
    conv2DInnerTilingData_.conv2dApiTiling.set_dilationW(0);
    conv2DInnerTilingData_.conv2dApiTiling.set_padTop(0);
    conv2DInnerTilingData_.conv2dApiTiling.set_padBottom(0);
    conv2DInnerTilingData_.conv2dApiTiling.set_padLeft(0);
    conv2DInnerTilingData_.conv2dApiTiling.set_padRight(0);
    conv2DInnerTilingData_.conv2dApiTiling.set_groups(0);
    conv2DInnerTilingData_.conv2dApiTiling.set_hoL1(0);
    conv2DInnerTilingData_.conv2dApiTiling.set_woL1(0);
    conv2DInnerTilingData_.conv2dApiTiling.set_kAL1(0);
    conv2DInnerTilingData_.conv2dApiTiling.set_kBL1(0);
    conv2DInnerTilingData_.conv2dApiTiling.set_nBL1(0);
    conv2DInnerTilingData_.conv2dApiTiling.set_hoL0(0);
    conv2DInnerTilingData_.conv2dApiTiling.set_woL0(0);
    conv2DInnerTilingData_.conv2dApiTiling.set_kL0(0);
    conv2DInnerTilingData_.conv2dApiTiling.set_nL0(0);
    conv2DInnerTilingData_.conv2dApiTiling.set_khL0(0);
    conv2DInnerTilingData_.conv2dApiTiling.set_kwL0(0);
    conv2DInnerTilingData_.conv2dApiTiling.set_pBufferFlag(0);
    conv2DInnerTilingData_.conv2dApiTiling.set_cinAInCore(0);
    conv2DInnerTilingData_.conv2dApiTiling.set_cinATailInCore(0);
    conv2DInnerTilingData_.conv2dApiTiling.set_cinBInCore(0);
    conv2DInnerTilingData_.conv2dApiTiling.set_cinBTailInCore(0);
    conv2DInnerTilingData_.conv2dApiTiling.set_a1SpaceSize(0);
    conv2DInnerTilingData_.conv2dApiTiling.set_b1SpaceSize(0);
    conv2DInnerTilingData_.conv2dApiTiling.set_c0SpaceSize(0);
    conv2DInnerTilingData_.conv2dApiTiling.set_multiNBL1(0);
    conv2DInnerTilingData_.conv2dApiTiling.set_mStep(0);
    conv2DInnerTilingData_.conv2dApiTiling.set_kStep(0);
    conv2DInnerTilingData_.conv2dApiTiling.set_nStep(0);
    conv2DInnerTilingData_.conv2dApiTiling.set_biasFullLoadFlag(0);
    conv2DInnerTilingData_.conv2dApiTiling.set_fixpParamsFullLoadFlag(0);
    conv2DInnerTilingData_.conv2dApiTiling.set_iterateMNOrder(0);
    conv2DInnerTilingData_.conv2dApiTiling.set_isC04Flag(0);
    conv2DInnerTilingData_.conv2dApiTiling.set_hasBias(0);
    conv2DInnerTilingData_.conv2dApiTiling.set_hasPreQuant(0);
    conv2DInnerTilingData_.conv2dApiTiling.set_hasPreRelu(0);
    conv2DInnerTilingData_.conv2dApiTiling.set_hasPreClipRelu(0);
    conv2DInnerTilingData_.conv2dApiTiling.set_hasAntiQuant(0);
    conv2DInnerTilingData_.conv2dApiTiling.set_hasEltwise(0);
    conv2DInnerTilingData_.conv2dApiTiling.set_hasPostQuant(0);
    conv2DInnerTilingData_.conv2dApiTiling.set_hasPostRelu(0);
    conv2DInnerTilingData_.conv2dApiTiling.set_hasPostClipRelu(0);
    conv2DInnerTilingData_.conv2dApiTiling.set_hasPadding(0);
    conv2DInnerTilingData_.conv2dApiTiling.set_preQuantMode(0);
    conv2DInnerTilingData_.conv2dApiTiling.set_preReluMode(0);
    conv2DInnerTilingData_.conv2dApiTiling.set_preClipReluMode(0);
    conv2DInnerTilingData_.conv2dApiTiling.set_antiQuantMode(0);
    conv2DInnerTilingData_.conv2dApiTiling.set_eltwiseOp(0);
    conv2DInnerTilingData_.conv2dApiTiling.set_eltwiseBrcEn(0);
    conv2DInnerTilingData_.conv2dApiTiling.set_postQuantMode(0);
    conv2DInnerTilingData_.conv2dApiTiling.set_postReluMode(0);
    conv2DInnerTilingData_.conv2dApiTiling.set_postClipReluMode(0);
    conv2DInnerTilingData_.conv2dApiTiling.set_paddingMode(0);
    conv2DInnerTilingData_.conv2dApiTiling.set_dualOutput(0);
    return npucl::SUCCESS;
}

// 5、计算TilingKey
ge::graphStatus Conv2DInnerTiling::GetTplTilingKey()
{
    // 待补充实现
    tilingKey_ = GET_TPL_TILING_KEY(0, // FmapTiling
                                    0, // WeightTiling
                                    0, // L1PingPong
                                    0, // L0PingPong
                                    0, // OutputOrder
                                    0, // IterOrder
                                    0, // Bl1ByPass
                                    0); // GroupType
    return npucl::SUCCESS;
}

uint64_t Conv2DInnerTiling::GetTilingKey() const
{
    return tilingKey_;
}

// 6、计算Workspace 大小
uint32_t Conv2DInnerTiling::GetWorkspaceSize()
{
    // 暂无workspace需求
    workspaceSize_ = 0;
    return npucl::SUCCESS;
}

// 7、保存Tiling数据
uint32_t Conv2DInnerTiling::PostTiling()
{
    size_t *currentWorkspace = context_->GetWorkspaceSizes(0);
    if (currentWorkspace != nullptr) {
        currentWorkspace[0] = workspaceSize_;
    }
    conv2DInnerTilingData_.SaveToBuffer(context_->GetRawTilingData()->GetData(),
                                        context_->GetRawTilingData()->GetCapacity());
    context_->GetRawTilingData()->SetDataSize(conv2DInnerTilingData_.GetDataSize());
    context_->SetBlockDim(blockDim_);
    return npucl::SUCCESS;
}

void Conv2DInnerTiling::DumpTilingInfoApi(nlohmann::json &apiTiling)
{
    apiTiling["orgHi"]                  = conv2DInnerTilingData_.conv2dApiTiling.get_orgHi();
    apiTiling["orgWi"]                  = conv2DInnerTilingData_.conv2dApiTiling.get_orgWi();
    apiTiling["oriHixWi"]               = conv2DInnerTilingData_.conv2dApiTiling.get_oriHixWi();
    apiTiling["orgHo"]                  = conv2DInnerTilingData_.conv2dApiTiling.get_orgHo();
    apiTiling["orgWo"]                  = conv2DInnerTilingData_.conv2dApiTiling.get_orgWo();
    apiTiling["singleCoreHo"]           = conv2DInnerTilingData_.conv2dApiTiling.get_singleCoreHo();
    apiTiling["singleCoreWo"]           = conv2DInnerTilingData_.conv2dApiTiling.get_singleCoreWo();
    apiTiling["orgCi"]                  = conv2DInnerTilingData_.conv2dApiTiling.get_orgCi();
    apiTiling["orgCo"]                  = conv2DInnerTilingData_.conv2dApiTiling.get_orgCo();
    apiTiling["singleCoreCi"]           = conv2DInnerTilingData_.conv2dApiTiling.get_singleCoreCi();
    apiTiling["singleCoreCo"]           = conv2DInnerTilingData_.conv2dApiTiling.get_singleCoreCo();
    apiTiling["kernelH"]                = conv2DInnerTilingData_.conv2dApiTiling.get_kernelH();
    apiTiling["kernelW"]                = conv2DInnerTilingData_.conv2dApiTiling.get_kernelW();
    apiTiling["kernelHxkernelW"]        = conv2DInnerTilingData_.conv2dApiTiling.get_kernelHxkernelW();
    apiTiling["strideH"]                = conv2DInnerTilingData_.conv2dApiTiling.get_strideH();
    apiTiling["strideW"]                = conv2DInnerTilingData_.conv2dApiTiling.get_strideW();
    apiTiling["dilationH"]              = conv2DInnerTilingData_.conv2dApiTiling.get_dilationH();
    apiTiling["dilationW"]              = conv2DInnerTilingData_.conv2dApiTiling.get_dilationW();
    apiTiling["padTop"]                 = conv2DInnerTilingData_.conv2dApiTiling.get_padTop();
    apiTiling["padBottom"]              = conv2DInnerTilingData_.conv2dApiTiling.get_padBottom();
    apiTiling["padLeft"]                = conv2DInnerTilingData_.conv2dApiTiling.get_padLeft();
    apiTiling["padRight"]               = conv2DInnerTilingData_.conv2dApiTiling.get_padRight();
    apiTiling["groups"]                 = conv2DInnerTilingData_.conv2dApiTiling.get_groups();
    apiTiling["hoL1"]                   = conv2DInnerTilingData_.conv2dApiTiling.get_hoL1();
    apiTiling["woL1"]                   = conv2DInnerTilingData_.conv2dApiTiling.get_woL1();
    apiTiling["kAL1"]                   = conv2DInnerTilingData_.conv2dApiTiling.get_kAL1();
    apiTiling["kBL1"]                   = conv2DInnerTilingData_.conv2dApiTiling.get_kBL1();
    apiTiling["nBL1"]                   = conv2DInnerTilingData_.conv2dApiTiling.get_nBL1();
    apiTiling["hoL0"]                   = conv2DInnerTilingData_.conv2dApiTiling.get_hoL0();
    apiTiling["woL0"]                   = conv2DInnerTilingData_.conv2dApiTiling.get_woL0();
    apiTiling["kL0"]                    = conv2DInnerTilingData_.conv2dApiTiling.get_kL0();
    apiTiling["nL0"]                    = conv2DInnerTilingData_.conv2dApiTiling.get_nL0();
    apiTiling["khL0"]                   = conv2DInnerTilingData_.conv2dApiTiling.get_khL0();
    apiTiling["kwL0"]                   = conv2DInnerTilingData_.conv2dApiTiling.get_kwL0();
    apiTiling["pBufferFlag"]            = conv2DInnerTilingData_.conv2dApiTiling.get_pBufferFlag();
    apiTiling["cinAInCore"]             = conv2DInnerTilingData_.conv2dApiTiling.get_cinAInCore();
    apiTiling["cinATailInCore"]         = conv2DInnerTilingData_.conv2dApiTiling.get_cinATailInCore();
    apiTiling["cinBInCore"]             = conv2DInnerTilingData_.conv2dApiTiling.get_cinBInCore();
    apiTiling["cinBTailInCore"]         = conv2DInnerTilingData_.conv2dApiTiling.get_cinBTailInCore();
    apiTiling["a1SpaceSize"]            = conv2DInnerTilingData_.conv2dApiTiling.get_a1SpaceSize();
    apiTiling["b1SpaceSize"]            = conv2DInnerTilingData_.conv2dApiTiling.get_b1SpaceSize();
    apiTiling["c0SpaceSize"]            = conv2DInnerTilingData_.conv2dApiTiling.get_c0SpaceSize();
    apiTiling["multiNBL1"]              = conv2DInnerTilingData_.conv2dApiTiling.get_multiNBL1();
    apiTiling["mStep"]                  = conv2DInnerTilingData_.conv2dApiTiling.get_mStep();
    apiTiling["kStep"]                  = conv2DInnerTilingData_.conv2dApiTiling.get_kStep();
    apiTiling["nStep"]                  = conv2DInnerTilingData_.conv2dApiTiling.get_nStep();
    apiTiling["biasFullLoadFlag"]       = conv2DInnerTilingData_.conv2dApiTiling.get_biasFullLoadFlag();
    apiTiling["fixpParamsFullLoadFlag"] = conv2DInnerTilingData_.conv2dApiTiling.get_fixpParamsFullLoadFlag();
    apiTiling["iterateMNOrder"]         = conv2DInnerTilingData_.conv2dApiTiling.get_iterateMNOrder();
    apiTiling["isC04Flag"]              = conv2DInnerTilingData_.conv2dApiTiling.get_isC04Flag();
    apiTiling["hasBias"]                = conv2DInnerTilingData_.conv2dApiTiling.get_hasBias();
    apiTiling["hasPreQuant"]            = conv2DInnerTilingData_.conv2dApiTiling.get_hasPreQuant();
    apiTiling["hasPreRelu"]             = conv2DInnerTilingData_.conv2dApiTiling.get_hasPreRelu();
    apiTiling["hasPreClipRelu"]         = conv2DInnerTilingData_.conv2dApiTiling.get_hasPreClipRelu();
    apiTiling["hasAntiQuant"]           = conv2DInnerTilingData_.conv2dApiTiling.get_hasAntiQuant();
    apiTiling["hasEltwise"]             = conv2DInnerTilingData_.conv2dApiTiling.get_hasEltwise();
    apiTiling["hasPostQuant"]           = conv2DInnerTilingData_.conv2dApiTiling.get_hasPostQuant();
    apiTiling["hasPostRelu"]            = conv2DInnerTilingData_.conv2dApiTiling.get_hasPostRelu();
    apiTiling["hasPostClipRelu"]        = conv2DInnerTilingData_.conv2dApiTiling.get_hasPostClipRelu();
    apiTiling["hasPadding"]             = conv2DInnerTilingData_.conv2dApiTiling.get_hasPadding();
    apiTiling["preQuantMode"]           = conv2DInnerTilingData_.conv2dApiTiling.get_preQuantMode();
    apiTiling["preReluMode"]            = conv2DInnerTilingData_.conv2dApiTiling.get_preReluMode();
    apiTiling["preClipReluMode"]        = conv2DInnerTilingData_.conv2dApiTiling.get_preClipReluMode();
    apiTiling["antiQuantMode"]          = conv2DInnerTilingData_.conv2dApiTiling.get_antiQuantMode();
    apiTiling["eltwiseOp"]              = conv2DInnerTilingData_.conv2dApiTiling.get_eltwiseOp();
    apiTiling["eltwiseBrcEn"]           = conv2DInnerTilingData_.conv2dApiTiling.get_eltwiseBrcEn();
    apiTiling["postQuantMode"]          = conv2DInnerTilingData_.conv2dApiTiling.get_postQuantMode();
    apiTiling["postReluMode"]           = conv2DInnerTilingData_.conv2dApiTiling.get_postReluMode();
    apiTiling["postClipReluMode"]       = conv2DInnerTilingData_.conv2dApiTiling.get_postClipReluMode();
    apiTiling["paddingMode"]            = conv2DInnerTilingData_.conv2dApiTiling.get_paddingMode();
    apiTiling["dualOutput"]             = conv2DInnerTilingData_.conv2dApiTiling.get_dualOutput();
}

void Conv2DInnerTiling::DumpTilingInfoOp(nlohmann::json &opTiling)
{
    opTiling["hi"]                       = conv2DInnerTilingData_.conv2dRunInfo.get_hi();
    opTiling["wi"]                       = conv2DInnerTilingData_.conv2dRunInfo.get_wi();
    opTiling["ho"]                       = conv2DInnerTilingData_.conv2dRunInfo.get_ho();
    opTiling["wo"]                       = conv2DInnerTilingData_.conv2dRunInfo.get_wo();
    opTiling["batch"]                    = conv2DInnerTilingData_.conv2dRunInfo.get_batch();
    opTiling["ci"]                       = conv2DInnerTilingData_.conv2dRunInfo.get_ci();
    opTiling["co"]                       = conv2DInnerTilingData_.conv2dRunInfo.get_co();
    opTiling["kernelH"]                  = conv2DInnerTilingData_.conv2dRunInfo.get_kernelH();
    opTiling["kernelW"]                  = conv2DInnerTilingData_.conv2dRunInfo.get_kernelW();
    opTiling["batchDim"]                 = conv2DInnerTilingData_.conv2dRunInfo.get_batchDim();
    opTiling["coDim"]                    = conv2DInnerTilingData_.conv2dRunInfo.get_coDim();
    opTiling["hoDim"]                    = conv2DInnerTilingData_.conv2dRunInfo.get_hoDim();
    opTiling["strideH"]                  = conv2DInnerTilingData_.conv2dRunInfo.get_strideH();
    opTiling["padTop"]                   = conv2DInnerTilingData_.conv2dRunInfo.get_padTop();
    opTiling["groupDim"]                 = conv2DInnerTilingData_.conv2dRunInfo.get_groupDim();
    opTiling["groups"]                   = conv2DInnerTilingData_.conv2dRunInfo.get_groups();
}

// 8、dump Tiling数据
void Conv2DInnerTiling::DumpTilingInfo()
{
    nlohmann::json tiling;
    nlohmann::json conv2dApiTiling;
    nlohmann::json conv2dRunInfo;
    DumpTilingInfoApi(conv2dApiTiling);
    DumpTilingInfoOp(conv2dRunInfo);
    tiling["conv2dApiTiling"] = conv2dApiTiling;
    tiling["conv2dRunInfo"] = conv2dRunInfo;

    std::string path = "./debug/conv_2d_inner_tilingdata.json";
    std::string json_str = tiling.dump(4);
    // 将json字符串写入文件
    std::ofstream file(path);
    if (!file.is_open()) {
        ASC_LOGE("Unable to open file %s", path.c_str());
        return;
    }
    file << json_str;
    file.close();
    ASC_LOGD("JSON saved to %s", path.c_str());
}
} // optiling
