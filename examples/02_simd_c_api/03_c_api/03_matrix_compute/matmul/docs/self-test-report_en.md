# MatMul Self-Test Report

## 1. Test Object

```text
C[30,70] = A[30,40] @ B[40,70] + bias[70]
dtype = FP32
transposeA = false
transposeB = false
numBlocks = 1
NPU arch = dav-3510
```

## 2. Conclusion

| Verification | Result | Evidence |
| --- | --- | --- |
| Ascend 950 Python/source contract tests | PASS | `Ran 21 tests ... OK` |
| CANN 9.1.0 simulation build and run | PASS | clean build, P01-P08 `summary=8/8 passed` |
| CANN 9.1.0 Ascend 950 build | PASS | clean `dav-3510` NPU build |
| Ascend 950 precision and repeatability | PASS | P01-P08, 10 repetitions per case, `summary=80/80 passed` |
| mssanitizer memcheck | PASS | exit code 0, no out-of-bounds or leak report |
| mssanitizer synccheck | PASS | `No error detected` |
| mssanitizer initcheck | TOOL LIMITATION | reports reads from NZ alignment padding; the official `mmad` sample reports the same behavior |
| msprof P08 | PASS (environment workaround required) | exported successfully; single-block task duration `6.156 us` |

The operator satisfies the task requirements for a single Cube-Core FP32 implementation with bias and no Host-side elementwise computation. CANN 9.0.0-beta.2 does not provide the ordinary Cube C APIs required by this sample, so the supported verification baseline is CANN 9.1.0. This compatibility limitation is tracked in Issue #1404.

## 3. Test Environment

### Ascend 950

- Device: Ascend 950PR validation host, Linux x86_64
- NPU: `Ascend950PR_9579`, Health=OK
- NPU architecture: `dav-3510`
- CANN: `9.1.0`
- Python: `3.x`, with `requirements.txt` installed
- Project directory: this sample directory

Device check:

```bash
source /path/to/Ascend/cann/set_env.sh
python3 -c "import acl; print(acl.get_soc_name())"
# Ascend950PR_9579
```

## 4. Automated Test Coverage

| Category | Coverage |
| --- | --- |
| Data generation | P01-P08, fixed shapes and dtype, exact file lengths, fixed seeds, range `[-100,100]` |
| Semantic cases | bias broadcast, all-zero, boundary values, positive/negative cancellation, sparse B layout, three random inputs |
| Verifier | exact pass, localized large error, wrong length, NaN/Inf, matched ratio, absolute and ULP limits |
| Runner | multiple cases, repeated runs, generation/execution/verification error propagation, stale output cleanup |
| Device contract | `__cube__`, one block, required C APIs, HF32 disabled, fixed shapes |
| Layout contract | B L1/L0B=3,200, `dn2nz`, `asc_copy_l12l0b`, logical Mmad N=70 |
| Memory safety | compile-time capacity assertions for L1/L0A/L0B/L0C/BiasTable |
| Host contract | exact file sizes, no Host elementwise compute loop, ACL cleanup error reporting |

## 5. Precision Standard

The golden output is calculated with NumPy FP64:

```python
golden = src0.astype(np.float64) @ src1.astype(np.float64) + weight.astype(np.float64)
```

The verifier applies the ecosystem standard's mixed criteria:

```text
rtol = 2^-10
atol = 2^-16
matched_ratio >= 0.99
max_abs_error <= 1e-2 or ULP error at the max-absolute-error element <= 32
```

`max_ulp_error` is retained as a diagnostic metric. Because the ULP spacing near zero is small, it can exceed 32 without changing the standard pass decision. All device cases achieved `matched_ratio=1.0`; the ULP error at the maximum-absolute-error element was at most 4.

## 6. Precision Case Results

The following metrics are from the first run of each case on Ascend 950. The ten repetitions of every case produced identical results.

| Case | Construction | matched | max abs error | max ULP | max-abs ULP | Result |
| --- | --- | ---: | ---: | ---: | ---: | --- |
| P01 | A/B all zero, nonzero bias | 1.000000 | 0.000000000e+00 | 0 | 0 | PASS |
| P02 | A/B/bias all zero | 1.000000 | 0.000000000e+00 | 0 | 0 | PASS |
| P03 | `-100/100/0` periodic boundary | 1.000000 | 0.000000000e+00 | 0 | 0 | PASS |
| P04 | paired positive/negative cancellation | 1.000000 | 2.288818359e-03 | 1952 | 300 | PASS |
| P05 | sparse basis vectors in B | 1.000000 | 3.576278687e-05 | 1 | 1 | PASS |
| P06 | random seed `20260804` | 1.000000 | 1.559900981e-02 | 442 | 4 | PASS |
| P07 | random seed `20260805` | 1.000000 | 1.549896409e-02 | 1201 | 4 | PASS |
| P08 | random seed `20260806` | 1.000000 | 1.425332281e-02 | 335 | 4 | PASS |

The complete P01-P08 device log is archived with the acceptance materials as `summary=80/80 passed`.

## 7. Build and Run Evidence

### Simulation

Build from an empty directory with CANN 9.1.0:

```bash
cmake -S . -B build-sim-dn2nz \
  -DCMAKE_ASC_ARCHITECTURES=dav-3510 \
  -DCMAKE_ASC_RUN_MODE=sim
cmake --build build-sim-dn2nz -j
python3 scripts/run_cases.py --binary ./build-sim-dn2nz/demo --repeat 1
```

The simulation run completed with `summary=8/8 passed`.

### Ascend 950 Device

```bash
cmake -S . -B build-npu-dn2nz \
  -DCMAKE_ASC_ARCHITECTURES=dav-3510 \
  -DCMAKE_ASC_RUN_MODE=npu
cmake --build build-npu-dn2nz -j
python3 scripts/run_cases.py --binary ./build-npu-dn2nz/demo --repeat 10
```

The CANN 9.1.0 `dav-3510` NPU build completed successfully. The device run completed with `summary=80/80 passed`.

## 8. Memory and Synchronization Checks

```text
mssanitizer memcheck: exit 0, no out-of-bounds or leak report
mssanitizer synccheck: No error detected
```

`initcheck` reports reads at L1 addresses `0x0`, `0x1400`, and `0x4600` with sizes 320, 6400, and 32 bytes. These addresses belong to the NZ alignment/padding areas for A, B, and bias rather than the logical GM input range. Logical output is stable and correct for P01-P08 and all 80 repeated runs. The official CANN 9.1.0 `mmad` sample reports the same alignment-area reads on the same device, so this is recorded as a tool limitation for C API NZ padding rather than an unconditional pass.

## 9. Profiler Status

The task has no performance threshold. One P08 profile was collected on Ascend 950. The first export attempt failed because the CANN 9.1.0 `msprof` exporter used the system Python library: the dynamic linker bound `_ctypes` to a system `libpython3.12` that lacked `_PyErr_SetLocaleString`.

The raw profile was collected successfully. Export succeeded after placing the matching Python 3.12 library directory first in `LD_LIBRARY_PATH` and explicitly selecting the matching interpreter:

```bash
source /path/to/Ascend/cann/set_env.sh
cd examples/02_simd_c_api/03_c_api/03_matrix_compute/matmul
msprof \
  --output=./profiling-p08 \
  --task-time=on --runtime-api=on --task-memory=on \
  --aic-metrics=PipeUtilization ./build-npu-dn2nz/demo

export LD_LIBRARY_PATH=/opt/buildtools/python-3.12.9/lib:$LD_LIBRARY_PATH
msprof --output=./profiling-p08 \
  --python-path=/opt/buildtools/python-3.12.9/bin/python3.12 \
  --export=on --summary-format=csv
```

The exported `op_summary_*.csv` reports:

| Metric | Value |
| --- | ---: |
| Kernel | `_Z13matmul_customPfS_S_S_` |
| Block Num | 1 |
| Task duration | 6.156 us |
| AI core time | 5.43 us |
| AI core MAC time | 0.308 us |
| MTE2 time | 1.708 us |
| Cube utilization | 3.045% |

The raw profile and exported CSV are kept in `./profiling-p08/`.

## 10. CANN 9.0.0-beta.2 Compatibility Evidence

The Arch 3510 headers bundled with CANN 9.0.0-beta.2 do not declare the ordinary Cube APIs required by this sample:

```text
asc_set_gm2l1_nz_para
asc_copy_gm2l1_nd2nz
asc_copy_l12l0a
asc_copy_l12l0b
asc_copy_l12bt
asc_mmad
asc_copy_l0c2gm
```

Combining the 9.0 compiler with `asc-devkit/master` headers is diagnostic only: P01/P02 run, but nonzero P03 fails. The sample therefore does not claim CANN 9.0.0-beta.2 support. The version-range issue is tracked in [asc-devkit #1404](https://gitcode.com/cann/asc-devkit/issues/1404).

[asc-devkit #1408](https://gitcode.com/cann/asc-devkit/issues/1408) was clarified by the maintainer after this sample was submitted: the 2201 and 3510 `asc_copy_l12l0b_trans` declarations and implementations are isolated by architecture macros, so they do not create an actual conflict for a target architecture. This sample does not call that API; it uses `asc_copy_gm2l1_dn2nz` followed by ordinary `asc_copy_l12l0b`, so #1408 has no impact on the design.

## 11. Reproduction and Delivery Status

This report corresponds to the verification commit for this sample. Before upstream merge, rerun the contract tests, a clean build, and `git diff --check` on the server, then retain the actual commit SHA and CI/device verification links in the PR.
