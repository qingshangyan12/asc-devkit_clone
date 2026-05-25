# 基于SIMD&SIMT混编的时间戳打点功能实现样例

## 概述

本样例基于 Gather 和 Adds 融合计算，演示在 SIMD&SIMT 混合编程场景中使用 `clock()` 接口实现时间戳打点的方法。样例以 SIMT 方式实现离散访存的 Gather，以 SIMD 方式实现连续访存的 Adds，并在 SIMT Gather 阶段前后打点，用于记录执行周期。

## 支持的产品

- Ascend 950PR/Ascend 950DT

## 支持的CANN软件版本

- \>= CANN 9.0.0-beta.2

## 目录结构

```
├── 06_clock
│   ├── scripts
│   │   ├── gen_data.py             # 输入数据和真值数据生成脚本
│   │   └── verify_result.py        # 真值对比脚本
│   ├── CMakeLists.txt              # cmake编译文件
│   ├── data_utils.h                # 数据读入写出函数
│   ├── gather_and_adds.asc         # SIMD&SIMT混编与clock打点实现样例
│   └── README.md
```

## 样例描述

- 样例功能：

  本样例完成 Gather 和 Adds 融合计算，计算公式如下：

  ```text
  output[i] = input[index[i]] + 1
  ```

- 混编流程：

  `simt_gather` 使用 SIMT 编程方式，根据 `index[i]` 从 Global Memory 中读取离散位置的数据，并写入 Unified Buffer。`simd_adds` 使用 SIMD 编程方式，从 Unified Buffer 中读取连续数据，完成加 1 计算后写回 Unified Buffer。最后通过 `DataCopy` 将结果搬运到 Global Memory。

- clock打点说明：

  `simt_gather` 中在 Gather 计算前调用一次 `clock()` 记录开始时间戳，在 Gather 计算后再次调用 `clock()` 记录结束时间戳，并输出两次打点之间的周期差。为避免每个 SIMT 线程都打印一行日志，样例仅由第一个线程输出周期统计结果。

  ```text
  execute_cycle = clock_after_gather - clock_before_gather
  ```

## 编译运行

在本样例根目录下执行如下步骤，编译并执行样例。

- 配置环境变量

  请根据当前环境上 CANN 开发套件包的[安装方式](../../../../docs/quick_start.md#prepare&install)，选择对应配置环境变量的命令。

  默认路径，root 用户安装 CANN 软件包：

  ```bash
  source /usr/local/Ascend/cann/set_env.sh
  ```

  默认路径，非 root 用户安装 CANN 软件包：

  ```bash
  source $HOME/Ascend/cann/set_env.sh
  ```

  指定路径 `install_path`，安装 CANN 软件包：

  ```bash
  source ${install_path}/cann/set_env.sh
  ```

- 样例执行

  ```bash
  mkdir -p build && cd build
  cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..; make -j
  python3 ../scripts/gen_data.py
  ./demo
  python3 ../scripts/verify_result.py output/output.bin output/golden.bin
  ```

  使用 NPU 仿真模式时，添加 `-DCMAKE_ASC_RUN_MODE=sim` 参数即可。

  ```bash
  cmake -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..; make -j
  ```

- 编译选项说明

  | 选项 | 可选值 | 说明 |
  |------|--------|------|
  | `CMAKE_ASC_RUN_MODE` | `npu`（默认）、`sim` | 运行模式：NPU 运行、NPU 仿真 |
  | `CMAKE_ASC_ARCHITECTURES` | `dav-3510` | NPU 架构：`dav-3510` 对应 Ascend 950PR/Ascend 950DT |

- 执行结果

  执行结果如下，说明时间打点功能和精度对比成功。

  ```text
  simt_gather execute cycle : 22289
  test pass!
  ```
