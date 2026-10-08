// R6.5 · WP-C1 · B-CCU② 覆写 golden 测试（host 镜像公式）
//
// 被测对象：hccl_ccu_v0_prepare.h CcuPrepareForUnifiedPlan 的 AG 段帧数据槽覆写序
//（__aicore__ 设备函数 host 不可执行——本测试以**同公式同槽序的 host 镜像**
// OverwriteCollectAllFrame 复刻覆写路径，逐槽断言 = v2 CcuPrepareForAllGatherM2M
// :273-296 公式手算期望；设备函数侧以源码级逐槽对账（测试头注明））。
//
// 期望值全部手写字面量（golden 主用例数值与审查方 R5 ST A 案交叉锚）：
//   8 卡 FP16 count=300000 rankId=3 → 槽 3=1800000、槽 4=600000、
//   goSize={524288,2,36314670042185728,10176}、槽 10=0xFFFFFFFFFFFFFFFE
//
// 三坑断言（验收硬条件）：
//   坑1 槽 14：资源期 blob 槽 14 恒 1（count=0 病灶）+ golden 双态（合成后 1/0）
//   坑2 槽 1/2：非零 blob 槽 1 的合成式（防"只测退化态"）
//   坑3 槽 3：strideCount≠0 分支（两值显式不等）
// 边界双取：2GiB 通过 / 2GiB+1 拒
// RS 帧级拒收：帧槽 0 != CollectAll → 拒
#include "action_spec.h"
#include "action_collect_all.h"
#include "action_reduce_slice.h"   // R7 · WP-1：RS Compile 链（ActionForOp 映射 ReduceSlice）
#include "backend_registry.h"
#include "l1_adapter.h"
#include "plan_serializer.h"
#include "backend_ccu_collect_all.h"
#include "backend_ccu_reduce_slice.h"  // R7 · WP-1：RS CCU 后端注册（Validate 需要）

#include <cstdint>
#include <cstdio>
#include <string>
#include <vector>
using namespace mc2_ops_hccl::refactor;

static int g_pass = 0, g_fail = 0;
static void Check(bool cond, const std::string& what)
{
    if (cond) { ++g_pass; printf("  [PASS] %s\n", what.c_str()); }
    else      { ++g_fail; printf("  [FAIL] %s\n", what.c_str()); }
}

// ---------------------------------------------------------------------------
// host 镜像：exec 现值 + blob 帧 → 覆写后帧（CcuPrepareForUnifiedPlan 覆写序
// 逐槽同式——源码级对账：hccl_ccu_v0_prepare.h 的 B-CCU② 段）
// ---------------------------------------------------------------------------
// 槽位别名（kfc_server_message.h KfcUnifiedPlanParamIndex 的 host 镜像——不经设备头）
constexpr uint32_t KFC_SLOT_ACTION_ID = 0;
constexpr uint32_t KFC_SLOT_INPUT = 1;
constexpr uint32_t KFC_SLOT_OUTPUT = 2;
constexpr uint32_t KFC_SLOT_OUTPUT_OFFSET = 3;
constexpr uint32_t KFC_SLOT_SLICE_SIZE = 4;
constexpr uint32_t KFC_SLOT_GO0 = 5, KFC_SLOT_GO1 = 6, KFC_SLOT_GO2 = 7, KFC_SLOT_GO3 = 8;
constexpr uint32_t KFC_SLOT_CUR_IN_OFF = 9;
constexpr uint32_t KFC_SLOT_REPEAT_INV = 10;
constexpr uint32_t KFC_SLOT_IN_RPT_STRIDE = 11, KFC_SLOT_OUT_RPT_STRIDE = 12;
constexpr uint32_t KFC_SLOT_LAST_SLICE = 13;
constexpr uint32_t KFC_SLOT_IN_OUT_EQUAL = 14;

struct CcuExecMsg {
    uint64_t sendBuf{0};
    uint64_t recvBuf{0};
    uint64_t count{0};
    uint64_t strideCount{0};
    uint32_t dataType{0};
    uint32_t rankId{0};
    uint32_t repeatIndex{0};
};

static uint64_t HostDts(uint32_t dt)
{
    // GetHcclDataTypeSize（hccl_inner_def.h:33）host 镜像——逐 idx 对齐设备
    // DATA_TYPE_MAP（:28）：{1,2,4,2,4,8,8,1,2,4,8,2,0,0,1,1,1,1,1}。
    // ★idx12 教训（R6.5 审查 MINOR-1，镜像漂移活例）：本镜像曾写 idx12=16（想当然
    // int128=16B），设备表 idx12=0——镜像比设备表"多态"会让后人扩 idx12 用例时
    // 得到错误期望（期望值源污染，非生产行为错误）。镜像纪律：以设备表逐 idx 抄写，
    // 不按类型语义推断。
    switch (dt) {
        case 0: case 7: case 14: case 15: case 16: case 17: case 18: return 1;   // int8/uint8/fp8 系
        case 1: case 3: case 8: case 11: return 2;                               // int16/fp16/uint16/bfp16
        case 2: case 4: case 9: return 4;                                        // int32/fp32/uint32
        case 5: case 6: case 10: return 8;                                       // int64/uint64/fp64
        case 12: case 13: return 0;                                              // 设备表 0（reserved 位）
        default: return 0;
    }
}

// 设备侧 CalcGoSize（hccl_ccu_xn_utils.h:148）host 镜像——含 GetParallelParameters
// 三段位域（:120 同式）
static void HostCalcGoSize(uint64_t sliceSize, uint64_t* go)
{
    const uint64_t loopCount = 8;          // CCU_LOOP_COUNT_M2M_AG
    const uint64_t memSlice = 4096 * 8;    // CCU_MEMSLICE_SIZE * 8
    const uint64_t loopSize = loopCount * memSlice;
    uint64_t m = sliceSize / loopSize;
    uint64_t n = (sliceSize - m * loopSize) / memSlice;
    uint64_t p = sliceSize - m * loopSize - n * memSlice;
    go[0] = loopSize * m;
    go[1] = m;
    if (n == 0 && p == 0) {
        go[2] = 0; go[3] = 0;
    } else if (n != 0 && p == 0) {
        go[2] = (((n - 1) & 127ULL) << 55) | (0ULL << 48) | (1ULL << 41);
        go[3] = memSlice;
    } else if (n == 0 && p != 0) {
        go[2] = (0ULL << 55) | (0ULL << 48) | (1ULL << 41);
        go[3] = p;
    } else {
        go[2] = (1ULL << 55) | (1ULL << 48) | (2ULL << 41);
        go[3] = p;
    }
}

// 设备侧 CalcGoSize 的 RS config host 镜像（R7 · WP-1）——loopCount=CCU_LOOP_COUNT_
// M2M_RE=16（hccl_inner_def.h:172）、memSlice=CCU_MEMSLICE_SIZE=4096（:176，不 ×8）
// ——与 AG 镜像 HostCalcGoSize 的 {8, 32768} 分立声明禁止照抄（漏配即 goSize 与
// LoopGroup 步进错位，kernel ccu_kernel_kfc_reduce_scatter_mesh1d_mem2mem.cc:26-30）
static void HostCalcRsGoSize(uint64_t size, uint64_t* go)
{
    const uint64_t loopCount = 16;
    const uint64_t memSlice = 4096;
    const uint64_t loopSize = loopCount * memSlice;
    uint64_t m = size / loopSize;
    uint64_t n = (size - m * loopSize) / memSlice;
    uint64_t p = size - m * loopSize - n * memSlice;
    go[0] = loopSize * m;
    go[1] = m;
    if (n == 0 && p == 0) {
        go[2] = 0; go[3] = 0;
    } else if (n != 0 && p == 0) {
        go[2] = (((n - 1) & 127ULL) << 55) | (0ULL << 48) | (1ULL << 41);
        go[3] = memSlice;
    } else if (n == 0 && p != 0) {
        go[2] = (0ULL << 55) | (0ULL << 48) | (1ULL << 41);
        go[3] = p;
    } else {
        go[2] = (1ULL << 55) | (1ULL << 48) | (2ULL << 41);
        go[3] = p;
    }
}

// 覆写结果（帧终值）：err 非空 = 拒收（上界/未知动作 ID——与设备函数 return 路径对应）
struct OverwriteResult {
    bool ok{false};
    std::string err;
    uint64_t slots[32]{0};   // 覆写后的落盘帧
};

// RS 帧实例化 host 镜像（R7 · WP-1：CcuPrepareForUnifiedPlan 的 ReduceSlice 分支
// 逐槽同式——源码级对账 hccl_ccu_v0_prepare.h 的 R7·WP-1 段；公式出处 =
// v2 CcuPrepareForConcurrentReduceScatterM2M :528-575 mission0 Mesh 区间）。
// scratchAddr/rankNum 是设备侧自持资源（ccuParam_），host 镜像经 CcuExecMsg 扩展
// 字段注入。
static OverwriteResult OverwriteReduceSliceFrame(const std::vector<uint64_t>& blobFrame32,
                                                 const CcuExecMsg& m, uint64_t scratchAddr, uint32_t rankNum)
{
    OverwriteResult r;
    if (blobFrame32[KFC_SLOT_ACTION_ID] != 1ULL) {
        r.err = "action != ReduceSlice";
        return r;
    }
    const uint64_t dts = HostDts(m.dataType);
    const uint64_t sliceSize = m.count * dts;   // RS 语义：count = 每 rank 输出份元素数
    const uint64_t repeatOffset = m.count * m.repeatIndex * dts;
    for (uint32_t i = 0; i < 32; ++i) {
        r.slots[i] = blobFrame32[i];
    }
    // 槽 1/2：合成式（blob 槽值 = 段内结构 offset）
    r.slots[1] = m.sendBuf + repeatOffset + blobFrame32[1];
    r.slots[2] = m.recvBuf + repeatOffset + blobFrame32[2];
    // 槽 3：scratch 直替
    r.slots[3] = scratchAddr;
    // 槽 4：rankSliceOffset 三分支
    r.slots[4] = m.rankId * ((m.strideCount == 0U) ? sliceSize : (m.strideCount * dts));
    // 槽 5..7：chunk 三元组（64MB scratch 契约 / 128B 对齐 / 倒计数哨兵）
    constexpr uint64_t kRsScratchSize = 64ULL * 1024 * 1024;
    constexpr uint64_t kRsSplitAlign = 128ULL;
    const uint64_t chunkSize = kRsScratchSize / rankNum / kRsSplitAlign * kRsSplitAlign;
    const uint64_t fullChunkCount = (sliceSize == 0U || chunkSize == 0U) ? 0 : (sliceSize - 1) / chunkSize;
    const uint64_t chunkCount = (sliceSize == 0U || chunkSize == 0U) ? 0 : fullChunkCount + 1;
    const uint64_t tailSize = sliceSize - fullChunkCount * chunkSize;
    r.slots[5] = chunkSize;
    r.slots[6] = ~0ULL - chunkCount;
    r.slots[7] = tailSize;
    // 槽 8..15：full/tail 双 goSize（★RS config：loopCount=16、memSlice=4096 不 ×8
    // ——与 AG 的 {8, 32768} 不同源，kernel :26-30 步进错位硬约束）
    uint64_t fullGo[4];
    uint64_t tailGo[4];
    HostCalcRsGoSize(chunkSize, fullGo);
    HostCalcRsGoSize(tailSize, tailGo);
    for (int i = 0; i < 4; ++i) {
        r.slots[8 + i] = fullGo[i];
        r.slots[12 + i] = tailGo[i];
    }
    r.ok = true;
    return r;
}

static OverwriteResult OverwriteCollectAllFrame(const std::vector<uint64_t>& blobFrame32,
                                                const CcuExecMsg& m)
{
    OverwriteResult r;
    // 帧级动作 ID 判据（设备函数帧内判：R7 起白名单 {CollectAll, ReduceSlice}，
    // 未知 ID 拒收——镜像 hccl_ccu_v0_prepare.h 帧循环首判）
    if (blobFrame32[KFC_SLOT_ACTION_ID] != 0ULL && blobFrame32[KFC_SLOT_ACTION_ID] != 1ULL) {
        r.err = "action not in {CollectAll, ReduceSlice} (unknown action id)";
        return r;
    }
    // goSize 上界（设备函数前置检查：262144 × 8192）
    const uint64_t dts = HostDts(m.dataType);
    const uint64_t sliceSize = m.count * dts;
    constexpr uint64_t kGoSizeLoopLimit = 262144ULL * 8192ULL;
    if (sliceSize > kGoSizeLoopLimit) {
        r.err = "sliceSize > 2GiB goSize loop limit";
        return r;
    }
    // 覆写序（设备函数逐槽同式）
    const uint64_t repeatOffset = m.count * m.repeatIndex * dts;
    const uint64_t outputOffset = m.rankId *
        ((m.strideCount == 0U) ? sliceSize : (m.strideCount * dts));
    for (uint32_t i = 0; i < 32; ++i) {
        r.slots[i] = blobFrame32[i];   // 先全量拷贝（设备函数的逐槽拷）
    }
    r.slots[1] = m.sendBuf + repeatOffset + blobFrame32[1];
    r.slots[2] = m.recvBuf + repeatOffset + blobFrame32[2];
    r.slots[3] = outputOffset;
    r.slots[4] = sliceSize;
    uint64_t go[4];
    HostCalcGoSize(sliceSize, go);
    r.slots[5] = go[0]; r.slots[6] = go[1]; r.slots[7] = go[2]; r.slots[8] = go[3];
    r.slots[13] = sliceSize;
    r.slots[14] = (r.slots[1] == r.slots[2] + r.slots[3]) ? 1ULL : 0ULL;
    r.ok = true;
    return r;
}

// 资源期 blob 构造（真 Compile：count=0 生产形态）
static std::vector<u64> CompileBlob(u32 rankSize, u64 count, const std::string& cfg,
                                    u64 inBaseOff = 0)
{
    OpParam op;
    op.opType = static_cast<int>(HcclCMDType::HCCL_CMD_ALLGATHER);
    op.DataDes.count = count;
    op.DataDes.dataType = HCCL_DATA_TYPE_FP16;
    op.engine = CommEngine::COMM_ENGINE_CCU;
    op.userRank = 0;
    TopoInfoWithNetLayerDetails topo;
    topo.userRankSize = rankSize; topo.topoLevelNums = 1; topo.level0Topo = Level0Shape::MESH_1D;
    std::vector<u32> ranks;
    for (u32 i = 0; i < rankSize; ++i) ranks.push_back(i);
    Plan plan; std::string err;
    if (L1Adapter::Compile(op, topo, {ranks}, cfg, plan, err) != HCCL_SUCCESS) {
        printf("  [FATAL] compile: %s\n", err.c_str());
        return {};
    }
    // 手工注入非零 inBaseOff（坑2：非零槽 1 的合成式用例）——重排 segments[0].layout
    if (inBaseOff != 0 && !plan.segments.empty()) {
        // Compile 产物不可回写（const 链），直接改 SerializePlanV3 输入：重建 plan 副本
        Plan p2 = plan;
        p2.segments[0].layout.inBaseOff = inBaseOff;
        plan = p2;
    }
    std::vector<u64> blob;
    if (!SerializePlanV3(plan, blob, err)) {
        printf("  [FATAL] serialize: %s\n", err.c_str());
        return {};
    }
    return blob;
}

// blob → 帧 0 的 32 槽视图
static std::vector<uint64_t> Frame0Of(const std::vector<u64>& blob)
{
    return std::vector<uint64_t>(blob.begin() + 7, blob.begin() + 7 + 32);
}

int main()
{
    printf("== ① golden 主用例：8 卡 FP16 count=300000 rankId=3（审查方 R5 ST A 案交叉锚） ==\n");
    {
        const std::vector<u64> blob = CompileBlob(8, 0, "sole[mesh]");   // 资源期 count=0
        const std::vector<uint64_t> frame = Frame0Of(blob);
        Check(frame.size() == 32, "blob 帧区 32 槽");
        // 资源期病灶事实：count=0 → 槽 4=0、槽 14 恒 1（0==0 误判）
        Check(frame[KFC_SLOT_SLICE_SIZE] == 0, "资源期槽 4 = 0（count=0 占位——覆写必要性证据）");
        Check(frame[KFC_SLOT_IN_OUT_EQUAL] == 1,
              "资源期槽 14 恒 1（count=0 全零误判——R5 B0 注释④病灶本体，透传必错）");

        CcuExecMsg m;
        m.sendBuf = 0x1000000; m.recvBuf = 0x2000000;
        m.count = 300000; m.dataType = 3 /*FP16*/; m.rankId = 3; m.repeatIndex = 0;
        const OverwriteResult r = OverwriteCollectAllFrame(frame, m);
        Check(r.ok, "覆写 ok: " + r.err);
        // 手算期望（全部字面量）：slice=600000、槽3=3*600000=1800000、
        // goSize m=600000/262144=2、n=(600000-524288)/32768=2、p=600000-524288-65536=10176
        Check(r.slots[KFC_SLOT_SLICE_SIZE] == 600000, "槽 4 = 600000");
        Check(r.slots[KFC_SLOT_OUTPUT_OFFSET] == 1800000, "槽 3 = 1800000");
        Check(r.slots[KFC_SLOT_GO0] == 524288, "槽 5 = 524288");
        Check(r.slots[KFC_SLOT_GO1] == 2, "槽 6 = 2");
        Check(r.slots[KFC_SLOT_GO2] == 36314670042185728ULL, "槽 7 = PP(1,1,2) = 36314670042185728");
        Check(r.slots[KFC_SLOT_GO3] == 10176, "槽 8 = 10176");
        Check(r.slots[KFC_SLOT_LAST_SLICE] == 600000, "槽 13 = 600000（AG 等分）");
        Check(r.slots[KFC_SLOT_REPEAT_INV] == 0xFFFFFFFFFFFFFFFEULL, "槽 10 = UINT64_MAX-1（透传）");
        Check(r.slots[KFC_SLOT_CUR_IN_OFF] == 0, "槽 9 = 0（透传）");
        Check(r.slots[KFC_SLOT_IN_RPT_STRIDE] == 0 && r.slots[KFC_SLOT_OUT_RPT_STRIDE] == 0,
              "槽 11/12 = 0（透传）");
        // 槽 1/2：合成式（blob 槽 1=0 退化 + sendBuf 现址）
        Check(r.slots[KFC_SLOT_INPUT] == 0x1000000, "槽 1 = sendBuf + 0（退化态）");
        Check(r.slots[KFC_SLOT_OUTPUT] == 0x2000000, "槽 2 = recvBuf + 0（退化态）");
        // 槽 14：0x1000000 != 0x2000000 + 1800000 → 0（非就地）
        Check(r.slots[KFC_SLOT_IN_OUT_EQUAL] == 0,
              "槽 14 = 0（msg 合成后非就地——覆盖了资源期的恒 1 误判）");
    }

    printf("== ② 坑1 槽 14 双态：合成后 in==out+off → 1 ==\n");
    {
        const std::vector<u64> blob = CompileBlob(8, 0, "sole[mesh]");
        const std::vector<uint64_t> frame = Frame0Of(blob);
        Check(frame[KFC_SLOT_IN_OUT_EQUAL] == 1, "资源期槽 14 == 1（病灶事实显式覆盖）");
        CcuExecMsg m;
        // 就地构造：sendBuf == recvBuf + outputOffset（rankId=1、count=512K FP16 → slice=1MB、off=1MB）
        m.count = 512 * 1024; m.dataType = 3; m.rankId = 1;
        m.recvBuf = 0x1000000;
        m.sendBuf = 0x1000000 + 1024 * 1024;   // == recvBuf + outputOffset(1MB)
        const OverwriteResult r = OverwriteCollectAllFrame(frame, m);
        Check(r.ok && r.slots[KFC_SLOT_IN_OUT_EQUAL] == 1,
              "就地态：sendBuf==recvBuf+off → 槽 14 = 1（与①非就地 0 构成双态）");
        Check(r.slots[KFC_SLOT_OUTPUT_OFFSET] == 1024 * 1024, "槽 3 = 1MB（strideCount=0 分支）");
    }

    printf("== ③ 坑2 槽 1/2 合成式：非零 blob 槽 1（防只测退化态） ==\n");
    {
        // 手工 inBaseOff=0x100000 的 blob：槽 1 非零 → 覆写后 = sendBuf + 0x100000
        const std::vector<u64> blob = CompileBlob(8, 0, "sole[mesh]", 0x100000);
        const std::vector<uint64_t> frame = Frame0Of(blob);
        Check(frame[KFC_SLOT_INPUT] == 0x100000, "blob 槽 1 = 0x100000（非零构造成功）");
        CcuExecMsg m;
        m.sendBuf = 0xA000000; m.recvBuf = 0xB000000;
        m.count = 1000; m.dataType = 3; m.rankId = 0;
        const OverwriteResult r = OverwriteCollectAllFrame(frame, m);
        Check(r.ok, "覆写 ok");
        Check(r.slots[KFC_SLOT_INPUT] == 0xA000000 + 0x100000,
              "槽 1 = sendBuf + blob槽1 = 0xA010000（合成式非退化）");
        Check(r.slots[KFC_SLOT_OUTPUT] == 0xB000000, "槽 2 = recvBuf + 0（blob 槽 2 仍 0）");
    }

    printf("== ④ 坑3 槽 3 strideCount≠0 分支（两值显式不等） ==\n");
    {
        const std::vector<u64> blob = CompileBlob(8, 0, "sole[mesh]");
        const std::vector<uint64_t> frame = Frame0Of(blob);
        CcuExecMsg m;
        m.sendBuf = 0x1000000; m.recvBuf = 0x2000000;
        m.count = 300000; m.dataType = 3; m.rankId = 3;
        const OverwriteResult r0 = OverwriteCollectAllFrame(frame, m);   // stride=0
        m.strideCount = 400000;
        const OverwriteResult r1 = OverwriteCollectAllFrame(frame, m);   // stride=400000
        Check(r0.ok && r1.ok, "两态覆写 ok");
        // 手算：stride=0 → 3*600000=1800000；stride=400000 → 3*800000=2400000
        Check(r0.slots[KFC_SLOT_OUTPUT_OFFSET] == 1800000, "stride=0：槽 3 = 3×600000 = 1800000");
        Check(r1.slots[KFC_SLOT_OUTPUT_OFFSET] == 2400000, "stride=400000：槽 3 = 3×800000 = 2400000");
        Check(r0.slots[KFC_SLOT_OUTPUT_OFFSET] != r1.slots[KFC_SLOT_OUTPUT_OFFSET],
              "两态显式不等（分支区分）");
        Check(r1.slots[KFC_SLOT_SLICE_SIZE] == 600000, "槽 4 不受 strideCount 影响（=600000）");
    }

    printf("== ⑤ 边界双取：2GiB 通过 / 2GiB+1 拒（goSize 上界） ==\n");
    {
        const std::vector<u64> blob = CompileBlob(8, 0, "sole[mesh]");
        const std::vector<uint64_t> frame = Frame0Of(blob);
        CcuExecMsg m;
        m.sendBuf = 0x1000000; m.recvBuf = 0x2000000;
        m.dataType = 3; m.rankId = 0;
        // FP16：count=1073741824 → slice=2147483648=2GiB（通过）；+1 → 拒
        m.count = 1073741824ULL;
        const OverwriteResult ok = OverwriteCollectAllFrame(frame, m);
        Check(ok.ok, "slice=2GiB（=上限）通过: " + ok.err);
        m.count = 1073741825ULL;
        const OverwriteResult bad = OverwriteCollectAllFrame(frame, m);
        Check(!bad.ok && bad.err.find("2GiB") != std::string::npos,
              "slice=2GiB+1 拒收（goSize loop limit）: " + bad.err);
        // 上限值自证防线：262144×8192 = 2147483648（字面量）
        Check(262144ULL * 8192ULL == 2147483648ULL, "上限常量 = 2147483648（字面量锁定）");
    }

    printf("== ⑥ 未知动作 ID 帧级拒收（R7 翻转：白名单 {CollectAll=0, ReduceSlice=1}） ==\n");
    {
        const std::vector<u64> blob = CompileBlob(8, 0, "sole[mesh]");
        std::vector<uint64_t> frame = Frame0Of(blob);
        frame[KFC_SLOT_ACTION_ID] = 2;   // ExchangeAll（未交付动作——server v3 白名单外）
        CcuExecMsg m;
        m.sendBuf = 0x1000000; m.recvBuf = 0x2000000;
        m.count = 1000; m.dataType = 3; m.rankId = 0;
        const OverwriteResult r = OverwriteCollectAllFrame(frame, m);
        Check(!r.ok && r.err.find("unknown action id") != std::string::npos,
              "未知 ID 帧（槽 0=2）拒收: " + r.err);
        // 病灶反例：无帧 ID 白名单的实现会静默按 AG 名单覆写未知帧——本断言锁定
        // 拒收路径的存在性（R6 的 RS 拒收断言随 WP-1 放开翻转为 ⑧ 的 golden）
    }

    printf("== ⑧ R7 · WP-1：RS 帧实例化 golden（8 卡 FP16 count=300000 rankId=3） ==\n");
    {
        // RS blob 构造：Compile 产 RS 段（two_shot AR 配方的段 0，或手工 ActionSpec
        // 序列化）——走真链路：opType=REDUCE_SCATTER → ActionForOp 映射 ReduceSlice
        OpParam op;
        op.opType = static_cast<int>(HcclCMDType::HCCL_CMD_REDUCE_SCATTER);
        op.DataDes.count = 0;                 // 资源期生产恒态（方向 B：count 占位）
        op.DataDes.dataType = HCCL_DATA_TYPE_FP16;
        op.engine = CommEngine::COMM_ENGINE_CCU;
        op.userRank = 0;
        TopoInfoWithNetLayerDetails topo;
        topo.userRankSize = 8; topo.topoLevelNums = 1; topo.level0Topo = Level0Shape::MESH_1D;
        std::vector<u32> ranks;
        for (u32 i = 0; i < 8; ++i) ranks.push_back(i);
        Plan plan; std::string cerr;
        Check(L1Adapter::Compile(op, topo, {ranks}, "sole[mesh]", plan, cerr) == HCCL_SUCCESS,
              "RS Compile 成功（count=0 结构底版）: " + cerr);
        Check(plan.segments.size() == 1 && plan.segments[0].kind == ActionKind::ReduceSlice,
              "RS plan seg0 = ReduceSlice");
        std::vector<u64> blob;
        std::string serr;
        Check(SerializePlanV3(plan, blob, serr), "RS blob 序列化成功");
        const std::vector<uint64_t> frame = Frame0Of(blob);
        Check(frame[KFC_SLOT_ACTION_ID] == 1ULL, "blob 帧 0 槽 0 = 1（ReduceSlice）");
        // 资源期病灶事实：count=0 → 序列化器 chunk 槽 5..15 全占位
        //（chunkSize 是 sliceSize 的函数、sliceSize=0 → chunkCount=0 → chunkLoopNum=
        // UINT64_MAX；tailSize=0；goSize 全 0——透传必错，WP-1 覆写必要性证据）
        Check(frame[6] == ~0ULL && frame[7] == 0ULL,
              "资源期槽 6 = UINT64_MAX（count=0 chunkCount=0 占位）/ 槽 7 = 0");

        CcuExecMsg m;
        m.sendBuf = 0x1000000; m.recvBuf = 0x2000000;
        m.count = 300000; m.dataType = 3 /*FP16*/; m.rankId = 3; m.repeatIndex = 0;
        const uint64_t scratchAddr = 0x5000000;
        const uint32_t rankNum = 8;
        const OverwriteResult r = OverwriteReduceSliceFrame(frame, m, scratchAddr, rankNum);
        Check(r.ok, "RS 帧覆写 ok: " + r.err);
        // 手算期望（全字面量）：
        //   slice = 300000×2 = 600000
        //   chunk = 64MB/8/128×128 = 8388608（8MB——每 rank 一槽的 scratch 份）
        //   fullChunkCount = (600000-1)/8388608 = 0 → chunkCount = 1 → 槽 6 = UINT64_MAX-1
        //   tail = 600000 - 0 = 600000（单 chunk 即尾 chunk）
        //   rankSliceOff = 3×600000 = 1800000（strideCount=0 分支）
        //   fullGo: loopSize=16×4096=65536；m=8388608/65536=128、n=0、p=0
        //     → {8388608, 128, 0, 0}
        //   tailGo: 600000/65536=9 → m=9、n=(600000-589824)/4096=2、p=1984
        //     → addrOffset=9×65536=589824、loopParam=9、PP(1,1,2)、residual=1984
        Check(r.slots[1] == 0x1000000, "槽 1 = sendBuf + 0（blob 槽 1 结构 offset=0）");
        Check(r.slots[2] == 0x2000000, "槽 2 = recvBuf + 0");
        Check(r.slots[3] == 0x5000000, "槽 3 = scratchAddr 直替");
        Check(r.slots[4] == 1800000, "槽 4 rankSliceOffset = 3×600000 = 1800000");
        Check(r.slots[5] == 8388608, "槽 5 chunkSize = 8388608（64MB/8 对齐 128）");
        Check(r.slots[6] == ~0ULL - 1ULL, "槽 6 chunkLoopNum = UINT64_MAX-1（单 chunk）");
        Check(r.slots[7] == 600000, "槽 7 tailSize = 600000（chunk 吃满单份）");
        Check(r.slots[8] == 8388608 && r.slots[9] == 128 && r.slots[10] == 0 && r.slots[11] == 0,
              "槽 8..11 fullGo = {8388608, 128, 0, 0}（RS config 16×4096）");
        Check(r.slots[12] == 589824 && r.slots[13] == 9 &&
              r.slots[14] == (1ULL << 55) + (1ULL << 48) + (2ULL << 41) && r.slots[15] == 1984,
              "槽 12..15 tailGo = {589824, 9, PP(1,1,2), 1984}");
        // RS config 分立守卫：若误用 AG config {8, 32768}，fullGo.addrOffset 会是
        // 262144×32=8388608 同值但 loopParam=32≠128——loopParam 断言即防"两 config
        // 合流"的漂移病灶（addrOffset 同值因 8×32768==16×4096，loopParam 是分立判据）
        Check(r.slots[9] == 128 && r.slots[9] != 32ULL,
              "fullGo.loopParam = 128 非 32（RS config {16,4096} ≠ AG {8,32768} 分立）");
        // 防退化反例：透传实现（不覆写）会落资源期占位值（槽 6=UINT64_MAX、
        // 槽 4=0）——上方两断言在透传形态必 FAIL
    }

    printf("== ⑨ R7 · WP-1：RS strideCount≠0 分支 + count=0 空数据形态 ==\n");
    {
        const std::vector<u64> blob = CompileBlob(8, 0, "sole[mesh]");
        std::vector<uint64_t> frame = Frame0Of(blob);
        frame[KFC_SLOT_ACTION_ID] = 1;   // 手工转 RS 帧（复用 AG blob 的结构底版）
        CcuExecMsg m;
        m.sendBuf = 0x1000000; m.recvBuf = 0x2000000;
        m.count = 300000; m.dataType = 3; m.rankId = 3;
        const OverwriteResult r0 = OverwriteReduceSliceFrame(frame, m, 0x5000000, 8);
        m.strideCount = 400000;
        const OverwriteResult r1 = OverwriteReduceSliceFrame(frame, m, 0x5000000, 8);
        Check(r0.ok && r1.ok, "两态覆写 ok");
        // 手算：stride=0 → 3×600000=1800000；stride=400000 → 3×800000=2400000
        Check(r0.slots[4] == 1800000, "stride=0：槽 4 = 1800000");
        Check(r1.slots[4] == 2400000, "stride=400000：槽 4 = 3×800000 = 2400000");
        Check(r0.slots[4] != r1.slots[4], "两态显式不等（分支区分）");
        // count=0（exec 期空数据——防御形态）：chunkCount=0、tail=0、tailGo 全 0
        //（chunkSize 仍按 rankNum 算出——8388608；fullGo 是 chunk 的函数非 slice 的）
        CcuExecMsg m0;
        m0.count = 0; m0.dataType = 3; m0.rankId = 0;
        const OverwriteResult r2 = OverwriteReduceSliceFrame(frame, m0, 0x5000000, 8);
        Check(r2.ok && r2.slots[5] == 8388608 && r2.slots[6] == ~0ULL && r2.slots[7] == 0 &&
              r2.slots[8] == 8388608 && r2.slots[12] == 0 && r2.slots[15] == 0,
              "count=0：chunkLoopNum=UINT64_MAX、tail=0、tailGo 全 0（kernel 零循环直过）");
    }

    printf("== ⑦ repeat 修正：repeatIndex>0 的槽 1/2 偏移 ==\n");
    {
        const std::vector<u64> blob = CompileBlob(8, 0, "sole[mesh]");
        const std::vector<uint64_t> frame = Frame0Of(blob);
        CcuExecMsg m;
        m.sendBuf = 0x1000000; m.recvBuf = 0x2000000;
        m.count = 1000; m.dataType = 3; m.rankId = 0; m.repeatIndex = 2;
        const OverwriteResult r = OverwriteCollectAllFrame(frame, m);
        Check(r.ok, "覆写 ok");
        // 手算：repeatOffset = 1000*2*2 = 4000
        Check(r.slots[KFC_SLOT_INPUT] == 0x1000000 + 4000, "槽 1 = sendBuf + 4000（repeat 偏移）");
        Check(r.slots[KFC_SLOT_OUTPUT] == 0x2000000 + 4000, "槽 2 = recvBuf + 4000");
    }

    printf("\n%d passed, %d failed\n", g_pass, g_fail);
    return g_fail == 0 ? 0 : 1;
}
