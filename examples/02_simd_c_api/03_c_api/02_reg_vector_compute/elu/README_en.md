# ELU Example

## Overview
This example implements the ELU (Exponential Linear Unit) activation operator using the C API. It is a pure Vector-Core implementation that never calls the Cube-Core unit, calling APIs such as [asc_exp](../../../../../docs/en/api/SIMD-API/C-API/reg/arithmetic_compute/asc_exp.md), [asc_mul_scalar](../../../../../docs/en/api/SIMD-API/C-API/reg/arithmetic_compute/asc_mul_scalar.md) and [asc_select](../../../../../docs/en/api/SIMD-API/C-API/reg/arithmetic_compute/asc_select.md).

## Supported Products and CANN Versions

| Product | CANN Version |
| --- | --- |
| Ascend 950PR/Ascend 950DT | >= CANN 9.0.0 |

## Directory Structure
```
├── elu
│   ├── scripts
│   │   ├── gen_data.py                // Input data and ground truth generation script
│   ├── CMakeLists.txt                 // Build configuration file
│   ├── data_utils.h                   // Data read/write functions
│   ├── elu.asc                        // AscendC example implementation & invocation example
│   ├── README.md                      // Example description
│   └── README_en.md                   // Example description (English)
```

## Example Description
- Example functionality:  
  Computes the element-wise ELU activation of the input. The formula is:
  $$
  ELU(x)=
  \begin{cases}
  scale \cdot x, & x > 0 \\
  \alpha \cdot scale \cdot (e^{x \cdot inputScale} - 1), & x \le 0
  \end{cases}
  $$

- Example specifications:
  <table>
  <tr><td rowspan="1" align="center">Sample Type (OpType)</td><td colspan="3" align="center">AIV Sample</td></tr>
  <tr><td rowspan="5" align="center">Sample Input</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td></tr>
  <tr><td align="center">src</td><td align="center">any valid shape</td><td align="center">float / float16</td></tr>
  <tr><td align="center">alpha</td><td align="center">scalar</td><td align="center">float</td></tr>
  <tr><td align="center">scale</td><td align="center">scalar</td><td align="center">float</td></tr>
  <tr><td align="center">input_scale</td><td align="center">scalar</td><td align="center">float</td></tr>
  <tr><td rowspan="2" align="center">Sample Output</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td></tr>
  <tr><td align="center">dst</td><td align="center">same as src</td><td align="center">float / float16</td></tr>
  <tr><td rowspan="1" align="center">Kernel Function</td><td colspan="3" align="center">elu_custom (fp32) / elu_custom_half (fp16)</td></tr>
  </table>

- Example implementation:
  It uses segment-based dynamic tiling to copy the input into UB in segments, computes the ELU chain in the register domain (`vector_float`), then copies it out to GM; fp16 is computed in the fp32 domain and packed back to half. The whole flow is branchless, batched vector compute, supporting any valid shape.
  - Invocation
    The kernel is invoked with the `<<<>>>` kernel invocation operator.

## Build and Run
Execute the following steps in the example root directory.
- Configure the environment variables  
  Configure the environment variables according to the [installation](../../../../../docs/en/quick_start.md#prepare&install) of the CANN toolkit on your environment.
  ```bash
  source ${install_path}/cann/set_env.sh
  ```

  > **Note:** `${install_path}` is the CANN installation directory; if not specified, it defaults to `/usr/local/Ascend`.

- Build and run

  Run the following commands in the example directory.
  ```bash
  mkdir -p build && cd build;                                                    # create and enter the build directory
  cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..;make -j;                           # build the project (npu mode by default)
  python3 ../scripts/gen_data.py                                                 # generate test input data
  ./demo                                                                         # run the compiled executable
  ```

  To use NPU simulation mode, add `-DCMAKE_ASC_RUN_MODE=sim`:
  ```bash
  cmake -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..;make -j; # NPU simulation mode
  ```

  > **Note:** Clear the cmake cache before switching build modes. Run `rm CMakeCache.txt` in the build directory, then re-run cmake.

- Sample execution output  
  The output below indicates that the precision comparison passed.
  ```bash
  [Success] File mode verification passed.
  ```

  Built-in comprehensive self-test (for community task validation):
  ```bash
  ./demo --selftest
  ```
  It outputs `[Success] All cases verification passed.` and returns exit code 0.

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
