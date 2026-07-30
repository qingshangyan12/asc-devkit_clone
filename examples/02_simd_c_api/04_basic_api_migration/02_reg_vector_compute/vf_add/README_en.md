# vf_add Example

## Overview

This example migrates scenario 4 of the Basic API `04_memory_management/tensor_allocation` example to the Ascend C C API. The original `dav-3510` scenario defines `AddVF` as `__simd_vf__`, invokes it through `asc_vf_call`, and performs RegTensor load, addition, and store in that VF function.

The migration preserves the calculation, data type, and input formula: elementwise addition `z = x + y` of two float32 ND vectors with 1024 elements. C API address-space-qualified UB arrays replace LocalTensor/LocalMemAllocator, while `asc_loadalign`, `asc_add`, and `asc_storealign` replace `Reg::LoadAlign`, `Reg::Add`, and `Reg::StoreAlign`.

This example supports only Ascend 950PR/Ascend 950DT (`dav-3510`) and provides no CPU debug mode.

## Supported Products and CANN Versions

| Product | CANN version |
| --- | --- |
| Ascend 950PR/Ascend 950DT | >= CANN 9.1.0 |

## Directory Structure

```text
├── vf_add
│   ├── scripts
│   │   ├── gen_data.py              // Generates inputs and golden data
│   │   └── verify_result.py         // Verifies output and prints data samples
│   ├── CMakeLists.txt               // CMake build file
│   ├── vf_add.asc                   // C API kernel and host launcher
│   ├── README.md                    // Chinese documentation
│   └── README_en.md                 // English documentation
```

## Example Specification

| Item | Description |
| --- | --- |
| Operator | Add, `z = x + y` |
| Input x | `[1024]`, float32, ND |
| Input y | `[1024]`, float32, ND |
| Output z | `[1024]`, float32, ND |
| Kernel | `vf_add` |
| Core count | 1 |

The generator retains the deterministic Basic API formulas: `x[i] = (i % 100) * 0.1` and `y[i] = ((i + 50) % 100) * 0.2`. Golden data is float32 `x + y`.

## Implementation

| Basic API scenario 4 | C API implementation | Description |
| --- | --- | --- |
| `LocalMemAllocator<Hardware::UB>` + `LocalTensor` | `__ubuf__ float x_ub/y_ub/z_ub[1024]` | C API allocates UB with arrays. |
| GM-to-UB `DataCopy` | `asc_copy_gm2ub` | The asynchronous transfer is followed by `asc_sync_notify/asc_sync_wait(PIPE_MTE2, PIPE_V, EVENT_ID0)`. |
| `asc_vf_call<AddVF>` | Direct `__simd_vf__` call to `AddVfCapi` | C API does not use the Basic API VF-call wrapper. |
| `Reg::LoadAlign` | `asc_loadalign` | The UB base and each VL offset are 32-byte aligned; one float VL is loaded in NORM mode. |
| `Reg::Add` | `asc_add` | Performs addition with `asc_create_mask_b32(PAT_ALL)`. |
| `Reg::StoreAlign` | `asc_storealign` | Stores one float VL to an aligned UB address with the b32 full mask. |
| UB-to-GM `DataCopy` | `asc_copy_ub2gm` | `asc_sync_notify/asc_sync_wait(PIPE_V, PIPE_MTE3, EVENT_ID0)` protects the dependency, and `asc_sync_pipe(PIPE_MTE3)` completes writeback. |

On `dav-3510`, `asc_get_vf_len()` is 256 bytes; a float32 VL therefore has 64 elements. 1024 is divisible by 64, so the VF function has 16 full-mask iterations. This matches the original calculation based on `GetVecLen() / sizeof(float)`.

```text
GM x/y --asc_copy_gm2ub(PIPE_MTE2)--> UB x/y
      --asc_sync_notify/wait(MTE2->V)-->
UB x/y --asc_loadalign--> vector_float --asc_add--> vector_float --asc_storealign--> UB z
      --asc_sync_notify/wait(V->MTE3)-->
UB z --asc_copy_ub2gm(PIPE_MTE3)--> GM z --asc_sync_pipe(PIPE_MTE3)
```

The host reads `input/x.bin` and `input/y.bin`, allocates host/device memory and a stream, launches `vf_add<<<1, 0, stream>>>`, then writes `output/output.bin`. ACL calls and file I/O are checked, and the common cleanup label releases every allocated resource.

## Data Generation and Verification

`scripts/gen_data.py` creates float32 input and golden files. `scripts/verify_result.py` compares every output element using the same `1e-3` absolute tolerance as the Basic API sample.

Whether verification passes or fails, the verifier prints the first eight float32 elements from x, y, output, and golden. On failure, it also prints up to 101 indexes with expected value, actual value, and absolute difference.

## Build and Run

Run the following commands in the example root directory:

```bash
source ${install_path}/cann/set_env.sh
mkdir -p build && cd build
cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 .. && make -j
python3 ../scripts/gen_data.py
./demo
python3 ../scripts/verify_result.py output/output.bin output/golden.bin
```

> **Note:** Run the generator, executable, and verifier in `build` so they share the `input/` and `output/` directories. This migration was statically reviewed only; these commands were not run.

For NPU simulation, add `-DCMAKE_ASC_RUN_MODE=sim` to CMake. Clear the CMake cache before switching run modes.

Successful verification includes:

```text
test pass!
```
