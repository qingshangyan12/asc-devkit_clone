/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * CANN Open Software License Agreement Version 2.0.
 *
 * R6 · WP-A2 AICPU unified 执行器（替代 v1 摘要的消费方，2026-09-22）
 *
 * 定位（exec 期分叉，R6 主会话裁决）：不注册进 CollAlgExecRegistryV2、不改任何存量
 * executor——消费方是独立函数 RunUnifiedAicpuOp，挂点在设备侧 Orchestrate 调用点前
 * 分叉（kernel_launch.cc 的 GetAlgExec 后 / comm_kfc_open_kernel_executor.cc 同点）。
 * 分叉判定 = opParam.algName == kUnifiedAicpuAlgKey（资源期 unified 成功路径回填，
 * strcmp 全串比对——无前缀/长度陷阱，第四轮 BLOCKER-1 教训）。
 *
 * 消费链（M0 必答③口径——符号布局 + exec 期实例化，方向 B 终裁）：
 *   资源期（GetUnifiedOpParamResCtxImpl）：Compile 产**结构底版** blob 进 ctx 旁路
 *     ——段序/动作 ID/秩布局（domain/pattern）/transport/sync 语义固化；
 *   exec 期（本文件）：param/resCtx → (blob, msg, 资源束) 适配 → 消费方本体
 *     unified_exec/aicpu_consumer.h 的 ConsumeUnifiedPlanAicpu（帧解读 + 现值实例化
 *     + 逐段 Emit 注入 HcommVerbSink——真原语）。本体已由
 *     unified_exec/test/poc_unified_exec_test.cc 53 项单测覆盖（含切轮多段驱动）。
 *
 * exec 期实例化的数据字段清单（M0 打卡终版——对齐 adapter.cc:120-158 覆写集，
 * msg 现值为唯一权威源（R6 终裁 (a)）；头部三槽（槽 4/5/6）仅维测 dump，不作
 * 语义决策输入——不一致时 WARNING 可观测）：
 *   | 字段        | 实例化（exec 现值）                    | blob 结构期已定           |
 *   |-------------|----------------------------------------|---------------------------|
 *   | count       | param.DataDes.count（adapter :142）→   | 段几何比例（槽 4 比例口径 |
 *   |             | sliceSize = count×dts 重算             |  切轮 SplitRounds 的比值）|
 *   | inputPtr    | param.inputPtr（:123，msg.sendBuffer） | COPY/READ 的段内 offset   |
 *   | outputPtr   | param.outputPtr（:124，msg.recvBuffer）| （inBaseOff/outBaseOff/   |
 *   |             |                                        |  peers[].src/dstOff）     |
 *   | dataType    | param.DataDes.dataType（:141 强等照抄， | 头部槽 4（维测 dump，     |
 *   |             |  RS dstDataType 差异被抹=存量行为，     |  与现值不一致时 WARNING） |
 *   |             |  P2 债台账登记）                       |                           |
 *   | strideCount | param.DataDes.strideCount（:143-155：  | —（AG 现状无 input stride）|
 *   |             |  0→count 同式 OrchestrateLoop :166）    |                           |
 *   | reduceType  | param.reduceType（:159；AG 不消费透传） | 头部槽 6（维测 dump）     |
 *   ★ 资源期 count=0（PrepareParamForAllGather :511，M0 实证）→ blob 段记录区的
 *    槽 4（sliceSize 绝对值）不可信；本消费方按 **槽 4 比例** × exec 现值重算
 *    （末段吃余量——SplitRounds 语义的实例化）。
 *
 * v4 blob 消费（头部 7 槽，公共线 WP-B2 落盘口径）：[0]=magic("UPL3") [1]=version(4)
 * [2]=段数 [3]=保留 [4]=dataType [5]=outputType [6]=reduceType（三槽纯维测 dump）；
 * 段记录区 32 槽/段（与 CCU server 同格式、异消费方）。AICPU 侧**不消费段记录区的
 * CCU 专用槽**（goSize/repeatNumInv 等是 CCU kernel 口径）：重建走"段数 + 动作 ID
 * 对账 + exec 现值重算"，段记录区仅取槽 0（动作 ID）与槽 4（段间比例）。
 */
#include "hcomm_verb_sink.h"      // l5/：HcommVerbSink（A1 交付，真原语）+ HcommVerbRes
#include "unified_aicpu_exec.h"
#include "aicpu_consumer.h"       // 消费方本体（unified_exec/）：blob 解读 + 现值实例化 + 逐段驱动

#include "alg_param.h"
#include "plan_serializer.h"      // UNIFIED_V3_* 常量（与 kfc_server_message.h 镜像互锁）

#include <cstring>
#include <vector>

namespace mc2_ops_hccl::refactor {

HcclResult RunUnifiedAicpuOp(const OpParam& param, const AlgResourceCtxSerializable& resCtx,
                             HcommVerbSink& sink)
{
    HCCL_INFO("[UnifiedExec][AICPU] start, opType[%u], resCtx[%p], ctxSize[%llu], threads[%zu], "
              "blobAddr[0x%llx], blobSize[%llu].",
        static_cast<u32>(param.opType), param.resCtx,
        static_cast<unsigned long long>(param.ctxSize), resCtx.threads.size(),
        static_cast<unsigned long long>(resCtx.unifiedBlobAddr),
        static_cast<unsigned long long>(resCtx.unifiedBlobSize));

    // ① blob 取回（R11 · resCtx 双语义修复，2026-10-08）：第二通道 = 资源束序列化
    //    字段 unifiedBlobAddr/unifiedBlobSize（方案 B，主会话裁决）——资源期
    //    AssembleUnifiedAicpuResCtx 把 blob 的 device 地址/字节数随资源束序列化
    //    携带，KFC BuildHolder / kernel_launch MISS 反序列化资源束后经 resCtx 形参
    //    自然送达（两存量泛型路径零改动——方案 B 的核心优势）。
    //    R10 形态废除：原"按 param.resCtx/ctxSize 切 u64 槽"——resCtx 语义回归
    //    资源束后 param.ctxSize 是资源束长度（不保证 8 对齐，706%8=2 实测形态），
    //    blob 尺寸判据唯一正确源是 unifiedBlobSize。
    //    三重前置闸（R10 ㊱ 断言迁移非删除——判据改绑新字段源，覆盖面不减）：
    //      闸 1：blobAddr 空 / blobSize 0 → E_PARA（资源期未回填形态必拒）
    //      闸 2：blobSize % 8 != 0 → E_PARA（SerializePlanV3 槽数×8 恒 8 对齐）
    //      闸 3：尺寸不符（槽数 != 7+segs*32）→ ParseUnifiedBlobV4 拒（真入口判据）
    if (resCtx.unifiedBlobAddr == 0U || resCtx.unifiedBlobSize == 0U) {
        HCCL_ERROR("[UnifiedExec][AICPU] unifiedBlobAddr null or unifiedBlobSize 0 "
                   "(unified blob backfill missing in res-ctx).");
        return HCCL_E_PARA;
    }
    if (resCtx.unifiedBlobSize % sizeof(u64) != 0U) {
        HCCL_ERROR("[UnifiedExec][AICPU] unifiedBlobSize %llu not u64-aligned.",
            static_cast<unsigned long long>(resCtx.unifiedBlobSize));
        return HCCL_E_PARA;
    }
    const void* blobDeviceAddr = reinterpret_cast<const void*>(resCtx.unifiedBlobAddr);
    const u64* raw = static_cast<const u64*>(const_cast<void*>(blobDeviceAddr));
    const std::vector<u64> blob(raw, raw + resCtx.unifiedBlobSize / sizeof(u64));

    // 头部校验 + 帧解读：本体 ParseUnifiedBlobV4（三重校验 + 段 ID 对账 +
    // 切轮步长恢复）。此处只取头部三槽做维测 dump 一致性观测（R6 终裁 (a)
    // 交付标准）：dump 值与 exec 期 msg 现值不一致 → WARNING（不阻断——编译期
    // 与 exec 期的漂移可观测，排障抓手）
    BlobView head;
    std::string err;
    if (!ParseUnifiedBlobV4(blob, head, err)) {
        HCCL_ERROR("[UnifiedExec][AICPU] blob rejected: %s", err.c_str());
        return HCCL_E_PARA;
    }
    // 空 Plan 口径（终审收尾件 2——与 M4-④ 写入方拒 segNum=0 统一）：
    // 序列化器对空 Plan 产 header-only blob（segs=0，合法产出），CCU 侧 device
    // 写入方对 segNum==0 拒发。AICPU 消费方同口径**显式拒绝**（不取快路径）：
    // 空计划 = 编译期没有可执行段，静默"成功"会让上层误以为数据已搬运（数据
    // 不一致比报错更危险——与写入方"空计划在 device 侧显式失败，不静默"的
    // 口径自洽）。资源期正常 Compile 至少产 1 段（AG 单段），segs=0 只能来自
    // 篡改/上游 bug——E_PARA 上抛。
    if (head.segNum == 0U) {
        HCCL_ERROR("[UnifiedExec][AICPU] empty plan blob (segs=0) — consistent with writer's "
                   "segNum==0 rejection (M4-④); no segment to orchestrate.");
        return HCCL_E_PARA;
    }
    if (head.dataType != static_cast<u64>(param.DataDes.dataType)) {
        HCCL_WARNING("[UnifiedExec][AICPU] dump dataType[%llu] != msg dataType[%u] "
                     "(header slots are diagnostics-only; msg is authoritative).",
            static_cast<unsigned long long>(head.dataType), static_cast<u32>(param.DataDes.dataType));
    }
    if (head.reduceType != static_cast<u64>(param.reduceType)) {
        HCCL_WARNING("[UnifiedExec][AICPU] dump reduceType[%llu] != msg reduceType[%u] (diagnostics-only).",
            static_cast<unsigned long long>(head.reduceType), static_cast<u32>(param.reduceType));
    }

    // ② 域成员：resCtx.algHierarchyInfo.infos[0][0]（资源期 hierarchyRanks 的
    //    resCtx 落点——topo_match_1d.cc:53-55 同形态；对照 AG 模板构造的
    //    algHierarchyInfo.infos[0] 消费）。空 → 单 rank 回退。
    std::vector<u32> domainRanks;
    if (!resCtx.algHierarchyInfo.infos.empty() && !resCtx.algHierarchyInfo.infos[0].empty()) {
        domainRanks = resCtx.algHierarchyInfo.infos[0][0];   // 单域 PoC 口径
    }
    if (domainRanks.empty()) {
        domainRanks = std::vector<u32>{param.userRank};
    }
    const u32 rankNum = static_cast<u32>(domainRanks.size());
    if (rankNum <= 1U) {
        HCCL_INFO("[UnifiedExec][AICPU] single-rank domain, nothing to orchestrate.");
        return HCCL_SUCCESS;
    }

    // ③ HcommVerbRes 组装（threads + 逐 peer 通道——l5/MakeHcommVerbRes，
    //    kernel_launch.cc 分叉点同式：同源不重复）
    const HcommVerbRes res = MakeHcommVerbRes(resCtx);
    if (res.channels.empty()) {
        HCCL_ERROR("[UnifiedExec][AICPU] no channels in resCtx (VIA_CCUBUF AG needs one per peer).");
        return HCCL_E_PARA;
    }
    // 挂点形参 sink 不再重绑定（HcommVerbSink 不可拷贝赋值——res_ 含 map/vector
    // 的按值束，重绑=重建；消费方本体逐段自建 sink，见 ⑤）。挂点侧的
    // unifiedSink 构造仅提供 resCtx 原始资源的诊断可见性；本函数执行期的
    // 真原语下发全在本体段内 sink。

    // ④ exec 期现值实例化（M0 必答③清单落地）
    const u64 dataCount = param.DataDes.count;                  // adapter :142 注入的现值
    if (dataCount == 0U) {
        HCCL_ERROR("[UnifiedExec][AICPU] exec-time count is 0 (msg injection missing?).");
        return HCCL_E_PARA;
    }
    const HcclDataType dataType = static_cast<HcclDataType>(param.DataDes.dataType);
    if (dataType < 0 || dataType >= HCCL_DATA_TYPE_RESERVED ||
        DATATYPE_SIZE_TABLE[dataType] == 0U) {
        HCCL_ERROR("[UnifiedExec][AICPU] dataType %u invalid or zero size.",
            static_cast<u32>(dataType));
        return HCCL_E_PARA;
    }
    const u32 dts = DATATYPE_SIZE_TABLE[dataType];   // 日志口径（段几何由本体实例化）
    // ④' exec 期 msg 现值组装（消费方本体的实例化输入——方向 B 字段集：
    //    count/inputAddr/outputAddr/strideCount/dataType/repeatCnt/reduceType/cclBuff）
    UnifiedExecMsg msg;
    msg.count = dataCount;
    msg.inputAddr = reinterpret_cast<u64>(param.inputPtr);      // adapter :123（msg.sendBuffer）
    msg.outputAddr = reinterpret_cast<u64>(param.outputPtr);    // adapter :124（msg.recvBuffer）
    msg.strideCount = param.DataDes.strideCount;                // adapter :143-155（0→count 由本体 strideFallback 处理）
    msg.dataType = static_cast<u32>(param.DataDes.dataType);    // adapter :141 强等照抄（P2 债台账）
    msg.repeatCnt = 1U;                                         // AG 单 turn（repeat 由 exec 链逐 turn 重入）
    msg.reduceType = static_cast<int>(param.reduceType);        // adapter :159（AG 不消费透传）
    msg.cclBuff = resCtx.cclMem;                                // 本地中转 buffer 现址

    // ⑤ 消费方本体驱动（unified_exec/aicpu_consumer.h）：帧解读 + 现值实例化 +
    //    逐段 Emit（段内自建 sink——READ 线程序按段从 threads[0] 起算，存量
    //    KernelRun 逐轮重入同型）。外层 sink 参数保留（挂点签名兼容；本体的
    //    perSegTrace 即其调用记录的 TraceSink 重放——诊断同源）。
    const UnifiedExecResult r = ConsumeUnifiedPlanAicpu(
        blob, msg, param.userRank, domainRanks,
        static_cast<u32>(param.opType), res);
    if (r.ret != HCCL_SUCCESS) {
        HCCL_ERROR("[UnifiedExec][AICPU] ConsumeUnifiedPlanAicpu failed: %s (segments run %u).",
            r.err.c_str(), r.segmentsRun);
        return r.ret;
    }
    (void)sink;   // 本体段内自建 sink（见上注）；挂点形参保留供后续轮直接注入
    HCCL_INFO("[UnifiedExec][AICPU] done: segs[%u], slice[%lluB], peers[%zu], threads[%zu].",
        r.segmentsRun, static_cast<unsigned long long>(msg.count * dts),
        res.channels.size(), res.threads.size());
    return HCCL_SUCCESS;
}

} // namespace mc2_ops_hccl::refactor
