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

// ---------------------------------------------------------------------------
// R12 · CCU 动词序执行器 · v5 动词帧独立基线 + checker 扩展（2026-10-08）
//
// 背景（用户方向 + 主会话裁决 R12-1，2026-10-08）：退役整体流水 kernel 的 server
// 直调（DispatchUnifiedPlanV3 直调 CcuKfcAllGatherMesh1DMem2MemKernel /
// CcuReduceScatterMesh1DMem2MemKernel，ccu_kernel_kfc_server.cc:200-255），改
// **动词 ID → 原语组合**；灰度并行 = 新 role UNIFIED_VERB + v5 blob（"CcuVerb"
// 前缀第二档），v3/"CcuAction" 路径零变化（T3 灰度回归断言的既有面）。
//
// 本组九件（期望值全部独立手算，不转抄生成方；行号 = 本仓实数）：
//   ① StPrim 原语调用序基线（T1.1）：AG sole mesh / RS mesh 老 kernel 逐行推导
//   ② 帧预算容量表（T1.2，裁决 1 动词帧口径）
//   ③ v5 帧布局锚（T1.3，裁决 2 设计针 + ST 槽位锚）
//   ④ SimulateKfcServerVerb（T2.1）：v5 动词帧逐轮解包模拟器（镜像 V3 三防御）
//   ⑤ VerbFrameRecord/VerbSimResult + ExtractVerbParams（T2.2，分发读取序锚）
//   ⑥ CheckVerbSeqEquivalent（T2.3）：基线 vs verb 帧展开的数据流等价判据
//   ⑦ StBuildAicpuRsVerbBaseline（裁决 4①：AICPU RS 写侧动词序独立基线）
//   ⑧ CheckPullPushEdgeEquivalence（裁决 5，#10：pull/push 边级等价证明件）
//   ⑨ RS 单 chunk 限界镜像（裁决 3：chunkCount>1 拒收的 ST 判据半边）
// ---------------------------------------------------------------------------

// ---- ③-1 XN 统一表（裁决 2 设计针：v5 SYNC 帧掩码的位定义）----
// 旧 kernel 常量 → v5 统一表迁移登记（等价判据的**声明性差异**，非不等价——
// CheckVerbSeqEquivalent 比较 sync 边界位置与搬运集合，不比较两时代 XN 编码）：
//   AG 老（ccu_kernel_kfc_all_gather_mesh1d_mem2mem.cc:14-17）：output=1→OUT=3
//     （迁位避让 SCR）、token=2→TOK=2（不动）、post=3→POST=4（迁位）
//   RS 老（ccu_kernel_kfc_reduce_scatter_mesh1d_mem2mem.cc:17-20）：input=0→IN=0、
//     scratch=1→SCR=1、token=2→TOK=2（RS 三元恒等映射）、post=3→POST=4
constexpr u32 ST_V5_XN_IN = 0U;
constexpr u32 ST_V5_XN_SCR = 1U;
constexpr u32 ST_V5_XN_TOK = 2U;
constexpr u32 ST_V5_XN_OUT = 3U;
constexpr u32 ST_V5_XN_POST = 4U;
// SYNC_PRE 帧携 XN 掩码（裁决 2）：AG=OUT|TOK、RS=IN|SCR|TOK；SYNC_POST=POST 位
constexpr u32 ST_V5_XN_MASK_AG_PRE = (1U << ST_V5_XN_OUT) | (1U << ST_V5_XN_TOK);      // 0xC
constexpr u32 ST_V5_XN_MASK_RS_PRE = (1U << ST_V5_XN_IN) | (1U << ST_V5_XN_SCR) |
                                     (1U << ST_V5_XN_TOK);                              // 0x7
constexpr u32 ST_V5_XN_MASK_POST = (1U << ST_V5_XN_POST);                               // 0x10

// ---- ③-2 v5 第一级动词 ID 锚（帧槽 0 的 dispatch 键）----
// 锚定 VerbCall::Verb 枚举序（verb_sink.h:40-52：COPY=0..SYNC_PEER_POST=7）——
// 两级动词的第一级即帧动词，第二级 = 原语组合（server verb 分发循环展开）。
constexpr u64 ST_V5_VERB_COPY = 0U;
constexpr u64 ST_V5_VERB_POST_COPY = 1U;
constexpr u64 ST_V5_VERB_READ = 2U;
constexpr u64 ST_V5_VERB_WRITE = 3U;
constexpr u64 ST_V5_VERB_REDUCE = 4U;
constexpr u64 ST_V5_VERB_BROADCAST = 5U;
constexpr u64 ST_V5_VERB_SYNC_PRE = 6U;
constexpr u64 ST_V5_VERB_SYNC_POST = 7U;
constexpr u64 ST_V5_VERB_ID_MAX = 7U;
// 帧几何（沿 v3：server LoadFuncParamFromMemory 一次 32 槽 = CCU_PARAM_NUM_PER_DIE，
// 环形队列 CCU_TASK_NUM_MAX=64；哨兵 = INVALID_U64 同槽位惯例）
constexpr u64 ST_V5_SENTINEL = 0xFFFFFFFFFFFFFFFFULL;
constexpr u32 ST_V5_SLOTS_PER_FRAME = 32U;
constexpr u32 ST_V5_RING_CAPACITY = 64U;
constexpr u32 ST_V5_MAX_PLAN_FRAMES = 63U;       // 环容量 - 1 出口哨兵

// ---- ② 帧预算容量表（T1.2，R12-1 裁决 1：动词帧口径，全部手算）----
//   AG sole mesh 单段    = 3（SYNC_PRE + BROADCAST + SYNC_POST；任意 N——BROADCAST
//                          是声明式原语，扇出在引擎内部，不随 N 涨帧）
//   RS mesh 单 chunk ≤8P = N+2（SYNC_PRE + READ×(N-1) + REDUCE + SYNC_POST；8P=10）
//   RS mesh 单 chunk >8P = N+3（+1 COPY 帧：>8P 本 rank 片先落 scratch 槽，kernel
//                          :176-179 LocalCopy；9P=12）
//   AR two_shot（RS→AG） = (N+2 或 N+3) + 3（RS 段 + AG 段；≤8P 即 N+5，8P=13；
//                          >8P 为 N+6——RS 的 COPY 帧随 RS 段带入；合取屏障规则下
//                          post[i] AND pre[i+1] 合并的是**等待语义**，帧仍各自存在）
//   单 Plan 上限         = 63（哨兵 +1 = 64 环几何不变）
//   边界 N（手算）：RS 单 chunk >8P 形态 N+3 ≤ 63 → N ≤ 60（61 溢出）；AR two_shot
//   >8P 形态 N+6 ≤ 63 → N ≤ 57（58 溢出）；≤8P 形态恒远低于上限（RS 最大 10、
//   AR 最大 13）。
//   A/B tiered（登记行，裁决 1：**非容量断言对象**）——v5 放开 NHR 前不进 verb
//   灰度，双因：①NHR v3 拒收既有边界（帧无 pattern 载体，R9 M0-④）；②B 步进
//   超限：16P 4×4（mesh4×nhr4）档 stepNum=6（scale 分支：slowRankSize=4 ≤ ratio
//   4.286 → maxStep=6，tier_expand.h:162/:172）→ 12 段 × (4+2)帧 = 72 > 63。
inline u64 StVerbFrameBudgetAg() { return 3ULL; }
inline u64 StVerbFrameBudgetRs(u32 n) { return static_cast<u64>(n) + 2ULL + (n > 8U ? 1ULL : 0ULL); }
inline u64 StVerbFrameBudgetArTwoShot(u32 n) { return StVerbFrameBudgetRs(n) + 3ULL; }

// ---- ⑨ RS verb 单 chunk 限界镜像（裁决 3）----
// chunkSize = scratchSize(64MB)/rankNum/128*128（128B 对齐下取整；设备侧写入方
// hccl_ccu_v0_prepare.h:546-563 与 loop_splitter.h CalcRsChunks:168-182 同式——
// 本镜像是 ST 独立第三实现，与生成方数字互为交叉锚）。手算例：
//   8P：67108864/8=8388608 → /128=65536 → *128=8388608（恰 8MB 整）
//   9P：67108864/9=7456540 → /128=58254（7456540/128=58254.53 下取整）→ *128=7456512
// chunkCount>1 的 RS 段 v5 序列化器拒收（T3 负例锚 [A7]）。
inline u64 StRsChunkSizeMirror(u32 rankNum)
{
    constexpr u64 ST_RS_SCRATCH_MIRROR = 64ULL * 1024 * 1024;
    constexpr u64 ST_RS_ALIGN_MIRROR = 128ULL;
    if (rankNum == 0U) {
        return 0ULL;
    }
    return ST_RS_SCRATCH_MIRROR / rankNum / ST_RS_ALIGN_MIRROR * ST_RS_ALIGN_MIRROR;
}
inline u64 StRsChunkCountMirror(u64 sliceSize, u32 rankNum)
{
    const u64 chunkSize = StRsChunkSizeMirror(rankNum);
    if (sliceSize == 0ULL || chunkSize == 0ULL) {
        return 0ULL;
    }
    return (sliceSize - 1ULL) / chunkSize + 1ULL;
}

// ---------------------------------------------------------------------------
// ① 原语调用序基线（T1.1）：老 kernel 逐行推导的结构化期望。
//
// 推导源（行号 = 本仓实数，ST 独立推导不转抄生成方）：
//   AG（ccu_kernel_kfc_all_gather_mesh1d_mem2mem.cc，376 行）：
//     PreSync :110-125   每通道 2 条 WriteVariableWithNotify（output[XN=1] +
//                        token[XN=2]，:115-120 循环体两条为一组）→ 2×(N-1) 条；
//                        再 NotifyWait(mask=OUT|TOK=0x6) ×(N-1)（:121-123）
//     三阶段 :222-270    repeatNum=1 → CCU_WHILE 单趟：
//       Phase1 :237-253 → DoAllGatherWrite :164-182：rankIdx 0..N-1 升序，
//         ==me → EventRecord(events[0], 1<<me)（:174）；否则 Write(ch, dst[rankIdx],
//         src, sliceSize, events[0], 1<<rankIdx)（:176-178）→ (N-1) Write + 1 ER
//       Phase2 :256     → DoAllGatherGroupCopy :198-220：CCU_IF(isInputOutputEqual
//         ==0)（:200）→ GroupCopy(localDst, srcLocCopy, goSize)（:214）
//       Phase3 :260-266 → DoAllGatherWait :184-196：eventNumPerIter=ceil(N/16)（:168）
//         → EventWait(mask=(1<<min(N,16))-1)（:190-193）
//     PostSync :127-137  NotifyRecord(POST=3, bit 0x8) ×(N-1)（:130-132）→
//                        NotifyWait(mask 0x8) ×(N-1)（:133-135）
//   RS（ccu_kernel_kfc_reduce_scatter_mesh1d_mem2mem.cc，462 行）：
//     PreSync :134-149   每通道 3 条 WVWN（input[XN=0]/scratch[XN=1]/token[XN=2]，
//                        :136-143 三条为一组）→ 3×(N-1)；NotifyWait(mask=0x7)（:144-147）
//     三阶段 :321-368    单 chunk（chunkLoopNum=UINT64_MAX-1 → 1 趟，:377）：
//       Phase1 :330-334 → DoReduceScatterRead :162-186：rankIdx 升序，==me →
//         N≤8 EventRecord 占位（:172-174，零搬运）/ N>8 LocalCopy(scratchMem[me],
//         myInput)（:176-179）；否则 Read(ch, scratchMem[rankIdx], remoteInput
//         [rankIdx], slice)（:180-183）
//       Phase2 :354-360 → DoReduceScatterWait :189-201：EventWait(mask=(1<<N)-1，
//         N≤16 单条 :195-199)
//       Phase3 :366     → DoReduceScatterReduce :297-318 单趟 → DoReduceScatter
//         :235-256：N≤8 → GroupLocalReduce（scratch[me] 替换为 input 直读，:246-250）；
//         N>8 → PairwiseLocalReduce :204-232（remainPieces 链 N→..→1，每轮
//         LocalReduce(inputVec[0], inputVec[remain-rp], rp*slice) + EventWait(1)
//         :222-223；末尾 LocalCopy(myOutput, inputVec[0]) + EventWait :228-229）
//     PostSync :151-160  NotifyRecord(POST=3, 0x8) + NotifyWait(0x8) 各 ×(N-1)
//
// 计数手算（8P me=3、repeatNum=1、单 chunk、inOutEqual=0）：
//   AG = 14 WVWN + 7 NW + 7 WR + 1 ER + 1 GC + 1 EW + 7 NR + 7 NW = 45 条
//   RS(≤8P) = 21 WVWN + 7 NW + 7 RD + 1 ER + 1 EW + 1 GLR + 7 NR + 7 NW = 52 条
//   RS(9P)  = 24 WVWN + 8 NW + 8 RD + 1 LC + 1 EW(0x1FF) + [4 LR + 4 EW + 1 LC +
//             1 EW]（Pairwise 折叠链 9→5→3→2→1）+ 8 NR + 8 NW = 69 条
// ---------------------------------------------------------------------------
struct StPrim {
    enum Op : u32 {
        OP_WVN = 0,   // WriteVariableWithNotify（PreSync 地址/token 广播）
        OP_NW,        // NotifyWait
        OP_NR,        // NotifyRecord
        OP_WR,        // Write（机间：src 本地 → dst 对端 output）
        OP_RD,        // Read（机间：src 对端 input → dst 本地 scratch 槽）
        OP_LC,        // LocalCopy（本地搬运）
        OP_GC,        // GroupCopy（本地分块搬运——goSize 分块为搬运内细节，总量计 1 条）
        OP_GLR,       // GroupLocalReduce（≤8P 本地折叠一步）
        OP_LR,        // LocalReduce（>8P Pairwise 折叠步）
        OP_ER,        // EventRecord（批内完成跟踪——v5 帧不承载，归一化丢弃）
        OP_EW         // EventWait（同上）
    };
    Op op{OP_WVN};
    u32 peer{0};           // OP_WR/OP_RD：通道对端 algRank（通道序 = remoteRank 升序跳过 me）
    u64 dstOff{0};         // WR：对端 output 基+me*S；RD：本地 scratch 基+peer 槽；LC/GC/LR：dst
    u64 srcOff{0};         // WR/RD/LC/GC：src；LR：折叠源槽
    u64 size{0};           // WR/RD/LC/LR：字节；GC：总量；GLR：本 rank 片
    u32 mask{0};           // ER/EW：事件位；NW：掩码；NR：位
    u32 xn{0};             // WVN：xnId；NW/NR：语义 XN（旧表登记用）
    bool foldReduce{false}; // >8P Pairwise 的 LR/尾 LC：归一化折叠进单条 REDUCE 事件
};

// 基线地址几何（符号 owner + 绝对 offset：所有 rank 的 input/output/scratch 共用
// 同名基址锚，rank 维度由 owner 区分——multiset 比较与边归一化的统一口径）。
// R12-7 stride 修正：inStride/outStride 是 kernel 槽 9/3 的**exec 真值**口径——
//   AG sole 布局：inStride=0（input 只含我的片，slot9=0 无需覆写）、outStride=
//   outputSliceStride（slot3=stride*me）
//   RS：inStride **exec 真值 = sliceSize**（v3 链由 exec 期 slot4 覆写注入
//   me*slice——StInstantiateRsFrame r[4] 同式；资源期布局 inStride=0 是占位，
//   v5 零覆写下面 RS READ 帧携带 0 是 F3 缺陷——基线取 exec 真值才能检出）
//   缺省 0（caller 显式给值；J 组自检按各自手算口径设置）
struct StMeshGeom {
    u32 n{8};
    u32 me{0};
    u64 slice{0};            // 正常片字节
    u64 tail{0};             // 尾片（0=均匀；AG 末 rank 生效，kernel :305 同式）
    u64 inBase{0};
    u64 outBase{0};
    u64 scratchBase{0};
    u64 inOutEqual{0};       // AG：isInputOutputEqual（0 → GroupCopy 在场）
    u64 inStride{0};         // input 片步长（kernel 槽 9 语义 = inStride*me）
    u64 outStride{0};        // output 片步长（kernel 槽 3 语义 = outStride*me）
};

// AG sole mesh 单段原语序（上述推导的实现；repeatNum 恒 1——v3 槽 10 单 die 语义）
std::vector<StPrim> StBuildAgM2mPrimBaseline(const StMeshGeom& g);
// RS mesh 单 chunk 原语序（≤8P 零搬运占位 / >8P LocalCopy+Pairwise 两形态）
std::vector<StPrim> StBuildRsM2mPrimBaseline(const StMeshGeom& g);

// ---- 归一化比较粒度 ----
// 本地地址 owner 哨兵（owner 仅对 remote 有意义；边归一化时以 me 代入本地侧）
constexpr u32 ST_MOVE_SELF = 0xFFFFFFFFU;
struct StMoveEdge {                 // 归一化搬运（符号地址：remote 标志 + owner + 绝对 offset）
    bool srcRemote{false};
    u32 srcOwner{ST_MOVE_SELF};
    u64 srcOff{0};
    bool dstRemote{false};
    u32 dstOwner{ST_MOVE_SELF};
    u64 dstOff{0};
    u64 size{0};
    u32 chanIdx{0};                 // R12-4：通道位降诊断字段（基线侧真值；帧侧隐式
                                    // 映射模型下 = 第 k 条数据帧的 k——比较不进键）
};
struct StNormEvent {                // 归一化事件：MOVE / REDUCE / SYNC 边界
    enum Kind : u32 { MOVE = 0, REDUCE = 1, SYNC_PRE = 2, SYNC_POST = 3 };
    Kind kind{MOVE};
    StMoveEdge mv;                  // MOVE/REDUCE（REDUCE：src=scratch 基、dst=output、size=片）
    u32 xnMask{0};                  // SYNC（两时代 XN 编码不同——声明性差异，不参与比较）
};

// 基线原语序 → 归一化事件流：前导 WVN/NW 连续块折叠单条 SYNC_PRE；尾随 NR/NW
// 连续块折叠单条 SYNC_POST；ER/EW 丢弃（批内完成跟踪，v5 帧不承载）；GC 折叠单条
// MOVE；GLR 单条 REDUCE；foldReduce 连续段（Pairwise LR*/EW*/LC/EW）折叠单条 REDUCE
//（src=首条 LR 的累积基 inputVec[0]、dst/size=末条 LC 落定值）。
// R12-4：OP_WR/OP_RD 的 owner 一律存对端 **algRank**（通道位 chanIdx 降诊断字段）；
// 归一化需要 me 做通道位→rank 映射——StSetBaselineMe 由 caller（等价判据入口）
// 预置，StBaselineMe 为缺省 me=0 的读出（单进程单基线，测试面安全；头文件无
// 状态全局的替代是给 StNormalizeBaseline 加 me 形参——会改既有调用面，从简）。
void StSetBaselineMe(u32 me);
u32 StBaselineMe();
std::vector<StNormEvent> StNormalizeBaseline(const std::vector<StPrim>& prims);

// ---------------------------------------------------------------------------
// ③-3 v5 帧布局锚（T1.3）+ ④⑤ 模拟器与参数抽取（T2.1/T2.2）
//
// 槽位锚（生成方 M0 落盘后重锚，见文末清单 [A2]/[A3]）：
//   槽 0 = verb ID（第一级动词）
//   数据帧 READ/WRITE/COPY/POST_COPY：槽1=src、槽2=dst、槽3=size、槽4..31=0
//     ——**无 channelIdx/peer 槽**（裁决 2：段内数据帧按通道序隐式映射，第 k 条
//     数据帧 ↔ channels[k]，channels 按 remoteRank 升序）
//   BROADCAST：槽1=src、槽2=localDstOff、槽3=size、槽4=remoteDstOff（扇出目的槽）
//   REDUCE：槽1=scratch 基、槽2=output、槽3=本 rank 片
//   SYNC_PEER_PRE/SYNC_PEER_POST：槽1=XN 掩码（AG 0xC / RS 0x7 / POST 0x10）
//   头部锚 [A4]：7 槽 {magic "UPL5"=0x55504C35, version 5, frameCount, reserved,
//     dataType, outputType, reduceType}（镜像 v4 头布局）
// ---------------------------------------------------------------------------
struct StVerbFrameLayout {
    u32 slotSrc{1};       // 数据帧 src
    u32 slotDst{2};       // 数据帧 dst
    u32 slotSize{3};      // 数据帧 size
    u32 slotAux{4};       // BROADCAST：remoteDstOff（其余数据动词未用=0）
    u32 slotXnMask{1};    // SYNC 帧掩码
};

struct VerbFrameRecord {            // 模拟器逐帧解包产物（server verb 循环视角）
    u64 verbId{0};                  // 槽 0（INVALID_U64 以外的合法动词 ID）
    std::vector<u64> slots;         // 本帧 32 槽原始值（含槽 0；未写槽为 0）
    u64 frameIndex{0};              // 第几帧（0 起，环回前语义）
};

struct VerbSimResult {
    std::vector<VerbFrameRecord> frames;   // 逐帧解包产物（哨兵帧不进列表）
    u64 rawFrameCount{0};                  // 含哨兵帧的总帧数（环回前）
    bool wrapped{false};                   // 是否发生 64 帧环形回绕
    std::string err;                       // 非空 = 拒发（server 视角的错误）
};

// v5 动词帧逐轮解包模拟器（镜像 SimulateKfcServerV3 的三防御 + verb ID 白名单）：
//   防御① 半帧（载荷非 32 倍数）——序列化器写错/截断，server 读半帧拿残影槽值
//   防御② 环形容量 64 帧（CCU_TASK_NUM_MAX）——超容量帧永远等不到信号
//   防御③ 整圈无出口哨兵——server 回绕重放 0 号帧死循环（满 64 帧全动作帧）
//   白名单：槽 0 verb ID 不在 caller 给的合法集合 → 拒发点名（server 微码对未知
//   ID 只打日志继续——v2/v3 同惯例；ST 加严显式化，v3 模拟器同口径）
// serialized = 帧记录区（caller 剥头部；哨兵帧若随载荷传入则按出口语义消费）。
VerbSimResult SimulateKfcServerVerb(const std::vector<u64>& serialized,
                                    const std::vector<u64>& legalVerbIds);

// 逐动词参数抽取（T2.2——server verb 分发循环读取序的 host 镜像，锚 [A6]：
// 生成方落盘后按真分发循环逐槽重锚）。frame 须含 32 槽。
struct StVerbParams {
    u64 verbId{0};
    u64 src{0};
    u64 dst{0};
    u64 size{0};
    u64 aux{0};            // BROADCAST：remoteDstOff
    u64 xnMask{0};         // SYNC 帧
};
StVerbParams ExtractVerbParams(const std::vector<u64>& frame, const StVerbFrameLayout& lo);

// v5 帧 → 归一化事件流（等价判据的 verb 半边）：
//   SYNC 帧 → SYNC_PRE/SYNC_POST 事件（SYNC_PRE 同时重置段内数据帧计数）
//   READ/WRITE → MOVE（peer = 段内第 k 条数据帧 ↔ 升序通道表第 k 项；
//     k ≥ N-1 → err"数据帧超通道数"——帧无 channelIdx 槽的越界防御形态）
//   COPY/POST_COPY → MOVE（本地）
//   BROADCAST → 展开 MOVE×N：本地 1（src→localDst）+ 机间写 (N-1)（src→各通道
//     remoteDstOff；kCcuVerbMap BROADCAST 行 = GroupBroadcast 的扇出语义）
//   REDUCE → REDUCE 事件（scratch 基 → output，原子级——Pairwise/GroupLocalReduce
//     两形态同为 REDUCE 动词的原语组合，等价判据不分解其内部）
std::vector<StNormEvent> StExpandVerbFrames(const std::vector<VerbFrameRecord>& frames,
                                            const StVerbFrameLayout& lo, u32 n, u32 me,
                                            std::string& err);

// ---------------------------------------------------------------------------
// ⑥ CheckVerbSeqEquivalent（T2.3）：老路径原语序基线 vs 新 verb 帧展开序。
// 数据流等价判据（三段）：
//   ① 同步边界结构：两侧归一化事件的 kind 序列（SYNC_PRE/MOVE/REDUCE/SYNC_POST
//      交错序）逐位相等——pre 在首个数据事件前、post 在末个数据事件后、段间
//      post→pre 相邻对（合取屏障：post[i] AND pre[i+1] 合并等待，帧各自存在）
//   ② 搬运集合：同步窗（相邻 SYNC 标记之间）内 MOVE 的 {src,dst,size} multiset
//      相等（allowBatchReorder=true：AG 批量 Write / RS 批量 Read 批次内乱序容许
//      ——显式声明的容许差异；false：逐条严格相等）
//   ③ REDUCE：窗内逐条（src/dst/size）相等且相对序一致
// XN 编码两时代不同（旧 AG OUT=1/POST=3 → v5 OUT=3/POST=4）——声明性差异，
//   不参与比较（v5 帧掩码正确性由 T3 对真帧直接断言）。
// 返回 0=等价；非 0 时 err 给出第一条不等价规则。
// ---------------------------------------------------------------------------
struct StEquivOptions {
    bool allowBatchReorder{true};   // 同步窗内搬运乱序容许（AG/RS 批量语义）
};
int CheckVerbSeqEquivalent(const std::vector<StPrim>& baseline,
                           const std::vector<VerbFrameRecord>& frames,
                           const StVerbFrameLayout& lo, u32 n, u32 me,
                           const StEquivOptions& opt, std::string& err);

// ---------------------------------------------------------------------------
// ⑦ AICPU RS 段动词序独立基线（裁决 4①；推导源 ins_temp_reduce_scatter_mesh_1D.cc
// 307 行，AICPU 是**写侧**语义——与 CCU 读侧不同）：
//   KernelRun :63-102：(threadNum>1) PreSyncInterThreads :80-84 → SYNC_PEER_PRE
//   RunReduceScatter :174-249：rankIdx=1..N-1（:181），nextRank=(me+rankIdx)%N
//     （:182 —— **环序**，非升序！与 v5 通道序的声明差）；每通道（channelsPerRank
//     =1）SendRecvBatchWrite（:243）→ WRITE 动词：
//       src = input 基 + nextRank*inputSliceStride（:224-228，我的 input 里属于
//             nextRank 输出份的那片）
//       dst = 对端 hcclBuff + me*outputSliceStride（:229-233，对端 scratch 我的槽）
//       size = sliceSize（nextRank 是末 rank 且 tail>0 时取 tail，:186-190）
//   PostSyncInterThreads :86-90 → SYNC_PEER_POST
//   PostCopy :104-172：in/out buff 型或基址不同（:132-133）→ LocalCopy（:134-144）
//     → COPY 动词（我的片 input→output 落位）；再 tmpRank≠me 逐个 LocalReduce
//     （:155-169，hcclBuff 对端槽 → output 我的槽）→ REDUCE×(N-1)
//   ★结构差登记（与 CCU 读侧）：AICPU 的 COPY+REDUCE×(N-1) 在 SYNC_POST **之后**
//   （PostCopy 在 KernelRun 返回后执行）；CCU 的 REDUCE 在 SYNC_POST 之前（Phase3）。
//   边集合两侧同构（每 rank 的入边 {(p,me)}），跨引擎等价走 ⑧ 的边级判据。
// ---------------------------------------------------------------------------
struct StVerbCall {                  // 动词序基线的元素（词法面对齐 VerbCall 值对象）
    u64 verbId{0};                   // ST_V5_VERB_*
    u64 srcOff{0};
    u64 dstOff{0};
    u64 size{0};
    u32 peer{0};                     // R12-4：WRITE/READ 对端 algRank——AICPU 侧 =
                                    // VerbCall 显式 peer 值（写侧配对负例的可表达性
                                    // 前提：两条 WRITE 换 peer 不换几何，纯几何键
                                    // 盲区）；CCU 侧 = 隐式模型"第 k 数据帧↔升序
                                    // 第 k peer"（帧无 channelIdx 槽，裁决 2）
};
// R12-7 stride 口径：WRITE src = inBase + inStride*nextRank。inStride 取值随
// 断言场景：Compile 期布局值 0（BuildRequest :113——当前实物形态）/ exec 真值
// sliceSize（v3 slot4 覆写语义——老 kernel :270-272 形态）。缺省 0 对齐当前实物。
std::vector<StVerbCall> StBuildAicpuRsVerbBaseline(u32 n, u32 me, u64 slice, u64 tail,
                                                   u64 inBase, u64 outBase, u64 scratchBase,
                                                   bool buffsDiffer, u64 inStride = 0);
// 实际动词序 vs 基线：动词 kind 序列严格相等；WRITE 段与 REDUCE 段内部容许排列
// （AICPU 环序 vs Emit peers 序的声明差），段间位置（PRE/POST/COPY）严格。
int CheckAicpuRsVerbSeq(const std::vector<StVerbCall>& actual, u32 n, u32 me, u64 slice,
                        u64 tail, u64 inBase, u64 outBase, u64 scratchBase, bool buffsDiffer,
                        std::string& err, u64 inStride = 0);

// ---------------------------------------------------------------------------
// ⑧ CheckPullPushEdgeEquivalence（裁决 5，#10：AICPU BROADCAST N/A 等价展开证明）。
// 命题：读侧聚合（AICPU copy+read×(N-1)，动词表 BROADCAST 行的等价展开）与写侧
// 扇出（CCU BROADCAST = local+write×(N-1)）搬运集合边级等价——
//   pull 侧每 rank 入边集 == push 侧每 rank 出边集的**转置**。
// 边归一化：producer = src.remote ? src.owner : me；consumer = dst.remote ?
//   dst.owner : me（4P AG 手算：me=1 时 pull 边 {(1,1),(0,1),(2,1),(3,1)}；
//   push 边 {(1,1),(1,0),(1,2),(1,3)}，转置后逐值相等——16 边全集两引擎同构）。
// 同一 helper 反向用即 RS 跨引擎（AICPU 写侧出边 vs CCU 读侧入边）。
// ---------------------------------------------------------------------------
int CheckPullPushEdgeEquivalence(const std::vector<StMoveEdge>& pullSide,
                                 const std::vector<StMoveEdge>& pushSide,
                                 u32 me, std::string& err);

// ---------------------------------------------------------------------------
// R12 重锚任务清单（T4；生成方 M0/落盘后逐项对数，零偏移闭环——R11 惯例）：
//   [A1] v5 第一级动词 ID 取值（ST_V5_VERB_* 锚 = VerbCall::Verb 枚举序 0..7；
//        生成方若另立 ID 表 → 重锚常量 + 测试白名单）
//   [A2] 数据帧槽位 slotSrc/Dst/Size/Aux = 1/2/3/4（未用槽 5..31 = 0）
//   [A3] SYNC 帧掩码槽位 slotXnMask = 1；BROADCAST aux 槽语义（remoteDstOff）
//   [A4] v5 blob 头部（锚 7 槽 UPL5/version 5/frameCount/reserved/dt/odt/rt，
//        镜像 v4 头布局；含 magic 字面量 0x55504C35）
//   [A5] role 枚举值（UNIFIED_VERB 锚 = 10，纯追加于 UNIFIED_PLAN=9 后）与
//        kernelFuncName 前缀（"CcuVerb"，len 7——KFC_UNIFIED_VERB_PREFIX 锚）
//   [A6] server verb 分发循环读取序（ExtractVerbParams 逐动词读取序镜像）
//   [A7] RS 单 chunk 拒收判据（chunkCount>1 → 序列化器 err——T3 负例锚）
//   [A8] WP-9 四件：AicpuReduceSlice Emit 动词序/地址口径（⑦ 基线对数）、
//        HcommVerbSink WRITE/REDUCE 接线入口序（WRITE → SendRecvWrite 五步
//        wrapper :275-294 record ACK/wait ACK/HcommWriteOnThread/record DS/wait DS；
//        REDUCE → LocalReduce wrapper :737——64bit/PROD 走 AicpuReduce）、RS 路由
//        AICPU 半边激活（route 表 RS 行全引擎 + :64-68 半激活注撤除）、
//        aicpu_consumer RS 段分派（:319/:343 CollectAll 硬编码改帧 kind 分派）
//   [A9] AR two_shot 混排序列化格式（两段 XN 掩码逐段不同 0x7→0xC——T3 格式级，
//        路由激活不在 R12，不断言运行时行为）
// 设计针（裁决 2 已定，非重锚项）：XN 统一表 {IN=0,SCR=1,TOK=2,OUT=3,POST=4}；
//   SYNC_PRE 帧携 XN 掩码；数据帧通道序隐式映射（无 channelIdx 槽）；合取屏障
//   （post[i] AND pre[i+1] 合并——帧各自存在，等待语义合一）。
//
// R12-4 口径登记（审查裁定 2026-10-08，合并键方案——等价判据 ⑥/⑦/⑧ 的键结构
// 与两条能力边界，回炉负例见 J 节 R12-4 组）：
//   ① CCU 通道置换免疫的**前提不变量**（multiset 容差语义正确的依据）：
//     (a) 帧不携对端裸地址——READ/WRITE 帧的 dst/src 都是"本 rank 视角的结构
//         offset + XN 基址"，同一 offset 在任何对端解析出等价槽位（peer 无关）；
//     (b) 规约交换律——REDUCE 的折叠次序不影响结果（SUM/MAX 等交换半格）；
//     (c) 双射前提——段内数据帧数 == 通道数，隐式映射 k↔升序第 k peer 是双射。
//     未来任一前提失效（动词携带 peer 相关远端 offset、非交换规约、帧数≠通道数），
//     免疫静默失效——届时合并键（owner 已在键内）从"配对敏感增强"升为**必选**，
//     本登记即升级触发器的判据清单。
//   ② host checker 能力极限：本 checker 只能验证"帧内容 ↔ checker 的 server 序
//     模型（升序通道表）"一致性；"server 的通道表真为升序"不在测试可达面——
//     由生成方 M0 的单源构造保证（通道表与隐式映射同源派生，主会话已下发要求）。
//     若通道表非升序，本 checker 全绿但真机错跑——该风险归生成方构造面，
//     ST 侧以本登记声明边界（不伪造覆盖）。
// ---------------------------------------------------------------------------

} // namespace mc2_ops_hccl::refactor::st

#endif
