/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * CANN Open Software License Agreement Version 2.0.
 *
 * unified PoC · ST 语义 checker（host 侧纯逻辑，不依赖设备/ST 框架）
 *
 * 定位（方案 §6.4.1 / §6.13）：L4 后端 Emit 产出的是动词序列（EmitCtx.trace 字符串，
 * 正式版将替换为结构化 TraceRecord）。本文件把 trace 解析回结构化记录，并提供三层
 * 语义断言，供 ST 用例与 test/run.sh 自检共用：
 *
 *   ① parseTrace       —— trace 字符串 → VerbRecord 序列（动词词汇表的 host 侧镜像）
 *   ② checkSemanticsEquivalent —— AICPU trace 与 CCU trace 的语义等价
 *        （broadcast ≈ copy + read×(N-1)，方案 §6.13.2 对照表；屏障结构一致）
 *   ③ checkPlanBook    —— Plan 计划书断言（段数/kind/pattern/shape、数据流边完整性）
 *   ④ checkRounds      —— 切轮数学断言（WP1 接口：Σ sliceSize == total、baseOff 单调、
 *                          尾轮 ≤ maxPerRound）
 *   ⑤ simulateKfcServerV3 —— CCU KFC server 轮级循环的 host 模拟器（A4 甲路线，§12-11）：
 *        按 KfcServerGenCircularQueue 语义逐 32 槽记录解读 v3 序列化产物
 *        （槽 0=动作 ID 分派、INVALID_U64 出口哨兵、环形容量上限）；R7（2026-10-06）
 *        起 REDUCE_SLICE 帧额外产出 kernel 实参记录（RsKernelArgsRecord——按 server
 *        v3 分支的转发序抽取 15 槽 + ctx 派生字段，供"server 将来读到的"侧断言）
 *
 * 与 SimWorld（tests/.../utils/src/sim_world）的关系：SimWorld 捕获的是真实 template
 * 的 TaskStub 流（AICPU 路径），本 checker 工作在重构侧动词层——两层在 P3 AICPU 后端
 * 落地后可以对齐（动词 → TaskStub 的映射即等价性证据）。当前 Emit 是 trace 字符串，
 * 本 checker 是它唯一的结构化消费者。
 *
 * 本文件属 ST agent 地盘（生成 agent 不改）；C++14 干净，编译进 run.sh 第二目标。
 */
#ifndef REFACTOR_POC_ST_SEMANTIC_CHECKER_H
#define REFACTOR_POC_ST_SEMANTIC_CHECKER_H

#include "action_spec.h"

#include <cstdint>
#include <string>
#include <vector>

namespace mc2_ops_hccl::refactor::st {

// ---------------------------------------------------------------------------
// 动词记录：EmitCtx.trace 的结构化镜像（七动词汇表的子集，§6.4.1）
// ---------------------------------------------------------------------------
struct VerbRecord {
    std::string verb;      // copy / read / broadcast / postCopy / syncPeer:pre / syncPeer:post
                           // write / reduce 预留给 P2 ReduceSlice 落地（词汇表枚举位先占，解析先拒）
    u64 a1{0};             // 动词参数（语义随动词：copy=src、read=srcOff、broadcast=outputOff）
    u64 a2{0};             // copy=dst、read=dstOff、broadcast=peer 数
    u64 a3{0};             // copy/read/postCopy=size、broadcast=size
    u32 peer{0};           // read 专属（0=无 peer 维度）
    bool isSyncPre{false}; // syncPeer:pre
    bool isSyncPost{false}; // syncPeer:post
};

// 动词分类（checker 内部用，不改 ActionSpec）
inline bool IsSyncVerb(const VerbRecord& r) { return r.isSyncPre || r.isSyncPost; }
inline bool IsDataVerb(const VerbRecord& r) { return !IsSyncVerb(r); }

// ---------------------------------------------------------------------------
// ① trace 解析
//    格式（与 l4 两后端 Verb() 的拼接格式逐字对应）：
//      copy(src->dst,size) read(src->dst,size,peerN) broadcast(a1,a2,a3)
//      syncPeer:pre(0,0,0) syncPeer:post(0,0,0)
//    解析失败返回空向量并置 err（调用方据此判定 trace 格式漂移——格式变更必须同步本文件）。
// ---------------------------------------------------------------------------
std::vector<VerbRecord> ParseTrace(const std::string& trace, std::string& err);

// ---------------------------------------------------------------------------
// ② 两引擎语义等价断言
//    规则（方案 §6.13.2 AICPU vs CCU 对照表）：
//      · 屏障结构：pre 在首个数据动词前、post 在末个数据动词后（两引擎同构）
//      · 搬运量守恒：CCU broadcast 的搬运量 ≈ AICPU copy+read×(N-1) 的搬运量
//        ——按 sliceSize 口径核算（broadcast(a1,peers,size) 的 size 为本 rank 切片，
//          fan-out 在引擎内部；AICPU 侧 read 每条 a3=size、共 N-1 条，加上本地 copy）
//      · AICPU 的 read 条数 == 域大小-1（MESH 一步直达；NHR 多步时为 Σ 步的搬运，
//        等价核算按"总搬运量 == (N-1)×sliceSize"宽松口径，具体步序由 pattern 专属用例断言）
//      · postCopy（VIA_CCUBUF 的本地中转回拷，§10.2 B4）**不计入守恒**——它是 scratch→output
//        的二段本地搬运（开销动词），不改变语义搬运量；但必须在末个远端数据动词之后
//    返回 0=等价；非 0 时 err 给出第一条不等价规则。
// ---------------------------------------------------------------------------
int CheckSemanticsEquivalent(const std::vector<VerbRecord>& traceAicpu,
                              const std::vector<VerbRecord>& traceCcu,
                              const ActionSpec& seg, u32 domainSize, std::string& err);

// ---------------------------------------------------------------------------
// ②b postCopy 语义断言（§10.2 B4）
//    expectPostCopy=true（VIA_CCUBUF）：至少一条 postCopy，且全部在末个远端数据动词
//    （read/broadcast）之后；expectPostCopy=false（SYMMEM/MEM2MEM）：一条都不能有。
//    真实依据 ins_temp_all_gather_mesh_1D.cc:278-300（scratch 槽→output 回拷，
//    outBuffType==HCCL_BUFFER 时 skip——unified 侧 transport 翻译该判定）。
// ---------------------------------------------------------------------------
int CheckPostCopySemantics(const std::vector<VerbRecord>& trace, bool expectPostCopy, std::string& err);

// ---------------------------------------------------------------------------
// ②c 两段中转衔接断言（§10.2 B2 / §12-9 决策：RS 段 output 落 scratch、AG 段 input 同源）
//    断言口径：RS 段对每 peer 的 scratch 落位 == AG 段对同一 peer 的取数源——
//    即两段 layout 的 ScratchOffset 同基（scratchBaseOff）、同 stride（sliceSize 口径）逐值相等。
//    真实依据 ins_temp_all_reduce_mesh_1D_two_shot.cc:222-250（ScatterData 落
//    hcclBuffBaseOff+idx*size）与 :326-360（GatherData 从同式取）。
//    rs/ag 传段序号仅供 err 文案；断言只看 layout 数学，不看动词（RS 的 Emit 属 P2）。
// ---------------------------------------------------------------------------
int CheckStageHandoff(const ActionSpec& rsSeg, const ActionSpec& agSeg, std::string& err);

// ---------------------------------------------------------------------------
// ③ 计划书断言（Plan 计划书 = 方案 §6.13.2 的 [Expand] 行结构化版本）
// ---------------------------------------------------------------------------
struct SegmentExpect {
    ActionKind kind;
    CommPattern pattern;
    SegShape shape{SegShape::NONE};
};
int CheckPlanBook(const Plan& plan, const std::vector<SegmentExpect>& expect, std::string& err);
// 数据流边完整性（通用协议，§6.3）：每段 inputRanks 非空（无孤儿）；
// 段数>1 时前段 outputRanks == 后段 inputRanks（衔接完整，无缺失）。
// 注意：parallel 配方的段间无数据依赖（各段独立），此时只查无孤儿——由 caller 传
// expectDataflowEdges=false。
int CheckDataflowEdges(const Plan& plan, bool expectDataflowEdges, std::string& err);

// ---------------------------------------------------------------------------
// ④ 切轮断言（WP1 A2 的数学验收，接口按 RoundCapacity 语义预留）
// ---------------------------------------------------------------------------
struct RoundRecord {
    u64 baseOff{0};     // 本轮起始偏移
    u64 sliceSize{0};   // 本轮每 rank 切片
};
int CheckRounds(const std::vector<RoundRecord>& rounds, u64 totalSliceSize, u64 maxPerRound,
                std::string& err);
// Σ rounds.sliceSize == total、baseOff 严格递增、每轮 sliceSize ≤ maxPerRound、
// 尾轮允许 < maxPerRound 且非尾轮应为 maxPerRound（整除时尾轮也 == maxPerRound）。

// ---------------------------------------------------------------------------
// ⑤ KFC server v3 轮级分发模拟器（第四轮 A4 · 甲路线，§12-11 决策）
//
// 语义源（逐行考古，v2 行为基线）：
//   · KfcServerGenCircularQueue（ccu_kernel_kfc_server.cc:351-410）：repeat 循环里
//     LoadFuncParamFromMemory 一次读 32 槽（CCU_PARAM_NUM_PER_DIE=32，:24）→ 槽 0 ==
//     INVALID_U64（UINT64_MAX，:23）置 repeatCond 出口；否则 DispatchKfcSubKernel 按
//     判据分派 → 每轮 paramAddr += CCU_PARAM_NUM_MAX*8 = 512B（64 槽双 die 几何，
//     单 die 只读前 32 槽）→ queueIdx 环形回绕于 CCU_TASK_NUM_MAX=64 轮。
//   · v2 分发判据是**实例级**（ctx.arg->opParam.opType / ctx.arg->role，:181-349 if-else）
//     ——一个 server 实例只能执行一种算法，Plan.segments 多段无法承载；
//   · v3（本轮交付）：槽 0 从 opId 哨兵升为**动作 ID**（role==UNIFIED 实例级分叉后
//     按 ID 轮级分派），协议演进依据 = 槽 0 作哨兵的既有惯例
//     （KFC_CONCURRENT_AG_OP_ID=0 等 kfc_server_message.h 各布局枚举 + server 出口
//     哨兵同槽位 ccu_kernel_kfc_server.cc:389）。
//
// 模拟器只解读**序列化产物**（BuildSegmentSlotsV3 / SerializePlanForCtx v3 的输出），
// 不调用被测方任何解释函数——server 将来在设备上读什么，本函数在 host 上读什么。
// 与 unified L3 BuildArgs 的等价（"server 将来读到的"=="L3 现在算的"）由 caller 用
// 逐值对照断言，不在本模拟器内做。
// ---------------------------------------------------------------------------
// v3 单段 32 槽布局（生成方第四轮 A4 实际交付，inc/plan_serializer.h：
// U_V3_* 槽位镜像 + devkit 侧 kfc_server_message.h 的 KfcUnifiedPlanParamIndex，
// 两处 static_assert 互锁；ST 不复用其枚举——期望值纪律，本注释仅作对照说明）：
//   [0]=动作 ID（==ActionKind 值：CollectAll=0）、[1]=inBaseOff、[2]=outBaseOff、
//   [3]=outputSliceStride*me（纯 rank 偏移，不含基址）、[4]=mySlice（尾 rank 修正）、
//   [5..8]=goSize（LoopSplitter + v2 写入方 config {8,8,32768}）、[9]=inputSliceStride*me、
//   [10]=UINT64_MAX-repeatNum、[11]=inputRepeatStride、[12]=outputRepeatStride、
//   [13]=lastSliceSize、[14]=inOutEqual、[15..31]=0
struct ServerTurnRecord {          // 模拟器逐轮解出的"参数包"（KfcServerContext 视角）
    u64 actionId{0};               // 槽 0（INVALID_U64 以外的合法动作 ID）
    std::vector<u64> slots;        // 本轮 32 槽原始值（含槽 0；未写槽为 0）
    u64 turnIndex{0};              // 第几轮（0 起，环回前语义）
};

struct ServerSimResult {
    std::vector<ServerTurnRecord> turns;   // 逐轮解包产物（哨兵轮不进列表）
    u64 rawTurnCount{0};                   // 含哨兵轮的总轮数（环回前）
    bool wrapped{false};                   // 是否发生 64 轮环形回绕
    std::string err;                       // 非空 = 拒发（server 视角的错误）
};

// ---------------------------------------------------------------------------
// ⑤b RS（REDUCE_SLICE）帧的 kernel 实参记录（R7，2026-10-06）
//
// 字段名逐一对齐 CcuReduceScatterMesh1DMem2MemKernel 的形参名
// （ccu_kernel_kfc_reduce_scatter_mesh1d_mem2mem.cc:414-421，19 参）：
//   inputAddr / outputAddr / tokenInfo / scratch / currentRankSliceInputOffset /
//   chunkSize / chunkLoopNum / tailSize / fullGoAddrOffset / fullGoLoopParam /
//   fullGoParallelParam / fullGoResidual / tailGoAddrOffset / tailGoLoopParam /
//   tailGoParallelParam / tailGoResidual / channels / channelCount / rankSize /
//   rankId / dataType / outputType / reduceType
//（channels 是句柄数组、token 是 server 终生不变量——两者 ST 不可观测，记录槽位
//  来源即可；四 goSize 字段由 15 槽压缩展开为形参名）。
//
// 抽取序 = server v3 REDUCE_SLICE 分支的转发序（ccu_kernel_kfc_server.cc:225-239，
// 逐参对照：槽 1→inputAddr、2→outputAddr、ctx.token→tokenInfo、3→scratch、
// 4→currentRankSliceInputOffset、5→chunkSize、6→chunkLoopNum、7→tailSize、
// 8..11→fullGo 四元组、12..15→tailGo 四元组；channels/channelCount/rankSize/rankId/
// dataType/outputType/reduceType 是 ctx.arg 派生字段，不经消息槽——ST 侧由 caller
// 注入 ctx 侧值（rankSize/rankId 传域事实，dataType/outputType/reduceType 传
// OpParam 投影），记录里只存值不做推导）。
// ---------------------------------------------------------------------------
struct RsKernelArgsRecord {
    bool valid{false};        // 本轮是否为 ReduceSlice 帧（slot0==1）
    // ---- 槽位转发（server 逐参读取序） ----
    u64 inputAddr{0};                    // 槽 1（实例化后 = sendBuf+repeatOffset+inBaseOff）
    u64 outputAddr{0};                   // 槽 2
    u64 scratch{0};                      // 槽 3
    u64 currentRankSliceInputOffset{0};  // 槽 4（rankId×stride 分支产物）
    u64 chunkSize{0};                    // 槽 5
    u64 chunkLoopNum{0};                 // 槽 6（UINT64_MAX-chunkCount 倒计数）
    u64 tailSize{0};                     // 槽 7
    u64 fullGoAddrOffset{0};             // 槽 8
    u64 fullGoLoopParam{0};              // 槽 9
    u64 fullGoParallelParam{0};          // 槽 10
    u64 fullGoResidual{0};               // 槽 11
    u64 tailGoAddrOffset{0};             // 槽 12
    u64 tailGoLoopParam{0};              // 槽 13
    u64 tailGoParallelParam{0};          // 槽 14
    u64 tailGoResidual{0};               // 槽 15
    // ---- ctx 派生字段（不占槽；server 从 KfcServerContext/ctx.arg 取） ----
    u32 rankSize{0};           // ctx.arg->rankSize（caller 注入：域大小）
    u32 rankId{0};             // ctx.arg->rankId（caller 注入：本 rank）
    u32 channelCount{0};       // ctx.arg->channelCount（caller 注入；ST 传占位语义值）
    u64 dataType{0};           // ctx.arg->opParam.DataDes.dataType（caller 注入）
    u64 outputType{0};         // ctx.arg->opParam.DataDes.outputType（caller 注入）
    u64 reduceType{0};         // ctx.arg->opParam.reduceType（caller 注入）
};

// RS 帧消费模拟的 ctx 侧输入（server 真链路从注册链/OpParam 来；ST 由 caller 给值，
// 模拟器只透传不推导——"server 读什么"与"槽值怎么算"两层分离）
struct RsServerCtx {
    u32 rankSize{0};
    u32 rankId{0};
    u32 channelCount{0};
    u64 dataType{0};
    u64 outputType{0};
    u64 reduceType{0};
};

// 对单帧 32 槽执行 server v3 REDUCE_SLICE 分支的转发序抽取（含 ctx 派生字段）。
// 帧槽 0 != ReduceSlice ID（1）时 valid=false（非 RS 帧不产记录——AG 帧走 ⑲ 既有
// 断言路径，本函数不越权）。frame 须含 32 槽（caller 从模拟器 turns[].slots 取）。
RsKernelArgsRecord ExtractRsKernelArgs(const std::vector<u64>& frame, const RsServerCtx& ctx);

// serialized：Plan 逐段 32 槽展平（segments.size() × 32 个 u64；caller 从生成方
// 序列化产物取，或手工构造负例）。合法 actionId 集合由 caller 传入（unified 实际
// 交付的动作 ID 值——模拟器不硬编码 ID 数值，避免与被测方自洽）。
// 返回值：err 非空 = 拒发（段数超上限 / 未知动作 ID / 空载荷）；err 空 = turns 逐轮可用。
ServerSimResult SimulateKfcServerV3(const std::vector<u64>& serialized,
                                    const std::vector<u64>& legalActionIds);

// ---------------------------------------------------------------------------
// ⑤b/⑤c 的配套类型与入口（R7）：ExtractRsKernelArgs / StRsExecMsg /
// StInstantiateRsFrame——RS 帧的 kernel 实参抽取 + exec 现值实例化（host 公式
// 全部从 v2 设备侧源码独立推导，实现声明见 .cc 头注——不转抄生成方测试镜像）。
// ---------------------------------------------------------------------------
// exec 期现值（commParam 投影 + ccuParam_ 半边）——RS 帧实例化的输入。
// 字段面与设备函数消费面一一对应（hccl_ccu_v0_prepare.h:681-699 的现值清单）：
// sendBuf/recvBuf/count/strideCount 来自 commParam（exec 期注入），
// rankId/repeatIndex/scratchAddr/rankNum 来自 ccuParam_（设备侧自持资源），
// dts 由 dataType 派生（GetHcclDataTypeSize——ST 传字节宽数值，不传枚举）。
struct StRsExecMsg {
    u64 sendBuf{0};
    u64 recvBuf{0};
    u64 count{0};                       // RS 语义：每 rank 输出份元素数
    u64 dts{2};                         // dataType 字节宽（caller 按 dataType 给）
    u64 strideCount{0};                 // ≠0 时槽 4 走 stride 分支
    u32 rankId{0};
    u32 repeatIndex{0};
    u64 scratchAddr{0};                 // ccuParam_.scratchAddr
    u32 rankNum{0};                     // ccuParam_.rankNum
};

// RS 帧实例化入口（资源期 blob 帧 → exec 落盘帧）。blobFrame 须 32 槽；RS 帧判据
// （槽 0 == ReduceSlice ID）由 caller 保证——本函数不重判（设备函数判据在帧循环，
// 与单帧覆写分离，双份动作 ID 检查的裁定见 hccl_ccu_v0_prepare.h:719-725 注）。
std::vector<u64> StInstantiateRsFrame(const std::vector<u64>& blobFrame,
                                      const StRsExecMsg& m);

// 合法动作 ID 的 ST 镜像（第五轮主会话裁决后收口：不做 0..4 全集白名单——
// 交付哪个动作的轮次再扩哪个 ID 的断言；R6 扩 ReduceSlice=1）。
// CollectAll=0 / ReduceSlice=1 与 unified ActionKind 的编译期对齐在 st_verification.cc
// R5 ① 以 static_assert 锁死（本头 include action_spec.h，可直接对 ActionKind 断言）。
constexpr u64 ST_KFC_V3_ACTION_COLLECT_ALL = 0;    // == ActionKind::CollectAll
constexpr u64 ST_KFC_V3_ACTION_REDUCE_SLICE = 1;   // == ActionKind::ReduceSlice（R6 交付）

// 便捷判据：unified CollectAll 的 v3 动作 ID 是否在合法集合里（ST 侧 ID 镜像常量，
// 与 unified 实现解耦——若生成方 ID 取值不同，测试里直接传生成方值，本常量仅锚定
// "ID 必须非零且非哨兵"的协议下限）。
constexpr u64 ST_KFC_V3_SENTINEL = 0xFFFFFFFFFFFFFFFFULL;   // INVALID_U64（ccu_kernel_kfc_server.h:23）
constexpr u32 ST_KFC_V3_SLOTS_PER_TURN = 32;                // CCU_PARAM_NUM_PER_DIE
constexpr u32 ST_KFC_V3_MAX_TURNS = 64;                     // CCU_TASK_NUM_MAX（环形队列容量）

// ---------------------------------------------------------------------------
// R10 · F1 断链可判基线（2026-10-08，AG×AICPU 生产链贯通轮）
//
// F1 两断点的 ST 侧可判素材（期望值独立手算，留痕 /tmp/st_r10_expectations.md）：
//   断点①（序列化分叉）：AICPU 引擎的 ctx 旁路产物从 v1 文本摘要
//     （SerializePlanForCtx，action_spec.h:314-330）切换为 v4 blob
//     （SerializePlanV3，plan_serializer.h）——切换前 v1 产物喂消费方入口层
//     （ParseUnifiedBlobV4，unified_exec/aicpu_consumer.h:100）必拒。本组提供
//     "v1 可判负例"的通用判据：v1 恒以 "PL" 开头（"PLAN v"+版本拼接，:316），
//     小端组槽后槽 0 低 16 位恒 0x4C50（'P'=0x50 | 'L'=0x4C<<8）；UPL3 魔数
//     0x55504C33（plan_serializer.h:141）低 16 位 0x4C33——恒不等 → v1 任何产物
//     （任意段数/版本号/长度）在 magic 判据下必拒，与长度无关。另一条死路
//     （对齐闸）由 StCheckCtxSizeGate 覆盖（v1 长度非 8 倍数时消费方
//     unified_aicpu_exec.cc:73 先拒——两闸任一命中，v1 形态消费必死）。
//   断点②（KFC executor 链）：unified 键（kUnifiedAicpuAlgKey）无
//     CollAlgExecRegistryV2 注册项 → GetAlgExec 必 nullptr——分叉判定必须前移到
//     查表之前（kernel_launch.cc:369 既有分叉同型，BLOCKER-1 教训）。本组以判序
//     参数镜像两种形态（修复后/病灶），unified 键判定函数由 caller 注入真入口
//     （unified/exec 的 IsUnifiedAicpuAlgName——REVIEW_CHECKLIST #4：判定入口
//     直接测试，不以字符串形态自证）。
// ---------------------------------------------------------------------------

// v1 文本摘要 → u64 槽视图（消费方 raw + ctxSize/8 的取整口径：floor(len/8)，
// 尾部不足 8 字节丢弃）。真链路里非 8 对齐的 v1 到不了这一步（对齐闸先行）——
// 本视图仅供入口层 magic 判据的可判性论证（"即便过了对齐闸也必拒"的双保险）。
std::vector<u64> StBytesToBlobSlots(const std::string& v1Text);

// v1 产物喂 v4 消费方入口层的必拒判据（通用：与段数/版本/长度无关）。
// 判据：首两字节 "PL" → 槽 0 低 16 位 0x4C50 ≠ UPL3 低 16 位 0x4C33；
// 长度 < 8 字节（组不出一个槽）同样必拒（"blob too small"）。
bool StV1TextBlobWouldBeRejected(const std::string& v1Text);

// 消费方 ctxSize 前置闸镜像（unified_aicpu_exec.cc:69-77 判据 1:1——真 .cc 在
// 真 alg_param/CANN 编译面，host 不可链，此处镜像其判定数学）：
//   闸 1（:69）：resCtx==nullptr || ctxSize==0 → 拒。
//     ★F1 附带病灶形态：资源期 AICPU 成功路径现状 opParam.ctxSize 置 0
//     （hccl_alloc_ctx_res.cc:1228）→ exec 期本闸必拒——消费方要求非 0。
//   闸 2（:73）：ctxSize % 8 != 0 → 拒（v1 文本 108B 等非 8 对齐产物的死路）。
enum class StCtxGateResult {
    CTX_GATE_PASS,                      // 两闸全过（blob 取回可进 ParseUnifiedBlobV4）
    CTX_GATE_REJECT_NULL_OR_ZERO,       // 闸 1 拒
    CTX_GATE_REJECT_NOT_U64_ALIGNED     // 闸 2 拒
};
StCtxGateResult StCheckCtxSizeGate(const void* resCtx, u64 ctxSize);

// KFC executor 判定序镜像（comm_kfc_open_kernel_executor.cc LaunchOpenOpParamDataImpl
// :194-201 的 GetAlgExec → CHK_SMART_PTR_NULL 拦截 → Orchestrate 三态 + unified 分叉）。
//   unifiedForkBeforeLookup=true：修复后判序（unified 键判定前移到 GetAlgExec 之前，
//     kernel_launch.cc:369 同型）——unified 键 → RunUnifiedAicpuOp 路径，
//     根本不进 GetAlgExec（无注册项也无妨）；
//   =false：病灶判序（GetAlgExec 先行）——unified 键无注册项必 nullptr，
//     死在 CHK_SMART_PTR_NULL 拦截，unified 分支永不可达（死代码——BLOCKER-1
//     在 KFC 链上的同型形态，即 F1 断点②）。
// isUnifiedKey：caller 注入真判定入口（IsUnifiedAicpuAlgName，strcmp 全串）。
// registered：algName 是否在 CollAlgExecRegistryV2 有注册项（存量名 true；
//   unified 键恒 false——不注册进 registry 是 R6 主会话裁决）。
enum class StKfcExecPath {
    KFC_PATH_UNIFIED_AICPU,       // → RunUnifiedAicpuOp（blob 消费方全链）
    KFC_PATH_LEGACY_ORCHESTRATE,  // 存量 executor Orchestrate（行为零变化）
    KFC_PATH_NULLPTR_INTERCEPT    // "Fail to find executor" 拦截（存量行为不变）
};
StKfcExecPath StResolveKfcExecPath(const char* algName, bool registered,
                                   bool (*isUnifiedKey)(const char*),
                                   bool unifiedForkBeforeLookup);

// ---------------------------------------------------------------------------
// R11 · resCtx 双语义可判基线（2026-10-08，资源束回归 + blob 第二通道 + algTag 回填）
//
// 病灶（R10 后仍存）：unified AICPU 链 opParam.resCtx 指 v4 blob（HcclEngineCtxCreate
// 产物），存量 exec 链（KFC BuildHolder open_res_ctx_cache.cc:126 / kernel_launch.cc:300）
// 按 AlgResourceCtxSerializable 反序列化必得垃圾；threads/channels 句柄 Materialize
// 申请后丢弃（op_common.cc HcclGetAlgResUnified/HcclChannelAcquireUnified :2854/:2900
// 申请不落束）；algTag 恒空（unified 从不调 SetOpParamAlgTag，KFC BuildCacheKey :111
// 拒收）。修复方向 = 仿 CCU AssembleUnifiedCcuDeliverables（hccl_alloc_ctx_res.cc
// :1486-1654）双 ctx：resCtx 回归资源束语义（tag=UnifiedAlgTag+"_resctx"）、blob 走
// 第二通道（倾向序列化尾部 append 16B {blobAddr,blobSize}）、broker 句柄落束、
// algTag 回填规范形。
//
// 本组判据（期望值独立手算，留痕 /tmp/st_r11_expectations.md + python
// /tmp/r11_st_expectations.py；真实现交叉锚见 st_verification.cc R11 节）：
//   ① StSerializeResCtxMirror / StDeserializeResCtxMirror —— 资源束
//     Serialize/DeSerialize 的紧凑流镜像（alg_param.h:529-611 字段序 1:1，
//     BinaryStream vector=size_t+元素、无字段间对齐）。ST 场景 infos/physIdx 恒空、
//     channels 多组（vector<vector>）——镜像只实现 ST 消费的形状（期望值纪律：
//     不 include 真实现，交叉锚由 R11 节用真 alg_param.h 编译面探针对数）。
//   ② StSerializeResCtxMirror 的 planB 形态 —— 方案 B（主会话裁决，2026-10-08）：
//     新增 u64 unifiedBlobAddr/unifiedBlobSize 两序列化字段（插 kfcServerArgSize
//     之后、topoInfoSeqSize 之前，topo 保持最后段）。原"尾部 append 16B"方案被
//     审查方证伪（topo 锚 data.size()-topoInfoSeqSize 硬性假设 topo 是最后一段，
//     尾部附加静默错位）——StAppendBlobTail/StReadBlobTail 保留作病灶判据素材
//     （证伪形态的可判性证据），不再作为修复方向断言。
//   ③ StKfcCacheKeyGate —— KFC BuildCacheKey 判定镜像（open_res_ctx_cache.cc
//     :73-119 1:1）：commName/algTag 均需非空有界串（std::find '\0'，空串
//     end==begin 拒、无 '\0' end==begin+N 拒）；algTag 恒空 = R11 病灶（拒收），
//     回填 UnifiedAlgTag+"_resctx" 规范形 = 修复（过闸）。
// ---------------------------------------------------------------------------
// 镜像构造输入（ST 消费的资源束形状：threads + 多组 channels + commInfoPtr/token）
struct StResCtxCase {
    std::vector<u64> threads;              // ThreadHandle 序列（uint64）
    std::vector<std::vector<std::pair<u32, u64>>> channels; // 多组：(remoteRank, handle)
    u64 commPtr{0};                        // commInfoPtr（KFC BuildHolder 的 token 校验源）
    u64 cclAddr{0};
    u64 cclSize{0};
    u32 slaveThreadNum{0};
    std::vector<u32> notifyPerThread;
    // 方案 B（主会话裁决）：unifiedBlobAddr/unifiedBlobSize 两 u64 序列化字段。
    //   =false（默认）：旧格式（无新字段）——legacy 无害性对账的基线半边；
    //   =true：方案 B 格式（新字段插 kfcServerArgSize 后、topoInfoSeqSize 前）。
    bool planB{false};
    u64 unifiedBlobAddr{0};
    u64 unifiedBlobSize{0};
};

// Serialize 镜像 → 字节串（长度即手算基线：小 666/682、中 1670/1686、边界 334/350、
// eq 1526/1542 = 旧/方案 B；topo 尾段恒 128）
std::vector<char> StSerializeResCtxMirror(const StResCtxCase& c);

// DeSerialize 镜像 → 消费字段（binary 段步进与 Serialize 同序；topo 尾段按
// size()-topoInfoSeqSize 倒切——镜像 alg_param.h:598-607 的窗口语义；
// 方案 B 格式下新字段从插入点读回）
struct StResCtxReadback {
    bool ok{false};                        // binary 段步进是否走完（不越界）
    std::vector<u64> threads;
    std::vector<std::vector<std::pair<u32, u64>>> channels;
    u64 commPtr{0};
    u32 slaveThreadNum{0};
    u32 topoInfoSeqSize{0};
    size_t topoTailStart{0};               // data.size()-topoInfoSeqSize（topo 锚观测点）
    u64 unifiedBlobAddr{0};                // 方案 B 新字段（旧格式产物上恒 0 占位）
    u64 unifiedBlobSize{0};
};
StResCtxReadback StDeserializeResCtxMirror(const std::vector<char>& data);

// 尾部 append 三元组（16B：blobAddr 8B LE + blobSize 8B LE）——★被证伪方案的
// 病灶判据素材（topo 锚静默错位形态），保留供 R11 病灶反例断言：
//   StAppendBlobTail  构造证伪形态（真实现从不会产出——ST 用它证明错位可判）
//   StReadBlobTail    末尾倒 16B 读回（长度 <16B 拒）
std::vector<char> StAppendBlobTail(const std::vector<char>& resCtxBytes, u64 blobAddr, u64 blobSize);
bool StReadBlobTail(const std::vector<char>& withTail, u64& blobAddr, u64& blobSize);

// KFC BuildCacheKey 判定镜像（真源 1:1：有界串判据 + 双字段与关系）
bool StKfcCacheKeyGate(const char* commName, const char* algTag);

// 交叉锚常量（真源对齐，探针 /tmp/r11_work/roundtrip_probe 实测）：
constexpr size_t ST_R11_SZ_ALGTYPE = 12;        // sizeof(AlgType)
constexpr size_t ST_R11_SZ_HCcLMEM = 24;        // sizeof(HcclMem)
constexpr size_t ST_R11_SZ_CHANNELINFO = 160;   // sizeof(ChannelInfo)（remoteRank@4 handle@32）
constexpr size_t ST_R11_SZ_PARALLELPORT = 24;   // sizeof(ParallelChannelPortInfo)
constexpr size_t ST_R11_TOPO_DEFAULT_SEQ = 128; // 默认构造 topo Serialize 长（userRank/RankSize 赋值）
constexpr u32 ST_R11_COMM_NAME_LEN = 160;       // COMM_INDENTIFIER_MAX_LENGTH（真实 alg_param.h:69-71）
constexpr u32 ST_R11_ALG_TAG_LEN = 288;         // ALG_TAG_LENGTH = TAG_LENGTH(160)+OP_ALG_LENGTH(128)

} // namespace mc2_ops_hccl::refactor::st

#endif
