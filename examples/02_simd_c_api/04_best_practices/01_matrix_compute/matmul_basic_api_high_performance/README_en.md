# Matmul Basic API High-Performance Sample

## Overview

This sample implements FP16 matrix multiplication with the Ascend C C API. It provides a single-buffer baseline and an L1/L0 Ping-Pong double-buffer scenario. Both scenarios use the same matrix dimensions, Cube tiling, core count, and compute APIs so that the performance effect of the three-stage pipeline can be compared directly.

## Supported Products and CANN Versions

| Product | CANN Version |
| --- | --- |
| Ascend 950PR/Ascend 950DT | >= CANN 9.1.0 |

## Directory Structure

```text
matmul_basic_api_high_performance
|-- scripts
|   |-- gen_data.py          # Generates input and golden data
|   `-- verify_result.py     # Verifies the output
|-- CMakeLists.txt           # Build configuration
|-- data_utils.h             # Binary file utilities
|-- mmad_double_buffer.asc   # Kernel and host implementation
|-- README.md
`-- README_en.md
```

## Sample Description

The sample computes `C = A x B` with the following matrices:

| Matrix | Data type | Logical shape | GM layout |
| --- | --- | --- | --- |
| A | FP16 | `[1024, 1024]` | ND, stored as `[M, K]` |
| B | FP16 | `[1024, 1024]` | Transposed ND, stored as `[N, K]` |
| C | FP32 | `[1024, 1024]` | ND, stored as `[M, N]` |

The base tile is `[baseM, baseN, baseK] = [128, 128, 64]`. M and N are each split into eight tiles, producing 64 logical Cube blocks. Each block computes one `128 x 128` output tile and accumulates 16 K iterations in L0C.

```text
GM(ND) --MTE2/ND2NZ--> L1(Nz) --MTE1--> L0A/L0B --M--> L0C(Nz) --FIX/NZ2ND--> GM(ND)
```

`SCENARIO_NUM` selects the implementation.

| SCENARIO_NUM | Scenario | L1/L0 buffers | Pipeline behavior |
| --- | --- | --- | --- |
| 0 | Single-buffer baseline | One A/B buffer group in L1 and L0 | Load, L0 transfer, and MMAD finish serially for each K tile |
| 1 | Double-buffer optimization | Ping/Pong A/B groups in L1 and L0 | MTE2, MTE1, and M process different K tiles concurrently |

### Scenario 0: Single-Buffer Baseline

The baseline allocates one A/B group in L1 and one group in L0A/L0B. Each K iteration performs GM-to-L1 ND2NZ movement, L1-to-L0 movement, MMAD, and a full completion wait before reusing the same addresses. It therefore provides a serial reference for the double-buffer scenario.

### Scenario 1: L1/L0 Double Buffering

The optimized scenario allocates Ping/Pong A and B buffers in both L1 and L0. Even K iterations use Ping and odd iterations use Pong. Two Event ID groups enforce the MTE2-to-MTE1, MTE1-to-MTE2, MTE1-to-M, and M-to-MTE1 dependencies.

In the steady state, MTE2 can load the next K tile while MTE1 transfers the current tile and the M pipeline computes the previous tile. After all K tiles are accumulated, `asc_copy_l0c2gm` converts L0C to ND and writes the result to GM.

## Performance Comparison

The scenarios differ only in buffer count and synchronization. Build `SCENARIO_NUM=0` and `SCENARIO_NUM=1` separately and collect the Kernel Task Duration with a profiling tool. Use the single-buffer duration as the baseline:

```text
performance improvement = baseline duration / double-buffer duration - 1
```

MTE2, MTE1, and Cube utilization and wait time can also be compared to confirm that the improvement comes from pipeline overlap rather than changes in shape, tiling, or core count.

## Build and Run

Configure the CANN environment:

```bash
source ${install_path}/cann/set_env.sh
```

Build and run one scenario from this sample directory:

```bash
SCENARIO_NUM=0
mkdir -p build && cd build
cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DSCENARIO_NUM=${SCENARIO_NUM} ..
make -j
python3 ../scripts/gen_data.py
./c_api_mmad_double_buffer_example
python3 ../scripts/verify_result.py output/output.bin output/golden.bin
```

Set `SCENARIO_NUM=1` and rerun CMake, build, execution, and verification to test the double-buffer scenario. Clear the CMake cache before switching run modes.

| Build option | Values | Description |
| --- | --- | --- |
| `CMAKE_ASC_ARCHITECTURES` | `dav-3510` | NPU architecture |
| `CMAKE_ASC_RUN_MODE` | `npu` (default), `cpu`, `sim` | Run mode |
| `SCENARIO_NUM` | `0`, `1` (default) | Single-buffer baseline or double-buffer optimization |

A successful verification prints:

```text
test pass!
```
