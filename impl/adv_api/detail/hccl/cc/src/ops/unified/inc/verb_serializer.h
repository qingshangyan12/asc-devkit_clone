/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * CANN Open Software License Agreement Version 2.0.
 *
 * R12 · WP-2：动词序列化器（v5 blob）——L4 Emit 的 VerbCall 序 → 动词帧序列
 *
 * 消费链（设备边界机制重建，M0 §一/§五）：L4 后端 Emit 产 VerbCall 序（既有机制，
 * backend_ccu_collect_all.h / backend_ccu_reduce_slice.h）→ 本序列化器编 v5 帧序列
 * （32 槽/帧，槽 0=动词 ID）→ device 写入方 CcuPrepareForUnifiedVerbPlan 落盘 →
 * server DispatchUnifiedVerbs 按 verb ID → 原语组合。v3/SerializePlanV3 零改动
 *（灰度分叉，unified_route.h 同款双开关）。
 *
 * 帧布局契约 = devkit 侧 common/kfc_server_message.h 的 v5 段（KFC_UNIFIED_VERB_* /
 * KfcUnifiedVerbSlot / KFC_VERB_XN_*——两侧互不 include，本文件镜像常量 + 测试
 * static_assert 三方互锁，"三处同名登记"防御）：
 *   槽 0=动词 ID（== VerbCall::Verb 枚举值）；数据帧槽 1=src 槽 2=dst 槽 3=size；
 *   BROADCAST 槽 4=remoteDstOff + 槽 5..8 goSize；REDUCE 槽 5..8 goSize；
 *   SYNC_PRE 槽 1=XN 掩码 槽 2..4=本端基址；SYNC_POST 槽 1=POST 掩码；未用槽清 0。
 *
 * ★通道隐式映射（R12-1 裁决① + R12-4 单源派生）：数据帧不携 channelIdx——段内
 * READ/WRITE 帧按**通道序**隐式消费（server 游标随数据帧推进）。序列化器数据帧的
 * peer 迭代序 = claim.channels 注册序（ChannelReq 序，不做二次 sort——与注册链
 * BuildUnifiedChannelDescs → HcclChannelAcquire → ctx.arg->channels 的序保持同源，
 * 序列化器只重放同一数组的序）。Emit 侧 VerbCall.peer 字段保留（trace/AICPU 侧消费），
 * v5 序列化忽略 peer 字段、按 peers 数组原序产帧。
 *
 * ★RS 单 chunk 限界（R12-1 裁决④）：chunkCount>1 的 RS 段拒收（err 上抛 → 灰度
 * 回退 v3 路径——v3+老 kernel chunk 循环是 R7 既有能力，无能力回归）。判据 =
 * CalcRsChunks 的 chunkLoopNum 恢复 chunkCount（loop_splitter.h:168-182 同式）。
 *
 * ★编排结构数据化（主会话裁决 R12-1②）：sync 位置、三阶段批次（先全发起再全等——
 * READ 帧天然非阻塞，等待点合并到批次末的 flush 语义由 server 承载，帧不显式带
 * WAIT 帧）、段间交替全部进帧序；微码侧只留原语调用。repeat/unroll 挂账 R13（帧格式
 * 已预留：版本字段 + 头部保留槽 + 未用槽清 0，不焊死——M0 §四）。
 */
#ifndef REFACTOR_POC_VERB_SERIALIZER_H
#define REFACTOR_POC_VERB_SERIALIZER_H

#include <cstdint>
#include <string>
#include <vector>

#include "action_spec.h"     // Plan / ActionSpec / HcclResult
#include "loop_splitter.h"   // CalcRsChunks / RsGoSizeConfig（RS 限界判据 + goSize）
#include "verb_sink.h"       // VerbCall（Emit 序的元素类型）

namespace mc2_ops_hccl::refactor {

// ---------------------------------------------------------------------------
// devkit 侧 kfc_server_message.h v5 段的镜像常量（两树互不 include——L3 值对象
// 铁律；测试 poc_unified_verb_exec_test.cc 以 static_assert 与 devkit 头探针对数）。
// ---------------------------------------------------------------------------
// 第一级动词 ID（== VerbCall::Verb 枚举值 0..8，构造性对齐：下方 static_assert）
constexpr u32 UNIFIED_V5_VERB_ID_MAX = 8U;
// 帧宽与上限（== CCU_PARAM_NUM_PER_DIE=32 / CCU_TASK_NUM_MAX-1=63）
constexpr u32 UNIFIED_V5_SLOTS_PER_FRAME = 32U;
constexpr u32 UNIFIED_V5_MAX_FRAMES = 63U;
// 头部 7 槽（镜像 v4 头布局）
constexpr u32 UNIFIED_V5_HEADER_SLOTS = 7U;
constexpr u32 UNIFIED_V5_HEADER_RESERVED = 3U;
constexpr u32 UNIFIED_V5_HEADER_DATA_TYPE = 4U;
constexpr u32 UNIFIED_V5_HEADER_OUTPUT_TYPE = 5U;
constexpr u32 UNIFIED_V5_HEADER_REDUCE_TYPE = 6U;
constexpr u64 PLAN_V5_MAGIC = 0x55504C35ULL;   // "UPL5"（v3 是 "UPL3"——灰度分叉判据之一）
constexpr u64 PLAN_V5_VERSION_FIELD = 5ULL;

// v5 帧槽位镜像（数据帧 src/dst/size = 1/2/3；BROADCAST aux = 4；goSize = 5..8；
// 槽 31 = 段 pattern 载体——R13/M0_R13 §五，仅 SYNC_PRE 帧有效）
enum UnifiedV5Slot : u32 {
    U_V5_VERB_ID = 0,
    U_V5_ARG0 = 1,
    U_V5_ARG1 = 2,
    U_V5_ARG2 = 3,
    U_V5_ARG3 = 4,
    U_V5_GO_SIZE_0 = 5,
    U_V5_GO_SIZE_1 = 6,
    U_V5_GO_SIZE_2 = 7,
    U_V5_GO_SIZE_3 = 8,
    U_V5_SLOT_PATTERN = 31,
};

// XN 统一变量表镜像（M0 §二；与 devkit KFC_VERB_XN_* 互锁）
constexpr u32 V5_XN_IN = 0U;
constexpr u32 V5_XN_SCR = 1U;
constexpr u32 V5_XN_TOK = 2U;
constexpr u32 V5_XN_OUT = 3U;
constexpr u32 V5_XN_POST = 4U;
constexpr u32 V5_XN_MASK_AG_PRE = (1U << V5_XN_OUT) | (1U << V5_XN_TOK);           // 0xC
constexpr u32 V5_XN_MASK_RS_PRE = (1U << V5_XN_IN) | (1U << V5_XN_SCR) | (1U << V5_XN_TOK);  // 0x7
constexpr u32 V5_XN_MASK_POST = (1U << V5_XN_POST);                                 // 0x10
// R13：NHR 段 SYNC_PRE 掩码（M0_R13 §五；devkit KFC_VERB_XN_MASK_NHR_PRE 互锁）
constexpr u32 V5_XN_MASK_NHR_PRE = (1U << V5_XN_IN) | (1U << V5_XN_TOK);            // 0x5
// R13：段 pattern 值（M0_R13 §五——SYNC_PRE 帧槽 31；MESH=0 缺省零变化）
constexpr u32 V5_PATTERN_MESH = 0U;
constexpr u32 V5_PATTERN_NHR = 1U;
// R18 · #11：展开动词帧槽区（M0_R18 §四定稿——SLICE_REDUCE 帧带索引+双侧
// peer+种子段级补写；CH_SYNC 帧四态 dir；两动词不计 dataFrames（隐式映射只
// 索引 READ/WRITE 不变）；镜像常量 KFC_VERB_SR_*/KFC_VERB_CS_*（devkit
// kfc_server_message.h 纯追加段，static_assert 三方互锁同步）
enum UnifiedV5SliceReduceSlot : u32 {
    U_V5_SR_STEP = 1,           // 步索引（执行端 NhrSliceOf 入参）
    U_V5_SR_SLICE_IDX = 2,      // slice 索引（同上）
    U_V5_SR_SIZE = 3,           // SliceSizeOf（校验用——与推导值比对，不等即拒收）
    U_V5_SR_PEER_WR = 4,        // WR 半边 peer（algRank——sendTo；执行端换 userRank 查表）
    U_V5_SR_PEER_RD = 5,        // RD 半边 peer（algRank——recvFrom）
    U_V5_SR_INPUT_SLICE_STRIDE = 6,  // step0-skip 的 input 直取步长（种子）
    U_V5_SR_REPEAT_NUM_INV = 7, // repeat 种子（照 NHR_RUN 槽 7 同型）
    U_V5_SR_IN_REPEAT_STRIDE = 8,
    U_V5_SR_OUT_REPEAT_STRIDE = 9,
    U_V5_SR_JETTY_NUM = 10,
};
enum UnifiedV5ChSyncSlot : u32 {
    U_V5_CS_PEER = 1,           // peer（algRank——方向选定后执行端换算）
    U_V5_CS_DIR = 2,            // 四态 {0=ACK-Record, 1=ACK-Wait, 2=DATA-Record, 3=DATA-Wait}
    U_V5_CS_IDX = 3,            // NOTIFY_IDX（0=ACK/1=DATA_SIGNAL——与 dir 冗余校验）
};

// R13：NHR_RUN 帧槽位镜像（M0_R13 §八；槽序镜像 devkit KfcVerbNhrRunSlot ——其值
// 又镜像 sole NHR 布局 KFC_RS_SOLE_NHR_*，静态互锁见下）
enum UnifiedV5NhrSlot : u32 {
    U_V5_NHR_INPUT = 1,
    U_V5_NHR_OUTPUT = 2,
    U_V5_NHR_SLICE_SIZE = 3,
    U_V5_NHR_INPUT_SLICE_STRIDE = 4,
    U_V5_NHR_SLICE_ONE_JETTY_SIZE = 5,
    U_V5_NHR_SLICE_LAST_JETTY_SIZE = 6,
    U_V5_NHR_REPEAT_NUM_INV = 7,
    U_V5_NHR_INPUT_REPEAT_STRIDE = 8,
    U_V5_NHR_OUTPUT_REPEAT_STRIDE = 9,
    U_V5_NHR_JETTY_NUM = 10,
    U_V5_NHR_PARAM_NUM = 11,
};

// 三方同名登记之一：unified VerbCall::Verb ↔ 本镜像 ↔ devkit KFC_UNIFIED_VERB_*
//（枚举值即动词 ID——构造性对齐，漂移即编译失败）
static_assert(static_cast<u32>(VerbCall::COPY) == 0U, "VerbCall::COPY must be verb id 0");
static_assert(static_cast<u32>(VerbCall::POST_COPY) == 1U, "VerbCall::POST_COPY must be verb id 1");
static_assert(static_cast<u32>(VerbCall::READ) == 2U, "VerbCall::READ must be verb id 2");
static_assert(static_cast<u32>(VerbCall::WRITE) == 3U, "VerbCall::WRITE must be verb id 3");
static_assert(static_cast<u32>(VerbCall::REDUCE) == 4U, "VerbCall::REDUCE must be verb id 4");
static_assert(static_cast<u32>(VerbCall::BROADCAST) == 5U, "VerbCall::BROADCAST must be verb id 5");
static_assert(static_cast<u32>(VerbCall::SYNC_PEER_PRE) == 6U, "VerbCall::SYNC_PEER_PRE must be verb id 6");
static_assert(static_cast<u32>(VerbCall::SYNC_PEER_POST) == 7U, "VerbCall::SYNC_PEER_POST must be verb id 7");
static_assert(static_cast<u32>(VerbCall::NHR_RUN) == 8U, "VerbCall::NHR_RUN must be verb id 8 (R13)");
static_assert(U_V5_NHR_PARAM_NUM <= UNIFIED_V5_SLOTS_PER_FRAME, "NHR_RUN slots fit the 32-slot frame");

// ---------------------------------------------------------------------------
// 序列化输入：Emit 序的逐段封装（L4 Emit 每段产一段 VerbCall 序；host 组装段逐段
// 捕获后交给本序列化器）。seg 携带 ActionSpec（基址/goSize/RS 限界判据来源）。
// ---------------------------------------------------------------------------
struct VerbSegment {
    VerbCall::Verb actionKind{VerbCall::BROADCAST};  // 段主导动词（AG=BROADCAST/RS=REDUCE）
    ActionSpec seg;                                  // 段值对象（布局/域/reduce）
    std::vector<VerbCall> calls;                     // Emit 序（后端 Emit 产物）
};

// ---------------------------------------------------------------------------
// 逐动词帧编码（32 槽；未用槽清 0——#17 后 32 槽登记：v5 维持 32 槽帧宽，物理帧
// 的后 32 槽不适用（server 只 Load 前 32，M0 §六-3））
// ---------------------------------------------------------------------------
inline void EncodeVerbFrame(const VerbCall& c, u64* slots)
{
    for (u32 i = 0; i < UNIFIED_V5_SLOTS_PER_FRAME; ++i) {
        slots[i] = 0;
    }
    slots[U_V5_VERB_ID] = static_cast<u64>(c.verb);
    slots[U_V5_ARG0] = c.src;
    slots[U_V5_ARG1] = c.dst;
    slots[U_V5_ARG2] = c.size;
    slots[U_V5_ARG3] = 0;   // BROADCAST 的 remoteDstOff 由 EncodeSegment 段级补写
    // R18 · #11：展开动词帧（M0_R18 §四槽布局——索引+双侧 peer 进帧，几何种子
    // 段级补写（与 NHR_RUN 九标量同机制）；通用槽 src/dst/size 复用承载
    // CH_SYNC 的 {WR peer, RD peer, dir 四态}）
    if (c.verb == VerbCall::SLICE_REDUCE && c.nhr.valid) {
        slots[U_V5_SR_STEP] = c.nhr.stepIdx;
        slots[U_V5_SR_SLICE_IDX] = c.nhr.sliceIdx;
        slots[U_V5_SR_SIZE] = c.size;
        slots[U_V5_SR_PEER_WR] = c.src;   // WR peer（sendTo——Emit 便捷件经通用槽携带）
        slots[U_V5_SR_PEER_RD] = c.dst;   // RD peer（recvFrom）
    } else if (c.verb == VerbCall::CH_SYNC && c.nhr.valid) {
        slots[U_V5_CS_PEER] = c.src;      // WR 侧 peer（方向选定后执行端换算）
        slots[U_V5_CS_DIR] = c.size;      // 四态 {0=ACK-Rec,1=ACK-Wait,2=DATA-Rec,3=DATA-Wait}
        slots[U_V5_CS_IDX] = (c.size == 0ULL || c.size == 1ULL) ? 0ULL : 1ULL;   // NOTIFY_IDX 冗余校验
    }
}

// ---------------------------------------------------------------------------
// 段级帧序编码：VerbSegment → 帧序列（追加到 frames；段内逐帧 32 槽）
//
// 帧序规则（M0 §一布局表 + R12-3②形态）：
//   AG 段（Emit 序 SYNC_PRE → BROADCAST → SYNC_POST）：
//     SYNC_PRE 帧掩码 OUT|TOK=0xC + 本端 OUT 基址（TOK 恒 ctx.token 不占槽）
//     BROADCAST 帧：src=InOffset(me)、localDst=OutOffset(me)、size=SliceSizeOf(me,n)、
//                   remoteDst=OutOffset(me)（对端 output 里我的片）、goSize 四元组
//     SYNC_POST 帧掩码 POST 位
//   RS 段（Emit 序 SYNC_PRE → [>8P COPY] → READ×(N-1) → REDUCE → SYNC_POST）：
//     SYNC_PRE 帧掩码 IN|SCR|TOK=0x7 + 本端 IN/SCR 基址
//     COPY 帧（>8P）：src=InOffset(me)、dst=ScratchOffset(me,n)、size=mySlice
//     READ 帧×(N-1)：peers 序（== BuildArgs 的遍历序 == 通道注册序，单源派生）、
//                    src/ dst=ScratchOffset(peer,n)、size
//     REDUCE 帧：src=scratch 基址、dst=output、size=mySlice、goSize 四元组（RS config）
//   ★RS 单 chunk 限界：chunkCount>1 拒收（err 上抛，M0 §六-2）
//   帧数守恒：段帧数 == calls.size()（逐动词一帧，Emit 序即帧序——编排结构数据化）
// ---------------------------------------------------------------------------
inline bool SerializeVerbSegment(const VerbSegment& vs, std::vector<u64>& frames, std::string& err)
{
    const ActionSpec& a = vs.seg;
    const u32 me = a.domain.MyAlgRank();
    const u32 n = a.domain.Size();

    // R13 · NHR 段分支（M0_R13 §八）：pattern==NHR 的 ReduceSlice 段走 NHR_RUN 帧布局
    //（槽序镜像 sole NHR——KfcVerbNhrRunSlot），跳过 mesh 的 RS 单 chunk 限界（NHR
    // 无 chunk 概念——步进流由 step/slice/jetty 循环承载，server 帧内展开）。
    // 动词序形态：{SYNC_PRE, NHR_RUN} 或 {SYNC_PRE, NHR_RUN, SYNC_POST}——SYNC_POST
    // 随段 sync.postBarrier（A 交错批内段 post=false：两帧形态，合取屏障由批边界
    // 段的 SYNC_POST 承载；B 步进/单段形态恒三帧）。
    if (a.pattern == CommPattern::NHR) {
        // R18 · #11：展开形态先判（动词序含 SLICE_REDUCE——动词级判据）。展开
        // 形态走下方通用帧序（逐动词一帧 + 段级种子补写），不走 NHR_RUN 紧凑帧
        bool expandedForm = false;
        for (const VerbCall& c : vs.calls) {
            if (c.verb == VerbCall::SLICE_REDUCE) {
                expandedForm = true;
                break;
            }
        }
        if (expandedForm) {
            // 展开形态：通用逐动词帧 + 段级补写两处——①SYNC_PRE 掩码/基址（NHR
            // 口径 0x5=IN|TOK + 槽 31 pattern 声明载体——紧凑帧分支删除后由本分支
            // 承载，server VerbSyncPre 掩码分派入口依赖 0x5）；②repeat 种子
            //（照 NHR_RUN 槽 7-10 同型——首 SLICE_REDUCE 帧写入；peer 索引已在
            // EncodeVerbFrame 进帧）
            const size_t base = frames.size();
            frames.resize(frames.size() + vs.calls.size() * UNIFIED_V5_SLOTS_PER_FRAME, 0);
            for (size_t k = 0; k < vs.calls.size(); ++k) {
                EncodeVerbFrame(vs.calls[k], frames.data() + (base + k * UNIFIED_V5_SLOTS_PER_FRAME));
            }
            bool seedWritten = false;
            for (size_t k = 0; k < vs.calls.size(); ++k) {
                u64* f = frames.data() + (base + k * UNIFIED_V5_SLOTS_PER_FRAME);
                const VerbCall& c = vs.calls[k];
                if (c.verb == VerbCall::SYNC_PEER_PRE) {
                    f[U_V5_ARG0] = static_cast<u64>(V5_XN_MASK_NHR_PRE);   // 0x5 = IN|TOK
                    f[U_V5_ARG1] = a.layout.inBaseOff;   // IN 基址广播（NHR 写侧组址源）
                    f[U_V5_ARG2] = a.layout.scratchBaseOff;   // 三槽缓存口径（R12-9，掩码不含无害）
                    f[U_V5_ARG3] = a.layout.outBaseOff;
                    f[U_V5_SLOT_PATTERN] = static_cast<u64>(V5_PATTERN_NHR);
                } else if (c.verb == VerbCall::SYNC_PEER_POST) {
                    f[U_V5_ARG0] = static_cast<u64>(V5_XN_MASK_POST);      // 0x10 = POST 位
                } else if (c.verb == VerbCall::SLICE_REDUCE && c.nhr.valid && !seedWritten) {
                    f[U_V5_SR_REPEAT_NUM_INV] = ~0ULL - c.nhr.repeatNum;
                    f[U_V5_SR_IN_REPEAT_STRIDE] = c.nhr.inputRepeatStride;
                    f[U_V5_SR_OUT_REPEAT_STRIDE] = c.nhr.outputRepeatStride;
                    f[U_V5_SR_JETTY_NUM] = a.nhrJettyNum;
                    f[U_V5_SR_INPUT_SLICE_STRIDE] = c.nhr.inputSliceStride;
                    seedWritten = true;   // 种子补写一次（首 SR 帧承载）；循环继续——
                                         // 尾部 SYNC_POST 掩码仍须补（R18 全删轮修复：
                                         // 原 break 跳出整循环致 POST 掩码漏补）
                }
            }
            return true;
        }
        // R18 · #11 全删轮（用户裁定 2026-10-14）：NHR_RUN 紧凑帧分支（三帧布局
        // 槽序镜像 sole NHR）删除——NHR 段唯一帧形态 = 上方展开分支（逐动词一帧）。
        // 无 SLICE_REDUCE 的 NHR 段（宏动词序——已无生产者）落到此：诚实报错
        err = "SerializeVerbSegment: NHR segment without SLICE_REDUCE verbs "
              "(NHR_RUN macro form removed R18; expanded form expected)";
        return false;
    }

    // RS 单 chunk 限界（chunkCount 从 chunkLoopNum 倒数恢复——CalcRsChunks 同式）
    if (vs.actionKind == VerbCall::REDUCE) {
        const u64 mySlice = a.layout.SliceSizeOf(me, n);
        const RsChunkParams ch = CalcRsChunks(mySlice, n);
        const u64 chunkCount = ~0ULL - ch.chunkLoopNum;
        if (chunkCount > 1ULL) {
            err = "SerializeVerbSegment: ReduceSlice chunkCount " + std::to_string(chunkCount) +
                  " > 1 not in v5 verb scope (multi-chunk stays on v3+kernel loop; P2 frame format)";
            return false;
        }
    }

    // Emit 序逐动词一帧（帧数守恒；peer 字段不进帧——通道隐式映射按序消费）
    const size_t base = frames.size();
    frames.resize(frames.size() + vs.calls.size() * UNIFIED_V5_SLOTS_PER_FRAME, 0);
    for (size_t k = 0; k < vs.calls.size(); ++k) {
        EncodeVerbFrame(vs.calls[k], frames.data() + (base + k * UNIFIED_V5_SLOTS_PER_FRAME));
    }

    // 段级槽位补写（SYNC 掩码/基址、BROADCAST/REDUCE 的 goSize 与扇出槽）
    for (size_t k = 0; k < vs.calls.size(); ++k) {
        u64* f = frames.data() + (base + k * UNIFIED_V5_SLOTS_PER_FRAME);
        const VerbCall& c = vs.calls[k];
        if (c.verb == VerbCall::SYNC_PEER_PRE) {
            if (vs.actionKind == VerbCall::BROADCAST) {
                // AG 段：OUT|TOK（本端 output 基址进槽 4；token 恒 ctx.token）
                f[U_V5_ARG0] = static_cast<u64>(V5_XN_MASK_AG_PRE);
                f[U_V5_ARG3] = a.layout.outBaseOff;
            } else {
                // RS 段：IN|SCR|TOK（本端 input/scratch 基址）
                f[U_V5_ARG0] = static_cast<u64>(V5_XN_MASK_RS_PRE);
                f[U_V5_ARG1] = a.layout.inBaseOff;
                f[U_V5_ARG2] = a.layout.scratchBaseOff;
            }
        } else if (c.verb == VerbCall::SYNC_PEER_POST) {
            f[U_V5_ARG0] = static_cast<u64>(V5_XN_MASK_POST);
        } else if (c.verb == VerbCall::BROADCAST) {
            // 扇出槽（remoteDst = 对端 output 里我的片 = OutOffset(me) 的结构量）+
            // goSize（AG M2M 写入方 config {8, 8, 32768}，plan_serializer.h v3 同源）
            f[U_V5_ARG3] = a.layout.OutOffset(me);
            LoopSplitter::LoopGroupConfig cfg;
            cfg.msInterleave = 8U;
            cfg.loopCount = 8U;
            cfg.memSlice = 32768ULL;
            const LoopSplitter::GoSize go = LoopSplitter::Calc(a.layout.SliceSizeOf(me, n), cfg);
            f[U_V5_GO_SIZE_0] = go.addrOffset;
            f[U_V5_GO_SIZE_1] = go.loopParam;
            f[U_V5_GO_SIZE_2] = go.parallelParam;
            f[U_V5_GO_SIZE_3] = go.residual;
        } else if (c.verb == VerbCall::REDUCE) {
            // RS goSize（RS config {8, 16, 4096}——与 AG 分开声明，裁决五；≤8P 的
            // GroupLocalReduce 消费，>8P Pairwise 由 server 按 rankSize 分叉不消费）
            const u64 mySlice = a.layout.SliceSizeOf(me, n);
            const RsChunkParams ch = CalcRsChunks(mySlice, n);
            f[U_V5_GO_SIZE_0] = ch.fullGo.addrOffset;
            f[U_V5_GO_SIZE_1] = ch.fullGo.loopParam;
            f[U_V5_GO_SIZE_2] = ch.fullGo.parallelParam;
            f[U_V5_GO_SIZE_3] = ch.fullGo.residual;
        }
    }
    return true;
}

// ---------------------------------------------------------------------------
// 整 Plan → v5 blob（头部 7 槽 + frameCount×32）
//   头部：[0]=UPL5 [1]=版本 5 [2]=frameCount [3]=保留 [4..6]=ReduceSpec dump
//   （R6 尾款终裁：纯维测 dump，不作语义判定输入——server 不读头部三槽）
//   帧数上限 63（环 64 − 1 哨兵；哨兵帧由 device 写入方按队列协议追加，不在本
//   序列化范围——v3 同契约）
// ---------------------------------------------------------------------------
inline u32 SerializedVerbPlanSize(const std::vector<VerbSegment>& segs)
{
    u32 frames = 0;
    for (const VerbSegment& vs : segs) {
        frames += static_cast<u32>(vs.calls.size());
    }
    return UNIFIED_V5_HEADER_SLOTS + frames * UNIFIED_V5_SLOTS_PER_FRAME;
}

inline bool SerializeVerbPlan(const std::vector<VerbSegment>& segs, const Plan& plan,
                              std::vector<u64>& destination, std::string& err)
{
    u64 frameCount = 0;
    for (const VerbSegment& vs : segs) {
        frameCount += static_cast<u64>(vs.calls.size());
    }
    if (frameCount == 0ULL) {
        err = "SerializeVerbPlan: empty verb plan (no executable frame)";
        return false;   // 与写入方 segNum==0 拒收同口径（M4-④：静默成功比报错危险）
    }
    if (frameCount > UNIFIED_V5_MAX_FRAMES) {
        err = "SerializeVerbPlan: frame count " + std::to_string(frameCount) +
              " exceeds queue capacity " + std::to_string(UNIFIED_V5_MAX_FRAMES) +
              " (multi-ring P2; A/B tiered out of R12 verb scope)";
        return false;
    }
    destination.assign(static_cast<size_t>(SerializedVerbPlanSize(segs)), 0);
    destination[0] = PLAN_V5_MAGIC;
    destination[1] = PLAN_V5_VERSION_FIELD;
    destination[2] = frameCount;
    destination[UNIFIED_V5_HEADER_RESERVED] = 0;
    // 头部三槽 ReduceSpec dump：取首个 reduce 段（无则 0）——v3 同式
    for (const ActionSpec& seg : plan.segments) {
        if (seg.kind == ActionKind::ReduceSlice) {
            destination[UNIFIED_V5_HEADER_DATA_TYPE] = static_cast<u64>(seg.reduce.dataType);
            destination[UNIFIED_V5_HEADER_OUTPUT_TYPE] = static_cast<u64>(seg.reduce.outputType);
            destination[UNIFIED_V5_HEADER_REDUCE_TYPE] = static_cast<u64>(seg.reduce.reduceOp);
            break;
        }
    }
    std::vector<u64> frames;
    frames.reserve(static_cast<size_t>(frameCount) * UNIFIED_V5_SLOTS_PER_FRAME);
    for (const VerbSegment& vs : segs) {
        if (!SerializeVerbSegment(vs, frames, err)) {
            return false;
        }
    }
    for (size_t i = 0; i < frames.size(); ++i) {
        destination[UNIFIED_V5_HEADER_SLOTS + i] = frames[i];
    }
    return true;
}

// ---------------------------------------------------------------------------
// host Validate（R12-1 裁决① + R12-4）：通道隐式映射的配对正确性防线——
//   ① 段内数据帧（READ/WRITE）数 == channelCount（== N-1，claim.channels 之和的
//      每 rank 条数口径）
//   ② 序列化器迭代序 == 注册通道序（同输入双推导：数据帧 peer 序 == Emit peers 序
//      == BuildArgs 遍历序 == claim.channels 注册序——非 sort 后的序）
//   ③ verbs 白名单（段内动词 ⊆ {SYNC_PRE,SYNC_POST,COPY,READ,WRITE,REDUCE,BROADCAST}）
// ---------------------------------------------------------------------------
inline bool ValidateVerbSegment(const VerbSegment& vs, std::string& err)
{
    const u32 n = vs.seg.domain.Size();
    const u32 channelCount = (n > 1U) ? n - 1U : 0U;
    // R13 · NHR 段分支（M0_R13 §二/§八）：NHR 段动词序 = {SYNC_PRE, NHR_RUN, SYNC_POST}
    // 恒三帧、零数据帧（READ/WRITE）——mesh 的隐式映射两断言（帧数==通道数/迭代序）
    // 不适用，改验：①动词序恰三帧且中位 NHR_RUN；②jettyNum∈[1,4]；③SYNC_PRE 帧
    // 槽 31 pattern==NHR（segment-level 声明性载体与 pattern 字段一致）。
    if (vs.seg.pattern == CommPattern::NHR) {
        // R18 · #11：展开形态先判（动词级——含 SLICE_REDUCE 即展开）。帧序规则：
        // ①上料 COPY×n 在首 CH_SYNC 前、归位 COPY×1 在末 CH_SYNC 后（序位——
        // #28 纪律：值域判据在 tier 半片形态有歧义）；②每步四条 CH_SYNC
        // （ACK-Rec/ACK-Wait 夹 SLICE_REDUCE×nSlices，DATA-Rec/ DATA-Wait 收尾）
        // ——步闭包形态；③SLICE_REDUCE 的 stepIdx 单调不减；④两动词不计
        // dataFrames（隐式映射只索引 READ/WRITE——下方通用校验不触发，天然满足）。
        bool expandedForm = false;
        for (const VerbCall& c : vs.calls) {
            if (c.verb == VerbCall::SLICE_REDUCE) {
                expandedForm = true;
                break;
            }
        }
        if (expandedForm) {
            u32 upCount = 0;
            bool seenChSync = false;
            const VerbCall* backCopy = nullptr;
            u32 lastStep = 0;
            bool firstSlice = true;
            bool hasCopy = false;
            for (const VerbCall& c : vs.calls) {
                switch (c.verb) {
                    case VerbCall::COPY:
                        hasCopy = true;
                        if (!seenChSync) {
                            ++upCount;
                        } else if (backCopy == nullptr) {
                            backCopy = &c;
                        }
                        break;
                    case VerbCall::CH_SYNC:
                        seenChSync = true;
                        if (c.size > 3ULL) {
                            err = "ValidateVerbSegment: CH_SYNC dir out of {0..3}";
                            return false;
                        }
                        break;
                    case VerbCall::SLICE_REDUCE: {
                        if (!c.nhr.valid) {
                            err = "ValidateVerbSegment: SLICE_REDUCE payload invalid";
                            return false;
                        }
                        if (firstSlice || c.nhr.stepIdx >= lastStep) {
                            lastStep = c.nhr.stepIdx;   // 单调不减（同步内 sliceIdx 递增）
                            firstSlice = false;
                        } else {
                            err = "ValidateVerbSegment: SLICE_REDUCE stepIdx not monotonic";
                            return false;
                        }
                        break;
                    }
                    default:
                        break;   // SYNC_PEER_PRE/POST（段级屏障）与展开动词共存合法
                }
            }
            // R18 · #11 全删轮补：上料/归位校验分口径——AICPU 形态含 COPY
            // （上料×n+归位×1）；CCU 形态无 COPY（步进流本体在 server 帧内承载，
            // 动词序仅 SYNC+CH_SYNC+SLICE_REDUCE）。判据动词级（序内有无 COPY，#28）
            if (hasCopy) {
                if (upCount != n) {
                    err = "ValidateVerbSegment: NHR expanded upload COPY count " +
                          std::to_string(upCount) + " != n " + std::to_string(n);
                    return false;
                }
                if (backCopy == nullptr) {
                    err = "ValidateVerbSegment: NHR expanded back-copy missing";
                    return false;
                }
            }
            // jettyNum∈[1,4] 校验（种子路径继承——宏形态分支删除后由展开分支承载；
            // 序列化器把 nhrJettyNum 补写进首 SR 帧，越界值不可进帧）
            if (vs.seg.nhrJettyNum == 0U || vs.seg.nhrJettyNum > 4U) {
                err = "ValidateVerbSegment: NHR jettyNum " + std::to_string(vs.seg.nhrJettyNum) +
                      " out of [1,4] (maxPathNum)";
                return false;
            }
            return true;
        }
        // R18 · #11 全删轮（用户裁定 2026-10-14）：NHR_RUN 三帧宏形态 Validate
        // 分支删除——上方展开分支（动词序含 SLICE_REDUCE）为 NHR 段唯一合法形态；
        // 无 SLICE_REDUCE 的 NHR 段 = 宏动词序（已无生产者）——形态优先报错。
        // jettyNum∈[1,4] 校验保留面：展开分支内部（上方 hasCopy 块之后）。
        err = "ValidateVerbSegment: NHR segment must be expanded form "
              "(contains SLICE_REDUCE verbs; NHR_RUN macro form removed R18)";
        return false;
    }
    u32 dataFrames = 0;
    std::vector<u32> framePeerOrder;
    for (const VerbCall& c : vs.calls) {
        if (c.verb == VerbCall::READ || c.verb == VerbCall::WRITE) {
            ++dataFrames;
            framePeerOrder.push_back(c.peer);
        }
    }
    if (dataFrames != channelCount) {
        err = "ValidateVerbSegment: data frame count " + std::to_string(dataFrames) +
              " != channel count " + std::to_string(channelCount) +
              " (implicit channel mapping broken)";
        return false;
    }
    // 同输入双推导：数据帧 peer 序须与注册通道序一致（R12-9 修复轮 P1-2 裁决：
    // 统一升序——BuildArgs peers 遍历序 = domain.ranks 升序跳过 me = claim.channels
    // 注册序（remoteRank 升序，R11 两遍对齐）；不做 sort 后比较，也不复用 NextPeer
    // 环序——环序是 AICPU 线程分配便利序，CCU 通道表恒升序（老 kernel InitResource
    // 按 peerId 升序遍历），四方（序列化器/server 游标/ST 模型/本断言）同一口径）
    if (dataFrames > 0U) {
        const u32 me = vs.seg.domain.MyAlgRank();
        u32 expectIdx = 0;
        for (u32 peerAlgRank = 0; peerAlgRank < n && expectIdx < dataFrames; ++peerAlgRank) {
            if (peerAlgRank == me) {
                continue;   // 升序跳过 me——通道表不含本 rank
            }
            if (framePeerOrder[expectIdx] != vs.seg.domain.ranks[peerAlgRank]) {
                err = "ValidateVerbSegment: data frame peer order drifts from registration "
                      "order at #" + std::to_string(expectIdx) +
                      " (single-source derivation broken; do not re-sort)";
                return false;
            }
            ++expectIdx;
        }
    }
    return true;
}

} // namespace mc2_ops_hccl::refactor

#endif
