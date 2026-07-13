# 3510 Special Buffer Dump Sample

## Overview

This sample demonstrates how to use `DumpTensor` to print data from L1 Buffer, Bias Table Buffer, and Fixpipe Buffer on Ascend 950PR/Ascend 950DT. Fixed inputs are used to explain the data mask applied to the quantization parameter area of Fixpipe Buffer.

## Supported Products and CANN Software Versions

| Product | CANN Software Version | HDK Version |
|---------|-----------------------|-------------|
| Ascend 950PR/Ascend 950DT | >= CANN 9.1.0 | >= 25.7.0 |

The `DumpTensor` and `asc_dump` API documents in this repository explicitly require HDK 25.7.0 or later for printing L1 Buffer data on Ascend 950PR/Ascend 950DT. Because this sample includes L1 Buffer printing, that requirement is the minimum version for the complete sample. Bias Table Buffer, Fixpipe Buffer, and L1 Buffer printing use the dav-3510 debug-bus path, and the API documents do not specify a separate higher HDK minimum for the other two buffers.

## Directory Structure

```text
├── special_buffer_dump
│   ├── scripts
│   │   └── verify_dump.py       // Parses and verifies the dump log
│   ├── CMakeLists.txt           // Build project file
│   ├── special_buffer_dump.asc  // Ascend C sample implementation and invocation code
│   ├── README.md                // Chinese sample document
│   └── README_en.md             // English sample document
```

## Sample Description

- Functions:

  1. Moves `int32_t` input from Global Memory to L1 Buffer and prints it, then moves the data to Bias Table Buffer and prints it again. Both dumps match the input.
  2. Moves `uint64_t` quantization parameters from Global Memory to L1 Buffer and prints them, then moves the data to the Fixpipe Buffer quantization area and prints it again. The L1 Buffer dump preserves the original input, while the Fixpipe Buffer dump shows the fields retained by the hardware.
  3. Uses `scripts/verify_dump.py` to parse the log, distinguish the four data sets by their `desc` values, and verify them automatically.

- Dump identifiers:

  | desc | Position | Description |
  |------|----------|-------------|
  | 101 | L1 Buffer | Original `int32_t` data before it is moved to Bias Table Buffer |
  | 201 | Bias Table Buffer | `int32_t` data in Bias Table Buffer |
  | 102 | L1 Buffer | Original `uint64_t` data before it is moved to Fixpipe Buffer |
  | 301 | Fixpipe Buffer | `uint64_t` data in the Fixpipe Buffer quantization area |

## Fixpipe Buffer Data Mask

On dav-3510, after quantization parameters are written from L1 Buffer to Fixpipe Buffer, the data visible in the physical buffer follows this rule:

```text
fixpipe_value = l1_value & 0x00007fe0ffffe0ff
```

The mask retains bits `[7:0]`, `[31:13]`, and `[46:37]`, and clears bits `[12:8]`, `[36:32]`, and `[63:47]`. Therefore, the dump with `desc=301` can differ from the original L1 data with `desc=102`. This is caused by the hardware bit-field write rule on the L1-to-Fixpipe Buffer path; `DumpTensor` does not modify the data.

For the dav-3510 quantization parameter API, the documented valid fields are M1 in bits `[31:13]`, Offset in bits `[45:37]`, and Sign in bit `46`. Although bits `[7:0]` are observable in a physical Fixpipe Buffer dump, they are not valid quantization parameter fields and must not be used for quantization configuration.

The logical Fixpipe Buffer address also contains an area selector: high 16 bits equal to `0` select the quantization parameter area, high 16 bits equal to `1` select the ReLU parameter area, and the low 16 bits specify the offset in that area. This sample allocates the quantization parameter area from address `0` with `LocalMemAllocator<Hardware::FIXBUF>` and demonstrates only its data mask.

## Build and Run

Run the following steps in the sample root directory.

- Configure environment variables

  Configure environment variables according to the CANN development kit [installation method](../../../../../docs/quick_start.md#prepare&install).

  ```bash
  source ${install_path}/cann/set_env.sh
  ```

  > **Note:** `${install_path}` is the CANN installation directory. If no directory is specified, the default installation path is `/usr/local/Ascend`.

- Build and run the sample

  ```bash
  mkdir -p build && cd build
  cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 .. && make -j
  ./demo 2>&1 | tee run.log
  python3 ../scripts/verify_dump.py run.log
  ```

- Result

  The log contains four `DumpTensor` outputs. When the Fixpipe Buffer input contains invalid bits, `desc=102` preserves the original value and `desc=301` retains only the fields selected by the mask. A successful verification prints:

  ```text
  Bias L1 raw input check: PASS
  Bias Table raw data check: PASS
  Fixpipe L1 raw input check: PASS
  Fixpipe quant visible-mask check: PASS
  test pass!
  ```
