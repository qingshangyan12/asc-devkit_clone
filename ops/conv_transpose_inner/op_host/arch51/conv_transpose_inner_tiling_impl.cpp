/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2026-2026. All rights reserved.
 * Description:
 */

#include "conv_transpose_inner_tiling_impl.h"

namespace optiling {
bool ConvTransposeInnerTiling::IsCapable()
{
    // 待补充实现
    return true;
}

// 1、获取平台信息比如CoreNum、UB/L1/L0C资源大小
uint32_t ConvTransposeInnerTiling::GetPlatformInfo()
{
    // 待补充实现
    return npucl::SUCCESS;
}

// 2、获取INPUT/OUTPUT/ATTR信息
uint32_t ConvTransposeInnerTiling::GetShapeAttrsInfo()
{
    // 待补充实现
    return npucl::SUCCESS;
}

// 3、计算数据切分TilingData
uint32_t ConvTransposeInnerTiling::DoOpTiling()
{
    // 待补充实现
    return npucl::SUCCESS;
}

// 4、计算高阶API的TilingData
uint32_t ConvTransposeInnerTiling::DoLibApiTiling()
{
    // 待补充实现
    return npucl::SUCCESS;
}

// 5、计算TilingKey
ge::graphStatus ConvTransposeInnerTiling::GetTplTilingKey()
{
    // 待补充实现
    tilingKey_ = 0;
    return npucl::SUCCESS;
}

uint64_t ConvTransposeInnerTiling::GetTilingKey() const
{
    // 待补充实现
    return tilingKey_;
}

// 6、计算Workspace 大小
uint32_t ConvTransposeInnerTiling::GetWorkspaceSize()
{
    // 暂无workspace需求
    workspaceSize_ = 0;
    return npucl::SUCCESS;
}

// 7、保存Tiling数据
uint32_t ConvTransposeInnerTiling::PostTiling()
{
    size_t *currentWorkspace = context_->GetWorkspaceSizes(0);
    if (currentWorkspace != nullptr) {
        currentWorkspace[0] = workspaceSize_;
    }
    convTransposeInnerTilingData_.SaveToBuffer(context_->GetRawTilingData()->GetData(),
                                               context_->GetRawTilingData()->GetCapacity());
    context_->GetRawTilingData()->SetDataSize(convTransposeInnerTilingData_.GetDataSize());
    context_->SetBlockDim(blockDim_);
    return npucl::SUCCESS;
}

// 8、dump Tiling数据
void ConvTransposeInnerTiling::DumpTilingInfo()
{
    // 将结构体序列化为JSON对象
    nlohmann::json tiling;
    std::string path = "./debug/conv_transpose_inner_tilingdata.json";
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
