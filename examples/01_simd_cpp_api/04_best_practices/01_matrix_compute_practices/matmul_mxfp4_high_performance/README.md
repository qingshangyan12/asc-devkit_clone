# MxFP4 Matmul 性能调优样例

## 概述

本样例以 MxFP4 矩阵乘法为例，介绍基于 Ascend C `Matmul` 高阶 API 的 MxMatmul 性能调优方法。整个调优过程分为三个步骤（Case 0-2），逐步展示从单核到多核、从自动 tiling 到手动 tiling 的完整调优路径。

**优化路径**：
- Case 0: 单核基础版本（基准）
- Case 1: 多核 MDL 参考路径（自动 tiling）
- Case 2: 多核 MDL 手动 tiling（精细控制 L1 参数）

## 支持的产品
- Ascend 950PR / Ascend 950DT

## 目录结构介绍

```
├── matmul_mxfp4_high_performance
│   ├── scripts
│   │   ├── gen_data.py         // 输入数据和真值数据生成脚本文件
│   │   └── verify_result.py    // 真值对比文件
│   ├── CMakeLists.txt          // 编译工程文件
│   ├── data_utils.h            // 数据读入写出函数
│   ├── matmul_mx.asc           // Ascend C样例实现（包含3个优化case）
│   ├── matmul_mx.h             // 样例头文件（kernel类与tiling生成）
```

## 样例描述

### 样例功能：

  样例实现的是固定 shape 为 8192×8192 的 MxFP4 矩阵乘法（带 scale 量化系数输入）。

### MxMatmul 简介

MxMatmul（Matrix Multiply with Scale）是一种**带量化缩放系数的矩阵乘法**，是 Ascend C Matmul API
 在 MX（Mixed-Precision）量化场景下的扩展能力。与基础 Matmul 相比，MxMatmul 的核心区别在于引入了**scale 输入**。

#### 计算公式

$$
C = (A \otimes \text{scaleA}) \times (B \otimes \text{scaleB})
$$

其中 $\otimes$ 表示广播乘（broadcast），左/右矩阵在左/右量化系数矩阵做乘积时，K 方向上每 32 个元素共享一个量化因子。

#### 参数说明

| 输入 | 名称 | 形状 | 数据类型 | 说明 |
|------|------|------|----------|------|
| A | 左矩阵 | [8192, 8192] | `fp4x2_e1m2_t` | MX FP4 量化后的左矩阵 |
| scaleA | 左量化系数矩阵 | [8192, 256] | `fp8_e8m0_t` | A 矩阵的缩放因子矩阵， K 方向每 32 个元素共享一个缩放因子 |
| B | 右矩阵 | [8192, 8192] | `fp4x2_e1m2_t` | MX FP4 量化后的右矩阵 |
| scaleB | 右量化系数矩阵 | [256, 8192] | `fp8_e8m0_t` | B 矩阵的缩放因子矩阵， K 方向每 32 个元素共享一个 |
| C | 输出 | [8192, 8192] | `bfloat16_t` | 计算结果 |

  <img src="figure/MxMatmul.png">

#### 四路输入说明

- 样例中 `sK = ceil(K / 64) * 2`，当 `K=8192` 时，`sK=256`
- 因此 `scaleA` 形状为 `[M, sK] = [8192, 256]`，`scaleB` 形状为 `[sK, N] = [256, 8192]`
- 等价含义是：K 方向每 32 个元素共享一个 scale（数据脚本按 `i // 32` 广播）
- 四路输入的搬运如下图所示

  <img src="figure/InputOfMxMatmul.png">

## 样例实现

### Case 0: 单核基础版本（基准程序）

**实现方式**：参考 `MatmulKernel<..., false>` 类实现（非 MDL 模式）

单核执行 MxFP4 矩阵乘法，使用 `CFG_NORM` 配置，tiling 由 `GenerateTilingSingleCore` 生成。

**关键代码**：
```cpp
MatmulKernel<fp4x2_e1m2_t, fp4x2_e1m2_t, bfloat16_t, bfloat16_t> matmulKernel;
REGIST_MATMUL_OBJ(&pipe, GetSysWorkSpacePtr(), matmulKernel.matmulObj, &tiling);
matmulKernel.Init(a, b, as, bs, bias, c, tiling);
matmulKernel.Process(&pipe);
```

**样例配置**：
- 单核运算
- `baseM/N/K = 64`（单核模式由 tiling API 自动生成）
- 非MDL模式（`CFG_NORM`）


**性能数据**：
 Task Duration(μs) | Block Num | aicore_time(μs) | aic_mac_time(μs) | aic_mac_ratio | aic_scalar_time(μs) | aic_scalar_ratio | aic_mte1_time(μs) | aic_mte1_ratio | aic_mte2_time(μs) | aic_mte2_ratio | aic_fixpipe_time(μs) | aic_fixpipe_ratio |
|------|------------------|-----------|----------------|-----------------|---------------|-------------------|-----------------|------------------|----------------|------------------|----------------|--------------------|
902824.399 | 1 | 902823.7 | 133473.8 | 0.148 | 899999.4 | 0.997 | 86440.15 | 0.096 | 787422.9 | 0.872 | 4188.456 | 0.005 |


**优化效果分析**：
- 单核模式仅使用一个 AIC Core 进行计算
- tiling 参数由 `MatmulApiTiling` 自动生成，无需手动配置
- 该 case 仅作为性能基准，不推荐用户使用

---

### Case 1: 多核 MDL 参考路径（自动 tiling）

**实现方式**：参考 `MatmulKernel<..., true>` 类实现（MDL 模式）

开启多核并行，使用 MDL（Multi-core Data Layout）配置，tiling 参数由 `MultiCoreMatmulTiling` 自动生成，`singleCoreM=2048, singleCoreN=1024`。

**关键代码**：
```cpp
// tiling 生成
tilingApi.SetDim(ascendcPlatform->GetCoreNumAic());
tilingApi.SetSingleShape(SINGLE_M, SINGLE_N, K);
tilingApi.SetFixSplit(BASE_M, BASE_N, BASE_K);

// kernel 执行
MatmulKernel<fp4x2_e1m2_t, fp4x2_e1m2_t, bfloat16_t, bfloat16_t, true> matmulKernel;
```

**样例配置**：
- 多核 MDL 模式（`CFG_MDL`）
- `singleCoreM = 2048, singleCoreN = 1024`
- `baseM = baseN = baseK = 256`
- M 方向 4 block × N 方向 8 block = 32 核

**性能数据**：
 Task Duration(μs) | Block Num | aicore_time(μs) | aic_mac_time(μs) | aic_mac_ratio | aic_scalar_time(μs) | aic_scalar_ratio | aic_mte1_time(μs) | aic_mte1_ratio | aic_mte2_time(μs) | aic_mte2_ratio | aic_fixpipe_time(μs) | aic_fixpipe_ratio |
|------|------------------|-----------|----------------|-----------------|---------------|-------------------|-----------------|------------------|----------------|------------------|----------------|--------------------|
943.188 | 32 | 942.49 | 704.632 | 0.748 | 653.212 | 0.693 | 419.628 | 0.445 | 744.684 | 0.79 | 30.333 | 0.032 |


**原理说明**：
- 使用MDL模板，启用"大包"搬运功能
- tiling API 自动计算 `depthA1/B1`、`stepKa/Kb` 等 L1 参数
  
  **参数含义：**
    - dbL0A、dbL0B：分别表示 A 矩阵、 B 矩阵的 MTE1 是否开启 double buffer（值为 1 或 2，2 表示开启双缓冲）
    - depthA1、depthB1：分别表示 L1 中全载基本块的份数
    - stepM、stepKa：分别表示A矩阵在 L1 中缓存的数据块 M 方向上 baseM 的倍数、Ka方向即A矩阵K方向上 baseK 的倍数
    - stepN、stepKb：分别表示B矩阵在 L1 中缓存的数据块 N 方向上 baseN 的倍数、Kb方向即B矩阵K方向上 baseK 的倍数



**下一步优化方向**：
- 自动 tiling 未必是最优 L1 配置，可通过手动 tiling 精细控制

---

### Case 2: 多核 MDL 手动 tiling（精细控制 L1 参数）

**实现方式**：参考 `MatmulKernel<..., true>` + `ApplyManualMdlL1Tiling()` 实现

在 Case 1 基础上，手动覆盖 L1 tiling 参数，开启 L1 双缓冲（double buffer），实现 L1 搬运与计算的流水线并行。同时设置 `mxTypePara` 以指定 MX scale 的 L1 全载base块。

**关键代码**：
```cpp
// 手动覆盖 L1 tiling 参数
tilingData.set_depthA1(4);
tilingData.set_depthB1(4);
tilingData.set_stepKa(2);
tilingData.set_stepKb(2);
tilingData.set_stepM(1);
tilingData.set_stepN(1);
tilingData.set_dbL0A(2);
tilingData.set_dbL0B(2);
tilingData.set_mxTypePara(MX_TYPE_PARA);
```

**样例配置**：
- 多核 MDL 模式（`CFG_MDL`）
- `singleCoreM = 2048, singleCoreN = 1024`
- 手动 L1 参数：
  - `depthA1 = 4, depthB1 = 4`（L1 一次搬 4 个 baseBlock）
  - `stepKa = 2, stepKb = 2`（K 方向步长为 2 个 baseK）
  - `stepM = 1, stepN = 1`
  - `dbL0A = 2, dbL0B = 2`（L0 双缓冲）
- `mxTypePara = 0x01010404`（scaleFactorKa=4, scaleFactorKb=4, scaleFactorM=1, scaleFactorN=1）

**性能数据**：
 Task Duration(μs) | Block Num | aicore_time(μs) | aic_mac_time(μs) | aic_mac_ratio | aic_scalar_time(μs) | aic_scalar_ratio | aic_mte1_time(μs) | aic_mte1_ratio | aic_mte2_time(μs) | aic_mte2_ratio | aic_fixpipe_time(μs) | aic_fixpipe_ratio |
|------|------------------|-----------|----------------|-----------------|---------------|-------------------|-----------------|------------------|----------------|------------------|----------------|--------------------|
691.349 | 32 | 690.75 | 640.93 | 0.928 | 432.82 | 0.627 | 426.876 | 0.618 | 616.492 | 0.892 | 33.667 | 0.049 |


**原理说明**：

- 在MxMatmul中，可以通过设置mxTypePara来控制Scale矩阵和矩阵A、B在L1中加载的比例。
- **MX Scale 缩放因子**：`scaleFactorKa=4` 表示 scaleA 数据在 K 方向的加载比例为 A 矩阵的 4 倍；`scaleFactorKb=4` 表示 scaleB 数据在 K 方向的加载比例为 B 矩阵的 4 倍

    - **mxTypePara**：组合参数，在 MxMatmul 场景使用，表示 scaleA/scaleB 载入 L1 的大小与 A/B 矩阵载入 L1 大小的倍数：
      - **bit [0:6]** `scaleFactorKa`：scaleA 与 A 矩阵在 K 方向载入数据量的比例系数，范围 [1, 127]
      - **bit [8:14]** `scaleFactorKb`：scaleB 与 B 矩阵在 K 方向载入数据量的比例系数，范围 [1, 127]
      - **bit [16:22]** `scaleFactorM`：scaleA 与 A 矩阵在 M 方向载入数据量的比例系数，范围 [1, 127]
      - **bit [24:30]** `scaleFactorN`：scaleB 与 B 矩阵在 N 方向载入数据量的比例系数，范围 [1, 127]
    - 使用约束：
      - 仅当 Ka 方向全载时（`baseK * stepKa * scaleFactorKa >= singleCoreK`），才能设置 `scaleFactorM > 1`
      - 仅当 Kb 方向全载时（`baseK * stepKb * scaleFactorKb >= singleCoreK`），才能设置 `scaleFactorN > 1`
      - scaleA、scaleB 在 M、N、K 方向的载入数据量不能超过实际大小
      - 该参数仅在 MDL 模式下生效



**性能优化建议**：
> 💡 **手动 tiling 需满足 MDL 约束**
>
> 修改 `step/depth/mxTypePara` 时，需确保满足：
> 1. `stepM * stepKa * db = depthA1`（L1 双缓冲约束）
> 2. `kaStepIter % kbStepIter == 0`（K 方向同步约束）
> 3. L1 空间不超出限制



---

## 性能对比总结

| Case | 优化策略 | Block Num | Task Duration(μs) | aic_mac_ratio | aic_mac_time(μs) | 相对 Case 0 加速比 |
|:---:|:---|:---:|:---:|:---:|:---:|:---:|
| 0 | 单核基础版本 | 1 | 902824.399 | 0.148 |133473.8 | 1.00x |
| 1 | 多核 MDL 自动 tiling | 32 | 943.188 | 0.748 | 704.632 |957.22x |
| 2 | 多核 MDL 手动 tiling + mxTypePara | 32 | 691.349 | 0.892 | 640.93 | 1305.79x |

结论：相较基础单核版本，Case 2 通过多核并行与手动 L1/mxTypePara 联合调优，端到端性能提升约 1305.79x。

理论性能对比：
本样例的性能数据在Ascend 950PR上运行得到，该处理器的主频为1.65GHz，对于MX-FP4的数据类型，每cycle处理16×64×16次乘加运算。Cube理论运算时间为
$$
T_{\text{theory}} = \frac{M \times N \times K}{16 \times 64 \times 16 \times 1.65 \times 10^9 \times \text{核数}} = \frac{8192 \times 8192 \times 8192}{4096 \times 1.65 \times 10^9 \times 32} = 635.5 μs
$$
Case 2 Cube计算耗时为691.349 μs，已经很接近理论性能峰值。

## 编译运行

### 编译执行

在本样例根目录下执行如下步骤，编译并执行样例：

- **配置环境变量**
  请根据当前环境上CANN开发套件包的[安装方式](../../../../../docs/quick_start.md#prepare&install)，选择对应配置环境变量的命令。
  - 默认路径，root用户安装CANN软件包
    ```bash
    source /usr/local/Ascend/cann/set_env.sh
    ```

  - 默认路径，非root用户安装CANN软件包
    ```bash
    source $HOME/Ascend/cann/set_env.sh
    ```

  - 指定路径install_path，安装CANN软件包
    ```bash
    source ${install_path}/cann/set_env.sh
    ```

- **样例执行**
  ```bash
  CASE_TYPE=2
  mkdir -p build && cd build;  # 创建并进入 build 目录
  cmake .. -DCASE_TYPE=$CASE_TYPE -DCMAKE_ASC_RUN_MODE=npu -DCMAKE_ASC_ARCHITECTURES=dav-3510; make -j;
  python3 ../scripts/gen_data.py
  ./demo
  python3 ../scripts/verify_result.py ./output/output.bin ./output/golden.bin
  ```

  使用NPU仿真模式时，设置 `-DCMAKE_ASC_RUN_MODE=sim` 即可
  ```bash
  cmake .. -DCASE_TYPE=$CASE_TYPE -DCMAKE_ASC_RUN_MODE=npu -DCMAKE_ASC_ARCHITECTURES=dav-3510; make -j; # npu模式
  cmake .. -DCASE_TYPE=$CASE_TYPE -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-3510; make -j; # npu仿真模式
  ```

  编译选项说明：

  | 参数 | 可选值 | 说明 |
  |------|--------|------|
  | `CASE_TYPE` | `0` / `1` / `2` | 0: 单核基础；1: 多核MDL自动tiling；2: 多核MDL手动tiling |
  | `CMAKE_ASC_RUN_MODE` | `npu`（默认）/ `cpu` / `sim` | 运行模式：NPU运行、CPU模式、NPU仿真 |
  | `CMAKE_ASC_ARCHITECTURES` | `dav-2201`（默认）/ `dav-3510` | 目标SoC架构 |

  > **注意：** 切换 `CMAKE_ASC_RUN_MODE` / `CMAKE_ASC_ARCHITECTURES` / `CASE_TYPE` 前需清理cmake缓存，可在build目录下执行 `rm CMakeCache.txt` 后重新 cmake。
  

  执行结果如下，说明精度对比成功。
  ```bash
  error ratio: 0.0000, tolerance:0.0001
  test pass!
  ```

### 性能分析

使用 `msprof` 工具获取详细性能数据：

```bash
msprof ./demo   # 分析性能
```

当前目录下会生成PROF_前缀的文件夹，`mindstudio_profiler_output`目录保存Host和各个Device的性能数据汇总，性能数据分析推荐查看该目录下文件

```bash
PROF_xxxx_XXXXXX
├── device_{id}
└── host
└── mindstudio_profiler_log
└── mindstudio_profiler_output    # 保存Host和各个Device的性能数据汇总
    ├── msprof_*.json
    ├── xx_*.csv
    └── README.txt
```
查看具体的性能分析结果：
```
# 查看Task Duration 以及各项数据
cat ./PROF_*/mindstudio_profiler_output/op_summary_*.csv
```
