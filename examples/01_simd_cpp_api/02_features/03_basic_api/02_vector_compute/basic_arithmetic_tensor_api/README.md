# 使用Tensor API实现基础计算样例

## 概述

本样例演示如何使用Tensor API完成寄存器基础计算。样例将两组`float`输入和一组PReLU斜率从Global Memory搬运到Unified Buffer，加载为带mask的`reg_tensor<float>`，依次执行一元数学、激活和二元算术运算，再将16组结果写回Global Memory。

样例重点展示Tensor的`load/store`与返回值风格寄存器计算接口如何衔接。mask保存在`reg_tensor`中，并随每个计算结果继续传递。

## 支持的产品

- Ascend 950PR/Ascend 950DT

## 目录结构介绍

```text
basic_arithmetic_tensor_api
├── CMakeLists.txt
├── README.md
├── README_En.md
├── basic_arithmetic_tensor_api.asc
└── scripts
    ├── gen_data.py
    └── verify_result.py
```

## 样例描述

输入包含64个`float`元素。`compute`函数执行以下16组逐元素计算：`abs`、`exp`、`sqrt`、`log`、`log2`、`log10`、一元负号、`relu`、`prelu`、`leaky_relu`、加、减、乘、除、`max`和`min`。

计算流程如下：

1. 将`src_a`、`src_b`和`slope`从Global Memory搬运到Unified Buffer。
2. 从Tensor加载寄存器数据，并使用全有效mask构造`reg_tensor<float>`。
3. 调用基础计算接口，将每种接口的结果写入输出Tensor的一行。
4. 将16行输出搬回Global Memory，并与NumPy生成的golden数据比较。

数学函数使用`rtol=5e-3`、`atol=5e-3`校验，其余结果使用`rtol=1e-4`、`atol=1e-4`校验。

## 编译运行

在样例根目录下执行以下步骤。

- 根据CANN开发套件的安装路径配置环境变量。

  root用户默认安装路径：

  ```bash
  source /usr/local/Ascend/cann/set_env.sh
  ```

  非root用户默认安装路径：

  ```bash
  source $HOME/Ascend/cann/set_env.sh
  ```

  指定安装路径：

  ```bash
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

校验成功时输出包含：

```text
basic_arithmetic_tensor_api executed successfully.
test pass! max absolute error: ...
```
