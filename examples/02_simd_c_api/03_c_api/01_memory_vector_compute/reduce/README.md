# reduce样例

## 概述

本样例基于Memory矢量计算接口[asc_repeat_reduce_sum](../../../../../docs/zh/api/SIMD-API/c_api/vector_compute/vector_reduce/asc_repeat_reduce_sum.md)和[asc_datablock_reduce_sum](../../../../../docs/zh/api/SIMD-API/c_api/vector_compute/vector_reduce/asc_datablock_reduce_sum.md)实现float数据的求和归约。

本样例支持两种求和归约场景，通过CMake编译参数`SCENARIO_NUM`选择。

| `SCENARIO_NUM` | 求和归约场景 |
| --- | --- |
| `1` | Repeat归约 |
| `2` | DataBlock归约 |

## 支持产品及CANN版本

| 产品 | CANN软件版本 |
| --- | --- |
| Atlas A2训练/推理系列、Atlas A3训练/推理系列 | >= CANN 9.2.0 |

## 目录结构

```text
reduce/
├── scripts/gen_data.py
├── CMakeLists.txt
├── data_utils.h
├── reduce.asc
├── README.md
└── README_en.md
```

## 样例描述

本样例将64个float输入元素从GM搬入UB后进行求和归约，再将结果搬回GM。主机侧使用内核调用符`<<<>>>`调用`reduce_kernel`，按`1e-4`绝对误差比对全部有效输出与`output/golden.bin`。

**场景1：Repeat归约**

- 样例功能：对一个repeat内的64个float元素求和。
- 样例规格：

  | 项目 | 规格 |
  | --- | --- |
  | 样例类型（OpType） | AIV样例 |
  | 样例输入 | `x`，[1, 64]，float |
  | 样例输出 | `y`，[1, 1]，float |
  | 核函数名 | `reduce_kernel` |

- 样例实现：
  - 搬入：调用`asc_copy_gm2ub_align`将64个float元素从GM搬入UB。
  - 计算：调用`asc_repeat_reduce_sum`对一个repeat内的64个元素求和。
  - 搬出：调用`asc_copy_ub2gm_align`将1个float结果从UB搬回GM。

**场景2：DataBlock归约**

- 样例功能：分别对8个32BDataBlock内的8个float元素求和。
- 样例规格：

  | 项目 | 规格 |
  | --- | --- |
  | 样例类型（OpType） | AIV样例 |
  | 样例输入 | `x`，[1, 64]，float |
  | 样例输出 | `y`，[1, 8]，float |
  | 核函数名 | `reduce_kernel` |

- 样例实现：
  - 搬入：调用`asc_copy_gm2ub_align`将64个float元素从GM搬入UB。
  - 计算：调用`asc_datablock_reduce_sum`对每个32BDataBlock内的8个float元素求和。
  - 搬出：调用`asc_copy_ub2gm_align`将8个float结果从UB搬回GM。

数据生成脚本为各场景生成确定性float输入和对应golden数据。

## 编译运行

在本样例根目录执行以下步骤。

- 配置环境变量

  请根据CANN开发套件包的[安装方式](../../../../../docs/zh/quick_start.md#prepare&install)配置环境变量。

```bash
source ${install_path}/cann/set_env.sh
```

  > **说明：** `${install_path}`为CANN包安装目录；root用户默认为`/usr/local/Ascend`，非root用户默认为`${HOME}/Ascend`。

- NPU运行

```bash
SCENARIO_NUM=1                                                                                  # 执行场景1
mkdir -p build && cd build;                                                                       # 创建并进入build目录
cmake -DSCENARIO_NUM=$SCENARIO_NUM -DCMAKE_ASC_ARCHITECTURES=dav-2201 ..;make -j; # 编译工程（默认NPU模式）
python3 ../scripts/gen_data.py -scenario_num=$SCENARIO_NUM                                       # 生成测试输入数据和真值数据
./demo                                                                                           # 执行编译生成的可执行程序，执行样例
```

- 编译选项说明

| 选项 | 可选值 | 说明 |
| --- | --- | --- |
| `CMAKE_ASC_ARCHITECTURES` | `dav-2201` | NPU架构：Atlas A2训练/推理系列、Atlas A3训练/推理系列 |
| `SCENARIO_NUM` | `1`、`2` | 样例场景：Repeat归约、DataBlock归约 |

- 执行结果

输出与golden数据比较通过时，打印如下信息：

```text
test pass!
```
