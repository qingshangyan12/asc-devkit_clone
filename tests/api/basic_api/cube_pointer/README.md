# Attachment regression

Source the CANN 9.2 environment, set `CUBE_ARCHIVE_ROOT` to the extracted task
`test-cases` directory and `CUBE_TEST_OUTPUT` to an absolute output directory, then run:

```bash
bash run_archive.sh fixpipe_l0c2gm 5
CUBE_FIX_BIAS=ON CUBE_TEST_OUTPUT=/tmp/bias-fixed bash run_archive.sh 'mmad' 3
```

`CUBE_ARCHIVE_ROOT`
selects the extracted task `test-cases` directory. The original archive remains
unchanged. CMake builds a copy with these narrowly scoped attachment corrections:

- Escape the embedded comment terminator in the introductory note.
- Use declared TAL0Type/TBL0Type instead of undeclared T in the MX load example.
- Use input T rather than accumulator U in the Load3DV2 pointer casts.
- Optional `CUBE_FIX_BIAS=ON` preserves C2 address space with `__biasbuf__`.

`CUBE_TENSOR_BASELINE=ON` converts simple pointer expressions back to Tensor
operands in the build copy, to distinguish attachment issues from pointer APIs.
The generator and verifier are the unmodified scripts supplied in the task.
Each run has a 60-second timeout. Build, generation, run and golden comparison
logs remain under the selected output directory. The dependency file must name
the local repository's kernel_operator.h. The two dav-2201-only sample projects
are skipped on this 950 runner.

For two-core Tensor/pointer/mixed SyncAll and IBSet/IBWait:

```bash
cmake -S . -B /tmp/cube-sync -DCUBE_SYNC_TEST=ON
cmake --build /tmp/cube-sync -j2
timeout --kill-after=5s 30s /tmp/cube-sync/sync_test
```

The synchronization test launches exactly two vector cores, executes matching
barriers twice, then one producer/consumer event handshake. It checks both GM
counters after synchronization. No intentionally deadlocking kernel is run.

## Baseline And Evidence

Without `SAMPLE_DIR`, CMake builds `cube_pointer_validation`. `bash run.sh <output>`
runs 18 FP16/BF16, 16/32/64, Tensor/pointer/mixed cases. `bash negative.sh <output>`
checks five invalid operand compilations; `bash check_binary.sh <baseline-output>`
compares device machine code. `matrix_compile.asc` retains matrix compile coverage.
The official repository generator creates kernel_tiling.h inside each build tree.

Recorded validation: 48 applicable attachment scenarios passed after the documented
attachment corrections; 18 baseline cases matched the independent CPU double golden,
with identical device machine code across modes. Eight further synchronization runs
covered Determine APIs and SyncAll/IB handshakes across Tensor, pointer and both mixed
forms. The standalone sync target above covers three of those operand modes.

Local investigation evidence: [attachment report](../../../../../cube-basic-api/attachment-regression.md),
[baseline report](../../../../../cube-basic-api/3.2-test-report.md), and
[eight synchronization runs](../../../../../cube-basic-api/tmp/sync-runtime/results.md).
These links refer to the adjacent local investigation workspace, not shipped fixtures.

The initial FP4 MX failure came from missing `en_dtypes`: the supplied generator silently
fell back to uint8 and produced invalid FP4 inputs/golden. Install `en_dtypes` and
`ml_dtypes` before running; passing reruns used the correct dtypes. The original bias
cast was also incorrect and requires the explicit opt-in above. Two attachment projects
are dav-2201-only; the int4 Mmad scenario is unsupported by the 950 LoadData2DV2 baseline.
Passing these representative tests does not establish all 61 overloads or performance
for all 48 attachment scenarios.
