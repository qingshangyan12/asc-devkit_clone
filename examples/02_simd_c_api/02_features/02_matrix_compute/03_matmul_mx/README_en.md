# High-Performance MXFP8 Matmul C API Sample

## Overview

This sample demonstrates how to implement a high-performance MXFP8 Matmul kernel using the C API, with double buffering in L1 Buffer, L0A Buffer, and L0B Buffer, large-block data movement, and pipeline synchronization. For the basic matrix computation flow, see [Cube Matrix Computation Programming](../../../../../docs/zh/guide/programming_guide/programming_model/ai_core_simd_programming/c_pointer_programming/cube_matrix_computation.md).

## Supported Products and CANN Versions

| Product | CANN Version |
|------|-------------|
| Ascend 950PR&950DT series products | >= CANN 9.3.0 |

## Directory Structure

```text
├── 03_matmul_mx
│   ├── scripts
│   │   ├── gen_data.py         // Generates input data and golden data
│   │   └── verify_result.py    // Verifies output data against golden data
│   ├── CMakeLists.txt          // Build configuration file
│   ├── data_utils.h            // Binary data read and write functions
│   ├── matmul_mx.asc           // C API sample implementation
│   ├── README_en.md            // English documentation
│   └── README.md               // Chinese documentation
```

## Introduction to MX Matmul

For matrix multiply-add computation with quantization scales and the corresponding data formats, see [asc_mmad_mx](../../../../../docs/zh/api/SIMD-API/c_api/cube_compute/mmad_compute/asc_mmad_mx.md).

Ordinary Matmul has two matrix inputs:

$$
C = A * B
$$

MX Matmul additionally takes two scale inputs. A and B contain low-bit-width MXFP8 data, while scaleA and scaleB contain quantization scales. The computation applies each scale to its corresponding matrix data:

$$
C = (\text{scaleA} \otimes A) \times (\text{scaleB} \otimes B)
$$

In MX Matmul, every 32 elements along the K axis share one scale factor. During computation, the scale is broadcast to the corresponding group of MXFP8 elements along the K axis.

## Sample Specifications

This sample implements MXFP8 Matmul with a fixed shape of `8192 x 8192 x 8192` and the output data type `half`.

### Input and Output Data

| Input/Output | Shape | Data Type | Data Layout |
| --- | --- | --- | --- |
| A | [8192, 8192] | fp8_e4m3fn_t | ND |
| B | [8192, 8192] | fp8_e4m3fn_t | ND |
| scale_a | [8192, 256] | fp8_e8m0_t | ND |
| scale_b | [8192, 256] | fp8_e8m0_t | ND |
| C | [8192, 8192] | half | ND |

In GM, A and B are stored in row-major order as `[M, K]` and `[N, K]`, respectively. scale_a and scale_b are stored in row-major order as `[M, K/32]` and `[N, K/32]`, respectively. Accumulation in L0C Buffer uses float, with C initialized to zero in the first iteration. The result is converted to half for output.

### Key Parameters

| Code Parameter | Value | Description |
| --- | --- | --- |
| matrix_m / matrix_n / matrix_k | 8192 / 8192 / 8192 | Full matrix dimensions |
| block_m / block_n / block_k | 256 / 256 / 128 | M/N/K dimensions of one Cube computation |
| core_m / core_n | 2048 / 1024 | Output region assigned to each block |
| num_blocks | 32 | Fixed number of Cube blocks launched |
| k_rounds | 64 | Number of accumulation iterations along K for each output block |
| data_step | 2 | Number of basic K blocks in each A/B data transfer block |
| data_k | 256 | Logical K length of each data transfer block |
| scale_step | 8 | Number of basic K blocks covered by each scale transfer block |
| scale_chunk | 32 | Bytes per row in each scale transfer block, corresponding to logical K=1024 |
| l0_buffer_bytes | 32768 | Bytes in each Ping/Pong buffer in L0A Buffer or L0B Buffer |

> **Constraint:** This sample does not support arbitrary shapes or tail processing.

## Sample Implementation

After partitioning the computation across cores, each core computes a `core_m * core_n` submatrix. The kernel iterates over the M/N output blocks, moves A/B and scales into L1 Buffer, and then iterates along K to load L0A Buffer and L0B Buffer and perform Cube accumulation. Finally, it writes the result back to GM.

| Function | Purpose |
| --- | --- |
| set_nz_config | Configure ND/DN-to-NZ layout conversion for a single matrix |
| copy_data_to_l1 | Move large A/B data blocks from GM to L1 Buffer |
| copy_scale_to_l1 | Move large scale data blocks from GM to L1 Buffer |
| load_to_l0 | Load matrix data and scales from L1 Buffer to L0A Buffer and L0B Buffer |
| compute_block | Perform MX matrix multiply-accumulate for the current K block |
| copy_result_to_gm | Move the computation result back to GM |
| process_output_block | Organize the initial data transfers, K loop, prefetching, and output |
| matmul_mx | Allocate on-chip memory and distribute work across a fixed set of 32 cores |

### Data Flow

```text
Memory Location                         Pipeline
  GM
  |  copy_data_to_l1                      MTE2
  |  copy_scale_to_l1                     MTE2
  v
  L1 Buffer
  |
  |  load_to_l0                           MTE1
  v
  L0A/L0B/L0A_MX/L0B_MX Buffer
  |
  |  compute_block                         M
  v
  L0C Buffer
  |
  |  copy_result_to_gm                    FIX
  v
  GM
```

#### 1. Multi-Core Partitioning: Divide the Matrix into 32 Subtasks

The M dimension is divided into `8192/2048=4` parts, and the N dimension into `8192/1024=8` parts, producing `4 x 8 = 32` submatrices:

```cpp
uint32_t m_start = block_idx % (matrix_m / core_m) * core_m;
uint32_t n_start = block_idx / (matrix_m / core_m) * core_n;
```

Each core uses an outer loop over N and an inner loop over M. Its output region contains `8×4=32` blocks of 256×256 elements. Each output block requires 64 multiply-accumulate iterations along K. The 32 launched blocks cover the complete output.

#### 2. GM to L1 Buffer: Move Matrices and Scales in Large Blocks

`copy_data_to_l1` calls [`asc_copy_gm2l1_nd2nz`](../../../../../docs/zh/api/SIMD-API/c_api/cube_datamove/cube_compute_load/asc_copy_gm2l1_nd2nz.md) for A and B. The GM row stride is 8192 bytes, the transfer width is 256 FP8 elements, and the source offset along K is `k_idx*block_k` bytes. The destination C0 strides for A and B are block_m and block_n, respectively.

```cpp
set_nz_config(block_m);
asc_copy_gm2l1_nd2nz((__cbuf__ uint8_t*)a_l1,
    (__gm__ uint8_t*)(a + k_idx * block_k), matrix_k,
    asc_load_l2_cache_mode::NORMAL_FIRST_VICTIM, block_m, data_k, 0, false);
```

`copy_scale_to_l1` treats two adjacent E8M0 bytes as one b16 element and uses [Dn2Nz](../../../../../docs/zh/api/SIMD-API/c_api/cube_datamove/cube_compute_load/asc_copy_gm2l1_dn2nz.md) to produce the layout required for MX computation:

```cpp
set_nz_config(scale_chunk / 2);
asc_copy_gm2l1_dn2nz((__cbuf__ half*)scale_a_l1,
    (__gm__ half*)(a_scale + k_idx * block_k / 32), scale_k,
    asc_load_l2_cache_mode::NORMAL_FIRST_VICTIM, scale_chunk / 2, block_m, 0, false);
```

The half pointer is used for 16-bit data movement; it does not perform numerical conversion. The GM row stride remains a byte count, `scale_k=256`, and must not be divided by 2 because of the pointer type change.

#### 3. L1 Buffer to L0A Buffer and L0B Buffer: Load FP8 Data and MX Scales

For scale address mapping and parameter units, see [asc_copy_l12l0a_mx](../../../../../docs/zh/api/SIMD-API/c_api/cube_datamove/cube_compute_load/asc_copy_l12l0a_mx.md).

`load_to_l0` uses `asc_copy_l12l0a/b` to load matrix data and `asc_copy_l12l0a_mx/b_mx` to load the corresponding scales.

- Each FP8 data fractal spans 32 elements along K, so the data parameter is `k_step=block_k/32=4`.
- Each MX scale fractal covers logical K=64, so the scale parameter is `y_step=block_k/64=2`. The data stride cannot be reused for scales.
- Within a data transfer block, the byte offset is `data_offset*block_k*block_m` for A or `data_offset*block_k*block_n` for B.
- Within a scale transfer block, the byte offset is `scale_offset_idx*(block_k/32)*16`. The source stride is `scale_chunk/2=16`, and the destination stride is `block_k/64=2`.
- The MX destination address is the corresponding data address in L0A Buffer or L0B Buffer divided by 16.

```cpp
asc_copy_l12l0a(a_dst, a_l1 + data_offset * block_k * block_m,
    0, 0, block_m / 16, block_k / 32, block_m / 16, block_m / 16);
asc_copy_l12l0b(b_dst, b_l1 + data_offset * block_k * block_n,
    0, 0, block_n / 16, block_k / 32, block_n / 16, block_n / 16);

uint32_t scale_offset = scale_offset_idx * (block_k / 32) * 16;
asc_copy_l12l0a_mx((uint64_t)a_dst / 16, scale_a_l1 + scale_offset,
    0, 0, block_m / 16, block_k / 64, scale_chunk / 2, block_k / 64);
asc_copy_l12l0b_mx((uint64_t)b_dst / 16, scale_b_l1 + scale_offset,
    0, 0, block_n / 16, block_k / 64, scale_chunk / 2, block_k / 64);
```

#### 4. Double Buffering in L1 Buffer, L0A Buffer, and L0B Buffer: Overlap Loading and Computation

The sample uses Ping/Pong double buffering in L1 Buffer, L0A Buffer, and L0B Buffer to overlap data movement, loading, and computation and reduce waiting.

The main buffer layout is as follows:

| Memory Level | Buffer | Contents | Purpose |
|------|--------|------|------|
| L1 Buffer | `a_l1 Ping/Pong` | Large A data blocks | Store A data moved from GM to L1 Buffer |
| L1 Buffer | `b_l1 Ping/Pong` | Large B data blocks | Store B data moved from GM to L1 Buffer |
| L1 Buffer | `scale_a_l1 Ping/Pong` | Scales for A | Store quantization scales for A |
| L1 Buffer | `scale_b_l1 Ping/Pong` | Scales for B | Store quantization scales for B |
| L0A Buffer | `a_l0 Ping/Pong` | Current K block of A | Left operand for Cube computation |
| L0B Buffer | `b_l0 Ping/Pong` | Current K block of B | Right operand for Cube computation |
| L0C Buffer | `c_l0` | Float accumulation result | Store the result for the output data-movement instruction to write back to GM |

During the K loop, data movement for the next batch of A/B/scaleA/scaleB can start while Cube computes the current K block. Ping and Pong are used alternately. Producers and consumers use event synchronization to determine whether a buffer can be written and whether its data is ready to read.

#### 5. Cube Computation: Accumulate Across K Blocks

`compute_block` waits for the data in L0A Buffer and L0B Buffer to become ready, then performs one 256×128×256 MX matrix multiply-accumulate operation. The first iteration initializes C; subsequent iterations accumulate:

```cpp
asc_mmad_mx(c_l0, a_dst, b_dst, block_m, block_k, block_n,
    asc_unit_flag_mode::DISABLE, false, false, k_idx == 0);
```

Each output block requires 64 MMAD operations. After each iteration, an M→MTE1 event is signaled so that the corresponding Ping/Pong buffers in L0A Buffer and L0B Buffer can be reused.

#### 6. Fixpipe Writeback: Convert float to half for Output

`copy_result_to_gm` waits for Cube computation to complete, converts the NZ layout in L0C Buffer to row-major ND, and uses [`F322F16`](../../../../../docs/zh/api/SIMD-API/c_api/cube_datamove/cube_compute_store/asc_copy_l0c2gm.md) to output half data:

```cpp
asc_set_l0c_copy_nz_para(1, 0, 0);
asc_copy_l0c2gm(dst, c_l0, block_n, block_m, matrix_n, block_m,
    asc_store_l2_cache_mode::NORMAL_FIRST_VICTIM, asc_unit_flag_mode::DISABLE,
    QuantMode_t::F322F16, asc_relu_pre_mode::NONE, false, true, false, false);
```

The destination row stride is 8192 half elements, the source NZ stride is block_m=256, and the output address is `c+m*matrix_n+n`.

A FIX→M event is signaled after writeback. The wait is deferred until the first MMAD of the next output block, allowing data movement into L1 Buffer, L0A Buffer, and L0B Buffer to start earlier while preventing L0C Buffer from being overwritten before FIX finishes reading it.

### Event Synchronization: Enable Pipeline Parallelism

[asc_sync_notify / asc_sync_wait](../../../../../docs/zh/api/SIMD-API/c_api/sync/intra_core_sync/asc_sync_notify.md) provide synchronization between different pipelines within a core.

| Direction | Event ID | Meaning |
| --- | --- | --- |
| MTE2→MTE1 | 0/1 | A/B Ping/Pong data is ready |
| MTE1→MTE2 | 0/1 | A/B Ping/Pong data can be overwritten |
| MTE2→MTE1 | 2/3 | Ping/Pong scale data is ready |
| MTE1→MTE2 | 2/3 | Ping/Pong scale data can be overwritten |
| MTE1→M | 0/1 | Ping/Pong data and scales for L0A Buffer and L0B Buffer are ready |
| M→MTE1 | 0/1 | Cube has consumed the Ping/Pong data in L0A Buffer and L0B Buffer |
| M→FIX | 0 | Accumulation for the complete output block has finished |
| FIX→M | 0 | L0C Buffer is available for the next output block |

Events are used independently for each pipeline direction. At kernel entry, MTE1→MTE2 events 0–3, M→MTE1 events 0/1, and FIX→M event 0 are signaled so that the first writes do not wait for consumers that have not yet run. At kernel exit, all remaining reverse-direction events are consumed, followed by `asc_sync_pipe(PIPE_ALL)`.

For each data or scale transfer block, readiness is checked only in its first iteration, and the buffer is released in its last iteration. A and B share the data events, while both scale inputs share the scale events. Tracking data and scale transfer progress independently avoids reloading scales from GM in every iteration.

## Performance Optimization Summary

- Use a fixed set of 32 cores in a 4×8 grid. Each core computes the full K dimension, avoiding cross-core reduction.
- Move A/B data in groups of two basic K blocks and scales in groups of eight basic K blocks to reduce the number of MTE2 instructions.
- Retain 256×256 output blocks to improve input reuse. The basic K length of 128 allows FP8 data to fit in the double buffers in L0A Buffer and L0B Buffer.
- Use separate Ping/Pong buffers and explicit synchronization events for L1 Buffer data, L1 Buffer scales, and the operands in L0A Buffer and L0B Buffer to overlap data movement and computation.
- Defer the wait for L0C Buffer reuse so that preloading inputs for the next output block can overlap writeback of the previous block.

## Comparison with Theoretical Performance

The following performance data was collected on Ascend 950PR&950DT series products with **32 available Cube cores running at 1.65 GHz**.

Performance varies across environments. These results are for reference only; actual performance must be measured on the target device.

| Metric | Description |
| --- | --- |
| Task Duration(μs) | Total kernel task duration; use this metric for end-to-end operator performance |
| Block Num | Fixed number of blocks launched |
| aicore_time(μs) | AI Core execution time reported by the profiler |
| aic_mac_time(μs) / aic_mac_ratio | Cube computation time and ratio |
| aic_scalar_time(μs) / aic_scalar_ratio | Time and ratio for scalar scheduling, address calculation, and related operations |
| aic_mte1_time(μs) / aic_mte1_ratio | Time and ratio for data movement from L1 Buffer to L0A Buffer and L0B Buffer |
| aic_mte2_time(μs) / aic_mte2_ratio | Time and ratio for data movement from GM to L1 Buffer |
| aic_fixpipe_time(μs) / aic_fixpipe_ratio | Output writeback time and ratio |

Measured results:

| Case version | Task Duration(μs) | Block Num | aicore_time(μs) | aic_mac_time(μs) | aic_mac_ratio | aic_scalar_time(μs) | aic_scalar_ratio | aic_mte1_time(μs) | aic_mte1_ratio | aic_mte2_time(μs) | aic_mte2_ratio | aic_fixpipe_time(μs) | aic_fixpipe_ratio |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| C API MXFP8 Matmul | 1318.15 | 32 | 1317.12 | 1277.07 | 0.97 | 67.57 | 0.05 | 552.28 | 0.42 | 1185.64 | 0.9 | 32.26 | 0.02 |

Pipeline execution can overlap, so pipeline times must not be added together to obtain task duration. The Cube time ratio is not the global utilization of all physical cores.

### Cube Computation Performance Analysis

For FP8 data, each core processes 16×32×16 multiply-add operations per cycle.

The theoretical Cube computation time $T_{cube}$ is:

$$T_{cube}=\frac{M\times N\times K}{16\times32\times16\times1.65\times10^9\times32}
=\frac{8192^3}{8192\times1.65\times10^9\times32}\approx1271.001\ \mu s$$

This is a theoretical lower bound for computation alone. It excludes pipeline fill, drain, writeback, and other overhead.

The measured Cube time is `1277.07 μs`. Its relative deviation from the theoretical value is:

$$E_{cube}=\frac{1277.07-1271.001}{1271.001}\times100\%\approx0.48\%$$

### MTE2 Bandwidth Analysis

Each row block of A participates in `N/block_n=32` output blocks, and each row block of B participates in `M/block_m=32` output blocks. Scales have the same reuse counts. Each FP8 element occupies one byte, so the total logical data volume moved from GM to L1 Buffer is:

$$D_{total}=\frac{N}{block_n}MK+\frac{M}{block_m}NK+
\frac{N}{block_n}M\cdot scale_k+\frac{M}{block_m}N\cdot scale_k$$

$$=2\ GiB+2\ GiB+64\ MiB+64\ MiB=4.125\ GiB=4,429,185,024\ B$$

The input data volume without counting repeated accesses is:

$$D_{input}=MK+NK+M\cdot scale_k+N\cdot scale_k=132\ MiB=138,412,032\ B$$

If each input byte is read from HBM only on its first access and all subsequent accesses hit L2, the subsequent access volume is `D_total-D_input=4,290,772,992 B`. Using the bandwidth assumptions from the original sample—1.6 TB/s for HBM and 5 TB/s for L2—the ideal data-movement time can be estimated as:

$$T_{MTE2,model}=\frac{138,412,032}{1.6\times10^{12}}+
\frac{4,290,772,992}{5\times10^{12}}\approx944.662\ \mu s$$

Capacity uses binary MiB/GiB, while bandwidth uses decimal TB/s. These bandwidth values are model assumptions and do not replace measurements on the target device. HBM and L2 transfers can overlap, and the model excludes format conversion, instruction overhead, and cache conflicts. The 4.125 GiB value is the logical data-movement volume, not actual HBM traffic. With the 128 MiB L2 capacity assumed by the original sample, the 132 MiB input cannot all remain in L2. Output and other traffic also affect cache usage, so the actual hit rate must be verified with the profiler.

The measured `aic_mte2_time` is `1185.64 μs`, approximately `25.51%` higher than the model value of `944.662 μs`. The difference comes from cache hit behavior, ND/DN-to-NZ data movement, and instruction and synchronization overhead. MTE2 overlaps other pipelines, so its time cannot be added directly to the Cube time.

## Build and Run

Perform the following steps from the sample root directory to build and run the sample.

### Configure Environment Variables

Configure the environment variables according to the [installation method](../../../../../docs/zh/quick_start.md#prepare&install) used for the CANN development toolkit in your environment.

```bash
source ${install_path}/cann/set_env.sh
```

> **Note:** `${install_path}` is the CANN package installation directory. If no installation directory is specified, the default is `/usr/local/Ascend`.

### Run the Sample

Run the following commands in the sample directory:

```bash
mkdir -p build && cd build;  # Create and enter the build directory
cmake .. -DCMAKE_ASC_RUN_MODE=npu -DCMAKE_ASC_ARCHITECTURES=dav-3510; make -j;
python3 ../scripts/gen_data.py
./demo
python3 ../scripts/verify_result.py ./output/output.bin ./output/golden.bin
```

For NPU simulation, set `-DCMAKE_ASC_RUN_MODE=sim`.

```bash
cmake .. -DCMAKE_ASC_RUN_MODE=npu -DCMAKE_ASC_ARCHITECTURES=dav-3510; make -j; # NPU execution
cmake .. -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-3510; make -j; # NPU simulation
```

Build options:

| Option | Supported Values | Description |
|------|--------|------|
| `CMAKE_ASC_RUN_MODE` | `npu` / `sim` | Run mode: NPU execution or NPU simulation |
| `CMAKE_ASC_ARCHITECTURES` | `dav-3510` | Target SoC architecture |

> **Note:** Clear the CMake cache before changing `CMAKE_ASC_RUN_MODE` or `CMAKE_ASC_ARCHITECTURES`. Run `rm CMakeCache.txt` in the build directory, then run CMake again.

Successful result verification prints:

```bash
test pass!
```

### Performance Analysis

### msOpProf Tool

msOpProf is a single-operator performance analysis tool with two usage modes: `msopprof` and `msopprof simulator`. It helps locate issues in operator memory access, code, and instructions for comprehensive operator tuning. It supports performance data collection and automatic parsing for different run modes (NPU execution or simulation) and file formats (executables or operator binary .o files).

- Performance collection on an NPU

  Collecting performance data on an NPU directly measures the operator execution time on an Ascend AI Processor. This method is suitable for quickly locating operator performance issues on the device.

  Use msopprof with the executable `demo` to analyze operator performance:

  ```bash
  msopprof ./demo
  ```

  - Performance data description

    After the command completes, a folder named `OPPROF_{timestamp}_XXX` is generated in the default directory. The following shows an example of its contents:

    ```text
    ├──dump                       # Raw performance data; no user action is required
    ├──ArithmeticUtilization.csv  # Cycle ratios of Cube/Vector instructions
    ├──L2Cache.csv                # L2 Cache hit rate, which affects MTE2; plan data movement to improve the hit rate
    ├──Memory.csv                 # Read/write bandwidth for UB, L1 Buffer, and main memory
    ├──MemoryL0.csv               # Read/write bandwidth for L0A Buffer, L0B Buffer, and L0C Buffer
    ├──MemoryUB.csv               # Read/write bandwidth from Vector and Scalar to UB
    ├──OpBasicInfo.csv            # Basic operator information
    ├──PipeUtilization.csv        # Duration and ratios for computation and data-movement units
    ├──ResourceConflictRatio.csv  # Ratios of UB bank group conflicts, bank conflicts, and resource conflicts across all instructions
    └──visualize_data.bin         # Visualization data for MindStudio Insight
    ```

View the detailed performance analysis results:

```bash
cat ./OPPROF_*/PipeUtilization.csv
```
