# Tensor API Copy Cube Scale 接口验证样例

本样例将 MXFP4 Matmul 缩小为 `M=N=32, K=128` 的单 Tile。`COPY_ROUTE=0/1` 验证 `L1->L0ScaleA/B`，`SCENARIO_NUM=0..2` 验证三种接口形式。基准和测试scale使用随M/N分型及K group变化的126/127/128指数，使错位、错误stride和区域未生效都能产生不同结果。ScaleA的S0复制`32x2`区域、S2复制`16x4`区域；ScaleB的S0复制`4x16`区域、S2复制`2x32`区域。基准load与覆盖load之间显式串行化MTE1写，最终逐元素比较MX MMAD的bfloat16输出。

数据脚本依赖`en_dtypes`，校验脚本依赖`ml_dtypes`。执行 `python3 scripts/gen_data.py`，配置两个 CMake 参数并运行后，使用 `python3 scripts/verify_result.py --route <n> --scenario <n>` 验证。共 6 组组合。
