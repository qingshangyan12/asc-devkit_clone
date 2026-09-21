# Warp Branch Divergence Predication Optimization Sample

## Overview

This sample demonstrates tuning methods for [Warp Divergence](../../../../../docs/zh/guide/programming_guide/programming_model/ai_core_simt_programming/thread_architecture.md#warp执行机制) in SIMT programming mode through the branch chains of the mathematical functions `hlog10` and `j0f`, focusing on the performance gains and applicable scenarios of predicate optimization. The sample contains 2 baseline versions and 3 optimized versions: the baseline versions return early upon hitting a branch, where the branch condition produces a real jump and triggers branch divergence; the optimized versions eliminate branch jumps by converting branches into predicated straight-line execution, and select the applicable scope of predication versus real jumps according to branch body length.

## Supported Products and CANN Versions

| Product | CANN Version |
|------|-------------|
| Ascend 950PR/Ascend 950DT | >= CANN 9.2.0 |

## Directory Structure

```text
├── predicate_optimization
│   ├── predicate_optimization.asc  // SIMT implementation & invocation sample
│   ├── figures                     // Image resources for README
│   ├── CMakeLists.txt              // cmake build script
│   ├── README.md                   // Sample documentation (Chinese)
│   └── README_en.md                // English sample documentation
```

## Sample Description

- Sample Function

  For the branch chains of the mathematical functions `hlog10` (half input, 6 precision special values requiring correction) and `j0f` (float input, computed in low/middle/high value segments with NaN/Inf handling), kernels are implemented with different branch organizations such as "early return" and "unified return": the former represents a short correction chain, and the latter represents a mixed-length interval segmentation chain. Through comparisons of different branch organizations, the sample demonstrates the performance gains and applicable scenarios of predicate optimization in SIMT programming.

- **Warp and Branch Divergence**

  In C++ development, when developers encounter special values, they usually place special-value handling before the normal computation logic and use control flow statements to divide different execution branches. However, in SIMT programming mode, the Warp is the basic scheduling and execution unit. When branch conditions appear in the code, threads within a Warp will diverge: each branch's code can only execute serially; at the divergence point, the reconvergence address and each direction's active thread mask are pushed onto the Divergence Stack, and the active mask masks threads that do not enter the current branch, allowing only threads entering that branch to execute. For more details, refer to the [Warp Execution Mechanism](../../../../../docs/zh/guide/programming_guide/programming_model/ai_core_simt_programming/thread_architecture.md#warp执行机制).

  When a branch condition depends on each thread's own data, threads within the same Warp have inconsistent jump targets, and branch divergence arises. Take an if-else where half the threads in a Warp have `cond` true and half false:

  ```cpp
  if (cond) {
      r = a + b;    // if body
  } else {
      r = a - b;    // else body
  }
  ```

  The control flow of the above if-else under branch divergence is as follows:

  ![Control flow of Warp branch divergence](./figures/branch.png "Control flow of Warp branch divergence")

  The corresponding instruction stream:

  ```text
      cmp.eq  p0, cond, 1          ; each thread independently evaluates the branch condition
      branch  .BB_IF, p0           ; conditional jump: inconsistent targets within Warp → divergence
      ; ─── branch stack push: {reconvergence address .END_DVG, else-direction active mask} ───
  .BB_IF:                          ; ── Pass 1: execute if body ──
      add     r4, r2, r3           ; effective for the 16 threads with cond=1,
      ;                              the 16 threads with cond=0 are masked and idle
      branch  .END_DVG             ; if body done, jump to the reconvergence point
      ; ─── branch stack pop: switch active mask to else direction, jump back to execute else body ───
  .BB_ELSE:                        ; ── Pass 2: execute else body ──
      sub     r4, r2, r3           ; effective for the 16 threads with cond=0,
      ;                              the 16 threads with cond=1 are masked and idle
  .END_DVG:                        ; ── reconvergence point ──
      ; ─── branch stack pop: restore full mask, Warp reconverges ───
      stg     r2, [r5]             ; sequential execution resumes after reconvergence
  ```

- **Predicate Optimization**

  To avoid the above overhead, the compiler can rewrite simpler branches into straight-line execution via predication (also known as if-conversion): the branch condition no longer drives a jump; instead it is evaluated into a per-thread predicate flag. Instructions in the branch body are rewritten as predicated instructions; all 32 threads in a Warp are issued, the instruction stream stays a single straight line, and only threads whose predicate is true write back the result — threads whose predicate is false discard it (no register write-back, no memory side effects). For the same if-else, the control flow after predicate optimization is as follows:

  ![Control flow after predicate optimization](./figures/predicate.png "Control flow after predicate optimization")


  The corresponding instruction stream:

  ```text
      cmp.eq  p0, cond, 1          ; each thread independently evaluates the predicate, no jump
      add     r4, r2, r3,  p0      ; all 32 threads issued; only the 16 threads with p0=1 write back r4
      sub     r4, r2, r3, ~p0      ; all 32 threads issued; only the 16 threads with p0=0 write back r4
      ; No jump instructions, no branch stack operations; the Warp's instruction stream stays one line
  ```

  Comparing the complete instruction streams of the Warp under the two mechanisms step by step:

  | Aspect | Branch divergence | Predication |
  | --- | --- | --- |
  | Control flow instructions | One jump instruction per branch | No jumps, only one condition evaluation |
  | Branch stack | Push at divergence point, pop per direction, pop at reconvergence; nested branches stack up | No branch stack operations at all |
  | Branch body execution | Each direction executes serially, about half the threads idle per pass | Both branch bodies execute once each; every issue slot is effective for half the threads |
  | Reconvergence | Requires hardware synchronization to restore the mask | Naturally sequential, no reconvergence needed |

  Summarized as instruction sequences: under branch divergence, the Warp issues in turn `condition evaluation → jump → stack push → else body → stack pop and switch → if body → stack pop and reconverge`; under predication, the Warp issues `condition evaluation → if body → else body`. The computation of branch bodies is identical in both sequences; the difference comes entirely from the extra jumps and branch stack operations in the divergent sequence. Therefore, predication trades "issuing instructions of both directions" for saving the control overhead of jumps, the branch stack, and reconvergence.

  Whether to predicate depends not only on branch body length but also on branch hit rate (the proportion of inputs actually hitting that branch). The jump and branch stack overhead saved by predication is fixed, while the extra issued instructions are proportional to branch body length: when the branch body is short (a few instructions of assignment, return, arithmetic), the extra issue is small and predication pays off clearly; when the branch body is long (function calls, long computations), the extra execution exceeds the control overhead saved, and keeping the branch jump to execute only the hit direction is better; when a branch has a high hit rate, early return lets most threads directly skip the judgment and execution of all subsequent branches — in that case, regardless of branch body length, keeping the real jump is more favorable. In addition, branch bodies containing complex control flow or instructions with side effects (memory writes, atomic operations, synchronization) cannot be predicated, and the compiler keeps real branches for them.

  On the same branch chain, long and short branches can coexist, with both mechanisms combined: branches with high hit rates or long bodies return early upon hit; short-body branches use a unified return, overwriting a temporary variable through predicated assignments one by one. Based on the two sample groups hlog10 (short correction chain) and j0f (mixed-length interval segmentation chain), this sample shows the gains and applicable boundaries of the two mechanisms under different branch body lengths and hit situations.

- Sample Specifications:

  <table>
  <tr><td rowspan="1" align="center">Sample Type (OpType)</td><td colspan="4" align="center">hlog10 / j0f (mathematical function branch chains)</td></tr>
  <tr><td rowspan="3" align="center">Sample Input/Output</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td align="center">input / output (hlog10 group)</td><td align="center">[8192]</td><td align="center">half</td><td align="center">ND</td></tr>
  <tr><td align="center">input / output (j0f group)</td><td align="center">[8192]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">Kernel Name</td><td colspan="4" align="center">hlog10_case0_kernel / hlog10_case1_kernel / j0f_case2_kernel / j0f_case3_kernel / j0f_case4_kernel</td></tr>
  </table>

  Since the two gains must be measured separately under different branch hit situations, the j0f group constructs multiple test data batches by hit situation; the hlog10 group directly uses a single batch of input mixing special and normal values.

- j0f group:

  case2 and case3 share a regular input batch (most data falls in the low segment with `ax <= 8.0f`, about 1/16 NaN/Inf special values, constructed consistently with the hlog10 group), comparing the real-jump baseline against fully predicating both long and short branches under the regular data distribution;

  case4 further constructs four input batches by hit situation, delimiting the applicable boundary of predicate optimization with performance data at different hit rates:

  | Data Batch | Input Composition |
  | --- | --- |
  | Low-value-dominant batch (low) | low segment with `ax <= 8.0f` |
  | High-value-dominant batch (high) | high segment with `ax > 8.0f` (including middle segment) |
  | All-special batch (special) | NaN / +Inf alternating fill |
  | Mixed batch (mixed) | low segment, high segment, NaN, -Inf alternating by `i % 4` |

## Sample Implementation

This sample contains 5 independent kernels, each corresponding to a specific Case version.

| Case | Implementation Characteristics | Kernel Used | Optimization Feature |
|--------|---------------------------------------------------|--------------------|---------------------|
| Case 0 | Special-value handling placed before the normal path, early return upon hit | hlog10_case0_kernel | Baseline version, real-jump correction chain |
| Case 1 | Normal path first, correction chain with unified return overwriting temp | hlog10_case1_kernel | Predicated correction chain, fewer jumps and branch stack operations |
| Case 2 | NaN/Inf special values first, all branches return immediately upon hit | j0f_case2_kernel | Baseline version, all real jumps |
| Case 3 | Segmented computation overwriting result level by level, special-value handling last, unified return | j0f_case3_kernel | All branches unified return, both long and short branches predicated |
| Case 4 | Long-body segments return upon hit, remaining short corrections use unified return | j0f_case4_kernel | Real jumps and predication combined, each used where best |

Each case shares an identical launch configuration and input generation scheme (same-group cases use the same input-batch construction rules); the only difference is the organization of the branch chain inside the kernel.

### Performance Metrics

|             Field             | Meaning                                             |
|:---------------------------:|:-------------------------------------------------|
|      execute cycle          | Single-thread execution cycles sampled by `clock()` inside the kernel, reflecting the kernel's instruction stream length and branch overhead.          |
|      Task Duration(μs)      | Total Task time, including scheduling to the accelerator, execution time on the accelerator, and response completion time.          |
|      aiv_total_cycles       | Total CPU cycles consumed by the Task executing on the Vector Core.          |

Except for Task Duration, all metrics are averages over all blocks. execute cycle is printed directly by `clock()` inside the kernel; Task Duration and aiv_total_cycles are collected via msOpProf, see [Performance Profiling](#performance-profiling).

### Case 0: hlog10 Baseline (Branch Divergence)

**Implementation**: First correct the six special values with precision deviations, returning the corrected result immediately upon a hit; if none hit, continue with the normal path.

**Key Code**:

```cpp
__aicore__ inline half hlog10_case0(half x)
{
    if (x == static_cast<half>(0.2362060546875f)) {
        return static_cast<half>(-0.62646484375f);
    } else if (x == static_cast<half>(0.2490234375f)) {
        return static_cast<half>(-0.60400390625f);
    } else if (x == static_cast<half>(3.0703125f)) {
        return static_cast<half>(0.487060546875f);
    } else if (x == static_cast<half>(126.0625f)) {
        return static_cast<half>(2.099609375f);
    } else if (x == static_cast<half>(11496.0f)) {
        return static_cast<half>(4.05859375f);
    } else if (x == static_cast<half>(2976.0f)) {
        return static_cast<half>(3.474609375f);
    }
    float x_fp32 = __half2float(x);
    float result_fp32 = logf(x_fp32) / logf(10.0f); // log10(x) = log(x) / log(10)
    return __float2half_rn(result_fp32);
}
```

**Performance Data** (about 1/16 special values in the input, the rest normal values):

| execute cycle | Task Duration(μs) | aiv_total_cycles |
|:-----------:|:---------------:|:--------------:|
| 1890 | 14.919 | 5232.88 |

**Performance Data Analysis**:

- A minority of threads in a Warp hit special values and return early, while the majority continue along the normal path. Early-returning threads cannot make the Warp skip subsequent instructions; they still wait at the reconvergence point for other threads to finish the normal path, wasting compute resources; the branch stack overhead of the 6-level real-jump judgment chain also exists unconditionally. Together these form the comparison baseline for the optimized versions below.

### Case 1: hlog10 Optimized (Predicate Optimization)

**Implementation**: First fully execute the normal path to get temp, then correct the six special values with precision deviations, overwriting temp, and finally return temp uniformly. Each correction branch body is only one assignment statement, and the whole chain shares a unified return exit.

**Key Code**:

```cpp
__aicore__ inline half hlog10_case1(half x)
{
    float x_fp32 = __half2float(x);
    float result_fp32 = logf(x_fp32) / logf(10.0f); // log10(x) = log(x) / log(10)
    half temp = __float2half_rn(result_fp32);
    if (x == static_cast<half>(0.2362060546875f)) {
        temp = static_cast<half>(-0.62646484375f);
    } else if (x == static_cast<half>(0.2490234375f)) {
        temp = static_cast<half>(-0.60400390625f);
    } else if (x == static_cast<half>(3.0703125f)) {
        temp = static_cast<half>(0.487060546875f);
    } else if (x == static_cast<half>(126.0625f)) {
        temp = static_cast<half>(2.099609375f);
    } else if (x == static_cast<half>(11496.0f)) {
        temp = static_cast<half>(4.05859375f);
    } else if (x == static_cast<half>(2976.0f)) {
        temp = static_cast<half>(3.474609375f);
    }
    return temp;
}
```

**Performance Data** (input is the same mixed batch as Case 0: about 1/16 special values, the rest normal values):

| execute cycle | Task Duration(μs) | aiv_total_cycles |
|:-----------:|:---------------:|:--------------:|
| 1406 | 14.946 | 5194.75 |

**Optimization Analysis**:

- With the unified return, the compiler converts the branches in the correction chain into predicated straight-line execution; jumps and branch stack operations are reduced, and execute cycle drops from Case 0's 1890 to 1406.
- With the normal path moved before the branch divergence region, threads hitting special values wait less at the reconvergence point; when the special-value proportion is small, the extra execution from predication is tiny, and aiv_total_cycles drops from Case 0's 5232.88 to 5194.75, a clear performance gain.

### Case 2: j0f Baseline (Branch Divergence)

**Implementation**: First check NaN/Inf special values and return immediately; then match segments in order of low, middle, and high value segments, returning that segment's result upon a hit. Each branch returns independently, with no unified exit.

**Key Code**:

```cpp
__aicore__ inline float j0f_case2(float x)
{
    if (isnan(x)) {
        return x;
    }
    float ax = fabsf(x);
    if (isinf(ax)) {
        return 0.0f;
    }
    if (ax <= 8.0f) {
        return __internal_j0f_less8(ax);
    }
    if (ax <= 1.0e13f) {
        return __internal_j0f_middle_range(ax);
    }
    return __internal_j0f_huge_range(ax);
}
```

Expanding the early-return style above into the equivalent explicit if-else nesting, its real control flow is:

```cpp
__aicore__ inline float j0f_case2(float x)
{
    if (isnan(x)) {
        return x;
    } else {
        float ax = fabsf(x);
        if (isinf(ax)) {
            return 0.0f;
        } else {
            if (ax <= 8.0f) {
                return __internal_j0f_less8(ax);
            } else {
                if (ax <= 1.0e13f) {
                    return __internal_j0f_middle_range(ax);
                } else {
                    return __internal_j0f_huge_range(ax);   // the tail return is in fact the innermost else
                }
            }
        }
    }
}
```

Early returns implicitly carry else branches, and the jump and branch stack overhead accumulates level by level along the 4-level judgment chain.

**Performance Data** (regular input: most data falls in the low segment with `ax <= 8.0f`, about 1/16 NaN/Inf special values):

| execute cycle | Task Duration(μs) | aiv_total_cycles |
|:-----------:|:---------------:|:--------------:|
| 2345 | 14.567 | 5025.12 |

**Performance Data Analysis**:

- Most of the regular-batch inputs skip the two special-value checks and hit the low-segment branch, while a small amount of NaN/Inf input hits and returns at the first branch; the missed checks still incur jump and branch stack overhead, but each thread executes only the hit direction's segment body, with an execute cycle of 2345;
- aiv_total_cycles is the average over all blocks, where the first block includes the extra overhead of kernel launch (same below).

### Case 3: j0f Fully Predicated Version (Predicate Optimization)

**Implementation**: First compute result by the high-value segment, then execute the middle segment, low segment, and NaN/Inf special-value handling in turn, overwriting result upon a hit, and finally return uniformly.

**Key Code**:

```cpp
__aicore__ inline float j0f_case3(float x)
{
    float ax = fabsf(x);
    float result = __internal_j0f_huge_range(ax);   // unconditional high-segment computation
    if (ax <= 1.0e13f) {
        result = __internal_j0f_middle_range(ax);
    }
    if (ax <= 8.0f) {
        result = __internal_j0f_less8(ax);
    }
    if (isnan(x)) {
        result = x;                         // NaN -> NaN
    }
    if (isinf(ax)) {
        result = 0.0f;                      // ±Inf -> 0
    }
    return result;
}
```

**Performance Data** (input is the same regular batch as Case 2: low-value dominant + about 1/16 special values):

| execute cycle | Task Duration(μs) | aiv_total_cycles |
|:-----------:|:---------------:|:--------------:|
| 14692 | 22.726 | 19514.88 |

**Optimization Analysis**:

- Execute cycle rises from Case 2's 2345 to 14692, Task Duration from 14.567μs to 22.726μs, and aiv_total_cycles from 5025.12 to 19514.88. Case 3 is significantly slower than the baseline because of predicating long branch bodies: in the hlog10 group (Case 1) each correction branch body is only one assignment statement, so the extra issue cost is tiny; whereas the three segment bodies of j0f are each polynomial evaluations of dozens of instructions — after predication every input unconditionally executes the high-segment computation, and most low-segment inputs additionally execute the middle segment, so the extra execution far exceeds the saved jump and branch stack overhead.
- The Case 2 versus Case 3 comparison shows: the control overhead saved by predication is fixed, while the extra issue cost is proportional to branch body length; for long branch bodies the cost exceeds the gain. Case 4 further incorporates hit situations to explore the applicable boundary of predication.

### Case 4: j0f Optimized (Real Jumps and Predicate Optimization Combined)

**Implementation**: The low and middle segments cover wide ranges and have long branch bodies, returning upon hit; inputs that miss compute result by the high segment, then overwrite result via NaN/Inf special-value handling, and finally return uniformly.

**Key Code**:

```cpp
__aicore__ inline float j0f_case4(float x)
{
    float ax = fabsf(x);
    if (ax <= 8.0f) {
        return __internal_j0f_less8(ax);    // long branch body: return upon hit, execute only the hit direction
    }
    if (ax <= 1.0e13f) {
        return __internal_j0f_middle_range(ax);
    }
    float result = __internal_j0f_huge_range(ax);
    if (isnan(x)) {
        result = x;
    }
    if (isinf(ax)) {
        result = 0.0f;
    }
    return result;
}
```

**Performance Data**:

| Data Batch | execute cycle | Task Duration(μs) | aiv_total_cycles |
|:---:|:-----------:|:---------------:|:--------------:|
| Low-value-dominant batch (low) | 1767 | 14.389 | 4860.21 |
| High-value-dominant batch (high) | 2342 | 14.563 | 5468.46 |
| All-special batch (special) | 2395 | 14.088 | 5367.21 |
| Mixed batch (mixed) | 2704 | 14.608 | 5756.00 |

**Optimization Analysis**:

- Low-value-dominant batch: hitting the low segment returns immediately, with no extra computation beyond two missed branch checks; execute cycle is 1767, the lowest of the four batches;
- High-value-dominant batch: only the hit branch executes, execute cycle is 2342, faster than Case 3's fully predicated version;
- All-special batch: NaN/Inf inputs first pass two missed segment checks, then execute the high-segment computation, and are finally overwritten by special-value correction; execute cycle is 2395;
- Mixed batch: real jumps for long branch bodies and predication for short ones, each playing to its strength, with only a little extra execution for inputs of any distribution; execute cycle is 2704.

## Performance Comparison Summary

**Overall Optimization Effect**:

- hlog10 group (short correction branch chain): from the Case 0 baseline to the Case 1 optimized version, execute cycle drops from 1890 to 1406, a decrease of about 25.6%;
- j0f group (mixed-length segmentation chain): Case 3 predicates both long and short branches, and execute cycle on the regular input rises from Case 2's 2345 to 14692; Case 4 keeps real jumps for long branches and predicates only short corrections, faster than Case 3 on every batch, best overall.

- hlog10 group (regular input with about 1/16 special values):

| Case | execute cycle | Task Duration(μs) | aiv_total_cycles | Optimization Point |
|--------|:-----------:|:---------------:|:--------------:|--------------------|
| Case 0 | 1890 | 14.919 | 5232.88 | Baseline: special values first, early return upon hit |
| Case 1 | **1406** | 14.946 | **5194.75** | Correction chain with unified return, jumps reduced from 6 to 4 levels, last level predicated |

- j0f group case2/3 (regular input: low-value dominant + about 1/16 special values):

| Case | execute cycle | Task Duration(μs) | aiv_total_cycles | Optimization Point |
|--------|:-----------:|:---------------:|:--------------:|--------------------|
| Case 2 | **2345** | **14.567** | **5025.12** | Baseline: special values first, all branches return upon hit |
| Case 3 | 14692 | 22.726 | 19514.88 | All branches unified return, overwriting level by level, both long and short branches predicated; execute cycle above baseline |

- j0f group case4 (execute cycle, by data batch):

| Data Batch | Case 4 | Hit-rate Characteristic |
|--------|:-----:|--------------------|
| Low-value-dominant batch (low) | **1767** | High low-segment hit rate |
| High-value-dominant batch (high) | 2342 | High middle-segment hit rate |
| All-special batch (special) | 2395 | High special-value hit rate |
| Mixed batch (mixed) | 2704 | Hit rates close across directions |

It can be seen that the gains and costs of predicate optimization are jointly determined by branch body length and hit situation: predicating short, low-hit-rate branches pays off clearly; keep real jumps with early return for long or high-hit-rate branches. Combining the two mechanisms by this trade-off covers a mixed-length branch chain (Case 4).

## Tuning Recommendations

1. **Predicate short branches**: predication pays off clearly for short branch bodies, and an entire shallow correction chain gains even more; keep real branches when the branch body is long or has side effects (memory writes, atomic operations, synchronization) (see the predication applicability analysis in [Sample Description](#sample-description)).

2. **Organize branches so they are if-convertible**: use a temporary variable to carry each branch's result and return at a unified exit, without returning early mid-chain; keep each branch body to short instructions such as overwriting the temporary variable, and leave long computations in real branches.

3. **Keep early return for long branch bodies or high-hit-rate branches**: the more subsequent computation is skipped after a branch hit, the more worthwhile it is to keep the real jump and execute only the hit direction; such branches can be placed first.

## Build and Run

Run the following steps in the root directory of this sample to build and execute the sample.

- Configure Environment Variables

  Configure environment variables based on the [installation method](../../../../../docs/en/quick_start.md#prepare&install) of the CANN development kit in the current environment.

  ```bash
  source ${install_path}/cann/set_env.sh
  ```

  > **Note:** `${install_path}` is the CANN package installation directory. When no installation directory is specified, the default installation path is `/usr/local/Ascend`.

- Run the Sample

  Run the following commands in this sample directory.

  ```bash
  mkdir -p build && cd build;                         # create and enter the build directory
  cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DSCENARIO_NUM=1 ..; make -j;   # build the project (select case 1)
  ./predicate_optimization                            # run the sample
  ```

  When using NPU simulation mode, add the `-DCMAKE_ASC_RUN_MODE=sim` parameter.

  Example:
  ```bash
  cmake -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DSCENARIO_NUM=1 ..; make -j;   # NPU simulation mode
  ```

  > **Note:** Before switching cases or build modes, clean the cmake cache: run `rm CMakeCache.txt` in the build directory and re-run cmake.

- Build Options

  | Option                        | Values        | Description                                                |
  |---------------------------|------------|---------------------------------------------------|
  | `CMAKE_ASC_RUN_MODE` | `npu` (default), `sim` | Run mode: NPU execution, NPU simulation |
  | `CMAKE_ASC_ARCHITECTURES` | `dav-3510` | NPU architecture: this sample supports only dav-3510 (Ascend 950PR/Ascend 950DT) |
  | `SCENARIO_NUM` | `1` ~ `8` (default `1`) | Selects the case to run at compile time, see table below |

  | SCENARIO_NUM | Case | Description |
  | --- | --- | --- |
  | 1 | hlog10_case0 | Baseline version |
  | 2 | hlog10_case1 | Optimized version |
  | 3 | j0f_case2 | Baseline version |
  | 4 | j0f_case3 | Fully predicated version |
  | 5 / 6 / 7 / 8 | j0f_case4_low / j0f_case4_high / j0f_case4_special / j0f_case4_mixed | Optimized version; input batches are low-dominant / high-dominant / all-special / mixed in turn |

  Each run builds and runs only the selected case: build with a different SCENARIO_NUM and run in turn to complete the full comparison of each group.

## Performance Profiling

### msOpProf Tool Introduction

msOpProf is a single-operator performance profiling tool, with two usage modes: msopprof and msopprof simulator. It helps users locate operator memory, operator code, and operator instruction anomalies, achieving all-round operator tuning. It currently supports performance data collection and automatic parsing based on different run modes (on-board or simulation) and different file forms (executable files or operator binary .o files).

Use the `msOpProf` tool to collect performance data of a single component:

```bash
msopprof ./predicate_optimization   # profile the sample's performance (case decided by SCENARIO_NUM at build time)
```

After the command completes, a folder named "OPPROF_{timestamp}_XXX" is generated in the default directory. The performance data folder structure is as follows:

```text
├──dump                       # raw performance data, users need not pay attention
├──ArithmeticUtilization.csv  # cube/vector instruction cycle share
├──L2Cache.csv                # L2 Cache hit rate
├──Memory.csv                 # UB, L1 and main memory read/write bandwidth rates
├──MemoryL0.csv               # L0A, L0B, and L0C read/write bandwidth rates
├──MemoryUB.csv               # Vector and Scalar to UB read/write bandwidth rates
├──OpBasicInfo.csv            # operator basic information
├──PipeUtilization.csv        # compute unit and transfer unit time consumption and share
├──ResourceConflictRatio.csv  # UB bank group, bank conflict and resource conflict ratio among all instructions
└──visualize_data.bin         # MindStudio Insight presentation file
```

View specific performance analysis results:

```
# e.g., view Task Duration related data
cat ./OPPROF_*/OpBasicInfo.csv
```
