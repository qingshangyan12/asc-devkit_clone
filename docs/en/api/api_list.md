# Ascend C API List

<!-- md-trans-meta sourceCommit=180ed2c11cdca7228578f5c22ef2bb8cd01dc25f translatedAt=2026-10-23T07:58:17.265Z pushedAt=2026-10-26T11:12:09.578Z nl=2 -->

Ascend C provides a hierarchical API system that spans the full range of capabilities, from low-level C extensions to high-level C++ class libraries. It enables you to use standard C/C++ syntax as the foundation to flexibly implement fine-grained memory management and efficient vector/matrix operations across multiple programming models, including AI Core (SIMD/SIMT) and AI CPU.

## API Classification Overview

The following table shows the overall classification of Ascend C APIs, helping you quickly locate the required APIs based on the programming model and functional requirements.

**Table 1**  Ascend C API Classification

| API Level-1 Classification | API Level-2 Classification | Classification Description |
| --- | --- | --- |
| [SIMD API](SIMD-API/basic_api/basic_api_list.md) | [Basic API](SIMD-API/basic_api/basic_api.md) | Abstracts hardware capabilities, exposes chip capabilities, and ensures completeness and compatibility. APIs marked as ISASI (Instruction Set Architecture Special Interface) do not guarantee compatibility across hardware versions. |
| [SIMD API](SIMD-API/SIMD-API.md) | [Tensor API](SIMD-API/tensor_api/tensor_api_list.md) | Describes data through Tensor and its Layout, and provides the currently supported data movement and matrix computation capabilities. Hardware capabilities not covered can be used together with the C API. |
| [SIMD API](SIMD-API/c_api/c_api.md) | [C API](SIMD-API/c_api/c_api.md) | A pure C API that exposes the complete chip programming capabilities, supports array memory allocation, is generally based on pointer programming, and provides a C language programming experience consistent with the industry. |
| [SIMD API](SIMD-API/adv_api/adv_api_list.md) | [High-level API](SIMD-API/adv_api/adv_api.md) | Implements some commonly used computation algorithms to improve programming efficiency, and is usually implemented by calling multiple basic APIs. High-level APIs include the math library, Matmul, Softmax, and other APIs. High-level APIs guarantee compatibility. |
| [SIMT API](SIMT-API/overview.md) | - | Aligned with the industry, provides single-instruction multiple-thread APIs. Implements parallel computation in the form of a single instruction with multiple threads. SIMT programming is mainly used for vector computation and is especially suitable for scenarios such as discrete access and complex control logic. The SIMT API supports two programming models: SIMT programming and SIMD-SIMT hybrid programming. For the specific supported APIs, see [SIMT Programming API List](SIMT-API/SIMT_programming_intro/api_list.md) and [SIMD-SIMT Hybrid Programming API List](SIMT-API/SIMD_SIMT_hybrid_programming_intro/api_list.md). |
| [AI CPU API](AI-CPU-API/ai_cpu_api_list.md) | - | Usually serves as a supplement to the above APIs, mainly handling non-matrix, branch-intensive computations with relatively complex logic. |
| [Utils API](Utils-API/utils_api_list.md) | - | Rich general-purpose utility classes covering the standard library (currently only SIMD is supported), platform information retrieval, runtime compilation, and log output, helping you efficiently implement operator development and performance optimization. |

## Header Files and Library Files Required for Calling APIs

After installing the firmware, driver, and CANN software package, you can reference the header files and library files of the Ascend C APIs when compiling and running applications.

You can include the dependent files based on the Ascend C APIs you actually use. The purpose of each header file is shown in the following table.

**Table 2**  SIMT Programming Model Header File List

| Header File | Description | Corresponding Library File |
| --- | --- | --- |
| simt_api/asc_simt.h | Provides the SIMT API external interface. If the computation only requires a specific data type such as fp8, you can include only simt_api/asc_fp8.h. | Not applicable |

**For more details about SIMT programming header file inclusion, see [SIMT-API](SIMT-API/overview.md).**

**Table 3**  SIMD Basic Programming Capability Header File List

| API capability level | Development scenario | Header File | Description | Corresponding library file |
| --- | --- | --- | --- | --- |
| C language pointer-based programming | Develop any operator (including fused operators) | c_api/asc_simd.h | Provides the full set of SIMD C API external interfaces. | Not applicable |
| C++ Tensor-based programming | Develop data movement, matrix computation, and related fused operators | tensor_api/tensor.h | Provides the currently supported SIMD C++ Tensor API external interfaces. Capabilities not covered can be used together with the C API. | Not applicable |
| TPipe/TQue framework-based programming | Develop any operator (including fused operators) | basic_api/kernel_basic_intf.h | Provides the full set of SIMD external interfaces for TPipe/TQue framework-based programming. | Not applicable |
| TPipe/TQue framework-based programming | Develop vector operators only | basic_api/kernel_vec_intf.h | Provides the SIMD external interfaces for TPipe/TQue framework-based programming required by vector computation. | Not applicable |
| TPipe/TQue framework-based programming | Develop matrix operators only | basic_api/kernel_cube_intf.h | Provides the SIMD external interfaces for TPipe/TQue framework-based programming required by matrix computation. | Not applicable |

**Table 4** SIMD advanced API header file list

| Header File | Description | Corresponding library file |
| --- | --- | --- |
| adv_api/kernel_api.h | Provides the device-side external interfaces of the SIMD advanced API. Can be called only in the TPipe/TQue framework-based programming mode. | Not applicable |
| adv_api/tiling_api.h | Provides the host-side external interfaces of the SIMD advanced API. Can be called only in the TPipe/TQue framework-based programming mode. | libtiling_api.a |
