# Matmul and LeakyRelu Fusion Example Based on the TPipe-TQue Framework

## Overview

This example implements fused Matmul and LeakyRelu using the [TPipe and TQue programming paradigm](../../../../../docs/zh/guide/programming_guide/programming_model/ai_core_simd_programming/tpipe_tque_programming/tpipe_tque_paradigm.md), with TPipe managing local resources and TQue managing data synchronization and buffer reuse. [Double Buffer](../../../../../docs/zh/guide/technical_appendix/concepts_and_terms/performance_optimization/double_buffer.md) is enabled to demonstrate cooperative computation between the Cube and Vector units. Cube cores perform matrix multiplication, and Vector cores apply LeakyRelu.

In this example, the A1 (physically in L1 Buffer), A2 (L0A Buffer), CO1 (L0C Buffer), VECIN (UB) and VECOUT (UB) queues enable Double Buffer so that transfers and computation for different tiles can overlap. The right-hand matrix B is reused across M tiles, loaded only once and retained in L0B Buffer.

## Supported Products and CANN Versions

| Product | CANN Version |
|---------|--------------|
| Ascend 950PR&950DT Series Products | >= CANN 9.3.0 |
| Atlas A3 Series Products | >= CANN 9.3.0 |
| Atlas A2 Series Products | >= CANN 9.3.0 |

## Directory Structure

```text
tpipe_matmul_leakyrelu_basic_api/
├── scripts/
│   ├── gen_data.py                       # Generate inputs and reference data
│   └── verify_result.py                  # Compare output with the reference
├── CMakeLists.txt                        # Build configuration
├── data_utils.h                          # File I/O helpers
├── tpipe_matmul_leakyrelu_basic_api.asc  # Device implementation and host invocation
├── README.md                             # Chinese documentation
└── README_en.md                          # English documentation
```

## Operator Description

- **Function**

  The mathematical operation is:

  $$
  C_{ij}=\sum_k A_{ik}B_{kj}
  $$

  followed by:

  $$
  C_{ij}=\begin{cases}
  C_{ij}, & C_{ij}\geq0,\\
  C_{ij}\times0.001, & C_{ij}<0.
  \end{cases}
  $$

  A has shape [M, K], B has shape [K, N], and C has shape [M, N]. In the implementation, Mmad accumulates into FP32, Fixpipe converts the result to FP16, and LeakyRelu operates on FP16 with slope `(half)0.001`. Applying activation in FP32 before converting to FP16 would use a different rounding order. Fixpipe must keep `reluEn=false` to preserve negative values for LeakyRelu.

- **Specifications**

  | Input/output | Name | Shape | Type | Format |
  |--------------|------|-------|------|--------|
  | Input | A | [512, 128] | half | ND |
  | Input | B | [128, 128] | half | ND |
  | Output | C | [512, 128] | half | ND |

  Kernel name: `mmad_vec_custom`. The example launches two Cube cores and four Vector cores.

  **Core partitioning**

  C is partitioned along M only. Each Cube core processes `singleCoreM=256` rows as two tiles with `baseM=128`. For each tile, its two paired Vector cores each process `baseM/2 × baseN = 64 × 128` elements. K and N are not partitioned: `baseK=K=128` and `baseN=N=128`.

  **M/N/K diagram**

  ```text
           K                N
       +---------+      +----------+
       |         |      |          |
     M |    A    |  x K |    B     |  = C (M x N)
       |         |      |          |
       +---------+      +----------+
  ```

  - M partitions: `M / singleCoreM = 512 / 256 = 2`.
  - N partitions: `N / singleCoreN = 128 / 128 = 1`.
  - Cube cores: `2 × 1 = 2`; Vector cores: `2 × 2 = 4`.
  - Tiles per Cube core: `singleCoreM / baseM = 2`.

  | Cube core | mIndex | Global output rows, inclusive | First paired Vector core | Second paired Vector core |
  |-----------|--------|-------------------------------|--------------------------|---------------------------|
  | AIC0 | 0 | 0–127 | AIV0: 0–63 | AIV1: 64–127 |
  | AIC0 | 1 | 128–255 | AIV0: 128–191 | AIV1: 192–255 |
  | AIC1 | 0 | 256–383 | AIV2: 256–319 | AIV3: 320–383 |
  | AIC1 | 1 | 384–511 | AIV2: 384–447 | AIV3: 448–511 |

- **Implementation**

  **Overall flow and memory hierarchy**

  | Physical memory | Logical position in this example | Contents and data path |
  |-----------------|----------------------------------|------------------------|
  | GM | GlobalTensor | Inputs and intermediate/final C, ND half |
  | L1 | A1, B1 | NZ half inputs, MTE2 → MTE1 |
  | L0A | A2 | Left operand, MTE1 → M |
  | L0B | B2 | ZN half right operand, MTE1 → M |
  | L0C | CO1 | FP32 accumulator, M → FIX |
  | UB | VECIN | Vector input, MTE2 → V |
  | UB | VECOUT | Vector output, V → MTE3 |

  ND is ordinary row-major storage. NZ/ZZ/ZN, also written Nz/Zz/Zn, are Cube fractal layouts. A 16×16 half fractal occupies 512 bytes. The layout names describe both the organization of fractals and their internal layout; they are not simply aliases for transposing an ordinary ND matrix.

  Dependencies within a core are managed by [TPipe](../../../../../docs/zh/api/SIMD-API/basic_api/resource_management/TPipe/TPipe.md) and [TQue](../../../../../docs/zh/api/SIMD-API/basic_api/resource_management/TQue/TQue.md). EnQue/DeQue establish producer-to-consumer dependencies; FreeTensor/AllocTensor protect reuse. No local HardEvent IDs are assigned manually.

  Cross-core dependencies use [CrossCoreSetFlag](../../../../../docs/zh/api/SIMD-API/basic_api/sync_control/inter_core_sync/CrossCoreSetFlag_ISASI.md) and [CrossCoreWaitFlag](../../../../../docs/zh/api/SIMD-API/basic_api/sync_control/inter_core_sync/CrossCoreWaitFlag_ISASI.md).

  **1. Data paths**

  ```text
  GM(A:ND,half) -> L1(A1:NZ) -> L0A(A2:ZZ/NZ) --+
                   DataCopy       LoadData      |
                                                Mmad -> L0C(CO1,float)
  GM(B:ND,half) -> L1(B1:NZ) -> L0B(B2:ZN) -----+             |
                   DataCopy       LoadData                  Fixpipe
                                                        ND, float -> half
                                                             |
                                                        GM(C:ND,half)
  ```

  L0A uses ZZ on 2201 and NZ on 3510. Both branches load B into ZN. The Vector path for each half tile is:

  ```text
  FIX publishes READY -> AIV waits -> GM(C) -> UB(VECIN) -> UB(VECOUT) -> GM(C)
                                     DataCopyPad        LeakyRelu     DataCopyPad
                                        MTE2               V            MTE3
                                          |
                                 publish FREE after read
  ```

  **2. Detailed stages**

  **Cube computation: GlobalTensor and core offsets**

  `GlobalTensor<half>` offsets are measured in elements, not bytes. Cube A/C base addresses advance by `GetBlockIdx()*singleCoreM*K` and `GetBlockIdx()*singleCoreM*N`. On the Vector side, `GetBlockIdx()/2` maps to the paired Cube core. Both Cube cores load the complete B matrix without an M-dependent offset.

  A single TPipe instance is created at the Kernel entry and passed to the operator through `Init(..., &pipe)`; the class stores only its pointer. This follows [Avoid Creating and Initializing TPipe Inside an Object](../../../../../docs/zh/guide/operator_practice/simd_operator_optimization/overhead_optimization/avoid_tpipe_init_in_object.md) and the [Reduce example](../../01_memory_vector_compute/reduce/reduce.asc), allowing constant folding and propagation of Scalar variables in the operator class.

  The queue members and shared constants match the source. Tile dimensions in these excerpts are supplied by the nine template parameters of KernelMatmul. Most comments are omitted from the code excerpts; the complete template is in the [source](tpipe_matmul_leakyrelu_basic_api.asc).

```cpp
constexpr uint32_t CUBE_BLOCK = 16;
constexpr uint32_t BUFFER_NUM = 2;
constexpr uint16_t READY_FLAG = 8;
constexpr uint16_t FREE_FLAG = 10;
AscendC::TPipe* pipe;
AscendC::TQue<AscendC::TPosition::A1, BUFFER_NUM> a1Queue;
AscendC::TQue<AscendC::TPosition::B1, 1> b1Queue;
AscendC::TQue<AscendC::TPosition::A2, 1> a2Queue;
AscendC::TQue<AscendC::TPosition::B2, 1> b2Queue;
AscendC::TQue<AscendC::TPosition::CO1, 1> cQueue;
AscendC::TQue<AscendC::TPosition::VECIN, 1> vecInQueue;
AscendC::TQue<AscendC::TPosition::VECOUT, 1> vecOutQueue;
```

  The following is the template class's Init method:

```cpp
__aicore__ inline void Init(
    __gm__ uint8_t* xMatrix, __gm__ uint8_t* yMatrix, __gm__ uint8_t* zMatrix, AscendC::TPipe* pipeIn)
{
    pipe = pipeIn;
    static_assert(M % singleCoreM == 0 && singleCoreM % baseM == 0, "M must be divisible by tiles");
    static_assert(
        singleCoreK == K && baseK == K && singleCoreN == N && baseN == N, "This sample only partitions M");
    static_assert(baseM == 32 || baseM == 64 || baseM == 128, "Supported baseM: 32, 64, 128");
    static_assert(K == 128 && N == 128, "This sample requires K=N=128");
    static_assert(singleCoreM / baseM >= BUFFER_NUM, "At least two tiles are required for double buffer");
    aGM.SetGlobalBuffer((__gm__ half*)xMatrix);
    bGM.SetGlobalBuffer((__gm__ half*)yMatrix);
    cGM.SetGlobalBuffer((__gm__ half*)zMatrix);
    if ASCEND_IS_AIC {
        aGM = aGM[AscendC::GetBlockIdx() * singleCoreM * K];
        cGM = cGM[AscendC::GetBlockIdx() * singleCoreM * N];
        pipe->InitBuffer(a1Queue, BUFFER_NUM, baseM * baseK * sizeof(half));
        pipe->InitBuffer(a2Queue, BUFFER_NUM, baseM * baseK * sizeof(half));
        pipe->InitBuffer(cQueue, BUFFER_NUM, baseM * baseN * sizeof(float));
        pipe->InitBuffer(b1Queue, 1, baseK * baseN * sizeof(half));
        pipe->InitBuffer(b2Queue, 1, baseK * baseN * sizeof(half));
    }
    if ASCEND_IS_AIV {
        cGM = cGM[AscendC::GetBlockIdx() / 2 * singleCoreM * N];
        pipe->InitBuffer(vecInQueue, BUFFER_NUM, baseM / 2 * baseN * sizeof(half));
        pipe->InitBuffer(vecOutQueue, BUFFER_NUM, baseM / 2 * baseN * sizeof(half));
    }
}
```

  **Memory transfers and computation**

  - **LocalTensor allocation:** InitBuffer initializes the queues. AllocTensor obtains a tensor, EnQue publishes the producer operation, DeQue establishes the consumer dependency, and FreeTensor returns ownership for later reuse.
  - **GM → L1:** DataCopy loads B once and prefetches two A tiles while converting ND to NZ.
  - **L1 → L0A/L0B:** LoadData loads each A tile and loads B once, with transpose, retaining B2 throughout the M-tile loop.
  - **L0A/L0B → L0C:** Each Mmad computes the complete K reduction for one M tile. Each call initializes a fresh accumulator; there is no accumulation across K tiles.
  - **L0C → GM:** Fixpipe converts the result to ND half and writes the current tile's GM region.

  The Cube branch of Process is shown below. The Vector branch is described separately afterward:

```cpp
__aicore__ inline void Process()
{
    constexpr uint32_t mLoops = singleCoreM / baseM;
    if ASCEND_IS_AIC {
        AscendC::LocalTensor<half> b1Local = b1Queue.AllocTensor<half>();
        AscendC::DataCopy(b1Local, bGM, AscendC::Nd2NzParams{1, baseK, baseN, 0, N, baseK, 1, 0});
        b1Queue.EnQue(b1Local);
        for (uint32_t mIndex = 0; mIndex < BUFFER_NUM; ++mIndex) {
            CopyInA(mIndex);
        }
        LoadB();
        AscendC::LocalTensor<half> b2Local = b2Queue.DeQue<half>();
        for (uint32_t mIndex = 0; mIndex < mLoops; ++mIndex) {
            AscendC::LocalTensor<half> a1Local = a1Queue.DeQue<half>();
            AscendC::LocalTensor<half> a2Local = a2Queue.AllocTensor<half>();
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 2201)
            for (int i = 0; i < baseM / CUBE_BLOCK; ++i) {
                AscendC::LoadData(
                    a2Local[i * baseK * CUBE_BLOCK], a1Local[i * 512 / sizeof(half)],
                    AscendC::LoadData2DParams{0, baseK / CUBE_BLOCK, baseM / CUBE_BLOCK, 0, 0, false, 0});
            }
#elif defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
            AscendC::LoadData(
                a2Local, a1Local,
                AscendC::LoadData2DParamsV2{
                    0, 0, baseM / CUBE_BLOCK, baseK / CUBE_BLOCK, baseM / CUBE_BLOCK, baseM / CUBE_BLOCK, false,
                    0});
#endif
            a2Queue.EnQue(a2Local);
            a1Queue.FreeTensor(a1Local);
            if (mIndex + BUFFER_NUM < mLoops) {
                CopyInA(mIndex + BUFFER_NUM);
            }
            a2Local = a2Queue.DeQue<half>();
            AscendC::LocalTensor<float> cLocal = cQueue.AllocTensor<float>();
            AscendC::Mmad(cLocal, a2Local, b2Local, AscendC::MmadParams{baseM, baseN, baseK, 0, false, true});
            cQueue.EnQue(cLocal);
            a2Queue.FreeTensor(a2Local);
            cLocal = cQueue.DeQue<float>();
            uint32_t flagOffset = mIndex % BUFFER_NUM;
            if (mIndex >= BUFFER_NUM) {
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
                AscendC::CrossCoreWaitFlag<0x2, PIPE_FIX>(FREE_FLAG + flagOffset);
#else
                AscendC::CrossCoreWaitFlag<0x2>(FREE_FLAG + flagOffset);
#endif
            }
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 2201)
            AscendC::FixpipeParamsV220 fixpipeParams{baseN, baseM, baseM, N, false, QuantMode_t::F322F16,
                                                     0,     1,     0,     0, 0};
#elif defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
            AscendC::FixpipeParamsArch3510<AscendC::CO2Layout::ROW_MAJOR> fixpipeParams{baseN, baseM, baseM, N};
            fixpipeParams.quantPre = QuantMode_t::F322F16;
            fixpipeParams.reluEn = false;
            fixpipeParams.params = {1, 0, 0};
#endif
            AscendC::Fixpipe<half, float, AscendC::CFG_ROW_MAJOR>(cGM[mIndex * baseM * N], cLocal, fixpipeParams);
            AscendC::CrossCoreSetFlag<0x2, PIPE_FIX>(READY_FLAG + flagOffset);
            cQueue.FreeTensor(cLocal);
        }
        for (uint32_t flagOffset = 0; flagOffset < BUFFER_NUM; ++flagOffset) {
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
            AscendC::CrossCoreWaitFlag<0x2, PIPE_FIX>(FREE_FLAG + flagOffset);
#else
            AscendC::CrossCoreWaitFlag<0x2>(FREE_FLAG + flagOffset);
#endif
        }
        b2Queue.FreeTensor(b2Local);
    }
}
```

  Private helpers handle A prefetch and architecture-specific B loading:

```cpp
__aicore__ inline void CopyInA(uint32_t mIndex)
{
    AscendC::LocalTensor<half> a1Local = a1Queue.AllocTensor<half>();
    AscendC::DataCopy(a1Local, aGM[mIndex * baseM * K], AscendC::Nd2NzParams{1, baseM, baseK, 0, K, baseM, 1, 0});
    a1Queue.EnQue(a1Local);
}
// Transfer B from the B1 queue's L1 memory block to the B2 queue's L0B memory block, converting Nz to Zn.
// Enqueue the B2 tensor and release the B1 tensor. Process calls this function once before the M-tile loop,
// then dequeues B2 for reuse by Mmad across all M tiles and releases it after the loop.
__aicore__ inline void LoadB()
{
    AscendC::LocalTensor<half> b1Local = b1Queue.DeQue<half>();
    AscendC::LocalTensor<half> b2Local = b2Queue.AllocTensor<half>();
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 2201)
    for (int i = 0; i < baseK / CUBE_BLOCK; ++i) {
        AscendC::LoadData(
            b2Local[i * baseN * CUBE_BLOCK], b1Local[i * 512 / sizeof(half)],
            AscendC::LoadData2DParams{0, baseN / CUBE_BLOCK, baseK / CUBE_BLOCK, 0, 0, true, 0});
    }
#elif defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
    AscendC::LoadData(
        b2Local, b1Local,
        AscendC::LoadData2DParamsV2{
            0, 0, baseK / CUBE_BLOCK, baseN / CUBE_BLOCK, baseK / CUBE_BLOCK, baseN / CUBE_BLOCK, true, 0});
#endif
    b2Queue.EnQue(b2Local);
    b1Queue.FreeTensor(b1Local);
}
```

  **Vector computation**

  - **Cross-core synchronization:** Wait for READY for the current tile. After reading, publish FREE from MTE2; the Cube core waits for both paired AIVs before reusing that flagId.
  - **LocalTensor allocation:** Separate VECIN and VECOUT queues each have two memory blocks.
  - **GM → UB:** DataCopyPad reads `baseM/2` rows of `baseN` half elements.
  - **UB computation:** LeakyRelu writes a separate output tensor and applies the FP16 negative slope.
  - **UB → GM:** DataCopyPad writes the same half-tile region; release the output tensor and continue to the next tile.

```cpp
if ASCEND_IS_AIV {
    for (uint32_t mIndex = 0; mIndex < mLoops; ++mIndex) {
        LeakyReluCompute(mIndex, 0, baseM / 2 * baseN);
    }
}
```

```cpp
__aicore__ inline void LeakyReluCompute(uint32_t mIndex, uint32_t nIndex, uint32_t len)
{
    uint32_t blockLen = baseN * sizeof(half);
    uint32_t srcStride = (N - baseN) * sizeof(half);
    uint32_t gmOffset = (mIndex * baseM * N + nIndex * baseN) + AscendC::GetBlockIdx() % 2 * (baseM / 2 * N);
    uint32_t flagOffset = mIndex % BUFFER_NUM;
    AscendC::LocalTensor<half> vecInLocal = vecInQueue.AllocTensor<half>();
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
    AscendC::CrossCoreWaitFlag<0x2, PIPE_MTE2>(READY_FLAG + flagOffset);
#else
    AscendC::CrossCoreWaitFlag<0x2>(READY_FLAG + flagOffset);
#endif
    AscendC::DataCopyPad<half>(
        vecInLocal, cGM[gmOffset],
        AscendC::DataCopyExtParams{static_cast<uint16_t>(baseM / 2), blockLen, srcStride, 0, 0},
        AscendC::DataCopyPadExtParams<half>{true, 0, 0, 0});
    AscendC::CrossCoreSetFlag<0x2, PIPE_MTE2>(FREE_FLAG + flagOffset);
    vecInQueue.EnQue(vecInLocal);
    vecInLocal = vecInQueue.DeQue<half>();
    AscendC::LocalTensor<half> vecOutLocal = vecOutQueue.AllocTensor<half>();
    AscendC::LeakyRelu(vecOutLocal, vecInLocal, (half)0.001, len);
    vecInQueue.FreeTensor(vecInLocal);
    vecOutQueue.EnQue(vecOutLocal);
    vecOutLocal = vecOutQueue.DeQue<half>();
    AscendC::DataCopyPad<half>(
        cGM[gmOffset], vecOutLocal,
        AscendC::DataCopyExtParams{static_cast<uint16_t>(baseM / 2), blockLen, 0, srcStride, 0});
    vecOutQueue.FreeTensor(vecOutLocal);
}
```

  `gmOffset` contains both the tile offset and the upper/lower-half offset. `nIndex` is always zero because N is not tiled. Each transfer contains 64 rows of 256 bytes, or 16 KiB. The two AIVs write disjoint rows.

  **Constraints**

  1. `K=singleCoreK=baseK=128` and `N=singleCoreN=baseN=128`; only M is partitioned.
  2. `baseM` is 32, 64 or 128, with 128 as the default. Cube tile dimensions are multiples of 16.
  3. M must be divisible by singleCoreM, and singleCoreM by baseM. There is no tail handling.
  4. `singleCoreM/baseM >= BUFFER_NUM=2` so initial prefetch does not access a nonexistent tile.
  5. The Cube-to-Vector ratio is 1:2. Changing it requires changes to address mapping and synchronization.
  6. Inputs and output are contiguous ND half matrices. The generator uses fixed M/K/N=512/128/128; the template signature does not imply arbitrary-shape support.

  **Kernel invocation**

  `__global__` declares the device entry point. `__mix__(1, 2)` specifies one Cube core and two paired Vector cores per logical block. The Kernel entry creates the TPipe instance and passes its pointer to the operator:

```cpp
template <
    uint32_t M, uint32_t K, uint32_t N, uint32_t singleCoreM, uint32_t singleCoreK, uint32_t singleCoreN,
    uint32_t baseM, uint32_t baseK, uint32_t baseN>
__global__ __mix__(1, 2) void mmad_vec_custom(__gm__ uint8_t* xMatrix, __gm__ uint8_t* yMatrix, __gm__ uint8_t* zMatrix)
{
    AscendC::TPipe pipe;
    KernelMatmul<M, K, N, singleCoreM, singleCoreK, singleCoreN, baseM, baseK, baseN> op;
    op.Init(xMatrix, yMatrix, zMatrix, &pipe);
    op.Process();
}
```

- **API parameters**

  The tables below describe constructor arguments in brace-initialized calls and explicitly assigned member fields. That order may differ from member declaration order. Some field types vary by architecture, so these are semantic descriptions rather than complete struct declarations. Consult the linked repository documentation for target-specific ranges. The links use the repository's Chinese API references.

  **TPipe and TQue**

  | API | Meaning in this example |
  |-----|-------------------------|
  | `TQue<position, depth>` | Logical position and maximum queued tensors awaiting dequeue; A1 depth is 2, others 1 |
  | `pipe->InitBuffer(queue, num, len)` | num is the number of allocated memory blocks; len is the size of each block in bytes; double buffering uses num=2 |
  | `AllocTensor<T>()` | Obtain a writable local tensor with the appropriate element type |
  | `EnQue(tensor)` | Publish a producer operation and its dependency |
  | `DeQue<T>()` | Obtain a tensor in queue order and establish the consumer dependency |
  | `FreeTensor(tensor)` | Return ownership; future reuse remains protected against outstanding reads |

  For example, `pipe->InitBuffer(a2Queue, 2, baseM*baseK*sizeof(half))` allocates two 32 KiB memory blocks in L0A even though a2Queue has template depth 1.

  **[Nd2NzParams](../../../../../docs/zh/api/SIMD-API/basic_api/cube_compute_ISASI/cube_compute_load/DataCopy_GMToL1_ND2NZ.md)** — GM-to-L1 ND-to-NZ conversion:

  | Constructor parameter | Meaning |
  |-----------------------|---------|
  | ndNum | Number of ND matrices; 1 here |
  | nValue | ND row count |
  | dValue | ND column count |
  | srcNdMatrixStride | Distance between source matrix starts, in elements; unused for one matrix |
  | srcDValue | Distance between source row starts, in elements |
  | dstNzC0Stride | Destination NZ stride between column-fractal groups, in 32-byte units |
  | dstNzNStride | Destination NZ stride corresponding to adjacent source rows, in 32-byte units |
  | dstNzMatrixStride | Distance between destination matrices, in elements; zero for this single matrix |

  A uses `{1, baseM, baseK, 0, K, baseM, 1, 0}`; B uses `{1, baseK, baseN, 0, N, baseK, 1, 0}`. Source row strides describe the complete GM matrix, not merely the tile width.

  **[LoadData2DParams](../../../../../docs/zh/api/SIMD-API/basic_api/cube_compute_ISASI/cube_compute_load/LoadData_2D.md)** — 2201 L1-to-L0 loading:

  | Constructor parameter | Meaning |
  |-----------------------|---------|
  | startIndex | Starting fractal index relative to the supplied tensor, in 512-byte units for half |
  | repeatTimes | Number of fractals loaded per call |
  | srcStride | Distance between successive source fractal starts, in 512-byte units |
  | sid | Reserved setting; 0 |
  | dstGap | Gap between successive destination fractals, in 512-byte units |
  | ifTranspose | Whether to transpose each fractal; false for A, true for B |
  | addrMode | Address update mode; 0 for increasing addresses |

  Each A call uses `{0, baseK/16, baseM/16, 0, 0, false, 0}`. Each B call uses `{0, baseN/16, baseK/16, 0, 0, true, 0}`. Source tensor slices advance by `512/sizeof(half)=256` elements per loop iteration. Combined with the strides and destination slices, this produces ZZ for A and ZN for B.

  **[LoadData2DParamsV2](../../../../../docs/zh/api/SIMD-API/basic_api/cube_compute_ISASI/cube_compute_load/LoadData_2D_V2.md)** — 3510 whole-tile loading:

  | Constructor parameter | Meaning |
  |-----------------------|---------|
  | mStartPosition | Starting position in the M direction, in 512-byte units |
  | kStartPosition | Starting position in the K direction, in 512-byte units |
  | mStep | Number of fractals in the first direction |
  | kStep | Number of fractals in the second direction |
  | srcStride | Source fractal stride, in 512-byte units |
  | dstStride | Destination fractal stride, in 512-byte units |
  | ifTranspose | false for A, true for B |
  | sid | Reserved setting; 0 |

  A uses `{0,0,baseM/16,baseK/16,baseM/16,baseM/16,false,0}` for NZ-to-NZ. B uses `{0,0,baseK/16,baseN/16,baseK/16,baseN/16,true,0}` for NZ-to-ZN. For B, the API's logical m/k directions correspond to the matrix's K/N dimensions; they should not be interchanged merely because the default sizes are equal.

  **[MmadParams](../../../../../docs/zh/api/SIMD-API/basic_api/cube_compute_ISASI/mmad_compute/Mmad.md)** — matrix multiplication:

  | Constructor parameter | Value and meaning |
  |-----------------------|-------------------|
  | m | baseM=128, output rows |
  | n | baseN=128, output columns |
  | k | baseK=128, complete reduction length |
  | unitFlag | Fine-grained parallelism control for Mmad and Fixpipe; 0 by default |
  | cmatrixSource | Source of the initial C matrix: false for CO1, true for C2; false by default |
  | cmatrixInitVal | true for every M tile, initializing a separate accumulator |

  The call is `{baseM, baseN, baseK, 0, false, true}`. M tiles cover different output rows, not successive segments of a K reduction.

  **[FixpipeParamsV220](../../../../../docs/zh/api/SIMD-API/basic_api/cube_compute_ISASI/cube_compute_store/Fixpipe_L0CToGM.md)** — L0C-to-GM transfer and conversion for Atlas A2 Series Products and Atlas A3 Series Products:

  | Constructor parameter | Value and meaning |
  |-----------------------|-------------------|
  | nSize | baseN=128, result columns |
  | mSize | baseM=128, result rows |
  | srcStride | baseM=128; units of C0_SIZE, which is 16×sizeof(float)=64 bytes for this source |
  | dstStride | N=128, distance between destination ND row starts, in elements |
  | reluEn | false; preserve negatives for LeakyRelu |
  | quantPre | QuantMode_t::F322F16 |
  | deqScalar | 0; no additional dequantization scale is used |
  | ndNum | 1 matrix |
  | srcNdStride | Distance between source NZ matrix starts, in units of 16×C0_SIZE; 0 for one matrix |
  | dstNdStride | Distance between destination ND matrix starts, in elements; 0 for one matrix |
  | unitFlag | 0, consistent with Mmad |

  The call is `{baseN,baseM,baseM,N,false,QuantMode_t::F322F16,0,1,0,0,0}` and writes to `cGM[mIndex*baseM*N]`.

  **[FixpipeParamsArch3510](../../../../../docs/zh/api/SIMD-API/basic_api/cube_compute_ISASI/cube_compute_store/Fixpipe_L0CToGM.md)** — used for Ascend 950PR&950DT Series Products:

  `FixpipeParamsArch3510<CO2Layout::ROW_MAJOR>` and `Fixpipe<half, float, CFG_ROW_MAJOR>` convert NZ to ND and FP32 to FP16, then transfer the result to GM.

  | Parameter | Configuration and meaning in this example |
  |-----------|-------------------------------------------|
  | `nSize`, `mSize` | First two constructor arguments: baseN and baseM, the output tile's columns and rows |
  | `srcStride` | Third argument: baseM, the offset between adjacent Z layouts in the source NZ matrix, in C0_SIZE units; C0_SIZE is 64 bytes for float |
  | `dstStride` | Fourth argument: N, the offset between destination ND row starts, in elements |
  | `quantPre` | Explicitly set to `QuantMode_t::F322F16` |
  | `reluEn` | Explicitly false to preserve negative values for LeakyRelu |
  | `params` | `Nz2NdParams{1, 0, 0}`: ndNum, srcNdStride and dstNdStride; the matrix strides are unused for a single matrix |
  | `unitFlag` | Default value 0, consistent with Mmad |
  | `preReluMode`, `preClipReluMode` | Defaults `NO_RELU` and `NOCLIP_RELU`; no activation before conversion |

  Unlike FixpipeParamsV220, the 3510 structure stores NZ2ND parameters in its `params` member. srcNdStride is measured in C0_SIZE units and dstNdStride in elements. This example requires no quantization scale and does not set deqScalar.

  **[DataCopyExtParams](../../../../../docs/zh/api/SIMD-API/basic_api/memory_vector_compute/data_move/DataCopyPad_GMToUB.md)** — GM/UB transfers:

  | Constructor parameter | Meaning |
  |-----------------------|---------|
  | blockCount | Number of rows; baseM/2=64 |
  | blockLen | Bytes per row; baseN×sizeof(half)=256 |
  | srcStride | Gap from the end of one source block to the next start |
  | dstStride | Gap from the end of one destination block to the next start |
  | rsv | Reserved; 0 |

  GM gaps are expressed in bytes; UB gaps in the default mode are expressed in 32-byte blocks. They must not all be interpreted as byte strides. Here both gaps are zero. The variable `srcStride=(N-baseN)*sizeof(half)` represents the GM gap; it is used as the destination stride for the reverse UB-to-GM transfer.

  Input uses `{baseM/2, blockLen, srcStride, 0, 0}` with the required uint16_t cast on the first argument. Output uses `{baseM/2, blockLen, 0, srcStride, 0}`.

  **[DataCopyPadExtParams\<half\>](../../../../../docs/zh/api/SIMD-API/basic_api/memory_vector_compute/data_move/DataCopyPad_GMToUB.md)** — padding settings:

  | Constructor parameter | Meaning |
  |-----------------------|---------|
  | isPad | true, enable explicit padding settings |
  | leftPadding | 0 elements |
  | rightPadding | 0 elements |
  | paddingValue | 0 |

  `{true,0,0,0}` adds no explicit padding elements. Each row is already 256 bytes and therefore 32-byte aligned. Internal padding-value representation and stride field types differ across architectures; the table describes the call semantics.

  **LeakyRelu and cross-core parameters**

  `LeakyRelu(vecOutLocal, vecInLocal, (half)0.001, len)` receives the output tensor, input tensor, negative slope and number of elements. `len=8192`, not bytes.

  `CrossCoreSetFlag<0x2, PIPE_FIX>(READY_FLAG+flagOffset)` sends READY after FIX completes. `CrossCoreSetFlag<0x2, PIPE_MTE2>(FREE_FLAG+flagOffset)` sends FREE after the AIV finishes reading. READY_FLAG=8 and FREE_FLAG=10 are the starting flagId values defined by this example. With `flagOffset=mIndex%BUFFER_NUM`, READY uses 8/9 and FREE uses 10/11. One mode-2 wait on the AIC requires both paired AIVs to complete synchronization.

  On Ascend 950PR&950DT Series Products, the `pipe` template argument takes effect; omitting it blocks only the default `PIPE_S`. The AIV uses `CrossCoreWaitFlag<0x2, PIPE_MTE2>` for READY so GM-to-UB reads follow Fixpipe completion. The AIC uses `CrossCoreWaitFlag<0x2, PIPE_FIX>` for FREE to wait for both AIV reads before reusing the flagId, ordering subsequent Fixpipe operations and READY events. The final FREE waits also use `PIPE_FIX`; the all-pipeline synchronization in `Destroy()`, called by the TPipe destructor on Kernel exit, ensures completion.

  On Atlas A2 Series Products and Atlas A3 Series Products, `pipe` does not affect the hardware instruction: the wait blocks all pipelines. This branch retains `CrossCoreWaitFlag<0x2>` as recommended by the repository API documentation. Both architecture branches use mode 2 to match the corresponding Set calls.

## Compilation and Execution

Execute the following steps in the root directory of this example to compile and run the example.

- Configure Environment Variables

  Configure the environment variables according to the [installation method](../../../../../docs/en/quick_start.md#prepare&install) of the CANN development kit package in the current environment.
  ```bash
  source ${install_path}/cann/set_env.sh
  ```

  > **Note:** `${install_path}` is the CANN package installation directory. When no installation directory is specified, it defaults to `/usr/local/Ascend`.

- Example Execution

  Execute the following commands in this example directory.
  ```bash
  mkdir -p build && cd build;                                               # Create and enter build directory
  cmake -DCMAKE_ASC_ARCHITECTURES=dav-2201 ..;make -j;                      # Compile project (default npu mode)
  python3 ../scripts/gen_data.py                                            # Generate test input data
  ./demo                                                                    # Execute the compiled executable program to run the example
  python3 ../scripts/verify_result.py output/output.bin output/golden.bin   # Verify output results for correctness, confirm algorithm logic is correct
  ```

  When using CPU debugging or NPU simulation mode, add the `-DCMAKE_ASC_RUN_MODE=cpu` or `-DCMAKE_ASC_RUN_MODE=sim` parameter.

  Examples:
  ```bash
  cmake -DCMAKE_ASC_RUN_MODE=cpu -DCMAKE_ASC_ARCHITECTURES=dav-2201 ..;make -j; # CPU debugging mode
  cmake -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-2201 ..;make -j; # NPU simulation mode
  ```

  > **Note:** Before switching compilation modes, you need to clean the cmake cache. You can execute `rm CMakeCache.txt` in the build directory and then run cmake again.

- Compilation Option Description

  | Option | Possible Values | Description |
  | -------| ----------------| --------------------------------------------------------------------------------------|
  | `CMAKE_ASC_RUN_MODE` | `npu` (default), `cpu`, `sim` | Run mode: NPU execution, CPU debugging, NPU simulation |
  | `CMAKE_ASC_ARCHITECTURES` | `dav-2201` (default), `dav-3510` | NPU architecture: dav-2201 corresponds to Atlas A2 Series Products and Atlas A3 Series Products, dav-3510 corresponds to Ascend 950PR&950DT Series Products |

- Execution Result

  The execution result is as follows, indicating precision comparison passed.
  ```bash
  test pass!
  ```

## Implementation Flow Analysis

The following tables break down the Cube and Vector operations in execution order.

### Cube Core Flow

| Stage | Operation | Purpose |
|-------|-----------|---------|
| Initialization | Double buffers for A1/A2/CO1, single buffers for B1/B2 | Manage local resources and synchronization |
| GM → L1 | Load B once, prefetch two A tiles, enqueue | Submit independent transfers early |
| L1 → L0B | Dequeue B1, LoadB, enqueue/dequeue B2 | Reuse invariant B across all M tiles |
| L1 → L0A | Dequeue A1, LoadData, enqueue A2, release A1 | Architecture-specific layout conversion; prefetch another tile when needed |
| Matrix compute | Dequeue A2, allocate CO1, Mmad, enqueue CO1, release A2 | Independent full-K computation for each M tile |
| Local synchronization | Queue operations manage producer/consumer and reuse dependencies | MTE2→MTE1, MTE1→M, M→FIX and reverse reuse protection |
| L0C → GM | Dequeue CO1, Fixpipe, release CO1 | Allow different tiles' M/FIX work to overlap |
| Cross-core completion | Send READY, wait for FREE before reusing flagId and at the end, release B2 | Complete paired synchronization events and release the B2 buffer |

### Vector Core Flow

| Stage | Operation | Purpose |
|-------|-----------|---------|
| Initialization | Two buffers each for VECIN/VECOUT | Separate input/output lifetimes |
| Wait | Allocate VECIN, wait for the tile's READY | Read only after Fixpipe completes |
| GM → UB | DataCopyPad, publish FREE from MTE2, enqueue VECIN | Acknowledge the read and establish MTE2→V |
| Activation | Dequeue input, allocate output, LeakyRelu, release input, enqueue output | Apply the FP16 negative slope |
| UB → GM | Dequeue output, DataCopyPad, release output | V→MTE3 dependency and safe reuse |

### Double-Buffer Resources and Scheduling

| Memory | Default allocation | Total |
|--------|--------------------|-------|
| L1 | A1: 2×32 KiB; B1: 1×32 KiB | 96 KiB/AIC |
| L0A | A2: 2×32 KiB | 64 KiB/AIC |
| L0B | B2: 1×32 KiB | 32 KiB/AIC |
| L0C | CO1: 2×64 KiB | 128 KiB/AIC |
| UB | VECIN: 2×16 KiB; VECOUT: 2×16 KiB | 64 KiB/AIV |

Stages within one tile still obey their dependencies. Different buffers allow MTE1 to load while Mmad reads another tile, Mmad to write while FIX reads another result, and Vector transfers to overlap work on another half tile. FreeTensor does not immediately destroy the underlying local memory; subsequent reuse remains synchronized. Double buffering offers overlap, not a guaranteed 2× speedup.

With two tiles per core, the AIC sends READY with flagId 8/9 and waits for FREE with flagId 10/11 at the end. With baseM=64 and four tiles, tile 2 reuses flagId 8 only after FREE for tile 0 (flagId 10); tile 3 similarly waits for FREE for tile 1 (flagId 11) before reusing flagId 9. The final waits complete FREE synchronization for tiles 2 and 3.

FREE indicates that the corresponding GM read is complete, not that activation and final output are complete. Tiles occupy distinct GM regions, so a later Cube tile does not overwrite an earlier Vector output. Queue dependencies and the all-pipeline synchronization in the TPipe destructor on Kernel exit ensure that the writes complete. No extra GM workspace is used.
