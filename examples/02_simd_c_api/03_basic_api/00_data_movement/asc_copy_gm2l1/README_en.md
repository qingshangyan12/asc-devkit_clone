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

Both input matrices use `half`. Scenarios 1-4 produce `float` output, and scenario 5 produces `int8` output:

```text
A[M, K] × B[K, N] = C[M, N]
M = 128, K = 128, N = 256
```

`SCENARIO_NUM` selects the GM-to-L1 and output path:

| Scenario | GM input layout | Additional operation | C API focus |
| --- | --- | --- |
| 1 | NZ | None | `asc_copy_gm2l1_sync` |
| 2 | ND | None | `asc_copy_gm2l1_nd2nz_sync` |
| 3 | DN | None | `asc_copy_gm2l1_dn2nz_sync` |
| 4 | ND | `C = A * B + bias`, `bias[1, N]` | `asc_copy_l12bt_sync` and BiasTable-backed MMAD |
| 5 | ND | Vector quantization of `C` to `int8` | `asc_copy_l12fb_sync`, `asc_set_l0c2gm_config`, and `VQF322B8_PRE` |

Scenario 1 stores the input in NZ format. Scenarios 2, 4, and 5 convert ND to NZ while moving data from GM to L1; scenario 3 converts DN to NZ. Scenario 4 adds the same `bias[1, N]` row to every output row. Scenario 5 loads one vector-quantization parameter per output column and writes `int8` output.

## Implementation

The kernel uses the following C API sequence:

1. Initialize kernel state with `asc_init`.
2. Move A and B to L1 using `asc_copy_gm2l1_sync`, `asc_copy_gm2l1_nd2nz_sync`, or `asc_copy_gm2l1_dn2nz_sync`.
3. In scenario 4, move the ND bias to L1 and then to BiasTable with `asc_copy_l12bt_sync`, so `asc_mmad_sync` can use C2 as its initial-value source.
4. Move NZ data to L0A and L0B with `asc_copy_l12l0a_sync` and `asc_copy_l12l0b_transpose_sync`.
5. Perform matrix multiplication with `asc_mmad_sync`.
6. In scenario 5, move per-column quantization parameters from L1 to Fixpipe Buffer with `asc_copy_l12fb_sync`, then configure their address with `asc_set_l0c2gm_config`.
7. Convert the NZ result in L0C to row-major `float` (scenarios 1-4) or vector-quantized `int8` (scenario 5) data in GM with `asc_copy_l0c2gm_sync`.

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

Use the same value (`1` through `5`) for both `SCENARIO_NUM` and `-scenarioNum`.

### Build Options

| Option | Description | Values |
| --- | --- | --- |
| `CMAKE_ASC_ARCHITECTURES` | NPU architecture | `dav-3510` |
| `SCENARIO_NUM` | GM-to-L1/output scenario | `1` (NZ), `2` (ND), `3` (DN), `4` (Bias), `5` (vector quantization) |

## Expected Result

`verify_result.py` prints `test pass!` when the NPU result is within the accepted float tolerance (scenarios 1-4) or exactly matches the `int8` golden data (scenario 5).
