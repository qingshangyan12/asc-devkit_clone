# SwiGLU样例

## 概述

本样例在LLaMA等大语言模型FFN层场景下，基于SwiGLU高阶API实现SwiGLU算子。该API采用Swish作为激活函数的GLU（Gated Linear Unit）变体，提供更好的表达能力，常用于Transformer架构中的前馈网络。计算公式为：dst = Swish(srcTensor0, beta) * srcTensor1，其中Swish(x, beta) = x * sigmoid(beta * x)。本样例使用float数据类型，输入Tensor元素个数为32，通过CopyIn-Compute-CopyOut三阶段流水线完成计算。

## 支持的产品

- Ascend 950PR/Ascend 950DT
- Atlas A3 训练系列产品/Atlas A3 推理系列产品
- Atlas A2 训练系列产品/Atlas A2 推理系列产品

## 目录结构介绍

```
├── swiglu
│   ├── scripts
│   │   ├── gen_data.py         // 输入数据和真值数据生成脚本
│   ├── CMakeLists.txt          // 编译工程文件
│   ├── data_utils.h            // 数据读入写出函数
│   └── swiglu.asc              // Ascend C算子实现 & 调用样例
```

## 样例描述

- 样例功能：
  SwiGLU是采用Swish作为激活函数的GLU变体。

  计算公式如下：
  $$dstTensor_i=(srcTensor0_i)\bigotimes Swish(srcTensor1_i)$$
  其中Swish激活函数的计算公式如下（β为常量）：
  $$Swish(x)=x/(1 + e^{(-\beta x)})$$

- 样例规格：
  <table border="2" align="center">
  <caption>表1：样例规格表</caption>
  <tr><td rowspan="1" align="center">样例类型(OpType)</td><td colspan="4" align="center"> swiglu </td></tr>

  <tr><td rowspan="4" align="center">样例输入</td></tr>
  <tr><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td align="center">src0</td><td align="center">[32]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td align="center">src1</td><td align="center">[32]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td rowspan="2" align="center">样例输出</td></tr>
  <tr><td align="center">dst</td><td align="center">[32]</td><td align="center">float</td><td align="center">ND</td></tr>

  <tr><td rowspan="1" align="center">核函数名</td><td colspan="4" align="center">swiglu_custom</td></tr>
  </table>

- 样例实现：
  本样例中实现的是固定shape为输入src0[32]、src1[32]，输出dst[32]的swiglu_custom样例。

  - Kernel实现

    核心计算步骤：将输入数据从Global Memory搬运到Local Tensor后，调用 `AscendC::SwiGLU(dstLocal, src0Local, src1Local, betaValue)` 完成SwiGLU计算，再将结果搬回Global Memory。

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
  cmake -DNPU_ARCH=dav-2201 ..;make -j; # 默认npu模式
  python3 ../scripts/gen_data.py
  ./demo
  ```

  使用 CPU调试 或 NPU仿真 模式时，添加 `-DRUN_MODE=cpu` 或 `-DRUN_MODE=sim` 参数即可。

  示例如下：
  ```bash
  cmake -DRUN_MODE=cpu -DNPU_ARCH=dav-2201 ..;make -j; # cpu调试模式
  cmake -DRUN_MODE=sim -DNPU_ARCH=dav-2201 ..;make -j; # NPU仿真模式
  ```

  > **注意：** 切换编译模式前需清理 cmake 缓存，可在 build 目录下执行 `rm CMakeCache.txt` 后重新 cmake。

- 编译选项说明

  | 选项 | 可选值 | 说明 |
  |------|--------|------|
  | `RUN_MODE` | `npu`（默认）、`cpu`、`sim` | 运行模式：NPU 运行、CPU调试、NPU仿真 |
  | `NPU_ARCH` | `dav-2201`（默认）、`dav-3510` | NPU 架构：dav-2201 对应 Atlas A2/A3 系列，dav-3510 对应 Ascend 950PR/Ascend 950DT |

- 执行结果

  执行结果如下，说明精度对比成功。
  ```bash
  test pass!
  ```