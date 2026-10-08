// unified PoC · 第四轮 A4（§12-11 甲路线）：Plan 序列化 v3 golden-bytes 自检
// 被测对象：inc/plan_serializer.h 的 SerializePlanV3 / SerializeSegmentV3 /
//           SerializedPlanV3Size（真序列化，第四轮兑现点——此前三轮为 PoC 摘要格式）
// 期望值全部手写字面量（禁止引用被测函数产出做自洽比较）；goSize 按
// {loopCount=8, memSlice=32768}（存量 AG M2M 写入方 config）手算基线。

#include "action_spec.h"       // Plan / ActionSpec / ActionKind（含尾部 plan_serializer.h）
#include "backend_registry.h"  // LoopSplitter（对照组：两 config goSize 数值不等）
#include "action_reduce_slice.h"   // ReduceSlice::BuildArgs（⑤ 段参数与 BuildArgs 对照）

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

// ============================================================================
// ① 三处同名登记（§6.12 性质）：ActionKind 枚举 ↔ 槽 0 动作 ID 常量 ↔ server 分支 ID
//    ——unified 树不 include devkit CCU 侧头（kfc_server_message.h），devkit 侧值
//    以镜像常量登记在此，用 static_assert 编译期锁死。若 devkit 侧改了
//    KFC_UNIFIED_ACTION_*，本镜像未跟 → 本测试编译失败（守门目的达成）。
// ============================================================================
// ①' unified 直连：ActionKind 枚举值（CollectAll 必须 = 0）
static_assert(static_cast<u64>(ActionKind::CollectAll) == 0ULL, "ActionKind::CollectAll must be 0");
static_assert(static_cast<u64>(ActionKind::ReduceSlice) == 1ULL, "ActionKind::ReduceSlice must be 1");
static_assert(static_cast<u64>(ActionKind::ExchangeAll) == 2ULL, "ActionKind::ExchangeAll must be 2");
static_assert(static_cast<u64>(ActionKind::LocalCopy) == 3ULL, "ActionKind::LocalCopy must be 3");
static_assert(static_cast<u64>(ActionKind::LocalReduce) == 4ULL, "ActionKind::LocalReduce must be 4");

// ①'' devkit 镜像：kfc_server_message.h 的 KFC_UNIFIED_ACTION_*（slot0 动作 ID 常量）
//    与 KfcUnifiedPlanParamIndex 槽位值——值抄自该文件（改前请先读 devkit 侧核实）
constexpr u64 DEVKIT_KFC_UNIFIED_ACTION_COLLECT_ALL = 0U;   // == ActionKind::CollectAll
constexpr u64 DEVKIT_KFC_UNIFIED_ACTION_REDUCE_SLICE = 1U;  // == ActionKind::ReduceSlice
constexpr u64 DEVKIT_KFC_UNIFIED_ACTION_EXCHANGE_ALL = 2U;  // == ActionKind::ExchangeAll
constexpr u64 DEVKIT_KFC_UNIFIED_ACTION_LOCAL_COPY = 3U;    // == ActionKind::LocalCopy
constexpr u64 DEVKIT_KFC_UNIFIED_ACTION_LOCAL_REDUCE = 4U;  // == ActionKind::LocalReduce
static_assert(DEVKIT_KFC_UNIFIED_ACTION_COLLECT_ALL == static_cast<u64>(ActionKind::CollectAll),
    "three-way registration: devkit slot0 id == ActionKind::CollectAll");
static_assert(DEVKIT_KFC_UNIFIED_ACTION_REDUCE_SLICE == static_cast<u64>(ActionKind::ReduceSlice),
    "three-way registration: devkit slot0 id == ActionKind::ReduceSlice");
static_assert(DEVKIT_KFC_UNIFIED_ACTION_EXCHANGE_ALL == static_cast<u64>(ActionKind::ExchangeAll),
    "three-way registration: devkit slot0 id == ActionKind::ExchangeAll");
static_assert(DEVKIT_KFC_UNIFIED_ACTION_LOCAL_COPY == static_cast<u64>(ActionKind::LocalCopy),
    "three-way registration: devkit slot0 id == ActionKind::LocalCopy");
static_assert(DEVKIT_KFC_UNIFIED_ACTION_LOCAL_REDUCE == static_cast<u64>(ActionKind::LocalReduce),
    "three-way registration: devkit slot0 id == ActionKind::LocalReduce");
// ①''' server 分支 ID：DispatchUnifiedPlanV3 的 CCU_IF 直接引用
//     KFC_UNIFIED_ACTION_COLLECT_ALL（构造性对齐）+ 该文件另有 static_assert
//     (== 0)（ccu_kernel_kfc_server.cc）——此处镜像锁"server 分支常量==枚举值"
static_assert(DEVKIT_KFC_UNIFIED_ACTION_COLLECT_ALL == 0ULL,
    "server branch id (KFC_UNIFIED_ACTION_COLLECT_ALL) must equal ActionKind::CollectAll (0)");
// 槽位镜像：KfcUnifiedPlanParamIndex 槽 1..14（CollectAll 参数序）
constexpr u64 DEVKIT_KFC_UNIFIED_SLOT_INPUT = 1;
constexpr u64 DEVKIT_KFC_UNIFIED_SLOT_OUTPUT = 2;
constexpr u64 DEVKIT_KFC_UNIFIED_SLOT_OUTPUT_OFFSET = 3;
constexpr u64 DEVKIT_KFC_UNIFIED_SLOT_SLICE_SIZE = 4;
constexpr u64 DEVKIT_KFC_UNIFIED_SLOT_GO_SIZE_0 = 5;
constexpr u64 DEVKIT_KFC_UNIFIED_SLOT_GO_SIZE_1 = 6;
constexpr u64 DEVKIT_KFC_UNIFIED_SLOT_GO_SIZE_2 = 7;
constexpr u64 DEVKIT_KFC_UNIFIED_SLOT_GO_SIZE_3 = 8;
constexpr u64 DEVKIT_KFC_UNIFIED_SLOT_CUR_RANK_IN_OFF = 9;
constexpr u64 DEVKIT_KFC_UNIFIED_SLOT_REPEAT_NUM_INV = 10;
constexpr u64 DEVKIT_KFC_UNIFIED_SLOT_IN_REPEAT_STRIDE = 11;
constexpr u64 DEVKIT_KFC_UNIFIED_SLOT_OUT_REPEAT_STRIDE = 12;
constexpr u64 DEVKIT_KFC_UNIFIED_SLOT_LAST_SLICE_SIZE = 13;
constexpr u64 DEVKIT_KFC_UNIFIED_SLOT_IN_OUT_EQUAL = 14;
static_assert(DEVKIT_KFC_UNIFIED_SLOT_INPUT == U_V3_INPUT, "slot mirror: INPUT");
static_assert(DEVKIT_KFC_UNIFIED_SLOT_OUTPUT == U_V3_OUTPUT, "slot mirror: OUTPUT");
static_assert(DEVKIT_KFC_UNIFIED_SLOT_OUTPUT_OFFSET == U_V3_OUTPUT_OFFSET, "slot mirror: OUTPUT_OFFSET");
static_assert(DEVKIT_KFC_UNIFIED_SLOT_SLICE_SIZE == U_V3_SLICE_SIZE, "slot mirror: SLICE_SIZE");
static_assert(DEVKIT_KFC_UNIFIED_SLOT_GO_SIZE_0 == U_V3_GO_SIZE_0, "slot mirror: GO_SIZE_0");
static_assert(DEVKIT_KFC_UNIFIED_SLOT_GO_SIZE_1 == U_V3_GO_SIZE_1, "slot mirror: GO_SIZE_1");
static_assert(DEVKIT_KFC_UNIFIED_SLOT_GO_SIZE_2 == U_V3_GO_SIZE_2, "slot mirror: GO_SIZE_2");
static_assert(DEVKIT_KFC_UNIFIED_SLOT_GO_SIZE_3 == U_V3_GO_SIZE_3, "slot mirror: GO_SIZE_3");
static_assert(DEVKIT_KFC_UNIFIED_SLOT_CUR_RANK_IN_OFF == U_V3_CUR_RANK_SLICE_INPUT_OFFSET,
    "slot mirror: CUR_RANK_SLICE_INPUT_OFFSET");
static_assert(DEVKIT_KFC_UNIFIED_SLOT_REPEAT_NUM_INV == U_V3_REPEAT_NUM_INV, "slot mirror: REPEAT_NUM_INV");
static_assert(DEVKIT_KFC_UNIFIED_SLOT_IN_REPEAT_STRIDE == U_V3_INPUT_REPEAT_STRIDE,
    "slot mirror: INPUT_REPEAT_STRIDE");
static_assert(DEVKIT_KFC_UNIFIED_SLOT_OUT_REPEAT_STRIDE == U_V3_OUTPUT_REPEAT_STRIDE,
    "slot mirror: OUTPUT_REPEAT_STRIDE");
static_assert(DEVKIT_KFC_UNIFIED_SLOT_LAST_SLICE_SIZE == U_V3_LAST_SLICE_SIZE, "slot mirror: LAST_SLICE_SIZE");
static_assert(DEVKIT_KFC_UNIFIED_SLOT_IN_OUT_EQUAL == U_V3_INPUT_OUTPUT_EQUAL,
    "slot mirror: INPUT_OUTPUT_EQUAL");
// RS 槽位镜像（R6）：KfcUnifiedPlanParamIndex 的 KFC_UNIFIED_REDUCE_SLICE_* 槽 1..15
//（值抄自 kfc_server_message.h，改前先读 devkit 侧核实）——与 UnifiedV3RsSlot 逐槽对齐
constexpr u64 DEVKIT_KFC_UNIFIED_RS_SLOT_INPUT = 1;
constexpr u64 DEVKIT_KFC_UNIFIED_RS_SLOT_OUTPUT = 2;
constexpr u64 DEVKIT_KFC_UNIFIED_RS_SLOT_SCRATCH = 3;
constexpr u64 DEVKIT_KFC_UNIFIED_RS_SLOT_RANK_SLICE_OFFSET = 4;
constexpr u64 DEVKIT_KFC_UNIFIED_RS_SLOT_CHUNK_SIZE = 5;
constexpr u64 DEVKIT_KFC_UNIFIED_RS_SLOT_CHUNK_LOOP_NUM = 6;
constexpr u64 DEVKIT_KFC_UNIFIED_RS_SLOT_TAIL_SIZE = 7;
constexpr u64 DEVKIT_KFC_UNIFIED_RS_SLOT_FULL_GO_0 = 8;
constexpr u64 DEVKIT_KFC_UNIFIED_RS_SLOT_FULL_GO_1 = 9;
constexpr u64 DEVKIT_KFC_UNIFIED_RS_SLOT_FULL_GO_2 = 10;
constexpr u64 DEVKIT_KFC_UNIFIED_RS_SLOT_FULL_GO_3 = 11;
constexpr u64 DEVKIT_KFC_UNIFIED_RS_SLOT_TAIL_GO_0 = 12;
constexpr u64 DEVKIT_KFC_UNIFIED_RS_SLOT_TAIL_GO_1 = 13;
constexpr u64 DEVKIT_KFC_UNIFIED_RS_SLOT_TAIL_GO_2 = 14;
constexpr u64 DEVKIT_KFC_UNIFIED_RS_SLOT_TAIL_GO_3 = 15;
static_assert(DEVKIT_KFC_UNIFIED_RS_SLOT_INPUT == U_V3_RS_INPUT, "RS slot mirror: INPUT");
static_assert(DEVKIT_KFC_UNIFIED_RS_SLOT_OUTPUT == U_V3_RS_OUTPUT, "RS slot mirror: OUTPUT");
static_assert(DEVKIT_KFC_UNIFIED_RS_SLOT_SCRATCH == U_V3_RS_SCRATCH, "RS slot mirror: SCRATCH");
static_assert(DEVKIT_KFC_UNIFIED_RS_SLOT_RANK_SLICE_OFFSET == U_V3_RS_RANK_SLICE_OFFSET,
    "RS slot mirror: RANK_SLICE_OFFSET");
static_assert(DEVKIT_KFC_UNIFIED_RS_SLOT_CHUNK_SIZE == U_V3_RS_CHUNK_SIZE, "RS slot mirror: CHUNK_SIZE");
static_assert(DEVKIT_KFC_UNIFIED_RS_SLOT_CHUNK_LOOP_NUM == U_V3_RS_CHUNK_LOOP_NUM,
    "RS slot mirror: CHUNK_LOOP_NUM");
static_assert(DEVKIT_KFC_UNIFIED_RS_SLOT_TAIL_SIZE == U_V3_RS_TAIL_SIZE, "RS slot mirror: TAIL_SIZE");
static_assert(DEVKIT_KFC_UNIFIED_RS_SLOT_FULL_GO_0 == U_V3_RS_FULL_GO_ADDR_OFFSET, "RS slot mirror: FULL_GO_0");
static_assert(DEVKIT_KFC_UNIFIED_RS_SLOT_FULL_GO_1 == U_V3_RS_FULL_GO_LOOP_PARAM, "RS slot mirror: FULL_GO_1");
static_assert(DEVKIT_KFC_UNIFIED_RS_SLOT_FULL_GO_2 == U_V3_RS_FULL_GO_PARALLEL_PARAM, "RS slot mirror: FULL_GO_2");
static_assert(DEVKIT_KFC_UNIFIED_RS_SLOT_FULL_GO_3 == U_V3_RS_FULL_GO_RESIDUAL, "RS slot mirror: FULL_GO_3");
static_assert(DEVKIT_KFC_UNIFIED_RS_SLOT_TAIL_GO_0 == U_V3_RS_TAIL_GO_ADDR_OFFSET, "RS slot mirror: TAIL_GO_0");
static_assert(DEVKIT_KFC_UNIFIED_RS_SLOT_TAIL_GO_1 == U_V3_RS_TAIL_GO_LOOP_PARAM, "RS slot mirror: TAIL_GO_1");
static_assert(DEVKIT_KFC_UNIFIED_RS_SLOT_TAIL_GO_2 == U_V3_RS_TAIL_GO_PARALLEL_PARAM, "RS slot mirror: TAIL_GO_2");
static_assert(DEVKIT_KFC_UNIFIED_RS_SLOT_TAIL_GO_3 == U_V3_RS_TAIL_GO_RESIDUAL, "RS slot mirror: TAIL_GO_3");
// 版本/记录宽度镜像——版本双常量分工（R6/审查方点名）：
//   KFC_SERVER_PROTOCOL_VERSION_V3 = 3（v2 协议头版本：v3 分发机制的协议族常量，
//   不随 blob 格式演进）≠ KFC_UNIFIED_BLOB_VERSION = 4（blob 格式版本：R6 头部
//   4→7 槽随之升 4）。unified 侧 UNIFIED_V3_VERSION 镜像的是**后者**（blob 格式）。
//   CCU_PARAM_NUM_PER_DIE=32、段数上限 63（64 队列 − 1 哨兵）
constexpr u64 DEVKIT_KFC_SERVER_PROTOCOL_VERSION_V3 = 3U;   // 协议族常量（不联动）
constexpr u64 DEVKIT_KFC_UNIFIED_BLOB_VERSION = 4U;         // blob 格式版本（R6 升 4）
constexpr u32 DEVKIT_KFC_UNIFIED_BLOB_HEADER_SLOTS = 7U;    // R6 头部 4→7
static_assert(UNIFIED_V3_VERSION == 4U, "version mirror: KFC_UNIFIED_BLOB_VERSION (R6 header 4->7)");
static_assert(DEVKIT_KFC_UNIFIED_BLOB_VERSION == UNIFIED_V3_VERSION,
    "blob version double-registration: devkit 4 == unified mirror");
static_assert(DEVKIT_KFC_SERVER_PROTOCOL_VERSION_V3 == 3U && UNIFIED_V3_VERSION == 4U,
    "version constants decoupled: protocol-family 3 != blob-format 4 (no accidental coupling)");
static_assert(UNIFIED_V3_HEADER_SLOTS == 7U && DEVKIT_KFC_UNIFIED_BLOB_HEADER_SLOTS == 7U,
    "header slots mirror: 7 == 7 (R6 ReduceSpec in header)");
static_assert(UNIFIED_V3_SLOTS_PER_RECORD == 32U, "record width mirror: CCU_PARAM_NUM_PER_DIE");
static_assert(UNIFIED_V3_MAX_SEGMENTS == 63U, "segment cap mirror: CCU_TASK_NUM_MAX(64) - 1 sentinel");

// ============================================================================
// 手算 goSize 基线（存量 AG M2M 写入方 config {loopCount=8, memSlice=32768}，
// loopSize = 8*32768 = 262144）：
//   size = 1MB (1048576) = 4*262144 整除 → m=4, n=0, p=0
//     addrOffset = 32768*8*4 = 1048576；loopParam = 4；parallelParam = 0；residual = 0
//   size = 1048576+65536+1000 → m=4, n=2, p=1000
//     addrOffset = 1048576；loopParam = 4；
//     parallelParam = PP(1,1,2) = (1<<55)|(1<<48)|(2<<41)；residual = 1000
// 对照组（AICPU 默认 config {128,4096}，loopSize=524288）：
//   size = 1MB → m=2, n=0, p=0 → addrOffset = 1048576, loopParam = 2 ≠ v3 的 4
//   ——证明两 config 产出不等，v3 锁写入方 config 是必要的（不是同义反复断言）
// ============================================================================
static u64 PP(u64 rn, u64 ri, u64 tot)
{
    return ((rn & 127ULL) << 55) | ((ri & 127ULL) << 48) | ((tot & 127ULL) << 41);
}

int main()
{
    printf("== ① 三处同名登记 static_assert（编译期已过，运行期登记计数） ==\n");
    {
        Check(static_cast<u64>(ActionKind::CollectAll) == DEVKIT_KFC_UNIFIED_ACTION_COLLECT_ALL,
              "ActionKind::CollectAll(0) == devkit KFC_UNIFIED_ACTION_COLLECT_ALL");
        Check(U_V3_REPEAT_NUM_INV == 10 && UNIFIED_V3_SLOTS_PER_RECORD == 32,
              "slot mirror enum intact (REPEAT_NUM_INV=10, 32 slots)");
    }

    printf("== ② golden bytes：2 段 Plan（CollectAll mesh 8 卡 + 切轮段）逐槽断言 ==\n");
    {
        // 段 0：AG mesh 8 卡，rank3 视角。FP16 512K 元素 = 1MB/片，无尾片，
        // inBaseOff=0x100000、outBaseOff=0x200000、stride=1MB、repeatNum=1。
        Plan plan;
        ActionSpec s0;
        s0.kind = ActionKind::CollectAll;
        s0.layout.sliceSize = 1048576ULL;             // 1MB
        s0.layout.tailSize = 0;
        s0.layout.inputSliceStride = 0;               // AG 现状：input 无片间步长
        s0.layout.outputSliceStride = 1048576ULL;     // 1MB/片落位步长
        s0.layout.repeatNum = 1;
        s0.layout.inputRepeatStride = 0;
        s0.layout.outputRepeatStride = 0;
        s0.layout.inBaseOff = 0x100000ULL;
        s0.layout.outBaseOff = 0x200000ULL;
        for (u32 r = 0; r < 8; ++r) s0.domain.ranks.push_back(r);
        s0.domain.myRank = 3;                          // 本 rank algRank = 3
        plan.segments.push_back(s0);

        // 段 1：切轮段（1.5MB → 整轮 1MB + 尾轮 512KB，round1）：offset 基址推进 1MB
        ActionSpec s1 = s0;
        s1.layout.sliceSize = 524288ULL;               // 尾轮 512KB
        s1.layout.inBaseOff = 0x100000ULL + 1048576ULL;
        s1.layout.outBaseOff = 0x200000ULL + 1048576ULL;
        plan.segments.push_back(s1);

        std::vector<u64> buf;
        std::string err;
        const bool ok = SerializePlanV3(plan, buf, err);
        Check(ok, "serialize ok: " + err);
        // 尺寸：头 7 + 2×32 = 71 槽（R6 头部 4→7：dataType/outputType/reduceType 进头部）
        Check(buf.size() == 71, "size = 7 + 2*32 = 71 slots");

        // 头部：magic "UPL3"、版本 4、段数 2、保留 0、ReduceSpec 三槽 0（纯 AG Plan
        // 无 reduce 段 → dataType/outputType/reduceType 全 0——全部字面量）
        Check(buf[0] == 0x55504C33ULL, "header[0] magic = 0x55504C33 (\"UPL3\")");
        Check(buf[1] == 4ULL, "header[1] version = 4 (R6 header 4->7 slots)");
        Check(buf[2] == 2ULL, "header[2] segment count = 2");
        Check(buf[3] == 0ULL, "header[3] reserved = 0");
        Check(buf[4] == 0ULL && buf[5] == 0ULL && buf[6] == 0ULL,
              "header[4..6] ReduceSpec = 0 (AG-only plan, no reduce segment)");

        // 段 0 逐槽（手算基线，全部字面量）。槽 3 是纯 rank 偏移（stride*me，不含
        // outBaseOff——kernel 侧自行加基址槽，v2 写入方同式）：
        const u64* s = buf.data() + 7;
        Check(s[0] == 0ULL, "seg0[0] action id = 0 (CollectAll)");
        Check(s[1] == 0x100000ULL, "seg0[1] input = 0x100000");
        Check(s[2] == 0x200000ULL, "seg0[2] output = 0x200000");
        Check(s[3] == 3145728ULL, "seg0[3] outputOffset = 3 * 1MB = 3145728 (纯偏移)");
        Check(s[4] == 1048576ULL, "seg0[4] sliceSize = 1MB (no tail)");
        // goSize：1MB / 262144 = 4 整除 → {4*262144, 4, 0, 0}
        Check(s[5] == 1048576ULL, "seg0[5] goSize0 addrOffset = 32768*8*4 = 1048576");
        Check(s[6] == 4ULL, "seg0[6] goSize1 loopParam = 4 (m = 1MB/262144)");
        Check(s[7] == 0ULL, "seg0[7] goSize2 parallelParam = 0 (整除)");
        Check(s[8] == 0ULL, "seg0[8] goSize3 residual = 0");
        Check(s[9] == 0ULL, "seg0[9] curRankSliceInputOffset = 0 (inputStride=0)");
        Check(s[10] == 0xFFFFFFFFFFFFFFFEULL, "seg0[10] repeatNumInv = UINT64_MAX - 1");
        Check(s[11] == 0ULL, "seg0[11] inputRepeatStride = 0");
        Check(s[12] == 0ULL, "seg0[12] outputRepeatStride = 0");
        Check(s[13] == 1048576ULL, "seg0[13] lastSliceSize = 1MB (无尾片==正常片)");
        Check(s[14] == 0ULL, "seg0[14] inOutEqual = 0 (input != output+offset)");
        // 未用槽 15..31 恒 0
        bool tailZero = true;
        for (u32 i = 15; i < 32; ++i) { if (s[i] != 0) tailZero = false; }
        Check(tailZero, "seg0[15..31] unused slots all zero");

        // 段 1 逐槽（切轮段：基址推进 1MB、片 512KB）
        const u64* t = buf.data() + 7 + 32;
        Check(t[0] == 0ULL, "seg1[0] action id = 0");
        Check(t[1] == 0x200000ULL, "seg1[1] input = 0x100000 + 1MB = 0x200000");
        Check(t[2] == 0x300000ULL, "seg1[2] output = 0x200000 + 1MB = 0x300000");
        Check(t[3] == 3145728ULL, "seg1[3] outputOffset = 3 * 1MB (纯偏移，与基址解耦)");
        Check(t[4] == 524288ULL, "seg1[4] sliceSize = 512KB (尾轮)");
        // goSize：512KB / 262144 = 2 整除 → {524288, 2, 0, 0}
        Check(t[5] == 524288ULL, "seg1[5] goSize0 addrOffset = 2*262144 = 524288");
        Check(t[6] == 2ULL, "seg1[6] goSize1 loopParam = 2");
        Check(t[7] == 0ULL && t[8] == 0ULL, "seg1[7..8] parallelParam/residual = 0");
        Check(t[10] == 0xFFFFFFFFFFFFFFFEULL, "seg1[10] repeatNumInv = UINT64_MAX - 1");
        Check(t[13] == 524288ULL, "seg1[13] lastSliceSize = 512KB (无尾片字段==本轮全片)");

        // 就地标记：curInOff=0、outOff=3*1MB → in == out + 3*1MB 时 inOutEqual=1
        //（段 0 out=0x200000 → in 取 0x500000；段 1 独立布局不受影响）
        Plan inPlace = plan;
        inPlace.segments[0].layout.inBaseOff = 0x500000ULL;    // in == out + 3*1MB
        std::vector<u64> b2;
        Check(SerializePlanV3(inPlace, b2, err), "inplace serialize ok");
        Check(b2.size() == 71 && b2[7 + 1] == 0x500000ULL && b2[7 + 3] == 3145728ULL &&
                  b2[7 + 2] == 0x200000ULL && b2[7 + 14] == 1ULL,
              "inplace: in==out+off -> seg0[14] inOutEqual = 1");

        // 尾片路径：rank7 有尾片 512KB → 段 0 的 sliceSize 取尾片、lastSlice=尾片
        Plan tailed = plan;
        tailed.segments[0].domain.myRank = 7;
        tailed.segments[0].layout.tailSize = 524288ULL;
        std::vector<u64> b3;
        Check(SerializePlanV3(tailed, b3, err), "tailed serialize ok");
        Check(b3.size() == 71 && b3[7 + 4] == 524288ULL, "tail rank: sliceSize = tailSize (512KB)");
        Check(b3[7 + 13] == 524288ULL, "tail rank: lastSliceSize = tailSize");
        // 非末 rank 的 mySlice 仍取全片（rank3 + tail → sliceSize 不变）
        Plan notTail = plan;
        notTail.segments[0].layout.tailSize = 524288ULL;   // rank3 非末 rank
        std::vector<u64> b4;
        Check(SerializePlanV3(notTail, b4, err), "non-tail serialize ok");
        Check(b4[7 + 4] == 1048576ULL, "non-tail rank: sliceSize stays full (1MB)");
    }

    printf("== ③ goSize config 分歧对照（v3 锁存量写入方 config 的必要性） ==\n");
    {
        // 同尺寸 1MB：写入方 config {8,32768} → m=4；AICPU 默认 {128,4096} → m=2。
        // 若 v3 误用默认 config，段 0 槽 6 将是 2 而非 4——②已断言 =4。
        // 此处直接给出两组 LoopSplitter 手算对照，锁公式 + config 双重正确。
        LoopSplitter::LoopGroupConfig writer;
        writer.msInterleave = 8;
        writer.loopCount = 8;
        writer.memSlice = 32768;
        LoopSplitter::LoopGroupConfig aicpu;   // 默认 {8, 128, 4096}
        const LoopSplitter::GoSize gw = LoopSplitter::Calc(1048576ULL, writer);
        const LoopSplitter::GoSize ga = LoopSplitter::Calc(1048576ULL, aicpu);
        Check(gw.loopParam == 4ULL, "writer config: 1MB → m=4 (v3 的值)");
        Check(ga.loopParam == 2ULL, "aicpu default config: 1MB → m=2 (≠v3，分歧实证)");
        Check(gw.loopParam != ga.loopParam, "two configs produce different goSize (drift guard)");
        // 有零头+尾块：1MB + 64KB + 1000（writer config，loopSize=262144）
        const LoopSplitter::GoSize gz = LoopSplitter::Calc(1048576ULL + 65536ULL + 1000ULL, writer);
        Check(gz.addrOffset == 1048576ULL && gz.loopParam == 4ULL, "writer: m=4, addrOffset=1MB");
        Check(gz.parallelParam == PP(1, 1, 2) && gz.residual == 1000ULL,
              "writer: n=2,p=1000 → PP(1,1,2)+residual 1000");
    }

    printf("== ④ 边界：未知动作 ID 拒绝 + 段数上限 ==\n");
    {
        // 未交付动作（ExchangeAll 尚无 v3 布局）→ SerializeSegmentV3 拒绝并带动作值。
        // R6 起 ReduceSlice 已交付（见 ⑤ golden-bytes），负例 ID 换用 ExchangeAll=2
        //（"合法但未交付"形态——若未来 2 转正，改用越界构造）
        ActionSpec bad;
        bad.kind = ActionKind::ExchangeAll;   // 枚举合法、v3 未含 → 拒绝
        u64 slots[32] = {0};
        std::string err;
        Check(!SerializeSegmentV3(bad, slots, err), "ExchangeAll rejected by v3 (not in scope)");
        Check(err.find("#2") != std::string::npos, "err carries action kind value (#2)");
        // 空指针拒绝
        Check(!SerializeSegmentV3(bad, nullptr, err), "null slots rejected");
        // 整 Plan 含未知动作段：拒绝且 destination 不含可消费的完整段记录
        //（段 0 已写好但头部段数/内容不完整一致——实现按序写段，失败即停；
        //  消费方以返回 false 为拒收依据，不得使用半写 buffer）
        Plan p;
        ActionSpec good;
        good.kind = ActionKind::CollectAll;
        good.layout.sliceSize = 4096;
        good.layout.outputSliceStride = 4096;
        for (u32 r = 0; r < 2; ++r) good.domain.ranks.push_back(r);
        p.segments.push_back(good);
        p.segments.push_back(bad);
        std::vector<u64> b;
        Check(!SerializePlanV3(p, b, err), "plan with unknown action segment rejected");
        // 半写防御：失败段（段 1）的槽 0 已被 SerializeSegmentV3 置 0 清洗后按
        // kind=2 写入——调用方以返回 false 拒收，buffer 不得被消费。锁两个事实：
        //   a) 段 1 槽 0 = 2（ExchangeAll 的动作 ID）——证明失败发生在段 1 写槽之后
        //      （未静默吞错）
        //   b) 头部段数 2 与实际可消费段不符 → 消费方必须看返回值
        Check(!b.empty() && b[2] == 2ULL && b[7 + 32 + 0] == 2ULL,
              "failed segment slot0 = 2 (ExchangeAll id); caller must reject by return value");

        // 段数上限：63 段 = 合法上限（64 帧队列 − 1 哨兵），64 段拒绝
        Plan cap;
        cap.segments.assign(63, good);
        std::vector<u64> b63;
        Check(SerializePlanV3(cap, b63, err), "63 segments = legal cap (64 queue - 1 sentinel)");
        Check(b63.size() == 7 + 63 * 32, "63-seg blob = 7 + 63*32 = 2023 slots");
        Check(SerializedPlanV3Size(cap) == 2023U, "SerializedPlanV3Size(63) = 2023 (literal)");
        Plan over;
        over.segments.assign(64, good);
        std::vector<u64> b64;
        Check(!SerializePlanV3(over, b64, err), "64 segments rejected (exceeds queue capacity)");
        Check(err.find("63") != std::string::npos, "cap error names 63");
        // 空 Plan：头 7 槽、段数 0（合法——"空计划"由调用方/哨兵语义解释）
        Plan empty;
        std::vector<u64> be;
        Check(SerializePlanV3(empty, be, err) && be.size() == 7 && be[2] == 0,
              "empty plan: header-only blob (7 slots), segs=0");
    }

    printf("== ⑤ R6 ReduceSlice 段 golden bytes（RS mesh 8 卡，手算基线） ==\n");
    {
        // 段构造：RS mesh 8 卡、rank3 视角。sliceSize=20MB、inputSliceStride=20MB
        //（RS 的 input 步长 = 各 rank 片在 input 里的间隔）、inBaseOff=0x100000、
        // outBaseOff=0x200000、scratchBaseOff=0x500000、repeatNum=1。
        // 手算（python 复算，chunk 公式：64MB/8/128*128 = 8,388,608）：
        //   chunkSize = 8,388,608；fullChunkCount = (20,000,000-1)/8,388,608 = 2；
        //   chunkCount = 3 → chunkLoopNum = UINT64_MAX-3 = 0xFFFFFFFFFFFFFFFC；
        //   tailSize = 20,000,000 - 2*8,388,608 = 3,222,784；
        //   fullGo = Calc(8388608, {16,4096})：8388608/65536 = 128 整除 →
        //            {8388608, 128, 0, 0}；
        //   tailGo = Calc(3222784, {16,4096})：m=49（3211264）、余 11520 → n=2、p=3328
        //            → {3211264, 49, PP(1,1,2), 3328}，PP(1,1,2)=36314670042185728；
        //   rankSliceOffset = 20,000,000 * 3 = 60,000,000。
        // goSize RS config 独立性对照：同尺寸 8,388,608 若误用 CollectAll 写入方
        // config {8,32768}（loopSize=262144）→ m=32 ≠ RS 的 128（loopSize=65536）
        // ——两 config 分歧的 RS 实证（防"顺手照抄 CollectAll config"）。
        Plan plan;
        ActionSpec rs;
        rs.kind = ActionKind::ReduceSlice;
        rs.pattern = CommPattern::MESH;
        rs.layout.sliceSize = 20000000ULL;
        rs.layout.tailSize = 0;
        rs.layout.inputSliceStride = 20000000ULL;   // RS input 步长 = 片间隔
        rs.layout.repeatNum = 1;
        rs.layout.inBaseOff = 0x100000ULL;
        rs.layout.outBaseOff = 0x200000ULL;
        rs.layout.scratchBaseOff = 0x500000ULL;
        rs.reduce.dataType = 3;      // FP16
        rs.reduce.outputType = 3;    // FP16
        rs.reduce.reduceOp = 0;      // SUM
        for (u32 r = 0; r < 8; ++r) rs.domain.ranks.push_back(r);
        rs.domain.myRank = 3;
        plan.segments.push_back(rs);

        std::vector<u64> buf;
        std::string err;
        Check(SerializePlanV3(plan, buf, err), "RS serialize ok: " + err);
        Check(buf.size() == 7 + 32, "RS blob = 7 + 1*32 = 39 slots");
        // 头部：ReduceSpec 三槽进头部（FP16=3/FP16=3/SUM=0——纯维测 dump，R6 尾款
        // 终裁：消费方以 exec 期 msg 现值为准，此值仅日志/trace 用）
        Check(buf[1] == 4ULL, "RS header[1] version = 4");
        Check(buf[4] == 3ULL && buf[5] == 3ULL && buf[6] == 0ULL,
              "RS header[4..6] ReduceSpec = {FP16, FP16, SUM}");
        // 段 0 逐槽（手算基线，全部字面量）
        const u64* s = buf.data() + 7;
        Check(s[0] == 1ULL, "rs[0] action id = 1 (ReduceSlice)");
        Check(s[1] == 0x100000ULL, "rs[1] input = 0x100000");
        Check(s[2] == 0x200000ULL, "rs[2] output = 0x200000");
        Check(s[3] == 0x500000ULL, "rs[3] scratch = 0x500000 (scratchBaseOff)");
        Check(s[4] == 60000000ULL, "rs[4] rankSliceOffset = 3 * 20MB (纯偏移)");
        Check(s[5] == 8388608ULL, "rs[5] chunkSize = 64MB/8/128*128 = 8388608");
        Check(s[6] == 0xFFFFFFFFFFFFFFFCULL, "rs[6] chunkLoopNum = UINT64_MAX - 3");
        Check(s[7] == 3222784ULL, "rs[7] tailSize = 20MB - 2*8388608");
        Check(s[8] == 8388608ULL, "rs[8] fullGo addrOffset = 128*65536");
        Check(s[9] == 128ULL, "rs[9] fullGo loopParam = 128 (m = 8388608/65536)");
        Check(s[10] == 0ULL, "rs[10] fullGo parallelParam = 0 (整除)");
        Check(s[11] == 0ULL, "rs[11] fullGo residual = 0");
        Check(s[12] == 3211264ULL, "rs[12] tailGo addrOffset = 49*65536");
        Check(s[13] == 49ULL, "rs[13] tailGo loopParam = 49");
        Check(s[14] == 36314670042185728ULL, "rs[14] tailGo parallelParam = PP(1,1,2)");
        Check(s[15] == 3328ULL, "rs[15] tailGo residual = 3328");
        // 槽 16..31 恒 0（RS 15 参数 + 槽 0 = 16 槽，恰贴 KFC_CONCURRENT_RS_MESH_
        // PARAM_NUM=16 上限——第四轮槽容量预警解除的实证）
        bool tailZero = true;
        for (u32 i = 16; i < 32; ++i) { if (s[i] != 0) tailZero = false; }
        Check(tailZero, "rs[16..31] unused slots all zero (16-slot RS params fit 32-slot record)");

        // 单 chunk 场景：slice=3,000,000 < chunkSize → chunkCount=1、loopNum=MAX-1、
        // tail=slice、fullGo 仍按 chunkSize 算（kernel 首 chunk 满块满块用 full）
        ActionSpec single = rs;
        single.layout.sliceSize = 3000000ULL;
        single.layout.inputSliceStride = 3000000ULL;
        single.domain.myRank = 0;
        u64 ss[32] = {0};
        Check(SerializeSegmentV3(single, ss, err), "single-chunk RS serialize ok");
        Check(ss[4] == 0ULL, "single-chunk rank0: rankSliceOffset = 0");
        Check(ss[5] == 8388608ULL, "single-chunk chunkSize 不变（scratch 容量函数）");
        Check(ss[6] == 0xFFFFFFFFFFFFFFFEULL, "single-chunk chunkLoopNum = UINT64_MAX - 1");
        Check(ss[7] == 3000000ULL, "single-chunk tailSize = slice（唯一 chunk 即尾 chunk）");
        Check(ss[8] == 8388608ULL && ss[9] == 128ULL, "single-chunk fullGo 按 chunkSize 算");
        // tailGo(3,000,000)：m=45（2949120）、余 50880 → n=12、p=1728 → PP(11,1,2)
        Check(ss[12] == 2949120ULL && ss[13] == 45ULL &&
                  ss[14] == 396602640231825408ULL && ss[15] == 1728ULL,
              "single-chunk tailGo(3M) = {2949120,45,PP(11,1,2),1728}");

        // NHR 拒收：R9 交付物（R9 M0-④ 勘误后拒收因 = v3 帧无 pattern 载体 +
        // NHR kernel 的 jettyNum/stepInfoVector/rank2ChannelIdx 三件 ctx.arg 自持量
        // 在 unified 注册链无人填——非"槽宽超限"，25 槽 ≤ 32 记录宽装得下）
        ActionSpec nhr = rs;
        nhr.pattern = CommPattern::NHR;
        u64 ns[32] = {0};
        Check(!SerializeSegmentV3(nhr, ns, err), "RS NHR rejected (R9 scope, no pattern carrier in v3 frame)");
        Check(err.find("NHR") != std::string::npos, "RS NHR err names pattern");

        // 头部三槽的"首个 reduce 段"语义：AG 段在前 + RS 段在后 → 头部取 RS 的值
        Plan mixed;
        ActionSpec agSeg;
        agSeg.kind = ActionKind::CollectAll;
        agSeg.layout.sliceSize = 4096;
        agSeg.layout.outputSliceStride = 4096;
        for (u32 r = 0; r < 2; ++r) agSeg.domain.ranks.push_back(r);
        mixed.segments.push_back(agSeg);
        mixed.segments.push_back(rs);
        std::vector<u64> mb;
        Check(SerializePlanV3(mixed, mb, err), "mixed AG+RS serialize ok");
        // 头部三槽=纯 dump（R6 尾款终裁）：断言只锁"dump 值 = 首个 reduce 段的投影"
        //（dump 完备性由段间恒同不变量保证），不表达任何消费语义
        Check(mb[4] == 3ULL && mb[5] == 3ULL && mb[6] == 0ULL,
              "mixed plan header dump takes first ReduceSlice segment's ReduceSpec");
        // RS BuildArgs 对照（半写防御同 ②：段参数与 BuildArgs 产物逐槽一致）
        ReduceSliceArgs rargs;
        std::string berr;
        Check(ReduceSlice::BuildArgs(rs, rargs, berr) == HCCL_SUCCESS, "BuildArgs ok: " + berr);
        Check(rargs.inputAddrOff == 0x100000ULL && rargs.outputAddrOff == 0x200000ULL &&
                  rargs.scratchAddrOff == 0x500000ULL, "BuildArgs base offsets");
        Check(rargs.rankSliceOffset == 60000000ULL, "BuildArgs rankSliceOffset");
        Check(rargs.chunks.chunkSize == 8388608ULL && rargs.chunks.chunkLoopNum == 0xFFFFFFFFFFFFFFFCULL &&
                  rargs.chunks.tailSize == 3222784ULL, "BuildArgs chunk triplet");
        Check(rargs.peers.size() == 7, "BuildArgs 7 peers");
        // NHR 拒收对齐（BuildArgs 与序列化同一防线）
        ReduceSliceArgs nArgs;
        Check(ReduceSlice::BuildArgs(nhr, nArgs, berr) == HCCL_E_NOT_SUPPORT,
              "BuildArgs rejects NHR (same fence as serializer)");
    }

    printf("\n%d passed, %d failed\n", g_pass, g_fail);
    return g_fail == 0 ? 0 : 1;
}
