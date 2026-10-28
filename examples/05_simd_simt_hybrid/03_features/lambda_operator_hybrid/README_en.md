# Hybrid Programming Lambda Capturing Operator Practice Sample (LeakyReLU-Affine Fusion and Clamp Operators)

## Overview

This sample is the **SIMD and SIMT hybrid programming** counterpart of the [Lambda capturing operator practice sample (pure SIMT version)](../../../03_simt_api/02_features/02_language_features/00_lambda_operator_capture/README_en.md): the same set of operators, a LeakyReLU-Affine fused operator (`y = scale * leaky_relu(x, alpha) + bias`, tallying negative values in the same pass) and a Clamp operator (`y = clamp(x, lo, hi)`), is reorganized under hybrid programming following the Lambda marking rules of each execution context, demonstrating the practical uses of Lambda capturing in hybrid operator development:

1. **Lambdas inside a SIMT VF are marked `__simt_callee__`**: capture the LeakyReLU slope and affine coefficients by value to complete the "activation → affine" fusion; capture the thread-private negative-value counter by reference to tally in the same pass, with each thread's count atomically accumulated into the per-core count via `asc_atomic_add`.
2. **Lambdas inside a SIMD VF are marked `__simd_callee__`**: capture the Clamp bounds by value for boundary computation; the scalar result is computed before the vector loop (bounds broadcast into vector registers via `asc_duplicate_scalar`), and the continuous vector clipping is then done with the `asc_min` and `asc_max` vector intrinsics. This is the standard data flow of "Lambda scalar logic plus vector intrinsics".
3. **Lambdas inside the kernel body are marked `__aicore__`** (the SIMD-side scalar code of `__global__ __vector__`): compute this core's data slice according to the core index, for reuse by both the SIMT VF and the SIMD VF.
4. **Separation of policy and framework**: the generic `elementwise_simt` framework inside the SIMT VF (a `__simt_callee__` template function) only handles thread guarding and element-wise dispatch, with the fused closure injected through the template parameter. This is isomorphic to the pure SIMT version's `elementwise_transform`, with the thread-mapping code written only once.

> [!IMPORTANT]
> In hybrid programming scenarios, Lambdas defined in each execution context **must be explicitly marked with function attributes**, otherwise compilation fails with `call to __host__ function`: mark `__simt_callee__` inside `__simt_vf__` function bodies, `__simd_callee__` inside `__simd_vf__` function bodies, and `__aicore__` inside hybrid programming kernel bodies. The marker goes after the Lambda parameter list (before `mutable` when they coexist). For the complete marking rules, see the [Lambda Expressions](../../../../docs/zh/guide/technical_appendix/cpp_standard_support/cpp_language_feature/lambda_expressions.md) documentation.

## Supported Products and CANN Software Versions

| Product | CANN Software Version |
|---|---|
| Ascend 950PR&950DT系列产品 | \>= CANN 9.3.0 |

## Directory Structure

```text
├── lambda_operator_hybrid
│   ├── CMakeLists.txt             // cmake build file (hybrid programming mode, no --enable-simt)
│   ├── lambda_operator_hybrid.asc // Hybrid programming Lambda capturing sample implementation using LeakyReLU-Affine and Clamp as the carrier
│   └── README.md                  // sample description
```

## Sample Implementation

### Execution Flow

The Host generates the input `x[i] = (i*7) % 11 - 5` covering `[-5, 5]` (covering positive values, negative values, and out-of-range clipping points at both bounds), launches 8 Vector Cores with 1024 elements per core (8192 elements in total), and a single kernel launch completes both operators and the negative-value statistics:

| Stage | Programming Mode | Role of Lambda Capturing | Expected Output |
|---|---|---|---|
| SIMT VF `simt_leaky_affine_vf` | 1024 threads/core, 1 element per thread | `leaky/affine` capture hyperparameters by value; `tally` captures the thread-private counter by reference; `fused` captures the three inner closures by reference for single-pass fusion; `asc_atomic_add` merges into the per-core count | `y1[i] = scale*leaky_relu(x[i],alpha)+bias`, `y1[5]=0.4` |
| SIMD VF `simd_clamp_vf` | continuous computation with vector intrinsics | bounds enter vector registers via `asc_duplicate_scalar`, clipping done by `asc_min`/`asc_max` | `y2[i] = clamp(x[i], -2, 3)`, `y2[5]=-2.0` |
| Kernel body (scalar) | `__aicore__` Lambda | `per_block` computes this core's slice and copies the per-block GM input slice into UB, shared by both VFs | Per-core count `negCount[b]`, total `negTotal=3723` |

Data flow: the per-block GM input slice is copied into UB via `asc_copy_gm2ub_align` → the SIMT VF computes fusion and statistics on that UB input → the SIMD VF computes Clamp on the same UB input → `asc_copy_ub2gm_align` copies both UB results back to GM. Both operators act on the original input `x[i]`; `neg_count` is written with a per-block offset so each block counts independently.

### Key Code Explanation

**Closure construction inside the SIMT VF** (isomorphic to the pure SIMT version; the only difference is that every Lambda explicitly carries the `__simt_callee__` marker):

```cpp
auto leaky  = [alpha](float x) __simt_callee__ { return x > 0.0f ? x : alpha * x; };
auto affine = [scale, bias](float x) __simt_callee__ { return x * scale + bias; };
uint32_t neg_local = 0;
auto tally  = [&neg_local](float x) __simt_callee__ { if (x < 0.0f) { ++neg_local; } };
auto fused  = [&tally, &leaky, &affine](float x) __simt_callee__ {
    tally(x);
    return affine(leaky(x));
};
elementwise_simt(fused, output, v);          // closure injected into the framework through the template parameter
asc_atomic_add(neg_count, neg_local);        // thread counts atomically merged into the per-core count
```

**Scalar/vector data flow inside the SIMD VF**: when the scalar result of a callee-Lambda participates in vector computation, compute it before the vector loop and feed it in as an operand. For example, in this sample, after the bounds are broadcast into vector registers via `asc_duplicate_scalar`, the clipping is completed at register level by `asc_min` and `asc_max`.

**Pipeline synchronization**: before `asc_copy_ub2gm_align` moves data out of the UB written by the SIMT and SIMD VFs, the Vector compute pipeline and the MTE3 copy-out pipeline must be synchronized with `asc_sync_notify(PIPE_V, PIPE_MTE3, EVENT_ID0)` and `asc_sync_wait(...)`.

## Compilation and Execution

Execute the following steps in the root directory of this sample to compile and execute the sample.
- Configure Environment Variables  
  Configure environment variables according to the [installation method](../../../../docs/en/quick_start.md#prepare&install) of the CANN development kit package on the current environment.
  ```bash
  source ${install_path}/cann/set_env.sh
  ```

  > **Note:** `${install_path}` is the CANN package installation directory. When the installation directory is not specified, it is installed to `/usr/local/Ascend` by default.

- Sample Execution

  Execute the following command in this sample directory:
  ```bash
  mkdir -p build && cd build
  cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..
  make -j
  ./demo
  ```

  When using NPU simulation mode, add the `-DCMAKE_ASC_RUN_MODE=sim` parameter.

  Example:

  ```bash
  cmake -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DSCENARIO_NUM=$SCENARIO_NUM ..; make -j;   # NPU simulation mode
  ```

  > **Note:** Clear the cmake cache before switching build modes. Execute `rm CMakeCache.txt` in the build directory, then run cmake again.

  Build option description

  | Option | Value | Description |
  |------|--------|------|
  | `CMAKE_ASC_RUN_MODE` | `npu` (default), `sim` | Run mode: NPU execution, NPU simulation |
  | `CMAKE_ASC_ARCHITECTURES` | `dav-3510` | NPU architecture: this sample supports only dav-3510 (Ascend 950PR&950DT products) |

  ```text
  [lambda_operator_hybrid] leaky-affine(SIMT VF) + clamp(SIMD VF) via capturing lambdas: PASS (y[5]=0.400000, yClamp[5]=-2.000000, negTotal=3723)
  ```
