# compare样例

## 概述

本样例使用Memory矢量计算C API[asc_lt](../../../../../docs/zh/api/SIMD-API/c_api/vector_compute/vector_compare/asc_lt.md)、[asc_lt_scalar](../../../../../docs/zh/api/SIMD-API/c_api/vector_compute/vector_compare/asc_lt_scalar.md)、[asc_gt_scalar](../../../../../docs/zh/api/SIMD-API/c_api/vector_compute/vector_compare/asc_gt_scalar.md)和`asc_get_cmp_mask`，演示比较结果的小端bit压缩格式。

## 支持产品及CANN版本

| 产品 | CANN软件版本 |
| --- | --- |
| Atlas A2训练/推理系列、Atlas A3训练/推理系列 | >= CANN 9.2.0 |

## 目录结构

```text
compare/
├── scripts/gen_data.py
├── CMakeLists.txt
├── data_utils.h
├── compare.asc
├── README.md
└── README_en.md
```

## 场景说明

输入为64个float元素，输出的前8个字节按小端bit保存比较结果。

| `SCENARIO_NUM` | 比较操作 |
| --- | --- |
| 1 | `x[i] < y[i]` |
| 2 | `x[i] < y[i]`，结果经CmpMask读取 |
| 3 | `x[i] < y[0]` |
| 4 | `x[i] > y[0]` |

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

程序会对关键ACL调用失败立即返回非零，不会把未执行内核的输出当作通过结果。

- 编译选项说明

| 选项 | 可选值 | 说明 |
| --- | --- | --- |
| `CMAKE_ASC_ARCHITECTURES` | `dav-2201` | NPU架构：Atlas A2训练/推理系列、Atlas A3训练/推理系列 |
| `SCENARIO_NUM` | `1`、`2`、`3`、`4` | 样例场景：向量比较、CmpMask读取、向量标量比较、向量标量反向比较 |

- 执行结果

输出与golden数据比较通过时，打印如下信息：

```text
test pass!
```
