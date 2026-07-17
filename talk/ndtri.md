# prims_ndtri — asc-devkit adv_api — findings + plan

## SPEC SOURCE (authoritative, no longer self-sourced)
`/Users/evmenoff/Documents/code/catlass/cann-ops-competitions/04_tasks/01_community-task-2026/docs/202607/`
- `prims_ndtri_task_doc.md`            <- 任务书
- `self_test_case/prims_ndtri/`        <- **验收自验证用例 (ALREADY EXISTS)**: ndtri.asc, run.sh,
                                          CMakeLists.txt, data_utils.h, scripts/gen_data.py, README.md
Sibling repo `catlass/cann-ops-competitions` (NOT a subdir of asc-devkit). Ours: fork evmenoff/asc-devkit.

## TASK (from 任务书 — verbatim requirements)
- 技术标签 高阶API开发 | 适配硬件 **Ascend 950PR** (`__NPU_ARCH__`=3510) | 语言 Ascend C | CANN >= 9.1.0
- Align PyTorch IR `prims.ndtri` (torch.special.ndtri). **float32 only**, ND format. Elementwise.
  $$\mathrm{Ndtri}(p)=\sqrt{2}\sum_k c_k\left(\frac{p-1/2}{1/2}\right)^{2k+1}$$
  self_test README states **$c_k$ 为 Acklam 算法系数** -> Acklam explicitly endorsed.
- 边界: 输入在[0,1]外、极端概率值、NaN/Inf 均须正确处理.
- **精度 双万分之一**: 1万个数据中，误差(rel)超过1e-4的不超过1个. (harness: EPS=1e-4, ERR_RATIO=1e-4)
- **性能 aiv_vec 占比 > 90%**. 小shape不达标须给性能仿真图+分析结论佐证.
- 交付: fork仓(API代码+README+UT+功能测试) + 自验证报告 + **评审通过的API设计文档**(提issue, 2工作日反馈).
- PR -> `gitcode.com/cann/asc-devkit` **experimental/**.
- 编码规范: cann/community coding-standards. 环境: 开源仓100h免费时长 / hidevlab WebIDE.

## GROUNDED FINDINGS

### F1 — 3510 uses REGISTER-level programming, not UB-tile vector API  **biggest correction**
`erf_3510_impl.h` AND `digamma_3510_impl.h` are both `__simd_vf__` / `Reg::`, NOT LocalTensor Muls/Adds:
- `Reg::RegTensor<float>`, `Reg::MaskReg`, `Reg::UpdateMask<float>(calCount)`
- loop `repeatTimes = CeilDivision(calCount, B32_DATA_NUM_PER_REPEAT)` (64 fp32/repeat)
- `Reg::LoadAlign(reg, ub + i*B32_DATA_NUM_PER_REPEAT)` ... compute in regs ... `Reg::StoreAlign`
- **all temporaries are REGISTERS. `sharedTmpBuffer` accepted but UNUSED on 3510.**
=> "sharedTmpBuffer holds alt-branch result + masks + q/r temps" was WRONG. Tiling kept for API parity only.
=> **Digamma is the closest reference**: it IS the harness placeholder AND has a Reg:: 3510 impl.

### F2 — Reg ops needed all exist on 3510
`include/basic_api/reg_compute/kernel_reg_compute_vec_unary_intf.h` + `impl/.../reg_compute/dav_3510/`:
Sqrt, Ln, Log, Exp, Abs, Neg, Not | binary(Div,Mul,Sub,Add) | binary_scalar(Muls,Adds)
| cmpsel(Compare,Select) | duplicate | fused(FusedMulDstAdd) | vconv(Cast).
erf already uses Div/Log/Exp/Select/Compare/Abs/Neg/Duplicate/And/Or/Mins/Maxs/FusedMulDstAdd.
=> Sqrt + Ln confirmed. **No missing primitive for Acklam.**

### F3 — SIGNATURE CONFIRMED by the acceptance harness
`self_test_case/prims_ndtri/ndtri.asc` says replace `AscendC::Digamma<T,false>(yLocal,xLocal,blockLength)`
with **`AscendC::Ndtri<T, false>(yLocal, xLocal, this->blockLength)`**.
=> `template <typename T, bool isReuseSource = false>` — **2 template params, NO config param.**
=> **Do NOT create `ndtri_utils.h`/`NdtriConfig`.** (erf's `ErfConfig` exists only because erf has 2 algos.)
=> The (dst,src,calCount) overload must work WITHOUT a caller tmp buffer -> falls back to
   `PopStackBuffer<uint8_t, TPosition::LCM>` like `ErfImpl` does (harmless on 3510, buffer unused).

### F4 — file set is SMALLER than first thought
- `kernel_operator_erf_intf.h` = **DEPRECATED shim** -> no ndtri version.
- `erf_tiling_intf.h`          = **DEPRECATED shim** -> no ndtri version.
- `erf_common_impl.h`          = only 2002/2201 branch -> **skip** (3510-only target).
- `api_check/.../erf_check.h`  = included ONLY by legacy `erf_common_impl.h`; `erf_3510_impl.h` has ZERO
                                 `CHECK_FUNC_HIGHLEVEL_API`, checks inline via `ErfCheckParams`
                                 (`CheckTensorPos`/`CheckCalCount` from `impl/adv_api/detail/common/check.h`).
                                 NOTE `xor_3510_impl.h` DOES use the macro -> both patterns exist on 3510.
                                 Default: **follow erf (inline checks, skip api_check files)**.

### F5 — wiring is EXPLICIT, and there are 2 registrations the first pass missed
Per `docs/zh/asc_adv_api_contributing.md` (the official guide; path is `docs/zh/`, not `docs/`):
1. `include/adv_api/kernel_api.h`  -> add `#include "math/ndtri.h"`      (flat list, erf.h at L73)
2. `include/adv_api/tiling_api.h`  -> add `#include "math/ndtri_tiling.h"` (erf_tiling.h at L69)
3. `tests/api/adv_api/CMakeLists.txt` L260 `file(GLOB ASCENDC_TEST_ascend950pr_9599_AIV_CASE_SRC_FILES ...)`
   hand-lists paths; `math/erf/test_operator_erf.cpp` is in it (~L298). Add ndtri line THERE.
4. Tiling UT is centralized: `tests/api/adv_api/tiling/test_tiling.cpp` + `ASCENDC_TILING_TEST_SRC_FILES`.
950pr AIV defines `__NPU_ARCH__=3510;__DAV_C310__;__DAV_C310_VEC__`, links `tikicpulib::ascend950pr_9599`.
Build/run UT: `bash build.sh -t`; target `ascendc_utest_ascend950pr_9599_AIV`; filter via `tests/main_global.cpp`.

### F6 — `erfinvf` ships as SIMT API on 950PR
`docs/zh/api/SIMT-API/数学函数/float类型数学库函数/erfinvf.md` -> 950PR 支持.
adv_api headers hard-`#error` under `--enable-simt` -> **cannot call from SIMD adv_api path.**
Use as golden/cross-check oracle only. Task is unambiguously 高阶API -> adv_api SIMD. Scope Q resolved.

### F7 — ACCEPTANCE HARNESS ALREADY WRITTEN (do not author our own perf harness)
`self_test_case/prims_ndtri/run.sh -r <cpu|sim|npu> --size <N> --is_perf <0|1>` (all mandatory).
Currently wired to **Digamma** as placeholder. **3 swaps to make it ours:**
  a. `ndtri.asc`: `AscendC::Digamma<T,false>` -> `AscendC::Ndtri<T,false>` (2 sites: ComputeOnce+ComputeLoop)
  b. `gen_data.py`: `from scipy.special import digamma` -> `ndtri`; golden `digamma(x)` -> `ndtri(x)`
  c. `gen_data.py`: input range -> `np.random.uniform(0, 1, ...)` (already (0,1); README says "改回(0,1)")
TEST_MODE: 1=功能(precision), 2=空跑基线(copy only), 3=计算循环1000次.
**AIV_VEC占比 = aiv_vec_time(MODE3) / (aiv_time(MODE3) - aiv_time(MODE2))**, threshold 90%, from
`op_summary_*.csv` via msprof. run.sh computes+asserts it automatically.
验证范围: 功能 shape `1`/`32`/`1023`/`2048` (对齐+非对齐), float, (0,1) + `±Inf`;
          性能 `1k`/`4k`/`8k`/`16k`/`32k`/`64k` ALL must be >=90%.
Kernel is single-core (`USED_CORE_NUM=1`), DataCopyPad, whole tensor in UB at once.

### F8 — ENV DECISION: sim in docker first, silicon later (user, 2026-07-17)
Container `catlass950` UP, image `ascendai/cann:9.0.1-950-openeuler24.03-py3.10`. VERIFIED inside it:
- `msprof` at `/usr/local/Ascend/cann-9.0.1/bin/msprof`; `msprof op simulator` subcmd EXISTS (msopprof).
- simulator targets present: `Ascend950PR_9599` (our SoC) and `dav_3510` (our arch). Sim is viable.
- **RISK: container is CANN 9.0.1, task doc requires >= 9.1.0.** Sim targets exist anyway. Watch for
  API/ABI gaps; final numbers must come from silicon regardless.
**`run.sh` HARD-BLOCKS perf on sim** (L61-64): `--is_perf 1` + `-r != npu` -> `exit 1`.
The `-r sim` -> `msprof op simulator` branch (L95-96) is **DEAD CODE** — unreachable, since build_and_run
2/3 is only called from run_perf_verify, which only runs when IS_PERF=1, which the guard forces to npu.
=> In docker sim we get **functional only**: `bash run.sh -r sim --size <N> --is_perf 0`.
=> For a sim perf SIGNAL (trend, not acceptance): patch the L61 guard locally to allow sim, which
   activates the dead L95-96 branch. **Dev-only hack — never ship it in the deliverable.**
=> **Acceptance perf numbers (aiv_vec>=90%) MUST come from silicon** (`-r npu --is_perf 1`). User will run.

### F9 — INPUT IS A SEGMENT (0,1), AND THE SHIPPED SAMPLING UNDER-TESTS THE HARD PART
Input domain = open interval (0,1) (probability). Memory-wise also a segment: contiguous 1-D ND
[2048] fp32, **single core** (`USED_CORE_NUM=1`), one `DataCopyPad` into UB, computed in place. No tiling.
`gen_data.py` uses `np.random.uniform(0,1,[size])` = **FLAT sampling of a NON-FLAT problem**:
  - central [0.02425, 0.97575] = 95.15% of the segment -> ~1949 of 2048 samples
  - tails                      =  4.85%                -> ~99 of 2048
  - deep tail p<1e-6           =  1e-4 %               -> ~0.002 -> **NEVER SAMPLED**
=> The tail branch (sqrt(-2 ln p), different coeffs, **most likely to be wrong**) is barely exercised;
   the deep tail (p~1e-9, z~-6) is unreachable by uniform draws (needs ~1e9 samples to see one).
=> **A kernel with a broken deep tail PASSES the default harness.** And 双万分之一 TOLERATES 1e-4 of
   elements failing -> even the ~485 tail pts in a 10k run can hide tail failures inside the budget.
=> Task doc says 验收 uses **泛化数据** + demands 边界 handling -> graders WILL go where uniform doesn't.
**ACTION (Phase 1/3):** extend gen_data ourselves — sample the way the FUNCTION is shaped, not the
interval: log-spaced p in {1e-12..1e-3} + 1-p mirror; breakpoint neighborhoods (just either side of
0.02425 / 0.97575, where the Select mask flips — branch-seam bugs live here); exact p=0/1/0.5;
out-of-range; NaN/±Inf. Shipped gen_data = smoke test, NOT a precision test. Extend, don't trust.

### F11 — NPU op accuracy is DOCUMENTED -> the fp32 margin risk is bounded (good news)
`docs/zh/api/SIMD-API/基础API/Reg矢量计算/基础算术/{Ln,Sqrt,Div}.md` — **最大精度误差**:
- `Reg::Ln`   : **1 ulp** (INTRINSIC / PRECISION_1ULP_FTZ_*). float+half.
- `Reg::Sqrt` : 1 ulp (INTRINSIC); **0 ulp** via `SqrtAlgo::PRECISION_0ULP_FTZ_FALSE` / `FAST_INVERSE`
                (float only, input <= 8.5e37; also enables Subnormal). `precisionMode=true` same effect.
- `Reg::Div`  : 1 ulp (INTRINSIC); **0 ulp** via `DivAlgo::DIFF_COMPENSATION` / `PRECISION_0ULP_FTZ_TRUE`
                (float). `precisionMode=true` -> 差值补偿 algorithm.
Error budget: Ln 1ulp -> rel err of ln(t) ~6e-8; q=sqrt(-2 ln t) -> dq/q = 0.5*6e-8 = **3e-8**. Negligible.
Sqrt/Div can be forced to **0 ulp** by config. => transcendentals contribute ~1e-7, while our measured
error is 5.3e-5 (dominated by the rational's fp32 conditioning). **The ~1.9x margin should HOLD on
device.** Levers if it doesn't: 0-ulp Div/Sqrt configs, then refit P0/Q0, then split central.
(Tail only uses t<=0.02425 -> ln t <= -3.7, never near the ln(1)=0 blowup.)

### F12 — binary `Reg::Min`/`Reg::Max` EXIST; the `0.5-|q|` fold is UNSAFE — REJECTED
`include/basic_api/reg_compute/kernel_reg_compute_vec_binary_intf.h`: `Min(dst,src0,src1,mask)` and
`Max(...)` exist (not just scalar `Mins`/`Maxs`). So `t = Reg::Min(p, 1-p)` is directly available.
Considered `t = 0.5-|q|` (reuses q, saves an op). **VERIFIED BROKEN** — Sterbenz only guarantees
`p-0.5` exact for p in [0.25,1]. For p<0.25, ulp(0.5)=6e-8 swamps p and q rounds to exactly -0.5:
```
p=1e-10 -> q=-0.5 -> t_fold=0.0   (t_min=1e-10)  BROKEN -> Ln(0)=-inf -> deep tail destroyed
p=1e-3  -> t_fold=0.00099998713   (t_min=0.001)  BROKEN
p>=0.25 -> OK
```
=> **MUST use `Reg::Min(p, 1-p)`.** Do not "optimize" into the fold.

### F13 — WHERE THE VECTOR-CORE FUNCS LIVE (traced end-to-end)
The `Reg::` ops are **NOT library code** — they bottom out in **Bisheng (ASC) compiler intrinsics**:
```
AscendC::Reg::Sqrt<...>            include/basic_api/reg_compute/kernel_reg_compute_vec_unary_intf.h
  -> Reg::SqrtImpl<...>            impl/basic_api/reg_compute/dav_3510/kernel_reg_compute_vec_unary_impl.h
     (arch dispatch: dav_3510 / dav_m510 / dav_l300 / dav_l311 ... selected by __NPU_ARCH__)
    -> vsqrt(dst, src, mask, mode)  <-- COMPILER INTRINSIC, not in this repo
       /usr/local/Ascend/cann-9.0.1/tools/bisheng_compiler/lib/clang/15.0.5/include/
           __clang_cce_vector_intrinsics.h   (1094 intrinsics)
           __clang_cce_vector_types.h
      -> __builtin_cce_v*  (clang builtin) -> single hardware vector instruction
```
Leaf builtins used by erf/digamma/our ndtri on 3510 (all present in the dav_3510 unary impl):
`vsqrt vln vdiv vmul vmuls vadds vmula(FMA) vsel vcmps_lt vcmps_le vcmps_eq vcmp_lt`
**Register widths (from `__clang_cce_vector_types.h`):**
- `vector_f32 = float __attribute__((ext_vector_type(64)))` -> **64 x fp32 = 256 B = 2048-bit vreg**
  => matches `B32_DATA_NUM_PER_REPEAT = 64` (`impl/basic_api/utils/kernel_utils_constants.h:254`)
  => this is WHY the repeat loop is `CeilDivision(calCount, 64)`.
- `vector_f16 = 128 x f16`, `vector_u8 = 256 x u8`, `vector_bool = 4 x i64 = 256 predicate bits`
- `Reg::MaskReg = vector_bool`; `Reg::RegTensor<T>::RegType = TypeGet<T>::T` (= `vector_f32` for float),
  i.e. RegTensor is a THIN WRAPPER over the compiler's native vector type (`RegType reg[REG_NUM]`).
Consequence: `Reg::` code is register-resident SIMD; the compiler allocates physical vregs. No UB traffic
between ops -> that is the mechanism behind the aiv_vec>90% target.
`Sqrt`/`Div` precision configs are implemented in the devkit impl (e.g. `SqrtFastInverseImpl`), NOT the
hardware — the 0-ulp modes cost extra instructions (vcmps_lt/vmuls/vsel wrappers around vsqrt).

### F14 — MENTAL MODEL for Reg:: (why the code looks the way it does)
**Not array code. Scalar code where every variable is 64 floats wide, and `if` does not exist.**
- OLD UB/LocalTensor model (2201/2002) ~= **numpy**: each op streams arrays through UB memory.
- NEW Reg model (3510)              ~= **assembly with wide regs**: each op = 1 instruction on 1 vreg.
  `RegTensor<float>` IS `vector_f32` (2048-bit machine register) + a C++ type. Not a container.
  => erf_3510_impl.h's "weird straight-line bit-twiddling" is the NATURAL OUTPUT of this model.

**Analogy:** 64 workstations, one foreman. One instruction -> all 64 act on their own workpiece.
You CANNOT tell station 7 to do something else — only to **ignore its result** (that's the mask).

**The key non-obvious fact:** *masking a lane off saves NO time.* The instruction still issues; the lane
just discards. 64 live lanes cost the same as 1 live lane. Everything below follows from this:
1. **Branching buys nothing** — no per-lane control flow exists to skip work with. You pay for both paths
   anyway => compute both, `vsel` the winner. Our ndtri double-compute is FREE.
2. **Arithmetic ~free, memory is the enemy** (inverted cost model). UB touched exactly 2x per repeat
   (LoadAlign / StoreAlign); all intermediates stay in vregs. **THIS is the aiv_vec>90% mechanism.**
3. **Registers are the scarce resource**, not memory. Holding both branches live has a real budget ->
   Horner (single running accumulator) is the right shape.
4. **Ragged edges free**: `UpdateMask<float>(calCount)` predicates the tail repeat (1023 -> 63/64 lanes
   live). No scalar cleanup loop => spec shapes 1 / 1023 cost nothing extra.

**Consequences (design stops being a choice):** Select is the ONLY combine primitive; `vln` garbage on
discarded lanes is safe because `vsel` is a bit-level lane pick that never does arithmetic on the loser
(=> NEVER turn it into mul-by-mask: 0*NaN=NaN); `repeatTimes=CeilDiv(calCount,64)` because 64 IS the
register width, not a tuning knob.

**Stack:** `Ndtri(dst,src,n)` [adv_api, our job] -> `Reg::Sqrt(d,s,mask)` [typed+arch-dispatched ISA,
devkit] -> `vsqrt()` [Bisheng intrinsic, leaves repo] -> `__builtin_cce_v*` -> 1 HW instruction.
We deliver the top layer but must THINK in the second.

### F15 — **SIM RUN PASSED, ALL SPEC SHAPES** (2026-07-17, catlass950, 950PR sim, dav-3510)
`talk/sim_check/{ndtri.asc,gen_data.py}` — inline Reg:: hybrid, run via the official harness scaffold.
Function-shaped sampling (@2048: 51.3% tail pts, 258 deep-tail, z in [-12.785, +5.295]).
```
size=1     max rel err 0.000e+00  0/1     PASS   (deep tail p=1e-37; 1 of 64 lanes live)
size=8     max rel err 2.133e-05  0/8     PASS
size=32    max rel err 3.607e-05  0/32    PASS
size=1023  max rel err 4.035e-05  0/1023  PASS   (unaligned; tail repeat 63/64 lanes)
size=2048  max rel err 4.478e-05  0/2048  PASS
```
Spec-required shapes are 1/32/1023/2048 -> **all pass, zero failures.**
NOTE: an early `size=1` FAIL was a bug in OUR gen_data (`size-8` went negative for size<8 ->
crash -> stale golden), NOT the kernel. Fixed by striding the whole pool when size<16.
**NPU (4.48e-5) BEATS the numpy model (5.32e-5)** — as predicted: `MulDstAdd` single-rounds where numpy
Horner double-rounds. Margin vs 1e-4 is ~2.2x. Boundary lanes (7 non-finite) all matched exactly.
=> **The algorithm is validated on real NPU arithmetic.** F10's hybrid stands.

### F16 — sim flags `vec_err_idata_inf_nan` on RV_VLN — OPEN, must resolve
```
[error] [core0.veccore0] [vec_err_idata_inf_nan_t0] (PC: ...) RVECEX : RV_VLN Dtype: F32
```
The branchless design runs `Reg::Ln` on ALL lanes incl. p<=0 / NaN -> Ln sees 0/neg/NaN -> HW raises an
input-data Inf/NaN error flag. **Result is still CORRECT** (Select discards those lanes, test passed),
but the flag is real and the task demands 正确处理边界情况. Risk: a persistent error/overflow status
register (cf. `asc_get_overflow_status`) could fail acceptance or trip a checker.
**Fix is NOT a naive clamp:** `t_safe = Max(t, tiny)` would break NaN propagation — IEEE maxNum(NaN,x)=x,
so p=NaN would return a FINITE wrong answer. If clamping, MUST add an explicit NaN patch first
(`Compares<NE>(m, p, p)` -> isnan) and then clamp. Sequence: patch NaN -> clamp Ln input -> compute.
TODO: (a) determine whether the flag sets a sticky status bit or is sim-only diagnostics;
      (b) if sticky, implement patch-then-clamp and re-verify NaN/±Inf/p=0/p=1 lanes.

### F17 — CANN 9.0.1 vs 9.1.0 compat gotchas (dev-only, DO NOT SHIP)
The shipped harness targets CANN >= 9.1.0; container is 9.0.1. To build, 2 fixes were needed:
1. `DataCopyExtParams` here takes **5** fields `(count,len,srcStride,dstStride,rsv)`; harness passes 4.
2. `DataCopyPad(LocalTensor dst, GlobalTensor src, ...)` needs a 4th `DataCopyPadExtParams<T>()` arg.
Also: `Reg::FusedMulDstAdd` (our repo) does NOT exist in 9.0.1 — only **`Reg::MulDstAdd`** (in both).
**`.asc` is compiled TWICE (device + host).** Device pass lacks `std::isfinite/isnan`; host pass lacks
`__simd_vf__`/`__simd_callee__` (compiler attrs from `__clang_cce_defines.h`) AND all of `Reg::`
(reg_compute is guarded on `__NPU_ARCH__`; note `kernel_operator.h` does NOT include reg_compute in
9.0.1 — `kernel_operator_intf.h` does).
=> Device-only code MUST be wrapped `#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)` with an
   **empty host stub** — exactly the pattern `include/adv_api/math/erf.h` uses. This is why erf.h's
   `#else` branch has empty function bodies; it is NOT dead code.
=> Host-side compare must avoid `std::` math: use `isnan(x) <=> x!=x`, `isfinite(x) <=> (x-x)==0`.

## FILES TO CREATE (corrected, 3510-only)
```
include/adv_api/math/ndtri.h                          # 4 overloads, <T, isReuseSource>; 3510-family guard
include/adv_api/math/ndtri_tiling.h                   # GetNdtriMaxMinTmpSize / GetNdtriTmpBufferFactorSize
impl/adv_api/detail/math/ndtri/ndtri_3510_impl.h      # __simd_vf__ Reg:: core + NdtriCheckParams + NdtriImpl
impl/adv_api/tiling/math/ndtri_tiling_impl.cpp
tests/api/adv_api/math/ndtri/test_operator_ndtri.cpp
docs/zh/api/SIMD-API/高阶API/数学计算/Ndtri接口/{Ndtri接口.md,Ndtri.md,GetNdtri*TmpSize.md}
examples/01_simd_cpp_api/04_advanced_api/10_math/ndtri/{ndtri.asc,CMakeLists.txt,README.md,scripts/}
```
+ edits: `kernel_api.h`, `tiling_api.h`, `tests/api/adv_api/CMakeLists.txt`, `tiling/test_tiling.cpp`.
NO: ndtri_utils.h, ndtri_common_impl.h, *_intf.h shims, api_check files.

## ALGORITHM — **HYBRID: Cephes central + Acklam tail** (Phase 1 DONE, measured)

### F10 — **STOCK ACKLAM FAILS 双万分之一 IN fp32.** Earlier "no refit needed" claim was WRONG.
Measured (`talk/ndtri_acklam_check.py`, `ndtri_diag.py`, `ndtri_refit.py`, `ndtri_central_fix.py`,
run in catlass950, numpy 2.2.6 / scipy 1.15.3):
- Acklam **formula** error (f64 vs scipy) = **1.13e-9** — matches its advertised 1.15e-9. Formula is FINE.
- Acklam **central fp32 evaluation** error (same fp32 input both precisions) = **1.46e-4 -> FAILS 1e-4.**
- Acklam **tail fp32** = **3.7e-7 -> well-conditioned, KEEP.**
**Root cause = catastrophic cancellation, NOT rounding.** A(r) Horner intermediates peak ~276 and collapse
to the final value; amplification = peak/|final|:
  r=0.0 -> 110x | r=0.15 -> 1234x | r=0.2264 -> **21191x**  => fp32 eps 6e-8 x 21191 = **1.3e-3 floor**.
r=0.2264 is exactly the seam (p=0.02425) -> that's why seam_low/high/tail_upper failed too.
My earlier error: conflated the rational's APPROXIMATION error (1e-9, exact arithmetic) with its
EVALUATION conditioning in fp32. Acklam's coeffs were designed for double.
Naive monomial refit of R(r)=ndtri(p)/q ALSO fails (coef -> 1e10): singularity at r=0.25 sits just
outside the fit interval [0, 0.2264] -> poly convergence is poor, monomial basis ill-conditioned.

### THE FIX — factor out the leading term (Cephes structure; scipy.special.ndtri IS Cephes)
central: **z = s2pi * q * (1 + r*P0(r)/Q0(r))**, s2pi=2.5066282746310005, q=p-0.5, r=q*q
  P0 = Cephes deg-4, Q0 = Cephes MONIC deg-8 (`p1evl`). The rational is a bounded **CORRECTION**
  in [0, +0.655] — every contribution same-signed, dominant term `q` carries the answer -> NO cancellation.
tail:    **z = ±C(q)/D(q)**, q=sqrt(-2 ln t), t=min(p,1-p), sign=-1 if p>0.5  (Acklam, D monic-ish +1)
breakpoints pLow=0.02425, pHigh=1-pLow.

### MEASURED RESULT (hybrid, 32002 pts, fp32, vs scipy) — **ALL PASS, 0 failures**
```
deep_lower 3.83e-07 | tail_lower 2.65e-05 | central 3.28e-05 | tail_upper 2.63e-05
seam_low   5.32e-05 | seam_high  5.32e-05 | uniform    4.41e-05
TOTAL max rel err 5.32e-05, #>1e-4 = 0 / 32002   (Acklam was 1.73e-4, 168 fails)
```
Boundaries all match scipy: p=0->-inf, p=1->+inf, p=0.5->0, p<0|p>1|±inf|nan -> nan.
**No Halley/Newton refine needed** — the hybrid clears 1e-4 outright.
**MARGIN IS ONLY ~1.9x (5.3e-5 vs 1e-4).** numpy has no FMA (NPU `FusedMulDstAdd` should be slightly
BETTER), but **NPU Log/Sqrt/Div are approximations of unknown ulp** — could eat the margin. **Must
re-measure on device; if margin dies, options: refit P0/Q0 for fp32, or split central into 2 sub-ranges.**

### fp32 facts that matter
- **Sterbenz**: p in [0.25,1] -> `p-0.5` and `1-p` are **EXACT** in fp32. No cancellation in q or t.
- **Asymmetric domain**: ulp(1)=1.19e-7 -> smallest representable (1-p) is 2^-24=5.96e-8 -> upper tail
  saturates at z~+5.4, while lower tail reaches p~1e-38 -> z~-13. Don't expect symmetric test coverage.
Boundaries: p<0|p>1 -> NaN; p==0 -> -Inf; p==1 -> +Inf; p==0.5 -> 0; NaN -> NaN.
(harness `CompareResult` computes `|a-b|/|b|`; at p=0.5 golden=0 -> re=NaN -> `NaN>EPS` is false in C ->
 not counted wrong. Safe, but don't rely on it — verify p=0.5 exactly.)

### Branchless-in-registers (drives the >90% aiv_vec target)
Compute BOTH forms on every lane, combine with `Reg::Select` on a `Reg::Compare` mask (exactly erf's
`ErfSubsectionCompute` pattern). Horner via `Reg::FusedMulDstAdd`. Constants via `Reg::Duplicate`;
NaN/±Inf as uint32 bit-patterns + reinterpret cast (erf does this).
**CARE-POINT:** tail arg `sqrt(-2 ln(min(p,1-p)))` is evaluated on ALL lanes incl. central/boundary where
arg<=0 (p=0 -> Ln(0)=-Inf; p<0 -> Ln(neg)=NaN). Safe ONLY because `Reg::Select` picks lanes (no arithmetic
blend) — garbage in the unused branch cannot poison the kept lane. **Never** "optimize" Select into
mul-by-mask. Clamp Ln input as belt-and-braces.
**Perf lever:** 2 `Div` (one per form) is the main cost -> consider combining to a single Div, or
`Reg::Select` the numerators/denominators FIRST then one Div. Decide empirically in Phase 3.

## PLAN
- **Phase 0 (de-risk gate, DO FIRST):** build asc-devkit; run existing `erf` UT (ascend950pr_9599_AIV) +
  run the UNMODIFIED `self_test_case/prims_ndtri` (Digamma placeholder) via
  `run.sh -r npu --size 2048 --is_perf 1`. Proves toolchain + msprof + the exact acceptance metric BEFORE
  any ndtri code — and gives a **Digamma baseline aiv_vec%** to compare against.
- **Phase 1:** host golden — Acklam in python vs `scipy.special.ndtri`, fp32-simulated, over
  [1e-12 .. 0.5 .. 1-1e-12] + edges. Confirm no-Halley holds 双万分之一 and whether coeffs need refit.
- **Phase 2:** author files per corrected list. Core = `ndtri_3510_impl.h` (Reg::, branchless). Mirror
  erf/digamma 3510 structure exactly.
- **Phase 3:** UT + self-test. Swap harness (F7 a/b/c). Functional shapes 1/32/1023/2048 + ±Inf +
  out-of-range + p=0.5. Perf 1k..64k all >=90%; iterate Div count / op order. Sim-plot any small-shape miss.
- **Phase 4:** 设计文档 -> review issue (2工作日 SLA — **file EARLY, it gates the PR**) + README +
  自验证报告(日志/截图/性能数据) + PR -> experimental/.

## UNRESOLVED QUESTIONS
- ~~sim vs npu for perf~~ RESOLVED: sim(docker) for functional, silicon for perf acceptance. See F8.
- Container is CANN **9.0.1**, task doc demands **>= 9.1.0**. Does the 自验证报告 need a 9.1.0 env, or is
  9.0.1-sim acceptable for dev given final numbers come from silicon?
- 设计文档 template `BatchNorm接口设计文档.docx` — task doc links `./docs/` but path looks broken. Locate it.
- Does `Ndtri` need the `sharedTmpBuffer` overloads at all, given 3510 ignores the buffer and the harness
  only uses the 3-arg form? (Keep for convention parity — confirm reviewer expectation.)
- api_check: follow erf (skip) or xor (use `CHECK_FUNC_HIGHLEVEL_API`)? Both exist on 3510.
- Example under `examples/.../10_math/ndtri/` — required, or is the self_test_case sufficient? (交付件 lists
  功能测试代码; erf has both.) Presmoke case registration needed (`scripts/presmoke/cases/.../10_math/erf`)?
- fp32 only per task doc — but erf/digamma support half via Cast. Add half opportunistically or refuse?

### F18 — CANN **9.1.0 IS public — as BETA** (checked 2026-07-17, Docker Hub `ascendai/cann`)
462 tags. No GA 9.1.0 exists; every 9.1 tag is `-beta.N`. **950 + devel images DO exist publicly:**
```
ascendai/cann:9.1.0-beta.3-950-openeuler24.03-py3.12-devel   updated 2026-07-03  ~11.8GB  amd64,arm64
ascendai/cann:9.1.0-beta.3-950-ubuntu22.04-py3.12-devel      updated 2026-07-03  ~11.7GB  amd64,arm64
ascendai/cann:9.1.0-beta.1-950-openeuler24.03-py3.12-devel   updated 2026-05-28  ~10.0GB  amd64,arm64
```
Host is arm64 / container aarch64 -> **arm64 variant matches, pullable.**
Current container `catlass950` = `9.0.1-950-openeuler24.03-py3.10` (GA).
**Why upgrading matters:** harness requires >=9.1.0 and does NOT compile on 9.0.1 (F17). On 9.1-beta.3
the UNMODIFIED harness should build -> drops our DataCopyPad/DataCopyExtParams hacks, and likely
restores `Reg::FusedMulDstAdd` (our repo's name, absent in 9.0.1) so sim code matches repo code.
**Caveat:** beta.3 != whatever GA 9.1 the user's silicon runs. Treat as closer-but-not-identical;
silicon remains the source of truth for both precision and perf.

### F19 — **CORRECTION to F17: the "9.0.1 hacks" are NOT 9.0.1-specific** (verified on 9.1.0-beta.3)
Pulled `ascendai/cann:9.1.0-beta.3-950-openeuler24.03-py3.12-devel` (22GB, container `cann91`).
Empirical build test on 9.1-beta.3:
```
harness-native form (4-field DataCopyExtParams, 3-arg DataCopyPad) : FAILS to build
our form (5-field DataCopyExtParams, 4-arg DataCopyPad)            : BUILDS
```
=> 9.1-beta.3 has the **SAME** DataCopy API as 9.0.1: `DataCopyExtParams` has ONLY a 5-field ctor
   (count,len,srcStride,dstStride,rsv); `DataCopyPad(LocalTensor dst, GlobalTensor src, ...)` is 4-arg only.
=> **The official self_test_case harness compiles on NEITHER 9.0.1 nor 9.1-beta.3.** It must target GA 9.1
   or an internal build. My earlier "ndtri_silicon.asc is the 9.1-native one" assumption was WRONG —
   that variant is now deleted; ONE file (`talk/sim_check/ndtri.asc`) builds on both.
=> `Reg::FusedMulDstAdd` does NOT exist in 9.0.1 OR 9.1-beta.3 — only `Reg::MulDstAdd`. Our repo's
   erf_3510_impl.h uses FusedMulDstAdd => **our repo master is NEWER than CANN 9.1.0-beta.3.**
**Open for silicon:** if the user's GA 9.1 device rejects the 5-field form, flip to the harness's
4-field/3-arg form — compute is unaffected, only the 3 DataCopyPad call sites change.

### F20 — OUR standalone test (`talk/sim_check/`) — self-contained, no competition repo needed
```
talk/sim_check/{CMakeLists.txt, data_utils.h, ndtri.asc, gen_data.py, run.sh}
  ./run.sh sim|npu [--perf|--perf-only]
```
Ours vs official run.sh (deliberate): sweeps ALL shapes per invocation; does NOT hard-block perf on
non-npu; prints a paste-ready summary; surfaces the RV_VLN inf/nan flag (F16) when raised.
Env overrides: `SHAPES="1 32 1023 2048"`, `PERF_SIZES="1024 ... 65536"`.

### F21 — 9.1.0-beta.3 sim results are **BIT-IDENTICAL to 9.0.1** (2026-07-17)
`./run.sh sim` in container `cann91` (CANN 9.1.0-beta.3, numpy 2.5.0 / scipy 1.18.0):
```
size=1     0.000e+00  0/1     PASS
size=32    3.607e-05  0/32    PASS
size=1023  4.035e-05  0/1023  PASS
size=2048  4.478e-05  0/2048  PASS      -> ALL PASS
```
Identical to the 9.0.1 run (F15) to every digit => **the arithmetic is stable across CANN versions**;
the Reg:: op semantics/precision did not change between 9.0.1 and 9.1-beta.3. Raises confidence that
GA 9.1 on silicon will match, though silicon remains the source of truth.
**RV_VLN inf/nan flag (F16) reproduces on 9.1** — and correlates EXACTLY with whether the sample
contains boundary/out-of-domain points: raised for size=32/1023/2048 (gen_data includes the 8 edges
when size>=16), NOT raised for size=1 (single deep-tail point p=1e-37 -> Ln arg finite).
=> Confirms the diagnosis: the flag is caused by Ln() on discarded p<=0 / NaN lanes, nothing else.
=> Still OPEN whether it sets a sticky status on real silicon. Watch for it in the npu run.

### F22 — **GA CANN 9.1.0 BUILDS OUR KERNEL** (user's device, 2026-07-17) — F19 concern RESOLVED
User ran `VERBOSE=1 ./run.sh npu` on real hw box: `ASCEND_HOME_PATH=/root/Ascend/cann-9.1.0`,
`Version=9.1.0` (GA, not beta). Result:
```
/root/Ascend/cann-9.1.0/bin/bisheng -DSIZE=2048 -DTEST_MODE=1 -std=c++17 --npu-arch=dav-3510 ...
[100%] Built target demo      [build] ok -> 383232 bytes
```
=> **The 5-field DataCopyExtParams / 4-arg DataCopyPad form builds on 9.0.1, 9.1-beta.3 AND GA 9.1.0.**
   No need to flip to the harness's 4-field/3-arg form. ONE `ndtri.asc` covers every CANN we've seen.
   (Still unexplained: what CANN the official harness's own 4-field form targets. Not our problem.)
=> `Reg::MulDstAdd` + `Reg::Compares` + `Reg::Min` all fine on GA 9.1.0.

### F23 — run.sh bugs found by the user's first real run (both mine, both fixed)
1. **gen_data.py path**: repo had `talk/sim_check/gen_data.py` but run.sh calls `../scripts/gen_data.py`.
   (Worked in my containers only because I `docker cp`'d it INTO `scripts/`.) -> moved to
   `talk/sim_check/scripts/gen_data.py`. Layout now matches the harness convention.
2. **preflight detected the missing file but did not abort** -> built 4x and failed 4x anyway.
   -> `preflight || exit 1`. Fatal: cmake/python3/scipy/numpy missing, set_env.sh not sourced,
   any required file missing. `npu-smi` NOT fatal (absent on PATH in many containers even when the
   NPU works — user's box has no npu-smi but bisheng builds fine).

### F24 — **SILICON RESULTS** (user's box, GA CANN 9.1.0, 2026-07-17) — precision CONFIRMED, perf ~100%
**FUNCTIONAL on real 950PR silicon — BIT-IDENTICAL to both simulators:**
```
size=1     0.000e+00  0/1     PASS
size=32    3.607e-05  0/32    PASS
size=1023  4.035e-05  0/1023  PASS
size=2048  4.478e-05  0/2048  PASS
```
Same digits as 9.0.1 sim (F15) and 9.1-beta.3 sim (F21). => **Algorithm fully confirmed on hardware.**
双万分之一 met with ~2.2x margin. No RV_VLN flag reported on silicon (F16 may be sim-only diagnostics —
still unconfirmed since our sample DOES include boundary pts and the flag did NOT appear in the npu log).

**PERF (aiv_vec, need >=90%):**
```
size=1024   100.01%   (t1=4.09  t2=583.08    vec=579.058)
size=4096   100.0 %   (t1=4.67  t2=2198.25   vec=2193.688)
size=8192   100.02%   (t1=6.21  t2=4351.94   vec=4346.639)
size=16384   99.99%   (t1=7.57  t2=8659.8    vec=8651.702)
size=32768  100.0 %   (t1=11.77 t2=17276.39  vec=17264.136)
size=65536  NODATA    (t1=18.34 t2=18.34 vec=0.0)  <- see F25
```
**~100% aiv_vec on 5/6 sizes — the branchless design delivers exactly what F14 predicted.** Slight >100%
readings are formula artifact (vec_time vs the approximate t2-t1 subtraction); read as "fully vec-bound".

### F25 — 64k FAILED: whole-tensor-in-UB exceeds the 248KB UB. Wrapper now TILES.
`TOTAL_UB_SIZE = 248*1024` for `__NPU_ARCH__==3510` (`impl/basic_api/utils/kernel_utils_constants.h:165`).
The official harness wrapper (and mine, copied from it) does `InitBuffer(q, 1, alignLength*sizeof(T))`
for BOTH in and out => needs **N*4*2 bytes**:
```
N=16384 -> 128KB fits | N=32768 -> 256KB (at/over limit, still ran) | N=65536 -> 512KB EXCEEDS -> crash
```
=> demo crashed at 64k, msprof wrote no CSV, and my `extract` silently read the **STALE mode-2 CSV**
   => t1==t2, vec=0 => "NODATA". TWO bugs of mine: no tiling, and stale-CSV masking a crash.
**NOTE the official harness CANNOT do its own required 64k perf size** — its README says
单核处理，数据一次性搬入UB (whole tensor into UB at once), which caps at N=32768 on a 248KB UB.
Spec demands perf at 64k. That is an inconsistency in THEIR harness, not in the API.
**FIX (ours):** wrapper tiles at `TILE_ELEMS=8192` (8192*4*2 = 64KB, N-independent footprint).
Ndtri is compute-only on LocalTensor — data movement is the CALLER's job, so tiling belongs in the
wrapper, NOT in the API. run.sh now `prof_clean`s PROF_* before each msprof run and FAILS LOUDLY if no
CSV is produced, instead of reading a stale one.

### F26 — tiled wrapper VERIFIED in sim, incl. tile-boundary cases (9.1-beta.3)
```
size=1      0.000e+00  PASS
size=1023   4.035e-05  PASS   <- identical to pre-tiling => tiling transparent for N < TILE_ELEMS
size=8192   4.604e-05  PASS   <- exactly one tile
size=8193   4.604e-05  PASS   <- 2 tiles, 2nd tile = 1 element (off-by-one trap)
size=20000  4.611e-05  PASS   <- 2 full tiles + 3616 ragged
```
UB footprint now constant 64KB regardless of N => 64k (and beyond) is unblocked.
NOTE `TQue` depth is 1 (no ping-pong), inherited from the official harness. Deliberate: with
PERF_LOOPS=1000 the copy is only 0.07-0.70% of measured time (from silicon t1/t2), and the metric
subtracts the copy baseline anyway => BUFFER_NUM=2 would not move aiv_vec (already ~100%).
A REAL operator (1 compute pass/tile) WOULD need ping-pong — cf. `examples/.../10_math/erf/erf.asc`
which uses `BUFFER_NUM = 2`. If we ship an ndtri example, it should ping-pong like erf.

### F27 — PIPELINE: depth-1 serializes MTE2->V->MTE3; our 100% aiv_vec HIDES the stall
Wrapper `TQue` depth is 1 => strictly serial per tile; **V idles during MTE2 and MTE3.**
Our aiv_vec ~100% does NOT prove a stall-free pipeline — `PERF_LOOPS=1000` amortizes the copy to
0.07-0.70% of measured time (silicon t1/t2), AND the metric subtracts the copy baseline. Both erase
exactly the stall.
**Instruction count (counted from source, not guessed):** ~**71 vector instrs per 64-elem repeat**
= 31 direct Reg:: calls + 19 FmaStep x2 (Duplicate+MulDstAdd) + LoadAlign + StoreAlign.
Breakdown: Adds4 Mul4 Duplicate7 Div2 Muls2 Sub1 Min1 Ln1 Sqrt1 Neg1 Select6 Abs1 + 38 (FmaStep).
Per 8192-elem tile: ~9088 vector instrs, moving 32KB in + 32KB out.
**Back-of-envelope (assumes ~1 instr/cyc, ~64B/cyc MTE — NOT measured):**
```
compute ~9088 cyc | copy ~1024 cyc -> copy/compute ~11%
single-buffered (serial) : aiv_vec ~90%   <- RIGHT ON THE BAR
ping-pong (BUFFER_NUM=2) : aiv_vec ~100%  (copy hidden under compute)
```
=> A REAL one-pass ndtri operator would sit ~90% single-buffered — uncomfortably tight. Ping-pong
   buys the headroom. ndtri is compute-heavy enough (~71 instr / 256B) to stay vector-bound, but an
   11% serialization penalty does not vanish.
**Does NOT affect the deliverable's graded number:** the API has no data movement (LocalTensor in, UB
already loaded). Spec grades 作为vector类高阶API实现Vector Bound = the API's COMPUTE, and the
1000x-loop-minus-baseline metric is deliberately built to isolate that. Overlap is the caller's job.
**BUT:** if we ship `examples/.../10_math/ndtri/`, it MUST ping-pong like `erf.asc` (BUFFER_NUM=2),
else we'd demo a pattern that stalls ~10%.
**Also noted:** 26 of the ~71 instrs are `Duplicate` of loop-INVARIANT constants. If the compiler
hoists them out of the repeat loop the inner loop drops to ~45 instrs (-36%); if register pressure
blocks hoisting they stay. FMA path (Duplicate+MulDstAdd) costs the SAME 2 instrs as Mul+Adds but
gives 1 rounding instead of 2 => strictly better; keep it. Worth checking the generated asm someday.

### F28 — THREE measurement flaws inherited from the official harness (all fixed)
1. **48s per perf point = REBUILD, not kernel.** The harness bakes `SIZE`/`TEST_MODE` in as `-D`
   compile-time constants (`constexpr uint32_t TOTAL_LENGTH = SIZE;`, `#if TEST_MODE == 1`), so every
   size AND every mode forces a full cmake+bisheng rebuild (perf = 2 rebuilds/size). Kernel itself is
   ~0.6us. **FIX: size/mode/perfLoops are now RUNTIME argv** -> `./demo <size> <mode> <loops>`, ONE build.
2. **"wall time" was process wall** (~7s at N=1) = aclInit + sim model startup, independent of N.
   FIX: `demo` now times ONLY launch->sync via `gettimeofday` (NOT <chrono>: device pass has no std::)
   and prints `kernel time: X us`. In **sim** that is still ~21s of SOFTWARE MODEL, so run.sh labels it
   `simwall=...s (SIM MODEL, not device time)` — never quote sim time as perf. Real device: microseconds.
3. **aiv_vec ~100% is DEGENERATE, not impressive.** `aiv_vec = vec_time/(t3-t2)`: mode2 and mode3 do
   IDENTICAL copies so they cancel in the subtraction, and `PERF_LOOPS=1000` makes copy 1/1000 of a
   tile anyway => the ratio is FORCED to ~100% **at every size**; it cannot distinguish 1k from 128k.
   It measures "is NdtriCompute pure vector work?" (yes) and says NOTHING about the pipeline (F27).
   **FIX: `PERF_LOOPS` is now a runtime knob.** `PERF_LOOPS=1 ./run.sh npu --perf-only` = real
   single-pass operator: copy is no longer amortised, the depth-1 MTE2->V->MTE3 stall shows up, and the
   number actually varies with size. Use 1000 to satisfy the spec gate; use 1 to learn the truth.
Reference (user's silicon): N=1024, t2=583.08us over 1000 passes -> **~0.58us/pass, ~0.57ns/element**.
Simulator is ~40,000x slower than the chip.

### F29 — single-run usage + sim perf is genuinely unavailable
Since size/mode/loops are runtime args (F28), a one-off run needs NO rebuild and NO sweep:
```
cd build && python3 ../scripts/gen_data.py 2048 && ./demo 2048 1
./demo <size> <mode 1|2|3> [loops]     # 1=precision 2=copy-baseline 3=compute-loop
```
`msprof op simulator` produces NO `op_summary_*.csv` -> **sim perf does not work at all** (confirmed;
this was the official run.sh's dead-code branch, never functional). Perf REQUIRES `-r npu`. run.sh now
fails loudly there instead of reading a stale CSV and printing a fake ratio.
Dropped the `ns/elem` column (derived noise). Perf line now shows real `kernel=` (launch->sync of the
mode-3 run) alongside the raw `aiv: t1/t2/vec`.

### F30 — **REAL single-pass aiv_vec = 82.3%, BELOW the 90% bar.** Ping-pong added. (silicon msprof)
User's msprof dump, N=131072 single-pass (PERF_LOOPS=1), BUFFER_NUM=1. Recovered total cycles from
MTE2's active rate and cross-checked against MTE3 + Scalar to 6 decimals (both reproduce exactly):
```
total kernel   128,892 cyc
  MTE2 active   14,744 cyc  11.4%   (18 instrs = 16 tiles + 2)
  MTE3 active    8,065 cyc   6.3%
  Scalar         2,281 cyc   1.8%   (725 instrs, loop overhead — small, fine)
  => Vector    106,083 cyc  82.3%   <- THE REAL aiv_vec
  V blocked on copy 22,809 cyc (17.7%)  <- depth-1 serialisation MTE2->V->MTE3
```
Run identified from `Vector Read UB = 2048 requests` -> 2048 repeats x 64 = 131072 elements.
**So F27's ~90% estimate was OPTIMISTIC; reality is 82.3% -> would FAIL a real single-pass gate.**
The PERF_LOOPS=1000 spec metric structurally CANNOT see this (amortises copy to 0.1%) — which is
exactly why it reports ~100% at every size (F28.3).
**FIX: BUFFER_NUM=2 ping-pong** — MTE2 fetches tile N+1 while V computes tile N, hiding 22,809 cyc
under 106,083 cyc of compute => aiv_vec ~100%, ~1.22x wall speedup.
UB: 2 queues x 2 buf x 32KB = 128KB, fits the 248KB UB.
**Other numbers from the dump, all benign:**
- `L2 Cache Read hit 0% (0/4097)` — elementwise streaming, zero reuse. Expected, unavoidable, harmless.
- `GM read/write 6.25 GB/s` each — this is ONE core; the kernel is COMPUTE-bound (MTE2 active only
  11.4%), so bandwidth is not the limit. `UB Read GM peak 3.83%` confirms memory is nowhere near saturated.
- `iCache 100% hit`, `Scalar 1.8%` — no icache thrash, negligible scalar overhead. Good.
- Consistency check: 0.57 ns/elem (from N=1024 t2=583us/1000 passes) x 131072 = 74.7us ~= 128,892 cyc
  @ ~1.7GHz = 75.8us. The two independent measurements agree.

### F31 — **PING-PONG CONFIRMED ON SILICON: 82.3% -> 98%** (single-pass, N=128k, PERF_LOOPS=1)
Predicted 98.7%, measured **98%**. The cycle model from F30 is now validated end-to-end:
```
BEFORE  128,892 cyc = MTE2 14,744 + V 106,083 + MTE3 8,065     -> aiv_vec 82.3%
AFTER   ~107,509 cyc = V 106,083 + fill 922 + drain 504         -> aiv_vec 98.7% (measured 98%)
        wall 1.20x faster (predicted 1.22x)
```
**Residual ~2% = pipeline fill/drain**: the FIRST tile's MTE2 (~922 cyc) and the LAST tile's MTE3
(~504 cyc) have nothing to overlap with. Irreducible at 16 tiles — NOT a bug, not worth chasing.
Shrinking TILE_ELEMS would shave it (4096 -> ~99.3%, 2048 -> ~99.7%) but trades against per-tile
scalar/loop overhead. 98% is far past the 90% gate.
**SIGNIFICANCE: we now clear 90% on the HONEST metric (PERF_LOOPS=1, real single-pass operator),**
not merely on the spec's degenerate PERF_LOOPS=1000 metric that reports ~100% for anything.
That is a much stronger claim than the original "100%".
=> Perf story is CLOSED: spec gate ~100% (PERF_LOOPS=1000) AND real single-pass 98% (PERF_LOOPS=1).
=> Any shipped example MUST keep BUFFER_NUM=2; without it a real operator sits at 82%.
