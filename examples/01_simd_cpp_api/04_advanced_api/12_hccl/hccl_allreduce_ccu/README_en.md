# HCCL AllReduce (CCU) Kernel Direct-Launch Sample
## Overview
This sample implements the HCCL AllReduce task launch flow based on the CCU server type using the Ascend C programming language. On the kernel side it calls `InitV2`, `SetCcTilingV2`, `AllReduce` and `Commit` of `AscendC::Hccl<HcclServerType::HCCL_SERVER_TYPE_CCU>`, where `AllReduce` finally forwards to `CommonPrepareImpl` of the CCU implementation. That function validates the communication data size, allocates a handle and fills the CCU message body. The `<<<>>>` kernel launch operator is used to run and verify the kernel function on the NPU side, providing a corresponding end-to-end implementation.

## Supported Products and CANN Versions

| Product | CANN Version |
|---------|--------------|
| Ascend 950PR/Ascend 950DT | >= CANN 9.1.0 |

> **Note:** `HcclServerType::HCCL_SERVER_TYPE_CCU` is only available on the `dav-3510` architecture, so this sample does not support other products.

## Environment Requirements

| Item | Requirement |
|------|-------------|
| CANN package | Must contain the compiler (`ccec_compiler`) and the runtime headers (`include/acl/acl.h`). Source its `set_env.sh` before building. |
| Host architecture | Both x86_64 and aarch64 are supported. CMake selects the matching `x86_64-linux`/`aarch64-linux` subdirectory of the CANN package based on `CMAKE_SYSTEM_PROCESSOR`. |
| CMake | >= 3.16 |
| NPU device | Only `npu` mode requires a real Ascend 950PR/950DT device. `cpu` mode runs on the host and needs no NPU. |

> **Note:** After switching environments or host architectures, remove the whole `build` directory before running cmake again. The cmake cache stores absolute paths from the previous environment, and removing only `CMakeCache.txt` is not enough.

## Directory Structure
```
├── hccl_allreduce_ccu
│   ├── CMakeLists.txt              // Build project file
│   ├── hccl_allreduce_ccu.asc      // Ascend C operator implementation & invocation sample
│   └── README_en.md                // Sample introduction
```

## Description

- Operator implementation:
  - Kernel implementation
    1. `InitV2` binds the communication context and the init tiling. `Mc2InitTilingInner::version` is set to `INIT_TILING_VERSION`, so the context is parsed with the `HcclCombineOpParam` layout.
    2. `SetCcTilingV2` registers the cc tiling of AllReduce. Its argument is the offset of the cc tiling relative to the address passed to `InitV2`, therefore `Mc2InitTilingInner` must be the first member of the tiling structure.
    3. `AllReduce` launches one AllReduce task and returns a handleId. The communication data size is `count * sizeof(dataType)` and must fall within `(0, 256MB]`, and the handleId must be less than `HCCL_MAX_HANDLE_ID`; otherwise the validation in `CommonPrepareImpl` fails.
    4. `Commit` notifies the CCU to execute the launched task.
    5. The handleId, rankId and rankDim are written back to Global Memory for host-side verification.

  - Invocation implementation
    The kernel function is called with the `<<<>>>` kernel launch operator, and the task launch runs on a single core (block 0). The host side builds the communication context by hand, including the workspace, rankId/rankNum, `windowsOut[0]` (scratch address), `xnOffset` (CCU message queue) and `ckeOffset` (CKE area).

> **Note:**
> - In a real operator project (single-operator aclnn invocation) the communication context is provided by the framework through `GetHcclContext()`. This sample is a kernel direct-launch form and builds the context on the host side to demonstrate and verify the call path into `CommonPrepareImpl`.
> - `Wait` and `Finalize` only return once a real communication group drives the CCU to write back the CKE; without a real communication group they keep polling. They are therefore guarded by the `ASCENDC_CPU_DEBUG` macro in this sample.
> - The host side cannot use `AscendC::HcclCombineOpParam` directly and uses the layout-compatible `CcuCombineOpParamHost` from this sample instead. `HcclCombineOpParam` provides two layouts selected by the `__NPU_ARCH__` macro, and that macro is only defined while compiling device code, so host code would see the layout of the other products, where `windowsOut` sits at a different offset and `xnOffset`/`ckeOffset` do not exist. In addition, its `GM_ADDR` members would introduce `__gm__` address-space qualified pointers into host code, which makes the build fail with `error pointer address space cast`. The sample uses `static_assert` to verify that the member offsets of both structures match.

## Build and Run

Run the following steps in the root directory of this sample to build and execute the operator.

- Configure environment variables
  Configure the environment variables according to the [installation method](../../../../../docs/en/quick_start.md#prepare&install) of the CANN development kit in your environment.
  ```bash
  source ${install_path}/cann/set_env.sh
  ```

  > **Note:** `${install_path}` is the CANN package installation directory. If not specified, it is installed to `/usr/local/Ascend` by default.

- Run the sample

  Run the following commands in this sample directory (`hccl_allreduce_ccu`). All paths are relative, so they do not depend on where the repository is located.
  ```bash
  mkdir -p build && cd build
  cmake .. -DCMAKE_ASC_ARCHITECTURES=dav-3510   # npu mode by default
  make -j
  ./demo
  ```

  To use CPU debug mode or NPU simulation mode, add the `-DCMAKE_ASC_RUN_MODE=cpu` or `-DCMAKE_ASC_RUN_MODE=sim` argument.

  For example:
  ```bash
  cmake .. -DCMAKE_ASC_RUN_MODE=cpu -DCMAKE_ASC_ARCHITECTURES=dav-3510   # CPU debug mode
  cmake .. -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-3510   # NPU simulation mode
  ```

  > **Note:**
  > - `-DCMAKE_ASC_ARCHITECTURES=dav-3510` is mandatory. The CCU implementation is only compiled when `__NPU_ARCH__ == 3510`. With any other architecture, `Hccl<HCCL_SERVER_TYPE_CCU>` resolves to the default fallback implementation whose interfaces simply return `-1`, so `CommonPrepareImpl` is never reached.
  > - Clear the cmake cache before switching build modes. Run `rm CMakeCache.txt` in the build directory and then run cmake again, or remove the whole build directory.

- Build options

  | Option | Values | Description |
  |--------|--------|-------------|
  | `CMAKE_ASC_RUN_MODE` | `npu` (default), `cpu`, `sim` | Run mode: NPU execution, CPU debug, NPU simulation |
  | `CMAKE_ASC_ARCHITECTURES` | `dav-3510` | NPU architecture: dav-3510 corresponds to Ascend 950PR/Ascend 950DT |

  The following result indicates a successful run.
  ```bash
  AllReduce handleId: 0
  rankId: 0, rankDim: 8
  test pass!
  ```

## Verifying the Call Chain

CPU debug mode prints kernel-side logs, which can be used to confirm whether `AllReduce` is actually dispatched to the CCU implementation.

```bash
mkdir -p build && cd build
cmake .. -DCMAKE_ASC_RUN_MODE=cpu -DCMAKE_ASC_ARCHITECTURES=dav-3510
make -j
./demo 2>&1 | grep hccl_ccu_v0
```

You should see logs from `InitInner`, `CcuPrepareForAllReduceM2M` and `CcuSendMsg` in order, and the `rankId`/`rankNum` printed by `InitInner` should match the communication group parameters configured in the sample. To observe the internals of `CommonPrepareImpl`, temporarily add a `KERNEL_LOG(KERNEL_INFO, ...)` statement inside that function.

> **Note:** The headers used at build time come from the CANN installation package (`${ASCEND_HOME_PATH}/asc/include`). This path is built into the compiler and takes precedence over `-I`/`-isystem`, so editing a header with the same name outside the CANN package has no effect. To validate a modified implementation, replace the directory that `ASCEND_HOME_PATH` points to.
