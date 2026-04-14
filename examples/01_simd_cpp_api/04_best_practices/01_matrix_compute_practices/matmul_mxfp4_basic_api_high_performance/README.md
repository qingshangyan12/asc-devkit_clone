# MXFP4 Basic API 高性能样例（支持尾块）

## 概述
该样例基于基础 API 实现 `mxmatmul`，并满足以下约束：

- 固定使用 32 核，分核策略为 `M 切 4`、`N 切 8`。
- 输入 `B` 与 `scaleB` 按预转置格式提供（分别为 `[N, K]`、`[N, scaleK]`）。
- 输出为 `bfloat16`。
- 支持 `M/N/K` 非对齐场景（尾块泛化）。
- Kernel 使用静态 Tensor 编程方式（手工地址管理 + 手工同步）。

## 与普通 Matmul 的区别

- 普通 Matmul 输入：`A`、`B`
- MX Matmul 输入：`A`、`scaleA`、`B`、`scaleB`

即 MX 场景比普通 Matmul 多两路输入：`scaleA/scaleB`。

## 目录

```text
matmul_mxfp4_basic_api_high_performance
├── CMakeLists.txt
├── data_utils.h
├── mmad_mx.asc
└── scripts
    ├── gen_data.py
    └── verify_result.py
```

## 数据规格

- `A`: `fp4x2_e1m2_t`，shape `[M, K]`（脚本中自动打包）
- `B`: `fp4x2_e1m2_t`，shape `[N, K]`（预转置）
- `scaleA`: `fp8_e8m0_t`，shape `[M, scaleK]`
- `scaleB`: `fp8_e8m0_t`，shape `[N, scaleK]`（预转置）
- `C`: `bfloat16_t`，shape `[M, N]`
- `scaleK = align_even(ceil(K / 32))`

## 运行方式

- **样例执行**
  ```bash
  SCENARIO_NUM=2
  mkdir -p build && cd build;  # 创建并进入 build 目录
  cmake .. -DSCENARIO_NUM=1 -DCMAKE_ASC_RUN_MODE=npu -DCMAKE_ASC_ARCHITECTURES=dav-3510; make -j;
  python3 ../scripts/gen_data.py
  ./demo
  python3 ../scripts/verify_result.py ./output/output.bin ./output/golden.bin
  ```

  使用NPU仿真模式时，设置 `-DCMAKE_ASC_RUN_MODE=sim` 即可
  ```bash
  cmake .. -DSCENARIO_NUM=1 -DCMAKE_ASC_RUN_MODE=npu -DCMAKE_ASC_ARCHITECTURES=dav-3510; make -j; # npu模式
  cmake .. -DSCENARIO_NUM=1 -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-3510; make -j; # npu仿真模式
  ```

## NPU SIM 小矩阵验证

```bash
mkdir -p build_sim && cd build_sim
cmake -DRUN_MODE=sim -DNPU_ARCH=dav-3510 ..
make -j
M=33 N=35 K=37 python3 ../scripts/gen_data.py
./demo 33 35 37
python3 ../scripts/verify_result.py output/output.bin output/golden.bin
```

> 注意：该样例依赖 MXFP4 基础类型与指令链路，需要 `dav-3510` 架构支持。

## 尾块测试

通过环境变量生成非对齐数据，再传给可执行文件：

```bash
M=8193 N=8201 K=8187 python3 ../scripts/gen_data.py
./demo 8193 8201 8187
python3 ../scripts/verify_result.py output/output.bin output/golden.bin
```
