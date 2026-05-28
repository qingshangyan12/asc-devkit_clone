# 基于Add算子的CAmodel仿真样例

## 概述

本样例基于Add算子，聚焦 CAmodel 仿真与问题分析流程。用户可根据输出的性能数据，快速定位算子的软、硬件性能瓶颈，提升算子性能的分析效率。

## 支持的产品

- Ascend 950PR/Ascend 950DT

## 支持的CANN软件版本
- \>= CANN 9.0.0

## 目录结构介绍

```
├── 07_simulator
│   ├── CMakeLists.txt          // 编译工程文件
│   ├── data_utils.h            // 数据读入写出函数
│   ├── add.asc                 // SIMT实现add调用样例
│   └── scripts                 // 数据生成与结果校验脚本目录
│       ├── gen_data.py         // 输入数据和真值数据生成脚本
│       └── verify_result.py    // 真值比对脚本
```

## 算子描述
  Add算子实现了两个输入张量逐元素相加得到输出张量的功能，具体功能描述可参考[Add算子详情](../../00_introduction/01_add/README.md)章节。

  - 算子规格：
  <table>
  <tr><td rowspan="1" align="center">算子类型(OpType)</td><td colspan="4" align="center">add</td></tr>
  </tr>
  <tr><td rowspan="3" align="center">算子输入</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td align="center">x</td><td align="center">48 * 256</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td align="center">y</td><td align="center">48 * 256</td><td align="center">float</td><td align="center">ND</td></tr>
  </tr>
  </tr>
  <tr><td rowspan="1" align="center">算子输出</td><td align="center">z</td><td align="center">48 * 256</td><td align="center">float</td><td align="center">ND</td></tr>
  </tr>
  <tr><td rowspan="1" align="center">核函数名</td><td colspan="4" align="center">add_custom</td></tr>
  </table>

## 编译运行

在本样例根目录下执行如下步骤，编译并执行算子。
- 配置环境变量

  请根据当前环境上CANN开发套件包的[安装方式](../../../../docs/quick_start.md#prepare&install)，选择对应配置环境变量的命令。
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
  cmake -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..; make -j
  python3 ../scripts/gen_data.py
  msprof op simulator --soc-version=Ascend950PR ./demo    # 通过msprof op simulator执行算子调优
  python3 ../scripts/verify_result.py output/output.bin output/golden.bin
  ```
  - 编译选项说明
    | 选项 | 说明 |
    |------|------|
    | `CMAKE_ASC_RUN_MODE` | 指定为`sim`, 开启NPU仿真模式 |
    | `CMAKE_ASC_ARCHITECTURES` | 指定NPU架构版本号。本样例仅支持`dav-3510`（Ascend 950PR/Ascend 950DT） |

  执行结果如下，说明精度对比成功。
  ```bash
  test pass!
  ```

## 仿真调优

基于 `./demo` 可通过 msprof op simulator 进行仿真性能分析，生成可视化的指令流水图等信息，指令如下：
```bash
msprof op simulator --soc-version=<soc_version> ./demo
```
  > AI处理器的型号<soc_version>请通过如下方式获取：
  > - 在安装昇腾AI处理器的服务器执行`npu-smi info -t board -i <id> -c <chip_id>`命令进行查询，获取**Chip Name**和**NPU Name**信息，实际配置值为Chip Name_NPU Name。例如**Chip Name**取值为Ascendxxx，**NPU Name**取值为1234，实际配置值为Ascendxxx_1234。其中：
  >
  >   id：设备id，通过`npu-smi info -l`命令查出的NPU ID即为设备id
  >
  >   chip_id：芯片id，通过`npu-smi info -m`命令查出的Chip ID即为芯片id

命令完成后，会在当前目录下生成以 `OPPROF_{timestamp}_XXX` 命名的文件夹，产物结构如下：
```
OPPROF_{timestamp}_XXX/
├── dump                    # 原始性能数据，用户无需关注
└── simulator
  ├── core0.veccore0/      # 各核的仿真指令流水图文件
  ├── trace.json           # 仿真流水图和热点函数可视化呈现文件
  └── visualize_data.bin   # MindStudio Insight 呈现文件
```

执行后可通过以下方式查看指令流水图：
- **MindStudio Insight**：打开 `visualize_data.bin` 或 `trace.json` 进行可视化呈现
- **Chrome浏览器**：在地址栏输入 `chrome://tracing`，将 `trace.json` 文件拖到空白处打开


更多 msProf 工具使用方式请参考[MindStudio工具](https://www.hiascend.com/document/redirect/CannCommercialToolOpDev)算子调优（msProf）中的内容。