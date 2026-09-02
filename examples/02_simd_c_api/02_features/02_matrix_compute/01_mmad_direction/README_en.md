# Mmad Computation Direction Optimization Sample

## Overview

This sample demonstrates how the Mmad computation direction affects performance when UnitFlag is enabled. Better performance can be achieved when the result-fractal production order matches the output order of the data movement instruction.

The sample contains three groups of scenarios. Each group provides a recommended example and a counterexample that changes only the Mmad computation direction:

1. float result with Nz2Nz output: `asc_set_mmad_direction_m()` is recommended;
2. B4 quantization with Nz2Nz output, where hardware automatically enables ChannelMerge: `asc_set_mmad_direction_n()` is recommended;
3. float result with Nz2ND on-the-fly format conversion: `asc_set_mmad_direction_n()` is recommended.

`asc_set_mmad_direction_m()` and `asc_set_mmad_direction_n()` do not change the matrix multiplication workload, computation result, or output format. They change only the production order of 16 x 16 result fractals in L0C Buffer.

## Supported Products and CANN Versions

**Table 1** Supported products and CANN versions

| Product | CANN Version |
| --- | --- |
| Ascend 950PR/Ascend 950DT | >= CANN 9.2.0 |

## Directory Structure

```text
01_mmad_direction
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

The Kernel executes in the following order:

1. Call `asc_set_gm2l1_nz_para` and `asc_copy_gm2l1_nd2nz` to move A and B from GM to L1 Buffer and convert them to Nz;
2. call `asc_copy_l12l0a` and `asc_copy_l12l0b` to load A and B from L1 Buffer to L0A Buffer and L0B Buffer;
3. preset `asc_set_l0c_copy_prequant` or `asc_set_l0c_copy_nz_para` according to the output scenario;
4. call the direction configuration API before `asc_mmad` and use `asc_unit_flag_mode::ENABLE_UPDATE` to enable UnitFlag;
5. call `asc_mmad` once to complete the matrix multiplication, then call `asc_copy_l0c2gm` once with `asc_unit_flag_mode::ENABLE_UPDATE` to enable UnitFlag and move out the result.

### Scenario Configuration

The `SCENARIO_NUM` build parameter selects a recommended example or counterexample. Within each pair, all configurations except the Mmad computation direction are identical.

**Table 3** Six build scenarios

| `SCENARIO_NUM` | Type | Output Scenario | `[M, N, K]` | MMAD Direction | Description |
| ---: | --- | --- | --- | --- | --- |
| 1 | Recommended | float Nz2Nz + UnitFlag | `[256, 256, 128]` | `asc_set_mmad_direction_m()` | Normal Nz output reads in the M direction. |
| 2 | Counterexample | float Nz2Nz + UnitFlag | `[256, 256, 128]` | `asc_set_mmad_direction_n()` | The production order does not match the output order. |
| 3 | Recommended | B4 ChannelMerge + UnitFlag | `[192, 64, 128]` | `asc_set_mmad_direction_n()` | ChannelMerge reads in the N direction. |
| 4 | Counterexample | B4 ChannelMerge + UnitFlag | `[192, 64, 128]` | `asc_set_mmad_direction_m()` | The production order does not match the output order. |
| 5 | Recommended | float Nz2ND + UnitFlag | `[256, 256, 128]` | `asc_set_mmad_direction_n()` | Nz2ND reads in the N direction. |
| 6 | Counterexample | float Nz2ND + UnitFlag | `[256, 256, 128]` | `asc_set_mmad_direction_m()` | The production order does not match the output order. |

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

## Performance Comparison

**Table 4** Test configuration

| Configuration | Value |
| --- | --- |
| Block count | 1 |
| Kernel pipeline repetitions | 50 |
| Performance improvement | `(counterexample mean - recommended mean) / counterexample mean x 100%` |

**Table 5** Multiple-run means for recommended examples and counterexamples

| Scenario | Recommended Mean (us) | Counterexample Mean (us) | Performance Improvement |
| --- | ---: | ---: | ---: |
| float Nz2Nz + UnitFlag | 61.295 ± 0.084 | 61.948 ± 0.141 | **1.055%** |
| B4 ChannelMerge + UnitFlag | 13.154 ± 0.053 | 15.149 ± 0.043 | **13.172%** |
| float Nz2ND + UnitFlag | 61.935 ± 0.113 | 112.891 ± 0.056 | **45.137%** |

Performance gains depend on the data volume, shape, partitioning strategy, and complete operator pipeline. The percentages in Table 5 must not be directly extrapolated to other scenarios and are provided only to demonstrate the relationship between computation direction and performance optimization.

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
| `SCENARIO_NUM` | `1` to `6`, default `1` | Selects a scenario from Table 3. |

Successful verification prints:

```text
test pass!
```
