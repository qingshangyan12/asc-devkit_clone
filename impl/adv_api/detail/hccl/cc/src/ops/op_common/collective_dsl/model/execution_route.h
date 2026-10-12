// Copyright (c) 2026 Huawei Technologies Co., Ltd.
// Licensed under the CANN Open Software License Agreement Version 2.0.
#ifndef MC2_COLLECTIVE_DSL_EXECUTION_ROUTE_H
#define MC2_COLLECTIVE_DSL_EXECUTION_ROUTE_H
#include "collective_plan.h"
#include <string>
namespace mc2_ops_hccl {
namespace dsl {
enum class Initiator { HOST, AIC, AIV };
enum class Dispatcher { DIRECT, KFC_AICPU_SERVER, KFC_CCU_SERVER };
enum class Executor { AICPU, AIV, AIC, CCU };
struct ExecutionRoute {
    Initiator initiator;
    Dispatcher dispatcher;
    Executor executor;
};
struct EngineResources {
    uint32_t clientRequestSlots = 0; // HcclMsgArea 中 client 发布请求需要的槽位
    uint32_t serverQueues = 0;       // KFC server 消费请求的队列
    uint32_t workerThreads = 0;      // AICPU HComm thread
    uint32_t notifies = 0;           // AICPU 主从 thread 同步
    uint32_t channelGroups = 0;      // 由真实 channel selector 填充，非 DSL 猜测
    uint32_t ccuMissions = 0;        // CCU kernel/mission 数
    uint32_t aivKernels = 0;         // AIV/AIC 直接执行的通信 kernel 数
};
bool IsSupported(const ExecutionRoute& route, Primitive primitive);
EngineResources AnalyzeRoute(const Plan& plan, const ExecutionRoute& route);
const char* RouteName(const ExecutionRoute& route);
} // namespace dsl
} // namespace mc2_ops_hccl
#endif
