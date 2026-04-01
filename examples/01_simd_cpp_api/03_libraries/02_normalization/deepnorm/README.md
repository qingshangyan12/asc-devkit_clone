# DeepNorm样例

## 概述

本样例在深层神经网络训练场景下，基于Kernel直调算子工程，介绍了调用Ascend C提供的DeepNorm高阶API实现DeepNorm归一化。该API在执行层LayerNorm归一化时，可以通过扩大残差连接（α系数）来提高Transformer深层网络的训练稳定性。

## 支持的产品
- Ascend 950PR/Ascend 950DT
- Atlas A3 训练系列产品/Atlas A3 推理系列产品
- Atlas A2 训练系列产品/Atlas A2 推理系列产品

## 目录结构介绍

```
├── deepnorm
│   ├── scripts
│   │   ├── gen_data.py         // 输入数据和真值数据生成脚本
│   ├── CMakeLists.txt          // 编译工程文件
│   ├── data_utils.h            // 数据读入写出函数
│   └── deepnorm.asc            // Ascend C样例实现 & 调用样例
```

## 样例描述

- 样例功能：  
  本样例实现了对shape大小为[B, S, H]的输入数据的DeepNorm归一化。 

- 样例规格：  
  <table>
  <caption>表1：DeepNorm样例规格表</caption>
  <tr><td rowspan="1" align="center">样例类型(OpType)</td><td colspan="4" align="center"> deepnorm </td></tr>

  <tr><td rowspan="6" align="center">样例输入</td></tr>
  <tr><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td align="center">inputX</td><td align="center">[4, 16, 64]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td align="center">inputGx</td><td align="center">[4, 16, 64]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td align="center">beta</td><td align="center">[1, 64]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td align="center">gamma</td><td align="center">[1, 64]</td><td align="center">float</td><td align="center">ND</td></tr>

  <tr><td rowspan="4" align="center">样例输出</td></tr>
  <tr><td align="center">output</td><td align="center">[4, 16, 64]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td align="center">outputMean</td><td align="center">[4, 16]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td align="center">outputVariance</td><td align="center">[4, 16]</td><td align="center">float</td><td align="center">ND</td></tr>

  <tr><td rowspan="1" align="center">核函数名</td><td colspan="4" align="center">deepnorm_custom</td></tr>
  </table>

- 样例实现：  
  本样例中实现的是固定shape(inputX[4, 16, 64]、inputGx[4, 16, 64]、beta[1, 64]、gamma[1, 64]， output[4, 16, 64]、 outputMean[4, 16]、 outputVariance[4, 16])的DeepNorm样例。

  - Kernel实现

    计算逻辑是：使用DeepNorm高阶API接口完成deepnorm计算得到最终结果，再搬出到外部存储上，所用API可详细参考[DeepNorm](../../../../../docs/api/context/DeepNorm.md)。

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
  mkdir -p build && cd build;   # 创建并进入build目录
  cmake -DNPU_ARCH=dav-2201 ..;make -j;             # 编译工程（默认npu模式）
  python3 ../scripts/gen_data.py   # 生成测试输入数据
  ./demo                        # 执行编译生成的可执行程序，执行样例
  ```

  使用 CPU调试 模式时，添加 `-DRUN_MODE=cpu` 参数即可。

  示例如下：
  ```bash
  cmake -DRUN_MODE=cpu -DNPU_ARCH=dav-2201 ..;make -j; # cpu调试模式
  ```

  > **注意：** 切换编译模式前需清理 cmake 缓存，可在 build 目录下执行 `rm CMakeCache.txt` 后重新 cmake。

- 编译选项说明

  | 选项 | 可选值 | 说明 |
  |------|--------|------|
  | `RUN_MODE` | `npu`（默认）、`cpu`、`sim` | 运行模式：NPU 运行、CPU调试、NPU仿真 |
  | `NPU_ARCH` | `dav-2201`（默认）、`dav-3510` | NPU 架构：dav-2201 对应 Atlas A2/A3 系列，dav-3510 对应 Ascend 950PR/Ascend 950DT |

  执行结果如下，说明精度对比成功。
  ```bash
  test pass!
  ```