# MMAD Direction C API Sample

## Overview

This sample shows how to select the MMAD result-fractal production direction according to the Fixpipe output order when UnitFlag is enabled on Ascend 950PR/Ascend 950DT. It provides a recommended example and a counterexample that changes only the direction for each workload:

1. float Nz output without ChannelMerge: use `asc_set_mmad_direction_m()`;
2. B4 quantized Nz output with automatic ChannelMerge: use `asc_set_mmad_direction_n()`;
3. float Nz2ND on-the-fly format conversion: use `asc_set_mmad_direction_n()`.

The direction APIs do not change the matrix multiplication, numerical result, or output format. They change only the production order of 16 x 16 result fractals in L0C Buffer. Matching this order to the Fixpipe read order reduces pipeline stalls when UnitFlag overlaps MMAD and Fixpipe.

## Supported Products and CANN Versions

| Product | NPU Architecture | CANN Version |
| --- | --- | --- |
| Ascend 950PR/Ascend 950DT | `dav-3510` | CANN version matching the current asc-devkit repository |

The sample supports NPU and NPU simulation modes and does not support CPU debugging. API availability follows the current asc-devkit repository. Upgrade an older installed CANN package before building this sample.

## Directory Structure

```text
01_mmad_direction
├── performance
│   ├── 3510_sample_raw.csv       # 42 records from the historical performance test
│   └── 3510_sample_summary.csv   # Historical means and standard deviations
├── scripts
│   ├── gen_data.py               # Generates random inputs and golden data
│   └── verify_result.py          # Verifies float or packed B4 output
├── CMakeLists.txt
├── data_utils.h
├── mmad_direction.asc
├── README.md
└── README_en.md
```

## Sample Description

The sample computes `C = A x B`. A and B are half and the L0C accumulator is float. `gen_data.py` uses a fixed seed to generate sparse signed values from `-1`, `0`, and `1`. B is transposed before it is written so K is contiguous in GM. The non-constant inputs validate matrix computation, Nz fractal ordering, B4 signs, saturation, and two-element packing.

| Data | I/O | Type | Logical Shape | File and GM Layout |
| --- | --- | --- | --- | --- |
| A | Input | half | `[M, K]` | ND stored as `[M, K]` |
| B | Input | half | `[K, N]` | Transposed ND stored as `[N, K]` |
| C, scenarios 1/2 | Output | float | `[M, N]` | Nz stored as `[N/16, M/16, 16, 16]` |
| C, scenarios 3/4 | Output | int4b_t | `[M, N]` | Nz stored as `[N/64, M/16, 16, 64]`, two signed B4 values per byte |
| C, scenarios 5/6 | Output | float | `[M, N]` | ND stored as `[M, N]` |

The Kernel executes in this order:

1. copy A and B from GM to L1 Buffer and convert ND to Nz;
2. load A and B from L1 Buffer to L0A Buffer and L0B Buffer;
3. preset scalar quantization or Nz2ND parameters when required;
4. configure the MMAD direction immediately before `asc_mmad`;
5. execute one `asc_mmad` and one `asc_copy_l0c2gm` with matching UnitFlag update modes.

## Scenarios

Each pair uses identical input, shape, MMAD parameters, quantization parameters, and output parameters. Only the direction differs.

| `SCENARIO_NUM` | Role | Output Workload | `[M, N, K]` | MMAD Direction |
| ---: | --- | --- | --- | --- |
| 1 | Recommended | float Nz + UnitFlag | `[256, 256, 128]` | `asc_set_mmad_direction_m()` |
| 2 | Counterexample | float Nz + UnitFlag | `[256, 256, 128]` | `asc_set_mmad_direction_n()` |
| 3 | Recommended | B4 ChannelMerge + UnitFlag | `[192, 64, 128]` | `asc_set_mmad_direction_n()` |
| 4 | Counterexample | B4 ChannelMerge + UnitFlag | `[192, 64, 128]` | `asc_set_mmad_direction_m()` |
| 5 | Recommended | float Nz2ND + UnitFlag | `[256, 256, 128]` | `asc_set_mmad_direction_n()` |
| 6 | Counterexample | float Nz2ND + UnitFlag | `[256, 256, 128]` | `asc_set_mmad_direction_m()` |

### Float Nz Output

Scenarios 1 and 2 use float output, `NoQuant`, and no Nz2ND/Nz2DN conversion. The output remains Nz and ChannelMerge is not enabled. Normal Nz output reads L0C in the M direction, so scenario 1 uses `asc_set_mmad_direction_m()`.

### B4 ChannelMerge

Scenarios 3 and 4 convert float L0C data to `int4b_t` with `QF322S4_PRE` scalar quantization and keep Nz2NZ output. Under these conditions, hardware automatically merges four adjacent 16 x 16 fractals along N into one 16 x 64 fractal. ChannelMerge reads in the N direction, so scenario 3 uses `asc_set_mmad_direction_n()`. Enabling Nz2ND would no longer test this ChannelMerge workload.

### Float Nz2ND

Scenarios 5 and 6 use float output without B8/B4 quantization. `asc_set_l0c_copy_nz_para` configures the on-the-fly conversion, and `asc_copy_l0c2gm` enables Nz2ND. Nz2ND reads L0C in the N direction, so scenario 5 uses `asc_set_mmad_direction_n()`.

### UnitFlag

Both `asc_mmad` and `asc_copy_l0c2gm` use `asc_unit_flag_mode::ENABLE_UPDATE`. The producer and consumer must use matching UnitFlag modes. The Fixpipe scalar parameters are preset before MMAD starts, and the direction API is called immediately before the corresponding MMAD.

## API Version Selection

The source uses the newest APIs in the current asc-devkit repository: the strongly typed `asc_mmad` UnitFlag overload, `asc_set_l0c_copy_prequant(float, uint16_t, bool)`, `asc_set_l0c_copy_nz_para`, and the `asc_copy_l0c2gm` overload whose control parameters use the strongly typed cache, UnitFlag, quantization, and ReLU enums.

The installed CANN 9.2.0 package in this environment predates those declarations. Earlier device validation temporarily mapped them to older signatures with identical hardware settings. No legacy compatibility code remains in the delivered sample.

## Performance Comparison

### Historical Results

The historical measurement revision repeated the same MMAD/Fixpipe sequence 47 times inside one Kernel to amplify steady-state throughput differences. Those numbers are retained as research evidence but do not represent single-MMAD latency. The current `mmad_direction.asc` executes one MMAD and one Fixpipe operation.

| Workload | Recommended Mean (us) | Counterexample Mean (us) | Reduction | Result |
| --- | ---: | ---: | ---: | --- |
| float Nz + UnitFlag, 256 x 256 x 128 | 61.295 ± 0.084 | 61.948 ± 0.141 | **1.055%** | M-first is faster |
| B4 ChannelMerge + UnitFlag, 192 x 64 x 128 | 13.154 ± 0.053 | 15.149 ± 0.043 | **13.172%** | N-first is substantially faster |
| float Nz2ND + UnitFlag, 256 x 256 x 128 | 61.935 ± 0.113 | 112.891 ± 0.056 | **45.137%** | N-first is substantially faster |

Each direction has seven AB/BA-alternated samples. Each sample used two warmups and twelve timed launches, and all 42 records passed correctness verification. See [`performance/3510_sample_raw.csv`](performance/3510_sample_raw.csv) and [`performance/3510_sample_summary.csv`](performance/3510_sample_summary.csv).

The percentages are shape-dependent and must not be extrapolated directly to other tilings or complete operators.

### Measuring the Current Source

The current Kernel and Host do not contain benchmark loops. Use msOpProf to measure one normal Kernel execution and repeat the executable externally:

```bash
SCENARIO_NUM=1
rm -rf build
mkdir build && cd build
cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DSCENARIO_NUM=$SCENARIO_NUM ..
make -j
python3 ../scripts/gen_data.py -scenarioNum=$SCENARIO_NUM
msopprof ./c_api_mmad_direction_example
python3 ../scripts/verify_result.py -scenarioNum=$SCENARIO_NUM \
    output/output.bin output/golden.bin
```

Collect each recommended/counterexample pair at least five times in AB/BA order. Use `Task Duration(us)` from `OpBasicInfo.csv` for end-to-end comparison. Cube and Fixpipe metrics in `PipeUtilization.csv` explain stalls but must not be summed.

The msOpProf profiling channel previously failed in this container, so invalid `0.000000` records were not used to replace the historical table. Recollect the current one-operation source when a matching CANN package and profiling channel are available.

## Build and Run

```bash
source $INSTALL_PATH/cann/set_env.sh
SCENARIO_NUM=1
mkdir -p build && cd build
cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DSCENARIO_NUM=$SCENARIO_NUM ..
make -j
python3 ../scripts/gen_data.py -scenarioNum=$SCENARIO_NUM
./c_api_mmad_direction_example
python3 ../scripts/verify_result.py -scenarioNum=$SCENARIO_NUM \
    output/output.bin output/golden.bin
```

Set `SCENARIO_NUM` from 1 through 6 to run all cases. Clear the CMake cache before changing scenarios. A successful verification prints:

```text
test pass!
```

## Constraints

- Configure the direction before the first corresponding `asc_mmad` or `asc_mmad_mx` and do not depend on state left by another Kernel.
- Use M-first for normal Nz and other M-direction Fixpipe workloads.
- Use N-first for Nz2ND and B8/B4 Nz2NZ output that automatically enables ChannelMerge.
- ChannelMerge requires a B8/B4 destination and Nz2NZ output; Nz2ND and Nz2DN are not ChannelMerge workloads.
- A B4 destination fractal is 16 x 64, with two signed 4-bit elements per byte.
- CPU debugging is unsupported. Final performance conclusions must come from a real 3510 device.
