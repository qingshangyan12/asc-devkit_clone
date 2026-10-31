# C API

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-23T04:27:26.826031+00:00 pushedAt=2026-10-27T02:26:31.912Z nl=2 -->

The C API exposes the complete programming capabilities of the chip and supports the [method of allocating memory as an array](general_description_and_constraints.md#allocating-buffers-as-arrays). It is generally based on pointer programming and provides a C language programming experience consistent with industry standards.

## Positioning and Features

The C API is the **language extension layer SIMD API** in the Ascend C three-tier programming API. It is the lowest-level C language API, uses pointer-based programming, and provides complete C language programming capabilities. The C API maps directly to NPU hardware instructions and exposes all underlying hardware capabilities. Developers manage memory data movement and synchronization themselves, whereas TPipe/TQue manage memory and synchronization automatically. It follows C programming practices and is suitable for operator development that requires high performance and precise control, enabling developers to fully utilize NPU hardware performance.

For details, see [C Language Programming Overview](../../../guide/programming_guide/programming_model/ai_core_simd_programming/c_pointer_programming/c_programming_overview.md).

## Header Files to Include

> [!NOTE] 
> Include the asc_simd.h file to call the corresponding C APIs. Unless otherwise specified, including this header file meets the API calling requirements.
> If the API documentation contains special instructions, follow the specific instructions of the API.

```cpp
#include "c_api/asc_simd.h"
```

## API Types

- Computation of the first n data elements: This type of API uses a "tightly packed" data reading method during computation, that is, it sequentially and contiguously obtains the required data starting from the initial position. For example, if N data elements need to be processed, they are obtained starting from position 0 of the source operand and continuing sequentially to position N-1.
- High-dimensional slicing computation: This type of API "skips part of the data" according to the configured rules. It is suitable for scenarios that require interval sampling and offers high flexibility, but additional related parameters need to be configured.
- Synchronous computation: This type of API automatically inserts synchronization operations internally, providing greater ease of use.

## Keywords

**Table 1** Keyword list

|Name|Description|
|-----------------------|-----------------------|
|\_\_gm\_\_|Storage space definition modifier, indicating that the modified variable is located in the Global Memory address space.|
|\_\_ubuf\_\_|Storage space definition modifier, indicating that the modified variable is located in the UB address space.|
|\_\_aicore\_\_|Execution space qualifier, indicating that the function can be executed only on the AI Core.|
|\_\_cbuf\_\_|Storage space definition modifier, indicating that the modified variable is located in the L1 Buffer address space.|
|\_\_simd\_callee\_\_|Execution space qualifier, indicating that the function can be executed only on Reg.|

## Pipeline Type

The NPU contains different computation units. During computation, the computation data often needs to be moved to the computation units. The computation processes and data movement processes on different computation units can be divided into different pipelines. Operations executed on the AI Core are assigned to different pipelines (PIPEs) for execution, including the following:
- PIPE_S: Scalar pipeline, responsible for instruction dispatch and scalar computation.
- PIPE_V: Vector computation pipeline.
- PIPE_M: Cube computation pipeline.
- PIPE_MTE1: Data movement operations, including data movement from L1 Buffer to L0A Buffer or L0B Buffer, from L1 Buffer to Unified Buffer (UB), and initialization of L0A Buffer or L0B Buffer.
- PIPE_MTE2: Transfer operation, including transfer operations from GM to L1 Buffer, from GM to L0A Buffer or L0B Buffer, from GM to UB, and initialization operations of L1 Buffer.
- PIPE_MTE3: Transfer operation, including operations from UB to GM, from UB to L1 Buffer, from L1 Buffer to GM, and transfer operations from UB to UB.
- PIPE_FIX: Fixpipe pipeline.
- PIPE_ALL: all pipelines.

The [Synchronization Control](sync/sync.md) APIs can be used to control the execution order within the same pipeline and the execution order between different pipelines.

## API List

|      Directory      |
|-----------------------|
| [Basic definitions](c_api.md#basic-definitions) |
| [Utility APIs](c_api.md#utility-apis) |
| [Cube computation](c_api.md#cube-computation) |
| [Cube computation data movement](c_api.md#cube-computation-data-movement) |
| [Memory vector computation](c_api.md#memory-vector-computation) |
| [Vector data movement](c_api.md#vector-data-movement) |
| [Reg vector computation](c_api.md#reg-vector-computation) |
| [Scalar computation](c_api.md#scalar-computation) |
| [Atomic operations](c_api.md#atomic-operations) |
| [Cache control](c_api.md#cache-control) |
| [Synchronization control](c_api.md#synchronization-control) |
| [Other operations](c_api.md#other-operations) |
| [Special register access](c_api.md#special-register-access) |
| [Deprecated APIs](deprecated_interface/deprecated_interface.md) |

## Examples

For C API samples of Atlas A3 products, see [C API Add Sample](../../../../../examples/02_simd_c_api/00_introduction/01_add).
For C API samples of Ascend 950PR&950DT products, see [C API Add Sample](../../../../../examples/02_simd_c_api/00_introduction/04_reg_base_add_compute).
