# M510 (npu_arch_5102) capi Test Suite

This directory contains the Ascend C C-API (capi) unit tests for the **M510 chip series**. Design goal: **maximize reuse of the 3510 (`npu_arch_3510`) cases**, and annotate separately only the known differences between M510 and 3510.

> Companion document: `docs/zh/api/SIMD-API/c_api/rfc_capi_m510_adaptation.md`

## 1. Compile-time Identifiers

M510 is identified in the build project as `__NPU_ARCH__ == 5102` (coupled architecture `PRE_COMPILE_KERNEL_MIX_COUPLING`, core macros `__DAV_M200__` / `__DAV_M200_VEC__`). The capi directory naming follows the existing numeric convention: **`npu_arch_5102`**.

## 2. Case Status Legend

| Status | Meaning |
|--------|---------|
| **Reuse** | Clone the 3510 case with the same name directly (API/instruction set is identical); proves reuse is feasible |
| **Adapt** | Needs rewriting for M510 differences (cube coupled paths, expdiff type combinations) |
| **Drop** | 3510-only, or not provided in the first M510 phase: fp4 MX (`asc_mmad_mx*`), cube L0 load paths (GM->L0A/L0B, L1->L0A/L0B transpose/sparse, L1->BT/FB, LoadImage->cbuf), the expdiff `Tuple<float,half>` combination (`asc_exp_sub_half2float`), and VF-debug/ring-buffer debug facilities. **Note**: the fp4/fp8/s4 instruction paths are actually **supported** on M510 (corrected in RFC 4.10, so the related cases are Reuse) |

## 3. Full Test Matrix (aligned with npu_arch_3510)

| Category (subdir) | 3510 cases | M510 status | Notes |
|-------------------|-----------|-------------|-------|
| `atomic/` | `test_set_atomic_add_int16.cpp` and the other 3510 atomic cases | Reuse | Atomic APIs are identical to 3510 |
| `cube_compute/` | `test_enable_fp8.cpp`, `test_enable_hf32.cpp`, `test_set_fp32_mode.cpp`, `test_mmad.cpp`, `test_set_hf32_round_mode.cpp`, `test_enable_hf32_trans.cpp`, `test_mmad_mx.cpp`, `test_enable_hif8.cpp`, `test_mmad_mx_sync.cpp`, `test_mmad_bias.cpp`, `test_set_mm_transform.cpp` | Reuse (shared mmad subset: `test_mmad.cpp`, `test_mmad_bias.cpp`, `test_set_mm_transform.cpp`, `test_set_fp32_mode.cpp`, `test_set_hf32_round_mode.cpp`, etc.) | M510 supports shared operators such as mmad/fixpipe/l0c2gm |
| `cube_compute/` | `test_mmad_mx.cpp`, `test_mmad_mx_bias.cpp`, `test_mmad_mx_sync.cpp` | Drop | fp4 MX is 3510-only; not supported in the first M510 phase |
| `cube_datamove/` | `test_asc_copy_gm2l1_align.cpp`, `test_asc_copy_gm2l1.cpp`, `test_asc_copy_gm2l1_dn2nz.cpp`, `test_asc_copy_gm2l1_nd2nz.cpp`, `test_asc_set_l0c2gm_nz2nd.cpp`, `test_copy_l0c2l1_instr.cpp`, `test_asc_set_l0c2gm_lrelu_alpha.cpp`, `test_asc_set_l0c2gm_config.cpp`, `test_asc_copy_l0c2gm.cpp`, `test_asc_copy_l0c2ub.cpp`, `test_asc_copy_l12ub.cpp`, `test_copy_l12l0.cpp` (L1->L0C), `test_set_gm2l1_lp2_stride.cpp`, `test_set_gm2l1_lp1_stride.cpp`, `test_set_gm2l1_pad.cpp`, `test_set_gm2l1_lp_size.cpp`, `test_set_l1_3d_fmatrix.cpp`, `test_fill_l1_value.cpp`, `test_set_l13d_rpt.cpp`, `test_set_l0c_copy_prequant.cpp` | Reuse | Shared cube operators supported by M510: gm2l1* / l0c2gm / l0c2l1 / l0c2ub / l12l0c / l12ub / l12gm |
| `cube_datamove/` | `test_asc_copy_l12l0a.cpp`, `test_asc_copy_l12l0b.cpp`, `test_copy_l12l0b_trans.cpp`, `test_copy_l12l0a_mx.cpp`, `test_asc_copy_l12bt.cpp`, `test_asc_copy_l12fb.cpp` | Drop | Paths such as GM->L0A/L0B, L1->L0A/L0B transpose/sparse, L1->BT/FB, and LoadImage to cbuf are not supported on M510 (stubbed in `impl/basic_api/dav_m510/kernel_operator_mm_impl.h`); the shared/generic cube_datamove headers exclude them via "exclude only 5102" guards per RFC 4.8, so cloned cases must not reference these functions |
| Dispatch chain | - | Note | `asc_copy_l0c2gm` / `asc_copy_l0c2l1` are **available** on M510 (the top-level cube dispatch chain now has a 5102 branch, RFC 4.9); cases using these two operators can be cloned from 3510 directly |
| `cache_ctrl/` | `test_asc_data_cache_preload.cpp`, `test_dci.cpp`, `test_icache_preload.cpp`, `test_dcci.cpp`, `test_asc_scalar_cache_mode.cpp`, `test_get_icache_preload_status.cpp` | Reuse (verify the specific instructions are consistent on M510) | |
| `misc/` | `test_asc_init.cpp`, `TestAscNopCAPI.cpp` | Reuse | |
| `scalar_compute/` | `test_asc_atomic_inc.cpp` and the other 3510 scalar cases | Reuse | M510 follows the generic scalar path (`dav_m510` has no scalar-specific files) |
| `sync/` | `test_asc_lock.cpp` and the other 3510 sync cases | Reuse | |
| `sys_var/` | See the full 3510 `sys_var/` | Reuse | |
| `vector_compute/` | `test_return_vconv.cpp` (vconv overloads, including fp4/fp8/s4 data types) | Reuse | `vcvt` / `vcvt_s42*` / `vcvt_f162s4` etc. are all supported on M510 (RFC 4.10) |
| `vector_compute/` | The expdiff `Tuple<float,half>` cases in `test_return_vconv.cpp` (`ExpSubHalfToFloatUsesVexpdif`) | Drop | M510's `FusedExpSubImpl` supports only `Tuple<half,half>` / `Tuple<float,float>` (RFC 4.10) |
| `vector_compute/` | The expdiff `Tuple<float,float>` / `Tuple<half,half>` cases in `test_return_vconv.cpp` (`ExpSubUsesVexpdif`) | Reuse | `vexpdif` is supported on M510 (RFC 4.10) |
| `vector_datamove/` | See the full 3510 `vector_datamove/` | Reuse | The top-level dispatch chain now has a 5102 branch (RFC 4.9/4.10) |
| `reg_compute/` | See the corresponding 3510 `reg_compute/` | Reuse | 18 non-cube dispatch headers now have a 5102 branch (RFC 4.10) |
| `reg_compute/reg_convert/` | Cases related to `vcvt_s42*` / `vcvt_f162s4` / `UNPK4_B8` | Reuse | **4.10 correction**: M510 supports these instructions (the original Drop judgement was wrong) |
| `reg_compute/loadalign/`, `storealign/` | `UNPK4_B8` unpack/pack related cases | Reuse | **4.10 correction**: M510 `CheckLoadDist` supports `DIST_UNPACK4_B8` |
| `reg_compute/loadunalign/`, `storeunalign/` | Unaligned load/store related cases | Reuse | **4.10 correction**: `vldus`/`vstus`/`vstar` etc. are all supported on M510 |
| `reg_compute/reg_fused/` | Cases related to `asc_exp_sub_half2float` | Drop | `Tuple<float,half>` is not supported on M510 (RFC 4.10, guard retained) |
| `reg_compute/reg_fused/` | Cases related to `asc_exp_sub(float,float)` | Reuse | `Tuple<float,float>` is supported on M510 (RFC 4.10, guard removed) |
| `reg_compute/reg_permute_sel/`, `reg_gather/` | `vintlv`/`vselr` related select/interleave/gather cases | Reuse | **4.10 correction**: supported on M510 (the original Drop judgement was wrong) |
| debug (VF-debug / ring-buffer) | `asc_vf_debug_publish`, `get_ring_buf_*`, etc. | Drop (known gap) | `npu_arch_5102/asc_debug_utils_impl.h` is a 105-line stub and needs to be completed later (RFC 4.10) |

## 4. Cases Already Landed in This Directory (can be cloned directly to verify reuse)

- `atomic/test_set_atomic_add_int16.cpp` - Reuse (3510 clone)
- `sync/test_asc_sync_block_wait.cpp` - Reuse (3510 clone)
- `sys_var/test_get_block_idx.cpp` - Reuse (3510 clone)
- `cube_compute/test_mmad.cpp` - Reuse (3510 clone, verifies the shared cube subset)
- `vector_compute/test_return_vconv.cpp` - Reuse (3510 clone; only the expdiff `Tuple<float,half>` case needs to be removed, i.e. `ExpSubHalfToFloatUsesVexpdif`)
- `vector_compute/test_exp_sub_m510_policy.cpp` - Adapt placeholder (backfilled per 4.10 for the expdiff type-combination difference)
- When cloning 3510 cases into `reg_compute/reg_convert/`, `loadalign/`, `storealign/`, `loadunalign/`, `storeunalign/`, `reg_permute_sel/`, `reg_gather/`, `reg_fused/`, **there is no need to strip fp4/fp8/s4 functions** (RFC 4.10 confirmed M510 supports these instructions and the guards were shrunk); the only function to strip is `asc_exp_sub_half2float` in `reg_fused` (`Tuple<float,half>` is unsupported).

## 5. How to Integrate and Run

Depends on the CMake patch in RFC 7.1 (adding `ascendm510_AIC` / `ascendm510_AIV` to `tests/api/c_api/CMakeLists.txt`), and on the external `tikicpulib::ascendm510` CPU model library being available.

```bash
# In the asc-devkit build directory, build and run the M510 capi unit tests
# (requires tikicpulib::ascendm510 to be available)
cmake --build . --target ascendc_ut_c_api_ascendm510_AIC ascendc_ut_c_api_ascendm510_AIV
ctest -R ascendc_ut_c_api_ascendm510
```

## 6. Conventions for Writing New Cases

1. To reuse a 3510 case: copy `npu_arch_3510/<cat>/<name>.cpp` to `npu_arch_5102/<cat>/`; no modification is needed if the API/instruction set is identical.
2. Stub the underlying instructions with `MOCKER_CPP(<instr>, <sig>).times(1).will(invoke(<stub>))`, and assert the parameters and return types of the `asc_*` wrapper functions.
3. Distinguish AIC/AIV via `g_coreType = C_API_AIC_TYPE / C_API_AIV_TYPE` (set AIC for cube cases, AIV for vector cases).
4. Name M510-specific difference cases with the `_m510` suffix, and note the source of the difference in the file header comment (referencing `impl/basic_api/dav_m510/...` or an RFC section).
