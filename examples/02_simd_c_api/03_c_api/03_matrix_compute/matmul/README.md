# Ascend C C API MatMul 样例

本项目实现固定规格的单 Cube-Core FP32 MatMul：

```text
C[30, 70] = A[30, 40] @ B[40, 70] + bias[1, 70]
```

Kernel 使用 Ascend C C API 完成 `GM -> L1 -> L0A/L0B/BT -> Mmad -> L0C -> GM` 全流程，固定启动一个 `__cube__` block。Host 只负责文件读写、ACL 内存管理、数据拷贝、Kernel 启动和同步，不执行逐元素矩阵乘或 bias 加法。

## 支持范围

| 项目 | 规格 |
| --- | --- |
| 产品 | Ascend 950PR / Ascend 950DT |
| NPU 架构 | `dav-3510` |
| CANN | `>= 9.1.0` |
| 输入 | A `[30,40]`、B `[40,70]`、bias `[1,70]`，FP32 ND |
| 输出 | C `[30,70]`，FP32 ND |
| 转置 | A=false，B=false |
| 核数 | 1 个 Cube Core |
| HF32 | 不启用 |

任务书给出的软件范围为 CANN 9.0.0 至 9.1.0，但当前 `asc-devkit/master` 的 3510 C API `mmad` 样例声明 `>= 9.1.0`。CANN `9.0.0-beta.2` 自带头文件缺少普通 Cube API。仅用于诊断的“9.0 编译器 + master 头文件”组合虽然能够构建和启动，但非零乘法用例结果错误，因此不能作为受支持环境或验收证据。

## 数据通路与内存

| Buffer | 对齐元素数 | 字节数 |
| --- | ---: | ---: |
| A L1 / L0A | 1,280 / 1,280 | 5,120 / 5,120 |
| B L1 / L0B | 3,200 / 3,200 | 12,800 / 12,800 |
| bias L1 / BiasTable | 80 / 80 | 320 / 320 |
| C L0C | 2,560 | 10,240 |

L1 峰值为 18,240 字节。源码使用 `static_assert` 同时检查计算值和 Ascend 950 的 L1、L0A、L0B、L0C、BiasTable 容量上限。

## 目录结构

```text
.
├── CMakeLists.txt
├── matmul.asc
├── data_utils.h
├── scripts/
│   ├── gen_data.py
│   ├── run_cases.py
│   └── verify_result.py
├── tests/
├── docs/
│   ├── self-test-report.md
│   └── self-test-report_en.md
├── figures/
├── README.md
├── README_en.md
└── requirements.txt
```

## 环境准备

```bash
python3 -m venv .venv
source .venv/bin/activate
python3 -m pip install -r requirements.txt
source /path/to/Ascend/cann/set_env.sh
```

确认 `npu-smi info` 显示 Ascend 950，且 CANN 版本不低于 9.1.0。

## 编译

真机模式：

```bash
cmake -S . -B build -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DCMAKE_ASC_RUN_MODE=npu
cmake --build build -j
```

仿真模式：

```bash
cmake -S . -B build-sim -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DCMAKE_ASC_RUN_MODE=sim
cmake --build build-sim -j
```

切换模式时使用不同 build 目录，避免复用旧 CMake Cache。

## 单用例运行

以下命令必须在项目根目录执行，因为 Host 固定读取 `./input` 并写入 `./output`：

```bash
python3 scripts/gen_data.py --case P01 --output-dir input
./build/demo
python3 scripts/verify_result.py output/output.bin input/golden.bin
```

生成文件大小：A 4,800 B、B 11,200 B、bias 280 B、输出 8,400 B、FP64 golden 16,800 B。Host 会拒绝大小不准确的输入文件。

## 全量用例

P01-P08 每个重复 10 次：

```bash
python3 scripts/run_cases.py --binary ./build/demo --repeat 10
```

只运行指定用例：

```bash
python3 scripts/run_cases.py --binary ./build/demo --repeat 10 --cases P01 P05 P08
```

Runner 每轮删除旧输出；生成、执行或验证任一步失败都会立即返回非零状态。

## 精度标准

Golden 使用 NumPy FP64 计算。逐元素容差为 `atol=2^-16`、`rtol=2^-10`，匹配比例要求不低于 0.99；同时要求最大绝对误差不超过 `1e-2`，或最大 ULP 误差不超过 32。验证器还会报告非有限值数量、最大误差索引和对应 actual/golden。

## 本地合同测试

```bash
python3 -m unittest discover -s tests -v
```

测试覆盖 8 个确定性数据用例、文件大小、取消场景、NaN/Inf、最大误差保护、批量 runner、单核/Cube API 合同、布局参数、Local Memory 容量和 Host 禁止逐元素计算。

实测环境、用例参数、精度结果、性能数据和工具检查结果见[自测报告](docs/self-test-report.md)。本目录是独立样例，不修改既有 `mmad` 场景。

## 易用性反馈

- [asc-devkit #1404：MatMul 任务书与 C API 发布版本范围不一致](https://gitcode.com/cann/asc-devkit/issues/1404) 仍记录本样例的 CANN 9.0/9.1 支持范围差异。
- [asc-devkit #1408：3510 `asc_copy_l12l0b_trans` 公共声明与实现签名不一致](https://gitcode.com/cann/asc-devkit/issues/1408) 已由维护者澄清：2201 与 3510 的接口声明和实现分别由架构宏隔离，在目标架构下不会产生实际冲突。本样例不调用该接口，使用 `asc_copy_gm2l1_dn2nz` 和普通 `asc_copy_l12l0b`，因此不受 #1408 影响。
