# fixpipe_l0c2ub样例

## 概述

本样例介绍如何使用Fixpipe将矩阵乘的结果从L0C（CO1 Buffer）搬出到UB（Unified Buffer），支持多种输出格式（NZ、ND）以及双目标模式（按M维度或N维度拆分）等功能。这些接口用于将L0C中的矩阵乘计算结果高效地传输到统一缓存区，并支持各种数据格式转换和拆分能力。

## 支持的产品

- Ascend 950PR/Ascend 950DT

## 目录结构介绍

```
├── fixpipe_l0c2ub
│   ├── scripts
│   │   ├── gen_data.py                // 输入数据和真值数据生成脚本
│   │   └── verify_result.py           // 验证输出数据和真值数据是否一致的验证脚本
│   ├── CMakeLists.txt                 // 编译工程文件
│   ├── data_utils.h                   // 数据读入写出函数
│   └── fixpipe_l0c2ub.asc             // Ascend C样例实现 & 调用样例
```

## 场景详细说明

本样例通过编译参数 `SCENARIO_NUM` 选择不同的输出场景，所有场景基于相同的矩阵乘规格：[M, N, K] = [128, 256, 128]，核函数名为 `fixpipe_l0c2ub`。

**场景1：输出格式NZ，输出数据类型float**
- 输入：A [128, 128] half类型，ND格式；B [128, 256] half类型，ND格式
- 输出：C [128, 256] float类型，NZ格式
- 实现：使用 `Fixpipe<outputType, l0cType, CFG_NZ_UB>` 将数据从CO1搬出到UB，输出为NZ格式
- 说明：CO1数据为NZ格式直接输出到UB的NZ格式，数据保持原格式不变

**场景2：输出格式ND，输出数据类型float**
- 输入：A [128, 128] half类型，ND格式；B [128, 256] half类型，ND格式
- 输出：C [128, 256] float类型，ND格式
- 实现：使用 `Fixpipe<outputType, l0cType, CFG_ROW_MAJOR_UB>` 指定ROW_MAJOR格式转换
- 说明：将CO1中的NZ格式数据转换为ND格式输出到UB

**场景3：输出格式ND，输出数据类型float，使能双目标模式，按M维度拆分**
- 输入：A [128, 128] half类型，ND格式；B [128, 256] half类型，ND格式
- 输出：C [64, 256] float类型，ND格式（双目标模式，按M维度拆分，每个目标输出64行）
- 实现：设置 `fixpipeParams.dualDstCtl = 0b01`，按M维度拆分，M必须为2的倍数
- 说明：使用双目标模式将数据拆分输出到UB，按M维度拆分的两个核各处理一半数据
<p align="center">
  <img src="img/fixpipe_l0c2ub_split_m.png" width="500">
</p>

**场景4：输出格式ND，输出数据类型float，使能双目标模式，按N维度拆分**
- 输入：A [128, 128] half类型，ND格式；B [128, 256] half类型，ND格式
- 输出：C [128, 128] float类型，ND格式（双目标模式，按N维度拆分，每个目标输出128列）
- 实现：设置 `fixpipeParams.dualDstCtl = 0b10`，按N维度拆分，N必须为32的倍数
- 说明：使用双目标模式将数据拆分输出到UB，按N维度拆分的两个核各处理一半数据
<p align="center">
  <img src="img/fixpipe_l0c2ub_split_n.png" width="500">
</p>

| scenarioNum | L0C数据类型 | 输出数据类型 | 输出格式 | 双目标模式 | 拆分维度 |
|:-----------:|:----------:|:-----------:|:--------:|:----------:|:--------:|
| 1 | float | float | NZ | 否 | - |
| 2 | float | float | ND | 否 | - |
| 3 | float | float | ND | 是 | M维度 |
| 4 | float | float | ND | 是 | N维度 |

## 编译运行

在本样例根目录下执行如下步骤，编译并执行样例。
- 配置环境变量
  请根据当前环境上CANN开发套件包的[安装方式](../../../../../../docs/quick_start.md#prepare&install)，选择对应配置环境变量的命令。
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
  SCENARIO_NUM=1
  mkdir -p build && cd build;      # 创建并进入build目录
  cmake -DSCENARIO_NUM=$SCENARIO_NUM -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..;make -j;    # 编译工程，默认npu模式
  python3 ../scripts/gen_data.py -scenarioNum=$SCENARIO_NUM   # 生成测试输入数据
  ./demo                           # 执行编译生成的可执行程序，执行样例
  python3 ../scripts/verify_result.py output/output.bin ./output/golden.bin  # 验证输出结果是否正确
  ```
    使用 NPU仿真 模式时，添加 `-DCMAKE_ASC_RUN_MODE=sim` 参数即可。
  
  示例如下：
  ```bash
  cmake -DSCENARIO_NUM=$SCENARIO_NUM -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..;make -j; # NPU仿真模式
  ```

  > **注意：** 切换编译模式前需清理 cmake 缓存，可在 build 目录下执行 `rm CMakeCache.txt` 后重新 cmake。

- 编译选项说明

  | 参数 | 说明 | 可选值 | 默认值 |
  |------|------|---------|--------|
  | CMAKE_ASC_RUN_MODE | 运行模式 | npu, sim | npu |
  | CMAKE_ASC_ARCHITECTURES | NPU硬件架构 | dav-3510 | dav-3510 |
  | SCENARIO_NUM | 场景编号 | 1-4 | 1 |

  执行结果如下，说明精度对比成功。
  ```bash
  test pass!
  ```
