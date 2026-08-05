# ELU Sample

## Overview

This sample implements element-wise ELU with the Ascend C register C API:

```text
y = scale * x                                      , x > 0
y = alpha * scale * (exp(x * input_scale) - 1)    , x <= 0
```

It supports dense contiguous `float32` and `float16` inputs. All three scalars remain runtime
`float` arguments. Half inputs are widened to float for the entire computation and rounded back to
half with RN. The computation uses Vector Cores only; the Host manages partitioning, transfers, and
the kernel lifecycle.

## Supported Products

| Product | CANN version |
| --- | --- |
| Ascend 950PR / Ascend 950DT | CANN 9.0.0 to 9.1.0 |

## Directory Structure

```text
elu/
├── scripts/
│   ├── gen_data.py
│   ├── gen_cases.py
│   ├── run_precision_cases.sh
│   ├── run_perf_cases.sh
│   └── summarize_perf.py
├── CMakeLists.txt
├── data_utils.h
├── elu.asc
├── README.md
└── README_en.md
```

## Implementation Notes

- Dense ND tensors are flattened by `numel`; the element count uses `uint32_t`, and empty inputs are
  a Host no-op.
- Each block loops over 2048-element input/output UB tiles. Unaligned tails and inputs spanning
  multiple tiles are supported.
- For `|x * input_scale| < 0.1`, float32 uses a fifth-order and float16 a fourth-order Horner
  polynomial for `expm1`; other values use `asc_exp(z) - 1`.
- `asc_select` isolates the positive and negative candidates before the common multiplication by
  `scale`.
- `src` and `dst` must be non-overlapping contiguous addresses. This sample has no in-place or
  strided interface.

## Build and Run

After setting up the CANN environment, run the following commands in this directory:

```bash
source ${install_path}/cann/set_env.sh
cmake -S . -B build -DCMAKE_ASC_ARCHITECTURES=dav-3510
cmake --build build -j
python3 scripts/gen_data.py --dtype float32 --length 2048
./build/demo float32 2048 1.0 1.0 1.0 input/input_x.bin output/golden.bin
```

The defaults of `gen_data.py` match the no-argument mode of `./build/demo`, so the final command can
also be shortened to `./build/demo`.

Command-line syntax:

```text
demo <float32|float16> <length> <alpha> <scale> <input_scale> <input.bin> <golden.bin>
```

Golden files are always float32. The executable reports `matched_ratio` and `max_abs_error` using
the open-source ecosystem operator precision criteria. Generate and run the complete precision matrix:

```bash
python3 scripts/gen_cases.py --output-root cases
bash scripts/run_precision_cases.sh ./build/demo cases/precision precision-results
```

Case generation requires Python 3 with NumPy. When multiple Python installations are present, explicitly
use an interpreter that can successfully `import numpy`. Case execution and result summarization use only
Bash and the Python standard library.

The results are stored in `precision-results/runner.log`, per-case logs, and
`precision_summary.csv`. The results directory must not already exist, which prevents accidental
evidence overwrite. A passing run ends with:

```text
PRECISION_SUMMARY passed=54 failed=0 total=54
```

The matrix covers both required dtypes and lengths `1/32/1024/2048`, plus empty inputs, unaligned
tails, runtime coefficient combinations, FLOAT-scalar preservation, unselected-branch isolation,
non-finite attributes, and NaN/±Inf tensor inputs. For both dtypes, lengths `131072` and `1000001`
execute 2 and 9 UB tiles per block, respectively, covering the second and later
GM-to-UB-to-Vector-to-GM iterations and their cross-iteration synchronization.

Precision case list:

| Group | Cases | Coverage |
| --- | ---: | --- |
| shape | 26 | Both dtypes × `1/31/32/33/255/1023/1024/2048/4096/5000/131072/1000001/0` |
| common coefficients | 16 | SELU, distinct scalars, near-zero, saturation, negative and zero scalars |
| dtype contracts | 8 | FLOAT scalar preservation, premultiply isolation, non-finite attributes, overflow |
| non-finite input | 4 | Separate NaN and ±Inf tensor-input cases for both dtypes |

### Performance Collection

The task specifies no performance threshold. The performance runner first checks correctness and then
profiles the formal `demo` binary for both dtypes at
`32/256/1024/2048/4096/16384/65536`:

```bash
ELU_PERF_REPEATS=5 bash scripts/run_perf_cases.sh \
  ./build/demo cases/performance performance-results
```

`ELU_PERF_REPEATS` defaults to 1. The runner retains every profiler log and raw `op_summary.csv`,
then writes `performance_raw.csv` and `performance_summary.csv`. The summary records
`Task Duration(us)`, `aiv_time(us)`, `aiv_total_cycles`, `Block Num`, and `Task Type`; it also checks
for pure `AI_VECTOR_CORE`, zero Cube/AIC activity, and the expected launch plan. Numbers produced by
different metrics, code, devices, CANN versions, or collection strategies are not directly comparable
performance improvements.

Use a separate build directory for NPU simulation mode:

```bash
cmake -S . -B build-sim -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-3510
cmake --build build-sim -j
```
