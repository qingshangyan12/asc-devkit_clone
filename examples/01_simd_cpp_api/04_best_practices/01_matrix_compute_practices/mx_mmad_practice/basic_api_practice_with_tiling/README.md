# MxFP4 Matmul 基础 API 分块多场景样例

## 概述

本样例基于 Ascend C 基础 API 和静态 Tensor 编程方式，实现带 scale 的 MxFP4 Matmul。Kernel 主体参考 `matmul_mxfp4_basic_api_high_performance`，保留 GM->L1 大包搬运、L1/L0 双缓冲和 K 方向流水；`main` 参考 `matmul_mxfp4_single_core.asc`，通过编译参数 `SCENARIO_NUM` 和数据脚本参数 `scenarioNum` 选择不同场景。

## 支持的产品

- Ascend 950PR / Ascend 950DT

## 目录结构

```text
├── basic_api_practice_with_tiling
│   ├── CMakeLists.txt                     // 编译工程文件
│   ├── README.md                          // 样例说明文档
│   ├── data_utils.h                       // 数据读写工具
│   ├── scripts
│   │   ├── gen_data.py                    // 输入数据和真值生成脚本
│   │   └── verify_result.py               // 结果校验脚本
│   └── matmul_mxfp4_basic_api_with_tiling.asc   // Ascend C 样例实现
```

## 样例规格

样例当前支持两个场景，输出类型均为 `bfloat16_t`，输入 B/scaleB 均按 `[N, K]`、`[N, scaleK]` 排布：

| `SCENARIO_NUM` | M | K | N | `baseM` | `baseK` | `baseN` | `singleCoreM` | `singleCoreK` | `singleCoreN` | `stepKa/stepKb` | `scaleFactorKa/scaleFactorKb` | `numBlocks` |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| `1` | `8192` | `8192` | `8192` | `256` | `256` | `256` | `2048` | `8192` | `1024` | `2 / 2` | `4 / 4` | `32` |
| `2` | `256` | `256` | `256` | `256` | `256` | `256` | `256` | `256` | `256` | `1 / 1` | `1 / 1` | `1` |

其中：

- A 输入为 `[M, K]`，类型为 `fp4x2_e1m2_t`
- scaleA 输入为 `[M, scaleK]`，类型为 `fp8_e8m0_t`
- B 输入为 `[N, K]`，类型为 `fp4x2_e1m2_t`
- scaleB 输入为 `[N, scaleK]`，类型为 `fp8_e8m0_t`
- C 输出为 `[M, N]`，类型为 `bfloat16_t`
- `scaleK = align_even(ceil(K / 32))`

## 样例实现

每个核负责一个 `singleCoreM * singleCoreN` 子块，在 M/N 方向循环、在 K 方向按 `baseK` 累加。A/B 和 scaleA/scaleB 先从 GM 搬到 L1，再通过 `LoadData(..., scale, ...)` 进入 MX 计算路径，最后通过 `Fixpipe` 写回 GM。

关键点如下：

- 使用 `stepKa/stepKb` 控制 GM->L1 的 K 方向大包搬运粒度
- 使用 `scaleFactorKa/scaleFactorKb` 控制 scale 数据一次覆盖的 K 范围
- L1 和 L0 都采用 Ping-Pong 双缓冲，搬运、加载和计算可以交叠
- `SCENARIO_NUM=2` 使用单核、单块 `256 x 256 x 256`，便于做最小场景验证

## 编译运行

```bash
SCENARIO_NUM=2
mkdir -p build && cd build;   # 创建并进入build目录
cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DSCENARIO_NUM=$SCENARIO_NUM ..;make -j;  # 编译工程，默认npu模式
python3 ../scripts/gen_data.py -scenarioNum=$SCENARIO_NUM   # 生成测试输入数据
./demo                        # 执行编译生成的可执行程序
python3 ../scripts/verify_result.py output/output.bin output/golden.bin   # 验证输出结果是否正确，确认算法逻辑正确
```

使用 CPU调试 或 NPU仿真 模式时，添加 `-DCMAKE_ASC_RUN_MODE=cpu` 或 `-DCMAKE_ASC_RUN_MODE=sim` 参数即可。

示例如：
```bash
cmake -DCMAKE_ASC_RUN_MODE=cpu -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DSCENARIO_NUM=$SCENARIO_NUM ..;make -j; # cpu调试模式
cmake -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DSCENARIO_NUM=$SCENARIO_NUM ..;make -j; # NPU仿真模式
```

> **注意：** 切换编译模式或 `SCENARIO_NUM` 前需清理 cmake 缓存，可在 build 目录下执行 `rm CMakeCache.txt` 后重新 cmake。

- 编译选项说明

| 参数 | 说明 | 可选值 | 默认值 |
|------|------|---------|--------|
| CMAKE_ASC_RUN_MODE | 运行模式 | npu, cpu, sim | npu |
| CMAKE_ASC_ARCHITECTURES | NPU硬件架构 | dav-3510 | dav-3510 |
| SCENARIO_NUM | 样例场景编号 | 1, 2 | 2 |

数据生成脚本参数说明：

| 参数 | 说明 | 可选值 | 默认值 |
|------|------|---------|--------|
| `-scenarioNum` | 数据生成场景编号 | `1`, `2` | `2` |

- 执行结果

执行结果如下，说明精度对比成功：
```bash
test pass!
```
