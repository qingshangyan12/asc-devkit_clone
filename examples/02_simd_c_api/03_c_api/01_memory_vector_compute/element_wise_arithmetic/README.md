# ElementWiseArithmetic样例

本样例使用 Vector C API [asc_leakyrelu](../../../../../docs/zh/api/SIMD-API/c_api/vector_compute/vector_fused/asc_leakyrelu.md) 实现 Leaky ReLU：`y = x (x > 0)`，否则 `y = 0.01 * x`。输入和输出均为512个half元素，内核使用`__ubuf__`缓冲区。

支持 Atlas A2训练/推理系列和 Atlas A3训练/推理系列产品（`dav-2201`，CANN 9.2.0）。API文档标注 Ascend 950PR/950DT 不支持。

```bash
cmake -S . -B build -DCMAKE_ASC_ARCHITECTURES=dav-2201
cmake --build build -j
(cd build && ./demo)
```

成功时输出 `test pass!`。
