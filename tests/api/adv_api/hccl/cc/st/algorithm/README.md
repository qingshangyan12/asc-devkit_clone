# HCCL algorithm ST

This directory is the single entry point for HCCL algorithm simulation tests.

## Test layers

- `utils/src/sim_world` and `utils/src/hccl_proxy` provide the shared host-side
  simulation model. AICPU and MC2 CCU tests must reuse these targets instead of
  copying the stubs into a collective-specific directory.
- The AICPU target uses `hccl_verifier` to validate the generated task DAG,
  memory accesses, and collective semantics. Existing AllGather and
  ReduceScatter cases belong to this layer.
- `testcase/mc2_ccu/host` validates the MC2 CCU host control plane, including
  tiling parsing, algorithm selection, resource contexts, KFC server arguments,
  and CCU kernel registration. These cases do not execute CCU instructions.
- `testcase/mc2_ccu/device` directly compiles the current product KFC server and
  collective kernel. Its focused kernel cases validate logical IR, TaskArg,
  synchronization and data semantics; its KFC cases validate queue, message,
  opId dispatch, completion and finalize behavior. Both groups belong to the
  MC2 device execution path and share one test executable.

## Adding an MC2 CCU collective

1. Add Host control-plane cases under `testcase/mc2_ccu/host` and KFC/kernel
   execution cases under `testcase/mc2_ccu/device`.
2. Link the existing `hccl-model-proxy` target. Add a file under
   `utils/src/mc2_ccu` only when the MC2 control plane needs behavior that does
   not belong in the shared simulation model.
3. Verify the collective's tiling-to-`OpParam` mapping, selector result,
   resource isolation and reuse, KFC arguments, and kernel registration.
4. Add TaskArg or data-semantics checks only when the logical simulator models
   the required operation and synchronization behavior. Keep hardware execution
   in the appropriate device-level validation suite.

The current MC2 CCU product coverage in this tree is limited to
`CcuAlltoallVMesh1D`. The layout is intentionally collective-neutral so future
algorithms can be added without another copy of `sim_world` or `hccl_proxy`.

MC2 CCU device tests are host-side logical validation. They do not cover the
real CCU Translator, physical resource allocation, instruction loading, driver,
or hardware execution.
