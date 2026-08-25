# load_data_l0b_start_index Sample

## Overview

This sample is extracted from the legacy deep-shape and legacy square_32-shape tests. It demonstrates how to use `LoadData2DParams` to move the B matrix from L1 Buffer to L0B Buffer.

Both scenarios use half inputs and float outputs. Their specifications are as follows:

| Scenario | Original test | [M, N, K] | blockDim |
|----------|---------------|-----------|----------|
| 1 | legacy deep | [16, 16, 32] | 2 |
| 2 | legacy square_32 | [32, 32, 32] | 1 |

The B matrix contains two K-direction fractals, so the sample calls `LoadData` twice. Scenario 1 uses a nonzero `startIndex` to select the second fractal on dav-2201. Scenario 2 and dav-3510 select each fractal through a source Tensor offset and keep `startIndex` at 0. On dav-2201, scenario 2 creates the L1 Nz layout as one 32x32 matrix and uses `srcStride = K / 16` to read the two N-direction fractals belonging to the same K-direction fractal.

The L0C Buffer-to-GM path uses `FixpipeParamsV220` with NZ2ND conversion enabled. This interface supports both dav-2201 and dav-3510.

## Supported Products and CANN Versions

| Product | CANN Version |
|---------|--------------|
| Ascend 950PR/Ascend 950DT | >= CANN 9.1.0 |
| Atlas A3 Training Series Products/Atlas A3 Inference Series Products | >= CANN 9.1.0 |
| Atlas A2 Training Series Products/Atlas A2 Inference Series Products | >= CANN 9.1.0 |

## Directory Structure

```text
├── load_data_l0b_start_index
│   ├── scripts
│   │   ├── gen_data.py                 // Generates inputs and golden data
│   │   └── verify_result.py            // Verifies output bit by bit
│   ├── CMakeLists.txt                   // Build configuration
│   ├── data_utils.h                     // Data I/O utilities
│   ├── load_data_l0b_start_index.asc    // Ascend C sample
│   ├── README.md                        // Chinese documentation
│   └── README_en.md                     // English documentation
```

## Sample Description

The sample executes the following pipeline:

```text
GM(ND) -> L1(Nz) -> L0A(Nz)/L0B(Zn) -> L0C(Nz) -> GM(ND)
       │         │                  │          │
    DataCopy  LoadData             Mmad        Fixpipe
```

The key L1 Buffer-to-L0B Buffer code is:

```cpp
for (uint32_t kIdx = 0; kIdx < kFractals; ++kIdx) {
#if SCENARIO_NUM == 2 && defined(__NPU_ARCH__) && (__NPU_ARCH__ == 2201)
    AscendC::LoadData2DParams params(0, nFractals, kFractals, 0, 0, true, 0);
    AscendC::LoadData(dst[kIdx * N * FRACTAL_EDGE], src[kIdx * FRACTAL_ELEMENTS], params);
#elif SCENARIO_NUM == 2 || (defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510))
    AscendC::LoadData2DParams params(0, nFractals, 1, 0, 0, true, 0);
    AscendC::LoadData(dst[kIdx * N * FRACTAL_EDGE],
        src[kIdx * nFractals * FRACTAL_ELEMENTS], params);
#else
    AscendC::LoadData2DParams params(kIdx * nFractals, nFractals, 1, 0, 0, true, 0);
    AscendC::LoadData(dst[kIdx * N * FRACTAL_EDGE], src, params);
#endif
}
```

## Build and Run

Run the following steps in the root directory of this sample.

- Configure the environment

  Configure the environment according to the CANN development kit [installation guide](../../../../../docs/en/quick_start.md#prepare&install).

  ```bash
  source ${install_path}/cann/set_env.sh
  ```

- Run the sample

  ```bash
  SCENARIO_NUM=1
  mkdir -p build && cd build;
  cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DCMAKE_ASC_RUN_MODE=sim -DSCENARIO_NUM=$SCENARIO_NUM ..;make -j;
  python3 ../scripts/gen_data.py -scenarioNum=$SCENARIO_NUM
  ./demo
  python3 ../scripts/verify_result.py output/output.bin output/golden.bin
  ```

- Build options

  | Option | Values | Description |
  |--------|--------|-------------|
  | `CMAKE_ASC_RUN_MODE` | `npu` (default), `cpu`, `sim` | NPU execution, CPU debugging, or NPU simulation |
  | `CMAKE_ASC_ARCHITECTURES` | `dav-2201`, `dav-3510` (default) | NPU architecture |
  | `SCENARIO_NUM` | `1` (default), `2` | Scenario number |

Successful verification prints:

```text
test pass!
```
