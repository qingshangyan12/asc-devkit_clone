# SSBuffer Inter-Core Communication Sample

## Overview

This sample demonstrates how to use SSBuffer inter-core communication on Ascend 950PR/Ascend 950DT. SSBuffer is an on-chip storage unit for small data communication between AIC and AIV cores. It is suitable for passing control information, addresses, or a small amount of scalar data within the same task. Massive data transfer should use the corresponding data paths instead of treating SSBuffer as a general-purpose data cache.

## Supported Products and CANN Versions

| Product | CANN Version |
|------|-------------|
| Ascend 950PR/Ascend 950DT | >= CANN 9.1.0 |

## Directory Structure

```
├── ssbuffer
│   ├── scripts
│   │   ├── gen_data.py              // Script to generate golden data
│   │   └── verify_result.py         // Script to verify output against golden data
│   ├── CMakeLists.txt               // CMake build file
│   ├── data_utils.h                 // Host data read/write utilities
│   ├── ssbuffer.h                   // SSBuffer message definition and constants
│   ├── ssbuffer.asc                 // Kernel implementation and host invocation
│   └── README.md                    // Sample description
```

## Sample Description

<table border="1" style="text-align: center;">
  <tr>
    <td>SCENARIO_NUM</td>
    <td>Scenario</td>
    <td>Verification</td>
  </tr>
  <tr>
    <td>0</td>
    <td>Base address verification: launch the kernel on a single AIV block and verify that GetSsbufBaseAddr can be called on the NPU</td>
    <td>Check the GetSsbufBaseAddr output in the log</td>
  </tr>
  <tr>
    <td>1</td>
    <td>AIV-to-AIC message verification: in a separated mode with AIC:AIV = 1:2, AIV0 writes a 32-byte message to SSBuffer and AIC reads it after inter-core synchronization</td>
    <td>Compare output.bin with golden.bin</td>
  </tr>
</table>

### SCENARIO_NUM=0 (Base Address Verification)

This scenario only calls `GetSsbufBaseAddr` to obtain the base address and does not read or write messages in SSBuffer. After launching the kernel on a single AIV block and synchronizing the stream, the log should contain the output of `GetSsbufBaseAddr`. On the NPU the special base address may be shown as `0` or `nil`; it must not be dereferenced as an ordinary null pointer.

### SCENARIO_NUM=1 (AIV-to-AIC Message Verification)

This scenario verifies one-way communication with AIC:AIV = 1:2. Only the device-side implementation is provided; the host allocates 8 `uint64_t` elements for `output`, launches the kernel on a single block, synchronizes the stream, copies `output` back, and validates it.

- Message definition: a 32-byte-aligned `SsbufferMessage` (4 x `uint64_t`, 32 bytes in total).
- Message layout: the message starts at byte offset 256 (`VERIFY_MESSAGE_OFFSET`); it must be 32-byte aligned and must not cross the end of SSBuffer.
- Communication flow:
  1. AIV0 writes the complete 32-byte message into SSBuffer and echoes the message to GM (`output[4..7]`) so the host can validate the AIV-side write;
  2. AIV0 first establishes the scalar-to-vector in-core ordering (`SetFlag<HardEvent::S_V>`/`WaitFlag<HardEvent::S_V>`), then publishes the AIV-to-AIC ready flag via `CrossCoreSetFlag<2, PIPE_V>` (mode 2);
  3. AIC calls `CrossCoreWaitFlag<2, PIPE_M>` to wait until both AIVs in the same logical AI Core have published, establishes the cube-to-scalar ordering before the read (`SetFlag<HardEvent::M_S>`/`WaitFlag<HardEvent::M_S>`), then reads the message from SSBuffer into `output[0..3]`.

> **Note:** `AIV_TO_AIC_FLAG` (flagId = 14) must avoid the flag IDs occupied by Matmul advanced APIs or SyncAll in the current operator.
> **Note:** In the 1:2 scenario, only AIV0 produces the message, but AIV1 must still participate in the mode-2 synchronization (both AIVs must call `CrossCoreSetFlag`, otherwise AIC keeps waiting forever).

## Implementation

### 1. Get the Base Address

```cpp
__ssbuf__ void* ssbuf = AscendC::GetSsbufBaseAddr();
AscendC::printf("GetSsbufBaseAddr: %p\n", ssbuf);
```

### 2. AIV Writes and Publishes

```cpp
if ASCEND_IS_AIV {
    if (AscendC::GetSubBlockIdx() == 0) {
        message->value[0] = EXPECTED_VALUE[0];
        message->value[1] = EXPECTED_VALUE[1];
        message->value[2] = EXPECTED_VALUE[2];
        message->value[3] = EXPECTED_VALUE[3];
        output[4] = message->value[0];
        output[5] = message->value[1];
        output[6] = message->value[2];
        output[7] = message->value[3];
    }
    AscendC::SetFlag<AscendC::HardEvent::S_V>(0);
    AscendC::WaitFlag<AscendC::HardEvent::S_V>(0);
    AscendC::CrossCoreSetFlag<2, PIPE_V>(AIV_TO_AIC_FLAG);
}
```

### 3. AIC Waits and Reads

```cpp
if ASCEND_IS_AIC {
    AscendC::CrossCoreWaitFlag<2, PIPE_M>(AIV_TO_AIC_FLAG);
    AscendC::SetFlag<AscendC::HardEvent::M_S>(0);
    AscendC::WaitFlag<AscendC::HardEvent::M_S>(0);
    output[0] = message->value[0];
    output[1] = message->value[1];
    output[2] = message->value[2];
    output[3] = message->value[3];
}
```

## Notes

1. SSBuffer only provides shared storage, not data-readiness guarantees. Producers and consumers must use the inter-core synchronization interfaces (`CrossCoreSetFlag`/`CrossCoreWaitFlag`) to establish ordering; they must be paired.
2. SSBuffer initially contains dirty data; reads are not guaranteed to be zero. Each communication must write the complete message; consumers must not rely on historical contents.
3. Only 32-byte-aligned reads/writes are supported. The start address and access granularity of a communication data block must be 32-byte aligned; do not perform member reads/writes smaller than 32 bytes on SSBuffer.
4. In non-MIX mode, AIC, AIV0 and AIV1 each have their own 1 KB space; in MIX mode (AIC:AIV = 1:2) they share the whole 3 KB space. The data layout must be identical on both communication sides and must not cross the end of SSBuffer.
5. The current task must enable the SSBuffer capability (compile option `ENABLE_CV_COMM_VIA_SSBUF=true`) before accessing SSBuffer.
6. SSBuffer cannot be accessed when AIC and AIV are started with different tasks.

## Build and Run

Execute the following commands in the sample root directory.

- Configure environment variables  
  Configure the environment variables according to the [installation guide](../../../../../docs/zh/quick_start.md#prepare&install) of the CANN package.
  ```bash
  source ${install_path}/cann/set_env.sh
  ```

  > **Note:** `${install_path}` is the CANN package installation directory, which defaults to `/usr/local/Ascend`.

- Run the sample

  ```bash
  SCENARIO_NUM=1  # set the scenario number (0 or 1)
  mkdir -p build && cd build;
  cmake .. -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DSCENARIO_NUM=$SCENARIO_NUM;make -j;
  python3 ../scripts/gen_data.py -scenarioNum $SCENARIO_NUM
  ./demo
  python3 ../scripts/verify_result.py ./output/output.bin ./output/golden.bin
  ```

  Scenario 0 (base address verification) produces no output data file; check the log for the printed output of `GetSsbufBaseAddr`.

- Build options

  | Option | Values | Description |
  |------|--------|------|
  | `CMAKE_ASC_RUN_MODE` | `npu` (default), `cpu`, `sim` | Run mode: NPU, CPU debug, NPU simulation |
  | `CMAKE_ASC_ARCHITECTURES` | `dav-3510` (default) | NPU architecture: Ascend 950PR/Ascend 950DT |
  | `SCENARIO_NUM` | `1` (default), `0` | Scenario: 1 (AIV-to-AIC message), 0 (base address verification) |

- Result

  For scenario 1, the output below shows that the SSBuffer communication is correct.
  ```bash
  test pass!
  ```