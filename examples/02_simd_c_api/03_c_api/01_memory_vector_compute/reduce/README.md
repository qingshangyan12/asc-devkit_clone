# Reduce样例

本样例使用 Vector/UB C API `asc_repeat_reduce_sum` 对64个float元素求和，结果写入输出第0个元素并在主机侧校验。

支持 Atlas A2/A3（`dav-2201`，CANN 9.2.0）；不支持 Ascend 950PR/950DT。

```bash
cmake -S . -B build -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-2201
cmake --build build -j && (cd build && ./demo)
```
