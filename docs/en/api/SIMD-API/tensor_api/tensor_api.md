# Tensor API

<!-- md-trans-meta sourceCommit=0c697a9252886e83ab05f536c267cd58e7c9d89b translatedAt=2026-10-27T11:11:07.275Z pushedAt=2026-10-27T13:04:46.177Z nl=2 -->

Tensor API is a set of fundamental programming APIs for Ascend C operator development. It uses Tensor and Layout to uniformly describe the storage location, data type, size, and arrangement rules of data, and performs data movement and cube computation based on this information, reducing the workload of manually organizing hardware instruction parameters and data layout for developers.

Tensor API provides Tensor-based data movement and cube computation capabilities. Developers can combine Tensor API and C API in the same kernel function to implement complete operator functionality.

## API Hierarchy

**Table 1** API hierarchy description

| Hierarchy | Function | Main Contents |
| :--- | :--- | :--- |
| Tensor+Layout | Describes the storage location, data type, size, and arrangement rules of data. | Tensor, Pointer, Layout, Shape, Stride, and Coord. |
| Arch | Provides Operations related to data movement and cube computation, Traits for describing compile-time characteristics, and Params for describing runtime parameters. | Operation: `copy_gm_to_l1`, `mmad_operation`<br>Trait: `gm_to_l1_trait`<br>Params: `l0c_to_gm_params`. |
| Atom | Combines an Operation, a Trait, and optional Params into an atomic object for movement or cube computation. | `copy_atom`, `mmad_atom`. |
| Algorithm | Provides a unified entry point for Tensor-oriented movement and cube computation. | `copy`, `make_copy`, `mmad`, and `make_mmad`. |

When using the Tensor API, include the unified entry header file:

```cpp
#include "tensor_api/tensor.h"
```

## Capability

- [Layout](layout/layout_structure.md): Describes the construction of Layout, Shape, Stride, and Coord, the data layout description, and the related attribute query capabilities.
- [Tensor](tensor/tensor_structure.md): Describes the creation, access, and slicing capabilities of Tensor and Pointer.
- [Algorithm](algorithm.md): Describes the roles, composition relationships, and usage of Operation, Trait, and Atom in movement and cube computation.
- [Data Movement](data_move/data_move.md): Describes multiple data movement paths, including bidirectional movement between Global Memory and Unified Buffer, and movement from Global Memory to L1 Buffer.
- [Cube Computation](matrix_compute/matrix_compute.md): Describes cube data loading, cube multiply-accumulate, cube result unloading, and related key features.
- [Vector Computation](reg_vector_compute/reg_vector_compute.md): Describes data movement between Global Memory and Unified Buffer, within Unified Buffer, and from Unified Buffer to L1 Buffer.
- [Utility APIs](utils/tool_functions.md): Describes compile-time integer, dimension-preserving marker, and Layout Pattern query APIs, as well as how to use utility APIs such as type checking for Layout, Tensor, Coord, Shape, and quantization parameters.
- [Deprecated APIs](deprecated_interface/deprecated_interface.md): Describes the Upper Camel Case style APIs retained for compatibility.

## Using with C API

Tensor API provides data organization, data movement, and cube computation capabilities based on Tensor and Layout, making it suitable for building the main data processing flow of an operator. Underlying control capabilities such as cache control, synchronization control, system variables, atomic operations, and initialization are provided by C API. Developers can combine Tensor API and C API in the same kernel function to implement complete operator functionality.

The API support scope and parameter constraints of Tensor API and C API are independent of each other. When using them together, the requirements of both types of APIs must be satisfied simultaneously.

The capabilities not fully covered by Tensor API and the corresponding C API documentation are as follows.

**Table 2**  C API capability description

| Capability | C API Purpose | API Example | Related Documentation |
| :--- | :--- | :--- | :--- |
| Initialization | Restores the global control state to its initial value. | `asc_init` | [asc_init](../c_api/utils/sys_init/asc_init.md) |
| Cache Control | Preloads, flushes, and invalidates the Data Cache or Instruction Cache. | `asc_datacache_preload`, `asc_dcci`, `asc_dci` | [Cache Control](../c_api/cache_ctrl/cache_ctrl.md) |
| Synchronization Control | Performs pipeline synchronization and inter-core synchronization. | `asc_lock`, `asc_unlock`, `asc_sync_notify`, `asc_sync_wait` | [Synchronization Control](../c_api/sync/sync.md) |
| System Variables | Queries runtime information such as the core index, core count, and system Cycle. | `asc_get_arch_ver`, `asc_get_core_id`, `asc_get_system_cycle` | [System Variables](../c_api/utils/sys_var/sys_var.md) |
| Atomic Operations | Performs in-band atomic operations and Scalar atomic updates at a specified Global Memory address. | `asc_set_atomic_add`, `asc_set_atomic_none`, `asc_atomic_add` | [In-band Atomic Operations](../c_api/atomic/atomic.md), [Scalar Computation](../c_api/scalar_compute/scalar_compute.md) |
