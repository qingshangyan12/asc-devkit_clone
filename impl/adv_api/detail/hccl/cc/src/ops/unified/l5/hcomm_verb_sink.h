/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * CANN Open Software License Agreement Version 2.0.
 *
 * R6 · WP-A1：IVerbSink 的真实实现（HcommVerbSink）——动词 → hcomm 真原语接线
 *
 * 归属与依赖：本头是"引擎知识唯一居所"的执行半边（verb_sink.h 是词汇表半边）。
 * 与 verb_sink.h 不同，本头**引入真实引擎头**（alg_param.h 链带 hcomm_primitives_dl.h
 * → hcomm_primitives.h 的 ThreadHandle/ChannelHandle/原语 extern 声明），故不可进
 * unified 树 inc/（那里是"真引擎头零 include"的 L3 值对象铁律）——落 l5/ 下由
 * 存量侧/消费方 include，host 自检经 shim 提供的类型面同样可编译（sink 只消费
 * ChannelInfo/ThreadHandle/notifyIdx 常量，均为 alg_param.h 可见面）。
 *
 * 动词 → 真原语映射（M0 考古结论，逐行对照存量）：
 *   READ          SendRecvRead 五步全套（alg_data_trans_wrapper.cc:521-551）
 *                 ①ChannelNotifyRecord(sendCh,ACK=0) ②ChannelNotifyWait(recvCh,ACK,1836)
 *                 ③HcommReadOnThread ④ChannelNotifyRecord(recvCh,DATA_SIGNAL=1)
 *                 ⑤ChannelNotifyWait(sendCh,DATA_SIGNAL,1836)
 *                 ★直调 HcommReadOnThread 会破坏 cclBuff 流控（对端读到旧数据或
 *                 覆盖未读数据）——五步必须全套复制，此为 R6 主会话裁决。
 *                 ★tx/rx 同通道口径：AG 现状 TxRxChannels(linkRemote,linkRemote)
 *                 （ins_temp_all_gather_mesh_1D.cc:213），本 sink 同式取
 *                 channels[peer][0] 一条通道（multi-channel 时取 [0]，:120 同式）。
 *   COPY/POST_COPY HcommLocalCopyOnThread(thread,dst,src,len)（wrapper.cc:733 同式）
 *   SYNC_PEER_PRE  PreSyncInterThreads 两段循环序（wrapper.cc:848-857）：
 *                 先 record 全下发（main→sub 逐个）再逐个 wait（sub 线程侧）
 *   SYNC_PEER_POST PostSyncInterThreads 两段循环序（wrapper.cc:881-890）：
 *                 先 wait 全部（main 线程侧）再 record 全下发（sub→main 逐个）
 *                 ★两段循环不是逐对 record-wait 交错——照抄存量序。
 *   WRITE/REDUCE/BROADCAST 未实现（P2 交付）：HCCL_ERROR 拒绝，不静默跳过。
 *
 * 与 TraceSink 的同源性：同一 Emit 序列喂 TraceSink 得 trace 字符串、喂本 sink 得
 * 真原语调用记录——VerbCall 值对象是唯一中转（§6.13.4 trace 与执行同源）。单测
 * 以"两者逐动词一致"断言（run_unified_exec.sh 目标 2）。
 *
 * 错误路径：任一原语返回非 0 → 记 err + 首个错误码，后续动词照常记录（诊断完整）
 * 但不再下发真原语（防半途损坏链路继续搬数）；调用方查 Err() 判定失败。
 */
#ifndef REFACTOR_POC_HCOMM_VERB_SINK_H
#define REFACTOR_POC_HCOMM_VERB_SINK_H

#include "verb_sink.h"

#include "alg_param.h"   // ChannelInfo/HcclMem/NOTIFY_IDX_ACK/DATA_SIGNAL/CUSTOM_TIMEOUT
                         //（经 hcomm_primitives_dl.h → hcomm_primitives.h 带出
                         //  ThreadHandle/ChannelHandle 与原语 extern 声明）
#include <map>
#include <string>
#include <vector>

namespace mc2_ops_hccl::refactor {

// ---------------------------------------------------------------------------
// 资源束：HcommVerbSink 的构造输入（对应存量 TemplateResource 的可执行半边）
//   threads —— resCtx.threads（threads[0]=主线程，[1..]=从线程，与 KernelRun
//              templateResource.threads 同序；AG mesh 每从线程认领一个 peer：
//              RunAllGatherMesh 的 threads[threadIdx] 逐 peer 取用，:216）
//   channels —— 本 rank 逐 remoteRank 的通道表（RestoreChannelMap 产出的
//               map<u32, vector<ChannelInfo>> 单层形态；取 [0] 与 :120 同式）
// ---------------------------------------------------------------------------
struct HcommVerbRes {
    std::vector<ThreadHandle> threads;
    std::map<u32, std::vector<ChannelInfo>> channels;
};

// resCtx → HcommVerbRes 的组装便捷件（R6 WP-A2 消费方接线，2026-09-22）：
// kernel_launch.cc 分叉点与 unified_aicpu_exec.cc 同式调用——同源不重复。
// channels[0] 线性扫进 map（RestoreChannelMap 的单层形态）——不经 InsCollAlgBase
// 继承（不改存量）；每 rank 保留全部条目（sink 取 [0] 同 :120 口径）。
// ★类型面：AlgResourceCtxSerializable 由 alg_param.h 引入——真实链路在
// mc2_ops_hccl 域；host shim 面在全局域（fake_alg_param.h 无 namespace 包裹）。
// 统一用 mc2_ops_hccl::AlgResourceCtxSerializable（真链路正确名）；shim 单测
// （unified_exec/test）已将 shim 结构包进 mc2_ops_hccl 域以对齐。
inline HcommVerbRes MakeHcommVerbRes(const mc2_ops_hccl::AlgResourceCtxSerializable& resCtx)
{
    HcommVerbRes res;
    res.threads = resCtx.threads;
    if (!resCtx.channels.empty()) {
        for (const ChannelInfo& ch : resCtx.channels[0]) {
            res.channels[ch.remoteRank].push_back(ch);
        }
    }
    return res;
}

// 调用记录（单测/诊断用：真原语调用的结构化 trace，与 TraceSink 字符串同源）
struct HcommCallRecord {
    VerbCall::Verb verb{VerbCall::COPY};
    u64 p0{0};    // 语义随动词：READ=src、COPY=src、SYNC=0
    u64 p1{0};    // READ=dst、COPY=dst
    u64 p2{0};    // READ=size、COPY=size
    u32 peer{0};  // READ 专属
    int32_t ret{0};   // 本动词首个原语的返回值（0=全组成功）
};

class HcommVerbSink : public IVerbSink {
public:
    explicit HcommVerbSink(const HcommVerbRes& res) : res_(res) {}

    // 动词分发（词汇表全集 switch，未实现动词显式拒绝——不静默跳过）
    void Emit(const VerbCall& call) override
    {
        HcommCallRecord rec;
        rec.verb = call.verb;
        rec.p0 = call.src;
        rec.p1 = call.dst;
        rec.p2 = call.size;
        rec.peer = call.peer;
        // 已有失败 → 不再下发真原语（防半途损坏链路继续搬数），仅记录
        if (errCode_ == 0) {
            errCode_ = Dispatch(call);
        }
        rec.ret = errCode_;
        calls_.push_back(rec);
    }

    // 首错误码（0=全部成功）与错误描述
    int32_t ErrCode() const { return errCode_; }
    const std::string& Err() const { return err_; }
    // 结构化调用记录（与 TraceSink 的 trace 字符串同源断言用）
    const std::vector<HcommCallRecord>& Calls() const { return calls_; }

    // VerbCall → 已发射动词名的同源渲染（与 TraceSink 拼串口径一致；单测断言用）
    static std::string RenderVerb(const VerbCall& c)
    {
        std::string s = VerbName(c.verb);
        s += "(" + std::to_string(c.src) + "->" + std::to_string(c.dst) + "," +
             std::to_string(c.size) + (c.peer ? ",peer" + std::to_string(c.peer) : "") + ")";
        return s;
    }

private:
    int32_t Dispatch(const VerbCall& call)
    {
        switch (call.verb) {
            case VerbCall::READ:
                return DoRead(call);
            case VerbCall::COPY:
            case VerbCall::POST_COPY:
                return DoCopy(call);
            case VerbCall::SYNC_PEER_PRE:
                return DoSyncPre();
            case VerbCall::SYNC_PEER_POST:
                return DoSyncPost();
            default:
                // WRITE/REDUCE/BROADCAST：P2 交付（REDUCE 随 ReduceSlice、WRITE 随
                // RS/AR 写路径、BROADCAST 是 CCU 声明式专属）——显式拒绝
                Fail("verb '" + std::string(VerbName(call.verb)) + "' not wired in R6 AICPU scope (P2)");
                return -1;
        }
    }

    // ① peer → 通道（取 [0]，与 ins_temp_all_gather_mesh_1D.cc:120 同式）
    const ChannelInfo* ChannelOf(u32 peer)
    {
        const auto it = res_.channels.find(peer);
        if (it == res_.channels.end() || it->second.empty()) {
            Fail("no channel to peer " + std::to_string(peer));
            return nullptr;
        }
        return &it->second[0];
    }

    // READ：SendRecvRead 五步全套（wrapper.cc:521-551 逐行对照，tx/rx 同通道 :213）
    int32_t DoRead(const VerbCall& call)
    {
        // 从线程选择：逐 peer 一从线程（现状 threads[threadIdx]，threadIdx=peer 步序
        // ——Emit 序即 BuildArgs.peers 序，第 i 条 READ 对 threads[i]（主线程算第 0
        // 步，AG 现状 threadNum=N-1、主线程 LocalDataCopy 后由从线程跑全部 read，
        // :103 循环 threads.size()=N-1 与 :216 threads[threadIdx] 同口径）
        const ThreadHandle thread = ThreadForRead();
        if (thread == 0) {
            return -1;    // Fail 已置
        }
        const ChannelInfo* ch = ChannelOf(call.peer);
        if (ch == nullptr) {
            return -1;
        }
        const ChannelHandle handle = ch->handle;
        // ① 通道级 ACK：告知对端可写（wrapper :530）
        int32_t r = HcommChannelNotifyRecordOnThread(thread, handle, NOTIFY_IDX_ACK);
        if (r != 0) {
            Fail("READ step1 ACK record ret=" + std::to_string(r));
            return r;
        }
        // ② 等对端 ACK（wrapper :531-532；同通道口径下即等对端的同等告知）
        r = HcommChannelNotifyWaitOnThread(thread, handle, NOTIFY_IDX_ACK, CUSTOM_TIMEOUT);
        if (r != 0) {
            Fail("READ step2 ACK wait ret=" + std::to_string(r));
            return r;
        }
        // ③ 读（wrapper :543；dst/src = 动词 offset + 基址，基址由消费方在 VerbCall
        //    合成时已并入（src=对端 cclBuff 基址+srcOff、dst=本端 output 基址+dstOff））
        if (call.size != 0) {
            void* dst = reinterpret_cast<void*>(call.dst);
            const void* src = reinterpret_cast<const void*>(call.src);
            r = HcommReadOnThread(thread, handle, dst, src, call.size);
            if (r != 0) {
                Fail("READ step3 HcommReadOnThread ret=" + std::to_string(r));
                return r;
            }
        } else {
            // size=0 与 wrapper :537-540 的 continue 语义一致（跳过读，握手不省）
        }
        // ④ 通道级 DATA_SIGNAL：读毕告知（wrapper :546-547）
        r = HcommChannelNotifyRecordOnThread(thread, handle, NOTIFY_IDX_DATA_SIGNAL);
        if (r != 0) {
            Fail("READ step4 DATA_SIGNAL record ret=" + std::to_string(r));
            return r;
        }
        // ⑤ 等对端 DATA_SIGNAL（wrapper :548-549）
        r = HcommChannelNotifyWaitOnThread(thread, handle, NOTIFY_IDX_DATA_SIGNAL, CUSTOM_TIMEOUT);
        if (r != 0) {
            Fail("READ step5 DATA_SIGNAL wait ret=" + std::to_string(r));
            return r;
        }
        return 0;
    }

    // COPY / POST_COPY：HcommLocalCopyOnThread（wrapper LocalCopy :733 同式）。
    // dst/src = 动词携带的绝对地址（消费方合成），size=0 与 LocalCopy 的 size==0
    // 早退同语义（wrapper :721-723 返回 SUCCESS）
    int32_t DoCopy(const VerbCall& call)
    {
        if (call.size == 0) {
            return 0;
        }
        const ThreadHandle thread = MainThread();
        if (thread == 0) {
            return -1;
        }
        void* dst = reinterpret_cast<void*>(call.dst);
        const void* src = reinterpret_cast<const void*>(call.src);
        const int32_t r = HcommLocalCopyOnThread(thread, dst, src, call.size);
        if (r != 0) {
            Fail(std::string(VerbName(call.verb)) + " HcommLocalCopyOnThread ret=" + std::to_string(r));
            return r;
        }
        return 0;
    }

    // SYNC_PEER_PRE：PreSyncInterThreads 两段循环序（wrapper :848-857）
    //   notifyIdxMainToSub = 全 0（GetNotifyIdxMainToSub，ins_temp_all_gather_mesh_1D.cc:338-346）
    int32_t DoSyncPre()
    {
        const ThreadHandle main = MainThread();
        if (main == 0) {
            return -1;
        }
        if (res_.threads.size() < 2) {
            Fail("SYNC_PEER_PRE with no slave threads");
            return -1;
        }
        // 第一段：record 全下发（main → 逐 sub，idx 全 0）
        for (size_t i = 1; i < res_.threads.size(); ++i) {
            const int32_t r = HcommThreadNotifyRecordOnThread(main, res_.threads[i], 0);
            if (r != 0) {
                Fail("SYNC_PRE record loop ret=" + std::to_string(r));
                return r;
            }
        }
        // 第二段：逐个 wait（sub 线程侧等 idx 0）
        for (size_t i = 1; i < res_.threads.size(); ++i) {
            const int32_t r = HcommThreadNotifyWaitOnThread(res_.threads[i], 0, CUSTOM_TIMEOUT);
            if (r != 0) {
                Fail("SYNC_PRE wait loop ret=" + std::to_string(r));
                return r;
            }
        }
        return 0;
    }

    // SYNC_PEER_POST：PostSyncInterThreads 两段循环序（wrapper :881-890）
    //   notifyIdxSubToMain = 0..slaveNum-1（GetNotifyIdxSubToMain，:348-356）
    int32_t DoSyncPost()
    {
        const ThreadHandle main = MainThread();
        if (main == 0) {
            return -1;
        }
        if (res_.threads.size() < 2) {
            Fail("SYNC_PEER_POST with no slave threads");
            return -1;
        }
        // 第一段：wait 全部（main 逐个等 sub[i-1] 的 idx i-1）
        for (size_t i = 1; i < res_.threads.size(); ++i) {
            const int32_t r = HcommThreadNotifyWaitOnThread(main, static_cast<u32>(i - 1), CUSTOM_TIMEOUT);
            if (r != 0) {
                Fail("SYNC_POST wait loop ret=" + std::to_string(r));
                return r;
            }
        }
        // 第二段：record 全下发（逐 sub → main，idx i-1）
        for (size_t i = 1; i < res_.threads.size(); ++i) {
            const int32_t r =
                HcommThreadNotifyRecordOnThread(res_.threads[i], main, static_cast<u32>(i - 1));
            if (r != 0) {
                Fail("SYNC_POST record loop ret=" + std::to_string(r));
                return r;
            }
        }
        return 0;
    }

    // READ 的线程选择：第 i 条 READ → threads[i]（对照现状 threads[threadIdx]，
    // ins_temp_all_gather_mesh_1D.cc:103/:216——threadIdx 与 peer 步序一致，
    // **threads[0]（主线程）承载首条 read**，i∈[0,N-2] 全用主+从）。
    // 与 WP-A1 首版（i→threads[i+1]，read 数 > slave 数拒绝）的差别：按主会话
    // M0 对账后的存量形态修正——AG mesh 的 threadNum=N-1（主+从共 N-1 条），
    // read 数 = N-1 恰好等于 threads.size()，主线程承载第 0 条、从线程承载
    // 第 1..N-2 条。越界判定 = readIdx ≥ threads.size()。
    ThreadHandle ThreadForRead()
    {
        u32 readIdx = 0;
        for (const auto& c : calls_) {
            if (c.verb == VerbCall::READ) {
                ++readIdx;
            }
        }
        if (readIdx >= res_.threads.size()) {
            Fail("READ exceeds thread count (readIdx=" + std::to_string(readIdx) +
                 ", threads=" + std::to_string(res_.threads.size()) + ")");
            return 0;
        }
        return res_.threads[readIdx];
    }

    ThreadHandle MainThread()
    {
        if (res_.threads.empty() || res_.threads[0] == 0) {
            Fail("no main thread");
            errCode_ = -1;
            return 0;
        }
        return res_.threads[0];
    }

    void Fail(const std::string& msg)
    {
        err_ = msg;
    }

    HcommVerbRes res_;
    std::vector<HcommCallRecord> calls_;
    int32_t errCode_{0};
    std::string err_;
};

} // namespace mc2_ops_hccl::refactor

#endif
