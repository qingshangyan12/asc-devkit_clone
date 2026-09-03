# Reduce Example

This example uses Vector/UB C API `asc_repeat_reduce_sum` to sum 64 float elements. The result is stored in output element 0 and verified on the host.

It supports Atlas A2/A3 (`dav-2201`, CANN 9.2.0), not Ascend 950PR/950DT.

```bash
cmake -S . -B build -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-2201
cmake --build build -j && (cd build && ./demo)
```
