# Mull Tensor API 样例

## 概述

本样例基于Tensor API实现`mull`接口，对两组各64个`uint32_t`元素执行乘法，并分别输出乘积的低32位和高32位。

## 支持的产品

- Ascend 950PR/Ascend 950DT

## 目录结构介绍

```plain
├── mull
│   ├── scripts
│   │   ├── gen_data.py          // 输入数据和真值数据生成脚本
│   │   └── verify_result.py     // 输出结果校验脚本
│   ├── CMakeLists.txt           // 编译工程文件
│   ├── data_utils.h             // 数据读写工具
│   ├── README.md
│   ├── README_en.md
│   └── mull.asc  // Ascend C算子实现与调用样例
```

## 样例描述

- 样例功能：

  ```text
  product_i = uint64(src0_i) * uint64(src1_i)
  low_i = product_i & 0xffffffff
  high_i = product_i >> 32
  ```

- 样例规格：

  <table>
    <tr><td align="center">样例类型(OpType)</td><td colspan="3" align="center">AIV样例</td></tr>
    <tr><td rowspan="3" align="center">样例输入</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td></tr>
    <tr><td align="center">src0</td><td align="center">[64]</td><td align="center">uint32_t</td></tr>
    <tr><td align="center">src1</td><td align="center">[64]</td><td align="center">uint32_t</td></tr>
    <tr><td rowspan="2" align="center">样例输出</td><td align="center">low</td><td align="center">[64]</td><td align="center">uint32_t</td></tr>
    <tr><td align="center">high</td><td align="center">[64]</td><td align="center">uint32_t</td></tr>
    <tr><td align="center">核函数名</td><td colspan="3" align="center">mull</td></tr>
  </table>

- 样例实现：

  1. 为GM和UB数据搬运创建ND Tensor视图，并为UB寄存器计算创建一维Tensor视图。
  2. 将`src0`和`src1`从GM搬运到UB，使用一维坐标`make_coord(offset)`加载到寄存器。
  3. 使用64元素mask调用`mull`，函数返回乘积低32位，并通过`high`输出乘积高32位。
  4. 将两个结果写回UB，再搬运到GM。
  5. Host侧保存输出文件，校验脚本分别比较低32位和高32位结果与真值。

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
  | --- | --- | --- |
  | `CMAKE_ASC_RUN_MODE` | `npu`（默认）、`sim` | 运行模式：NPU运行、NPU仿真 |
  | `CMAKE_ASC_ARCHITECTURES` | `dav-3510` | NPU架构：`dav-3510`对应Ascend 950PR/Ascend 950DT |
  | `CANN_ASC_USE_EXPERIMENTAL` | `ON`（本样例必选）、`OFF`（默认） | 开启实验性ASC接口 |

- 执行结果

  执行结果如下，说明精度对比成功。

  ```text
  mull executed successfully.
  test pass! verified low and high uint32 mull results.
  ```
