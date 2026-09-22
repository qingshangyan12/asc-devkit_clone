# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2026 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ----------------------------------------------------------------------------------------------------------
# 分层重构（unified）：L2 配方 / L3 语义动作 / L4 引擎后端 / L5 参数排布 / 公共组件
#   inc/  ActionSpec·DataLayout·ResourceClaim·后端注册表·动词发射框架（引擎无关值对象）
#   l2/   algConfig 解析 + L1 适配（OpParam → PrimitiveRequest）+ 配方 Expand（sole/
#         parallel/sequence/split → Plan.segments）+ Compile 接线入口
#   l3/   语义动作（CollectAll…）
#   l4/   引擎后端（AICPU 命令式 / CCU 声明式）
#   l5/   消息槽参数排布声明宏（ACTION_ARGS）+ HcommVerbSink（R6：动词→真原语，
#         引擎头消费面，仅存量侧/消费方 include）
#   exec/ R6 · WP-A2 AICPU unified 执行器（unified_aicpu_exec.cc：blob 校验 +
#         exec 期现值实例化 + L4 Emit 驱动——挂点 kernel_launch.cc 分叉）。
#         ★unified_exec/（消费方本体 aicpu_consumer.h）与其并存：本体是纯头
#         （host 可测），exec/ 是真 alg_param 编译面的薄壳（param/resCtx 适配 +
#         挂点签名），两层经 ConsumeUnifiedPlanAicpu 单点衔接。
#
# 值对象层（inc/l2/l3/l4/l5 头）经 include 路径暴露；R6 起新增唯一 .cc：
# exec/unified_aicpu_exec.cc（编译期开关无关编译进 mc2_client——分叉判定在
# 运行期 algName 键，关闭时零执行，与 §10.2 A5 灰度语义一致）。
# 落地为正式件后按方案 §11 平移到 src/unified/mc2/。
# ----------------------------------------------------------------------------------------------------------

target_include_directories(mc2_client PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/inc
    ${CMAKE_CURRENT_SOURCE_DIR}/l2
    ${CMAKE_CURRENT_SOURCE_DIR}/l3
    ${CMAKE_CURRENT_SOURCE_DIR}/l4
    ${CMAKE_CURRENT_SOURCE_DIR}/l5
    ${CMAKE_CURRENT_SOURCE_DIR}/../unified_exec
)

# R6 · WP-A2：AICPU unified 执行器（exec 期消费方薄壳，含本体 include 路径）
target_sources(mc2_client PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/exec/unified_aicpu_exec.cc
)

# 灰度开关默认值（R6.5 后用户拍板：编译期默认开，灰度收敛到运行期环境变量）：
#   MC2_UNIFIED_RESOURCE 宏默认定义——unified 代码常驻产物，运行期由
#   MC2_USE_UNIFIED_RES 环境变量控制（未设置=关，老路径行为不变）。
#   安全性依据：B0 拆闸后 unified 失败仍灰度回退老路径（分叉任一步失败
#   WARNING + 落回），编译期关的"零风险合入"由运行期关等价承接。
#   显式关闭逃生门：cmake -DMC2_UNIFIED_RES=OFF（宏不定义，回到 R5 前形态）。
option(MC2_UNIFIED_RES "Compile unified path into mc2_client (runtime gate: MC2_USE_UNIFIED_RES)" ON)
if(MC2_UNIFIED_RES)
    target_compile_definitions(mc2_client PRIVATE MC2_UNIFIED_RESOURCE)
endif()
