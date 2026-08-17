# 基于Tensor API实现的基础计算样例

## 概述

本样例基于Tensor API编程方式实现寄存器基础计算。样例将两组`float`输入和一组PReLU斜率数据从Global Memory（GM）搬运到Unified Buffer（UB），通过Tensor的`load`接口加载为带mask的`reg_tensor<float>`，依次完成一元算术、数学函数、激活函数和二元算术计算，并通过Tensor的`store`接口将16组结果写回GM。

样例主要展示Tensor数据搬运接口与返回值风格寄存器计算接口的组合使用方式。有效lane信息保存在`reg_tensor`的mask中，并随计算结果继续传递。

## 本样例支持的产品及CANN软件版本

| 产品 | CANN软件版本 |
| :--- | :----------- |
| Ascend 950PR/Ascend 950DT | CANN master（包含本样例依赖的experimental Tensor API接口） |

## 目录结构介绍

```text
basic_arithmetic_tensor_api
├── scripts
│   ├── gen_data.py                         // 输入数据和真值数据生成脚本
│   └── verify_result.py                    // 输出结果校验脚本
├── CMakeLists.txt                          // 编译工程文件
├── basic_arithmetic_tensor_api.asc         // Tensor API基础计算实现及调用代码
├── README.md                               // 中文样例说明文档
└── README_En.md                            // 英文样例说明文档
```

样例同时复用上一级目录中的`reg_compute_example_utils.h`，用于完成ACL运行时初始化、Device内存申请、输入搬运和输出保存。

## 样例描述

- 样例功能：

  本样例对64个`float`元素执行16组逐元素基础计算，整体处理流程如下：

  1. 在Host侧读取`src_a.bin`、`src_b.bin`和`slope.bin`，并将输入数据复制到Device。
  2. 在Kernel中使用Tensor API构造GM Tensor和UB Tensor，通过`CopyGM2UB`将输入数据搬运至UB。
  3. 使用全有效mask加载`src_a`、`src_b`和`slope`，得到三个`reg_tensor<float>`对象。
  4. 调用基础计算接口，将每种接口的结果保存到输出Tensor的独立行中。
  5. 通过`CopyUB2GM`将16行结果写回GM，并在Host侧保存为`output/output.bin`。
  6. 使用NumPy生成的`output/golden.bin`逐元素校验计算结果。

  样例覆盖的接口和计算表达式如下：

  | 输出行 | 接口或运算符 | 样例中的计算表达式 |
  | :----- | :----------- | :----------------- |
  | 0 | `abs` | `abs(src_a)` |
  | 1 | `exp` | `exp(src_a)` |
  | 2 | `sqrt` | `sqrt(abs(src_a) + 0.5)` |
  | 3 | `log` | `log(abs(src_a) + 0.5)` |
  | 4 | `log2` | `log2(abs(src_a) + 0.5)` |
  | 5 | `log10` | `log10(abs(src_a) + 0.5)` |
  | 6 | 一元`operator-` | `-src_a` |
  | 7 | `relu` | `max(src_a, 0)` |
  | 8 | `prelu` | `src_a >= 0 ? src_a : src_a * slope` |
  | 9 | `leaky_relu` | `src_a >= 0 ? src_a : src_a * 0.1` |
  | 10 | `operator+` | `src_a + src_b` |
  | 11 | 二元`operator-` | `src_a - src_b` |
  | 12 | `operator*` | `src_a * src_b` |
  | 13 | `operator/` | `src_a / src_b` |
  | 14 | `max` | `max(src_a, src_b)` |
  | 15 | `min` | `min(src_a, src_b)` |

  `sqrt`、`log`、`log2`和`log10`使用`abs(src_a) + 0.5`作为输入，保证输入位于函数的有效数学定义域内；`src_b`始终为正数，避免除零。

- 样例规格：

  <table border="2">
  <caption>表1：样例输入输出规格</caption>
  <tr><td align="left">样例类型（OpType）</td><td colspan="4" align="left">Basic Arithmetic</td></tr>
  <tr><td rowspan="4" align="left">样例输入</td><td align="left">name</td><td align="left">shape</td><td align="left">data type</td><td align="left">format</td></tr>
  <tr><td align="left">src_a</td><td align="left">[64]</td><td align="left">float</td><td align="left">ND</td></tr>
  <tr><td align="left">src_b</td><td align="left">[64]</td><td align="left">float</td><td align="left">ND</td></tr>
  <tr><td align="left">slope</td><td align="left">[64]</td><td align="left">float</td><td align="left">ND</td></tr>
  <tr><td align="left">样例输出</td><td align="left">output</td><td align="left">[16, 64]</td><td align="left">float</td><td align="left">ND</td></tr>
  <tr><td align="left">核函数名</td><td colspan="4" align="left">basic_arithmetic_tensor_api</td></tr>
  </table>

  样例中的固定参数如下：

  | 参数 | 默认值 | 说明 |
  | :--- | :----- | :--- |
  | `VECTOR_LENGTH` | 64 | 每组输入和每行输出的元素数量 |
  | `RESULT_ROWS` | 16 | 输出结果行数，每行对应一种计算 |
  | Kernel Block数 | 1 | 样例使用单个Vector Core执行 |
  | mask | 全有效 | 64个输入lane均参与计算 |

- 精度验证：

  `gen_data.py`使用NumPy生成输入和真值数据。`verify_result.py`按照输出行逐元素比较：

  | 结果类型 | 相对误差`rtol` | 绝对误差`atol` |
  | :------- | :------------- | :------------- |
  | `exp`、`sqrt`、`log`、`log2`、`log10` | `5e-3` | `5e-3` |
  | 其他浮点结果 | `1e-4` | `1e-4` |

  校验失败时，脚本会输出首个错误结果所在的row、lane、实际值和真值，便于定位问题。本样例用于验证接口调用流程和典型输入的数值结果，不代表对全部支持类型和所有边界条件的完整覆盖。

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

  执行成功且精度校验通过时，终端输出包含：

  ```text
  basic_arithmetic_tensor_api executed successfully.
  test pass! max absolute error: ...
  ```
