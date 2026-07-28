# C API 多维 GM 到 UB 搬运样例

本样例迁移自 `01_simd_cpp_api/03_basic_api/00_data_movement/data_copy_gm2ub_nddma`，展示二维固定值 Padding：将 `3 x 4` 的 float 输入搬入 `9 x 6` 的 UB 数组，内层左/右分别补 2/1 个元素，外层上/下分别补 2/3 个元素，补充值为 `88.0f`。

配置使用 C API 文档的二维 Padding 调用形式。原样例中最近值 Padding、转置、广播和切片的 `NdDmaLoopInfo` 到 C API 参数映射尚待确认，详见上级 [MIGRATION_NOTES.md](../../MIGRATION_NOTES.md)。本样例未编译或运行。
