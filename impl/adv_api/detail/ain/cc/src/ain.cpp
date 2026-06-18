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
 * \file ain.cpp
 * \brief Ain host-side control-plane implementation
 */
#include "adv_api/ain/ain_host.h"

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#include "hccl/hccl_rank_graph.h"

static CommSymWindow EncodeWindow(uint32_t idx)
{
    return reinterpret_cast<CommSymWindow>(static_cast<uintptr_t>(idx));
}

namespace {

uint32_t windowIdx = 1; // 当前索引0为cclBuffer需要跳过

}  // namespace

void FindNearestAllPeerLinks(HcclComm comm, std::vector<CommLink> &linkVec)
{
    // 获取layer信息
    uint32_t *netLayers;
    uint32_t layerNum;
    HcclRankGraphGetLayers(comm, &netLayers, &layerNum);
    std::sort(netLayers, netLayers + layerNum);

    // 获取本卡ID及总Rank数量
    uint32_t rankSize = 0;
    HcclGetRankSize(comm, &rankSize);
    uint32_t myRankId;
    HcclGetRankId(comm, &myRankId);

    for (uint32_t peerRankId = 0; peerRankId < rankSize; peerRankId++)
    {
        if (myRankId == peerRankId) {
            continue;
        }
        for (uint32_t layerIdx = 0; layerIdx < layerNum; layerIdx++)
        {
            uint32_t netLayer = netLayers[layerIdx];
            uint32_t* ranks;
            uint32_t rankNum;
            // 获取当前layer中所包含的rank数
            HcclRankGraphGetRanksByLayer(comm, netLayer, &ranks, &rankNum);
            uint32_t* pos = std::find(ranks, ranks + rankNum, peerRankId);
            if (pos == ranks + rankNum) {
                continue;
            }
            CommLink* links;
            uint32_t linkNum;
            HcclRankGraphGetLinks(comm, netLayer, myRankId, peerRankId, &links, &linkNum);
            if (linkNum < 1) {
                continue;
            }
            linkVec[peerRankId] = links[0];
            break; // 默认取最小layer中的第一个链路
        }
    }
}

HcclResult HcclCommSymWinRegisterExp(HcclComm comm, void *addr, uint64_t size, CommSymWindow *winHandle, uint32_t flag, HcclMemHandle *memHandle)
{
    (void)flag;
    if (addr == nullptr) {
        return HCCL_E_PTR;
    }
    std::string memTag = "AinBuf" + std::to_string(reinterpret_cast<uint64_t>(addr));
    CommMem regMem{COMM_MEM_TYPE_DEVICE, addr, size};
    auto ret = HcclCommMemReg(comm, memTag.c_str(), &regMem, memHandle);
    if (ret != HCCL_SUCCESS) {
        return ret;
    }
    *winHandle = EncodeWindow(windowIdx);
    windowIdx++;
    return HCCL_SUCCESS;
}

HcclResult HcclDevCommCreateExp(HcclComm comm, HcclDevComm* outDevComm, std::vector<HcclMemHandle> &memHandles)
{
    uint32_t rankSize = 0;
    HcclGetRankSize(comm, &rankSize);
    uint32_t myRankId;
    HcclGetRankId(comm, &myRankId);
    std::vector<CommLink> linkVec(rankSize);
    FindNearestAllPeerLinks(comm, linkVec);

    // 注册本卡共享内存
    void* signalBuf = nullptr;
    size_t signalBufSize = rankSize * sizeof(uint64_t);
    aclrtMalloc(&signalBuf, signalBufSize, ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMemset(signalBuf, signalBufSize, 0, signalBufSize);

    std::string memTag = "SignalBuf";
    CommMem regMem{COMM_MEM_TYPE_DEVICE, signalBuf, signalBufSize};
    HcclMemHandle signalHandle;
    auto ret = HcclCommMemReg(comm, memTag.c_str(), &regMem, &signalHandle);
    if (ret != HCCL_SUCCESS) {
        std::cout << "Signal memReg failed. " << std::endl;
    }
    memHandles.emplace_back(signalHandle);

    // 遍历CommLink, 填充HcclChannelDesc
    std::vector<HcclChannelDesc> channelDescVec;
    for (uint32_t peerIdx = 0; peerIdx < rankSize; peerIdx++) {
        HcclChannelDesc channelDesc;
        HcclChannelDescInit(&channelDesc, 1);
        if (myRankId == peerIdx) {
            continue;
        }
        channelDesc.remoteRank = peerIdx;

        CommLink link = linkVec[peerIdx];
        channelDesc.localEndpoint.protocol = link.srcEndpointDesc.protocol;
        channelDesc.localEndpoint.commAddr = link.srcEndpointDesc.commAddr;
        channelDesc.localEndpoint.loc = link.srcEndpointDesc.loc;
        channelDesc.remoteEndpoint.protocol = link.dstEndpointDesc.protocol;
        channelDesc.remoteEndpoint.commAddr = link.dstEndpointDesc.commAddr;
        channelDesc.remoteEndpoint.loc = link.dstEndpointDesc.loc;
        channelDesc.channelProtocol = link.linkAttr.linkProtocol;
        channelDesc.notifyNum = 3;
        channelDesc.memHandles = memHandles.data();
        channelDesc.memHandleNum = windowIdx;
        channelDescVec.emplace_back(channelDesc);
    }

    // 获取其他卡的channel信息
    CommEngine engine = CommEngine::COMM_ENGINE_AIV;
    uint32_t channelNum = rankSize - 1;
    std::vector<ChannelHandle> channels(channelNum);
    HcclChannelAcquire(comm, engine, channelDescVec.data(), channelNum, channels.data());

    // 在host构造device所需信息
    std::vector<uint64_t> commTable;
    void *shadowSignalBuf = nullptr;
    size_t shadowSignalSize = rankSize * sizeof(uint64_t);
    aclrtMalloc(&shadowSignalBuf, shadowSignalSize, ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMemset(shadowSignalBuf, shadowSignalSize, 0, shadowSignalSize);
    commTable.emplace_back(myRankId);
    commTable.emplace_back(rankSize);
    commTable.emplace_back(windowIdx);
    commTable.emplace_back(reinterpret_cast<uint64_t>(shadowSignalBuf));

    uint32_t channelIdx = 0;
    for (uint32_t peerIdx = 0; peerIdx < rankSize; peerIdx++) {
        if (peerIdx == myRankId) {
            // 对应自己rank位置无需channel为，故为无效值
            commTable.emplace_back(0);
            continue;
        }
        commTable.emplace_back(static_cast<uint64_t>(channels[channelIdx++]));
    }


    // 将host侧commTable拷贝至device侧
    void *devCommTable = nullptr;
    size_t tableBytes = commTable.size() * sizeof(uint64_t);
    auto aclRet = aclrtMalloc(&devCommTable, tableBytes, ACL_MEM_MALLOC_HUGE_FIRST);
    if (aclRet != ACL_SUCCESS) {
        std::cout << "DevComm table malloc failed. " << std::endl;
        return HCCL_E_PTR;
    }
    aclRet = aclrtMemcpy(devCommTable, tableBytes, commTable.data(), tableBytes, ACL_MEMCPY_HOST_TO_DEVICE);
    if (aclRet != ACL_SUCCESS) {
        std::cout << "DevComm table memcpy failed. " << std::endl;
        (void)aclrtFree(devCommTable);
        return HCCL_E_PTR;
    }

    *outDevComm = devCommTable;
    return HCCL_SUCCESS;
}
