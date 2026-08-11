# Tensor API Copy Vector Interface Example

This example covers `GM->UB`, `UB->GM`, `UB->L1`, and `UB->UB`. Select the route with `COPY_ROUTE=0..3` and the call form with `SCENARIO_NUM=0..2`: explicit Atom plus Coord/Shape, inferred Atom for the whole Tensor, or inferred Atom plus Coord/Shape.

```bash
python3 scripts/gen_data.py
cmake -S . -B build -DCOPY_ROUTE=0 -DSCENARIO_NUM=0
cmake --build build -j
./build/demo
python3 scripts/verify_result.py --route 0 --scenario 0
```

Regional cases use 32-byte-aligned nonzero coordinates. GM-to-UB, UB-to-GM, and UB-to-UB use elementwise checks. For UB-to-L1, AIV performs the tested Copy and AIC right-multiplies the result by a `64x64` identity matrix, reconstructing the complete `32x64` L1 matrix in GM. S0 copies a `32x32` region across two M fractals; S2 copies a `16x64` region across four N fractals. Source data, destination data, and sentinel regions are therefore checked element by element. Iterate all route and scenario values for 12 combinations.
