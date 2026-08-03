# MatMul C API Sample

## Overview

This sample implements a fixed-shape, single-core FP32 matrix multiplication with bias using Ascend C C APIs:

```text
C[30, 70] = A[30, 40] x B[40, 70] + Bias[1, 70]
```

All tensors use `float` and row-major ND layout in Global Memory. A single `__cube__` kernel performs all device-side data movement and computation. The Host only handles file I/O, memory management, kernel launch, and result copy-back.

The pipeline is:

1. `asc_copy_gm2l1_nd2nz` for GM-to-L1 ND-to-Nz conversion;
2. `asc_copy_l12l0a` for A;
3. `asc_copy_l12l0b_transpose` for non-transposed B and L0B Zn layout;
4. `asc_copy_l12bt` for BiasTable initialization;
5. `asc_mmad` for Cube matrix multiply-add;
6. `asc_copy_l0c2gm` for L0C-to-GM Nz-to-ND conversion.

The sample explicitly manages non-aligned tails: M is padded from 30 to 32, B is stored as `[48,80]`, and L0C is `[32,80]`. Mmad consumes the aligned `right_width=80`, while Fixpipe writes only the valid `[30,70]` output.

HF32 is intentionally disabled to prioritize FLOAT accuracy. The reproducible software baseline is CANN 9.1.0 on Ascend 950PR/Ascend 950DT (`dav-3510`).

## One-command build and validation

Run the following command from the sample directory to build the executable and execute all six precision cases:

```bash
./run.sh
```

`run.sh` configures and builds `matmul.asc`, generates FP32 inputs and an FP64 CPU golden result, launches the single-core Cube kernel, verifies the result, and repeats the process for all six cases. It exits immediately with a nonzero status if compilation, execution, or verification fails. A successful run ends with:

```text
Cases: 6/6 passed
Overall result: PASS
```

The final table also shows the matched ratio, maximum absolute error, effective limit, and PASS/FAIL result for each case, followed by the architecture, run mode, header source, build directory, and report directory.

The default build directory is `build/`. Case manifests, NPU outputs, golden files, and strict JSON reports are written to `output/cases/`.

Use the CANN installation headers in the formal CANN 9.1.0 validation environment:

```bash
USE_REPO_HEADERS=0 ./run.sh
```

The current CANN 9.0.0-beta.2 environment does not declare the standard matrix C APIs, so `USE_REPO_HEADERS=1` is the default for development validation with the current asc-devkit repository headers.

| Environment variable | Default | Description |
| --- | --- | --- |
| `BUILD_DIR` | `./build` | CMake build directory |
| `RUN_DIR` | Sample directory | Input, output, and report root |
| `JOBS` | `2` | Parallel build jobs |
| `SEED` | `20260803` | Seed of the first case; subsequent cases increment it by one |
| `ARCH` | `dav-3510` | NPU architecture |
| `ASC_RUN_MODE` | `npu` | ASC run mode |
| `USE_REPO_HEADERS` | `1` | Whether to add the asc-devkit repository headers |
| `CANN_ENV` | Unset | Optional path to the CANN environment script |

### Run one case manually

```bash
source ${install_path}/cann/set_env.sh

mkdir -p build
cd build
cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..
make -j
cd ..

python3 scripts/gen_data.py --case uniform --seed 20260803
./build/demo
python3 scripts/verify_result.py output/output.bin output/golden.bin
```

For simulator mode, use an independent build directory: `ASC_RUN_MODE=sim BUILD_DIR=build_sim ./run.sh`.

The verifier follows the opbase FLOAT32 standard: `rtol=2^-10`, `atol=2^-16`, matched ratio at least 0.99, and maximum absolute error no greater than `max(1e-2, 32 ULP)`. It rejects files with an unexpected size and non-finite output values. Failure reports remain strict JSON by representing unavailable numeric fields as `null`.

All six cases passed on an Ascend950PR device on August 3, 2026, with a 100% matched ratio. The dedicated `non_aligned_tail` case touches `A[29,39]`, `B[39,69]`, and `Bias[69]` and has zero error. The validation used the CANN 9.0.0-beta.2 compiler/runtime plus the current asc-devkit master headers because the installed 9.0 headers do not expose the standard C APIs. A clean CANN 9.1.0 rebuild, a complete rerun, and screenshots from that environment are required before formal acceptance.

The CANN version-range ambiguity found during development is tracked in [asc-devkit issue #1404](https://gitcode.com/cann/asc-devkit/issues/1404).

See [SELF_TEST_REPORT.md](./SELF_TEST_REPORT.md) for the hardware validation record.
