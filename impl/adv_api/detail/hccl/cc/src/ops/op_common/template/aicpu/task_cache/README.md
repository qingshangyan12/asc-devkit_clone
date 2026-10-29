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

Only compilation is authorized for this change. Compilation does not prove output correctness, queue progress or performance. Before deployment, validate exact task semantics, clear/rebuild, alternating shapes, repeat 1/2/8/32, repeated Prepare, memory boundaries, registration changes and capacity fallback. Compare policy, execute and total sq_expand, including binding cost in init.

## Stage 2

Equal-count A2A repeats translate input/output bases while preserving per-peer displacements; ordinary and A2A repeats reuse message-level selection. A2AV remains exact-shape. Variable-array packing is deferred to the actual executor callback. HCOMM binds each WQE array directly to its token vector, fuses patch and submission, and removes destination zero fills that were immediately overwritten by full-length copies.

Token values still resolve on each replay because no registration-generation invalidation contract is available. Per-WQE publication barriers remain unchanged. Cross-WQE barrier removal and persistent registration-range bindings are deliberately not enabled without a visibility/lifetime proof. These are remaining design subitems, not completed optimizations.

## Stage 3: constrained replay groups

AicpuAllToAllVSoleMesh normalizes a common displacement translation into the buffer base. Counts and every relative displacement remain part of exact shape identity. Nonuniform per-peer displacement changes still require separate entries; no address-to-peer guessing is used. General per-peer provenance replay remains unimplemented.

The backend precomputes adjacent pure-SDMA groups on the same RTSQ and thread (at most 128 SQEs). It coalesces only when cached queue state already guarantees enough space, otherwise preserves the original submissions. It never crosses WQE, DB, Notify or queue boundaries. Task IDs refresh in original order, and profiling/exception records still report each original array. Additional private scratch copying trades against fewer tail updates; benefit must be measured.

Added UT cases cover SDMA grouping boundaries and the nonblocking capacity predicate. They are not executed in this compile-only task. Signed-package functionality and all performance claims remain pending.
