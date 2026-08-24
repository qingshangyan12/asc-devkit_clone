# Tensor API直方图计算样例

## 概述

本样例基于Tensor API演示寄存器直方图计算接口`histograms`。样例显式选择
高半区间`cumulative`模式，读取256个`uint8_t`输入元素，输出128个`uint16_t`累计统计结果。

## 支持的产品

- Ascend 950PR/Ascend 950DT

## 目录结构介绍

```plain
├── histogram_compute
│   ├── scripts
│   │   ├── gen_data.py          // 输入数据和真值数据生成脚本
│   │   └── verify_result.py     // 输出结果校验脚本
│   ├── CMakeLists.txt           // 编译工程文件
│   ├── data_utils.h             // 数据读写工具
│   ├── README.md
│   ├── README_En.md
│   └── histogram_compute_tensor_api.asc  // Ascend C算子实现与调用样例
```

## 样例描述

- 样例功能：
  对高半区间`[128, 255]`执行累计统计。输出初始值为3，计算公式如下：

  ```text
  output_i = 3 + count(input_j <= 128 + i), 0 <= i < 128
  ```

- 样例规格：
  <table>
  <caption>表1：样例输入输出规格</caption>
  <tr><td rowspan="1" align="center">样例类型(OpType)</td><td colspan="4" align="center">histogram_compute_tensor_api</td></tr>
  <tr><td rowspan="2" align="center">样例输入</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td align="center">input</td><td align="center">[1, 256]</td><td align="center">uint8_t</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">样例输出</td><td align="center">output</td><td align="center">[1, 128]</td><td align="center">uint16_t</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">核函数名</td><td colspan="4" align="center">histogram_compute_tensor_api</td></tr>
  </table>

- 样例实现：

  - Kernel实现

    1. 使用`asc::te::make_tensor`、`asc::te::make_mem_ptr`和`asc::te::make_frame_layout`创建GM和UB上的Tensor对象。
    2. 使用`asc::te::make_copy`和`asc::te::copy`完成GM到UB的数据搬运。
    3. 使用Tensor `load`接口加载输入为`reg_tensor<uint8_t>`。
    4. 初始化`reg_tensor<uint16_t>`目的寄存器为3，并配置`histogram_half::high`和
       `histogram_mode::cumulative`。
    5. 调用`histograms<histogram_config>`执行累计统计并写回UB。
    6. 使用`asc::te::copy`将结果从UB搬运回GM。

  - 调用实现

    Host侧程序读取`scripts/gen_data.py`生成的输入文件，使用内核调用符`<<<>>>`调用核函数，执行完成后保存输出文件并通过`scripts/verify_result.py`进行逐元素校验。

## 核心接口说明

### histograms

```cpp
template <const histogram_options& options = default_histogram_options>
__simd_callee__ inline decltype(auto) histograms(
    const reg_tensor<uint16_t>& dst, const reg_tensor<uint8_t>& src);
```

该接口根据`options.half`和`options.mode`统计`uint8_t`输入寄存器。本样例使用具名`constexpr`
配置选择高半区累计模式，并返回`reg_tensor<uint16_t>`结果。

## 编译运行

在本样例根目录下执行如下步骤，编译并执行样例。

- 配置环境变量

  请根据当前环境上CANN开发套件包的安装路径配置环境变量。

  ```bash
  source ${install_path}/cann/set_env.sh
  ```

- 样例执行

  ```bash
  mkdir -p build && cd build
  cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..
  make -j
  python3 ../scripts/gen_data.py
  ./demo
  python3 ../scripts/verify_result.py
  ```

- 编译选项说明

  | 选项 | 可选值 | 说明 |
  |------|--------|------|
  | `CMAKE_ASC_RUN_MODE` | `npu`（默认）、`sim` | 运行模式：NPU运行、NPU仿真 |
  | `CMAKE_ASC_ARCHITECTURES` | `dav-3510` | NPU架构 |

- 执行结果

  执行成功时输出如下：

  ```text
  histogram_compute_tensor_api executed successfully.
  test pass! verified cumulative histogram for high half bins.
  ```
