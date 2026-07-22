# HCOMM Exception Fault Cases

This directory contains two AIV fault kernels for validating exception callback channel dump.

## Kernels

- `HcommUrmaTimeoutFaultKernel`: calls `Hcomm::WriteNbi` with `len > 256MB` and then `Drain`.
- `HcommCqeErrorFaultKernel`: calls `Hcomm::WriteNbi` with a valid remote GM address and an invalid local GM address,
  then `Drain`.

Both kernels take a device pointer to `CommContext`.

## Host Flow

1. Build or acquire a valid URMA `ChannelHandle`.
2. Fill `CommContext` on host:

```cpp
CommContext hostCtx = {};
hostCtx.channelHandle = channel;
hostCtx.localBufferAddr = reinterpret_cast<uint64_t>(localBuf);
hostCtx.remoteBufferAddr = reinterpret_cast<uint64_t>(remoteBuf);
hostCtx.rankId = rank;

// URMA timeout case.
hostCtx.len = HCOMM_URMA_TIMEOUT_TEST_LEN;

// CQE error case.
hostCtx.len = HCOMM_CQE_ERROR_TEST_LEN;
hostCtx.invalidBufferAddr = HCOMM_INVALID_BUFFER_ADDR;
```

3. Allocate and copy `CommContext` to device.
4. Register the Runtime exception callback and, when supported by the Runtime, bind the device `CommContext` as
   exception args for the fault kernel binary:

```cpp
aclrtSetExceptionInfoCallback(CustomCallback);
aclrtBinarySetExceptionCallback(binHandle, CustomCallback, devContext);
```

The callback is intentionally lightweight. It only records Runtime exception fields and the device args pointer reported
by `aclrtGetArgsFromExceptionInfo`, then sets a pending flag. It does not call `aclrtMemcpy`, does not synchronize a
stream, and does not call the channel dump interface.

5. Launch one fault kernel:

```cpp
HcommUrmaTimeoutFaultKernel<<<1, nullptr, stream>>>(devContext);
// or
HcommCqeErrorFaultKernel<<<1, nullptr, stream>>>(devContext);
```

6. Synchronize the stream and report at the host safe point. The callback path is preferred. If the Runtime reports the
stream error without entering the callback, the runner falls back to the cached current-rank `ChannelHandle` and still
calls `HcommChannelInfoDump` for diagnosis.

## Build

```bash
cd <asc-devkit-repo>/examples/debug
mkdir -p build
cd build
cmake .. -DCMAKE_ASC_ARCHITECTURES=dav-2201
cmake --build . -j
```

The build produces:

- `hcomm_exception_fault_kernels`
- `hcomm_channel_dump`
- `exception_callback_demo`
- `hcomm_exception_fault_runner`

## One-Command Run

The runner prepares a legal `ChannelHandle` by initializing an HCCL communicator, registering device memory with
`HcclCommMemReg`, creating an AIV UBC_CTP channel with `HcclChannelAcquire`, and getting the peer buffer with
`HcclChannelGetRemoteMems`.

```bash
cd <asc-devkit-repo>/examples/debug
bash run_hcomm_exception_fault.sh urma
# or
bash run_hcomm_exception_fault.sh cqe
```

Optional environment variables:

- `RANK_SIZE=2`: number of ranks/devices used by the runner.
- `CMAKE_ASC_ARCHITECTURES=dav-2201`: target 950 NPU architecture.
- `CMAKE_ASC_COMPILER=/path/to/tools/bisheng_compiler/bin/bisheng`: optional Ascend C compiler override.
- `HCOMM_CHANNEL_DUMP_SO=/path/to/libhcomm_channel_dump.so`: shim dump library path.
- `HCOMM_REAL_DUMP_SO=/path/to/libhccl.so`: real library that exports `HcommChannelInfoDump`.
- `HCOMM_DFX_QUEUE_READ_KERNEL_PATH=/path/to/hccl_dfx_queue_read_kernel_910_95.o`: optional HCCL DFX queue read helper
  kernel path. If unset, HCCL looks for it under `$ASCEND_HOME_PATH/lib64`.

## Run

1. Make sure the HCCL dump shared library is available on the device host and set the path:

```bash
export HCOMM_CHANNEL_DUMP_SO=/path/to/libhcomm_channel_dump.so
```

2. Prepare a valid communication `ChannelHandle` and a real pair of device buffers.
3. Copy `CommContext` to device memory, register the Runtime callback, and bind the exception args when the Runtime
   exposes the binary exception callback API:

```cpp
aclrtSetExceptionInfoCallback(CustomCallback);
aclrtBinarySetExceptionCallback(binHandle, CustomCallback, devContext);
```

4. Launch one fault kernel:

```cpp
HcommUrmaTimeoutFaultKernel<<<1, nullptr, stream>>>(devContext);
// or
HcommCqeErrorFaultKernel<<<1, nullptr, stream>>>(devContext);
```

5. Call `aclrtSynchronizeStream(stream)`, then call the report path and check the logs.

## Expected Logs

- URMA timeout case: the preferred path is callback-triggered report. On Runtime builds that only return the stream
  error, the runner prints `callbackCount=0` and dumps the cached current-rank channel. `HcommChannelInfoDump` prints
  channel handle, local/remote buffer addresses, SQ/CQ context, WQE fields, and CQ bytes.
- CQE error case: same reporting flow as above. `HcommChannelInfoDump` prints the channel dump together with SQ/CQ/WQE
  and CQE raw bytes. Use `CqContext`, `CqHead`, `CqTail`, `CqTailEntries`, and `scqVa` as log filters.
