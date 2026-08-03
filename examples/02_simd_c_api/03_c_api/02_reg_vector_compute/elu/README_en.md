# ELU Example Using the C API (Pure Vector-Core, Register Vector Compute Mode)

## Overview

This example implements the ELU (Exponential Linear Unit) activation operator using the C API. It is a pure Vector-Core implementation that never calls the Cube-Core unit. It is based on the `02_reg_vector_compute` register vector compute mode and calls APIs such as `asc_exp`/`asc_mul_scalar`/`asc_add_scalar`/`asc_gt_scalar`/`asc_select`.

## Supported Products and CANN Versions

| Product | CANN Version |
| --- | --- |
| Ascend 950PR/Ascend 950DT | >= CANN 9.0.0 |

## Directory Structure

```
├── elu
│   ├── scripts
│   │   └── gen_data.py        // Input data and ground truth generation script
│   ├── CMakeLists.txt         // Build configuration file
│   ├── data_utils.h           // Data read/write functions
│   ├── elu.asc                // Ascend C example implementation & invocation example
│   ├── README.md              // Example description
│   └── README_en.md           // Example description (English)
```

## Example Description

- Example functionality:
  Computes the ELU activation value for each element of the input vector. The formula is:
  ```
  ELU(x) = scale * x                                        if x > 0
         = alpha * scale * (exp(x * inputScale) - 1)        if x <= 0
  ```

- Example specifications:

  | Parameter | Direction | Type | Description | Data Type | Format |
  | :--- | :--- | :--- | :--- | :--- | :--- |
  | `src` | Input | Tensor | ELU input, i.e. x in the formula | FLOAT / FLOAT16 | ND |
  | `dst` | Output | Tensor | ELU output, same shape as src | FLOAT / FLOAT16 | ND |
  | `alpha` | Input | Scalar | Activation coefficient α in the formula | FLOAT | - |
  | `scale` | Input | Scalar | Scaling coefficient scale | FLOAT | - |
  | `input_scale` | Input | Scalar | Input scaling coefficient inputScale | FLOAT | - |

- Kernel function names: `elu_custom` (fp32) / `elu_custom_half` (fp16)

- Example implementation:
  The kernel is implemented in register vector compute mode. The compute flow is:
  1. Copy the input src from Global Memory (GM) to Local Memory (UB);
  2. Compute ELU on each vector register block:
     - `y_pos = scale * x`
     - `t = x * inputScale`, `e = exp(t)`, `e_minus1 = e - 1`, `y_neg = alpha * scale * e_minus1`
     - `mask = (x > 0)`, `dst = select(mask, y_pos, y_neg)`
  3. Copy the output from Local Memory to Global Memory dst.

  fp32 and fp16 share the same fp32-domain compute chain (fully aligned with official ops-nn elu): fp32 uses `vector_float` registers with a b32 mask directly; fp16 is first expanded via `asc_loadalign_unpack` (UNPK_B16), converted to fp32 domain by `asc_half2float`, computed, then packed back to half by `asc_float2half_rn` + `asc_storealign_pack` (64 elements per repeat, `asc_update_mask_b32` auto-decrements to handle the unaligned tail).

  The whole flow is branchless, batched vector compute, satisfying the constraints of "no Host-side serial computation" and "no Cube-Core".

## Build and Run

Execute the following steps in the example root directory.

- Configure the environment variables (choose according to the CANN installation):
  ```bash
  source /usr/local/Ascend/cann/set_env.sh    # default path for root user
  # or
  source $HOME/Ascend/cann/set_env.sh          # non-root user
  ```

- Build and run (file mode, same as other examples in this directory):
  ```bash
  mkdir -p build && cd build;   # create and enter the build directory
  cmake ..; make -j;            # build the project
  python3 ../scripts/gen_data.py   # generate test input data (use an interpreter with numpy if the default lacks it)
  ./demo                        # run the example (file mode: read input/golden and compare)
  ```

  Expected output (file mode passed, exit code 0):
  ```bash
  ===== file mode (fp32, gen_data.py) =====
  [Success] matched_ratio=1 max_abs_error=xxx
  ===== file mode (fp16, gen_data.py) =====
  [Success] fp16 matched_ratio=1 max_abs_error=xxx
  [Success] File mode verification passed.
  ```

- Built-in comprehensive self-test (optional, for community task validation):
  ```bash
  ./demo --selftest             # 11 shapes × fp32/fp16 + special values / param combos / tiny-x / empty tensor
  ```
  Output `[Success] All cases verification passed.`, exit code 0.

> Note: This example is adapted to the CANN 9.0.0 ~ 9.1.0 dav-3510 interface
> (`asc_copy_gm2ub_align` 10-parameter high-dim split version, `asc_copy_ub2gm_align` with `uint8_t`
> l2_cache_mode, and Host-side linking of libm). `asc_get_vf_len()` is a new API in CANN 9.1.0
> (returns 256, i.e. the vector register length in bytes on the 3510 architecture); to stay compatible
> with CANN 9.0.0, the code uses the compile-time constant `VECTOR_LENGTH_BYTES=256` as an equivalent
> replacement, so behavior is identical on both versions. If you use another CANN version, the data-move
> API signatures may differ; always refer to the actually installed headers.

## Precision Verification

- Precision standard: satisfies the ecosystem operator precision standard (experimental_standard.md), judged by
  `|actual-golden| <= atol + rtol*|golden|`, with `matched_ratio >= 0.99` and `max_abs_error` within the limit:
  - FLOAT32: rtol=9.77e-4, atol=1.53e-5, max_abs_error<=1e-2
  - FLOAT16: rtol=1.95e-3, atol=1.95e-3, max_abs_error<=1e-1
- Test data range: [-100, 100] (covers positive and negative).
- Test shapes (11 groups, covering powers of two and powers-of-two-minus-one, including non-32B-aligned tails):
  `1 / 31 / 32 / 1023 / 1024 / 2047 / 2048 / 8191 / 16384 / 100000 / 1048575`, each run for fp32/fp16.
- Special cases: special values (NaN/±Inf/±0), 8 parameter combinations (alpha/scale/input_scale, incl. alpha=0/scale=0/negative alpha/negative input_scale/large coefficients), tiny-x precision (expm1 correction), empty tensor (shape=0).
- Verification method: `./demo` uses file mode by default, reading the input/golden bins generated by
  `scripts/gen_data.py` and comparing; `./demo --selftest` is a self-contained comprehensive test (Host
  generates golden and compares with the Device result per the ecosystem standard); `scripts/gen_data.py`
  can independently generate input/golden bins (fp32/fp16) for external comparison.
- expm1 precision correction: when `|x*inputScale|` is small, the negative branch uses a Horner series to
  approximate expm1, avoiding the catastrophic cancellation of `exp(t)-1`.
