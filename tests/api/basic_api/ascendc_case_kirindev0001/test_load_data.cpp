#include <gtest/gtest.h>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <string>
#include "kernel_tensor.h"
#include "kernel_tpipe.h"
#include "kernel_operator_data_copy_intf.h"
namespace bisheng {
namespace cce {
void enable_s4_expand_s8_datacopy();
void enable_s3_expand_s8_datacopy();
}
}
namespace AscendC {
struct LoadData2DDecompParamsV2 {
    uint32_t mStartPosition = 0;
    uint32_t kStartPosition = 0;
    uint16_t mStep = 0;
    uint16_t kStep = 0;
    int32_t srcStride = 0;
    uint16_t dstStride = 0;
    uint8_t sid = 0;
    bool decomp = false;
};
} // namespace AscendC
#include "kernel_operator_mm_intf.h"

using namespace std;
using namespace AscendC;

namespace {
[[maybe_unused]] constexpr size_t kGmBytes = 1024; // host GM staging array for the R11/R12 legs
constexpr size_t kObsBytes = 128;  // src-canary / dst-observation window at pool offset 0

void FillRamp(uint8_t* buf, size_t bytes)
{
    for (size_t i = 0; i < bytes; ++i) {
        buf[i] = static_cast<uint8_t>(i & 0xFF);
    }
}

void FillSentinel(uint8_t* buf, size_t bytes)
{
    memset(buf, 0xA5, bytes);
}

// Byte-morphology record for a model-internal dst window: how many bytes are
// zeroed / still the 0xA5 sentinel / neither, plus the first "other" byte.
// RECORDED, never asserted -- the exact span is a harness (stub-behavior)
// artifact, not a product contract.
struct DstMorph {
    size_t zeros = 0;
    size_t sentinels = 0;
    size_t others = 0;
    size_t firstOtherOff = static_cast<size_t>(-1);
    uint8_t firstOtherVal = 0;
};

DstMorph ObserveDst(const uint8_t* buf, size_t bytes)
{
    DstMorph m;
    for (size_t i = 0; i < bytes; ++i) {
        if (buf[i] == 0x00) {
            ++m.zeros;
        } else if (buf[i] == 0xA5) {
            ++m.sentinels;
        } else {
            ++m.others;
            if (m.firstOtherOff == static_cast<size_t>(-1)) {
                m.firstOtherOff = i;
                m.firstOtherVal = buf[i];
            }
        }
    }
    return m;
}

void ReportDstMorph(const char* leg, const DstMorph& m, size_t bytes)
{
    fprintf(stderr, "[blocked %s] dst morphology over %zuB: zeros=%zu sentinels=%zu others=%zu", leg, bytes,
        m.zeros, m.sentinels, m.others);
    if (m.others > 0) {
        fprintf(stderr, " (first-other @+0x%zx val=0x%02x)", m.firstOtherOff, m.firstOtherVal);
    }
    fprintf(stderr, "\n");
}

// Hard canary veto (rogue-write detector, same philosophy as the promoted
// GM->L1 suites): a leg that is contractually read-only over its source must
// leave the byte ramp untouched.
void ExpectRampIntact(const char* tag, const uint8_t* buf, size_t bytes)
{
    for (size_t i = 0; i < bytes; ++i) {
        EXPECT_EQ(buf[i], static_cast<uint8_t>(i & 0xFF))
            << tag << ": source buffer modified at offset " << i << " -- rogue write";
    }
}

// Env gate (see the file header): an enrolled copy of this file that gets
// swept into an official make-driven full run must not fire the run-time-dead
// legs (R8 teardown-134 / R10 family-gate SIGABRT). Inert legs print one
// stderr line and SUCCEED, exactly like the probe legs did (D7).
bool LegEnabled(const char* leg)
{
    const char* legEnv = getenv("KIRINDEV0001_BLOCKED_LEGS");
    if (legEnv == nullptr || *legEnv == '\0') {
        fprintf(stderr, "[blocked %s] KIRINDEV0001_BLOCKED_LEGS not set -- leg inert (official full run)\n", leg);
        return false;
    }
    return true;
}
} // namespace

// All four locked paths carry no core-type gate (see the file header) and the
// probe verdicts were collected under AIC, mirroring the promoted C2-family
// fixture. Do NOT "correct" this fixture to AIV(2) when re-probing: the
// archived evidence was collected as-is.
class TestBlockedSiblings : public testing::Test {
protected:
    void SetUp() override { SetGCoreType(1); }
    void TearDown() override { SetGCoreType(0); }
};

// ------------------------------------------------------------------ R8 leg
// DataCopy L1(A1) -> PT(C2PT), the L1-local three-arg overload -- dispatches
// through the PT arm (kernel_operator_data_copy_intf_impl.h:421-424) into
// DataCopyL12PTImpl (dav_5161/kernel_operator_data_copy_impl.h:313-319) ->
// copy_cbuf_to_pt. Round-35 D3 form: leg OK, src canary intact, dst fully
// zeroed, then the process dies at teardown (exit 134, free(): invalid
// pointer). Unblock watch: the CPU-debug stub for copy_cbuf_to_pt.
TEST_F(TestBlockedSiblings, R8L12Pt_CopyCbufToPt)
{
    if (!LegEnabled("r8")) { SUCCEED() << "inert"; return; }
    TPipe tpipe;
    LocalTensor<half> src;
    TBuffAddr srcAddr;
    srcAddr.logicPos = (uint8_t)TPosition::A1;
    src.SetAddr(srcAddr);
    src.InitBuffer(0, kObsBytes / sizeof(half));

    LocalTensor<half> dst;
    TBuffAddr dstAddr;
    dstAddr.logicPos = (uint8_t)TPosition::C2PT; // PT hardware position, model-internal
    dst.SetAddr(dstAddr);
    dst.InitBuffer(0, kObsBytes / sizeof(half));

    uint8_t* srcP = reinterpret_cast<uint8_t*>(src.GetPhyAddr());
    uint8_t* dstP = reinterpret_cast<uint8_t*>(dst.GetPhyAddr());
    FillRamp(srcP, kObsBytes);
    FillSentinel(dstP, kObsBytes);

    DataCopyParams params{1, 32 * sizeof(half), 0, 0};
    DataCopy(dst, src, params);
    PipeBarrier<PIPE_ALL>();

    ExpectRampIntact("r8/pt src", srcP, kObsBytes);
    ReportDstMorph("r8/pt", ObserveDst(dstP, kObsBytes), kObsBytes);
    SUCCEED() << "R8 leg dispatched and returned (the archived death form is process "
                 "exit 134 at teardown -- watch the exit code, not this line)";
}

// ----------------------------------------------------------------- R10 leg
// LoadData 2dv2 Local(A1) -> L0B(B2) -- kernel_operator_mm_intf.h:92-94
// (Local/Local form), entry mm_intf_impl.h:102-111, dispatch into
// LoadData2DL12L0BCal (dav_5161/kernel_operator_mm_impl.h:72-91) ->
// load_cbuf_to_cb (the .so signature matches; this path LINKS). Round-35 D4
// form: SIGABRT mid-leg at kernel_operator_mm_load2d_impl.h:193 -- the
// CheckFuncLoadData2dv2 #else arm hard-rejects (real checks compiled for
// 3102/3510/5102 only). The lines below LoadData are unreachable in that
// world and stay for the reopened-evaluation branch: if this leg ever logs a
// morphology record, the family gate opened and the block verdict must be
// re-run (round-28 evidence re-evaluation), not assumed.
TEST_F(TestBlockedSiblings, R10L12B2_LoadDataCbufToCb)
{
    if (!LegEnabled("r10")) { SUCCEED() << "inert"; return; }
    TPipe tpipe;
    LocalTensor<half> src;
    TBuffAddr srcAddr;
    srcAddr.logicPos = (uint8_t)TPosition::A1;
    src.SetAddr(srcAddr);
    src.InitBuffer(0, kObsBytes / sizeof(half));

    LocalTensor<half> dst;
    TBuffAddr dstAddr;
    dstAddr.logicPos = (uint8_t)TPosition::B2;
    dst.SetAddr(dstAddr);
    dst.InitBuffer(0, 512); // pool offset 0 -> the 512B dst-align gate passes

    uint8_t* srcP = reinterpret_cast<uint8_t*>(src.GetPhyAddr());
    uint8_t* dstP = reinterpret_cast<uint8_t*>(dst.GetPhyAddr());
    FillRamp(srcP, kObsBytes);
    FillSentinel(dstP, kObsBytes); // if the leg ever survives the family gate, see what wrote here

    // LoadData2DParamsV2 field order per kernel_struct_mm.h:85-92:
    // {mStartPosition=0, kStartPosition=0, mStep=1, kStep=1,
    //  srcStride=0, dstStride=0, ifTranspose=false, sid=0}
    LoadData2DParamsV2 params{0, 0, 1, 1, 0, 0, false, 0};
    LoadData(dst, src, params); // archived form: CheckFuncLoadData2dv2 #else hard-reject -> SIGABRT
    PipeBarrier<PIPE_ALL>();

    ExpectRampIntact("r10/l12b2 src", srcP, kObsBytes);
    ReportDstMorph("r10/l12b2", ObserveDst(dstP, kObsBytes), kObsBytes);
    SUCCEED() << "R10 leg RETURNED -- family gate OPENED: re-run the round-28 r10 evidence "
                 "(checker rejection) and re-evaluate the block verdict";
}

// ------------------------------------------------- R11 leg (LINK-time dead)
// OFF behind -DKIRINDEV0001_ENABLE_R11_LEG: any binary compiling this leg in
// DIES AT FINAL LINK (probe D1-c/D5):
//   (a) the Kirindev0001 libcpudebug exports load_gm_to_cb_2dv2(half*, half*,
//       uint, uint, ushort, ushort, ushort, uchar, uchar, uchar) -- a 10-arg
//       form with three trailing uchar params -- while the dav_5161 Cal site
//       (kernel_operator_mm_impl.h:100-120) calls an 8-arg form: signature
//       drift between backend and model .so, undefined at final link;
//   (b) the 5161 #else dispatch arm (kernel_operator_mm_load2d_impl.h:
//       257-265) puts the L0A/L0B/L1 Cals in ONE function body, so this leg
//       also instantiates LoadData2DGM2L1Cal -> load_gm_to_cbuf_2dv2 (zero
//       exports) -- the R12 link veto rides along as a passenger (probe
//       D1-d). The MTE2 entry is kernel_operator_mm_intf.h:96-98
//       (Local/Global form), mm_intf_impl.h:124-133.
// The archived link rows (probe criteria D1 复跑) are the verdict; keep this
// gate OFF unless reproducing them after an upstream signature alignment.
#ifdef KIRINDEV0001_ENABLE_R11_LEG
TEST_F(TestBlockedSiblings, R11Gm2B2_LoadDataGmToCb2dv2)
{
    if (!LegEnabled("r11")) { SUCCEED() << "inert"; return; }
    uint8_t srcGm[kGmBytes];
    FillRamp(srcGm, sizeof(srcGm));

    TPipe tpipe;
    LocalTensor<half> dst;
    TBuffAddr dstAddr;
    dstAddr.logicPos = (uint8_t)TPosition::B2;
    dst.SetAddr(dstAddr);
    dst.InitBuffer(0, 512);

    uint8_t* dstP = reinterpret_cast<uint8_t*>(dst.GetPhyAddr());
    FillSentinel(dstP, kObsBytes);

    GlobalTensor<half> srcGlobal;
    srcGlobal.SetGlobalBuffer(reinterpret_cast<__gm__ half*>(srcGm), kGmBytes / sizeof(half));

    LoadData2DParamsV2 params{0, 0, 1, 1, 0, 0, false, 0};
    LoadData(dst, srcGlobal, params); // 8-arg backend call vs 10-arg .so export -> link undefined
    PipeBarrier<PIPE_ALL>();

    ExpectRampIntact("r11/gm2b2 srcGm", srcGm, kGmBytes);
    ReportDstMorph("r11/gm2b2", ObserveDst(dstP, kObsBytes), kObsBytes);
    SUCCEED() << "unreachable when the R11 gate is on: the link never completes";
}
#endif

// ------------------------------------------------- R12 leg (LINK-time dead)
// OFF behind -DKIRINDEV0001_ENABLE_R12_LEG: load_gm_to_cbuf_2dv2 has ZERO
// exports in the stub .so (probe D1-d/D6), so the direct dst=C1
// instantiation (LoadData2DGM2L1Cal, dav_5161/kernel_operator_mm_impl.h:
// 123-143; GetPhyType(C1) == L1) fails the final link exactly like the R11
// passenger instantiation did. The undefined-reference row for this very
// symbol was already harvested verbatim during the probe; this body exists
// so the pure-direct form stays after the archived link evidence. Same MTE2
// entry as R11 (kernel_operator_mm_intf.h:96-98).
#ifdef KIRINDEV0001_ENABLE_R12_LEG
TEST_F(TestBlockedSiblings, R12Gm2L1_LoadDataGmToCbuf2dv2)
{
    if (!LegEnabled("r12")) { SUCCEED() << "inert"; return; }
    uint8_t srcGm[kGmBytes];
    FillRamp(srcGm, sizeof(srcGm));

    TPipe tpipe;
    LocalTensor<half> dst;
    TBuffAddr dstAddr;
    dstAddr.logicPos = (uint8_t)TPosition::C1; // GetPhyType(C1) == L1 -> LoadData2DGM2L1Cal
    dst.SetAddr(dstAddr);
    dst.InitBuffer(0, 512);

    uint8_t* dstP = reinterpret_cast<uint8_t*>(dst.GetPhyAddr());
    FillSentinel(dstP, kObsBytes);

    GlobalTensor<half> srcGlobal;
    srcGlobal.SetGlobalBuffer(reinterpret_cast<__gm__ half*>(srcGm), kGmBytes / sizeof(half));

    LoadData2DParamsV2 params{0, 0, 1, 1, 0, 0, false, 0};
    LoadData(dst, srcGlobal, params); // zero .so exports -> final-link undefined
    PipeBarrier<PIPE_ALL>();

    ExpectRampIntact("r12/gm2l1 srcGm", srcGm, kGmBytes);
    ReportDstMorph("r12/gm2l1", ObserveDst(dstP, kObsBytes), kObsBytes);
    SUCCEED() << "unreachable when the R12 gate is on: the link never completes";
}
#endif
