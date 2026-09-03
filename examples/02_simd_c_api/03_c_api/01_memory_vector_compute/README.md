# Memory Vector Compute C API样例

本目录迁移自 Basic API 的 `memory_vector_compute` 样例，使用 `__ubuf__` 操作数的 Vector C API 实现；它与 `02_reg_vector_compute` 中使用寄存器操作数的 Reg C API 不同。

| 目录 | C API | 功能 |
| --- | --- | --- |
| [compare](./compare) | `asc_lt`、`asc_lt_scalar`、`asc_gt_scalar` | 向量/标量比较并输出压缩bit结果 |
| [element_wise_arithmetic](./element_wise_arithmetic) | `asc_leakyrelu` | Leaky ReLU |
| [reduce](./reduce) | `asc_repeat_reduce_sum` | 向量求和归约 |
| [cast](./cast) | `asc_half2int4_ru`、`asc_half2int32_ru` | half类型转换 |

这些 Vector C API 支持 Atlas A2训练/推理系列和 Atlas A3训练/推理系列产品（`dav-2201`）。对应 API 文档标注 Ascend 950PR/Ascend 950DT 不支持，因此本目录不支持 `dav-3510`。

当前环境的运行验证状态见 [VALIDATION.md](./VALIDATION.md)。
