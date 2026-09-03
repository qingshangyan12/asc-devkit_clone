# Mmad Computation Direction Optimization Sample

## Overview

This sample shows how to select the Mmad computation direction from the Fixpipe L0C Buffer read order when UnitFlag is enabled. `asc_set_mmad_direction_m()` and `asc_set_mmad_direction_n()` change only the production order of 16 x 16 result fractals in L0C Buffer, not the matrix multiplication workload, result, or output format.

UnitFlag maintains a flag for every 512-byte L0C Buffer memory block. This sample uses `asc_unit_flag_mode::ENABLE_UPDATE` for both Mmad and Fixpipe: Mmad waits for flag 0 before writing and changes it to 1 after writing; Fixpipe waits for flag 1 before reading and changes it to 0 after reading. Fixpipe can therefore move out a produced fractal without a manual full-pipeline synchronization.

With matching production and read orders, the fractals read first by Fixpipe are also produced first, so Mmad and Fixpipe overlap at fractal granularity. A mismatched order can make Fixpipe wait for a later-produced fractal. Normal Nz2NZ and Nz2DN read in the M direction; Nz2ND and B8/B4 ChannelMerge that remains Nz2NZ read in the N direction. ChannelMerge is automatically enabled only for B8/B4 destinations with neither Nz2ND nor Nz2DN enabled.

![Mmad and Fixpipe both write and read in the M direction](figures/mmad_unitflag.png)

The sample contains five scenario pairs. Each pair includes a recommended example and a counterexample that changes only the Mmad direction:

1. float Nz2Nz: use `asc_set_mmad_direction_m()`;
2. B4 ChannelMerge: use `asc_set_mmad_direction_n()`;
3. float Nz2ND: use `asc_set_mmad_direction_n()`;
4. float Nz2DN: use `asc_set_mmad_direction_m()`;
5. B8 ChannelMerge: use `asc_set_mmad_direction_n()`.

## Supported Products and CANN Versions

**Table 1** Supported products and CANN versions

| Product | CANN Version |
| --- | --- |
| Ascend 950PR/Ascend 950DT | >= CANN 9.2.0 |

## Directory Structure

```text
01_mmad_direction
├── figures
│   └── mmad_unitflag.png   // UnitFlag synchronization diagram
├── performance
│   ├── 3510_sample_raw.csv       // Historical 3510 raw measurements
│   └── 3510_sample_summary.csv   // Historical 3510 summary
├── scripts
│   ├── gen_data.py         // Generates random input and golden data
│   └── verify_result.py    // Verifies output results
├── CMakeLists.txt          // Build configuration
├── data_utils.h            // Binary data read/write functions
├── mmad_direction.asc    // Kernel and Host implementation
├── README.md
└── README_en.md
```

## Sample Description

The sample computes `C = A x B`. A and B are half, and C is float in L0C Buffer.

**Table 2** Input and output specifications

| Data | Input/Output | Data Type | Dimensions | Data Layout |
| --- | --- | --- | --- | --- |
| A | Input | half | `[M, K]` | ND |
| B | Input | half | `[N, K]` | ND |
| C, scenarios 1/2 | Output | float | `[M, N]` | Nz |
| C, scenarios 3/4 | Output | int4b_t | `[M, N]` | Nz |
| C, scenarios 5/6 | Output | float | `[M, N]` | ND |
| C, scenarios 7/8 | Output | float | `[M, N]` | DN |
| C, scenarios 9/10 | Output | int8_t | `[M, N]` | Nz |

The Kernel executes in the following order:

1. Call `asc_set_gm2l1_nz_para` and `asc_copy_gm2l1_nd2nz` to move A and B from GM to L1 Buffer and convert them to Nz;
2. call `asc_copy_l12l0a` and `asc_copy_l12l0b` to load A and B from L1 Buffer to L0A Buffer and L0B Buffer;
3. preset `asc_set_l0c_copy_prequant` or `asc_set_l0c_copy_nz_para` according to the output scenario;
4. call the direction configuration API before `asc_mmad` and use `asc_unit_flag_mode::ENABLE_UPDATE` to enable UnitFlag;
5. call `asc_mmad` once to complete the matrix multiplication, then call `asc_copy_l0c2gm` once with `asc_unit_flag_mode::ENABLE_UPDATE` to enable UnitFlag and move out the result.

### Scenario Configuration

The `SCENARIO_NUM` build parameter selects a recommended example or counterexample. Within each pair, all configurations except the Mmad computation direction are identical.

**Table 3** Ten build scenarios

| `SCENARIO_NUM` | Type | Output Scenario | `[M, N, K]` | MMAD Direction | Description |
| ---: | --- | --- | --- | --- | --- |
| 1 | Recommended | float Nz2Nz + UnitFlag | `[256, 256, 128]` | `asc_set_mmad_direction_m()` | Normal Nz output reads in the M direction. |
| 2 | Counterexample | float Nz2Nz + UnitFlag | `[256, 256, 128]` | `asc_set_mmad_direction_n()` | The production order does not match the output order. |
| 3 | Recommended | B4 ChannelMerge + UnitFlag | `[192, 64, 128]` | `asc_set_mmad_direction_n()` | ChannelMerge reads in the N direction. |
| 4 | Counterexample | B4 ChannelMerge + UnitFlag | `[192, 64, 128]` | `asc_set_mmad_direction_m()` | The production order does not match the output order. |
| 5 | Recommended | float Nz2ND + UnitFlag | `[256, 256, 128]` | `asc_set_mmad_direction_n()` | Nz2ND reads in the N direction. |
| 6 | Counterexample | float Nz2ND + UnitFlag | `[256, 256, 128]` | `asc_set_mmad_direction_m()` | The production order does not match the output order. |
| 7 | Recommended | float Nz2DN + UnitFlag | `[256, 256, 128]` | `asc_set_mmad_direction_m()` | Nz2DN reads in the M direction. |
| 8 | Counterexample | float Nz2DN + UnitFlag | `[256, 256, 128]` | `asc_set_mmad_direction_n()` | The production order does not match the output order. |
| 9 | Recommended | B8 ChannelMerge + UnitFlag | `[256, 64, 128]` | `asc_set_mmad_direction_n()` | ChannelMerge reads in the N direction. |
| 10 | Counterexample | B8 ChannelMerge + UnitFlag | `[256, 64, 128]` | `asc_set_mmad_direction_m()` | The production order does not match the output order. |

### Scenarios 1/2: float Nz Output

The destination type in scenarios 1 and 2 is float, the quantization mode is `NoQuant`, and neither Nz2ND nor Nz2DN is enabled. Therefore, the result remains in Nz layout and ChannelMerge is not enabled. Normal Nz output reads L0C Buffer in the M direction, so scenario 1 uses `asc_set_mmad_direction_m()` and scenario 2 uses the N direction as a counterexample.

### Scenarios 3/4: B4 ChannelMerge

Scenarios 3 and 4 meet all of the following conditions:

- the L0C source data type is float and the GM destination data type is `int4b_t`;
- the quantization mode is `QF322S4_PRE`, with scalar quantization parameters configured by `asc_set_l0c_copy_prequant`;
- neither Nz2ND nor Nz2DN is enabled, so the output remains Nz2NZ;
- both MMAD and Fixpipe use UnitFlag update mode.

Hardware automatically enables ChannelMerge and merges four consecutive 16 x 16 fractals along the N direction into one 16 x 64 fractal. ChannelMerge reads L0C Buffer in the N direction, so scenario 3 uses `asc_set_mmad_direction_n()` and scenario 4 uses the M direction as a counterexample.

### Scenarios 5/6: float Nz2ND

Scenarios 5 and 6 configure on-the-fly format conversion parameters with `asc_set_l0c_copy_nz_para` and enable Nz2ND in `asc_copy_l0c2gm`. Nz2ND reads L0C Buffer in the N direction, so scenario 5 uses `asc_set_mmad_direction_n()` and scenario 6 uses the M direction as a counterexample.

### Scenarios 7/8: float Nz2DN

Scenarios 7 and 8 enable Nz2DN in `asc_copy_l0c2gm` and preset the format-conversion parameters before Mmad with `asc_set_l0c_copy_nz_para(1, 0, 0)` and `asc_set_l0c_copy_channel_para(1)`. DN stores data in N, M order and Fixpipe reads L0C Buffer in the M direction, so scenario 7 uses `asc_set_mmad_direction_m()`. With UnitFlag enabled, `src_nz_fractal_stride` must be 1.

### Scenarios 9/10: B8 ChannelMerge

Scenarios 9 and 10 use `QF322B8_PRE` to quantize float to signed `int8_t` and keep Nz2NZ. Hardware automatically merges two adjacent 16 x 16 fractals along N into one 16 x 32 fractal, so the layout rule selects `asc_set_mmad_direction_n()`. For this Shape, the measured difference is within noise; scenario 10 remains as a direction-only counterexample.

## Performance Comparison

**Table 4** Test configuration

| Configuration | Value |
| --- | --- |
| Block count | 1 |
| Historical kernel pipeline repetitions | 47 for Nz, B4, Nz2ND, and Nz2DN; 190 for B8. The current sample has no performance loop. |
| Mean source | Seven valid samples per direction; each sample is `mean_us` for 12 consecutive Kernel submissions; AB/BA execution order alternates. |
| Performance improvement | `(counterexample mean - recommended mean) / counterexample mean x 100%` |

**Table 5** Multiple-run means for recommended examples and counterexamples

| Scenario | Recommended Mean (us) | Counterexample Mean (us) | Performance Improvement |
| --- | ---: | ---: | ---: |
| float Nz2Nz + UnitFlag | 61.295 ± 0.084 | 61.948 ± 0.141 | **1.055%** |
| B4 ChannelMerge + UnitFlag | 13.154 ± 0.053 | 15.149 ± 0.043 | **13.172%** |
| float Nz2ND + UnitFlag | 61.935 ± 0.113 | 112.891 ± 0.056 | **45.137%** |
| float Nz2DN + UnitFlag | 60.937 ± 0.170 | 61.653 ± 0.191 | **1.162%** |
| B8 ChannelMerge + UnitFlag | 60.963 ± 0.140 | 60.962 ± 0.061 | Statistically equivalent (-0.002%) |

The raw records for Table 5 are in [`performance/3510_sample_raw.csv`](performance/3510_sample_raw.csv), and the summary is in [`performance/3510_sample_summary.csv`](performance/3510_sample_summary.csv). Gains depend on data volume, shape, partitioning strategy, and the complete operator pipeline. The B8 row demonstrates that the layout rule should be followed, but statistically equivalent results must not be reported as a gain.

### Performance Test Method

### msOpProf Tool

msOpProf collects single-operator performance data on real NPUs or in simulation. After the command completes, it generates an `OPPROF_{timestamp}_*` directory containing:

- `OpBasicInfo.csv`, which records basic Task information and end-to-end execution time;
- `PipeUtilization.csv`, which records the duration and utilization of the Cube, Scalar, MTE, and Fixpipe pipelines;
- `Memory.csv` and `MemoryL0.csv`, which record memory access metrics at different levels.

Use the following commands to collect Kernel performance data for this sample:

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

Repeat this flow seven times for each side of a pair, alternating recommended/counterexample and counterexample/recommended. Record `Task Duration(us)` from `OpBasicInfo.csv` for every run, calculate the arithmetic mean and sample standard deviation from the seven valid values for each direction, then calculate the improvement with the Table 4 formula. A record with `Task Duration(us)=0` or a Profiling channel startup failure is invalid and must not be included.

Simulation results are suitable for analyzing pipeline behavior. Final performance conclusions must be obtained from a real 3510 device.

## Build and Run

Configure the CANN environment:

```bash
source $INSTALL_PATH/cann/set_env.sh
```

`$INSTALL_PATH` is the CANN installation directory. Build and run a selected scenario from the sample directory:

```bash
SCENARIO_NUM=1
mkdir -p build && cd build
cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DSCENARIO_NUM=$SCENARIO_NUM ..
make -j
python3 ../scripts/gen_data.py -scenarioNum=$SCENARIO_NUM
./c_api_mmad_direction_example
python3 ../scripts/verify_result.py -scenarioNum=$SCENARIO_NUM \
    output/output.bin output/golden.bin
```

Clear the CMake cache in the `build` directory before switching scenarios.

**Table 6** Build options

| Build Option | Values | Description |
| --- | --- | --- |
| `CMAKE_ASC_ARCHITECTURES` | `dav-3510` | NPU architecture. |
| `CMAKE_ASC_RUN_MODE` | `npu` (default), `sim` | NPU execution or NPU simulation; CPU debugging is not supported. |
| `SCENARIO_NUM` | `1` to `10`, default `1` | Selects a scenario from Table 3. |

Successful verification prints:

```text
test pass!
```
