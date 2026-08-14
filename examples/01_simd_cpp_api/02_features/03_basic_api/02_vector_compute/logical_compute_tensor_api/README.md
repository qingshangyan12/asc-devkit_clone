# 使用Tensor API实现逻辑计算样例

## 概述

本样例演示Tensor API的寄存器逻辑计算。输入数据加载为带mask的`reg_tensor<int32_t>`，通过C++运算符直接完成逻辑非、按位与、按位或、按位异或以及左右移位，并将8组结果写回Global Memory。

样例同时覆盖寄存器操作数和标量操作数，展示二元接口如何从`reg_tensor`操作数继承有效lane的mask。

## 支持的产品

- Ascend 950PR/Ascend 950DT

## 目录结构介绍

```text
logical_compute_tensor_api
├── CMakeLists.txt
├── README.md
├── README_En.md
├── logical_compute_tensor_api.asc
└── scripts
    ├── gen_data.py
    └── verify_result.py
```

## 样例描述

样例输入为64个`int32_t`元素，输出包含以下8行：

| 运算 | 操作数形式 |
| --- | --- |
| `!a` | 寄存器逻辑非，0映射为1，非0映射为0 |
| `a & b` | 寄存器与寄存器 |
| `a | 16` | 寄存器与右标量 |
| `7 ^ a` | 左标量与寄存器 |
| `a << shift` | 寄存器移位量 |
| `a << 2` | `int16_t`标量移位量 |
| `a >> shift` | 寄存器移位量 |
| `a >> 2` | `int16_t`标量移位量 |

内核完成GM到UB搬运、Tensor加载、寄存器逻辑计算和UB到GM回写。生成脚本使用NumPy构造相同运算的golden数据，校验脚本要求输出的形状和值逐元素完全一致。

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
logical_compute_tensor_api executed successfully.
test pass!
```
