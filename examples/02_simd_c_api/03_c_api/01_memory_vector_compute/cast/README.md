# Cast样例

本样例使用 Vector/UB C API `asc_half2int4_ru` 与 `asc_half2int32_ru` 保留 Basic API 样例的向上取整语义。`SCENARIO_NUM=0` 为half到打包int4，`SCENARIO_NUM=1` 为half到int32；程序内生成边界输入并与golden逐字节比较。

支持 Atlas A2/A3（`dav-2201`，CANN 9.2.0）；不支持 Ascend 950PR/950DT。

```bash
cmake -S . -B build -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-2201 -DSCENARIO_NUM=0
cmake --build build -j && (cd build && ./demo)
```
