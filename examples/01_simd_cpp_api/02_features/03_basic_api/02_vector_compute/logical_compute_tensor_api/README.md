# 基于Tensor API实现的逻辑计算样例

## 概述

本样例基于Tensor API编程方式实现寄存器逻辑计算。样例将两组`int32_t`输入和一组移位量从Global Memory（GM）搬运到Unified Buffer（UB），加载为带mask的`reg_tensor<int32_t>`，通过C++运算符完成逻辑非、按位与、按位或、按位异或以及左右移位，并将8组结果写回GM。

样例同时使用寄存器与寄存器、寄存器与右标量、左标量与寄存器三种操作数组合，展示Tensor API如何以C++运算符形式表达逐lane逻辑计算。有效lane的mask由参与计算的`reg_tensor`提供，并随结果继续传递。

## 本样例支持的产品及CANN软件版本

| 产品 | CANN软件版本 |
| :--- | :----------- |
| Ascend 950PR/Ascend 950DT | CANN master（包含本样例依赖的experimental Tensor API接口） |

## 目录结构介绍

```text
logical_compute_tensor_api
├── scripts
│   ├── gen_data.py                         // 输入数据和真值数据生成脚本
│   └── verify_result.py                    // 输出结果校验脚本
├── CMakeLists.txt                          // 编译工程文件
├── logical_compute_tensor_api.asc          // Tensor API逻辑计算实现及调用代码
├── README.md                               // 中文样例说明文档
└── README_En.md                            // 英文样例说明文档
```

样例同时复用上一级目录中的`reg_compute_example_utils.h`，用于完成ACL运行时初始化、Device内存申请、输入搬运和输出保存。

## 样例描述

- 样例功能：

  本样例对64个`int32_t`元素执行8组逐lane逻辑计算，整体处理流程如下：

  1. 在Host侧读取`src_a.bin`、`src_b.bin`和`shifts.bin`，并将输入复制到Device。
  2. 在Kernel中构造GM Tensor和UB Tensor，通过`CopyGM2UB`完成输入搬运。
  3. 使用全有效mask加载三个输入Tensor，得到`reg_tensor<int32_t>`对象`a`、`b`和`shift`。
  4. 直接使用C++逻辑和移位运算符完成计算，并将每组结果写入输出Tensor的独立行中。
  5. 通过`CopyUB2GM`将8行结果写回GM，并保存为`output/output.bin`。
  6. 使用NumPy生成的`output/golden.bin`逐元素精确校验结果。

  样例覆盖的运算符和操作数形式如下：

  | 输出行 | 样例调用 | 操作数形式 | 计算说明 |
  | :----- | :------- | :--------- | :------- |
  | 0 | `!a` | 单寄存器 | 逻辑非：输入为0时结果为1，否则结果为0 |
  | 1 | `a & b` | 寄存器与寄存器 | 对两个输入逐lane执行按位与 |
  | 2 | `a \| 16` | 寄存器与右标量 | 将标量16广播后逐lane执行按位或 |
  | 3 | `7 ^ a` | 左标量与寄存器 | 将标量7广播后逐lane执行按位异或 |
  | 4 | `a << shift` | 寄存器移位量 | 每个lane使用对应的`shift`元素左移 |
  | 5 | `a << 2` | 标量移位量 | 所有lane使用`int16_t`标量2左移 |
  | 6 | `a >> shift` | 寄存器移位量 | 每个lane使用对应的`shift`元素右移 |
  | 7 | `a >> 2` | 标量移位量 | 所有lane使用`int16_t`标量2右移 |

  本样例中的`operator!`表示逻辑非，不是按位取反。移位量由`0`到`3`循环生成，处于`int32_t`移位的有效范围内。

- 样例规格：

  <table border="2">
  <caption>表1：样例输入输出规格</caption>
  <tr><td align="left">样例类型（OpType）</td><td colspan="4" align="left">Logical Compute</td></tr>
  <tr><td rowspan="4" align="left">样例输入</td><td align="left">name</td><td align="left">shape</td><td align="left">data type</td><td align="left">format</td></tr>
  <tr><td align="left">src_a</td><td align="left">[64]</td><td align="left">int32_t</td><td align="left">ND</td></tr>
  <tr><td align="left">src_b</td><td align="left">[64]</td><td align="left">int32_t</td><td align="left">ND</td></tr>
  <tr><td align="left">shifts</td><td align="left">[64]</td><td align="left">int32_t</td><td align="left">ND</td></tr>
  <tr><td align="left">样例输出</td><td align="left">output</td><td align="left">[8, 64]</td><td align="left">int32_t</td><td align="left">ND</td></tr>
  <tr><td align="left">核函数名</td><td colspan="4" align="left">logical_compute_tensor_api</td></tr>
  </table>

  样例中的固定参数如下：

  | 参数 | 默认值 | 说明 |
  | :--- | :----- | :--- |
  | `VECTOR_LENGTH` | 64 | 每组输入和每行输出的元素数量 |
  | `RESULT_ROWS` | 8 | 输出结果行数，每行对应一种逻辑计算 |
  | 寄存器移位范围 | `[0, 3]` | `shifts`中的移位量循环范围 |
  | 标量移位量 | 2 | 标量左右移位测试使用的固定值 |
  | Kernel Block数 | 1 | 样例使用单个Vector Core执行 |
  | mask | 全有效 | 64个输入lane均参与计算 |

- 结果验证：

  `gen_data.py`构造包含负数、零和正数的`src_a`，并使用NumPy生成8行真值数据。`verify_result.py`要求输出文件的元素数量和所有`int32_t`结果与真值逐元素完全一致，不使用浮点容差。

  校验失败时，脚本会输出首个错误元素的线性索引、实际值和真值。本样例用于验证典型操作数组合和有效移位范围，不覆盖全部支持类型，也不测试无效或越界移位量。

## 编译运行

在本样例根目录下执行如下步骤，编译并运行样例。

- 配置环境变量

  请根据CANN开发套件的安装方式选择对应的环境变量配置命令。使用的CANN版本需要包含本样例依赖的experimental Tensor API接口。

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

- 样例执行

  ```bash
  cmake -S . -B build -DCMAKE_ASC_ARCHITECTURES=dav-3510
  cmake --build build -j
  cd build
  python3 ../scripts/gen_data.py
  ./demo
  python3 ../scripts/verify_result.py output/output.bin output/golden.bin
  ```

  NPU仿真模式编译示例如下：

  ```bash
  cmake -S . -B build_sim \
    -DCMAKE_ASC_RUN_MODE=sim \
    -DCMAKE_ASC_ARCHITECTURES=dav-3510
  cmake --build build_sim -j
  ```

  > **注意：** 切换编译模式时，建议使用新的构建目录；也可以清理原构建目录中的CMake缓存后重新配置。

- 编译选项说明

  | 选项 | 可选值 | 说明 |
  | :--- | :----- | :--- |
  | `CMAKE_ASC_RUN_MODE` | `npu`（默认）、`sim` | NPU运行模式或NPU仿真模式 |
  | `CMAKE_ASC_ARCHITECTURES` | `dav-3510`（默认） | `dav-3510`对应Ascend 950PR/Ascend 950DT |

- 执行结果

  执行成功且结果校验通过时，终端输出包含：

  ```text
  logical_compute_tensor_api executed successfully.
  test pass!
  ```
