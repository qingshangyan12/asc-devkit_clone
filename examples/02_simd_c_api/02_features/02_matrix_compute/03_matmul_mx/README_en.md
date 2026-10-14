# High-Performance MXFP8 Matmul C API Sample

## Overview

This sample implements a high-performance MXFP8 Matmul kernel using the C API, L1/L0 [double buffering](../../../../../docs/zh/guide/technical_appendix/concepts_and_terms/performance_optimization/double_buffer.md), large-block data transfers, and pipeline synchronization. See [Cube matrix computation programming](../../../../../docs/zh/guide/programming_guide/programming_model/ai_core_simd_programming/c_pointer_programming/cube_matrix_computation.md) for the basic computation flow. The linked repository guides are in Chinese.

## Supported Products and CANN Versions

| Product | CANN version |
| --- | --- |
| Ascend 950PR&950DT系列产品 | >= CANN 9.2.0 |

The sample launches a fixed number of 32 Cube blocks for `dav-3510`. Only execution on an NPU is supported; CPU debugging is not supported. API definitions and implementations come from this repository. CMake prioritizes repository headers to avoid using older APIs from the installed CANN package.

## Directory Structure

```text
03_matmul_mx
├── scripts
│   ├── gen_data.py         # Generate inputs and reference results
│   └── verify_result.py    # Verify output against the reference
├── CMakeLists.txt          # Build configuration
├── data_utils.h           # File I/O and ACL error checking
├── matmul_mx.asc           # C API implementation
├── README_en.md           # English documentation
└── README.md              # Chinese documentation
```

## MX Matmul

See [asc_mmad_mx](../../../../../docs/zh/api/SIMD-API/c_api/cube_compute/mmad_compute/asc_mmad_mx.md) for matrix multiply-add with quantization scales and its data formats.

Ordinary Matmul has two matrix inputs:

$$
C = A * B
$$

MX Matmul additionally takes two quantization scale matrices, ScaleA and ScaleB. A and B contain MXFP8 data, and the computation is:

$$
C = (\text{scaleA} \otimes A) \times (\text{scaleB} \otimes B)
$$

Each scale is shared by 32 consecutive elements along K. The broadcast multiplication applies that scale to the corresponding group of MXFP8 elements.

## Sample Specifications

The fixed shape is `8192 x 8192 x 8192`, and the output type is `half`.

### Inputs and Output

| Input/output | Shape | Type | Data format |
| --- | --- | --- | --- |
| A | [8192, 8192] | fp8_e4m3fn_t | ND |
| B | [8192, 8192] | fp8_e4m3fn_t | ND |
| scale_a | [8192, 256] | fp8_e8m0_t | ND |
| scale_b | [8192, 256] | fp8_e8m0_t | ND |
| C | [8192, 8192] | half | ND |

In GM, A and B are stored in row-major order as `[M, K]` and `[N, K]`. Stored B is the transpose of the logical right matrix in the formula. The scales are stored as `[M, K/32]` and `[N, K/32]`. Each FP8 value and each scale occupies one byte. L0C accumulates in float, starting from zero in the first iteration, and the result is converted to half.

### Key Parameters

| Parameter | Value | Description |
| --- | --- | --- |
| matrix_m / matrix_n / matrix_k | 8192 / 8192 / 8192 | Full matrix dimensions |
| tile_m / tile_n / tile_k | 256 / 256 / 128 | M/N/K dimensions of one Cube operation |
| core_m / core_n | 2048 / 1024 | Output region assigned to each block |
| num_blocks | 32 | Fixed number of Cube blocks |
| k_rounds | 64 | K accumulation iterations per output block |
| data_step | 2 | Basic K blocks per A/B data transfer |
| data_k | 256 | Logical K length of a data transfer |
| scale_step | 8 | Basic K blocks covered by one scale transfer |
| scale_chunk | 32 | Scale bytes per row per transfer, covering logical K=1024 |
| l0_buffer_bytes | 32768 | Bytes in each Ping/Pong buffer in L0A/B |

The sample does not support arbitrary shapes or tail blocks.

## Implementation

Each core computes a `core_m * core_n` submatrix. The kernel iterates over N and M output blocks and then over K, transfers A/B and scales into L1, loads L0A/L0B, performs Cube accumulation, and writes the result to GM.

| Function | Responsibility |
| --- | --- |
| set_nz_config | Configure ND/DN-to-NZ conversion for one matrix |
| copy_data_to_l1 | Transfer A/B data blocks from GM to L1 |
| copy_scale_to_l1 | Transfer scale blocks from GM to L1 |
| load_to_l0 | Load matrix data and scales from L1 to L0 |
| compute_tile | Perform MX matrix multiply-add for the current K block |
| copy_result_to_gm | Write the result to GM |
| process_tile | Schedule the first transfers, K iterations, prefetching, and output |
| matmul_mx | Allocate on-chip buffers and assign work to 32 cores |

### Data Flow

```text
GM A/B and scales
  | copy_data_to_l1 / copy_scale_to_l1    MTE2
  v
L1
  | load_to_l0                          MTE1
  v
L0A / L0B / L0A_MX / L0B_MX
  | compute_tile                        M
  v
L0C
  | copy_result_to_gm                    FIX
  v
GM
```

#### 1. Partition the Matrix into 32 Tasks

M is split into `8192/2048=4` regions and N into `8192/1024=8`, giving `4 x 8 = 32` submatrices:

```cpp
uint32_t m_start = block_idx % (matrix_m / core_m) * core_m;
uint32_t n_start = block_idx / (matrix_m / core_m) * core_n;
```

Each core iterates over N and then M, computing `8 x 4 = 32` output blocks of 256 x 256. Each output block requires 64 K iterations. The 32 blocks cover the entire output.

#### 2. Transfer Matrix Data and Scales from GM to L1

`copy_data_to_l1` calls [asc_copy_gm2l1_nd2nz](../../../../../docs/zh/api/SIMD-API/c_api/cube_datamove/cube_compute_load/asc_copy_gm2l1_nd2nz.md) for A and B. The GM row stride is 8192 bytes, the transfer width is 256 FP8 elements, and the source K offset is `k_idx*tile_k` bytes. The destination C0 strides are tile_m and tile_n.

```cpp
set_nz_config(tile_m);
asc_copy_gm2l1_nd2nz((__cbuf__ uint8_t*)a_l1,
    (__gm__ uint8_t*)(a + k_idx * tile_k), matrix_k,
    asc_load_l2_cache_mode::NORMAL_FIRST_VICTIM, tile_m, data_k, 0, false);
```

`copy_scale_to_l1` treats each pair of adjacent E8M0 bytes as one b16 element and uses [Dn2Nz](../../../../../docs/zh/api/SIMD-API/c_api/cube_datamove/cube_compute_load/asc_copy_gm2l1_dn2nz.md) to arrange the scales for MX computation:

```cpp
set_nz_config(scale_chunk / 2);
asc_copy_gm2l1_dn2nz((__cbuf__ half*)scale_a_l1,
    (__gm__ half*)(a_scale + k_idx * tile_k / 32), scale_k,
    asc_load_l2_cache_mode::NORMAL_FIRST_VICTIM, scale_chunk / 2, tile_m, 0, false);
```

The half pointer is used for 16-bit data transfers, without numerical conversion. The GM row-stride parameter is still in bytes: `scale_k=256`. It must not be divided by two because of the pointer type.

#### 3. Load FP8 Data and MX Scales from L1 to L0

See [asc_copy_l12l0a_mx](../../../../../docs/zh/api/SIMD-API/c_api/cube_datamove/cube_compute_load/asc_copy_l12l0a_mx.md) for scale address mapping and parameter units.

`load_to_l0` uses `asc_copy_l12l0a/b` for matrix data and `asc_copy_l12l0a_mx/b_mx` for scales.

- An FP8 data fractal spans 32 K elements: `k_step=tile_k/32=4`.
- An MX scale fractal covers logical K=64: `y_step=tile_k/64=2`.
- Data offsets within a transfer block are `data_offset*tile_k*tile_m` bytes for A and `data_offset*tile_k*tile_n` bytes for B.
- The scale offset is `scale_offset_idx*(tile_k/32)*16` bytes. The source stride is `scale_chunk/2=16`; the destination stride is `tile_k/64=2`.
- The MX destination address is the corresponding L0A/B data address divided by 16.

```cpp
asc_copy_l12l0a(a_dst, a_l1 + data_offset * tile_k * tile_m,
    0, 0, tile_m / 16, tile_k / 32, tile_m / 16, tile_m / 16);
asc_copy_l12l0b(b_dst, b_l1 + data_offset * tile_k * tile_n,
    0, 0, tile_n / 16, tile_k / 32, tile_n / 16, tile_n / 16);

uint32_t scale_offset = scale_offset_idx * (tile_k / 32) * 16;
asc_copy_l12l0a_mx((uint64_t)a_dst / 16, scale_a_l1 + scale_offset,
    0, 0, tile_m / 16, tile_k / 64, scale_chunk / 2, tile_k / 64);
asc_copy_l12l0b_mx((uint64_t)b_dst / 16, scale_b_l1 + scale_offset,
    0, 0, tile_n / 16, tile_k / 64, scale_chunk / 2, tile_k / 64);
```

#### 4. Overlap Loading and Computation with L1/L0 Double Buffering

L1 and L0 use Ping/Pong buffers to overlap transfers, loads, and computation.

| Memory | Buffer | Contents | Purpose |
| --- | --- | --- | --- |
| L1 | a_l1 Ping/Pong | A data blocks | GM-to-L1 transfers of A |
| L1 | b_l1 Ping/Pong | B data blocks | GM-to-L1 transfers of B |
| L1 | scale_a_l1 Ping/Pong | A scales | Quantization scales for A |
| L1 | scale_b_l1 Ping/Pong | B scales | Quantization scales for B |
| L0A | a_l0 Ping/Pong | Current K block of A | Left Cube operand |
| L0B | b_l0 Ping/Pong | Current K block of B | Right Cube operand |
| L0C | c_l0 | Float accumulation result | Result to be written to GM |

While Cube processes the current K block, the next A/B and scale transfers can be issued. Events protect buffer reuse and ensure data is ready before consumption.

#### 5. Accumulate along K on Cube

`compute_tile` waits for L0 data and performs a 256 x 128 x 256 MX matrix multiply-add. The first iteration initializes C; subsequent iterations accumulate:

```cpp
asc_mmad_mx(c_l0, a_dst, b_dst, tile_m, tile_k, tile_n,
    asc_unit_flag_mode::DISABLE, false, false, k_idx == 0);
```

Each output block requires 64 MMAD operations. Each iteration signals M-to-MTE1 to allow the corresponding L0 buffer to be reused.

#### 6. Write Back with Float-to-Half Conversion

`copy_result_to_gm` waits for Cube completion, converts L0C NZ data to row-major ND, and selects [F322F16](../../../../../docs/zh/api/SIMD-API/c_api/cube_datamove/cube_compute_store/asc_copy_l0c2gm.md) for half output:

```cpp
asc_set_l0c_copy_nz_para(1, 0, 0);
asc_copy_l0c2gm(dst, c_l0, tile_n, tile_m, matrix_n, tile_m,
    asc_store_l2_cache_mode::NORMAL_FIRST_VICTIM, asc_unit_flag_mode::DISABLE,
    QuantMode_t::F322F16, asc_relu_pre_mode::NONE, false, true, false, false);
```

The destination row stride is 8192 half elements, the source NZ stride is tile_m=256, and the destination address is `c+m*matrix_n+n`.

After writeback, a FIX-to-M event is signaled. Its wait is deferred until the first MMAD of the next output block, allowing L1/L0 transfers to start earlier while protecting L0C from overwrite.

### Pipeline Synchronization

[asc_sync_notify / asc_sync_wait](../../../../../docs/zh/api/SIMD-API/c_api/sync/intra_core_sync/asc_sync_notify.md) synchronize different pipelines within an AI Core.

| Direction | Event ID | Meaning |
| --- | --- | --- |
| MTE2 → MTE1 | 0/1 | A/B data Ping/Pong ready |
| MTE1 → MTE2 | 0/1 | A/B data Ping/Pong available for overwrite |
| MTE2 → MTE1 | 2/3 | Scale Ping/Pong ready |
| MTE1 → MTE2 | 2/3 | Scale Ping/Pong available for overwrite |
| MTE1 → M | 0/1 | L0 Ping/Pong data and scales ready |
| M → MTE1 | 0/1 | Cube has consumed L0 Ping/Pong |
| M → FIX | 0 | Output block accumulation complete |
| FIX → M | 0 | L0C available for the next output block |

Event IDs are independent for each pipeline direction. The kernel initializes MTE1-to-MTE2 events 0–3, M-to-MTE1 events 0/1, and FIX-to-M event 0 before the first write. It waits for all remaining reverse events at exit, then calls `asc_sync_pipe(PIPE_ALL)`.

For each data or scale transfer block, the first K iteration waits for readiness and the last releases its buffer. A/B share data events, and both scale matrices share scale events. Independent transfer progress avoids fetching scales from GM every iteration.

## Performance Optimizations

- Use a fixed 4 x 8 partition across 32 cores. Each core computes the entire K dimension, avoiding cross-core reduction.
- Transfer two basic K blocks of A/B data and eight basic K blocks of scales at a time to reduce MTE2 instruction count.
- Retain 256 x 256 output blocks for input reuse, with K=128 to fit FP8 L0 double buffers.
- Use separate Ping/Pong buffers and synchronization events for L1 data, L1 scales, and L0.
- Defer the L0C reuse wait so input preloading for the next output block can overlap the previous writeback.

## Theoretical Performance Comparison

The analysis assumes **32 available Cube cores at 1.65 GHz**. Measurements must be collected under these conditions before filling in the table.

| Metric | Description |
| --- | --- |
| Task Duration(μs) | Total kernel task duration; use this for end-to-end operator performance |
| Block Num | Number of launched blocks |
| aicore_time(μs) | AI Core execution time reported by the profiler |
| aic_mac_time(μs) / aic_mac_ratio | Cube computation time and ratio |
| aic_scalar_time(μs) / aic_scalar_ratio | Scalar scheduling/address calculation time and ratio |
| aic_mte1_time(μs) / aic_mte1_ratio | L1-to-L0 load time and ratio |
| aic_mte2_time(μs) / aic_mte2_ratio | GM-to-L1 transfer time and ratio |
| aic_fixpipe_time(μs) / aic_fixpipe_ratio | Result writeback time and ratio |

32-core measurements (blank pending collection):

| Case version | Task Duration(μs) | Block Num | aicore_time(μs) | aic_mac_time(μs) | aic_mac_ratio | aic_scalar_time(μs) | aic_scalar_ratio | aic_mte1_time(μs) | aic_mte1_ratio | aic_mte2_time(μs) | aic_mte2_ratio | aic_fixpipe_time(μs) | aic_fixpipe_ratio |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| C API MXFP8 Matmul | | 32 | | | | | | | | | | | |

Pipeline execution overlaps, so these times must not be summed. The Cube time ratio is not the global utilization of all physical cores.

### Cube Computation

For FP8, each core performs 16 x 32 x 16 multiply-adds per cycle:

$$T_{cube}=\frac{M\times N\times K}{16\times32\times16\times1.65\times10^9\times32}
=\frac{8192^3}{8192\times1.65\times10^9\times32}\approx1271.001\ \mu s$$

This is the theoretical lower bound for computation alone, excluding pipeline startup, completion, and writeback overhead. After collecting measurements, calculate the relative deviation as:

$$E_{cube}=\frac{T_{mac,actual}-T_{cube}}{T_{cube}}\times100\%$$

### MTE2 Bandwidth

Each A row block participates in `N/tile_n=32` output blocks, and each B row block in `M/tile_m=32`. Scales have the same reuse counts. At one byte per FP8 element, the total logical GM-to-L1 transfer volume is:

$$D_{total}=\frac{N}{tile_n}MK+\frac{M}{tile_m}NK+
\frac{N}{tile_n}M\cdot scale_k+\frac{M}{tile_m}N\cdot scale_k$$

$$=2\ GiB+2\ GiB+64\ MiB+64\ MiB=4.125\ GiB=4,429,185,024\ B$$

The unique input volume is:

$$D_{input}=MK+NK+M\cdot scale_k+N\cdot scale_k=132\ MiB=138,412,032\ B$$

If every input byte is fetched from HBM only once and all subsequent reads hit L2, the subsequent volume is `D_total-D_input=4,290,772,992 B`. Using the original sample's bandwidth assumptions of 1.6 TB/s for HBM and 5 TB/s for L2 gives an idealized transfer-time estimate:

$$T_{MTE2,model}=\frac{138,412,032}{1.6\times10^{12}}+
\frac{4,290,772,992}{5\times10^{12}}\approx944.662\ \mu s$$

Capacities use binary MiB/GiB; bandwidth uses decimal TB/s. These bandwidths are modeling assumptions, not device measurements. HBM and L2 transfers can overlap, and the model excludes format conversion, instructions, and cache conflicts. The logical volume of 4.125 GiB is not the actual HBM traffic. With the original sample's assumed 128 MiB L2 capacity, the 132 MiB input cannot all remain cached; output and other traffic also affect hit rates. Verify these effects with the profiler.

MTE2 measurements are left blank. Once collected, compare them with the model and examine cache reuse and ND/DN-to-NZ conversion overhead.

## Build and Run

Run the following steps in the root directory of this example to build and run the example.

> **Notice**: This example requires `ml_dtypes` library version `0.2.0`, Installation command:

```bash
python3 -m pip install ml_dtypes==0.2.0
```

### Configure Environment Variables

Configure environment variables based on the [installation method](../../../../../docs/en/quick_start.md#prepare&install) of the CANN development kit in the current environment.


```bash
source ${install_path}/cann/set_env.sh
```

> **Note:** `${install_path}` is the CANN package installation directory. When no installation directory is specified, the default installation path is `/usr/local/Ascend`.

### Run the Example

Run the following commands in this example directory.

```bash
mkdir -p build && cd build;  # Create and enter the build directory
cmake .. -DCMAKE_ASC_RUN_MODE=npu -DCMAKE_ASC_ARCHITECTURES=dav-3510; make -j;
python3 ../scripts/gen_data.py
./demo
python3 ../scripts/verify_result.py ./output/output.bin ./output/golden.bin
```

To use NPU simulation mode, set `-DCMAKE_ASC_RUN_MODE=sim`.

```bash
cmake .. -DCMAKE_ASC_RUN_MODE=npu -DCMAKE_ASC_ARCHITECTURES=dav-3510; make -j; # NPU mode
cmake .. -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-3510; make -j; # NPU simulation mode
```

Build option description:

| Parameter | Values | Description |
|------|--------|------|
| `CMAKE_ASC_RUN_MODE` | `npu` / `sim` | Run mode: NPU execution, NPU simulation |
| `CMAKE_ASC_ARCHITECTURES` | `dav-3510` | Target SoC architecture |

> **Notice:** Clear CMake cache before switching `CMAKE_ASC_RUN_MODE` / `CMAKE_ASC_ARCHITECTURES`. Run `rm CMakeCache.txt` in the build directory and then re-run cmake.

The following execution result indicates that the accuracy comparison succeeded.

```bash
test pass!
```

### Performance Analysis

### Introduction to the msOpProf Tool

msOpProf is a single-operator performance analysis tool with two usage modes: `msopprof` and `msopprof simulator`. It helps users identify anomalies in operator memory, code, and instructions for comprehensive operator tuning. It currently supports performance data collection and automatic parsing for different run modes (on-device or simulation) and file types (executables or operator binary `.o` files).

- On-device performance collection

    On-device performance collection directly measures the operator's execution time on the Ascend AI Processor. This method is suitable for quickly locating operator performance issues in an on-device environment.

    Run msopprof on the `demo` executable for operator tuning:
    ```
    msopprof ./demo
    ```

    - Performance data description  
      After the command completes, a folder named "OPPROF_{timestamp}_XXX" will be generated in the default directory. The performance data folder structure is as follows:

      ```bash
      ├──dump                       # Raw performance data; users do not need to inspect it
      ├──ArithmeticUtilization.csv  # Cube/Vector instruction cycle proportions
      ├──L2Cache.csv                # L2 Cache hit rate; affects MTE2. Plan data transfer logic properly to increase the hit rate
      ├──Memory.csv                 # Read/write bandwidth rates of UB, L1, and main memory
      ├──MemoryL0.csv               # Read/write bandwidth rates of L0A, L0B, and L0C
      ├──MemoryUB.csv               # Read/write bandwidth rates from Vector and Scalar to UB
      ├──OpBasicInfo.csv            # Basic operator information
      ├──PipeUtilization.csv        # Durations and proportions of computation and data transfer units
      ├──ResourceConflictRatio.csv  # Proportions of UB bank groups, bank conflicts, and resource conflicts among all instructions
      └──visualize_data.bin         # MindStudio Insight presentation file
      ```

View the detailed performance analysis results:

```bash
cat ./OPPROF_*/PipeUtilization.csv
```
