# Tensor API Copy Cube Load 接口验证样例

`COPY_ROUTE=0..4` 依次验证 `GM->L1`、`L1->UB`、`L1->BiasTable`、`L1->L0A`、`L1->L0B`。`SCENARIO_NUM=0..2` 选择显式 Atom 区域接口、无 Atom 整 Tensor接口、无 Atom 区域接口。五条方向全部执行数据golden：前两条通过共享UB和跨核同步回写GM，BiasTable通过带Bias MMAD观测，L0A/L0B分别通过右乘/左乘`32x32`单位矩阵重构完整目标矩阵。L0A/L0B的S0为`32x16`跨M分型区域，S2为`16x32`跨N分型区域；区域场景同时校验未覆盖的哨兵区域。

运行 `python3 scripts/gen_data.py` 后用 CMake 传入两个参数构建运行，再执行 `python3 scripts/verify_result.py --route <n> --scenario <n>`。共 15 组组合。
