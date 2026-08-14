# 使用Tensor API实现直方图计算样例

## 概述

本样例演示Tensor API的寄存器直方图计算。一个`uint8_t`寄存器可以表示取值范围为0到255的输入，直方图接口将该范围分为`bin0`和`bin1`两个128-bin区间，并支持频次统计和累积统计两种模式。

样例使用同一组输入分别执行四种模板参数组合，展示目标寄存器初值如何参与直方图结果累加，以及模板参数如何在编译期选择底层计算接口。

## 支持的产品

- Ascend 950PR/Ascend 950DT

## 目录结构介绍

```text
histogram_compute_tensor_api
├── CMakeLists.txt
├── README.md
├── README_En.md
├── histogram_compute_tensor_api.asc
└── scripts
    ├── gen_data.py
    └── verify_result.py
```

## 样例描述

输入包含256个`uint8_t`元素，输出是4行、每行128个`uint16_t`元素：

| 输出行 | 统计范围和模式 |
| --- | --- |
| `frequency/bin0` | 统计取值0到127的频次 |
| `frequency/bin1` | 统计取值128到255的频次 |
| `accumulate/bin0` | 计算取值0到127的累积频次 |
| `accumulate/bin1` | 计算取值128到255的累积频次 |

目标寄存器的每个lane先初始化为3，四种直方图结果均在该初值基础上计算。内核将输入从GM搬到UB，加载为`reg_tensor<uint8_t>`，调用`histograms`后把`reg_tensor<uint16_t>`结果写回GM。校验脚本要求输出与NumPy计算结果逐元素完全一致。

## 编译运行

在样例根目录下执行以下步骤。

- 根据CANN开发套件的安装路径配置环境变量。

  ```bash
  # root用户默认安装路径
  source /usr/local/Ascend/cann/set_env.sh

  # 非root用户默认安装路径
  source $HOME/Ascend/cann/set_env.sh

  # 指定安装路径
  source ${install_path}/cann/set_env.sh
  ```

- 编译并执行样例。

  ```bash
  cmake -S . -B build -DCMAKE_ASC_ARCHITECTURES=dav-3510
  cmake --build build -j
  cd build
  python3 ../scripts/gen_data.py
  ./demo
  python3 ../scripts/verify_result.py output/output.bin output/golden.bin
  ```

校验成功时输出为：

```text
histogram_compute_tensor_api executed successfully.
test pass!
```
