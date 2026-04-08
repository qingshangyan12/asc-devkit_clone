# 基于MatmulLeakyRelu算子的CAmodel仿真样例

## 概述

本样例基于 MatmulLeakyRelu 算子，演示了通过 CAmodel 进行算子仿真，并结合 `msprof op simulator` 采集和解析仿真性能数据的方法。用户可基于仿真日志、精度校验结果与仿真产物，快速定位算子在指令流水、代码热点和访存路径上的问题，提升问题分析与调优效率。


## 支持的产品

- Ascend 950PR/Ascend 950DT
- Atlas A3 训练系列产品/Atlas A3 推理系列产品
- Atlas A2 训练系列产品/Atlas A2 推理系列产品

## 目录结构介绍

```
├── 07_simulator
│   ├── CMakeLists.txt          # 编译工程文件
│   ├── data_utils.h            # 数据读入写出函数
│   ├── matmul_leakyrelu.asc    # Ascend C算子实现与调用样例
│   ├── README.md               # 样例说明文档
│   └── scripts
│       ├── gen_data.py         # 输入数据和真值数据生成脚本
│       └── verify_result.py    # 真值比对脚本
```

## 样例描述

- 算子描述

  MatmulLeakyRelu算子通过将矩阵乘加（Matmul）与LeakyRelu激活函数计算实现，具体功能描述可参考[MatmulLeakyRelu算子详情](../../../00_introduction/03_fusion_operation/matmul_leakyrelu/README.md)章节。


## 编译运行

在本样例根目录下执行如下步骤，编译并执行样例。
- 配置环境变量

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

- 样例执行

```bash
mkdir -p build && cd build
cmake -DNPU_ARCH=dav-2201 ..
make -j
python3 ../scripts/gen_data.py
./demo
python3 ../scripts/verify_result.py output/output.bin output/golden.bin
```

说明：

- 本样例 CMake 仅生成 `demo_sim`，不生成常规 `demo`
- CMake 已启用 `-DASCENDC_TRACE_ON`，用于支持后续 `msprof op simulator` 解析

### 3. 运行成功判据

`./demo_sim` 日志中出现以下信息，表示 CAmodel 成功进入执行阶段：

```bash
[INFO] Top sim cfg file: ...
[INFO] Model cfg file: ...
>>>> Total no. of 1 chip(s) Model Init Success!
[info] [context_start] : ...
[info] [block_start] : ...
```

并且精度比对通过：

```bash
test pass!
```

## 基于msprof的仿真解析与问题分析

### 1. 采集仿真profiling数据

在 `build` 目录执行：

```bash
msprof op simulator --soc-version=Ascend910B1 ./demo_sim
```

若实际型号不是 `Ascend910B1`，请替换为真实 SoC。

### 2. 结果文件说明

命令完成后会生成 `OPPROF_*` 目录，重点关注：

- `OPPROF_*/simulator/trace.json`
- `OPPROF_*/simulator/core*.veccore*/`
- `OPPROF_*/simulator/core*.cubecore*/`
- `*_instr_exe.csv`（指令级明细）
- `*_code_exe.csv`（代码行级耗时）
- `visualize_data.bin`（MindStudio Insight 可视化文件）

### 3. 问题分析建议

- 流水并行度问题
  - 在 `trace.json` 中查看 Cube/Vector/MTE 时间线是否存在长串行段。
- 流水空泡问题
  - 在 `trace.json` 中定位空闲段，再结合 `*_instr_exe.csv` 查对应指令阶段。
- 热点问题
  - 用 `*_code_exe.csv` 找耗时 Top 代码行，再用 `*_instr_exe.csv` 下钻具体指令。
- 访存/资源冲突问题
  - 结合 `trace.json` 和指令明细判断搬运与计算重叠度、等待时间和冲突热点。

## 常见问题

- 报错：`Failed to find profiling data`
  - 先单独执行 `./demo_sim`，确认应用能完整跑完
  - 检查 `--soc-version` 是否匹配
  - 检查 `msopprof` 是否存在：
    ```bash
    ls -l $ASCEND_HOME_PATH/tools/msopprof/bin/msopprof
    ```

- 仿真耗时长
  - CAmodel 可能较慢，日志持续刷新时请等待执行结束再判断。
