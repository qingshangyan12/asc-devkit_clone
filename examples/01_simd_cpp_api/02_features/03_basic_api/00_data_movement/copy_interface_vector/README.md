# Tensor API Copy Vector 接口验证样例

本样例验证 `GM->UB`、`UB->GM`、`UB->L1`、`UB->UB` 四个方向。`COPY_ROUTE=0..3` 选择方向，`SCENARIO_NUM=0..2` 分别选择“传 Atom 和 Coord/Shape”“不传 Atom 的整 Tensor”“不传 Atom、传 Coord/Shape”。

```bash
python3 scripts/gen_data.py
cmake -S . -B build -DCOPY_ROUTE=0 -DSCENARIO_NUM=0
cmake --build build -j
./build/demo
python3 scripts/verify_result.py --route 0 --scenario 0
```

区域场景使用满足32字节对齐的非零源、目标坐标。`GM->UB`、`UB->GM`和`UB->UB`逐元素校验；`UB->L1`在AIV完成被测Copy后，由AIC右乘`64x64`单位矩阵，将完整`32x64` L1矩阵重构到GM。S0复制`32x32`区域并跨两个M分型，S2复制`16x64`区域并跨四个N分型，因此源数据、目标数据和哨兵区都可逐元素验证。遍历两个参数可完成12组验证。
