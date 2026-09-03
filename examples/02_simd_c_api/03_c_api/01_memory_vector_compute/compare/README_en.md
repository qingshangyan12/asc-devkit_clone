# Compare Example

This example uses Vector/UB C APIs `asc_lt`, `asc_lt_scalar`, `asc_gt_scalar`, and `asc_get_cmp_mask`. `SCENARIO_NUM=1..4` demonstrates vector comparison, CmpMask output, vector-less-than-scalar, and scalar-less-than-vector. Results are packed in little-endian bits.

The APIs support Atlas A2/A3 (`dav-2201`, CANN 9.2.0), not Ascend 950PR/950DT.

```bash
cmake -S . -B build -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-2201 -DSCENARIO_NUM=1
cmake --build build -j && (cd build && ./demo)
```
