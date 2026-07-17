## Scalar Cache Mode Verification Sample (C_API)

## Overview

This sample verifies the correctness of `asc_set_scalar_cache_mode` and `asc_get_scalar_cache_mode` using a **set→get round-trip consistency + no-op behaviour check** strategy.

## Supported Products

- Ascend 950PR/Ascend 950DT

## Directory Structure

```
├── 00_scalar_cache_mode
│   ├── CMakeLists.txt               // Build configuration
│   ├── README.md                    // Documentation (Chinese)
│   ├── README_en.md                 // Documentation (English)
│   └── scalar_cache_mode.asc        // Sample implementation (kernel + host + main)
```

## API Description

- `asc_set_scalar_cache_mode`: Sets the Scalar Cache mode for the scalar unit. The parameter type distinguishes the read channel (`asc_load_l2_cache_mode`, CTRL[19:16]) from the write channel (`asc_store_l2_cache_mode`, CTRL[23:20]).
- `asc_get_scalar_cache_mode`: Gets the Scalar Cache mode for the scalar unit, also distinguishing read/write channels via the output parameter type.

### Enum Value Support

| Channel | Enum Type | Supported Values | Unsupported Values (set is no-op) |
|---------|-----------|-----------------|----------------------------------|
| Read | `asc_load_l2_cache_mode` | `NORMAL_FIRST_VICTIM`, `NORMAL_LAST_VICTIM`, `NORMAL_PERSISTENT`, `NOTALLOC_KEEP` | `NOTALLOC_CLEAN`, `NOTALLOC_DROP` |
| Write | `asc_store_l2_cache_mode` | `NORMAL_FIRST_VICTIM`, `NORMAL_LAST_VICTIM`, `NORMAL_PERSISTENT`, `NOTALLOC_CLEAN` | None |

## Verification Strategy

The sample defines 10 test cases, each mapped to a bit in `passMask` (bit 0 ~ bit 9). All pass when `passMask == 0x3FF`.

| Bit | Channel | Type | Set Value | Expected Get |
|-----|---------|------|-----------|-------------|
| 0 | Read | Round-trip | `NORMAL_FIRST_VICTIM` | `NORMAL_FIRST_VICTIM` |
| 1 | Read | Round-trip | `NORMAL_LAST_VICTIM` | `NORMAL_LAST_VICTIM` |
| 2 | Read | Round-trip | `NORMAL_PERSISTENT` | `NORMAL_PERSISTENT` |
| 3 | Read | Round-trip | `NOTALLOC_KEEP` | `NOTALLOC_KEEP` |
| 4 | Read | No-op | Set `NORMAL_LAST_VICTIM`, then `NOTALLOC_CLEAN` | `NORMAL_LAST_VICTIM` (unchanged) |
| 5 | Read | No-op | Set `NORMAL_LAST_VICTIM`, then `NOTALLOC_DROP` | `NORMAL_LAST_VICTIM` (unchanged) |
| 6 | Write | Round-trip | `NORMAL_FIRST_VICTIM` | `NORMAL_FIRST_VICTIM` |
| 7 | Write | Round-trip | `NORMAL_LAST_VICTIM` | `NORMAL_LAST_VICTIM` |
| 8 | Write | Round-trip | `NORMAL_PERSISTENT` | `NORMAL_PERSISTENT` |
| 9 | Write | Round-trip | `NOTALLOC_CLEAN` | `NOTALLOC_CLEAN` |

### Data Transfer Mechanism

Since `cache_ctrl.h` is mutually exclusive with `--enable-simt`, this sample uses the C-API programming model. The scalar result (`passMask`) is transferred to GM via the following pipeline:

```
PIPE_S  : 10 cases of set/get/compare → accumulate passMask
  ↓ asc_sync_notify/wait(PIPE_S, PIPE_V, EVENT_ID0)
PIPE_V  : asc_duplicate_scalar + asc_store → write to UB
  ↓ asc_sync_notify/wait(PIPE_V, PIPE_MTE3, EVENT_ID0)
PIPE_MTE3: asc_copy_ub2gm_align → copy to GM
```

The host reads the first 4 bytes (uint32_t passMask) from GM, prints PASS/FAIL per bit, and checks `passMask == 0x3FF`.

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
  ./scalar_cache_mode_example
  ```

  Expected output:
  ```bash
  [Case 0] read  NORMAL_FIRST_VICTIM round-trip ... PASS
  [Case 1] read  NORMAL_LAST_VICTIM  round-trip ... PASS
  [Case 2] read  NORMAL_PERSISTENT   round-trip ... PASS
  [Case 3] read  NOTALLOC_KEEP       round-trip ... PASS
  [Case 4] read  NOTALLOC_CLEAN      no-op ... PASS
  [Case 5] read  NOTALLOC_DROP       no-op ... PASS
  [Case 6] write NORMAL_FIRST_VICTIM round-trip ... PASS
  [Case 7] write NORMAL_LAST_VICTIM  round-trip ... PASS
  [Case 8] write NORMAL_PERSISTENT   round-trip ... PASS
  [Case 9] write NOTALLOC_CLEAN      round-trip ... PASS
  passMask=0x3ff
  [Success] Scalar cache mode verification passed.
  ```
