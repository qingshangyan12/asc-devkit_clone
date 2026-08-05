# Progressive SIMD VF Assert Test

## Overview

This directory combines SIMD VF `printf` and `assert` in 16 independent cases. Every case is built and run separately, so one compiler failure or Vector Core exception cannot hide the remaining cases. The host validates stream synchronization and the Python verifier checks printed values, assertion messages, and expected behavior.

## Cases

| Case | Coverage | Expected result |
| --- | --- | --- |
| 0 | One passing site with output before and after | Successful completion |
| 1 | One plain failing assert | Vector Core exception |
| 2 | Formatted output followed by a failing assert with a message | Vector Core exception |
| 3 | Two assert sites in one SIMD VF; the first passes and the second fails | Vector Core exception |
| 4 | Several SIMD VFs interleaved with AICore output; the final VF fails | Vector Core exception |
| 5 | More than 2 KB emitted by one SIMD VF before assert | Complete output, then Vector Core exception |
| 6 | Layered SIMD callee calls with a deep failing check | Vector Core exception |
| 7 | Passing plain `ascendc_assert(expr)` | Successful completion |
| 8 | Failing plain `ascendc_assert(expr)` with post-failure suppression checks | Vector Core exception |
| 9 | Failing formatted `ascendc_assert` with signed, unsigned, hex, float, and string values | Vector Core exception |
| 10 | The same SIMD VF called twice; the first invocation passes and the second fails | Vector Core exception |
| 11 | One assert site in a loop passes repeatedly and then fails | Vector Core exception |
| 12 | All assertions pass across multiple SIMD VFs interleaved with AICore output | Successful completion |
| 13 | `ASCENDC_DUMP=0` disables failing conditions and debug output | Successful completion without device debug output |
| 14 | `NDEBUG` disables assertions while ordinary printing remains enabled | Successful completion with device output |
| 15 | Passing formatted `assert(expr, fmt, ...)` | Successful completion with post-assert output |

Every case explicitly includes `<cassert>` before `asc_assert.h`, covering the documented include-order requirement. Cases 3, 4, 6, and 12 exercise several static assert sites in one kernel and also detect the current BiSheng multi-assert compilation problem.

## Coverage Boundary

This test covers the common SIMD VF combinations: both `assert` and `ascendc_assert` aliases, plain and formatted forms, passing and failing conditions, post-failure termination, single and multiple sites, repeated VF calls, multiple VFs, SIMD callees, loop execution, interleaved printing, more than 2 KB of preceding output, mixed format arguments, `ASCENDC_DUMP=0`, `NDEBUG`, and header include order.

CPU debug mode, the SIMT programming model, ordinary AICore assertions, and forced host-acknowledgment timeouts are outside this SIMD VF example and require their corresponding specialized tests.

## Run

With `CANN_PATH`, `ASCEND_HOME_PATH`, or `ASCEND_TOOLKIT_HOME` configured, run:

```bash
./run.sh
```

The runner also detects `/usr/local/Ascend/cann`. It attempts all 16 cases and returns one aggregate status without interactive input.

## Logs

Each run recreates `logs/`:

- `configure.log`: CMake configuration output.
- `case_N_build.log`: per-case build output.
- `case_N_run.log`: complete device and host output for manual review.
- `case_N_verify.log`: per-case automatic verification result.
- `summary.log`: aggregate case status.
