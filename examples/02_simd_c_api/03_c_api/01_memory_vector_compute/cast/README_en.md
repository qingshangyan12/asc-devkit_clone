# Cast Example

This example uses Vector/UB C APIs `asc_half2int4_ru` and `asc_half2int32_ru`, preserving the Basic API example's round-up behavior. `SCENARIO_NUM=0` converts half to packed int4; `SCENARIO_NUM=1` converts half to int32. The program generates boundary input and compares every output byte with golden data.

It supports Atlas A2/A3 (`dav-2201`, CANN 9.2.0), not Ascend 950PR/950DT.

```bash
cmake -S . -B build -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-2201 -DSCENARIO_NUM=0
cmake --build build -j && (cd build && ./demo)
```
