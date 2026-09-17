
/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2026-2026. All rights reserved.
 * Description: Conv2DInner tiling func
 */

#include "op_info_constants.h"
#include "register/op_impl_registry.h"
#include "tiling/tiling_templates_registry.h"

namespace optiling {
ge::graphStatus Tiling4Conv2DInner(gert::TilingContext* context)
{
    return TilingRegistry::GetInstance().DoTilingImpl(context);
}

ge::graphStatus GenSimplifiedKeyFunc4Conv2DInner(gert::TilingContext* context, ge::char_t* simplifiedKey)
{
    if (context == nullptr || simplifiedKey == nullptr || context->GetInputDesc(0) == nullptr ||
        context->GetOutputDesc(0) == nullptr) {
        return ge::GRAPH_FAILED;
    }

    const char* key = "default";
    return strcpy_s(simplifiedKey, ascendc::SIMPLIFIED_KEY_MAX_LEN, key) == EOK
        ? ge::GRAPH_SUCCESS : ge::GRAPH_FAILED;
}

IMPL_OP(Conv2DInner)
    .Tiling(Tiling4Conv2DInner)
    .GenSimplifiedKey(GenSimplifiedKeyFunc4Conv2DInner);
}
