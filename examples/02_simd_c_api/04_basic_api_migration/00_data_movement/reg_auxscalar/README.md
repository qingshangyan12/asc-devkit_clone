# C API AuxScalar Reg 搬运样例

本样例迁移自 `01_simd_cpp_api/03_basic_api/00_data_movement/reg_auxscalar`。输入包含 512 个 `half` 元素和 4 个有效标量（以 16 个 `half` 对齐存储）。每个 VL 片段从 UB 加载到 `vector_half`，读取对应 UB 标量，以 `asc_add_scalar` 完成计算，再回写 UB 和 GM。

样例使用 C API 数组分配 UB 内存，显式更新每次寄存器搬运的地址；512 个元素为目标 VL 的整倍数。本样例仅作静态迁移，未编译或运行。
