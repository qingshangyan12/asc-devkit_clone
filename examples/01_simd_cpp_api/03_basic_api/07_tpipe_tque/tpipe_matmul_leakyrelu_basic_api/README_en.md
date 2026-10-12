# TPipe-TQue Matmul and LeakyRelu Fusion Example

## Overview

This example implements fused Matmul and LeakyRelu using the TPipe-TQue programming model. TPipe manages local resources, while TQue manages data transfer between stages and safe buffer reuse. Cube cores perform matrix multiplication, and their paired Vector cores apply LeakyRelu.

A1, A2, CO1, VECIN and VECOUT use double buffering so that transfers, computation and stores for different tiles can overlap. B is loaded once per Cube core and retained in L0B across M tiles.

The example retains the original introduction sample's structure: one templated `KernelMatmul` class, its `Init` and `Process` methods, private helpers, the templated `mmad_vec_custom` kernel, and a main function that calls ACL directly.

## Supported Products and CANN Versions

| Product | Architecture | Toolchain requirements and validation |
|---------|--------------|---------------------------------------|
| Ascend 950PR/Ascend 950DT | dav-3510 | Requires a compiler compatible with the repository APIs; tested on Ascend950PR |
| Atlas A3 Training/Inference Series | dav-2201 | Architecture branch provided; validation on the corresponding toolchain and hardware is pending |
| Atlas A2 Training/Inference Series | dav-2201 | Architecture branch provided; validation on the corresponding toolchain and hardware is pending |

API headers are taken from the repository containing this example by default. `ASC_DEVKIT_ROOT` can override that location. The compiler and runtime still come from the installed CANN package. The original validation on October 10, 2026 used `/home/developer/Ascend/cann-9.2.0` and headers from `/workspace/asc-devkit`.

That older compiler rejected the repository's `__callee__` attribute in `kernel_struct_data_copy.h` when targeting 2201. After migration, the same compiler also rejects `__callee__` on the `ScalarBitcode` constructor in the target repository's `kernel_utils_struct_norm_sort.h`. The minimum CANN version documented for the original static-tensor sample is therefore not sufficient evidence that this version can compile. See [validation notes](VALIDATION.md) for the original results and the checks performed after migration.

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
├── README_en.md                          # English documentation
└── VALIDATION.md                         # Validation scope and results
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

  The following `Init` method is taken from the templated class; comments are omitted here. The full class and declarations are in the [source](tpipe_matmul_leakyrelu_basic_api.asc).

```cpp
__aicore__ inline void Init(__gm__ uint8_t* xMatrix, __gm__ uint8_t* yMatrix, __gm__ uint8_t* zMatrix)
{

    static_assert(M % singleCoreM == 0 && singleCoreM % baseM == 0, "M must be divisible by tiles");
    static_assert(singleCoreK == K && baseK == K && singleCoreN == N && baseN == N,
        "This sample only partitions M");
    static_assert(baseM == 32 || baseM == 64 || baseM == 128, "Supported baseM: 32, 64, 128");
    static_assert(K == 128 && N == 128, "This sample requires K=N=128");
    static_assert(singleCoreM / baseM >= BUFFER_NUM, "At least two tiles are required for double buffer");
    aGM.SetGlobalBuffer((__gm__ half*)xMatrix);
    bGM.SetGlobalBuffer((__gm__ half*)yMatrix);
    cGM.SetGlobalBuffer((__gm__ half*)zMatrix);

    if ASCEND_IS_AIC {
        aGM = aGM[AscendC::GetBlockIdx() * singleCoreM * K];
        cGM = cGM[AscendC::GetBlockIdx() * singleCoreM * N];

        pipe.InitBuffer(a1Queue, BUFFER_NUM, baseM * baseK * sizeof(half));
        pipe.InitBuffer(a2Queue, BUFFER_NUM, baseM * baseK * sizeof(half));
        pipe.InitBuffer(cQueue, BUFFER_NUM, baseM * baseN * sizeof(float));

        pipe.InitBuffer(b1Queue, 1, baseK * baseN * sizeof(half));
        pipe.InitBuffer(b2Queue, 1, baseK * baseN * sizeof(half));
    }
    if ASCEND_IS_AIV {

        cGM = cGM[AscendC::GetBlockIdx() / 2 * singleCoreM * N];
        pipe.InitBuffer(vecInQueue, BUFFER_NUM, baseM / 2 * baseN * sizeof(half));
        pipe.InitBuffer(vecOutQueue, BUFFER_NUM, baseM / 2 * baseN * sizeof(half));
    }
}
```

  The queues are class members:

```cpp
AscendC::TPipe pipe;
AscendC::TQue<AscendC::TPosition::A1, BUFFER_NUM> a1Queue;
AscendC::TQue<AscendC::TPosition::B1, 1> b1Queue;
AscendC::TQue<AscendC::TPosition::A2, 1> a2Queue;
AscendC::TQue<AscendC::TPosition::B2, 1> b2Queue;
AscendC::TQue<AscendC::TPosition::CO1, 1> cQueue;
AscendC::TQue<AscendC::TPosition::VECIN, 1> vecInQueue;
AscendC::TQue<AscendC::TPosition::VECOUT, 1> vecOutQueue;
```

  Global constants are `CUBE_BLOCK=16`, `BUFFER_NUM=2`, `READY_FLAG=8`, and `FREE_FLAG=10`.

  **Memory transfers and computation**

  - **LocalTensor allocation:** InitBuffer initializes the queues. AllocTensor obtains a tensor, EnQue publishes the producer operation, DeQue establishes the consumer dependency, and FreeTensor returns ownership for later reuse.
  - **GM → L1:** DataCopy loads B once and prefetches two A tiles while converting ND to NZ.
  - **L1 → L0A/L0B:** LoadData loads each A tile and loads B once, with transpose, retaining B2 throughout the M-tile loop.
  - **L0A/L0B → L0C:** Each Mmad computes the complete K reduction for one M tile. Each call initializes a fresh accumulator; there is no accumulation across K tiles.
  - **L0C → GM:** Fixpipe converts the result to ND half and writes the current tile's GM region.

  The `Process` method also shows where the Vector branch invokes the half-tile helper:

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
                    0, 0, baseM / CUBE_BLOCK, baseK / CUBE_BLOCK, baseM / CUBE_BLOCK, baseM / CUBE_BLOCK, false, 0});
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
            uint32_t slot = mIndex % BUFFER_NUM;
            if (mIndex >= BUFFER_NUM) {

                AscendC::CrossCoreWaitFlag<0x2>(FREE_FLAG + slot);
            }

            AscendC::Fixpipe(
                cGM[mIndex * baseM * N], cLocal,
                AscendC::FixpipeParamsV220{baseN, baseM, baseM, N, false, QuantMode_t::F322F16, 0, 1, 0, 0, 0});
            AscendC::CrossCoreSetFlag<0x2, PIPE_FIX>(READY_FLAG + slot);
            cQueue.FreeTensor(cLocal);
        }

        for (uint32_t slot = 0; slot < BUFFER_NUM; ++slot) {
            AscendC::CrossCoreWaitFlag<0x2>(FREE_FLAG + slot);
        }
        b2Queue.FreeTensor(b2Local);
    }
    if ASCEND_IS_AIV {
        for (uint32_t mIndex = 0; mIndex < mLoops; ++mIndex) {

            LeakyReluCompute(mIndex, 0, baseM / 2 * baseN);
        }
    }
}
```

  Private helpers handle A prefetch and architecture-specific B loading:

```cpp
__aicore__ inline void CopyInA(uint32_t mIndex)
{
    AscendC::LocalTensor<half> a1Local = a1Queue.AllocTensor<half>();
    AscendC::DataCopy(
        a1Local, aGM[mIndex * baseM * K], AscendC::Nd2NzParams{1, baseM, baseK, 0, K, baseM, 1, 0});
    a1Queue.EnQue(a1Local);
}

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

  - **Cross-core synchronization:** Wait for READY for the current tile. After reading, publish FREE from MTE2; the Cube core waits for both paired AIVs before reusing that notification slot.
  - **LocalTensor allocation:** Separate VECIN and VECOUT queues each have two physical buffers.
  - **GM → UB:** DataCopyPad reads `baseM/2` rows of `baseN` half elements.
  - **UB computation:** LeakyRelu writes a separate output tensor and applies the FP16 negative slope.
  - **UB → GM:** DataCopyPad writes the same half-tile region; release the output tensor and continue to the next tile.

```cpp
__aicore__ inline void LeakyReluCompute(uint32_t mIndex, uint32_t nIndex, uint32_t tileLen)
{
    uint32_t blockLen = baseN * sizeof(half);
    uint32_t srcStride = (N - baseN) * sizeof(half);

    uint32_t gmOffset = (mIndex * baseM * N + nIndex * baseN) + AscendC::GetBlockIdx() % 2 * (baseM / 2 * N);
    uint32_t slot = mIndex % BUFFER_NUM;
    AscendC::LocalTensor<half> vecInLocal = vecInQueue.AllocTensor<half>();
    AscendC::CrossCoreWaitFlag<0x2>(READY_FLAG + slot);

    AscendC::DataCopyPad<half>(
        vecInLocal, cGM[gmOffset],
        AscendC::DataCopyExtParams{static_cast<uint16_t>(baseM / 2), blockLen, srcStride, 0, 0},
        AscendC::DataCopyPadExtParams<half>{true, 0, 0, 0});

    AscendC::CrossCoreSetFlag<0x2, PIPE_MTE2>(FREE_FLAG + slot);
    vecInQueue.EnQue(vecInLocal);

    vecInLocal = vecInQueue.DeQue<half>();
    AscendC::LocalTensor<half> vecOutLocal = vecOutQueue.AllocTensor<half>();
    AscendC::LeakyRelu(vecOutLocal, vecInLocal, (half)0.001, tileLen);
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

  `__global__` declares the device entry point. `__mix__(1, 2)` specifies one Cube core and two paired Vector cores per logical block. The wrapper matches the original sample:

```cpp
template <
    uint32_t M, uint32_t K, uint32_t N, uint32_t singleCoreM, uint32_t singleCoreK, uint32_t singleCoreN,
    uint32_t baseM, uint32_t baseK, uint32_t baseN>
__global__ __mix__(1, 2) void mmad_vec_custom(__gm__ uint8_t* xMatrix, __gm__ uint8_t* yMatrix, __gm__ uint8_t* zMatrix)
{
    AscendC::InitSocState();
    KernelMatmul<M, K, N, singleCoreM, singleCoreK, singleCoreN, baseM, baseK, baseN> op;
    op.Init(xMatrix, yMatrix, zMatrix);
    op.Process();
    AscendC::PipeBarrier<PIPE_ALL>();
}
```

  Main directly initializes ACL, selects device 0, creates a stream, allocates input/output buffers, copies inputs to the device, launches the kernel once, synchronizes, copies output to the host, writes it to disk, and releases resources. The launch is:

```cpp
mmad_vec_custom<M, K, N, singleCoreM, singleCoreK, singleCoreN, baseM, baseK, baseN>
    <<<numBlocks, 0, stream>>>(aDevice, bDevice, cDevice);
```

- **API parameters**

  The tables below follow the constructor argument order used by the brace-initialized calls. That order may differ from member declaration order. Some field types vary by architecture, so these are semantic descriptions rather than complete struct declarations. Consult the linked repository documentation for target-specific ranges. The links use the repository's Chinese API references.

  **TPipe and TQue**

  | API | Meaning in this example |
  |-----|-------------------------|
  | `TQue<position, depth>` | Logical position and maximum queued tensors awaiting dequeue; A1 depth is 2, others 1 |
  | `InitBuffer(queue, num, len)` | Physical buffer count and bytes per buffer; double buffering uses num=2 |
  | `AllocTensor<T>()` | Obtain a writable local tensor with the appropriate element type |
  | `EnQue(tensor)` | Publish a producer operation and its dependency |
  | `DeQue<T>()` | Obtain a tensor in queue order and establish the consumer dependency |
  | `FreeTensor(tensor)` | Return ownership; future reuse remains protected against outstanding reads |

  For example, `InitBuffer(a2Queue, 2, baseM*baseK*sizeof(half))` allocates two 32 KiB L0A buffers even though a2Queue has template depth 1.

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
  | mStartPosition | Starting position in the first logical fractal direction |
  | kStartPosition | Starting position in the second logical fractal direction |
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
  | unitFlag | 0; dependencies are managed through queues |
  | cmatrixSource | false, retaining the original C-source setting |
  | cmatrixInitVal | true for every M tile, initializing a separate accumulator |

  The call is `{baseM, baseN, baseK, 0, false, true}`. M tiles cover different output rows, not successive segments of a K reduction.

  **[FixpipeParamsV220](../../../../../docs/zh/api/SIMD-API/basic_api/cube_compute_ISASI/cube_compute_store/Fixpipe_L0CToGM.md)** — L0C-to-GM transfer and conversion:

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
  | srcNdStride | 0; unused for one matrix |
  | dstNdStride | 0; unused for one matrix |
  | unitFlag | 0, consistent with Mmad |

  The call is `{baseN,baseM,baseM,N,false,QuantMode_t::F322F16,0,1,0,0,0}` and writes to `cGM[mIndex*baseM*N]`. The 3510 branch also uses this interface form; the V220 name does not mean only the 2201 branch executes it.

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

  **[DataCopyPadExtParams<half>](../../../../../docs/zh/api/SIMD-API/basic_api/memory_vector_compute/data_move/DataCopyPad_GMToUB.md)** — padding settings:

  | Constructor parameter | Meaning |
  |-----------------------|---------|
  | isPad | true, enable explicit padding settings |
  | leftPadding | 0 elements |
  | rightPadding | 0 elements |
  | paddingValue | 0 |

  `{true,0,0,0}` adds no explicit padding elements. Each row is already 256 bytes and therefore 32-byte aligned. Internal padding-value representation and stride field types differ across architectures; the table describes the call semantics.

  **LeakyRelu and cross-core parameters**

  `LeakyRelu(vecOutLocal, vecInLocal, (half)0.001, tileLen)` receives the output tensor, input tensor, negative slope and number of elements. `tileLen=8192`, not bytes.

  `CrossCoreSetFlag<0x2, PIPE_FIX>(READY_FLAG+slot)` publishes readiness after FIX completes. `CrossCoreSetFlag<0x2, PIPE_MTE2>(FREE_FLAG+slot)` acknowledges a completed AIV read. `slot=mIndex%2`; READY uses 8/9 and FREE uses 10/11. The AIC waits for both AIV acknowledgements. The default waiting-pipeline semantics differ between 2201 and 3510; this code retains `CrossCoreWaitFlag<0x2>` without an explicit pipeline argument.

## Build and Run

From the sample directory:

```bash
source ${install_path}/cann/set_env.sh
mkdir -p build
cd build
cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..
cmake --build . -j
python3 ../scripts/gen_data.py
./demo
python3 ../scripts/verify_result.py output/output.bin output/golden.bin
```

Load the environment script from the actual CANN installation. Python requires NumPy. The program uses device 0 and one kernel launch; file paths are relative to the working directory.

The CMake architecture default remains dav-2201, while the command explicitly selects the verified 3510 target. Use dav-2201 only with the corresponding compatible toolchain and hardware. Create a separate build directory when changing architectures or run modes.

CMake prefers repository headers using `-iquote`, since compiler-injected package paths could otherwise select older headers. `ASC_DEVKIT_ROOT` defaults to the repository root computed relative to this sample. Override it with `-DASC_DEVKIT_ROOT=/absolute/path/to/asc-devkit` if using a different source tree. This does not add new capabilities to an old compiler.

CPU and simulation options from the original sample are retained. Only npu mode has been validated; cpu/sim availability depends on the installed tools and mixed-kernel support. Example configurations, using separate directories from the sample root:

```bash
cmake -S . -B build_cpu -DCMAKE_ASC_RUN_MODE=cpu -DCMAKE_ASC_ARCHITECTURES=dav-2201
cmake --build build_cpu -j
cmake -S . -B build_sim -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-2201
cmake --build build_sim -j
```

| Option | Values | Description |
|--------|--------|-------------|
| CMAKE_ASC_RUN_MODE | npu (default), cpu, sim | Run mode; only npu verified |
| CMAKE_ASC_ARCHITECTURES | dav-2201 (default), dav-3510 | Device architecture |
| ASC_DEVKIT_ROOT | Absolute repository path | Defaults to the enclosing repository |

Successful verification prints `test pass!`. The verifier requires exactly 65,536 half elements (131,072 bytes) in each file and rejects empty, truncated, oversized or nonfinite data. The default integer comparison retains rtol=1e-6, atol=1e-9 and a maximum mismatch fraction of 1e-4. See [validation notes](VALIDATION.md) for the actual checks, numerical comparison criteria and architecture limitations.

## Implementation Flow Analysis

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
| Cross-core completion | Publish READY, consume FREE before slot reuse and at the end, release B2 | Bound outstanding notifications and complete the B lifecycle |

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

Stages within one tile still obey their dependencies. Different physical buffers allow MTE1 to load while Mmad reads another tile, Mmad to write while FIX reads another result, and Vector transfers to overlap work on another half tile. FreeTensor does not immediately destroy physical local memory; subsequent reuse remains synchronized. Double buffering offers overlap, not a guaranteed 2× speedup.

With two tiles per core, the AIC publishes READY8/READY9 and consumes FREE10/FREE11 at the end. With baseM=64 and four tiles, tile2 reuses slot0 only after both AIVs acknowledge tile0 via FREE10; tile3 similarly waits for tile1's FREE11. Final acknowledgements are then consumed for tile2/tile3.

FREE indicates that the corresponding GM read is complete, not that activation and final output are complete. Tiles occupy distinct GM regions, so a later Cube tile does not overwrite an earlier Vector output. Queue dependencies and the final PipeBarrier complete the writes. No extra GM workspace is used.

## Further Optimization

| Direction | Current implementation | Further analysis and constraints |
|-----------|------------------------|----------------------------------|
| L1/L0 double buffering | Enabled for A1/A2/CO1 | Compare with a single-buffer control and inspect M/FIX overlap; account for capacity |
| B reuse | Loaded once and retained in L0B | Redesign lifetime if N/K tiling is introduced |
| Tile size | baseM=128, two tiles/core | Compare 32/64/128; smaller tiles add queue/sync overhead, larger tiles consume more local memory |
| UB double buffering | Enabled separately for VECIN/VECOUT | Inspect MTE2/V/MTE3 utilization and cross-core waits |
| Vector VF fusion | One LeakyRelu Basic API | Evaluate RegBase/VF fusion if more elementwise operations are added |
| Core partitioning | Two Cube cores, M partitioning | Evaluate more cores for larger matrices and update constraints/data generation |
| GM intermediate | L0C→GM→UB | A 3510-only direct-to-UB path needs a new cross-core protocol and numerical review |

Preserve FP32 accumulation → FP16 conversion → FP16 activation during optimization. Setting Fixpipe reluEn=true would implement a different operation.

## Functional Debugging

### printf

Use device-side formatted output to inspect core and tile indices:

```cpp
AscendC::printf("matmul blockIdx=%d\n", AscendC::GetBlockIdx());
```

Debug output changes timing. Remove it before measuring performance, or use the installed toolkit's ASCENDC_DUMP controls as appropriate.

### DumpTensor

[DumpTensor](../../../../../docs/zh/api/SIMD-API/basic_api/debug_interface/onboard_print/DumpTensor.md) can inspect a local tensor with a uint32_t identifier. For example, replace the existing LeakyRelu call in LeakyReluCompute with the following debug-only sequence, before releasing or enqueuing the output:

```cpp
AscendC::LeakyRelu(vecOutLocal, vecInLocal, (half)0.001, tileLen);
AscendC::PipeBarrier<PIPE_ALL>(); // Debug only: wait for the producer.
AscendC::DumpTensor(vecOutLocal, mIndex, 16);
```

The tensor must still be valid; never dump after FreeTensor. Remove the extra barrier and printing before profiling because they alter the intended overlap. The repository's debug documentation describes the ASCENDC_DUMP controls.

## Performance Debugging

### msOpProf

msOpProf supports operator-level analysis of execution, memory access and resource conflicts. On supported hardware, run the installed tool from the build directory after generating input/output files:

```bash
msopprof ./demo
```

Typical output is an `OPPROF_{timestamp}_XXX` directory; exact files depend on the tool version, device and collection options:

```text
dump/                       # Raw data
ArithmeticUtilization.csv  # Cube/Vector arithmetic utilization
L2Cache.csv                # L2 access and hit information
Memory.csv                 # Memory access metrics
MemoryL0.csv               # L0A/L0B/L0C access
MemoryUB.csv               # UB access
OpBasicInfo.csv            # Operator configuration
PipeUtilization.csv        # Compute and transfer pipeline metrics
ResourceConflictRatio.csv  # Resource conflicts
visualize_data.bin         # Visualization data
```

```bash
cat ./OPPROF_*/PipeUtilization.csv
```

Main launches once. For microsecond-level comparisons, use identical inputs, warmup and launch counts, and separate device execution from process startup, file I/O and H2D/D2H transfer time. The original benchmark launched 110 times, discarded the first 10, and summarized 100 AI_CORE tasks.

Historical results from October 10, 2026 on 3510 are shown below. They were obtained before migration, with the same device source but a different API repository checkout; they are not fresh performance measurements of this checkout.

| Version | Median device time (μs) |
|---------|------------------------:|
| Original static-tensor example | 4.3665 |
| TPipe-TQue, double buffer | 4.0175 |
| Same tile, single buffer | 4.6995 |

The measured median was approximately 8.0% lower than the original and 14.5% lower than the single-buffer control for that shape/environment. Eight input cases produced exactly matching outputs between the old and new NPU kernels. 2201 validation remains pending. See [validation notes](VALIDATION.md) for the migration checks and measurement scope.

API documentation links use repository-relative paths. The compiler header location is controlled independently by `ASC_DEVKIT_ROOT`.
