# Compare样例

本样例使用 Vector/UB C API `asc_lt`、`asc_lt_scalar`、`asc_gt_scalar` 和 `asc_get_cmp_mask`。`SCENARIO_NUM=1..4` 分别演示向量比较、比较结果存入CmpMask寄存器、向量小于标量和标量小于向量；结果按小端序压缩为8个字节。

API仅支持 Atlas A2/A3（`dav-2201`，CANN 9.2.0），不支持 Ascend 950PR/950DT。

```bash
cmake -S . -B build -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-2201 -DSCENARIO_NUM=1
cmake --build build -j && (cd build && ./demo)
```
