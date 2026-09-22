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
    // homm 兼容布局是干扰项不抄）。槽 16..31 = 0（未用槽；RS NHR 形态槽位起 16 跨 20 槽
    // 超 32 槽记录宽，R9 落地时另行评估扩段）。dataType/outputType/reduceType 不占段槽：
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
