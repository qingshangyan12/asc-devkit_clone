# DMA Pointer API Sample Self-Test

Official data-movement samples in this directory now call the hardware-pointer
overloads of `DataCopy` / `DataCopyPad`
(`__ubuf__` / `__gm__` / `__cbuf__` / `__cc__` / `__fbuf__`) to validate the
Basic API pointer extensions in asc-devkit. Tensor overloads remain compatible.

## Environment

```bash
source /usr/local/Ascend/cann/set_env.sh
# Use repository headers instead of toolkit-only headers (950, dav-3510)
export ASCFLAGS="-I/workspace/asc-devkit/include -I/workspace/asc-devkit/include/basic_api"
# Data scripts require numpy; python3.12 is recommended
```

## Build and Accuracy Check (950PR)

Run the following in each sample subdirectory. Scenario samples must use the
same `-scenarioNum` for cmake, gen_data, and verify.

### DataCopy slice GM↔UB

```bash
cd data_copy_gm2ub_slice
mkdir -p build && cd build
cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..
make -j
python3.12 ../scripts/gen_data.py
./demo
python3.12 ../scripts/verify_result.py output/output.bin output/golden.bin
```

### DataCopyPad GM↔UB (scenarios 1/2/3)

```bash
cd data_copy_pad_gm2ub_ub2gm
mkdir -p build && cd build
cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DSCENARIO_NUM=1 ..
make -j
python3.12 ../scripts/gen_data.py -scenarioNum 1
./demo
python3.12 ../scripts/verify_result.py output/output.bin output/golden.bin \
  -scenarioNum 1
```

Change `SCENARIO_NUM` and `-scenarioNum` to 2 or 3 to cover rightPadding /
Compact.

### NdDma multi-dimensional copy

```bash
cd data_copy_gm2ub_nddma
mkdir -p build && cd build
cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DSCENARIO_NUM=1 ..
make -j
python3.12 ../scripts/gen_data.py -scenarioNum 1
./demo
python3.12 ../scripts/verify_result.py output/output.bin output/golden.bin
```

### GM→L1 / UB→L1 / L0C→GM

```bash
# GM -> L1 (ND2NZ, scenario 2)
cd data_copy_gm2l1 && mkdir -p build && cd build
cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DSCENARIO_NUM=2 .. && make -j
python3.12 ../scripts/gen_data.py -scenarioNum 2
./demo
python3.12 ../scripts/verify_result.py output/output.bin output/golden.bin

# UB -> L1 (this sample README has no standalone verify script)
cd data_copy_ub2l1 && mkdir -p build && cd build
cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DSCENARIO_NUM=1 .. && make -j
python3.12 ../scripts/gen_data.py --scenarioNum=1
./demo

# L0C -> GM (inline quantization, scenario 1)
cd data_copy_l0c2gm && mkdir -p build && cd build
cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DSCENARIO_NUM=1 .. && make -j
python3.12 ../scripts/gen_data.py -scenarioNum 1
./demo
python3.12 ../scripts/verify_result.py -scenarioNum 1 \
  output/output.bin output/golden.bin
```

## Accuracy Criteria

Match the sample `verify_result.py` (relative error about 1e-3 to 1e-6,
absolute error about 1e-5 to 1e-9). The pointer path keeps the Tensor
semantics from before the change and does not allocate extra Device buffers
that scale with the input size.

## Performance

This task has no mandatory performance target. Pointer extensions are
compile-time overloads and do not change the underlying `*Impl` copy semantics.
