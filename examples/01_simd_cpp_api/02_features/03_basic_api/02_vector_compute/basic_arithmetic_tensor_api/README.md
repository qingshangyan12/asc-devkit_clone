# 基于Tensor API实现的基础计算样例

## 概述

本样例基于Tensor API编程方式实现寄存器基础计算。样例将两组`float`输入、一组PReLU斜率和两组`uint32_t`输入从Global Memory（GM）搬运到Unified Buffer（UB），通过Tensor的`load`接口加载为带mask的`reg_tensor`。样例计算28组浮点结果，并额外计算10组无符号整数进位、借位和宽乘结果，最后通过Tensor的`store`接口写回GM。

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

  本样例分别对64个`float`元素和64个`uint32_t`元素执行38组逐元素基础计算。浮点部分覆盖一元函数、激活函数，以及二元接口的`reg-reg`、`reg-scalar`和`scalar-reg`形式；整数部分覆盖带进位输出的加减法、带进位输入的加减法和32位宽乘。整体处理流程如下：

  1. 在Host侧读取三组浮点输入和两组`uint32_t`输入，并将数据复制到Device。
  2. 在Kernel中使用Tensor API构造GM Tensor和UB Tensor，通过`CopyGM2UB`将输入数据搬运至UB。
  3. 使用全有效mask加载浮点和整数输入，得到对应的`reg_tensor<float>`和`reg_tensor<uint32_t>`对象。
  4. 调用基础计算接口，将每种接口调用形式的结果保存到输出Tensor的独立行中。
  5. 通过`CopyUB2GM`将28行浮点结果和10行整数结果写回GM，分别保存为`output/output.bin`和`output/uint32_output.bin`。
  6. 使用NumPy生成的`output/golden.bin`和`output/uint32_golden.bin`逐元素校验计算结果。

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
  | 10 | `operator+`（reg-reg） | `src_a + src_b` |
  | 11 | `operator+`（reg-scalar） | `src_a + 0.5` |
  | 12 | `operator+`（scalar-reg） | `0.5 + src_a` |
  | 13 | 二元`operator-`（reg-reg） | `src_a - src_b` |
  | 14 | 二元`operator-`（reg-scalar） | `src_a - 0.75` |
  | 15 | 二元`operator-`（scalar-reg） | `0.75 - src_a` |
  | 16 | `operator*`（reg-reg） | `src_a * src_b` |
  | 17 | `operator*`（reg-scalar） | `src_a * 1.25` |
  | 18 | `operator*`（scalar-reg） | `1.25 * src_a` |
  | 19 | `operator/`（reg-reg） | `src_a / src_b` |
  | 20 | `operator/`（reg-scalar） | `src_a / 2.0` |
  | 21 | `operator/`（scalar-reg） | `2.0 / (abs(src_a) + 0.5)` |
  | 22 | `max`（reg-reg） | `max(src_a, src_b)` |
  | 23 | `max`（reg-scalar） | `max(src_a, 0.25)` |
  | 24 | `max`（scalar-reg） | `max(0.25, src_a)` |
  | 25 | `min`（reg-reg） | `min(src_a, src_b)` |
  | 26 | `min`（reg-scalar） | `min(src_a, -0.25)` |
  | 27 | `min`（scalar-reg） | `min(-0.25, src_a)` |

  `sqrt`、`log`、`log2`和`log10`使用`abs(src_a) + 0.5`作为输入，保证输入位于函数的有效数学定义域内；`src_b`始终为正数，`scalar-reg`除法也使用该正数中间结果作为除数，从而避免除零。

  `uint32_t`输出行如下。进位/借位谓词在样例中转换为`0`或`1`，便于保存和精确校验。

  | 输出行 | 接口 | 输出内容 |
  | :----- | :--- | :------- |
  | 0 | `add` | `src_a + src_b`的低32位 |
  | 1 | `add` | 逐lane进位，发生进位为`1` |
  | 2 | `add_c` | `src_a + src_b + add_carry`的低32位 |
  | 3 | `add_c` | 新的逐lane进位 |
  | 4 | `sub` | `src_a - src_b`的低32位 |
  | 5 | `sub` | 逐lane进位，`1`表示未借位，`0`表示发生借位 |
  | 6 | `sub_c` | `src_a + ~src_b + sub_carry`的低32位 |
  | 7 | `sub_c` | 新的逐lane进位，`1`表示未借位 |
  | 8 | `mull` | 32位乘法结果的低32位 |
  | 9 | `mull` | 32位乘法结果的高32位 |

- 样例实现：

  `compute`函数负责28组浮点计算；`compute_uint32`负责10组进位、借位和宽乘计算。`predicate_to_uint32`将进位谓词转换为可保存的`uint32_t`值。Kernel统一负责GM与UB之间的数据搬运和流水同步，Host侧负责加载输入、启动Kernel并保存两类输出。

- 样例规格：

  <table border="2">
  <caption>表1：样例输入输出规格</caption>
  <tr><td align="left">样例类型（OpType）</td><td colspan="4" align="left">Basic Arithmetic</td></tr>
  <tr><td rowspan="6" align="left">样例输入</td><td align="left">name</td><td align="left">shape</td><td align="left">data type</td><td align="left">format</td></tr>
  <tr><td align="left">src_a</td><td align="left">[64]</td><td align="left">float</td><td align="left">ND</td></tr>
  <tr><td align="left">src_b</td><td align="left">[64]</td><td align="left">float</td><td align="left">ND</td></tr>
  <tr><td align="left">slope</td><td align="left">[64]</td><td align="left">float</td><td align="left">ND</td></tr>
  <tr><td align="left">uint32_src_a</td><td align="left">[64]</td><td align="left">uint32_t</td><td align="left">ND</td></tr>
  <tr><td align="left">uint32_src_b</td><td align="left">[64]</td><td align="left">uint32_t</td><td align="left">ND</td></tr>
  <tr><td rowspan="2" align="left">样例输出</td><td align="left">float_output</td><td align="left">[28, 64]</td><td align="left">float</td><td align="left">ND</td></tr>
  <tr><td align="left">uint32_output</td><td align="left">[10, 64]</td><td align="left">uint32_t</td><td align="left">ND</td></tr>
  <tr><td align="left">核函数名</td><td colspan="4" align="left">basic_arithmetic_tensor_api</td></tr>
  </table>

  样例中的固定参数如下：

  | 参数 | 默认值 | 说明 |
  | :--- | :----- | :--- |
  | `VECTOR_LENGTH` | 64 | 每组输入和每行输出的元素数量 |
  | `FLOAT_RESULT_ROWS` | 28 | 浮点输出结果行数 |
  | `UINT32_RESULT_ROWS` | 10 | 整数输出结果行数 |
  | Kernel Block数 | 1 | 样例使用单个Vector Core执行 |
  | mask | 全有效 | 64个输入lane均参与计算 |

- 精度验证：

  `gen_data.py`使用NumPy生成输入和真值数据。`verify_result.py`按照输出行逐元素比较：

  | 结果类型 | 相对误差`rtol` | 绝对误差`atol` |
  | :------- | :------------- | :------------- |
  | `exp`、`sqrt`、`log`、`log2`、`log10` | `5e-3` | `5e-3` |
  | 其他浮点结果 | `1e-4` | `1e-4` |
  | `uint32_t`计算、进位和借位 | 精确比较 | 精确比较 |

  校验脚本还会检查四个结果文件的元素数量。校验失败时，脚本会输出接口表达式、row、lane、实际值和真值，便于定位问题。整数输入按奇偶lane分别构造溢出/不溢出和借位/不借位场景，使两种谓词结果都能得到验证。

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
  python3 ../scripts/verify_result.py \
    output/output.bin output/golden.bin \
    output/uint32_output.bin output/uint32_golden.bin
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
  test pass! verified 38 result rows (2432 elements).
  max float absolute error: ...; uint32 results matched exactly.
  ```
