# GM-to-L1 Data Movement with the C API (Cube Scenario)

This sample uses the Ascend C API to move matrices from Global Memory (GM) to the L1 Buffer, then performs L1-to-L0 movement, matrix multiplication, and L0C-to-GM result verification.

It supports Ascend 950PR and Ascend 950DT (`dav-3510`) only. The sample runs on an NPU only; CPU Debug and simulation modes are not provided.

## Directory Structure

```text
asc_copy_gm2l1
├── CMakeLists.txt                 # Build script
├── asc_copy_gm2l1.asc             # C API kernel and host launcher
├── data_utils.h                   # Binary data I/O helpers
├── scripts
│   ├── gen_data.py                # Generates input and golden data
│   └── verify_result.py           # Verifies the result
├── README.md
└── README_en.md
```

## Operator Description

Both input matrices use `half`, and the output matrix uses `float`:

```text
A[M, K] × B[K, N] = C[M, N]
M = 128, K = 128, N = 256
```

`SCENARIO_NUM` selects the input layout in GM:

| Scenario | GM input layout | GM-to-L1 C API |
| --- | --- | --- |
| 1 | NZ | `asc_copy_gm2l1_sync` |
| 2 | ND | `asc_copy_gm2l1_nd2nz_sync` |
| 3 | DN | `asc_copy_gm2l1_dn2nz_sync` |

Scenario 1 stores the input in NZ format. Scenarios 2 and 3 convert ND to NZ and DN to NZ, respectively, while moving the input from GM to L1.

## Implementation

The kernel uses the following C API sequence:

1. Initialize kernel state with `asc_init`.
2. Move A and B to L1 using `asc_copy_gm2l1_sync`, `asc_copy_gm2l1_nd2nz_sync`, or `asc_copy_gm2l1_dn2nz_sync`.
3. Move NZ data to L0A and L0B with `asc_copy_l12l0a_sync` and `asc_copy_l12l0b_transpose_sync`.
4. Perform matrix multiplication with `asc_mmad_sync`.
5. Convert the NZ result in L0C to row-major `float` data in GM with `asc_copy_l0c2gm_sync`.

All device-side operations use synchronous C APIs. The sample does not depend on C++ `GlobalTensor`, `LocalTensor`, `DataCopyParams`, or CPU Debug macros.

## Build and Run

Configure the CANN build environment and NPU runtime environment before running the commands below.

```bash
cd asc_copy_gm2l1
mkdir -p build && cd build
cmake -DSCENARIO_NUM=1 -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..
make -j
python3 ../scripts/gen_data.py -scenarioNum=1
./demo
python3 ../scripts/verify_result.py output/output.bin output/golden.bin
```

Use the same value (`1`, `2`, or `3`) for both `SCENARIO_NUM` and `-scenarioNum` to run the corresponding input-layout scenario.

### Build Options

| Option | Description | Values |
| --- | --- | --- |
| `CMAKE_ASC_ARCHITECTURES` | NPU architecture | `dav-3510` |
| `SCENARIO_NUM` | GM input-layout scenario | `1` (NZ), `2` (ND), `3` (DN) |

## Expected Result

`verify_result.py` prints `test pass!` when the NPU result is within the accepted error tolerance of the golden data.
