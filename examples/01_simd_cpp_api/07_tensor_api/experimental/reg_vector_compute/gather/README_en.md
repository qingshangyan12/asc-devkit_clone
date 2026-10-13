# Register Gather with Tensor API

This sample mirrors `03_basic_api/02_reg_vector_compute/gather`: one vector core gathers
128 `half` elements using `uint16_t` indexes in `[0, 127]`, computing `dst[i] = src[index[i]]`.

GM/UB transfers use Tensor API `make_tensor` and `copy`. Register computation uses
`experimental::load`, `experimental::gather`, and `experimental::store`.
The gather result inherits the index register mask. Indexes refer to elements within one
256-byte register, not byte offsets or global memory indexes.

## Build and Run

Requires CANN with `dav-3510` support, a compatible NPU or instruction simulator, Python 3 and NumPy.
Run from this sample directory, adjusting the environment script path if needed:

```bash
source /usr/local/Ascend/cann-9.3.0/set_env.sh
cmake -S . -B build -DCMAKE_ASC_ARCHITECTURES=dav-3510 \
  -DCMAKE_ASC_RUN_MODE=npu -DCANN_ASC_USE_EXPERIMENTAL=ON
cmake --build build -j2
cd build
for case in random identity reverse repeated; do
  python3 ../scripts/gen_data.py --case "$case" && \
    ./demo && python3 ../scripts/verify_result.py || exit 1
done
```

Use `-DCMAKE_ASC_RUN_MODE=sim` and a separate build directory for simulation.
CMake prioritizes this repository's Tensor API headers and implementation.
The default random case uses a fixed seed and includes both boundary indexes; additional
cases cover identity, reversed and repeated indexes. Golden data is generated with NumPy
`source[indexes]`. Both the executable and verification script require all 128 elements
to match bit-for-bit and return a nonzero exit status on failure.
