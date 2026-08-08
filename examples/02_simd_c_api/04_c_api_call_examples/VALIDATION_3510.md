# DAV 3510 Validation Record

Validation environment:

- Target architecture: `dav-3510`
- CANN: `/usr/local/Ascend/cann-9.2.0`
- Compiler: `$ASCEND_HOME_PATH/bin/bisheng`
- Validation date: 2026-08-07

## Scope

The C API documentation marks 327 generated examples as supported on Ascend
950PR/950DT. Two documented APIs are not declared or implemented by the current
repository and installed CANN headers, so their examples cannot be compiled and
are excluded from the runnable set:

- `asc_duplicate_merge`
- `asc_duplicate_scalar_merge`

The final DAV 3510 example set contains 325 examples. All DAV 2201-only examples
were removed; the timeout exceptions are listed below.

## Initial Validation

- Compile: 325 passed, 2 failed as described above.
- Runtime precision: 300 passed, 8 failed, 6 timed out, and 11 reported device
  runtime errors.

The eight initial precision failures were addressed:

- `asc_create_mask`, `asc_get_mask_spr`, and `asc_update_mask`: the Golden data
  incorrectly treated 16-bit and 32-bit values as byte fills.
- `asc_atomic_and`, `asc_atomic_dec`, `asc_atomic_inc`, `asc_atomic_or`, and
  `asc_atomic_xor`: the kernels missed the DCache maintenance required by the
  scalar atomic API contract.

The corrected mask cases and the scalar atomic cases that completed returned
matching Golden data. Atomic timeout cases are covered by the retest below.

The runtime errors were addressed by correcting matrix parameters, completing
VA register setup, using the DAV 3510 mixed-core ratio, initializing source
storage, and launching incompatible asynchronous/synchronous variants in
separate kernels.

## Initial Timeout Cases

The following examples exceeded the 30-second validation limit and were skipped:

- `reg/reg_load/asc_loadalign_brc_datablock_postupdate.asc`
- `reg/reg_load/asc_loadalign_unpack4.asc`
- `reg/reg_load/asc_load.asc`
- `reg/reg_store/asc_storealign_pack_quarter.asc`
- `sync/asc_sync_block_arrive.asc`
- `sync/asc_sync_block_wait.asc`
- `cube_datamove/asc_copy_l0c2ub.asc`
- `cube_datamove/asc_copy_l12ub.asc`
- `vector_datamove/asc_copy_ub2l1.asc`

The `asc_ub_dcci_single` variant also timed out when isolated. The final combined
`asc_dcci` example therefore demonstrates only the four DAV 3510 GM-cache
variants that completed the temporary precision validation.

## Final Minimal Validation

After removing the temporary precision comparison code, all 325 examples were
compiled again for `dav-3510`; 325 passed and none failed.

The corrected matrix, Cube data-move, and intra-sync examples were also run
again in their final minimal form. Seven completed successfully. The following
additional runs exceeded the 30-second limit:

- `cache_ctrl/asc_dcci.asc`
- `scalar_compute/asc_atomic_add.asc`
- `scalar_compute/asc_atomic_and.asc`
- `scalar_compute/asc_atomic_cas.asc`
- `scalar_compute/asc_atomic_dec.asc`
- `vector_compute/asc_transto5hd.asc`

The remaining scalar atomic examples were skipped after the repeated timeout
pattern. The timeout retest below covers all eleven scalar atomic examples; all
of their final kernels use the same DCache maintenance pattern.

## Timeout Retest

The timeout cases were retested on 2026-08-07 with the temporary precision
comparison binaries and a 60-second per-example limit. An initial attempt
overlapped another NPU workload and was discarded. The complete retest was run
after the device became idle. A known-good matrix example was run after every
timeout and always completed successfully, confirming that each timeout was
isolated from the following result.

Four initial timeout cases completed with matching Golden data:

- `reg/reg_load/asc_loadalign_brc_datablock_postupdate.asc` (5 seconds)
- `reg/reg_load/asc_loadalign_unpack4.asc` (6 seconds)
- `reg/reg_load/asc_load.asc` (6 seconds)
- `reg/reg_store/asc_storealign_pack_quarter.asc` (6 seconds)

The following 18 examples still exceeded 60 seconds:

- `sync/asc_sync_block_arrive.asc`
- `sync/asc_sync_block_wait.asc`
- `cube_datamove/asc_copy_l0c2ub.asc`
- `cube_datamove/asc_copy_l12ub.asc`
- `vector_datamove/asc_copy_ub2l1.asc`
- `cache_ctrl/asc_dcci.asc`
- `scalar_compute/asc_atomic_add.asc`
- `scalar_compute/asc_atomic_and.asc`
- `scalar_compute/asc_atomic_cas.asc`
- `scalar_compute/asc_atomic_dec.asc`
- `scalar_compute/asc_atomic_exch.asc`
- `scalar_compute/asc_atomic_inc.asc`
- `scalar_compute/asc_atomic_max.asc`
- `scalar_compute/asc_atomic_min.asc`
- `scalar_compute/asc_atomic_or.asc`
- `scalar_compute/asc_atomic_sub.asc`
- `scalar_compute/asc_atomic_xor.asc`
- `vector_compute/asc_transto5hd.asc`

No retested example returned a precision mismatch or another runtime error.

The original minimal examples removed temporary precision comparisons. The
functional rewrite below supersedes that policy and retains concise Golden
checks for computational and data-layout behavior.

## Functional Rewrite Progress

The functional rewrite replaces marker-only calls with scenarios that expose
the documented effect of each API. Completed batches are compiled and run with
a 30-second timeout before each checkpoint.

The 18 synchronization examples were revalidated on 2026-08-07. All 18
compiled and ran successfully, with no timeout. Local pipeline APIs now guard a
real GM-to-UB, vector-add, and UB-to-GM dependency chain; event APIs pair
producer and consumer pipelines; and inter/subblock APIs copy and compare input
data across their synchronization point. The intra APIs use an AIV-to-AIC
mixed-core visibility scenario. The former `asc_sync_block_arrive` and
`asc_sync_block_wait` timeouts were fixed by replacing the invalid single-AIV
self-pairing with an AIV-arrive/AIC-wait mixed-core scenario. DAV 3510 does not
use `asc_set_ffts_base_addr`; that setter is intentionally unavailable in the
3510 implementation and the product documentation states that Ascend 950 does
not require the FFTS base-address setup.

The five memory-vector-compute examples were revalidated on 2026-08-07. All
five compiled and ran successfully with exact Golden comparisons and no
timeout. `asc_bitsort` validates descending `(score,index)` pairs,
`asc_mrgsort4` merges four pre-sorted queues, and `asc_transpose` checks a
16x16 matrix transpose for both regular and synchronous variants. The
`asc_set_va_reg` example rebinds VA0/VA1 to a second destination and verifies
that both address lists receive the transpose. The former
`asc_transto5hd` timeout is resolved: separate aligned b8, b16, and b32
workspaces now validate all six regular/synchronous entry points, including
the low/high half behavior of the b8 variant.

The 18 vector data-movement examples were revalidated on 2026-08-07. All 18
compiled and ran successfully with exact byte comparisons and no timeout.
Continuous and aligned copies preserve two independent regular/synchronous
results; the GM-to-UB and UB-to-GM loop-register examples expose a 2x2 strided
block selection; and the padding setters show two 24-byte payloads surrounded
by four fixed-value bytes. The N-D examples demonstrate a 3x4 transpose and a
two-dimensional constant border. They explicitly clear the persistent N-D
pad-count register before a non-padding transfer, after a whole-batch rerun
showed that a preceding padding kernel otherwise changes the next standalone
example. The former `asc_copy_ub2l1` timeout is resolved by a complete
UB-to-L1-to-UB round trip: both AIV subblocks participate in the group arrive,
the AIC waits and copies L1 back to AIV0, and an intra-core signal releases the
final GM write.

The six Cube-only system-variable setters were revalidated on 2026-08-07. All
six compiled and ran successfully, with no timeout:

- `asc_set_gm2l1_nz_para`: ND-to-NZ configuration followed by a 16x16 MMAD and
  an identity-matrix Golden comparison.
- `asc_set_l0c2gm_channel_para`: NZ-to-DN conversion of a non-symmetric 16x32
  matrix, including the second source fragment controlled by the channel
  stride.
- `asc_set_l0c2gm_relu_alpha`: Scalar ReLU alpha 0.5 changes an FP16 `-1.0`
  diagonal to `-0.5`.
- `asc_set_l12l0_padding_val`: a zero L1 feature map with one top-padding row
  produces a row of FP16 `1.0` values through 3D L1-to-L0B movement and MMAD.
- `asc_set_l3d_rpt_b`: one L1-to-L0B call repeats in the K direction and moves
  both 16-column fragments.
- `asc_set_l0c2gm_quant_post`: 257 alternating/final register writes are timed
  with the system cycle counter.

`asc_set_l0c2gm_quant_post` cannot currently be validated through a numeric
post-quantization result using the public DAV 3510 C API. The paired
`asc_copy_l0c2gm`, `asc_copy_l0c2l1`, and `asc_copy_l0c2ub` documentation marks
all `quant_post` arguments as invalid. Three board attempts used
`SHIFT2S8_POST`, `QF162B8_POST`, and an int32-to-FP16-to-int8 two-stage path;
each produced only the pre-quantized FP16 `0x3c00`. The final example therefore
validates the documented register encoding and measurable setter execution
without claiming an unsupported numeric effect.

The 12 Cube compute examples were revalidated on 2026-08-07. All 12 compiled
and ran successfully with a 30-second limit, with no timeout:

- `asc_mmad` compares a real 16x32 by 32x16 int8 matrix product against a CPU
  Golden, while `asc_mmad_mx` validates FP8 E4M3 inputs with unit E8M0 scales.
- `asc_enable_fp8` and `asc_enable_hif8` use the raw encodings `0x38` and
  `0x08` for 1.0 and verify that the selected 8bit MMAD mode produces an
  identity matrix.
- `asc_enable_hf32` exposes the loss from rounding 1.0003 to HF32, and
  `asc_set_fp32_mode` shows the full-precision result after restoring FP32.
- `asc_set_hf32_round_mode` and `asc_enable_hf32_trans` run the two modes in
  separate kernel launches. A value exactly halfway above 1.0 rounds to
  1.0009765625 in mode 1/NEAREST_ZERO and to 1.0 in
  mode 0/NEAREST_EVEN.
- `asc_set_l0c2gm_config` loads per-channel scale 2.0 into FB and checks that
  Fixpipe quantizes an FP32 diagonal of 3.0 to signed int8 6.
- `asc_set_l0c2gm_nz2nd` converts the MMAD result from the internal NZ layout
  to a row-major ND identity matrix.
- `asc_set_mmad_direction_m` and `asc_set_mmad_direction_n` print launch
  timings and verify that both fragment-generation orders preserve the same
  matrix values.

The initial two-mode 8bit and HF32 rounding prototypes exposed that control
register changes in the same in-flight matrix context can be observed by the
wrong MMAD. The final 8bit examples set one mode per kernel, and the HF32
rounding comparisons use separate launches so each matrix operation observes
its intended register value.

The first eight Cube data-movement examples were revalidated on 2026-08-07.
All eight compiled and ran successfully with a 30-second limit, with no
timeout. `asc_copy_gm2l1`, `asc_copy_gm2l1_align`, and `asc_copy_l12ub`
perform complete GM-to-L1-to-UB-to-GM round trips for their regular and
synchronous variants. The three loop-register examples select two or four
non-contiguous 32-byte source blocks and expose their packed L1 layout.
`asc_set_gm2l1_pad` produces eight `0xa5` bytes on each side of a 16-byte
payload, and both `asc_fill_l1` variants initialize 64 float values to 1.0.

The next nine Cube data-movement examples were revalidated on 2026-08-07.
All nine compiled and ran successfully with exact Golden comparisons and no
timeout. The regular/synchronous `asc_copy_gm2l1_nd2nz`, L1-to-L0A,
L1-to-L0B, transposed L1-to-L0B, and L0C-to-GM paths each carry real int8
matrix operands through MMAD and produce an int32 diagonal of 2. The DN-to-NZ
and two L1-to-L0 MX copies carry unit E8M0 scales into FP8 E4M3 MMAD and
produce a float identity matrix. The two L1-to-FB variants load a per-channel
scale of 2 and quantize a float diagonal of 3 to signed int8 6.

The final nine Cube data-movement examples were revalidated on 2026-08-07.
All nine compiled and ran successfully with exact Golden comparisons and no
timeout. L0C-to-L1 and L0C-to-UB convert a real 16x64 float MMAD result to
int4; both regular/synchronous variants produce 512 bytes of packed `0x33`.
L1-to-Bias-Table initializes MMAD from the Bias vector 1..16 for both copy
variants. Leaky ReLU alpha 0.5 changes -1.0 to FP16 -0.5, while scalar
prequant scale 2 changes float 3.0 to signed int8 6. The FeatureMap A/B and
padding setters turn one padded row into 16 values of 1.0, and the Load3D
repeat setter loads positive and negative identity fragments into consecutive
height blocks.

## Full Functional Validation

A clean full-set compile on 2026-08-07 passed all 325 examples for `dav-3510`.
The first continuous runtime pass used a 30-second timeout per example and
completed with 324 passes, one precision failure, and no timeout. The only
failure was `simd_atomic/asc_set_atomic_add.asc`.

The failure was caused by reading an asynchronous GM-to-UB result from another
pipeline before the transfer had reliably completed. Repeated standalone runs
therefore observed different stale UB values. The atomic examples now use
`asc_copy_gm2ub_align_sync` before consuming their local input. The primary
`asc_set_atomic_add` example also follows the documented functional sequence:
write `input0` to GM, wait for MTE3, enable float atomic add, and write
`input1` to the same address. Its output is compared with `input0 + input1`.

After the correction, all nine SIMD atomic examples compiled and ran
independently with exact Golden comparisons under the same 30-second timeout.
There were no atomic timeouts to retest.

The second continuous full-set runtime pass then completed with all 325
examples passing: no precision failure, runtime error, or timeout. Every
historical timeout listed above also completed within the final 30-second
per-example limit. Because the final timeout list was empty, no separate
60-second timeout retest was required.

## asc_duplicate Highest-Position Validation

The `asc_duplicate` example was extended and revalidated on 2026-08-08. The
public C API exposes only the lowest-position, zeroing form, so the two
highest-position cases directly call the compiler `vdup` intrinsic with
`POS_HIGHEST` and either `MODE_ZEROING` or `MODE_MERGING`.

The source vector contains 1 through 64, the first four destination lanes are
selected by the mask, and every destination lane starts at -7. The three
observed results exactly matched their Golden data:

- Lowest plus zeroing: `1 1 1 1 0 0 0 0 ...`
- Highest plus zeroing: `64 64 64 64 0 0 0 0 ...`
- Highest plus merging: `64 64 64 64 -7 -7 -7 -7 ...`

This confirms that `POS_HIGHEST` broadcasts the highest vector element on
DAV 3510. It also confirms that zeroing clears unselected destination lanes,
while merging preserves their original values. Both `asc_duplicate.asc` and
the regenerated `asc_duplicate_scalar.asc` compiled and ran successfully with
the 30-second runtime timeout.
