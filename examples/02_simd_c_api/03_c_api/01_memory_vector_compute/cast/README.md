# cast样例

## 概述

本样例基于Memory矢量计算C API[asc_half2int4](../../../../../docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_half2int4.md)和[asc_half2int32](../../../../../docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_half2int32.md)，演示half数据向上取整转换为int4或int32。

## 支持产品及CANN版本

| 产品 | CANN软件版本 |
| --- | --- |
| Atlas A2训练/推理系列、Atlas A3训练/推理系列 | >= CANN 9.2.0 |

## 目录结构

```text
cast/
├── scripts/gen_data.py       # 输入和golden数据生成脚本
├── CMakeLists.txt            # 编译工程文件
├── data_utils.h              # 二进制数据读取工具
├── cast.asc                  # C API内核和主机调用代码
├── README.md
└── README_en.md
```

## 场景说明

| `SCENARIO_NUM` | 功能 | 输入 | 输出 |
| --- | --- | --- | --- |
| 0 | half转打包int4，CEIL舍入并饱和到[-8, 7] | 512个half | 256字节 |
| 1 | half转int32，CEIL舍入 | 512个half | 512个int32 |

脚本生成固定的边界输入和对应golden数据。程序读取`input/input_x.bin`、执行内核并逐字节比对`output/golden.bin`。

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
SCENARIO_NUM=0                                                                                  # 执行场景0
mkdir -p build && cd build;                                                                       # 创建并进入build目录
cmake -DSCENARIO_NUM=$SCENARIO_NUM -DCMAKE_ASC_ARCHITECTURES=dav-2201 ..;make -j; # 编译工程（默认NPU模式）
python3 ../scripts/gen_data.py -scenario_num=$SCENARIO_NUM                                       # 生成测试输入数据和真值数据
./demo                                                                                           # 执行编译生成的可执行程序，执行样例
```

- 编译选项说明

| 选项 | 可选值 | 说明 |
| --- | --- | --- |
| `CMAKE_ASC_ARCHITECTURES` | `dav-2201` | NPU架构：Atlas A2训练/推理系列、Atlas A3训练/推理系列 |
| `SCENARIO_NUM` | `0`、`1` | 样例场景：half转打包int4、half转int32 |

- 执行结果

输出与golden数据比较通过时，打印如下信息：

```text
test pass!
```
