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
#include "nhr_steps.h"          // R15 · ②：CalcNhrSteps（DoNhrRun 步几何重算）
#include "nhr_slice_geometry.h" // R18 · #11：NhrSliceOf 几何公共件（DoNhrRun 与展开 Emit 同源）
#include "unified_route.h"      // R15 · ②：UseUnifiedNhrAicpuPath（gated 执行面）

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
    // R12 · WP-9②：REDUCE 的 dataType/reduceOp 注入口（真签名 6 参——
    // hcomm_primitives.h:99；缺省 FP16/SUM，真链路由消费方从 OpParam 注入）
    explicit HcommVerbSink(const HcommVerbRes& res,
                           HcommDataType reduceDataType = HCOMM_DATA_TYPE_FP16,
                           HcommReduceOp reduceOp = HCOMM_REDUCE_SUM)
        : res_(res), reduceDataType_(reduceDataType), reduceOp_(reduceOp)
    {
        reduceDataTypeSize_ = DataTypeSizeOf_(reduceDataType);
    }

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

    // R16 · #7：动词→线程策略的表驱动统一入口（kAicpuVerbMap.thread 列单点声明，
    // 策略偏移量从表来——ThreadPolicyOf 查表）。原三个独立取线程函数
    //（ThreadForRead/ThreadForWrite/MainThread 计数器逻辑内联策略）收敛于此；
    // 语义零变化（COUNTER_OFF_0/1 的计数与越界判定与原实现逐字一致）。
    ThreadHandle ThreadFor(VerbCall::Verb verb)
    {
        switch (ThreadPolicyOf(verb, /*isCcu*/ false)) {
            case ThreadPolicy::MAIN:
            case ThreadPolicy::MAIN_PLUS_SLAVES:
                return MainThread();
            case ThreadPolicy::COUNTER_OFF_0:
                return ThreadByCounter(VerbCall::READ, 0U, "READ");
            case ThreadPolicy::COUNTER_OFF_1:
                return ThreadByCounter(VerbCall::WRITE, 1U, "WRITE");
            case ThreadPolicy::BY_CHANNEL:
                // NHR_RUN：threads[channelIdx] 在 DoNhrRun 的通道循环内取（载荷
                // channelsPerRank 是动词级知识，此入口无通道语境）——不可达
            case ThreadPolicy::NOT_WIRED:
            case ThreadPolicy::NA:
            default:
                Fail("verb '" + std::string(VerbName(verb)) +
                     "' has no thread policy in AICPU sink (BROADCAST/CCU-only)");
                return 0;
        }
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
            case VerbCall::WRITE:
                return DoWrite(call);
            case VerbCall::REDUCE:
                return DoReduce(call);
            case VerbCall::COPY:
            case VerbCall::POST_COPY:
                return DoCopy(call);
            case VerbCall::SYNC_PEER_PRE:
                return DoSyncPre();
            case VerbCall::SYNC_PEER_POST:
                return DoSyncPost();
            // R15 · ②（M0_R15 §四）：NHR_RUN 的 AICPU 半边执行（gated——
            // UseUnifiedNhrAicpuPath 关闭时 Emit 不产 NHR_RUN 动词，此分支不可达；
            // 手工构造 NHR_RUN 动词直发未 gated sink 时按原 default 拒绝语义。
            // 载荷 = VerbCall.nhr 11 标量；步几何 CalcNhrSteps 重算）。
            // R18 · #11 全删轮（用户裁定 2026-10-14）：NHR_RUN 宏动词 case 删除——
            // NHR 步进流一律逐动词展开（SLICE_REDUCE/CH_SYNC，下行两 case）；宏
            // 形态与展开形态均未经真机验证，双路径无保留价值，B 形态帧预算的
            // 段切分兜底留 B 形态启用时验证。
            // R18 · #11：NHR 展开动词（M0_R18 §一——与 mesh 逐动词地位对等）。
            // 几何经公共件 NhrSliceOf（与 DoNhrRun 同源）；方向执行端自判
            //（isDmaRead 通道协议知识下沉——Emit 产物两侧同构）。
            case VerbCall::SLICE_REDUCE:
                if (!call.nhr.valid || !UseUnifiedNhrAicpuPath()) {
                    Fail("SLICE_REDUCE payload invalid or NHR AICPU path gated off");
                    return -1;
                }
                return DoSliceReduce(call);
            case VerbCall::CH_SYNC:
                if (!call.nhr.valid || !UseUnifiedNhrAicpuPath()) {
                    Fail("CH_SYNC payload invalid or NHR AICPU path gated off");
                    return -1;
                }
                return DoChSync(call);
            default:
                // BROADCAST：CCU 声明式专属（AICPU 命令式不用——语义等价展开 =
                // copy + read×(N-1)，§6.13.2 对照表，AicpuCollectAll Emit 即此形态）
                // ——显式拒绝
                Fail("verb '" + std::string(VerbName(call.verb)) + "' not wired in AICPU sink scope "
                     "(BROADCAST is CCU declarative-only)");
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
        // :103 循环 threads.size()=N-1 与 :216 threads[threadIdx] 同口径）。
        // R16 · #7：策略声明收敛到 kAicpuVerbMap READ 行（COUNTER_OFF_0），
        // 本处经 ThreadFor(READ) 查表。
        const ThreadHandle thread = ThreadFor(VerbCall::READ);
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
    // 早退同语义（wrapper :721-723 返回 SUCCESS）。
    // R16 · #7：线程取值改走表驱动 ThreadFor（kAicpuVerbMap COPY/POST_COPY 行
    // = MAIN——恒主线程，与原 MainThread() 直调等价）。
    int32_t DoCopy(const VerbCall& call)
    {
        if (call.size == 0) {
            return 0;
        }
        const ThreadHandle thread = ThreadFor(call.verb);
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

    // WRITE（R12 · WP-9②，2026-10-09）：SendRecvWrite 五步（R12-2③——与 READ 同构
    // 方向相反，wrapper DoSendRecvBatchTx :116-140 的逐条 fallback 形态）：
    //   ①ChNotifyRecord(thread, recvCh, ACK)  ②ChNotifyWait(thread, sendCh, ACK)
    //   ③HcommWriteOnThread(sendCh, dst, src, len)  ④ChNotifyRecord(sendCh, DATA_SIGNAL)
    //   ⑤ChNotifyWait(recvCh, DATA_SIGNAL)
    // ★线程选择（R12-2②）：AICPU RS 的 threadNum=N（≠AG 的 N-1），WRITE 用
    //   threads[1..N-1]（queIdx 从 1 起，ins_temp_reduce_scatter_mesh_1D.cc:180-181/
    //   :243-245），主线程不下发写——DoWrite 独立计数 + 偏移 1（照抄 ThreadForRead
    //   的"READ 计数含主线程"必错位：第 i 条 WRITE 对 threads[i+1]）。
    //   R16 · #7：策略声明收敛到 kAicpuVerbMap WRITE 行（COUNTER_OFF_1），
    //   本处经 ThreadFor(WRITE) 查表。
    // ★dst 合成（R12-2①）：写侧目的 = 对端 cclBuff 的 me 槽——绝对基址 =
    //   ChannelInfo.remoteCclMem.addr（alg_param.h:453，layout 里没有）+ 帧 dst
    //   结构量。tx/rx 同通道口径（:199-200 linkSend=linkRecv=curChannels[0]）。
    int32_t DoWrite(const VerbCall& call)
    {
        const ThreadHandle thread = ThreadFor(VerbCall::WRITE);
        if (thread == 0) {
            return -1;    // Fail 已置
        }
        const ChannelInfo* ch = ChannelOf(call.peer);
        if (ch == nullptr) {
            return -1;
        }
        const ChannelHandle handle = ch->handle;
        // ① 通道级 ACK record（wrapper :124 —— recvCh 半边；同通道口径下即告知对端
        //    本端就绪）。方向与 READ 相反：READ 先 record ACK（等对端写权），WRITE
        //    先 record ACK（告知对端本端将写）
        int32_t r = HcommChannelNotifyRecordOnThread(thread, handle, NOTIFY_IDX_ACK);
        if (r != 0) {
            Fail("WRITE step1 ACK record ret=" + std::to_string(r));
            return r;
        }
        // ② 等 sendCh ACK（wrapper :125-126——对端 cclBuff 可用的握手）
        r = HcommChannelNotifyWaitOnThread(thread, handle, NOTIFY_IDX_ACK, CUSTOM_TIMEOUT);
        if (r != 0) {
            Fail("WRITE step2 ACK wait ret=" + std::to_string(r));
            return r;
        }
        // ③ 写（wrapper :129-138 的单 slice 形态；dst = 对端 cclBuff 基址 + 动词
        //    dst 结构量——remoteCclMem 合成，R12-2①）
        if (call.size != 0) {
            void* dst = reinterpret_cast<void*>(reinterpret_cast<u64>(ch->remoteCclMem.addr) + call.dst);
            const void* src = reinterpret_cast<const void*>(call.src);
            r = HcommWriteOnThread(thread, handle, dst, src, call.size);
            if (r != 0) {
                Fail("WRITE step3 HcommWriteOnThread ret=" + std::to_string(r));
                return r;
            }
        } else {
            // size=0 与 wrapper 的 continue 语义一致（跳过写，握手不省）
        }
        // ④ DATA_SIGNAL record（wrapper :132-133——写毕告知）
        r = HcommChannelNotifyRecordOnThread(thread, handle, NOTIFY_IDX_DATA_SIGNAL);
        if (r != 0) {
            Fail("WRITE step4 DATA_SIGNAL record ret=" + std::to_string(r));
            return r;
        }
        // ⑤ 等 recvCh DATA_SIGNAL（wrapper :134-135——对端收毕）
        r = HcommChannelNotifyWaitOnThread(thread, handle, NOTIFY_IDX_DATA_SIGNAL, CUSTOM_TIMEOUT);
        if (r != 0) {
            Fail("WRITE step5 DATA_SIGNAL wait ret=" + std::to_string(r));
            return r;
        }
        return 0;
    }

    // REDUCE（R12 · WP-9②）：LocalReduce wrapper 语义（alg_data_trans_wrapper.cc
    // :737-758——R12-2⑤）：非 64bit/PROD → HcommLocalReduceOnThread（threads[0]
    // 主线程，RS :167）；64bit/PROD → wrapper 内 AicpuReduce 分叉（:741-744——AICPU
    // 编译面符号，host sink 面不可链；由后端 Caps.support64bitReduce=false 的
    // Validate 前置拦截保证不可达，本 sink 对 64bit/PROD 显式拒绝不静默）。
    //   src = cclBuff 对端槽（绝对 = cclMem.addr + 动词 src）、dst = output 我的槽。
    // R16 · #7：线程取值改走表驱动 ThreadFor（REDUCE 行 = MAIN）。
    // ★真签名（hcomm_primitives.h:99）：count=元素数（非字节）+ dataType + reduceOp
    //   ——shim 面的 4 参签名是 R12 首版误简，本调用按真签名 6 参；dataType/reduceOp
    //   从 sink 构造注入（HcommVerbSinkRes 扩两字段，缺省 FP16/SUM——AG 单一用途
    //   的 PoC 口径，真链路由消费方从 OpParam 注入）。
    int32_t DoReduce(const VerbCall& call)
    {
        if (call.size == 0) {
            return 0;
        }
        const ThreadHandle thread = ThreadFor(call.verb);
        if (thread == 0) {
            return -1;
        }
        void* dst = reinterpret_cast<void*>(call.dst);
        const void* src = reinterpret_cast<const void*>(call.src);
        const uint64_t count = (reduceDataTypeSize_ > 0U) ? call.size / reduceDataTypeSize_ : call.size;
        const int32_t r = HcommLocalReduceOnThread(
            thread, dst, src, count, reduceDataType_, reduceOp_);
        if (r != 0) {
            Fail("REDUCE HcommLocalReduceOnThread ret=" + std::to_string(r));
            return r;
        }
        return 0;
    }

    // algRank → userRank（NHR 步表值是域内 algRank，通道表键是 userRank——
    // 载荷 domainRanks 映射；越界 0xFFFFFFFF）
    static u32 UserRankOfAlg(const VerbCall::NhrRunScalars& sc, u32 algRank)
    {
        if (algRank >= sc.domainRanks.size()) {
            return 0xFFFFFFFFU;
        }
        return sc.domainRanks[algRank];
    }

    // R18 · #11：SLICE_REDUCE 执行——方向无关的"该 slot 与对端规约合一"。
    // isDmaRead 自判（DoNhrRun :446-450 同式逐 peer 首通道协议）→ WR 发对端
    //（HcommWriteReduceOnThread）/ RD 拉对端（HcommReadReduceOnThread）；几何
    // 全部经 NhrSliceOf 公共件（与 DoNhrRun 同源——本函数零自算偏移）。
    // repeat 轮循环在动词内（帧槽/载荷 repeatNum 种子——循环维不展开，M0_R18 §二）。
    int32_t DoSliceReduce(const VerbCall& call)
    {
        const VerbCall::NhrRunScalars& sc = call.nhr;
        if (res_.threads.empty()) {
            Fail("SLICE_REDUCE with empty thread pool");
            return -1;
        }
        bool isDmaRead = false;
        for (std::map<u32, std::vector<ChannelInfo>>::const_iterator it = res_.channels.begin();
             it != res_.channels.end(); ++it) {
            if (!it->second.empty() && it->second[0].protocol == CommProtocol::COMM_PROTOCOL_PCIE) {
                isDmaRead = true;
                break;
            }
        }
        const ThreadHandle thread = res_.threads[0];   // BY_CHANNEL：devkit ch=1 → threads[0]
        const u64 rptNum = (sc.repeatNum == 0ULL) ? 1ULL : sc.repeatNum;
        const u64 srcCountBase = (reduceDataTypeSize_ > 0ULL) ? sc.sliceSize / reduceDataTypeSize_
                                                              : sc.sliceSize;
        for (u64 rpt = 0; rpt < rptNum; ++rpt) {
            const NhrSliceGeom g = NhrSliceOf(sc, sc.stepIdx, sc.sliceIdx, isDmaRead, rpt);
            if (g.size == 0ULL) {
                Fail("SLICE_REDUCE geometry degenerated (step/sliceIdx out of range)");
                return -1;
            }
            const u32 peerUser = UserRankOfAlg(sc, g.peerAlg);
            if (peerUser == 0xFFFFFFFFU) {
                Fail("SLICE_REDUCE algRank out of domain (payload rankNum mismatch)");
                return -1;
            }
            const ChannelInfo* chInfo = ChannelOfIdx(peerUser, 0U);
            if (chInfo == nullptr) {
                return -1;   // Fail 已置（建链集外 peer = 正确拒绝）
            }
            int32_t r = 0;
            if (isDmaRead) {
                void* dst = reinterpret_cast<void*>(g.localOff);
                void* src = reinterpret_cast<void*>(reinterpret_cast<u64>(chInfo->remoteCclMem.addr) +
                                                   g.remoteOff);
                r = HcommReadReduceOnThread(thread, chInfo->handle, dst, src, srcCountBase,
                                            reduceDataType_, reduceOp_);
            } else {
                void* src = reinterpret_cast<void*>(g.localOff);
                void* dst = reinterpret_cast<void*>(reinterpret_cast<u64>(chInfo->remoteCclMem.addr) +
                                                   g.remoteOff);
                r = HcommWriteReduceOnThread(thread, chInfo->handle, dst, src, srcCountBase,
                                             reduceDataType_, reduceOp_);
            }
            if (r != 0) {
                Fail("SLICE_REDUCE ret=" + std::to_string(r));
                return r;
            }
        }
        return 0;
    }

    // R18 · #11：CH_SYNC 执行——通道级握手半边（四态：WR/RD × ACK/DATA）。
    // call.src=WR peer（sendTo）、call.dst=RD peer（recvFrom）、call.size=0/1（ACK/DATA）
    // ——与 DoNhrRun :487-496/:537-543 的四分支逐条对应（动词化平移）。
    int32_t DoChSync(const VerbCall& call)
    {
        const VerbCall::NhrRunScalars& sc = call.nhr;
        if (res_.threads.empty()) {
            Fail("CH_SYNC with empty thread pool");
            return -1;
        }
        bool isDmaRead = false;
        for (std::map<u32, std::vector<ChannelInfo>>::const_iterator it = res_.channels.begin();
             it != res_.channels.end(); ++it) {
            if (!it->second.empty() && it->second[0].protocol == CommProtocol::COMM_PROTOCOL_PCIE) {
                isDmaRead = true;
                break;
            }
        }
        const ThreadHandle thread = res_.threads[0];   // BY_CHANNEL：devkit ch=1 → threads[0]
        // 方向选 peer：RD 拉 → recvFrom（call.dst）；WR 发 → sendTo（call.src）
        const u32 peerAlg = isDmaRead ? static_cast<u32>(call.dst) : static_cast<u32>(call.src);
        const u32 peerUser = UserRankOfAlg(sc, peerAlg);
        if (peerUser == 0xFFFFFFFFU) {
            Fail("CH_SYNC algRank out of domain");
            return -1;
        }
        const ChannelInfo* chInfo = ChannelOfIdx(peerUser, 0U);
        if (chInfo == nullptr) {
            return -1;
        }
        // 四态 dir 解码（M0_R18 §三——size 槽 {0=ACK-Rec, 1=ACK-Wait, 2=DATA-Rec,
        // 3=DATA-Wait}）：半边 = bit1（0/1=ACK、2/3=DATA）；Record/Wait = bit0
        //（0/2=Record、1/3=Wait）。全删轮修复：旧解码 idx=（size==0?ACK:DATA）
        // + isWait=（size>=2）在 ACK-Wait(1)/DATA-Rec(2) 两态错位（旧断言只锁
        // 首条 Record 未及 Wait 侧——T5 展开序位锚暴露）
        const u32 idx = ((call.size & 2ULL) == 0ULL) ? NOTIFY_IDX_ACK : NOTIFY_IDX_DATA_SIGNAL;
        const bool isWait = ((call.size & 1ULL) != 0ULL);
        int32_t r = 0;
        if (isWait) {
            r = HcommChannelNotifyWaitOnThread(thread, chInfo->handle, idx, CUSTOM_TIMEOUT);
        } else {
            r = HcommChannelNotifyRecordOnThread(thread, chInfo->handle, idx);
        }
        if (r != 0) {
            Fail("CH_SYNC ret=" + std::to_string(r));
            return r;
        }
        return 0;
    }

    // R18 · #11 全删轮（用户裁定 2026-10-14）：DoNhrRun（宏动词执行——步循环
    // 内核 + 五段握手 + skipStep0Tx 自判）整体删除。NHR 步进流一律走
    // DoSliceReduce/DoChSync 逐动词执行（上方）；几何公共件 NhrSliceOf 保留
    // （两展开动词消费）。宏/展开两形态均未经真机验证，双路径无保留价值。
    // NHR 通道查表（带 channelIdx——模板 channels_[peer].at(channelIdx) :278-279
    // 同式；与 ChannelOf（取 [0]）并存：mesh 动词取首通道、NHR 按通道序）
    const ChannelInfo* ChannelOfIdx(u32 peer, u32 channelIdx)
    {
        const auto it = res_.channels.find(peer);
        if (it == res_.channels.end() || static_cast<size_t>(channelIdx) >= it->second.size()) {
            Fail("no channel[" + std::to_string(channelIdx) + "] to peer " + std::to_string(peer) +
                 " (NHR connect-set member expected)");
            return nullptr;
        }
        return &it->second[channelIdx];
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

    // READ 的线程选择（R16 · #7：表驱动 ThreadFor(READ) → COUNTER_OFF_0）：
    // 第 i 条 READ → threads[i]（对照现状 threads[threadIdx]，
    // ins_temp_all_gather_mesh_1D.cc:103/:216——threadIdx 与 peer 步序一致，
    // **threads[0]（主线程）承载首条 read**，i∈[0,N-2] 全用主+从）。
    // 与 WP-A1 首版（i→threads[i+1]，read 数 > slave 数拒绝）的差别：按主会话
    // M0 对账后的存量形态修正——AG mesh 的 threadNum=N-1（主+从共 N-1 条），
    // read 数 = N-1 恰好等于 threads.size()，主线程承载第 0 条、从线程承载
    // 第 1..N-2 条。越界判定 = readIdx ≥ threads.size()。
    ThreadHandle ThreadForRead()
    {
        return ThreadFor(VerbCall::READ);
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

    // WRITE 的线程选择（R16 · #7：表驱动 ThreadFor(WRITE) → COUNTER_OFF_1）：
    // 第 i 条 WRITE → threads[i+1]（偏移 1——主线程不下发写；对照 RS 现状
    // queIdx 从 1 起 :180-181/:243-245）。独立计数：只数 WRITE（与 READ 计数
    // 互不干扰——同一 sink 实例内两类动词可共存）。越界判定 =
    // writeIdx+1 ≥ threads.size()（threadNum=N 时 write 数 = N-1 恰好用满
    // threads[1..N-1]）。
    ThreadHandle ThreadForWrite()
    {
        return ThreadFor(VerbCall::WRITE);
    }

    // 计数器策略唯一实现点（R16 · #7）：第 i 条 countVerb → threads[i+offset]。
    // 独立计数（只数 countVerb——READ/WRITE 计数互不干扰）；offset 从线程策略
    // 表来（READ=0 主线程承载首条 / WRITE=1 主线程不下发写）。
    ThreadHandle ThreadByCounter(VerbCall::Verb countVerb, u32 offset, const char* name)
    {
        u32 idx = 0;
        for (const auto& c : calls_) {
            if (c.verb == countVerb) {
                ++idx;
            }
        }
        if (static_cast<size_t>(idx) + offset >= res_.threads.size()) {
            Fail(std::string(name) + " exceeds thread count (idx=" + std::to_string(idx) +
                 ", threads=" + std::to_string(res_.threads.size()) + ")");
            return 0;
        }
        return res_.threads[idx + offset];
    }

    void Fail(const std::string& msg)
    {
        err_ = msg;
    }

    HcommVerbRes res_;
    std::vector<HcommCallRecord> calls_;
    int32_t errCode_{0};
    std::string err_;
    HcommDataType reduceDataType_{HCOMM_DATA_TYPE_FP16};   // R12 · WP-9②：REDUCE 注入
    HcommReduceOp reduceOp_{HCOMM_REDUCE_SUM};
    uint64_t reduceDataTypeSize_{2};

    // hcomm 域 dataType → 字节宽（HCOMM_DATA_TYPE_* 与 HCCL_DATA_TYPE_* 同值序——
    // DATATYPE_SIZE_TABLE 的 shim 面 alg_param.h 已带；真头经 hcomm_primitives 链同表）
    static uint64_t DataTypeSizeOf_(HcommDataType t)
    {
        return DATATYPE_SIZE_TABLE[t];
    }
};

} // namespace mc2_ops_hccl::refactor

#endif
