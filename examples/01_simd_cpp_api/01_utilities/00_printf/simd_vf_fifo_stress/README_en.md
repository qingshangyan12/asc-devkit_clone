# SIMD VF FIFO Stress Test

## Overview

This directory provides a one-command FIFO print stress test. It covers a large burst inside one SIMD VF, several distinct and repeated VF calls, interleaved AICore and SIMD VF output, and integer, hexadecimal, and floating-point formatting.

Each process launches five independent kernels. Every kernel round emits 16,408 records, so one process emits 82,040 records and more than 10 MB of logs. `run.sh` executes three process-level iterations by default for more than 30 MB in total. `acl.json` configures a 16 MB per-core SIMD print area; every encoded record remains below 2 KB and continuously exercises transfer and reuse of the reserved 2 KB UB FIFO.

## Run

With `CANN_PATH`, `ASCEND_HOME_PATH`, or `ASCEND_TOOLKIT_HOME` configured, run:

```bash
./run.sh
```

The runner also detects `/usr/local/Ascend/cann`. `ASC_ARCH`, `JOBS`, and `ITERATIONS` override the default `dav-3510` architecture, build parallelism, and three iterations.

A successful run prints `FIFO_STRESS PASS`. Every record carries its round, category, and index. `verify_log.py` checks the complete record sets, duplicates, missing indexes, formatted values, stage markers, and byte count for every iteration. Any lost record fails the test.

## Logs

Each run recreates `logs/`:

- `configure.log`: CMake configuration output.
- `build.log`: build output.
- `run_N.log`: complete device and host output for iteration N.
- `verify_N.log`: automatic verification result for iteration N.
- `summary.log`: aggregate status and byte count for all iterations.
