# 基于Tensor API实现的复合计算样例

## 概述

本样例基于Tensor API编程方式实现寄存器复合计算。样例将两组`float`输入从Global Memory（GM）搬运到Unified Buffer（UB），加载为带mask的`reg_tensor<float>`，分别调用`axpy`、`absdiff`、`exp_sub`、`fma`和`muls_cast`接口，并将5组计算结果写回GM。

复合计算接口用于直接表达由乘、加、减、绝对值、指数或类型转换组成的常用计算形式。用户无需在调用侧拆分运算并维护中间寄存器，接口实现负责复用对应的底层计算能力。

## 支持的产品

- Ascend 950PR/Ascend 950DT

## 目录结构介绍

```text
composite_compute_tensor_api
├── scripts
│   ├── gen_data.py                         // 输入数据和真值数据生成脚本
│   └── verify_result.py                    // 输出结果校验脚本
├── CMakeLists.txt                          // 编译工程文件
├── composite_compute_tensor_api.asc        // Tensor API复合计算实现及调用代码
├── README.md                               // 中文样例说明文档
└── README_En.md                            // 英文样例说明文档
```

样例同时复用上一级目录中的`reg_compute_example_utils.h`，用于完成ACL运行时初始化、Device内存申请、输入搬运和输出保存。

## 样例描述

- 样例功能：

  本样例对两组各64个`float`元素执行5种复合计算，整体处理流程如下：

  1. 在Host侧生成并读取`src_a.bin`和`src_b.bin`，将输入复制到Device。
  2. 在Kernel中构造GM Tensor和UB Tensor，通过`copy_gm_to_ub`完成输入搬运。
  3. 使用全有效mask加载两组输入，得到`reg_tensor<float>`对象`a`和`b`。
  4. 调用复合计算接口，并将每种接口的结果写入输出Tensor的独立行中。
  5. 通过`copy_ub_to_gm`将结果写回GM，并保存为`output/output.bin`。
  6. 使用NumPy生成的`output/golden.bin`逐元素校验计算结果。

  样例覆盖的接口和计算表达式如下：

  | 输出行 | 接口 | 样例调用 | 数学表达式 | 接口原始输出类型 |
  | :----- | :--- | :------- | :--------- | :--------------- |
  | 0 | `axpy` | `axpy(b, a, 0.25f)` | `b + 0.25 * a` | `reg_tensor<float>` |
  | 1 | `absdiff` | `absdiff(a, b)` | `abs(a - b)` | `reg_tensor<float>` |
  | 2 | `exp_sub` | `exp_sub(a, b)` | `exp(a - b)` | `reg_tensor<float>` |
  | 3 | `fma` | `fma(a, b, b)` | `a * b + b` | `reg_tensor<float>` |
  | 4 | `muls_cast` | `muls_cast(a, 1.5f)` | `half(a * 1.5)` | `reg_tensor<half>` |

  `muls_cast`的接口输出为`reg_tensor<half>`。为了让5组样例结果使用同一个`float`输出Tensor保存，第4行在样例内部额外调用`asc_half2float`将`half`结果转换回`float`。该转换仅用于统一样例输出格式，不改变`muls_cast`接口本身的返回类型和计算语义。

- 样例规格：

  <table border="2">
  <caption>表1：样例输入输出规格</caption>
  <tr><td align="left">样例类型（OpType）</td><td colspan="4" align="left">Composite Compute</td></tr>
  <tr><td rowspan="3" align="left">样例输入</td><td align="left">name</td><td align="left">shape</td><td align="left">data type</td><td align="left">format</td></tr>
  <tr><td align="left">src_a</td><td align="left">[64]</td><td align="left">float</td><td align="left">ND</td></tr>
  <tr><td align="left">src_b</td><td align="left">[64]</td><td align="left">float</td><td align="left">ND</td></tr>
  <tr><td align="left">样例输出</td><td align="left">output</td><td align="left">[5, 64]</td><td align="left">float</td><td align="left">ND</td></tr>
  <tr><td align="left">核函数名</td><td colspan="4" align="left">composite_compute_tensor_api</td></tr>
  </table>

  样例中的固定参数如下：

  | 参数 | 默认值 | 说明 |
  | :--- | :----- | :--- |
  | `VECTOR_LENGTH` | 64 | 每组输入和每行输出的元素数量 |
  | `RESULT_ROWS` | 5 | 输出结果行数，每行对应一种复合计算 |
  | `axpy`系数 | `0.25f` | `a`参与融合乘加时使用的标量系数 |
  | `muls_cast`系数 | `1.5f` | 类型转换前的标量乘系数 |
  | Kernel Block数 | 1 | 样例使用单个Vector Core执行 |
  | mask | 全有效 | 64个输入lane均参与计算 |

- 精度验证：

  `gen_data.py`使用NumPy生成输入和真值数据。`muls_cast`真值会先转换为`float16`，再转换为`float32`，以模拟接口的窄化结果。`verify_result.py`采用以下组合容差：

  | 结果类型 | 相对误差`rtol` | 绝对误差`atol` |
  | :------- | :------------- | :------------- |
  | `exp_sub` | `5e-3` | `5e-3` |
  | `axpy`、`absdiff`、`fma`、`muls_cast` | `1e-4` | `1e-4` |

  校验失败时，脚本会输出首个错误结果所在的row、lane、实际值和真值。本样例使用`float`输入展示典型调用流程，不代表对接口全部支持类型和所有参数组合的完整覆盖。

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
  composite_compute_tensor_api executed successfully.
  test pass! max absolute error: ...
  ```
