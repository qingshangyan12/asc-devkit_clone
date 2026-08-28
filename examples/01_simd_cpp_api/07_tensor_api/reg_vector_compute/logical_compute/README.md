# Shift Left Tensor API 样例

## 概述

本样例基于Tensor API实现`operator<<`接口，对64个`uint32_t`元素和对应的`int32_t`移位量执行逐元素左移。

## 支持的产品

- Ascend 950PR/Ascend 950DT

## 目录结构介绍

```plain
├── logical_compute
│   ├── scripts
│   │   ├── gen_data.py          // 输入数据和真值数据生成脚本
│   │   └── verify_result.py     // 输出结果校验脚本
│   ├── CMakeLists.txt           // 编译工程文件
│   ├── data_utils.h             // 数据读写工具
│   ├── README.md
│   ├── README_En.md
│   └── logical_compute_tensor_api.asc  // Ascend C算子实现与调用样例
```

## 样例描述

- 样例功能：
  对输入Tensor中的每个元素使用对应的移位量执行左移计算，计算公式如下：

  ```text
  output_i = input_i << shifts_i
  ```

- 样例规格：
  <table>
  <tr><td align="center">样例类型(OpType)</td><td colspan="3" align="center">AIV样例</td></tr>
  <tr><td rowspan="3" align="center">样例输入</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td></tr>
  <tr><td align="center">input</td><td align="center">[64]</td><td align="center">uint32_t</td></tr>
  <tr><td align="center">shifts</td><td align="center">[64]</td><td align="center">int32_t</td></tr>
  <tr><td align="center">样例输出</td><td align="center">output</td><td align="center">[64]</td><td align="center">uint32_t</td></tr>
  <tr><td align="center">核函数名</td><td colspan="3" align="center">logical_compute_tensor_api</td></tr>
  </table>

- 样例实现：

  1. 为GM和UB数据搬运创建ND Tensor视图，并为UB寄存器计算创建一维Tensor视图。
  2. 使用`asc::te::copy`将数据和移位量从GM搬运到UB。
  3. 使用一维坐标`make_coord(offset)`分别加载为`reg_tensor<uint32_t>`和`reg_tensor<int32_t>`。
  4. 导入`operator<<`并执行`src << shift`，将结果写回UB并搬运到GM。
  5. Host侧使用内核调用符`<<<>>>`调用核函数，并通过校验脚本比较输出与真值。

## 编译运行

在本样例根目录下执行如下步骤，编译并执行样例。

- 配置环境变量

  请根据当前环境上CANN开发套件包的[安装方式](https://gitcode.com/cann/asc-devkit/blob/master/docs/zh/quick_start.md)，配置环境变量。

  ```bash
  source ${install_path}/cann/set_env.sh
  ```

- 样例执行

  ```bash
  mkdir -p build && cd build
  cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DCANN_ASC_USE_EXPERIMENTAL=ON ..
  make -j
  python3 ../scripts/gen_data.py
  ./demo
  python3 ../scripts/verify_result.py
  ```

  使用NPU仿真模式时，在CMake命令中添加`-DCMAKE_ASC_RUN_MODE=sim`参数。

  > **注意：** 切换编译模式前需清理CMake缓存，可在`build`目录下删除`CMakeCache.txt`后重新执行CMake命令。

- 编译选项说明

  | 选项 | 可选值 | 说明 |
  |------|--------|------|
  | `CMAKE_ASC_RUN_MODE` | `npu`（默认）、`sim` | 运行模式：NPU运行、NPU仿真 |
  | `CMAKE_ASC_ARCHITECTURES` | `dav-3510` | NPU架构：`dav-3510`对应Ascend 950PR/Ascend 950DT |
  | `CANN_ASC_USE_EXPERIMENTAL` | `ON`（本样例必选）、`OFF`（默认） | 开启实验性ASC接口 |

- 执行结果

  执行成功时输出如下：

  ```text
  logical_compute_tensor_api executed successfully.
  test pass! verified uint32 register shift-left.
  ```
