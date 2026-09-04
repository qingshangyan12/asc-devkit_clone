# element_wise_arithmetic样例

## 概述

本样例使用Memory矢量计算C API[asc_leakyrelu](../../../../../docs/zh/api/SIMD-API/c_api/vector_compute/vector_fused/asc_leakyrelu.md)实现Leaky ReLU：当`x > 0`时`y = x`，否则`y = 0.01 * x`。

## 支持产品及CANN版本

| 产品 | CANN软件版本 |
| --- | --- |
| Atlas A2训练/推理系列、Atlas A3训练/推理系列 | >= CANN 9.2.0 |

## 目录结构

```text
element_wise_arithmetic/
├── scripts/gen_data.py
├── CMakeLists.txt
├── data_utils.h
├── element_wise_arithmetic.asc
├── README.md
└── README_en.md
```

## 样例说明

输入和输出均为512个half元素。数据生成脚本提供固定的正数、负数和零输入，程序逐元素比对输出与golden数据。

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
mkdir -p build && cd build;                                                                       # 创建并进入build目录
cmake -DCMAKE_ASC_ARCHITECTURES=dav-2201 ..;make -j; # 编译工程（默认NPU模式）
python3 ../scripts/gen_data.py                                                                    # 生成测试输入数据和真值数据
./demo                                                                                           # 执行编译生成的可执行程序，执行样例
```

- 编译选项说明

| 选项 | 可选值 | 说明 |
| --- | --- | --- |
| `CMAKE_ASC_ARCHITECTURES` | `dav-2201` | NPU架构：Atlas A2训练/推理系列、Atlas A3训练/推理系列 |

- 执行结果

输出与golden数据比较通过时，打印如下信息：

```text
test pass!
```
