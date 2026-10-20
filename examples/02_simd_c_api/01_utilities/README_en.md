# Utility Functions

## Overview

Introduction to common debugging interfaces and tools for Ascend C.

The kernel computation, data movement, and synchronization use the C API and target `dav-3510`. Debug utilities such as `printf`, `asc_dump`, and `ascendc_assert` can be shared with C++ API samples.

## Sample List

| Directory Name | Description | Supported Products |
| -------------- | ----------- | --- |
| [00_printf](./00_printf) |  Printf interface printing function | Ascend 950PR&950DT products |
| [01_assert](./01_assert) |  Assert interface exception detection function | Ascend 950PR&950DT products |
| [02_dump](./02_dump) |  Dump data debugging function | Ascend 950PR&950DT products |
| [03_clock](./03_clock) |  Kernel function execution time printing | Ascend 950PR&950DT products |
| [04_profiling](./04_profiling) |  Collecting performance data using Profiling tool | Ascend 950PR&950DT products |
| [05_sanitizer](./05_sanitizer) |  Usage of Sanitizer debugging tool | Ascend 950PR&950DT products |
| [06_simulator](./06_simulator) |  CAmodel simulation and problem analysis | Ascend 950PR&950DT products |
| [07_log](./07_log) |  Kernel log functions (log to screen/disk/level control) | Ascend 950PR&950DT products |
