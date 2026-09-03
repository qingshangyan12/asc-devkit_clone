# ElementWiseArithmetic Example

This example uses Vector C API [asc_leakyrelu](../../../../../docs/zh/api/SIMD-API/c_api/vector_compute/vector_fused/asc_leakyrelu.md) to compute Leaky ReLU: `y = x` for `x > 0`, otherwise `y = 0.01 * x`. Both input and output contain 512 half elements in `__ubuf__` buffers.

It supports Atlas A2 Training/Inference and Atlas A3 Training/Inference products (`dav-2201`, CANN 9.2.0). The API documentation marks Ascend 950PR/950DT unsupported.

```bash
cmake -S . -B build -DCMAKE_ASC_ARCHITECTURES=dav-2201
cmake --build build -j
(cd build && ./demo)
```

Success prints `test pass!`.
