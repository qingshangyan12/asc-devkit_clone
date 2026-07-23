# HCOMM Exception Fault Cases

This directory contains two AIV fault kernels for validating exception callback channel dump.

## Kernels

- `HcommUrmaTimeoutFaultKernel`: calls `Hcomm::WriteNbi` with `len > 256MB` and then `Drain`.
- `HcommCqeSeedDumpKernel`: seeds recognizable non-zero WQE/CQE records into the channel SQ/CQ ring for dump validation.
- `HcommCqeErrorFaultKernel`: triggers a Runtime exception after the seed kernel has completed on the same stream.

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

5. Launch the target fault path:

```cpp
HcommUrmaTimeoutFaultKernel<<<1, nullptr, stream>>>(devContext);
// or
HcommCqeSeedDumpKernel<<<1, nullptr, stream>>>(devContext);
HcommCqeErrorFaultKernel<<<1, nullptr, stream>>>(devContext);
```

6. Synchronize the stream and report at the host safe point. The callback path records the exception and the report path
   calls the HCOMM dump buffer API for diagnosis.

## Verified Callback Pattern

The validated Runtime behavior on 950 is:

- `aclrtSetExceptionInfoCallback` registers the global callback entry.
- `aclrtBinarySetExceptionCallback(binHandle, CustomCallback, devContext)` binds the fault kernel binary to the device
  `CommContext`.
- In the callback, `aclrtGetArgsFromExceptionInfo` returns a pointer-sized argument (`devArgsLen == 8`). The argument is
  not the full `CommContext`; it is a device-side pointer to `CommContext`.
- The callback must stay lightweight. It records Runtime fields and `devArgs`, then sets a pending flag.
- After `aclrtSynchronizeStream` returns, the host report path copies the pointer-sized argument first, then copies the
  real `CommContext`, then calls `HcommChannelGetDumpInfoSize` and `HcommChannelDumpInfo`.
- HCCL returns a binary raw dump buffer. The demo parses that buffer in asc-devkit and prints WQE/CQE fields compatible
  with the former device-side `AscendC::printf` diagnostics.

Recommended shape:

```cpp
struct ExceptionSummary {
    uint32_t taskId;
    uint32_t streamId;
    uint32_t threadId;
    uint32_t deviceId;
    uint32_t errorCode;
    void *devArgs;
    uint32_t devArgsLen;
};

void CustomCallback(aclrtExceptionInfo *info)
{
    ExceptionSummary summary = {};
    summary.taskId = aclrtGetTaskIdFromExceptionInfo(info);
    summary.streamId = aclrtGetStreamIdFromExceptionInfo(info);
    summary.threadId = aclrtGetThreadIdFromExceptionInfo(info);
    summary.deviceId = aclrtGetDeviceIdFromExceptionInfo(info);
    summary.errorCode = aclrtGetErrorCodeFromExceptionInfo(info);
    (void)aclrtGetArgsFromExceptionInfo(info, &summary.devArgs, &summary.devArgsLen);

    SaveSummaryAndSetPending(summary);
}

void ReportPendingException()
{
    ExceptionSummary summary = TakePendingSummary();

    uint64_t devCtxAddr = 0;
    if (summary.devArgsLen == sizeof(uint64_t)) {
        aclrtMemcpy(&devCtxAddr, sizeof(devCtxAddr), summary.devArgs, sizeof(devCtxAddr),
            ACL_MEMCPY_DEVICE_TO_HOST);
    } else {
        devCtxAddr = reinterpret_cast<uint64_t>(summary.devArgs);
    }

    CommContext ctx = {};
    aclrtMemcpy(&ctx, sizeof(ctx), reinterpret_cast<void *>(devCtxAddr), sizeof(ctx), ACL_MEMCPY_DEVICE_TO_HOST);

    ChannelHandle channel = static_cast<ChannelHandle>(ctx.channelHandle);
    uint32_t len = 0;
    HcommChannelGetDumpInfoSize(1, &channel, &len);
    std::vector<uint8_t> buffer(len, 0);
    HcommChannelDumpInfo(1, &channel, buffer.data(), len);
    ParseAndPrintHcommChannelDumpInfo(buffer.data(), len);
}
```

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
- `HCOMM_REAL_DUMP_SO=/path/to/libhccl.so`: real library that exports `HcommChannelGetDumpInfoSize` and
  `HcommChannelDumpInfo`.
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

4. Launch the target fault path:

```cpp
HcommUrmaTimeoutFaultKernel<<<1, nullptr, stream>>>(devContext);
// or
HcommCqeSeedDumpKernel<<<1, nullptr, stream>>>(devContext);
HcommCqeErrorFaultKernel<<<1, nullptr, stream>>>(devContext);
```

5. Call `aclrtSynchronizeStream(stream)`, then call the report path and check the logs.

## Expected Logs

- URMA timeout case: callback-triggered report calls `HcommChannelDumpInfo`, which returns raw channel/SQ/CQ/WQE/CQE
  bytes; asc-devkit parses and prints the protocol fields.
- CQE error case: the runner first launches `HcommCqeSeedDumpKernel` and then launches
  `HcommCqeErrorFaultKernel` on the same stream. The dump should therefore contain non-zero WQE/CQE fields seeded by
  the first kernel. Use `CqContext`, `Hcomm URMA CQE`, `status`, `substatus`, and `entryIdx` as log filters.

## Validation Checklist

Callback flow is considered valid when the log contains all of the following:

```text
CustomCallback entered
runtime exception count=...
exception dev args point to CommContext device addr=...
exception ctx rank=... channel=0x...
HcommChannelDumpInfo ret=0
[hcomm_fault_runner][parsed] Channel[0] handle=0x...
```

Channel dump content is considered basically valid when these values match the runner setup:

- `exception ctx channel` equals the channel handle printed during channel creation.
- `exception ctx local` equals the current-rank local buffer printed during channel creation.
- `exception ctx remote` equals the peer buffer printed during channel creation.
- `[parsed] SqContext sqVa/depth/entrySize/head/tail` is printed.
- `[parsed] Hcomm URMA WQE` is printed and contains protocol fields such as `sqe_bb_idx`, `owner`, `opcode`, `tp_id`,
  `sge_num`, plus SGE/notify/AMO fields when present in the captured raw bytes.
- `[parsed] CqContext cqVa/depth/entrySize/head/tail` is printed.
- `[parsed] Hcomm URMA CQE` is printed and contains `owner`, `opcode`, `status`, `substatus`, `entryIdx`, `byteCnt`,
  `rmtIdx`, and `tpn`.

Useful filters:

```bash
grep -E "CustomCallback entered|runtime exception|exception dev args point|exception ctx|HcommChannelDumpInfo ret=0" urma_950.log
grep -E "rank=0 channel=|exception ctx|\\[parsed\\] SqContext|\\[parsed\\] Hcomm URMA WQE|\\[parsed\\] CqContext|\\[parsed\\] Hcomm URMA CQE" urma_950.log
grep -E "rank=0 channel=|exception ctx|\\[parsed\\] SqContext|\\[parsed\\] Hcomm URMA WQE|\\[parsed\\] CqContext|\\[parsed\\] Hcomm URMA CQE" cqe_950.log
```
