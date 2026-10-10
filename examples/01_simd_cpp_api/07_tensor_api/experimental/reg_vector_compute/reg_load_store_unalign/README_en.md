# Unaligned Register Load and Store Sample

## Overview

This sample demonstrates unaligned transfers between UB and registers using the experimental Tensor APIs `asc::te::experimental::unalign_load` and `unalign_store`. It does not directly call unaligned C APIs or the Basic APIs `AscendC::Reg::LoadUnAlign/StoreUnAlign`. GM/UB transfers use Tensor API `copy`, register computation uses experimental Tensor APIs, and pipeline synchronization still uses the C APIs `asc_lock/asc_unlock`.

Select a scenario with `SCENARIO_NUM`:

| Scenario | Input | Output | Operation |
| --- | --- | --- | --- |
| 1 | `float [1024]` | `float [1024]` | Copy two registers (128 elements) starting at element 1. All other output elements remain zero. |
| 2 | `uint16_t [14, 255]` | `uint16_t [14, 16]` | Four cores process up to four rows each; the last core processes two rows. Reduce every 16 elements by maximum, with 15 valid elements in the last datablock. |

## Supported Products

- Ascend 950PR/Ascend 950DT

The CANN installation must include the `unalign_load` / `unalign_store` APIs added by PR 6049. Verification uses CANN 9.3.0 with those APIs synchronized into the package and the contiguous-load state advancement corrected. Internally, `unalign_load` uses a postupdate operation to advance hardware state without modifying external tensors or coordinates. An unmodified package without these APIs cannot build this sample. CMake loads the package headers, not the repository headers.

## Directory Structure

```text
├── CMakeLists.txt                 // Build configuration and CTest entry
├── data_utils.h                   // Local sample utility header
├── README.md                      // Chinese documentation
├── README_en.md                   // English documentation
└── reg_load_store_unalign.asc     // Kernels, input generation, and verification
```

## Implementation

### Scenario 1: Unaligned Copy

1. Copy the input from GM to UB and zero the output UB using `fill/store`.
2. Create source and destination tensor views using `slice` in the kernel, offset both by one element (four bytes), and pass the views into the VF.
3. Create `unalign_load` and `unalign_store` sessions and prepare the load stream with `loader.prepare(src_stream)`.
4. Load and store two full registers with `loader.load` and `storer.store`, using coordinates relative to the views and an explicit `count` for each load and store.
5. Commit pending stores with `storer.flush(dst_stream, 2 * elements_per_register)` and copy the output back to GM.

```cpp
asc::te::experimental::unalign_load loader;
asc::te::experimental::unalign_store storer;
loader.prepare(src_stream);
for (uint32_t repeat = 0; repeat < 2; ++repeat) {
    auto coord = asc::te::make_coord(repeat * elements_per_register);
    auto src = loader.load(src_stream, coord, elements_per_register);
    storer.store(dst_stream, coord, src, elements_per_register);
}
storer.flush(dst_stream, static_cast<int32_t>(2 * elements_per_register));
```

### Scenario 2: Multicore Reduction with Unaligned Addresses and Lengths

1. Use `block_idx` to compute the first row and actual row count. Copy each core's input into a contiguous, one-dimensional UB buffer.
2. Keep the 255-element rows packed without inter-row padding. Subsequent UB rows and subsequent cores' GM input addresses are not 32-byte aligned.
3. Create per-row tensor views in the kernel and call the VF once per row. Each VF creates separate `unalign_load` / `unalign_store` sessions and loads two 256-byte registers. Reserve an additional register of source UB space so a full-register tail load cannot access the output buffer.
4. Mask the last invalid element using `update_mask<uint16_t>` and call `reduce_max<reduce_scope::datablock>` for the maximum of every 32-byte datablock. The data type is deduced from the input.
5. Store eight results at destination row coordinates 0 and 8 with `storer.store`. Call `storer.flush(dst_row, output_columns)` and copy results back to GM.

**Session requirements:** Call `prepare` before each contiguous load stream; the first load must use the prepared address. Coordinates are relative to the supplied tensor view and are computed explicitly by the sample; the API does not update external tensors or coordinates. Loads read a full register, with `count` specifying the number of elements by which to advance the load stream; this sample uses the number of elements per register. Stores write `count` elements without using the Reg Tensor mask. Apply masks during reduction. Call `flush` once per store stream, passing the first destination view and the end-of-stream element offset relative to that view. In CANN 9.3.0, `slice` cannot be called directly inside a VF, so views are constructed in the kernel.

### Verification

The host generates deterministic input and computes golden results without Python or external files. Scenario 1 checks all 1024 outputs, including zeros outside the copied range. Scenario 2 checks all 224 results, including each row's tail datablock and the last core. ACL errors or mismatches cause a nonzero exit status.

## Build and Run

Configure the environment using the CANN installation directory:

```bash
source ${install_path}/set_env.sh

```

Replace `${install_path}` with the CANN installation directory. The sample uses `find_package(ASC REQUIRED)` to obtain Tensor/C API headers, the compiler, ACL runtime, and simulator from CANN.

Run these commands from the sample directory. NPU mode and scenario 1 are the defaults:

```bash
mkdir -p build && cd build
cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DCANN_ASC_USE_EXPERIMENTAL=ON ..;make -j;
./demo
```

For scenario 2, reconfigure and rebuild from the build directory:

```bash
cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DCANN_ASC_USE_EXPERIMENTAL=ON -DSCENARIO_NUM=2 ..;make -j;
./demo
```

For NPU simulation, add `-DCMAKE_ASC_RUN_MODE=sim`:

```bash
cmake -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DCANN_ASC_USE_EXPERIMENTAL=ON -DSCENARIO_NUM=1 ..;make -j;
./demo
```

Use `-DSCENARIO_NUM=2` for scenario 2. Input and golden results are generated by the host program; no data generation script is needed.

**Note:** Before switching run modes, remove the CMake cache with `rm CMakeCache.txt` in the build directory and reconfigure. Scenario selection is cached; explicitly pass `-DSCENARIO_NUM=1` to switch back to scenario 1.

After updating CANN package headers, run `make clean` followed by `make -j`, or use a new build directory to avoid reusing stale compilation artifacts.

| Option | Values | Description |
| --- | --- | --- |
| `CMAKE_ASC_RUN_MODE` | `npu` (default), `sim` | NPU execution or simulation |
| `CMAKE_ASC_ARCHITECTURES` | `dav-3510` | Ascend 950PR/Ascend 950DT |
| `CANN_ASC_USE_EXPERIMENTAL` | `ON` (required), `OFF` (default) | Enable experimental ASC APIs |
| `SCENARIO_NUM` | `1` (default), `2` | Unaligned copy or datablock maximum reduction |

Successful verification prints the corresponding line:

```text
scenario 1: test pass!
scenario 2: test pass!
```
