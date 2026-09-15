# SSBuffer AIV/AIC Communication C API Sample

## Overview

This sample follows the DAV3510 KFC message state design without including or calling internal KFC APIs. The device code uses the Ascend C APIs `asc_init`, `asc_sync_intra_arrive`, `asc_sync_intra_wait`, and `asc_get_sub_block_id` to implement two SSBuffer ring queues from AIV to AIC in one `__mix__(1, 2)` task.

## Supported Products

| Product | NPU Architecture |
| --- | --- |
| Ascend 950PR/Ascend 950DT | dav-3510 |

Only NPU and simulation modes are supported. The sample exclusively uses the first 2 KB of SSBuffer. This space must not overlap with internal KFC paths, the Matmul high-level API, or other SSBuffer protocols. The public KFC resource grouping API uses `KfcWorkspace` in GM and does not provide the SSBuffer base address.

## Implementation

- Each AIV uses a ring queue with a depth of 8 and a message size of 128 bytes. Each AIV sends 12 messages to verify queue wrap-around.
- AIC clears all message headers and then uses mode 4 inter-core synchronization to notify AIV0 and AIV1 that they can start sending.
- After writing a message, AIV sends a message-ready notification. AIC clears the message header and returns an ACK after consuming the message. Starting from the ninth message, AIV must receive an ACK before reusing message space at the beginning of the queue.
- Startup, message-ready, and ACK notifications use different `sync_id` values. On AIC, IDs 0 to 15 map to AIV0, and IDs 16 to 31 map to AIV1.
- Fields shared across cores use `volatile`. The sample does not use a custom compiler barrier. `asc_sync_intra_arrive` and `asc_sync_intra_wait` provide inter-core synchronization. `asc_sync_pipe` and `asc_sync_data_barrier` provide intra-core synchronization and cannot replace the synchronization required for this SSBuffer message exchange.
- The C API does not provide an interface named `GetSsbufBaseAddr`. On a DAV3510 NPU, address zero in the `__ssbuf__` address space is the SSBuffer base address, so the sample obtains it using `reinterpret_cast<__ssbuf__ SsbufWorkspace*>(0)`. This behavior comes from the language address space semantics and is not a KFC interface.
- The current compiler does not support directly declaring an array with the `__ssbuf__` qualifier. The sample declares the two-dimensional message queue as a member of `SsbufWorkspace` and accesses it through an `__ssbuf__ SsbufWorkspace*`.
- The host code launches one Mix block, reads the 24 results written by AIC to GM, and validates each result.

Queue wrap-around occurs when a message sequence exceeds the queue depth and a new message reuses storage at the beginning of the queue. With a depth of 8, sequences 0 to 7 use positions 0 to 7, while sequences 8 to 11 reuse positions 0 to 3. Waiting for an ACK before reuse ensures that old messages have been processed and prevents unconsumed data from being overwritten.

## Build and Run

```bash
source ${install_path}/cann/set_env.sh
mkdir -p build && cd build
cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DCMAKE_ASC_RUN_MODE=npu ..
make -j
./demo
```

Expected output:

```text
test pass!
```
