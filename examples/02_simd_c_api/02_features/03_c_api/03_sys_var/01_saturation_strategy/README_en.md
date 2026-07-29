## Saturation Strategy Verification Sample (C_API)

## Overview

This sample verifies the correctness of `asc_set_saturation_strategy` and `asc_get_saturation_strategy` using a **set→get round-trip consistency** strategy.

## Supported Products

- Ascend 950PR/Ascend 950DT

## Directory Structure

```
├── 01_saturation_strategy
│   ├── CMakeLists.txt               // Build configuration
│   ├── README.md                    // Documentation (Chinese)
│   ├── README_en.md                 // Documentation (English)
│   └── saturation_strategy.asc      // Sample implementation (kernel + host + main)
```

## API Description

- `asc_set_saturation_strategy`: Sets the saturation override strategy by writing CTRL[60].
- `asc_get_saturation_strategy`: Gets the saturation override strategy by reading CTRL[60].

### Strategy to CTRL Bit Mapping

| Strategy | CTRL Bit | Meaning |
|----------|---------|---------|
| USE_API | CTRL[60]=0 | Use API-level saturation config (set via asc_set_saturation_flag) |
| USE_GLOBAL | CTRL[60]=1 | Use global saturation config, ignore API-level settings |

## Verification Strategy

The sample defines 2 test cases, each mapped to a bit in `passMask` (bit 0 ~ bit 1). All pass when `passMask == 0x3`.

| Bit | Input (set call) | Expected Output (get) | Verification Point |
|-----|-----------------|----------------------|-------------------|
| 0 | USE_API | USE_API | CTRL[60]=0 round-trip |
| 1 | USE_GLOBAL | USE_GLOBAL | CTRL[60]=1 round-trip |

### Data Transfer Mechanism

The scalar result (`passMask`) is transferred to GM via the following pipeline:

```
PIPE_S  : 2 cases of set/get/compare → accumulate passMask
  ↓ asc_sync_notify/wait(PIPE_S, PIPE_V, EVENT_ID0)
PIPE_V  : asc_duplicate_scalar + asc_store → write to UB
  ↓ asc_sync_notify/wait(PIPE_V, PIPE_MTE3, EVENT_ID0)
PIPE_MTE3: asc_copy_ub2gm_align → copy to GM
```

The host reads the first 4 bytes (uint32_t passMask) from GM, prints PASS/FAIL per bit, and checks `passMask == 0x3`.

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
  ./saturation_strategy_example
  ```

  Expected output:
  ```bash
  [Case 0] USE_API    round-trip ... PASS
  [Case 1] USE_GLOBAL round-trip ... PASS
  passMask=0x3
  [Success] Saturation strategy verification passed.
  ```
