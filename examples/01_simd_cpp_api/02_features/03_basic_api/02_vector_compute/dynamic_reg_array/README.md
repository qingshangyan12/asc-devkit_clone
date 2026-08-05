# Four-way Reg Value Test

This diagnostic version does not use a register array. It tests `reg_value` return-by-value operations with four
explicit accumulators:

```cpp
for (uint16_t iter_m = 0; iter_m < 4; ++iter_m) {
    src0 = src0 + 1.0F;
    max0 = max(max0, src0);
    src0 = src0 + 1.0F;
    max1 = max(max1, src0);
    src0 = src0 + 1.0F;
    max2 = max(max2, src0);
    src0 = src0 + 1.0F;
    max3 = max(max3, src0);
}
```

The kernel receives the initial `src0` value as a runtime scalar. It performs 16 scalar additions and updates one
of four explicit maximum registers after each addition. Finally, it compares `max0` and `max1`. With the sample's
initial value of `3.25`, every output element should be `17.25`.

Build and run on a dav-3510 environment:

```bash
mkdir build
cd build
cmake ..
cmake --build . -j
./demo
```

The executable performs 20 warmup launches followed by 1000 measured launches. The printed average includes host
launch overhead. This version isolates aggregate register return handling from register-array indexing.
