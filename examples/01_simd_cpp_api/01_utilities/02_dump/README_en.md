# Dump Samples

## Overview

This sample shows how to use the Ascend C asc_dump interface series and the simd_vf asc_dump interfaces. It also provides a Runtime-level plug-in Exception Dump sample (automatic exception-scene saving without in-kernel instrumentation, with a different positioning from asc_dump_*; see python_model_exception_dump).

## Sample List

| Directory | Description | Supported Products |
| --- | --- | --- |
| [simple_dump](./simple_dump) | Uses static Tensor programming mode to implement matrix multiplication, showing basic usage of the asc_dump_gm/asc_dump_cbuf/asc_dump_ubuf interfaces; asc_dump_l1buf is demonstrated only on Atlas A2/A3 products | Ascend 950PR/Ascend 950DT<br>Atlas A3 Training Series/Atlas A3 Inference Series<br>Atlas A2 Training Series/Atlas A2 Inference Series |
| [simd_vf_dump](./simd_vf_dump) | Uses vector programming mode, showing basic usage of the simd_vf asc_dump_ubuf/asc_dump/asc_dump_reg interfaces | Ascend 950PR/Ascend 950DT |
| [cube_buffer_dump](./cube_buffer_dump) | Uses HiFloat8 quantized MatMul as the context and DumpTensor to print the HiFloat8 input matrix, bias, and per-channel scale from L1 Buffer, Bias Table Buffer, and Fixpipe Buffer | Ascend 950PR/Ascend 950DT |
| [python_model_exception_dump](./python_model_exception_dump) | Runtime-level plug-in Exception Dump sample: when an operator fails an AICore task, an exception callback attached via the Python API automatically saves the kernel args and all registered inputs (zero operator change, no in-kernel instrumentation) for offline reproduction; its positioning differs from the in-kernel asc_dump_* data-printing interfaces | Ascend 910B4 |
