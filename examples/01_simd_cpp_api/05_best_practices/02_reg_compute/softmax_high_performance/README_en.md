# Softmax Performance Tuning Example

## Overview

This example uses single-core Softmax to demonstrate the complete performance tuning path from MemBase to RegBase, from basic loops to fusion and unrolling, including seven progressively optimized versions Case 0-6.

> **Prerequisite Reading**: [Gelu Operator Introduction Example](https://gitcode.com/cann/asc-devkit/blob/master/examples/01_simd_cpp_api/00_introduction/04_reg_compute/gelu/README.md). It is recommended to read the introduction example first.

**Optimization Path**:
- Case 0 → Case 1: Loop splitting (MemBase)
- Case 1 → Case 2: MemBase → RegBase API (register-level computation)
- Case 2 → Case 3: Loop fusion + ExpSub fusion + UpdateMask tail handling
- Case 3 → Case 4: Outer loop unrolling (UpdateMask mode)
- Case 3 → Case 5: Main-tail block mode
- Case 5 → Case 6: Main-tail + unrolling + ExpSub fusion

## Supported Products and CANN Versions

| Product | CANN Version |
|------|-------------|
| Ascend 950PR/Ascend 950DT | >= CANN 9.1.0 |

## Directory Structure

```
├── softmax_high_performance
│   ├── scripts
│   │   ├── gen_data.py         // Input data and ground truth generation script
│   │   └── verify_result.py    // Ground truth comparison script
│   ├── CMakeLists.txt          // Build project file
│   ├── build.sh                // One-click build & verify script (single and batch modes)
│   ├── data_utils.h            // Data read/write functions
│   ├── softmax.asc             // Ascend C example (Case 0-6 seven versions)
│   └── README.md               // Example documentation
```

## Example Description

**Example Function**:

Softmax computation with fixed shape (m,n) = (128,128).

Softmax formula:

$$
\text{Softmax}(x_{i,j}) = \frac{e^{x_{i,j} - \max_i}}{\sum_{j=0}^{n-1} e^{x_{i,j} - \max_i}}
$$

- x: input, shape [128, 128], dtype float
- z: output, shape [128, 128], dtype float

**Example Specifications**:

<table border="2">
<tr><td rowspan="1" align="center">Example Type (OpType)</td><td colspan="4" align="center">Softmax</td></tr>
<tr><td rowspan="2" align="center">Example Input</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
<tr><td align="center">x</td><td align="center">[128, 128]</td><td align="center">float</td><td align="center">ND</td></tr>
<tr><td rowspan="1" align="center">Example Output</td><td align="center">y</td><td align="center">[128, 128]</td><td align="center">float</td><td align="center">ND</td></tr>
<tr><td rowspan="1" align="center">Kernel Function Name</td><td colspan="4" align="center">softmax_custom</td></tr>
</table>

## Version Overview

| Version | API | Key Feature | Vector-only Performance |
|:---:|:---:|:---|:---:|
| **Case 0** | MemBase | Baseline, all ops in single loop | 48572 cycles |
| **Case 1** | MemBase | Loop splitting into 3 phases | 48412 cycles |
| **Case 2** | RegBase | First Reg API, no fusion, redundant Ld/St/sync | 31894 cycles |
| **Case 3** | RegBase | UpdateMask + loop fusion + ExpSub | 3413 cycles |
| **Case 4** | RegBase | UpdateMask + outer loop unrolling | 2197 cycles |
| **Case 5** | RegBase | Main-tail block mode | 3424 cycles |
| **Case 6** | RegBase | Main-tail + unrolling + ExpSub (optimal) | 1785 cycles |

## Optimization Strategy Analysis

### Performance Metric Description

**Table 1 AI Core Performance Metric Field Description**

| Field Name | Field Meaning |
|:---|:---|
|aiv_vec_time(μs)|vec type instruction (vector computation) duration in μs.|
|Converted aiv_vec_cycle|Cycle count of vec type instructions.|

---

## Phase 1: MemBase Optimization (Case 0 → Case 1)

### Case 0: MemBase Baseline — All ops in a single loop

**Implementation**: 7 steps (ReduceMax, Duplicate, Sub, Exp, ReduceSum, Duplicate, Div) per row in a single for loop using basic Ascend C API.

**Performance**: 48572 cycles (vector only)

### Case 0 → Case 1: Loop Splitting

**Motivation**: 7 varied operations per row in one loop create data dependencies that limit compiler optimization.

**Change**: Split into 3 independent phases:
- Phase 1 (all rows): ReduceMax
- Phase 2 (all rows): Duplicate + Sub + Exp + ReduceSum
- Phase 3 (all rows): Duplicate + Div

**Effect**: 48572 → 48412 cycles. Same-type operations batched together improve cache and pipeline efficiency.

---

## Phase 2: MemBase → RegBase (Case 1 → Case 2)

### Case 2: First RegBase API — No fusion, redundant operations

**Motivation**: MemBase API performs implicit Load→Compute→Store per operation. RegBase allows data to stay resident in registers with only one final Store to UB.

**Key changes**:
- Explicit `Reg::LoadAlign` / `Reg::StoreAlign` for data movement
- Register-level compute APIs: `Reg::Max`, `Reg::Sub`, `Reg::Exp`, `Reg::ReduceMax`, `Reg::ReduceSum`
- `UpdateMask` for unaligned tail handling
- `LocalMemBar` for data coherence

**Case 2 issues**: Unfused loops with redundant Load/Store, explicit sync after every operation, no ExpSub fusion.

**Performance**: 31894 cycles (34% improvement over Case 1)

---

## Phase 3: Deep RegBase Optimization (Case 2 → Case 3)

### Case 3: Loop Fusion + ExpSub Fusion + UpdateMask

**Optimizations**:

1. **ExpSub fusion instruction**: Sub + Exp combined into single ExpSub:
   ```cpp
   // Case 2: two steps
   Reg::Sub(dst, src, max, mask); Reg::Exp(dst, dst, mask);
   // Case 3: one step
   Reg::ExpSub(exp, src, max, mask);
   ```

2. **Loop fusion**: Share Load between ReduceMax and ExpSub, merge ReduceSum and Div — eliminates 2 redundant Load passes.

3. **UpdateMask tail handling**: `UpdateMask<float>(count)` auto-updates mask each inner loop iteration.

**Effect**: 31894 → 3413 cycles (9.3x improvement)

---

## Phase 4: Loop Unrolling Optimization

### Case 3 → Case 4: Outer Loop Unrolling (UpdateMask mode)

**Motivation**: 128 rows serial execution limits compiler cross-row scheduling and VF dual-issue efficiency.

**Change**: Manually unroll outer loop to process 2 rows per iteration using dual register sets (`*Reg` and `*Reg1`).

**Effect**: 3413 → 2197 cycles (35.6% improvement)

### Case 3 → Case 5: Main-Tail Block Mode

**Motivation**: UpdateMask introduces scalar overhead each inner loop iteration. Main-tail mode uses full mask for main blocks, eliminating this overhead.

**Tail handling performance comparison**:

| Mode | aiv_vec_cycle | Note |
|:---|:---:|:---|
| UpdateMask (Case 4, unrolled) | 2197 | Concise, UpdateMask has per-iteration cost |
| Main-tail with tail blocks | 4030 | Tail for(0) depends on Hardware Loop |
| Main-tail without tail (Case 5) | 3424 | Full mask for main blocks |
| Main-tail+unroll+ExpSub (Case 6) | 1785 | Optimal combination |

> For current shape (last axis=128), tail=0 so full-mask mode wins. If last axis is larger and not divisible, UpdateMask offers cleaner code.

### Case 5 → Case 6: Optimal — Main-Tail + Unrolling + ExpSub

Combines all optimizations:
- Main-tail: full mask for main blocks, no UpdateMask overhead
- Outer unrolling: dual register sets, 2 rows/iteration
- ExpSub: fused Sub+Exp instruction

**Performance**: 1785 cycles (27.2x over Case 0) — optimal for this sample.

---

## Performance Comparison Summary

| Version | Optimization Strategy | aiv_vec_cycle | vs Case 0 |
|:---:|:---|:---:|:---:|
| Case 0 | MemBase, single loop | 48572 | 1.00x |
| Case 1 | MemBase, loop splitting | 48412 | 1.00x |
| Case 2 | RegBase, no fusion | 31894 | 1.52x |
| Case 3 | RegBase, UpdateMask+ExpSub+fusion | 3413 | 14.23x |
| Case 4 | RegBase, UpdateMask+unroll | 2197 | 22.11x |
| Case 5 | RegBase, main-tail block | 3424 | 14.18x |
| Case 6 | RegBase, main-tail+unroll+ExpSub | 1785 | 27.21x |

### Optimization Summary

| Method | Core Principle | Applicable Versions |
|:---|:---|:---|
| Loop splitting | Batch same-type ops for cache/pipeline efficiency | Case 0→Case 1 |
| MemBase→RegBase | Register resident, reduce UB↔register round-trips | Case 1→Case 2 |
| ExpSub fusion | Single instruction replaces Sub+Exp | Case 2→Case 3 |
| Loop fusion | Share Load/Store, eliminate redundancy | Case 2→Case 3 |
| UpdateMask | Auto-update mask, concise tail handling | Case 3,Case 4 |
| Main-tail block | Full mask for main blocks, no update overhead | Case 5,Case 6 |
| Outer unrolling | Dual register sets, improve instruction dual-issue | Case 3→Case 4, Case 5→Case 6 |

---

## Build and Run

- Switch version via `-DSCENARIO_NUM=N` (N=0~6):

  | SCENARIO_NUM | Version | Description | CMAKE_VF_MODE |
  |:---:|:---:|:---|:---:|
  | 0 | Case 0 | MemBase, single loop | false |
  | 1 | Case 1 | MemBase, loop splitting | false |
  | 2 | Case 2 | RegBase, no fusion | false |
  | 3 | Case 3 | RegBase, UpdateMask+ExpSub+fusion | true |
  | 4 | Case 4 | RegBase, UpdateMask+unroll | true |
  | 5 | Case 5 | RegBase, main-tail block | true |
  | 6 | Case 6 | RegBase, main-tail+unroll+ExpSub | true |

- Configure environment:
  ```bash
  source ${install_path}/cann/set_env.sh
  ```

- Single version:
  ```bash
  mkdir -p build && cd build
  cmake .. -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DSCENARIO_NUM=6 -DCMAKE_VF_MODE=true
  make -j
  python3 ../scripts/gen_data.py
  ./demo
  python3 ../scripts/verify_result.py output/output.bin output/golden.bin
  ```

- Using build.sh:
  ```bash
  ./build.sh -c 6              # Build and run Case 6
  ./build.sh batch             # Batch build and verify Case 0-6
  ```

- Build options:

  | Option | Values | Description |
  |:---|:---|:---|
  | `CMAKE_ASC_RUN_MODE` | `npu`(default), `cpu`, `sim` | Run mode |
  | `CMAKE_ASC_ARCHITECTURES` | `dav-3510` | NPU architecture |
  | `SCENARIO_NUM` | `0`~`6` | Version Case 0-6 |
  | `CMAKE_VF_MODE` | `true`, `false` | VF fusion: Case 0-2=false, Case 3-6=true |

### Performance Analysis

```bash
msprof ./demo
cat ./PROF_*/mindstudio_profiler_output/op_summary_*.csv
```
