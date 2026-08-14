# 使用Tensor API实现复合计算样例

## 概述

本样例演示如何使用Tensor API完成可由单条或一组底层指令表达的复合寄存器计算。相较于把乘、加、绝对值、指数和类型转换拆成多个调用，复合接口可以直接表达完整计算意图，并减少用户维护中间寄存器的工作。

样例将两组`float`向量加载为`reg_tensor<float>`，调用5个复合计算接口，并将每个接口的结果写入独立输出行。

## 支持的产品

- Ascend 950PR/Ascend 950DT

## 目录结构介绍

```text
composite_compute_tensor_api
├── CMakeLists.txt
├── README.md
├── README_En.md
├── composite_compute_tensor_api.asc
└── scripts
    ├── gen_data.py
    └── verify_result.py
```

## 样例描述

输入包含两组各64个`float`元素，样例计算以下5行结果：

| 接口 | 样例中的计算 |
| --- | --- |
| `axpy` | `src_b + 0.25 * src_a` |
| `absdiff` | `abs(src_a - src_b)` |
| `exp_sub` | `exp(src_a - src_b)` |
| `fma` | `src_a * src_b + src_b` |
| `muls_cast` | `float`乘1.5后转换为`half`，再转回`float`用于统一保存和校验 |

内核先完成GM到UB的数据搬运，再在`compute`中加载寄存器、调用复合接口并存储结果，最后将输出搬回GM。`gen_data.py`使用NumPy生成输入和golden数据，`verify_result.py`完成逐元素精度比较。

`exp_sub`使用`rtol=5e-3`、`atol=5e-3`校验，其余结果使用`rtol=1e-4`、`atol=1e-4`校验。

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

校验成功时输出包含：

```text
composite_compute_tensor_api executed successfully.
test pass! max absolute error: ...
```
