## Saturation Flag Verification Sample (C_API)

## Overview

This sample verifies the correctness of `asc_set_saturation_flag` and `asc_get_saturation_flag` using a **set→get round-trip consistency** strategy.

## Supported Products

- Ascend 950PR/Ascend 950DT

## Directory Structure

```
├── 00_saturation_flag
│   ├── CMakeLists.txt               // Build configuration
│   ├── README.md                    // Documentation (Chinese)
│   ├── README_en.md                 // Documentation (English)
│   └── saturation_flag.asc          // Sample implementation (kernel + host + main)
```

## API Description

- `asc_set_saturation_flag`: Sets the saturation flag for a specified compute type by writing the corresponding CTRL register bit.
- `asc_get_saturation_flag`: Gets the saturation flag for a specified compute type by reading the corresponding CTRL register bit.

### Saturation Mode to CTRL Bit Mapping

| Mode | CTRL Bit | Polarity | enable=true (saturation) | enable=false (no-sat/truncation) |
|------|---------|----------|-------------------------|----------------------------------|
| FLOAT | CTRL[48] | Normal | bit=0 | bit=1 |
| FLOAT8 | CTRL[50] | Normal | bit=0 | bit=1 |
| **INT** | **CTRL[53]** | **Inverted** | **bit=1** | **bit=0** |
| CAST | CTRL[59] | Normal | bit=0 | bit=1 |

> **Note**: INT mode has inverted polarity compared to the other three modes. `enable=true` sets CTRL[53] to 1 (saturation), `enable=false` sets it to 0 (truncation).

## Verification Strategy

The sample defines 8 test cases, each mapped to a bit in `passMask` (bit 0 ~ bit 7). All pass when `passMask == 0xFF`.

| Bit | Mode | Input (set call) | Expected Output (get) | Verification Point |
|-----|------|-----------------|----------------------|-------------------|
| 0 | FLOAT | (FLOAT, true) | true | FLOAT saturation bit round-trip |
| 1 | FLOAT | (FLOAT, false) | false | FLOAT non-saturation bit round-trip |
| 2 | FLOAT8 | (FLOAT8, true) | true | FLOAT8 saturation bit |
| 3 | FLOAT8 | (FLOAT8, false) | false | FLOAT8 non-saturation bit |
| 4 | INT | (INT, true) | true | **INT inverted polarity: enable=true→CTRL[53]=1** |
| 5 | INT | (INT, false) | false | **INT inverted polarity: enable=false→CTRL[53]=0** |
| 6 | CAST | (CAST, true) | true | CAST saturation bit |
| 7 | CAST | (CAST, false) | false | CAST truncation bit |

### Key Verification Point

The inverted polarity of INT mode (bit 4-5) is the core verification point. If the implementation incorrectly uses normal polarity for INT, set(INT, true) would write bit=0, and get would return false ≠ true, causing the case to FAIL.

### Data Transfer Mechanism

The scalar result (`passMask`) is transferred to GM via the following pipeline:

```
PIPE_S  : 8 cases of set/get/compare → accumulate passMask
  ↓ asc_sync_notify/wait(PIPE_S, PIPE_V, EVENT_ID0)
PIPE_V  : asc_duplicate_scalar + asc_store → write to UB
  ↓ asc_sync_notify/wait(PIPE_V, PIPE_MTE3, EVENT_ID0)
PIPE_MTE3: asc_copy_ub2gm_align → copy to GM
```

The host reads the first 4 bytes (uint32_t passMask) from GM, prints PASS/FAIL per bit, and checks `passMask == 0xFF`.

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
  ./saturation_flag_example
  ```

  Expected output:
  ```bash
  [Case 0] FLOAT  enable=true  (sat) ... PASS
  [Case 1] FLOAT  enable=false (no-sat) ... PASS
  [Case 2] FLOAT8 enable=true  (sat) ... PASS
  [Case 3] FLOAT8 enable=false (no-sat) ... PASS
  [Case 4] INT    enable=true  (sat) ... PASS
  [Case 5] INT    enable=false (trunc) ... PASS
  [Case 6] CAST   enable=true  (sat) ... PASS
  [Case 7] CAST   enable=false (trunc) ... PASS
  passMask=0xff
  [Success] Saturation flag verification passed.
  ```
