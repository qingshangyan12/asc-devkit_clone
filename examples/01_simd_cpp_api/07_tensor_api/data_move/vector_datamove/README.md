# Vector Datamove样例

## 概述

本样例介绍 Tensor API 在 AIV 侧的四个 Vector 搬运通路：`GM->UB`、`UB->GM`、`UB->UB` 和
`UB->L1`，并演示 `GM->UB` 搬运的左右常量填充。场景1至4使用 `[32, 32]` 的 `int8_t` Tensor；场景5将 `[32, 24]` 输入填充为 `[32, 32]` 输出。每个场景均做完整输出校验。

## 支持的产品

- Ascend 950PR/Ascend 950DT

## 样例场景

| SCENARIO_NUM | 重点通路 | Layout | 主要写法 |
| --- | --- | --- | --- |
| 1 | GM->UB | ND | `copy(dst, src)` 自动推导通路 |
| 2 | UB->GM | DN | `make_copy(..., trait)` 后调用 `atom.call(dst, src)` |
| 3 | UB->UB | NZ | 显式 atom 与坐标/shape 区域搬运重载 |
| 4 | UB->L1 | ZN | 显式 `copy(atom, dst, src)` 并验证 AIV 搬运可完成执行 |
| 5 | GM->UB | ND | 左填充3个、右填充5个元素，并使用 `asc_set_copy_pad_val` 配置填充值，并通过c_api指令`asc_set_copy_pad_val(PADDING_VALUE)` 将 padding 元素配置为 `-9` |

本示例仅展示vector侧的矢量搬运，五个场景的 Kernel 均声明为 `__global__ __vector__`。场景中的辅助搬运仅用于准备输入或将结果回传到 GM。由于 dav-3510 的 `L1->UB` 通路只在 AIC 生效，场景4无法在纯 AIV Kernel 中回读 L1；该场景验证 `UB->L1` 能正确编译、下发并完成同步，然后从源 UB 回传数据作为 Kernel 完成性校验。

## 目录结构

```text
├── CMakeLists.txt
├── README.md
├── README_En.md
├── vector_datamove.asc
├── data_utils.h
└── scripts
    └── gen_data.py
```

## 编译运行

在本样例根目录下执行如下步骤，编译并执行算子。

- 配置环境变量  
  请根据当前环境上CANN开发套件包的[安装方式](https://gitcode.com/cann/asc-devkit/blob/master/docs/zh/quick_start.md)，配置环境变量。
  ```bash
  source ${install_path}/cann/set_env.sh
  ```

- 样例执行

  在本样例目录下执行如下命令。
  ```bash
  mkdir -p build && cd build;      # 创建并进入build目录
  cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..;make -j;    # 编译工程，默认npu模式
  python3 ../scripts/gen_data.py   # 生成测试输入数据
  ./demo                           # 执行编译生成的可执行程序，执行样例
  ```

  使用 CPU调试 或 NPU仿真 模式时，添加 `-DCMAKE_ASC_RUN_MODE=cpu` 或 `-DCMAKE_ASC_RUN_MODE=sim` 参数即可。
  
  示例如下：

  ```bash
  cmake -DCMAKE_ASC_RUN_MODE=cpu -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..;make -j; # cpu调试模式
  cmake -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..;make -j; # NPU仿真模式
  ```

  > **注意：** 切换编译模式前需清理 cmake 缓存，可在 build 目录下执行 `rm CMakeCache.txt` 后重新 cmake。

- 编译选项说明

  | 选项 | 可选值 | 说明 |
  |------|--------|------|
  | `CMAKE_ASC_RUN_MODE` | `npu`（默认）、`cpu`、`sim` | 运行模式：NPU 运行、CPU调试、NPU仿真 |
  | `CMAKE_ASC_ARCHITECTURES` | `dav-3510` | NPU 架构：dav-3510 对应 Ascend 950PR/Ascend 950DT 及 Atlas A3 训练系列产品/Atlas A3 推理系列产品 |

- 执行结果

  执行结果如下，说明精度对比成功。

  ```bash
  test pass!

- 样例执行

  在本样例目录下执行如下命令。

  ```bash
  SCENARIO_NUM=1                                                                 # 执行场景1
  mkdir -p build && cd build;                                                    # 创建并进入build目录
  cmake -DSCENARIO_NUM=$SCENARIO_NUM -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..;make -j;  # 编译工程（默认npu模式）
  python3 ../scripts/gen_data.py -scenarioNum=$SCENARIO_NUM                      # 生成测试输入数据
  ./demo                                                                         # 执行编译生成的可执行程序，执行样例
  ```

  使用NPU仿真模式时，添加`-DCMAKE_ASC_RUN_MODE=sim` 参数即可。

  示例如下：

  ```bash
  cmake -DSCENARIO_NUM=4 -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..;make -j; # NPU仿真模式
  ```

  > **注意：** 切换编译模式前需清理 cmake 缓存，可在 build 目录下执行 `rm CMakeCache.txt` 后重新 cmake。

执行结果：

```text
test pass!
```