/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#include "hccl_rank_graph_dl.h"
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>

DEFINE_DLSYM_FUNC(
    HcclResult, HcclRankGraphGetLayers, (HcclComm comm, uint32_t** netLayers, uint32_t* netLayerNum),
    (comm, netLayers, netLayerNum));
DEFINE_DLSYM_FUNC(
    HcclResult, HcclRankGraphGetRanksByLayer,
    (HcclComm comm, uint32_t netLayer, uint32_t** ranks, uint32_t* rankNum), (comm, netLayer, ranks, rankNum));
DEFINE_DLSYM_FUNC(
    HcclResult, HcclRankGraphGetRankSizeByLayer, (HcclComm comm, uint32_t netLayer, uint32_t* rankNum),
    (comm, netLayer, rankNum));
DEFINE_DLSYM_FUNC(
    HcclResult, HcclRankGraphGetTopoTypeByLayer, (HcclComm comm, uint32_t netLayer, CommTopo* topoType),
    (comm, netLayer, topoType));
DEFINE_DLSYM_FUNC(
    HcclResult, HcclRankGraphGetInstSizeListByLayer,
    (HcclComm comm, uint32_t netLayer, uint32_t** instSizeList, uint32_t* listSize),
    (comm, netLayer, instSizeList, listSize));
DEFINE_DLSYM_FUNC(
    HcclResult, HcclRankGraphGetLinks,
    (HcclComm comm, uint32_t netLayer, uint32_t srcRank, uint32_t dstRank, CommLink** links, uint32_t* linkNum),
    (comm, netLayer, srcRank, dstRank, links, linkNum));
DEFINE_DLSYM_FUNC(
    HcclResult, HcclRankGraphGetTopoInstsByLayer,
    (HcclComm comm, uint32_t netLayer, uint32_t** topoInsts, uint32_t* topoInstNum),
    (comm, netLayer, topoInsts, topoInstNum));
DEFINE_DLSYM_FUNC(
    HcclResult, HcclRankGraphGetTopoType,
    (HcclComm comm, uint32_t netLayer, uint32_t topoInstId, CommTopo* topoType),
    (comm, netLayer, topoInstId, topoType));
DEFINE_DLSYM_FUNC(
    HcclResult, HcclRankGraphGetRanksByTopoInst,
    (HcclComm comm, uint32_t netLayer, uint32_t topoInstId, uint32_t** ranks, uint32_t* rankNum),
    (comm, netLayer, topoInstId, ranks, rankNum));
DEFINE_DLSYM_FUNC(
    HcclResult, HcclRankGraphGetEndpointNum, (HcclComm comm, uint32_t layer, uint32_t topoInstId, uint32_t* num),
    (comm, layer, topoInstId, num));
DEFINE_DLSYM_FUNC(
    HcclResult, HcclRankGraphGetEndpointDesc,
    (HcclComm comm, uint32_t layer, uint32_t topoInstId, uint32_t* descNum, EndpointDesc* endpointDesc),
    (comm, layer, topoInstId, descNum, endpointDesc));
DEFINE_DLSYM_FUNC(
    HcclResult, HcclRankGraphGetEndpointInfo,
    (HcclComm comm, uint32_t rankId, const EndpointDesc* endpointDesc, EndpointAttr endpointAttr, uint32_t infoLen,
        void* info),
    (comm, rankId, endpointDesc, endpointAttr, infoLen, info));

void HcclRankGraphDlInit(void* libHcommHandle)
{
    INIT_SUPPORT_FLAG(libHcommHandle, HcclRankGraphGetLayers);
    INIT_SUPPORT_FLAG(libHcommHandle, HcclRankGraphGetRanksByLayer);
    INIT_SUPPORT_FLAG(libHcommHandle, HcclRankGraphGetRankSizeByLayer);
    INIT_SUPPORT_FLAG(libHcommHandle, HcclRankGraphGetTopoTypeByLayer);
    INIT_SUPPORT_FLAG(libHcommHandle, HcclRankGraphGetInstSizeListByLayer);
    INIT_SUPPORT_FLAG(libHcommHandle, HcclRankGraphGetLinks);
    INIT_SUPPORT_FLAG(libHcommHandle, HcclRankGraphGetTopoInstsByLayer);
    INIT_SUPPORT_FLAG(libHcommHandle, HcclRankGraphGetTopoType);
    INIT_SUPPORT_FLAG(libHcommHandle, HcclRankGraphGetRanksByTopoInst);
    INIT_SUPPORT_FLAG(libHcommHandle, HcclRankGraphGetEndpointNum);
    INIT_SUPPORT_FLAG(libHcommHandle, HcclRankGraphGetEndpointDesc);
    INIT_SUPPORT_FLAG(libHcommHandle, HcclRankGraphGetEndpointInfo);
}
