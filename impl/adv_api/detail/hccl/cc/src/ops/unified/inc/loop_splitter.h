/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * CANN Open Software License Agreement Version 2.0.
 *
 * 重构 PoC · L4 公共组件 · LoopSplitter（从 backend_registry.h 拆出，2026-09-21）
 *
 * CCU 三段切分的唯一实现点（公共组件 B，纯函数、无引擎头，方案 §6.5）。
 * 拆出原因：plan_serializer.h（action_spec.h 尾部 include）需要 LoopSplitter 完整
 * 类型；backend_registry.h 又 include action_spec.h——若 plan_serializer 直接引
 * backend_registry.h，在"backend_registry 先被 include"的次序下其 guard 已置位、
 * LoopSplitter 不可见（include 环的 guard 空过问题）。LoopSplitter 本身只依赖
 * u32/u64 标量，最小化成独立头后依赖图为无环：
 *   action_spec.h → plan_serializer.h → loop_splitter.h → alg_param.h
 * 内容与拆出前逐字一致（backend_registry.h include 本头，行为零变化）。
 *
 * 与现状 CalGoSize（ccu_kernel_alg_base.cc:45）逐公式对齐：
 *   loopSize = loopCount × memSlice；m = size/loopSize、n = 余量 loop 数、p = 尾块字节
 *   边界特例：size == maxSize（= loopSize × (MaxLoopIterNum()+1)）时取满
 *   （m = MaxIter, n = loopCount-1, p = memSlice）——对应硬件 loop 计数器位宽上限
 * 输出四字段 {addrOffset, loopParam, parallelParam, residual}：
 *   addrOffset = memSlice × loopCount × m（第一段末尾偏移，接力棒）
 *   loopParam  = m（满并行串行轮数）
 *   parallelParam = GetParallelParam 打包的零头展开（三段位域，见 ParallelParam 注）
 *   residual   = 尾块（不足一个 memSlice 的字节；整 loop 零头时 = memSlice）
 */
#ifndef REFACTOR_POC_LOOP_SPLITTER_H
#define REFACTOR_POC_LOOP_SPLITTER_H

#include <cstdint>

#include "alg_param.h"   // u32/u64

namespace mc2_ops_hccl::refactor {

class LoopSplitter {
public:
    // 硬件配置层（现状 LoopGroupConfig；devkit 仓同源值见 ccu_kernel_alg_base.h:27-29：
    // msInterleave=8、默认 loopCount=128、memSlice=4096=CCU_MS_SIZE）
    struct LoopGroupConfig {
        u32 msInterleave{8};
        u32 loopCount{128};
        u64 memSlice{4096};
    };

    // 运行时切分层输出，4 字段定长（§6.5 规格；即现状返回值 {offset, loopIterNum, loopExtendNum, tailSize}）
    struct GoSize {
        u64 addrOffset{0};
        u64 loopParam{0};
        u64 parallelParam{0};
        u64 residual{0};
    };

    // loop 计数器位宽上限：12 bit 全 1 = 8191（现状 GetMaxLoopIterNum = SetBits(12)，
    // 而 SetBits(end) = (1<<(end+1))-1，故 SetBits(12) = (1<<13)-1 = 8191，ccu_kernel_utils.cc:35）
    static u64 MaxLoopIterNum() { return (1ULL << 13) - 1; }

    // 现状 GetParallelParam（ccu_kernel_utils.cc:53）的三段位域打包：
    //   [55..61]=repeatNum(7bit) [48..54]=repeatLoopIndex(7bit) [41..47]=totalLoopNum(7bit)
    static u64 ParallelParam(u64 repeatNum, u64 repeatLoopIndex, u64 totalLoopNum)
    {
        constexpr u64 REPEAT_BIT_MASK = (1ULL << 7) - 1;
        constexpr u64 REPEAT_NUM_SHIFT = 55;
        constexpr u64 REPEAT_LOOP_SHIFT = 48;
        constexpr u64 TOTAL_LOOP_SHIFT = 41;
        return ((repeatNum & REPEAT_BIT_MASK) << REPEAT_NUM_SHIFT) |
               ((repeatLoopIndex & REPEAT_BIT_MASK) << REPEAT_LOOP_SHIFT) |
               ((totalLoopNum & REPEAT_BIT_MASK) << TOTAL_LOOP_SHIFT);
    }

    static GoSize Calc(u64 size, const LoopGroupConfig& cfg)
    {
        GoSize go;
        const u64 loopSize = static_cast<u64>(cfg.loopCount) * cfg.memSlice;
        const u64 maxSize = loopSize * (MaxLoopIterNum() + 1);

        u64 m = size / loopSize;
        u64 n = (size - m * loopSize) / cfg.memSlice;
        u64 p = size - m * loopSize - n * cfg.memSlice;

        // 边界特例：正好取满（size == maxSize）时直接取到上限，避免 m 溢出 12bit
        if (size == maxSize) {
            m = MaxLoopIterNum();
            n = cfg.loopCount - 1;
            p = cfg.memSlice;
        }

        go.addrOffset = cfg.memSlice * cfg.loopCount * m;   // 第一段末尾偏移（接力棒）
        go.loopParam = m;                                   // 满并行串行轮数

        // 零头展开（现状四分支，ccu_kernel_alg_base.cc:71-86 逐字对齐）：
        //   n==0&&p==0 整除 → 只走 LoopGroup0，零头为 0
        //   n!=0&&p==0    → 零头整 loop，repeatLoopIndex=0、totalLoop=1
        //   n==0&&p!=0    → 只有尾块
        //   其余           → 零头 loop + 尾块（LoopNumTwo=2）
        if (n == 0 && p == 0) {
            go.parallelParam = 0;
            go.residual = 0;
        } else if (n != 0 && p == 0) {
            go.parallelParam = ParallelParam(n - 1, 0, 1);
            go.residual = cfg.memSlice;
        } else if (n == 0 && p != 0) {
            go.parallelParam = ParallelParam(0, 0, 1);
            go.residual = p;
        } else {
            go.parallelParam = ParallelParam(n - 1, 1, 2);
            go.residual = p;
        }
        return go;
    }

    // 兼容旧调用面（CollectAll::BuildArgs 的 goSize[4]）：默认硬件配置。
    // 现状 CalGoSize(size) 单参版 = 默认 LoopGroupConfig 的薄包装（ccu_kernel_alg_base.cc:15）
    void Calc(u64 size, u64 (&goSize)[4]) const
    {
        const GoSize go = Calc(size, LoopGroupConfig{});
        goSize[0] = go.addrOffset;
        goSize[1] = go.loopParam;
        goSize[2] = go.parallelParam;
        goSize[3] = go.residual;
    }
};

// ---------------------------------------------------------------------------
// ReduceSlice（RS MESH）的 goSize 独立 config + chunk 切分参数（R6/裁决五，2026-09-21）
//
// 落本头（而非 l3/action_reduce_slice.h）的原因见 plan_serializer.h 的 include 注：
// 本头是 action_spec.h 尾部链（值对象→序列化）上的最小依赖层，L3 动作头与序列化器
// 共用同一实现点（零复制）；L3 头另依赖 action_collect_all.h（NextPeer），值对象链
// 不可反向 include L3。
//
// 【config 独立声明（裁决五）】RS 用 {msInterleave=8, loopCount=16, memSlice=4096}
//（loopSize=65536），与 CollectAll 的写入方 config {8, 8, 32768}（loopSize=262144）
// 是两套口径——漏配即"同尺寸两 config 产出不同 goSize"的漂移病灶（本头 CalcCollectAllWriter
// 侧同理）。证据链：
//   CCU_LOOP_COUNT_M2M_RE=16（hccl_inner_def.h:172）+ CCU_MEMSLICE_SIZE=4096（:176）
//   + 设备侧写入方 CalcGoSize(chunkSize, CCU_LOOP_COUNT_M2M_RE, CCU_MEMSLICE_SIZE, ...)
//   （hccl_ccu_v0_prepare.h:561-563）+ kernel 侧本地常量 CCU_LOOP_COUNT_M2M_RE=16 及
//   "必须与宿主 CalcGoSize loopCount 一致否则 goSize 与 LoopGroup 步进错位"注释
//   （ccu_kernel_kfc_reduce_scatter_mesh1d_mem2mem.cc:26-30）。
// 【chunk 公式】对齐设备侧写入方 CcuPrepareForConcurrentReduceScatterM2M（:546-563）：
//   chunkSize = scratchSize(64MB)/rankNum/128*128（128B 对齐下取整；scratchSize 与
//   host 侧 alloc_ctx_res.cc:266 的 4*MC2_WORKSPACE_SIZE=64MB 双端契约）
//   chunkLoopNum = UINT64_MAX - chunkCount（倒计数哨兵：kernel
//   CCU_WHILE(chunkLoopNum != UINT64_MAX) 每轮 +1，:377-391）
//   tailSize = sliceSize - fullChunkCount*chunkSize；full/tail 两组 goSize 分别按
//   chunkSize/tailSize 算（满 chunk 用 fullGo、尾 chunk 用 tailGo，kernel :380-390）
// ---------------------------------------------------------------------------
inline LoopSplitter::LoopGroupConfig RsGoSizeConfig()
{
    LoopSplitter::LoopGroupConfig cfg;
    cfg.msInterleave = 8U;     // CCU_MS_INTERLEAVE（ccu_kernel_alg_base.h:27）
    cfg.loopCount = 16U;       // CCU_LOOP_COUNT_M2M_RE（hccl_inner_def.h:172）
    cfg.memSlice = 4096ULL;    // CCU_MEMSLICE_SIZE（:176；== CCU_MS_SIZE）
    return cfg;
}

// RS chunk 切分参数（BuildArgs / SerializeSegmentV3 / 后端三处共用）
struct RsChunkParams {
    u64 chunkSize{0};        // scratchSize/rankNum/128*128 下取整对齐
    u64 chunkLoopNum{0};     // UINT64_MAX - chunkCount（倒计数哨兵）
    u64 tailSize{0};         // sliceSize - fullChunkCount*chunkSize
    LoopSplitter::GoSize fullGo;   // Calc(chunkSize, RsGoSizeConfig())
    LoopSplitter::GoSize tailGo;   // Calc(tailSize,  RsGoSizeConfig())
};

// 唯一实现点（sliceSize=0 时 chunkCount=0 → chunkLoopNum=UINT64_MAX，kernel 零循环
// 直接过——上游 Compile 对空数据另有语义，此处不防御）
inline RsChunkParams CalcRsChunks(u64 sliceSize, u32 rankNum)
{
    RsChunkParams c;
    constexpr u64 SCRATCH_SIZE = 64ULL * 1024 * 1024;   // 4 * MC2_WORKSPACE_SIZE(16MB)
    constexpr u64 MIN_SLICE_ALIGN = 128ULL;             // 写入方 :535 splitAlignment
    const u64 chunkSize = (rankNum > 0) ? (SCRATCH_SIZE / rankNum / MIN_SLICE_ALIGN * MIN_SLICE_ALIGN) : 0;
    const u64 fullChunkCount = (sliceSize == 0 || chunkSize == 0) ? 0 : (sliceSize - 1) / chunkSize;
    const u64 chunkCount = (sliceSize == 0 || chunkSize == 0) ? 0 : fullChunkCount + 1;
    c.chunkSize = chunkSize;
    c.chunkLoopNum = ~0ULL - chunkCount;                // UINT64_MAX - chunkCount（写入方 :558 同式）
    c.tailSize = sliceSize - fullChunkCount * chunkSize;
    c.fullGo = LoopSplitter::Calc(chunkSize, RsGoSizeConfig());
    c.tailGo = LoopSplitter::Calc(c.tailSize, RsGoSizeConfig());
    return c;
}

} // namespace mc2_ops_hccl::refactor

#endif
