# MatMul Self-Test Report

## Test Target

| Item | Value |
| --- | --- |
| Formula | `C[30,70] = A[30,40] @ B[40,70] + bias[1,70]` |
| Data type | FP32 |
| Transpose A | false |
| Transpose B | false |
| Block Num | 1 |
| NPU architecture | `dav-3510` |

## Test Environment

| Item | Value |
| --- | --- |
| NPU | Ascend 950PR, Health=OK |
| CANN | 9.1.0 |
| Python | 3.12.13 |
| NumPy | 2.5.1 |
| CMake | 3.22.1 |
| Test date | August 9, 2026 |
| Device validation code commit | `9a7b971a4dff96605ffd9fea156fbd2cbc64cceb` |

**Figure 1**  Test environment and NPU status
![](../figures/environment.png)

## Test Cases

| Case | Input construction | Repetitions | Validation target |
| --- | --- | ---: | --- |
| P01 | Zero A and B, nonzero bias | 10 | Bias broadcast |
| P02 | Zero A, B, and bias | 10 | Zero boundary |
| P03 | Periodic `-100/100/0` values | 10 | Input range boundary |
| P04 | Positive and negative cancellation | 10 | Cancellation error |
| P05 | Sparse basis vectors in B | 10 | Sparse layout |
| P06 | Random input with seed `20260804` | 10 | Random precision |
| P07 | Random input with seed `20260805` | 10 | Random precision |
| P08 | Random input with seed `20260806` | 10 | Random precision and performance |

The precision thresholds are as follows:

| Parameter | Value |
| --- | ---: |
| rtol | `2^-10` |
| atol | `2^-16` |
| matched ratio | `>= 0.99` |
| Maximum absolute error guard | `<= 1e-2`, or ULP at the maximum-error element `<= 32` |

## Contract Test and Build Results

| Check | Result |
| --- | --- |
| Python contract tests | PASS, `Ran 21 tests`, `OK`, exit code 0 |
| Ascend 950 NPU build | PASS, `Built target demo`, exit code 0 |

**Figure 2**  Contract test results
![](../figures/contract_tests.png)

**Figure 3**  Ascend 950 NPU build result
![](../figures/build_pass.png)

## Precision Results

| Case | matched ratio | max abs error | max ULP | max-abs ULP | Result |
| --- | ---: | ---: | ---: | ---: | --- |
| P01 | 1.000000 | 0.000000000e+00 | 0 | 0 | PASS |
| P02 | 1.000000 | 0.000000000e+00 | 0 | 0 | PASS |
| P03 | 1.000000 | 0.000000000e+00 | 0 | 0 | PASS |
| P04 | 1.000000 | 2.288818359e-03 | 1952 | 300 | PASS |
| P05 | 1.000000 | 3.576278687e-05 | 1 | 1 | PASS |
| P06 | 1.000000 | 1.559900981e-02 | 442 | 4 | PASS |
| P07 | 1.000000 | 1.549896409e-02 | 1201 | 4 | PASS |
| P08 | 1.000000 | 1.425332281e-02 | 335 | 4 | PASS |

P01-P08 were each repeated ten times. All 80 runs passed with `summary=80/80 passed`. The final P08 run reported `matched_ratio=1.000000`, `max_abs_error=1.425332281e-02`, `max_ulp_error=335`, and `max_abs_error_ulp=4`.

**Figure 4**  All 80 precision runs passed
![](../figures/precision_80_of_80.png)

## Memory and Synchronization Results

| Tool | Result |
| --- | --- |
| `mssanitizer memcheck` | PASS, `No error detected`, exit code 0 |
| `mssanitizer synccheck` | PASS, `No error detected`, exit code 0 |

**Figure 5**  memcheck result
![](../figures/memcheck.png)

**Figure 6**  synccheck result
![](../figures/synccheck.png)

## Performance Result

P08 was used as the performance case with Block Num set to 1.

| Metric | Value |
| --- | ---: |
| Kernel | `_Z13matmul_customPfS_S_S_` |
| Task Duration | 6.375 us |
| AI Core Time | 5.61 us |
| AI Core MAC Time | 0.307 us |
| MTE2 Time | 1.63 us |
| Cube Utilization | 3.030% |

**Figure 7**  P08 performance data
![](../figures/performance.png)

## Conclusion

The MatMul sample completed the contract tests, Ascend 950 NPU build, 80 precision runs, memory check, synchronization check, and performance collection on Ascend 950PR with CANN 9.1.0. All listed tests passed and the precision results satisfied the defined thresholds.
