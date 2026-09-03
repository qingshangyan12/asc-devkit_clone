# Memory Vector Compute C API Examples

This directory migrates Basic API `memory_vector_compute` examples using Vector C APIs with `__ubuf__` operands. These APIs are distinct from the register-operand Reg C APIs in `02_reg_vector_compute`.

| Directory | C API | Function |
| --- | --- | --- |
| [compare](./compare) | `asc_lt`, `asc_lt_scalar`, `asc_gt_scalar` | Vector/scalar comparison with packed-bit output |
| [element_wise_arithmetic](./element_wise_arithmetic) | `asc_leakyrelu` | Leaky ReLU |
| [reduce](./reduce) | `asc_repeat_reduce_sum` | Vector sum reduction |
| [cast](./cast) | `asc_half2int4_ru`, `asc_half2int32_ru` | half conversion |

These Vector C APIs support Atlas A2 Training/Inference and Atlas A3 Training/Inference products (`dav-2201`). Their API documents mark Ascend 950PR/Ascend 950DT as unsupported, so this directory does not support `dav-3510`.

See [VALIDATION.md](./VALIDATION.md) for the current validation status.
