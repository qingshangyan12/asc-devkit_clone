# Softmax样例

## 概述

本样例在激活函数场景下，基于SoftMax高阶API对输入Tensor按行做SoftMax计算，并使用AdjustSoftMaxRes对计算结果做后处理。本样例使用float数据类型，输入Tensor shape为[32, 32]，通过CopyIn-Compute-CopyOut三阶段流水线完成计算。更多API详细信息请参考[SoftMax API文档](../../../../../docs/api/context/SoftMax.md)。

> **进阶接口提示：** 除本样例使用的 `SoftMax` 接口外，Ascend C 还提供了 `SimpleSoftMax` 接口，使用预计算的sum和max数据对输入Tensor做SoftMax计算，内部无reduce过程。将 `softmax.asc` 中 `AscendC::SoftMax` 替换为 `AscendC::SimpleSoftMax` 即可切换。

## 支持的产品

- Ascend 950PR/Ascend 950DT
- Atlas A3 训练系列产品/Atlas A3 推理系列产品
- Atlas A2 训练系列产品/Atlas A2 推理系列产品

## 目录结构介绍

```
├── softmax
│   ├── scripts
│   │   └── gen_data.py         // 输入数据和真值数据生成脚本
│   ├── CMakeLists.txt          // 编译工程文件
│   ├── data_utils.h            // 数据读入写出函数
│   ├── README.md               // 样例说明文档
│   └── softmax.asc             // Ascend C算子实现 & 调用样例
```

## 样例描述

- 样例功能：  
  本样例对输入Tensor按行做SoftMax计算，并使用AdjustSoftMaxRes对计算结果做后处理。当输入的max中存在指定的值（0xFF7FFFFF）时，调整输出中对应位置的数据为自定义的值（0.0）。

  将输入tensor[m0, m1, ...mt, n]（t大于等于0）的非尾轴长度相乘的结果看作m，则输入tensor的shape看作[m, n]。

- 样例规格：  
  <table>
  <tr><td rowspan="1" align="center">样例类型(OpType)</td><td colspan="4" align="center"> softmax </td></tr>

  <tr><td rowspan="3" align="center">样例输入</td></tr>
  <tr><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td align="center">x</td><td align="center">[32, 32]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td rowspan="3" align="center">样例输出</td></tr>
  <tr><td align="center">y</td><td align="center">[32, 32]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td align="center">max</td><td align="center">[32, 8]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td align="center">sum</td><td align="center">[32, 8]</td><td align="center">float</td><td align="center">ND</td></tr>

  <tr><td rowspan="1" align="center">核函数名</td><td colspan="4" align="center">softmax_custom</td></tr>
  </table>

- 样例实现：  
  本样例中实现的是固定shape为输入x[32, 32]，输出y[32, 32]，max[32, 8]，sum[32, 8]的softmax_custom样例。

  - Kernel实现

    核心计算步骤：将输入数据从Global Memory搬运到Local Tensor后，调用 `AscendC::SoftMax` 完成SoftMax计算，再调用 `AscendC::AdjustSoftMaxRes` 对结果做后处理，最后将结果搬回Global Memory。

  - Tiling实现

    按照行数进行分核，使用平均分配法，调用 `AscendC::SoftMaxTilingFunc` 获取API所需tiling参数。

  - 调用实现  
    使用内核调用符<<<>>>调用核函数。

## 编译运行

在本样例根目录下执行如下步骤，编译并执行样例。
- 配置环境变量
  请根据当前环境上CANN开发套件包的[安装方式](../../../../../docs/quick_start.md#prepare&install)，选择对应配置环境变量的命令。
  - 默认路径，root用户安装CANN软件包
    ```bash
    source /usr/local/Ascend/cann/set_env.sh
    ```

  - 默认路径，非root用户安装CANN软件包
    ```bash
    source $HOME/Ascend/cann/set_env.sh
    ```

  - 指定路径install_path，安装CANN软件包
    ```bash
    source ${install_path}/cann/set_env.sh
    ```

- 样例执行
  ```bash
  mkdir -p build && cd build;
  cmake -DCMAKE_ASC_ARCHITECTURES=dav-2201 ..;make -j; # 默认npu模式
  python3 ../scripts/gen_data.py
  ./demo
  ```

  使用 CPU调试 或 NPU仿真 模式时，添加 `-DCMAKE_ASC_RUN_MODE=cpu` 或 `-DCMAKE_ASC_RUN_MODE=sim` 参数即可。

  示例如下：
  ```bash
  cmake -DCMAKE_ASC_RUN_MODE=cpu -DCMAKE_ASC_ARCHITECTURES=dav-2201 ..;make -j; # cpu调试模式
  cmake -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-2201 ..;make -j; # NPU仿真模式
  ```

  > **注意：** 切换编译模式前需清理 cmake 缓存，可在 build 目录下执行 `rm CMakeCache.txt` 后重新 cmake。

- 编译选项说明

  | 选项 | 可选值 | 说明 |
  |------|--------|------|
  | `CMAKE_ASC_RUN_MODE` | `npu`（默认）、`cpu`、`sim` | 运行模式：NPU 运行、CPU调试、NPU仿真 |
  | `CMAKE_ASC_ARCHITECTURES` | `dav-2201`（默认）、`dav-3510` | NPU 架构：dav-2201 对应 Atlas A2/A3 系列，dav-3510 对应 Ascend 950PR/Ascend 950DT |

- 执行结果

  执行结果如下，说明精度对比成功。
  ```bash
  test pass!
  ```
