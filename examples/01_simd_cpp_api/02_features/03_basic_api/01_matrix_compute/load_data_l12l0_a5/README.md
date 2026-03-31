# LoadData L1->L0 (Ascend 950PR/Ascend 950DT)

## 概述

本样例适用于 Ascend 950PR/Ascend 950DT，介绍了在矩阵乘法场景下，使用 `Load2dV2` 接口将矩阵数据从 L1 搬运到 L0A/L0B 的方法。

**关键区别**：Ascend 950PR/Ascend 950DT 的 L0A 数据排布格式为 **Nz**，与 Atlas A2/A3 系列产品的 **Zz** 排布不同。

## 支持的场景

本样例共支持 **6 种场景**，涵盖 int8_t、half、float 三种数据类型，以及两种转置组合：

| scenarioNum | 数据类型 | A矩阵转置 | B矩阵转置 | A矩阵GM输入 | B矩阵GM输入 | L1->L0A接口 | L1->L0B接口 |
|-------------|----------|-----------|-----------|-------------|-------------|-------------|-------------|
| 1 | int8_t | 否 | 是 | [m, k] | [n, k] | Load2dV2(不转置) | Load2dV2(不转置) |
| 2 | int8_t | 是 | 否 | [k, m] | [k, n] | Load2dV2(转置) | Load2dV2(转置) |
| 3 | half | 否 | 是 | [m, k] | [n, k] | Load2dV2(不转置) | Load2dV2(不转置) |
| 4 | half | 是 | 否 | [k, m] | [k, n] | Load2dV2(转置) | Load2dV2(转置) |
| 5 | float | 否 | 是 | [m, k] | [n, k] | Load2dV2(不转置) | Load2dV2(不转置) |
| 6 | float | 是 | 否 | [k, m] | [k, n] | Load2dV2(转置) | Load2dV2(转置) |

**矩阵乘规格**：[m, n, k] = [40, 50, 70]

**输出数据类型**：
- int8_t 输入 → int32_t 输出
- half 输入 → float 输出
- float 输入 → float 输出

## 数据排布格式差异

### Ascend 950PR/Ascend 950DT vs Atlas A2/A3 系列

| 存储位置 | Ascend 950PR/950DT | Atlas A2/A3 系列 |
|----------|---------------------|------------------|
| L0A | **Nz** 格式 | **Zz** 格式 |
| L0B | Zn 格式 | Zn 格式 |
| L1 | Nz 格式 | Nz 格式 |
| L0C | Nz 格式 | Nz 格式 |

**Nz 格式分形大小**：16 × (32B / sizeof(T))
**Zn 格式分形大小**：(32B / sizeof(T)) × 16

## 整体流程

矩阵乘法完整流程如下：

```
GM(ND) -> L1(Nz) -> L0A(Nz)/L0B(Zn) -> L0C(Nz) -> GM(ND)
   │         │              │              │         │
 DataCopy  Load2dV2       Mmad        Fixpipe    输出
```

1. **GM -> L1**：使用 `DataCopy` 的 `Nd2NzParams` 实现 ND 到 Nz 格式转换
2. **L1 -> L0A/L0B**：使用 `Load2dV2` 接口搬运，支持转置功能
3. **矩阵乘**：使用 `Mmad` 接口执行矩阵乘法
4. **L0C -> GM**：使用 `Fixpipe` 接口搬出结果

## Load2dV2 接口参数说明

`LoadData2DParamsV2` 结构体参数：

| 参数 | 含义 |
|------|------|
| mStartPosition | 源矩阵 M 轴方向的起始位置，单位为 16 个元素 |
| kStartPosition | 源矩阵 K 轴方向的起始位置，单位为 32B |
| mStep | 源矩阵 M 轴方向搬运长度，单位为 16 个元素 |
| kStep | 源矩阵 K 轴方向搬运长度，单位为 32B |
| srcStride | 源矩阵 K 方向前一个分形与后一个分形起始地址的间隔，单位为 512B |
| dstStride | 目标矩阵 K 方向前一个分形与后一个分形起始地址的间隔，单位为 512B |
| ifTranspose | 是否启用转置功能（true/false） |
| sid | 预留参数，配置为 0 |

**转置约束**：
- 当数据类型为 b8 时，mStep 必须是 2 的倍数
- 当数据类型为 b16 时，mStep 必须是 1 的倍数
- 当数据类型为 b32 时，kStep 必须是 2 的倍数

## 对齐要求

### L1 上对齐要求（Nz 格式）

| 输入类型 | A不转置[m,k] | A转置[k,m] | B不转置[k,n] | B转置[n,k] |
|----------|--------------|------------|--------------|------------|
| int8_t | m向16对齐, k向32对齐 | k向32对齐, m向32对齐 | k向32对齐, n向32对齐 | n向16对齐, k向32对齐 |
| half | m向16对齐, k向16对齐 | k向16对齐, m向16对齐 | k向16对齐, n向16对齐 | n向16对齐, k向16对齐 |
| float | m向16对齐, k向8对齐 | k向16对齐, m向16对齐 | k向16对齐, n向16对齐 | n向16对齐, k向8对齐 |

### L0 上对齐要求

| 输入类型 | L0A(Nz格式) | L0B(Zn格式) |
|----------|-------------|-------------|
| int8_t | m向16对齐, k向32对齐 | k向32对齐, n向32对齐(转置时) |
| half | m向16对齐, k向16对齐 | k向16对齐, n向16对齐(转置时) |
| float | m向32对齐, k向8对齐 | k向8对齐, n向16对齐(转置时) |

## 编译运行

在本样例根目录下执行如下步骤，编译并执行样例。

### 配置环境变量

请根据当前环境上 CANN 开发套件包的安装方式，选择对应配置环境变量的命令。

```bash
# 默认路径，root用户安装CANN软件包
source /usr/local/Ascend/cann/set_env.sh

# 默认路径，非root用户安装CANN软件包
source $HOME/Ascend/cann/set_env.sh

# 指定路径install_path，安装CANN软件包
source ${install_path}/cann/set_env.sh
```

### 样例执行

```bash
SCENARIO=1 M=40 K=70 N=50
mkdir -p build && cd build
cmake .. -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DSCENARIO_NUM=$SCENARIO -DM_SIZE=$M -DK_SIZE=$K -DN_SIZE=$N
make -j
python3 ../scripts/gen_data.py -scenarioNum=$SCENARIO -m=$M -k=$K -n=$N
./demo
python3 ../scripts/verify_result.py -scenarioNum=$SCENARIO output/output.bin output/golden.bin
```

使用 CPU调试 或 NPU仿真 模式时，添加 `-DCMAKE_ASC_RUN_MODE=cpu` 或 `-DCMAKE_ASC_RUN_MODE=sim` 参数即可。

示例如：
```bash
cmake .. -DCMAKE_ASC_RUN_MODE=cpu -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DSCENARIO_NUM=$SCENARIO -DM_SIZE=$M -DK_SIZE=$K -DN_SIZE=$N; make -j
cmake .. -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DSCENARIO_NUM=$SCENARIO -DM_SIZE=$M -DK_SIZE=$K -DN_SIZE=$N; make -j
```

> **注意：** 切换编译模式前需清理 cmake 缓存，可在 build 目录下执行 `rm CMakeCache.txt` 后重新 cmake。

### 参数说明

| 参数 | 说明 | 可选值 | 默认值 |
|------|------|--------|--------|
| CMAKE_ASC_RUN_MODE | 运行模式 | npu, cpu, sim | npu |
| CMAKE_ASC_ARCHITECTURES | NPU硬件架构 | dav-2201, dav-3510 | dav-2201 |
| SCENARIO_NUM | 场景编号 | 1-6 | 1 |
| M_SIZE | 矩阵 M 维度 | - | 40 |
| K_SIZE | 矩阵 K 维度 | - | 70 |
| N_SIZE | 矩阵 N 维度 | - | 50 |

### 验证结果

执行结果如下，说明精度对比成功：

```bash
test pass!
```

## 目录结构

```
load_data_l12l0_a5/
├── load_data_l12l0_a5.asc   # 核函数代码
├── data_utils.h             # 数据读写工具
├── CMakeLists.txt           # 编译配置
├── README.md                # 说明文档
├── scripts/
│   ├── gen_data.py          # 测试数据生成脚本
│   └── verify_result.py     # 结果验证脚本
└── img/                     # 图片目录
```

## 相关文档

- [Load2D API 文档](https://www.hiascend.com/document/detail/zh/CANNCommunityEdition/900beta2/API/ascendcopapi/atlasascendc_api_07_00169.html)
- [Mmad API 文档](https://www.hiascend.com/document/detail/zh/CANNCommunityEdition/900beta2/API/ascendcopapi/atlasascendc_api_07_0249.html)
- [Fixpipe API 文档](https://www.hiascend.com/document/detail/zh/CANNCommunityEdition/900beta2/API/ascendcopapi/atlasascendc_api_07_0251.html)