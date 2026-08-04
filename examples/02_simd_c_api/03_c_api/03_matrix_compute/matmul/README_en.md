# MatMul C API Sample

## Function

This sample implements fixed-shape FP32 matrix multiplication with bias on one Cube Core:

```text
C[30, 70] = A[30, 40] × B[40, 70] + Bias[1, 70]
```

| Parameter | Type | Shape | GM layout | Transposed |
| --- | --- | --- | --- | --- |
| `src0` / A | float32 | `[30,40]` | ND | No |
| `src1` / B | float32 | `[40,70]` | ND | No |
| `weight` / Bias | float32 | `[1,70]` | ND | N/A |
| `dst` / C | float32 | `[30,70]` | ND | N/A |

The kernel always launches one Cube Core. The Host only performs file I/O, device memory management, kernel launch, and result copy-back.

## Requirements

| Item | Requirement |
| --- | --- |
| Hardware | Ascend 950PR / Ascend 950DT |
| NPU architecture | `dav-3510` |
| CANN | 9.1.0 |
| Python | 3.8+ with NumPy |

Load the CANN environment if required:

```bash
source ${install_path}/cann/set_env.sh
```

## One-command build and test

```bash
cd examples/02_simd_c_api/03_c_api/03_matrix_compute/matmul
USE_REPO_HEADERS=0 ./run.sh
```

The script builds `matmul.asc`, generates inputs and an FP64 CPU golden result, runs all six precision cases on one Cube Core, verifies the outputs, and prints a final summary. Any failure returns a nonzero exit status. A successful run ends with:

```text
Cases: 6/6 passed
Overall result: PASS
```

For development validation with the current asc-devkit repository headers, run `./run.sh`.

## Manual build and test

```bash
cmake -S . -B build \
  -DCMAKE_ASC_ARCHITECTURES=dav-3510 \
  -DCMAKE_ASC_RUN_MODE=npu \
  -DCMAKE_ASC_FLAGS=

cmake --build build -j2

python3 scripts/run_cases.py \
  --demo build/demo \
  --root . \
  --seed 20260803
```

## Test cases

### Precision cases

| Case | Purpose | Input range |
| --- | --- | --- |
| `uniform` | Uniform random inputs | `[-100,100]` |
| `normal` | Normal distribution and clipping boundaries | `[-100,100]` |
| `boundary` | ±100, ±1, ±0.5, zero, and cancellation patterns | `[-100,100]` |
| `bias_only` | Validate Bias broadcast with zero A/B | `[-100,100]` |
| `non_aligned_tail` | Validate the last valid M/K/N coordinates | `[-2,3.25]` |
| `zeros` | All-zero boundary case | `0` |

The verifier requires `rtol=2^-10`, `atol=2^-16`, `matched_ratio>=0.99`, `max_abs_error<=max(1e-2, 32 ULP)`, and no NaN or Inf values.

### Performance cases

The task specifies no performance requirement, so no performance case is defined and performance data is not applicable.

## Outputs

The test generates the executable under `build/` and per-case inputs, outputs, golden files, manifests, and JSON reports under `input/` and `output/cases/`. These generated directories are not committed to Git.

See [SELF_TEST_REPORT.md](./SELF_TEST_REPORT.md) for test parameters and results. The CANN version-range usability issue is tracked in [asc-devkit #1404](https://gitcode.com/cann/asc-devkit/issues/1404).
