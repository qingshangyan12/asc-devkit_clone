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

Without `SAMPLE_DIR`, CMake builds `cube_pointer_validation` and
`cube_pointer_compile_probe`. `bash run.sh <output>` runs 18 FP16/BF16, 16/32/64,
Tensor/pointer/mixed cases. `bash negative.sh <output>` checks five invalid operand
compilations; `bash check_binary.sh <baseline-output>` compares device machine code.
`matrix_compile.asc` instantiates 950-applicable pointer overloads on Cube and Vector
cores, including LoadData/Mmad/Fixpipe/Dump/SetAddrWithOffset and SyncAll/IB/Determine.
`MmadWithSparse` / `LoadDataWithSparse` pointer overloads keep the original dav-2201
gate; 950 runners skip those archive projects. The official repository generator
creates kernel_tiling.h inside each build tree.

The reproducible acceptance record, including which requirements are **not** yet
verified, lives in [ACCEPTANCE.md](ACCEPTANCE.md). Read it before reusing any older
claim: the attachment regression needed narrowly scoped fixes to the attachment
sources, and the `mmad_unitflag` sample fails against its original golden.
