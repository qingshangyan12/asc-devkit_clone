## Saturation Strategy Functional Effect Verification Sample (C_API)

## Overview

This sample verifies that `asc_set_saturation_strategy` switches the source of saturation control between API-level (`_sat` suffix) and global (CTRL[59] flag). 4 cross-product cases prove the strategy switch changes which control mechanism takes effect.

Difference from [01_saturation_strategy](../01_saturation_strategy):
- `01_saturation_strategy`: Verifies set→get register round-trip consistency (interface contract)
- `03_saturation_strategy_perf`: Verifies strategy switch effect on computation results (functional effect)

## Supported Products

- Ascend 950PR/Ascend 950DT

## Directory Structure

```
├── 03_saturation_strategy_perf
│   ├── scripts
│   │   └── gen_data.py                      // Input/golden data generation script
│   ├── CMakeLists.txt                       // Build configuration
│   ├── saturation_strategy_perf.asc         // Sample entry (kernel invocation + main)
│   ├── saturation_strategy_perf.h           // Kernel implementation (4 cases)
│   ├── data_utils.h                         // File I/O utilities
│   ├── README.md                            // Documentation (Chinese)
│   └── README_en.md                         // Documentation (English)
```

## CTRL[60] Mechanism

Refer to the saturation mode global/single-instruction configuration table in [SetCtrlSpr(ISASI).md](../../../../../../../docs/zh/api/SIMD-API/基础API/特殊寄存器访问/SetCtrlSpr(ISASI).md):

| CTRL[60] | Control | Description |
|----------|---------|-------------|
| 0 (USE_API) | API `_sat`/non-`_sat` suffix | **Per-instruction** saturation control: `_sat`→saturation, non-`_sat`→truncation |
| 1 (USE_GLOBAL) | CTRL[48]/CTRL[59] | **Global** saturation control: `asc_set_saturation_flag` takes effect, API suffix ignored |

**Key point**: CTRL[48] and CTRL[59] only take effect when CTRL[60]=1 (USE_GLOBAL). Under USE_API, `asc_set_saturation_flag` settings are **ignored**, and saturation behavior is determined solely by the API's `_sat`/non-`_sat` suffix.

## API Cooperation

| API | Role | Description |
|-----|------|-------------|
| `asc_set_saturation_strategy` | **Under test** | Sets CTRL[60], switches USE_API/USE_GLOBAL |
| `asc_set_saturation_flag` | **Cooperating** | Sets CTRL[59] (CAST mode), only effective under USE_GLOBAL |
| `asc_int322int16` (non-`_sat`) | **Cooperating** | Register-level int32→int16 conversion, `_sat` suffix only effective under USE_API |
| `asc_int322int16_sat` (`_sat`) | **Cooperating** | Register-level int32→int16 saturated conversion, `_sat` suffix only effective under USE_API |

## Sample Description

Uses int32→int16 conversion (CAST mode, CTRL[59]) as the test vehicle. Input 50000 (int32) exceeds int16 MAX (32767), producing distinguishable overflow behavior.

4 cases form a cross-product of strategy × API variant:

| Case | Strategy | Flag Setting | API Variant | Expected Behavior | Expected Result |
|------|----------|-------------|-------------|-------------------|-----------------|
| 1 | USE_API | CAST=true (ignored) | `asc_int322int16` (non-`_sat`) | Truncation | -15536 |
| 2 | USE_API | CAST=false (ignored) | `asc_int322int16_sat` (`_sat`) | Saturation | 32767 |
| 3 | USE_GLOBAL | CAST=true | `asc_int322int16` (non-`_sat`) | Saturation | 32767 |
| 4 | USE_GLOBAL | CAST=false | `asc_int322int16_sat` (`_sat`) | Truncation | -15536 |

### Key Verification Logic

- **Case 1 vs 3**: Same API (non-`_sat`), different strategy → different results (truncation vs saturation), proving strategy switch enables flag
- **Case 2 vs 4**: Same API (`_sat`), different strategy → different results (saturation vs truncation), proving strategy switch makes flag override API suffix
- **Case 1 vs 2**: Same strategy (USE_API), different API suffix → different results, proving USE_API respects API suffix
- **Case 3 vs 4**: Same strategy (USE_GLOBAL), different flag → different results, proving USE_GLOBAL respects flag

> **Output layout**: int32→int16 conversion uses PART_EVEN + MODE_ZEROING, valid values at even indices, zeros at odd indices. Each case outputs 128 int16 values (64 valid + 64 zeros), total output 512 int16 values.

## Build and Run

- Set environment variables

  ```bash
  source ${install_path}/cann/set_env.sh
  ```

- Build and run

  ```bash
  mkdir -p build && cd build
  cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..
  make -j
  python3 ../scripts/gen_data.py
  ./saturation_strategy_perf
  ```

  Expected output:
  ```bash
  [PASS] Case 1: USE_API + non-_sat API (expect truncation -15536)
  [PASS] Case 2: USE_API + _sat API (expect saturation 32767)
  [PASS] Case 3: USE_GLOBAL + flag=true (expect saturation 32767)
  [PASS] Case 4: USE_GLOBAL + flag=false (expect truncation -15536)
  [Success] Saturation strategy verification passed.
  ```
