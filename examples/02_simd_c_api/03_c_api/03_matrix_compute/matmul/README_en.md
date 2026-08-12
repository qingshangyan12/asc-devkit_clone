# Ascend C C API MatMul Sample

This project implements one fixed-shape FP32 MatMul on a single Cube Core:

```text
C[30, 70] = A[30, 40] × B[40, 70] + bias[70]
```

The `__cube__` kernel uses Ascend C C APIs for the complete `GM -> L1 -> L0A/L0B/BT -> Mmad -> L0C -> GM` pipeline. The Host only performs file I/O, ACL memory management, data copies, launch, and synchronization. It performs no elementwise MatMul or bias computation.

## Support

| Item | Value |
| --- | --- |
| Product | Ascend 950PR / Ascend 950DT |
| NPU architecture | `dav-3510` |
| CANN | `>= 9.1.0` |
| Inputs | A `[30,40]`, B `[40,70]`, bias `[1,70]`, FP32 ND |
| Output | C `[30,70]`, FP32 ND |
| Transpose | A=false, B=false |
| Blocks | one Cube Core |
| HF32 | disabled |

The task specification mentions CANN 9.0.0 through 9.1.0, while the current `asc-devkit/master` C API `mmad` sample for Arch 3510 declares `>= 9.1.0`. The headers bundled with CANN `9.0.0-beta.2` do not expose the ordinary Cube APIs. A diagnostic-only combination of the 9.0 compiler and master headers builds and launches, but produces incorrect results for nonzero MatMul, so it is not a supported or valid acceptance environment.

## Local Memory

| Buffer | Aligned elements | Bytes |
| --- | ---: | ---: |
| A L1 / L0A | 1,280 / 1,280 | 5,120 / 5,120 |
| B L1 / L0B | 3,200 / 3,200 | 12,800 / 12,800 |
| bias L1 / BiasTable | 80 / 80 | 320 / 320 |
| C L0C | 2,560 | 10,240 |

Peak L1 usage is 18,240 bytes. Compile-time assertions check both the calculated sizes and Ascend 950 memory capacity limits.

## Prerequisites

```bash
python3 -m venv .venv
source .venv/bin/activate
python3 -m pip install -r requirements.txt
source /path/to/Ascend/cann/set_env.sh
```

Verify that `npu-smi info` reports an Ascend 950 device and that CANN is at least 9.1.0.

## Build

NPU mode:

```bash
cmake -S . -B build -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DCMAKE_ASC_RUN_MODE=npu
cmake --build build -j
```

Simulation mode:

```bash
cmake -S . -B build-sim -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DCMAKE_ASC_RUN_MODE=sim
cmake --build build-sim -j
```

Use separate build directories when switching modes so that CMake cache entries are not reused.

## Run One Case

Run from the project root because the Host reads `./input` and writes `./output`:

```bash
python3 scripts/gen_data.py --case P01 --output-dir input
./build/demo
python3 scripts/verify_result.py output/output.bin input/golden.bin
```

The exact file sizes are: A 4,800 bytes, B 11,200 bytes, bias 280 bytes, output 8,400 bytes, and FP64 golden 16,800 bytes. The Host rejects incorrectly sized inputs.

## Run All Cases

Run P01-P08 ten times each:

```bash
python3 scripts/run_cases.py --binary ./build/demo --repeat 10
```

Select cases with `--cases P01 P05 P08`. The runner removes stale output before every execution and returns nonzero as soon as generation, execution, or verification fails.

## Precision

Golden output is calculated with NumPy FP64. Element matching uses `atol=2^-16`, `rtol=2^-10`, and a required ratio of 0.99. The maximum error must also satisfy either absolute error `<= 1e-2` or ULP error `<= 32`. The verifier reports non-finite values, the worst index, and the corresponding actual and golden values.

## Contract Tests

```bash
python3 -m unittest discover -s tests -v
```

The tests cover all eight deterministic cases, exact file sizes, cancellation, NaN/Inf handling, maximum-error guards, the batch runner, required Cube APIs, one-block launch, layout parameters, Local Memory capacity, and the absence of Host elementwise compute loops.

This is an independent sample and does not modify the existing `mmad` scenarios.

## Usability Feedback

- [asc-devkit #1404: version range mismatch between the MatMul task and C API release](https://gitcode.com/cann/asc-devkit/issues/1404) remains the compatibility issue tracked by this sample.
- [asc-devkit #1408: inconsistent public and implementation signatures for 3510 `asc_copy_l12l0b_trans`](https://gitcode.com/cann/asc-devkit/issues/1408) was clarified by the maintainer: the 2201 and 3510 declarations and implementations are isolated by architecture macros, so there is no actual overload conflict for a target architecture. This sample does not call that API; it uses `asc_copy_gm2l1_dn2nz` and ordinary `asc_copy_l12l0b`, so #1408 has no impact here.
