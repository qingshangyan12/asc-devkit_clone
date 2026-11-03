# MC2 asccomm command replay — stage one (WIP)

This branch implements native primitive-command capture/replay, not SQE/WQE binary-template replay. On a hit, the collective executor is skipped; asccomm still rebuilds tasks, resolves current registration tokens and advances queue/task state through its existing primitives.

The private `asccomm_command_replay_v1` ABI has create/begin/end/replay/abort/destroy/query actions. Callers serialize all handle access and keep resource and buffer lifetimes valid. Batch descriptor arrays are deep-copied. Recording allocation/budget failures invalidate the template while allowing the original communication to finish; replay stops on the first error and must never fall back by reexecuting a partially submitted collective. An empty/control-only capture is not publishable.

The asc-devkit companion pins the resource holder, checks immutable channel support once per domain and compares exact dynamic shape/OpParam resource bytes and input/output addresses, and clears native entries using the existing MC2 clear kernel. Different holders or addresses do not share commands. The holder is immutable after deserialization; replacing resource metadata requires a new holder. The asccomm key path does not build the legacy HCOMM digest/tag. Zero-copy, symmetric-memory, unsupported protocol/engine and CPU reduction paths bypass. Resource release/recreation requires cache invalidation; this patch does not add HCOMM lifetime/generation APIs.

HCOMM remains at PR #6402 without optimization modifications. Original asc-devkit #6843 / asc-comm #317 branches are untouched. Use paired packages from the new WIP PRs, including the Device runtime asccomm library. Never mix a new Host library with an old Device library.

For a real uncached baseline, `HCCL_OP_EXPANSION_MODE=AICPU_CacheDisable` is serialized by the companion Host into the existing `OpParam.aicpuCacheEnable`; ordinary AI_CPU enables capture. The framework's `bypass` / `blocked_clear` policies validate the corresponding states. Output comparison must remain enabled.

No NPU correctness or performance acceptance is claimed by compilation or host-stub tests. Required board gates include five collectives, repeat 1/2/8/32, repeated Prepare, changing addresses/layouts, clear→miss→hit, queue wrap/pressure and partial-submit failures. Use debugMode=4 for the three-stage timing; stage-one mode-5 backend detail is not implemented.

Binary templates, dynamic relocation, prebound token patch descriptors and merged WQE patch/submission are a later phase. Persistent token caching, independent per-peer relocation and cross-WQE barrier removal remain out of scope.
