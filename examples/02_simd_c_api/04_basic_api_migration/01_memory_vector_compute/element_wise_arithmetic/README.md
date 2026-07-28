# C API LeakyReLU 样例

本样例迁移自 `01_simd_cpp_api/03_basic_api/01_memory_vector_compute/element_wise_arithmetic`。

输入和输出均为 512 个 `half` 元素。核函数将输入从 GM 搬运到 UB 数组，在 UB 上计算
`y = x > 0 ? x : 0.01 * x`，再把结果搬回 GM。

由于 `asc_leakyrelu` 的当前 C API 文档未声明支持 `dav-3510` 对应产品，本样例不调用该接口；具体证据与后续替换条件见上级 [MIGRATION_NOTES.md](../../MIGRATION_NOTES.md)。

本样例仅完成静态迁移，未在本机配置、编译或运行。
