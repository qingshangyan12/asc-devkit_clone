# reduce样例

## 概述

本样例使用Memory矢量计算C API[asc_repeat_reduce_sum](../../../../../docs/zh/api/SIMD-API/c_api/vector_compute/vector_reduce/asc_repeat_reduce_sum.md)对一个repeat内的float数据求和。

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

## 样例说明

输入为64个float元素，`asc_repeat_reduce_sum`将求和结果写入输出第0个元素。脚本生成确定性输入和单个float golden，主机侧按`1e-4`绝对误差验证结果。

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
