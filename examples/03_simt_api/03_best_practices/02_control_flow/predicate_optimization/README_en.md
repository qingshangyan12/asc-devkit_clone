# Warp Branch Divergence Predicate Optimization Sample

## Overview

This sample demonstrates tuning methods for [Warp Branch Divergence](../../../../../docs/zh/guide/programming_guide/programming_model/ai_core_simt_programming/thread_architecture.md#warp执行机制) in SIMT programming mode through the `hlog10` and `j0f` interfaces, focusing on the performance gains and applicable scenarios of predicate optimization.

## Supported Products and CANN Versions

| Product | CANN Version |
|------|-------------|
| Ascend 950PR/Ascend 950DT | >= CANN 9.2.0 |

## Directory Structure

```text
├── predicate_optimization
│   ├── predicate_optimization.asc  // SIMT implementation and invocation sample
│   ├── figures                     // Image resources for README
│   ├── CMakeLists.txt              // cmake build script
│   ├── README.md                   // Sample documentation
│   └── README_en.md                // English sample documentation
```

## Basic Concepts

- **Warp and Branch Divergence**

  In SIMT programming mode, a Warp is the basic scheduling and execution unit. When branch conditions appear in the code, threads within a Warp diverge, and each branch executes serially. Take a simple if statement as an example:

  ```cpp
  if (cond) {
      r = a + b;
  } else {
      r = a - b;
  }
  ```

  The control flow of the above if-else under branch divergence is as follows:

  ![Control flow of Warp branch divergence](./figures/branch.png)

  The execution order is as follows:
  1. START_DVG pushes the current active mask onto the stack.
  2. branch cond jumps to the else branch and pushes the PC and active mask of the if branch onto the stack.
  3. Execute the else branch.
  4. The first END_DVG pops the stack, jumps to the PC of the if branch, and sets the active mask.
  5. Execute the if branch.
  6. The second END_DVG pops the stack and sets the active mask.
  7. The Warp exits the divergence state.

- **Predicate Optimization**

  To avoid the overhead caused by branch switching, the compiler can rewrite simple branches through predicate optimization. Instructions in the branch body are rewritten as predicated instructions. All 32 threads in a Warp are issued, the instruction stream remains straight-line, only threads whose predicate is true write back results, and threads whose predicate is false discard results without writing back registers or causing memory side effects. The control flow after predicate optimization is as follows:

  ![Control flow after predicate optimization](./figures/predicate.png)

  When warp divergence occurs, each branch can only execute serially. While one branch is executing, threads that do not enter that branch are masked by the active thread mask and idle. The hardware pushes the reconvergence address and the active thread mask of each direction onto the branch stack. After all branches finish execution, the Warp reconverges at the reconvergence point. Predicate optimization can eliminate jump and branch stack overhead and improve operator performance.

## hlog10 Sample Description

- Sample Function

  Take the [hlog10](../../../../../docs/zh/api/SIMT-API/math_functions/half_type/half_math_functions/hlog10.md) interface as an example. The hlog10 interface uses $\log_{10}(x) = \dfrac{\log(x)}{\log(10)}$ to calculate the common logarithm of input data and corrects six special values with precision deviations. The input contains 8192 half elements. By comparing the performance of different special-value handling methods, this sample shows the source of the performance gain.

- Sample Specifications:
  <table>
  <tr><td rowspan="1" align="center">Sample Type (OpType)</td><td colspan="4" align="center">hlog10</td></tr>
  <tr><td rowspan="3" align="center">Sample Input/Output</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td align="center">input / output</td><td align="center">[8192]</td><td align="center">half</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">Kernel Name</td><td colspan="4" align="center">hlog10</td></tr>
  </table>

## hlog10 Sample Implementation

| Case | Implementation Characteristics | Kernel Used | Branch Chain Handling |
|--------|---------------------------------------------------|--------------------|---------------------|
| Case 0 | Uses early return to handle special values | hlog10 | Baseline divergence version |
| Case 1 | Uses a temporary variable to handle special values and returns uniformly at the end | hlog10 | Predicate optimization |

Each case launches 8 blocks by default, with 1024 threads per block. Each thread processes one element.

### Performance Metrics

| Field | Meaning |
|:---------------------------:|:-------------------------------------------------|
| execute cycle | Single-thread execution cycles sampled by `clock()` inside the kernel, reflecting the length of the kernel instruction stream and branch overhead. |
| Task Duration(μs) | Total Task time, including scheduling to the accelerator, execution time on the accelerator, and response completion time. |
| aiv_total_cycles | Total CPU cycles consumed by the Task executing on the Vector Core. |

To prevent warp scheduling from affecting the result, execute cycle is collected with 1 block and 32 threads.

### Case 0: hlog10 Branch Chain Divergence Version

**Implementation**: Handles precision-abnormal special values through early return.

**Key Code**:

```cpp
__aicore__ inline half hlog10(half x)
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
    float result_fp32 = logf(x_fp32) / logf(10.0f);
    return __float2half_rn(result_fp32);
}
```

The actual control flow of the early-return style above is:

```cpp
__aicore__ inline half hlog10(half x)
{
    if (x == static_cast<half>(0.2362060546875f)) {
        return static_cast<half>(-0.62646484375f);
    } else {
        if (x == static_cast<half>(0.2490234375f)) {
            return static_cast<half>(-0.60400390625f);
        } else {
            if (x == static_cast<half>(3.0703125f)) {
                return static_cast<half>(0.487060546875f);
            } else {
                if (x == static_cast<half>(126.0625f)) {
                    return static_cast<half>(2.099609375f);
                } else {
                    if (x == static_cast<half>(11496.0f)) {
                        return static_cast<half>(4.05859375f);
                    } else {
                        if (x == static_cast<half>(2976.0f)) {
                            return static_cast<half>(3.474609375f);
                        } else {
                            float x_fp32 = __half2float(x);
                            float result_fp32 = logf(x_fp32) / logf(10.0f);
                            return __float2half_rn(result_fp32);
                        }
                    }
                }
            }
        }
    }
}
```

**Performance Data**:

| execute cycle | Task Duration(μs) | aiv_total_cycles |
|:-----------:|:---------------:|:--------------:|
| 218 | 2.246 | 2458.75 |

**Performance Data Analysis**:

- The interface has six levels of branch conditions that produce divergence and introduce additional jump and branch stack overhead.

### Case 1: hlog10 Branch Chain Predicate Optimization Version

**Implementation**: First fully executes the normal-value path to obtain the temporary variable temp, then overwrites the temporary variable level by level to handle precision-abnormal special values.

**Key Code**:

```cpp
__aicore__ inline half hlog10(half x)
{
    float x_fp32 = __half2float(x);
    float result_fp32 = logf(x_fp32) / logf(10.0f);
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

**Performance Data**:

| execute cycle | Task Duration(μs) | aiv_total_cycles |
|:-----------:|:---------------:|:--------------:|
| 190 | 2.186 | 2400.62 |

**Optimization Analysis**:

- Early return is difficult to trigger in actual scenarios. SIMT code executes in units of Warps, so all threads in a Warp must contain special values for early return to occur. This probability is negligible. In most scenarios, special-value threads must wait for other threads to finish calculation before continuing.
- Writing the code as a temporary variable that stores intermediate values, is overwritten level by level, and finally returns uniformly guides the compiler to perform predicate optimization, eliminating jump and branch stack overhead. Compared with Case 0, execute cycle decreases from 218 to 190, a reduction of about 12.8%.

Predicate optimization improves performance by eliminating jump and branch stack overhead, but the compiler does not apply it to complex logic. Therefore, simple logic such as special-value handling is recommended for predicate optimization.
Predicate optimization executes all instructions in all branches, while the divergence mechanism executes only the necessary paths. If a branch has a high hit rate, predicate optimization causes these threads to execute many useless instructions, resulting in performance loss. In this case, using divergence saves computation overhead. The following sample uses different handling methods for branches with different hit rates to show the applicable scope of predicate optimization.

## j0f Sample Description

- Sample Function

  Take the [j0f](../../../../../docs/zh/api/SIMT-API/math_functions/float_math_functions/j0f.md) interface as an example. The j0f interface calculates the first-kind zero-order Bessel function of the input data. The interface divides calculation into three segments by the absolute value of the input data and separately handles two special values.

- Sample Specifications:
  <table>
  <tr><td rowspan="1" align="center">Sample Type (OpType)</td><td colspan="4" align="center">j0f</td></tr>
  <tr><td rowspan="3" align="center">Sample Input/Output</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td align="center">input / output</td><td align="center">[8192]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">Kernel Name</td><td colspan="4" align="center">j0f</td></tr>
  </table>

  Case 2 and Case 3 compare the divergence baseline with predicate optimization on input2. Case 4 constructs two input batches by branch hit situation and explores the applicable boundary of predicate optimization by comparing performance data under different hit rates:

  | Input Data | Data Range |
  | --- | --- |
  | input1 | Random values in the real domain, including special values |
  | input2 | Random values in the range `[-1.0e13f,1.0e13f]` |

## j0f Sample Implementation

| Case | Implementation Characteristics | Kernel Used | Branch Chain Handling |
|--------|---------------------------------------------------|--------------------|---------------------|
| Case 2 | Uses early return to handle all branches | j0f | Baseline divergence version |
| Case 3 | Uses a temporary variable to store intermediate values, overwrites it level by level, and returns uniformly at the end | j0f | Predicate optimization with unified return |
| Case 4 | Uses early return for high-hit-rate branches and a temporary variable for the remaining branches, then returns uniformly at the end | j0f | Combination of divergence and predicate optimization |

The performance metrics and collection methods of the cases in this group are the same as above. See [Performance Metrics](#performance-metrics).

### Case 2: j0f Branch Chain Divergence Version

**Implementation**: Handles each branch through early return.

**Key Code**:

```cpp
__aicore__ inline float j0f(float x)
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

**Performance Data**:

| execute cycle | Task Duration(μs) | aiv_total_cycles |
|:-----------:|:---------------:|:--------------:|
| 1111 | 2.486 | 2802.00 |

**Performance Data Analysis**:

- input2 is in the range `[-1.0e13f,1.0e13f]`. Each thread executes only the branch body in the hit direction, while branches that are not hit still generate jump and branch stack overhead. execute cycle is 1111.

### Case 3: j0f Branch Chain Unified Return Version

**Implementation**: Uses a temporary variable to store intermediate results. Each branch overwrites the temporary variable when hit, and the function returns uniformly at the end.

**Key Code**:

```cpp
__aicore__ inline float j0f(float x)
{
    float ax = fabsf(x);
    float result = __internal_j0f_huge_range(ax);
    if (ax <= 1.0e13f) {
        result = __internal_j0f_middle_range(ax);
    }
    if (ax <= 8.0f) {
        result = __internal_j0f_less8(ax);
    }
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

| execute cycle | Task Duration(μs) | aiv_total_cycles |
|:-----------:|:---------------:|:--------------:|
| 3490 | 8.587 | 12086.38 |

**Optimization Analysis**:

- Compared with Case 2, execute cycle increases from 1111 to 3490, Task Duration increases from 2.486 μs to 8.587 μs, and aiv_total_cycles increases from 2802.00 to 12086.38. Case 3 is significantly slower than the baseline because the unified return causes many threads to perform useless calculations, resulting in performance loss.
- Comparing Case 2 and Case 3 shows that predicate optimization does not always bring performance gains when warp divergence occurs. Case 4 introduces branch hit situations on this basis to explore the applicable boundary of predicate optimization.

### Case 4: j0f Branch Chain Version with Early Return for High-Hit-Rate Branches and Predicate Optimization for Low-Hit-Rate Branches

**Implementation**: Uses early return for high-hit-rate branches and a temporary variable to store the calculation results of low-hit-rate branches, guiding the compiler to perform predicate optimization on the low-hit-rate branches.

**Key Code**:

```cpp
__aicore__ inline float j0f(float x)
{
    float ax = fabsf(x);
    if (ax <= 8.0f) {
        return __internal_j0f_less8(ax);
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
| input1 | 3646 | 12.502 | 17648.88 |
| input2 | 1115 | 2.453 | 2793.00 |

**Optimization Analysis**:

- input1: The input data covers the real domain and includes special values. Segment jump branches in a Warp produce divergence, and execute cycle is 3646.
- input2: All threads return early and do not execute subsequent branch calculations that are not hit. execute cycle is 1115, close to the Case 2 baseline of 1111. Task Duration and aiv_total_cycles are 2.453 μs and 2793.00 respectively, basically the same as the Case 2 values of 2.486 μs and 2802.00. The overall overhead is comparable to the baseline.

## Performance Comparison Summary

**Overall Optimization Effect**:

- hlog10 sample: From the Case 0 baseline to the Case 1 optimized version, execute cycle decreases from 218 to 190, a reduction of about 12.8%.
- j0f sample: With the same input composition, execute cycle increases from 1111 in the Case 2 baseline to 3490 in Case 3. On input2, Case 4 achieves an execute cycle of 1115, close to Case 2 and significantly faster than Case 3.

- hlog10 sample:

| Case | execute cycle | Task Duration(μs) | aiv_total_cycles | Optimization Point |
|--------|:-----------:|:---------------:|:--------------:|--------------------|
| Case 0 | 218 | 2.246 | 2458.75 | Baseline version with special values placed first and early return upon hit |
| Case 1 | **190** | 2.186 | 2400.62 | Correction chain with unified return; the whole chain is predicated to eliminate jump and branch stack overhead |

- j0f group, Case 2/3 (input2):

| Case | execute cycle | Task Duration(μs) | aiv_total_cycles | Optimization Point |
|--------|:-----------:|:---------------:|:--------------:|--------------------|
| Case 2 | **1111** | 2.486 | 2802.00 | Baseline version with special values placed first and immediate return upon any branch hit |
| Case 3 | 3490 | 8.587 | 12086.38 | All branches use unified return and level-by-level overwrite; both long and short branches are fully predicated, and execute cycle is higher than the baseline |

- j0f group, Case 4 (execute cycle by data batch):

| Data Batch | Case 4 | Hit Rate Characteristics |
|--------|:-----:|--------------------|
| input1 | 3646 | Hit rates of the low, middle, and high value segments are close; includes NaN/-Inf special values |
| input2 | **1115** | High hit rate in the middle value segment |

## Tuning Suggestions

1. **Use predicate optimization for branch chains**: When warp divergence occurs in a simple branch, use a temporary variable to guide the compiler to perform predicate optimization and eliminate branch jump overhead.

2. **Combine divergence and predicate optimization**: Predicate optimization executes all instructions in all branches, while divergence executes only the necessary paths. When branches in a chain have different hit rates, keep divergence for high-hit-rate branches and use predicate optimization for low-hit-rate branches. This avoids unnecessary instructions while eliminating branch jump overhead.

## Build and Run

Perform the following steps in the sample root directory to build and run the sample.

- Configure environment variables
  Configure environment variables according to the [installation method](../../../../../docs/zh/quick_start.md#prepare&install) of the CANN development kit on the current environment.

  ```bash
  source ${install_path}/cann/set_env.sh
  ```

  > **Note:** `${install_path}` is the CANN installation directory. If no installation directory is specified, the default installation path is `/usr/local/Ascend`.

- Run the sample

  Run the following commands in the sample directory.

  ```bash
  mkdir -p build && cd build;                         # Create and enter the build directory
  cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DSCENARIO_NUM=1 ..; make -j;   # Build the project (select case 1)
  ./predicate_optimization                            # Run the sample
  ```

  To use NPU simulation mode, add the `-DCMAKE_ASC_RUN_MODE=sim` option.

  Example:
  ```bash
  cmake -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DSCENARIO_NUM=1 ..; make -j;   # NPU simulation mode
  ```

  > **Note:** Before switching the case or build mode, clear the CMake cache. Run `rm CMakeCache.txt` in the build directory and run cmake again.

- Build options

  | Option | Optional Values | Description |
  |---------------------------|------------|---------------------------------------------------|
  | `CMAKE_ASC_RUN_MODE` | `npu` (default), `sim` | Running mode: NPU or NPU simulation |
  | `CMAKE_ASC_ARCHITECTURES` | `dav-3510` | NPU architecture: this sample supports only dav-3510 (Ascend 950PR/Ascend 950DT) |
  | `SCENARIO_NUM` | `1` ~ `6` (default `1`) | Case selected at build time; see the following table |
  | `CYCLE_PROFILE` | `ON`, `OFF` (default) | Enables execute cycle collection |

  | SCENARIO_NUM | Case | Description |
  | --- | --- | --- |
  | 1 | hlog10 | Baseline version |
  | 2 | hlog10 | Optimized version |
  | 3 | j0f | Baseline version |
  | 4 | j0f | Fully predicated version |
  | 5 | j0f | Optimized version (input2) |
  | 6 | j0f | Optimized version (input1) |

  Each build contains only the selected case. Rebuild and run with different SCENARIO_NUM values to complete the full comparison of all cases and groups. execute cycle must be collected in a separate build with `-DCYCLE_PROFILE=ON`. Task Duration and aiv_total_cycles are collected with the default build and msOpProf.

## Performance Profiling

### msOpProf Introduction

msOpProf is a single-operator performance profiling tool. It provides msopprof and msopprof simulator modes. The tool helps users locate exceptions in operator memory, operator code, and operator instructions, and supports comprehensive operator tuning. It currently supports performance data collection and automatic parsing in different running modes (on-board or simulation) and for different file forms (executable files or operator binary .o files).

Use `msOpProf` to obtain performance data for a single component:

```bash
msopprof ./predicate_optimization   # Profile the sample (the case is determined by SCENARIO_NUM at build time)
```

After the command completes, a folder named OPPROF_{timestamp}_XXX is generated in the default directory. The performance data folder structure is as follows:

```text
├──dump                       # Raw performance data; users do not need to focus on it
├──ArithmeticUtilization.csv  # cube/vector instruction cycle ratio
├──L2Cache.csv                # L2 Cache hit rate
├──Memory.csv                 # UB, L1, and main memory read/write bandwidth rates
├──MemoryL0.csv               # L0A, L0B, and L0C read/write bandwidth rates
├──MemoryUB.csv               # Vector and Scalar read/write bandwidth rates to UB
├──OpBasicInfo.csv            # Basic operator information
├──PipeUtilization.csv        # Collection of compute unit and transfer unit time and ratio
├──ResourceConflictRatio.csv  # Ratios of bank group, bank conflict, and resource conflicts on UB among all instructions
└──visualize_data.bin         # MindStudio Insight presentation file
```

To view detailed profiling results:

```
# For example, view Task Duration data
cat ./OPPROF_*/OpBasicInfo.csv
```
