# C API Mmad 样例

## 概述

本样例以 int8_t 和 bfloat16 两种输入数据类型为例，演示如何通过 Ascend C API 实现矩阵乘法（C = A × B + Bias）。每个场景的流水线函数拆分到独立的头文件中（`c_api_mmad_s1.h` / `c_api_mmad_s2.h`），主文件 `c_api_mmad.asc` 通过 `if constexpr (scenario_num)` 在核函数入口处按场景分发，结构清晰，易于扩展。

## 支持的产品及 CANN 软件版本

| 产品 | CANN 软件版本 |
|------|-------------|
| Ascend 950PR / Ascend 950DT | >= CANN 9.1.0 |

## 目录结构

```
├── c_api_mmad
│   ├── c_api_mmad_s1.h            // 场景1（int8_t）的常量与完整流水线函数
│   ├── c_api_mmad_s2.h            // 场景2（bfloat16）的常量与完整流水线函数
│   ├── c_api_mmad.asc             // 主文件（公共常量、核函数入口、Host 端 main）
│   ├── scripts
│   │   ├── gen_data.py             // 输入数据和真值数据生成脚本
│   │   ├── mmad.py                 // 矩阵乘法辅助脚本
│   │   └── verify_result.py        // 精度验证脚本
│   ├── CMakeLists.txt              // 编译工程文件
│   ├── data_utils.h                // 数据读写函数
│   └── README.md                   // 样例说明文档
```

## 代码组织

每个场景的常量（对齐后的缓冲区大小等）和四阶段流水线函数分别定义在独立的头文件中：

| 文件 | 场景 | 函数数量 | 内容 |
|------|------|----------|------|
| `c_api_mmad_s1.h` | 场景 1（int8_t） | 8 | 场景专属常量 + 完整流水线：GM→L1（×3）、L1→L0A/L0B/BT（×3）、Mmad、L0C→GM |
| `c_api_mmad_s2.h` | 场景 2（bfloat16） | 6 | 场景专属常量 + 完整流水线：GM→L1（×2）、L1→L0A/L0B（×2）、Mmad、L0C→GM |
| `c_api_mmad.asc` | 主文件 | — | 公共常量（维度 M/N/K、`ceil_div`/`ceil_align` 辅助宏）、核函数入口（`if constexpr` 场景分发）、Host 端 `main` |

## 样例描述

一次完整的矩阵乘法涉及的数据搬运过程包括：GM → L1、L1 → L0A / L0B、L1 → BT（BiasTable Buffer）、L0C → GM。不同存储单元的数据排布格式如下表所示。

<a name="表1"></a>
<table border="2" align="center">
<caption style="font-weight: normal;">
    <span style="font-weight: bold; font-size: 1.2em;">表1：不同存储单元的数据排布格式</span></caption>
  <tr>
    <td>存储单元</td>
    <td>数据排布格式</td>
  </tr>
  <tr>
    <td>GM</td>
    <td>输入 A、B 矩阵和输出 C 矩阵为 ND 排列。</td>
  </tr>
  <tr>
    <td>L1</td>
    <td>A、B 矩阵为 Nz 排列。</td>
  </tr>
  <tr>
    <td>L0A</td>
    <td>A 矩阵为 Nz 排列。</td>
  </tr>
  <tr>
    <td>L0B</td>
    <td>B 矩阵为 Zn 排列。</td>
  </tr>
  <tr>
    <td>BT（BiasTable Buffer）</td>
    <td>Bias 是 shape 为 [N] 的一维 Tensor。</td>
  </tr>
  <tr>
    <td>L0C</td>
    <td>C 矩阵为 Nz 排列。</td>
  </tr>
</table>

矩阵乘法计算公式：C = A × B + Bias，其中 A、B、Bias、C 矩阵需满足的 shape 分别为 [M, K]、[K, N]、[N] 和 [M, N]。Bias 仅在场景 1 中启用，其数据类型与 C 矩阵的对应关系如下表所示。

<a name="表2"></a>
<table border="2" align="center">
<caption style="font-weight: normal;">
    <span style="font-weight: bold; font-size: 1.2em;">表2：Bias 与 C 矩阵的数据类型对应关系</span></caption>
  <tr>
    <td>Bias 在 GM/L1 上的数据类型</td>
    <td>Bias 在 BT 上的数据类型</td>
    <td>C 矩阵在 L0C 上的数据类型</td>
  </tr>
  <tr>
    <td>int32_t</td>
    <td>int32_t</td>
    <td>int32_t</td>
  </tr>
</table>

编译参数 `SCENARIO_NUM` 的不同取值对应如下场景：

<a name="表3"></a>
<table border="2" align="center">
<caption style="font-weight: normal;">
    <span style="font-weight: bold; font-size: 1.2em;">表3：SCENARIO_NUM 不同取值的含义</span></caption>
  <tr>
    <td>SCENARIO_NUM</td>
    <td>输入数据类型</td>
    <td>输出数据类型</td>
    <td>A 矩阵</td>
    <td>B 矩阵</td>
    <td>Bias</td>
  </tr>
  <tr>
    <td>1</td>
    <td>int8_t</td>
    <td>int32_t</td>
    <td>不转置</td>
    <td>不转置</td>
    <td>带 Bias，C 矩阵初始值来源于 BT</td>
  </tr>
  <tr>
    <td>2</td>
    <td>bfloat16_t</td>
    <td>float</td>
    <td>不转置</td>
    <td>转置</td>
    <td>不带 Bias，C 矩阵初始值来源于 CO1（两次 Mmad 累加）</td>
  </tr>
</table>

### 场景详细说明

本样例通过编译参数 `SCENARIO_NUM` 选择场景，所有场景基于相同的矩阵乘规格：[M, N, K] = [30, 40, 70]，核函数名为 `mmad_custom`。

**场景 1：int8_t 输入，int32_t 输出，带 Bias**

- 输入：
  - A 不转置 [30, 70] int8_t，ND 格式
  - B 不转置 [70, 40] int8_t，ND 格式
  - Bias [1, 40] int32_t
- 输出：C [30, 40] int32_t，ND 格式
- 实现（详见 `c_api_mmad_s1.h`）：
  1. `asc_copy_gm2l1_nd2nz` + `asc_set_gm2l1_nz_para`：A、B、Bias 从 GM 搬运至 L1（ND → Nz）
  2. `asc_copy_l12l0a`：A 从 L1 搬运至 L0A；`asc_copy_l12l0b_trans`：B 从 L1 转置搬运至 L0B；`asc_copy_l12bt`：Bias 从 L1 搬运至 BT
  3. `asc_mmad`：矩阵乘加，C 矩阵初始值来源于 BT
  4. `asc_copy_l0c2gm` + `asc_set_l0c2gm_nz2nd`：结果从 L0C 搬运至 GM（Nz → ND）
- 说明：int8_t 输入、B 不转置场景下，N 轴向需要对齐到 `BLOCK_CUBE * fractalNum`（即 32），Mmad 调用中 `right_width` 需设置为 `ceil_align(N, BLOCK_CUBE * fractalNum)`，搬运出结果时通过 `n_size = N` 排除无效数据。

**场景 2：bfloat16_t 输入，float 输出，无 Bias，两次 Mmad 累加**

- 输入：
  - A 不转置 [30, 70] bfloat16_t，ND 格式
  - B 转置 [40, 70] bfloat16_t，ND 格式
- 输出：C [30, 40] float，ND 格式
- 实现（详见 `c_api_mmad_s2.h`）：
  1. `asc_copy_gm2l1_nd2nz` + `asc_set_gm2l1_nz_para`：A、B 从 GM 搬运至 L1（B 以转置后的 ND 排布存放于 GM）
  2. `asc_copy_l12l0a`：A 从 L1 搬运至 L0A；`asc_copy_l12l0b`：B 从 L1 直接搬运至 L0B（B 已在 L1 中转置为 Nz 排布）
  3. `asc_mmad`：两次调用。首次 `c_matrix_init_val = true` 将 C 初始化为 0 并计算 A×B；第二次 `c_matrix_init_val = false`、`c_matrix_source = false` 以 CO1 为初值累加第二次 A×B
  4. `asc_copy_l0c2gm` + `asc_set_l0c2gm_nz2nd`：结果从 L0C 搬运至 GM

### C API 流水线同步

各阶段之间通过事件（EVENT_ID0 / EVENT_ID1 / EVENT_ID2）进行流水线同步：

- 阶段 1 的函数发射后通过 `asc_sync_notify(MTE2, MTE1, EVENT_IDx)` 通知搬运完成
- 阶段 2 的函数通过 `asc_sync_wait(MTE2, MTE1, EVENT_IDx)` 等待数据就绪，完成后通过 `asc_sync_notify(MTE1, M, EVENT_IDx)` 通知计算单元
- 阶段 3 通过 `asc_sync_wait(MTE1, M, ...)` 等待所有数据就绪后执行 Mmad，完成后通知 FIX 流水线
- 阶段 4 通过 `asc_sync_wait(M, FIX, EVENT_ID0)` 等待计算完成后搬运结果
- 核函数末尾调用 `asc_sync_pipe(PIPE_ALL)` 同步所有流水线

## 编译运行

在本样例根目录下执行如下步骤，编译并执行算子。

- 配置环境变量
  ```bash
  source ${install_path}/cann/set_env.sh
  ```

  > **说明：** `${install_path}` 为 CANN 包安装目录，未指定时默认安装至 `/usr/local/Ascend`。

- 样例执行

  ```bash
  SCENARIO_NUM=1
  mkdir -p build && cd build
  cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DSCENARIO_NUM=$SCENARIO_NUM .. && make -j
  python3 ../scripts/gen_data.py -scenarioNum=$SCENARIO_NUM
  ./demo
  python3 ../scripts/verify_result.py -scenarioNum=$SCENARIO_NUM output/output.bin output/golden.bin
  ```

  使用 CPU 调试或 NPU 仿真模式时，添加 `-DCMAKE_ASC_RUN_MODE=cpu` 或 `-DCMAKE_ASC_RUN_MODE=sim` 参数：

  ```bash
  cmake -DCMAKE_ASC_RUN_MODE=cpu -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DSCENARIO_NUM=$SCENARIO_NUM .. && make -j
  cmake -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DSCENARIO_NUM=$SCENARIO_NUM .. && make -j
  ```

  > **注意：** 切换编译模式前需清理 cmake 缓存，可在 build 目录下执行 `rm CMakeCache.txt` 后重新 cmake。

- 编译选项说明

  | 选项 | 可选值 | 说明 |
  |------|--------|------|
  | `CMAKE_ASC_RUN_MODE` | `npu`（默认）、`sim`、`cpu` | 运行模式：NPU 运行、NPU 仿真、CPU 调试 |
  | `CMAKE_ASC_ARCHITECTURES` | `dav-3510`（默认） | NPU 架构，对应 Ascend 950PR / Ascend 950DT |
  | `SCENARIO_NUM` | `1`（默认）、`2` | 场景编号：1 = int8_t 输入，2 = bfloat16_t 输入 |

- 执行结果

  精度对比成功时的输出：

  ```bash
  test pass!
  ```
