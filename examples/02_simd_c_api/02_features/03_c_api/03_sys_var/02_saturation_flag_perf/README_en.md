## Saturation Flag Functional Effect Verification Sample (C_API)

## Overview

This sample verifies that `asc_set_saturation_flag` controls overflow handling in actual computation. The same overflow input is computed twice (saturation ON then OFF), and results are compared against golden values.

Difference from [00_saturation_flag](../00_saturation_flag):
- `00_saturation_flag`: Verifies set→get register round-trip consistency (interface contract)
- `02_saturation_flag_perf`: Verifies saturation/truncation mode effect on computation results (functional effect)

## Supported Products

- Ascend 950PR/Ascend 950DT

## Directory Structure

```
├── 02_saturation_flag_perf
│   ├── scripts
│   │   └── gen_data.py                  // Input/golden data generation script
│   ├── CMakeLists.txt                   // Build configuration
│   ├── saturation_flag_perf.asc         // Sample entry (kernel invocation + main)
│   ├── saturation_flag_perf.h           // Kernel implementation (3 cases)
│   ├── data_utils.h                     // File I/O utilities
│   ├── README.md                        // Documentation (Chinese)
│   └── README_en.md                     // Documentation (English)
```

## API Cooperation

`asc_set_saturation_flag` only writes the CTRL register; it needs overflow-producing compute APIs to observe the effect:

| Mode | Compute API | API Level | CTRL bit | Polarity | Prerequisite strategy |
|------|------------|-----------|----------|----------|----------------------|
| FLOAT | `asc_add` (half, register-level) | `__simd_vf__` | [48] | Normal (0=sat) | `USE_API` (default) |
| INT | `asc_add` (int16, register-level) | `__simd_vf__` | [53] | Inverted (1=sat) | `USE_API` (default) |
| CAST | `asc_int322int16` (register-level) | `__simd_vf__` | [59] | Normal (0=sat) | **`USE_GLOBAL`** (required) |

> **Note**: CAST mode (CTRL[59]) only takes effect when CTRL[60]=1 (`USE_GLOBAL`). This constraint is only documented in `asc_set_ctrl.md`.
>
> UB-level conversion APIs (e.g., `asc_float2int16_rn_sync`) are not available on dav-3510, so register-level APIs with `__simd_vf__` helpers are used. Non-`_sat` variants (e.g., `asc_int322int16`) respect CTRL[59], while `_sat` variants always saturate (bypassing CTRL), so non-`_sat` variants are used for testing.

## Sample Description

### Case 1: FLOAT — half add overflow

- **Input**: x = 60000.0(half), y = 10000.0(half)
- **Compute**: 60000 + 10000 = 70000 > 65504(half MAX) → overflow to +INF
- **Saturation ON** (CTRL[48]=0): INF → 65504.0 (half MAX)
- **Saturation OFF** (CTRL[48]=1): +INF preserved

### Case 2: INT — int16 add overflow

- **Input**: x = 30000(int16), y = 30000(int16)
- **Compute**: 30000 + 30000 = 60000 > 32767(int16 MAX) → overflow
- **Saturation ON** (CTRL[53]=1): 32767 (int16 MAX)
- **Saturation OFF** (CTRL[53]=0): 60000 truncated to int16 = -5536

### Case 3: CAST — int32→int16 conversion overflow

- **Input**: x = 50000(int32)
- **Conversion**: 50000 > 32767(int16 MAX) → overflow
- **Saturation ON** (CTRL[59]=0): 32767 (int16 MAX)
- **Saturation OFF** (CTRL[59]=1): 50000 truncated to int16 = -15536

> **Output layout**: int32→int16 conversion uses PART_EVEN mode; valid values at even indices, zeros at odd indices.

## Verification

| Case | Saturation ON | Saturation OFF | Criteria |
|------|--------------|---------------|----------|
| 1 (FLOAT) | 65504.0 | +INF | Both differ and match golden → PASS |
| 2 (INT) | 32767 | -5536 | Both differ and match golden → PASS |
| 3 (CAST) | 32767 | -15536 | Both differ and match golden → PASS |

**Key verification point**: Saturation ON and OFF results **must differ**, proving `asc_set_saturation_flag` affects computation behavior.

## Build and Run

- Set environment variables

  ```bash
  source ${install_path}/cann/set_env.sh
  ```

- Build and run (Case 1 example)

  ```bash
  SCENARIO_NUM=1
  mkdir -p build && cd build
  cmake -DSCENARIO_NUM=$SCENARIO_NUM -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..
  make -j
  python3 ../scripts/gen_data.py -scenarioNum $SCENARIO_NUM
  ./saturation_flag_perf
  ```

  Expected output:
  ```bash
  [PASS] FLOAT sat-ON  (expect 65504)
  [PASS] FLOAT sat-OFF (expect +INF)
  [Success] Saturation flag FLOAT verification passed.
  ```

- Build options

  | Parameter | Description | Options | Default |
  |------|------|--------|--------|
  | `SCENARIO_NUM` | Scenario number | 1-3 | 1 |
  | `CMAKE_ASC_ARCHITECTURES` | NPU architecture | `dav-3510` (only) | `dav-3510` |
