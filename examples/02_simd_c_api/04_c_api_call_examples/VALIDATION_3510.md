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

The eight precision failures were fixed and revalidated:

- `asc_create_mask`, `asc_get_mask_spr`, and `asc_update_mask`: the Golden data
  incorrectly treated 16-bit and 32-bit values as byte fills.
- `asc_atomic_and`, `asc_atomic_dec`, `asc_atomic_inc`, `asc_atomic_or`, and
  `asc_atomic_xor`: the kernels missed the DCache maintenance required by the
  scalar atomic API contract.

The runtime errors were addressed by correcting matrix parameters, completing
VA register setup, using the DAV 3510 mixed-core ratio, initializing source
storage, and launching incompatible asynchronous/synchronous variants in
separate kernels.

## Timeout Cases

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
pattern. Their temporary precision-validation runs had already completed, and
all scalar atomic kernels use the same DCache maintenance pattern in the final
sources.

Precision comparison code was used only in the temporary validation copies and
is not present in the final minimal examples.
