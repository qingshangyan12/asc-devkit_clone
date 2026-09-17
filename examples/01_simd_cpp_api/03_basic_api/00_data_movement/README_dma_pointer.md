# DMA 指针化样例自测说明

本目录官方搬运样例已改为调用 `DataCopy` / `DataCopyPad` 的硬件指针重载
（`__ubuf__` / `__gm__` / `__cbuf__` / `__cc__` / `__fbuf__`），用于验证
asc-devkit 中 Basic API 指针扩展。Tensor 路径声明保持兼容。

## 环境

```bash
source /usr/local/Ascend/cann/set_env.sh
# 使用仓库头文件而非仅验证 toolkit（950 架构 dav-3510）
export ASCFLAGS="-I/workspace/asc-devkit/include -I/workspace/asc-devkit/include/basic_api"
# 数据脚本依赖 numpy，建议使用 python3.12
```

## 编译与精度校验（950PR）

在各样例子目录执行。场景类样例需让 cmake、gen_data、verify 使用同一
`-scenarioNum`。

### DataCopy 切片 GM↔UB

```bash
cd data_copy_gm2ub_slice
mkdir -p build && cd build
cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..
make -j
python3.12 ../scripts/gen_data.py
./demo
python3.12 ../scripts/verify_result.py output/output.bin output/golden.bin
```

### DataCopyPad GM↔UB（场景 1/2/3）

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

将 `SCENARIO_NUM` 与 `-scenarioNum` 改为 2 或 3 可覆盖 rightPadding / Compact。

### NdDma 多维搬运

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
# GM -> L1（ND2NZ，场景 2）
cd data_copy_gm2l1 && mkdir -p build && cd build
cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DSCENARIO_NUM=2 .. && make -j
python3.12 ../scripts/gen_data.py -scenarioNum 2
./demo
python3.12 ../scripts/verify_result.py output/output.bin output/golden.bin

# UB -> L1（该样例 README 无独立 verify 脚本）
cd data_copy_ub2l1 && mkdir -p build && cd build
cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DSCENARIO_NUM=1 .. && make -j
python3.12 ../scripts/gen_data.py --scenarioNum=1
./demo

# L0C -> GM（随路量化，场景 1）
cd data_copy_l0c2gm && mkdir -p build && cd build
cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DSCENARIO_NUM=1 .. && make -j
python3.12 ../scripts/gen_data.py -scenarioNum 1
./demo
python3.12 ../scripts/verify_result.py -scenarioNum 1 \
  output/output.bin output/golden.bin
```

## 精度标准

与样例 `verify_result.py` 一致（相对误差约 1e-3 ~ 1e-6，绝对误差约 1e-5 ~
1e-9）。指针路径与改造前 Tensor 语义一致，不额外申请与输入规模线性相关的
Device 拷贝。

## 性能

本任务无强制性能指标。指针扩展为编译期重载，不改变底层 `*Impl` 搬运语义。
