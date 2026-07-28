# C API GM 到 L1 数据搬运样例

本样例迁移自 `01_simd_cpp_api/03_basic_api/00_data_movement/data_copy_gm2l1`，使用 512 个 `half` 元素演示最小的 GM→L1 数据通路。为了能够在主机侧观察结果，样例继续将 L1 数据通过 UB 回写到 GM：

```text
GM → L1 (`asc_copy_gm2l1_sync`) → UB (`asc_copy_l12ub_sync`) → GM (`asc_copy_ub2gm_sync`)
```

`asc_copy_l12ub` 是 Cube/Vector 核间搬运接口，因此 kernel 入口不带 `__cube__` 或 `__vector__` 修饰。该样例仅完成静态迁移，未编译或运行。
