# UB Bank Conflict and Address Overlap Scenario Analysis Example

## Overview

This example is based on the Add instruction. By configuring different LocalTensor addresses, it verifies and analyzes the impact of bank conflicts and address overlaps in UB (Unified Buffer) on performance. The example uses the compile-time parameter `SCENARIO_NUM` to select different address configuration scenarios, with a total of 8 scenarios covering typical cases such as no conflict, read-read conflict, read-write conflict, and complete address overlap.

## Supported Products and CANN Software Versions

| Product | CANN Software Version |
|---------|----------------------|
| Atlas A3 Training Series Products / Atlas A3 Inference Series Products | >= CANN 9.0.0 |
| Atlas A2 Training Series Products / Atlas A2 Inference Series Products | >= CANN 9.0.0 |

## Directory Structure

``` text
├── bank_conflict
│   ├── scripts
│   │   ├── gen_data.py                    // Input data and ground truth data generation script
│   │   └── verify_result.py               // Verification script to check if output data matches ground truth
│   ├── figures                            // UB memory layout diagrams and read/write illustrations for each scenario
│   ├── CMakeLists.txt                     // Build project file
│   ├── config.h                           // Address configurations for each SCENARIO_NUM
│   ├── data_utils.h                       // Data read/write functions
│   ├── bank_conflict.asc                  // Ascend C example implementation and usage sample
│   └── README.md                          // Example documentation
```

## UB (Unified Buffer) Memory Structure

![UB Memory Layout](./figures/ub_memory_layout.png)

- On the A2/A3 architecture, the UB space size is 192KB;
- There are 48 **banks** in total, forming 16 **bank groups**, with each bank group containing 3 banks (`bank_id` = 0, 1, 2);
- Each bank consists of 128 rows (`BANK_DEPTH = 128`), with each row being 32B long (`BANK_WIDTH = 32B`), making each bank 4KB in size;
- **Bank global numbering rule**: `bank_number = bank_group_id + bank_id * 16`.
- The same bank_number indicates the same bank; the same bank_group_id indicates the same bank group.

## UB Address Encoding Rules

The UB 18-bit address encoding follows the rules below:

`UB_ADDR[17:0] = { BANK[1:0], BANK_DEPTH[6:0], BG[3:0], BANK_WIDTH[4:0] }`

| Bit Field | BANK[1:0] | BANK_DEPTH[6:0] | BG[3:0] | BANK_WIDTH[4:0] |
| --- | --- | --- | --- | --- |
| Bits | 2 bits | 7 bits | 4 bits | 5 bits |
| Meaning | bank_id | row_id (row number within bank) | bank_group_id (bank group number) | offset (intra-row offset) |

**Decoding example**: Address `0x10020`

Convert the address to 18-bit binary, divided by bit fields: `01 0000000 0001 00000`

| Bit Field | Binary Value | Decimal | Meaning |
| --- | --- | --- | --- |
| BANK[1:0] | `01` | 1 | bank_id = 1 |
| BANK_DEPTH[6:0] | `0000000` | 0 | row_id = 0 |
| BG[3:0] | `0001` | 1 | bank_group_id = 1 |
| BANK_WIDTH[4:0] | `00000` | 0 | offset = 0 |

Based on the UB memory layout diagram and bank numbering rules above, this address is located in the bank with bank_id = 1 within bank group 1, i.e., global bank 17 (`1 + 1 x 16 = 17`), at row 0.

## Bank Conflict Types

Based on the memory structure, bank conflicts are mainly classified into the following three types:

- Read-read conflict: Multiple read operations simultaneously attempt to access the same bank group;
- Write-write conflict: Multiple write operations simultaneously attempt to access the same bank group;
- Read-write conflict: A read operation and a write operation simultaneously attempt to access the same bank.

## Scenario Details

This example conducts verification and analysis from two perspectives: bank conflicts and address overlaps. The input data type for all scenarios is `float`, with a data length of 4096, using continuous read/write mode (`BlkStride = 1`, `RepStride = 8`, `mask = 64`). Special configurations are described separately in the corresponding scenario. The kernel function name is `add_custom`.

> **Performance note**: The "Benchmark Performance" in the table below represents the cycle count of the Add instruction executed 1000 consecutive times. Lower values indicate better performance.

| Scenario | Formula | Description | Bank Conflict Status | src0 Base Addr | src1 Base Addr | dst Base Addr | Benchmark Perf | Remarks |
| --- | --- | --- | --- | --- | --- | --- | --- | --- |
| 1 | dst = src0 + src1 | src0/src1/dst base addresses distributed across different banks in different bank groups | No read-read or read-write conflict | 0 | 0x10100 | 0x20000 | 2418 | Baseline performance reference (no bank conflict) |
| 2 | dst = src0 + src1 | src0/src1 base addresses in the same bank | **Read-read conflict** | 0 | 0x4000 | 0x10000 | 4195 | Verify read-read conflict (multiple reads accessing the same bank) |
| 3 | dst = src0 + src1 | src0/src1 base addresses in the same bank group | **Read-read conflict** | 0 | 0x10000 | 0x14100 | 4195 | Verify read-read conflict (multiple reads accessing the same bank group) |
| 4 | dst = src0 + src1 | src0/dst base addresses in the same bank | No read-write conflict (hardware optimization) | 0 | 0x10100 | 0x4000 | 2501 | Baseline reference for scenario 5 |
| 5 | dst = src0 + src1 | src0/dst base addresses not in the same bank | **Read-write conflict** | 0 | 0x10100 | 0x4020 | 3612 | Verify read-write conflict (compare with scenario 4) |
| 6 | dst = src0 + src1 | src1/dst base addresses in the same bank,<br>src1/dst addresses completely overlap | No read-write conflict (hardware optimization) | 0 | 0x10100 | 0x10100 | 2501 | Verify src/dst address overlap (no read-write conflict, hardware optimization) |
| 7 | dst = src0 + src1 | src0 repeatedly reads the same datablock on each repeat (src0BlkStride=0, src0RepStride=1),<br>src1/dst addresses completely overlap | **Read-read conflict** | 0 | 0x10100 | 0x10100 | 3778 | Compare with scenario 6/scenario 8, verify correlation between src0RepStride configuration and bank conflicts |
| 8 | dst = src0 + src1 | src0 repeatedly reads the same datablock on each repeat (src0BlkStride=0, src0RepStride=8),<br>src1/dst addresses completely overlap | No read-read or read-write conflict | 0 | 0x10100 | 0x10100 | 2501 | Compare with scenario 7, verify correlation between src0RepStride configuration and bank conflicts |

Detailed analysis for each scenario is provided in the corresponding sections below.

### Scenario 1: No Read-Read Conflict, No Read-Write Conflict

#### Address Decoding

| Operand | Base Address | 18-bit Binary (BANK DEPTH BG WIDTH) | bank_id | bank_group_id | Global Bank Number |
| --- | --- | --- | --- | --- | --- |
| src0 | `0x0` | `00 0000000 0000 00000` | 0 | 0 | 0 |
| src1 | `0x10100` | `01 0000000 1000 00000` | 1 | 8 | 24 |
| dst | `0x20000` | `10 0000000 0000 00000` | 2 | 0 | 32 |

#### UB Read/Write Illustration

![scenario1](./figures/scenario1.png)

- Every 8 green data blocks (32B) represent the data read by src0 per repeat (256B);
- Every 8 yellow data blocks (32B) represent the data read by src1 per repeat (256B);
- Every 8 blue data blocks (32B) represent the data written by dst per repeat (256B);
- Number of iterations required to process all data: `repeat_times = data_length * sizeof(float) / bytes_per_repeat = 4096 * 4 / 256 = 64`.

#### Analysis Conclusion

- 1st repeat: src0 (bank_id=0, bg 0-7) and src1 (bank_id=1, bg 8-15): all datablocks in the repeat are located in different bank groups, and subsequent repeats follow the same pattern — **no read-read conflict**.
- 1st repeat: src0/src1 reads and dst writes (bank_id=2, bg 0-7): all datablocks in the repeat are located in different banks, and subsequent repeats follow the same pattern — **no read-write conflict**.

### Scenario 2: src0/src1 Base Addresses in the Same Bank — Read-Read Conflict

#### Address Decoding

| Operand | Base Address | 18-bit Binary (BANK DEPTH BG WIDTH) | bank_id | bank_group_id | Global Bank Number |
| --- | --- | --- | --- | --- | --- |
| src0 | `0x0` | `00 0000000 0000 00000` | 0 | 0 | 0 |
| src1 | `0x4000` | `00 0100000 0000 00000` | 0 | 0 | 0 |
| dst | `0x10000` | `01 0000000 0000 00000` | 1 | 0 | 16 |

#### UB Read/Write Illustration

![scenario2](./figures/scenario2.png)

- Every 8 green data blocks (32B) represent the data read by src0 per repeat (256B);
- Every 8 yellow data blocks (32B) represent the data read by src1 per repeat (256B);
- Every 8 blue data blocks (32B) represent the data written by dst per repeat (256B);
- Number of iterations required to process all data: `repeat_times = 4096 * 4 / 256 = 64`.

#### Analysis Conclusion

- 1st repeat: src0 (bank_id=0, bg 0-7) and src1 (bank_id=0, bg 0-7): all datablocks in the repeat are located in the same bank group with the same bank_id, and subsequent repeats follow the same pattern — **read-read conflict exists**.
- 1st repeat: src0/src1 reads (bank_id=0, bg 0-7) and dst writes (bank_id=1, bg 0-7): different bank_ids, not in the same bank, and subsequent repeats follow the same pattern — **no read-write conflict**.

### Scenario 3: src0/src1 Base Addresses in the Same Bank Group — Read-Read Conflict

#### Address Decoding

| Operand | Base Address | 18-bit Binary (BANK DEPTH BG WIDTH) | bank_id | bank_group_id | Global Bank Number |
| --- | --- | --- | --- | --- | --- |
| src0 | `0x0` | `00 0000000 0000 00000` | 0 | 0 | 0 |
| src1 | `0x10000` | `01 0000000 0000 00000` | 1 | 0 | 16 |
| dst | `0x14100` | `01 0100000 1000 00000` | 1 | 8 | 24 |

#### UB Read/Write Illustration

![scenario3](./figures/scenario3.png)

- Every 8 green data blocks (32B) represent the data read by src0 per repeat (256B);
- Every 8 yellow data blocks (32B) represent the data read by src1 per repeat (256B);
- Every 8 blue data blocks (32B) represent the data written by dst per repeat (256B);
- Number of iterations required to process all data: `repeat_times = 4096 * 4 / 256 = 64`.

#### Analysis Conclusion

- 1st repeat: src0 (bank_id=0, bg 0-7) and src1 (bank_id=1, bg 0-7): all datablocks in the repeat are located in the same bank group, and subsequent repeats follow the same pattern — **read-read conflict exists**.
- 1st repeat: src0/src1 reads (bank_id=0/1, bg 0-7) and dst writes (bank_id=1, bg 8-15): all datablocks in the repeat are located in different banks, and subsequent repeats follow the same pattern — **no read-write conflict**.

> **Comparison with scenario 2**: In scenario 2, src0/src1 are in the same bank (both bank_id and bank_group_id are the same); in scenario 3, they are in the same bank group (only bank_group_id is the same). Both trigger read-read conflicts with identical performance overhead (4195 vs 4195), indicating that read-read conflict detection is based on bank group rather than bank.

### Scenario 4: src0/dst Base Addresses in the Same Bank — No Read-Write Conflict (Hardware Optimization)

#### Address Decoding

| Operand | Base Address | 18-bit Binary (BANK DEPTH BG WIDTH) | bank_id | bank_group_id | Global Bank Number |
| --- | --- | --- | --- | --- | --- |
| src0 | `0x0` | `00 0000000 0000 00000` | 0 | 0 | 0 |
| src1 | `0x10100` | `01 0000000 1000 00000` | 1 | 8 | 24 |
| dst | `0x4000` | `00 0100000 0000 00000` | 0 | 0 | 0 |

#### UB Read/Write Illustration

![scenario4](./figures/scenario4.png)

- Every 8 green data blocks (32B) represent the data read by src0 per repeat (256B);
- Every 8 yellow data blocks (32B) represent the data read by src1 per repeat (256B);
- Every 8 blue data blocks (32B) represent the data written by dst per repeat (256B);
- Number of iterations required to process all data: `repeat_times = 4096 * 4 / 256 = 64`.

#### Analysis Conclusion

- 1st repeat: src0 (bank_id=0, bg 0-7) and src1 (bank_id=1, bg 8-15): all datablocks in the repeat are located in different bank groups, and subsequent repeats follow the same pattern — **no read-read conflict**.
- 1st repeat: src0 reads (bank_id=0, bg 0-7) and dst writes (bank_id=0, bg 0-7): all datablocks in the repeat are located in the same bank, theoretically causing a read-write conflict; however, the hardware automatically delays dst writes by 1 repeat, staggering the read and write operations, and subsequent repeats follow the same pattern — **no actual read-write conflict**.

**Theoretical execution timing** (unoptimized):

| | 1st repeat | 2nd repeat | 3rd repeat | ... |
| --- | --- | --- | --- | --- |
| src0 read | bank 0-7 (bg 0-7) | bank 8-15 (bg 8-15) | bank 0-7 (bg 0-7) | ... |
| src1 read | bank 24-31 (bg 8-15) | bank 16-23 (bg 0-7) | bank 24-31 (bg 8-15) | ... |
| dst write | bank 0-7 (bg 0-7) | bank 8-15 (bg 8-15) | bank 0-7 (bg 0-7) | ... |

Theoretical result: src0 and dst access the same bank every repeat — read-write conflict exists.

**Actual execution timing** (hardware optimized):

| | 1st repeat | 2nd repeat | 3rd repeat | ... |
