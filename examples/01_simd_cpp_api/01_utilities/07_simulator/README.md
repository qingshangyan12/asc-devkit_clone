# 基于Add的CAmodel仿真样例

## 概述

本样例基于 Add，展示 Ascend C 程序在 CAmodel 仿真模式下的编译、运行、结果校验与性能数据采集流程。用户可通过 `msprof op simulator` 获取仿真性能数据，用于分析向量计算程序的执行情况。

## 支持的产品

- Ascend 950PR/Ascend 950DT
- Atlas A3 训练系列产品/Atlas A3 推理系列产品
- Atlas A2 训练系列产品/Atlas A2 推理系列产品

## 目录结构介绍

```
├── 07_simulator
│   ├── add.asc                 // Add实现与调用样例
│   ├── CMakeLists.txt          // 编译工程文件
│   ├── data_utils.h            // 数据读入写出函数
│   └── scripts                 // 数据生成与结果校验脚本目录
│       ├── gen_data.py         // 输入数据和真值数据生成脚本
│       └── verify_result.py    // 真值比对脚本
```

## 样例描述

- 样例功能

  Add 对两个 `float16` 输入 Tensor 逐元素相加，并输出 `float16` 结果。本样例中输入 shape 为 `(8, 2048)`，计算公式如下：

  $$
  z = x + y
  $$

- 样例规格

  <table border="2">
  <caption>表1：AddCustom样例规格描述</caption>
  <tr><td rowspan="1" align="center">样例类型(OpType)</td><td colspan="4" align="center">AddCustom</td></tr>
  <tr><td align="center"></td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td align="center">样例输入</td><td align="center">x</td><td align="center">[8, 2048]</td><td align="center">half</td><td align="center">ND</td></tr>
  <tr><td align="center">样例输入</td><td align="center">y</td><td align="center">[8, 2048]</td><td align="center">half</td><td align="center">ND</td></tr>
  <tr><td align="center">样例输出</td><td align="center">z</td><td align="center">[8, 2048]</td><td align="center">half</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">核函数名</td><td colspan="4" align="center">add_custom</td></tr>
  </table>

## 编译运行

在本样例根目录下执行如下步骤，编译并执行程序。

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
  cmake -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-2201 ..; make -j
  python3 ../scripts/gen_data.py
  msprof op simulator --soc-version=Ascend910B1 ./demo
  python3 ../scripts/verify_result.py output/output.bin output/golden.bin
  ```

  请根据实际测试的 NPU 硬件架构选择对应的 `CMAKE_ASC_ARCHITECTURES` 参数。

  | 选项 | 说明 |
  |------|------|
  | `CMAKE_ASC_RUN_MODE` | 指定为 `sim`，开启 NPU 仿真模式 |
  | `CMAKE_ASC_ARCHITECTURES` | 指定 NPU 架构版本号。`dav-2201` 对应 Atlas A2 训练系列产品/Atlas A2 推理系列产品和 Atlas A3 训练系列产品/Atlas A3 推理系列产品，`dav-3510` 对应 Ascend 950PR/Ascend 950DT |

  执行结果如下，说明精度比对成功。

  ```bash
  test pass!
  ```

## 仿真调优

基于 `./demo` 可通过 `msprof op simulator` 进行仿真性能分析，生成可视化的指令流水图等信息，指令如下：

```bash
msprof op simulator --soc-version=<soc_version> ./demo
```

> AI 处理器的型号 `<soc_version>` 请通过如下方式获取：
>
> - 针对如下产品型号：在安装昇腾 AI 处理器的服务器执行 `npu-smi info` 命令进行查询，获取 **Name** 信息。实际配置值为 AscendName，例如 **Name** 取值为 xxxyy，实际配置值为 Ascendxxxyy。
>   - Atlas A2 训练系列产品 / Atlas A2 推理系列产品
>
> - 针对如下产品型号，在安装昇腾 AI 处理器的服务器执行 `npu-smi info -t board -i <id> -c <chip_id>` 命令进行查询，获取 **Chip Name** 和 **NPU Name** 信息，实际配置值为 Chip Name_NPU Name。例如 **Chip Name** 取值为 Ascendxxx，**NPU Name** 取值为 1234，实际配置值为 Ascendxxx_1234。其中：
>   - Ascend 950PR/Ascend 950DT
>   - Atlas A3 训练系列产品 / Atlas A3 推理系列产品
>
> `id` 为设备 ID，通过 `npu-smi info -l` 命令查出的 NPU ID 即为设备 ID；`chip_id` 为芯片 ID，通过 `npu-smi info -m` 命令查出的 Chip ID 即为芯片 ID。

命令完成后，会在当前目录下生成以 `OPPROF_{timestamp}_XXX` 命名的文件夹，产物结构如下：

```
OPPROF_{timestamp}_XXX/
├── dump                    // 原始性能数据，用户无需关注
└── simulator
    ├── core*.veccore*/     // 各向量核的仿真指令流水图文件
    ├── trace.json          // 仿真流水图和热点函数可视化呈现文件
    └── visualize_data.bin  // MindStudio Insight呈现文件
```

执行后可通过以下方式查看指令流水图：

- **MindStudio Insight**：打开 `visualize_data.bin` 或 `trace.json` 进行可视化呈现
- **Chrome 浏览器**：在地址栏输入 `chrome://tracing`，将 `trace.json` 文件拖到空白处打开

更多 msProf 工具使用方式请参考[MindStudio工具](https://www.hiascend.com/document/redirect/CannCommercialToolOpDev)调优（msProf）中的内容。
