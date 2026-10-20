# Lambda Capturing Operator Practice Sample (LeakyReLU-Affine Fusion and Clamp Operators)

## Overview

This sample answers one question: **what practical problems Lambda capturing actually solves in real operator development**; for the hybrid programming counterpart implementation (including the Lambda marking rules), see the [lambda_operator_hybrid](../../../../05_simd_simt_hybrid/03_features/lambda_operator_hybrid/README_en.md) sample.

Using two simple operators as the carrier, a LeakyReLU-Affine fused operator (`y = scale * leaky_relu(x, alpha) + bias`) and a Clamp operator (`y = clamp(x, lo, hi)`), this sample demonstrates four practical uses of Lambda capturing external parameters inside a pure SIMT kernel:

1. **Capture-by-value carries operator hyperparameters**: hyperparameters such as `alpha/scale/bias/lo/hi` are passed in as kernel arguments and then **captured by value** (copied into the closure) by in-kernel Lambdas. The generic framework function `elementwise_transform` does not carry any operator parameter in its signature; it only handles thread mapping. Adding a new operator requires no extra parameters and produces no long parameter chains.
2. **Capture-by-reference accumulates statistics**: the negative-value counter `negCount` is updated in place by the statistics Lambda via **capture by reference**, so "transform and tally" is done in a single pass, with the statistics result going straight into a thread-private variable without passing through parameters or return values.
3. **Closure composition expresses a fused operator**: the outer `fused` Lambda captures the three closures `leaky/affine/tally` by reference, composing "tally → activation → affine" into a single-pass fused operator, with no separate multiply-add expansion needed for the fusion.
4. **Separation of policy and framework**: `elementwise_transform` is written once; injecting the LeakyReLU-Affine Lambda yields the fused operator, and injecting the Clamp Lambda yields the clipping operator, which keeps policy (Lambda) and execution framework (template function) orthogonal.

> [!NOTE]
> An Ascend C Lambda is a Device-side local closure: kernel arguments support only basic types, PODs, and pointers, so **a Lambda cannot be passed into a kernel from the Host side** (there is no equivalent of CUDA's `--extended-lambda`). The standard pattern is therefore: **pass hyperparameters into the kernel as plain scalars, and define Lambdas inside the kernel to capture and consume them**; this sample is organized exactly that way. In pure SIMT scenarios, in-kernel Lambdas neither need nor can carry callee attributes; in hybrid programming scenarios they must be marked with `__simt_callee__`/`__simd_callee__`/`__aicore__` according to the execution context. For details, see the [Lambda Expressions](../../../../../docs/zh/guide/technical_appendix/cpp_standard_support/cpp_language_feature/lambda_expressions.md) documentation and the hybrid programming counterpart sample.

## Supported Products and CANN Software Versions

| Product | CANN Software Version |
|---|---|
| Ascend 950PR&950DT系列产品 | \>= CANN 9.3.0 |

## Directory Structure

```text
├── 00_lambda_operator_capture
│   ├── CMakeLists.txt               // cmake build file (pure SIMT mode, --enable-simt)
│   ├── lambda_operator_capture.asc  // Lambda capturing sample implementation using LeakyReLU-Affine and Clamp as the carrier
│   └── README.md                    // sample description
```

## Sample Implementation

### Execution Flow

The Host generates the input `x[i] = (i*7) % 11 - 5` covering `[-5, 5]` (covering positive values, negative values, and out-of-range clipping points at both bounds). A single kernel launch computes both operators and tallies the number of negative values processed by each thread:

| Computation | Implementation | Expected Output |
|---|---|---|
| `y1[i] = scale * leaky_relu(x[i], alpha) + bias` | the `fused` closure (capturing `tally/leaky/affine` by reference) injected into `elementwise_transform` | `y1[5] = 0.4` (`x[5]=-3`: `2×0.1×(-3)+1`) |
| `y2[i] = clamp(x[i], lo, hi)` | the `clamp` closure (capturing `lo/hi` by value) injected into the same `elementwise_transform` | `y2[5] = -2.0` (`x[5]=-3` clipped by `lo=-2`) |
| Per-thread negative count `negStats[t]` | `tally` capturing `negCount` by reference, accumulated in the same pass as the fused operator | Sum over all threads `negTotal = 1862` (the number of negative values among the 4096 input elements, recomputed per thread following the thread mapping) |

The Host recomputes the expected values with the same formulas and the same thread mapping (`grid-stride loop`), comparing element by element with a relative tolerance of `1e-5` and the per-thread counts exactly.

### Key Code Explanation

**Generic element-wise framework (Device-side template function)**: its signature carries no operator parameter. For example:

```cpp
template <typename Op>
__aicore__ inline void elementwise_transform(Op op, const float* input, float* output, uint32_t total_length)
{
    uint32_t idx = blockIdx.x * blockDim.x + threadIdx.x;
    const uint32_t stride = gridDim.x * blockDim.x;
    for (; idx < total_length; idx += stride) {
        output[idx] = op(input[idx]);
    }
}
```

**Constructing closures inside the kernel**: capture hyperparameters by value, capture statistics state by reference, and compose closures. For example:

```cpp
auto leaky  = [alpha](float v) { return v > 0.0f ? v : alpha * v; };   // capture hyperparameters by value
auto affine = [scale, bias](float v) { return v * scale + bias; };      // capture hyperparameters by value
uint32_t negCount = 0;
auto tally  = [&negCount](float v) { if (v < 0.0f) { ++negCount; } };   // capture statistics state by reference
auto fused  = [&tally, &leaky, &affine](float v) {                      // capture inner closures by reference
    tally(v);
    return affine(leaky(v));
};
elementwise_transform(fused, input, yLeakyAffine, totalLength);
```

Injecting another closure yields the second operator: `elementwise_transform(clamp, input, yClamp, totalLength)`.

## Compilation and Execution

Execute the following steps in the root directory of this sample to compile and execute the sample.
- Configure Environment Variables  
  Configure environment variables according to the [installation method](../../../../../docs/en/quick_start.md#prepare&install) of the CANN development kit package on the current environment.
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

  The following output indicates that both operator outputs and the negative-value statistics all pass verification.

  ```text
  [lambda_operator_capture] leaky-affine + clamp via capturing lambdas: PASS (y[5]=0.400000, yClamp[5]=-2.000000, negTotal=1862)
  ```
