# 基于基础API的MxFP4矩阵乘样例

## 概述

本样例基于 Ascend C 基础 API 和静态 Tensor 编程方式，实现带 scale 的 MxFP4 Matmul矩阵乘法。通过编译参数 `SCENARIO_NUM` 和数据脚本参数 `scenarioNum` 选择不同场景。

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

样例当前支持两个场景，输出类型均为 `bfloat16_t`，输入 A/scaleA 均按 `[M, K]`、`[M, scaleK]` 排布，输入 B/scaleB 均按 `[N, K]`、`[N, scaleK]` 排布：
- 场景1：多核场景，固定 shape 为 `8192 x 8192 x 8192` 的 MxFP4 Matmul，输出数据类型为 `bfloat16_t`。
- 场景2：单核场景，固定 shape 为 `256 x 256 x 256` 的 MxFP4 Matmul，输出数据类型为 `bfloat16_t`。

### 输入输出format
  <img src="figure/MxMatmul.png">

| 输入/输出 | 逻辑形状 | 数据类型 | 数据排布类型 | 说明 |
|------|------|----------|------|------|
| A | `[M, K]` | `fp4x2_e1m2_t` | ND | 左矩阵，每个字节打包 2 个 fp4 元素 |
| scaleA | `[M, scaleK]` | `fp8_e8m0_t` | ND | A 矩阵的缩放因子，A矩阵在K 方向每 32 个元素共享一个 scale |
| B | `[N, K]` | `fp4x2_e1m2_t` | ND | 右矩阵，按 `[N, K]` 形式输入 kernel |
| scaleB | `[N, scaleK]` | `fp8_e8m0_t` | ND | B 矩阵的缩放因子，B矩阵在K 方向每 32 个元素共享一个 scale |
| C | `[M, N]` | `bfloat16_t` | ND | 输出矩阵 |

- 四路输入的数据排布格式如下图所示：

  <img src="figure/formatOfMx.png">


其中：
- 两个`fp4x2_e1m2_t`打包存储在一个字节中，因此当数据类型为`fp4`时，K 需要为偶数
- `scaleK = align_even(ceil(K / 32))`，表示先向上整除再对齐到 2 的倍数。由于硬件约束要求 scale 数据在 K 方向满足 2Byte 连续对齐，因此 scaleK 必须为偶数
- 由于硬件约束，当 scaleB 矩阵为 `[scaleK, N]` 输入时，需要 K 方向 2 Byte连续，因此推荐 ScaleB 采用`[N, scaleK]`输入

## 样例实现

每个核负责一个 `singleCoreM * singleCoreN` 子块，在 M/N 方向循环、在 K 方向按 `baseK` 累加。A/B 和 scaleA/scaleB 先从 GM 搬到 L1，再加载到 L0A/L0B，完成 Cube 累加计算，最后通过 `Fixpipe` 写回 GM。

  <img src="figure/InputOfMxMatmul.png">

### 关键参数说明

| 参数 |  | 说明 |
|------|----|------|
| `M` |  | 矩阵 M 维度规模 |
| `N` |  | 矩阵 N 维度规模 |
| `K` |  | 矩阵 K 维度规模 |
| `baseM` |  | Cube 计算基本块 M 维度大小 |
| `baseK` |  | Cube 计算基本块 K 维度大小 |
| `baseN` | | Cube 计算基本块 N 维度大小 |
| `singleCoreM` |  | 单核 M 方向计算范围 |
| `singleCoreN` |  | 单核 N 方向计算范围 |
| `singleCoreK` |  | 单核 K 方向计算范围 |
| `stepKa` |  | GM->L1搬运中，A 在 K 方向的搬运步长，用于控制 GM->L1 的 K 方向搬运粒度|
| `stepKb` |  | GM->L1搬运中，B 在 K 方向的搬运步长，用于控制 GM->L1 的 K 方向搬运粒度|
| `scaleFactorKa` |  | GM->L1搬运中，scaleA 相对 A 的 K 方向搬运比例 |
| `scaleFactorKb` |  | GM->L1搬运中，scaleB 相对 B 的 K 方向搬运比例 |
| `numBlocks` |  | 使用核数 |

> **约束说明**：本样例暂时不支持 K 方向存在尾块的场景，即要求 `K` 能被 `baseK` 整除。

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
