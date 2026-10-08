/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * CANN Open Software License Agreement Version 2.0.
 *
 * unified PoC · ST 语义 checker 实现（host 侧纯逻辑）
 * 接口契约见 st_semantic_checker.h。trace 格式与 l4 两后端 Verb() 的拼接逐字对应：
 *   AICPU: verb(src->dst,size[,peerN]) —— backend_aicpu_collect_all.h:77
 *   CCU:   verb(a1,a2,a3)              —— backend_ccu_collect_all.h:73
 * 后端格式变更必须同步本文件（这就是"占位 trace"的最低限度契约）。
 */
#include "st_semantic_checker.h"

#include <cstdlib>
#include <cstring>   // R11：镜像字段的字节序读写（memcpy 语义经逐字节移位实现）

namespace mc2_ops_hccl::refactor::st {

// u32/u64 双编译面注入（本 TU 不自定义——action_spec.h:24 同口径）：
//   shim 面：fake_alg_param.h 全局 using（经 shim/alg_param.h 转发）
//   devkit 面：base.h extern "C" typedef（经 alg_param.h 链带出，全局可见）
// 两面下 ::u32/::u64 均可用——此处仅显式指名全局域，避免依赖 using 注入点差异。
using ::u32;
using ::u64;

namespace {

// 无符号十进制解析；失败返回 false（不用 stoull 的异常路径，C++14 -Werror 干净）
bool ParseU64(const std::string& s, u64& out)
{
    if (s.empty() || s.size() > 20) return false;
    for (size_t i = 0; i < s.size(); ++i) {
        const char c = s[i];
        if (c < '0' || c > '9') return false;
    }
    out = std::strtoull(s.c_str(), nullptr, 10);
    return true;
}

// 解析单个 "(...)" 参数组：返回消耗的结束位置；vals 依次填 a1/a2/a3
// 支持两种形制：AICPU 的 "src->dst,size[,peerN]"（-> 拆成 a1,a2）与 CCU 的 "a1,a2,a3"
bool ParseArgGroup(const std::string& s, size_t open, VerbRecord& r, size_t& close)
{
    if (open >= s.size() || s[open] != '(') return false;
    size_t pos = open + 1;
    std::vector<std::string> toks;              // 逗号/-> 切分出的原始段
    std::vector<char> seps;                     // 每段前的分隔符（peer 前必为逗号）
    std::string cur;
    for (size_t i = pos; i < s.size(); ++i) {
        const char c = s[i];
        if (c == ',' || (c == '-' && i + 1 < s.size() && s[i + 1] == '>')) {
            toks.push_back(cur);
            seps.push_back(c);
            cur.clear();
            if (c == '-') ++i;                  // 跳过 '>' 的一半
            continue;
        }
        if (c == ')') {
            toks.push_back(cur);
            close = i;
            // 归位：a1/a2/a3
            if (toks.size() < 2 || toks.size() > 4) return false;
            u64 tmp = 0;
            if (!ParseU64(toks[0], tmp)) return false;
            r.a1 = tmp;
            if (!ParseU64(toks[1], tmp)) return false;
            r.a2 = tmp;
            if (toks.size() >= 3) {
                if (seps.size() < 2) return false;
                if (!ParseU64(toks[2], tmp)) return false;
                r.a3 = tmp;
            }
            if (toks.size() == 4) {
                // 第 4 段必须形如 peerN（AICPU read 专属）
                if (seps.size() < 3 || seps[2] != ',') return false;
                if (toks[3].rfind("peer", 0) != 0) return false;
                const std::string peerNum = toks[3].substr(4);
                if (!ParseU64(peerNum, tmp)) return false;
                r.peer = static_cast<u32>(tmp);
            }
            return true;
        }
        cur.push_back(c);
    }
    return false;                                // 无闭括号
}

} // namespace

std::vector<VerbRecord> ParseTrace(const std::string& trace, std::string& err)
{
    err.clear();                                  // 成功路径也清 err（约定：返回后 err 空即成功）
    std::vector<VerbRecord> out;
    size_t i = 0;
    while (i < trace.size()) {
        // 跳过分隔空白
        while (i < trace.size() && trace[i] == ' ') ++i;
        if (i >= trace.size()) break;
        // 动词名：到 '(' 为止
        const size_t open = trace.find('(', i);
        if (open == std::string::npos || open == i) {
            err = "trace format drift: verb name expected at offset " + std::to_string(i);
            return {};
        }
        VerbRecord r;
        r.verb = trace.substr(i, open - i);
        size_t close = 0;
        if (!ParseArgGroup(trace, open, r, close)) {
            err = "trace format drift: bad arg group after '" + r.verb + "'";
            return {};
        }
        // 动词合法性（词汇表子集；后端新增动词必须扩这里 + 等价规则）
        if (r.verb == "syncPeer:pre") {
            r.isSyncPre = true;
        } else if (r.verb == "syncPeer:post") {
            r.isSyncPost = true;
        } else if (r.verb != "copy" && r.verb != "read" && r.verb != "broadcast" &&
                   r.verb != "postCopy") {
            // write/reduce 枚举位已预留但未启用（P2 ReduceSlice 落地时扩）——先拒并点名
            err = "unknown verb '" + r.verb + "' (vocabulary extended? sync checker)";
            return {};
        }
        out.push_back(r);
        i = close + 1;
    }
    if (out.empty()) {
        err = "empty trace";
        return {};
    }
    return out;
}

namespace {

// 屏障结构（与 l4 两后端 Emit 顺序一致，§6.13.2 对照表）：
//   AICPU: copy(本地归位) → syncPeer:pre → read×(N-1) → syncPeer:post
//   CCU:   syncPeer:pre → broadcast → syncPeer:post
// 规则：pre 在首个**远端**数据动词（read/broadcast）之前——本地 copy 不依赖 peer，
// 允许在屏障前；post 在末个数据动词之后。
bool BarrierStructureOk(const std::vector<VerbRecord>& t, std::string& err)
{
    // 远端数据动词 = read / broadcast（copy 是本地动作，不受屏障保护）
    size_t firstRemote = 0;
    size_t last = 0;
    bool foundRemote = false;
    for (size_t i = 0; i < t.size(); ++i) {
        if (t[i].verb != "read" && t[i].verb != "broadcast") continue;
        if (!foundRemote) { firstRemote = i; foundRemote = true; }
        last = i;
    }
    for (size_t i = 0; i < t.size(); ++i) {
        if (t[i].isSyncPre && foundRemote && i > firstRemote) {
            err = "syncPeer:pre appears after first remote data verb (idx " + std::to_string(i) + ")";
            return false;
        }
        if (t[i].isSyncPost && i < last) {
            err = "syncPeer:post appears before last data verb (idx " + std::to_string(i) + ")";
            return false;
        }
    }
    return true;
}

} // namespace

int CheckSemanticsEquivalent(const std::vector<VerbRecord>& traceAicpu,
                              const std::vector<VerbRecord>& traceCcu,
                              const ActionSpec& seg, u32 domainSize, std::string& err)
{
    (void)seg;
    // ① 屏障结构同构（两引擎各自满足）
    if (!BarrierStructureOk(traceAicpu, err)) {
        err = "[AICPU] " + err;
        return 1;
    }
    if (!BarrierStructureOk(traceCcu, err)) {
        err = "[CCU] " + err;
        return 2;
    }
    // ② 搬运量守恒（§6.13.2：broadcast ≈ copy + read×(N-1)）
    //    AICPU 侧：Σ copy+read 的 a3（copy 本地块 + read 的 N-1 块）。
    //    postCopy 不计入——本地 scratch→output 的二段搬运（VIA_CCUBUF 路径的开销动词，
    //    §10.2 B4），不改变语义搬运量（CCU 侧该步并入 broadcast 的 fan-out）。
    u64 aicpuBytes = 0;
    u32 readCount = 0;
    for (const auto& v : traceAicpu) {
        if (v.verb == "read") { aicpuBytes += v.a3; ++readCount; }
        if (v.verb == "copy") { aicpuBytes += v.a3; }
    }
    u64 ccuSlice = 0;
    u32 bcastCount = 0;
    for (const auto& v : traceCcu) {
        if (v.verb == "broadcast") { ccuSlice = v.a3; ++bcastCount; }
    }
    if (readCount + 1 != domainSize) {
        err = "AICPU read count " + std::to_string(readCount) + " != domainSize-1 (" +
              std::to_string(domainSize - 1) + ")";
        return 3;
    }
    // 全量拼齐口径：copy(本地) + read×(N-1) == sliceSize × N
    // 尾 rank 场景（tailSize≠0）各块大小不同，口径放宽为 == Σ 各块（由 caller 用
    // SliceSizeOf 预算的期望值传入 domainSize==0 时跳过本条——保留扩展位）
    const u64 expectedTotal = ccuSlice * domainSize;
    if (aicpuBytes != expectedTotal) {
        err = "bytes moved mismatch: AICPU " + std::to_string(aicpuBytes) + " vs CCU-broadcast-derived " +
              std::to_string(expectedTotal);
        return 4;
    }
    if (bcastCount != 1) {
        err = "CCU broadcast count " + std::to_string(bcastCount) + " != 1";
        return 5;
    }
    return 0;
}

int CheckPostCopySemantics(const std::vector<VerbRecord>& trace, bool expectPostCopy, std::string& err)
{
    // 末个远端数据动词位置（postCopy 必须全部在其后——真实 PostLocalCopy 在收发完成后执行，
    // ins_temp_all_gather_mesh_1D.cc:278：外层逐 peer 回拷，位于 PostSync 语义之后；
    // 动词层约束取"末个远端数据动词之后"）
    size_t lastRemote = 0;
    bool foundRemote = false;
    u32 postCopyCount = 0;
    for (size_t i = 0; i < trace.size(); ++i) {
        if (trace[i].verb == "read" || trace[i].verb == "broadcast") {
            lastRemote = i;
            foundRemote = true;
        }
    }
    for (size_t i = 0; i < trace.size(); ++i) {
        if (trace[i].verb != "postCopy") continue;
        ++postCopyCount;
        if (foundRemote && i < lastRemote) {
            err = "postCopy at idx " + std::to_string(i) + " before last remote data verb (idx " +
                  std::to_string(lastRemote) + ")";
            return 1;
        }
    }
    if (expectPostCopy && postCopyCount == 0) {
        err = "expected postCopy (VIA_CCUBUF: scratch->output writeback) but none found";
        return 2;
    }
    if (!expectPostCopy && postCopyCount != 0) {
        err = "unexpected postCopy x" + std::to_string(postCopyCount) +
              " (SYMMEM/MEM2MEM paths must not write back through scratch)";
        return 3;
    }
    return 0;
}

int CheckStageHandoff(const ActionSpec& rsSeg, const ActionSpec& agSeg, std::string& err)
{
    // B2 决策（§12-9）：RS 段 output 落 scratch、AG 段 input 取同一 scratch——
    // 数据流边的物理兑现。断言口径：两段 layout 的 scratch 几何逐值相等
    //（同 scratchBaseOff、同 sliceSize 步长、同 rankNum 维度 → ScratchOffset(i) 逐值同）。
    const DataLayout& rl = rsSeg.layout;
    const DataLayout& al = agSeg.layout;
    if (rl.scratchBaseOff != al.scratchBaseOff) {
        err = "scratchBaseOff mismatch: RS " + std::to_string(rl.scratchBaseOff) + " vs AG " +
              std::to_string(al.scratchBaseOff) + " (handoff broken: RS writes A, AG reads B)";
        return 1;
    }
    if (rl.sliceSize != al.sliceSize) {
        err = "sliceSize mismatch: RS " + std::to_string(rl.sliceSize) + " vs AG " +
              std::to_string(al.sliceSize) + " (scratch slot geometry differs)";
        return 2;
    }
    if (rsSeg.domain.Size() != agSeg.domain.Size()) {
        err = "domain size mismatch: RS " + std::to_string(rsSeg.domain.Size()) + " vs AG " +
              std::to_string(agSeg.domain.Size());
        return 3;
    }
    // 逐 peer 逐值：ScratchOffset(i) 公式 = scratchBaseOff + sliceSize*i（rpt=0）
    const u32 n = rsSeg.domain.Size();
    for (u32 i = 0; i < n; ++i) {
        const u64 rsOff = rl.ScratchOffset(i, n);
        const u64 agOff = al.ScratchOffset(i, n);
        if (rsOff != agOff) {
            err = "scratch offset for peer#" + std::to_string(i) + ": RS " + std::to_string(rsOff) +
                  " vs AG " + std::to_string(agOff) + " (handoff broken)";
            return 4;
        }
    }
    // AG 段的取数入口应是 scratch 口径（InOffset 的 inBaseOff 指 scratch 源）——
    // 由 caller 按 unified 的中转建模细节断言（ForSecondStageViaScratch 的字段映射），
    // 本函数守住 scratch 几何同源这一层。
    return 0;
}

int CheckPlanBook(const Plan& plan, const std::vector<SegmentExpect>& expect, std::string& err)
{
    if (plan.segments.size() != expect.size()) {
        err = "segment count " + std::to_string(plan.segments.size()) + " != expected " +
              std::to_string(expect.size());
        return 1;
    }
    for (size_t i = 0; i < expect.size(); ++i) {
        const ActionSpec& s = plan.segments[i];
        const SegmentExpect& e = expect[i];
        if (s.kind != e.kind) {
            err = "seg#" + std::to_string(i) + " kind mismatch";
            return 2;
        }
        if (e.pattern != CommPattern::AUTO && s.pattern != e.pattern) {
            err = "seg#" + std::to_string(i) + " pattern mismatch";
            return 3;
        }
        if (s.shape != e.shape) {
            err = "seg#" + std::to_string(i) + " shape mismatch";
            return 4;
        }
    }
    return 0;
}

int CheckDataflowEdges(const Plan& plan, bool expectDataflowEdges, std::string& err)
{
    for (size_t i = 0; i < plan.segments.size(); ++i) {
        const ActionSpec& s = plan.segments[i];
        if (s.inputRanks.empty()) {
            err = "seg#" + std::to_string(i) + " has empty inputRanks (orphan segment)";
            return 1;
        }
        if (expectDataflowEdges && i > 0) {
            const ActionSpec& prev = plan.segments[i - 1];
            if (s.inputRanks != prev.outputRanks) {
                err = "seg#" + std::to_string(i) + " inputRanks != seg#" + std::to_string(i - 1) +
                      " outputRanks (dataflow edge broken)";
                return 2;
            }
        }
    }
    return 0;
}

int CheckRounds(const std::vector<RoundRecord>& rounds, u64 totalSliceSize, u64 maxPerRound,
                std::string& err)
{
    if (rounds.empty()) {
        err = "no rounds";
        return 1;
    }
    u64 sum = 0;
    u64 prevBase = 0;
    for (size_t i = 0; i < rounds.size(); ++i) {
        const RoundRecord& r = rounds[i];
        if (r.sliceSize == 0) {
            err = "round#" + std::to_string(i) + " zero sliceSize";
            return 2;
        }
        if (r.sliceSize > maxPerRound) {
            err = "round#" + std::to_string(i) + " sliceSize " + std::to_string(r.sliceSize) +
                  " > maxPerRound " + std::to_string(maxPerRound);
            return 3;
        }
        // 非尾轮必须打满（整除时尾轮也打满——统一断言 i<size-1 或整除）
        const bool isLast = (i == rounds.size() - 1);
        if (!isLast && r.sliceSize != maxPerRound) {
            err = "round#" + std::to_string(i) + " (non-last) sliceSize " + std::to_string(r.sliceSize) +
                  " != maxPerRound " + std::to_string(maxPerRound);
            return 4;
        }
        if (i > 0 && r.baseOff <= prevBase) {
            err = "round#" + std::to_string(i) + " baseOff not strictly increasing";
            return 5;
        }
        // 首轮 baseOff 由 caller 约定（通常 0），从第二轮起校验推进量
        if (i > 0 && r.baseOff - prevBase != rounds[i - 1].sliceSize) {
            err = "round#" + std::to_string(i) + " baseOff advance != prev sliceSize";
            return 6;
        }
        prevBase = r.baseOff;
        sum += r.sliceSize;
    }
    if (sum != totalSliceSize) {
        err = "sum(sliceSize) " + std::to_string(sum) + " != total " + std::to_string(totalSliceSize);
        return 7;
    }
    return 0;
}

// ---------------------------------------------------------------------------
// ⑤ KFC server v3 轮级分发模拟器（A4 甲路线）。语义镜像 KfcServerGenCircularQueue
// （ccu_kernel_kfc_server.cc:351-410），逐行对应关系：
//   server: repeatCond 循环 → 本函数 while 游标推进
//   server: LoadFuncParamFromMemory(paramAddr, param) 读 32 槽 → 取 serialized 的
//           第 turn*32 .. turn*32+31 个 u64（CCU_PARAM_NUM_PER_DIE=32）
//   server: CCU_IF(param[0]==INVALID_U64) 出口 → 哨兵轮终止，哨兵轮不产生动作
//   server: DispatchUnifiedPlanV3（v3 新增，:198-240）按槽 0 动作 ID 分派
//           → 本函数查 legalActionIds 白名单。server 微码对未知 ID 只打日志继续
//           （:226-233 注释：无同步回传通道、返回值丢弃——v2 同惯例）；模拟器把
//           "静默掉帧"显式化为错误，供 host 侧拒收——这是 ST 加严口径，差异点
//           见 R4 断言注释（真注册链落地后 host Validate 拦截，server 只收已校验 ID）
//   server: paramAddr += CCU_PARAM_NUM_MAX*8（64 槽双 die 几何）→ 游标按帧 +1
//           （v3 blob 每段 32 槽占一帧；server 读前 32 槽、跳后 32 槽——序列化产物
//             只含前 32 槽，等价）
//   server: queueIdx == CCU_TASK_NUM_MAX 环形回绕（:401-407）→ wrapped 标记 + 上限防御
//   注意：v3 blob 头部 4 槽（magic/version/段数/保留）不是 server 帧记录——模拟器
//   只消费段记录区（caller 剥头），头部由 R4 断言单独校验。
// ---------------------------------------------------------------------------
ServerSimResult SimulateKfcServerV3(const std::vector<u64>& serialized,
                                    const std::vector<u64>& legalActionIds)
{
    ServerSimResult out;
    if (serialized.empty()) {
        out.err = "empty payload (server would spin on garbage frame)";
        return out;
    }
    // 协议防御①：载荷必须整帧对齐（32 槽/帧）——半帧意味着序列化器写错或被截断，
    // server 读半帧会拿到上一轮残影的槽值（静默数据错乱，困难点 2 同型）
    if (serialized.size() % ST_KFC_V3_SLOTS_PER_TURN != 0) {
        out.err = "payload size " + std::to_string(serialized.size()) +
                  " not a multiple of 32 slots per turn";
        return out;
    }
    const u64 frameCount = serialized.size() / ST_KFC_V3_SLOTS_PER_TURN;
    for (u64 turn = 0; turn < frameCount; ++turn) {
        // 协议防御②：server 环形队列容量 CCU_TASK_NUM_MAX=64 帧（:380/:401）——
        // 超容量的段永远等不到信号（回绕覆盖未决帧），必须在编译期拒绝
        if (turn >= ST_KFC_V3_MAX_TURNS) {
            out.err = "turn " + std::to_string(turn) + " exceeds circular queue capacity (64)";
            out.rawTurnCount = turn;
            return out;
        }        out.rawTurnCount = turn + 1;
        ServerTurnRecord rec;
        rec.turnIndex = turn;
        rec.slots.assign(serialized.begin() + turn * ST_KFC_V3_SLOTS_PER_TURN,
                         serialized.begin() + (turn + 1) * ST_KFC_V3_SLOTS_PER_TURN);
        rec.actionId = rec.slots[0];
        if (rec.actionId == ST_KFC_V3_SENTINEL) {
            break;                       // 出口哨兵：repeatCond=1，本轮不派发（:389）
        }
        // v3 轮级分派（甲路线核心）：槽 0 = 动作 ID，必须在 caller 给的合法集合内。
        // v2 同槽位是 GetOpId 打包的 op 类型位域（hccl_ccu_xn_utils.h:42，恒非零）；
        // v3 的 CollectAll ID == 0（ActionKind 值）——模拟器不硬编码拒 0（0 是合法
        // v3 ID），合法性完全由白名单裁决；未登记 ID = 写入方与 host 白名单漂移，
        // 拒发点名（server 微码对未知 ID 只打日志继续——:226-233，ST 加严显式化）。
        bool legal = false;
        for (u64 id : legalActionIds) {
            if (id == rec.actionId) { legal = true; break; }
        }
        if (!legal) {
            out.err = "turn " + std::to_string(turn) + ": unknown action id " +
                      std::to_string(rec.actionId) + " (v3 dispatch would fall through)";
            return out;
        }
        out.turns.push_back(rec);
    }
    // 协议防御③：整圈无出口哨兵——server 循环回绕后重读 0 号帧，永不退出
    //（queueIdx==64 回绕 :401-407）。哨兵由 device 写入方追加（机制轮约定，
    // kfc_server_message.h:201-203）；payload 是环形队列内容 → 全圈无哨兵 = 写入方
    // 漏哨兵，server 将无限重放 0 号帧（静默死循环）——ST 显式化为错误。
    // 判定口径：载荷撑满整圈 64 帧**且全部是动作帧**（turns==64，无哨兵消费）。
    // 不足 64 帧的"裸段记录区"是合法中间状态（机制轮哨兵未随 blob 序列化——
    // caller 剥头后直接送模拟器，视为"队列尚有空位、写入方还可能追加"，不做
    // 哨兵要求）；满 64 帧且第 64 帧是哨兵（turn 63 哨兵 → turns==63）也是合法
    // 边界（哨兵占最后一个队列槽位、回绕前到达）。wrapped 标记环回将发生。
    if (frameCount >= ST_KFC_V3_MAX_TURNS && out.err.empty() &&
        out.turns.size() == ST_KFC_V3_MAX_TURNS) {
        out.wrapped = true;
        out.err = "no exit sentinel in payload (server would spin on wrap, replaying frame 0)";
    }
    return out;
}

// ---------------------------------------------------------------------------
// ⑤b RS（REDUCE_SLICE）帧的 kernel 实参抽取（R7）。逐参对照 server v3 分支的
// 转发序（ccu_kernel_kfc_server.cc:228-238）——本函数是该调用点的 host 镜像：
//   server 第 1 参 inputAddr                     ← 本函数 rec.inputAddr         ← 槽 1
//   server 第 2 参 outputAddr                    ← rec.outputAddr               ← 槽 2
//   server 第 3 参 tokenInfo = ctx.token         ←（ST 不可观测，不记录）
//   server 第 4 参 scratch                       ← rec.scratch                  ← 槽 3
//   server 第 5 参 currentRankSliceInputOffset   ← rec.currentRankSliceInputOffset ← 槽 4
//   server 第 6 参 chunkSize                     ← rec.chunkSize                ← 槽 5
//   server 第 7 参 chunkLoopNum                  ← rec.chunkLoopNum             ← 槽 6
//   server 第 8 参 tailSize                      ← rec.tailSize                 ← 槽 7
//   server 第 9..12 参 fullGo 四元组             ← rec.fullGo*                 ← 槽 8..11
//   server 第 13..16 参 tailGo 四元组            ← rec.tailGo*                 ← 槽 12..15
//   server 第 17..18 参 channels/channelCount    ← rec.channelCount（句柄不记录）← ctx
//   server 第 19..20 参 rankSize/rankId          ← rec.rankSize / rec.rankId   ← ctx
//   server 第 21..23 参 dataType/outputType/reduceType ← rec.dataType/outputType/
//   reduceType                                                            ← ctx（opParam 投影）
// 槽序本身（1..15 的语义）在协议头 kfc_server_message.h 的
// KFC_UNIFIED_REDUCE_SLICE_* 与 KFC_CONCURRENT_RS_MESH_* 逐槽 static_assert 锁定
//（:377-405 区间，v3/v2 读取序复用），ST 侧不重复声明枚举——期望值纪律。
// ---------------------------------------------------------------------------
RsKernelArgsRecord ExtractRsKernelArgs(const std::vector<u64>& frame, const RsServerCtx& ctx)
{
    RsKernelArgsRecord rec;
    if (frame.size() < ST_KFC_V3_SLOTS_PER_TURN) return rec;   // 半帧防御（同模拟器口径）
    if (frame[0] != ST_KFC_V3_ACTION_REDUCE_SLICE) return rec; // 非 RS 帧：不越权
    rec.valid = true;
    rec.inputAddr = frame[1];
    rec.outputAddr = frame[2];
    rec.scratch = frame[3];
    rec.currentRankSliceInputOffset = frame[4];
    rec.chunkSize = frame[5];
    rec.chunkLoopNum = frame[6];
    rec.tailSize = frame[7];
    rec.fullGoAddrOffset = frame[8];
    rec.fullGoLoopParam = frame[9];
    rec.fullGoParallelParam = frame[10];
    rec.fullGoResidual = frame[11];
    rec.tailGoAddrOffset = frame[12];
    rec.tailGoLoopParam = frame[13];
    rec.tailGoParallelParam = frame[14];
    rec.tailGoResidual = frame[15];
    rec.rankSize = ctx.rankSize;
    rec.rankId = ctx.rankId;
    rec.channelCount = ctx.channelCount;
    rec.dataType = ctx.dataType;
    rec.outputType = ctx.outputType;
    rec.reduceType = ctx.reduceType;
    return rec;
}

// ---------------------------------------------------------------------------
// ⑤c RS 帧的 exec 现值实例化（R7 ST 自有 host 公式实现）。
//
// ★实现来源声明：本函数公式逐式从 v2 设备侧写入方源码独立推导（ST 考古
// hccl_ccu_v0_prepare.h 的 CcuPrepareForConcurrentReduceScatterM2M :528-575 与
// R7 覆写实现 :787-823 + 设备侧 CalcGoSize hccl_ccu_xn_utils.h:148-175 +
// GetParallelParameters 位域 :121-124），**不转抄生成方 unified_exec/test 里
// 已有的 OverwriteReduceSliceFrame**（同名公式两份独立实现互为交叉锚——
// 生成方数字与本实现若不一致，两份测试独立报错，单边改数无法对齐过关）。
//
// 逐式对照（行号 = hccl_ccu_v0_prepare.h）：
//   dts           = exec 期 dataType 字节宽            （:737 GetHcclDataTypeSize）
//   sliceSize     = count * dts                         （:738；RS 语义 count=每 rank
//                  输出份元素数——selector 口径，与 v2 :538 的 sliceSize 同源）
//   repeatOffset  = count * repeatIndex * dts           （:749，v2 :539 同式）
//   槽 1 input    = sendBuf + repeatOffset + blob 槽 1   （:791-792，v2 :543/:548——
//                  合成式：blob 槽值是段内 inBaseOff 结构量；v2 的 inputBase
//                  :543 + 槽写入 :548 两行展开即此合成式）
//   槽 2 output   = recvBuf + repeatOffset + blob 槽 2   （:793-794，v2 :544/:549）
//   槽 3 scratch  = ccuParam_.scratchAddr 直替           （:796，v2 :550——blob 编译期
//                  值是 scratchBaseOff 占位，无结构量可保留）
//   槽 4 rankSliceOffset = rankId × (strideCount==0 ? sliceSize
//                                                : strideCount×dts)（:798-799，v2 :551——
//                  v2 的 sliceStride 三分支在 :542 先算再乘 rankId）
//   槽 5 chunkSize = 64MB / rankNum / 128 × 128          （:802-804，v2 :552-553 双端
//                  契约常量；rankNum 来自 ccuParam_——ST 由 caller 注入）
//   fullChunkCount = (sliceSize==0||chunkSize==0) ? 0 : (sliceSize-1)/chunkSize （:805）
//   chunkCount     = 同条件 ? 0 : fullChunkCount+1       （:806，v2 :554-555）
//   槽 6 chunkLoopNum = UINT64_MAX - chunkCount          （:809，v2 :558）
//   槽 7 tailSize  = sliceSize - fullChunkCount*chunkSize（:807/:810，v2 :556）
//   槽 8..11 fullGo = CalcGoSize(chunkSize, 16, 4096)    （:814-815，v2 :560-561——
//                  ★RS config：loopCount=CCU_LOOP_COUNT_M2M_RE=16、memSlice=
//                  CCU_MEMSLICE_SIZE=4096 不 ×8，与 AG 的 {8, 32768} 分立，
//                  kernel :26-30 步进错位硬约束）
//   槽 12..15 tailGo = CalcGoSize(tailSize, 16, 4096)    （:816-817，v2 :562-563）
// CalcGoSize 四分支（hccl_ccu_xn_utils.h:148-175）：m/n/p 三段分解 + GetParallelParameters
// 位域（:121-124：(rn&0x7f)<<55 | (rli&0x7f)<<48 | (tln&0x7f)<<41）——n!=0&&p!=0 分支
// totalLoopNum 取常量 2（CCU_PARAM_INDEX，:35/:172）。
// ---------------------------------------------------------------------------
// StRsExecMsg（exec 现值）定义在头（调用方构造输入需要字段面）；此处不再重复。

// ST 独立实现的 CalcGoSize（RS config 口径：loopCount=16、memSlice=4096）。
// 输入 size，输出四元组 {addrOffset, loopParam, parallelParam, residual}。
static void StCalcRsGoSize(u64 size, u64 (&go)[4])
{
    const u64 loopCount = 16;
    const u64 memSlice = 4096;
    const u64 loopSize = loopCount * memSlice;   // 65536
    const u64 m = size / loopSize;
    const u64 n = (size - m * loopSize) / memSlice;
    const u64 p = size - m * loopSize - n * memSlice;
    go[0] = loopSize * m;
    go[1] = m;
    const u64 mask = (1ULL << 7) - 1;
    const auto pp = [mask](u64 rn, u64 rli, u64 tln) {
        return ((rn & mask) << 55) | ((rli & mask) << 48) | ((tln & mask) << 41);
    };
    if (n == 0 && p == 0) {
        go[2] = 0; go[3] = 0;
    } else if (n != 0 && p == 0) {
        go[2] = pp(n - 1, 0, 1); go[3] = memSlice;
    } else if (n == 0 && p != 0) {
        go[2] = pp(0, 0, 1); go[3] = p;
    } else {
        go[2] = pp(n - 1, 1, 2); go[3] = p;
    }
}

// RS 帧实例化（输入 = 资源期 blob 帧 32 槽 + exec 现值；输出 = 落盘帧 32 槽）。
// 与设备函数 CcuPrepareForUnifiedPlan RS 分支同构：先逐槽拷贝（:778-780），再按
// 上方逐式对照覆写槽 1..15。槽 0 动作 ID 与槽 16..31 不动（透传语义）。
std::vector<u64> StInstantiateRsFrame(const std::vector<u64>& blobFrame, const StRsExecMsg& m)
{
    std::vector<u64> r = blobFrame;     // 逐槽拷贝（设备函数 :778-780 同构）
    const u64 sliceSize = m.count * m.dts;
    const u64 repeatOffset = m.count * m.repeatIndex * m.dts;
    r[1] = m.sendBuf + repeatOffset + blobFrame[1];
    r[2] = m.recvBuf + repeatOffset + blobFrame[2];
    r[3] = m.scratchAddr;
    r[4] = m.rankId * ((m.strideCount == 0U) ? sliceSize : (m.strideCount * m.dts));
    constexpr u64 kRsScratchSize = 64ULL * 1024 * 1024;
    constexpr u64 kRsSplitAlign = 128ULL;
    const u64 chunkSize = kRsScratchSize / m.rankNum / kRsSplitAlign * kRsSplitAlign;
    const u64 fullChunkCount = (sliceSize == 0U || chunkSize == 0U) ? 0 : (sliceSize - 1) / chunkSize;
    const u64 chunkCount = (sliceSize == 0U || chunkSize == 0U) ? 0 : fullChunkCount + 1;
    const u64 tailSize = sliceSize - fullChunkCount * chunkSize;
    r[5] = chunkSize;
    r[6] = ~0ULL - chunkCount;
    r[7] = tailSize;
    u64 fullGo[4];
    u64 tailGo[4];
    StCalcRsGoSize(chunkSize, fullGo);
    StCalcRsGoSize(tailSize, tailGo);
    for (u32 i = 0; i < 4; ++i) {
        r[8 + i] = fullGo[i];
        r[12 + i] = tailGo[i];
    }
    return r;
}

// ---------------------------------------------------------------------------
// R10 · F1 断链可判基线实现（声明契约见 st_semantic_checker.h 的 R10 节注）。
//
// ★实现来源声明：三个函数都是消费方/分叉点判据的 1:1 数学镜像（不 include 被测方
// 的 unified_aicpu_exec.cc / aicpu_consumer.h——真 .cc 在真 alg_param/CANN 编译面
// host 不可链；镜像判据的值锚定由 st_verification.cc R10 节用**真** ParseUnifiedBlobV4 /
// IsUnifiedAicpuAlgName 直测交叉验证，防"镜像自证"）。
// ---------------------------------------------------------------------------

// v1 文本 → u64 槽视图（floor(len/8)，尾字节丢弃——消费方 const u64* raw +
// ctxSize/sizeof(u64) 的同一取整口径，unified_aicpu_exec.cc:78-79）。
std::vector<u64> StBytesToBlobSlots(const std::string& v1Text)
{
    std::vector<u64> slots;
    const size_t nSlots = v1Text.size() / sizeof(u64);
    slots.reserve(nSlots);
    for (size_t i = 0; i < nSlots; ++i) {
        u64 v = 0;
        for (size_t b = 0; b < sizeof(u64); ++b) {
            v |= (static_cast<u64>(static_cast<unsigned char>(v1Text[i * sizeof(u64) + b]))) << (8 * b);
        }
        slots.push_back(v);   // 小端组槽（device 内存视图）
    }
    return slots;
}

// v1 产物喂 v4 消费方入口层的必拒判据（通用论证，与段数/版本/长度无关）：
//   v1 恒以 "PL" 开头（"PLAN v"+版本，SerializePlanForCtx :316——任何 Plan 的
//   摘要都如此）→ 槽 0 低 16 位 = 'P' | ('L'<<8) = 0x4C50；
//   UPL3 = 0x55504C33 低 16 位 = 0x4C33——恒不等 → ParseUnifiedBlobV4 的
//   magic 判据（aicpu_consumer.h:112）必拒。长度 < 8 组不出槽 → "blob too small"
//   同样必拒。两形态都返回 true（拒），errOut 携带判据说明（与真消费方的
//   err 文案关键词对齐：magic / too small）。
bool StV1TextBlobWouldBeRejected(const std::string& v1Text)
{
    // 小于一个 u64 槽：连头部都组不出（真消费方 :102 "blob too small for v4
    // header"）。真 v1 产物恒 ≥ 15B（"PLAN v3 segs:0\n"），此分支是防御完备性。
    if (v1Text.size() < sizeof(u64)) {
        return true;
    }
    const std::vector<u64> slots = StBytesToBlobSlots(v1Text);
    // 槽 0 低 16 位："PL" 小端 = 'P'(0x50) | 'L'(0x4C)<<8 = 0x4C50
    constexpr u64 V1_HEAD16 = 0x4C50ULL;
    constexpr u64 UPL3_LOW16 = 0x4C33ULL;   // PLAN_V3_MAGIC 0x55504C33 的低 16 位
    // 与真 magic 判据同式（blob[0] != PLAN_V3_MAGIC 拒）：只看低 16 位已足以
    // 判定不等（高 48 位任取都不可能让 0x...4C50 == 0x55504C33）
    return (slots[0] & 0xFFFFULL) == V1_HEAD16 && UPL3_LOW16 != V1_HEAD16;
}

// ctxSize 前置闸镜像（unified_aicpu_exec.cc:69-77 的判定数学 1:1）：
//   闸 1（:69-72）：param.resCtx == nullptr || param.ctxSize == 0 → HCCL_E_PARA
//     （"unified blob ctx missing"）。F1 附带病灶：AICPU 成功路径现状
//     opParam.ctxSize 置 0（hccl_alloc_ctx_res.cc:1228）→ 本闸必拒——修复 = 资源期
//     回填 blob 字节数（39 槽 → 312）。
//   闸 2（:73-77）：ctxSize % 8 != 0 → HCCL_E_PARA（"not u64-aligned"）。
//     v1 文本产物（如 108B）死在这条闸上。
StCtxGateResult StCheckCtxSizeGate(const void* resCtx, u64 ctxSize)
{
    if (resCtx == nullptr || ctxSize == 0U) {
        return StCtxGateResult::CTX_GATE_REJECT_NULL_OR_ZERO;
    }
    if (ctxSize % sizeof(u64) != 0U) {
        return StCtxGateResult::CTX_GATE_REJECT_NOT_U64_ALIGNED;
    }
    return StCtxGateResult::CTX_GATE_PASS;
}

// KFC executor 判定序镜像（comm_kfc_open_kernel_executor.cc :194-201 三态 +
// unified 分叉的判定数学）：
//   修复后判序（unifiedForkBeforeLookup=true，kernel_launch.cc:358-394 同型）：
//     ① isUnifiedKey(algName) → KFC_PATH_UNIFIED_AICPU（RunUnifiedAicpuOp——
//        不进 GetAlgExec，无注册项也无妨）
//     ② registered → KFC_PATH_LEGACY_ORCHESTRATE（查表命中 → Orchestrate）
//     ③ 其余 → KFC_PATH_NULLPTR_INTERCEPT（CHK_SMART_PTR_NULL 拦截）
//   病灶判序（=false，GetAlgExec 先行）：unified 键无注册项 → 查表 nullptr →
//     拦截先行（unified 分支不可达 = F1 断点②的死代码形态）。
StKfcExecPath StResolveKfcExecPath(const char* algName, bool registered,
                                   bool (*isUnifiedKey)(const char*),
                                   bool unifiedForkBeforeLookup)
{
    if (unifiedForkBeforeLookup && isUnifiedKey != nullptr && isUnifiedKey(algName)) {
        return StKfcExecPath::KFC_PATH_UNIFIED_AICPU;
    }
    if (!registered) {
        return StKfcExecPath::KFC_PATH_NULLPTR_INTERCEPT;   // GetAlgExec nullptr → 拦截
    }
    if (!unifiedForkBeforeLookup && isUnifiedKey != nullptr && isUnifiedKey(algName)) {
        // 病灶序下本分支不可达（registered=false 的 unified 键已在上一行拦截）——
        // 保留判定仅用于 ST 病灶反例的可判性证据（st_verification R10 ㊲ 断言其值）。
        return StKfcExecPath::KFC_PATH_UNIFIED_AICPU;
    }
    return StKfcExecPath::KFC_PATH_LEGACY_ORCHESTRATE;
}

// ---------------------------------------------------------------------------
// R11 · resCtx 双语义可判基线实现（声明契约见 st_semantic_checker.h R11 节注）。
//
// ★实现来源声明：Serialize/DeSerialize 镜像是 alg_param.h:529-611 字段序的紧凑流
// 1:1 复刻（不 include 真 AlgResourceCtxSerializable——真头在 CANN 编译面，shim 面
// 无该完整定义）；字节布局期望值全部独立手算（python /tmp/r11_st_expectations.py），
// 并由 st_verification.cc R11 节用真 alg_param.h 编译的探针对数交叉锚（防镜像自证）。
// BuildCacheKey 判定镜像同理（open_res_ctx_cache.cc:73-119 的 std::find '\0' 语义）。
// ---------------------------------------------------------------------------
namespace {

// 小端写（BinaryStream stream.write 原样字节——本机 aarch64 即小端）
void PutU32(std::vector<char>& out, u32 v)
{
    for (int i = 0; i < 4; ++i) out.push_back(static_cast<char>((v >> (8 * i)) & 0xFF));
}
void PutU64(std::vector<char>& out, u64 v)
{
    for (int i = 0; i < 8; ++i) out.push_back(static_cast<char>((v >> (8 * i)) & 0xFF));
}
u32 GetU32(const std::vector<char>& in, size_t pos)
{
    u32 v = 0;
    for (int i = 0; i < 4; ++i) {
        v |= static_cast<u32>(static_cast<unsigned char>(in[pos + i])) << (8 * i);
    }
    return v;
}
u64 GetU64(const std::vector<char>& in, size_t pos)
{
    u64 v = 0;
    for (int i = 0; i < 8; ++i) {
        v |= static_cast<u64>(static_cast<unsigned char>(in[pos + i])) << (8 * i);
    }
    return v;
}
// vector<T> 序列化 = size_t(8B) + 逐元素（binary_stream.h:44-52）
void PutVecSize(std::vector<char>& out, size_t n) { PutU64(out, static_cast<u64>(n)); }

// topoInfo 默认构造的 Serialize 镜像（TopoInfoWithNetLayerDetails::VisitFields 定长
// 清单，alg_param.h:257-266 逐字段 + Serialize 尾部 physicalLevelNum；默认值：
// deviceType=DEV_TYPE_COUNT(7)、serverIdx/superPodIdx=INVALID_UINT(0xFFFFFFFF)、
// 其余 0/空容器）。userRank/userRankSize 由 case 携带（本镜像只构造 ST 消费形态）。
std::vector<char> PackTopoDefault(u32 userRank, u32 userRankSize)
{
    std::vector<char> o;
    PutU32(o, userRank);          // userRank
    PutU32(o, userRankSize);      // userRankSize
    PutU32(o, 0xFFFFFFFFU);       // serverIdx
    PutU32(o, 0xFFFFFFFFU);       // superPodIdx
    PutU32(o, 7U);                // deviceType = DEV_TYPE_COUNT
    PutU32(o, 0U);                // deviceNumPerModule
    PutU32(o, 0U); PutU32(o, 0U); PutU32(o, 0U); PutU32(o, 0U); PutU32(o, 0U);
    // ↑ serverNumPerSuperPod serverNum moduleNum superPodNum moduleIdx（行2 共 5 个）
    o.push_back('\0'); o.push_back('\0'); o.push_back('\0');   // 3 bool（行3）
    o.push_back('\0');                                           // isHCCSSWNumEqualToTwiceSIONum
    PutU64(o, 0U);                // mainThread
    PutU32(o, 0U);                // notifyNumOnMainThread
    PutU32(o, 0U);                // topoLevelNums
    PutU32(o, 0U);                // level0Topo（CLOS=0）
    for (int i = 0; i < 4; ++i) o.push_back('\0');   // Level0Nhr Level1Nhr Level1Hd is2DieFullMesh
    for (int i = 0; i < 5; ++i) o.push_back('\0');   // level0PcieMix level0BigClosRange topLevelUboe level2UbRtp hostDpuOnly
    o.push_back('\0'); o.push_back('\0');            // level0Symmetric level1Symmetric
    PutU32(o, 0U);                // topoInstDetailsOfLayerSize
    o.push_back('\0');            // isPod
    PutU32(o, 0U);                // level0MeshType（NOT_MESH=0）
    PutU32(o, 0U);                // netLayerDetails.netLayerNum
    PutVecSize(o, 0);             // netLayers
    PutVecSize(o, 0);             // netInstNumOfLayer
    PutVecSize(o, 0);             // instSizeListOfLayer
    PutVecSize(o, 0);             // localNetInsSizeOfLayer
    PutU32(o, 0U);                // physicalLevelNum（Serialize 尾部，空 physicalLevels）
    return o;                      // = 128 B（手算 == 探针实测）
}

// ChannelInfo 原样字节镜像（sizeof=160：isValid@0(1B+pad3) remoteRank@4 protocol@8
// locType@12 notifyNum@16 portGroupSize@20 dieId@24 pad4 handle@32 + 5×HcclMem(24)@40..160）
std::vector<char> PackChannelInfo(u32 remoteRank, u64 handle)
{
    std::vector<char> o(ST_R11_SZ_CHANNELINFO, '\0');
    // 直接在缓冲上写偏移字段（pad 保持 0）
    auto w32 = [&](size_t off, u32 v) {
        for (int i = 0; i < 4; ++i) o[off + i] = static_cast<char>((v >> (8 * i)) & 0xFF);
    };
    auto w64 = [&](size_t off, u64 v) {
        for (int i = 0; i < 8; ++i) o[off + i] = static_cast<char>((v >> (8 * i)) & 0xFF);
    };
    w32(4, remoteRank);   // remoteRank
    w64(32, handle);      // handle
    return o;
}

} // namespace

std::vector<char> StSerializeResCtxMirror(const StResCtxCase& c)
{
    std::vector<char> out;
    out.insert(out.end(), ST_R11_SZ_ALGTYPE, '\0');  // algType（3×enum4 全 0）
    PutVecSize(out, 0);                              // algHierarchyInfo.infos（空）
    PutVecSize(out, 0);                              // physicalIdxForAlgoLevels（空）
    // cclMem：HcclMem{type(4)+pad(4)+addr(8)+size(8)}=24B
    PutU32(out, 0U);                                 // HcclMemType::HCCL_MEM_TYPE_DEVICE
    out.insert(out.end(), 4, '\0');                  // padding
    PutU64(out, c.cclAddr);
    PutU64(out, c.cclSize);
    PutU32(out, 0U);                                 // notifyNumOnMainThread
    PutU32(out, c.slaveThreadNum);                   // slaveThreadNum
    PutU32(out, 0U);                                 // waitTimeout
    PutU32(out, 0U);                                 // fullTimeout
    PutVecSize(out, c.notifyPerThread.size());       // notifyNumPerThread
    for (u32 n : c.notifyPerThread) PutU32(out, n);
    PutU64(out, c.commPtr);                          // commInfoPtr
    PutVecSize(out, c.threads.size());               // threads
    for (u64 t : c.threads) PutU64(out, t);
    PutU64(out, 0U);                                 // unfoldThread
    PutVecSize(out, c.channels.size());              // channels（多组 vector<vector>）
    for (const auto& grp : c.channels) {
        PutVecSize(out, grp.size());
        for (const auto& ch : grp) {
            std::vector<char> b = PackChannelInfo(ch.first, ch.second);
            out.insert(out.end(), b.begin(), b.end());
        }
    }
    out.push_back('\0');                             // isHcommBatchTransferOnThreadSupported
    out.push_back('\0');                             // isHcclThreadAcquireWithConfigSupported
    PutU64(out, 0U);                                 // npu2DpuShmemPtr
    PutU64(out, 0U);                                 // dpu2NpuShmemPtr
    PutVecSize(out, 0);                              // ccuKernelNum
    PutVecSize(out, 0);                              // ccuKernels
    PutU64(out, 0U);                                 // dieSplitRatio（0.0 的位模式）
    out.insert(out.end(), ST_R11_SZ_PARALLELPORT, '\0');  // parallelPortInfo
    PutVecSize(out, 0);                              // kfcServerArgs
    PutU32(out, 0U);                                 // kfcServerArgSize
    if (c.planB) {
        // 方案 B（主会话裁决）：unifiedBlobAddr/unifiedBlobSize 两 u64 插在
        // kfcServerArgSize 之后、topoInfoSeqSize 之前——topo 保持最后段
        //（topo 锚 data.size()-topoInfoSeqSize 不变，这是方案 B 的核心正确性）
        PutU64(out, c.unifiedBlobAddr);
        PutU64(out, c.unifiedBlobSize);
    }
    // topoInfo.Serialize() 尾段（默认构造 128B）+ topoInfoSeqSize(4B) 在前
    std::vector<char> topo = PackTopoDefault(3U, 8U);   // ST 等价命题场景（me=3/8 卡）
    PutU32(out, static_cast<u32>(topo.size()));     // topoInfoSeqSize
    out.insert(out.end(), topo.begin(), topo.end());// result 末尾 append（不进 BinaryStream）
    return out;
}

StResCtxReadback StDeserializeResCtxMirror(const std::vector<char>& data)
{
    StResCtxReadback r;
    size_t pos = 0;
    // 读游标 lambda 命名避开类型名 u32/u64（C++ 声明点先于初始化器——同名变量
    // 会在自身参数列表里遮蔽类型）
    auto rd = [&](size_t n) -> bool {
        if (pos + n > data.size()) return false;
        pos += n;
        return true;
    };
    auto rdU32 = [&](u32& v) -> bool {
        if (pos + 4 > data.size()) return false;
        v = GetU32(data, pos); pos += 4; return true;
    };
    auto rdU64 = [&](u64& v) -> bool {
        if (pos + 8 > data.size()) return false;
        v = GetU64(data, pos); pos += 8; return true;
    };
    auto vecSize = [&](u64& n) -> bool { return rdU64(n); };
    if (!rd(ST_R11_SZ_ALGTYPE)) return r;           // algType
    u64 n = 0;
    if (!vecSize(n)) return r;                      // infos.size
    for (u64 i = 0; i < n; ++i) {                   // 三维展开（ST 输入恒 0 层）
        u64 m = 0; if (!vecSize(m)) return r;
        for (u64 j = 0; j < m; ++j) { u64 k = 0; if (!vecSize(k)) return r; if (!rd(4 * k)) return r; }
    }
    if (!vecSize(n)) return r;                      // physicalIdxForAlgoLevels.size
    for (u64 i = 0; i < n; ++i) { u64 m = 0; if (!vecSize(m)) return r; if (!rd(4 * m)) return r; }
    if (!rd(ST_R11_SZ_HCcLMEM)) return r;           // cclMem
    if (!rd(4)) return r;                           // notifyNumOnMainThread
    if (!rdU32(r.slaveThreadNum)) return r;
    if (!rd(4) || !rd(4)) return r;                 // waitTimeout/fullTimeout
    if (!vecSize(n)) return r;                      // notifyNumPerThread.size
    if (!rd(4 * n)) return r;
    if (!rdU64(r.commPtr)) return r;                // commInfoPtr
    if (!vecSize(n)) return r;                      // threads.size
    for (u64 i = 0; i < n; ++i) { u64 t = 0; if (!rdU64(t)) return r; r.threads.push_back(t); }
    if (!rd(8)) return r;                           // unfoldThread
    u64 groups = 0;
    if (!vecSize(groups)) return r;                 // channels.size
    r.channels.resize(static_cast<size_t>(groups));
    for (u64 g = 0; g < groups; ++g) {
        u64 m = 0; if (!vecSize(m)) return r;       // 组内条数
        r.channels[g].reserve(static_cast<size_t>(m));
        for (u64 i = 0; i < m; ++i) {
            if (pos + ST_R11_SZ_CHANNELINFO > data.size()) return r;
            r.channels[g].emplace_back(GetU32(data, pos + 4), GetU64(data, pos + 32));
            pos += ST_R11_SZ_CHANNELINFO;
        }
    }
    if (!rd(1) || !rd(1)) return r;                 // 两 bool
    if (!rd(8) || !rd(8)) return r;                 // shmem 双指针
    if (!vecSize(n)) return r;
    if (!rd(4 * n)) return r;                       // ccuKernelNum
    if (!vecSize(n)) return r;
    if (!rd(8 * n)) return r;                       // ccuKernels
    if (!rd(8)) return r;                           // dieSplitRatio
    if (!rd(ST_R11_SZ_PARALLELPORT)) return r;
    if (!vecSize(n)) return r;
    if (!rd(8 * n)) return r;                       // kfcServerArgs
    if (!rd(4)) return r;                           // kfcServerArgSize
    // 方案 B 判别（产物自描述性由 topoInfoSeqSize 锚承担）：
    //   binary 段剩余（data 尾 128B 是 topo）在旧格式下 = 4B（topoInfoSeqSize），
    //   方案 B 下 = 20B（两 u64 + topoInfoSeqSize）。判序：
    //     ① pos 处 u32 == 128 且 pos+4+128 == data.size() → 旧格式；
    //     ② pos+16 处 u32 == 128 且 pos+20+128 == data.size() → 方案 B；
    //     ③ 其余 = 截断/漂移，ok=false（含 append 形态——pos 处是 blobAddr 低 4B，
    //        既非 128 也非方案 B 锚——病灶形态在 mirror 下不可解，符合预期：
    //        真实现读 append 产物时字段序错位，本镜像以 ok=false 表达"解不出"）。
    {
        const size_t topoLen = ST_R11_TOPO_DEFAULT_SEQ;
        if (data.size() < pos + 4 + topoLen) return r;   // 连旧格式最小形状都不够
        const u32 seqOld = GetU32(data, pos);
        if (seqOld == topoLen && pos + 4 + topoLen == data.size()) {
            // 旧格式：topoInfoSeqSize@pos，无新字段
            r.topoInfoSeqSize = seqOld;
            r.unifiedBlobAddr = 0;
            r.unifiedBlobSize = 0;
        } else if (pos + 16 + 4 + topoLen == data.size() && GetU32(data, pos + 16) == topoLen) {
            // 方案 B：unifiedBlobAddr@pos unifiedBlobSize@pos+8 topoInfoSeqSize@pos+16
            r.unifiedBlobAddr = GetU64(data, pos);
            r.unifiedBlobSize = GetU64(data, pos + 8);
            r.topoInfoSeqSize = GetU32(data, pos + 16);
        } else {
            return r;                               // 两种格式都不匹配（截断/漂移/append 病灶）
        }
    }
    r.ok = true;
    // topo 尾段窗口（真 DeSerialize :606 的切法）：data.size() - topoInfoSeqSize
    r.topoTailStart = (r.topoInfoSeqSize <= data.size()) ? (data.size() - r.topoInfoSeqSize) : 0;
    return r;
}

std::vector<char> StAppendBlobTail(const std::vector<char>& resCtxBytes, u64 blobAddr, u64 blobSize)
{
    std::vector<char> out = resCtxBytes;
    PutU64(out, blobAddr);
    PutU64(out, blobSize);
    return out;
}

bool StReadBlobTail(const std::vector<char>& withTail, u64& blobAddr, u64& blobSize)
{
    if (withTail.size() < 16) return false;
    const size_t base = withTail.size() - 16;
    blobAddr = GetU64(withTail, base);
    blobSize = GetU64(withTail, base + 8);
    return true;
}

bool StKfcCacheKeyGate(const char* commName, const char* algTag)
{
    // GetBoundedString 镜像（open_res_ctx_cache.cc:73-82）：缓冲内须存在 '\0' 且不在首位。
    // 真源按定长数组（commName[160]/algTag[288]）扫；ST 入参是 NTS 指针——
    // 以"非空 + 有 '\0' 终止"等价判定（空串 = 首字符 '\0' = end==begin 拒）。
    auto bounded = [](const char* s) -> bool {
        if (s == nullptr) return false;             // 真源不会传 null，防御完备性
        if (s[0] == '\0') return false;             // 空串：end == buffer → false
        for (size_t i = 0; s[i] != '\0'; ++i) {
            if (i + 1 >= ST_R11_ALG_TAG_LEN) return false;   // 无 '\0'（截断上界）→ end==begin+N 拒
        }
        return true;
    };
    return bounded(commName) && bounded(algTag);
}

} // namespace mc2_ops_hccl::refactor::st
