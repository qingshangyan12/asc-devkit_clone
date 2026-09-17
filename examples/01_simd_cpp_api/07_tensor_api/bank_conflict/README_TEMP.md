# bank_conflict temporary run guide

## Scenario 1: Reg VF, stride 144 (conflict baseline)

```bash
mkdir -p build_s1 && cd build_s1
cmake -DSCENARIO_NUM=1 -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..
cmake --build . -j
python3 ../scripts/gen_data.py
./demo
python3 ../scripts/verify_result.py output/output.bin output/golden.bin
```

## Scenario 2: Reg VF, stride 145 (bank offset)

```bash
mkdir -p build_s2 && cd build_s2
cmake -DSCENARIO_NUM=2 -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..
cmake --build . -j
python3 ../scripts/gen_data.py
./demo
python3 ../scripts/verify_result.py output/output.bin output/golden.bin
```

## Scenario 3: column-copy loop, stride 144 (conflict baseline)

```bash
mkdir -p build_s3 && cd build_s3
cmake -DSCENARIO_NUM=3 -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..
cmake --build . -j
python3 ../scripts/gen_data.py
./demo
python3 ../scripts/verify_result.py output/output.bin output/golden.bin
```

## Scenario 4: column-copy loop, explicit offset (stride 145)

```bash
mkdir -p build_s4 && cd build_s4
cmake -DSCENARIO_NUM=4 -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..
cmake --build . -j
python3 ../scripts/gen_data.py
./demo
python3 ../scripts/verify_result.py output/output.bin output/golden.bin
```
