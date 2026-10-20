# L0A/L0B Buffer DumpTensor API Description

## Overview

This sample uses the Cube-programmed half matrix multiplication as context. It uses `DumpTensor` to print
matrices A and B from the **L0A Buffer and L0B Buffer**, and prints the same data from L1 Buffer as a
reference, demonstrating L0A/L0B Buffer data printing on dav-9201 (A6).

L0A/L0B have no direct DataCopy path to GM. Dump data is read through the chip **DebugBus** path (the
`mem_copy_abuf_to_gm_impl` / `mem_copy_bbuf_to_gm_impl` implementations for `__NPU_ARCH__ == 9201`). This
path takes effect only on the AIC (cube) core.

## Supported Products and CANN Software Versions

| Product | Architecture | CANN Software Version | HDK Version |
|---------|--------------|------------------------|-------------|
| Ascend 960DT products | dav-9201 (A6) | >= CANN 9.1.0 | >= 25.7.RC1.6 |

> L0A/L0B dump depends on the runtime to correctly initialize `DebugBlockHeadInfo::debugBusAddr`. When
> `debugBusAddr == 0`, the dump is silently skipped and no TLV is produced. This sample has not yet been
> verified on a physical A6 board; the sample and implementation are provided first and will be verified on
> the A6 environment later.

## Directory Structure

```text
├── l0ab_buffer_dump
│   ├── scripts
│   │   ├── gen_data.py          // Generates input and golden data
│   │   └── verify_result.py     // Verifies computation output and dump records
│   ├── CMakeLists.txt           // Build project file
│   ├── data_utils.h             // Binary file read and write helpers
│   ├── l0ab_buffer_dump.asc     // MatMul, DumpTensor calls, and host launch
│   ├── README.md                // Chinese sample document
│   └── README_en.md             // English sample document
```

## Sample Description

- Function:

  Uses half matrix multiplication as context and computes `C[m, n] = sum(A[m, k] * B[k, n])`. A and B are
  moved from Global Memory to L1 (with ND2NZ conversion on the fly), then from L1 to L0A/L0B via `LoadData`,
  and finally accumulated by `Mmad`; `Fixpipe` writes the float result in L0C back to Global Memory.

- Specifications:

  Fixed dimensions `M=16, K=32, N=16`:

  <table>
  <tr><td rowspan="1" align="center">OpType</td><td colspan="4" align="center">MatmulDumpL0ab</td></tr>
  <tr><td rowspan="3" align="center">Inputs</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td align="center">a</td><td align="center">[M, K]</td><td align="center">half</td><td align="center">ND</td></tr>
  <tr><td align="center">b</td><td align="center">[K, N]</td><td align="center">half</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">Output</td><td align="center">c</td><td align="center">[M, N]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">Kernel function</td><td colspan="4" align="center">L0abBufferDump</td></tr>
  </table>

  A holds distinct integers `0`-`511` (exactly representable by half) and B holds small integers `1`-`7`,
  so the dumped data can be checked element by element against the physical layout. The result is an integer
  smaller than `2^24`, exactly representable by float32.

- DumpTensor positions:

  | desc | Position | Purpose |
  |------|----------|---------|
  | 100 | L1 Buffer | Matrix A before being moved to L0A (NZ layout, reference) |
  | 101 | L1 Buffer | Matrix B before being moved to L0B (NZ layout, reference) |
  | 200 | L0A Buffer | Matrix A after `LoadData` moves it into L0A |
  | 201 | L0B Buffer | Matrix B after `LoadData` moves it into L0B (each fractal transposed) |

  `scripts/verify_result.py` verifies the computation output and all four `DumpTensor` records.

## L0A/L0B Physical Layout and Golden Data

The NZ physical layout produced by `DataCopy` (ND2NZ on the fly) in L1 differs from the physical layout in
L0A/L0B (determined by `LoadData`). The dump is output in physical storage order, and
`verify_result.py` organizes golden data according to the following model:

- **A1 (L1) / A2 (L0A)**: arranged as `[K fractal j][M row][16 contiguous K elements]`,
  `phys[j*256 + m*16 + kk] = A[m][j*16 + kk]`. Under this sample's configuration the two layouts are identical.
- **B1 (L1)**: `nValue=K, dValue=N`; each K row is exactly 32B (16 half), arranged as `[K, N]` row-major,
  `phys[k*16 + n] = B[k][n]`.
- **B2 (L0B)**: `LoadData` transposes each `[16,16]` fractal and places them in K-fractal order,
  `phys[f*256 + n*16 + kk] = B[f*16 + kk][n]`.

> Note: The DebugBus model base for L0A/L0B (`0x410000`/`0x420000`), byte addressing, and 32B-per-round
> step are currently copied from the dav-3510 implementation. If the A6 chip differs, fix it inside
> `npu_arch_9201/asc_aicore_dump_utils.h`. If the on-board dump result does not match the layout model above,
> adjust the golden organization in `verify_result.py` according to the actual physical layout.

## Build and Run

Run the following commands in the sample root directory:

```bash
source ${install_path}/cann/set_env.sh
mkdir -p build && cd build
cmake -DCMAKE_ASC_ARCHITECTURES=dav-9201 .. && make -j
python3 ../scripts/gen_data.py
./demo 2>&1 | tee run.log
python3 ../scripts/verify_result.py output/output.bin output/golden.bin run.log \
    input/a_gm.bin input/b_gm.bin
```

`${install_path}` is the CANN installation directory. The default installation root is `/usr/local/Ascend`.

A successful verification prints:

```text
Output result check: PASS
Matrix A in L1 data check: PASS
Matrix B in L1 data check: PASS
Matrix A in L0A data check: PASS
Matrix B in L0B data check: PASS
test pass!
```
