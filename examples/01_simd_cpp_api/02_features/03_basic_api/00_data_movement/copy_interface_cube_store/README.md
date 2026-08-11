# Tensor API Copy Cube Store 接口验证样例

`COPY_ROUTE=0..2` 验证 `L0C->GM/UB/L1`，`SCENARIO_NUM=0..2` 验证三种接口形式。仅 `COPY_ROUTE=0` 使用 `QUANT_MODE=0/1/2` 覆盖无量化、Scalar Quant 和 L1 Tensor Quant；其他方向要求 `QUANT_MODE=0`。Tensor Quant 在实现内部使用 FixBuffer，本样例不调用公开的 `L1->FixBuffer` Copy。

三个方向均执行数据golden。区域S0复制`32x16`并跨M分型，S2复制`16x32`并跨N分型。`L0C->GM`的S1覆盖完整`32x32` NZ到ND搬运，Tensor Quant对前后16列分别使用1.0和0.5，验证quant分片切换。`L0C->UB`通过跨核同步由AIV回写GM，并检查区域外哨兵；其S1使用保留父NZ stride的`16x32` Tensor slice view验证不传Atom/Coord/Shape的whole-tensor Copy。`L0C->L1`使用`float`类型L0C源和`half`类型L1目的，Tensor API内部推导`F322F16`，再通过`L1->UB->GM`回读NZ物理数据；S0/S2同时检查区域外哨兵。先用 `python3 scripts/gen_data.py --route <n> --quant-mode <n>` 生成数据，其中验证`L0C->L1`时必须传`--route 2`；再按相同参数构建运行，最后执行 `python3 scripts/verify_result.py --route <n> --scenario <n> --quant-mode <n>`。该目录共15组组合。
