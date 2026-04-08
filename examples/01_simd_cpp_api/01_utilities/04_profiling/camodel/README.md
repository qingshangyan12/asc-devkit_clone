# 基于MatmulLeakyRelu算子的CAmodel仿真样例

## 概述

本样例聚焦两件事：

1. 使用 CAmodel 完成算子仿真执行（`demo_sim`）。
2. 使用 `msprof op simulator` 解析仿真数据并进行问题分析。

样例不提供常规上板运行路径，数据生成与精度校验均内置在 `matmul_leakyrelu.asc` 中。

## 支持的产品

- Atlas A3 训练系列产品/Atlas A3 推理系列产品
- Atlas A2 训练系列产品/Atlas A2 推理系列产品

## 目录结构介绍

```
├── camodel
│   ├── CMakeLists.txt          # 编译工程文件（仅生成 demo_sim，已开启 DASCENDC_TRACE_ON 和 -g）
│   ├── matmul_leakyrelu.asc    # Ascend C算子实现 + 数据生成 + 精度校验
│   └── README.md               # 样例说明文档
```

## CAmodel仿真执行

### 1. 环境准备

按实际安装方式配置 CANN 环境变量：

```bash
source /usr/local/Ascend/cann/set_env.sh
```

```bash
source $HOME/Ascend/cann/set_env.sh
```

```bash
source ${install_path}/cann/set_env.sh
```

### 2. 编译与运行（仅 CAmodel）

```bash
mkdir -p build && cd build
cmake -DNPU_ARCH=dav-2201 ..
make -j
./demo_sim
```

### 3. 运行成功判据

日志中出现以下信息可判断仿真执行正常进入：

```bash
[INFO] Top sim cfg file: ...
[INFO] Model cfg file: ...
>>>> Total no. of 1 chip(s) Model Init Success!
[info] [context_start] : ...
[info] [block_start] : ...
```

执行完成后出现：

```bash
[Success] Case accuracy is verification passed.
```

表示 CAmodel 仿真执行和精度校验均通过。

## msprof解析与问题分析

### 1. 采集仿真 profiling 数据

在 `build` 目录执行：

```bash
msprof op simulator --soc-version=Ascend910B1 ./demo_sim
```

若型号不是 `Ascend910B1`，请按实际 SoC 修改 `--soc-version`。

### 2. 查看采集结果

命令执行后会生成 `OPPROF_*` 目录。重点查看：

- `OPPROF_*/simulator/trace.json`
- `OPPROF_*/simulator/core*.veccore*/`
- `OPPROF_*/simulator/core*.cubecore*/`
- `*_instr_exe.csv`
- `*_code_exe.csv`
- `visualize_data.bin`

可用 MindStudio Insight 打开 `visualize_data.bin` 做可视化分析。

### 3. 问题分析建议

- 流水并行度不足
  - 看 `trace.json` 中各执行单元时间线是否存在明显串行段。
  - 结合 `*_instr_exe.csv` 定位耗时长的关键指令。

- 流水空泡较多
  - 看 `trace.json` 中空闲区段与前后指令关系。
  - 对照 `*_code_exe.csv` 定位对应代码段，检查数据搬运与计算是否失衡。

- 热点集中在少数代码行
  - 用 `*_code_exe.csv` 找 Top 耗时代码行。
  - 用 `*_instr_exe.csv` 细化到具体指令类型（Cube/Vector/MTE）。

- 访存瓶颈或资源冲突
  - 在 `trace.json` 中观察搬运与计算重叠情况。
  - 结合 `*_instr_exe.csv` 判断是否长期被搬运或等待阻塞。
