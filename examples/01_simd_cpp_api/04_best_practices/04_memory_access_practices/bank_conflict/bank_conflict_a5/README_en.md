# A5 Bank Conflict Kernel Direct Invocation Sample

## Overview

This sample demonstrates UB (Unified Buffer) bank conflict scenarios and their performance impact on the Atlas A5 architecture (`dav-3510`). It uses the Mul instruction with different buffer address configurations to validate bank conflict trigger conditions on A5.

## A5 UB Key Features

- **256KB** UB, 8 bank groups, **2 banks per group** (16 banks total)
- Each bank: 512 rows x 32B = 16KB; each group: 32KB
- **2 read ports + 1 write port** per bank group (vs 1 read port on A2/A3)
- **Low address interleaving**: `ADDR = {DEPTH[8:0], BANK[0], BG[2:0], WIDTH[4:0]}`
- 4 subbanks per bank (only relevant for gather/scatter discrete access)

## Supported Products

- Atlas A5 Training Series / Atlas A5 Inference Series (`dav-3510` / `__NPU_ARCH__=3510`)

## Directory Structure

```
├── bank_conflict_a5
│   ├── scripts
│   │   ├── gen_data.py
│   │   └── verify_result.py
│   ├── CMakeLists.txt
│   ├── data_utils.h
│   ├── bank_conflict_a5.asc
│   ├── analysis.md
│   └── README.md
```

## How to Run

### Simulation Environment

```bash
source /path/to/cann/set_env.sh

mkdir -p build && cd build
cmake .. -DCMAKE_ASC_ARCHITECTURES=dav-3510
make -j

python3 ../scripts/gen_data.py

# Run via simulator (command depends on your env)
run_simulator ./demo

python3 ../scripts/verify_result.py output/output_z.bin output/golden.bin
```

### Switching Cases

Modify `CASE_NUM` in `bank_conflict_a5.asc` (1-4) and rebuild.

## Expected Output

```
test pass!
```
