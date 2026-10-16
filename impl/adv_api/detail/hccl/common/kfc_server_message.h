/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef HCCL_KFC_SERVER_MESSAGE_H
#define HCCL_KFC_SERVER_MESSAGE_H

#include <cstdint>

namespace HcclKfcProtocol {

constexpr uint32_t KFC_MAX_MISSION_NUM = 2U;
constexpr uint32_t KFC_SIGNAL_REGIONS_PER_MISSION = 2U;
constexpr uint32_t KFC_SIGNAL_REGION_NUM = KFC_MAX_MISSION_NUM * KFC_SIGNAL_REGIONS_PER_MISSION;
constexpr uint32_t KFC_COMMIT_REGION_INDEX = 0U;
constexpr uint32_t KFC_WAIT_REGION_INDEX = 1U;

enum KfcConcurrentAllGatherParamIndex : uint32_t {
    KFC_CONCURRENT_AG_OP_ID = 0,
    KFC_CONCURRENT_AG_MESH_INPUT = 1,
    KFC_CONCURRENT_AG_MESH_OUTPUT = 2,
    KFC_CONCURRENT_AG_MESH_OUTPUT_OFFSET = 3,
    KFC_CONCURRENT_AG_MESH_SLICE_SIZE = 4,
    KFC_CONCURRENT_AG_MESH_GO_SIZE_0 = 5,
    KFC_CONCURRENT_AG_MESH_GO_SIZE_1 = 6,
    KFC_CONCURRENT_AG_MESH_GO_SIZE_2 = 7,
    KFC_CONCURRENT_AG_MESH_GO_SIZE_3 = 8,
    KFC_CONCURRENT_AG_MESH_CURRENT_RANK_SLICE_INPUT_OFFSET = 9,
    KFC_CONCURRENT_AG_MESH_REPEAT_NUM_INV = 10,
    KFC_CONCURRENT_AG_MESH_INPUT_REPEAT_STRIDE = 11,
    KFC_CONCURRENT_AG_MESH_OUTPUT_REPEAT_STRIDE = 12,
    KFC_CONCURRENT_AG_MESH_LAST_SLICE_SIZE = 13,
    KFC_CONCURRENT_AG_MESH_INPUT_OUTPUT_EQUAL = 14,
    KFC_CONCURRENT_AG_MESH_PARAM_NUM = 15,
    KFC_CONCURRENT_AG_NHR_INPUT = KFC_CONCURRENT_AG_MESH_PARAM_NUM,
    KFC_CONCURRENT_AG_NHR_OUTPUT = 16,
    KFC_CONCURRENT_AG_NHR_SLICE_SIZE = 17,
    KFC_CONCURRENT_AG_NHR_SLICE_SIZE_PER_JETTY = 18,
    KFC_CONCURRENT_AG_NHR_LAST_SLICE_SIZE_PER_JETTY = 19,
    KFC_CONCURRENT_AG_NHR_REPEAT_NUM_INV = 20,
    KFC_CONCURRENT_AG_NHR_INPUT_SLICE_STRIDE = 21,
    KFC_CONCURRENT_AG_NHR_OUTPUT_SLICE_STRIDE = 22,
    KFC_CONCURRENT_AG_NHR_INPUT_REPEAT_STRIDE = 23,
    KFC_CONCURRENT_AG_NHR_OUTPUT_REPEAT_STRIDE = 24,
    KFC_CONCURRENT_AG_NHR_INPUT_OUTPUT_EQUAL = 25,
    KFC_CONCURRENT_AG_NHR_GO_SIZE_0 = 26,
    KFC_CONCURRENT_AG_NHR_GO_SIZE_1 = 27,
    KFC_CONCURRENT_AG_NHR_GO_SIZE_2 = 28,
    KFC_CONCURRENT_AG_NHR_GO_SIZE_3 = 29,
    KFC_CONCURRENT_AG_PARAM_NUM = 30,
};

// Parallel Mesh+NHR AllGather uses the same two persistent KFC missions as the
// concurrent variant, but each mission executes twice.  The payload therefore
// carries invariant addresses/sizes; the server reconstructs the four
// phase-specific TemplateDataParams from these values and its static topology.
enum KfcParallelAllGatherParamIndex : uint32_t {
    KFC_PARALLEL_AG_OP_ID = 0,
    KFC_PARALLEL_AG_INPUT = 1,
    KFC_PARALLEL_AG_OUTPUT = 2,
    KFC_PARALLEL_AG_TOTAL_SIZE = 3,
    KFC_PARALLEL_AG_OUTPUT_STRIDE = 4,
    KFC_PARALLEL_AG_PART0_SIZE = 5,
    KFC_PARALLEL_AG_PART1_SIZE = 6,
    KFC_PARALLEL_AG_PART1_OFFSET = 7,
    KFC_PARALLEL_AG_MESH_PHASE_DONE_ADDR = 8,
    KFC_PARALLEL_AG_NHR_PHASE_DONE_ADDR = 9,
    KFC_PARALLEL_AG_PART0_GO_SIZE_0 = 10,
    KFC_PARALLEL_AG_PART0_GO_SIZE_1 = 11,
    KFC_PARALLEL_AG_PART0_GO_SIZE_2 = 12,
    KFC_PARALLEL_AG_PART0_GO_SIZE_3 = 13,
    KFC_PARALLEL_AG_PART1_GO_SIZE_0 = 14,
    KFC_PARALLEL_AG_PART1_GO_SIZE_1 = 15,
    KFC_PARALLEL_AG_PART1_GO_SIZE_2 = 16,
    KFC_PARALLEL_AG_PART1_GO_SIZE_3 = 17,
    KFC_PARALLEL_AG_PART0_SLICE_PER_JETTY = 18,
    KFC_PARALLEL_AG_PART0_LAST_SLICE_PER_JETTY = 19,
    KFC_PARALLEL_AG_PART1_SLICE_PER_JETTY = 20,
    KFC_PARALLEL_AG_PART1_LAST_SLICE_PER_JETTY = 21,
    KFC_PARALLEL_AG_PARAM_NUM = 22,
    // These two words live in the same 32-XN queue record. They are GM
    // rendezvous flags shared by the Mesh and NHR CCU missions.
    KFC_PARALLEL_AG_MESH_PHASE_DONE_STORAGE = 30,
    KFC_PARALLEL_AG_NHR_PHASE_DONE_STORAGE = 31,
    KFC_PARALLEL_AG_STORAGE_NUM = 32,
};

enum KfcConcurrentAllToAllParamIndex : uint32_t {
    KFC_CONCURRENT_A2A_OP_ID = 0,
    KFC_CONCURRENT_A2A_MESH_INPUT = 1,
    KFC_CONCURRENT_A2A_MESH_OUTPUT = 2,
    KFC_CONCURRENT_A2A_MESH_SLICE_SIZE = 3,
    KFC_CONCURRENT_A2A_MESH_SRC_STRIDE = 4,
    KFC_CONCURRENT_A2A_MESH_SRC_OFFSET = 5,
    KFC_CONCURRENT_A2A_MESH_DST_OFFSET = 6,
    KFC_CONCURRENT_A2A_MESH_GO_SIZE_0 = 7,
    KFC_CONCURRENT_A2A_MESH_GO_SIZE_1 = 8,
    KFC_CONCURRENT_A2A_MESH_GO_SIZE_2 = 9,
    KFC_CONCURRENT_A2A_MESH_GO_SIZE_3 = 10,
    KFC_CONCURRENT_A2A_MESH_SLICE_SIZE_PER_JETTY = 11,
    KFC_CONCURRENT_A2A_MESH_LAST_SLICE_SIZE_PER_JETTY = 12,
    KFC_CONCURRENT_A2A_MESH_PARAM_NUM = 13,
    KFC_CONCURRENT_A2A_CLOS_INPUT = KFC_CONCURRENT_A2A_MESH_PARAM_NUM,
    KFC_CONCURRENT_A2A_CLOS_OUTPUT = 14,
    KFC_CONCURRENT_A2A_CLOS_SLICE_SIZE = 15,
    KFC_CONCURRENT_A2A_CLOS_SRC_STRIDE = 16,
    KFC_CONCURRENT_A2A_CLOS_SRC_OFFSET = 17,
    KFC_CONCURRENT_A2A_CLOS_DST_OFFSET = 18,
    KFC_CONCURRENT_A2A_CLOS_GO_SIZE_0 = 19,
    KFC_CONCURRENT_A2A_CLOS_GO_SIZE_1 = 20,
    KFC_CONCURRENT_A2A_CLOS_GO_SIZE_2 = 21,
    KFC_CONCURRENT_A2A_CLOS_GO_SIZE_3 = 22,
    KFC_CONCURRENT_A2A_CLOS_SLICE_SIZE_PER_JETTY = 23,
    KFC_CONCURRENT_A2A_CLOS_LAST_SLICE_SIZE_PER_JETTY = 24,
    KFC_CONCURRENT_A2A_PARAM_NUM = 25,
};

enum KfcAllToAllMultiJettyParamIndex : uint32_t {
    KFC_A2A_MJ_OP_ID = 0,
    KFC_A2A_MJ_INPUT = 1,
    KFC_A2A_MJ_OUTPUT = 2,
    KFC_A2A_MJ_SLICE_SIZE = 3,
    KFC_A2A_MJ_SRC_STRIDE = 4,
    KFC_A2A_MJ_SRC_OFFSET = 5,
    KFC_A2A_MJ_DST_OFFSET = 6,
    KFC_A2A_MJ_GO_SIZE_0 = 7,
    KFC_A2A_MJ_GO_SIZE_1 = 8,
    KFC_A2A_MJ_GO_SIZE_2 = 9,
    KFC_A2A_MJ_GO_SIZE_3 = 10,
    KFC_A2A_MJ_SLICE_SIZE_PER_JETTY = 11,
    KFC_A2A_MJ_LAST_SLICE_SIZE_PER_JETTY = 12,
    KFC_A2A_MJ_PARAM_NUM = 13,
};

// Sole NHR MultiLink（CcuSchedReduceScatterSoleNHRMultiLink）单帧 xnData 布局：
// [0]=opId 哨兵，NHR 参数从 1 顺排。AIV prepare（CcuPrepareForReduceScatterSoleNhrM2M）写入、
// KFC dispatch 逐槽转发、kernel 形参三处索引必须一致。
enum KfcReduceScatterSoleNhrParamIndex : uint32_t {
    KFC_RS_SOLE_NHR_OP_ID = 0,
    KFC_RS_SOLE_NHR_INPUT = 1,
    KFC_RS_SOLE_NHR_OUTPUT = 2,
    KFC_RS_SOLE_NHR_SLICE_SIZE = 3,
    KFC_RS_SOLE_NHR_INPUT_SLICE_STRIDE = 4,
    KFC_RS_SOLE_NHR_SLICE_ONE_JETTY_SIZE = 5,
    KFC_RS_SOLE_NHR_SLICE_LAST_JETTY_SIZE = 6,
    KFC_RS_SOLE_NHR_REPEAT_NUM_INV = 7,
    KFC_RS_SOLE_NHR_INPUT_REPEAT_STRIDE = 8,
    KFC_RS_SOLE_NHR_OUTPUT_REPEAT_STRIDE = 9,
    KFC_RS_SOLE_NHR_PARAM_NUM = 10,
};

// Sole NHR 2Die（CcuSchedReduceScatterSoleNHR，两级拓扑 L1 NHR 中继）单帧 xnData 布局：
// [0]=opId 哨兵，参数从 1 顺排（token 走 dispatch 的 ctx.token，不占槽；其余 20 参数与 hccl 模板
// taskArgs 布局一致）。双 die 时两个 mission 共享同一区间（参数相同，kernelArg.axisId 区分实例）。
// AIV prepare（CcuPrepareForReduceScatterSoleNhr2DieM2M）写入、KFC dispatch 逐槽转发、
// kernel 形参三处索引必须一致。
enum KfcReduceScatterSoleNhr2DieParamIndex : uint32_t {
    KFC_RS_SOLE_NHR_2DIE_OP_ID = 0,
    KFC_RS_SOLE_NHR_2DIE_INPUT = 1,
    KFC_RS_SOLE_NHR_2DIE_OUTPUT = 2,
    KFC_RS_SOLE_NHR_2DIE_DIE0_SIZE = 3,
    KFC_RS_SOLE_NHR_2DIE_DIE1_SIZE = 4,
    KFC_RS_SOLE_NHR_2DIE_DIE0_LAST_SLICE_SIZE = 5,
    KFC_RS_SOLE_NHR_2DIE_DIE1_LAST_SLICE_SIZE = 6,
    KFC_RS_SOLE_NHR_2DIE_INPUT_SLICE_STRIDE = 7,
    KFC_RS_SOLE_NHR_2DIE_CURRENT_RANK_SLICE_OUTPUT_OFFSET = 8,
    KFC_RS_SOLE_NHR_2DIE_INPUT_REPEAT_STRIDE = 9,
    KFC_RS_SOLE_NHR_2DIE_OUTPUT_REPEAT_STRIDE = 10,
    KFC_RS_SOLE_NHR_2DIE_REPEAT_NUM_VAR = 11,
    KFC_RS_SOLE_NHR_2DIE_IS_INPUT_OUTPUT_EQUAL = 12,
    KFC_RS_SOLE_NHR_2DIE_GO_SIZE_NORMAL_0 = 13,
    KFC_RS_SOLE_NHR_2DIE_GO_SIZE_NORMAL_1 = 14,
    KFC_RS_SOLE_NHR_2DIE_GO_SIZE_NORMAL_2 = 15,
    KFC_RS_SOLE_NHR_2DIE_GO_SIZE_NORMAL_3 = 16,
    KFC_RS_SOLE_NHR_2DIE_GO_SIZE_LAST_0 = 17,
    KFC_RS_SOLE_NHR_2DIE_GO_SIZE_LAST_1 = 18,
    KFC_RS_SOLE_NHR_2DIE_GO_SIZE_LAST_2 = 19,
    KFC_RS_SOLE_NHR_2DIE_GO_SIZE_LAST_3 = 20,
    // die1 专属 goSize 组（双 die 切分后各 die 收尾 GroupCopy 长度不同，按 axisId 选组）
    KFC_RS_SOLE_NHR_2DIE_DIE1_GO_SIZE_NORMAL_0 = 21,
    KFC_RS_SOLE_NHR_2DIE_DIE1_GO_SIZE_NORMAL_1 = 22,
    KFC_RS_SOLE_NHR_2DIE_DIE1_GO_SIZE_NORMAL_2 = 23,
    KFC_RS_SOLE_NHR_2DIE_DIE1_GO_SIZE_NORMAL_3 = 24,
    KFC_RS_SOLE_NHR_2DIE_DIE1_GO_SIZE_LAST_0 = 25,
    KFC_RS_SOLE_NHR_2DIE_DIE1_GO_SIZE_LAST_1 = 26,
    KFC_RS_SOLE_NHR_2DIE_DIE1_GO_SIZE_LAST_2 = 27,
    KFC_RS_SOLE_NHR_2DIE_DIE1_GO_SIZE_LAST_3 = 28,
    KFC_RS_SOLE_NHR_2DIE_PARAM_NUM = 29,
};

// SoleNHR（CcuSchedAllGatherSoleNHR）单帧 xnData 布局（N2 压缩至 8 槽 = 64B = 单 DCCI 行，
// AIV 提交路径 flush 2→1、写槽数 15→8）：
//  [0]=INPUT 兼作轮次有效哨兵——server 仅检查 param[0] != INVALID_U64（真实地址必不等于），
//  原 opId 槽纯哨兵用途（kernel 不接收 opId），故由 INPUT 占位。
//  常量参数不占槽、由 kernel LoadNhr1DArgs 内联（sole executor 恒定）：
//  die0LastSize=die0Size、repeatNum=1、三 stride=0。
//  isInputOutputEqual 不占槽：in-place 时 AIV 将 goSize 按 0 字节计算（CalcGoSize(0) →
//  m=n=p=0 全零），GroupCopy 内部两个 CCU_IF 块（addrOffset/parallelParam != 0）均不触发，
//  本地拷贝退化为空，kernel 无需该参数。
// AIV prepare（CcuPrepareForAllGatherSoleNhrM2M）写入、KFC dispatch 逐槽转发、
// kernel 形参三处索引必须一致。
// 单 die 语义：die0Size 即全量分片（hccl dieNum=1 口径），die1 参数省略。
enum KfcAllGatherSoleNhrParamIndex : uint32_t {
    KFC_AG_SOLE_NHR_INPUT = 0,
    KFC_AG_SOLE_NHR_OUTPUT = 1,
    KFC_AG_SOLE_NHR_DIE0_SIZE = 2,
    KFC_AG_SOLE_NHR_OUTPUT_SLICE_STRIDE = 3,
    KFC_AG_SOLE_NHR_GO_SIZE_0 = 4,
    KFC_AG_SOLE_NHR_GO_SIZE_1 = 5,
    KFC_AG_SOLE_NHR_GO_SIZE_2 = 6,
    KFC_AG_SOLE_NHR_GO_SIZE_3 = 7,
    KFC_AG_SOLE_NHR_PARAM_NUM = 8,
};

// ReduceScatter 并发(Mesh+NHR)单帧 xnData 布局：mission0 读 Mesh 区间，mission1 读 NHR 区间。
enum KfcConcurrentReduceScatterParamIndex : uint32_t {
    KFC_CONCURRENT_RS_OP_ID = 0,
    KFC_CONCURRENT_RS_MESH_INPUT = 1,
    KFC_CONCURRENT_RS_MESH_OUTPUT = 2,
    KFC_CONCURRENT_RS_MESH_SCRATCH = 3,
    KFC_CONCURRENT_RS_MESH_RANK_SLICE_OFFSET = 4,
    KFC_CONCURRENT_RS_MESH_CHUNK_SIZE = 5,
    KFC_CONCURRENT_RS_MESH_CHUNK_LOOP_NUM = 6,
    KFC_CONCURRENT_RS_MESH_TAIL_SIZE = 7,
    KFC_CONCURRENT_RS_MESH_FULL_GO_ADDR_OFFSET = 8,
    KFC_CONCURRENT_RS_MESH_FULL_GO_LOOP_PARAM = 9,
    KFC_CONCURRENT_RS_MESH_FULL_GO_PARALLEL_PARAM = 10,
    KFC_CONCURRENT_RS_MESH_FULL_GO_RESIDUAL = 11,
    KFC_CONCURRENT_RS_MESH_TAIL_GO_ADDR_OFFSET = 12,
    KFC_CONCURRENT_RS_MESH_TAIL_GO_LOOP_PARAM = 13,
    KFC_CONCURRENT_RS_MESH_TAIL_GO_PARALLEL_PARAM = 14,
    KFC_CONCURRENT_RS_MESH_TAIL_GO_RESIDUAL = 15,
    KFC_CONCURRENT_RS_MESH_PARAM_NUM = 16,
    KFC_CONCURRENT_RS_NHR_INPUT = KFC_CONCURRENT_RS_MESH_PARAM_NUM,
    KFC_CONCURRENT_RS_NHR_OUTPUT = 17,
    KFC_CONCURRENT_RS_NHR_SLICE_SIZE = 18,
    KFC_CONCURRENT_RS_NHR_INPUT_SLICE_STRIDE = 19,
    KFC_CONCURRENT_RS_NHR_SLICE_ONE_JETTY_SIZE = 20,
    KFC_CONCURRENT_RS_NHR_SLICE_LAST_JETTY_SIZE = 21,
    KFC_CONCURRENT_RS_NHR_REPEAT_NUM_INV = 22,
    KFC_CONCURRENT_RS_NHR_INPUT_REPEAT_STRIDE = 23,
    KFC_CONCURRENT_RS_NHR_OUTPUT_REPEAT_STRIDE = 24,
    KFC_CONCURRENT_RS_PARAM_NUM = 25,
};

// ============================================================================
// 消息槽协议 v3（unified Plan 轮级动作 ID 分发，§12-11 甲路线 2026-09-21）
//
// v2 的分发判据是实例级：role 由 host 按 algName 构建后随 CCU 实例创建固定，
// 一个 server 生命周期只执行一种算法。v3 把判据下沉到每轮数据：unified 序列化器
// （unified/inc/action_spec.h 的 SerializePlanV3ToSlots）把 Plan.segments 逐段编成
// 一条 32×u64 消息槽记录，server 侧 DispatchUnifiedPlanV3 按记录序列执行多段计划。
//
// 记录布局（32 槽 == server LoadFuncParamFromMemory 一次 Load 的宽度，
// ccu_kernel_kfc_server.h:24 CCU_PARAM_NUM_PER_DIE=32）：
//   槽 0      = 动作 ID（KFC_UNIFIED_ACTION_*，值 == unified ActionKind 枚举值）
//   槽 1..14  = CollectAll 首发动作参数（复用 v2 AG Mesh 已验证读取序，见下）
//   槽 1..15  = ReduceSlice 参数（R6：复用 v2 并发 RS Mesh 读取序，见下）
//   槽 16..31 = 0（未用槽，序列化器置 0）
// 出口哨兵：与 v2 同一判定（server 循环 param[0]==INVALID_U64 退出，
// ccu_kernel_kfc_server.cc:389）。哨兵记录由 device 写入方按队列协议追加，
// 不属于序列化 blob。
//
// ★ 槽 0 取值互认（v2/v3 不可混读）：v3 槽 0 = ActionKind 值（CollectAll=0 …
//   LocalReduce=4），v2 槽 0 = opId 控制字（存量 AG 写入方 FillCachedArgs 首实参
//   =1，ccu_temp_kfc_all_gather_mesh_1D_mem2mem.cc:115-117）。两种解读由 server 按
//   role 分叉隔离（UNIFIED_PLAN 走 v3 分发，其余走 v2 if-else），同一条 32 槽记录
//   不会被两种解读同时消费；出口哨兵 INVALID_U64=UINT64_MAX 不与任何动作 ID
//   （0..4）冲突，循环出口判定两协议通用。
// ============================================================================

// v3 动作 ID 常量：值必须与 unified inc/action_spec.h 的 ActionKind 枚举逐项相等
// （CollectAll=0 / ReduceSlice=1 / ExchangeAll=2 / LocalCopy=3 / LocalReduce=4）。
// unified 侧测试 poc_unified_serialize_v3_test.cc 以 static_assert 三处对齐守门：
// ActionKind 枚举 ↔ 本常量 ↔ server DispatchUnifiedPlanV3 分支 ID（server 分支
// 直接引用本常量，构造性对齐）。
constexpr uint32_t KFC_UNIFIED_ACTION_COLLECT_ALL = 0U;  // == ActionKind::CollectAll
constexpr uint32_t KFC_UNIFIED_ACTION_REDUCE_SLICE = 1U; // == ActionKind::ReduceSlice
constexpr uint32_t KFC_UNIFIED_ACTION_EXCHANGE_ALL = 2U; // == ActionKind::ExchangeAll
constexpr uint32_t KFC_UNIFIED_ACTION_LOCAL_COPY = 3U;   // == ActionKind::LocalCopy
constexpr uint32_t KFC_UNIFIED_ACTION_LOCAL_REDUCE = 4U; // == ActionKind::LocalReduce
static_assert(KFC_UNIFIED_ACTION_LOCAL_REDUCE < UINT64_MAX,
    "v3 action ids must never collide with the INVALID_U64 exit sentinel");

// unified 动作 kernel 注册名（server host 侧 v3 分叉的识别标记）：
// == unified l4/backend_ccu_collect_all.h 的 CcuKernelPlan.kernelFuncName；
// R6 追加 RS 注册名（l4/backend_ccu_reduce_slice.h）。两名字均以
// KFC_UNIFIED_ACTION_PREFIX（"CcuAction"）开头——InheritKfcServerKernelArg 按
// 前缀泛化命中，不逐名扩名单。
constexpr char KFC_UNIFIED_ACTION_COLLECT_ALL_KERNEL_NAME[] = "CcuActionCollectAll";
constexpr char KFC_UNIFIED_ACTION_REDUCE_SLICE_KERNEL_NAME[] = "CcuActionReduceSlice";
// M5（第五轮 MINOR）：host 前缀判定的共享常量化——ccu_temp_kfc_server.cc 两处
// compare(0, 9, "CcuAction") 的前缀字面量与长度共享本组常量（该文件本头已 include），
// 防字面量与子串长度再脱钩（第四轮 BLOCKER-1 病灶形态：长度误写 10 恒假）。
constexpr char KFC_UNIFIED_ACTION_PREFIX[] = "CcuAction";
constexpr uint32_t KFC_UNIFIED_ACTION_PREFIX_LEN = 9U;   // == strlen(KFC_UNIFIED_ACTION_PREFIX)

// ============================================================================
// v5 动词帧协议（R12 · CCU 动词序执行器，2026-10-09）：server 从"动作 ID → 整体
// kernel 直调"改为"动词 ID → 原语组合"（DispatchUnifiedVerbs，ccu_kernel_kfc_server.cc
// 纯新增分支）。v3/v4 链路零变化（纯追加段）。
//
// 【两级动词（主会话裁决 R12-1）】
//   叶子动词：SYNC_PEER_PRE/SYNC_PEER_POST/READ/WRITE/COPY——server 直接发射原语；
//   组原语动词：BROADCAST → GroupBroadcast、REDUCE → GroupLocalReduce(≤8P)/
//   PairwiseLocalReduce(>8P)——复用保留的基础 kernel 原语层（ccu_kernel_alg_base）。
//
// 【第一级动词 ID = VerbCall::Verb 枚举值 0..7】（unified inc/verb_sink.h:40-52）——
// 序列化器写 static_cast<u64>，server 分支引用本组常量，unified 测试镜像 static_assert
// 三方互锁。与 v3 动作 ID（0..4）的值域重叠由三层隔离消化：role 分叉（UNIFIED_VERB=10
// vs UNIFIED_PLAN=9，kfc_server_protocol.h 追加）、blob 头部（UPL5/版本 5）、注册名前缀
//（"CcuVerb" vs "CcuAction"）。
//
// 【帧布局（32 槽 == server Load 宽度；未用槽清 0；出口哨兵 = 槽 0 INVALID_U64 同 v3）】
//   槽 0     = 动词 ID（KFC_UNIFIED_VERB_*）
//   数据帧（READ/WRITE/COPY/POST_COPY）：槽 1=srcOff、槽 2=dstOff、槽 3=size、槽 4=0
//     ——★无 channelIdx/peer 槽（R12-1 裁决①隐式映射：段内数据帧按通道序隐式消费，
//       帧不携通道号，游标随数据帧推进、非数据帧不动；host Validate 校验
//       "段内数据帧数==channelCount"+"序列化器迭代序==注册通道序"（单源派生，
//       R12-4：序列化器 peer 迭代序 = claim.channels 注册序，不做二次 sort））
//   BROADCAST：槽 1=srcOff（IN 相对）、槽 2=localDstOff（OUT 相对）、槽 3=size、
//              槽 4=remoteDstOff（对端 OUT 相对）、槽 5..8=goSize 四元组
//   REDUCE：   槽 1=srcOff（SCR 相对）、槽 2=dstOff（OUT 相对）、槽 3=size、
//              槽 5..8=goSize 四元组（>8P Pairwise 由 server 按 rankSize 分叉，不占槽）
//   SYNC_PEER_PRE：槽 1=XN 掩码（AG=OUT|TOK=0xC / RS=IN|SCR|TOK=0x7）、槽 2=本端 IN 基址、
//              槽 3=本端 SCR 基址、槽 4=本端 OUT 基址（掩码含位才有效；TOK 恒取
//              ctx.token 不占槽——v2/v3 同格局）
//   SYNC_PEER_POST：槽 1=XN 掩码（恒 POST 位 0x10）
//
// 【XN 统一变量表（R12-1 裁决②/R12-2 尾款）】{IN=0, SCR=1, TOK=2, OUT=3, POST=4}——
//   对端地址不进帧：沿老 kernel PreSync 互发机制（WriteVariableWithNotify 广播本端
//   基址 + GetResByChannel 取对端值）。POST 迁 4 避让：AG 老表 POST=3 与新 OUT=3
//   撞位会互踩 CKE mask（审查方已核）。
//
// 【v5 blob 头部（镜像 v4 头布局，7 槽）】[0]=magic "UPL5"、[1]=版本 5、[2]=frameCount
//   （≠v3 段数——一段产多帧）、[3]=保留、[4..6]=dataType/outputType/reduceType（纯
//   维测 dump，R6 尾款终裁口径不变）。总槽 = 7 + frameCount*32。
// ============================================================================

// v5 第一级动词 ID 常量：值必须与 unified inc/verb_sink.h 的 VerbCall::Verb 枚举逐项相等
//（COPY=0/POST_COPY=1/READ=2/WRITE=3/REDUCE=4/BROADCAST=5/SYNC_PEER_PRE=6/
// SYNC_PEER_POST=7）。三方同名登记：unified 枚举 ↔ 本常量 ↔ server DispatchUnifiedVerbs
// 分支（分支直接引用本常量，构造性对齐；unified 测试以镜像常量 static_assert 守门）。
// R13（2026-10-09）：NHR_RUN=8 尾追加（NHR 段数据动词——ReduceScatter NHR 的
// WriteReduce 步进流，M0_R13_NHR_V5.md §八；枚举尾追加零既有消费回归）。
constexpr uint32_t KFC_UNIFIED_VERB_COPY = 0U;          // == VerbCall::COPY
constexpr uint32_t KFC_UNIFIED_VERB_POST_COPY = 1U;     // == VerbCall::POST_COPY
constexpr uint32_t KFC_UNIFIED_VERB_READ = 2U;          // == VerbCall::READ
constexpr uint32_t KFC_UNIFIED_VERB_WRITE = 3U;         // == VerbCall::WRITE
constexpr uint32_t KFC_UNIFIED_VERB_REDUCE = 4U;        // == VerbCall::REDUCE
constexpr uint32_t KFC_UNIFIED_VERB_BROADCAST = 5U;     // == VerbCall::BROADCAST
constexpr uint32_t KFC_UNIFIED_VERB_SYNC_PRE = 6U;      // == VerbCall::SYNC_PEER_PRE
constexpr uint32_t KFC_UNIFIED_VERB_SYNC_POST = 7U;     // == VerbCall::SYNC_PEER_POST
constexpr uint32_t KFC_UNIFIED_VERB_NHR_RUN = 8U;       // == VerbCall::NHR_RUN（R13）
// R18 · #11（M0_R18 §四）：NHR 步进流展开动词——SLICE_REDUCE（方向无关的逐
// slice 规约，帧带 step/sliceIdx 索引+双侧 peer）+ CH_SYNC（步间通道握手半边
// 四态）。纯追加（枚举尾），NHR_RUN 紧凑帧形态保留（灰度共存）。
constexpr uint32_t KFC_UNIFIED_VERB_SLICE_REDUCE = 9U;  // == VerbCall::SLICE_REDUCE（R18）
constexpr uint32_t KFC_UNIFIED_VERB_CH_SYNC = 10U;      // == VerbCall::CH_SYNC（R18）
constexpr uint32_t KFC_UNIFIED_VERB_ID_MAX = 10U;
static_assert(KFC_UNIFIED_VERB_ID_MAX < UINT64_MAX,
    "v5 verb ids must never collide with the INVALID_U64 exit sentinel");

// v5 帧槽位枚举（逐动词载荷布局见上方注释表）
enum KfcUnifiedVerbSlot : uint32_t {
    KFC_UNIFIED_VERB_ID = 0,
    KFC_VERB_SLOT_ARG0 = 1,    // 数据帧=srcOff / SYNC=掩码 / BROADCAST=srcOff
    KFC_VERB_SLOT_ARG1 = 2,    // 数据帧=dstOff / SYNC_PRE=本端 IN 基址
    KFC_VERB_SLOT_ARG2 = 3,    // 数据帧=size / SYNC_PRE=本端 SCR 基址
    KFC_VERB_SLOT_ARG3 = 4,    // BROADCAST=remoteDstOff / SYNC_PRE=本端 OUT 基址
    KFC_VERB_GO_SIZE_0 = 5,    // BROADCAST/REDUCE：goSize 四元组（其余动词 0）
    KFC_VERB_GO_SIZE_1 = 6,
    KFC_VERB_GO_SIZE_2 = 7,
    KFC_VERB_GO_SIZE_3 = 8,
    KFC_VERB_SLOTS_PER_FRAME = 32U,   // == CCU_PARAM_NUM_PER_DIE（server Load 宽度）
};

// R13 · NHR_RUN 帧槽位（M0_R13 §八；槽序镜像 KFC_RS_SOLE_NHR_*（上方 :140-152）
// + 槽 10 jettyNum 扩展——v5 段数据帧布局沿用 sole NHR 槽序，主会话预置裁决 1）：
//   槽 1 = INPUT 基址偏移（段内结构量，server 合成本端 ve.localIn / 对端 peerIn[chIdx]）
//   槽 2 = OUTPUT 基址（★绝对地址——写入方 recvBuf 合成式注入；NHR 收尾 LocalCopy
//          直写 output，无对端交换）
//   槽 3 = SLICE_SIZE（每 rank 输出份字节）
//   槽 4 = INPUT_SLICE_STRIDE（各 rank 分片在 input 的步长）
//   槽 5 = SLICE_ONE_JETTY_SIZE（jettyNum==1 时不被消费——kernel :114 循环零次）
//   槽 6 = SLICE_LAST_JETTY_SIZE（尾 jetty 字节）
//   槽 7 = REPEAT_NUM_INV（UINT64_MAX - repeatNum 倒计数）
//   槽 8 = INPUT_REPEAT_STRIDE
//   槽 9 = OUTPUT_REPEAT_STRIDE
//   槽 10 = JETTY_NUM（devkit 链恒 1；>1 是 kernel 能力协议面保留）
//   槽 11..30 = 0；槽 31 = pattern（段级载体在 SYNC_PRE 帧，数据帧槽 31 恒 0）
enum KfcVerbNhrRunSlot : uint32_t {
    KFC_VERB_NHR_INPUT = 1,
    KFC_VERB_NHR_OUTPUT = 2,
    KFC_VERB_NHR_SLICE_SIZE = 3,
    KFC_VERB_NHR_INPUT_SLICE_STRIDE = 4,
    KFC_VERB_NHR_SLICE_ONE_JETTY_SIZE = 5,
    KFC_VERB_NHR_SLICE_LAST_JETTY_SIZE = 6,
    KFC_VERB_NHR_REPEAT_NUM_INV = 7,
    KFC_VERB_NHR_INPUT_REPEAT_STRIDE = 8,
    KFC_VERB_NHR_OUTPUT_REPEAT_STRIDE = 9,
    KFC_VERB_NHR_JETTY_NUM = 10,
    KFC_VERB_NHR_PARAM_NUM = 11,
};
// 槽序互锁（三处同名登记之二——与 sole NHR 布局静态守门，漂移即编译失败；
// 值比较走 uint32_t 显式转换（两枚举类型不同，直接 == 触发 -Wenum-compare 告警——
// 下方 V3_SLOT_GUARD 同惯例）
constexpr uint32_t NHR_SLOT_GUARD(uint32_t v) { return v; }
static_assert(NHR_SLOT_GUARD(KFC_VERB_NHR_SLICE_SIZE) == NHR_SLOT_GUARD(KFC_RS_SOLE_NHR_SLICE_SIZE),
    "v5 NHR slot 3 drift");
static_assert(NHR_SLOT_GUARD(KFC_VERB_NHR_INPUT_SLICE_STRIDE) ==
                  NHR_SLOT_GUARD(KFC_RS_SOLE_NHR_INPUT_SLICE_STRIDE),
    "v5 NHR slot 4 drift");
static_assert(NHR_SLOT_GUARD(KFC_VERB_NHR_SLICE_ONE_JETTY_SIZE) ==
                  NHR_SLOT_GUARD(KFC_RS_SOLE_NHR_SLICE_ONE_JETTY_SIZE),
    "v5 NHR slot 5 drift");
static_assert(NHR_SLOT_GUARD(KFC_VERB_NHR_SLICE_LAST_JETTY_SIZE) ==
                  NHR_SLOT_GUARD(KFC_RS_SOLE_NHR_SLICE_LAST_JETTY_SIZE),
    "v5 NHR slot 6 drift");
static_assert(NHR_SLOT_GUARD(KFC_VERB_NHR_REPEAT_NUM_INV) ==
                  NHR_SLOT_GUARD(KFC_RS_SOLE_NHR_REPEAT_NUM_INV),
    "v5 NHR slot 7 drift");
static_assert(NHR_SLOT_GUARD(KFC_VERB_NHR_INPUT_REPEAT_STRIDE) ==
                  NHR_SLOT_GUARD(KFC_RS_SOLE_NHR_INPUT_REPEAT_STRIDE),
    "v5 NHR slot 8 drift");
static_assert(NHR_SLOT_GUARD(KFC_VERB_NHR_OUTPUT_REPEAT_STRIDE) ==
                  NHR_SLOT_GUARD(KFC_RS_SOLE_NHR_OUTPUT_REPEAT_STRIDE),
    "v5 NHR slot 9 drift");

// R18 · #11（M0_R18 §四）：NHR 展开动词帧槽（SLICE_REDUCE/CH_SYNC——步/slice
// 循环外置到帧序，索引+双侧 peer 进帧，repeat 种子段级补写）。槽位与 unified
// 侧 UnifiedV5SliceReduceSlot/UnifiedV5ChSyncSlot 镜像（三处同名登记之二）。
enum KfcVerbSliceReduceSlot : uint32_t {
    KFC_VERB_SR_STEP = 1,               // 步索引（执行端步表重建入参）
    KFC_VERB_SR_SLICE_IDX = 2,          // slice 索引（tx 序重建入参）
    KFC_VERB_SR_SIZE = 3,               // SliceSizeOf（校验用）
    KFC_VERB_SR_PEER_WR = 4,            // WR 半边 peer（algRank——sendTo）
    KFC_VERB_SR_PEER_RD = 5,            // RD 半边 peer（algRank——recvFrom）
    KFC_VERB_SR_INPUT_SLICE_STRIDE = 6, // step0-skip 的 input 直取步长（种子）
    KFC_VERB_SR_REPEAT_NUM_INV = 7,     // repeat 种子（照 NHR_RUN 槽 7 同型）
    KFC_VERB_SR_IN_REPEAT_STRIDE = 8,
    KFC_VERB_SR_OUT_REPEAT_STRIDE = 9,
    KFC_VERB_SR_JETTY_NUM = 10,
};
enum KfcVerbChSyncSlot : uint32_t {
    KFC_VERB_CS_PEER = 1,               // peer（algRank）
    KFC_VERB_CS_DIR = 2,                // 四态 {0=ACK-Rec,1=ACK-Wait,2=DATA-Rec,3=DATA-Wait}
    KFC_VERB_CS_IDX = 3,                // NOTIFY_IDX（0=ACK/1=DATA——与 dir 冗余校验）
};

// XN 统一变量表 + SYNC_PRE 掩码（M0 §二；值 = 位号，掩码 = 位集合）
constexpr uint32_t KFC_VERB_XN_IN = 0U;
constexpr uint32_t KFC_VERB_XN_SCR = 1U;
constexpr uint32_t KFC_VERB_XN_TOK = 2U;
constexpr uint32_t KFC_VERB_XN_OUT = 3U;
constexpr uint32_t KFC_VERB_XN_POST = 4U;
constexpr uint32_t KFC_VERB_XN_MASK_AG_PRE = (1U << KFC_VERB_XN_OUT) | (1U << KFC_VERB_XN_TOK);
constexpr uint32_t KFC_VERB_XN_MASK_RS_PRE =
    (1U << KFC_VERB_XN_IN) | (1U << KFC_VERB_XN_SCR) | (1U << KFC_VERB_XN_TOK);
constexpr uint32_t KFC_VERB_XN_MASK_POST = (1U << KFC_VERB_XN_POST);
// R13 · NHR 段 SYNC_PRE 掩码（M0_R13_NHR_V5.md §五）：IN|TOK = 0x5。老 NHR kernel 表
//（INPUT=0/TOKEN=1 → 掩码 0x3，ccu_kernel_kfc_...nhr1d_...cc:19-20）是旧编码，迁 v5
// 统一位（TOK=2 恒等；SCR/OUT 位不广播——NHR 无 scratch、无 output 交换）。
constexpr uint32_t KFC_VERB_XN_MASK_NHR_PRE = (1U << KFC_VERB_XN_IN) | (1U << KFC_VERB_XN_TOK);

// R13 · v5 帧段 pattern 载体（M0_R13 §五）：SYNC_PRE 帧槽 31 = pattern（段级语义挂
// 段首帧——每段必有 SYNC_PRE，天然段界标；blob 头部槽是 Plan 级不适用）。MESH=0
// 缺省零变化（既有帧该槽恒 0）；NHR=1 声明性登记——server 实际分派按 SYNC_PRE 掩码
//（0xC/0x7/0x5 三互异），槽 31 是写入方/host Validate/ST 消费的冗余防线。
constexpr uint32_t KFC_VERB_SLOTS_PER_FRAME_ = 32U;   // == KFC_VERB_SLOTS_PER_FRAME
constexpr uint32_t KFC_VERB_SLOT_PATTERN = 31U;
constexpr uint32_t KFC_VERB_PATTERN_MESH = 0U;
constexpr uint32_t KFC_VERB_PATTERN_NHR = 1U;

// v5 动词 kernel 注册名前缀（server host 侧第二档识别键，与 "CcuAction" 同机制：
// InheritKfcServerKernelArg 按 compare(0, LEN, PREFIX) 命中覆写 role=UNIFIED_VERB）。
// 第四轮 BLOCKER-1 纪律：前缀字面量与子串长度 static_assert 互锁 + 判定回归断言。
constexpr char KFC_UNIFIED_VERB_PREFIX[] = "CcuVerb";
constexpr uint32_t KFC_UNIFIED_VERB_PREFIX_LEN = 7U;    // == strlen(KFC_UNIFIED_VERB_PREFIX)

// v5 blob 头部常量（镜像 v4 头布局；与 unified inc/verb_serializer.h 镜像常量互锁）
constexpr uint64_t KFC_UNIFIED_VERB_BLOB_MAGIC = 0x55504C35ULL;     // "UPL5"
constexpr uint64_t KFC_UNIFIED_VERB_BLOB_VERSION = 5ULL;
constexpr uint32_t KFC_UNIFIED_VERB_BLOB_HEADER_SLOTS = 7U;
constexpr uint32_t KFC_UNIFIED_VERB_BLOB_HEADER_RESERVED = 3U;      // 槽 3 保留（0）
constexpr uint32_t KFC_UNIFIED_VERB_BLOB_HEADER_DATA_TYPE = 4U;     // 纯维测 dump（R6 终裁口径）
constexpr uint32_t KFC_UNIFIED_VERB_BLOB_HEADER_OUTPUT_TYPE = 5U;
constexpr uint32_t KFC_UNIFIED_VERB_BLOB_HEADER_REDUCE_TYPE = 6U;
constexpr uint32_t KfcUnifiedVerbBlobTotalSlots(uint32_t frameCount)
{
    return KFC_UNIFIED_VERB_BLOB_HEADER_SLOTS + frameCount * KFC_VERB_SLOTS_PER_FRAME;
}
// 单 Plan 帧数上限：环形队列 64 帧（CCU_TASK_NUM_MAX）− 1 出口哨兵（跨圈多帧挂 P2，
// A/B tiered 不进 R12 verb 灰度——B 步进 16P 档 72 帧超限，M0 §三）
constexpr uint32_t KFC_UNIFIED_VERB_MAX_FRAMES = 63U;

// ── 数据帧计数契约（R12-19 登记项；本表是 v5 帧序隐式映射的权威口径）──────────
// 隐式映射（数据帧不携 channelIdx，帧序即通道序）下，两处消费者按**同一计数规则**
// 推导数据帧 ↔ 通道的配对，错一处即配对错位（隐式映射断链）：
//   ① 写入方 CcuPrepareForUnifiedVerbPlan（hccl_ccu_v0_prepare.h）的 dataFrameIdx
//      ——第 k 条数据帧的 peerAlgRank = 升序跳过 me 的第 k 个 rank；
//   ② server DispatchUnifiedVerbs（ccu_kernel_kfc_server.cc）的 ve.dataCursor
//      ——第 k 条数据帧消费 channels[k]。
// 逐动词计数规则（动词 ID 见上方常量）：
//   | 动词            | 计数 | 理由                                                |
//   |-----------------|------|-----------------------------------------------------|
//   | READ(2)         | ++   | RS 读侧——一通道一帧                                 |
//   | WRITE(3)        | ++   | 写侧——一通道一帧（CCU v5 不产，协议完备保留）       |
//   | COPY(0)         | 不计 | 本端上料/本地搬运，不占通道序（server 游标同口径不动）|
//   | BROADCAST(5)    | 不计 | 组原语自含扇出，独占本段通道表（server 游标置满防越界）|
//   | REDUCE(4)       | 不计 | 本地折叠，不走通道                                   |
//   | SYNC_PEER_PRE(6)| 复位 | 段边界——新段数据帧从首通道重新配对（计数器清 0）     |
//   | SYNC_PEER_POST(7)| 不计 | 非数据动词                                          |
//   | POST_COPY(1)    | 不计 | AICPU 专用（CCU 不发射），且语义为本地落位不占通道   |
//   | NHR_RUN(8)      | 不计 | R13（M0_R13 §二）：NHR 段整段自持通道表——数据帧序
//   |                 |      | ≠ 通道序（通道 = chIdx(toRank) 显式推导，每步全部
//   |                 |      | slice 走同一通道），隐式游标对 NHR 不适用（游标不动）|
// ★演进契约：序列化器（unified inc/verb_serializer.h）新增**数据动词**（隐式消费
//   通道序的动词）时，必须同步三处——本表 + 写入方 dataFrameIdx 计数分支 + server
//   Verb* 游标推进分支；host Validate 的"数据帧数==channelCount"断言是配对断链的
//   唯一前置防线（M0_R12_VERB_EXECUTOR.md §六-1 引用本表）。
// ─────────────────────────────────────────────────────────────────────────────

// 段数上限（机制轮）：单圈队列 64 记录（CCU_TASK_NUM_MAX，ccu_kernel_kfc_server.h:27）
// 预留 1 条出口哨兵 → 有效 63 段；跨圈多段随 device 写入方（下一轮）落地后放开。
constexpr uint32_t KFC_UNIFIED_MAX_SEGMENTS = 63U;

// ============================================================================
// v3 blob 头部布局（M2，2026-09-21；R6 头部扩槽 4→7，2026-09-21）：unified 序列化器
// （plan_serializer.h 的 SerializePlanV3）产出的槽序列前 7 槽是定长头部——device 写入方
// （CcuPrepareForUnifiedPlan）剥掉它取段记录区。布局镜像自
// unified/inc/plan_serializer.h（两侧互不 include：本头在 devkit 设备链，unified
// 树不引设备头），三处同名登记守门（unified 测试 static_assert 镜像常量）：
//   槽 0 = magic        0x55504C33（"UPL3"，PLAN_V3_MAGIC）
//   槽 1 = 版本          4（R6 升版：dataType/outputType/reduceType 三值进头部。
//                         ★头部三槽降级为纯维测 dump（R6 尾款终裁，dataType=0 两口径
//                         裁决）：AICPU 消费方一律以 exec 期 msg 现值为唯一权威源
//                         （方向 B），头部值仅日志/trace 用，不作语义判定输入——原
//                         "从 blob 重建 ReduceSpec"口径作废。
//                         版本升 4 而非复用槽 3 保留位：dataType=0（INT8）是合法值，
//                         与"保留 0"无法区分。KFC_SERVER_PROTOCOL_VERSION_V3=3 是
//                         协议族常量（v3 分发机制），与 blob 版本字段解耦不联动）
//   槽 2 = 段数          N（== 段记录帧数；写入方做头部段数 vs blob 帧数交叉校验，M4-①）
//   槽 3 = 保留          0（既有语义不动）
//   槽 4 = dataType      （ReduceSpec.dataType；AG 段也写——头部是 Plan 级而非段级；dump）
//   槽 5 = outputType    （ReduceSpec.outputType；★存量语义分歧登记（P2 债台账项，
//                         只登记不改行为）：AICPU exec 期适配器强等 dataType
//                         （comm_kfc_open_kernel_adapter.cc:141）vs 资源期 AG prepare
//                         的 outputType=dstDataType（hccl_alloc_ctx_res.cc:510））
//   槽 6 = reduceType    （ReduceSpec.reduceOp；dump）
// 段记录区 = N × 32 槽（每帧即 server 一次 Load 的宽度）。
// CCU server 不读头部三槽（dataType/outputType/reduceType 从 ctx.arg->opParam 取，
// 现状不动）；AICPU 消费方不以其为判定输入——头部三槽纯 dump（R6 主会话裁决一 +
// 尾款终裁）。
// ============================================================================
constexpr uint64_t KFC_UNIFIED_BLOB_MAGIC = 0x55504C33ULL;       // == PLAN_V3_MAGIC
constexpr uint64_t KFC_UNIFIED_BLOB_VERSION = 4ULL;              // == PLAN_V3_VERSION_FIELD（R6 升 4）
constexpr uint32_t KFC_UNIFIED_BLOB_HEADER_SLOTS = 7U;           // == UNIFIED_V3_HEADER_SLOTS（R6 扩 4→7）
constexpr uint32_t KFC_UNIFIED_BLOB_HEADER_RESERVED = 3U;        // 槽 3 保留位（既有语义不动）
constexpr uint32_t KFC_UNIFIED_BLOB_HEADER_DATA_TYPE = 4U;       // == UNIFIED_V3_HEADER_DATA_TYPE
constexpr uint32_t KFC_UNIFIED_BLOB_HEADER_OUTPUT_TYPE = 5U;     // == UNIFIED_V3_HEADER_OUTPUT_TYPE
constexpr uint32_t KFC_UNIFIED_BLOB_HEADER_REDUCE_TYPE = 6U;     // == UNIFIED_V3_HEADER_REDUCE_TYPE
// blob 总槽数（头部 + N 段）——写入方按此校验 blob 边界。KFC_UNIFIED_PARAM_NUM_PER_RECORD
//（=32，段记录槽宽）定义在下文 KfcUnifiedPlanParamIndex 尾部，此处先给字面等值常量。
constexpr uint32_t KFC_UNIFIED_BLOB_SLOTS_PER_RECORD = 32U;      // == KFC_UNIFIED_PARAM_NUM_PER_RECORD
constexpr uint32_t KfcUnifiedBlobTotalSlots(uint32_t segNum)
{
    return KFC_UNIFIED_BLOB_HEADER_SLOTS + segNum * KFC_UNIFIED_BLOB_SLOTS_PER_RECORD;
}

// CollectAll 首发段的槽位布局：槽 1..14 与 v2 KFC_CONCURRENT_AG_MESH_* 的 1..14
// 逐槽相等（复用 server AG else 分支已验证的读取序，ccu_kernel_kfc_server.cc:223-241
// 的参数转发序），下方 static_assert 编译期守门；token 不占槽位（v2 格局照抄：
// server 侧恒取 ctx.token，与 KFC_CONCURRENT_AG_MESH_* 不含 token 的格局一致）。
enum KfcUnifiedPlanParamIndex : uint32_t {
    KFC_UNIFIED_ACTION_ID = 0,
    KFC_UNIFIED_COLLECT_ALL_INPUT = 1,                              // 本 rank input 基址（段内 offset）
    KFC_UNIFIED_COLLECT_ALL_OUTPUT = 2,                             // output 基址（段内 offset）
    KFC_UNIFIED_COLLECT_ALL_OUTPUT_OFFSET = 3,                      // 本 rank 片在 output 的落位偏移
    KFC_UNIFIED_COLLECT_ALL_SLICE_SIZE = 4,                         // 本 rank 片字节（含尾片修正）
    KFC_UNIFIED_COLLECT_ALL_GO_SIZE_0 = 5,                          // loop 三段切分（CalGoSize 同式）
    KFC_UNIFIED_COLLECT_ALL_GO_SIZE_1 = 6,
    KFC_UNIFIED_COLLECT_ALL_GO_SIZE_2 = 7,
    KFC_UNIFIED_COLLECT_ALL_GO_SIZE_3 = 8,
    KFC_UNIFIED_COLLECT_ALL_CURRENT_RANK_SLICE_INPUT_OFFSET = 9,    // 本 rank 片在 input 的源偏移
    KFC_UNIFIED_COLLECT_ALL_REPEAT_NUM_INV = 10,                    // UINT64_MAX - repeatNum
    KFC_UNIFIED_COLLECT_ALL_INPUT_REPEAT_STRIDE = 11,
    KFC_UNIFIED_COLLECT_ALL_OUTPUT_REPEAT_STRIDE = 12,
    KFC_UNIFIED_COLLECT_ALL_LAST_SLICE_SIZE = 13,                   // 末 rank 片字节（kernel 末 rank 分支用）
    KFC_UNIFIED_COLLECT_ALL_INPUT_OUTPUT_EQUAL = 14,                // 就地（inplace）标记
    KFC_UNIFIED_COLLECT_ALL_PARAM_NUM = 15,
    // ReduceSlice 段（R6，2026-09-21）：槽 1..15 == v2 KFC_CONCURRENT_RS_MESH_* 的 1..15
    // （并发布局，KfcConcurrentReduceScatterParamIndex——sole-mesh host 写入方的 12 参
    // homm 兼容布局是干扰项不抄）。槽 16..31 = 0（未用槽）。★R9 M0-④ 槽位实数
    // 勘误（2026-10-07）：v2 并发 RS 的 NHR 区间为槽 16..24（KFC_CONCURRENT_RS_NHR_*
    // 至 KFC_CONCURRENT_RS_PARAM_NUM=25，:173-182），含动作 ID 25 槽 ≤ 32 槽记录宽
    // ——原注"起 16 跨 20 超 32"系未逐槽核数的误传。R9 维持 RS 帧 MESH-only 的真实
    // 缺口：v3 帧无 pattern 载体（DispatchUnifiedPlanV3 只认槽 0 ∈ {CollectAll,
    // ReduceSlice}，帧槽按 Mesh 布局解读）+ NHR kernel 形参的 jettyNum/
    // nhrStepInfoVector/nhrRank2ChannelIdx 三件 ctx.arg 自持量在 unified 注册链
    // （"CcuAction" 前缀 → UNIFIED_PLAN 覆写分支）无人继承填充——放开须 v5 blob
    // 协议面 + 注册链继承 + server NHR 分支三件联动（方案文档 R9 M0-④）。
    // dataType/outputType/reduceType 不占段槽：
    // server 分支与 v2 同式从 ctx.arg->opParam 取（ccu_kernel_kfc_server.cc:337-339）；
    // 头部三槽（KFC_UNIFIED_BLOB_*_TYPE）是**纯维测 dump**（R6 尾款终裁：AICPU 消费方
    // 一律以 exec 期 msg 现值为唯一权威源，头部值仅日志/trace 用，不作语义判定输入），
    // CCU server 不读头部三槽（ctx.arg->opParam 现状不动）。
    // ★存量 outputType 语义分歧登记（P2 债台账项，只登记不改行为）：AICPU exec 期适配器
    // 强等 dataType（comm_kfc_open_kernel_adapter.cc:141）vs 资源期 AG prepare 的
    // outputType=dstDataType（hccl_alloc_ctx_res.cc:510）——RS 段 outputType 语义两链路
    // 不同源，P2 接 AICPU 消费方时统一。
    KFC_UNIFIED_REDUCE_SLICE_INPUT = 1,
    KFC_UNIFIED_REDUCE_SLICE_OUTPUT = 2,
    KFC_UNIFIED_REDUCE_SLICE_SCRATCH = 3,                           // scratch 中转基址（hcclBuff 语义）
    KFC_UNIFIED_REDUCE_SLICE_RANK_SLICE_OFFSET = 4,                 // rankId*sliceStride（纯偏移，不含基址）
    KFC_UNIFIED_REDUCE_SLICE_CHUNK_SIZE = 5,                         // scratchSize(64MB)/rankNum/128*128 对齐
    KFC_UNIFIED_REDUCE_SLICE_CHUNK_LOOP_NUM = 6,                     // UINT64_MAX - chunkCount（倒计数哨兵）
    KFC_UNIFIED_REDUCE_SLICE_TAIL_SIZE = 7,                          // 尾 chunk 字节
    KFC_UNIFIED_REDUCE_SLICE_FULL_GO_ADDR_OFFSET = 8,               // 满 chunk goSize 四元组
    KFC_UNIFIED_REDUCE_SLICE_FULL_GO_LOOP_PARAM = 9,
    KFC_UNIFIED_REDUCE_SLICE_FULL_GO_PARALLEL_PARAM = 10,
    KFC_UNIFIED_REDUCE_SLICE_FULL_GO_RESIDUAL = 11,
    KFC_UNIFIED_REDUCE_SLICE_TAIL_GO_ADDR_OFFSET = 12,               // 尾 chunk goSize 四元组
    KFC_UNIFIED_REDUCE_SLICE_TAIL_GO_LOOP_PARAM = 13,
    KFC_UNIFIED_REDUCE_SLICE_TAIL_GO_PARALLEL_PARAM = 14,
    KFC_UNIFIED_REDUCE_SLICE_TAIL_GO_RESIDUAL = 15,
    KFC_UNIFIED_REDUCE_SLICE_PARAM_NUM = 16,
    KFC_UNIFIED_PARAM_NUM_PER_RECORD = 32U, // == CCU_PARAM_NUM_PER_DIE（server Load 宽度）
};
static_assert(KFC_UNIFIED_COLLECT_ALL_PARAM_NUM <= KFC_UNIFIED_PARAM_NUM_PER_RECORD,
    "v3 CollectAll parameters exceed the 32-slot record width");
static_assert(KFC_UNIFIED_REDUCE_SLICE_PARAM_NUM <= KFC_UNIFIED_PARAM_NUM_PER_RECORD,
    "v3 ReduceSlice parameters (16) fit the 32-slot record width (R6 slot-cap proof)");
// v3 槽 1..14 == v2 AG Mesh 槽 1..14：读取序复用的编译期守门（三处同名登记之一）。
// 值比较走 uint32_t 显式转换（两枚举类型不同，直接 == 触发 -Wenum-compare 告警）。
constexpr uint32_t V3_SLOT_GUARD(uint32_t v) { return v; }
static_assert(V3_SLOT_GUARD(KFC_UNIFIED_COLLECT_ALL_INPUT) == V3_SLOT_GUARD(KFC_CONCURRENT_AG_MESH_INPUT),
    "v3/v2 slot 1 drift");
static_assert(V3_SLOT_GUARD(KFC_UNIFIED_COLLECT_ALL_OUTPUT) == V3_SLOT_GUARD(KFC_CONCURRENT_AG_MESH_OUTPUT),
    "v3/v2 slot 2 drift");
static_assert(V3_SLOT_GUARD(KFC_UNIFIED_COLLECT_ALL_OUTPUT_OFFSET) == V3_SLOT_GUARD(KFC_CONCURRENT_AG_MESH_OUTPUT_OFFSET),
    "v3/v2 slot 3 drift");
static_assert(V3_SLOT_GUARD(KFC_UNIFIED_COLLECT_ALL_SLICE_SIZE) == V3_SLOT_GUARD(KFC_CONCURRENT_AG_MESH_SLICE_SIZE),
    "v3/v2 slot 4 drift");
static_assert(V3_SLOT_GUARD(KFC_UNIFIED_COLLECT_ALL_GO_SIZE_0) == V3_SLOT_GUARD(KFC_CONCURRENT_AG_MESH_GO_SIZE_0),
    "v3/v2 slot 5 drift");
static_assert(V3_SLOT_GUARD(KFC_UNIFIED_COLLECT_ALL_GO_SIZE_1) == V3_SLOT_GUARD(KFC_CONCURRENT_AG_MESH_GO_SIZE_1),
    "v3/v2 slot 6 drift");
static_assert(V3_SLOT_GUARD(KFC_UNIFIED_COLLECT_ALL_GO_SIZE_2) == V3_SLOT_GUARD(KFC_CONCURRENT_AG_MESH_GO_SIZE_2),
    "v3/v2 slot 7 drift");
static_assert(V3_SLOT_GUARD(KFC_UNIFIED_COLLECT_ALL_GO_SIZE_3) == V3_SLOT_GUARD(KFC_CONCURRENT_AG_MESH_GO_SIZE_3),
    "v3/v2 slot 8 drift");
static_assert(V3_SLOT_GUARD(KFC_UNIFIED_COLLECT_ALL_CURRENT_RANK_SLICE_INPUT_OFFSET) ==
                  V3_SLOT_GUARD(KFC_CONCURRENT_AG_MESH_CURRENT_RANK_SLICE_INPUT_OFFSET),
    "v3/v2 slot 9 drift");
static_assert(V3_SLOT_GUARD(KFC_UNIFIED_COLLECT_ALL_REPEAT_NUM_INV) == V3_SLOT_GUARD(KFC_CONCURRENT_AG_MESH_REPEAT_NUM_INV),
    "v3/v2 slot 10 drift");
static_assert(V3_SLOT_GUARD(KFC_UNIFIED_COLLECT_ALL_INPUT_REPEAT_STRIDE) ==
                  V3_SLOT_GUARD(KFC_CONCURRENT_AG_MESH_INPUT_REPEAT_STRIDE),
    "v3/v2 slot 11 drift");
static_assert(V3_SLOT_GUARD(KFC_UNIFIED_COLLECT_ALL_OUTPUT_REPEAT_STRIDE) ==
                  V3_SLOT_GUARD(KFC_CONCURRENT_AG_MESH_OUTPUT_REPEAT_STRIDE),
    "v3/v2 slot 12 drift");
static_assert(V3_SLOT_GUARD(KFC_UNIFIED_COLLECT_ALL_LAST_SLICE_SIZE) == V3_SLOT_GUARD(KFC_CONCURRENT_AG_MESH_LAST_SLICE_SIZE),
    "v3/v2 slot 13 drift");
static_assert(V3_SLOT_GUARD(KFC_UNIFIED_COLLECT_ALL_INPUT_OUTPUT_EQUAL) ==
                  V3_SLOT_GUARD(KFC_CONCURRENT_AG_MESH_INPUT_OUTPUT_EQUAL),
    "v3/v2 slot 14 drift");
// v3 RS 槽 1..15 == v2 并发 RS Mesh 槽 1..15：读取序复用的编译期守门（R6 三处同名
// 登记的 RS 半边——server v3 REDUCE_SLICE 分支的参数转发序与 v2 REDUCE_SCATTER_MESH
// 分支逐槽一致，漂移即编译失败）。
static_assert(V3_SLOT_GUARD(KFC_UNIFIED_REDUCE_SLICE_INPUT) == V3_SLOT_GUARD(KFC_CONCURRENT_RS_MESH_INPUT),
    "v3/v2 RS slot 1 drift");
static_assert(V3_SLOT_GUARD(KFC_UNIFIED_REDUCE_SLICE_OUTPUT) == V3_SLOT_GUARD(KFC_CONCURRENT_RS_MESH_OUTPUT),
    "v3/v2 RS slot 2 drift");
static_assert(V3_SLOT_GUARD(KFC_UNIFIED_REDUCE_SLICE_SCRATCH) == V3_SLOT_GUARD(KFC_CONCURRENT_RS_MESH_SCRATCH),
    "v3/v2 RS slot 3 drift");
static_assert(V3_SLOT_GUARD(KFC_UNIFIED_REDUCE_SLICE_RANK_SLICE_OFFSET) ==
                  V3_SLOT_GUARD(KFC_CONCURRENT_RS_MESH_RANK_SLICE_OFFSET),
    "v3/v2 RS slot 4 drift");
static_assert(V3_SLOT_GUARD(KFC_UNIFIED_REDUCE_SLICE_CHUNK_SIZE) == V3_SLOT_GUARD(KFC_CONCURRENT_RS_MESH_CHUNK_SIZE),
    "v3/v2 RS slot 5 drift");
static_assert(V3_SLOT_GUARD(KFC_UNIFIED_REDUCE_SLICE_CHUNK_LOOP_NUM) ==
                  V3_SLOT_GUARD(KFC_CONCURRENT_RS_MESH_CHUNK_LOOP_NUM),
    "v3/v2 RS slot 6 drift");
static_assert(V3_SLOT_GUARD(KFC_UNIFIED_REDUCE_SLICE_TAIL_SIZE) == V3_SLOT_GUARD(KFC_CONCURRENT_RS_MESH_TAIL_SIZE),
    "v3/v2 RS slot 7 drift");
static_assert(V3_SLOT_GUARD(KFC_UNIFIED_REDUCE_SLICE_FULL_GO_ADDR_OFFSET) ==
                  V3_SLOT_GUARD(KFC_CONCURRENT_RS_MESH_FULL_GO_ADDR_OFFSET),
    "v3/v2 RS slot 8 drift");
static_assert(V3_SLOT_GUARD(KFC_UNIFIED_REDUCE_SLICE_FULL_GO_LOOP_PARAM) ==
                  V3_SLOT_GUARD(KFC_CONCURRENT_RS_MESH_FULL_GO_LOOP_PARAM),
    "v3/v2 RS slot 9 drift");
static_assert(V3_SLOT_GUARD(KFC_UNIFIED_REDUCE_SLICE_FULL_GO_PARALLEL_PARAM) ==
                  V3_SLOT_GUARD(KFC_CONCURRENT_RS_MESH_FULL_GO_PARALLEL_PARAM),
    "v3/v2 RS slot 10 drift");
static_assert(V3_SLOT_GUARD(KFC_UNIFIED_REDUCE_SLICE_FULL_GO_RESIDUAL) ==
                  V3_SLOT_GUARD(KFC_CONCURRENT_RS_MESH_FULL_GO_RESIDUAL),
    "v3/v2 RS slot 11 drift");
static_assert(V3_SLOT_GUARD(KFC_UNIFIED_REDUCE_SLICE_TAIL_GO_ADDR_OFFSET) ==
                  V3_SLOT_GUARD(KFC_CONCURRENT_RS_MESH_TAIL_GO_ADDR_OFFSET),
    "v3/v2 RS slot 12 drift");
static_assert(V3_SLOT_GUARD(KFC_UNIFIED_REDUCE_SLICE_TAIL_GO_LOOP_PARAM) ==
                  V3_SLOT_GUARD(KFC_CONCURRENT_RS_MESH_TAIL_GO_LOOP_PARAM),
    "v3/v2 RS slot 13 drift");
static_assert(V3_SLOT_GUARD(KFC_UNIFIED_REDUCE_SLICE_TAIL_GO_PARALLEL_PARAM) ==
                  V3_SLOT_GUARD(KFC_CONCURRENT_RS_MESH_TAIL_GO_PARALLEL_PARAM),
    "v3/v2 RS slot 14 drift");
static_assert(V3_SLOT_GUARD(KFC_UNIFIED_REDUCE_SLICE_TAIL_GO_RESIDUAL) ==
                  V3_SLOT_GUARD(KFC_CONCURRENT_RS_MESH_TAIL_GO_RESIDUAL),
    "v3/v2 RS slot 15 drift");

} // namespace HcclKfcProtocol

#endif
