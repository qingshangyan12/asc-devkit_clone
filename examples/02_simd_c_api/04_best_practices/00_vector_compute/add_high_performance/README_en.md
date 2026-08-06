# Add High-Performance Sample

## Overview

This sample implements half-precision element-wise Add with the Ascend C C API. It provides a single-buffer baseline and a Ping-Pong double-buffer scenario. Both scenarios use the same inputs, core count, tiling, and vector arithmetic so that the effect of overlapping GM/UB data movement with Vector computation can be compared directly.

## Supported Products and CANN Versions

| Product | CANN Version |
| --- | --- |
| Ascend 950PR/Ascend 950DT | >= CANN 9.1.0 |

## Directory Structure

```text
add_high_performance
|-- figures
|   `-- DoubleBuffer.png     # Double-buffer pipeline diagram
|-- scripts
|   |-- gen_data.py          # Generates input and golden data
|   `-- verify_result.py     # Verifies the output
|-- CMakeLists.txt           # Build configuration
|-- c_api_add.asc            # Kernel and host implementation
|-- data_utils.h             # Binary file utilities
|-- README.md
`-- README_en.md
```

## Sample Description

The sample performs element-wise addition on two half matrices with shape `[8192, 8192]`:

```text
z = x + y
```

Both scenarios use 64 cores and process 21760 contiguous elements per iteration. `SCENARIO_NUM` selects the implementation.

| SCENARIO_NUM | Scenario | UB buffers | Pipeline behavior |
| --- | --- | --- | --- |
| 0 | Single-buffer baseline | One x/y/z buffer group | Load, compute, and store finish serially before the next tile |
| 1 | Double-buffer optimization | Ping/Pong x/y/z buffer groups | MTE2, Vector, and MTE3 operate on different groups concurrently |

### Scenario 0: Single-Buffer Baseline

The baseline allocates one x/y/z buffer group in UB. Each iteration loads x and y with `asc_copy_gm2ub_align`, computes with the register form of `asc_add`, stores z with `asc_copy_ub2gm_align`, and waits for all pipelines before reusing the same UB addresses. Cross-iteration overlap is therefore disabled.

### Scenario 1: Ping-Pong Double Buffering

The optimized scenario alternates between Ping and Pong x/y/z groups. Two Event ID groups enforce the MTE2-to-Vector, Vector-to-MTE2, Vector-to-MTE3, and MTE3-to-Vector dependencies. While Vector processes Ping, MTE2 and MTE3 can process Pong, and vice versa.

<p align="center">
  <img src="figures/DoubleBuffer.png" width="50%">
</p>

## Performance Comparison

The scenarios differ only in buffer count and synchronization. Build `SCENARIO_NUM=0` and `SCENARIO_NUM=1` separately and collect the Kernel Task Duration with a profiling tool. Use the single-buffer duration as the baseline:

```text
performance improvement = baseline duration / double-buffer duration - 1
```

The benefit depends on the balance between data movement and computation. Double buffering generally helps most when these costs are comparable; a workload limited entirely by GM bandwidth may see less improvement.

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
./c_api_add_example
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
