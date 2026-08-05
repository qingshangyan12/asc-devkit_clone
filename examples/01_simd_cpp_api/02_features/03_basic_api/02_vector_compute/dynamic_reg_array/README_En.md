# Four-way Reg Value Test

## Overview

This sample isolates return-by-value behavior for a structure containing `vector_float` and `vector_bool`. It uses
four explicit maximum accumulators instead of a register array. A four-iteration loop contains four expanded
`add + max` operations, for a total of 16 additions.

The input scalar is read from `input/input.bin`. The kernel result contains 64 `float32` values and is written to
`output/output.bin`. An independent Python script compares it with `output/golden.bin`.

## Supported Products

- Ascend 950PR/Ascend 950DT

## Directory Structure

```text
dynamic_reg_array/
|-- scripts/
|   |-- gen_data.py
|   `-- verify_result.py
|-- CMakeLists.txt
|-- README.md
|-- README_En.md
|-- data_utils.h
|-- dynamic_reg_array.asc
`-- run.sh
```

## Computation

With an initial value of `3.25`, the kernel performs 16 additions by `1.0`. `max0` receives additions 1, 5, 9,
and 13, while `max1` receives additions 2, 6, 10, and 14. The final comparison between `max0` and `max1` therefore
produces `17.25` in every lane.

The executable performs 20 warmup launches followed by 1000 measured launches and prints the average host-observed
time per kernel. Use the platform profiler for device-side cycles.

## Build And Run

After configuring the CANN environment, run:

```bash
bash run.sh
```

Or execute each step manually:

```bash
cmake -S . -B build
cmake --build build -j
cd build
python3 ../scripts/gen_data.py
./demo
python3 ../scripts/verify_result.py output/output.bin output/golden.bin
```

A successful verification prints:

```text
test pass!
```
