# 基于Tensor API实现的直方图计算样例

## 概述

本样例基于Tensor API编程方式实现寄存器直方图计算。样例将256个`uint8_t`输入元素从Global Memory（GM）搬运到Unified Buffer（UB），加载为带mask的`reg_tensor<uint8_t>`，分别执行低位区间和高位区间的频次统计与累计统计，并将4组`uint16_t`结果写回GM。

`uint8_t`输入的取值范围为0到255，一个`uint16_t`目的寄存器在一个256Byte的Vector Length内可保存128个统计结果，因此接口将完整取值范围拆分为`low`和`high`两个区间。目标寄存器既是统计结果的初值，也是本次统计的累加目标。

## 本样例支持的产品及CANN软件版本

| 产品 | CANN软件版本 |
| :--- | :----------- |
| Ascend 950PR/Ascend 950DT | CANN master（包含本样例依赖的experimental Tensor API接口） |

## 目录结构介绍

```text
histogram_compute_tensor_api
├── scripts
│   ├── gen_data.py                         // 输入数据和真值数据生成脚本
│   └── verify_result.py                    // 输出结果校验脚本
├── CMakeLists.txt                          // 编译工程文件
├── histogram_compute_tensor_api.asc        // Tensor API直方图计算实现及调用代码
├── README.md                               // 中文样例说明文档
└── README_En.md                            // 英文样例说明文档
```

样例同时复用上一级目录中的`reg_compute_example_utils.h`，用于完成ACL运行时初始化、Device内存申请、输入搬运和输出保存。

## 样例描述

- 样例功能：

  本样例使用同一组输入执行`low/high`与`frequency/cumulative`的4种模板参数组合，整体处理流程如下：

  1. 在Host侧生成256个`uint8_t`输入元素，并将`input.bin`复制到Device。
  2. 在Kernel中构造GM Tensor和UB Tensor，通过`CopyGM2UB`完成输入搬运。
  3. 使用全有效mask加载输入，得到`reg_tensor<uint8_t>`对象`src`。
  4. 创建`reg_tensor<uint16_t>`目的寄存器，并将每个lane初始化为3，用于验证结果在原始目标数据上累加的语义。
  5. 分别调用4种配置的`histograms`接口，将结果写入输出Tensor的独立行中。
  6. 通过`CopyUB2GM`将4行结果写回GM，并使用NumPy真值逐元素精确校验。

  直方图接口使用`histogram_options`在编译期选择统计区间和统计模式：

  | 输出行 | 接口配置 | 统计结果说明 |
  | :----- | :------- | :----------- |
  | 0 | 默认配置：`low + frequency` | 第`i`个元素为`3 + count(src == i)`，`i`范围为0到127 |
  | 1 | `high + frequency` | 第`i`个元素为`3 + count(src == 128 + i)`，`i`范围为0到127 |
  | 2 | `low + cumulative` | 第`i`个元素为`3 + count(src <= i)`，`i`范围为0到127 |
  | 3 | `high + cumulative` | 第`i`个元素为`3 + count(src <= 128 + i)`，`i`范围为0到127 |

  默认模板参数为`histogram_half::low`和`histogram_mode::frequency`，因此第0行可以直接调用`histograms(initial, src)`。其他三行通过具名的`constexpr histogram_options`传入不同配置，编译期会选择对应的底层直方图计算接口。

- 类型与区间说明：

  | 项目 | 说明 |
  | :--- | :--- |
  | 源操作数 | `reg_tensor<uint8_t>`，输入取值范围为0到255 |
  | 目的操作数 | `reg_tensor<uint16_t>`，一个寄存器包含128个统计lane |
  | `low`区间 | 统计输入值0到127 |
  | `high`区间 | 统计输入值128到255 |
  | `frequency`模式 | 分别统计每个输入值出现的次数 |
  | `cumulative`模式 | 统计不大于当前取值的输入元素数量 |
  | 初值语义 | 统计结果累加到调用者传入的目的寄存器初值上 |

  `uint16_t`统计结果最大值为65535，使用接口时需要由调用者保证累加结果不会溢出。本样例输入长度为256，且初值为3，不会触发溢出。

- 样例规格：

  <table border="2">
  <caption>表1：样例输入输出规格</caption>
  <tr><td align="left">样例类型（OpType）</td><td colspan="4" align="left">Histograms</td></tr>
  <tr><td rowspan="2" align="left">样例输入</td><td align="left">name</td><td align="left">shape</td><td align="left">data type</td><td align="left">format</td></tr>
  <tr><td align="left">input</td><td align="left">[256]</td><td align="left">uint8_t</td><td align="left">ND</td></tr>
  <tr><td align="left">样例输出</td><td align="left">output</td><td align="left">[4, 128]</td><td align="left">uint16_t</td><td align="left">ND</td></tr>
  <tr><td align="left">核函数名</td><td colspan="4" align="left">histogram_compute_tensor_api</td></tr>
  </table>

  样例中的固定参数如下：

  | 参数 | 默认值 | 说明 |
  | :--- | :----- | :--- |
  | `INPUT_LENGTH` | 256 | 输入元素数量 |
  | `BIN_LENGTH` | 128 | 每个低位或高位区间的统计结果数量 |
  | `RESULT_ROWS` | 4 | 两个统计区间与两种统计模式的组合数量 |
  | `INITIAL_VALUE` | 3 | 目的寄存器每个lane的初始值 |
  | Kernel Block数 | 1 | 样例使用单个Vector Core执行 |
  | mask | 全有效 | 256个`uint8_t`输入lane均参与统计 |

- 结果验证：

  `gen_data.py`生成确定性的`uint8_t`输入，通过`numpy.bincount`和`numpy.cumsum`计算4行真值，并在每个结果元素上加上目的寄存器初值3。`verify_result.py`要求输出文件的元素数量和所有`uint16_t`结果与真值逐元素完全一致。

  校验失败时，脚本会输出首个错误元素的线性索引、实际值和真值。本样例验证全有效mask下的4种模式和非零目的初值，不覆盖部分mask及`uint16_t`累加溢出场景。

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
  histogram_compute_tensor_api executed successfully.
  test pass!
  ```
