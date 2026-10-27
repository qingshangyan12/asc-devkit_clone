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
