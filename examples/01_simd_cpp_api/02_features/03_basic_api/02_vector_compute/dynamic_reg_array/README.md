# Dynamic Reg Array Index Test

This example tests whether a four-element vector-register array can be selected with a runtime index:

```cpp
max_array[iter_m % 4] = max(max_array[iter_m % 4], src0);
```

The kernel receives the initial `src0` value as a runtime scalar. It performs 16 scalar additions and updates one
of four maximum registers after each addition. Finally, it compares `max_array[0]` and `max_array[1]`. With the
sample's initial value of `3.25`, every output element should be `17.25`.

Build and run on a dav-3510 environment:

```bash
mkdir build
cd build
cmake ..
cmake --build . -j
./demo
```

The executable performs 20 warmup launches followed by 1000 measured launches. The printed average includes host
launch overhead. Use the platform profiler and generated device code to evaluate register spills, branches, and
the kernel's device-side cycle count.
