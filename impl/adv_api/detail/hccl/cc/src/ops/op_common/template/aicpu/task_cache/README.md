# MC2 resource-bound replay (experimental)

This implementation is for paired asc-devkit/HCOMM performance evaluation on 950.
It does not change public communication API signatures. The original tag-based path remains the fallback when the private backend capability is absent.

## Stage 1

`BindMc2ReplayDomain` binds to the immutable deserialized resource holder. A domain retains that holder, owns opaque backend handles and serializes mutable replay state. Resource fields are validated once at binding. A binary shape vector retains explicit fields and uses complete equality after hashing; a recent-entry comparison avoids hash work on repeated shapes. OpParam resource fields remain in the key because they are not owned by the resource holder.

`Mc2ReplayScope` spans one Server message. Ordinary collective repeats reuse the selected entry and spans; each iteration still validates the current buffers and performs its original wait/record sequence. Executor construction occurs only in the miss/bypass callback. A2A/A2AV initially keep exact array contents in the key.

An entry is published only after successful capture finalization. Replay errors are returned, never retried by normal execution after partial submission. Communicator cache clear waits for active domain use and destroys all matching handles. Domains are bounded to 256, each template budget to 10 MiB and the combined retained template budget to 64 MiB. Allocation during capture is transient and can exceed the retained budget before admission is decided.

## Backend contract

`HcommMc2ReplayV1` is an experimental, dynamically discovered C dispatcher: create, begin capture, end capture, replay, destroy. No binary templates cross backend versions. An asc-comm migration should implement the same ownership and publication contract using asc-comm queues and channel resources, rather than alias HCOMM objects.

## Validation boundary

Compilation alone does not prove output correctness, queue progress or performance. Before deployment, validate exact task semantics, clear/rebuild, alternating shapes, repeat 1/2/8/32, repeated Prepare, memory boundaries, registration changes and capacity fallback. Compare policy, execute and total sq_expand, including binding cost in init.

## Stage 2

Equal-count A2A repeats translate input/output bases while preserving per-peer displacements; ordinary and A2A repeats reuse message-level selection. A2AV remains exact-shape. Variable-array packing is deferred to the actual executor callback. HCOMM binds each WQE array directly to its token vector, fuses patch and submission, and removes destination zero fills that were immediately overwritten by full-length copies.

Token values still resolve on each replay because no registration-generation invalidation contract is available. Per-WQE publication barriers remain unchanged. Cross-WQE barrier removal and persistent registration-range bindings are deliberately not enabled without a visibility/lifetime proof. These are remaining design subitems, not completed optimizations.

## Stage 3: constrained replay groups

AicpuAllToAllVSoleMesh normalizes a common displacement translation into the buffer base. Counts and every relative displacement remain part of exact shape identity. Nonuniform per-peer displacement changes still require separate entries; no address-to-peer guessing is used. General per-peer provenance replay remains unimplemented.

The backend precomputes adjacent pure-SDMA groups on the same RTSQ and thread (at most 128 SQEs). It coalesces only when cached queue state already guarantees enough space, otherwise preserves the original submissions. It never crosses WQE, DB, Notify or queue boundaries. Task IDs refresh in original order, and profiling/exception records still report each original array. Additional private scratch copying trades against fewer tail updates; benefit must be measured.

Added UT cases cover SDMA grouping boundaries and the nonblocking capacity predicate. Target UT, signed-package functionality and performance must be validated for the exact paired revisions.

## First-version fallback on current master

This branch ports the initial three-stage optimization for an independent fallback comparison. It does not include later thread deduplication, pending-DB capacity reuse, contiguous capture storage, allocation recycling or DFX predecoding. Optional detailed timing probes are described below. Existing upstream features and basic debugMode=4 Server timing remain in place. The earlier roughly 30 us hit observation is not a performance guarantee for this port.

Validate the paired packages against the current optimization packages using the same toolkit, cards, algorithms, dtype, data sizes and debugMode=4: five collectives at 4 KiB, 10 rounds, each with one cold invocation, 29 warmups, 30 miss and 30 hit invocations. Keep cache clearing outside the timed interval. Compare each operation separately and alternate package order to expose environment drift. Also cover repeat >1, repeated Prepare, varying counts/displacements, zero-length peers and queue wraparound before adoption.

## Optional debugMode=5 diagnostics

`SetDebugMode(4)` retains the three basic stage means and Top3. `SetDebugMode(5)` additionally collects Server phase durations/calls and HCOMM SQE, copy, capture allocation, thread scan, replay token/patch/submit and DFX statistics. Temporary additions are marked `MC2_CACHE_DETAIL_DIAG`. Neither mode changes cache decisions or queue ordering.

Use paired diagnostic packages and hccl_api_test branch `perf/server-detail-timing-20261029`. Set `server.debug_mode` to 4 or 5 in `suite_config.json`; the suite command is unchanged. Detailed results are in `server_detail.csv`/`server_detail_samples.csv` and `server_backend.csv`/`server_backend_samples.csv`; `summary.csv` and `aicpu_benchmark.xlsx` keep the three-stage summary. Missing backend symbols are explicitly reported as unavailable.

Mode 5 accumulates unsigned 64-bit integers without per-event logging. After the message loop has exited, every 30 launches the summary is serialized into bounded 512-byte fragments, compatible with the existing parser. Mode 4 does not read detail clocks, accumulate detail counters, clear detail arrays, bind backend buffers or format detail records. Large serialization buffers live in a separate non-inline function. Policy/replay and the per-WQE loop use compile-time disabled probe specializations. Other probe sites retain a flag/null-pointer check; code-layout and branch effects are not claimed to be zero and require target A/B measurement.

For performance acceptance use mode 4. Compare its paired package with the previous fallback package under identical conditions; use mode 5 separately for attribution, since its timing includes diagnostic overhead. Host mock tests cover disabled-clock/counter behavior, 4/5 switching, batch resets and bounded log framing; these tests do not replace target compilation or NPU performance validation.
