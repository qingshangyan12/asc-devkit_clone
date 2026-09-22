/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * CANN Open Software License Agreement Version 2.0.
 *
 * 重构 PoC · L3 值对象 · Plan 序列化 v3（§10.2 A4 / §12-11 甲路线，2026-09-21）
 *
 * KFC 消息槽协议 v3 字节布局（KfcUnifiedPlanParamIndex，devkit 侧
 * common/kfc_server_message.h）——真序列化，第四轮兑现点：此前三轮为 PoC 摘要格式
 * （不可重建 Plan，三轮 MAJOR 降级声明）。
 *
 * 单独成头的原因：序列化要算 goSize（LoopSplitter 公式，config 对齐 v2 写入方），
 * 依赖 backend_registry.h 的完整类型；而 backend_registry.h 又 include
 * action_spec.h（Engine/IBackend 需 ActionSpec）——action_spec.h 内直接 include
 * backend_registry.h 会形成 include 循环且 namespace 收口易错，故拆此头，
 * 由 action_spec.h 尾部 include（值对象先定义完）。
 */
#ifndef REFACTOR_POC_PLAN_SERIALIZER_H
#define REFACTOR_POC_PLAN_SERIALIZER_H

#include <cstdint>
#include <string>
#include <vector>

#include "action_spec.h"   // Plan / ActionSpec（值对象；本头被其尾部 include，guard
                           // 置位时此行空过）
#include "loop_splitter.h" // LoopSplitter + CalcRsChunks（goSize/chunk 计算；从
                           // backend_registry.h 拆出的最小依赖头——backend_registry.h
                           // 的 include 次序由 verb_sink 环改为 alg_param.h 后虽已无环，
                           // 但本头独立成头仍取最小依赖，不复引 backend_registry.h 的
                           // 注册表全景。★RS chunk 参数（CalcRsChunks）也落 loop_splitter.h
                           // 而非 l3/action_reduce_slice.h：action_reduce_slice.h 为
                           // CollectAll::NextPeer 依赖 action_collect_all.h——若本头
                           //（被 action_spec.h 尾部 include）转引 L3 动作头，则
                           // "TU 先 include action_collect_all.h" 的次序下其 guard 已
                           // 置位、CollectAll 未定义，L3 头内 BuildArgs 的
                           // CollectAll::NextPeer 解析失败（include 环的 guard 空过
                           // 病灶，同 loop_splitter.h 拆出头注）。纯函数下沉最小依赖头，
                           // L3 头复用同一实现点（零复制））

namespace mc2_ops_hccl::refactor {

// ---------------------------------------------------------------------------
// KFC 消息槽协议 v3 的槽位契约镜像。
// 值与 devkit 侧 kfc_server_message.h（KfcUnifiedPlanParamIndex /
// KFC_UNIFIED_ACTION_*）逐项相等——两树互不 include（unified 不带 CCU 设备头是
// L3 值对象铁律），由测试 poc_unified_serialize_v3_test.cc 的 static_assert
// （编译期常量对比 + 对 ActionKind 枚举的直连断言）守门，即"三处同名登记"防御
// （§6.12 性质）：①ActionKind 枚举 ↔ ②本镜像常量 ↔ ③server 分支 ID。
// ---------------------------------------------------------------------------
// 槽 0 动作 ID == ActionKind 值（CollectAll=0/ReduceSlice=1/ExchangeAll=2/
// LocalCopy=3/LocalReduce=4）——SerializeSegmentV3 直接写
// static_cast<u64>(seg.kind)，构造性对齐①③。
constexpr u32 UNIFIED_V3_SLOTS_PER_RECORD = 32U;  // == CCU_PARAM_NUM_PER_DIE（server Load 宽度）
// v4 头部（R6/裁决一，2026-09-21；尾部终裁：头部三槽降级纯 dump）：4→7 槽——
// 槽 4=dataType/槽 5=outputType/槽 6=reduceType（AICPU 消费方以 exec 期 msg 现值为
// 唯一权威源（方向 B），头部值仅日志/trace 用；CCU server 不读头部三槽，
// ctx.arg->opParam 现状不动）。版本字段升 4（KFC_UNIFIED_BLOB_VERSION=4）
// ——不占原槽 3 保留位的原因：dataType=0（HCCL_DATA_TYPE_INT8）是合法值，与
// "保留 0"语义无法区分。槽 3 保留位继续存在（0）。
constexpr u32 UNIFIED_V3_HEADER_SLOTS = 7U;       // v4 头部定长槽数（== KFC_UNIFIED_BLOB_HEADER_SLOTS）
constexpr u32 UNIFIED_V3_VERSION = 4U;            // == KFC_UNIFIED_BLOB_VERSION（R6 升 4）
constexpr u32 UNIFIED_V3_HEADER_RESERVED = 3U;    // == KFC_UNIFIED_BLOB_HEADER_RESERVED（槽 3 保留位）
constexpr u32 UNIFIED_V3_HEADER_DATA_TYPE = 4U;   // == KFC_UNIFIED_BLOB_HEADER_DATA_TYPE
constexpr u32 UNIFIED_V3_HEADER_OUTPUT_TYPE = 5U; // == KFC_UNIFIED_BLOB_HEADER_OUTPUT_TYPE
constexpr u32 UNIFIED_V3_HEADER_REDUCE_TYPE = 6U; // == KFC_UNIFIED_BLOB_HEADER_REDUCE_TYPE
// 槽位枚举镜像（CollectAll 段 1..14；ReduceSlice 段 1..15；槽 0 动作 ID）
enum UnifiedV3Slot : u32 {
    U_V3_ACTION_ID = 0,
    U_V3_INPUT = 1,                        // == KFC_UNIFIED_COLLECT_ALL_INPUT
    U_V3_OUTPUT = 2,
    U_V3_OUTPUT_OFFSET = 3,
    U_V3_SLICE_SIZE = 4,
    U_V3_GO_SIZE_0 = 5,
    U_V3_GO_SIZE_1 = 6,
    U_V3_GO_SIZE_2 = 7,
    U_V3_GO_SIZE_3 = 8,
    U_V3_CUR_RANK_SLICE_INPUT_OFFSET = 9,
    U_V3_REPEAT_NUM_INV = 10,              // UINT64_MAX - repeatNum（kernel 终止语义）
    U_V3_INPUT_REPEAT_STRIDE = 11,
    U_V3_OUTPUT_REPEAT_STRIDE = 12,
    U_V3_LAST_SLICE_SIZE = 13,
    U_V3_INPUT_OUTPUT_EQUAL = 14,
};
// RS 段槽位镜像（R6）：槽 1..15 == v2 KFC_CONCURRENT_RS_MESH_* 槽 1..15 逐槽相等
//（并发布局——server :327-339 读取序复用；下方 static_assert 逐槽守门）。
// 注意与 CollectAll 段同名槽（U_V3_INPUT/U_V3_OUTPUT）值相等是协议共性（1/2 恒为
// in/out 基址），[3..15] 语义不同属段内布局，互不冲突。
enum UnifiedV3RsSlot : u32 {
    U_V3_RS_INPUT = 1,                     // == KFC_UNIFIED_REDUCE_SLICE_INPUT（槽 1）
    U_V3_RS_OUTPUT = 2,                    // == KFC_UNIFIED_REDUCE_SLICE_OUTPUT
    U_V3_RS_SCRATCH = 3,                   // == KFC_UNIFIED_REDUCE_SLICE_SCRATCH
    U_V3_RS_RANK_SLICE_OFFSET = 4,         // == KFC_UNIFIED_REDUCE_SLICE_RANK_SLICE_OFFSET
    U_V3_RS_CHUNK_SIZE = 5,                // == KFC_UNIFIED_REDUCE_SLICE_CHUNK_SIZE
    U_V3_RS_CHUNK_LOOP_NUM = 6,            // == KFC_UNIFIED_REDUCE_SLICE_CHUNK_LOOP_NUM
    U_V3_RS_TAIL_SIZE = 7,                 // == KFC_UNIFIED_REDUCE_SLICE_TAIL_SIZE
    U_V3_RS_FULL_GO_ADDR_OFFSET = 8,       // == KFC_UNIFIED_REDUCE_SLICE_FULL_GO_ADDR_OFFSET
    U_V3_RS_FULL_GO_LOOP_PARAM = 9,
    U_V3_RS_FULL_GO_PARALLEL_PARAM = 10,
    U_V3_RS_FULL_GO_RESIDUAL = 11,
    U_V3_RS_TAIL_GO_ADDR_OFFSET = 12,
    U_V3_RS_TAIL_GO_LOOP_PARAM = 13,
    U_V3_RS_TAIL_GO_PARALLEL_PARAM = 14,
    U_V3_RS_TAIL_GO_RESIDUAL = 15,
};
// 段数上限：单圈队列 64 记录（CCU_TASK_NUM_MAX）− 1 出口哨兵（机制轮跨圈未放开）
constexpr u32 UNIFIED_V3_MAX_SEGMENTS = 63U;
// 头部三槽槽位（R6/裁决一）：镜像 devkit 侧 KFC_UNIFIED_BLOB_HEADER_*_TYPE
//（kfc_server_message.h——测试 static_assert 守门）
enum UnifiedV3HeaderSlot : u32 {
    U_V3_HEADER_RESERVED = 3,
    U_V3_HEADER_DATA_TYPE = 4,
    U_V3_HEADER_OUTPUT_TYPE = 5,
    U_V3_HEADER_REDUCE_TYPE = 6,
};

// ---------------------------------------------------------------------------
// 布局（u64 槽序列，与消息槽内存同构）：
//   头部 7 槽定长（R6/裁决一；尾部终裁：三槽纯 dump）：[0]=magic("UPL3")，[1]=版本(4)，
//     [2]=段数 N，[3]=保留(0)，[4]=dataType，[5]=outputType，[6]=reduceType
//     （ReduceSpec 头部登记，纯维测 dump——AICPU 消费方以 exec 期 msg 现值为准；
//     dataType=0 合法故升版本而非占保留位）
//   逐段 N×32 槽：与 server LoadFuncParamFromMemory 读取宽度一致
//     （CCU_PARAM_NUM_PER_DIE=32）；段内槽 0=动作 ID（ActionKind 值），槽 1..14
//     CollectAll 参数（布局同 v2 AG Mesh 读取序）或槽 1..15 ReduceSlice 参数
//     （布局同 v2 并发 RS Mesh 读取序），未用槽填 0。
//   出口哨兵记录（param[0]=INVALID_U64）由 device 写入方按队列协议追加，不在本
//   序列化范围（本轮不接 CCU 真注册链，§12-11 恢复条件）。
//
// 【goSize config 对齐】goSize 用存量 AG M2M 写入方同款 LoopGroupConfig：
//   {msInterleave=8, loopCount=CCU_MS_LOCAL_COPY_LOOP_COUNT=8, memSlice=CCU_MS_SIZE*8=32768}
//   （ccu_temp_kfc_all_gather_mesh_1D_mem2mem.cc:84-88，常量 ccu_kernel_alg_base.h:27-33），
//   非本仓 LoopSplitter 默认 {128, 4096}（AICPU 侧 CalGoSize 单参版口径）——同尺寸
//   两 config 产出不同 goSize，v3 必须与 device 端 consumer（GroupCopy 微码）等价，
//   即对齐 v2 写入方 config。计算本体复用 LoopSplitter::Calc（公式同源，仅 config 不同）。
// 【repeatNumInv】同存量式 UINT64_MAX - repeatNum（ccu_temp_kfc_all_gather_mesh_1D_
//   mem2mem.cc:91；kernel 侧 CCU_WHILE(tmpRepeatNum != UINT64_MAX) 终止，
//   ccu_kernel_kfc_all_gather_mesh1d_mem2mem.cc:231；repeatNum=1 → 0xFFFFFFFFFFFFFFFE）。
//
// destination 由调用方给 buffer（host 单测用 vector<u64> 模拟消息槽内存）。
// ---------------------------------------------------------------------------
constexpr u64 PLAN_V3_MAGIC = 0x55504C33ULL;   // "UPL3"（Unified Plan v3，调试 dump 可辨识）
constexpr u64 PLAN_V3_VERSION_FIELD = 4ULL;    // R6/裁决一：升 4（头部 4→7 槽）

// 单段 → 32 槽记录（CollectAll 首发布局；未知动作拒绝，err 带动作值）
inline bool SerializeSegmentV3(const ActionSpec& seg, u64* slots, std::string& err)
{
    if (slots == nullptr) {
        err = "SerializeSegmentV3: null slots";
        return false;
    }
    for (u32 i = 0; i < UNIFIED_V3_SLOTS_PER_RECORD; ++i) {
        slots[i] = 0;   // 未用槽填 0
    }
    slots[U_V3_ACTION_ID] = static_cast<u64>(seg.kind);   // 槽 0 = 动作 ID（== ActionKind 值）
    switch (seg.kind) {
        case ActionKind::CollectAll: {
            // 槽 1..14 布局 == v2 AG Mesh（U_V3_* 与 KFC_UNIFIED_COLLECT_ALL_* /
            // KFC_CONCURRENT_AG_MESH_* 槽 1..14 逐槽相等——server 复用同一读取序）
            const u32 me = seg.domain.MyAlgRank();
            const u32 n = seg.domain.Size();
            // 尾片修正同存量写入方（ccu_temp_kfc_all_gather_mesh_1D_mem2mem.cc:70-74）：
            // 本 rank 是末 rank 且有尾片 → 本轮搬运字节取尾片
            const u64 mySlice = (seg.layout.tailSize != 0 && me == n - 1U) ? seg.layout.tailSize
                                                                           : seg.layout.sliceSize;
            slots[U_V3_INPUT] = seg.layout.inBaseOff;             // input 基址（段内 offset）
            slots[U_V3_OUTPUT] = seg.layout.outBaseOff;           // output 基址（段内 offset）
            // 本 rank 片落位偏移：**纯 rank 偏移（stride*me），不含 outBaseOff**——
            // kernel 侧 dst.addr = output[rankId] + offset（基址槽 + 偏移槽），v2 写入方
            // 槽 3 = rankId*stride（ccu_temp_kfc_all_gather_mesh_1D_mem2mem.cc:68 同式）。
            // 注意不能用 DataLayout::OutOffset(me)（其含 outBaseOff，会双计基址）
            slots[U_V3_OUTPUT_OFFSET] = seg.layout.outputSliceStride * me;
            slots[U_V3_SLICE_SIZE] = mySlice;                     // 本 rank 片字节
            // goSize：存量 AG M2M 写入方 config（LoopSplitter 公式 + v2 写入方常数）
            LoopSplitter::LoopGroupConfig cfg;
            cfg.msInterleave = 8U;       // CCU_MS_INTERLEAVE（ccu_kernel_alg_base.h:27）
            cfg.loopCount = 8U;          // CCU_MS_LOCAL_COPY_LOOP_COUNT（:32）
            cfg.memSlice = 32768ULL;     // CCU_MS_SIZE * LOCAL_COPY_MS_PER_LOOP = 4096*8（:29/:31）
            const LoopSplitter::GoSize go = LoopSplitter::Calc(mySlice, cfg);
            slots[U_V3_GO_SIZE_0] = go.addrOffset;
            slots[U_V3_GO_SIZE_1] = go.loopParam;
            slots[U_V3_GO_SIZE_2] = go.parallelParam;
            slots[U_V3_GO_SIZE_3] = go.residual;
            slots[U_V3_CUR_RANK_SLICE_INPUT_OFFSET] = seg.layout.inputSliceStride * me;
            // repeatNumInv：存量式 UINT64_MAX - repeatNum（ccu_temp_kfc_all_gather_mesh_1D_
            // mem2mem.cc:91；kernel 侧 CCU_WHILE(tmpRepeatNum != UINT64_MAX) 终止
            // （ccu_kernel_kfc_all_gather_mesh1d_mem2mem.cc:231），repeatNum=1 →
            // 0xFFFFFFFFFFFFFFFE——注意不是 UINT64_MAX-(repeatNum-1)，那是零次执行）
            slots[U_V3_REPEAT_NUM_INV] = ~0ULL - seg.layout.repeatNum;
            slots[U_V3_INPUT_REPEAT_STRIDE] = seg.layout.inputRepeatStride;
            slots[U_V3_OUTPUT_REPEAT_STRIDE] = seg.layout.outputRepeatStride;
            // 末 rank 片（kernel 末 rank 分支用；无尾片时 == 正常片）
            slots[U_V3_LAST_SLICE_SIZE] =
                (seg.layout.tailSize != 0) ? seg.layout.tailSize : seg.layout.sliceSize;
            // 就地标记：input 源地址 == output 落位地址（存量写入方同式，:94）
            slots[U_V3_INPUT_OUTPUT_EQUAL] =
                (slots[U_V3_INPUT] + slots[U_V3_CUR_RANK_SLICE_INPUT_OFFSET] ==
                 slots[U_V3_OUTPUT] + slots[U_V3_OUTPUT_OFFSET]) ? 1ULL : 0ULL;
            return true;
        }
        case ActionKind::ReduceSlice: {
            // R6：RS 段 v3 布局。槽 1..15 == v2 并发 RS Mesh（KFC_CONCURRENT_RS_MESH_*
            // 槽 1..15，U_V3_RS_* 逐槽相等——server REDUCE_SCATTER_MESH 分支 :327-339
            // 复用同一读取序）。NHR 拒收（R9 交付物；并发 RS 的 NHR 区间起 16 跨 20
            // 槽超 32 槽记录宽）。仅 MESH/AUTO 受理（AUTO 的 MESH 语义由 BuildStage
            // 的拓扑校验保证——CLOS 域已在 Compile 期被拒）。
            if (seg.pattern == CommPattern::NHR) {
                err = "SerializeSegmentV3: ReduceSlice NHR not in R6 scope (20-slot span at 16 exceeds 32-slot record)";
                return false;
            }
            const u32 me = seg.domain.MyAlgRank();
            const u32 n = seg.domain.Size();
            const u64 mySlice = seg.layout.SliceSizeOf(me, n);
            // chunk 参数经唯一实现点（loop_splitter.h CalcRsChunks——
            // scratchSize/128 对齐/chunkCount 倒数/full+tail 双 goSize 全部同源）
            const RsChunkParams ch = CalcRsChunks(mySlice, n);
            slots[U_V3_RS_INPUT] = seg.layout.inBaseOff;          // input 基址（段内 offset）
            slots[U_V3_RS_OUTPUT] = seg.layout.outBaseOff;        // output 基址（RS 无 per-rank 偏移）
            slots[U_V3_RS_SCRATCH] = seg.layout.scratchBaseOff;   // scratch 中转基址（hcclBuff）
            // rankSliceOffset = 纯 rank 偏移（stride*me，不含 inBaseOff）——kernel 侧
            // myInput/remoteInput.addr = input[rankId] + offset（基址槽+偏移槽分列，
            // 设备侧写入方 :551 同式）
            slots[U_V3_RS_RANK_SLICE_OFFSET] = seg.layout.inputSliceStride * me;
            slots[U_V3_RS_CHUNK_SIZE] = ch.chunkSize;
            slots[U_V3_RS_CHUNK_LOOP_NUM] = ch.chunkLoopNum;      // UINT64_MAX - chunkCount 倒计数
            slots[U_V3_RS_TAIL_SIZE] = ch.tailSize;
            slots[U_V3_RS_FULL_GO_ADDR_OFFSET] = ch.fullGo.addrOffset;
            slots[U_V3_RS_FULL_GO_LOOP_PARAM] = ch.fullGo.loopParam;
            slots[U_V3_RS_FULL_GO_PARALLEL_PARAM] = ch.fullGo.parallelParam;
            slots[U_V3_RS_FULL_GO_RESIDUAL] = ch.fullGo.residual;
            slots[U_V3_RS_TAIL_GO_ADDR_OFFSET] = ch.tailGo.addrOffset;
            slots[U_V3_RS_TAIL_GO_LOOP_PARAM] = ch.tailGo.loopParam;
            slots[U_V3_RS_TAIL_GO_PARALLEL_PARAM] = ch.tailGo.parallelParam;
            slots[U_V3_RS_TAIL_GO_RESIDUAL] = ch.tailGo.residual;
            // repeatNumInv 不占 RS 槽位：v2 并发布局 [1..15] 无此参数（kernel 三阶段
            // 的 repeatNum 单 die 恒 UINT64_MAX-1 硬编码，kernel :395），段级 repeat
            // 由 chunk 循环承载——与 CollectAll 段语义不同，不镜像写槽。
            // dataType/outputType/reduceType 同 v2 不占槽（ctx.arg->opParam；头部
            // [4..6] 是 AICPU 消费方的冗余登记，CCU server 不读头部）
            return true;
        }
        default:
            err = "SerializeSegmentV3: action kind #";
            err += std::to_string(static_cast<int>(seg.kind));
            err += " not in v3 delivery scope";
            return false;
    }
}

// 整 Plan → v3 槽序列（destination 需 ≥ SerializedPlanV3Size(plan) 个 u64 槽）
inline u32 SerializedPlanV3Size(const Plan& plan)
{
    return UNIFIED_V3_HEADER_SLOTS + static_cast<u32>(plan.segments.size()) * UNIFIED_V3_SLOTS_PER_RECORD;
}

inline bool SerializePlanV3(const Plan& plan, std::vector<u64>& destination, std::string& err)
{
    if (plan.segments.size() > UNIFIED_V3_MAX_SEGMENTS) {
        err = "SerializePlanV3: segment count " + std::to_string(plan.segments.size()) +
              " exceeds queue capacity " + std::to_string(UNIFIED_V3_MAX_SEGMENTS);
        return false;
    }
    destination.assign(static_cast<size_t>(SerializedPlanV3Size(plan)), 0);
    destination[0] = PLAN_V3_MAGIC;
    destination[1] = PLAN_V3_VERSION_FIELD;
    destination[2] = static_cast<u64>(plan.segments.size());
    destination[UNIFIED_V3_HEADER_RESERVED] = 0;   // 槽 3 保留（既有语义不动）
    // 头部三槽（R6/裁决一；dataType=0 两口径终裁后降级）：ReduceSpec 冗余登记，
    // 取首个 reduce 段的语义（无 reduce 段的 Plan——如纯 AG——填 0）。
    // ★**纯维测 dump 语义（R6 尾款终裁）**：AICPU 消费方一律以 exec 期 msg 现值为
    //   唯一权威源（方向 B：count/dataType 等数据真值都在 exec 期 msg 注入），头部
    //   三槽仅日志/trace 用，**不作语义判定输入**——原"消费方按段级 ActionSpec 的
    //   reduce 有效性解读、头部重建缺省 ReduceSpec"口径作废（编译期值与 msg 值在
    //   "RS 段且 dataType 真=INT8(0)"场景不可区分，头部权威语义 = 同 Plan 两个真值
    //   源的结构性矛盾）。RS 段的 ReduceSpec 消费路径在段级 ActionSpec（CCU 分支
    //   ctx.arg->opParam / AICPU 消费方 exec 期 msg），头部值不参与。
    // ★不变量"同 Plan 内多 RS 段的 ReduceSpec 恒同"（R6 审查方 MINOR-B3-1 论证）：
    //   一个 OpParam 编译出一个 Plan（Compile 单入口），Plan 的每段 reduce 均拷贝自
    //   同一 PrimitiveRequest.reduce（recipe_sole.h BuildStage 的 act.reduce = req.reduce
    //   ——段间无独立 reduce 来源），故段间恒同、取首段即取全体（dump 值的完备性
    //   由该不变量保证）。该不变量依赖"一 OpParam 一 Plan"的编译模型——未来多段
    //   配方若引入段间异构 ReduceSpec，头部单值登记连 dump 语义都不完备，届时须
    //   升版（v5）改逐段登记或头部携带异构标记。
    for (const ActionSpec& seg : plan.segments) {
        if (seg.kind == ActionKind::ReduceSlice) {
            destination[UNIFIED_V3_HEADER_DATA_TYPE] = static_cast<u64>(seg.reduce.dataType);
            destination[UNIFIED_V3_HEADER_OUTPUT_TYPE] = static_cast<u64>(seg.reduce.outputType);
            destination[UNIFIED_V3_HEADER_REDUCE_TYPE] = static_cast<u64>(seg.reduce.reduceOp);
            break;
        }
    }
    u64* segBase = destination.data() + UNIFIED_V3_HEADER_SLOTS;
    for (size_t i = 0; i < plan.segments.size(); ++i) {
        if (!SerializeSegmentV3(plan.segments[i], segBase + i * UNIFIED_V3_SLOTS_PER_RECORD, err)) {
            return false;
        }
    }
    return true;
}

} // namespace mc2_ops_hccl::refactor

#endif
